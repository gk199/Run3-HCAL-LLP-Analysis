import ROOT
import numpy as np

# Initialize ROOT
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPalette(ROOT.kCandy)

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

    # Fill the histograms using tree.Draw() with cuts
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> hist3d_CR_all", "", "")
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> hist3d_CR_mistag", "jet1_scores_inc >= 0 && jet1_scores_inc < 0.5", "")
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> hist3d_CR_mistag_2", "jet1_scores_inc >= 0 && jet1_scores_inc < 0.7", "")
    tree.Draw("jet0_Phi:jet0_Eta:jet0_Pt >> hist3d_multiplied", "jet1_scores_inc >= 0 && jet1_scores_inc < 0.9", "")

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

    # Plot the projections for pT
    c1.cd(1)
    MakePlot([proj_pT_no_cut, proj_pT_with_cut, proj_pT_with_cut_2, proj_pT_multiplied], ["no cuts", "low DNN", "mid DNN", "high DNN"])

    # Plot the projections for eta
    c1.cd(2)
    MakePlot([proj_eta_no_cut, proj_eta_with_cut, proj_eta_with_cut_2, proj_eta_multiplied], ["no cuts", "low DNN", "mid DNN", "high DNN"])

    # proj_eta_no_cut.SetLineColor(ROOT.kRed)
    # proj_eta_with_cut.SetLineColor(ROOT.kBlue)
    # proj_eta_with_cut_2.SetLineColor(ROOT.kGreen)
    # proj_eta_multiplied.SetLineColor(ROOT.kBlack)

    # proj_eta_no_cut.Draw()
    # proj_eta_with_cut.Draw("SAME")
    # proj_eta_with_cut_2.Draw("SAME")
    # proj_eta_multiplied.Draw("SAME")

    # # Add legend for eta plot
    # legend_eta = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    # legend_eta.AddEntry(proj_eta_no_cut, "No Cut", "l")
    # legend_eta.AddEntry(proj_eta_with_cut, "E > 50 GeV", "l")
    # legend_eta.AddEntry(proj_eta_with_cut_2, "30 < E < 50 GeV", "l")
    # legend_eta.AddEntry(proj_eta_multiplied, "E > 50 and 30 < E < 50", "l")
    # legend_eta.Draw()

    # # Manually set axis limits for eta to ensure all histograms are visible
    # proj_eta_no_cut.SetMaximum(max(proj_eta_no_cut.GetMaximum(), proj_eta_with_cut.GetMaximum(), proj_eta_with_cut_2.GetMaximum(), proj_eta_multiplied.GetMaximum()) * 1.1)
    # proj_eta_no_cut.SetMinimum(0)

    # Plot the projections for phi
    c1.cd(3)
    MakePlot([proj_phi_no_cut, proj_phi_with_cut, proj_phi_with_cut_2, proj_phi_multiplied], ["no cuts", "low DNN", "mid DNN", "high DNN"])
    # proj_phi_no_cut.SetLineColor(ROOT.kRed)
    # proj_phi_with_cut.SetLineColor(ROOT.kBlue)
    # proj_phi_with_cut_2.SetLineColor(ROOT.kGreen)
    # proj_phi_multiplied.SetLineColor(ROOT.kBlack)

    # proj_phi_no_cut.Draw()
    # proj_phi_with_cut.Draw("SAME")
    # proj_phi_with_cut_2.Draw("SAME")
    # proj_phi_multiplied.Draw("SAME")

    # # Add legend for phi plot
    # legend_phi = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
    # legend_phi.AddEntry(proj_phi_no_cut, "No Cut", "l")
    # legend_phi.AddEntry(proj_phi_with_cut, "E > 50 GeV", "l")
    # legend_phi.AddEntry(proj_phi_with_cut_2, "30 < E < 50 GeV", "l")
    # legend_phi.AddEntry(proj_phi_multiplied, "E > 50 and 30 < E < 50", "l")
    # legend_phi.Draw()

    # # Manually set axis limits for phi to ensure all histograms are visible
    # proj_phi_no_cut.SetMaximum(max(proj_phi_no_cut.GetMaximum(), proj_phi_with_cut.GetMaximum(), proj_phi_with_cut_2.GetMaximum(), proj_phi_multiplied.GetMaximum()) * 1.1)
    # proj_phi_no_cut.SetMinimum(0)

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

    # Update the canvas to ensure the legend is drawn
    ROOT.gPad.Update()
# ------------------------------------------------------------------------------
def main():

    infilepath = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv1_NoSel_scores_2025_02_03.root"
    label = "NoSel"

    GetData(infilepath, label)

if __name__ == '__main__':
	main()