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

	// vectors to fill wiht energy in each depth
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