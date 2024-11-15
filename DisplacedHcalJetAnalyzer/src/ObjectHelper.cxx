// Put object-related functions here (e.g. selection functions, etc)

/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::DeltaR( float eta1, float eta2, float phi1, float phi2){

	//if( debug ) cout<<"DisplacedHcalJetAnalyzer::DeltaR()"<<endl;

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

// double DisplacedHcalJetAnalyzer::deltaR(double eta1, double phi1, double eta2, double phi2) { // calculate deltaR given two eta and phi values
//   double deta = eta1 - eta2;
//   double dphi = deltaPhi(phi1, phi2);
//   return sqrt(deta*deta + dphi*dphi);
// }

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
	vector<float> energy_LLP_total	= {0,0,0,0}; // LLP energy distribution, from combination of b quark decay products
	vector<float> energy_daughter1 	= {0,0,0,0};
	vector<float> energy_daughter2 	= {0,0,0,0};
	vector<float> energy_LLP 		= {0,0,0,0}; // LLP energy distribution, from LLP direction only (not decay products)
	float totalE_LLPb = 0, totalE_daughter1 = 0, totalE_daughter2 = 0, totalE_LLP = 0; // for total energy calculation 

	// for LLP matched with the decay products (match rechits to b quarks)
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
				totalE_daughter1 += hbheRechit_E->at(hbhe_matched_indices);
				matched_indices.push_back(hbhe_matched_indices); // contains all matched indicies for decay product 1
				energy_LLP_total[hbheRechit_depth->at(hbhe_matched_indices) - 1] += hbheRechit_E->at(hbhe_matched_indices);
				totalE_LLPb += hbheRechit_E->at(hbhe_matched_indices);
			}
			if (idx_llp_decay == 1) { // for decay product 2, make sure to not double count the same hbhe_matched_indices for overall LLP energy
				energy_daughter2[hbheRechit_depth->at(hbhe_matched_indices) - 1] += hbheRechit_E->at(hbhe_matched_indices);
				totalE_daughter2 += hbheRechit_E->at(hbhe_matched_indices);
				if (!(std::count(matched_indices.begin(), matched_indices.end(), hbhe_matched_indices))) { 
					energy_LLP_total[hbheRechit_depth->at(hbhe_matched_indices) - 1] += hbheRechit_E->at(hbhe_matched_indices);
					totalE_LLPb += hbheRechit_E->at(hbhe_matched_indices);
				}
			}
		}
	}

	// for just LLP matched, not to the decay products
	vector<float> matchedRechitLLP;
	matchedRechitLLP = GetMatchedHcalRechits_LLP(idx_llp, deltaR_cut);
	for (int i = 0; i < matchedRechitLLP.size(); i++) {
		int hbhe_matched_indices_LLP = matchedRechitLLP[i];
		energy_LLP[hbheRechit_depth->at(hbhe_matched_indices_LLP) - 1] += hbheRechit_E->at(hbhe_matched_indices_LLP);
		totalE_LLP += hbheRechit_E->at(hbhe_matched_indices_LLP);
	}

	// energy normalization
	if (totalE_LLPb > 0) for (int i=0; i<energy_LLP_total.size(); i++) energy_LLP_total[i] = energy_LLP_total[i] / totalE_LLPb;
	if (totalE_daughter1 > 0) for (int i=0; i<energy_daughter1.size(); i++) energy_daughter1[i] = energy_daughter1[i] / totalE_daughter1;
	if (totalE_daughter2 > 0) for (int i=0; i<energy_daughter2.size(); i++) energy_daughter2[i] = energy_daughter2[i] / totalE_daughter2;
	if (totalE_LLP > 0) for (int i=0; i<energy_LLP.size(); i++) energy_LLP[i] = energy_LLP[i] / totalE_LLP;

	vector<float> totalEnergies = {totalE_LLPb, totalE_daughter1, totalE_daughter2, totalE_LLP};

	vector<vector<float>> energy = {energy_LLP_total, energy_daughter1, energy_daughter2, energy_LLP, totalEnergies};
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

		if (!isRechitValid(hbheRechit_E->at(j), hbheRechit_depth->at(j))) continue;

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

	vector<float> matchedRechit = GetMatchedHcalRechits_Jet(idx_jet, deltaR_cut); // already accounts for valid rechits
	
	for (int i = 0; i < matchedRechit.size(); i++) {
		energy_jet[hbheRechit_depth->at(matchedRechit[i]) - 1] += hbheRechit_E->at(matchedRechit[i]);
	}

	float totalE_jet = 0;
	for (int i = 0; i < energy_jet.size(); i++) totalE_jet += energy_jet[i]; // total energy calculation
	// energy normalization
	if (totalE_jet > 0) { 
		for (int i=0; i<energy_jet.size(); i++) energy_jet[i] = energy_jet[i] / totalE_jet;
	}
	else energy_jet = {-1, -1, -1, -1}; // default if no matched rechits

	return energy_jet;
}


/* ====================================================================================================================== */
vector<pair<float,int>> DisplacedHcalJetAnalyzer::Get3RechitE_Jet(int idx_jet, float deltaR_cut) { // given a jet, find the 3 highest energy HB rechits

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::Get3RechitE_Jet()"<<endl;

	// vectors to fill with highest three rechit energies for those matched to jet. Last entry is the total energy of all matched rechits
	pair<float,int> p1 = make_pair(0.0, 0); // leading energy rechit
	pair<float,int> p2 = make_pair(0.0, 0); // sub-leading rechit
	pair<float,int> p3 = make_pair(0.0, 0); // ssub-leading
	pair<float,int> p4 = make_pair(0.0, 0); // total energy
	vector<pair<float,int>> RechitE_D = {p1, p2, p3, p4};
	float totalE = 0;

	vector<float> matchedRechit = GetMatchedHcalRechits_Jet(idx_jet, deltaR_cut);
	
	for (int i = 0; i < matchedRechit.size(); i++) {
		float currentE = hbheRechit_E->at(matchedRechit[i]);
		float currentD = hbheRechit_depth->at(matchedRechit[i]);
		totalE += currentE;
		if (currentE > RechitE_D[0].first) {
			RechitE_D[2].first = RechitE_D[1].first;
			RechitE_D[1].first = RechitE_D[0].first;
			RechitE_D[0].first = currentE;
			RechitE_D[2].second = RechitE_D[1].second;
			RechitE_D[1].second = RechitE_D[0].second;
			RechitE_D[0].second = currentD;
		}
		else if (currentE > RechitE_D[1].first) {
			RechitE_D[2].first = RechitE_D[1].first;
			RechitE_D[1].first = currentE;
			RechitE_D[2].second = RechitE_D[1].second;
			RechitE_D[1].second = currentD;
		}
		else if (currentE > RechitE_D[2].first) {
			RechitE_D[2].first = currentE;
			RechitE_D[2].second = currentD;
		}
	}
	RechitE_D[3].first = totalE;

	return RechitE_D;
}

/* ====================================================================================================================== */
vector<float> DisplacedHcalJetAnalyzer::GetEtaPhiSpread_Jet(int idx_jet, float deltaR_cut) { 
	// given a jet, find the normalized energy profile from associated HB rechits 
	// returns eta, phi spread (not energy weighted), eta, phi spread (energy weighted), S_eta eta, S_phi phi, S_eta phi

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetEtaPhiSpread_Jet()"<<endl;

	// vector to fill with eta and phi spread
	vector<float> eta_phi = {0,0,0,0};

	vector<float> matchedRechit = GetMatchedHcalRechits_Jet(idx_jet, deltaR_cut);

	float spread_Eta = 0, spread_Phi = 0, spread_Eta_E = 0, spread_Phi_E = 0, totalE = 0;
	float S_ee = 0, S_pp = 0, S_ep = 0;
	for (int i = 0; i < matchedRechit.size(); i++) {
		float delta_Eta = hbheRechit_Eta->at(matchedRechit[i]) - jet_Eta->at(idx_jet);
		float delta_Phi = deltaPhi(hbheRechit_Phi->at(matchedRechit[i]), jet_Phi->at(idx_jet));
		spread_Eta 		+= abs(delta_Eta);
		spread_Phi		+= abs(delta_Phi);
		// now do energy weighting
		spread_Eta_E 	+= pow(delta_Eta * hbheRechit_E->at(matchedRechit[i]),2);
		spread_Phi_E 	+= pow(delta_Phi * hbheRechit_E->at(matchedRechit[i]),2);
		totalE 			+= hbheRechit_E->at(matchedRechit[i]);

		// second moment calculation
		S_ee 		+= delta_Eta * delta_Eta * hbheRechit_E->at(matchedRechit[i]);
		S_pp 		+= delta_Phi * delta_Phi * hbheRechit_E->at(matchedRechit[i]);
		S_ep 		+= abs(delta_Eta) * abs(delta_Phi) * hbheRechit_E->at(matchedRechit[i]);
	}
	
	if (matchedRechit.size() > 0) {
		spread_Eta = spread_Eta / matchedRechit.size();
		spread_Phi = spread_Phi / matchedRechit.size();
		spread_Eta_E = sqrt(spread_Eta_E) / totalE;
		spread_Phi_E = sqrt(spread_Phi_E) / totalE;

		S_ee = S_ee / totalE;
		S_pp = S_pp / totalE;
		S_ep = S_ep / totalE;
	}
	else {
		spread_Eta = spread_Phi = spread_Eta_E = spread_Phi_E = S_ee = S_pp = S_ep = -1; // default if no matched rechits! 
	}

	vector<float> spread_Eta_Phi = {spread_Eta, spread_Phi, spread_Eta_E, spread_Phi_E, S_ee, S_pp, S_ep};

	return spread_Eta_Phi;
}

/* ====================================================================================================================== */
vector<float> DisplacedHcalJetAnalyzer::GetTDCavg_Jet(int idx_jet, float deltaR_cut) {
	// given a jet, find the average TDC value for energetic rechits in the jet
	// returns avg TDC, energy weighted average TDC, and n delayed TDC over threshold

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetTDCavg_Jet()"<<endl;

	vector<float> matchedRechit = GetMatchedHcalRechits_Jet(idx_jet, deltaR_cut);

	float rechitN = 0;
	int totalTDC = 0;
	int totalTime = 0;
	float totalEnergy = 0;
	int energyTDC = 0;
	float avgTDC = 0;
	float avgTDCenergy = 0;
	float nDelayedTDC = 0;
	float avgTime = 0;

	for (int i = 0; i < matchedRechit.size(); i++) {
		int TDC = hbheRechit_auxTDC->at(matchedRechit[i]); // decoded in ntupler (six bit mask, bit shifting applied)
		int time = hbheRechit_time->at(matchedRechit[i]); // MAHI time, may be unreliable
		int energy = hbheRechit_E->at(matchedRechit[i]);
		if (energy > 4 && TDC < 3) {
			rechitN += 1;
			totalTDC += TDC;
			totalTime += time;
			totalEnergy += energy;
			energyTDC += TDC * energy;
			if (TDC > 0) nDelayedTDC += 1;
		}
	}

	if (rechitN > 0) {
		avgTDC = totalTDC / rechitN;
		avgTime = totalTime / rechitN;
		avgTDCenergy = energyTDC / totalEnergy;
	}
	
	// debugging, seeing ints 0,1,2 not floats
	/*
	if (rechitN > 1) {
		std::cout << avgTDC << " = avgTDC from totalTDC = " << totalTDC << " and rechitN = " << rechitN << std::endl;
		std::cout << avgTDCenergy << " = avgTDCenergy from energyTDC = " << energyTDC << " and totalEnergy = " << totalEnergy << std::endl;
	}
	*/
	vector<float> TDC_TDCenergy = {avgTDC,avgTDCenergy,nDelayedTDC,avgTime};

	return TDC_TDCenergy;
}

/* ====================================================================================================================== */
vector<float> DisplacedHcalJetAnalyzer::GetDepthTowers_Jet(int idx_jet, float deltaR_cut) { // given a jet, find the emulated number of depth flagged towers from associated HB rechits 

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetDepthTowers_Jet()"<<endl;

	// tracking the dpeth flagged towers 
	int depth_flag[4][32][72] = {0};

	vector<float> matchedRechit = GetMatchedHcalRechits_Jet(idx_jet, deltaR_cut); // already accounts for valid rechits
	
	for (int i = 0; i < matchedRechit.size(); i++) {
		int depth = hbheRechit_depth->at(matchedRechit[i]);
		float energy = hbheRechit_E->at(matchedRechit[i]);
		int ieta = hbheRechit_iEta->at(matchedRechit[i]);
		int iphi = hbheRechit_iPhi->at(matchedRechit[i])

		int shift = 15;
		if (ieta < 0) shift = 16;
		value = 0;
		if ((depth == 1 || depth == 2) && energy < 1) value = 1;
		if ((depth == 3 || depth == 4) && energy >= 5) value = 1;
		depth_flag[depth - 1][ieta + shift][iphi - 1] = value;
	}

	n_depth_towers = 0;
	for ieta in range(32) {
		for iphi in range(72) {
			int sum123 = depth_flag[0][ieta][iphi] + depth_flag[1][ieta][iphi] + depth_flag[2][ieta][iphi];
			int sum124 = depth_flag[0][ieta][iphi] + depth_flag[1][ieta][iphi] + depth_flag[3][ieta][iphi];
			if (sum123 >= 3 || sum124 >= 3) n_depth_towers += 1;
			sum123 = 0;
			sum124 = 0;			
		}
	}

	return n_depth_towers;
}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::IsMuonIsolatedTight(int muon_index) {
	// given a muon, determine if it is isolated (tight)
	// https://github.com/cms-lpc-llp/llp_analyzer/blob/master/src/RazorAnalyzer.cc#L2336C32-L2336C32 and https://cds.cern.ch/record/2815162/files/SMP-21-005-pas.pdf 

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::IsMuonIsolatedTight()"<<endl;

	if ( ( (muon_chargedIso->at(muon_index) + fmax(0.0,  muon_photonIso->at(muon_index) + muon_neutralHadIso->at(muon_index) - 0.5*muon_pileupIso->at(muon_index))) / muon_Pt->at(muon_index) < 0.15) ) return true;
	else return false;
}

/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::GetElectronEffectiveAreaMean(int i) {
	// given an electron, return the effective area for the isolation calculation
	// from https://github.com/cms-lpc-llp/llp_analyzer/blob/master/src/RazorAnalyzer.cc#L1885-L1905

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetElectronEffectiveAreaMean()"<<endl;

    float effArea = 0.0;
	float eta = ele_EtaSC->at(i);
    //Effective areas below are for the sum of Neutral Hadrons + Photons
    if (fabs(eta) < 1.0) {
        effArea = 0.1440;
    } else if (fabs(eta) < 1.479) {
        effArea = 0.1562;
    } else if (fabs(eta) < 2.0) {
        effArea = 0.1032;
    } else if (fabs(eta) < 2.2) {
        effArea = 0.0859;
    } else if (fabs(eta) < 2.3) {
		effArea = 0.1116;
    } else if (fabs(eta) < 2.4) {
        effArea = 0.1321;
    } else if (fabs(eta) < 2.5) {
        effArea = 0.1654;
    }
    return effArea;
}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::IsElectronIsolatedTight(int ele_index) {
	// given an electron, determine if iit is isolated (tight)
	// https://github.com/cms-lpc-llp/llp_analyzer/blob/master/src/RazorAnalyzer.cc#L1885-L1905 and https://cds.cern.ch/record/2815162/files/SMP-21-005-pas.pdf 

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::IsElectronIsolatedTight()"<<endl;

	float combined_iso = (ele_chargedIso->at(ele_index) + fmax(0.0,  ele_photonIso->at(ele_index) + ele_neutralHadIso->at(ele_index) - GetElectronEffectiveAreaMean(ele_index)*fixedGridRhoFastjetAll)) / ele_Pt->at(ele_index);
	bool pass = false;

	if (fabs(ele_EtaSC->at(ele_index)) < 1.479) {
    	if ( combined_iso < 0.0695) {
            pass = true;
        }
    } else {
        if ( combined_iso < 0.0821) {
            pass = true;
        }
    }
    return pass;
}

/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::TransverseLeptonMass(float pT, float phi) {
	// given a lepton, find the transverse mass

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::transverseLeptonMass()"<<endl;

	float transverseM = sqrt( 2 * pT * met_Pt * (1 - cos( phi - met_Phi ) ) ); 
	return transverseM;
}

/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::PhiVectorSum(float pT, float phi) {
	// given a lepton, find the phi vector sum of it and the event MET

	float x_vector = pT * cos(phi) + met_Pt * cos(met_Phi);
	float y_vector = pT * sin(phi) + met_Pt * sin(met_Phi);
	float phiVectorSum = atan2(y_vector, x_vector);
	return phiVectorSum;
}