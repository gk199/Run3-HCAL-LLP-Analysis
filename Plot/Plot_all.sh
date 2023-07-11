#!/bin/bash
echo "MC: all"
python Plotter.py /eos/user/g/gkopp/LLP_Analysis/output_minituples_2023_07_10_InputFiles_2023_06_29_MCsignal_100k/hadd_hists_output.root MC all
echo "MC: preECAL"
python Plotter.py /eos/user/g/gkopp/LLP_Analysis/output_minituples_2023_07_10_InputFiles_2023_06_29_MCsignal_100k/hadd_hists_output.root MC preECAL
echo "MC: ECAL"
python Plotter.py /eos/user/g/gkopp/LLP_Analysis/output_minituples_2023_07_10_InputFiles_2023_06_29_MCsignal_100k/hadd_hists_output.root MC ECAL
echo "MC: depth12"
python Plotter.py /eos/user/g/gkopp/LLP_Analysis/output_minituples_2023_07_10_InputFiles_2023_06_29_MCsignal_100k/hadd_hists_output.root MC depth12
echo "MC: depth34"
python Plotter.py /eos/user/g/gkopp/LLP_Analysis/output_minituples_2023_07_10_InputFiles_2023_06_29_MCsignal_100k/hadd_hists_output.root MC depth34
echo "data!"
python Plotter.py /eos/user/g/gkopp/LLP_Analysis/output_minituples_2023_07_10_InputFiles_2023_06_29_Run2023C-EXOLLPJetHCAL-PromptReco-v4_500k/hadd_hists_output.root data all