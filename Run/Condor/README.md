# Condor Submission

**1. Modify submission parameters**

In `condor_run.py`: Modify file paths in "MODIFY ME" section, if necessary.

**2. Create grid proxy:**

```
voms-proxy-init --voms cms 
```
Copy over output file from `/tmp/` area into local or home area.

**3. Run:**

Remember to recompile the DisplacedHcalJetAnalyzer executable! Important: make sure that the environment `DisplacedHcalJetAnalyzer` was compiled in is the same as the CMSSW release `condor_executable.sh` sets up! This is currently `CMSSW_14_0_0`. Do `cmsenv` in this release, logout, and login to submit the Condor jobs. 

Check the trees that are run over in `ScoreSToEventBased-v3.py` with `trees_to_iterate`. 

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

## Scrape Condor Output
This script can be used to check the run time of condor jobs, to inform setting a reasonable job flavor in `condor_header.cmd`. 

```
python3 scrape_condor_output.py <directory>
```
The script will look at all the `Job*` subdirectories and plot the runtime in minutes.

## TestBeforeCondor submission
```
./TestBeforeCondor.sh <test filename> <path to ROOT file with root://cmsxrootd.fnal.gov// prefix>
```
Test the file running before submitting to Condor.

## Debugging issues with DNN score addition
The DNN score addition is done by setting up a virtual environment and then running the DNN model over the trees. 

The virtual environment is set up based on the requirements file from the SWAN area where the DNN model was created:
```
pip freeze > requirements.txt # in SWAN area
conda create --name <env_name> --file requirements.txt
```
In both November and March, had issues with the requirements.txt, as many package are not available (including tensorflow, keras, and uproot!). Instead used an approach of sourcing the LCG environment. 

The environment name is choosen to be `CondaDNNenv`, and then scores are added with:
```
conda activate CondaDNNenv
source /cvmfs/sft.cern.ch/lcg/views/LCG_105a_cuda/x86_64-el9-gcc11-opt/setup.sh
python3 ScoresToEventBased-v3.py minituple_$filetag.root
```

In November 2024 the above sourcing worked, however in March 2025 it did not work. This was added based on the recommendation to source a LCG file with an up to date tensorflow: https://root-forum.cern.ch/t/incompatibility-among-tensorflow-of-lxplus-with-tensorflow-macbook-pro-colab-notebook-and-swan/56336. Without this, the DNN model would not work on lxplus / Condor area. 

Still debugging March issue.... Newer versions of the `setup.sh` file have not worked yet.