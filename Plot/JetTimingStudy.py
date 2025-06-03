import uproot
import numpy as np
import matplotlib.pyplot as plt

# -- File paths
data_file = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.14/minituple_LLPskim_2023Dv1.root"
mc_file = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.14/minituple_HToSSTo4b_125_50_CTau3000.root"

# -- Tree name
tree_name = "PerJet_NoSel"

# -- Variables to plot
variables_to_plot = ["perJet_EnergyFrac_Depth1", "perJet_NeutralHadEFrac"]

# -- Selection functions
def load_tree(file_path, tree_name):
    file = uproot.open(file_path)
    return file[tree_name]

def apply_selection(array_dict, selection_mask):
    return {var: array[selection_mask] for var, array in array_dict.items()}

# -- Load data
data_tree = load_tree(data_file, tree_name)
mc_tree = load_tree(mc_file, tree_name)

# -- Read variables and selection flags
data_arrays = data_tree.arrays(variables_to_plot + ["Pass_WPlusJets"], library="np")
mc_arrays = mc_tree.arrays(variables_to_plot + ["Pass_LLPMatched", "perJet_MatchedLLP_DecayR"], library="np")

# -- Basic selections
data_mask = data_arrays["Pass_WPlusJets"] == 1
mc_mask = mc_arrays["Pass_LLPMatched"] == 1 # would also add must pass the LLP trigger selection

# -- Option: Additional MC cuts
def get_mc_cut(mc_arrays, decayr_min, decayr_max):
    return (mc_arrays["perJet_MatchedLLP_DecayR"] > decayr_min) & (mc_arrays["perJet_MatchedLLP_DecayR"] < decayr_max)

# -- Plotting function with normalization option
def make_overlay_plot(var_name, bins=50, range=None, extra_mc_cuts=None, normalize_to_one=False, output_prefix="plot"):
    print("Running plotting function: make_overlay_plot() for " + var_name)
    plt.figure(figsize=(8, 6))

    # Base selections
    data_vals = data_arrays[var_name][data_mask]
    mc_vals = mc_arrays[var_name][mc_mask]

    # Draw data
    hist_kwargs = dict(bins=bins, range=range, histtype='step', linewidth=2)

    if normalize_to_one:
        data_weights = np.ones_like(data_vals) / len(data_vals) if len(data_vals) > 0 else None # basically doing 1/N for the histogram to normalize it
        mc_weights = np.ones_like(mc_vals) / len(mc_vals) if len(mc_vals) > 0 else None
        plt.hist(data_vals, weights=data_weights, label="Data", color="black", **hist_kwargs)
        plt.hist(mc_vals, weights=mc_weights, label="MC (All)", color="blue", **hist_kwargs)
    else:
        plt.hist(data_vals, label="Data", color="black", **hist_kwargs)
        plt.hist(mc_vals, label="MC (All)", color="blue", **hist_kwargs)

    # Extra MC cuts (optional)
    if extra_mc_cuts:
        for label, (rmin, rmax), color in extra_mc_cuts:
            cut_mask = mc_mask & get_mc_cut(mc_arrays, rmin, rmax)
            vals = mc_arrays[var_name][cut_mask]
            if normalize_to_one and len(vals) > 0:
                weights = np.ones_like(vals) / len(vals)
            else:
                weights = None
            plt.hist(vals, weights=weights, label=f"MC ({label})", color=color, **hist_kwargs)

    plt.xlabel(var_name)
    plt.ylabel("Normalized Fraction of Entries" if normalize_to_one else "Entries")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    outname = f"{output_prefix}_{var_name}_normalized.png" if normalize_to_one else f"{output_prefix}_{var_name}.png"
    plt.savefig(outname)
    plt.close()

    print("Saved plots to current directory!")

# -- Extra MC regions to compare (optional)
extra_mc_regions = [
    ("DecayR 0-129 cm", (0, 129), "green"),
    ("DecayR 183–295 cm", (183, 295), "red"),
]

# -- Loop over variables and make plots with normalization option
normalize = True  # Set this flag to True or False based on your need
for var in variables_to_plot:
    make_overlay_plot(var, bins=50, range=None, extra_mc_cuts=extra_mc_regions, normalize_to_one=normalize, output_prefix="overlay")