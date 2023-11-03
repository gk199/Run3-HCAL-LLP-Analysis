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

	float phiVectorSum_ele = -99;
	float phiVectorSum_muon = -99;

	int n_ele_over20 = 0;
	int n_muon_over20 = 0;

	// Lepton must be over 35 GeV, eta < 2.4, tight ID, isolated, and select the first (highest pT) lepton passing these requriements. Count number of leptons over 20 GeV
	for (int i = 0; i < n_ele; i++) {
		if (! (ele_passCutBasedIDTight->at(i) == 1) ) continue; 								// tight electron ID
		if ( ! IsElectronIsolatedTight(i) ) continue;											// electron isolation requirement
		std::cout << "electron passed ID and isolation requirements" << std::endl;
		float transverseM_ele = sqrt( 2 * ele_Pt->at(i) * met_Pt * (1 - cos( ele_Phi->at(i) - met_Phi ) ) ); 
		if (ele_Pt->at(i) > 20) n_ele_over20 += 1;
		if (ele_Pt->at(i) > 35 && abs(ele_Eta->at(i)) < 2.4 && transverseM_ele > 55 && electron == false) {
			electron = true;

			float x_vector = ele_Pt->at(i) * cos(ele_Phi->at(i)) + met_Pt * cos(met_Phi);
			float y_vector = ele_Pt->at(i) * sin(ele_Phi->at(i)) + met_Pt * sin(met_Phi);
			phiVectorSum_ele = atan2(y_vector, x_vector);
		}
	}

	for (int i = 0; i < n_muon; i++) {
		if (! (muon_IsTight->at(i) == 1 && fabs(muon_ip3dSignificance->at(i)) < 4) ) continue; 		// tight muon ID
		if ( ! IsMuonIsolatedTight(i) ) continue; 													// muon isolation requirement 
		std::cout << "muon passed ID and isolation requirements" << std::endl;
		float transverseM_muon = sqrt( 2 * muon_Pt->at(i) * met_Pt * (1 - cos( muon_Phi->at(i) - met_Phi ) ) ); 
		if (muon_Pt->at(i) > 20) n_muon_over20 += 1;
		if (muon_Pt->at(i) > 35 && abs(muon_Eta->at(i)) < 2.4 && transverseM_muon > 55 && muon == false) {
			muon = true;

			float x_vector = muon_Pt->at(i) * cos(muon_Phi->at(i)) + met_Pt * cos(met_Phi);
			float y_vector = muon_Pt->at(i) * sin(muon_Phi->at(i)) + met_Pt * sin(met_Phi);
			phiVectorSum_muon = atan2(y_vector, x_vector);
		}
	}

	if (electron) std::cout << "electron passed all requirements, with n_ele_over20 = " << n_ele_over20 << std::endl;
	if (muon) std::cout << "muon passed all requirements, with n_muon_over20 = " << n_muon_over20 << std::endl;

	if ( electron && muon ) return false;									// reject events with 2 leptons passing
	if ( !electron && !muon ) return false;									// reject events with no leptons passing
	if ( (n_ele_over20 + n_muon_over20) > 1 ) return false;					// reject events with additional leptons over 20 GeV

	// only check jets once we know a lepton has been found
	double jet_lepton_dPhi_ele = -999; 
	double jet_lepton_dPhi_muon = -999;
	bool matched_jet = false;
	for (int i = 0; i < n_jet; i++) { 			
		if (jet_Pt->at(i) > 30 && abs(jet_Eta->at(i)) < 2.4 ) {
			if (electron) 	jet_lepton_dPhi_ele = deltaPhi(jet_Phi->at(i), phiVectorSum_ele);
			if (muon) 		jet_lepton_dPhi_muon = deltaPhi(jet_Phi->at(i), phiVectorSum_muon);
			std::cout << "jet " << i << " with dPhi = " << jet_lepton_dPhi_ele << ", " << jet_lepton_dPhi_muon << std::endl;
			if ( (electron && abs(jet_lepton_dPhi_ele) > 2) || (muon && abs(jet_lepton_dPhi_muon) > 2) ) matched_jet = true;
			if (matched_jet) std::cout << "found a jet back to back with lepton!" << std::endl;
		}
	}

	if ( matched_jet ) return true;
	else return false;
}
