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
float DisplacedHcalJetAnalyzer::EventHT(){

	if ( debug ) cout<<"DisplacedHcalJetAnalyzer::EventHT()"<<endl;

	float HT = 0;
	for (int i = 0; i < n_jet; i++) { 
		HT += jet_Pt->at(i);
	}
	return HT;
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

	// Lepton must be over 20 GeV, eta < 2.4, tight ID, isolated, and select the first (highest pT) lepton passing these requriements. Count number of leptons over 20 GeV
	for (int i = 0; i < n_ele; i++) {
		if (! (ele_passCutBasedIDTight->at(i) == 1) ) continue; 								// tight electron ID
		if ( ! IsElectronIsolatedTight(i) ) continue;											// electron isolation requirement

		float transverseM_ele = TransverseLeptonMass(ele_Pt->at(i), ele_Phi->at(i));
		if (ele_Pt->at(i) > 20) n_ele_over20 += 1;
		if (ele_Pt->at(i) > 20 && abs(ele_Eta->at(i)) < 2.4 && transverseM_ele > 55 && electron == false) {
			electron = true;
			phiVectorSum_ele = PhiVectorSum(ele_Pt->at(i), ele_Phi->at(i));
		}
	}

	for (int i = 0; i < n_muon; i++) {
		if (! (muon_IsTight->at(i) == 1 )) continue; // && fabs(muon_ip3dSignificance->at(i)) < 4) ) continue; 		// tight muon ID
		// in files, IP 3D variables (as well as 2D PV and BS) are not defined. However, tight muon ID already requires dB < 0.2 cm: https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonIdRun2#Tight_Muon
		if ( ! IsMuonIsolatedTight(i) ) continue; 																	// muon isolation requirement 

		float transverseM_muon = TransverseLeptonMass(muon_Pt->at(i), muon_Phi->at(i));
		if (muon_Pt->at(i) > 20) n_muon_over20 += 1;
		if (muon_Pt->at(i) > 20 && abs(muon_Eta->at(i)) < 2.4 && transverseM_muon > 55 && muon == false) {
			muon = true;
			phiVectorSum_muon = PhiVectorSum(muon_Pt->at(i), muon_Phi->at(i));
		}
	}

	if (met_SumEt < 30) return false;										// should this be met pT or eT?
	if ( electron && muon ) return false;									// reject events with 2 leptons passing
	if ( !electron && !muon ) return false;									// reject events with no leptons passing
	if ( (n_ele_over20 + n_muon_over20) > 1 ) return false;					// reject events with additional leptons over 20 GeV

	// only check jets once we know a lepton has been found
	double jet_lepton_dPhi_ele = -999; 
	double jet_lepton_dPhi_muon = -999;
	bool matched_jet = false;
	for (int i = 0; i < n_jet; i++) { 			
		if (jet_Pt->at(i) > 30 && abs(jet_Eta->at(i)) < 1.26 ) {
			if (electron) {
				jet_lepton_dPhi_ele = deltaPhi(jet_Phi->at(i), phiVectorSum_ele);
				WPlusJets_leptonPhi = phiVectorSum_ele;
			}
			if (muon) {
				jet_lepton_dPhi_muon = deltaPhi(jet_Phi->at(i), phiVectorSum_muon);
				WPlusJets_leptonPhi = phiVectorSum_muon;
			}

			if ( (electron && abs(jet_lepton_dPhi_ele) > 2) || (muon && abs(jet_lepton_dPhi_muon) > 2) ) matched_jet = true;
		}
	}

	if ( matched_jet ) return true;
	else return false;
}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::PassLeptonVeto() {
	// given a event, veto on leptons, in high MET sample this will select for Z to neutrinos (20%) or QCD

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::PassLeptonVeto()"<<endl;

	bool electron = false;
	bool muon = false;

	int n_ele_over20 = 0;
	int n_muon_over20 = 0;

	// Lepton must be over 20 GeV, eta < 2.4, tight ID, isolated, and select the first (highest pT) lepton passing these requriements. Count number of leptons over 20 GeV
	for (int i = 0; i < n_ele; i++) {
		if (! (ele_passCutBasedIDTight->at(i) == 1) ) continue; 								// tight electron ID
		if ( ! IsElectronIsolatedTight(i) ) continue;											// electron isolation requirement

		float transverseM_ele = TransverseLeptonMass(ele_Pt->at(i), ele_Phi->at(i));
		if (ele_Pt->at(i) > 20) n_ele_over20 += 1;
		if (ele_Pt->at(i) > 20 && abs(ele_Eta->at(i)) < 2.4 && transverseM_ele > 55 && electron == false) electron = true;
	}

	for (int i = 0; i < n_muon; i++) {
		if (! (muon_IsTight->at(i) == 1 )) continue; 											// tight muon ID
		if ( ! IsMuonIsolatedTight(i) ) continue; 												// muon isolation requirement 

		float transverseM_muon = TransverseLeptonMass(muon_Pt->at(i), muon_Phi->at(i));
		if (muon_Pt->at(i) > 20) n_muon_over20 += 1;
		if (muon_Pt->at(i) > 20 && abs(muon_Eta->at(i)) < 2.4 && transverseM_muon > 55 && muon == false) muon = true;
	}

	if (met_SumEt < 30) return false;										// should this be met pT or eT?
	// if ( electron || muon ) return false;								// require neither electron or muon is found
	if ( (n_ele_over20 + n_muon_over20) > 0 ) return false;					// reject events with any leptons over 20 GeV

	// only check jets once we know there are no leptons
	bool matched_jet = false;
	for (int i = 0; i < n_jet; i++) { 			
		if (jet_Pt->at(i) > 30 && abs(jet_Eta->at(i)) < 1.26 ) {
			matched_jet = true;
		}
	}

	if ( matched_jet ) return true;
	else return false;
}