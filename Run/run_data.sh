#!/bin/bash
cd ../DisplacedHcalJetAnalyzer/
source compile.sh 
cd ../Run/
root -q -b -l '../DisplacedHcalJetAnalyzer/util/DisplacedHcalJetAnalyzer.C("test", "/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/Run3-HCAL-LLP-NTupler/run/ntuple_output_test_data1_April6.root")' 
today=$(date +%d-%b-%Y)
mv hists_test.root hists_test_data1_$today.root
