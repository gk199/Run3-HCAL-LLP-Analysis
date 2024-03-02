#!/bin/bash

echo "submitting 2023 displaced jet skim"
echo " "
echo "2023Bv1 -- 61 files"
# python3 condor_run.py -i ../InputFiles_2023_11_08_Run2023B-EXOLLPJetHCAL-PromptReco-v1_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Bv1 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Bv1
echo "2023Cv1 -- 922 files"
# python3 condor_run.py -i ../InputFiles_2023_11_08_Run2023C-EXOLLPJetHCAL-PromptReco-v1_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Cv1 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Cv1
echo "2023Cv2 -- 1304 files"
python3 condor_run.py -i ../InputFiles_2023_11_08_Run2023C-EXOLLPJetHCAL-PromptReco-v2_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Cv2 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Cv2
echo "2023Cv3 -- 1484 files"
python3 condor_run.py -i ../InputFiles_2023_11_08_Run2023C-EXOLLPJetHCAL-PromptReco-v3_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Cv3 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Cv3
echo "2023Cv4 -- 1855 files"
python3 condor_run.py -i ../InputFiles_2023_11_14_Run2023C-EXOLLPJetHCAL-PromptReco-v4_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Cv4 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Cv4
echo "2023Dv1 -- 622 files"
python3 condor_run.py -i ../InputFiles_2023_11_14_Run2023D-EXOLLPJetHCAL-PromptReco-v1_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Dv1 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Dv1
echo "2023Dv2 -- 83 files"
python3 condor_run.py -i ../InputFiles_2023_11_14_Run2023D-EXOLLPJetHCAL-PromptReco-v2_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Dv2 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Dv2
# 6331 total

# to check number of files in each directory:
# ls -1 <directory> | wc -l