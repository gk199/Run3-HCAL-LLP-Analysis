# Running

```
python3 numberStudies.py
```

Code is based on MisTagParametrization.p. Evaluates the predicted number of events (both nominal and fractional) that are in SR based on the total number of events in the MR, by applying the calculated mistag rate from the CR (binned as a function of pt, eta, and phi).
Modify the code to either plot a cross-section (vary one type of cut while keeping the other one constant) using plotPoints1D or vary both simultaneously and obtain a heatmap using plotHeatmap, or just debug without evaluating by choosing which metrics to plot/print using the debugging function in MisTagParametrization function. 
The settings are to help simplify the amount of hard-coding needed. 
 - Triggered jet: the jet used for the depth tagger; default "jet0"
 - Other jet: the jet that isn't the triggered jet; default "jet1"
 - inc_train: selecting either train80 or train40 for inclusove score; default "80"
 - depth: selecting either hcal or anywhere for depth score; default 'hcal'
 - CNN: use 3DCNN scores; default False

Note that combined is not currently supported. Zmu is not currently supported, only LLPSkim files and signal files.
