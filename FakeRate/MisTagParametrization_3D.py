import ROOT
import numpy as np

# Initialize ROOT
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPalette(ROOT.kCandy)

from ROOT import SetOwnership

debug = False

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
        print(f"Found tree with label '{label}' and using in MisTagParametrization()")
        MisTagParametrization(tree)

# ------------------------------------------------------------------------------
def MisTagParametrization(tree):

    # Define the histogram bins
    pT_bins = np.linspace(0, 300, 20)  # pT axis from 0 to 500 GeV, 20 bins                 # pT_bins = [0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100]  # Define pT bins
    eta_bins = np.linspace(-1.3, 1.3, 10)  # eta axis from -2 to 2, 10 bins
    phi_bins = np.linspace(-np.pi, np.pi, 10)  # phi axis from -pi to pi, 10 bins

    # Create four 3D histograms with different cuts
    hist3d_CR_all = ROOT.TH3F("hist3d_CR_all", "3D histogram in CR (no cuts); p_{T}; #eta; #phi", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)
    hist3d_CR_mistag = ROOT.TH3F("hist3d_CR_mistag", "3D histogram of mistag in CR; ; p_{T}; #eta; #phi", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)
    hist3d_VR_all = ROOT.TH3F("hist3d_VR_all", "3D histogram in VR (no cuts); ; p_{T}; #eta; #phi", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)
    hist3d_VR_mistag = ROOT.TH3F("hist3d_VR_mistag", "3D histogram of mistag in VR; ; p_{T}; #eta; #phi", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)

    # Setup cuts for CR and VR
    # CR = jet1_scores_inc between 0-0.5. VR = jet1_scores_inc between 0.5-0.9. Mistag means jet0_scores over 0.9
    CR = GetCut("jet1_scores_inc", [0,0.5])
    VR = GetCut("jet1_scores_inc", [0.5,0.9])
    mistag = GetCut("jet0_scores", [0.9,1.1])
    # Need leading jet to be matched to a LLP, jet0_L1trig_Matched. Leading jet pT > 60, subleading > 40. Eta restrictions on both jets at 1.26
    triggered = GetCut("jet0_L1trig_Matched", 1)
    pt_eta = GetCut("jet0_Pt",[40,1000]) + GetCut("jet0_Eta",[-1.26,1.26]) + GetCut("jet1_Pt",[40,1000]) + GetCut("jet1_Eta",[-1.26,1.26])
    # Emulated towers are split with jet0_DepthTowers, TimingTowers
    depth_emu = GetCut("jet0_DepthTowers", [2,100])
    timing_emu = GetCut("jet0_TimingTowers", [2,100])
    depth_timing_emu = GetCut("jet0_DepthTowers", 1) + GetCut("jet0_TimingTowers", 1)

    # Fill the histograms using tree.Draw() with cuts
    # CR, CR mistag, VR, VR mistag. Then VR predicted mistag is calculated from VR * CR mistag / CR
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> hist3d_CR_all", CR + triggered + pt_eta, "")
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> hist3d_CR_mistag", CR + triggered + pt_eta + mistag, "")
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> hist3d_VR_all", VR + triggered + pt_eta, "")
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> hist3d_VR_mistag", VR + triggered + pt_eta + mistag, "")

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
    output_file = ROOT.TFile("output_3D_hists.root", "RECREATE")
    output_file.WriteObject(hist3d_CR_all)
    output_file.WriteObject(hist3d_CR_mistag)
    output_file.WriteObject(hist3d_VR_all)
    output_file.WriteObject(hist3d_VR_mistag)
    print("Created root file for output, wrote 3D histograms")
    # Close the ROOT file
    output_file.Close()

    # Project all histograms onto each axis to form 1D histograms
    proj_pT_CR_all, proj_eta_CR_all, proj_phi_CR_all = ProjectHistogram(hist3d_CR_all)
    proj_pT_CR_mistag, proj_eta_CR_mistag, proj_phi_CR_mistag = ProjectHistogram(hist3d_CR_mistag)
    proj_pT_VR_all, proj_eta_VR_all, proj_phi_VR_all = ProjectHistogram(hist3d_VR_all)
    proj_pT_VR_mistag, proj_eta_VR_mistag, proj_phi_VR_mistag = ProjectHistogram(hist3d_VR_mistag)

    # Create a canvas to display the plots of CR and VR all and mistag overlayed
    c1 = ROOT.TCanvas("c1", "Projection plots with different energy cuts", 2400, 600)
    c1.Divide(3, 1)
    # Legend labels
    legend_labels = ["CR (no cuts)", "CR, mistag", "VR (no cuts)", "VR, mistag"]
    # Plot the projections for pT
    c1.cd(1)
    MakePlot([proj_pT_CR_all, proj_pT_CR_mistag, proj_pT_VR_all, proj_pT_VR_mistag], legend_labels)
    proj_pT_CR_all.SetTitle("Jet p_{T} Projection with various cuts")
    # Plot the projections for eta
    c1.cd(2)
    MakePlot([proj_eta_CR_all, proj_eta_CR_mistag, proj_eta_VR_all, proj_eta_VR_mistag], legend_labels)
    proj_eta_CR_all.SetTitle("Jet #eta Projection with various cuts")
    # Plot the projections for phi
    c1.cd(3)
    MakePlot([proj_phi_CR_all, proj_phi_CR_mistag, proj_phi_VR_all, proj_phi_VR_mistag], legend_labels)
    proj_phi_CR_all.SetTitle("Jet #phi Projection with various cuts")
    # Show the canvas
    c1.Update()
    c1.Draw()
    # Save the result as an image or file
    c1.SaveAs("3d_hist_projection_overlay_CR_VR.png")

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
    proj_pT_VR_mistag_predict, proj_eta_VR_mistag_predict, proj_phi_VR_mistag_predict = ProjectHistogram(hist3d_VR_mistag_predict) # predicted mistag in VR

    # Find mistag rate in 1D histograms to evaluate plots
    hist3d_CR_mistag_rate = hist3d_CR_mistag.Clone()
    ResetAxis(hist3d_CR_mistag_rate)
    proj_pT_CR_mistag_rate, proj_eta_CR_mistag_rate, proj_phi_CR_mistag_rate = ProjectHistogram(hist3d_CR_mistag_rate)
    proj_pT_CR_mistag_rate.Divide(proj_pT_CR_all)
    proj_eta_CR_mistag_rate.Divide(proj_eta_CR_all)
    proj_phi_CR_mistag_rate.Divide(proj_phi_CR_all)

    # Create a canvas for the mistag rate plots (CR)
    c2 = ROOT.TCanvas("c2", "Mistag rate projections in the CR", 2400, 600)
    c2.Divide(3,1)
    legend_labels = ["Mistag rate (CR)"]
    c2.cd(1)
    MakePlot([proj_pT_CR_mistag_rate], legend_labels)
    proj_pT_CR_mistag_rate.SetTitle("Jet p_{T} Mistag Rate from CR")
    c2.cd(2)
    MakePlot([proj_eta_CR_mistag_rate], legend_labels)
    proj_eta_CR_mistag_rate.SetTitle("Jet #eta Mistag Rate from CR")
    c2.cd(3)
    MakePlot([proj_phi_CR_mistag_rate], legend_labels) 
    proj_phi_CR_mistag_rate.SetTitle("Jet #phi Mistag Rate from CR")
    c2.Update()
    c2.Draw()   
    c2.SaveAs("3d_hist_projection_CR_mistag_rate.png")

    # Create a canvas for the mistag prediction and observation (VR)
    c3 = ROOT.TCanvas("c3", "Mistag in the VR", 2400, 600)
    c3.Divide(3,1)
    legend_labels = ["Predicted mistag (VR)", "Observed mistag (VR)"]
    c3.cd(1)
    MakePlot([proj_pT_VR_mistag_predict, proj_pT_VR_mistag], legend_labels)
    proj_pT_VR_mistag_predict.SetTitle("Jet p_{T} Mistags in VR")
    c3.cd(2)
    MakePlot([proj_eta_VR_mistag_predict, proj_eta_VR_mistag], legend_labels)
    proj_eta_VR_mistag_predict.SetTitle("Jet #eta Mistags in VR")
    c3.cd(3)
    MakePlot([proj_phi_VR_mistag_predict, proj_phi_VR_mistag], legend_labels) 
    proj_phi_VR_mistag_predict.SetTitle("Jet #phi Mistags in VR")
    c3.Update()
    c3.Draw()   
    c3.SaveAs("3d_hist_projection_VR_mistags.png")    

# ------------------------------------------------------------------------------
def ProjectHistogram(hist3d):
    
    # Project the histogram in the x, y, and z directions
    proj_pT = hist3d.Project3D("x")
    proj_eta = hist3d.Project3D("y")
    proj_phi = hist3d.Project3D("z")
    
    # Return the projections
    return proj_pT, proj_eta, proj_phi

# ------------------------------------------------------------------------------
def MakePlot(hists, legends):
    colors = [ROOT.kBlue, ROOT.kBlue-9, ROOT.kGreen+3, ROOT.kGreen-6]
    i = 0

    # Draw the histograms and overlay 
    for hist in hists:
        hist.SetLineColor(colors[i])
        i += 1
    hists[0].Draw()
    for hist in hists[1:]:
        hist.Draw("SAME")

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
def LabelCMS():
    cmsLabel = "#scale[1]{#bf{CMS} }"
    cmsLabelExtra = "#scale[0.8]{#it{Private Work}}"
    yearLumi = "#scale[0.85]{2023 (13.6 TeV)}" # #sqrt{s} = 

    xpos = 0.13
    ypos = 0.85

    stamp_text = ROOT.TLatex()
    stamp_text.SetNDC()
    stamp_text.SetTextFont(42)
    stamp_text.SetTextSize(0.036)
    stamp_text.DrawLatex( xpos, ypos, cmsLabel)
    stamp_text.DrawLatex( xpos+0.07, ypos, cmsLabelExtra)
    stamp_text.DrawLatex( 0.75, 0.91, yearLumi)

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

    GetData(infilepath, label)

if __name__ == '__main__':
	main()