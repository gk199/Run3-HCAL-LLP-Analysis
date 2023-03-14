# Run3-HCAL-LLP-Analysis
Run 3 HCAL LLP analysis

Starting from the LLP ntuple in [cms-lpc-llp](https://github.com/cms-lpc-llp/llp_ntupler/tree/run3_GKdev_2022HCAL), the MakeClass was used to initalize the `LLP_HCAL_Analyzer.*` files. 

```
cd DisplacedHcalJetAnalyzer/
source compile.sh 
cd ../Run/
root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("test", "../NTuples/02_17_2023/ntuple_signal_test2k.root")'
root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("test", "/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/llp_ntupler/ntuple_output_2022Data_1k.root")'
```

In `DisplacedHcalJetAnalyzer/DisplacedHcalJetAnalyzer/DisplacedHcalJetAnalyzer.h` can set the variables: `debug, print_counts, save_hists, save_trees, blind_data`. 

In `HistHelper.cxx` set what histograms are filled, and categories are selected. Requirements for when each category is filled are listed in `Loop.cxx`.

The histograms that are read in are listed in  `DisplacedHcalJetAnalyzer.h`. 

## Plotting
```
python KinematicPlots.py ../Run/hists_test.root 
```
where `../Run/hists_test.root` is the output of the previous step. 

## Locations on lxplus
This directory: `/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/LLP_NtuplerAnalyzer`.

The cms-lpc-llp directory: `/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/llp_ntupler`.