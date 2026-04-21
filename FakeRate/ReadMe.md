# Running

## Mistag Rate Evaluation
The newer approach uses 3D histograms to evaluate the mistag rates, so information from each dimension can be used in the prediction.
```
python3 MisTagParametrization_3D.py -e <era or year> -i <inclusive DNN score> -d <depth DNN score> -b

python3 MisTagParametrization_3D_optimized.py -e <era or year> -i <inclusive DNN score> -d <depth DNN score> -c <CR inclusive score> -b

# Both LJDC and SJDC use same depth/inclusive cut:
python3 MisTagParametrization_3D_optimized.py -e 2023 -b -d 0.93 -i 0.97

# Asymmetric cuts (different for leading vs subleading depth candidate):
python3 MisTagParametrization_3D_optimized.py -e 2023 -b \
    --DNN_cut_LJDC 0.965 --DNN_cut_SJDC 0.975 \
    --DNN_cut_inc_LJDC 0.845 --DNN_cut_inc_SJDC 0.375 \
    -c 0.2
```

### Config file mode (scan multiple parameter sets efficiently)
Create a whitespace-delimited file where the first non-comment line is the header (e.g. `my_scan.txt`):
```
# era   DNN_cut_LJDC  DNN_cut_inc_LJDC  DNN_cut_SJDC  DNN_cut_inc_SJDC  CR_cut_inc
2022    0.93          0.97              0.93           0.97              0.20
2023    0.93          0.97              0.93           0.97              0.20
2023    0.95          0.97              0.95           0.97              0.20
```
Then run:
```
python3 MisTagParametrization_3D_optimized.py --config my_scan.txt -b
```
Parameter sets grouped by era share a single RDataFrame, so multiple 2023 rows only read the files once. `CR_cut_inc` is optional and defaults to 0.2 if the column is omitted.

This will write to `DNN_pt*_202*_forPython_combined.txt`. For the case with b-tagged categories separated and written to `DNN_pt*_202*_forPython.txt`, do For the case with b-tagged categories separated and written to `DNN_pt*_202*_forPython.txt`, do
```
python3 MisTagParametrization_3D.py -e <era or year> -s <depth DNN score> 
```
Now the arguments passed determine which year, DNN score, and b-tag option is run. Combined `-b` does not split by b-tagging. The bash script `./ScanScore.sh` scans over year and DNN score. 

Change the booleans `Zmu` and `LLPskim` and the era name at the beginning of the file to control which files are run over.

## Overlay Mistag Rates
```
python3 OverlayMistagRates.py
```
This takes the output root files from `MisTagParametrization_3D` and overlays them to evalute changes throughout the year.

## Convert Fake Rate Results to Latex Tables

### VR closure tables (output of MisTagParametrization_3D_optimized.py)
`OutputToLatex_VRclosureCheck.py` reads the `*_forPython_combined.txt` files and prints LaTeX tables showing observed vs predicted mistag events in the VR, with systematic uncertainties from the low/high PV variations.

Single parameter set:
```
python3 OutputToLatex_VRclosureCheck.py -e 2023 -d 0.93 -i 0.97 -b
```

Asymmetric LJDC/SJDC cuts:
```
python3 OutputToLatex_VRclosureCheck.py -e 2023 -d 0.965 -i 0.845 \
    --depth_score_sjdc 0.975 --inclusive_score_sjdc 0.375 -b
```

Config file mode — process the same scan config used to produce the output files, one table per row:
```
python3 OutputToLatex_VRclosureCheck.py --config my_scan.txt -b
```
Files that don't exist yet are skipped with a `% WARNING` comment. Add `--stat_only` to evaluate closure with statistical uncertainties only (no PV-variation systematic).

### Legacy tables (output of MisTagParametrization_3D.py)
```
python3 OutputToLatex.py -e 2022 -s 0.7 # uses DNN_pt*_forPython.txt
python3 OutputToLatex_combined.py -e 2022 -d 0.7 -i 0.7 -b # uses DNN_pt*_forPython_combined.txt
```
Score, year, and combined b-tag categories are now passed via arguments to the python script.

`OutputToLatex.py` is for the results of running `MisTagParametrization_3D.py` with depth, b-tagged (leading, subleading, combined); depth, not b-tagged; depth, b-tagged, low PV; depth, not b-tagged, low PV; depth, b-tagged, high PV; depth, not b-tagged, high PV (used for Tables 8-11 in v2 of the AN). `OutputToLatex_combined.py` is for depth (leading, subleading, combined); depth, low PV; depth, high PV (used for Table 12 in v2 of the AN).

To run everything:
```
./ScanScore.sh
./ProduceTables.sh
```

# Scan Scores for Optimization
## Full scan with defaults (depth: 0.90–0.97, inc: 0.93–0.99, eras 2022+2023)
```
cd FakeRate/
python3 ScanDNNCuts_bkg.py
```

## Custom scan
```
python3 ScanDNNCuts_bkg.py -e 2022 -d 0.93 0.95 0.97 -i 0.95 0.97
```

## Check commands without running
```
python3 ScanDNNCuts_bkg.py --dry-run
```

## Re-print summary from already-existing output files
```
python3 ScanDNNCuts_bkg.py --parse-only
```

## Config file scan (separate LJDC/SJDC cuts, my_scan.txt format)
This enables different cuts to be used for the leading and subleading jet, and different cuts for each year. 
```
python3 ScanDNNCuts_bkg_config.py --config my_scan.txt
python3 ScanDNNCuts_bkg_config.py --config my_scan.txt --dry-run
python3 ScanDNNCuts_bkg_config.py --config my_scan.txt --parse-only
```
Prints a flat summary table (one row per config entry) showing predicted SR yield for leading, sub-leading, and combined jets, plus the VR closure pull (obs−pred)/σ for leading and sub-leading. Uses the same config file as `MisTagParametrization_3D_optimized.py --config`.

# Archive

## Mistag Rate: Initial Version
```
python3 MisTagProbability.py
python3 MisTagProbability_emulated.py
python3 MisTagProbability_CR_VR.py
```

The above three files are: ``MisTagProbability`` for using the W+jets and Z+mu selections from the Zmu skim to perform the background estimation with the DNN split by run range, fractional track pT, or electron energy fraction; ``MisTagProbability_emulated`` for splitting by emulated depth or timing towers on the Zmu skim; and ``MisTagProbability_CR_VR`` for evaluating the mistag fraction in the data validation region (subleading jet inclusive score between 0.5 - 0.9) from the control region (inclusive score beween 0-0.5).

For the triggered jet: -9999 means not matched to a L1 jet, 0 means matched with LLP flag not set, 1 means matched with LLP flag set.
