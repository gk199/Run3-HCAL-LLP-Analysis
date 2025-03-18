#!/bin/bash

# find-replace v3.11 -> vx
# find-replace 2025_02_10 -> today's date

echo "Zmu 2023 skim"
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Bv1_Zmumu_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Bv1_2025_02_10/*Zmumu_scores.root

hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Cv1_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Cv1_2025_02_10/*.root

hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Cv2_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Cv2_2025_02_10/*.root

hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Cv3_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Cv3_2025_02_10/*.root

hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Cv4_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Cv4_2025_02_10/*.root

hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Dv1_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Dv1_2025_02_10/*.root

hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Dv2_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Dv2_2025_02_10/*.root
