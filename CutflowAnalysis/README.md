# Cut-flow Table

The first argument is the ROOT file to evaluate the cutflow on, and the second is the jet pT cut (typically 40 or 60 GeV) -- only an option in the background estimation cutflow file as of now.
```
python3 CutflowTable.py /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_v3.0_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2023_11_29.root

python3 BkgEstimation.py /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.6.1/minituple_v3.6_LLPskim_Run2023_HADD.root 60
```
With option to print as table to terminal, or print in a latex format (`print_latex = True`). 

# Final Analysis Selections (LLP MC)
```
python3 FinalAnalysisCutflow.py --inc 0.97 --depth 0.95 --latex
python3 FinalAnalysisCutflow.py --inc 0.97 --depth 0.95 --truth --latex

python3 FinalAnalysisCutflow.py file.root --depth 0.965 --depth-sjdc 0.975 --inc 0.845 --inc-sjdc 0.375

python3 FinalAnalysisCutflow.py --file <your_file.root> --latex --depth 0.965 --depth-sjdc 0.975 --inc 0.845 --inc-sjdc 0.375
python3 FinalAnalysisCutflow.py --file <your_file.root> --latex --truth --depth 0.965 --depth-sjdc 0.975 --inc 0.845 --inc-sjdc 0.375

python3 ScanDNNCuts.py --inc  0.90 0.95 0.97 0.99 --depth 0.90 0.93 0.95 0.97
```