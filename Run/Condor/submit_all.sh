#!/bin/bash

# Update these!
output_basedir=/eos/home-g/gkopp/LLP_Analysis/MiniTuples/v5.5/
proxypath=~/private/x509up_u101898
systematic=Nominal

#for input_file in ../Ntuples_v5/InputFiles_DisplacedJet_Run2023*_v5.txt; do
for input_file in ../Ntuples_v5/Central/RAW_HToSSTo4B_MH125_MS12_CTau9000_Premix_2022preEE_batch1_L1triggers_v5.txt; do

  filetag=`basename $input_file .txt | sed -E 's/^(InputFiles_|RAW_)//'`
  echo "Submitting job for: $input_files"
  echo "python3 condor_run.py -i $input_file -o $output_basedir$filetag -p $proxypath  -f $filetag$systematic -u $systematic"
  python3 condor_run.py -i $input_file -o $output_basedir$filetag -p $proxypath  -f $filetag$systematic -u $systematic

done
