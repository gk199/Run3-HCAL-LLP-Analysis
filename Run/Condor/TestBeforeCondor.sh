#!/bin/bash

echo "RUNNING..."

# Inputs
# Example: ./TestBeforeCondor.sh Filetag ntuple-file.root
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
echo "source /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/VirtualEnvs/keras2pt13pt1/bin/activate"
#source /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/VirtualEnvs/keras2pt13pt1/bin/activate
#conda activate kerasModelv4
conda activate /afs/cern.ch/work/k/kikenned/Conda/LLP_kerasModelv4

echo "Evaluating DNN..."
echo "python3 ScoresToEventBased-v3.py minituple_$filetag.root"
cd ../../Classifiers/
python3 ScoresToEventBased-v4.py ../Run/Condor/minituple_$filetag.root # $filepath

sleep 5
