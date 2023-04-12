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
int DisplacedHcalJetAnalyzer::GetRechitMult(int idx_llp, float deltaR_cut) { // given a LLP, find how many associated HB rechits there are

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetRechitMult()"<<endl;

	/* this finds the total number of HB rechits that are matched to a LLP
	int rechitMult = 0; 
	if (n_hbheRechit > 0) {
		for (int i = 0; i < n_hbheRechit; i++) if (IsTruthMatchedLLPDecay_HcalRechit(i, deltaR_cut)) rechitMult += 1;
	}
	if (rechitMult > 0) std::cout << rechitMult << " = total event rechitMult" << std::endl; 
	*/
	
	vector<float> matchedRechit[2];
	for (int idx_llp_decay = 0; idx_llp_decay < 2; idx_llp_decay++) {
		vector<float> hbhe_matched_indices = GetMatchedHcalRechits_LLPDecay(idx_llp, idx_llp_decay, deltaR_cut);
		matchedRechit[idx_llp_decay] = hbhe_matched_indices;
		// if (hbhe_matched_indices.size() > 0) std::cout << hbhe_matched_indices.size() << " = hbhe_matched_indices.size(), for LLP number " << idx_llp << std::endl;
		// if (matchedRechit[idx_llp_decay].size() > 0 ) std::cout << matchedRechit[idx_llp_decay].size() << " = matchedRechit[idx_llp_decay].size(), for LLP number " << idx_llp << std::endl;
	}
	std::vector<int> v_intersection;
	std::set_intersection( matchedRechit[0].begin(), matchedRechit[0].end(), matchedRechit[1].begin(), matchedRechit[1].end(), std::back_inserter(v_intersection));

	int rechits_forLLP = matchedRechit[0].size() + matchedRechit[1].size() - v_intersection.size();
	// if (rechits_forLLP > 0) std::cout << rechits_forLLP << " = total rechits for this LLP" << std::endl;
	return rechits_forLLP;
}

/* ====================================================================================================================== */
vector<float> DisplacedHcalJetAnalyzer::GetEnergyProfile(int idx_llp, float deltaR_cut) { // given a LLP, find the normalized energy profile from associated HB rechits 

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetEnergyProfile()"<<endl;

	vector<float> energy = {0,0,0,0};

	vector<float> matchedRechit[2];
	for (int idx_llp_decay = 0; idx_llp_decay < 2; idx_llp_decay++) {
		matchedRechit[idx_llp_decay] = GetMatchedHcalRechits_LLPDecay(idx_llp, idx_llp_decay, deltaR_cut);

		for (int i = 0; i < matchedRechit[idx_llp_decay].size(); i++) {
			energy[hbheRechit_depth->at(i) - 1] += hbheRechit_E->at(i); // how much energy in each depth? 
			// TODO this is overcounting rechits, exclude ones already in first set of matched indicies 
		}
	//std::remove_copy_if(matchedRechit[0].begin(), matchedRechit[0].end(), back_inserter(matchedRechit[1]), Contained(matchedRechit[1]));
	}
	int totalE = energy[0] + energy[1] + energy[2] + energy[3];
	if (totalE > 0) for (int i=0; i<energy.size(); i++) energy[i] = energy[i] / totalE;
	return energy;
}