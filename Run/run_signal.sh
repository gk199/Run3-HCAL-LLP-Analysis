#!/bin/bash
cd ../DisplacedHcalJetAnalyzer/
source compile.sh 
cd ../Run/
#root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("test", "root://cmsxrootd.fnal.gov///store/user/gkopp/ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV/LLP_MC_test__20230511_194008/230511_174021/0000/output_1.root")'
root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("test", "/eos/user/g/gkopp/LLP_Analysis/output_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_20230511_194008_20ksignal.root")'
today=$(date +%d-%b-%Y)
mv hists_test.root hists_test_signal20k_$today.root
