#!/bin/bash

# Update these!
output_basedir=/eos/home-k/kikenned/Run3-HCAL-LLP-Analysis/MiniTuples/v5.1/
proxypath=~/private/x509up_u101440

for input_file in ../Ntuples_v5/InputFiles*.txt; do

  filetag=`basename $input_file .txt | sed 's/^InputFiles_//'`
  echo "Submitting job for: $input_files"
  echo "python3 condor_run.py -i $input_file -o $output_basedir$filetag -p  ~/private/x509up_u101440  -f $filetag"
  python3 condor_run.py -i $input_file -o $output_basedir$filetag -p  ~/private/x509up_u101440  -f $filetag

done
