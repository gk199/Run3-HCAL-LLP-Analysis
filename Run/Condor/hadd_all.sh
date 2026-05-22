#!/bin/bash

# find-replace v5.5 -> vx

echo "LLP MC"
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_HToSSTo4B_350_160_CTau10000.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/HToSSTo4B_MH350_MS160_CTau10000_L1triggers_v5/minituple_job*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_HToSSTo4B_350_80_CTau500.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/HToSSTo4B_MH350_MS80_CTau500_L1triggers_v5/minituple_job*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_HToSSTo4B_250_120_CTau10000.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/HToSSTo4B_MH250_MS120_CTau10000_L1triggers_v5/minituple_job*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_HToSSTo4B_125_50_CTau3000.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/HToSSTo4B_MH125_MS50_CTau3000_L1triggers_v5/minituple_job*.root

# ================================================================================= #

echo "2022 displaced jet skim"
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2022Dv1.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/DisplacedJet_Run2022D-v1_RAW_v5/minituple_job*.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2022Ev1.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/DisplacedJet_Run2022E-v1_RAW_v5/minituple_job*.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2022Fv1.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/DisplacedJet_Run2022F-v1_RAW_v5/minituple_job*.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_2022Gv1.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/DisplacedJet_Run2022G-v1_RAW_v5/minituple_job*.root

echo "2023 displaced jet skim"
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_Run2023Cv1.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/DisplacedJet_Run2023C-EXOLLPJetHCAL-PromptReco-v1_AOD_v5/minituple_job*.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_Run2023Cv2.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/DisplacedJet_Run2023C-EXOLLPJetHCAL-PromptReco-v2_AOD_v5/minituple_job*.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_Run2023Cv3.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/DisplacedJet_Run2023C-EXOLLPJetHCAL-PromptReco-v3_AOD_v5/minituple_job*.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_Run2023Cv4.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/DisplacedJet_Run2023C-EXOLLPJetHCAL-PromptReco-v4_AOD_v5/minituple_job*.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_Run2023Dv1.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/DisplacedJet_Run2023D-EXOLLPJetHCAL-PromptReco-v1_AOD_v5/minituple_job*.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.5/minituple_data_Run2023Dv2.root /eos/user/g/gkopp/LLP_Analysis/MiniTuples/v5.5/DisplacedJet_Run2023D-EXOLLPJetHCAL-PromptReco-v2_AOD_v5/minituple_job*.root