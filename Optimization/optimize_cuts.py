import ROOT
import sys
import re
import subprocess
import argparse
import os 
import json 
import numpy as np
import math
import platform

import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

cwd = os.getcwd()

# ------------------------------------------------------------------------------
# Modify Me!

# ----- Calculate Results Inputs ----- # 

# NB: The following params *only* control inputs to the calculate_results function.

# Inputs:
root_file_path = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples"
version        = "v5.6" # Corresponds to directory name

# Binning
n_bins_global = 100
score_range_incl_global  = [0, 1]
score_range_depth_global = [0, 1]

# Score Mode: Use combination inclusive+depth ("Normal"), or only inclusive ("InclusiveOnly"), or only depth ("DepthOnly") to define analysis regions
score_mode_global = "Normal" # "InclusiveOnly", "DepthOnly"

# ----- Plot Results Inputs ----- # 

# NB: The following params *only* control inputs to the plot_results function.

# Require 2022 and 2023 to have same score selections
combine_years = True 

# Require LJDC and SJDC to have same score selections
# Not implemented # combine_categories = False

# ------------------------------------------------------------------------------
def parseArgs():
    """ Parse command-line arguments
    """
    parser = argparse.ArgumentParser(
        add_help=True,
        description=''
    )

    parser.add_argument("--sig22", action="store", help="Signal tag for 2022 (example: HToSSTo4B_125_50_CTau3000_2022)", required=True)
    parser.add_argument("--sig23", action="store", help="Signal tag for 2023 (example: HToSSTo4B_125_50_CTau3000_2023)", required=True)
    parser.add_argument("-s", "--settings-tag",    action="store", help="Input parameter tag", default="test", required=False)
    parser.add_argument("-c", "--calculate",       action="store_true", help="Run calculation of optimal SR cuts", default=False, required=False)
    parser.add_argument("-p", "--plot",            action="store_true", help="Plot calculation of optimal SR cuts", default=False, required=False)
    parser.add_argument("-t", "--print-info",      action="store_true", help="Print information to terminal", default=False, required=False)

    args = parser.parse_args()

    return args

# ------------------------------------------------------------------------------
def calculate_bkg_prediction( h2, i_bin, j_bin, n_bins):

    #h2["n_bkgpred_{0}_{1}".format(cat, year)], i_bin, j_bin, n_bins)

    i_cr = h2.FindBin( 0.2 ) # Control region inclusive candidate score cut

    nevents_bkg_cr       = h2.Integral( 1, i_cr, 1, h2.GetNbinsY() )
    nevents_bkg_cr_depth = h2.Integral( 1, i_cr, j_bin, h2.GetNbinsY() )
    nevents_bkg_sr_temp  = h2.Integral( i_bin, h2.GetNbinsX(), 1, h2.GetNbinsY() )

    ratio = (nevents_bkg_cr_depth / nevents_bkg_cr) if nevents_bkg_cr > 0 else 0.0

    nevents_bkg_srpred = nevents_bkg_sr_temp * ratio

    return nevents_bkg_srpred

# ------------------------------------------------------------------------------
def calculate_significance(n_signal, n_background):

    # Scalars
    if np.isscalar(n_signal) and np.isscalar(n_background):

        if n_background <= 0: return 0

        Z = math.sqrt( -2*( n_background*math.log(1+n_signal/n_background) - n_signal ) )
        Zprime = n_signal/math.sqrt( (n_signal/Z)**2. + n_signal + n_background )
        return Zprime

    # Array
    else:
        mask = (n_background > 0) & (n_signal > 0)

        s = n_signal[mask]
        b = n_background[mask]

        Zprime = np.zeros_like(np.broadcast_arrays(n_signal, n_background)[0], dtype=float)

        Z = np.sqrt(-2.0 * (b * np.log(1.0 + s / b) - s))
        #print(s, Z, b)
        Zprime[mask] = s / np.sqrt((s / Z) ** 2. + s + b)
        #quit()

        return Zprime

# ------------------------------------------------------------------------------
def read_filetags( arg ):
    """Return list of filetags from a .txt file (one tag per line) or a single tag string."""
    if arg.endswith('.txt') and os.path.isfile(arg):
        with open(arg) as f:
            return [line.strip() for line in f if line.strip() and not line.startswith('#')]
    return [arg]

# ------------------------------------------------------------------------------
def calculate_results( filetag, signaltag_22, signaltag_23 ):

    print( "Running calculate_results for", filetag, signaltag_22, signaltag_23 )

    # ----- Process Inputs ----- #

    signaltags_22 = read_filetags(signaltag_22)
    signaltags_23 = read_filetags(signaltag_23)

    # Local inputs for testing
    infile_data_22 = "../Files/MiniTuples/v5.1/minituple_data_2022Ev1_scores.root"
    infile_data_23 = "../Files/MiniTuples/v5.1/minituple_data_2023Dv2_scores.root"

    # All inputs on lxplus
    if platform.system() == "Linux": # On lxplus
        infile_data_22 = ""
        infile_data_23 = ""

    infiles_data_22 = [
        "{}/{}/minituple_data_2022Cv1_scores.root".format(root_file_path, version),
        "{}/{}/minituple_data_2022Dv1_scores.root".format(root_file_path, version),
        "{}/{}/minituple_data_2022Ev1_scores.root".format(root_file_path, version),
        "{}/{}/minituple_data_2022Fv1_scores.root".format(root_file_path, version),
        "{}/{}/minituple_data_2022Gv1_scores.root".format(root_file_path, version),
        # 2023, pre-BPix:
        "{}/{}/minituple_data_2023Cv1_scores.root".format(root_file_path, version),
        "{}/{}/minituple_data_2023Cv2_scores.root".format(root_file_path, version),
        "{}/{}/minituple_data_2023Cv3_scores.root".format(root_file_path, version),
        "{}/{}/minituple_data_2023Cv4_scores.root".format(root_file_path, version),
     ]

    infiles_data_23 = [
        "{}/{}/minituple_data_2023Dv1_scores.root".format(root_file_path, version),
        "{}/{}/minituple_data_2023Dv2_scores.root".format(root_file_path, version),
    ]

    # ----- Read in Data (Background Prediction) ----- #

    # TODO: Eventually should use Gillian's background estimation

    print("Reading in data tree... (this may take a few minutes)")

    # if using a partial dataset, how much to scale this up by
    lumi_sf_22 = 34.76 / 5.81
    lumi_sf_23 = 28.28 / (0.21*9.68)

    lumi_2022 = 7.9895 + 26.6749 + 17.9642      # 34.76 (just 2022)
    lumi_2023 = 9.67674                         # 28.28 (just 2023)
    lumi_total = lumi_2022 + lumi_2023

    tree_data_22 = ROOT.TChain("NoSel")
    if infile_data_22 != "":
        print("Adding", infile_data_22, "to chain")
        tree_data_22.Add(infile_data_22)
    else:
        lumi_sf_22 = 1
        for infile in infiles_data_22:
            print("Adding", infile, "to chain")
            tree_data_22.Add(infile)

    tree_data_23 = ROOT.TChain("NoSel")
    if infile_data_23 != "":
        print("Adding", infile_data_23, "to chain")
        tree_data_23.Add(infile_data_23)
    else:
        lumi_sf_23 = 1
        # lumi_sf_23 = 1.267 # TODO check why this was needed, is it for Cv1? 
        print( "Reading in lumi_sf_23 as", lumi_sf_23 )
        for infile in infiles_data_23:
            print("Adding", infile, "to chain")
            tree_data_23.Add(infile)

    # ----- Read in Signal ----- #

    print("Reading in signal trees...")
    tree_sig_22 = ROOT.TChain("NoSel")
    tree_sig_23 = ROOT.TChain("NoSel")

    if platform.system() == "Linux":
        for tag in signaltags_22:
            path = "{}/{}/minituple_{}_scores.root".format(root_file_path, version, tag)
            print("Opening 2022:", path)
            tree_sig_22.Add(path)
        for tag in signaltags_23:
            path = "{}/{}/minituple_{}_scores.root".format(root_file_path, version, tag)
            print("Opening 2023:", path)
            tree_sig_23.Add(path)
    else:
        tree_sig_22.Add("../Files/MiniTuples/v5.1.1/minituple_HToSSTo4B_125_50_CTau3000_scores.root")
        tree_sig_23.Add("../Files/MiniTuples/v5.1.1/minituple_HToSSTo4B_125_50_CTau3000_scores.root")

    outfile_temp = ROOT.TFile("deleteme_skimtemp_{0}.root".format(filetag),"RECREATE")
    outfile_temp.cd()

    # ----- Scan Over Scores ----- #

    n_bins = n_bins_global
    score_range_incl  = score_range_incl_global
    score_range_depth = score_range_depth_global
    incl_edges  = np.linspace(score_range_incl[0],  score_range_incl[1],  n_bins + 1)
    depth_edges = np.linspace(score_range_depth[0], score_range_depth[1], n_bins + 1)
    incl_centers  = 0.5 * (incl_edges[:-1]  + incl_edges[1:])
    depth_centers = 0.5 * (depth_edges[:-1] + depth_edges[1:])

    # Define Arrays and Histograms

    array2d = {}
    h2 = {}

    for year in ["2022", "2023", "Total"]:
        for cat in ["ljdc", "sjdc"]:
            array2d["n_bkgpred_{0}_{1}".format(cat, year)] = np.empty((n_bins, n_bins))
            array2d["n_signal_{0}_{1}".format(cat, year)]  = np.empty((n_bins, n_bins))

            h2["n_bkgpred_{0}_{1}".format(cat, year)] = ROOT.TH2F("n_bkgpred_{0}_{1}".format(cat, year), "Number of Background Events; Inclusive Score; Depth Score", n_bins, score_range_incl[0], score_range_incl[-1], n_bins, score_range_depth[0], score_range_depth[-1])
            h2["n_signal_{0}_{1}".format(cat, year)] = ROOT.TH2F("n_signal_{0}_{1}".format(cat, year), "Number of Signal Events; Inclusive Score; Depth Score", n_bins, score_range_incl[0], score_range_incl[-1], n_bins, score_range_depth[0], score_range_depth[-1])

    selections = {}
    selections["all"] = "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && abs(jet0_jet1_dPhi) > 0.2"
    selections["ljdc"] = selections["all"] + " && jet0_DepthTagCand == 1"
    selections["sjdc"] = selections["all"] + " && jet1_DepthTagCand == 1"

    # Fill Hists 
    score_mode = score_mode_global

    if score_mode == "Normal":
        tree_data_22.Draw( "jet0_scores_depth_LLPanywhere:jet1_scores_inc_train80 >> n_bkgpred_{0}_{1}".format("ljdc", "2022"), selections["ljdc"] )
        tree_data_22.Draw( "jet1_scores_depth_LLPanywhere:jet0_scores_inc_train80 >> n_bkgpred_{0}_{1}".format("sjdc", "2022"), selections["sjdc"] )
        tree_data_23.Draw( "jet0_scores_depth_LLPanywhere:jet1_scores_inc_train80 >> n_bkgpred_{0}_{1}".format("ljdc", "2023"), selections["ljdc"] )
        tree_data_23.Draw( "jet1_scores_depth_LLPanywhere:jet0_scores_inc_train80 >> n_bkgpred_{0}_{1}".format("sjdc", "2023"), selections["sjdc"] )

        tree_sig_22.Draw( "jet0_scores_depth_LLPanywhere:jet1_scores_inc_train80 >> n_signal_{0}_{1}".format("ljdc", "2022"), "(event_weight * weight) * ({})".format(selections["ljdc"]) )
        tree_sig_22.Draw( "jet1_scores_depth_LLPanywhere:jet0_scores_inc_train80 >> n_signal_{0}_{1}".format("sjdc", "2022"), "(event_weight * weight) * ({})".format(selections["sjdc"]) )
        tree_sig_23.Draw( "jet0_scores_depth_LLPanywhere:jet1_scores_inc_train80 >> n_signal_{0}_{1}".format("ljdc", "2023"), "(event_weight * weight) * ({})".format(selections["ljdc"]) )
        tree_sig_23.Draw( "jet1_scores_depth_LLPanywhere:jet0_scores_inc_train80 >> n_signal_{0}_{1}".format("sjdc", "2023"), "(event_weight * weight) * ({})".format(selections["sjdc"]) )

    elif score_mode == "InclusiveOnly":
        tree_data_22.Draw( "jet0_scores_inc_train80:jet1_scores_inc_train80 >> n_bkgpred_{0}_{1}".format("ljdc", "2022"), selections["ljdc"] )
        tree_data_22.Draw( "jet1_scores_inc_train80:jet0_scores_inc_train80 >> n_bkgpred_{0}_{1}".format("sjdc", "2022"), selections["sjdc"] )
        tree_data_23.Draw( "jet0_scores_inc_train80:jet1_scores_inc_train80 >> n_bkgpred_{0}_{1}".format("ljdc", "2023"), selections["ljdc"] )
        tree_data_23.Draw( "jet1_scores_inc_train80:jet0_scores_inc_train80 >> n_bkgpred_{0}_{1}".format("sjdc", "2023"), selections["sjdc"] )

        tree_sig_22.Draw( "jet0_scores_inc_train80:jet1_scores_inc_train80 >> n_signal_{0}_{1}".format("ljdc", "2022"), "(event_weight * weight) * ({})".format(selections["ljdc"]) )
        tree_sig_22.Draw( "jet1_scores_inc_train80:jet0_scores_inc_train80 >> n_signal_{0}_{1}".format("sjdc", "2022"), "(event_weight * weight) * ({})".format(selections["sjdc"]) )
        tree_sig_23.Draw( "jet0_scores_inc_train80:jet1_scores_inc_train80 >> n_signal_{0}_{1}".format("ljdc", "2023"), "(event_weight * weight) * ({})".format(selections["ljdc"]) )
        tree_sig_23.Draw( "jet1_scores_inc_train80:jet0_scores_inc_train80 >> n_signal_{0}_{1}".format("sjdc", "2023"), "(event_weight * weight) * ({})".format(selections["sjdc"]) )

    elif score_mode == "DepthOnly":
        tree_data_22.Draw( "jet0_scores_depth_LLPanywhere:jet1_scores_depth_LLPanywhere >> n_bkgpred_{0}_{1}".format("ljdc", "2022"), selections["ljdc"] )
        tree_data_22.Draw( "jet1_scores_depth_LLPanywhere:jet0_scores_depth_LLPanywhere >> n_bkgpred_{0}_{1}".format("sjdc", "2022"), selections["sjdc"] )
        tree_data_23.Draw( "jet0_scores_depth_LLPanywhere:jet1_scores_depth_LLPanywhere >> n_bkgpred_{0}_{1}".format("ljdc", "2023"), selections["ljdc"] )
        tree_data_23.Draw( "jet1_scores_depth_LLPanywhere:jet0_scores_depth_LLPanywhere >> n_bkgpred_{0}_{1}".format("sjdc", "2023"), selections["sjdc"] )

        tree_sig_22.Draw( "jet0_scores_depth_LLPanywhere:jet1_scores_depth_LLPanywhere >> n_signal_{0}_{1}".format("ljdc", "2022"), "(event_weight * weight) * ({})".format(selections["ljdc"]) )
        tree_sig_22.Draw( "jet1_scores_depth_LLPanywhere:jet0_scores_depth_LLPanywhere >> n_signal_{0}_{1}".format("sjdc", "2022"), "(event_weight * weight) * ({})".format(selections["sjdc"]) )
        tree_sig_23.Draw( "jet0_scores_depth_LLPanywhere:jet1_scores_depth_LLPanywhere >> n_signal_{0}_{1}".format("ljdc", "2023"), "(event_weight * weight) * ({})".format(selections["ljdc"]) )
        tree_sig_23.Draw( "jet1_scores_depth_LLPanywhere:jet0_scores_depth_LLPanywhere >> n_signal_{0}_{1}".format("sjdc", "2023"), "(event_weight * weight) * ({})".format(selections["sjdc"]) )

    """
    reweight_llp0 = "pow ( {0} / {1}, 1 ) * exp( -LLP0_DecayCtau * 10. * ( 1.0/{2} - 1.0/{3} ) )".format(ctau_sample, ctau_target, ctau_target, ctau_sample)
    reweight_llp1 = "pow ( {0} / {1}, 1 ) * exp( -LLP1_DecayCtau * 10. * ( 1.0/{2} - 1.0/{3} ) )".format(ctau_sample, ctau_target, ctau_target, ctau_sample)
    reweight = "(weight * {0} * {1})".format(reweight_llp0, reweight_llp1)
    """

    # Scale Hists 
    for cat in ["ljdc", "sjdc"]:
        h2["n_bkgpred_{0}_{1}".format(cat, "2022")].Scale(lumi_sf_22)
        h2["n_bkgpred_{0}_{1}".format(cat, "2023")].Scale(lumi_sf_23)

        h2["n_bkgpred_{0}_{1}".format(cat, "Total")] = h2["n_bkgpred_{0}_{1}".format(cat, "2022")].Clone()
        h2["n_bkgpred_{0}_{1}".format(cat, "Total")].Add( h2["n_bkgpred_{0}_{1}".format(cat, "2023")] )

        h2["n_signal_{0}_{1}".format(cat, "2022")].Scale(lumi_2022 / lumi_total)
        # h2["n_signal_{0}_{1}".format(cat, "2022")].Scale(100.)  # TEMPORARY: weight uses N_gen=3.99M instead of actual 40k; remove once sig22 files are reprocessed
        # print("TEMPORARY: Scaling signal 2022 by 100x to account for N_gen=40k; remove once sig22 files are reprocessed")
        h2["n_signal_{0}_{1}".format(cat, "2023")].Scale(lumi_2023 / lumi_total)

        h2["n_signal_{0}_{1}".format(cat, "Total")] = h2["n_signal_{0}_{1}".format(cat, "2022")].Clone()
        h2["n_signal_{0}_{1}".format(cat, "Total")].Add( h2["n_signal_{0}_{1}".format(cat, "2023")] )

    # Loop

    for i, j in ((i, j) for i in range(n_bins) for j in range(n_bins)):

        incl_score_cut  = incl_centers[i]
        depth_score_cut = depth_centers[j]

        i_bin = i+1 
        j_bin = j+1 

        if i == j and (i+j)%10 == 0: 
            print(i, j, "-->", incl_score_cut, depth_score_cut)

        # Fill Arrays
        for year in ["2022", "2023", "Total"]:
            for cat in ["ljdc", "sjdc"]:

                array2d["n_bkgpred_{0}_{1}".format(cat, year)][i][j] = calculate_bkg_prediction( h2["n_bkgpred_{0}_{1}".format(cat, year)], i_bin, j_bin, n_bins)
                array2d["n_signal_{0}_{1}".format(cat, year)][i][j]  = 100. * h2["n_signal_{0}_{1}".format(cat, year)].Integral(i_bin, n_bins, j_bin, n_bins)

    array2d["score_mode"]    = score_mode
    array2d["incl_edges"]    = incl_edges
    array2d["depth_edges"]   = depth_edges    
    array2d["incl_centers"]  = incl_centers
    array2d["depth_centers"] = depth_centers

    np.savez("optimized_arrays_{0}.npz".format(filetag), **array2d)

# ------------------------------------------------------------------------------
def plot_results( filetag ):

    print( "Running plot_results for", filetag )    

    array2d = np.load("optimized_arrays_{0}.npz".format(filetag))
    score_mode    = array2d["score_mode"]
    incl_edges    = array2d["incl_edges"] 
    depth_edges   = array2d["depth_edges"] 
    incl_centers  = array2d["incl_centers"] 
    depth_centers = array2d["depth_centers"] 
    n_bins = len(incl_centers)

    exclusion_significance = 1.96 #1.64

    # ----- Scan Over Scores ----- #

    #significance_optimal     = {}
    incl_score_cut_optimial  = {}
    depth_score_cut_optimial = {}

    significance_tot_minOK = 100
    sig_frac_optimal = 10.0
    signal_fraction_scale = np.logspace(-4, 0, 100)

    for sig_frac_temp in signal_fraction_scale:

        significance_tot_temp = 0

        significance_array_temp = {}
        significance_temp     = {}
        incl_score_cut_temp   = {}
        depth_score_cut_temp  = {}        
            
        significance_array_tot = np.zeros((n_bins, n_bins), dtype=float)

        years = ["2022", "2023"]
        if combine_years: 
            years = ["Total"]

        for year in years:
            for cat in ["ljdc", "sjdc"]:

                key = "{0}_{1}".format(cat, year)

                #array2d["n_signal_{0}".format(key)] = array2d["n_signal_{0}".format(key)] * 100.

                significance_array_temp[key] = calculate_significance( sig_frac_temp * array2d["n_signal_{0}".format(key)], array2d["n_bkgpred_{0}".format(key)])

                max_significance_temp = np.max(significance_array_temp[key])
                idx = np.unravel_index(np.argmax(significance_array_temp[key]), significance_array_temp[key].shape)

                significance_temp[key]    = max_significance_temp
                incl_score_cut_temp[key]  = incl_centers[idx[0]]
                depth_score_cut_temp[key] = incl_centers[idx[1]]

                significance_tot_temp = math.hypot( significance_tot_temp, max_significance_temp )
                
                significance_array_tot = np.sqrt(significance_array_tot**2 + significance_array_temp[key]**2)
        """
        if False:
            significance_tot_temp = np.max(significance_array_tot) 
            idx = np.unravel_index(np.argmax(significance_array_tot), significance_array_tot.shape)

            for year in ["Total"]:
                for cat in ["ljdc", "sjdc"]:
                    key = "{0}_{1}".format(cat, year)

                    incl_score_cut_temp[key]  = incl_centers[idx[0]]
                    depth_score_cut_temp[key] = incl_centers[idx[1]]
                    #significance_temp[key]    = significance_array_temp[key]
        """

        if significance_tot_temp > exclusion_significance and significance_tot_temp < significance_tot_minOK:
            #print(sig_frac_temp, significance_tot_temp )

            significance_tot_minOK   = significance_tot_temp
            sig_frac_optimal         = sig_frac_temp

            #msignificance_optimal     = significance_temp
            incl_score_cut_optimial  = incl_score_cut_temp
            depth_score_cut_optimial = depth_score_cut_temp

        #print( sig_frac_temp, significance_tot_temp )

    print( "Recommendations:" )
    print( "Signal Frac: ", sig_frac_optimal )
    print( "Significance:", significance_tot_minOK )
    
    #print( "Significance by Category:\n", significance_optimal )
    print( "Inclusive Scores: \n", incl_score_cut_optimial )
    print( "Depth Scores: \n", depth_score_cut_optimial )

    # ----- Plot Outputs ----- #

    X, Y = np.meshgrid(incl_edges[:-1], depth_edges[:-1])

    for year in ["2022", "2023", "Total"]:
        for cat in ["ljdc", "sjdc"]:
            key = "{0}_{1}".format(cat, year)

            plt.figure(figsize=(7,5))
            plt.imshow(
                array2d["n_bkgpred_{0}".format(key)].T, origin='lower', aspect='auto', norm=LogNorm(vmin=1e-2, vmax=1e7), # vmax=1e7,
                extent=[incl_edges[0], incl_edges[-1], depth_edges[0], depth_edges[-1]]
            )
            plt.colorbar(label='Events')
            plt.xlabel('Inclusive Candidate Score')
            plt.ylabel('Depth Candidate Score')
            plt.suptitle('Number of Background Events (Shallow Prediction)', fontsize=14 )
            plt.title("Category: {}, Year: {}, Scores: {}".format(cat, year, score_mode), fontsize=10)

            cs = plt.contour(X, Y, array2d["n_bkgpred_{0}".format(key)].T, levels=[10, 100, 1000], colors='black', linewidths=0.7)
            plt.clabel(cs, fontsize=10, fmt="%.0e")
            """
            for i in range(array2d["n_bkgpred_{0}".format(key)].shape[0]):
                for j in range(array2d["n_bkgpred_{0}".format(key)].shape[1]):
                    val = array2d["n_bkgpred_{0}".format(key)][i, j]
                    txt = f"{val:.2e}" if val > 0 else "0"
                    plt.text( incl_centers[i], depth_centers[j], txt, ha='center', va='center', fontsize=4, color='white')
            """

            plt.savefig("Plots/h2_bkgpred_{}_{}.png".format(key, filetag), dpi=450, bbox_inches='tight')
            plt.clf()

            #plt.figure(figsize=(5,5))
            # norm=LogNorm()
            plt.imshow(
                sig_frac_optimal*array2d["n_signal_{0}".format(key)].T, origin='lower', aspect='auto', #norm=LogNorm(vmin=1, vmax=1e2), #norm=LogNorm(vmin=1e-2, vmax=1e7), 
                extent=[incl_edges[0], incl_edges[-1], depth_edges[0], depth_edges[-1]]
            )
            plt.colorbar(label='Events')
            plt.xlabel('Inclusive Candidate Score')
            plt.ylabel('Depth Candidate Score')
            plt.suptitle('Number of Signal Events (SigStrength = {:.2e})'.format(sig_frac_optimal), fontsize=14 )
            plt.title("Category: {}, Year: {}, Scores: {}".format(cat, year, score_mode), fontsize=10)

            cs = plt.contour(X, Y, sig_frac_optimal*array2d["n_signal_{0}".format(key)].T, levels=[3, 10, 30, 100], colors='black', linewidths=0.7)
            plt.clabel(cs, fontsize=10) #, fmt="%.0e")
            """
            for i in range(array2d["n_signal_{0}".format(key)].shape[0]):
                for j in range(array2d["n_signal_{0}".format(key)].shape[1]):
                    val = sig_frac_optimal * array2d["n_signal_{0}".format(key)][i, j]
                    txt = f"{val:.2e}" if val > 0 else "0"
                    plt.text( incl_centers[i], depth_centers[j], txt, ha='center', va='center', fontsize=4, color='white')
            """
            plt.savefig("Plots/h2_n_signal_{}_{}.png".format(key, filetag), dpi=450, bbox_inches='tight')
            plt.clf()

            significance = calculate_significance( sig_frac_optimal * array2d["n_signal_{0}".format(key)], array2d["n_bkgpred_{0}".format(key)])

            #plt.figure(figsize=(5,5))
            plt.imshow(
                significance.T, origin='lower', aspect='auto', #norm=LogNorm(), #vmin=0, vmax=exclusion_significance,
                extent=[incl_edges[0], incl_edges[-1], depth_edges[0], depth_edges[-1]]
            )
            plt.colorbar(label='Events')
            plt.xlabel('Inclusive Candidate Score')
            plt.ylabel('Depth Candidate Score')
            plt.suptitle('Significance (SigStrength = {:.2e})'.format(sig_frac_optimal), fontsize=14)
            plt.title("Category: {}, Year: {}, Scores: {}".format(cat, year, score_mode), fontsize=10)

            levels = np.max(significance) * np.array([ 0.6, 0.8, 0.9 ])
            levels = sorted(set(round(l, 2) for l in levels))
            if len(levels) >= 2:
                cs = plt.contour(X, Y, significance.T, levels=levels, colors='black', linewidths=0.7)
                plt.clabel(cs, fontsize=10)
            #plt.clabel(cs, fmt=lambda x: f"{x:.2g}", fontsize=10)

            plt.savefig("Plots/h2_significance_{}_{}.png".format(key, filetag), dpi=450, bbox_inches='tight')
            plt.clf()

# ------------------------------------------------------------------------------
def print_results(filetag):

    array2d = np.load("optimized_arrays_{0}.npz".format(filetag))
    score_mode    = array2d["score_mode"]
    incl_edges    = array2d["incl_edges"] 
    depth_edges   = array2d["depth_edges"] 
    incl_centers  = array2d["incl_centers"] 
    depth_centers = array2d["depth_centers"] 
    n_bins = len(incl_centers)

    incl_cut = 0.415
    depth_cut = 0.975

    idx_incl  = math.floor(incl_cut*100) #np.where(incl_edges == incl_cut)[0]
    idx_depth = math.floor(depth_cut*100) #np.where(depth_edges == depth_cut)[0]

    sig_frac_optimal = 0.015199110829529346

    significance_total = 0

    for year in ["2022", "2023"]:
        for cat in ["ljdc", "sjdc"]:
            key = "{0}_{1}".format(cat, year)

            bkgpred = round(array2d["n_bkgpred_{0}".format(key)][idx_incl, idx_depth].item(), 2)
            signal  = round(array2d["n_signal_{0}".format(key)][idx_incl, idx_depth].item(), 2)

            significance = calculate_significance( sig_frac_optimal * signal, bkgpred )
            significance_total = math.hypot( significance_total, significance )
            print( key, "\t", significance )

    print("Total    \t", significance_total)

    return
    # ----- Compare Gillian ----- #

    gillian_depth_vals = np.array( [0.90, 0.93, 0.95, 0.97] )
    gillian_incl_vals = np.array( [0.90, 0.95, 0.97] )
    #print( depth_edges )
    #quit()

    gillian_results = {
        "2022": {
            "ljdc": {
                "values": [
                    [8.62, 6.15, 4.46, 2.42],
                    [3.53, 2.53, 1.82, 1.01],
                    [1.92, 1.38, 0.98, 0.56],
                ],
                "errors": [
                    [0.52, 0.44, 0.37, 0.27],
                    [0.21, 0.18, 0.15, 0.11],
                    [0.12, 0.10, 0.08, 0.06],
                ],
            },
            "sjdc": {
                "values": [
                    [3.27, 2.57, 1.88, 1.23],
                    [1.26, 0.97, 0.71, 0.46],
                    [0.64, 0.49, 0.36, 0.23],
                ],
                "errors": [
                    [0.37, 0.33, 0.28, 0.23],
                    [0.14, 0.12, 0.11, 0.09],
                    [0.07, 0.06, 0.05, 0.04],
                ],
            },
            "combined": {
                "values": [
                    [12.30, 8.95, 6.48, 3.64],
                    [4.90, 3.56, 2.56, 1.46],
                    [2.61, 1.89, 1.36, 0.78],
                ],
                "errors": [
                    [0.65, 0.55, 0.47, 0.34],
                    [0.26, 0.22, 0.18, 0.14],
                    [0.14, 0.12, 0.10, 0.07],
                ],
            },
        },
        "2023": {
            "ljdc": {
                "values": [
                    [12.63, 9.80, 7.57, 4.63],
                    [5.27, 4.08, 3.14, 1.94],
                    [2.77, 2.15, 1.64, 1.00],
                    [1.67, 1.29, 0.98, 0.60],
                ],
                "errors": [
                    [0.60, 0.53, 0.47, 0.36],
                    [0.26, 0.22, 0.20, 0.16],
                    [0.14, 0.12, 0.11, 0.08],
                    [0.08, 0.07, 0.06, 0.05],
                ],
            },
            "sjdc": {
                "values": [
                    [5.27, 3.79, 3.00, 1.67],
                    [2.22, 1.61, 1.29, 0.69],
                    [1.21, 0.88, 0.71, 0.37],
                    [0.76, 0.57, 0.47, 0.23],
                ],
                "errors": [
                    [0.45, 0.38, 0.34, 0.25],
                    [0.20, 0.17, 0.16, 0.10],
                    [0.13, 0.12, 0.11, 0.06],
                    [0.10, 0.10, 0.09, 0.04],
                ],
            },
            "combined": {
                "values": [
                    [18.22, 13.95, 10.77, 6.52],
                    [7.54, 5.76, 4.44, 2.69],
                    [3.95, 3.03, 2.32, 1.39],
                    [2.38, 1.81, 1.38, 0.82],
                ],
                "errors": [
                    [0.75, 0.65, 0.58, 0.45],
                    [0.31, 0.27, 0.24, 0.19],
                    [0.17, 0.15, 0.13, 0.10],
                    [0.10, 0.09, 0.08, 0.06],
                ],
            },
        },
    }

    for year in ["2022", "2023"]:
        for cat in ["ljdc", "sjdc"]:
            key = "{0}_{1}".format(cat, year)

            for incl_cut in gillian_incl_vals:
                for depth_cut in gillian_depth_vals:

                    idx_incl  = int(incl_cut*100) #np.where(incl_edges == incl_cut)[0]
                    idx_depth = int(depth_cut*100) #np.where(depth_edges == depth_cut)[0]

                    idx_incl_g  = np.where(gillian_incl_vals  == incl_cut)[0][0]
                    idx_depth_g = np.where(gillian_depth_vals == depth_cut)[0][0]

                    #print( array2d["n_bkgpred_{0}".format(key)].shape )

                    #print( idx_incl, idx_depth, idx_incl_g, idx_depth_g )
                    #my_result  = None
                    bkgpred_kiley   = round(array2d["n_bkgpred_{0}".format(key)][idx_incl, idx_depth].item(), 2)
                    bkgpred_gillian = round(gillian_results[year][cat]["values"][idx_incl_g][idx_depth_g], 2)

                    print( year, "\t", cat, "\t", incl_cut, "\t", depth_cut, "\t", bkgpred_kiley, "\t", bkgpred_gillian, "\t", round(bkgpred_gillian/bkgpred_kiley, 2) )

# ------------------------------------------------------------------------------
def main():

    args = parseArgs()
    signaltag_22  = args.sig22
    signaltag_23  = args.sig23
    settings_tag  = args.settings_tag
    calculate     = args.calculate
    plot          = args.plot
    print_info    = args.print_info

    tag22 = os.path.splitext(os.path.basename(signaltag_22))[0]
    tag23 = os.path.splitext(os.path.basename(signaltag_23))[0]
    filetag = "{}-{}-{}".format(tag22, tag23, settings_tag)

    if calculate:
        calculate_results( filetag, signaltag_22, signaltag_23 )

    if plot:
        plot_results( filetag )

    if print_info:
        print_results( filetag )

if __name__ == '__main__':
    main()
