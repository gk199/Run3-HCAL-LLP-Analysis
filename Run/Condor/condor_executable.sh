#!/bin/bash

echo "RUNNING..."

# Inputs
myproxy=$1
filetag=$2
shift 2
ds_in="$@"

# Proxy
echo ""
echo "Setting grid proxy: $myproxy"
export X509_USER_PROXY=$myproxy
voms-proxy-info -all
voms-proxy-info -all -file $myproxy

echo "ls"
ls *
# DON'T setup CMSSW -- causes jobs to take 4x longer to run

echo ""
echo "Infile: $ds_in"
echo ""
echo "Filetag: $filetag"
echo ""

echo "Running executable..."
echo "./DisplacedHcalJetAnalyzer $filetag $ds_in"
./DisplacedHcalJetAnalyzer $filetag $ds_in

# Environment setup -- no packages sourced, just setting up C++ for compiling needs
echo "CMSSW environment"
cmsrel CMSSW_14_0_0
cd CMSSW_14_0_0/src
cmsenv
cd ../..

#echo "Unzipping BDT Score Files..."
#unzip BDTWeightFiles.zip

echo "ls"
ls *

echo "Setting up environment for DNN"
echo "conda create, conda activate CondaDNNenv"
conda create --name CondaDNNenv
conda activate CondaDNNenv
source /cvmfs/sft.cern.ch/lcg/views/LCG_105a_cuda/x86_64-el9-gcc11-opt/setup.sh # try updating this after the DNN part did not work
#echo "source /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/VirtualEnvs/keras2pt13pt1/bin/activate"
#source /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/VirtualEnvs/keras2pt13pt1/bin/activate
#=======
#echo "Setting up environment for DNN"
#echo "conda create, conda activate CondaDNNenv"
# conda create --name CondaDNNenv
#conda activate CondaDNNenv
#source /cvmfs/sft.cern.ch/lcg/views/LCG_105a_cuda/x86_64-el9-gcc11-opt/setup.sh
#>>>>>>> Stashed changes

#echo "Evaluating DNN..."
#echo "python3 ScoresToEventBased-v3.py minituple_$filetag.root"
#python3 ScoresToEventBased-v3.py minituple_$filetag.root # $filepath

sleep 5
