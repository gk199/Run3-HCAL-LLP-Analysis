#!/bin/bash

echo "Running over 2022 data:"
echo "Scanning depth DNN score, with b-tagged categories combined"
python3 MisTagParametrization_3D.py -e 2022 -s 0.7 -b
python3 MisTagParametrization_3D.py -e 2022 -s 0.8 -b 
python3 MisTagParametrization_3D.py -e 2022 -s 0.9 -b 
echo "Scanning depth DNN score, with b-tagged categories separated"
python3 MisTagParametrization_3D.py -e 2022 -s 0.7 
python3 MisTagParametrization_3D.py -e 2022 -s 0.8 
python3 MisTagParametrization_3D.py -e 2022 -s 0.9 

echo 
echo "Running over 2023 data:"
echo "Scanning depth DNN score, with b-tagged categories combined"
python3 MisTagParametrization_3D.py -e 2023 -s 0.7 -b 
python3 MisTagParametrization_3D.py -e 2023 -s 0.8 -b 
python3 MisTagParametrization_3D.py -e 2023 -s 0.9 -b
echo "Scanning depth DNN score, with b-tagged categories separated"
python3 MisTagParametrization_3D.py -e 2023 -s 0.7 
python3 MisTagParametrization_3D.py -e 2023 -s 0.8 
python3 MisTagParametrization_3D.py -e 2023 -s 0.9 