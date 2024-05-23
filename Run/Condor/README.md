# Condor Submission

**1. Modify submission parameters**

In `condor_run.py`: Modify file paths in "MODIFY ME" section, if necessary.

**2. Create grid proxy:**

```
voms-proxy-init --voms cms 
```
Copy over output file from `/tmp/` area into local or home area.

**3. Run:**

Remember to recompile the DisplacedHcalJetAnalyzer executable! Important: make sure that the environment `DisplacedHcalJetAnalyzer` was compiled in is the same as the CMSSW release `condor_executable.sh` sets up! This is currently `CMSSW_13_2_0`. 

Zip the BDT weights files, `zip -r <zip file name> <directory name>`.

Submit a condor job for each file in input text file:
```
python3 condor_run.py -i <input_file_path> -o <output_directory_tag> -p <path_to_proxy_file> <other optional arguments, -d -t -f>
```

For full usage:
```
python3 condor_run.py --help
```

A bash script is set up to submit all the 2023 displaced jet skim and the LLP MC: `./submit_MC.sh`. It is helpful to check the number of files produced in each script to make sure that all the files are processed (may be due to job flavor not allowing for a long enough run time!). Use `ls -1 <directory> | wc -l` to check the number of files in a directory.

## Examples of submissions
```
voms-proxy-init --voms cms 
cp /tmp/x509up_u101898 ./

python3 condor_run.py -i ../InputFiles_2023_10_09_QCD_PT-15to7000_TuneCP5_13pt6TeV_pythia8_ntuplesv2.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v2.0_MC_QCD_250k -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -d -f MC_QCD
python3 condor_run.py -i ../InputFiles_2023_10_17_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_ntuplesv2.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v2.0_MC_LLP_500k -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -d -f MC_LLP
python3 condor_run.py -i ../InputFiles_2023_10_17_Run2023C-EXOLLPJetHCAL-PromptReco-v4_ntuplesv2.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v2.0_LLPskimRun2023C_500k -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -d -f LLPskim

condor_watch_q
```
Proxy should be copied to local directory, had issues when copied elsewhere. A CMSSW environment is be set up from the condor run script. `-t` is for test, remove this argument to run over the whole dataset. `-f` gives the flag for the job directory, such that multiple submissions can be made one after the other without overwriting the job directory.

outputs:
