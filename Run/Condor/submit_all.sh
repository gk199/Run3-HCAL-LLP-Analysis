#!/bin/bash

# Update these!
output_basedir=/eos/home-g/gkopp/LLP_Analysis/MiniTuples/v5.6/
proxypath=~/private/x509up_u101898
systematic=Nominal

# for input_file in ../Ntuples_v5/InputFiles_DisplacedJet_Run2022*_v5.txt; do
for input_file in ../Ntuples_v5/InputFiles_DisplacedJet_Run2022C-v1_RAW_v5.txt; do
# for input_file in ../Ntuples_v5/InputFiles_DisplacedJet_Run2023*_v5.txt; do
# for input_file in ../Ntuples_v5/InputFiles_HToSSTo4B*_L1triggers_v5.txt; do
# for input_file in ../Ntuples_v5/Central/RAW_HToSSTo4B*_v5.txt; do

  filetag=`basename $input_file .txt | sed -E 's/^(InputFiles_|RAW_)//'`
  echo "Submitting job for: $input_file"
  echo "python3 condor_run.py -i $input_file -o $output_basedir$filetag -p $proxypath  -f $filetag$systematic -u $systematic"
  python3 condor_run.py -i $input_file -o $output_basedir$filetag -p $proxypath  -f $filetag$systematic -u $systematic

done
