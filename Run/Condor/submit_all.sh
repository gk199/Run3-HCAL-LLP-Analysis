#!/bin/bash

echo "submitting all MC"

echo "Low mass LLP MC samples (train and test), 100 files total, 40-60 train-test split"
echo " "
python3 condor_run.py -i ../InputFiles_2024_01_19_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_ntuplesv3_TRAIN.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_MC_LLP125_15_TRAIN -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f MC_LLP_125_lowMass_train
python3 condor_run.py -i ../InputFiles_2024_01_19_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_ntuplesv3_TEST.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_MC_LLP125_15_TEST -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f MC_LLP_125_lowMass_test 

python3 condor_run.py -i ../InputFiles_2023_11_29_ggH_HTooSSTobbbb_MHH-350_MS-80_CTau500_ntuplesv3_TRAIN.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_MC_LLP350_80_TRAIN -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f MC_LLP_350_lowMass_train
python3 condor_run.py -i ../InputFiles_2023_11_29_ggH_HTooSSTobbbb_MHH-350_MS-80_CTau500_ntuplesv3_TEST.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_MC_LLP350_80_TEST -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f MC_LLP_350_lowMass_test

echo "High mass LLP MC samples (train and test, batch 1 and 2), 200 files each"
echo " "
echo "mH = 125"
python3 condor_run.py -i ../InputFiles_2024_02_12_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_MC_LLP125_50_batch1 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f MC_LLP_125_batch1
python3 condor_run.py -i ../InputFiles_2024_02_12_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_batch2_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_MC_LLP125_50_batch2 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f MC_LLP_125_batch2
echo "mH = 250"
python3 condor_run.py -i ../InputFiles_2024_02_12_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_MC_LLP250_120_batch1 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f MC_LLP_250_batch1
python3 condor_run.py -i ../InputFiles_2024_02_12_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_batch2_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_MC_LLP250_120_batch2 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f MC_LLP_250_batch2
echo "mH = 350"
python3 condor_run.py -i ../InputFiles_2024_02_12_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_MC_LLP350_160_batch1 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f MC_LLP_350_batch1
python3 condor_run.py -i ../InputFiles_2024_02_12_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_batch2_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_MC_LLP350_160_batch2 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f MC_LLP_350_batch2

echo "submitting 2023 displaced jet skim"
echo " "
echo "2023Bv1 -- 61 files"
# python3 condor_run.py -i ../InputFiles_2023_11_08_Run2023B-EXOLLPJetHCAL-PromptReco-v1_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Bv1 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Bv1
echo "2023Cv1 -- 922 files"
# python3 condor_run.py -i ../InputFiles_2023_11_08_Run2023C-EXOLLPJetHCAL-PromptReco-v1_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Cv1 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Cv1
echo "2023Cv2 -- 1304 files"
# python3 condor_run.py -i ../InputFiles_2023_11_08_Run2023C-EXOLLPJetHCAL-PromptReco-v2_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Cv2 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Cv2
echo "2023Cv3 -- 1484 files"
# python3 condor_run.py -i ../InputFiles_2023_11_08_Run2023C-EXOLLPJetHCAL-PromptReco-v3_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Cv3 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Cv3
echo "2023Cv4 -- 1855 files"
# python3 condor_run.py -i ../InputFiles_2023_11_14_Run2023C-EXOLLPJetHCAL-PromptReco-v4_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Cv4 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Cv4
echo "2023Dv1 -- 622 files"
# python3 condor_run.py -i ../InputFiles_2023_11_14_Run2023D-EXOLLPJetHCAL-PromptReco-v1_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Dv1 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Dv1
echo "2023Dv2 -- 83 files"
# python3 condor_run.py -i ../InputFiles_2023_11_14_Run2023D-EXOLLPJetHCAL-PromptReco-v2_ntuplesv3.txt -o /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.6_LLPskim_2023Dv2 -p /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/x509up_u101898 -b BDTWeightFiles.zip -f LLPskim_2023Dv2
# 6331 total

# to check number of files in each directory:
# ls -1 <directory> | wc -l