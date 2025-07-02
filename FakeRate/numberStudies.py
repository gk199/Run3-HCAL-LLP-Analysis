import ROOT
import numpy as np
import os
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

#CONSTANTS
# ------------------------------------------------------------------------------
pT_bins = np.linspace(0, 300, 20)  # pT axis from 0 to 500 GeV, 20 bins
pT_bins = [0, 40, 50, 60, 70, 80, 100, 120, 160, 240, 400]  # Define pT bins
#pT_bins = [0, 40, 60, 70, 100, 150, 240, 400]  # Define pT bins
pT_bins = np.array(pT_bins, dtype=float)
eta_bins = np.linspace(-1.26, 1.26, 10)  # eta axis from -2 to 2, 10 bins
phi_bins = np.linspace(-np.pi, np.pi, 10)  # phi axis from -pi to pi, 10 bins

runs_to_exclude = [367230, 367772, 368331, 368440, 368764, 370436, 370579, 370790]
debug = False

#HELPER FUNCTIONS
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
def ExcludedCut( branch_name, branch_sel ):

    selection = ""

    if type(branch_sel) is int:
        selection = branch_name + " != " + str(branch_sel) 

    elif type(branch_sel) is list:
        if len(branch_sel) == 0:
            print("WARNING <ExcludedCut> : Entry error (length = 0). No cut implemented")
        elif len(branch_sel) == 1:
            selection = branch_name + " != " + str(branch_sel[0])
        else:
            for exclude in branch_sel[:-1]:
                selection += branch_name + " != " + str(exclude) + " && "
            selection += branch_name + " != " + str(branch_sel[-1])

    else: 
        print("WARNING <ExcludedCut> : Type Error. No cut implemented")

    if debug: print(selection + "  in Excluded Cut ")
    return ROOT.TCut( selection + " " )

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
def ResetAxis(hist3d):
    # Explicitly set axis labels after cloning
    hist3d.GetXaxis().SetTitle("p_{T} (GeV)")
    hist3d.GetYaxis().SetTitle("#eta")
    hist3d.GetZaxis().SetTitle("#phi")
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
def GetData(infilepaths, label):
    # Create a TChain to hold the trees from all files
    chain = ROOT.TChain(label) # Access the tree by label

    # Loop over the list of file paths and add each file's tree to the chain
    for infilepath in infilepaths:
        print(f"Adding file: {infilepath}")
        chain.Add(infilepath)  # Add the tree from the current file to the chain
    
    # Check if the chain has any entries
    if chain.GetEntries() == 0:
        print(f"No entries found in the tree '{label}' across all files.")
        return None  # Return None if no data is found
    else:
        print(f"Found tree with label '{label}'")
        print("number of entries: ", chain.GetEntries())

    # Return the chain (holding trees from all files) 
    return chain  # No need to return the file, as TChain handles it


# ------------------------------------------------------------------------------
def MisTagParametrization(tree, DNN_cut, DNN_cut_inc, option="", settings= {}, debug=False):
    #settings = {"triggered_jet": "jet0", "other_jet": "jet1" "inc_train": 80, "depth" = "hcal", "CNN": False}
    if not settings:
        print("No settings provided of the form {triggered_jet, other_jet, inc_train, depth, CNN}")
        return
    #print("Settings: ", settings)

        
    run_exclusion = ExcludedCut("run", runs_to_exclude)
    CR = GetCut(f"jet1_scores_inc_train{settings['inc_train']}", [0,0.5])
    VR = GetCut(f"jet1_scores_inc_train{settings['inc_train']}", [0.5,DNN_cut_inc])
    SR = GetCut(f"jet1_scores_inc_train{settings['inc_train']}", [DNN_cut_inc,1.1])
    mistag = GetCut(f"jet0_scores_depth_{settings['depth']}", [DNN_cut,1.1])
    mistag_type = "leading" if settings['triggered_jet'] == 'jet0' else 'sub-leading'
    
    if settings['CNN']:
        mistag = GetCut("CNN3D_classifier3", [DNN_cut,1.1])
    
    triggered = GetCut("jet0_L1trig_Matched", 1) 
    if settings['triggered_jet'] == 'jet1':
        triggered += GetCut("jet1_L1trig_Matched", 1) 

    pt_eta = GetCut("jet0_Pt",[40,1000]) + GetCut("jet0_Eta",[-1.26,1.26]) + GetCut("jet1_Pt",[40,1000]) + GetCut("jet1_Eta",[-1.26,1.26])
    
    depth_emu = GetCut(f"{settings['triggered_jet']}_DepthTowers", [2,100]) # + GetCut("jet0_TimingTowers", 0)
    timing_emu = GetCut(f"{settings['triggered_jet']}_TimingTowers", [2,100]) # + GetCut("jet0_DepthTowers", 0)
    depth_timing_emu = GetCut(f"{settings['triggered_jet']}_DepthTowers", 1) + GetCut(f"{settings['triggered_jet']}_TimingTowers", 1)

    track_pT = GetCut(f"{settings['triggered_jet']}_Track0Pt / jet0_Pt",[0,1.1])

    run_before = GetCut("run", [360000, 368770])
    run_after = GetCut("run", [368770, 375000])

    
    # Define a mapping of options to their corresponding updates
    option_map = {
        "depth": (depth_emu, ": 2+ depth", "_depth"),
        "depth, trackPt": (depth_emu + track_pT, ": 2+ depth, frac. track pT", "_depth_trackPt"),
        "timing": (timing_emu,": 2+ timing", "_timing"),
        "depth, timing": (depth_timing_emu,  ": 1 depth, 1 timing", "_depth_timing"),
        "before alignment": (run_before, ": before alignment", "_before_align"),
        "after alignment": (run_after,": after alignment", "_after_align"),
        "before alignment, depth": (run_before + depth_emu, ": 2+ depth, before alignment", "_depth_before_align"),
        "after alignment, depth": (run_after + depth_emu, ": 2+ depth, after alignment", "_depth_after_align"),
        "after alignment, trackPt": (run_after + track_pT, ": frac. track pT, after alignment", "_trackPt_after_align")
    }

    # Default values for label and title
    label = ""
    title = ": inclusive"

    # Check if the option exists in the mapping
    if option in option_map:
        triggered += option_map[option][0] #+ cross_check_cuts # emulated option
        print("Triggered requirements:")
        print(triggered)
        title = option_map[option][1]
        label = option_map[option][2]

    total_cut = triggered + pt_eta + run_exclusion
    CR_all = CreateHistograms(tree, CR + triggered + pt_eta + run_exclusion, "hist3d_CR_all")
    CR_mistag = CreateHistograms(tree, CR + triggered + pt_eta + run_exclusion + mistag, "hist3d_CR_mistag")
    VR_all = CreateHistograms(tree, VR + triggered + pt_eta + run_exclusion, "hist3d_VR_all")
    VR_mistag = CreateHistograms(tree, VR + triggered + pt_eta + run_exclusion + mistag, "hist3d_VR_mistag")
    SR_all = CreateHistograms(tree, SR + triggered + pt_eta + run_exclusion, "hist3d_SR_all")
    print(mistag_type)
    if debug:
        metrics = {"jet0_scores_depth_hcal": "j0scores_debug"}
        debugging(tree, total_cut, metrics)
        return 

    # Clone CR_mistag and divide it by CR_all to get the mistag rate in CR
    CR_mistag_rate = CR_mistag.Clone("CR_mistag_rate")
    CR_mistag_rate.Divide(CR_all)
    VR_mistag_predict = VR_all.Clone("VR_mistag_predict")
    VR_mistag_predict.Multiply(CR_mistag_rate)
            
    # Explicitly set axis labels after cloning
    ResetAxis(CR_mistag_rate)
    ResetAxis(VR_mistag_predict)

    # Predict mistag in the SR
    MR_total = SR_all.Integral()
    SR_all.Multiply(CR_mistag_rate)
    predicted_SR = SR_all.Integral()

    return (predicted_SR, MR_total, DNN_cut, DNN_cut_inc)
        
# ------------------------------------------------------------------------------
def debugging(tree, cut, metrics):
    c = ROOT.TCanvas("c", "debug canvas", 800, 600)
    print(f"Entries in the tree total: {tree.GetEntries()}")
    print(f"Entries in the tree after cut: {tree.GetEntries(cut)}")
    filtered_tree = tree.CopyTree(cut)
    for metric, filepath in metrics.items():
        print(metric)
        for entry in range(10):  # Checking first 10 entries
                filtered_tree.GetEntry(entry)
                print(getattr(filtered_tree,metric))
            
        hist1d = ROOT.TH1F("hist1d_scores", f"{metric} with cuts applied", 100, 0, 1.01)
        filtered_tree.Draw(f"{metric} >> hist1d_scores", "", "")
        c.cd()
        hist1d.Draw()
        c.Update()           
        c.SaveAs(f"{filepath}.png")
# -------------------------------------------------------------------------------
def get_SR_data(tree, DNN_cuts, DNN_cuts_inc, option, settings):
    data = []
    for cut in DNN_cuts:
        for inc in DNN_cuts_inc:
            predSR, MR, depth, inc = MisTagParametrization(tree, cut, inc, option=option, settings=settings, debug=False)
            data.append([predSR, MR, depth, inc])
    data = np.array(data)
    return data
# -------------------------------------------------------------------------------
def plotPoints1D(ax, data, coord, fractional=False):
    cut_types = ['depth', 'inclusive']
    if fractional:
        mask = data[:, 1] != 0
        y = data[mask, 0] / data[mask, 1]
        y = data[:,0] / data[:,1]
        x = data[mask,coord]
    else:
        y = data[:,0]
        x = data[:,coord]
    ax.scatter(x, y, s=4, marker='x', label=cut_types[coord-2])
# -------------------------------------------------------------------------------
def plotHeatmap(ax, data, coord=2, fractional=False):
    if fractional:
        mask = data[:,1]
        z = data[mask, 0] / data[mask,1]
        df = pd.DataFrame({
        "sr": np.round(z,2),
        "depth": np.round(data[mask, 2],2),
        "inc": np.round(data[mask, 3],2)})
          
    else:
        z = data[:,0]
        df = pd.DataFrame({
        "sr": np.round(z,2),
        "depth": np.round(data[:, 2],2),
        "inc": np.round(data[:, 3],2)})
        
    matrix = df.pivot(index="inc", columns="depth", values="sr")
    sns.heatmap(
    matrix,
    annot=matrix,        
    cmap="viridis",
    linewidths=0.5,
    linecolor="white",
    ax=ax)
    ax.set_xlabel("depth")
    ax.set_ylabel("inc")
    ax.invert_yaxis()

# ------------------------------------------------------------------------------
def main():
    
    label = "NoSel"
    era = "v3.16 MH125MS15"
    print(era)


    if era == "2023 Bv1":   infilepath_list = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.13/minituple_v3.13_LLPskim_Run2023Bv1.root"]
    elif era == "2023 Cv1": infilepath_list = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.13/minituple_v3.13_LLPskim_Run2023Cv1.root"]
    elif era == "2023 Cv2": infilepath_list = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.13/minituple_v3.13_LLPskim_Run2023Cv2.root"]
    elif era == "2023 Cv3": infilepath_list = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.13/minituple_v3.13_LLPskim_Run2023Cv3.root"]
    elif era == "2023 Cv4": infilepath_list = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.13/minituple_v3.13_LLPskim_Run2023Cv4.root"]
    elif era == "2023 Dv1": infilepath_list = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.13/minituple_v3.13_LLPskim_Run2023Dv1.root"]
    elif era == "2023 Dv2" and not CNN: infilepath_list = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.13/minituple_v3.13_LLPskim_Run2023Dv2.root"]
    elif era == "2023 Dv2" and CNN: infilepath_list = ["/afs/cern.ch/work/f/fsimpson/public/minituple_outputs/minituple_Run2023D-EXOLLPJetHCAL-PromptReco-v2_partial28k-v4-scores_added.root"]
    elif era == "v3.16 2023Cv2": infilepath_list=["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_LLPskim_2023Cv2_allscores.root"]
    elif era == "v3.16 2023Dv2": infilepath_list = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_LLPskim_2023Dv2_allscores.root"]
    elif era == "v3.16 MH125MS15": infilepath_list = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_HToSSTo4b_125_15_CTau10000_allscores.root"]
    else: infilepath_list = ["/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.13/minituple_LLPskim_2023Bv1_allscores.root",
                        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_LLPskim_2023Cv1_allscores.root",
                        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_LLPskim_2023Cv2_allscores.root",
                        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_LLPskim_2023Cv3_allscores.root",
                        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_LLPskim_2023Cv4_allscores.root",
                        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_LLPskim_2023Dv1_allscores.root",
                        "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_LLPskim_2023Dv2_allscores.roott"]
    tree = GetData(infilepath_list, label)
    print(tree)

    if tree:
        print("LLP skim tree successfully accessed, will be passed to MisTagParametrization")
        settings = {"triggered_jet": "jet0", "other_jet": "jet1", "inc_train": 80, "depth" : "hcal", "CNN": False}

        DNN_cuts = np.linspace(0.5, 0.99, 30)
        DNN_cuts_inc = np.linspace(0.5, 0.99, 30)
        data = get_SR_data(tree, DNN_cuts, [0.9], "depth", settings)
        fig, ax = plt.subplots()
        #plotHeatmap(ax, data)
        plotPoints1D(ax,data, coord=2, fractional=True)
        data = get_SR_data(tree, [0.9], DNN_cuts_inc, "depth", settings)
        plotPoints1D(ax,data, coord=3, fractional=True)
        ax.set_xlabel("cut")
        ax.set_ylabel("Number of events predicted in SR")
        ax.set_title(f"{era}")
        plt.tight_layout()
        ax.legend()
        fig.savefig(f"{era}.png")


if __name__ == '__main__':
	main()
        

