#!/bin/bash
echo "MC: all, preECAL, ECAL, depth12, depth34, depth3, depth4"
python Plotter.py /eos/user/g/gkopp/LLP_Analysis/output_minituples_v1_MCsignal_500k_2023_07_14/hadd_hists_output.root MC
echo "data!"
python Plotter.py /eos/user/g/gkopp/LLP_Analysis/output_minituples_v1_LLPskim_500k_2023_07_14/hadd_hists_output.root data


#python Plotter.py /eos/user/g/gkopp/LLP_Analysis/output_minituples_2023_07_10_InputFiles_2023_06_29_MCsignal_100k/hadd_hists_output.root MC all
#python Plotter.py /eos/user/g/gkopp/LLP_Analysis/output_minituples_2023_07_10_InputFiles_2023_06_29_Run2023C-EXOLLPJetHCAL-PromptReco-v4_500k/hadd_hists_output.root data all