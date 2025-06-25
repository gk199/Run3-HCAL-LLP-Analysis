import uproot
import numpy as np
import ROOT
import argparse
import re

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0) # ROOT style options

# load variables 
variables = [
    "jet0_scores_inc_train80",
    "jet0_scores_inc_train40",
    "jet0_InclTagCand",
    "jet0_DepthTagCand",
    "jet1_DepthTagCand",
    "jet0_Pt",
    "Pass_WPlusJets",
    "jet0_isTruthMatched",
    "weight"
]

def load_combined_data(file_paths):
    # Loop through files and collect data
    combined = {var: [] for var in variables}
    for path in file_paths:
        print(f"... loading {path}")
        with uproot.open(path) as f:
            arr = f["NoSel"].arrays(variables, library="np")
            for var in variables:
                combined[var].append(arr[var])
    for var in variables: # concatenate arrays from all files
        combined[var] = np.concatenate(combined[var])
    return combined

def get_masks_and_selections(data, mode):
    pt = data["jet0_Pt"]
    third_dec = (np.floor(pt * 1000).astype(int)) % 10

    mask80 = (data["jet0_scores_inc_train80"] > -1) & (third_dec >= 8)
    mask40 = (data["jet0_scores_inc_train40"] > -1) & (third_dec >= 4)

    if mode == "background":
        print("Applying W+jets requirements")
        truth = (data["Pass_WPlusJets"] == 1)
    else:
        print("Applying LLP matching requirements")
        truth = (data["jet0_isTruthMatched"] == 1)

    # Selection masks
    sel0 = data["jet0_InclTagCand"] 
    sel1 = sel0 & data["jet1_DepthTagCand"]
    sel2 = truth # apply mask80 or 40, with truth matching requirements too
    if mode == "signal": sel2 &= sel0 & sel1 # for LLP MC, also require jet0 and jet 1 are tag candidatates

    selections = [sel0, sel1, sel2]
    if mode == "background":
        labels = [
            "prompt jet (background)",
            "jet0 !DepthTagCand (background)",
            "jet0 !DepthTagCand && jet1 DepthTagCand (background)"
        ]
    else:
        labels = [
            "LLP matched jet (signal)",
            "jet0 InclTagCand (signal)",
            "jet0 InclTagCand && jet1 DepthTagCand (signal)"
        ]
    return mask80, mask40, selections, labels

def print_mask_stats(data, mode):
    third_dec = np.floor(data["jet0_Pt"] * 1000).astype(int) % 10

    # Base score cuts
    mask_score80 = data["jet0_scores_inc_train80"] > -1
    mask_score40 = data["jet0_scores_inc_train40"] > -1
    print("Total entries with valid scores:")
    print(f"  train80 > -1: {np.sum(mask_score80)}")
    print(f"  train40 > -1: {np.sum(mask_score40)}")

    # After exclusion based on 3rd decimal digit
    mask_excl80 = mask_score80 & (third_dec >= 8)
    mask_excl40 = mask_score40 & (third_dec >= 4)
    print("\nAfter training exclusion based on third decimal digit:")
    print(f"  train80 (>=8): {np.sum(mask_excl80)}")
    print(f"  train40 (>=4): {np.sum(mask_excl40)}")

    # After matching (either truth matching or W+jets)
    if "background":
        match_mask = data["Pass_WPlusJets"] == 1
        print(f"\nBackground (W+jets) selected with Pass_WPlusJets: {np.sum(match_mask)}")
    else:
        match_mask = data["jet0_isTruthMatched"] == 1
        print(f"\nSignal selected with jet0_isTruthMatched == 1: {np.sum(match_mask)}")

    mask_final80 = mask_excl80 & match_mask
    mask_final40 = mask_excl40 & match_mask

    print("\nAfter applying physics matching and training mask:")
    print(f"  train80 final: {np.sum(mask_final80)}")
    print(f"  train40 final: {np.sum(mask_final40)}")

    # Check overlap (should be same as train 80)
    overlap = np.sum(mask_final80 & mask_final40)
    print(f"\n  Overlap between train80 and train40 (should be same as train80): {overlap}")

    # Optional: print distribution of third decimal digit
    print("\nDistribution of third decimal digit of jet0_Pt * 1000:")
    for i in range(10):
        count = np.sum(third_dec == i)
        print(f"  Digit {i}: {count}")

def plot_train_comparison(scores80, scores40, weights80, weights40, step_label, out_prefix, normalize = False):
    h80 = ROOT.TH1F(f"h80_{out_prefix}", "", 50, 0, 1)
    h40 = ROOT.TH1F(f"h40_{out_prefix}", "", 50, 0, 1)
    for val, w in zip(scores80, weights80): h80.Fill(val, w)
    for val, w in zip(scores40, weights40): h40.Fill(val, w)

    if normalize:
        if h80.Integral() > 0: h80.Scale(1.0 / h80.Integral())
        if h40.Integral() > 0: h40.Scale(1.0 / h40.Integral())

    # Set line colors and styles
    h80.SetLineColor(ROOT.kBlue); h80.SetLineWidth(2)
    h40.SetLineColor(ROOT.kGreen+2); h40.SetLineWidth(2)

    # Determine maximum y for axis range
    max_y = max(h80.GetMaximum(), h40.GetMaximum())
    h80.SetMaximum(max_y * 1.3)  # leave extra room

    # Draw on canvas
    c = ROOT.TCanvas(f"c_{out_prefix}", "", 800, 600)
    c.SetLogy()
    h80.SetTitle(f"{step_label};Score;Entries")
    h80.Draw("HIST")
    h40.Draw("HIST SAME")

    # Legend
    leg = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
    leg.AddEntry(h80, "Train 80%", "l")
    leg.AddEntry(h40, "Train 40%", "l")
    leg.Draw()

    # Save canvas
    c.SaveAs(f"{out_prefix}.png")
    c.Close()
    print(f"Saved {out_prefix}.png")

def plot_signal_vs_background(sig_scores, bkg_scores, sig_weights, title, out_prefix, normalize = False):
    hs = ROOT.TH1F(f"hs_{out_prefix}", "", 50, 0, 1)
    hb = ROOT.TH1F(f"hb_{out_prefix}", "", 50, 0, 1)
    for val, w in zip(sig_scores, sig_weights):
        hs.Fill(val, w)
    for val in bkg_scores:
        hb.Fill(val)

    if normalize:
        if hs.Integral() > 0: hs.Scale(1.0 / hs.Integral())
        if hb.Integral() > 0: hb.Scale(1.0 / hb.Integral())

    hs.SetLineColor(ROOT.kRed); hs.SetLineWidth(2)
    hb.SetLineColor(ROOT.kBlack); hb.SetLineStyle(2); hb.SetLineWidth(2)

    max_y = max(hs.GetMaximum(), hb.GetMaximum())
    hs.SetMaximum(max_y * 1.3)

    c = ROOT.TCanvas(f"c_{out_prefix}", "", 800, 600)
    c.SetLogy()
    hs.SetTitle(f"{title};Score;Entries")
    hs.Draw("HIST"); hb.Draw("HIST SAME")

    leg = ROOT.TLegend(0.6, 0.75, 0.88, 0.88)
    leg.AddEntry(hs, "Signal (train80)", "l")
    leg.AddEntry(hb, "Background (train80)", "l")
    leg.Draw()

    c.SaveAs(f"{out_prefix}_overlay.png")
    c.Close()
    print(f"Saved {out_prefix}_overlay.png")

def extract_dataset_id(filename):
    """Extract short identifier like '2023Cv1' from the filename"""
    match = re.search(r'20\d{2}[A-Z][a-z0-9]+', filename)
    return match.group(0) if match else filename.split("/")[-1]

def plot_multiple_background_files(background_files, normalize=False, use_train40=False, step=0):
    var_name = "jet0_scores_inc_train40" if use_train40 else "jet0_scores_inc_train80"
    ROOT.gStyle.SetPalette(ROOT.kRainBow)

    hist_list = []
    legends = []
    
    c = ROOT.TCanvas("c_multibkg", "", 800, 600)
    c.SetLogy()
    
    max_y = 0

    # Need one file to get the selection label
    temp_data = load_combined_data([background_files[0]])
    _, _, selections, labels = get_masks_and_selections(temp_data, "background")
    title_label = f"Step {step+1}: {labels[step]}"

    for idx, path in enumerate(background_files):
        data = load_combined_data([path])
        mask80, _, selections, _ = get_masks_and_selections(data, "background")

        sel_mask = selections[step]
        scores = data[var_name][mask80 & sel_mask]

        h = ROOT.TH1F(f"h_{idx}", "", 50, 0, 1)
        for val in scores:
            h.Fill(val)

        if normalize and h.Integral() > 0:
            h.Scale(1.0 / h.Integral())

        color = ROOT.TColor.GetColorPalette(idx * int(255 / len(background_files)))
        h.SetLineColor(color)
        h.SetLineWidth(2)
        h.SetTitle(f"{title_label};Score;{'Normalized Entries' if normalize else 'Entries'}")
        
        max_y = max(max_y, h.GetMaximum())
        hist_list.append(h)
        short_label = extract_dataset_id(path)
        legends.append(short_label)

    # Draw all histograms
    for i, h in enumerate(hist_list):
        h.SetMaximum(max_y * 1.3)
        draw_opt = "HIST" if i == 0 else "HIST SAME"
        h.Draw(draw_opt)

    # Legend
    legend = ROOT.TLegend(0.55, 0.65, 0.88, 0.88)
    for h, lbl in zip(hist_list, legends):
        legend.AddEntry(h, lbl, "l")
    legend.Draw()

    suffix = "_norm" if normalize else ""
    suffix += "_train40" if use_train40 else "_train80"
    c.SaveAs(f"multiple_background_overlay_step{step+1}{suffix}.png")
    c.Close()
    print(f"Saved multiple_background_overlay_step{step+1}{suffix}.png")

def main(mode):
    assert mode in ("signal", "background", "overlay", "background_overlay")

    # define file lists
    signal_files = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_125_15_CTau10000_scores_50percent.root",]
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_125_15_CTau3000_scores_50percent.root",     
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_125_50_CTau10000_scores_50percent.root",     
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_125_50_CTau3000_batch1_scores_50percent.root",     
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_125_50_CTau3000_batch2_scores_50percent.root",     
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_250_120_CTau10000_batch1_scores_50percent.root",     
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_250_120_CTau10000_batch2_scores_50percent.root",     
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_350_160_CTau10000_batch1_scores_50percent.root",     
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_350_160_CTau10000_batch2_scores_50percent.root",     
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_350_80_CTau500_scores_50percent.root"]
    background_files = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Bv1_scores_50percent.root",]
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Cv1_scores_50percent.root",
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Cv2_scores_50percent.root",
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Cv3_scores_50percent.root",
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Cv4_scores_50percent.root",
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Dv1_partial.root",
                # "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Dv2_partial.root"]

    if mode in ("signal", "background"):
        files = background_files if mode == "background" else signal_files
        data = load_combined_data(files)
        print_mask_stats(data, mode) # do some sanity checks
        mask80, mask40, sels, labels = get_masks_and_selections(data, mode)

        for i, (sel, label) in enumerate(zip(sels, labels), start=1):
            s80 = data["jet0_scores_inc_train80"][mask80 & sel]
            s40 = data["jet0_scores_inc_train40"][mask40 & sel]
            weights80 = data["weight"][mask80 & sel]
            weights40 = data["weight"][mask40 & sel]
            prefix = f"{mode}_step{i}"
            plot_train_comparison(s80, s40, weights80, weights40, f"Step {i}: {label}", prefix, normalize=args.normalize)

    elif mode == "overlay":
        print("Overlay, loading signal...")
        sig = load_combined_data(signal_files)
        print("Overlay, loading background...")
        bkg = load_combined_data(background_files)
        m80_s, _, sels_s, _ = get_masks_and_selections(sig, "signal")
        m80_b, _, sels_b, _ = get_masks_and_selections(bkg, "background")

        for step in range(3): # choose which step (1/2/3)
            ss = sig["jet0_scores_inc_train80"][m80_s & sels_s[step-1]]
            bb = bkg["jet0_scores_inc_train80"][m80_b & sels_b[step-1]]
            weights_sig = sig["weight"][m80_s & sels_s[step-1]]
            plot_signal_vs_background(ss, bb, weights_sig, f"Step {step}", f"signal_vs_background_step{step}", normalize=args.normalize)

    elif mode == "background_overlay":
        for step in range(3):  # step 0, 1, 2
            plot_multiple_background_files(background_files, normalize=args.normalize, use_train40=False, step=step)

if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("-m", "--mode", choices=["signal", "background", "overlay", "background_overlay"], default="signal")
    p.add_argument("--normalize", action="store_true", help="normalize histograms")
    p.add_argument("--train40", action="store_true", help="use jet0_scores_inc_train40 instead of train80")

    args = p.parse_args()
    main(args.mode)