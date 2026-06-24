#!/bin/bash

echo "RUNNING"
echo "Timestamp: $(date '+%Y-%m-%d %H:%M:%S')\n"

# ----- Read Inputs ----- #

echo "Reading inputs and setting user proxy..."

BASE_DIR=$1
myproxy=$2
filetag=$3
uncert=$4
shift 4
ds_in="$@"  # Read multuple input files

# Proxy
export X509_USER_PROXY=$myproxy
voms-proxy-info -all
voms-proxy-info -all -file $myproxy

echo "Timestamp: $(date '+%Y-%m-%d %H:%M:%S')\n"

# ----- Create Minituple ----- #

echo "Unzipping Jet Veto Maps..."
unzip JetVetoMaps_Run3.zip > /dev/null

echo "Unpacking and compiling executable..."
unzip DisplacedHcalJetAnalyzer.zip > /dev/null

cd DisplacedHcalJetAnalyzer/
source compile.sh
cd ../

echo "Exporting LD_LIBRARY_PATH..."
export LD_LIBRARY_PATH=$(root-config --libdir):$LD_LIBRARY_PATH

echo "Creating minituple..."
echo "  - Infile: $ds_in"
echo "  -Filetag: $filetag"
echo "Timestamp: $(date '+%Y-%m-%d %H:%M:%S')\n"

echo "Running executable..."
echo "DisplacedHcalJetAnalyzer/exe/DisplacedHcalJetAnalyzer $filetag $uncert $ds_in"
DisplacedHcalJetAnalyzer/exe/DisplacedHcalJetAnalyzer $filetag $uncert $ds_in

echo "Timestamp: $(date '+%Y-%m-%d %H:%M:%S')\n"

# ----- Add scores ----- #

echo "Setting up DNN environment..."

echo "Unpacking classifier information:"
unzip Classifiers.zip > /dev/null

source VirtualEnvs/keras2pt13pt1/bin/activate
pip3 install wrapt
pip3 install awkward
pip3 install zipp

echo "Evaluating DNN..."
if echo "$ds_in" | grep -qE "Run2022|Run2023C|2022preEE|2022postEE"; then
    DNN_DEPTH="Evaluate/depth_model_v7_cat12.keras"
    DNN_INCL="Evaluate/inclusive_model_v7_cat12.keras"
else
    DNN_DEPTH="Evaluate/depth_model_v7_cat3.keras"
    DNN_INCL="Evaluate/inclusive_model_v7_cat3.keras"
fi
echo "  - DNN models: $DNN_DEPTH, $DNN_INCL"
python3 Evaluate/ScoresToEventBased_iterate.py -f minituple_$filetag.root -t NoSel -d $DNN_DEPTH -i $DNN_INCL -c Evaluate/norm_constants_v4.csv -m filewrite

echo "Timestamp: $(date '+%Y-%m-%d %H:%M:%S')\n"

sleep 5
