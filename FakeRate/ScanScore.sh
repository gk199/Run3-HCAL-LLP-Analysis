#!/bin/bash

echo "Running over 2022 data:"
echo "Scanning depth DNN score, with b-tagged categories combined"
python3 MisTagParametrization_3D.py -e 2022 -d 0.0 -b
python3 MisTagParametrization_3D.py -e 2022 -d 0.93 -b 
python3 MisTagParametrization_3D.py -e 2022 -d 0.95 -b 
python3 MisTagParametrization_3D.py -e 2022 -d 0.97 -b 
# python3 MisTagParametrization_3D.py -e 2022_DE -d 0.85 -b
# python3 MisTagParametrization_3D.py -e 2022_DE -d 0.9 -b
# python3 MisTagParametrization_3D.py -e 2022_DE -d 0.95 -b
# python3 MisTagParametrization_3D.py -e 2022_FG -d 0.85 -b
# python3 MisTagParametrization_3D.py -e 2022_FG -d 0.9 -b
# python3 MisTagParametrization_3D.py -e 2022_FG -d 0.95 -b
# python3 MisTagParametrization_3D.py -e 2022_D -d 0.9 -b
# python3 MisTagParametrization_3D.py -e 2022_E -d 0.9 -b
# python3 MisTagParametrization_3D.py -e 2022_F -d 0.85 -b
# python3 MisTagParametrization_3D.py -e 2022_F -d 0.9 -b
# python3 MisTagParametrization_3D.py -e 2022_F -d 0.95 -b
# python3 MisTagParametrization_3D.py -e 2022_G -d 0.85 -b
# python3 MisTagParametrization_3D.py -e 2022_G -d 0.9 -b
# python3 MisTagParametrization_3D.py -e 2022_G -d 0.95 -b
echo "Scanning depth DNN score, with b-tagged categories separated"
# python3 MisTagParametrization_3D.py -e 2022 -d 0.7 
# python3 MisTagParametrization_3D.py -e 2022 -d 0.8 
# python3 MisTagParametrization_3D.py -e 2022 -d 0.9 
# python3 MisTagParametrization_3D.py -e 2022_DE -d 0.7 
# python3 MisTagParametrization_3D.py -e 2022_DE -d 0.8 
# python3 MisTagParametrization_3D.py -e 2022_DE -d 0.9 
# python3 MisTagParametrization_3D.py -e 2022_F -d 0.7 
# python3 MisTagParametrization_3D.py -e 2022_F -d 0.8 
# python3 MisTagParametrization_3D.py -e 2022_F -d 0.9 
# python3 MisTagParametrization_3D.py -e 2022_G -d 0.7 
# python3 MisTagParametrization_3D.py -e 2022_G -d 0.8 
# python3 MisTagParametrization_3D.py -e 2022_G -d 0.9 

echo 
echo "Running over 2023 data:"
echo "Scanning depth DNN score, with b-tagged categories combined"
python3 MisTagParametrization_3D.py -e 2023 -d 0.9 -b 
python3 MisTagParametrization_3D.py -e 2023 -d 0.93 -b
python3 MisTagParametrization_3D.py -e 2023 -d 0.95 -b 
python3 MisTagParametrization_3D.py -e 2023 -d 0.97 -b 
# python3 MisTagParametrization_3D.py -e 2023_Cv1 -d 0.9 -b
# python3 MisTagParametrization_3D.py -e 2023_Cv2 -d 0.9 -b
# python3 MisTagParametrization_3D.py -e 2023_Cv3 -d 0.9 -b
# python3 MisTagParametrization_3D.py -e 2023_Cv4 -d 0.9 -b
# python3 MisTagParametrization_3D.py -e 2023_Dv1 -d 0.9 -b
# python3 MisTagParametrization_3D.py -e 2023_Dv2 -d 0.9 -b
# python3 MisTagParametrization_3D.py -e 2023_C1234 -d 0.85 -b 
# python3 MisTagParametrization_3D.py -e 2023_C1234 -d 0.9 -b 
# python3 MisTagParametrization_3D.py -e 2023_C1234 -d 0.95 -b
# python3 MisTagParametrization_3D.py -e 2023_D -d 0.85 -b 
# python3 MisTagParametrization_3D.py -e 2023_D -d 0.9 -b 
# python3 MisTagParametrization_3D.py -e 2023_D -d 0.95 -b
echo "Scanning depth DNN score, with b-tagged categories separated"
# python3 MisTagParametrization_3D.py -e 2023 -d 0.7 
# python3 MisTagParametrization_3D.py -e 2023 -d 0.8 
# python3 MisTagParametrization_3D.py -e 2023 -d 0.9 
# python3 MisTagParametrization_3D.py -e 2023_C123 -d 0.7 
# python3 MisTagParametrization_3D.py -e 2023_C123 -d 0.8 
# python3 MisTagParametrization_3D.py -e 2023_C123 -d 0.9 
# python3 MisTagParametrization_3D.py -e 2023_C4D -d 0.7 
# python3 MisTagParametrization_3D.py -e 2023_C4D -d 0.8 
# python3 MisTagParametrization_3D.py -e 2023_C4D -d 0.9 
