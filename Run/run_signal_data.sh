#!/bin/bash
cd ../DisplacedHcalJetAnalyzer/
source compile.sh 
cd ../Run/
today=$(date +%d-%b-%Y)
./../DisplacedHcalJetAnalyzer/exe/DisplacedHcalJetAnalyzer data_30k_$today root://cmsxrootd.fnal.gov///store/user/gkopp/DisplacedJet/Run2023C-EXOLLPJetHCAL-PromptReco-v4_AOD_20230629_142804/230629_122816/0000/output_1-11.root  
./../DisplacedHcalJetAnalyzer/exe/DisplacedHcalJetAnalyzer signal_1k_$today root://cmsxrootd.fnal.gov///store/user/gkopp/ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV/LLP_MC_test__20230629_113607/230629_093620/0000/output_24.root