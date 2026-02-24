# Running

## Mistag Rate Evaluation
The newer approach uses 3D histograms to evaluate the mistag rates, so information from each dimension can be used in the prediction.
```
python3 MisTagParameterization_3D.py > DNN_pt*_forPython_combined.txt
```
Check the settings in 
Change the booleans `Zmu` and `LLPskim` and the era name at the beginning of the file to control which files are run over.

## Overlay Mistag Rates
```
python3 OverlayMistagRates.py
```
This takes the output root files from `MisTagParameterization_3D` and overlays them to evalute changes throughout the year.

## Convert Fake Rate Results to Latex Tables
```
python3 OutputToLatex.py DNN_pt*_forPython.txt
python3 OutputToLatex_combined.py DNN_pt*_forPython_combined.txt
```
`OutputToLatex.py` is for the results of running `MisTagParameterization_3D.py` with depth, b-tagged (leading, subleading, combined); depth, not b-tagged; depth, b-tagged, low PV; depth, not b-tagged, low PV; depth, b-tagged, high PV; depth, not b-tagged, high PV (used for Tables 8-11 in v2 of the AN). `OutputToLatex_combined.py` is for depth (leading, subleading, combined); depth, low PV; depth, high PV (used for Table 12 in v2 of the AN). 

# Archive

## Mistag Rate: Initial Version
```
python3 MisTagProbability.py
python3 MisTagProbability_emulated.py
python3 MisTagProbability_CR_VR.py
```

The above three files are: ``MisTagProbability`` for using the W+jets and Z+mu selections from the Zmu skim to perform the background estimation with the DNN split by run range, fractional track pT, or electron energy fraction; ``MisTagProbability_emulated`` for splitting by emulated depth or timing towers on the Zmu skim; and ``MisTagProbability_CR_VR`` for evaluating the mistag fraction in the data validation region (subleading jet inclusive score between 0.5 - 0.9) from the control region (inclusive score beween 0-0.5).

For the triggered jet: -9999 means not matched to a L1 jet, 0 means matched with LLP flag not set, 1 means matched with LLP flag set.
