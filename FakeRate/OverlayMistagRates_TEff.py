import ROOT
import array

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPalette(ROOT.kCandy)

nColors = ROOT.TColor.GetNumberOfColors()

from ROOT import SetOwnership
from MisTagParametrization_3D import ProjectHistogram, ResetAxis

debug = False
era = "2023"
DNN_cut = 0.965 # for LJDC depth cut

# ------------------------------------------------------------------------------
def LabelCMS(xpos=0.17, ypos=0.85, text_size=0.036):
    cmsLabel = "#scale[1]{#bf{CMS} }"
    cmsLabelExtra = "#scale[0.8]{#it{Private Work}}"
    yearLumi = "#scale[0.85]{2023 (13.6 TeV)}"
    if era == "2022": yearLumi = "#scale[0.85]{2022 (13.6 TeV)}"

    stamp_text = ROOT.TLatex()
    stamp_text.SetNDC()
    stamp_text.SetTextFont(42)
    stamp_text.SetTextSize(text_size)
    stamp_text.DrawLatex(xpos, ypos, cmsLabel)
    stamp_text.DrawLatex(xpos + 0.07, ypos, cmsLabelExtra)

    if ypos == 0.85:
        stamp_text.DrawLatex(xpos + 0.62, ypos + 0.06, yearLumi)
        stamp_text.DrawLatex(xpos + 0.35,  ypos,        "#scale[0.65]{DNN score > " + str(DNN_cut) + "}")
        stamp_text.DrawLatex(xpos + 0.35,  ypos - 0.04, "#scale[0.65]{Era = " + era + "}")
    else:
        stamp_text.DrawLatex(xpos + 0.6,  ypos + 0.03, yearLumi)
        stamp_text.DrawLatex(xpos + 0.3,  ypos,        "#scale[0.65]{DNN score > " + str(DNN_cut) + "}")
        stamp_text.DrawLatex(xpos + 0.3,  ypos - 0.04, "#scale[0.65]{Era = " + era + "}")

# ------------------------------------------------------------------------------
def MakeTEfficiency(passed_hist, total_hist, name):
    """
    Build a TEfficiency and immediately extract it as a TGraphAsymmErrors so
    that drawing is handled by the reliable TGraph painter rather than the
    TEfficiency one (which misbehaves when drawn with SAME P onto a bare frame).

    Stat option:
      - kFCP  (Clopper-Pearson) for unweighted integer counts  — CMS standard
      - kFNormal (normal approx) for weighted events            — only option ROOT supports
    """
    eff = ROOT.TEfficiency(passed_hist, total_hist)
    eff.SetName(name + "_eff")

    is_weighted = (passed_hist.GetSumw2N() > 0 or total_hist.GetSumw2N() > 0)
    if is_weighted:
        eff.SetUseWeightedEvents()
        eff.SetStatisticOption(ROOT.TEfficiency.kFNormal)   # only valid option for weights
    else:
        eff.SetStatisticOption(ROOT.TEfficiency.kFCP)       # Clopper-Pearson for counts

    # --- convert to TGraphAsymmErrors ---
    # Build point-by-point, skipping empty bins so sparse high-pT tails don't
    # distort the x-axis or produce spurious zero-efficiency points.
    n_bins = total_hist.GetNbinsX()
    xs, ys, exl, exh, eyl, eyh = [], [], [], [], [], []

    for b in range(1, n_bins + 1):
        if total_hist.GetBinContent(b) == 0:
            continue
        xs.append(total_hist.GetBinCenter(b))
        ys.append(eff.GetEfficiency(b))
        half_w = total_hist.GetBinWidth(b) / 2.0
        exl.append(half_w)
        exh.append(half_w)
        eyl.append(eff.GetEfficiencyErrorLow(b))
        eyh.append(eff.GetEfficiencyErrorUp(b))

    n_pts = len(xs)
    graph = ROOT.TGraphAsymmErrors(
        n_pts,
        array.array('d', xs),  array.array('d', ys),
        array.array('d', exl), array.array('d', exh),
        array.array('d', eyl), array.array('d', eyh),
    )
    graph.SetName(name)

    # Store the full x-range from the total histogram so the frame can use it
    graph.x_min = total_hist.GetXaxis().GetXmin()
    graph.x_max = total_hist.GetXaxis().GetXmax()
    graph.x_title = total_hist.GetXaxis().GetTitle()

    SetOwnership(graph, False)
    return graph

# ------------------------------------------------------------------------------
def GetHistogram(file_path, hist_name_all, hist_name_mistag):
    root_file = ROOT.TFile(file_path, "READ")

    all_hist    = root_file.Get(hist_name_all)
    mistag_hist = root_file.Get(hist_name_mistag)

    if not all_hist or not mistag_hist:
        raise ValueError(f"Histograms not found in file: {file_path}")

    mistag_clone = mistag_hist.Clone("mistag_" + file_path)
    ResetAxis(mistag_clone)
    SetOwnership(mistag_clone, False)
    proj_pT_mistag, proj_eta_mistag, proj_phi_mistag = ProjectHistogram(mistag_clone, "Mistag")

    total_clone = all_hist.Clone("total_" + file_path)
    SetOwnership(total_clone, False)
    proj_pT_total, proj_eta_total, proj_phi_total = ProjectHistogram(total_clone, "Total")

    tag = file_path.replace(".root", "")
    graph_pT  = MakeTEfficiency(proj_pT_mistag,  proj_pT_total,  f"eff_pT_{tag}")
    graph_eta = MakeTEfficiency(proj_eta_mistag, proj_eta_total, f"eff_eta_{tag}")
    graph_phi = MakeTEfficiency(proj_phi_mistag, proj_phi_total, f"eff_phi_{tag}")

    if debug:
        print(f"Graph name : {graph_pT.GetName()}, points: {graph_pT.GetN()}")
        for i in range(graph_pT.GetN()):
            print(f"  pt={graph_pT.GetPointX(i):.1f}  "
                  f"eff={graph_pT.GetPointY(i):.4f} "
                  f"+{graph_pT.GetErrorYhigh(i):.4f} "
                  f"-{graph_pT.GetErrorYlow(i):.4f}")

    return graph_pT, graph_eta, graph_phi, root_file

# ------------------------------------------------------------------------------
def OverlayHistograms(file_paths, hist_name_all, hist_name_mistag):
    pt_graphs, eta_graphs, phi_graphs, root_files = [], [], [], []

    for file_path in file_paths:
        g_pT, g_eta, g_phi, rf = GetHistogram(file_path, hist_name_all, hist_name_mistag)
        print("Got efficiency graph for: " + file_path)
        pt_graphs.append(g_pT)
        eta_graphs.append(g_eta)
        phi_graphs.append(g_phi)
        root_files.append(rf)

    legend_labels = ["2023 Cv1", "2023 Cv2", "2023 Cv3", "2023 Cv4", "2023 Dv1", "2023 Dv2"]
    if era == "2022": legend_labels = ["2022 D", "2022 E", "2022 F", "2022 G"]

    DrawCanvasAndPlots_overlay(
        "c1", "Projection plots", ": LLP skim, different eras", " in CR for different eras",
        [pt_graphs, eta_graphs, phi_graphs],
        legend_labels,
        "Overlay_LLPskim_Mistag",
        ["Jet p_{T} Mistag", "Jet #eta Mistag", "Jet #phi Mistag"],
        "OverlayEra"
    )

    for rf in root_files:
        rf.Close()

# ------------------------------------------------------------------------------
def DrawCanvasAndPlots_overlay(canvas_name, canvas_title, option, title,
                                plots, legend_labels, save_name, plot_titles, label):
    canvas = ROOT.TCanvas(f"{canvas_name}_{option}", f"{canvas_title} for {option}", 2400, 600)
    canvas.Divide(3, 1)

    for i, plot_group in enumerate(plots):
        pad = canvas.cd(i + 1)
        # Increase the left margin on all pads to ensure the y-axis title
        # is never clipped. The default after Divide() is ~0.1, which is too
        # tight for the middle and right pads where the title can underlap
        # the neighbouring pad boundary.
        pad.SetLeftMargin(0.15)
        pad.SetRightMargin(0.05)
        MakePlot_overlay(plot_group, legend_labels, plot_titles[i] + title)

    canvas.Update()
    canvas.Draw()
    canvas.SaveAs(f"outPlots_3D/{save_name}{label}_{era}.png")
    canvas.Clear()

# ------------------------------------------------------------------------------
def MakePlot_overlay(graphs, legends, title):
    """Draw a list of TGraphAsymmErrors overlaid on the current pad."""
    nGraphs = len(graphs)

    for i, g in enumerate(graphs):
        color_index = int(i / nGraphs * nColors)
        color = ROOT.TColor.GetColorPalette(color_index)
        g.SetLineColor(color)
        g.SetMarkerColor(color)
        g.SetMarkerStyle(20)
        g.SetLineWidth(2)

    # y-axis range across all graphs
    y_max = max(
        graphs[i].GetPointY(j) + graphs[i].GetErrorYhigh(j)
        for i in range(nGraphs)
        for j in range(graphs[i].GetN())
    )

    # Draw an explicit axis frame using the full histogram x-range stored on
    # the first graph. This is what was missing for pT: TEfficiency's auto-range
    # clips to non-empty bins, but a TH1 frame uses the declared axis bounds.
    x_min   = graphs[0].x_min
    x_max   = graphs[0].x_max
    x_title = graphs[0].x_title
    frame = ROOT.gPad.DrawFrame(x_min, 0, x_max, min(y_max * 1.2, 1.0))
    frame.GetXaxis().SetTitle(x_title)
    frame.GetYaxis().SetTitle("Mistag rate")
    frame.SetTitle(title)
    SetOwnership(frame, False)

    # All graphs drawn with SAME P — axes are owned by the frame
    for g in graphs:
        g.Draw("SAME P")

    ROOT.gPad.Update()

    legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    for i, g in enumerate(graphs):
        legend.AddEntry(g, legends[i], "lp")
    legend.Draw()
    SetOwnership(legend, 0)

    LabelCMS()

# ------------------------------------------------------------------------------
if __name__ == "__main__":
    root_files = [
        "output_3D_hists_depth_leading_2023_Cv1.root",
        "output_3D_hists_depth_leading_2023_Cv2.root",
        "output_3D_hists_depth_leading_2023_Cv3.root",
        "output_3D_hists_depth_leading_2023_Cv4.root",
        "output_3D_hists_depth_leading_2023_Dv1.root",
        "output_3D_hists_depth_leading_2023_Dv2.root"
    ]
    if era == "2022":
        root_files = [
            "output_3D_hists_depth_leading_2022_D.root",
            "output_3D_hists_depth_leading_2022_E.root",
            "output_3D_hists_depth_leading_2022_F.root",
            "output_3D_hists_depth_leading_2022_G.root"
        ]

    hist_name_all    = "hist3d_CR_all"
    hist_name_mistag = "hist3d_CR_mistag"

    OverlayHistograms(root_files, hist_name_all, hist_name_mistag)