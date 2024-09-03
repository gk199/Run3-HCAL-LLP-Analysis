/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::SetLLPVariables(){
	/* 
	Description: 
	 -- Loads indices of LLP decay products (corresponding to which LLP, and which gParticle) (pt-ordered)
	 -- Sets LLP decay R, magnitude
	*/

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::SetLLPVariables()"<<endl;

	// These should already be cleared, but just in case 
	gLLP_DecayVtx_R.clear();
	gLLP_DecayVtx_Mag.clear();

	gLLPDecay_iLLP.clear();
	gLLPDecay_iParticle.clear();
	map_gLLP_to_gParticle_indices.clear();
	//vector<vector<int>> map_gLLP_to_gParticle_indices;

	struct TruthParticleTemp {
		float pt;
		int i_LLP;
		int i_gParticle;
	};

	vector<TruthParticleTemp> LLPDecayProducts_temp;

	// Loop over LLPs, gParticles
	for( int i_llp=0; i_llp<gLLP_Pt->size(); i_llp++ ){

	   gLLP_DecayVtx_R.push_back( sqrt( pow(gLLP_DecayVtx_X->at(i_llp),2) + pow(gLLP_DecayVtx_Y->at(i_llp),2))  );
	   gLLP_DecayVtx_Mag.push_back( sqrt( pow(gLLP_DecayVtx_R.at(i_llp),2) + pow(gLLP_DecayVtx_Z->at(i_llp),2))  );

		vector<int> map_gLLP_to_gParticle_indices_temp;
		for( int i_tp=0; i_tp<gParticle_Pt->size(); i_tp++ ){

			// Require parent is LLP
			if( abs(gParticle_ParentId->at(i_tp)) != 9000006 && abs(gParticle_ParentId->at(i_tp)) != 6000113 ) continue; 

			// Require production is decay vtx of LLP
			if( gParticle_ProdVtx_X->at(i_tp) != gLLP_DecayVtx_X->at(i_llp) || gParticle_ProdVtx_Y->at(i_tp) != gLLP_DecayVtx_Y->at(i_llp) || gParticle_ProdVtx_Z->at(i_tp) != gLLP_DecayVtx_Z->at(i_llp) ) 
				continue;

			if (debug) std::cout << gParticle_ParentId->at(i_tp) << " = gParticle_ParentId->at(i_tp)" << std::endl;

			LLPDecayProducts_temp.push_back( { gParticle_Pt->at(i_tp),  i_llp, i_tp } );
			map_gLLP_to_gParticle_indices_temp.push_back( i_tp );
		}
		map_gLLP_to_gParticle_indices.push_back( map_gLLP_to_gParticle_indices_temp );
	}

	sort(LLPDecayProducts_temp.begin(), LLPDecayProducts_temp.end(), [](const auto& i, const auto& j) { return i.pt > j.pt; } );

	for( int i=0; i<LLPDecayProducts_temp.size(); i++ ){
		gLLPDecay_iLLP.push_back( LLPDecayProducts_temp.at(i).i_LLP );
		gLLPDecay_iParticle.push_back( LLPDecayProducts_temp.at(i).i_gParticle );
	}

	return;

}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::isRechitValid(float RechitEnergy, int RechitDepth) {
	vector<float> energy_cuts_2022initial = {0.1, 0.2, 0.3, 0.3};
	vector<float> energy_cuts_2022rereco = 	{0.25, 0.25, 0.3, 0.3}; // best agreement with MC
	vector<float> energy_cuts_2023 = 		{0.4, 0.3, 0.3, 0.3};
	// https://github.com/cms-sw/cmssw/blob/master/RecoParticleFlow/PFClusterProducer/python/particleFlowClusterHBHE_cfi.py#L6
	// https://github.com/swagata87/cmssw/blob/51067fbc70af1ef57a0f6a0f2d7297deefbeba45/RecoParticleFlow/PFClusterProducer/python/particleFlowClusterHBHE_cfi.py#L13
	// https://github.com/cms-sw/cmssw/blob/master/RecoParticleFlow/PFClusterProducer/python/particleFlowRecHitHBHE_cfi.py#L8

	if (RechitEnergy >= energy_cuts_2023[RechitDepth-1]) return true;
	else return false;
}

/* ====================================================================================================================== */
vector<float> DisplacedHcalJetAnalyzer::GetMatchedHcalRechits_LLPDecay( int idx_llp, int idx_llp_decay, float deltaR_cut ){
	/* 
	Description: Delivers vector of indices of matched hcal rechits (in hbheRechit)
	Inputs: idx_llp: 		LLP index (generally either 0 or 1)
			idx_llp_decay: 	LLP decay product index (generally either 0 or 1)
			deltaR: 		deltaR between hcalrechit and LLP decay prod (default: 0.4)
	*/

	// LLP Decay
	TVector3 vec_llp;
	vec_llp.SetXYZ( gLLP_DecayVtx_X->at(idx_llp), gLLP_DecayVtx_Y->at(idx_llp), gLLP_DecayVtx_Z->at(idx_llp) );

	// LLP Decay Products
	int idx_gParticle = map_gLLP_to_gParticle_indices.at(idx_llp).at(idx_llp_decay);

	vector<float> hbhe_matched_indices;

	for( int i=0; i<hbheRechit_E->size(); i++ ){
		if (!isRechitValid(hbheRechit_E->at(i), hbheRechit_depth->at(i))) continue;

		TVector3 vec_rechit;
		vec_rechit.SetXYZ( hbheRechit_X->at(i), hbheRechit_Y->at(i), hbheRechit_Z->at(i) );

		// Shift rechit into LLP frame of reference
		vec_rechit -= vec_llp; 

		float dR_temp = DeltaR( gParticle_Eta->at(idx_gParticle), vec_rechit.Eta(), gParticle_Phi->at(idx_gParticle), vec_rechit.Phi() );

		if( dR_temp < deltaR_cut ) hbhe_matched_indices.push_back( i );

	}

	return hbhe_matched_indices;

}

/* ====================================================================================================================== */
vector<float> DisplacedHcalJetAnalyzer::GetMatchedHcalRechits_LLP( int idx_llp, float deltaR_cut ){
	/* 
	Description: Delivers vector of indices of matched hcal rechits (in hbheRechit) to LLP decay 
	Inputs: idx_llp: 		LLP index (generally either 0 or 1)
			deltaR: 		deltaR between hcalrechit and LLP decay prod (default: 0.4)
	*/

	// LLP Decay
	TVector3 vec_llp;
	vec_llp.SetXYZ( gLLP_DecayVtx_X->at(idx_llp), gLLP_DecayVtx_Y->at(idx_llp), gLLP_DecayVtx_Z->at(idx_llp) );

	vector<float> hbhe_matched_indices;

	for( int i=0; i<hbheRechit_E->size(); i++ ){
		if (!isRechitValid(hbheRechit_E->at(i), hbheRechit_depth->at(i))) continue;

		TVector3 vec_rechit;
		vec_rechit.SetXYZ( hbheRechit_X->at(i), hbheRechit_Y->at(i), hbheRechit_Z->at(i) );

		// Shift rechit into LLP frame of reference
		vec_rechit -= vec_llp; 

		float dR_temp = DeltaR( vec_llp.Eta(), vec_rechit.Eta(), vec_llp.Phi(), vec_rechit.Phi() );

		if( dR_temp < deltaR_cut ) hbhe_matched_indices.push_back( i );

	}

	return hbhe_matched_indices;

}

/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::DeltaR_LLP_b( int idx_llp, int idx_llp_decay){
	/* 
	Description: Delivers delta R between LLP and b quarks
	Inputs: idx_llp: 		LLP index (generally either 0 or 1)
			idx_llp_decay: 	LLP decay product index (generally either 0 or 1)){
	*/

	int idx_gParticle = map_gLLP_to_gParticle_indices.at(idx_llp).at(idx_llp_decay);
	return DeltaR( gLLP_Eta->at(idx_llp), gParticle_Eta->at(idx_gParticle), gLLP_Phi->at(idx_llp), gParticle_Phi->at(idx_gParticle) );
}

/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::DeltaR_b( int idx_llp){
	/* 
	Description: Delivers delta R between LLP and b quarks
	Inputs: idx_llp: 		LLP index (generally either 0 or 1)
	*/

	int idx_gParticle1 = map_gLLP_to_gParticle_indices.at(idx_llp).at(0);
	int idx_gParticle2 = map_gLLP_to_gParticle_indices.at(idx_llp).at(1);
	return DeltaR( gParticle_Eta->at(idx_gParticle1), gParticle_Eta->at(idx_gParticle2), gParticle_Phi->at(idx_gParticle1), gParticle_Phi->at(idx_gParticle2) );
}

/* ====================================================================================================================== */
vector<float> DisplacedHcalJetAnalyzer::DeltaEta_Phi_b( int idx_llp){
	/* 
	Description: Delivers delta R between LLP and b quarks
	Inputs: idx_llp: 		LLP index (generally either 0 or 1)
	*/
	vector<float> delta_eta_phi;
	int idx_gParticle1 = map_gLLP_to_gParticle_indices.at(idx_llp).at(0);
	int idx_gParticle2 = map_gLLP_to_gParticle_indices.at(idx_llp).at(1);
	delta_eta_phi.push_back( abs(gParticle_Eta->at(idx_gParticle1) - gParticle_Eta->at(idx_gParticle2)) );
	delta_eta_phi.push_back(deltaPhi(gParticle_Phi->at(idx_gParticle1), gParticle_Phi->at(idx_gParticle2)));
	return delta_eta_phi;
}

/* ====================================================================================================================== */
vector<float> DisplacedHcalJetAnalyzer::JetIsMatchedTo( float jet_eta, float jet_phi, float deltaR_cut ){
	/* 
	Description: Delivers idx_llp and dR (jet, LLP) if there is an llp decay product matched to a reco jet object
	Merged this and the old JetIsTruthMatched function. To determine if jet is truth matched, just check if JetIsMatchedTo[0] is > -1
	deltaR_cut: deltaR between jet and LLP decay prod (default: 0.4)	
	*/

	TVector3 vec_jet;
	vec_jet.SetPtEtaPhi( 235., jet_eta, jet_phi );

	vector<float> idx_llp_dR;

	// check if matched to LLP
	for( int i_llp=0; i_llp<gLLP_Pt->size(); i_llp++ ){

		// Require decay in HB to match to LLP directly
		if( gLLP_DecayVtx_R.at(i_llp) < 177 || gLLP_DecayVtx_R.at(i_llp) >= 295 || fabs(gLLP_Eta->at(i_llp)) > 1.26 )
			continue;

		float dR_temp = DeltaR( gLLP_Eta->at(i_llp), jet_eta, gLLP_Phi->at(i_llp), jet_phi );

		if( dR_temp < deltaR_cut ) {
			idx_llp_dR.push_back(i_llp);
			idx_llp_dR.push_back(dR_temp);
			return idx_llp_dR;
		}
	}

	// Check if matched to LLP decay product
	for( int i_tru=0; i_tru < gLLPDecay_iParticle.size(); i_tru++ ){
		int idx_llp = gLLPDecay_iLLP.at(i_tru);

		// Require decay not HB, such that it makes sense to match to decay products instead
		if( gLLP_DecayVtx_R.at(idx_llp) >= 177 && gLLP_DecayVtx_R.at(idx_llp) < 295 && fabs(gLLP_Eta->at(idx_llp)) <= 1.26 )
			continue;

		TVector3 vec_llp;
		vec_llp.SetXYZ( gLLP_DecayVtx_X->at(idx_llp), gLLP_DecayVtx_Y->at(idx_llp), gLLP_DecayVtx_Z->at(idx_llp) );
		TVector3 vec_jet_new = vec_jet - vec_llp;

		int idx_gParticle = gLLPDecay_iParticle.at(i_tru);
		float dR_temp     = DeltaR( gParticle_Eta->at(idx_gParticle), vec_jet_new.Eta(), gParticle_Phi->at(idx_gParticle), vec_jet_new.Phi() );

		if( dR_temp < deltaR_cut ) {
			idx_llp_dR.push_back(idx_llp); // LLP index
			idx_llp_dR.push_back(dR_temp); // dR(decay product, jet)
			return idx_llp_dR;
		}
	}
	idx_llp_dR.push_back(-1);
	idx_llp_dR.push_back(-1);
	return idx_llp_dR;
}

/* ====================================================================================================================== */
pair<bool,float> DisplacedHcalJetAnalyzer::LLPDecayIsTruthMatched_LLP_b( int idx_gLLP, int idx_gParticle, float jetPt_cut, float deltaR_cut ){
	/* 
	Description: Delivers true/false on if there is an llp decay product matched to a reco akt jet object
	deltaR_cut: deltaR between jet and LLP decay prod (default: 0.4)	
	jetPt_cut: cut on jet Pt that LLP or decay product is matched to
	*/

	TVector3 vec_llp;
	vec_llp.SetXYZ( gLLP_DecayVtx_X->at(idx_gLLP), gLLP_DecayVtx_Y->at(idx_gLLP), gLLP_DecayVtx_Z->at(idx_gLLP) );

	for( int i=0; i<jet_Pt->size(); i++){
		if (jet_Pt->at(i) < jetPt_cut) continue;
		// Check if LLP is directly matched to a jet
		if( gLLP_DecayVtx_R.at(idx_gLLP) >= 177 && gLLP_DecayVtx_R.at(idx_gLLP) < 295 && fabs(gLLP_Eta->at(idx_gLLP)) <= 1.26 ) {

			float dR_temp = DeltaR( gLLP_Eta->at(idx_gLLP), jet_Eta->at(i), gLLP_Phi->at(idx_gLLP), jet_Phi->at(i) );

			if( dR_temp < deltaR_cut ) {
				pair<bool,float> matchedLLP_eta = make_pair(true, jet_Eta->at(i));
				return matchedLLP_eta;
			}
		}
		// Check if LLP decay products are matched to a jet
		else {		
			TVector3 vec_jet;
			vec_jet.SetPtEtaPhi( 235., jet_Eta->at(i), jet_Phi->at(i) );
			TVector3 vec_jet_new = vec_jet - vec_llp;

			float dR_temp = DeltaR( gParticle_Eta->at(idx_gParticle), vec_jet_new.Eta(), gParticle_Phi->at(idx_gParticle), vec_jet_new.Phi() );

			if( dR_temp < deltaR_cut ) {
				pair<bool,float> matchedDecay_eta = make_pair(true, jet_Eta->at(i));
				return matchedDecay_eta;
			}
		}
	}
	pair<bool,float> unmatched_eta = make_pair(false, -99.0);
	return unmatched_eta;
}

/* ====================================================================================================================== */
pair<bool,float> DisplacedHcalJetAnalyzer::LLPIsTruthMatched( int idx_gLLPDecay, float jetPt_cut, float deltaR_cut ){
	/* 
	Description: Delivers true/false on if there is an llp decay product matched to a reco akt jet object
	deltaR_cut: deltaR between jet and LLP decay prod (default: 0.4)	

	between the two similar LLPIsTruthMatched functions:
	in LLPIsTruthMatched, idx_gLLP  		=  idx_llp 			in LLPDecayIsTruthMatched_LLP_b
	in LLPIsTruthMatched, idx_gParticle  	=  idx_gParticle 	in LLPDecayIsTruthMatched_LLP_b
	*/

	if( idx_gLLPDecay >= gLLPDecay_iLLP.size() ) return make_pair(false, -99.0);

	int idx_gParticle = gLLPDecay_iParticle.at(idx_gLLPDecay); 	// index of this b-quark
	int idx_gLLP      = gLLPDecay_iLLP.at(idx_gLLPDecay); 		// which LLP is associated to this b-quark

	return LLPDecayIsTruthMatched_LLP_b(idx_gLLP, idx_gParticle, jetPt_cut, deltaR_cut); 

}

/* ====================================================================================================================== */
vector<TVector3> DisplacedHcalJetAnalyzer::GetLLPDecayProdCoords(int idx_llp, int idx_llp_decay, vector<float> intersection_depths){ // DEPRECATED
	/* DEPRECATED !!!
	Description: Delivers displaced truth particle eta & phi coordinates for a given set of depths (these vary with depth)
	Inputs: idx_llp: 					LLP index (generally either 0 or 1)
			idx_llp_decay: 				LLP decay product index (generally either 0 or 1)
			intersection_depths:		vector a radii at which to extract eta/phi info. 
	TODO: 	- Are Reco pt/eta/phi reported from (0,0,0) or from PV?
			- Currently only implemented for barrel with constant depths (this is generally ok for HB)
			- Are hbheRechit_X, Y, Z coordinates the center of a cell?
	*/

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetLLPDecayProdCoords()"<<endl;

	vector<TVector3> decay_product_coords;

	if( gLLP_DecayVtx_X->size() < idx_llp+1 ) return decay_product_coords;

	// Get LLP Trajectory // 

	TVector3 vec_pv, vec_sv, vec_llp;
	vec_pv.SetXYZ( 0., 0., 0. ); //PV_X, PV_Y, PV_Z ); TODO: Are Reco pt/eta/phi reported from (0,0,0) or from PV?
	vec_sv.SetXYZ( gLLP_DecayVtx_X->at(idx_llp), gLLP_DecayVtx_Y->at(idx_llp), gLLP_DecayVtx_Z->at(idx_llp) );
	vec_llp = vec_sv-vec_pv;

	// Get LLP Decay Products //

	vector<int> llp_decay_indices;

	// loop over gParticles to determine which gParticle is the LLP decay product 
	for( int i=0; i<gParticle_ProdVtx_X->size(); i++ ){
		if( gParticle_ProdVtx_X->at(i) != gLLP_DecayVtx_X->at(idx_llp) ) continue; // require production is decay vtx of LLP
		if( gParticle_ProdVtx_Y->at(i) != gLLP_DecayVtx_Y->at(idx_llp) ) continue;
		if( gParticle_ProdVtx_Z->at(i) != gLLP_DecayVtx_Z->at(idx_llp) ) continue;
		if (debug) std::cout << gParticle_ParentId->at(i) << " = gParticle_ParentId->at(i)" << std::endl;
		if( abs(gParticle_ParentId->at(i)) != 9000006  && abs(gParticle_ParentId->at(i)) != 6000113 ) continue; // require parent is LLP
		llp_decay_indices.push_back( i );
	}

	if( llp_decay_indices.size() != 2 ) cout<<"WARNING: Detected abnormal number of LLP decay products ("<<llp_decay_indices.size()<<")"<<endl;
	
	if( llp_decay_indices.size() < idx_llp_decay+1 ) return decay_product_coords;
	
	if( gParticle_E->size() < llp_decay_indices[idx_llp_decay]+1 ) return decay_product_coords;

	// Get LLP Trajectory Intersection with Detector Layer //

	for( int i=0; i<intersection_depths.size(); i ++ ){ // i=0

		if( vec_sv.Pt() > intersection_depths.at(i)*1.1 ) continue; // Allow 10% leeway

		float dist_to_depth = intersection_depths.at(i) - vec_llp.Pt(); // Here, Pt is the perp magnitude
		
		TVector3 vec_llp_decay_temp;
		vec_llp_decay_temp.SetPtEtaPhi( dist_to_depth, gParticle_Eta->at(llp_decay_indices[idx_llp_decay]), gParticle_Phi->at(llp_decay_indices[idx_llp_decay]) );
		// essentially, vec_llp_decay_temp is the trajectory of LLP decay product (distance to depth in Pt), eta, phi

		TVector3 vec_intersection_temp = vec_llp + vec_llp_decay_temp;
		decay_product_coords.push_back( vec_intersection_temp );

	}
	return decay_product_coords;
}


