# TMVA

**1. Modify Classifier:**

In `ExampleClassification.C`: Modify file paths, cuts, and signal and background selections.

**2. Run Classifier:**

```
root -q -b -l ExampleClassification.C
```
This will produce a scratch output root file in the working directory (can be deleted, needed to avoid issues about "TTree created as a memory-resident Tree") and an output file `BDTWeightFilesTest/Test.root`.

**3. Display Output:**

Display output ROC curves, variable overlays, coorelations, etc. with:
```
source display_gui.sh
```

## Documentation
[Official documentation](https://root.cern.ch/download/doc/tmva/TMVAUsersGuide.pdf)

[Tutorial](https://twiki.cern.ch/twiki/bin/view/TMVA) (what code is based on): 

Some [helpful slides](https://agenda.infn.it/event/13733/contributions/20520/attachments/14642/16541/MVATutorial.pdf)