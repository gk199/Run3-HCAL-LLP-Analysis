#!/bin/bash

# find-replace v3.11 -> vx
# find-replace 2025_02_03 -> today's date

# Zmumu_scores, WPlusJets_scores, NoSel_scores, PassedHLT_scores

echo "batch 1 and batch 2 of high LLP mass MC"
#hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_PassedHLT_scores_2025_02_11_batch1.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP350_160_batch1_2025_02_11/*PassedHLT_scores.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_PassedHLT_scores_2025_02_11_batch2.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP350_160_batch2_2025_02_11/*PassedHLT_scores.root
#hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_PassedHLT_scores_2025_02_11_batch1.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP250_120_batch1_2025_02_11/*PassedHLT_scores.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_PassedHLT_scores_2025_02_11_batch2.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP250_120_batch2_2025_02_11/*PassedHLT_scores.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_PassedHLT_scores_2025_02_11_batch1.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP125_50_batch1_2025_02_11/*PassedHLT_scores.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_PassedHLT_scores_2025_02_11_batch2.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP125_50_batch2_2025_02_11/*PassedHLT_scores.root

echo "test and train of low LLP mass MC"
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_PassedHLT_scores_2025_02_11_TEST.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP125_15_TEST_2025_02_11/*PassedHLT_scores.root
#hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_PassedHLT_scores_2025_02_11_TRAIN.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP125_15_TRAIN_2025_02_11/*PassedHLT_scores.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_PassedHLT_scores_2025_02_11_TEST.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP350_80_TEST_2025_02_11/*PassedHLT_scores.root
#hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_PassedHLT_scores_2025_02_11_TRAIN.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP350_80_TRAIN_2025_02_11/*PassedHLT_scores.root

echo "newest LLP MC mH = 125 samples"
#hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau10000_13p6TeV_PassedHLT_scores_2025_02_11.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP125_50_10m_2025_02_11/*PassedHLT_scores.root
#hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau10000_13p6TeV_PassedHLT_scores_2025_02_11.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP125_15_10m_2025_02_11/*PassedHLT_scores.root
#hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau3000_13p6TeV_PassedHLT_scores_2025_02_11.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_MC_LLP125_15_3m_2025_02_11/*PassedHLT_scores.root

echo "2023 displaced jet skim"
# WPlusJets_scores, Zmumu_scores, NoSel_scores, PassedHLT_scores, and *.root (no DNN scores)
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Bv1_NoSel_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Bv1_2025_02_03/*NoSel_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv1_NoSel_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv1_2025_02_03/*NoSel_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv2_NoSel_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv2_2025_02_03/*NoSel_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv3_NoSel_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv3_2025_02_03/*NoSel_scores.root

# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Bv1_WPlusJets_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Bv1_2025_02_03/*WPlusJets_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv1_WPlusJets_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv1_2025_02_03/*WPlusJets_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv2_WPlusJets_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv2_2025_02_03/*WPlusJets_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv3_WPlusJets_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv3_2025_02_03/*WPlusJets_scores.root

# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Bv1_Zmumu_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Bv1_2025_02_03/*Zmumu_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv1_Zmumu_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv1_2025_02_03/*Zmumu_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv2_Zmumu_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv2_2025_02_03/*Zmumu_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv3_Zmumu_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv3_2025_02_03/*Zmumu_scores.root

# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Bv1_PassedHLT_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Bv1_2025_02_03/*PassedHLT_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv1_PassedHLT_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv1_2025_02_03/*PassedHLT_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv2_PassedHLT_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv2_2025_02_03/*PassedHLT_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv3_PassedHLT_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv3_2025_02_03/*PassedHLT_scores.root

#hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Cv4_NoSel_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Cv4_2025_02_03/*NoSel_scores.root
#hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Dv1_NoSel_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Dv1_2025_02_03/*NoSel_scores.root
#hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023Dv2_NoSel_scores_2025_02_03.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_LLPskim_2023Dv2_2025_02_03/*NoSel_scores.root

echo "hadd all LLP skim"
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023_HADD.root /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_LLPskim_Run2023*.root

echo "hadd Zmu 2023 skim"
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Bv1_WPlusJets_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Bv1_2025_02_10/*WPlusJets_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Cv1_WPlusJets_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Cv1_2025_02_10/*WPlusJets_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Cv2_WPlusJets_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Cv2_2025_02_10/*WPlusJets_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Cv3_WPlusJets_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Cv3_2025_02_10/*WPlusJets_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Cv4_WPlusJets_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Cv4_2025_02_10/*WPlusJets_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Dv1_WPlusJets_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Dv1_2025_02_10/*WPlusJets_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Dv2_WPlusJets_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Dv2_2025_02_10/*WPlusJets_scores.root

# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Bv1_Zmumu_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Bv1_2025_02_10/*Zmumu_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Cv1_Zmumu_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Cv1_2025_02_10/*Zmumu_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Cv2_Zmumu_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Cv2_2025_02_10/*Zmumu_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Cv3_Zmumu_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Cv3_2025_02_10/*Zmumu_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Cv4_Zmumu_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Cv4_2025_02_10/*Zmumu_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Dv1_Zmumu_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Dv1_2025_02_10/*Zmumu_scores.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023Dv2_Zmumu_scores_2025_02_10.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.11_Zmu_2023Dv2_2025_02_10/*Zmumu_scores.root

# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023_HADD_Zmumu_scores_2025_02_10.root /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023*_Zmumu_scores_2025_02_10.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023_HADD_WPlusJets_scores_2025_02_10.root /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.11/minituple_v3.11_Zmu_Run2023*_WPlusJets_scores_2025_02_10.root