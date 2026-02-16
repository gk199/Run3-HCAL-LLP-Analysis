# Run3-HCAL-LLP-Analysis
Run 3 HCAL LLP analysis

Setup:
```
git clone git@github.com:gk199/Run3-HCAL-LLP-Analysis.git
cd Run3-HCAL-LLP-Analysis
git checkout -b <your-branch>
```

## Ntuples to Minituples

### Overview + Inputs
`DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C` is the main file that produces minituples. Its arguments are: 
- Filetag -- included in name of output file
- Systematic uncertainty -- default is "Nominal"; other options are JER_up and JER_down
- Input files -- can either be single or multiple files

### Compilation Options:

You can run in ROOT interpreter mode (no compilation necessary) or in compiled mode (preferred method). To compile:
```
cd DisplacedHcalJetAnalyzer/
mkdir exe
source compile.sh
```

### Run Locally:

```
# Go into run directory, since there are some local filepath dependencies
cd Run/

# Set up CMS proxy to access grid NTuples
voms-proxy-init -voms cms
```

Run in ROOT interpreter mode (no compilation necessary):
```
# Run over single input file (single output file produced)
root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("<tag>", "<systematic-uncert>", "<path/to/ntuple.root>")'

# Run over multiple input files (single output file produced):
root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("<tag>", "<systematic-uncert>", {"<path/to/ntuple1.root>", ..., "<path/to/ntupleN.root>"})'
```

Run in compiled mode (preferred):
```
# Run over single input file (single output file produced)
../DisplacedHcalJetAnalyzer/exe/DisplacedHcalJetAnalyzer <tag> <systematic-uncert> <path/to/ntuple1.root>

# Run over multiple input files (single output file produced):
../DisplacedHcalJetAnalyzer/exe/DisplacedHcalJetAnalyzer <tag> <systematic-uncert> <path/to/ntuple1.root> ... <path/to/ntupleN.root>
```

See examples in: `run_example.sh`

For running in compiled mode, remember to input the file path as `root://cmsxrootd.fnal.gov///store/user...`.

Text files of the ntuples can be made with `find "$PWD" -maxdepth 1 -type f` from the directories in `/hdfs/store/user/gkopp/`, or using the dedicated script in `ValidateFileProcessing`.

### Run via Condor:
Follow the [instructions](https://github.com/gk199/Run3-HCAL-LLP-Analysis/tree/main/Run/Condor) in the `Run/Condor` subdirectory. 

### Analyzer Settings
In `DisplacedHcalJetAnalyzer/DisplacedHcalJetAnalyzer/DisplacedHcalJetAnalyzer.h` can set the variables: `debug, print_counts, save_hists, save_trees, blind_data`. Setting `AnalysisReader.debug = true` in `DisplacedHcalJetAnalyzer.C` is very helpful for debugging. 

Main files to edit are in `DisplacedHcalJetAnalyzer/src`. Specific details: In `HistHelper.cxx` set what histograms are filled and select categories. Requirements for when each category is filled are listed in `Loop.cxx`. Careful to submit only Condor jobs for the correct configuration though! 

`OutputHelper.cxx` is the main file to edit, where output trees are booked and filled. `TriggerHelper.cxx` contains trigger-related functions. `ObjectHelper.cxx` is a placeholder to put object-related functions. The histograms that are read in are listed in `DisplacedHcalJetAnalyzer.h`. 

Note, if making another tree (jet tree vs. event tree, for example), need to initalize new set of `treenames`, `tree_output`, and `tree_output_vars*` in `DisplacedHcalJetAnalyzer.h`. 

## Plotting
Plotting is done in `MiniTuplePlotter`.

The main script recently in use is `Gillian/MiniTuplePlotter_CR_SR.C`.

## DNN Classifier
Created from minituples, in the `Classifiers` directory. The Inclusive DNN is run first, using the perJet trees, and then the Depth DNN is run based on the control region defined by the Inclusive Tagger scores.

Plotters for the ROC curves and DNN scores are also included in this directory. 

## Locations on lxplus
This directory: `/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/LLP_NtuplerAnalyzer`.

The cms-lpc-llp directory: `/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/llp_ntupler`.

## Checking File Content
RECO files:
```
voms-proxy-init --voms cms
root
TFile *f =TFile::Open("root://xrootd-cms.infn.it///store/data/Run2023C/DisplacedJet/AOD/EXOLLPJetHCAL-PromptReco-v1/000/367/104/00000/8e5b08c6-c450-4548-8295-2d8312bc81d0.root");
```
in a CMSSW environment: 
```
edmDumpEventContent root://xrootd-cms.infn.it///store/data/Run2023C/DisplacedJet/AOD/EXOLLPJetHCAL-PromptReco-v1/000/367/104/00000/8e5b08c6-c450-4548-8295-2d8312bc81d0.root
```
Ntuples:
```
root
TFile *f =TFile::Open("root://cmsxrootd.fnal.gov///store/user/gkopp/ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV/LLP_MC_test__20230713_102955/230713_083027/0000/output_10.root");
```
File contents are saved in `Ntuple_content.txt`, `edmDumpEventContent_LLPskim.txt`, and `LLPskim_content.txt` in main analysis directory for reference. 

# Old / Outdated Notes
Starting from the LLP ntuple in [cms-lpc-llp](https://github.com/cms-lpc-llp/llp_ntupler/tree/run3_GKdev_2022HCAL), the MakeClass was used to initalize the `LLP_HCAL_Analyzer.*` files, and Kiley's setup for the full framework. 

## Plotting
Outdated plotting scripts, in the `Plot` directory. Plotting from the minituples tree:
```
python Plotter.py <path to ntuple> <data / MC>

python Plotter.py /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v <version> /minituple_v <version> _LLPskim_500k_ <date> .root data
python Plotter.py /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v <version> /minituple_v <version> _MCsignal_500k_ <date> .root MC

./Plot_all.sh
```
Kinematic plots:
```
cd ../Plot/
python KinematicPlots.py ../Run/hists_test_<>.root <data / MC>

cp -r outPlots/* /eos/user/g/gkopp/www/LLP_HCAL_Run3Analysis/outPlots
cp -r outPlots_MC/* /eos/user/g/gkopp/www/LLP_HCAL_Run3Analysis/outPlots_MC

python Data_MC_overlay.py ../Run/hists_test_data1.root ../Run/hists_test_signal1.root
```
where `../Run/hists_test.root` is the output of the previous step. Indicate in the script whether MC or data is being used. Plots can be copied to the [EOS webpage](https://gkopp.web.cern.ch/gkopp/LLP_HCAL_Run3Analysis/outPlots/).

NOTE: for running over v2 ntuples for LLP MC, the variables for lepton isolation are not avaliable. Thus, a workaround is to comment out the W+Jets tree (`if (PassWPlusJetsSelection()) FillOutputTrees("WPlusJets");`) such that only the NoSel tree is filled.
