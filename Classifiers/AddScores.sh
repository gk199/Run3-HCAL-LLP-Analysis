#!/bin/bash

echo "Adding scores to event-based files..."
echo "2022 RAW"
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2022Dv1.root -t NoSel -d Evaluate/depth_model_v6.keras -i Evaluate/inclusive_model_v6.keras -c Evaluate/norm_constants_v4.csv -m filewrite
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2022Ev1.root -t NoSel -d Evaluate/depth_model_v6.keras -i Evaluate/inclusive_model_v6.keras -c Evaluate/norm_constants_v4.csv -m filewrite
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2022Fv1.root -t NoSel -d Evaluate/depth_model_v6.keras -i Evaluate/inclusive_model_v6.keras -c Evaluate/norm_constants_v4.csv -m filewrite
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2022Gv1.root -t NoSel -d Evaluate/depth_model_v6.keras -i Evaluate/inclusive_model_v6.keras -c Evaluate/norm_constants_v4.csv -m filewrite
echo "2023 RAW"
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2023Cv1.root -t NoSel -d Evaluate/depth_model_v6.keras -i Evaluate/inclusive_model_v6.keras -c Evaluate/norm_constants_v4.csv -m filewrite
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2023Cv2.root -t NoSel -d Evaluate/depth_model_v6.keras -i Evaluate/inclusive_model_v6.keras -c Evaluate/norm_constants_v4.csv -m filewrite
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2023Cv3.root -t NoSel -d Evaluate/depth_model_v6.keras -i Evaluate/inclusive_model_v6.keras -c Evaluate/norm_constants_v4.csv -m filewrite
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2023Cv4.root -t NoSel -d Evaluate/depth_model_v6.keras -i Evaluate/inclusive_model_v6.keras -c Evaluate/norm_constants_v4.csv -m filewrite
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2023Dv1.root -t NoSel -d Evaluate/depth_model_v6.keras -i Evaluate/inclusive_model_v6.keras -c Evaluate/norm_constants_v4.csv -m filewrite
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2023Dv2.root -t NoSel -d Evaluate/depth_model_v6.keras -i Evaluate/inclusive_model_v6.keras -c Evaluate/norm_constants_v4.csv -m filewrite
echo "LLP MC Signal"
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_HToSSTo4B_125_50_CTau3000.root -t NoSel -d Evaluate/depth_model_v6.keras -i Evaluate/inclusive_model_v6.keras -c Evaluate/norm_constants_v4.csv -m filewrite
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_HToSSTo4B_250_120_CTau1000.root -t NoSel -d Evaluate/depth_model_v6.keras -i.Evaluate/inclusive_model_v6.keras -c.Evaluate/norm_constants_v4.csv -m filewrite
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_HToSSTo4B_350_MS160_CTau10000.root -t NoSel -d Evaluate/depth_model_v6.keras -i.Evaluate/inclusive_model_v6.keras -c.Evaluate/norm_constants_v4.csv -m filewrite
python3 Evaluate/ScoresToEventBased_iterate.py -f /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_HToSSTo4B_350_80_CTau500.root -t NoSel -d Evaluate/depth_model_v6.keras -i.Evaluate/inclusive_model_v6.keras -c.Evaluate/norm_constants_v4.csv -m filewrite

#  -d Evaluate/depth_model_v6.keras