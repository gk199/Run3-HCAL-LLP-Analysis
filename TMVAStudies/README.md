# TMVA

**1. Modify Classifier:**

In `LLP_WPlusJets_Classification.C`: Modify file paths, cuts, and signal and background selections.

**2. Run Classifier:**

```
root -q -b -l LLP_WPlusJets_Classification.C
```
This will produce a scratch output root file in the working directory (can be deleted, needed to avoid issues about "TTree created as a memory-resident Tree") and an output file `BDTWeightFilesTest/Test.root`.

**3. Display Output:**

Display output ROC curves, variable overlays, coorelations, etc. with:
```
source display_gui.sh
```

**4. Plot Output:**
### TMVA output
Run the plotting script over the output root file from TMVA in `BDTWeightFilesTest/Test_*.root`. The script plots the BDT scores (overlayed for each signal and background), and a ROC curve (for each signal overlayed, with background in log scale). The ROC curve is made for each input file, and overlayed for all of them. Printouts are also made to quantify efficiencies at given backgrounds, etc. Filenames and specifiers are directly listed in the plotting script, when `BDTPerformancePlots` is called. The order is: Input file, label, color. To run:
```
root -l MakeMVAPerformancePlots.C+'()'
```

### Minituples
Run the plotting script over the produced minituples with the bdt scores saved. Signal and background files, labels, and colors are inputs to `BDTPerformancePlots`. For a given signal sample, multiple ROC curves are overlayed (testing on 125, 350, and combination BDTs). To run:
```
root -l MakeMVAPerformancePlots_SigBkg.C+'()'
```

**5. Weight Files:**
Copy over weights files to the directory `../BDTWeightFiles/v*/` such that they can be used in the minituple production. Make sure naming convention agrees with `BDTHelper.cxx`.

# Version Tracking
v0.0: First setup, trained on LLP MC with mH = 125 and 350, with W+jets backgrounds. For v0.0, the signal and backgrounds are from these files:
``
   sigTagList.push_back("v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2023_11_23");
   sigTagList.push_back("v3.0_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2023_11_29");
   TString bkgTag = "v3.0_LLPskim_2023Cv4_2023_11_22";
``

v0.1: Removed jet E, pT from training. Trained on LLP MC with mH = 125 and 350, with W+jets backgrounds. For v0.1, the signal and backgrounds are from these files:
```
   sigTagList.push_back("v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_01_20_TRAIN");
   sigTagList.push_back("v3.0_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_01_20_TRAIN");
   TString bkgTag = "v3.0_LLPskim_2023Cv4_2023_11_22";
```

## Documentation
[Official documentation](https://root.cern.ch/download/doc/tmva/TMVAUsersGuide.pdf)

[Tutorial](https://twiki.cern.ch/twiki/bin/view/TMVA) (what code is based on): 

Some [helpful slides](https://agenda.infn.it/event/13733/contributions/20520/attachments/14642/16541/MVATutorial.pdf)

Note that any redundancy in the variables input to the TMVA training will cause issues! Depth 1,2,3,4 HCAL energy fractions are redundant (since they must add to 1).