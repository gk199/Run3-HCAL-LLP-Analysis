import ROOT
import numpy as np

# Initialize ROOT
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPalette(ROOT.kCandy)

from ROOT import SetOwnership

debug = False

# Define the histogram bins
pT_bins = np.linspace(0, 300, 20)  # pT axis from 0 to 500 GeV, 20 bins
pT_bins = [0, 40, 50, 60, 70, 80, 100, 120, 160, 240, 400]  # Define pT bins
pT_bins = np.array(pT_bins, dtype=float)
eta_bins = np.linspace(-1.3, 1.3, 10)  # eta axis from -2 to 2, 10 bins
phi_bins = np.linspace(-np.pi, np.pi, 10)  # phi axis from -pi to pi, 10 bins

# ------------------------------------------------------------------------------
def GetData(infilepath, label):
    # Open the input ROOT file in read mode
    input_file = ROOT.TFile(infilepath, "READ")
    
    # Check if the file opened correctly
    if input_file.IsOpen():
        print(f"Successfully opened file: {infilepath}")
    else:
        print(f"Failed to open file: {infilepath}")
        return None  # Return None if the file couldn't be opened

    # Access the tree by label
    tree = input_file.Get(label)
    
    # Check if the tree exists
    if not tree:
        print(f"Tree with label '{label}' not found in the file.")
        input_file.Close()  # Close the file if the tree doesn't exist
        return None
    if tree:
        print(f"Found tree with label '{label}'")
        # Return the tree but make sure to keep the file open while using the tree
        return tree, input_file

# ------------------------------------------------------------------------------
def MisTagParametrization(tree, option=""):

    # Setup cuts for CR and VR. CR = jet1_scores_inc between 0-0.5. VR = jet1_scores_inc between 0.5-0.9. Mistag means jet0_scores over 0.9
    CR = GetCut("jet1_scores_inc", [0,0.5])
    VR = GetCut("jet1_scores_inc", [0.5,0.9])
    mistag = GetCut("jet0_scores", [0.9,1.1])
    # Need leading jet to be matched to a LLP, jet0_L1trig_Matched. Leading jet pT > 60, subleading > 40. Eta restrictions on both jets at 1.26
    triggered = GetCut("jet0_L1trig_Matched", 1)
    pt_eta = GetCut("jet0_Pt",[40,1000]) + GetCut("jet0_Eta",[-1.26,1.26]) + GetCut("jet1_Pt",[40,1000]) + GetCut("jet1_Eta",[-1.26,1.26])
    # Emulated towers are split with jet0_DepthTowers, TimingTowers
    depth_emu = GetCut("jet0_DepthTowers", [2,100]) # + GetCut("jet0_TimingTowers", 0)
    timing_emu = GetCut("jet0_TimingTowers", [2,100]) # + GetCut("jet0_DepthTowers", 0)
    depth_timing_emu = GetCut("jet0_DepthTowers", 1) + GetCut("jet0_TimingTowers", 1)

    # Define a mapping of options to their corresponding updates
    option_map = {
        "depth": (depth_emu, ": 2+ depth", "_depth"),
        "timing": (timing_emu, ": 2+ timing", "_timing"),
        "depth_timing": (depth_timing_emu, ": 1 depth, 1 timing", "_depth_timing")
    }

    # Default values for label and title
    label = ""
    title = ": inclusive"

    # Check if the option exists in the mapping
    if option in option_map:
        CR += option_map[option][0]
        VR += option_map[option][0]
        title = option_map[option][1]
        label = option_map[option][2]

    # Create the 3D histograms with different cuts. Arguments to CreateHistograms function are tree, cut, histogram name. Histograms are filled usnig tree.Draw() method
    hist3d_CR_all = CreateHistograms(tree, CR + triggered + pt_eta, "hist3d_CR_all")
    hist3d_CR_mistag = CreateHistograms(tree, CR + triggered + pt_eta + mistag, "hist3d_CR_mistag")
    hist3d_VR_all = CreateHistograms(tree, VR + triggered + pt_eta, "hist3d_VR_all")
    hist3d_VR_mistag = CreateHistograms(tree, VR + triggered + pt_eta + mistag, "hist3d_VR_mistag")
    
    if debug:
        print(f"Entries in the tree: {tree.GetEntries()}")
        print("Branches in the tree:")
        for branch in tree.GetListOfBranches():
            print(branch.GetName())

        print("Leading jet pT values:")
        for entry in range(10):  # Checking first 10 entries
            tree.GetEntry(entry)
            print(tree.jet0_Pt)  # Check if it prints a valid value

        hist1d_Pt = ROOT.TH1F("hist1d_Pt", "1D hist to test", 10, 0, 500)
        tree.Draw("jet0_Pt >> hist1d_Pt", "jet0_Pt > 0", "")
        print(f"Entries in 1D histogram: {hist1d_Pt.GetEntries()}") 
        print(f"Entries in 1D histogram: {hist1d_Pt.Integral()}")

        # Check the 3D histogram content (for debugging)
        print(f"Histogram Info: {hist3d_CR_all}")
        print(f"Number of entries in the 3D histogram: {hist3d_CR_all.GetEntries()}")

    # Write the base histograms to the output file
    output_file = ROOT.TFile("output_3D_hists"+label+".root", "RECREATE")
    output_file.WriteObject(hist3d_CR_all)
    output_file.WriteObject(hist3d_CR_mistag)
    output_file.WriteObject(hist3d_VR_all)
    output_file.WriteObject(hist3d_VR_mistag)
    print("Created root file for output, wrote 3D histograms")
    # Close the ROOT file
    output_file.Close()

    # Project all histograms onto each axis to form 1D histograms
    proj_pT_CR_all, proj_eta_CR_all, proj_phi_CR_all = ProjectHistogram(hist3d_CR_all, "Number of events")
    proj_pT_CR_mistag, proj_eta_CR_mistag, proj_phi_CR_mistag = ProjectHistogram(hist3d_CR_mistag, "Number of events")
    proj_pT_VR_all, proj_eta_VR_all, proj_phi_VR_all = ProjectHistogram(hist3d_VR_all, "Number of events")
    proj_pT_VR_mistag, proj_eta_VR_mistag, proj_phi_VR_mistag = ProjectHistogram(hist3d_VR_mistag, "Number of events")

    # Create a canvas to display the plots of CR and VR all and mistags overlayed
    legend_labels = ["CR (no cuts)", "CR, mistag", "VR (no cuts)", "VR, mistag"]
    DrawCanvasAndPlots(
        "c1", "Projection plots", option, title,
        [[proj_pT_CR_all, proj_pT_CR_mistag, proj_pT_VR_all, proj_pT_VR_mistag], 
        [proj_eta_CR_all, proj_eta_CR_mistag, proj_eta_VR_all, proj_eta_VR_mistag], 
        [proj_phi_CR_all, proj_phi_CR_mistag, proj_phi_VR_all, proj_phi_VR_mistag]],  # Wrap each plot in a list
        legend_labels,
        "3d_hist_projection_overlay_CR_VR",
        ["Jet p_{T} Projection with various cuts", "Jet #eta Projection with various cuts", "Jet #phi Projection with various cuts"], label
    )

    # Clone hist3d_CR_mistag and divide it by hist3d_CR_all to get the mistag rate in CR
    hist3d_CR_mistag_rate = hist3d_CR_mistag.Clone()
    hist3d_CR_mistag_rate.Divide(hist3d_CR_all)
    hist3d_VR_mistag_predict = hist3d_VR_all.Clone()
    hist3d_VR_mistag_predict.Multiply(hist3d_CR_mistag_rate)
    # Explicitly set axis labels after cloning
    ResetAxis(hist3d_CR_mistag_rate)
    ResetAxis(hist3d_VR_mistag_predict)
    # Use this to predict the mistag rate in the VR
    # proj_pT_CR_mistag_rate, proj_eta_CR_mistag_rate, proj_phi_CR_mistag_rate = ProjectHistogram(hist3d_CR_mistag_rate) # mistag rate from CR -- but this projection adds bins together to give a rate > 1! Use 1D from below
    proj_pT_VR_mistag_predict, proj_eta_VR_mistag_predict, proj_phi_VR_mistag_predict = ProjectHistogram(hist3d_VR_mistag_predict, "Number of events") # predicted mistag in VR

    # Find mistag rate in 1D histograms to evaluate plots
    hist3d_CR_mistag_rate = hist3d_CR_mistag.Clone()
    ResetAxis(hist3d_CR_mistag_rate)
    proj_pT_CR_mistag_rate, proj_eta_CR_mistag_rate, proj_phi_CR_mistag_rate = ProjectHistogram(hist3d_CR_mistag_rate, "Mistag rate")
    proj_pT_CR_mistag_rate.Divide(proj_pT_CR_all)
    proj_eta_CR_mistag_rate.Divide(proj_eta_CR_all)
    proj_phi_CR_mistag_rate.Divide(proj_phi_CR_all)

    # Create mistag rate plots in the CR
    legend_labels = ["Mistag rate (CR)"]
    DrawCanvasAndPlots(
        "c2", "Mistag rate plots in the CR", option, title,
        [[proj_pT_CR_mistag_rate], [proj_eta_CR_mistag_rate], [proj_phi_CR_mistag_rate]],  # Wrap each plot in a list
        legend_labels,
        "3d_hist_projection_CR_mistag_rate",
        ["Jet p_{T} Mistag Rate from CR", "Jet #eta Mistag Rate from CR", "Jet #phi Mistag Rate from CR"], label
    )

    # Create mistag plots in the VR (with two histograms per plot)
    legend_labels = ["Observed mistag (VR)", "Predicted mistag (VR)"]
    DrawCanvasAndPlots(
        "c3", "Mistag plots in the VR", option, title,
        [[proj_pT_VR_mistag, proj_pT_VR_mistag_predict], 
        [proj_eta_VR_mistag, proj_eta_VR_mistag_predict, ], 
        [proj_phi_VR_mistag, proj_phi_VR_mistag_predict]],  # Each group has two histograms
        legend_labels,
        "3d_hist_projection_VR_mistags",
        ["Jet p_{T} Mistags in VR", "Jet #eta Mistags in VR", "Jet #phi Mistags in VR"], label
    )

    legend_labels = ["Observed mistag (VR)", "Predicted mistag (VR)"]
    MakePlotWithRatio([proj_pT_VR_mistag, proj_pT_VR_mistag_predict], legend_labels, label + "_pT")
    MakePlotWithRatio([proj_eta_VR_mistag, proj_eta_VR_mistag_predict], legend_labels, label + "_eta")
    MakePlotWithRatio([proj_phi_VR_mistag, proj_phi_VR_mistag_predict], legend_labels, label + "_phi") 

    # below code is now done in DrawCanvasAndPlots to avoid so much duplication
    # c3 = ROOT.TCanvas(f"c3_{option}", f"Mistag plots in the VR for {option}", 2400, 600)
    # c3.Divide(3,1)
    # legend_labels = ["Observed mistag (VR)", "Predicted mistag (VR)"]
    # c3.cd(1)
    # MakePlot([proj_pT_VR_mistag, proj_pT_VR_mistag_predict], legend_labels)
    # proj_pT_VR_mistag.SetTitle("Jet p_{T} Mistags in VR" + title)
    # c3.cd(2)
    # MakePlot([proj_eta_VR_mistag, proj_eta_VR_mistag_predict], legend_labels)
    # proj_eta_VR_mistag.SetTitle("Jet #eta Mistags in VR" + title)
    # c3.cd(3)
    # MakePlot([proj_phi_VR_mistag, proj_phi_VR_mistag_predict], legend_labels) 
    # proj_phi_VR_mistag.SetTitle("Jet #phi Mistags in VR" + title)
    # c3.Update()
    # c3.Draw()   
    # c3.SaveAs("3d_hist_projection_VR_mistags"+label+".png") 

# ------------------------------------------------------------------------------
def CreateHistograms(tree, cut, hist_name):
    # Check if the histogram already exists and delete it
    if ROOT.gDirectory.Get(hist_name):
        ROOT.gDirectory.Get(hist_name).Delete()
    # Create and fill the histograms
    hist3d = ROOT.TH3F(hist_name, "3D histogram; p_{T}; #eta; #phi", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> " + hist_name, cut, "")
    return hist3d

# ------------------------------------------------------------------------------
def ProjectHistogram(hist3d, y_label = ""):
    # Project the histogram in the x, y, and z directions
    proj_pT = hist3d.Project3D("x")
    proj_eta = hist3d.Project3D("y")
    proj_phi = hist3d.Project3D("z")

    proj_pT.SetYTitle(y_label)
    proj_eta.SetYTitle(y_label)
    proj_phi.SetYTitle(y_label)
    
    # Return the projections
    return proj_pT, proj_eta, proj_phi

# ------------------------------------------------------------------------------
def DrawCanvasAndPlots(canvas_name, canvas_title, option, title, plots, legend_labels, save_name, plot_titles, label):
    # Create canvas
    canvas = ROOT.TCanvas(f"{canvas_name}_{option}", f"{canvas_title} for {option}", 2400, 600)
    canvas.Divide(3, 1)
    
    # Loop over the plots and generate them
    for i, plot_group in enumerate(plots):
        canvas.cd(i + 1)  # Navigate to the correct pad
        # MakePlot can take a list of histograms, so pass the group of histograms
        MakePlot(plot_group, legend_labels)
        
        # Set the title for the plot group
        plot_group[0].SetTitle(plot_titles[i] + title)  # Just set the title for the first plot in the group
    
    canvas.Update()
    canvas.Draw()
    canvas.SaveAs(f"{save_name}{label}.png")
    canvas.Clear()

# ------------------------------------------------------------------------------
def MakePlot(hists, legends):
    colors = [ROOT.kBlue, ROOT.kBlue-9, ROOT.kGreen+3, ROOT.kGreen-6]
    if len(hists) == 2: colors = [49, 29]
    i = 0

    # Draw the histograms and overlay 
    for hist in hists:
        if len(hists) == 2 and i == 1: # Check if only two histograms are passed, and apply shading (for predicted and observed plots)
            hist.SetFillStyle(3004)
            hist.SetFillColor(colors[i])
        hist.SetLineColor(colors[i])
        hist.SetLineWidth(2)
        i += 1
    if len(hists) == 1: hists[0].Draw("HIST")
    else: hists[0].Draw("HIST E")
    for hist in hists[1:]:
        hist.Draw("SAME HIST E")

    # Manually set axis limits for pT to ensure all histograms are visible
    hists[0].SetMaximum(max([hist.GetMaximum() for hist in hists]) * 1.1)
    hists[0].SetMinimum(0)

    # Add legend 
    legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    for i in range(len(hists)):
        legend.AddEntry(hists[i], legends[i], "l")
    legend.Draw()
    SetOwnership( legend, 0 ) # 0 = release (not keep), 1 = keep # when legend is in a separate function, it is not saved in memory for the canvas outside of function (scoping issue)
    LabelCMS()

# ------------------------------------------------------------------------------
def MakePlotWithRatio(hists, legends, type):
    # Check if there are exactly two histograms
    if len(hists) != 2:
        print("This function requires exactly two histograms.")
        return

    # Create a canvas with enough space for the main plot and ratio plot
    c_ratio = ROOT.TCanvas("c_ratio", "Canvas with Ratio", 800, 800)
    
    # Create the ratio plot (TRatioPlot). Set the second histogram (hists[1]) to be shaded
    hists[1].SetFillStyle(3004)
    hists[1].SetFillColor(29)
    ratio_plot = ROOT.TRatioPlot(hists[0], hists[1])  # h1 / h2
    ratio_plot.SetH1DrawOpt("HIST E")
    ratio_plot.SetH2DrawOpt("HIST E F")
    ratio_plot.Draw("HIST E F") # F for filled (second histogram), E for errors

    # Set the ratio plot's y-axis limits
    ratio_plot.GetLowYaxis().SetNdivisions(7)
    ratio_plot.GetLowerRefYaxis().SetRangeUser(0.5, 2)  # Minimum and maximum for ratio plot
    ratio_plot.GetLowerRefYaxis().SetTitle("Obs. / Predicted")

    # Add legend 
    legend = ROOT.TLegend(0.6, 0.75, 0.89, 0.92)
    for i in range(len(hists)):
        legend.AddEntry(hists[i], legends[i], "lef")
    legend.Draw()
    SetOwnership( legend, 0 ) # 0 = release (not keep), 1 = keep # when legend is in a separate function, it is not saved in memory for the canvas outside of function (scoping issue)
    LabelCMS(0.13, 0.9, 0.03)

    # Update the canvas
    c_ratio.Update()
    c_ratio.Draw()

    # Save the canvas
    c_ratio.SaveAs("3d_hist_projection_VR_mistags" + type + "_ratio.png")
    c_ratio.Clear()

# ------------------------------------------------------------------------------
def MakePlotWithRatio_v1(hists, legends, type):
    # Check if there are exactly two histograms
    if len(hists) != 2:
        print("This function requires exactly two histograms.")
        return

    # Create a canvas with two pads: one for the main plot, one for the ratio
    c_ratio = ROOT.TCanvas("c_ratio", "Canvas with Ratio", 800, 800)
    c_ratio.Divide(1, 2)  # First pad takes up 70% of the canvas, second pad (for ratio) takes 30%

    # Main plot: top pad
    c_ratio.cd(1)
    MakePlot(hists, legends)  # Reuse the MakePlot function for the main plot

    # Ratio plot: bottom pad
    c_ratio.cd(2)
    ratio = hists[0].Clone("ratio")  # Clone the first histogram for the ratio
    ratio.Divide(hists[1])  # Divide the first histogram by the second (observed / predicted)

    # Style the ratio plot
    ratio.SetLineColor(ROOT.kBlack)
    ratio.SetMarkerStyle(20)  # Set markers for the ratio plot
    ratio.SetMarkerColor(ROOT.kBlack)

    # Draw the ratio plot
    ratio.Draw("E")

    # Set the y-axis limits for the ratio plot
    ratio.SetMinimum(0.5)
    ratio.SetMaximum(1.5)

    # Set axis titles for the ratio plot
    ratio.SetXTitle(hists[0].GetXaxis().GetTitle())
    ratio.SetYTitle("Ratio")

    # Optional: Add a horizontal line at y = 1
    horizontal_line = ROOT.TLine(ratio.GetXaxis().GetXmin(), 1, ratio.GetXaxis().GetXmax(), 1)
    horizontal_line.SetLineColor(ROOT.kRed)
    horizontal_line.SetLineStyle(2)
    horizontal_line.Draw()

    # Update and display the canvas
    c_ratio.Update()
    c_ratio.Draw()

    # Save the canvas
    c_ratio.SaveAs("3d_hist_projection_VR_mistags" + type + "_ratio.png")

# ------------------------------------------------------------------------------
def LabelCMS(xpos=0.13, ypos=0.85, text_size=0.036):
    cmsLabel = "#scale[1]{#bf{CMS} }"
    cmsLabelExtra = "#scale[0.8]{#it{Private Work}}"
    yearLumi = "#scale[0.85]{2023 (13.6 TeV)}" # #sqrt{s} = 

    stamp_text = ROOT.TLatex()
    stamp_text.SetNDC()
    stamp_text.SetTextFont(42)
    stamp_text.SetTextSize(text_size)
    stamp_text.DrawLatex( xpos, ypos, cmsLabel)
    stamp_text.DrawLatex( xpos+0.07, ypos, cmsLabelExtra)
    if ypos == 0.85: stamp_text.DrawLatex( xpos+0.62, ypos+0.06, yearLumi)
    else: stamp_text.DrawLatex( xpos+0.6, ypos+0.03, yearLumi)

# ------------------------------------------------------------------------------
def ResetAxis(hist3d):
    # Explicitly set axis labels after cloning
    hist3d.GetXaxis().SetTitle("p_{T} (GeV)")
    hist3d.GetYaxis().SetTitle("#eta")
    hist3d.GetZaxis().SetTitle("#phi")

# ------------------------------------------------------------------------------
def GetCut( branch_name, branch_sel):

	selection = ""

	if type(branch_sel) is int:
		selection = branch_name + " == " + str(branch_sel) 

	elif type(branch_sel) is list:
		if len(branch_sel) == 0:
			print("WARNING <GetCut> : Entry error (length = 0). No cut implemented")
		elif len(branch_sel) == 1:
			selection = branch_name + " == " + str(branch_sel[0])
		else:
			selection = branch_name + " >= " + str(branch_sel[0]) + " && " + branch_name + " < " + str(branch_sel[1])

		if len(branch_sel) > 2:
			print("WARNING <GetCut> : 'branch_sel' input has more than two entries. Only using the first two!")

	else: 
		print("WARNING <GetCut> : Type Error. No cut implemented")

	return ROOT.TCut( selection + " " )

# ------------------------------------------------------------------------------
def main():

    infilepath = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv1_NoSel_scores_2025_02_03.root"
    label = "NoSel"

    tree, input_file = GetData(infilepath, label)
    if tree:
        print("Tree successfully passed to MisTagParametrization")
        #MisTagParametrization(tree)
        #MisTagParametrization(tree, "depth")
        #MisTagParametrization(tree, "timing")
        MisTagParametrization(tree, "depth_timing")
        # Don't close the file until you're done using the tree
        input_file.Close()
    else:
        print("Tree is invalid!")

if __name__ == '__main__':
	main()