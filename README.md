# Run3-HCAL-LLP-Analysis
Run 3 HCAL LLP analysis

Setup:
```
git clone git@github.com:gk199/Run3-HCAL-LLP-Analysis.git
cd Run3-HCAL-LLP-Analysis
git checkout -b <your-branch>
```

Starting from the LLP ntuple in [cms-lpc-llp](https://github.com/cms-lpc-llp/llp_ntupler/tree/run3_GKdev_2022HCAL), the MakeClass was used to initalize the `LLP_HCAL_Analyzer.*` files, and Kiley's setup for the full framework. 

```
py_setup # setup if needed for python environment 
cd DisplacedHcalJetAnalyzer/
source compile.sh 
cd ../Run/
root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("test", "/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/llp_ntupler/run/ntuple_output_test_signal1.root")'
mv hists_test.root hists_test_signal1.root
root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("test", "/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/llp_ntupler/run/ntuple_output_test_data1.root")'
mv hists_test.root hists_test_data1.root

cd Run/
proxy
crab_setup
./run_signal.sh
./run_data.sh
```

In `DisplacedHcalJetAnalyzer/DisplacedHcalJetAnalyzer/DisplacedHcalJetAnalyzer.h` can set the variables: `debug, print_counts, save_hists, save_trees, blind_data`. 

In `HistHelper.cxx` set what histograms are filled, and categories are selected. Requirements for when each category is filled are listed in `Loop.cxx`.

`TriggerHelper.cxx` contains trigger-related functions. `ObjectHelper.cxx` is a placeholder to put object-related functions.

The histograms that are read in are listed in `DisplacedHcalJetAnalyzer.h`. 

## Plotting
```
cd ../Plot/
python KinematicPlots.py ../Run/hists_test_<>.root <data / MC>

python KinematicPlots.py ../Run/hists_test_data1_04-May-2023.root data
python KinematicPlots.py ../Run/hists_test_signal1_04-May-2023.root MC
cp -r outPlots/* /eos/user/g/gkopp/www/LLP_HCAL_Run3Analysis/outPlots
cp -r outPlots_MC/* /eos/user/g/gkopp/www/LLP_HCAL_Run3Analysis/outPlots_MC

python Data_MC_overlay.py ../Run/hists_test_data1.root ../Run/hists_test_signal1.root
```
where `../Run/hists_test.root` is the output of the previous step. Indicate in the script whether MC or data is being used. Plots can be copied to the [EOS webpage](https://gkopp.web.cern.ch/gkopp/LLP_HCAL_Run3Analysis/outPlots/).

Plotting from the minituples tree:
```
python Plotter.py ../Run/hists_test_signal20k_17-May-2023.root MC
```

## Locations on lxplus
This directory: `/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/LLP_NtuplerAnalyzer`.

The cms-lpc-llp directory: `/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/llp_ntupler`.