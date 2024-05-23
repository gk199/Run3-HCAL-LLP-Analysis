#!/bin/bash

echo "RUNNING..."

# Inputs
myproxy=$1
filetag=$2
ds_in=$3

# Proxy
echo ""
echo "Setting grid proxy: $myproxy"
export X509_USER_PROXY=$myproxy
voms-proxy-info -all
voms-proxy-info -all -file $myproxy

# Environment setup -- no packages sourced, just setting up C++ for compiling needs
echo "CMSSW environment"
cmsrel CMSSW_13_2_0
cd CMSSW_13_2_0/src
cmsenv
cd ../..

echo "Unzipping BDT Score Files..."
unzip BDTWeightFiles.zip

echo "ls"
ls *

echo ""
echo "Infile: $ds_in"
echo ""
echo "Filetag: $filetag"
echo ""

echo "Running executable..."
echo "./DisplacedHcalJetAnalyzer $filetag $ds_in"
./DisplacedHcalJetAnalyzer $filetag $ds_in

sleep 5
