# Running

```
python3 MisTagProbability.py
python3 MisTagProbability_emulated.py
python3 MisTagProbability_CR_VR.py

```

The above three files are: ``MisTagProbability'' for using the W+jets and Z+mu selections from the Zmu skim to perform the background estimation with the DNN split by run range, fractional track pT, or electron energy fraction; ``MisTagProbability_emulated'' for splitting by emulated depth or timing towers on the Zmu skim; and ``MisTagProbability_CR_VR'' for evaluating the mistag fraction in the data validation region (subleading jet inclusive score between 0.5 - 0.9) from the control region (inclusive score beween 0-0.5).

For the triggered jet: -9999 means not matched to a L1 jet, 0 means matched with LLP flag not set, 1 means matched with LLP flag set.

TODO: lots of duplicated code between prediction and measurement, need to clean this up! 

TODO: change tree names automatically based on highMET or LLPskim

TODO: 3D histogram for projections for full background prediction