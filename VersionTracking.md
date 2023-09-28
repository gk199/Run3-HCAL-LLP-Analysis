# MiniTuples by version:

## V1
Updates: 
* Tracker information added
* LLP truth matching for efficiency studies
* Use unpacked TDC
* Variable names match ntupler

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
* Fix H/E (`HoverE` = (charged hadron energy) / (neutral hadron energy + photon + electron energy) ) -- note, ele is 0 now! 
* Add additional jet variables (mass, object multiplicities, tracks, SV info, flight distance) -- note, PtAllTracks, PtAllPVTracks, NVertexTracks, NSelectedTracks is 0 now! 
* Merged jet truth matching functions: now only have JetIsMatchedTo (giving LLP number and dR), simply check if `JetIsMatchedTo[0] > -1` to know if `JetIsTruthMatched` (removed)
* LLP truth matching to a jet with jet energy of 0, 40, 60, 80, 100 GeV
* LLP decay R of the LLP matched to leading / sub / ssub jet
* dR, dEta, dPhi between jet and associated tracks. dR between two leading tracks
* Save energy and depth of top three HCAL rechits associated to a jet, and total energy of rechits (with `Get3RechitE_Jet`)

Location of minituples:
```
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.2/minituple_v1.2_MCsignal_500k_2023_09_22.root
/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.2/minituple_v1.2_LLPskim_500k_2023_09_22.root
```

## V2

