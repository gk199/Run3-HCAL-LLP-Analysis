#!/bin/bash

echo "RUNNING..."

# Inputs
myproxy=$1
filetag=$2
shift 2
ds_in="$@"  # Read multuple input files

# Proxy
echo ""
echo "Setting grid proxy: $myproxy"
export X509_USER_PROXY=$myproxy
voms-proxy-info -all
voms-proxy-info -all -file $myproxy

echo "Timestamp: $(date '+%Y-%m-%d %H:%M:%S')"

echo "ls"
ls *

echo ""
echo "Infile: $ds_in"
echo ""
echo "Filetag: $filetag"
echo ""

echo "Timestamp: $(date '+%Y-%m-%d %H:%M:%S')"

echo "export LD_LIBRARY_PATH=$(root-config --libdir):$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH=$(root-config --libdir):$LD_LIBRARY_PATH

echo "Running executable..."
echo "./DisplacedHcalJetAnalyzer $filetag $ds_in"
./DisplacedHcalJetAnalyzer $filetag $ds_in

#echo "Timestamp: $(date '+%Y-%m-%d %H:%M:%S')"

#echo "Setting up environment for DNN"
# echo "conda create, conda activate CondaDNNenv"
# conda create --name CondaDNNenv
# conda activate CondaDNNenv
# source /cvmfs/sft.cern.ch/lcg/views/LCG_105a_cuda/x86_64-el9-gcc11-opt/setup.sh # try updating this after the DNN part did not work
#echo "source /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/VirtualEnvs/keras2pt13pt1/bin/activate"
#source /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/Condor/VirtualEnvs/keras2pt13pt1/bin/activate
#source /afs/cern.ch/work/g/gkopp/public/forKiley/activate/afs/cern.ch/work/g/gkopp/public/forKiley/activate/afs/cern.ch/work/g/gkopp/public/forKiley/activate
#source /afs/cern.ch/work/g/gkopp/public/forKiley/activate

#echo "source /afs/cern.ch/work/k/kikenned/Run3-HCAL-LLP-Analysis-Gillian/Run/Condor/keras2pt13pt1/bin/activate"
#source /afs/cern.ch/work/k/kikenned/Run3-HCAL-LLP-Analysis-Gillian/Run/Condor/keras2pt13pt1/bin/activate

#echo "Timestamp: $(date '+%Y-%m-%d %H:%M:%S')"

#echo "Evaluating DNN..."
#echo "python3 ScoresToEventBased-v4.py minituple_$filetag.root"
#python3 ScoresToEventBased-v4.py minituple_$filetag.root # $filepath
#rm minituple_$filetag.root

echo "Timestamp: $(date '+%Y-%m-%d %H:%M:%S')"

sleep 5
