#!/usr/bin/env python3
"""
Optimized version of MisTagParametrization_3D.py (b_tag_combined mode, 2022/2023 full datasets).

Key speedups vs. original:
  1. ROOT RDataFrame with EnableImplicitMT replaces TChain + tree.Draw().
     All histograms for all 3 options are booked lazily and evaluated in a
     single multi-threaded event loop over the data.
  2. Run exclusion uses a C++ std::set (O(log n) per event) instead of a
     119-term AND-chain of != conditions evaluated by ROOT's string interpreter.
  3. The common base filter (run exclusion + deltaPhi) is applied once,
     not 30 times (3 options x 10 tree.Draw calls each).

Usage (same flags as original):
  python MisTagParametrization_3D_optimized.py -e 2022 -b -d 0.9 -i 0.97
  python MisTagParametrization_3D_optimized.py -e 2023 -b -d 0.93 -i 0.97

PV systematic: two modes (see PV_SPLIT below)
  default     conservative — PV cut on the CR only; the two PV variations are
              applied to the PV-inclusive VR/SR and enveloped by
              OutputToLatex_VRclosureCheck.py.  Output in the current directory.
  --pv_split  tighter — PV cut on every region, so PV<42 and PV>=42 are summed
              into a PV-binned prediction and the systematic is
              |PV-binned - inclusive|.  Output under PVsplit/, and the txt
              carries a "PV treatment: split" marker plus a PV-binned summary.
              Read it back with:
                python3 OutputToLatex_VRclosureCheck.py --config my_scan.txt -b \\
                    --input_dir PVsplit
"""

import ROOT
import numpy as np
import os
import math
import argparse
import array
import sys
from contextlib import redirect_stdout

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPalette(ROOT.kCandy)
from ROOT import SetOwnership

debug = False

# ---- Histogram bin definitions ----
# pT_bins  = np.array([0, 40, 50, 60, 70, 80, 100, 120, 160, 240, 400], dtype=float) # used before reducing number of bins
# pT_bins  = np.array([0, 40, 160, 400], dtype=float)   # superseded — see below
#
# The top bin is the pT OVERFLOW bin: jet pT is clamped to just inside it when
# filling (jet{0,1}_PtAxis, built in _build_rdf_base), so jets above the top edge
# are counted there instead of disappearing.  This matters because Integral() and
# get_total_and_error() both ignore ROOT's own overflow bin — before this was
# added, every jet with pT > 400 was silently dropped from the CR/VR/SR yields and
# from the mistag rate (16 observed VR mistags and 95 SR events at WP1).
#
# NOTE: the clamp is deliberately ONE-SIDED (high side only).  jet{0,1}_Pt use
# -9999.9 as the "no such jet" sentinel, so clamping the low side would sweep
# non-existent jets into the first bin and count them as real low-pT jets.  The
# low edge is instead placed below anything the selection can produce, and
# check_axis_ranges() below verifies that nothing ever lands in ROOT's
# under/overflow, so a future change to the selection cannot silently lose events.
#
# Edge choice (binning study over both WP1 era groups, 2022_23_preBPix + 2023_postBPix):
#   * start at 40, not 0.  The histogrammed quantity is the DEPTH-tag jet's pT,
#     and DepthTagCand currently requires pT > 60 on both legs (checked over all
#     files: zero entries below 60 out of 14.8M / 2.36M).  But the INCLUSIVE-tag
#     jet does go down to 40 GeV (min jet1_Pt = 40.0013 for jet1_InclTagCand), so
#     40 is the lowest pT the candidate definitions can produce.  Starting the
#     axis there costs nothing today — [40,60) is simply empty — and keeps the
#     estimate correct if the depth-tag definition is ever loosened to match;
#   * split the old, very wide [40,160) bin: it held ~94% of the CR denominator
#     and ~71% of the VR mistags while the rate varied ~1.75x across it, and the
#     VR pT spectrum inside it is harder than the CR's, so a single bin-averaged
#     rate under-predicts;
#   * close the overflow bin at 250 rather than 400 — with a 400 edge the
#     [400,inf) x |eta|>1 cell has CR_mistag == 0 in both era groups, and a cell
#     with an empty numerator predicts exactly 0, biasing the estimate low.
pT_bins  = np.array([40, 100, 160, 400]) #np.array([40, 100, 400], dtype=float) # np.array([40, 100, 160, 250, 1000], dtype=float)
# Fill value for jets above the top edge — just inside the last bin, so they land
# in the overflow bin rather than in ROOT's (uncounted) overflow bin.
PT_FILL_CLAMP = pT_bins[-1] - 1e-3
# Set to True to book the eta axis (and jet0/1_EtaAxis columns) using |eta|
# instead of signed eta. Flip back to False to restore signed-eta behavior.
USE_ABS_ETA = True
if USE_ABS_ETA:
    # eta_bins = np.linspace(0, 1.26, 2)     # |eta|: to test impact on prediction closure
    eta_bins = np.array([0, 1.26], dtype=float)  # np.array([0, 1, 1.26], dtype=float)     # |eta|: to test impact on prediction closure
else:
    # eta_bins = np.linspace(-1.26, 1.26, 9) # used before reducing number of bins
    # eta_bins = np.linspace(-1.26, 1.26, 5) # signed eta
    eta_bins = np.array([-1.26, -1, 1, 1.26], dtype=float)  
# phi_bins = np.linspace(-np.pi, np.pi, 9)
phi_bins = np.linspace(-np.pi, np.pi, 2) # one bin in phi to test impact on prediction closure
b_tag_bins = np.array([0, 0.2435, 1.0], dtype=float)
c_tag_bins = np.array([0, 0.102,  1.0], dtype=float)

DNN_cut_inc = 0.97

CR_cut_inc = 0.2

# ---- Run exclusion lists (identical to original) ----
# v5.3 minituples
runs_to_exclude_2022 = [362615, 362653, 360485]   # based on CR 0.1 
runs_to_exclude_2022 = [362615, 362617, 362653, 362655]   # based on CR 0.2
# v5.5 minituples 
runs_to_exclude_2022 = [357698, 362653, 362655, 359661, 360991] # based on CR 0.2
runs_to_exclude_2022 = [357698, 362653, 359661, 360991] # based on CR 0.2, above 0.1%
runs_to_exclude_2022 = [357698, 359661, 360991] # based on CR 0.1, above 0.1%
runs_to_exclude_2022 = []
# v5.6 minituples
# runs_to_exclude_2022 = [361106] # based on CR 0.2, above 0.5% with depth > 0.9

    # 357776, 359661, 359690, 359869, 360890, 361044, 362087, 362437,
    # 362615, 362616, 362653, 360128, 360887, 360949, 361053, 361994,
    # 362063, 362698,
# runs_to_exclude_2022.append(362596)
# runs_to_exclude_2022.extend(range(362597, 362697))  # 100 runs from L1 LUT issue
runs_to_exclude_2022 = list(set(runs_to_exclude_2022))  # deduplicate

# v5.3 minituples
runs_to_exclude_2023 = [] # based on CR 0.1 # list(set([367228, 368684, 370460, 368684, 370460]))
runs_to_exclude_2023 = [367691, 368684] # based on CR 0.2
# v5.5 minituples
runs_to_exclude_2023 = [369998, 370790, 368676, 370093] # based on CR 0.2
runs_to_exclude_2023 = [370790] # based on CR 0.2, above 0.1% 
runs_to_exclude_2023 = [370790, 368676, 370093] # based on CR 0.1 above 0.1%
runs_to_exclude_2023 = []
# v5.6 minituples
# runs_to_exclude_2023 = [370790] # based on CR 0.2 above 0.5% with depth > 0.9

Zmu     = False
LLPskim = True
CNN     = False

current_jet_type = "leading"  # updated per-iteration in MisTagParametrization

output_dir = "outPlots_3D"
if not os.path.exists(output_dir):
    os.makedirs(output_dir)
    print(f"Directory '{output_dir}' created.")

# ---- PV systematic treatment -------------------------------------------------
# Default (PV_SPLIT = False): the low/high PV options apply the PV cut to the CR
# only, so the mistag rate from one PV slice is applied to the PV-inclusive
# VR/SR.  OutputToLatex_VRclosureCheck.py then takes the envelope of the two
# shifts -- the conservative systematic, and the historical behaviour.
#
# --pv_split (PV_SPLIT = True): the PV cut is applied to every region, so each
# PV option is a self-contained prediction for that PV slice and the two slices
# can be summed into a PV-binned prediction that uses the actual PV mix of the
# VR/SR.  The systematic is then |PV-binned - inclusive|, which vanishes when
# the VR/SR PV mix matches the CR's instead of carrying the subsampling noise of
# the envelope.  Same option as in SidebandParameterization_BkgPred.py.
# Output then goes to OUT_BASE_PV_SPLIT/ so it cannot overwrite the default run
# (neither the ROOT nor the txt filenames encode the PV treatment).
PV_SPLIT          = False
OUT_BASE_PV_SPLIT = "PVsplit"
OUT_BASE          = ""            # "" keeps every path exactly as it was


def configure_pv_mode(pv_split):
    """Select the PV treatment and the matching output directories."""
    global PV_SPLIT, OUT_BASE, output_dir
    PV_SPLIT = pv_split
    if not pv_split:
        return                     # default paths already set above
    OUT_BASE   = OUT_BASE_PV_SPLIT
    output_dir = os.path.join(OUT_BASE, "outPlots_3D")
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        print(f"Directory '{output_dir}' created.")

# ---- Declare C++ sets for O(log n) per-event run exclusion lookup ----
_excl_2022_cpp = "{" + ", ".join(str(r) for r in sorted(runs_to_exclude_2022)) + "}"
_excl_2023_cpp = "{" + ", ".join(str(r) for r in sorted(runs_to_exclude_2023)) + "}"
_excl_combined = sorted(set(runs_to_exclude_2022) | set(runs_to_exclude_2023))
_excl_combined_cpp = "{" + ", ".join(str(r) for r in _excl_combined) + "}"
ROOT.gInterpreter.Declare(f"""
#include <set>
static const std::set<int> g_excl_runs_2022          = {_excl_2022_cpp};
static const std::set<int> g_excl_runs_2023          = {_excl_2023_cpp};
static const std::set<int> g_excl_runs_2022_2023     = {_excl_combined_cpp};
""")

# ==============================================================================
# Utility / plotting functions — identical to original
# ==============================================================================

def ProjectHistogram(hist, y_label=""):
    projections = {}
    if isinstance(hist, ROOT.TH3):
        projections["pT"]  = hist.Project3D("x")
        projections["eta"] = hist.Project3D("y")
        projections["phi"] = hist.Project3D("z")
    elif isinstance(hist, ROOT.THnF):
        projections["pT"]    = hist.Projection(0)
        projections["eta"]   = hist.Projection(1)
        projections["phi"]   = hist.Projection(2)
        projections["b tag"] = hist.Projection(3)
        projections["c tag"] = hist.Projection(4)
    else:
        raise TypeError("Unsupported histogram type: expected TH3 or THnF.")
    for proj in projections.values():
        proj.SetYTitle(y_label)
    return projections["pT"], projections["eta"], projections["phi"]


def DrawCanvasAndPlots(canvas_name, canvas_title, option, title, plots,
                       legend_labels, save_name, plot_titles, label, normalize=False):
    canvas = ROOT.TCanvas(f"{canvas_name}_{option}", f"{canvas_title} for {option}", 2400, 600)
    old_canvas = ROOT.gDirectory.Get(f"{canvas_name}_{option}")
    if old_canvas:
        old_canvas.Delete()
    canvas.Divide(3, 1)

    normalized_all = []
    for i, plot_group in enumerate(plots):
        canvas.cd(i + 1)
        if normalize:
            normalized_group = []
            for hist in plot_group:
                hist_copy = hist.Clone()
                integral  = hist_copy.Integral()
                if integral > 0:
                    hist_copy.Scale(1.0 / integral)
                normalized_group.append(hist_copy)
            normalized_all.append(normalized_group)
            MakePlot(normalized_group, legend_labels)
            normalized_group[0].SetTitle(plot_titles[i] + title)
        else:
            MakePlot(plot_group, legend_labels)
            plot_group[0].SetTitle(plot_titles[i] + title)

    canvas.Update()
    canvas.Draw()
    canvas.SaveAs(f"{output_dir}/{save_name}{label}_{era_name}.png")
    print("saved canvas as " + output_dir + "/" + save_name + label + "_" + era_name + ".png")
    canvas.Clear()


def MakePlot(hists, legends):
    colors = [ROOT.kBlue, ROOT.kBlue - 9, ROOT.kGreen + 3, ROOT.kGreen - 6]
    if len(hists) == 2:
        colors = [49, 29]
    for i, hist in enumerate(hists):
        if len(hists) == 2 and i == 1:
            hist.SetFillStyle(3004)
            hist.SetFillColor(colors[i])
        hist.SetLineColor(colors[i])
        hist.SetLineWidth(2)
    if len(hists) == 1:
        hists[0].Draw("HIST")
    else:
        hists[0].Draw("HIST E")
    for hist in hists[1:]:
        hist.Draw("SAME HIST E")
    max_with_errors = max(
        h.GetBinContent(b) + h.GetBinError(b)
        for h in hists
        for b in range(1, h.GetNbinsX() + 1)
    )
    hists[0].SetMaximum(max_with_errors * 1.1)
    hists[0].SetMinimum(0)
    legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    for i, hist in enumerate(hists):
        legend.AddEntry(hist, legends[i], "l")
    legend.Draw()
    SetOwnership(legend, 0)
    LabelCMS()


def MistagRate(mistag_hist, all_hist, plot_type, option, title, label, type_of_jet):
    mistag_rate = mistag_hist.Clone("mistag_rate_" + plot_type)
    ResetAxis(mistag_rate)
    proj_pT_mistag_rate, proj_eta_mistag_rate, proj_phi_mistag_rate = \
        ProjectHistogram(mistag_rate, "Mistag rate")
    total = all_hist.Clone("total_" + plot_type)
    proj_pT_total, proj_eta_total, proj_phi_total = \
        ProjectHistogram(total, "Number of events")
    proj_pT_mistag_rate.Divide(proj_pT_total)
    proj_eta_mistag_rate.Divide(proj_eta_total)
    proj_phi_mistag_rate.Divide(proj_phi_total)

    legend_labels = ["Mistag rate (" + plot_type + ")"]
    png_title     = "3d_hist_projection_" + plot_type + "_mistag_rate_" + type_of_jet
    DrawCanvasAndPlots(
        "c_" + plot_type,
        "Mistag rate plots in the " + plot_type + " with " + type_of_jet,
        option, title,
        [[proj_pT_mistag_rate], [proj_eta_mistag_rate], [proj_phi_mistag_rate]],
        legend_labels, png_title,
        ["Jet p_{T} Mistag Rate from " + plot_type + ", " + type_of_jet,
         "Jet #eta Mistag Rate from " + plot_type + ", " + type_of_jet,
         "Jet #phi Mistag Rate from " + plot_type + ", " + type_of_jet],
        label
    )
    proj_pT_mistag_rate.Clear()
    proj_eta_mistag_rate.Clear()
    proj_phi_mistag_rate.Clear()


def MakePlotWithRatio(hists, legends, type, png_label):
    if len(hists) != 2:
        print("This function requires exactly two histograms.")
        return
    c_ratio = ROOT.TCanvas("c_ratio", "Canvas with Ratio", 800, 800)
    hists[1].SetFillStyle(3004)
    hists[1].SetFillColor(29)
    ratio_plot = ROOT.TRatioPlot(hists[0], hists[1])
    ratio_plot.SetH1DrawOpt("HIST E")
    ratio_plot.SetH2DrawOpt("HIST E F")
    ratio_plot.Draw("HIST E F")
    ratio_plot.GetLowYaxis().SetNdivisions(7)
    ratio_plot.GetLowerRefYaxis().SetRangeUser(0.5, 2)
    ratio_plot.GetLowerRefYaxis().SetTitle("Obs. / Predicted")
    legend = ROOT.TLegend(0.6, 0.75, 0.89, 0.92)
    for i, hist in enumerate(hists):
        legend.AddEntry(hist, legends[i], "lef")
    legend.Draw()
    SetOwnership(legend, 0)
    LabelCMS(0.13, 0.9, 0.03)
    c_ratio.Update()
    c_ratio.Draw()
    c_ratio.SaveAs(output_dir + "/" + png_label + type + "_ratio_" + era_name + ".png")
    c_ratio.Clear()


def LabelCMS(xpos=0.13, ypos=0.85, text_size=0.036):
    cmsLabel      = "#scale[1]{#bf{CMS} }"
    cmsLabelExtra = "#scale[0.8]{#it{Private Work}}"
    if "2022_2023" in era:
        yearLumi = "#scale[0.85]{2022+2023 (13.6 TeV)}"
    elif "2022" in era:
        yearLumi = "#scale[0.85]{2022 (13.6 TeV)}"
    else:
        yearLumi = "#scale[0.85]{2023 (13.6 TeV)}"
    stamp_text = ROOT.TLatex()
    stamp_text.SetNDC()
    stamp_text.SetTextFont(42)
    stamp_text.SetTextSize(text_size)
    stamp_text.DrawLatex(xpos, ypos, cmsLabel)
    stamp_text.DrawLatex(xpos + 0.07, ypos, cmsLabelExtra)
    if ypos == 0.85:
        if "2022_2023" in era:
            stamp_text.DrawLatex(xpos + 0.55, ypos + 0.06, yearLumi)
        else: stamp_text.DrawLatex(xpos + 0.62, ypos + 0.06, yearLumi)
        _dnn_label = DNN_cut_SJDC if current_jet_type == "sub-leading" else DNN_cut_LJDC
        stamp_text.DrawLatex(xpos + 0.4,  ypos,        "#scale[0.65]{DNN score > " + str(_dnn_label) + "}")
        stamp_text.DrawLatex(xpos + 0.4,  ypos - 0.04, "#scale[0.65]{Era = " + era + "}")
    else:
        _dnn_label = DNN_cut_SJDC if current_jet_type == "sub-leading" else DNN_cut_LJDC
        stamp_text.DrawLatex(xpos + 0.6,  ypos + 0.03, yearLumi)
        stamp_text.DrawLatex(xpos + 0.3,  ypos,        "#scale[0.65]{DNN score > " + str(_dnn_label) + "}")
        stamp_text.DrawLatex(xpos + 0.3,  ypos - 0.04, "#scale[0.65]{Era = " + era + "}")


def ResetAxis(hist):
    axis_labels = ["p_{T} (GeV)", "#eta", "#phi", "b tagging", "c tagging"]
    if isinstance(hist, ROOT.TH3):
        hist.GetXaxis().SetTitle(axis_labels[0])
        hist.GetYaxis().SetTitle(axis_labels[1])
        hist.GetZaxis().SetTitle(axis_labels[2])
    elif hasattr(hist, "GetAxis"):
        for i in range(hist.GetNdimensions()):
            hist.GetAxis(i).SetTitle(axis_labels[i])


def get_total_and_error(hist):
    total  = 0.0
    error2 = 0.0
    for i in range(1, hist.GetNbinsX() + 1):
        for j in range(1, hist.GetNbinsY() + 1):
            for k in range(1, hist.GetNbinsZ() + 1):
                content = hist.GetBinContent(i, j, k)
                error   = hist.GetBinError(i, j, k)
                total  += content
                error2 += error * error
    return total, math.sqrt(error2)


def check_axis_ranges(hist, name):
    """
    Warn loudly if any event landed in ROOT's under/overflow bins.

    Integral() and get_total_and_error() only sum bins 1..N, so anything in an
    under/overflow bin is silently missing from every number this script quotes.
    The pT axis is protected by the high-side fill clamp and by the pT > 60
    requirement inside DepthTagCand/InclTagCand; eta and phi are protected by the
    candidate definitions.  This is the tripwire that catches it if a change to
    the binning or the selection ever breaks one of those assumptions.
    """
    nx, ny, nz = hist.GetNbinsX(), hist.GetNbinsY(), hist.GetNbinsZ()
    axes = {
        "pT":  [(0, j, k)      for j in range(0, ny + 2) for k in range(0, nz + 2)] +
               [(nx + 1, j, k) for j in range(0, ny + 2) for k in range(0, nz + 2)],
        "eta": [(i, 0, k)      for i in range(0, nx + 2) for k in range(0, nz + 2)] +
               [(i, ny + 1, k) for i in range(0, nx + 2) for k in range(0, nz + 2)],
        "phi": [(i, j, 0)      for i in range(0, nx + 2) for j in range(0, ny + 2)] +
               [(i, j, nz + 1) for i in range(0, nx + 2) for j in range(0, ny + 2)],
    }
    bad = {}
    for axis, cells in axes.items():
        lost = sum(hist.GetBinContent(*c) for c in cells)
        if lost > 0:
            bad[axis] = lost
    if bad:
        detail = ", ".join(f"{axis}: {n:.0f}" for axis, n in bad.items())
        print(f"*** WARNING: histogram '{name}' has entries in ROOT's under/overflow "
              f"({detail}). Those events are NOT counted in any yield or rate below.")
    return bad


# ==============================================================================
# RDataFrame-based histogram booking and filling
# ==============================================================================

def _h3_model(name):
    """TH3DModel with the standard pT / eta / phi variable binning."""
    return ROOT.RDF.TH3DModel(
        name, "3D histogram; p_{T}; #eta; #phi",
        len(pT_bins)  - 1, array.array('d', pT_bins),
        len(eta_bins) - 1, array.array('d', eta_bins),
        len(phi_bins) - 1, array.array('d', phi_bins),
    )


def book_all_histograms(rdf_base, is_mc=False):
    """
    Book histograms for the three b_tag_combined options in one shot.

    All Histo3D calls are lazy — no data is read here.  RDataFrame will
    evaluate the full computation graph in one multi-threaded pass when the
    first result is accessed.

    Returns
    -------
    booked : dict
        booked[option]["CR_all"] etc. are RResultPtr<TH3D> objects.
    """
    # WPlusJets (v4.1) uses "_updated" suffixes; data (v5.5) uses the original names
    if is_mc:
        inc_suf   = "inc_train80_updated"
        depth_suf = "depth_LLPanywhere_updated"
    else:
        inc_suf   = "inc_train80"
        depth_suf = "depth_LLPanywhere"

    inc0   = f"jet0_scores_{inc_suf}"
    inc1   = f"jet1_scores_{inc_suf}"
    depth0 = f"jet0_scores_{depth_suf}"
    depth1 = f"jet1_scores_{depth_suf}"

    # LJDC: jet0 is the depth tag candidate; jet1 inclusive score defines CR/VR/SR
    CR_str     = f"{inc1} >= 0.0 && {inc1} < {CR_cut_inc}"
    VR_str     = f"{inc1} >= {CR_cut_inc} && {inc1} < {DNN_cut_inc_LJDC}"
    SR_str     = f"{inc1} >= {DNN_cut_inc_LJDC} && {inc1} < 1.1"
    mistag_str = f"{depth0} >= {DNN_cut_LJDC} && {depth0} < 1.1"

    # SJDC: jet1 is the depth tag candidate; jet0 inclusive score defines CR/VR/SR
    CR_0_str     = f"{inc0} >= 0.0 && {inc0} < {CR_cut_inc}"
    VR_0_str     = f"{inc0} >= {CR_cut_inc} && {inc0} < {DNN_cut_inc_SJDC}"
    SR_0_str     = f"{inc0} >= {DNN_cut_inc_SJDC} && {inc0} < 1.1"
    mistag_1_str = f"{depth1} >= {DNN_cut_SJDC} && {depth1} < 1.1"

    # Emulated trigger: leading jet has depth tag candidate + subleading has inclusive tag candidate
    # For MC (W+Jets), DepthTagCand/InclTagCand are almost never set; require 2 valid jets instead
    if is_mc:
        # j0 leg: jet0 is depth candidate, jet1 is inclusive candidate
        depth_j0_str = ("validJet >= 1"
                        " && jet0_Pt > 60 && abs(jet0_Eta) < 1.26"
                        " && jet1_Pt > 40 && abs(jet1_Eta) < 2")
        # j1 leg: jet1 is depth candidate, jet0 is inclusive candidate
        depth_j1_str = ("validJet >= 1"
                        " && jet1_Pt > 60 && abs(jet1_Eta) < 1.26"
                        " && jet0_Pt > 40 && abs(jet0_Eta) < 2")
    else:
        depth_j0_str = "jet0_DepthTagCand == 1 && jet1_InclTagCand == 1"
        depth_j1_str = "jet1_DepthTagCand == 1 && jet0_InclTagCand == 1"

    low_PV_str  = "PV >= 0  && PV < 42"
    high_PV_str = "PV >= 42 && PV < 100"

    options = ["depth", "depth, low PV", "depth, high PV"]
    booked  = {}

    for option in options:
        s = option.replace(", ", "_").replace(" ", "_")  # safe name fragment

        # Default: PV cut is applied only to the CR (mistag rate measurement
        # region), not to VR or SR — matches original MisTagParametrization logic
        # exactly.  --pv_split: the same PV cut is applied to the VR and SR too,
        # so each PV option is a complete prediction for that PV slice.
        cr_extra = ""
        if "low PV"  in option: cr_extra = " && " + low_PV_str
        if "high PV" in option: cr_extra = " && " + high_PV_str
        vr_sr_extra = cr_extra if PV_SPLIT else ""

        # --- jet0 triggered (jet0 has depth tag, jet1 defines CR/VR/SR) ---
        rdf_j0    = rdf_base.Filter(depth_j0_str,         f"depth_j0_{s}")
        rdf_CR_j0 = rdf_j0.Filter(CR_str + cr_extra,      f"CR_j0_{s}")
        rdf_VR_j0 = rdf_j0.Filter(VR_str + vr_sr_extra,   f"VR_j0_{s}")
        rdf_SR_j0 = rdf_j0.Filter(SR_str + vr_sr_extra,   f"SR_j0_{s}")

        booked[option] = {
            "CR_all":    rdf_CR_j0.Histo3D(_h3_model(f"h_CR_all_{s}"),    "jet0_PtAxis", "jet0_EtaAxis", "jet0_Phi"),
            "CR_mistag": rdf_CR_j0.Filter(mistag_str).Histo3D(
                             _h3_model(f"h_CR_mistag_{s}"),               "jet0_PtAxis", "jet0_EtaAxis", "jet0_Phi"),
            "VR_all":    rdf_VR_j0.Histo3D(_h3_model(f"h_VR_all_{s}"),    "jet0_PtAxis", "jet0_EtaAxis", "jet0_Phi"),
            "VR_mistag": rdf_VR_j0.Filter(mistag_str).Histo3D(
                             _h3_model(f"h_VR_mistag_{s}"),               "jet0_PtAxis", "jet0_EtaAxis", "jet0_Phi"),
            "SR_all":    rdf_SR_j0.Histo3D(_h3_model(f"h_SR_all_{s}"),    "jet0_PtAxis", "jet0_EtaAxis", "jet0_Phi"),
        }
        if is_mc:
            booked[option]["SR_mistag"] = rdf_SR_j0.Filter(mistag_str).Histo3D(
                             _h3_model(f"h_SR_mistag_{s}"),               "jet0_PtAxis", "jet0_EtaAxis", "jet0_Phi")

        # --- jet1 triggered (jet1 has depth tag, jet0 defines CR/VR/SR) ---
        rdf_j1    = rdf_base.Filter(depth_j1_str,        f"depth_j1_{s}")
        rdf_CR_j1 = rdf_j1.Filter(CR_0_str + cr_extra,     f"CR_j1_{s}")
        rdf_VR_j1 = rdf_j1.Filter(VR_0_str + vr_sr_extra,  f"VR_j1_{s}")
        rdf_SR_j1 = rdf_j1.Filter(SR_0_str + vr_sr_extra,  f"SR_j1_{s}")

        booked[option].update({
            "CR_all_1":    rdf_CR_j1.Histo3D(_h3_model(f"h_CR_all_1_{s}"),    "jet1_PtAxis", "jet1_EtaAxis", "jet1_Phi"),
            "CR_mistag_1": rdf_CR_j1.Filter(mistag_1_str).Histo3D(
                               _h3_model(f"h_CR_mistag_1_{s}"),               "jet1_PtAxis", "jet1_EtaAxis", "jet1_Phi"),
            "VR_all_1":    rdf_VR_j1.Histo3D(_h3_model(f"h_VR_all_1_{s}"),    "jet1_PtAxis", "jet1_EtaAxis", "jet1_Phi"),
            "VR_mistag_1": rdf_VR_j1.Filter(mistag_1_str).Histo3D(
                               _h3_model(f"h_VR_mistag_1_{s}"),               "jet1_PtAxis", "jet1_EtaAxis", "jet1_Phi"),
            "SR_all_1":    rdf_SR_j1.Histo3D(_h3_model(f"h_SR_all_1_{s}"),    "jet1_PtAxis", "jet1_EtaAxis", "jet1_Phi"),
        })
        if is_mc:
            booked[option]["SR_mistag_1"] = rdf_SR_j1.Filter(mistag_1_str).Histo3D(
                               _h3_model(f"h_SR_mistag_1_{s}"),               "jet1_PtAxis", "jet1_EtaAxis", "jet1_Phi")

    # ---- BLINDING TRIPWIRE -------------------------------------------------
    # For data, no histogram of the SR tagged cell (SR region AND the depth
    # mistag cut) may exist.  SR_all is inclusive in depth score and is allowed;
    # SR_mistag isolates the blinded cell and is booked for MC only.  If a future
    # edit books one for data, fail here rather than quietly unblinding.
    if not is_mc:
        for opt, d in booked.items():
            offenders = [k for k in d if k.startswith("SR_") and "mistag" in k]
            if offenders:
                raise RuntimeError(
                    "BLINDING VIOLATION: SR mistag histogram(s) booked for data "
                    f"in option '{opt}': {offenders}. The signal region (both DNN "
                    "cuts applied) must remain blinded.")

    return booked


def materialise(booked, option):
    """
    Convert RResultPtr objects to concrete TH3D histograms.

    The first call to GetPtr() triggers the RDataFrame event loop.  Because
    all options share the same computation graph rooted at rdf_base, RDataFrame
    computes ALL booked histograms (all options, both jet roles) in one pass.
    Subsequent calls return already-computed results instantly.
    """
    h = booked[option]

    def get(ptr, name):
        th = ptr.GetPtr()           # triggers computation on first access
        out = th.Clone(name)
        out.SetDirectory(0)
        if not out.GetSumw2N():
            out.Sumw2()             # activate proper error propagation
        check_axis_ranges(out, name)   # tripwire: nothing may sit in under/overflow
        return out

    key_to_name = {
        "CR_all":    "hist3d_CR_all",
        "CR_mistag": "hist3d_CR_mistag",
        "VR_all":    "hist3d_VR_all",
        "VR_mistag": "hist3d_VR_mistag",
        "SR_all":    "hist3d_SR_all",
        "SR_mistag": "hist3d_SR_mistag",
        "CR_all_1":    "hist3d_CR_all_1",
        "CR_mistag_1": "hist3d_CR_mistag_1",
        "VR_all_1":    "hist3d_VR_all_1",
        "VR_mistag_1": "hist3d_VR_mistag_1",
        "SR_all_1":    "hist3d_SR_all_1",
        "SR_mistag_1": "hist3d_SR_mistag_1",
    }
    return {k: get(v, key_to_name[k]) for k, v in h.items()}


# ==============================================================================
# Analysis logic — same as original MisTagParametrization(), but receives
# pre-computed histograms instead of filling them itself.
# ==============================================================================

_option_meta = {
    "depth":          (": 2+ depth",         "_depth"),
    "depth, low PV":  (": 2+ depth, low PV",  "_depth_lowPV"),
    "depth, high PV": (": 2+ depth, high PV", "_depth_highPV"),
}


def MisTagParametrization(hists, option):
    """
    Process pre-computed histograms for one option and produce all plots /
    ROOT output files.  Logic identical to the original function.

    Parameters
    ----------
    hists  : dict returned by materialise()
    option : str, one of the keys in _option_meta
    """
    title, label = _option_meta[option]

    CR_all    = hists["CR_all"]
    CR_mistag = hists["CR_mistag"]
    VR_all    = hists["VR_all"]
    VR_mistag = hists["VR_mistag"]
    SR_all    = hists["SR_all"]

    CR_all_1    = hists["CR_all_1"]
    CR_mistag_1 = hists["CR_mistag_1"]
    VR_all_1    = hists["VR_all_1"]
    VR_mistag_1 = hists["VR_mistag_1"]
    SR_all_1    = hists["SR_all_1"]

    # Combine leading + sub-leading jet triggered histograms
    CR_all_combined    = CR_all.Clone("hist3d_CR_all_combined");    CR_all_combined.Add(CR_all_1)
    CR_mistag_combined = CR_mistag.Clone("hist3d_CR_mistag_combined"); CR_mistag_combined.Add(CR_mistag_1)
    VR_all_combined    = VR_all.Clone("hist3d_VR_all_combined");    VR_all_combined.Add(VR_all_1)
    VR_mistag_combined = VR_mistag.Clone("hist3d_VR_mistag_combined"); VR_mistag_combined.Add(VR_mistag_1)
    SR_all_combined    = SR_all.Clone("hist3d_SR_all_combined");    SR_all_combined.Add(SR_all_1)

    if is_mc:
        SR_mistag   = hists["SR_mistag"]
        SR_mistag_1 = hists["SR_mistag_1"]
        SR_mistag_combined = SR_mistag.Clone("hist3d_SR_mistag_combined"); SR_mistag_combined.Add(SR_mistag_1)
        SR_mistag_list = [SR_mistag, SR_mistag_1, SR_mistag_combined]

    print("created histograms for 1D rate evaluation")
    print("completed 1D rate evaluation")

    CR_all_list    = [CR_all,    CR_all_1,    CR_all_combined]
    CR_mistag_list = [CR_mistag, CR_mistag_1, CR_mistag_combined]
    VR_all_list    = [VR_all,    VR_all_1,    VR_all_combined]
    VR_mistag_list = [VR_mistag, VR_mistag_1, VR_mistag_combined]
    SR_all_list    = [SR_all,    SR_all_1,    SR_all_combined]
    mistag_jet_list = ["leading", "sub-leading", "combined"]

    # Integrated yields per jet category, returned for the --pv_split summary.
    # SR_all is inclusive in depth score; the blinded SR mistag cell is not here.
    yields = {}

    _sr_mistag_iter = SR_mistag_list if is_mc else [None, None, None]
    for i, (CR_all_i, CR_mistag_i, VR_all_i, VR_mistag_i, SR_all_i, SR_mistag_i) in enumerate(
            zip(CR_all_list, CR_mistag_list, VR_all_list, VR_mistag_list, SR_all_list, _sr_mistag_iter)):

        global current_jet_type
        current_jet_type = mistag_jet_list[i]

        print(" ************* \n " + mistag_jet_list[i] + " \n *************")

        # Write base histograms to ROOT file using canonical names
        outfile_name = os.path.join(
            OUT_BASE,
            "output_3D_hists" + label + "_" + mistag_jet_list[i] + "_" + era_name + ".root")
        output_file  = ROOT.TFile(outfile_name, "RECREATE")
        output_file.cd()
        CR_all_i.Write("hist3d_CR_all")
        CR_mistag_i.Write("hist3d_CR_mistag")
        VR_all_i.Write("hist3d_VR_all")
        VR_mistag_i.Write("hist3d_VR_mistag")
        SR_all_i.Write("hist3d_SR_all")
        print("Created root file for output, wrote 3D histograms")
        output_file.Close()

        # Projection overlay: CR and VR all + mistag
        proj_pT_CR_all,    proj_eta_CR_all,    proj_phi_CR_all    = ProjectHistogram(CR_all_i,    "Number of events")
        proj_pT_CR_mistag, proj_eta_CR_mistag, proj_phi_CR_mistag = ProjectHistogram(CR_mistag_i, "Number of events")
        proj_pT_VR_all,    proj_eta_VR_all,    proj_phi_VR_all    = ProjectHistogram(VR_all_i,    "Number of events")
        proj_pT_VR_mistag, proj_eta_VR_mistag, proj_phi_VR_mistag = ProjectHistogram(VR_mistag_i, "Number of events")

        legend_labels = ["CR (no cuts)", "CR, mistag", "VR (no cuts)", "VR, mistag"]
        png_title     = "3d_hist_projection_overlay_CR_VR_" + mistag_jet_list[i]
        DrawCanvasAndPlots(
            "c1", "Projection plots", option, title,
            [[proj_pT_CR_all,  proj_pT_CR_mistag,  proj_pT_VR_all,  proj_pT_VR_mistag],
             [proj_eta_CR_all, proj_eta_CR_mistag, proj_eta_VR_all, proj_eta_VR_mistag],
             [proj_phi_CR_all, proj_phi_CR_mistag, proj_phi_VR_all, proj_phi_VR_mistag]],
            legend_labels, png_title,
            ["Jet p_{T} Projection with various cuts, " + mistag_jet_list[i],
             "Jet #eta Projection with various cuts, " + mistag_jet_list[i],
             "Jet #phi Projection with various cuts, " + mistag_jet_list[i]],
            label, normalize=True
        )

        # Mistag rate from CR; use it to predict VR and SR
        CR_mistag_rate    = CR_mistag_i.Clone("CR_mistag_rate")
        CR_mistag_rate.Divide(CR_all_i)
        VR_mistag_predict = VR_all_i.Clone("VR_mistag_predict")
        VR_mistag_predict.Multiply(CR_mistag_rate)

        ResetAxis(CR_mistag_rate)
        ResetAxis(VR_mistag_predict)

        proj_pT_VR_mistag_predict, proj_eta_VR_mistag_predict, proj_phi_VR_mistag_predict = \
            ProjectHistogram(VR_mistag_predict, "Number of events")

        # Raw region counts, captured before SR_all_i is turned into the
        # prediction in place below (used by the --pv_split coverage check).
        n_CR_all, n_CR_mistag = CR_all_i.Integral(), CR_mistag_i.Integral()
        n_VR_all, n_SR_all    = VR_all_i.Integral(), SR_all_i.Integral()

        print("\nNumber of events in CR = " + str(CR_all_i.Integral()))
        print("Number of events in VR = " + str(VR_all_i.Integral()))
        print("Number of events in SR = " + str(SR_all_i.Integral()) + "\n")

        SR_all_i.Multiply(CR_mistag_rate)
        print("Predicted number of mistagged events in SR = " + str(SR_all_i.Integral()) + "\n")
        proj_pT_SR_mistag_predict, proj_eta_SR_mistag_predict, proj_phi_SR_mistag_predict = \
            ProjectHistogram(SR_all_i, "Number of events")

        total_pred,   err_pred   = get_total_and_error(VR_mistag_predict)
        total_actual, err_actual = get_total_and_error(VR_mistag_i)
        total_SR,     err_SR     = get_total_and_error(SR_all_i)

        print(option + ", " + mistag_jet_list[i])
        print(f"Observed mistagged events in VR: {total_actual:.2f} \u00b1 {err_actual:.2f} (stat)")
        print(f"Predicted mistagged events in VR: {total_pred:.2f} \u00b1 {err_pred:.2f} (stat)")
        if is_mc:
            total_SR_actual, err_SR_actual = get_total_and_error(SR_mistag_i)
            print(f"Observed mistagged events in SR: {total_SR_actual:.2f} \u00b1 {err_SR_actual:.2f} (stat)")
        print(f"Predicted mistagged events in SR: {total_SR:.2f} \u00b1 {err_SR:.2f} (stat)")

        yields[mistag_jet_list[i]] = {
            "CR_all": n_CR_all, "CR_mistag": n_CR_mistag,
            "VR_all": n_VR_all, "SR_all": n_SR_all,
            "obs_VR":  (total_actual, err_actual),
            "pred_VR": (total_pred,   err_pred),
            "pred_SR": (total_SR,     err_SR),
        }

        MistagRate(CR_mistag_i, CR_all_i, "CR", option, title, label, mistag_jet_list[i])
        MistagRate(VR_mistag_i, VR_all_i, "VR", option, title, label, mistag_jet_list[i])

        # VR closure: observed vs predicted mistag
        legend_labels = ["Observed mistag (VR)", "Predicted mistag (VR)"]
        png_title     = "3d_hist_projection_VR_mistags_" + mistag_jet_list[i]
        DrawCanvasAndPlots(
            "c3", "Mistag plots in the VR", option, title,
            [[proj_pT_VR_mistag,  proj_pT_VR_mistag_predict],
             [proj_eta_VR_mistag, proj_eta_VR_mistag_predict],
             [proj_phi_VR_mistag, proj_phi_VR_mistag_predict]],
            legend_labels, png_title,
            ["Jet p_{T} Mistags in VR, " + mistag_jet_list[i],
             "Jet #eta Mistags in VR, " + mistag_jet_list[i],
             "Jet #phi Mistags in VR, " + mistag_jet_list[i]],
            label
        )

        MakePlotWithRatio([proj_pT_VR_mistag,  proj_pT_VR_mistag_predict],  legend_labels, label + "_pT",  png_title)
        MakePlotWithRatio([proj_eta_VR_mistag, proj_eta_VR_mistag_predict], legend_labels, label + "_eta", png_title)
        MakePlotWithRatio([proj_phi_VR_mistag, proj_phi_VR_mistag_predict], legend_labels, label + "_phi", png_title)

        # SR prediction (blinded — no observed mistag plotted)
        legend_labels = ["Predicted mistag (SR)"]
        png_title     = "3d_hist_projection_SR_mistags_" + mistag_jet_list[i]
        DrawCanvasAndPlots(
            "c4", "Mistag plots in the SR", option, title,
            [[proj_pT_SR_mistag_predict],
             [proj_eta_SR_mistag_predict],
             [proj_phi_SR_mistag_predict]],
            legend_labels, png_title,
            ["Jet p_{T} Projected Mistags from CR, " + mistag_jet_list[i],
             "Jet #eta Projected Mistags from CR, " + mistag_jet_list[i],
             "Jet #phi Projected Mistags from CR, " + mistag_jet_list[i]],
            label
        )

    return yields


def print_pv_split_summary(results):
    """
    --pv_split only: compare the PV-binned prediction, pred(PV<42) + pred(PV>=42),
    with the PV-inclusive prediction, and check that the two PV slices really
    partition the inclusive sample.

    results : {option: yields dict returned by MisTagParametrization}

    Line wording deliberately avoids everything OutputToLatex_VRclosureCheck.py
    and ScanDNNCuts_bkg_config.py key on ("... mistagged events in VR/SR",
    "low pv", "high pv", "depth", bare jet names), so appending this block
    cannot change what they parse.
    """
    inc, low, high = results["depth"], results["depth, low PV"], results["depth, high PV"]

    def _sum(a, b):
        return a[0] + b[0], math.sqrt(a[1] ** 2 + b[1] ** 2)   # disjoint slices

    def _pull(obs, pred):
        sigma = math.sqrt(obs[1] ** 2 + pred[1] ** 2)
        return (obs[0] - pred[0]) / sigma if sigma > 0 else 0.0

    print("\n \n ********************* \n PV-binned summary (--pv_split) \n ********************* \n")
    print("PV-binned = sum of the PV<42 and PV>=42 slices; shift = PV-binned - inclusive.")
    print("Pulls are (obs - pred) / sqrt(stat_obs^2 + stat_pred^2).\n")

    for jet in ["leading", "sub-leading", "combined"]:
        i_, l_, h_ = inc[jet], low[jet], high[jet]
        vr_binned = _sum(l_["pred_VR"], h_["pred_VR"])
        sr_binned = _sum(l_["pred_SR"], h_["pred_SR"])
        print(f"{jet} jet:")
        print(f"  VR observed : inclusive {i_['obs_VR'][0]:.2f} ± {i_['obs_VR'][1]:.2f}"
              f" | PV<42 {l_['obs_VR'][0]:.2f} ± {l_['obs_VR'][1]:.2f}"
              f" | PV>=42 {h_['obs_VR'][0]:.2f} ± {h_['obs_VR'][1]:.2f}")
        print(f"  VR predicted: inclusive {i_['pred_VR'][0]:.2f} ± {i_['pred_VR'][1]:.2f}"
              f" | PV<42 {l_['pred_VR'][0]:.2f} ± {l_['pred_VR'][1]:.2f}"
              f" | PV>=42 {h_['pred_VR'][0]:.2f} ± {h_['pred_VR'][1]:.2f}"
              f" | PV-binned {vr_binned[0]:.2f} ± {vr_binned[1]:.2f}"
              f" | shift {vr_binned[0] - i_['pred_VR'][0]:+.2f}")
        print(f"  VR pull     : inclusive {_pull(i_['obs_VR'], i_['pred_VR']):+.2f}"
              f" | PV<42 {_pull(l_['obs_VR'], l_['pred_VR']):+.2f}"
              f" | PV>=42 {_pull(h_['obs_VR'], h_['pred_VR']):+.2f}")
        print(f"  SR predicted: inclusive {i_['pred_SR'][0]:.2f} ± {i_['pred_SR'][1]:.2f}"
              f" | PV-binned {sr_binned[0]:.2f} ± {sr_binned[1]:.2f}"
              f" | shift {sr_binned[0] - i_['pred_SR'][0]:+.2f}")

        # Coverage tripwire: PV<42 and PV>=42 should add up to the inclusive
        # yield.  The high-PV slice stops at PV < 100, so a few events per
        # million sit above it.  Those matter only if they are TAGGED (they
        # enter the prediction directly) or exceed 0.1% of a region; otherwise
        # they are reported without a warning.
        lost_tagged, lost_denom = [], []
        d = i_["CR_mistag"] - (l_["CR_mistag"] + h_["CR_mistag"])
        if abs(d) > 0.5:
            lost_tagged.append(f"CR_mistag: {d:+.0f}")
        d = i_["obs_VR"][0] - (l_["obs_VR"][0] + h_["obs_VR"][0])
        if abs(d) > 0.5:
            lost_tagged.append(f"VR observed: {d:+.0f}")
        for key in ["CR_all", "VR_all", "SR_all"]:
            d = i_[key] - (l_[key] + h_[key])
            if abs(d) > 0.5:
                lost_denom.append((key, d, d / i_[key] if i_[key] else 0.0))
        worst = max((abs(f) for _, _, f in lost_denom), default=0.0)
        if lost_tagged or worst > 1e-3:
            print("  *** WARNING: PV slices do not add up to the inclusive sample "
                  "(events outside the PV ranges): "
                  + "; ".join(lost_tagged + [f"{k}: {d:+.0f} ({f:+.2%})" for k, d, f in lost_denom]))
        elif lost_denom:
            print("  PV coverage check: OK — "
                  + ", ".join(f"{k} {d:+.0f} ({f:+.4%})" for k, d, f in lost_denom)
                  + " outside the PV slices; none tagged, negligible")
        else:
            print("  PV coverage check: OK (slices add up to the inclusive yields)")
        print("")


# ==============================================================================
# Argument parsing and main
# ==============================================================================

# ==============================================================================
# Config file support
# ==============================================================================

def parse_config_file(path):
    """
    Parse a whitespace-delimited config file.  Lines starting with # are
    comments.  The first non-comment line is treated as the header and defines
    the column order.  All subsequent non-empty lines are data rows.

    Required columns: era, DNN_cut_LJDC, DNN_cut_inc_LJDC, DNN_cut_SJDC, DNN_cut_inc_SJDC
    Optional column:  CR_cut_inc  (defaults to 0.2 if absent)

    Example file
    ------------
    # era   DNN_cut_LJDC  DNN_cut_inc_LJDC  DNN_cut_SJDC  DNN_cut_inc_SJDC  CR_cut_inc
    2022    0.90          0.97              0.90           0.97              0.20
    2023    0.93          0.97              0.95           0.95              0.15
    2023    0.90          0.97              0.90           0.97              0.20
    """
    required = ["era", "DNN_cut_LJDC", "DNN_cut_inc_LJDC", "DNN_cut_SJDC", "DNN_cut_inc_SJDC"]
    defaults = {"CR_cut_inc": 0.2}

    rows = []
    header = None
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if header is None:
                # First non-comment line is the header
                header = line.split()
                continue
            values = line.split()
            if len(values) != len(header):
                raise ValueError(f"Config row has {len(values)} columns, expected {len(header)}: {line!r}")
            row = dict(zip(header, values))
            # Validate required columns
            for col in required:
                if col not in row:
                    raise ValueError(f"Config file is missing required column: {col}")
            # Apply defaults for optional columns
            for col, default in defaults.items():
                if col not in row:
                    row[col] = default
            # Type conversion
            row["era"] = str(row["era"])
            for col in ["DNN_cut_LJDC", "DNN_cut_inc_LJDC", "DNN_cut_SJDC", "DNN_cut_inc_SJDC", "CR_cut_inc"]:
                row[col] = float(row[col])
            rows.append(row)

    if not rows:
        raise ValueError(f"Config file {path!r} contains no data rows.")
    return rows


# ==============================================================================
# Argument parsing and main
# ==============================================================================

def parseArgs():
    parser = argparse.ArgumentParser(add_help=True, description="Optimised mistag parametrisation")

    # --- Config file mode (takes precedence over individual score flags) ---
    parser.add_argument("--config",             action="store", default=None,
                        help="Path to a config file listing parameter sets to scan "
                             "(see parse_config_file docstring for format). "
                             "When provided, -e/-d/-i/-c are ignored.")

    # --- Single-run flags (used when --config is not given) ---
    parser.add_argument("-e", "--era",            action="store", default=None,
                        help="era: '2022' or '2023'")
    parser.add_argument("-d", "--DNN_cut",          action="store", default=0.9,   type=float,
                        help="Depth DNN score cut for both LJDC and SJDC (default: 0.9)")
    parser.add_argument("--DNN_cut_LJDC",           action="store", default=None,  type=float,
                        help="Depth DNN score cut for LJDC only (default: same as -d)")
    parser.add_argument("--DNN_cut_SJDC",           action="store", default=None,  type=float,
                        help="Depth DNN score cut for SJDC only (default: same as -d)")
    parser.add_argument("-i", "--DNN_cut_inc",      action="store", default=0.97,  type=float,
                        help="Inclusive DNN score VR/SR boundary for both LJDC and SJDC (default: 0.97)")
    parser.add_argument("--DNN_cut_inc_LJDC",       action="store", default=None,  type=float,
                        help="Inclusive DNN score VR/SR boundary for LJDC only (default: same as -i)")
    parser.add_argument("--DNN_cut_inc_SJDC",       action="store", default=None,  type=float,
                        help="Inclusive DNN score VR/SR boundary for SJDC only (default: same as -i)")
    parser.add_argument("-c", "--CR_cut_inc",       action="store", default=0.2,   type=float,
                        help="Inclusive DNN score upper bound for CR, shared by LJDC and SJDC (default: 0.2)")

    parser.add_argument("--pv_split",               action="store_true",
                        help="Apply the low/high PV cut to every region (CR, VR, SR) instead of the "
                             "CR only, so the PV systematic comes from a PV-binned prediction "
                             f"(same option as SidebandParameterization_BkgPred.py). Output goes to "
                             f"{OUT_BASE_PV_SPLIT}/ instead of the current directory. Omit to get "
                             "the default, conservative CR-only PV variation.")

    # --- Always required ---
    parser.add_argument("-b", "--b_tag_combined",  action="store_true",
                        help="combined b-tag categories")
    parser.add_argument("-t", "--threads",         action="store", default=0, type=int,
                        help="Number of threads for RDataFrame (0 = auto-detect)")
    return parser.parse_args()


# Input file lists
_file_map = {
    "2022_2023": [
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Cv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Dv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Ev1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Fv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Gv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv2_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv3_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv4_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Dv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Dv2_scores.root",
    ],
    "2022_23_preBPix": [
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Cv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Dv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Ev1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Fv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Gv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv2_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv3_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv4_scores.root",
    ],
    "2022": [
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Cv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Dv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Ev1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Fv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Gv1_scores.root",
    ],
    "2023_preBPix": [
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv2_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv3_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv4_scores.root",
    ],
    "2022_D": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Dv1_scores.root"],
    "2022_E": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Ev1_scores.root"],
    "2022_F": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Fv1_scores.root"],
    "2022_G": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2022Gv1_scores.root"],

    "2023_postBPix": [
        # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv1_scores.root",
        # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv2_scores.root",
        # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv3_scores.root",
        # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv4_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Dv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Dv2_scores.root",
    ],
    "2023_Cv1": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv1_scores.root"],
    "2023_Cv2": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv2_scores.root"],
    "2023_Cv3": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv3_scores.root"],
    "2023_Cv4": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Cv4_scores.root"],
    "2023_Dv1": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Dv1_scores.root"],
    "2023_Dv2": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.6/minituple_data_2023Dv2_scores.root"],
    # "WPlusJets": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v4.1/minituple_WJetsToLNu_MC22preEE_NoSel_scores_v5updatedscores.root"]
    "WPlusJets": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v4.1/minituple_WJetsToLNu_MC22preEE_NoSel_scores_v7_cat12.root"]
}


def _build_rdf_base(era_key):
    """Build the base RDataFrame (run exclusion + deltaPhi) for a given era."""
    rdf = ROOT.RDataFrame("NoSel", _file_map[era_key])
    is_mc = era_key == "WPlusJets"
    if era_key == "2022_2023":
        run_excl = "g_excl_runs_2022_2023.find(run) == g_excl_runs_2022_2023.end()"
    elif era_key.startswith("2023"):
        run_excl = "g_excl_runs_2023.find(run) == g_excl_runs_2023.end()"
    else:
        run_excl = "g_excl_runs_2022.find(run) == g_excl_runs_2022.end()"
    # Flag_METFilters_2022_2023_PromptReco is always 0 in MC; skip it for MC samples
    met_filter = "abs(jet0_jet1_dPhi) > 0.2" if is_mc else \
                 "abs(jet0_jet1_dPhi) > 0.2 && Flag_METFilters_2022_2023_PromptReco == 1"
    # jet0_EtaAxis / jet1_EtaAxis feed the eta axis of Histo3D bookings below;
    # toggle USE_ABS_ETA (top of file) to switch between |eta| and signed eta.
    eta_expr0 = "abs(jet0_Eta)" if USE_ABS_ETA else "jet0_Eta"
    eta_expr1 = "abs(jet1_Eta)" if USE_ABS_ETA else "jet1_Eta"
    # jet0_PtAxis / jet1_PtAxis feed the pT axis: jets above the top bin edge are
    # clamped into the overflow bin so they are counted instead of being dropped
    # into ROOT's overflow (which Integral()/get_total_and_error() ignore).
    pt_expr0 = f"std::min<float>(jet0_Pt, {PT_FILL_CLAMP})"
    pt_expr1 = f"std::min<float>(jet1_Pt, {PT_FILL_CLAMP})"
    rdf_filtered = (rdf
                    .Filter(run_excl, "run exclusion")
                    .Filter(met_filter, "deltaPhi + METFilters")
                    .Define("jet0_EtaAxis", eta_expr0)
                    .Define("jet1_EtaAxis", eta_expr1)
                    .Define("jet0_PtAxis", pt_expr0)
                    .Define("jet1_PtAxis", pt_expr1))
    if is_mc:
        rdf_filtered = rdf_filtered.Filter("Pass_WPlusJets >= 0", "W+Jets selection")
    return rdf_filtered


def _run_one(rdf_base, row, b_tag_combined_flag):
    """
    Set globals for one parameter set, book histograms, and write all output.
    rdf_base is already filtered for run exclusion and deltaPhi.
    """
    global era, era_name, DNN_cut, DNN_cut_LJDC, DNN_cut_SJDC, \
           DNN_cut_inc, DNN_cut_inc_LJDC, DNN_cut_inc_SJDC, \
           CR_cut_inc, b_tag_combined, is_mc

    era              = row["era"]
    era_name         = era.replace(" ", "")
    is_mc            = era == "WPlusJets"
    DNN_cut_LJDC     = row["DNN_cut_LJDC"]
    DNN_cut_SJDC     = row["DNN_cut_SJDC"]
    DNN_cut_inc_LJDC = row["DNN_cut_inc_LJDC"]
    DNN_cut_inc_SJDC = row["DNN_cut_inc_SJDC"]
    CR_cut_inc       = row["CR_cut_inc"]
    b_tag_combined   = b_tag_combined_flag
    # Keep DNN_cut / DNN_cut_inc as the LJDC values for plot labels (LabelCMS)
    DNN_cut          = DNN_cut_LJDC
    DNN_cut_inc      = DNN_cut_inc_LJDC

    print(f"\n{'='*60}")
    print(f"Era: {era}")
    print(f"Depth DNN cut  — LJDC: {DNN_cut_LJDC},  SJDC: {DNN_cut_SJDC}")
    print(f"Inclusive cut  — LJDC: {DNN_cut_inc_LJDC},  SJDC: {DNN_cut_inc_SJDC}")
    print(f"CR inclusive upper bound (shared): {CR_cut_inc}")
    print(f"{'='*60}")

    print("Booking all histograms (lazy, no I/O yet)...")
    booked = book_all_histograms(rdf_base, is_mc=(era == "WPlusJets"))

    def _pt(v): return str(v).replace("0.", "pt")
    btag_str = "_combined" if b_tag_combined else ""
    if DNN_cut_LJDC == DNN_cut_SJDC and DNN_cut_inc_LJDC == DNN_cut_inc_SJDC:
        output_filename = f"DNN_{_pt(DNN_cut_LJDC)}_inc{_pt(DNN_cut_inc_LJDC)}_{era}_forPython{btag_str}.txt"
    else:
        output_filename = (f"DNN_LJDC{_pt(DNN_cut_LJDC)}_SJDC{_pt(DNN_cut_SJDC)}"
                           f"_incLJDC{_pt(DNN_cut_inc_LJDC)}_incSJDC{_pt(DNN_cut_inc_SJDC)}"
                           f"_{era}_forPython{btag_str}.txt")
    output_filename = os.path.join(OUT_BASE, output_filename)
    print(f"Writing numeric output to: {output_filename}")

    with open(output_filename, "w") as f:
        with redirect_stdout(f):
            print("\n \n ********************* \n DNN score = " + str(DNN_cut) +
                  " \n ********************* \n \n")
            if PV_SPLIT:
                # Marker read by OutputToLatex_VRclosureCheck.py to select the
                # PV-binned systematic.  Printed only in this mode, so the
                # default output is unchanged.
                print("PV treatment: split in all regions (--pv_split)\n")

            results = {}
            print("\n \n ********************* \n depth \n ********************* \n \n")
            hists = materialise(booked, "depth")
            results["depth"] = MisTagParametrization(hists, "depth")

            print("\n \n ********************* \n depth, low PV \n ********************* \n \n")
            hists = materialise(booked, "depth, low PV")
            results["depth, low PV"] = MisTagParametrization(hists, "depth, low PV")

            print("\n \n ********************* \n depth, high PV \n ********************* \n \n")
            hists = materialise(booked, "depth, high PV")
            results["depth, high PV"] = MisTagParametrization(hists, "depth, high PV")

            if PV_SPLIT:
                print_pv_split_summary(results)


def main():
    print("Parsing arguments...")
    args = parseArgs()
    configure_pv_mode(args.pv_split)
    print(f"PV treatment: {'split in all regions' if PV_SPLIT else 'CR only (default)'}"
          f"  ->  output under {OUT_BASE + '/' if OUT_BASE else 'the current directory'}")

    # Enable multi-threading once, before any RDataFrame is created
    if args.threads > 0:
        ROOT.EnableImplicitMT(args.threads)
    else:
        ROOT.EnableImplicitMT()

    # Build list of parameter sets to process
    if args.config:
        print(f"Reading parameter sets from config file: {args.config}")
        param_sets = parse_config_file(args.config)
    else:
        if args.era is None:
            raise ValueError("Either --config or -e/--era must be provided.")
        param_sets = [{
            "era":              args.era,
            "DNN_cut_LJDC":     args.DNN_cut_LJDC   if args.DNN_cut_LJDC   is not None else args.DNN_cut,
            "DNN_cut_SJDC":     args.DNN_cut_SJDC   if args.DNN_cut_SJDC   is not None else args.DNN_cut,
            "DNN_cut_inc_LJDC": args.DNN_cut_inc_LJDC if args.DNN_cut_inc_LJDC is not None else args.DNN_cut_inc,
            "DNN_cut_inc_SJDC": args.DNN_cut_inc_SJDC if args.DNN_cut_inc_SJDC is not None else args.DNN_cut_inc,
            "CR_cut_inc":       args.CR_cut_inc,
        }]

    print(f"Total parameter sets to process: {len(param_sets)}")

    # Group by era so rdf_base is built only once per era
    from collections import defaultdict
    by_era = defaultdict(list)
    for row in param_sets:
        if row["era"] not in _file_map:
            raise ValueError(f"Unsupported era: {row['era']}. Must be one of {list(_file_map.keys())}")
        by_era[row["era"]].append(row)

    for era_key, rows in by_era.items():
        print(f"\nBuilding RDataFrame for era {era_key} (shared across {len(rows)} parameter set(s))...")
        rdf_base = _build_rdf_base(era_key)

        for i, row in enumerate(rows, 1):
            print(f"\n--- Parameter set {i}/{len(rows)} for era {era_key} ---")
            _run_one(rdf_base, row, args.b_tag_combined)


if __name__ == "__main__":
    main()
