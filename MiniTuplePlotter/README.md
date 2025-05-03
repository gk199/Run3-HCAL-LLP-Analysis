# MiniTuplePlotter

Goal: A flexible and dynamic plotting framework -- input welcome!

## Description 

Code framework includes files where any committed changes should be backwards-compatible and for long-term / stable use:
* `MiniTuplePlotter.h` -- Plotter class lives here. Pass it input file names, tree names, cuts, etc 
* `PlotFunctions.h` -- General functions, including fits, that are called by the plotter class. Changes should be backwards-compatible
* `PlotParams.h` -- Defines a struct PlotParams that contains information you need to plot a flat ttree branch, including the branch name, the hist name, the x-axis name, and the x-axis bounds. Gets called by the MiniTuplePlotter. Add variables accordingly

Each user can define their own plotting scripts -- see an example in `MiniTuplePlotterExample.C`. In order to keep this working directory clean, if/when users commit their own plotting scripts, please do so in a directory here with your name (e.g. in `Kiley/`). If a script becomes "offical" enough we can move it to the main space here. 

## Run the Example

Please look at the various options in the example file (this will make the output actually make sense) and feel free to play around with them.
```
mkdir Plots
root -q -b -l MiniTuplePlotterExample.C
```

## Wishlist / To-Dos

* Better cut names in the legend
* Different debug levels

# Additional Scripts

## Phase Scan Triggers
```
python3 PhaseScanEnergyInvestigations.py
```
Evaluate the multiplicity of HCAL rechits > 4 GeV vs jet pT or event HT. The rechits in the minituples (v3.13 and above) are saved within dR < 0.4 of a L1 LLP flagged L1 jet, so there is some bias due to this (these jets will have 2+ rechits as they are triggered, and we are not evaluating rechits for jets that don't pass the trigger or rechits not affiliated with a jet). 