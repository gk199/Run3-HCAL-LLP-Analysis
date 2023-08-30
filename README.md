# Run3-HCAL-LLP-Analysis
Run 3 HCAL LLP analysis

Setup:
```
git clone git@github.com:gk199/Run3-HCAL-LLP-Analysis.git
cd Run3-HCAL-LLP-Analysis
git checkout -b <your-branch>
```

Starting from the LLP ntuple in [cms-lpc-llp](https://github.com/cms-lpc-llp/llp_ntupler/tree/run3_GKdev_2022HCAL), the MakeClass was used to initalize the `LLP_HCAL_Analyzer.*` files, and Kiley's setup for the full framework. 

To run in compiler mode:
```
cd Run/
proxy
crab_setup
root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("<tag>", "<path/to/ntuple.root>")'
```
Note that paths to ntuples must be local for running in compiler mode. To run in compiled mode (preferred method):
```
py_setup # setup if needed for python environment 
cd DisplacedHcalJetAnalyzer/
mkdir exe
source compile.sh 
cd ../Run
./../DisplacedHcalJetAnalyzer/exe/DisplacedHcalJetAnalyzer <file_output_tag> root://cmsxrootd.fnal.gov///store/user/gkopp/<path_to_file>

./run_signal_data.sh
# change number of events in DisplacedHcalJetAnalyzer.C -- particularly helpful for testing changes before processing all data. 

# multiprocessing, took 30 minutes for 100k events (MC); 1 hour for 500k events (data)
./mp_local.py InputFiles_2023_*_*_Run2023C-EXOLLPJetHCAL-PromptReco-v4_ntuplesv*.txt
./mp_local.py InputFiles_2023_*_*_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_ntuplesv*.txt
# files will be saved to /eos/user/g/gkopp/LLP_Analysis/
```
For running in compiled mode, remember to input the file path as `root://cmsxrootd.fnal.gov///store/user...`.

## Condor Processing
Follow the [instructions](https://github.com/gk199/Run3-HCAL-LLP-Analysis/tree/main/Run/Condor) in the condor subdirectory. 

## Analyzer Setttings
In `DisplacedHcalJetAnalyzer/DisplacedHcalJetAnalyzer/DisplacedHcalJetAnalyzer.h` can set the variables: `debug, print_counts, save_hists, save_trees, blind_data`. Setting `AnalysisReader.debug = true` in `DisplacedHcalJetAnalyzer.C` is very helpful for debugging. 

In `HistHelper.cxx` set what histograms are filled (plotted with the outdated Kinematic plotters), and categories are selected. Requirements for when each category is filled are listed in `Loop.cxx`.

`OutputHelper.cxx` is the main file to edit, where output trees are booked and filled. 

`TriggerHelper.cxx` contains trigger-related functions. `ObjectHelper.cxx` is a placeholder to put object-related functions.

The histograms that are read in are listed in `DisplacedHcalJetAnalyzer.h`. 

## Plotting
Plotting from the minituples tree:
```
python Plotter.py <path to ntuple> <data / MC>

python Plotter.py /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v <version> /minituple_v <version> _LLPskim_500k_ <date> .root data
python Plotter.py /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v <version> /minituple_v <version> _MCsignal_500k_ <date> .root MC

./Plot_all.sh
```

Outdated plotting scripts:
```
cd ../Plot/
python KinematicPlots.py ../Run/hists_test_<>.root <data / MC>

cp -r outPlots/* /eos/user/g/gkopp/www/LLP_HCAL_Run3Analysis/outPlots
cp -r outPlots_MC/* /eos/user/g/gkopp/www/LLP_HCAL_Run3Analysis/outPlots_MC

python Data_MC_overlay.py ../Run/hists_test_data1.root ../Run/hists_test_signal1.root
```
where `../Run/hists_test.root` is the output of the previous step. Indicate in the script whether MC or data is being used. Plots can be copied to the [EOS webpage](https://gkopp.web.cern.ch/gkopp/LLP_HCAL_Run3Analysis/outPlots/).

## Locations on lxplus
This directory: `/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/LLP_NtuplerAnalyzer`.

The cms-lpc-llp directory: `/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/llp_ntupler`.