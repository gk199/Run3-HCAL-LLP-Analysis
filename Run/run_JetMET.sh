#!/bin/bash
cd ../DisplacedHcalJetAnalyzer/
source compile.sh 
cd ../Run/
root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("test", "/eos/user/g/gkopp/LLP_Analysis/ntuple_output_test_data_METSkimtest.root")'
today=$(date +%d-%b-%Y)
mv hists_test.root hists_test_JetMET_$today.root
