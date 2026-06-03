# SR Optimization

## About

This simple SR optimization script runs computes the optimal SR cuts on the inclusive and depth score on a specific signal point. Here, a "shallow" background estimation method is used, which is basically an ABCD method -- similar to the full background estimation but inclusive in jet pt and eta. This uses a "brute force" approach, where the significance is calculated for a grid of score cuts. 

## Run 

See options:
```
python3 optimize_cuts.py -h

# Usage: 
python3 optimize_cuts.py -i <input-filetag> -s <settings-filetag> <optional arguments: -c, -p, -t>
```
To calculate significance for a grid of signal points with `-c`. The output will be a .npy file. This step may take a while, so it has been decoupled from the plotting step (next step). 
```
python3 optimize_cuts.py -i HToSSTo4B_125_50_CTau3000 -s mytest -c
```
To plot the map/grid of significance values and their contours, use the option `-p`. This will read in information from .npy files, so the input signal tag and settings tags will both need to match what you used with the calculation step.
```
mkdir Plots
python3 optimize_cuts.py -i HToSSTo4B_125_50_CTau3000 -s mytest -p
```
To print out misc information to the termial, use option `-t`. This is really just for playing around and isn't really part of this tool. Feel free to edit for your own usage.
```
python3 optimize_cuts.py -i HToSSTo4B_125_50_CTau3000 -s mytest -t
```
To run everything:
```
# You will need to make a "Plots" directory
python3 optimize_cuts.py -i HToSSTo4B_125_50_CTau3000 -s mytest -c -p -t
```
To run over all (private) signal points:
```
source run_examples.sh
```
## Editing Additional Options

You can edit additional options directly in `optimize_cuts.py`, including the following: 
* `root_file_path`
* `version`
* `n_bins_global`
* `score_range_incl_global`
* `score_range_depth_global`
* score_range_depth_global

Please note that editing many of these in-script global variables at the top of the scropt *only* controls inputs to the calculate_results (`-c`) function -- the plot_results (`-p`) function automatically stores these variables in the .npy file. 
