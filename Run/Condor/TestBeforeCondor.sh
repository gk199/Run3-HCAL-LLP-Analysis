#!/bin/bash

echo "RUNNING..."

# Inputs
filetag=$1
ds_in=$2 # root://cmsxrootd.fnal.gov///store/user/gkopp/DisplacedJet/Run2023C-EXOLLPJetHCAL-PromptReco-v4_AOD_20231114_143556/231114_133610/0000/output_104.root

echo "ls"
ls *

echo ""
echo "Infile: $ds_in"
echo ""
echo "Filetag: $filetag"
echo ""

echo "Running executable..."
echo "./DisplacedHcalJetAnalyzer $filetag $ds_in"
./../../DisplacedHcalJetAnalyzer/exe/DisplacedHcalJetAnalyzer $filetag $ds_in

echo "Setting up environment for DNN"
echo "conda create, conda activate CondaDNNenv"
# conda create --name CondaDNNenv
# conda activate CondaDNNenv
# source /cvmfs/sft.cern.ch/lcg/views/LCG_105a_cuda/x86_64-el9-gcc11-opt/setup.sh
# source /cvmfs/sft.cern.ch/lcg/views/LCG_107_cuda/x86_64-el9-gcc11-opt/setup.sh

conda activate CondaDNNenv_py3pt9

echo "Evaluating DNN..."
cd ../../Classifiers/
echo "python3 ScoresToEventBased-v3.py minituple_$filetag.root"
python3 ScoresToEventBased-v3.py ../Run/Condor/minituple_$filetag.root # $filepath

sleep 5