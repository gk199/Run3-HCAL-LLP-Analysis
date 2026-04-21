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

# ---- Histogram bin definitions (identical to original) ----
pT_bins  = np.array([0, 40, 50, 60, 70, 80, 100, 120, 160, 240, 400], dtype=float)
eta_bins = np.linspace(-1.26, 1.26, 9)
phi_bins = np.linspace(-np.pi, np.pi, 9)
b_tag_bins = np.array([0, 0.2435, 1.0], dtype=float)
c_tag_bins = np.array([0, 0.102,  1.0], dtype=float)

DNN_cut_inc = 0.97

CR_cut_inc = 0.2

# ---- Run exclusion lists (identical to original) ----
runs_to_exclude_2022 = [362615, 362653, 360485]   # based on CR 0.1 
runs_to_exclude_2022 = [362615, 362617, 362653, 362655]   # based on CR 0.2
    # 357776, 359661, 359690, 359869, 360890, 361044, 362087, 362437,
    # 362615, 362616, 362653, 360128, 360887, 360949, 361053, 361994,
    # 362063, 362698,
# runs_to_exclude_2022.append(362596)
# runs_to_exclude_2022.extend(range(362597, 362697))  # 100 runs from L1 LUT issue
runs_to_exclude_2022 = list(set(runs_to_exclude_2022))  # deduplicate

runs_to_exclude_2023 = [] # based on CR 0.1 # list(set([367228, 368684, 370460, 368684, 370460]))
runs_to_exclude_2023 = [367691, 368684] # based on CR 0.2

Zmu     = False
LLPskim = True
CNN     = False

current_jet_type = "leading"  # updated per-iteration in MisTagParametrization

output_dir = "outPlots_3D"
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
    hists[0].SetMaximum(max(h.GetMaximum() for h in hists) * 1.1)
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
        stamp_text.DrawLatex(xpos + 0.62, ypos + 0.06, yearLumi)
        _dnn_label = DNN_cut_SJDC if current_jet_type == "sub-leading" else DNN_cut_LJDC
        stamp_text.DrawLatex(xpos + 0.4,  ypos,        "#scale[0.65]{DNN score > " + str(_dnn_label) + "}")
        stamp_text.DrawLatex(xpos + 0.4,  ypos - 0.04, "#scale[0.65]{Era = " + era + "}")
    else:
        _dnn_label = DNN_cut_SJDC if current_jet_type == "sub-leading" else DNN_cut_LJDC
        if "2022_2023" in era:
            stamp_text.DrawLatex(xpos + 0.5,  ypos + 0.03, yearLumi)
        else: stamp_text.DrawLatex(xpos + 0.6,  ypos + 0.03, yearLumi)
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


def book_all_histograms(rdf_base):
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
    # LJDC: jet0 is the depth tag candidate; jet1 inclusive score defines CR/VR/SR
    CR_str     = f"jet1_scores_inc_train80 >= 0.0 && jet1_scores_inc_train80 < {CR_cut_inc}"
    VR_str     = f"jet1_scores_inc_train80 >= {CR_cut_inc} && jet1_scores_inc_train80 < {DNN_cut_inc_LJDC}"
    SR_str     = f"jet1_scores_inc_train80 >= {DNN_cut_inc_LJDC} && jet1_scores_inc_train80 < 1.1"
    mistag_str = f"jet0_scores_depth_LLPanywhere >= {DNN_cut_LJDC} && jet0_scores_depth_LLPanywhere < 1.1"

    # SJDC: jet1 is the depth tag candidate; jet0 inclusive score defines CR/VR/SR
    CR_0_str     = f"jet0_scores_inc_train80 >= 0.0 && jet0_scores_inc_train80 < {CR_cut_inc}"
    VR_0_str     = f"jet0_scores_inc_train80 >= {CR_cut_inc} && jet0_scores_inc_train80 < {DNN_cut_inc_SJDC}"
    SR_0_str     = f"jet0_scores_inc_train80 >= {DNN_cut_inc_SJDC} && jet0_scores_inc_train80 < 1.1"
    mistag_1_str = f"jet1_scores_depth_LLPanywhere >= {DNN_cut_SJDC} && jet1_scores_depth_LLPanywhere < 1.1"

    # Emulated trigger: leading jet has depth tag candidate + subleading has inclusive tag candidate
    depth_j0_str = "jet0_DepthTagCand == 1 && jet1_InclTagCand == 1"
    depth_j1_str = "jet1_DepthTagCand == 1 && jet0_InclTagCand == 1"

    low_PV_str  = "PV >= 0  && PV < 42"
    high_PV_str = "PV >= 42 && PV < 100"

    options = ["depth", "depth, low PV", "depth, high PV"]
    booked  = {}

    for option in options:
        s = option.replace(", ", "_").replace(" ", "_")  # safe name fragment

        # PV cut is applied only to the CR (mistag rate measurement region),
        # not to VR or SR — matches original MisTagParametrization logic exactly.
        cr_extra = ""
        if "low PV"  in option: cr_extra = " && " + low_PV_str
        if "high PV" in option: cr_extra = " && " + high_PV_str

        # --- jet0 triggered (jet0 has depth tag, jet1 defines CR/VR/SR) ---
        rdf_j0    = rdf_base.Filter(depth_j0_str,       f"depth_j0_{s}")
        rdf_CR_j0 = rdf_j0.Filter(CR_str + cr_extra,    f"CR_j0_{s}")
        rdf_VR_j0 = rdf_j0.Filter(VR_str,               f"VR_j0_{s}")
        rdf_SR_j0 = rdf_j0.Filter(SR_str,               f"SR_j0_{s}")

        booked[option] = {
            "CR_all":    rdf_CR_j0.Histo3D(_h3_model(f"h_CR_all_{s}"),    "jet0_Pt", "jet0_Eta", "jet0_Phi"),
            "CR_mistag": rdf_CR_j0.Filter(mistag_str).Histo3D(
                             _h3_model(f"h_CR_mistag_{s}"),               "jet0_Pt", "jet0_Eta", "jet0_Phi"),
            "VR_all":    rdf_VR_j0.Histo3D(_h3_model(f"h_VR_all_{s}"),    "jet0_Pt", "jet0_Eta", "jet0_Phi"),
            "VR_mistag": rdf_VR_j0.Filter(mistag_str).Histo3D(
                             _h3_model(f"h_VR_mistag_{s}"),               "jet0_Pt", "jet0_Eta", "jet0_Phi"),
            "SR_all":    rdf_SR_j0.Histo3D(_h3_model(f"h_SR_all_{s}"),    "jet0_Pt", "jet0_Eta", "jet0_Phi"),
        }

        # --- jet1 triggered (jet1 has depth tag, jet0 defines CR/VR/SR) ---
        rdf_j1    = rdf_base.Filter(depth_j1_str,        f"depth_j1_{s}")
        rdf_CR_j1 = rdf_j1.Filter(CR_0_str + cr_extra,  f"CR_j1_{s}")
        rdf_VR_j1 = rdf_j1.Filter(VR_0_str,             f"VR_j1_{s}")
        rdf_SR_j1 = rdf_j1.Filter(SR_0_str,             f"SR_j1_{s}")

        booked[option].update({
            "CR_all_1":    rdf_CR_j1.Histo3D(_h3_model(f"h_CR_all_1_{s}"),    "jet1_Pt", "jet1_Eta", "jet1_Phi"),
            "CR_mistag_1": rdf_CR_j1.Filter(mistag_1_str).Histo3D(
                               _h3_model(f"h_CR_mistag_1_{s}"),               "jet1_Pt", "jet1_Eta", "jet1_Phi"),
            "VR_all_1":    rdf_VR_j1.Histo3D(_h3_model(f"h_VR_all_1_{s}"),    "jet1_Pt", "jet1_Eta", "jet1_Phi"),
            "VR_mistag_1": rdf_VR_j1.Filter(mistag_1_str).Histo3D(
                               _h3_model(f"h_VR_mistag_1_{s}"),               "jet1_Pt", "jet1_Eta", "jet1_Phi"),
            "SR_all_1":    rdf_SR_j1.Histo3D(_h3_model(f"h_SR_all_1_{s}"),    "jet1_Pt", "jet1_Eta", "jet1_Phi"),
        })

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
        return out

    key_to_name = {
        "CR_all":    "hist3d_CR_all",
        "CR_mistag": "hist3d_CR_mistag",
        "VR_all":    "hist3d_VR_all",
        "VR_mistag": "hist3d_VR_mistag",
        "SR_all":    "hist3d_SR_all",
        "CR_all_1":    "hist3d_CR_all_1",
        "CR_mistag_1": "hist3d_CR_mistag_1",
        "VR_all_1":    "hist3d_VR_all_1",
        "VR_mistag_1": "hist3d_VR_mistag_1",
        "SR_all_1":    "hist3d_SR_all_1",
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

    print("created histograms for 1D rate evaluation")
    print("completed 1D rate evaluation")

    CR_all_list    = [CR_all,    CR_all_1,    CR_all_combined]
    CR_mistag_list = [CR_mistag, CR_mistag_1, CR_mistag_combined]
    VR_all_list    = [VR_all,    VR_all_1,    VR_all_combined]
    VR_mistag_list = [VR_mistag, VR_mistag_1, VR_mistag_combined]
    SR_all_list    = [SR_all,    SR_all_1,    SR_all_combined]
    mistag_jet_list = ["leading", "sub-leading", "combined"]

    for i, (CR_all_i, CR_mistag_i, VR_all_i, VR_mistag_i, SR_all_i) in enumerate(
            zip(CR_all_list, CR_mistag_list, VR_all_list, VR_mistag_list, SR_all_list)):

        global current_jet_type
        current_jet_type = mistag_jet_list[i]

        print(" ************* \n " + mistag_jet_list[i] + " \n *************")

        # Write base histograms to ROOT file using canonical names
        outfile_name = "output_3D_hists" + label + "_" + mistag_jet_list[i] + "_" + era_name + ".root"
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
        print(f"Predicted mistagged events in SR: {total_SR:.2f} \u00b1 {err_SR:.2f} (stat)")

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

    # --- Always required ---
    parser.add_argument("-b", "--b_tag_combined",  action="store_true",
                        help="combined b-tag categories")
    parser.add_argument("-t", "--threads",         action="store", default=0, type=int,
                        help="Number of threads for RDataFrame (0 = auto-detect)")
    return parser.parse_args()


# Input file lists
_file_map = {
    "2022_2023": [
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2022Dv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2022Ev1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2022Fv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2022Gv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Cv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Cv2_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Cv3_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Cv4_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Dv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Dv2_scores.root",
    ],
    "2022": [
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2022Dv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2022Ev1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2022Fv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2022Gv1_scores.root",
    ],
    "2022_D": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2022Dv1_scores.root"],
    "2022_E": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2022Ev1_scores.root"],
    "2022_F": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2022Fv1_scores.root"],
    "2022_G": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2022Gv1_scores.root"],

    "2023": [
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Cv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Cv2_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Cv3_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Cv4_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Dv1_scores.root",
        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Dv2_scores.root",
    ],
    "2023_Cv1": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Cv1_scores.root"],
    "2023_Cv2": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Cv2_scores.root"],
    "2023_Cv3": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Cv3_scores.root"],
    "2023_Cv4": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Cv4_scores.root"],
    "2023_Dv1": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Dv1_scores.root"],
    "2023_Dv2": ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_data_2023Dv2_scores.root"],
}


def _build_rdf_base(era_key):
    """Build the base RDataFrame (run exclusion + deltaPhi) for a given era."""
    rdf = ROOT.RDataFrame("NoSel", _file_map[era_key])
    if era_key == "2022_2023":
        run_excl = "g_excl_runs_2022_2023.find(run) == g_excl_runs_2022_2023.end()"
    elif era_key.startswith("2023"):
        run_excl = "g_excl_runs_2023.find(run) == g_excl_runs_2023.end()"
    else:
        run_excl = "g_excl_runs_2022.find(run) == g_excl_runs_2022.end()"
    return (rdf
            .Filter(run_excl, "run exclusion")
            .Filter("abs(jet0_jet1_dPhi) > 0.2 && Flag_METFilters_2022_2023_PromptReco == 1",
                    "deltaPhi + METFilters"))


def _run_one(rdf_base, row, b_tag_combined_flag):
    """
    Set globals for one parameter set, book histograms, and write all output.
    rdf_base is already filtered for run exclusion and deltaPhi.
    """
    global era, era_name, DNN_cut, DNN_cut_LJDC, DNN_cut_SJDC, \
           DNN_cut_inc, DNN_cut_inc_LJDC, DNN_cut_inc_SJDC, \
           CR_cut_inc, b_tag_combined

    era              = row["era"]
    era_name         = era.replace(" ", "")
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
    booked = book_all_histograms(rdf_base)

    def _pt(v): return str(v).replace("0.", "pt")
    btag_str = "_combined" if b_tag_combined else ""
    if DNN_cut_LJDC == DNN_cut_SJDC and DNN_cut_inc_LJDC == DNN_cut_inc_SJDC:
        output_filename = f"DNN_{_pt(DNN_cut_LJDC)}_inc{_pt(DNN_cut_inc_LJDC)}_{era}_forPython{btag_str}.txt"
    else:
        output_filename = (f"DNN_LJDC{_pt(DNN_cut_LJDC)}_SJDC{_pt(DNN_cut_SJDC)}"
                           f"_incLJDC{_pt(DNN_cut_inc_LJDC)}_incSJDC{_pt(DNN_cut_inc_SJDC)}"
                           f"_{era}_forPython{btag_str}.txt")
    print(f"Writing numeric output to: {output_filename}")

    with open(output_filename, "w") as f:
        with redirect_stdout(f):
            print("\n \n ********************* \n DNN score = " + str(DNN_cut) +
                  " \n ********************* \n \n")

            print("\n \n ********************* \n depth \n ********************* \n \n")
            hists = materialise(booked, "depth")
            MisTagParametrization(hists, "depth")

            print("\n \n ********************* \n depth, low PV \n ********************* \n \n")
            hists = materialise(booked, "depth, low PV")
            MisTagParametrization(hists, "depth, low PV")

            print("\n \n ********************* \n depth, high PV \n ********************* \n \n")
            hists = materialise(booked, "depth, high PV")
            MisTagParametrization(hists, "depth, high PV")


def main():
    print("Parsing arguments...")
    args = parseArgs()

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
