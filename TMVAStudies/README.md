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

Run the plotting script over the output root file from TMVA in `BDTWeightFilesTest/Test.root`. The script plots the BDT scores (overlayed for signal and background), and a ROC curve (with and without background in log scale). The ROC curve is made for each input file, and overlayed for both of them. Printouts are also made to quantify efficiencies at given backgrounds, etc. Arguments are:

Input file 1, label 1, color 1, input file 2, label 2, color 2. An example of running is:
```
root -l /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/MakeMVAPerformancePlots.C+'("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/BDTWeightFilesTest/Test_mh350.root","350",30,"/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/BDTWeightFilesTest/Test_mh125.root","125",38)'
```

## Documentation
[Official documentation](https://root.cern.ch/download/doc/tmva/TMVAUsersGuide.pdf)

[Tutorial](https://twiki.cern.ch/twiki/bin/view/TMVA) (what code is based on): 

Some [helpful slides](https://agenda.infn.it/event/13733/contributions/20520/attachments/14642/16541/MVATutorial.pdf)

Note that any redundancy in the variables input to the TMVA training will cause issues! Depth 1,2,3,4 HCAL energy fractions are redundant (since they must add to 1).