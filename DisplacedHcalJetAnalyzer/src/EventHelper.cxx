/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::GetEventRuntime( clock_t clock_start, Long64_t init_entry, Long64_t current_entry ){
	//if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetEventRuntime()"<<endl;
	double duration_sec = (clock()-clock_start)/(double)CLOCKS_PER_SEC;
	float events_per_sec = (current_entry - init_entry)/duration_sec; 
	return events_per_sec;
}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::ResetGlobalEventVars(){

	if ( debug ) cout<<"DisplacedHcalJetAnalyzer::ResetGlobalEventVars()"<<endl;

	gLLPDecay_iLLP.clear();
	gLLPDecay_iParticle.clear();
	map_gLLP_to_gParticle_indices.clear();

}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::PassWPlusJetsSelection() {
	// given a event, determine if it is a W + jets event, based off of lepton and jet selections

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::PassWPlusJetsSelection()"<<endl;

	bool electron = false;
	bool muon = false;

	int sel_ele = -1;
	int sel_muon = -1;

	int n_ele_over20 = 0;
	int n_muon_over20 = 0;

	// Check if these leptons are isolated! Need to add isolation var to ntuples.
	// Lepton must be over 35 GeV, eta < 2.4, tight ID, and select the first (highest pT) lepton passing these requriements. Count number of leptons over 20 GeV
	for (int i = 0; i < n_ele; i++) {
		if (ele_Pt->at(i) > 20) n_ele_over20 += 1;
		if (ele_Pt->at(i) > 35 && abs(ele_Eta->at(i)) < 2.4 && ele_passCutBasedIDTight->at(i) == 1 && electron == false) {
			electron = true;
			sel_ele = i;
		}
	}

	for (int i = 0; i < n_muon; i++) {
		if (muon_Pt->at(i) > 20) n_muon_over20 += 1;
		if (muon_Pt->at(i) > 35 && abs(muon_Eta->at(i)) < 2.4 && muon_IsTight->at(i) == 1 && muon == false) {
			muon = true;
			sel_muon = i;
		}
	}

	if ( electron && muon ) return false;					// reject events with 2 leptons passing
	if ( !electron && !muon ) return false;					// reject events with no leptons passing
	if ( (n_ele_over20 + n_muon_over20) > 1 ) return false;	// reject events with additional leptons over 20 GeV

	double jet_lepton_dPhi_ele = -999; 
	double jet_lepton_dPhi_muon = -999;
	bool matched_jet = false;
	for (int i = 0; i < n_jet; i++) { 			// only check jets once we know a lepton has been found
		if (jet_Pt->at(i) > 30 && abs(jet_Eta->at(i)) < 2.4 ) {
			if (electron) 	jet_lepton_dPhi_ele = deltaPhi(jet_Phi->at(i), ele_Phi->at(sel_ele));
			if (muon) 		jet_lepton_dPhi_muon = deltaPhi(jet_Phi->at(i), muon_Phi->at(sel_muon));
			std::cout << "jet " << i << " with dPhi = " << jet_lepton_dPhi_ele << ", " << jet_lepton_dPhi_muon << std::endl;
			if ( (electron && abs(jet_lepton_dPhi_ele) > 2) || (muon && abs(jet_lepton_dPhi_muon) > 2) ) matched_jet = true;
			if (matched_jet) std::cout << "found a jet back to back with lepton!" << std::endl;
		}
	}

	if ( matched_jet ) return true;
	else return false;
}