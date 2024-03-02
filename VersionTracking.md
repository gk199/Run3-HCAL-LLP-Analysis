# MiniTuples by version:

## V1
Updates: 
* Tracker information added
* LLP truth matching for efficiency studies
* Use unpacked TDC
* Variable names match ntupler

### v1.0
Location of minituples:
```
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.0/minituple_v1_MCsignal_500k_2023_07_14.root
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.0/minituple_v1_LLPskim_500k_2023_07_14.root
```

### v1.1
Updates:
* Fix jet-LLP matching (previously saved value even if jet not matched to LLP)
* Add L1 jet quantities (n, Pt, Eta, Phi, E, hwQual) for highest 3 pT L1 jets
* Add HLT results (for 12 triggers, will fix once ntupler has all 15 saved)
* Add LLP kinematics (pT, E, travel time, beta)
* Add event kinematics (MET pT, sum ET, phi)

Location of minituples:
```
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.1/minituple_v1.1_MCsignal_500k_2023_08_23.root
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.1/minituple_v1.1_LLPskim_500k_2023_08_23.root
```

### v1.2
Updates:
* Clear variables in minituples tree
* Truth matching for a jet done to LLP direction (decay in HCAL) or b quark (decay before HCAL)
* LLP truth matching to a jet considers two options: match to LLP (decay in HCAL) or b quark (decay before HCAL), with option to cut on jet energy (jet 0, 40, 70, 100 GeV now)
* Neutral and charged hadron, electron, photon, muon energy fraction (`NeutralHadEFrac, ChargedHadEFrac, EleEFrac, PhoEFrac, MuonEFrac`)
* H over E computed (`HoverE` = (charged + neutral hadron energy) / (photon + electron energy) )
* Eta-Phi quadrature sum, both energy weighted and standard

Location of minituples:
```
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.2/minituple_v1.2_MCsignal_500k_2023_09_18.root
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.2/minituple_v1.2_LLPskim_500k_2023_09_11.root (note, missing all EFrac quantities)
```

### v1.3
Updates:
* Fix H/E (`HoverE` = (charged hadron energy) / (neutral hadron energy + photon + electron energy) ) 
* Add additional jet variables (mass, object multiplicities, tracks, SV info, flight distance) -- note, PtAllTracks, PtAllPVTracks, NVertexTracks, NSelectedTracks is 0 now! 
* Merged jet truth matching functions: now only have JetIsMatchedTo (giving LLP number and dR), simply check if `JetIsMatchedTo[0] > -1` to know if `JetIsTruthMatched` (removed). Previously was undermatching to LLPs by only looking at decay products. 
* Added LLP eta cuts of 1.26 to match more accurately with the HB geometry
* LLP truth matching to a jet with jet energy of 0, 40, 60, 80, 100 GeV. Savev result of truth matching (true / false) and matched jet eta. 
* LLP decay R, eta, of the LLP matched to leading / sub / ssub jet
* dR, dEta, dPhi between jet and associated tracks. dR between two leading tracks
* Save energy and depth of top three HCAL rechits associated to a jet, and total energy of rechits (with `Get3RechitE_Jet`)

Location of minituples:
```
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.3/minituple_v1.3_MCsignal_500k_2023_10_16.root
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.3/minituple_v1.3_LLPskim_500k_2023_09_28.root
```

All v1.* minituples are also copied on to the LPC space in various sub-directories in `/eos/uscms/store/group/lpclonglived/gkopp/HCAL_LLP_Analysis/MiniTuples/v*`.

## V2
Updates:
* `FillTriggerBranches` ntuples function is now working, so HLT names are filled properly (temporary fix removed)
* Restrict HB rechit use to those above the PF cut thresholds, which are depth dependent. All `GetMatchedHcalRechits_*` functions now use `isRechitValid` to check if rechit should be included in computations
* Save dR, dEta, dPhi between LLP and decay products, and between the two decay products

### v2.0
Location of minituples:
```
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v2.0/minituple_v2.0_MCsignalLLP_500k_2023_10_18.root
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v2.0/minituple_v2.0_LLPskimRun2023C_500k_2023_10_18.root
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v2.0/minituple_v2.0_MC_QCD_250k_2023_10_18.root
```
Directory in personal eos space was accidentally removed.

Also copied to LPC space:
```
/eos/uscms/store/group/lpclonglived/gkopp/HCAL_LLP_Analysis/MiniTuples/v2.0/minituple_v2.0_LLPskimRun2023C_500k_2023_10_18.root
/eos/uscms/store/group/lpclonglived/gkopp/HCAL_LLP_Analysis/MiniTuples/v2.0/minituple_v2.0_MC_QCD_250k_2023_10_18.root
/eos/uscms/store/group/lpclonglived/gkopp/HCAL_LLP_Analysis/MiniTuples/v2.0/minituple_v2.0_MCsignalLLP_500k_2023_10_18.root
```
And CERN EOS phy_exotica space:
```
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v2.0/minituple_v2.0_LLPskimRun2023C_500k_2023_10_18.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v2.0/minituple_v2.0_MC_QCD_250k_2023_10_18.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v2.0/minituple_v2.0_MCsignalLLP_500k_2023_10_18.root
```

## V3
Updates:
* New tree filled with W+jets flagged events
* W+jets selection implemented in EventHelper, with tight lepton isolation selection, in ObjectHelper
* Compute event HT in the minituples
* S major, S minor jet spread variables calculated

### v3.0
```
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2023_11_23.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_v3.0_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2023_11_29.root

/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_v3.0_LLPskim_Run2023Bv1_2023Cv2_2023_11_23.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_v3.0_LLPskim_Run2023*_2023_11_22.root                                          # Cv4 is a very large sample 40M total events
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_v3.0_LLPskim_Run2023Bv1_Cv1_Cv2_Cv3_Cv4_Dv1_Dv2_2023_11_17.root                # does not yet have Sphiphi fix yet! 
```
These are hadd-ed from 
```
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v3.0/* 
```
with `* = minituple_v3.0_LLPskimRun2023Bv1_2023_11_09.root, minituple_v3.0_LLPskimRun2023Cv1_2023_11_14.root, minituple_v3.0_LLPskimRun2023Cv3_2023_11_15.root, minituple_v3.0_LLPskimRun2023Cv4_2023_11_16.root, minituple_v3.0_LLPskimRun2023Dv1_2023_11_16.root, minituple_v3.0_LLPskimRun2023Dv2_2023_11_16.root`.

Note that v3 minituples for LLP MC (125GeV) are currently run over v2 ntuples (which don't have the HLT monitoring paths or isolation variables - only needed for W+Jets). The W+Jets tree must be commented out for these. The fix for Sphiphi has been added to these minituples as well. 

### v3.1
Updates:
* HLT monitoring paths on all samples
* Fix for default values of S phi, S eta, and HCAL depth energy fractions (avoid NaN, different default value vs no energy)
* BDT score added to minituples, based off of BDT weight files in the directory `Run3-HCAL-LLP-Analysis/BDTWeightFiles/v*`
* Saves BDT score for both 125 and 350 trainings, against 2023Cv4 W+jets, and on signal Train specific files

Location of minituples:
```
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_v3.1_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_01_20_TRAIN.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_v3.1_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_01_20_TRAIN.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_v3.1_LLP_MC_ggH_HToSSTobbbb_MH-125_350_HADD_13p6TeV_2024_01_20_TRAIN.root

/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.1/minituple_v3.1_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_01_20_TEST.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.1/minituple_v3.1_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_01_20_TEST.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.1/minituple_v3.1_LLPskim_Run2023Cv3_2024_01_20.root
```

### v3.2
Updates:
* Track information: missing inner and outer hits, n pixel hits, n hits
* Test on 125, 350, and hadd-ed version of both samples

Location of minituples:
```
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.2/minituple_v3.2_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_01_23_TEST.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.2/minituple_v3.2_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_01_23_TEST.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.2/minituple_v3.2_LLPskim_Run2023Cv3_2024_01_23.root
```

### v3.3
Updates:
* Add a tree filled on a per jet basis, for jet information. With this structure, a BDT can be trained / tested on each jet
* Added more jet-track association variables to BDT, with the delta eta and phi between jet center and leading tracks
* Jet - track matching changed to be within dR of 0.4 at the minituple level
* Added this new delta eta, phi jet - track information to the per jet and per event BDTs
* Train on per event BDT (with leading jet) and per jet BDT (any jet above 40GeV and within 1.26 eta, and either matching to LLP or passing W+jets event selection, depending on sample)

Location of minituples:
```
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_02_05_TRAIN.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_02_05_TRAIN.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLP_MC_ggH_HToSSTobbbb_MH-125_350_HADD_13p6TeV_2024_02_05_TRAIN.root

/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLPskim_Run2023Bv1_2024_02_07_TEST.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLPskim_Run2023Cv1_2024_02_07_TEST.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLPskim_Run2023Cv2_2024_02_07_TEST.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLPskim_Run2023Cv3_2024_02_07_TEST.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLPskim_Run2023Cv4_2024_02_07_TEST.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLPskim_Run2023Dv1_2024_02_05_TRAIN.root
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLPskim_Run2023Dv2_2024_02_07_TEST.root
```

### v3.4
Updates:
* W+jets trees requires saved jet is dPhi > 2 from lepton
* Electron energy fraction debugged
* Non-matched LLP decay R added to perJet trees
* Added HCAL MAHI time()

For the LLP skim files, this is the number of files processed for each era: 61 (complete), 922 (complete), 1304 (complete), 1480 (near complete), 1830 (near complete), 617 (near complete), 83 (complete). Bv1 needs a longer job flavor. 

Expected numbers of files for each are: 61, 922, 1304, 1484, 1855, 622, and 83 (listed in `Run/Condor/submit_MC/sh`).

Location of minituples:
```
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.4
```

### v3.5
Updates:
* BDTs: all variables (jet eta, charged hadron fraction, neutral hadron fraction, photon fraction, electron fraction, leading and subleading track fractional pT, track dEta, track dPhi, energy fraction in each HCAL depth, Sphiphi, fractional energy in leading rechit) and HCAL variables (jet eta, energy fraction in each HCAL depth, Sphiphi, fractional energy in leading rechit), updated start of HCAL cut to be 177cm, v0.5
* Default value for track pT is 0 GeV (such that jets with no matched tracks will report a track pT of 0)
* Added dxy, dz, and significance to minituples (next iteration, can cut on these in TrackMatcher)
* Start LLP-jet matching at 177cm instead of 183.6cm (start of HCAL)
Last three are in the minituples, but the BDT (v0.5) was trained on v3.4 minituples without those edits, but with the HCAL start set at 177 in BDT-level cuts (iterative process).

Location of minituples:
```
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/
```

### v3.6
Updates:
* Save n_jet, n_ele, n_muon, n_pho
* Default value for dR, dEta, dPhi (jet, track) is 0.5 to indicate how many jets do not have matched tracks
* BDT (v0.6) trained on v3.5 minituples

Location of minituples:
```
/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.6/
```

## V4
Ongoing wish list:
* MET filters
* Trigger prescales
* Some duplication in coding between event and jet filled trees, can this be reduced or simplified?