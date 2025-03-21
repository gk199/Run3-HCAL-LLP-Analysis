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
double DisplacedHcalJetAnalyzer::DeltaPhi(double phi1, double phi2) {  // calculate delta phi given two phi values
  double result = phi1 - phi2;
  if(fabs(result) > 9999) return result;
  while (result > TMath::Pi()) result -= 2*TMath::Pi();
  while (result <= -TMath::Pi()) result += 2*TMath::Pi();
  return result;
}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::JetPassesHWQual( int jet_index, float &deltaR_jet_l1jet ){

	float dR = 999.9;
	float L1trig = -999.9;
	for (int j = 0; j < l1jet_Eta->size(); j++) { // loop over L1 jets to determine if a reco jet is matched to jet that passed L1
		float dR_temp = DeltaR( jet_Eta->at(jet_index), l1jet_Eta->at(j), jet_Phi->at(jet_index), l1jet_Phi->at(j) );
		if (dR_temp < dR) { // if matched, save the dR and whether the L1 jet is triggered by HCAL LLP
			dR = dR_temp;
			L1trig = l1jet_hwQual->at(j);
		}
	}

	// Passed by reference
	deltaR_jet_l1jet = dR;

	if( dR < 0.4 && L1trig > 0 ) return true;
	return false;

}

/* ====================================================================================================================== */
vector<pair<float,float>> DisplacedHcalJetAnalyzer::TrackMatcher(int jetIndex, vector<uint> jet_track_index) {

	if (debug) cout << "DisplacedHcalJetAnalyzer::TrackMatcher()"<<endl;

	vector<pair<float, float>> track_pt_index;
	for (int j = 0; j < jet_track_index.size(); j++) { 				// jet_NTracks->at(i) == jet_track_index.size()
		for (int k = 0; k < n_track; k++) { 						// find which generalTrack matches to jet_track_index[j]
			if (jet_track_index[j] == track_index->at(k)) {
				track_pt_index.push_back({track_Pt->at(k), k});
			}
		} 
	}	
	return track_pt_index;
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