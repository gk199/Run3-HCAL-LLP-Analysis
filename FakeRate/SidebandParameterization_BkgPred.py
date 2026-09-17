#!/usr/bin/env python3
"""
Sideband (ABCD-style) background prediction — companion to
MisTagParametrization_3D_optimized.py, from which it is copied.

Difference vs. the mistag-rate method
-------------------------------------
MisTagParametrization_3D_optimized.py measures

    mistag rate = N(CR, depth >= DNN_cut) / N(CR, all depth scores)

and multiplies the *inclusive* VR / SR yields by it.

This script instead measures a TRANSFER FACTOR from a data sideband in the
DEPTH score:

    TF = N(CR, depth >= DNN_cut) / N(CR, SB_low <= depth < DNN_cut)

i.e. numerator = the high-scoring (tagged) CR, denominator = the CR sideband
running from SB_low up to the tag threshold.  The two regions are disjoint,
so the numerator is no longer a subset of the denominator.  The TF is then
applied to the *sideband* of the VR and of the SR:

    predicted tags in VR = TF x N(VR, SB_low <= depth < DNN_cut)
    predicted tags in SR = TF x N(SR, SB_low <= depth < DNN_cut)

SB_low is 0.8 for postBPix eras and 0.3 for preBPix eras (see
POSTBPIX_ERAS / SIDEBAND_LOW_POSTBPIX / SIDEBAND_LOW_PREBPIX below, and the
--sideband_low override).

This is a 2D ABCD in (inclusive score of one jet) x (depth score of the other
jet):

      depth score
        ^
 tagged |   [CR tag]     [VR tag]     [SR tag]  <-- SR cell is BLINDED
        |   (A, meas.)   (obs.)       (predicted only)
 SB_low |-------------------------------------
        | [CR sideband] [VR sideband] [SR sideband]
        |   (B, meas.)   (C, meas.)   (D, meas.)
        +-------------------------------------> inclusive score
            CR             VR            SR

BLINDING
--------
The signal-region tagged cell (high depth score AND high inclusive score) is
NEVER read out of the data.  For data no histogram is even BOOKED for that
cell — book_all_histograms() only creates the SR *sideband* histogram, and
both book_all_histograms() and materialise() carry a hard tripwire that raises
if an SR-tagged histogram ever appears for a non-MC era.  Nothing in this
script integrates, projects, plots or prints the blinded cell.  The only SR
numbers reported are (a) the SR sideband yield, which is by construction below
the tag threshold, and (b) the *prediction* TF x N(SR sideband).
(The W+Jets MC era is the sole exception: MC is not blinded, so the SR-tagged
histogram is booked there to allow a closure test, exactly as in the original.)

Outputs
-------
Same format as MisTagParametrization_3D_optimized.py (same text lines, same
plot set, same ROOT histogram names), but everything is written under
OUT_BASE ("SidebandBkgPred/") so that nothing produced by the original script
is overwritten.  The .txt files keep their original names inside that
directory, so from FakeRate/ (use the same -b choice as for this script)

    python3 OutputToLatex_VRclosureCheck.py --config my_scan.txt --input_dir SidebandBkgPred
    python3 OutputToLatex_VRclosureCheck.py --config my_scan.txt --pv_split

parse them unchanged.

Usage (same flags as MisTagParametrization_3D_optimized.py):
  python SidebandParameterization_BkgPred.py -e 2022 -b -d 0.9 -i 0.97
  python SidebandParameterization_BkgPred.py -e 2023_postBPix -b -d 0.93 -i 0.97
  python SidebandParameterization_BkgPred.py --config my_scan.txt -b
Optional extra flags:
  --sideband_low 0.5      # override the era-derived depth sideband lower bound
  --pv_split              # PV systematic from a PV-binned prediction (see below)

PV systematic: two modes
------------------------
Default (no flag) — "CR-only" PV variation, identical to the original script:
the "depth, low PV" / "depth, high PV" options apply the PV cut to the CR ONLY,
so each measures a TF from one PV slice of the CR and applies it to the
PV-inclusive VR/SR.  OutputToLatex_VRclosureCheck.py takes the envelope of the
two shifts as the systematic.  Output goes to SidebandBkgPred/.

--pv_split — "PV-binned" prediction: the PV cut is applied to EVERY region
(CR, VR and SR sidebands, and the CR/VR tagged cells), so each PV option is a
self-contained prediction for that PV slice.  The two slices partition the
inclusive sample, so

    PV-binned prediction = pred(low PV) + pred(high PV)

uses the actual PV mix of the VR/SR, and the PV systematic is
|PV-binned - inclusive|.  It vanishes when the VR/SR PV mix matches the CR's,
and it does not pick up the subsampling noise of the CR-only variation.  Each
PV slice also gets its own VR closure test.  The SR is still booked in the
sideband only, so blinding is unchanged.  Output goes to
SidebandBkgPred_PVsplit/, the txt carries a "PV treatment: split" marker line
that OutputToLatex_VRclosureCheck.py uses to pick the matching systematic, and
a PV-binned summary (with a check that the two slices add up to the inclusive
yields) is appended to the txt.
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
pT_bins  = np.array([40, 100, 160, 400]) # np.array([40, 100, 400], dtype=float) # np.array([40, 100, 160, 250, 1000], dtype=float)
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

# ---- Depth-score sideband definition ----
# The transfer factor denominator is the depth-score band [SB_low, DNN_cut).
# SB_low depends on the era group: the depth score distribution is shifted
# between preBPix and postBPix, so the sideband is placed higher for postBPix.
SIDEBAND_LOW_POSTBPIX = 0.8
SIDEBAND_LOW_PREBPIX  = 0.3
# Eras taken after the BPix issue.  Everything else in _file_map (2022*, the
# 2023 C-eras, and the W+Jets MC, which is a 2022 sample) is preBPix.  The
# mixed "2022_2023" era group is predominantly preBPix and is treated as such,
# with a warning printed at run time.
POSTBPIX_ERAS = {"2023_postBPix", "2023_Dv1", "2023_Dv2"}
MIXED_BPIX_ERAS = {"2022_2023"}
SB_low = SIDEBAND_LOW_PREBPIX   # set per parameter set in _run_one()


def sideband_low_for_era(era_key):
    """Depth-score sideband lower bound for an era key (see POSTBPIX_ERAS)."""
    if era_key in POSTBPIX_ERAS:
        return SIDEBAND_LOW_POSTBPIX
    if era_key in MIXED_BPIX_ERAS:
        print(f"*** WARNING: era '{era_key}' mixes preBPix and postBPix data; "
              f"using the preBPix sideband bound {SIDEBAND_LOW_PREBPIX}. "
              f"Override with --sideband_low if that is not what you want.")
    return SIDEBAND_LOW_PREBPIX


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

current_jet_type = "leading"  # updated per-iteration in SidebandParametrization

# All output lives under OUT_BASE so that nothing written by
# MisTagParametrization_3D_optimized.py is overwritten.  Names inside are
# unchanged, so the downstream parsers work when run from within OUT_BASE.
# --pv_split writes to its own directory so both PV treatments can coexist.
OUT_BASE_DEFAULT  = "SidebandBkgPred"
OUT_BASE_PV_SPLIT = "SidebandBkgPred_PVsplit"
OUT_BASE   = OUT_BASE_DEFAULT
output_dir = os.path.join(OUT_BASE, "outPlots_3D")

# False: PV cut on the CR only (default).  True: PV cut on every region.
PV_SPLIT = False


def configure_pv_mode(pv_split):
    """Set the PV treatment and the matching output directories (see module docstring)."""
    global PV_SPLIT, OUT_BASE, output_dir
    PV_SPLIT   = pv_split
    OUT_BASE   = OUT_BASE_PV_SPLIT if pv_split else OUT_BASE_DEFAULT
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


def TransferFactorPlot(pass_hist, sideband_hist, plot_type, option, title, label, type_of_jet):
    """
    Plot the transfer factor N(tagged) / N(sideband) projected onto pT, eta, phi.

    Same structure as MistagRate() in MisTagParametrization_3D_optimized.py; only
    the denominator (depth sideband instead of all depth scores) and the labels
    differ.  Only ever called for the CR and the VR — never for the SR, whose
    tagged cell is blinded.
    """
    transfer_factor = pass_hist.Clone("transfer_factor_" + plot_type)
    ResetAxis(transfer_factor)
    proj_pT_TF, proj_eta_TF, proj_phi_TF = \
        ProjectHistogram(transfer_factor, "Transfer factor")
    total = sideband_hist.Clone("sideband_" + plot_type)
    proj_pT_total, proj_eta_total, proj_phi_total = \
        ProjectHistogram(total, "Number of events")
    proj_pT_TF.Divide(proj_pT_total)
    proj_eta_TF.Divide(proj_eta_total)
    proj_phi_TF.Divide(proj_phi_total)

    legend_labels = ["Transfer factor (" + plot_type + ")"]
    png_title     = "3d_hist_projection_" + plot_type + "_transfer_factor_" + type_of_jet
    DrawCanvasAndPlots(
        "c_" + plot_type,
        "Transfer factor plots in the " + plot_type + " with " + type_of_jet,
        option, title,
        [[proj_pT_TF], [proj_eta_TF], [proj_phi_TF]],
        legend_labels, png_title,
        ["Jet p_{T} Transfer Factor from " + plot_type + ", " + type_of_jet,
         "Jet #eta Transfer Factor from " + plot_type + ", " + type_of_jet,
         "Jet #phi Transfer Factor from " + plot_type + ", " + type_of_jet],
        label
    )
    proj_pT_TF.Clear()
    proj_eta_TF.Clear()
    proj_phi_TF.Clear()


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
        stamp_text.DrawLatex(xpos + 0.4,  ypos,        "#scale[0.65]{DNN score > " + str(_dnn_label) +
                                                       ", SB #geq " + str(SB_low) + "}")
        stamp_text.DrawLatex(xpos + 0.4,  ypos - 0.04, "#scale[0.65]{Era = " + era + "}")
    else:
        _dnn_label = DNN_cut_SJDC if current_jet_type == "sub-leading" else DNN_cut_LJDC
        stamp_text.DrawLatex(xpos + 0.6,  ypos + 0.03, yearLumi)
        stamp_text.DrawLatex(xpos + 0.3,  ypos,        "#scale[0.65]{DNN score > " + str(_dnn_label) +
                                                       ", SB #geq " + str(SB_low) + "}")
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

    Compared with MisTagParametrization_3D_optimized.py the depth-score
    categories change: instead of (all depth scores) and (depth >= DNN_cut),
    this books the SIDEBAND [SB_low, DNN_cut) and the TAGGED [DNN_cut, 1.1)
    cells.  For data the SR is booked ONLY in the sideband — the SR tagged cell
    is blinded and no histogram for it exists.

    Returns
    -------
    booked : dict
        booked[option]["CR_side"] etc. are RResultPtr<TH3D> objects.
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
    CR_str       = f"{inc1} >= 0.0 && {inc1} < {CR_cut_inc}"
    VR_str       = f"{inc1} >= {CR_cut_inc} && {inc1} < {DNN_cut_inc_LJDC}"
    SR_str       = f"{inc1} >= {DNN_cut_inc_LJDC} && {inc1} < 1.1"
    pass_str     = f"{depth0} >= {DNN_cut_LJDC} && {depth0} < 1.1"
    sideband_str = f"{depth0} >= {SB_low} && {depth0} < {DNN_cut_LJDC}"

    # SJDC: jet1 is the depth tag candidate; jet0 inclusive score defines CR/VR/SR
    CR_0_str       = f"{inc0} >= 0.0 && {inc0} < {CR_cut_inc}"
    VR_0_str       = f"{inc0} >= {CR_cut_inc} && {inc0} < {DNN_cut_inc_SJDC}"
    SR_0_str       = f"{inc0} >= {DNN_cut_inc_SJDC} && {inc0} < 1.1"
    pass_1_str     = f"{depth1} >= {DNN_cut_SJDC} && {depth1} < 1.1"
    sideband_1_str = f"{depth1} >= {SB_low} && {depth1} < {DNN_cut_SJDC}"

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

        # Default: PV cut is applied only to the CR (transfer factor measurement
        # region), not to VR or SR — matches original MisTagParametrization logic.
        # --pv_split: the same PV cut is applied to the VR and SR as well, so each
        # PV option is a complete prediction for that PV slice.
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
            "CR_side": rdf_CR_j0.Filter(sideband_str).Histo3D(
                           _h3_model(f"h_CR_side_{s}"),   "jet0_PtAxis", "jet0_EtaAxis", "jet0_Phi"),
            "CR_pass": rdf_CR_j0.Filter(pass_str).Histo3D(
                           _h3_model(f"h_CR_pass_{s}"),   "jet0_PtAxis", "jet0_EtaAxis", "jet0_Phi"),
            "VR_side": rdf_VR_j0.Filter(sideband_str).Histo3D(
                           _h3_model(f"h_VR_side_{s}"),   "jet0_PtAxis", "jet0_EtaAxis", "jet0_Phi"),
            "VR_pass": rdf_VR_j0.Filter(pass_str).Histo3D(
                           _h3_model(f"h_VR_pass_{s}"),   "jet0_PtAxis", "jet0_EtaAxis", "jet0_Phi"),
            # SR: SIDEBAND ONLY for data.  The SR tagged cell is blinded.
            "SR_side": rdf_SR_j0.Filter(sideband_str).Histo3D(
                           _h3_model(f"h_SR_side_{s}"),   "jet0_PtAxis", "jet0_EtaAxis", "jet0_Phi"),
        }
        if is_mc:
            # MC only — not blinded, used for a closure test of the prediction.
            booked[option]["SR_pass"] = rdf_SR_j0.Filter(pass_str).Histo3D(
                           _h3_model(f"h_SR_pass_{s}"),   "jet0_PtAxis", "jet0_EtaAxis", "jet0_Phi")

        # --- jet1 triggered (jet1 has depth tag, jet0 defines CR/VR/SR) ---
        rdf_j1    = rdf_base.Filter(depth_j1_str,        f"depth_j1_{s}")
        rdf_CR_j1 = rdf_j1.Filter(CR_0_str + cr_extra,     f"CR_j1_{s}")
        rdf_VR_j1 = rdf_j1.Filter(VR_0_str + vr_sr_extra,  f"VR_j1_{s}")
        rdf_SR_j1 = rdf_j1.Filter(SR_0_str + vr_sr_extra,  f"SR_j1_{s}")

        booked[option].update({
            "CR_side_1": rdf_CR_j1.Filter(sideband_1_str).Histo3D(
                             _h3_model(f"h_CR_side_1_{s}"),  "jet1_PtAxis", "jet1_EtaAxis", "jet1_Phi"),
            "CR_pass_1": rdf_CR_j1.Filter(pass_1_str).Histo3D(
                             _h3_model(f"h_CR_pass_1_{s}"),  "jet1_PtAxis", "jet1_EtaAxis", "jet1_Phi"),
            "VR_side_1": rdf_VR_j1.Filter(sideband_1_str).Histo3D(
                             _h3_model(f"h_VR_side_1_{s}"),  "jet1_PtAxis", "jet1_EtaAxis", "jet1_Phi"),
            "VR_pass_1": rdf_VR_j1.Filter(pass_1_str).Histo3D(
                             _h3_model(f"h_VR_pass_1_{s}"),  "jet1_PtAxis", "jet1_EtaAxis", "jet1_Phi"),
            # SR: SIDEBAND ONLY for data.  The SR tagged cell is blinded.
            "SR_side_1": rdf_SR_j1.Filter(sideband_1_str).Histo3D(
                             _h3_model(f"h_SR_side_1_{s}"),  "jet1_PtAxis", "jet1_EtaAxis", "jet1_Phi"),
        })
        if is_mc:
            # MC only — not blinded, used for a closure test of the prediction.
            booked[option]["SR_pass_1"] = rdf_SR_j1.Filter(pass_1_str).Histo3D(
                             _h3_model(f"h_SR_pass_1_{s}"),  "jet1_PtAxis", "jet1_EtaAxis", "jet1_Phi")

    # ---- BLINDING TRIPWIRE -------------------------------------------------
    # For data, no histogram of the SR tagged cell (high depth AND high
    # inclusive score) may exist.  If a future edit books one, fail loudly here
    # rather than quietly unblinding the analysis.
    if not is_mc:
        for opt, d in booked.items():
            offenders = [k for k in d if k.startswith("SR_") and "side" not in k]
            if offenders:
                raise RuntimeError(
                    "BLINDING VIOLATION: SR tagged histogram(s) booked for data "
                    f"in option '{opt}': {offenders}. The signal region (high depth "
                    "score + high inclusive score) must remain blinded.")

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

    # ---- BLINDING TRIPWIRE (second line of defence, at read-out time) ----
    if not is_mc:
        offenders = [k for k in h if k.startswith("SR_") and "side" not in k]
        if offenders:
            raise RuntimeError(
                "BLINDING VIOLATION: refusing to read out SR tagged histogram(s) "
                f"for data: {offenders}.")

    def get(ptr, name):
        th = ptr.GetPtr()           # triggers computation on first access
        out = th.Clone(name)
        out.SetDirectory(0)
        if not out.GetSumw2N():
            out.Sumw2()             # activate proper error propagation
        check_axis_ranges(out, name)   # tripwire: nothing may sit in under/overflow
        return out

    key_to_name = {
        "CR_side":   "hist3d_CR_sideband",
        "CR_pass":   "hist3d_CR_pass",
        "VR_side":   "hist3d_VR_sideband",
        "VR_pass":   "hist3d_VR_pass",
        "SR_side":   "hist3d_SR_sideband",
        "SR_pass":   "hist3d_SR_pass",
        "CR_side_1": "hist3d_CR_sideband_1",
        "CR_pass_1": "hist3d_CR_pass_1",
        "VR_side_1": "hist3d_VR_sideband_1",
        "VR_pass_1": "hist3d_VR_pass_1",
        "SR_side_1": "hist3d_SR_sideband_1",
        "SR_pass_1": "hist3d_SR_pass_1",
    }
    return {k: get(v, key_to_name[k]) for k, v in h.items()}


# ==============================================================================
# Analysis logic — same structure as MisTagParametrization(), but the CR rate
# is a sideband transfer factor and it is applied to the VR / SR sidebands.
# ==============================================================================

_option_meta = {
    "depth":          (": 2+ depth",         "_depth"),
    "depth, low PV":  (": 2+ depth, low PV",  "_depth_lowPV"),
    "depth, high PV": (": 2+ depth, high PV", "_depth_highPV"),
}


def SidebandParametrization(hists, option):
    """
    Process pre-computed histograms for one option and produce all plots /
    ROOT output files.

    Parameters
    ----------
    hists  : dict returned by materialise()
    option : str, one of the keys in _option_meta
    """
    title, label = _option_meta[option]

    CR_side = hists["CR_side"]
    CR_pass = hists["CR_pass"]
    VR_side = hists["VR_side"]
    VR_pass = hists["VR_pass"]
    SR_side = hists["SR_side"]

    CR_side_1 = hists["CR_side_1"]
    CR_pass_1 = hists["CR_pass_1"]
    VR_side_1 = hists["VR_side_1"]
    VR_pass_1 = hists["VR_pass_1"]
    SR_side_1 = hists["SR_side_1"]

    # Combine leading + sub-leading jet triggered histograms
    CR_side_combined = CR_side.Clone("hist3d_CR_sideband_combined"); CR_side_combined.Add(CR_side_1)
    CR_pass_combined = CR_pass.Clone("hist3d_CR_pass_combined");     CR_pass_combined.Add(CR_pass_1)
    VR_side_combined = VR_side.Clone("hist3d_VR_sideband_combined"); VR_side_combined.Add(VR_side_1)
    VR_pass_combined = VR_pass.Clone("hist3d_VR_pass_combined");     VR_pass_combined.Add(VR_pass_1)
    SR_side_combined = SR_side.Clone("hist3d_SR_sideband_combined"); SR_side_combined.Add(SR_side_1)

    if is_mc:
        SR_pass   = hists["SR_pass"]
        SR_pass_1 = hists["SR_pass_1"]
        SR_pass_combined = SR_pass.Clone("hist3d_SR_pass_combined"); SR_pass_combined.Add(SR_pass_1)
        SR_pass_list = [SR_pass, SR_pass_1, SR_pass_combined]

    print("created histograms for 1D rate evaluation")
    print("completed 1D rate evaluation")

    CR_side_list = [CR_side, CR_side_1, CR_side_combined]
    CR_pass_list = [CR_pass, CR_pass_1, CR_pass_combined]
    VR_side_list = [VR_side, VR_side_1, VR_side_combined]
    VR_pass_list = [VR_pass, VR_pass_1, VR_pass_combined]
    SR_side_list = [SR_side, SR_side_1, SR_side_combined]
    mistag_jet_list = ["leading", "sub-leading", "combined"]

    # Integrated yields per jet category, returned for the --pv_split summary.
    # Contains only sideband yields, VR observations and predictions — never
    # the SR tagged yield for data.
    yields = {}

    _sr_pass_iter = SR_pass_list if is_mc else [None, None, None]
    for i, (CR_side_i, CR_pass_i, VR_side_i, VR_pass_i, SR_side_i, SR_pass_i) in enumerate(
            zip(CR_side_list, CR_pass_list, VR_side_list, VR_pass_list, SR_side_list, _sr_pass_iter)):

        global current_jet_type
        current_jet_type = mistag_jet_list[i]

        print(" ************* \n " + mistag_jet_list[i] + " \n *************")

        # Write base histograms to ROOT file.  Both the descriptive sideband
        # names and the canonical names used by MisTagParametrization_3D_optimized.py
        # are written, so existing ROOT-reading tools (e.g. OverlayMistagRates.py)
        # work on these files too — bearing in mind that here "_all" means the
        # depth SIDEBAND, not all depth scores.
        outfile_name = os.path.join(
            OUT_BASE,
            "output_3D_hists" + label + "_" + mistag_jet_list[i] + "_" + era_name + ".root")
        output_file  = ROOT.TFile(outfile_name, "RECREATE")
        output_file.cd()
        CR_side_i.Write("hist3d_CR_sideband")
        CR_pass_i.Write("hist3d_CR_pass")
        VR_side_i.Write("hist3d_VR_sideband")
        VR_pass_i.Write("hist3d_VR_pass")
        SR_side_i.Write("hist3d_SR_sideband")
        CR_side_i.Write("hist3d_CR_all")
        CR_pass_i.Write("hist3d_CR_mistag")
        VR_side_i.Write("hist3d_VR_all")
        VR_pass_i.Write("hist3d_VR_mistag")
        SR_side_i.Write("hist3d_SR_all")
        print("Created root file for output, wrote 3D histograms")
        output_file.Close()

        # Projection overlay: CR and VR sideband + tagged
        proj_pT_CR_side, proj_eta_CR_side, proj_phi_CR_side = ProjectHistogram(CR_side_i, "Number of events")
        proj_pT_CR_pass, proj_eta_CR_pass, proj_phi_CR_pass = ProjectHistogram(CR_pass_i, "Number of events")
        proj_pT_VR_side, proj_eta_VR_side, proj_phi_VR_side = ProjectHistogram(VR_side_i, "Number of events")
        proj_pT_VR_pass, proj_eta_VR_pass, proj_phi_VR_pass = ProjectHistogram(VR_pass_i, "Number of events")

        legend_labels = ["CR, sideband", "CR, tagged", "VR, sideband", "VR, tagged"]
        png_title     = "3d_hist_projection_overlay_CR_VR_" + mistag_jet_list[i]
        DrawCanvasAndPlots(
            "c1", "Projection plots", option, title,
            [[proj_pT_CR_side,  proj_pT_CR_pass,  proj_pT_VR_side,  proj_pT_VR_pass],
             [proj_eta_CR_side, proj_eta_CR_pass, proj_eta_VR_side, proj_eta_VR_pass],
             [proj_phi_CR_side, proj_phi_CR_pass, proj_phi_VR_side, proj_phi_VR_pass]],
            legend_labels, png_title,
            ["Jet p_{T} Projection with various cuts, " + mistag_jet_list[i],
             "Jet #eta Projection with various cuts, " + mistag_jet_list[i],
             "Jet #phi Projection with various cuts, " + mistag_jet_list[i]],
            label, normalize=True
        )

        # Transfer factor from the CR sideband; use it to predict VR and SR.
        # Numerator and denominator are DISJOINT samples here (unlike the mistag
        # rate, where the numerator is a subset of the denominator), so the plain
        # uncorrelated error propagation done by TH1::Divide is the right one.
        CR_transfer_factor = CR_pass_i.Clone("CR_transfer_factor")
        CR_transfer_factor.Divide(CR_side_i)
        VR_pass_predict = VR_side_i.Clone("VR_pass_predict")
        VR_pass_predict.Multiply(CR_transfer_factor)

        ResetAxis(CR_transfer_factor)
        ResetAxis(VR_pass_predict)

        proj_pT_VR_pass_predict, proj_eta_VR_pass_predict, proj_phi_VR_pass_predict = \
            ProjectHistogram(VR_pass_predict, "Number of events")

        # NOTE: every number printed below is either a sideband yield (depth
        # score BELOW the tag threshold) or a prediction.  The SR tagged cell is
        # never read.
        print("\nNumber of events in CR sideband = " + str(CR_side_i.Integral()))
        print("Number of events in VR sideband = " + str(VR_side_i.Integral()))
        print("Number of events in SR sideband = " + str(SR_side_i.Integral()) + "\n")
        print("Number of tagged events in CR = " + str(CR_pass_i.Integral()))

        tf_num, tf_num_err = get_total_and_error(CR_pass_i)
        tf_den, tf_den_err = get_total_and_error(CR_side_i)
        if tf_den > 0:
            # Numerator and denominator are disjoint samples, so their relative
            # errors simply add in quadrature.
            tf_int      = tf_num / tf_den
            rel_num_sq  = (tf_num_err / tf_num) ** 2 if tf_num > 0 else 0.0
            rel_den_sq  = (tf_den_err / tf_den) ** 2
            tf_int_err  = tf_int * math.sqrt(rel_num_sq + rel_den_sq)
            print(f"Transfer factor (integrated, CR): {tf_int:.5f} ± {tf_int_err:.5f} (stat)")
        else:
            print("Transfer factor (integrated, CR): undefined — empty CR sideband")

        SR_pass_predict = SR_side_i.Clone("SR_pass_predict")
        SR_pass_predict.Multiply(CR_transfer_factor)
        print("Predicted number of mistagged events in SR = " + str(SR_pass_predict.Integral()) + "\n")
        proj_pT_SR_pass_predict, proj_eta_SR_pass_predict, proj_phi_SR_pass_predict = \
            ProjectHistogram(SR_pass_predict, "Number of events")

        total_pred,   err_pred   = get_total_and_error(VR_pass_predict)
        total_actual, err_actual = get_total_and_error(VR_pass_i)
        total_SR,     err_SR     = get_total_and_error(SR_pass_predict)

        print(option + ", " + mistag_jet_list[i])
        print(f"Observed mistagged events in VR: {total_actual:.2f} ± {err_actual:.2f} (stat)")
        print(f"Predicted mistagged events in VR: {total_pred:.2f} ± {err_pred:.2f} (stat)")
        if is_mc:
            # MC closure only — for data this branch is never taken and the SR
            # tagged yield does not exist.
            total_SR_actual, err_SR_actual = get_total_and_error(SR_pass_i)
            print(f"Observed mistagged events in SR: {total_SR_actual:.2f} ± {err_SR_actual:.2f} (stat)")
        print(f"Predicted mistagged events in SR: {total_SR:.2f} ± {err_SR:.2f} (stat)")

        yields[mistag_jet_list[i]] = {
            "CR_side": CR_side_i.Integral(),
            "CR_pass": CR_pass_i.Integral(),
            "VR_side": VR_side_i.Integral(),
            "SR_side": SR_side_i.Integral(),
            "obs_VR":  (total_actual, err_actual),
            "pred_VR": (total_pred,   err_pred),
            "pred_SR": (total_SR,     err_SR),
        }

        TransferFactorPlot(CR_pass_i, CR_side_i, "CR", option, title, label, mistag_jet_list[i])
        TransferFactorPlot(VR_pass_i, VR_side_i, "VR", option, title, label, mistag_jet_list[i])

        # VR closure: observed vs predicted tagged yield
        legend_labels = ["Observed mistag (VR)", "Predicted mistag (VR)"]
        png_title     = "3d_hist_projection_VR_mistags_" + mistag_jet_list[i]
        DrawCanvasAndPlots(
            "c3", "Mistag plots in the VR", option, title,
            [[proj_pT_VR_pass,  proj_pT_VR_pass_predict],
             [proj_eta_VR_pass, proj_eta_VR_pass_predict],
             [proj_phi_VR_pass, proj_phi_VR_pass_predict]],
            legend_labels, png_title,
            ["Jet p_{T} Mistags in VR, " + mistag_jet_list[i],
             "Jet #eta Mistags in VR, " + mistag_jet_list[i],
             "Jet #phi Mistags in VR, " + mistag_jet_list[i]],
            label
        )

        MakePlotWithRatio([proj_pT_VR_pass,  proj_pT_VR_pass_predict],  legend_labels, label + "_pT",  png_title)
        MakePlotWithRatio([proj_eta_VR_pass, proj_eta_VR_pass_predict], legend_labels, label + "_eta", png_title)
        MakePlotWithRatio([proj_phi_VR_pass, proj_phi_VR_pass_predict], legend_labels, label + "_phi", png_title)

        # SR prediction (blinded — the observed SR tagged yield is never read)
        legend_labels = ["Predicted mistag (SR)"]
        png_title     = "3d_hist_projection_SR_mistags_" + mistag_jet_list[i]
        DrawCanvasAndPlots(
            "c4", "Mistag plots in the SR", option, title,
            [[proj_pT_SR_pass_predict],
             [proj_eta_SR_pass_predict],
             [proj_phi_SR_pass_predict]],
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

    results : {option: yields dict returned by SidebandParametrization}

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
        d = i_["CR_pass"] - (l_["CR_pass"] + h_["CR_pass"])
        if abs(d) > 0.5:
            lost_tagged.append(f"CR_pass: {d:+.0f}")
        d = i_["obs_VR"][0] - (l_["obs_VR"][0] + h_["obs_VR"][0])
        if abs(d) > 0.5:
            lost_tagged.append(f"VR observed: {d:+.0f}")
        for key in ["CR_side", "VR_side", "SR_side"]:
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
# Config file support
# ==============================================================================

def parse_config_file(path):
    """
    Parse a whitespace-delimited config file.  Lines starting with # are
    comments.  The first non-comment line is treated as the header and defines
    the column order.  All subsequent non-empty lines are data rows.

    Required columns: era, DNN_cut_LJDC, DNN_cut_inc_LJDC, DNN_cut_SJDC, DNN_cut_inc_SJDC
    Optional columns: CR_cut_inc    (defaults to 0.2 if absent)
                      sideband_low  (defaults to the era-derived value if absent)

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
            # Optional per-row sideband lower bound; None -> derive from the era
            row["sideband_low"] = float(row["sideband_low"]) if "sideband_low" in row else None
            rows.append(row)

    if not rows:
        raise ValueError(f"Config file {path!r} contains no data rows.")
    return rows


# ==============================================================================
# Argument parsing and main
# ==============================================================================

def parseArgs():
    parser = argparse.ArgumentParser(add_help=True, description="Sideband transfer-factor background prediction")

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
    parser.add_argument("-s", "--sideband_low",     action="store", default=None,  type=float,
                        help="Lower bound of the depth-score sideband used as the transfer factor "
                             f"denominator (default: {SIDEBAND_LOW_POSTBPIX} for postBPix eras "
                             f"{sorted(POSTBPIX_ERAS)}, {SIDEBAND_LOW_PREBPIX} otherwise)")

    parser.add_argument("--pv_split",               action="store_true",
                        help="Apply the low/high PV cut to every region (CR, VR, SR) instead of the "
                             "CR only, so the PV systematic comes from a PV-binned prediction. "
                             f"Output goes to {OUT_BASE_PV_SPLIT}/ instead of {OUT_BASE_DEFAULT}/. "
                             "Omit to get the default CR-only PV variation.")

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


def _run_one(rdf_base, row, b_tag_combined_flag, sideband_low_override=None):
    """
    Set globals for one parameter set, book histograms, and write all output.
    rdf_base is already filtered for run exclusion and deltaPhi.
    """
    global era, era_name, DNN_cut, DNN_cut_LJDC, DNN_cut_SJDC, \
           DNN_cut_inc, DNN_cut_inc_LJDC, DNN_cut_inc_SJDC, \
           CR_cut_inc, b_tag_combined, is_mc, SB_low

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

    # Depth-score sideband lower bound: explicit override (CLI or config column)
    # wins, otherwise derived from the era group.
    if sideband_low_override is not None:
        SB_low = sideband_low_override
    elif row.get("sideband_low") is not None:
        SB_low = row["sideband_low"]
    else:
        SB_low = sideband_low_for_era(era)

    print(f"\n{'='*60}")
    print(f"Era: {era}")
    print(f"Depth DNN cut  — LJDC: {DNN_cut_LJDC},  SJDC: {DNN_cut_SJDC}")
    print(f"Inclusive cut  — LJDC: {DNN_cut_inc_LJDC},  SJDC: {DNN_cut_inc_SJDC}")
    print(f"CR inclusive upper bound (shared): {CR_cut_inc}")
    print(f"Depth sideband: [{SB_low}, DNN cut)  —  LJDC: [{SB_low}, {DNN_cut_LJDC}), "
          f"SJDC: [{SB_low}, {DNN_cut_SJDC})")
    print(f"{'='*60}")

    for leg, cut in (("LJDC", DNN_cut_LJDC), ("SJDC", DNN_cut_SJDC)):
        if cut <= SB_low:
            print(f"*** WARNING: {leg} depth cut {cut} is not above the sideband lower bound "
                  f"{SB_low} — the sideband [{SB_low}, {cut}) is EMPTY and the transfer "
                  f"factor is undefined. Lower --sideband_low or raise the depth cut.")

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
                  ", sideband from " + str(SB_low) +
                  " \n ********************* \n \n")
            if PV_SPLIT:
                # Marker read by OutputToLatex_VRclosureCheck.py to select the
                # PV-binned systematic.  Printed only in this mode, so the
                # default output is unchanged.
                print("PV treatment: split in all regions (--pv_split)\n")

            results = {}
            print("\n \n ********************* \n depth \n ********************* \n \n")
            hists = materialise(booked, "depth")
            results["depth"] = SidebandParametrization(hists, "depth")

            print("\n \n ********************* \n depth, low PV \n ********************* \n \n")
            hists = materialise(booked, "depth, low PV")
            results["depth, low PV"] = SidebandParametrization(hists, "depth, low PV")

            print("\n \n ********************* \n depth, high PV \n ********************* \n \n")
            hists = materialise(booked, "depth, high PV")
            results["depth, high PV"] = SidebandParametrization(hists, "depth, high PV")

            if PV_SPLIT:
                print_pv_split_summary(results)


def main():
    print("Parsing arguments...")
    args = parseArgs()
    configure_pv_mode(args.pv_split)
    print(f"PV treatment: {'split in all regions' if PV_SPLIT else 'CR only (default)'}"
          f"  ->  output under {OUT_BASE}/")

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
            "sideband_low":     None,
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
            _run_one(rdf_base, row, args.b_tag_combined, args.sideband_low)


if __name__ == "__main__":
    main()
