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

    # Create a sample ROOT file (or use your own)
    file = ROOT.TFile("example_data_with_cuts.root", "RECREATE")
    print("Created root file for output")

    # Define the histogram bins
    pT_bins = np.linspace(0, 300, 20)  # pT axis from 0 to 500 GeV, 20 bins                 # pT_bins = [0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100]  # Define pT bins
    eta_bins = np.linspace(-1.3, 1.3, 10)  # eta axis from -2 to 2, 10 bins
    phi_bins = np.linspace(-np.pi, np.pi, 10)  # phi axis from -pi to pi, 10 bins

    # Create four 3D histograms with different cuts
    hist3d_CR_all = ROOT.TH3F("hist3d_CR_all", "3D histogram in CR (no cuts); pT; #eta; #phi", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)
    hist3d_CR_mistag = ROOT.TH3F("hist3d_CR_mistag", "3D histogram of mistag in CR", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)
    hist3d_CR_mistag_2 = ROOT.TH3F("hist3d_CR_mistag_2", "3D histogram with energy cut (30 < E < 50)", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)
    hist3d_multiplied = ROOT.TH3F("hist3d_multiplied", "3D histogram multiplied (E > 50 & 30 < E < 50)", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)

    # Setup cuts for CR and VR
    # CR = jet1_scores_inc between 0-0.5
    # VR = jet1_scores_inc between 0.5-0.9
    # mistag means jet0_scores over 0.9
    CR = GetCut("jet1_scores_inc", [0,0.5])
    VR = GetCut("jet1_scores_inc", [0.5,0.9])
    mistag = GetCut("jet0_scores", [0.9,1.1])
    # need leading jet to be matched to a LLP, jet0_L1trig_Matched
    # leading jet pT > 60, subleading > 40
    # eta restrictions on both jets, 1.26
    triggered = GetCut("jet0_L1trig_Matched", 1)
    pt_eta = GetCut("jet0_Pt",[60,1000]) + GetCut("jet0_Eta",[-1.26,1.26]) + GetCut("jet1_Pt",[40,1000]) + GetCut("jet1_Eta",[-1.26,1.26])
    # emulated towers split with jet0_DepthTowers, TimingTowers
    depth_emu = GetCut("jet0_DepthTowers", [2,100])
    timing_emu = GetCut("jet0_TimingTowers", [2,100])
    depth_timing_emu = GetCut("jet0_DepthTowers", 1) + GetCut("jet0_TimingTowers", 1)

    # Fill the histograms using tree.Draw() with cuts
    # CR, CR mistag, VR, VR mistag
    # VR predicted mistag from VR * CR mistag / CR
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> hist3d_CR_all", CR + triggered + pt_eta, "")
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> hist3d_CR_mistag", CR + triggered + pt_eta + mistag, "")
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> hist3d_CR_mistag_2", VR + triggered + pt_eta, "")
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> hist3d_multiplied", VR + triggered + pt_eta + mistag, "")

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

    # Write the histograms to the output file
    output_file = ROOT.TFile("output_with_cuts.root", "RECREATE")
    output_file.Write()

    # Project all histograms onto each axis to form 1D histograms
    proj_pT_no_cut = hist3d_CR_all.Project3D("x")
    proj_eta_no_cut = hist3d_CR_all.Project3D("y")
    proj_phi_no_cut = hist3d_CR_all.Project3D("z")

    proj_pT_with_cut = hist3d_CR_mistag.Project3D("x")
    proj_eta_with_cut = hist3d_CR_mistag.Project3D("y")
    proj_phi_with_cut = hist3d_CR_mistag.Project3D("z")

    proj_pT_with_cut_2 = hist3d_CR_mistag_2.Project3D("x")
    proj_eta_with_cut_2 = hist3d_CR_mistag_2.Project3D("y")
    proj_phi_with_cut_2 = hist3d_CR_mistag_2.Project3D("z")

    proj_pT_multiplied = hist3d_multiplied.Project3D("x")
    proj_eta_multiplied = hist3d_multiplied.Project3D("y")
    proj_phi_multiplied = hist3d_multiplied.Project3D("z")

    # Create a canvas to display the plots
    c1 = ROOT.TCanvas("c1", "Projection plots with different energy cuts", 2400, 600)
    c1.Divide(3, 1)

    # Legend labels
    legend_labels = ["no cuts", "low DNN", "mid DNN", "high DNN"]

    # Plot the projections for pT
    c1.cd(1)
    MakePlot([proj_pT_no_cut, proj_pT_with_cut, proj_pT_with_cut_2, proj_pT_multiplied], legend_labels)
    proj_pT_no_cut.SetTitle("Jet p_{T} Projection with various cuts")

    # Plot the projections for eta
    c1.cd(2)
    MakePlot([proj_eta_no_cut, proj_eta_with_cut, proj_eta_with_cut_2, proj_eta_multiplied], legend_labels)
    proj_eta_no_cut.SetTitle("Jet #eta Projection with various cuts")

    # Plot the projections for phi
    c1.cd(3)
    MakePlot([proj_phi_no_cut, proj_phi_with_cut, proj_phi_with_cut_2, proj_phi_multiplied], legend_labels)
    proj_phi_no_cut.SetTitle("Jet #phi Projection with various cuts")

    # Show the canvas
    c1.Update()
    c1.Draw()

    # Save the result as an image or file
    c1.SaveAs("3d_hist_projection_overlay_with_multiplication_fixed.png")

    # Close the ROOT file
    file.Close()

# ------------------------------------------------------------------------------
def MakePlot(hists, legends):
    colors = [ROOT.kRed, ROOT.kBlue, ROOT.kGreen, ROOT.kBlack]
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