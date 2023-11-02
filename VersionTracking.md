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

### v2.1