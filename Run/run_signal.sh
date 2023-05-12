#!/bin/bash
cd ../DisplacedHcalJetAnalyzer/
source compile.sh 
cd ../Run/
#root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("test", "/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/Run3-HCAL-LLP-NTupler/run/ntuple_output_test_signal1.root")'
#root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("test", "/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/llp_ntupler/run/ntuple_output_test_signal1.root")'
#root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("test", "/afs/cern.ch/work/k/kikenned/public/Run3-HCAL-LLP-Analysis-Files/NTuples/2023-04-17/ntuple_output_test_signal_allrechits.root")'
root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("test", "root://cmsxrootd.fnal.gov///store/user/gkopp/ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV/LLP_MC_test__20230511_194008/230511_174021/0000/output_1.root")'
today=$(date +%d-%b-%Y)
mv hists_test.root hists_test_signal1_$today.root
