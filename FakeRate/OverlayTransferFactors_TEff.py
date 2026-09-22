#!/usr/bin/env python3
"""
Overlay the sideband-method TRANSFER FACTOR for several eras.

Companion to OverlayMistagRates_TEff.py, which overlays the mistag rate of
MisTagParametrization_3D.  This script does the same for the output of
SidebandParameterization_BkgPred.py, whose ROOT files
(default: SidebandBkgPred_PVsplit/output_3D_hists_depth_<jet>_<era>.root) hold

    hist3d_CR_pass        tagged CR     (depth score >= DNN cut)
    hist3d_CR_sideband    CR sideband   (SB_low <= depth score < DNN cut)

and the transfer factor is

    TF = N(pass) / N(sideband)

projected onto pT, eta and phi — bin by bin the same numbers as the per-era
3d_hist_projection_<region>_transfer_factor_*.png made by
SidebandParameterization_BkgPred.py, just drawn on one canvas.

Numerator and denominator are DISJOINT samples, so unlike the mistag rate this
is NOT a binomial efficiency and TEfficiency(passed, total) cannot be used
as-is.  Two error treatments are offered (--errors):

  divide  (default) symmetric propagation sigma_TF = TF * sqrt(1/N_pass + 1/N_side),
          i.e. exactly what TH1::Divide does in TransferFactorPlot() and what
          SidebandParameterization_BkgPred.py quotes for the integrated TF, so
          the overlay reproduces the per-era PNGs including their error bars.
  CP      exact conditional interval for a ratio of two independent Poisson
          counts: given N = pass + sideband, pass ~ Binomial(N, f) with
          f = TF / (1 + TF).  Clopper-Pearson bounds on f are transformed with
          TF = f / (1 - f), giving an asymmetric 68.3% interval (Cousins 1998).
          Better behaved for the low-count single-era files; falls back to
          "divide" if the histograms carry non-unit weights.

Usage (from FakeRate/):
    python3 OverlayTransferFactors_TEff.py                      # era/jet set at the top of the file
    python3 OverlayTransferFactors_TEff.py -e 2022 2023 combined
    python3 OverlayTransferFactors_TEff.py -e combined -j sub-leading
    python3 OverlayTransferFactors_TEff.py -e 2022_PU 2023_PU   # low vs high PV, from the same directory

Era groups (edit ERA_GROUPS to add more):
    2022      2022 D, E, F, G
    2023      2023 Cv1, Cv2, Cv3, Cv4, Dv1, Dv2
    combined  2022 + 2023 C (preBPix)  vs  2023 D (postBPix)
    2022_PU   2022 + 2023 C low PV vs high PV
    2023_PU   2023 D low PV vs high PV

Output: <input_dir>/outPlots_3D/Overlay_LLPskim_TransferFactorOverlayEra_<jet>_<era>.png

BLINDING: only the CR (or, for closure, the VR) histograms are ever read.  The
SR tagged cell does not exist in the data files and this script refuses to
look for it (see HistNames()).
"""

import os
import sys
import math
import array
import argparse

import ROOT
from ROOT import SetOwnership

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPalette(ROOT.kCandy)

nColors = ROOT.TColor.GetNumberOfColors()

debug = False

# ------------------------------------------------------------------------------
# Defaults — edit here (as in OverlayMistagRates_TEff.py) or override on the
# command line.
era        = "2023"                     # key of ERA_GROUPS
jet_type   = "leading"                  # "leading", "sub-leading", "combined"
region     = "CR"                       # "CR" or "VR" (SR is blinded and refused)
input_dir  = "SidebandBkgPred_PVsplit"  # or "SidebandBkgPred"
error_mode = "divide"                   # "divide" or "CP", see module docstring

CL_1SIGMA = 0.682689492137              # 68.27% central interval

# Each entry: (file stem, era name in the file name, legend label).
# File = <input_dir>/output_3D_hists_<stem>_<jet_type>_<era name>.root
ERA_GROUPS = {
    "2022": [
        ("depth", "2022_D", "2022 D"),
        ("depth", "2022_E", "2022 E"),
        ("depth", "2022_F", "2022 F"),
        ("depth", "2022_G", "2022 G"),
    ],
    "2023": [
        ("depth", "2023_Cv1", "2023 Cv1"),
        ("depth", "2023_Cv2", "2023 Cv2"),
        ("depth", "2023_Cv3", "2023 Cv3"),
        ("depth", "2023_Cv4", "2023 Cv4"),
        ("depth", "2023_Dv1", "2023 Dv1"),
        ("depth", "2023_Dv2", "2023 Dv2"),
    ],
    "combined": [
        ("depth", "2022_23_preBPix", "2022 + 2023 C"),
        ("depth", "2023_postBPix",   "2023 D"),
    ],
    "2022_PU": [
        ("depth_lowPV",  "2022_23_preBPix", "2022+23 C low PU"),
        ("depth_highPV", "2022_23_preBPix", "2022+23 C high PU"),
    ],
    "2023_PU": [
        ("depth_lowPV",  "2023_postBPix", "2023 D low PU"),
        ("depth_highPV", "2023_postBPix", "2023 D high PU"),
    ],
}

# Depth-score working points and sideband lower bound used by
# SidebandParameterization_BkgPred.py, for the plot header only.
POSTBPIX_ERAS = {"2023_postBPix", "2023_Dv1", "2023_Dv2"}
DEPTH_CUT = {
    "preBPix":  {"leading": 0.905, "sub-leading": 0.925},
    "postBPix": {"leading": 0.985, "sub-leading": 0.995},
}
SB_LOW = {"preBPix": 0.3, "postBPix": 0.8}

# ------------------------------------------------------------------------------
def HistNames(region):
    """Histogram names for the TF numerator and denominator.

    Blinding tripwire: the SR tagged cell is never read from data.  The data
    files do not even contain hist3d_SR_pass, but refuse explicitly so that
    nobody can point this script at an MC file and then forget.
    """
    if region not in ("CR", "VR"):
        raise RuntimeError(
            f"BLINDED: region '{region}' is not allowed. The transfer factor "
            "overlay reads only the CR or VR; the SR tagged cell is blinded.")
    return f"hist3d_{region}_pass", f"hist3d_{region}_sideband"

# ------------------------------------------------------------------------------
def ResetAxis(hist):
    """Axis titles after cloning (mirrors ResetAxis in the parametrisation
    scripts, but labels |eta| when the eta axis starts at zero)."""
    if isinstance(hist, ROOT.TH3):
        eta_label = "|#eta|" if hist.GetYaxis().GetXmin() >= 0.0 else "#eta"
        hist.GetXaxis().SetTitle("p_{T} (GeV)")
        hist.GetYaxis().SetTitle(eta_label)
        hist.GetZaxis().SetTitle("#phi")

def ProjectHistogram(hist, y_label=""):
    if not isinstance(hist, ROOT.TH3):
        raise TypeError("Unsupported histogram type: expected TH3.")
    proj_pT  = hist.Project3D("x")
    proj_eta = hist.Project3D("y")
    proj_phi = hist.Project3D("z")
    for proj in (proj_pT, proj_eta, proj_phi):
        proj.SetYTitle(y_label)
    return proj_pT, proj_eta, proj_phi

# ------------------------------------------------------------------------------
def IsWeighted(hist):
    """True if any bin has sumw2 != content, i.e. fill weights were not all 1.
    (RDataFrame always enables Sumw2, so GetSumw2N() > 0 alone says nothing.)"""
    if hist.GetSumw2N() == 0:
        return False
    for b in range(1, hist.GetNbinsX() + 1):
        c  = hist.GetBinContent(b)
        e2 = hist.GetBinError(b) ** 2
        if abs(e2 - c) > 1e-6 * max(1.0, abs(c)):
            return True
    return False

def TransferFactorPoint(n_pass, e_pass, n_side, e_side, use_cp):
    """TF = n_pass / n_side with (low, high) 1-sigma errors.  n_side > 0."""
    tf = n_pass / n_side
    if use_cp:
        # Conditional on N = pass + side, pass ~ Binomial(N, f), TF = f/(1-f).
        N = int(round(n_pass + n_side))
        k = int(round(n_pass))
        f_lo = ROOT.TEfficiency.ClopperPearson(N, k, CL_1SIGMA, False)
        f_hi = ROOT.TEfficiency.ClopperPearson(N, k, CL_1SIGMA, True)
        tf_lo = f_lo / (1.0 - f_lo)
        # f_hi == 1 only when k == N, i.e. n_side == 0, which the caller skips.
        tf_hi = f_hi / (1.0 - f_hi) if f_hi < 1.0 else float("inf")
        return tf, max(tf - tf_lo, 0.0), max(tf_hi - tf, 0.0)
    # Uncorrelated propagation, as TH1::Divide (numerator and denominator
    # are disjoint samples, so no binomial correlation term).
    rel2 = (e_side / n_side) ** 2
    if n_pass > 0:
        rel2 += (e_pass / n_pass) ** 2
    err = tf * math.sqrt(rel2)
    return tf, err, err

def MakeTransferFactorGraph(pass_hist, side_hist, name, mode):
    """
    Build a TGraphAsymmErrors of TF = pass / sideband, bin by bin.

    Drawn with the TGraph painter (as in OverlayMistagRates_TEff.py) rather
    than as a divided TH1, so asymmetric errors and empty-bin skipping work.
    Bins with an empty sideband are skipped: the TF is undefined there.
    """
    use_cp = (mode == "CP")
    if use_cp and (IsWeighted(pass_hist) or IsWeighted(side_hist)):
        print(f"  {name}: weighted histograms, falling back to 'divide' errors")
        use_cp = False

    n_bins = side_hist.GetNbinsX()
    xs, ys, exl, exh, eyl, eyh = [], [], [], [], [], []

    for b in range(1, n_bins + 1):
        n_side = side_hist.GetBinContent(b)
        n_pass = pass_hist.GetBinContent(b)
        if n_side <= 0:
            if n_pass > 0:
                print(f"  WARNING {name}: bin {b} has {n_pass:g} tagged but an "
                      "empty sideband; TF undefined, point skipped")
            continue
        tf, lo, hi = TransferFactorPoint(n_pass, pass_hist.GetBinError(b),
                                         n_side, side_hist.GetBinError(b), use_cp)
        xs.append(side_hist.GetBinCenter(b))
        ys.append(tf)
        half_w = side_hist.GetBinWidth(b) / 2.0
        exl.append(half_w)
        exh.append(half_w)
        eyl.append(lo)
        eyh.append(hi)

    graph = ROOT.TGraphAsymmErrors(
        len(xs),
        array.array('d', xs),  array.array('d', ys),
        array.array('d', exl), array.array('d', exh),
        array.array('d', eyl), array.array('d', eyh),
    )
    graph.SetName(name)

    # Full x-range from the histogram so the frame uses the declared axis
    # bounds rather than the range of non-empty bins.
    graph.x_min   = side_hist.GetXaxis().GetXmin()
    graph.x_max   = side_hist.GetXaxis().GetXmax()
    graph.x_title = side_hist.GetXaxis().GetTitle()

    SetOwnership(graph, False)
    return graph

# ------------------------------------------------------------------------------
def GetGraphs(file_path, hist_name_pass, hist_name_side, mode):
    root_file = ROOT.TFile(file_path, "READ")
    if not root_file or root_file.IsZombie():
        raise IOError(f"Could not open {file_path}")

    pass_hist = root_file.Get(hist_name_pass)
    side_hist = root_file.Get(hist_name_side)
    if not pass_hist or not side_hist:
        raise ValueError(f"Histograms {hist_name_pass} / {hist_name_side} "
                         f"not found in file: {file_path}")

    tag = os.path.basename(file_path).replace(".root", "")

    pass_clone = pass_hist.Clone("pass_" + tag)
    ResetAxis(pass_clone)
    SetOwnership(pass_clone, False)
    proj_pT_pass, proj_eta_pass, proj_phi_pass = ProjectHistogram(pass_clone, "Tagged")

    side_clone = side_hist.Clone("side_" + tag)
    ResetAxis(side_clone)
    SetOwnership(side_clone, False)
    proj_pT_side, proj_eta_side, proj_phi_side = ProjectHistogram(side_clone, "Sideband")

    graph_pT  = MakeTransferFactorGraph(proj_pT_pass,  proj_pT_side,  f"tf_pT_{tag}",  mode)
    graph_eta = MakeTransferFactorGraph(proj_eta_pass, proj_eta_side, f"tf_eta_{tag}", mode)
    graph_phi = MakeTransferFactorGraph(proj_phi_pass, proj_phi_side, f"tf_phi_{tag}", mode)

    if debug:
        print(f"Graph name : {graph_pT.GetName()}, points: {graph_pT.GetN()}")
        for i in range(graph_pT.GetN()):
            print(f"  pt={graph_pT.GetPointX(i):.1f}  "
                  f"TF={graph_pT.GetPointY(i):.4f} "
                  f"+{graph_pT.GetErrorYhigh(i):.4f} "
                  f"-{graph_pT.GetErrorYlow(i):.4f}")

    return graph_pT, graph_eta, graph_phi, root_file

# ------------------------------------------------------------------------------
def CutLabelLines(era_names, jet):
    """Header lines describing the depth working point(s) and sideband."""
    periods = []
    for e in era_names:
        p = "postBPix" if e in POSTBPIX_ERAS else "preBPix"
        if p not in periods:
            periods.append(p)

    def cut_str(p):
        if jet == "combined":
            return f"{DEPTH_CUT[p]['leading']}/{DEPTH_CUT[p]['sub-leading']} (LJDC/SJDC)"
        return str(DEPTH_CUT[p][jet])

    if len(periods) == 1:
        p = periods[0]
        return [f"DNN score > {cut_str(p)}, SB #geq {SB_LOW[p]}"]
    return [f"{p}: DNN > {cut_str(p)}, SB #geq {SB_LOW[p]}" for p in periods]

def LabelCMS(era_key, era_names, jet, xpos=0.17, ypos=0.85, text_size=0.036):
    cmsLabel      = "#scale[1]{#bf{CMS} }"
    cmsLabelExtra = "#scale[0.8]{#it{Private Work}}"
    yearLumi = "#scale[0.85]{2023 (13.6 TeV)}"
    if era_key in ("combined", "2022_PU"):
        yearLumi = "#scale[0.85]{2022-2023 (13.6 TeV)}"
    elif "2022" in era_key:
        yearLumi = "#scale[0.85]{2022 (13.6 TeV)}"

    stamp_text = ROOT.TLatex()
    stamp_text.SetNDC()
    stamp_text.SetTextFont(42)
    stamp_text.SetTextSize(text_size)
    stamp_text.DrawLatex(xpos, ypos, cmsLabel)
    stamp_text.DrawLatex(xpos + 0.07, ypos, cmsLabelExtra)
    stamp_text.DrawLatex(xpos + 0.62, ypos + 0.06, yearLumi)

    # Cut / era block on the RIGHT of the header row; the legend sits in the
    # top-LEFT under the CMS label, so the two never collide.  One line per
    # BPix period when the group mixes preBPix and postBPix eras.
    lines = CutLabelLines(era_names, jet) + [f"Era = {era_key}"]
    x_block = xpos + 0.42 if len(lines) <= 2 else xpos + 0.36
    for i, line in enumerate(lines):
        stamp_text.DrawLatex(x_block, ypos - 0.04 * i, "#scale[0.65]{" + line + "}")

# ------------------------------------------------------------------------------
def MakePlot_overlay(graphs, legends, title, era_key, era_names, jet, region):
    """Draw a list of TGraphAsymmErrors overlaid on the current pad."""
    nGraphs = len(graphs)

    for i, g in enumerate(graphs):
        color_index = int(i / nGraphs * nColors)
        color = ROOT.TColor.GetColorPalette(color_index)
        g.SetLineColor(color)
        g.SetMarkerColor(color)
        g.SetMarkerStyle(20)
        g.SetLineWidth(2)

    # y-axis range across all graphs.  A transfer factor is not bounded by 1,
    # so (unlike the mistag rate) there is no cap.
    y_vals = [
        graphs[i].GetPointY(j) + graphs[i].GetErrorYhigh(j)
        for i in range(nGraphs)
        for j in range(graphs[i].GetN())
        if math.isfinite(graphs[i].GetErrorYhigh(j))
    ]
    y_max = max(y_vals) if y_vals else 1.0
    y_top = y_max * 1.25

    x_min   = graphs[0].x_min
    x_max   = graphs[0].x_max
    x_title = graphs[0].x_title

    # Legend geometry: height follows the entry count, width the longest label
    # (same as the mistag overlay).  Default position is top-LEFT under the CMS
    # label.  Transfer factors can be large at low pT, where a left legend would
    # sit on top of the markers, so if the left box would cover a marker or its
    # vertical error bar, try the top-RIGHT under the cut/era header instead,
    # and only if that is worse raise the y-range to make room.
    leg_h   = 0.060 * len(graphs) + 0.02
    max_lab = max(len(str(legends[i])) for i in range(len(graphs)))
    leg_w   = min(0.30, max(0.16, 0.045 + 0.0125 * max_lab))
    n_header = len(CutLabelLines(era_names, jet)) + 1          # + "Era = ..." line
    candidates = [                                              # (x1, y2)
        (0.18, 0.80),
        (0.94 - leg_w, 0.85 - 0.04 * n_header),
    ]

    def y_needed(x1, y2):
        """Smallest frame top such that nothing under the legend reaches its bottom.
        Pad margins: left 0.15, right 0.05 (set by the caller), top/bottom 0.10."""
        y1 = max(y2 - leg_h, 0.20)
        frac = (y1 - 0.02 - 0.10) / 0.80        # frame fraction below the legend
        need = 0.0
        for g in graphs:
            for j in range(g.GetN()):
                x_ndc = 0.15 + (g.GetPointX(j) - x_min) / (x_max - x_min) * 0.80
                if x1 <= x_ndc <= x1 + leg_w and math.isfinite(g.GetErrorYhigh(j)):
                    need = max(need, g.GetPointY(j) + g.GetErrorYhigh(j))
        return need / frac if frac > 0 else float("inf")

    tops = [max(y_top, y_needed(x1, y2)) for x1, y2 in candidates]
    best = 0 if tops[0] <= tops[1] else 1     # prefer left on a tie
    leg_x1, leg_y2 = candidates[best]
    leg_y1 = max(leg_y2 - leg_h, 0.20)
    y_top  = tops[best]

    frame = ROOT.gPad.DrawFrame(x_min, 0, x_max, y_top)
    frame.GetXaxis().SetTitle(x_title)
    frame.GetYaxis().SetTitle(f"Transfer factor ({region} tagged / {region} sideband)")
    frame.SetTitle(title)
    SetOwnership(frame, False)

    for g in graphs:
        g.Draw("SAME P")

    ROOT.gPad.Update()

    legend = ROOT.TLegend(leg_x1, leg_y1, leg_x1 + leg_w, leg_y2)
    legend.SetBorderSize(1)
    legend.SetFillColor(0)
    legend.SetFillStyle(1001)
    legend.SetTextSize(0.030)
    for i, g in enumerate(graphs):
        legend.AddEntry(g, legends[i], "lp")
    legend.Draw()
    SetOwnership(legend, 0)

    LabelCMS(era_key, era_names, jet)

# ------------------------------------------------------------------------------
def DrawCanvasAndPlots_overlay(canvas_name, canvas_title, option, title,
                                plots, legend_labels, save_path, plot_titles,
                                era_key, era_names, jet, region):
    canvas = ROOT.TCanvas(f"{canvas_name}_{option}", f"{canvas_title} for {option}", 2400, 600)
    canvas.Divide(3, 1)

    for i, plot_group in enumerate(plots):
        pad = canvas.cd(i + 1)
        pad.SetLeftMargin(0.15)
        pad.SetRightMargin(0.05)
        MakePlot_overlay(plot_group, legend_labels, plot_titles[i] + title,
                         era_key, era_names, jet, region)

    canvas.Update()
    canvas.Draw()
    canvas.SaveAs(save_path)
    print("saved canvas as " + save_path)
    canvas.Clear()

# ------------------------------------------------------------------------------
def OverlayTransferFactors(era_key, jet, region, in_dir, mode):
    if era_key not in ERA_GROUPS:
        raise KeyError(f"Unknown era group '{era_key}'; choose from {sorted(ERA_GROUPS)}")

    hist_name_pass, hist_name_side = HistNames(region)

    entries = ERA_GROUPS[era_key]
    file_paths = [os.path.join(in_dir, f"output_3D_hists_{stem}_{jet}_{era_name}.root")
                  for stem, era_name, _ in entries]
    legend_labels = [lab for _, _, lab in entries]
    era_names     = [era_name for _, era_name, _ in entries]

    missing = [p for p in file_paths if not os.path.exists(p)]
    if missing:
        raise FileNotFoundError("Missing input file(s):\n  " + "\n  ".join(missing))

    pt_graphs, eta_graphs, phi_graphs, root_files = [], [], [], []
    for file_path in file_paths:
        g_pT, g_eta, g_phi, rf = GetGraphs(file_path, hist_name_pass, hist_name_side, mode)
        print("Got transfer factor graph for: " + file_path)
        pt_graphs.append(g_pT)
        eta_graphs.append(g_eta)
        phi_graphs.append(g_phi)
        root_files.append(rf)

    what = "pileup regions" if "PU" in era_key else "eras"
    title = f" in {region} ({jet}) for different {what}"

    out_dir = os.path.join(in_dir, "outPlots_3D")
    os.makedirs(out_dir, exist_ok=True)
    save_path = os.path.join(
        out_dir, f"Overlay_LLPskim_TransferFactorOverlayEra_{jet}_{era_key}.png")

    DrawCanvasAndPlots_overlay(
        "c1", "Projection plots", f"{era_key}_{jet}", title,
        [pt_graphs, eta_graphs, phi_graphs],
        legend_labels,
        save_path,
        ["Jet p_{T} Transfer Factor", "Jet #eta Transfer Factor", "Jet #phi Transfer Factor"],
        era_key, era_names, jet, region
    )

    for rf in root_files:
        rf.Close()

# ------------------------------------------------------------------------------
def parseArgs():
    parser = argparse.ArgumentParser(
        description="Overlay sideband transfer factors (tagged / sideband) for several eras.")
    parser.add_argument("-e", "--era", nargs="+", default=[era], choices=sorted(ERA_GROUPS),
                        help=f"era group(s) to overlay (default: {era})")
    parser.add_argument("-j", "--jet", default=jet_type,
                        choices=["leading", "sub-leading", "combined"],
                        help=f"which depth-tag jet files to read (default: {jet_type})")
    parser.add_argument("-r", "--region", default=region, choices=["CR", "VR"],
                        help=f"region whose TF is plotted (default: {region}); SR is blinded")
    parser.add_argument("-i", "--input_dir", default=input_dir,
                        help=f"directory with output_3D_hists_*.root (default: {input_dir})")
    parser.add_argument("--errors", default=error_mode, choices=["divide", "CP"],
                        help=f"error treatment, see module docstring (default: {error_mode})")
    return parser.parse_args()

if __name__ == "__main__":
    args = parseArgs()
    for era_key in args.era:
        OverlayTransferFactors(era_key, args.jet, args.region, args.input_dir, args.errors)
