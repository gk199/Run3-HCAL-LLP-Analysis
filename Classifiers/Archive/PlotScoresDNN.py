import uproot
import numpy as np
import ROOT

ROOT.gROOT.SetBatch(True)

WPlusJets = False # is this W+jets from LLP skim or LLP MC?

# list of filepaths
if WPlusJets: 
    file_paths = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Bv1_scores_50percent.root",
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Cv1_scores_50percent.root",
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Cv2_scores_50percent.root",
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Cv3_scores_50percent.root",
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Cv4_scores_50percent.root",
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Dv1_partial.root",
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Dv2_partial.root"]      
else: 
    file_paths = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_125_15_CTau10000_scores_50percent.root",
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_125_15_CTau3000_scores_50percent.root",     
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_125_50_CTau10000_scores_50percent.root",     
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_125_50_CTau3000_batch1_scores_50percent.root",     
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_125_50_CTau3000_batch2_scores_50percent.root",     
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_250_120_CTau10000_batch1_scores_50percent.root",     
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_250_120_CTau10000_batch2_scores_50percent.root",     
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_350_160_CTau10000_batch1_scores_50percent.root",     
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_350_160_CTau10000_batch2_scores_50percent.root",     
                "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_350_80_CTau500_scores_50percent.root"]

# Load variables
variables = [
    "jet0_scores_inc_train80",
    "jet0_scores_inc_train40",
    "jet0_InclTagCand",
    "jet0_DepthTagCand",
    "jet1_DepthTagCand",
    "jet0_Pt",
    "Pass_WPlusJets",
    "jet0_isTruthMatched"  
]

# Initialize dictionary to store all arrays
combined_data = {var: [] for var in variables}

# Loop through files and collect data
for path in file_paths:
    print(f"Processing {path}")
    with uproot.open(path) as file:
        tree = file["NoSel"]
        arrays = tree.arrays(variables, library="np")
        for var in variables:
            combined_data[var].append(arrays[var])

# Concatenate arrays from all files
for var in variables:
    combined_data[var] = np.concatenate(combined_data[var])

# Base cuts (value > -1)
mask_train80 = combined_data["jet0_scores_inc_train80"] > -1
mask_train40 = combined_data["jet0_scores_inc_train40"] > -1

# Extract third decimal digit to exclude jets used in training
jet0_Pt = combined_data["jet0_Pt"]
third_decimal_digit = np.floor(jet0_Pt * 1000).astype(int) % 10

if WPlusJets:
    mask_train80 = mask_train80 & (third_decimal_digit >= 8) & combined_data["Pass_WPlusJets"]
    mask_train40 = mask_train40 & (third_decimal_digit >= 4) & combined_data["Pass_WPlusJets"]
else:
    mask_train80 = mask_train80 & (third_decimal_digit >= 8) & (combined_data["jet0_isTruthMatched"] == 1)
    mask_train40 = mask_train40 & (third_decimal_digit >= 4) & (combined_data["jet0_isTruthMatched"] == 1)

# Progressive selection masks
sel0 = np.ones_like(combined_data["jet0_Pt"], dtype=bool)
sel1 = combined_data["jet0_InclTagCand"]
if WPlusJets: sel1 = (combined_data["jet0_DepthTagCand"] == 0) # for background, require not depth tag candidate instead
sel2 = sel1 & combined_data["jet1_DepthTagCand"]

selections = [sel0, sel1, sel2]
labels = ["Scores, for LLP matched jet (signal)",
    "Scores, with jet0 InclTagCand (signal)",
    "Scores, with jet0 InclTagCand && jet1 DepthTagCand (signal)"]
if WPlusJets: labels = ["Scores, for prompt jet (background)",
    "Scores, with jet0 !DepthTagCand (background)",
    "Scores, with jet0 !DepthTagCand && jet1 DepthTagCand (background)"]

# ROOT style options
ROOT.gStyle.SetOptStat(0)

for i, (sel, label) in enumerate(zip(selections, labels)):
    # Apply selections
    final_mask80 = mask_train80 & sel
    final_mask40 = mask_train40 & sel

    scores80 = combined_data["jet0_scores_inc_train80"][final_mask80]
    scores40 = combined_data["jet0_scores_inc_train40"][final_mask40]

    # Create histograms in memory
    h80 = ROOT.TH1F(f"h80_step{i+1}", "jet0_scores_inc_train80", 50, 0, 1)
    h40 = ROOT.TH1F(f"h40_step{i+1}", "jet0_scores_inc_train40", 50, 0, 1)

    for val in scores80:
        h80.Fill(val)
    for val in scores40:
        h40.Fill(val)

    # Set line colors and styles (unfilled)
    h80.SetLineColor(ROOT.kBlue)
    h80.SetLineWidth(2)
    h40.SetLineColor(ROOT.kGreen+2)
    h40.SetLineWidth(2)

    # Draw on canvas
    c = ROOT.TCanvas(f"c_step{i+1}", f"Step {i+1}", 800, 600)
    c.SetLogy()
    h80.SetTitle(f"Step {i+1}: {label};Score;Entries")

    h80.Draw("HIST")
    h40.Draw("HIST SAME")

    # Legend
    legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
    legend.AddEntry(h80, "Train 80%", "l")
    legend.AddEntry(h40, "Train 40%", "l")
    legend.Draw()

    # Save canvas
    label = "LLP_"
    if WPlusJets: label = "WPlusJets_"
    c.SaveAs(f"{label}jet0_scores_comparison_step{i+1}.png")
    c.Close()

    print(f"Saved: {label}jet0_scores_comparison_step{i+1}.png")