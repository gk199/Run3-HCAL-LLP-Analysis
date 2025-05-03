#!/bin/bash

echo "submitting 2023 Zmu skim"
echo " "
echo "2023Bv1 -- "
python3 condor_run.py -i ../InputFiles_2024_08_25_Run2023B-Zmu-PromptReco-v1_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.13_Zmu_2023Bv1 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -f Zmu_2023Bv1
echo "2023Cv1 -- "
python3 condor_run.py -i ../InputFiles_2024_08_25_Run2023C-Zmu-PromptReco-v1_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.13_Zmu_2023Cv1 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -f Zmu_2023Cv1
echo "2023Cv2 -- "
python3 condor_run.py -i ../InputFiles_2024_08_25_Run2023C-Zmu-PromptReco-v2_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.13_Zmu_2023Cv2 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -f Zmu_2023Cv2
echo "2023Cv3 -- "
python3 condor_run.py -i ../InputFiles_2024_08_25_Run2023C-Zmu-PromptReco-v3_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.13_Zmu_2023Cv3 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -f Zmu_2023Cv3
echo "2023Cv4 -- "
python3 condor_run.py -i ../InputFiles_2024_08_25_Run2023C-Zmu-PromptReco-v4_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.13_Zmu_2023Cv4 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -f Zmu_2023Cv4
echo "2023Dv1 -- "
python3 condor_run.py -i ../InputFiles_2024_08_25_Run2023D-Zmu-PromptReco-v1_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.13_Zmu_2023Dv1 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -f Zmu_2023Dv1
echo "2023Dv2 -- "
python3 condor_run.py -i ../InputFiles_2024_08_25_Run2023D-Zmu-PromptReco-v2_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.13_Zmu_2023Dv2 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -f Zmu_2023Dv2

# to check number of files in each directory:
# ls -1 <directory> | wc -l
