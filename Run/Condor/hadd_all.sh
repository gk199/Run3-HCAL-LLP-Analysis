#!/bin/bash

# find-replace v3.5 -> vx
# find-replace 2024_02_21 -> today's date

echo "batch 1 and batch 2 of high LLP mass MC"
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_02_21_batch1.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_MC_LLP350_160_batch1_2024_02_21/*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_02_21_batch2.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_MC_LLP350_160_batch2_2024_02_21/*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_02_21_batch1.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_MC_LLP250_120_batch1_2024_02_21/*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_02_21_batch2.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_MC_LLP250_120_batch2_2024_02_21/*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau10000_13p6TeV_2024_02_21_batch1.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_MC_LLP125_50_batch1_2024_02_21/*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau10000_13p6TeV_2024_02_21_batch2.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_MC_LLP125_50_batch2_2024_02_21/*.root

echo "test and train of low LLP mass MC"
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_02_21_TEST.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_MC_LLP125_15_TEST_2024_02_21/*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_02_21_TRAIN.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_MC_LLP125_15_TRAIN_2024_02_21/*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_02_21_TEST.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_MC_LLP350_80_TEST_2024_02_21/*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_02_21_TRAIN.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_MC_LLP350_80_TRAIN_2024_02_21/*.root

echo "2023 displaced jet skim"
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLPskim_Run2023Bv1_2024_02_21.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_LLPskim_2023Bv1_2024_02_21/*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLPskim_Run2023Cv1_2024_02_21.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_LLPskim_2023Cv1_2024_02_21/*.root
# hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLPskim_Run2023Cv2_2024_02_21.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_LLPskim_2023Cv2_2024_02_21/*.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLPskim_Run2023Cv3_2024_02_21.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_LLPskim_2023Cv3_2024_02_21/*.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLPskim_Run2023Cv4_2024_02_21.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_LLPskim_2023Cv4_2024_02_21/*.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLPskim_Run2023Dv1_2024_02_21.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_LLPskim_2023Dv1_2024_02_21/*.root
hadd -j /eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_v3.5_LLPskim_Run2023Dv2_2024_02_21.root /eos/user/g/gkopp/LLP_Analysis/output_minituples_v3.5_LLPskim_2023Dv2_2024_02_21/*.root