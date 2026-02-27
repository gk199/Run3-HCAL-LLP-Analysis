#!/bin/bash

echo "Running over 2022 output background predictions:"
echo "Scanning depth DNN score, with b-tagged categories combined"
python3 OutputToLatex_combined.py -e 2022 -s 0.7 -b
python3 OutputToLatex_combined.py -e 2022 -s 0.8 -b 
python3 OutputToLatex_combined.py -e 2022 -s 0.9 -b 
echo "Scanning depth DNN score, with b-tagged categories separated"
python3 OutputToLatex.py -e 2022 -s 0.7 
python3 OutputToLatex.py -e 2022 -s 0.8 
python3 OutputToLatex.py -e 2022 -s 0.9 

echo
echo "Running over 2023 output background predictions:"
echo "Scanning depth DNN score, with b-tagged categories combined"
python3 OutputToLatex_combined.py -e 2023 -s 0.7 -b 
python3 OutputToLatex_combined.py -e 2023 -s 0.8 -b 
python3 OutputToLatex_combined.py -e 2023 -s 0.9 -b
echo "Scanning depth DNN score, with b-tagged categories separated"
python3 OutputToLatex.py -e 2023 -s 0.7 
python3 OutputToLatex.py -e 2023 -s 0.8 
python3 OutputToLatex.py -e 2023 -s 0.9 