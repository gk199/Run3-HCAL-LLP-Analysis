import ROOT

# Initialize ROOT
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPalette(ROOT.kCandy)

from ROOT import SetOwnership

from MisTagParametrization_3D import ProjectHistogram, LabelCMS, ResetAxis # DrawCanvasAndPlots, MakePlot

debug = False

# ------------------------------------------------------------------------------
def GetHistogram(file_path, hist_name_all, hist_name_mistag):
    # Function to read and process histograms from a single ROOT file
    # Open the ROOT file
    root_file = ROOT.TFile(file_path, "READ")
    
    # Get histograms from the ROOT file
    all_hist = root_file.Get(hist_name_all)
    mistag_hist = root_file.Get(hist_name_mistag)
    
    # Check if the histograms exist in the file
    if not all_hist or not mistag_hist: raise ValueError(f"Histograms not found in file: {file_path}")
        
    mistag_rate = mistag_hist.Clone("mistag_rate_" + file_path)
    ResetAxis(mistag_rate)
    SetOwnership(mistag_rate, False)
    proj_pT_mistag_rate, proj_eta_mistag_rate, proj_phi_mistag_rate = ProjectHistogram(mistag_rate, "Mistag rate")

    total = all_hist.Clone("total_"+file_path)
    proj_pT_total, proj_eta_total, proj_phi_total = ProjectHistogram(total, "Number of events")

    # Divide the projections: proj_mistag / proj_all
    proj_pT_mistag_rate.Divide(proj_pT_total)
    proj_eta_mistag_rate.Divide(proj_eta_total)
    proj_phi_mistag_rate.Divide(proj_phi_total)
    
    if debug:
        print(f"Histogram Name: {proj_pT_mistag_rate.GetName()}")
        print(f"Histogram Title: {proj_pT_mistag_rate.GetTitle()}")
        print(f"Number of Bins: {proj_pT_mistag_rate.GetNbinsX()}")
        for i in range(proj_pT_mistag_rate.GetNbinsX()): print(f"Bins Content: {proj_pT_mistag_rate.GetBinContent(i)}")
        print(f"Histogram Type: {type(proj_pT_mistag_rate)}")

        if isinstance(proj_pT_mistag_rate, ROOT.TH1D):
            print("This is a TH1D histogram.")
        elif isinstance(proj_pT_mistag_rate, ROOT.TH1F):
            print("This is a TH1F histogram.")

    SetOwnership(proj_pT_mistag_rate, False) # This makes sure ROOT owns the histogram, and it can be saved for accessing outside this function (scoping issue)

    return proj_pT_mistag_rate, proj_eta_mistag_rate, proj_phi_mistag_rate, root_file
        
# ------------------------------------------------------------------------------
def OverlayHistograms(file_paths, hist_name_all, hist_name_mistag):    
    # Function to overlay projections from multiple ROOT files
    # Create a list to store the projection ratios (for adding legends, etc.)
    pt_ratios = []
    eta_ratios = []
    phi_ratios = []
    root_files = []
    
    # Loop over all ROOT files and process them
    for file_path in file_paths:
        pt_ratio, eta_ratio, phi_ratio, root_file = GetHistogram(file_path, hist_name_all, hist_name_mistag)
        print("got histogram for file " + file_path)

        if not pt_ratio:
            print(f"Error: pt_ratio for {file_path} is invalid.")
            continue  # Skip this iteration if pt_ratio is invalid

        # Store the projection ratio for later (for adding a legend)
        pt_ratios.append(pt_ratio)
        eta_ratios.append(eta_ratio)
        phi_ratios.append(phi_ratio)
        root_files.append(root_file)
        
    legend_labels = ["2023 Bv1", "2023 Cv1", "2023 Cv2", "2023 Cv4"]
    png_title = "Overlay_LLPskim_Mistag"
    
    DrawCanvasAndPlots_overlay(
        "c1", "Projection plots", ": LLP skim, different eras", " in CR for different eras",
        [[pt_ratios[0], pt_ratios[1], pt_ratios[2], pt_ratios[3]], 
        [eta_ratios[0], eta_ratios[1], eta_ratios[2], eta_ratios[3]], 
        [phi_ratios[0], phi_ratios[1], phi_ratios[2], phi_ratios[3]]],  # Wrap each plot in a list
        legend_labels,
        png_title,
        ["Jet p_{T} Mistag", "Jet #eta Mistag", "Jet #phi Mistag"], "OverlayEra"
    )

    for i in range(len(root_files)): root_files[i].Close()

# ------------------------------------------------------------------------------
def DrawCanvasAndPlots_overlay(canvas_name, canvas_title, option, title, plots, legend_labels, save_name, plot_titles, label):
    # Create canvas
    canvas = ROOT.TCanvas(f"{canvas_name}_{option}", f"{canvas_title} for {option}", 2400, 600)
    canvas.Divide(3, 1)
    
    # Loop over the plots and generate them
    for i, plot_group in enumerate(plots):
        canvas.cd(i + 1)  # Navigate to the correct pad
        # MakePlot can take a list of histograms, so pass the group of histograms
        MakePlot_overlay(plot_group, legend_labels)
        
        # Set the title for the plot group
        plot_group[0].SetTitle(plot_titles[i] + title)  # Just set the title for the first plot in the group
    
    canvas.Update()
    canvas.Draw()
    canvas.SaveAs(f"outPlots_3D/{save_name}{label}.png")
    canvas.Clear()

# ------------------------------------------------------------------------------
def MakePlot_overlay(hists, legends):
    colors = [30, 38, 40, 46, 49]
    i = 0

    # Draw the histograms and overlay 
    for hist in hists:
        if len(hists) == 2 and i == 1: # Check if only two histograms are passed, and apply shading (for predicted and observed plots)
            hist.SetFillStyle(3004)
            hist.SetFillColor(colors[i])
        hist.SetLineColor(colors[i])
        hist.SetLineWidth(2)
        i += 1
    if len(hists) == 1: hists[0].Draw("HIST") # huge error bars for rate plots -- why?
    else: hists[0].Draw("HIST")
    for hist in hists[1:]:
        hist.Draw("SAME HIST")

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
if __name__ == "__main__":
    # List of ROOT files to process
    root_files = ["output_3D_hists_depth_combined_2023Bv1.root", "output_3D_hists_depth_combined_2023Cv1.root", "output_3D_hists_depth_combined_2023Cv2.root", "output_3D_hists_depth_combined_2023Cv3.root"]

    # Names of the histograms in the ROOT files
    hist_name_all = "hist3d_CR_all_combined"
    hist_name_mistag = "hist3d_CR_mistag_combined"
    
    # Overlay histograms from the files
    OverlayHistograms(root_files, hist_name_all, hist_name_mistag)