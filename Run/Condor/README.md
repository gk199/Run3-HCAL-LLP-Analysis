# Condor Submission

**1. Modify submission parameters**

In `condor_run.py`: Modify file paths in "MODIFY ME" section, if necessary.

**2. Create grid proxy:**

```
voms-proxy-init --voms cms 
```
Copy over output file from `/tmp/` area into local or home area.

**3. Run:**

Submit a condor job for each file in input text file:
```
python3 condor_run.py -i <input_file_path> -o <output_directory_tag> -p <path_to_proxy_file> <other optional arguments>
```

For full usage:
```
python3 condor_run.py --help
```

## Examples of submissions
```
voms-proxy-init --voms cms 
cp /tmp/x509up_u101898 ./
python3 condor_run.py -i ../InputFiles_2023_06_29_MCsignal_100k.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_2023_06_29_MCsignal_100k -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -d -t
condor_watch_q
```
Proxy should be copied to local directory, had issues when copied elsewhere. A CMSSW environment is be set up from the condor run script. `-t` is for test, remove this argument to run over the whole dataset. 

outputs:
