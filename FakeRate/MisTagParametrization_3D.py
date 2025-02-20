import ROOT
import numpy as np

# Initialize ROOT
ROOT.gROOT.SetBatch(True)

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

    MisTagParametrization(tree)

# def GetData(infilepath, label):

#     # Open the input ROOT file and access the tree
#     input_file = ROOT.TFile( infilepath, "READ" )
#     tree = input_file.Get(label) 
#     print("got tree")

# ------------------------------------------------------------------------------
def MisTagParametrization(tree):

    # Create a sample ROOT file (or use your own)
    file = ROOT.TFile("example_data_with_cuts.root", "RECREATE")
    print("created root file for output")

    # Define the histogram bins
    pT_bins = np.linspace(0, 100, 20)  # pT axis from 0 to 100 GeV, 20 bins
    # pT_bins = [0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100]  # Define pT bins
    eta_bins = np.linspace(-3, 3, 30)  # eta axis from -3 to 3, 30 bins
    phi_bins = np.linspace(-np.pi, np.pi, 40)  # phi axis from -pi to pi, 40 bins

    # Create four 3D histograms with different cuts
    hist3d_CR_all = ROOT.TH3F("hist3d_CR_all", "Jet pT, Eta, and Phi",
                          20, 0, 500,   # Binning for pT: 50 bins, range 0-500
                          10, -2, 2,    # Binning for eta: 20 bins, range -5 to 5
                          10, -ROOT.TMath.Pi(), ROOT.TMath.Pi())  # Binning for phi: 20 bins, range -pi to pi

    # hist3d_CR_all = ROOT.TH3F("hist3d_CR_all", "3D histogram in CR (no cuts)", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)
    hist3d_CR_mistag = ROOT.TH3F("hist3d_CR_mistag", "3D histogram of mistag in CR", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)
    hist3d_CR_mistag_2 = ROOT.TH3F("hist3d_CR_mistag_2", "3D histogram with energy cut (30 < E < 50)", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)
    hist3d_multiplied = ROOT.TH3F("hist3d_multiplied", "3D histogram multiplied (E > 50 & 30 < E < 50)", len(pT_bins)-1, pT_bins, len(eta_bins)-1, eta_bins, len(phi_bins)-1, phi_bins)

    print(tree.GetEntries())
    print("Branches in the tree:")
    for branch in tree.GetListOfBranches():
        print(branch.GetName())
    
    hist1d_Pt = ROOT.TH1F("hist_test", "1D hist to test", 10, 0, 100)
    tree.Draw("jet0_Pt >> hist1d_Pt", "", "COLZ")
    print(f"Entries in 1D histogram: {hist1d_Pt.GetEntries()}") # this has no entries! 

    # Fill the histograms using tree.Draw() with cuts
    tree.Draw("jet0_Pt:jet0_Eta:jet0_Phi >> hist3d_CR_all", "", "COLZ")
    tree.Draw("jet0_Pt:jet0_Eta:jet0_Phi >> hist3d_CR_mistag", "jet1_scores_inc >= 0 && jet1_scores_inc < 0.5", "COLZ")
    tree.Draw("jet0_Pt:jet0_Eta:jet0_Phi >> hist3d_CR_mistag_2", "jet1_scores_inc >= 0 && jet1_scores_inc < 0.7", "COLZ")
    tree.Draw("jet0_Pt:jet0_Eta:jet0_Phi >> hist3d_multiplied", "jet1_scores_inc >= 0 && jet1_scores_inc < 0.9", "COLZ")

    # Check the number of entries in the histogram
    print("Entries in the histogram:", hist3d_CR_all.GetEntries())

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
    c1 = ROOT.TCanvas("c1", "Projection plots with different energy cuts", 800, 600)
    c1.Divide(3, 1)

    # Plot the projections for pT
    c1.cd(1)
    proj_pT_no_cut.SetLineColor(ROOT.kRed)
    proj_pT_with_cut.SetLineColor(ROOT.kBlue)
    proj_pT_with_cut_2.SetLineColor(ROOT.kGreen)
    proj_pT_multiplied.SetLineColor(ROOT.kBlack)

    proj_pT_no_cut.Draw()
    proj_pT_with_cut.Draw("SAME")
    proj_pT_with_cut_2.Draw("SAME")
    proj_pT_multiplied.Draw("SAME")

    # Add legend for pT plot
    legend_pT = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    legend_pT.AddEntry(proj_pT_no_cut, "No Cut", "l")
    legend_pT.AddEntry(proj_pT_with_cut, "E > 50 GeV", "l")
    legend_pT.AddEntry(proj_pT_with_cut_2, "30 < E < 50 GeV", "l")
    legend_pT.AddEntry(proj_pT_multiplied, "E > 50 and 30 < E < 50", "l")
    legend_pT.Draw()

    # Manually set axis limits for pT to ensure all histograms are visible
    proj_pT_no_cut.SetMaximum(max(proj_pT_no_cut.GetMaximum(), proj_pT_with_cut.GetMaximum(), proj_pT_with_cut_2.GetMaximum(), proj_pT_multiplied.GetMaximum()) * 1.1)
    proj_pT_no_cut.SetMinimum(0)

    # Plot the projections for eta
    c1.cd(2)
    proj_eta_no_cut.SetLineColor(ROOT.kRed)
    proj_eta_with_cut.SetLineColor(ROOT.kBlue)
    proj_eta_with_cut_2.SetLineColor(ROOT.kGreen)
    proj_eta_multiplied.SetLineColor(ROOT.kBlack)

    proj_eta_no_cut.Draw()
    proj_eta_with_cut.Draw("SAME")
    proj_eta_with_cut_2.Draw("SAME")
    proj_eta_multiplied.Draw("SAME")

    # Add legend for eta plot
    legend_eta = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    legend_eta.AddEntry(proj_eta_no_cut, "No Cut", "l")
    legend_eta.AddEntry(proj_eta_with_cut, "E > 50 GeV", "l")
    legend_eta.AddEntry(proj_eta_with_cut_2, "30 < E < 50 GeV", "l")
    legend_eta.AddEntry(proj_eta_multiplied, "E > 50 and 30 < E < 50", "l")
    legend_eta.Draw()

    # Manually set axis limits for eta to ensure all histograms are visible
    proj_eta_no_cut.SetMaximum(max(proj_eta_no_cut.GetMaximum(), proj_eta_with_cut.GetMaximum(), proj_eta_with_cut_2.GetMaximum(), proj_eta_multiplied.GetMaximum()) * 1.1)
    proj_eta_no_cut.SetMinimum(0)

    # Plot the projections for phi
    c1.cd(3)
    proj_phi_no_cut.SetLineColor(ROOT.kRed)
    proj_phi_with_cut.SetLineColor(ROOT.kBlue)
    proj_phi_with_cut_2.SetLineColor(ROOT.kGreen)
    proj_phi_multiplied.SetLineColor(ROOT.kBlack)

    proj_phi_no_cut.Draw()
    proj_phi_with_cut.Draw("SAME")
    proj_phi_with_cut_2.Draw("SAME")
    proj_phi_multiplied.Draw("SAME")

    # Add legend for phi plot
    legend_phi = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    legend_phi.AddEntry(proj_phi_no_cut, "No Cut", "l")
    legend_phi.AddEntry(proj_phi_with_cut, "E > 50 GeV", "l")
    legend_phi.AddEntry(proj_phi_with_cut_2, "30 < E < 50 GeV", "l")
    legend_phi.AddEntry(proj_phi_multiplied, "E > 50 and 30 < E < 50", "l")
    legend_phi.Draw()

    # Manually set axis limits for phi to ensure all histograms are visible
    proj_phi_no_cut.SetMaximum(max(proj_phi_no_cut.GetMaximum(), proj_phi_with_cut.GetMaximum(), proj_phi_with_cut_2.GetMaximum(), proj_phi_multiplied.GetMaximum()) * 1.1)
    proj_phi_no_cut.SetMinimum(0)

    # Show the canvas
    c1.Update()
    c1.Draw()

    # Save the result as an image or file
    c1.SaveAs("3d_hist_projection_overlay_with_multiplication_fixed.png")

    # Close the ROOT file
    file.Close()

# ------------------------------------------------------------------------------
def main():

    infilepath = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv1_NoSel_scores_2025_02_03.root"
    label = "NoSel"

    GetData(infilepath, label)

if __name__ == '__main__':
	main()