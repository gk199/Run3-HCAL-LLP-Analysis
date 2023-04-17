// Put object-related functions here (e.g. selection functions, etc)

/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::DeltaR( float eta1, float eta2, float phi1, float phi2){

	if( debug ) cout<<"DisplacedHggAnalysis::DeltaR()"<<endl;

 	float deta = fabs(eta2 - eta1);
 	float dphi = fabs(phi2 - phi1);
 	if( dphi > 3.14159 ) dphi -= 2*3.14159; // PBC

 	float dR = sqrt( pow(deta, 2.) + pow(dphi, 2.) );
 	return dR;
}

/* ====================================================================================================================== */
double DisplacedHcalJetAnalyzer::deltaPhi(double phi1, double phi2) {  // calculate delta phi given two phi values
  double result = phi1 - phi2;
  if(fabs(result) > 9999) return result;
  while (result > TMath::Pi()) result -= 2*TMath::Pi();
  while (result <= -TMath::Pi()) result += 2*TMath::Pi();
  return result;
}

double DisplacedHcalJetAnalyzer::deltaR(double eta1, double phi1, double eta2, double phi2) { // calculate deltaR given two eta and phi values
  double deta = eta1 - eta2;
  double dphi = deltaPhi(phi1, phi2);
  return sqrt(deta*deta + dphi*dphi);
}

/* ====================================================================================================================== */
vector<int> DisplacedHcalJetAnalyzer::GetRechitMult(int idx_llp, float deltaR_cut) { // given a LLP, find how many associated HB rechits there are (rechits for LLP total, first daughter, second daughter)

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetRechitMult()"<<endl;
	
	vector<float> matchedRechit[2];
	for (int idx_llp_decay = 0; idx_llp_decay < 2; idx_llp_decay++) {
		matchedRechit[idx_llp_decay] = GetMatchedHcalRechits_LLPDecay(idx_llp, idx_llp_decay, deltaR_cut);
	}
	vector<float> v_intersection;
	std::set_intersection( matchedRechit[0].begin(), matchedRechit[0].end(), matchedRechit[1].begin(), matchedRechit[1].end(), std::back_inserter(v_intersection));

	int rechitMult_LLP	 	= matchedRechit[0].size() + matchedRechit[1].size() - v_intersection.size();
	int rechitMult_decay1 	= matchedRechit[0].size();
	int rechitMult_decay2	= matchedRechit[1].size();

	vector<int> rechits_forLLP = {rechitMult_LLP, rechitMult_decay1, rechitMult_decay2};
	return rechits_forLLP;
}

/* ====================================================================================================================== */
vector<vector<float>> DisplacedHcalJetAnalyzer::GetEnergyProfile(int idx_llp, float deltaR_cut) { // given a LLP, find the normalized energy profile from associated HB rechits 

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetEnergyProfile()"<<endl;

	// vectors to fill with energy in each depth
	vector<float> energy_LLP 		= {0,0,0,0};
	vector<float> energy_daughter1 	= {0,0,0,0};
	vector<float> energy_daughter2 	= {0,0,0,0};

	vector<float> matchedRechit[2];
	for (int idx_llp_decay = 0; idx_llp_decay < 2; idx_llp_decay++) {
		matchedRechit[idx_llp_decay] = GetMatchedHcalRechits_LLPDecay(idx_llp, idx_llp_decay, deltaR_cut);
	}
	vector<float> v_intersection;
	std::set_intersection( matchedRechit[0].begin(), matchedRechit[0].end(), matchedRechit[1].begin(), matchedRechit[1].end(), std::back_inserter(v_intersection));
	
	vector<int> matched_indices; // prevent double counting rechits in energy profile for LLP 
	for (int idx_llp_decay = 0; idx_llp_decay < 2; idx_llp_decay++) {
		for (int i = 0; i < matchedRechit[idx_llp_decay].size(); i++) {
			int hbhe_matched_indices = matchedRechit[idx_llp_decay][i];
			if (idx_llp_decay == 0) {
				energy_daughter1[hbheRechit_depth->at(hbhe_matched_indices) - 1] += hbheRechit_E->at(hbhe_matched_indices);
				matched_indices.push_back(hbhe_matched_indices); // contains all matched indicies for decay product 1
				energy_LLP[hbheRechit_depth->at(hbhe_matched_indices) - 1] += hbheRechit_E->at(hbhe_matched_indices);
			}
			if (idx_llp_decay == 1) { // for decay product 2, make sure to not double count the same hbhe_matched_indices for overall LLP energy
				energy_daughter2[hbheRechit_depth->at(hbhe_matched_indices) - 1] += hbheRechit_E->at(hbhe_matched_indices);
				if (!(std::count(matched_indices.begin(), matched_indices.end(), hbhe_matched_indices))) { 
					energy_LLP[hbheRechit_depth->at(hbhe_matched_indices) - 1] += hbheRechit_E->at(hbhe_matched_indices);
				}
			}
		}
	}

	int totalE_LLP = 0, totalE_daughter1 = 0, totalE_daughter2 = 0;
	for (int i = 0; i < energy_LLP.size(); i++) { // total energy calculation
		totalE_LLP += energy_LLP[i];
		totalE_daughter1 += energy_daughter1[i];
		totalE_daughter2 += energy_daughter2[i];
	}
	// energy normalization
	if (totalE_LLP > 0) for (int i=0; i<energy_LLP.size(); i++) energy_LLP[i] = energy_LLP[i] / totalE_LLP;
	if (totalE_daughter1 > 0) for (int i=0; i<energy_daughter1.size(); i++) energy_daughter1[i] = energy_daughter1[i] / totalE_daughter1;
	if (totalE_daughter2 > 0) for (int i=0; i<energy_daughter2.size(); i++) energy_daughter2[i] = energy_daughter2[i] / totalE_daughter2;

	vector<vector<float>> energy = {energy_LLP, energy_daughter1, energy_daughter2};
	return energy;
}

/* ====================================================================================================================== */
vector<float> DisplacedHcalJetAnalyzer::GetMatchedHcalRechits_Jet( int idx_jet, float deltaR_cut ){
	/* 
	Description: Delivers vector of indices of matched hcal rechits (in hbheRechit)
	Inputs: idx_jet: 		Jet index
			deltaR: 		deltaR between hcalrechit and jet (default: 0.4)
	*/

	vector<float> hbhe_matched_indices;

	for( int j=0; j<hbheRechit_E->size(); j++ ){

		// q: for LLP matching, had to consider right depth, for jet should take all depths? 		
			
		float dR_temp = DeltaR( jet_Eta->at(idx_jet), hbheRechit_Eta->at(j), jet_Phi->at(idx_jet), hbheRechit_Phi->at(j) );

		if( dR_temp < deltaR_cut ) hbhe_matched_indices.push_back( j );
	}

	return hbhe_matched_indices;

}

/* ====================================================================================================================== */
vector<float> DisplacedHcalJetAnalyzer::GetEnergyProfile_Jet(int idx_jet, float deltaR_cut) { // given a jet, find the normalized energy profile from associated HB rechits 

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetEnergyProfile_Jet()"<<endl;

	// vectors to fill with energy in each depth
	vector<float> energy_jet = {0,0,0,0};

	vector<float> matchedRechit = GetMatchedHcalRechits_Jet(idx_jet, deltaR_cut);
	
	for (int i = 0; i < matchedRechit.size(); i++) {
		energy_jet[hbheRechit_depth->at(matchedRechit[i]) - 1] += hbheRechit_E->at(matchedRechit[i]);
	}

	int totalE_jet = 0;
	for (int i = 0; i < energy_jet.size(); i++) totalE_jet += energy_jet[i]; // total energy calculation
	// energy normalization
	if (totalE_jet > 0) for (int i=0; i<energy_jet.size(); i++) energy_jet[i] = energy_jet[i] / totalE_jet;

	return energy_jet;
}

/* ====================================================================================================================== */
vector<float> DisplacedHcalJetAnalyzer::GetEtaPhiSpread_Jet(int idx_jet, float deltaR_cut) { // given a jet, find the normalized energy profile from associated HB rechits 

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetEtaPhiSpread_Jet()"<<endl;

	// vector to fill with eta and phi spread
	vector<float> eta_phi = {0,0};

	vector<float> matchedRechit = GetMatchedHcalRechits_Jet(idx_jet, deltaR_cut);
	
	float spread_Eta = 0, spread_Phi = 0, totalE = 0;
	for (int i = 0; i < matchedRechit.size(); i++) {
		float delta_Eta = hbheRechit_Eta->at(matchedRechit[i]) - jet_Eta->at(idx_jet);
		float delta_Phi = deltaPhi(hbheRechit_Phi->at(matchedRechit[i]), jet_Phi->at(idx_jet));
		// now do energy weighting
		spread_Eta 	+= pow(delta_Eta * hbheRechit_E->at(matchedRechit[i]),2);
		spread_Phi 	+= pow(delta_Phi * hbheRechit_E->at(matchedRechit[i]),2);
		totalE 		+= hbheRechit_E->at(matchedRechit[i]);
	}

	spread_Eta = sqrt(spread_Eta) / totalE;
	spread_Phi = sqrt(spread_Phi) / totalE;

	vector<float> spread_Eta_Phi = {spread_Eta, spread_Phi};

	return spread_Eta_Phi;
}

