#!/bin/bash

echo "Running over 2022 output background predictions:"
echo "Scanning depth DNN score, with b-tagged categories combined"
# python3 OutputToLatex_VRclosureCheck.py -e 2022 -s 0.85 -b
# python3 OutputToLatex_VRclosureCheck.py -e 2022 -s 0.9 -b 
# python3 OutputToLatex_VRclosureCheck.py -e 2022 -s 0.95 -b 
# python3 OutputToLatex_VRclosureCheck.py -e 2022_DE -s 0.85 -b
# python3 OutputToLatex_VRclosureCheck.py -e 2022_DE -s 0.9 -b 
# python3 OutputToLatex_VRclosureCheck.py -e 2022_DE -s 0.95 -b 
# python3 OutputToLatex_VRclosureCheck.py -e 2022_FG -s 0.85 -b
# python3 OutputToLatex_VRclosureCheck.py -e 2022_FG -s 0.9 -b 
# python3 OutputToLatex_VRclosureCheck.py -e 2022_FG -s 0.95 -b 
# python3 OutputToLatex_VRclosureCheck.py -e 2022_F -s 0.85 -b
# python3 OutputToLatex_VRclosureCheck.py -e 2022_F -s 0.9 -b 
# python3 OutputToLatex_VRclosureCheck.py -e 2022_F -s 0.95 -b 
# python3 OutputToLatex_VRclosureCheck.py -e 2022_G -s 0.85 -b
# python3 OutputToLatex_VRclosureCheck.py -e 2022_G -s 0.9 -b 
# python3 OutputToLatex_VRclosureCheck.py -e 2022_G -s 0.95 -b 
# echo "Scanning depth DNN score, with b-tagged categories separated"
# python3 OutputToLatex.py -e 2022 -s 0.7 
# python3 OutputToLatex.py -e 2022 -s 0.8 
# python3 OutputToLatex.py -e 2022 -s 0.9 
# python3 OutputToLatex.py -e 2022_DE -s 0.7 
# python3 OutputToLatex.py -e 2022_DE -s 0.8 
# python3 OutputToLatex.py -e 2022_DE -s 0.9 
# python3 OutputToLatex.py -e 2022_F -s 0.7 
# python3 OutputToLatex.py -e 2022_F -s 0.8 
# python3 OutputToLatex.py -e 2022_F -s 0.9 
# python3 OutputToLatex.py -e 2022_G -s 0.7 
# python3 OutputToLatex.py -e 2022_G -s 0.8 
# python3 OutputToLatex.py -e 2022_G -s 0.9 

echo
# echo "Running over 2023 output background predictions:"
# echo "Scanning depth DNN score, with b-tagged categories combined"
python3 OutputToLatex_VRclosureCheck.py -e 2023 -s 0.85 -b 
python3 OutputToLatex_VRclosureCheck.py -e 2023 -s 0.9 -b 
python3 OutputToLatex_VRclosureCheck.py -e 2023 -s 0.95 -b
python3 OutputToLatex_VRclosureCheck.py -e 2023_C1234 -s 0.85 -b 
python3 OutputToLatex_VRclosureCheck.py -e 2023_C1234 -s 0.9 -b 
python3 OutputToLatex_VRclosureCheck.py -e 2023_C1234 -s 0.95 -b
python3 OutputToLatex_VRclosureCheck.py -e 2023_D -s 0.85 -b 
python3 OutputToLatex_VRclosureCheck.py -e 2023_D -s 0.9 -b 
python3 OutputToLatex_VRclosureCheck.py -e 2023_D -s 0.95 -b
# echo "Scanning depth DNN score, with b-tagged categories separated"
# python3 OutputToLatex.py -e 2023 -s 0.7 
# python3 OutputToLatex.py -e 2023 -s 0.8 
# python3 OutputToLatex.py -e 2023 -s 0.9 
# python3 OutputToLatex.py -e 2023_C123 -s 0.7 
# python3 OutputToLatex.py -e 2023_C123 -s 0.8 
# python3 OutputToLatex.py -e 2023_C123 -s 0.9 
# python3 OutputToLatex.py -e 2023_C4D -s 0.7 
# python3 OutputToLatex.py -e 2023_C4D -s 0.8 
# python3 OutputToLatex.py -e 2023_C4D -s 0.9 