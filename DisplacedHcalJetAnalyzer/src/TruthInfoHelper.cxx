/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::SetLLPDecayProducts(){
	/* 
	Description: Loads indices of LLP decay products (corresponding to which LLP, and which gParticle) (pt-ordered)
	*/

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::LoadLLPDecayProds()"<<endl;

	// These should already be cleared, but just in case
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

		vector<int> map_gLLP_to_gParticle_indices_temp;
		for( int i_tp=0; i_tp<gParticle_Pt->size(); i_tp++ ){

			// Require parent is LLP
			if( abs(gParticle_ParentId->at(i_tp)) != 9000006) continue; 

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
int DisplacedHcalJetAnalyzer::GetLLPDecayProductIndex( int idx_llp, int idx_llp_decay ){
	/* 
	Description: Returns gParticle index of LLP decay product. Must have already run SetLLPDecayProducts in the event
	*/

	return map_gLLP_to_gParticle_indices.at(idx_llp).at(idx_llp_decay);

}

/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::GetDecayRadiusHB_LLP( int idx_llp ) {
	/*
	Description: get the radius of LLP decay, given that decay is in HB
	Input: idx_llp:        LLP index (generally either 0 or 1)
	*/
	// HB coordinates in cm
	double HB_outer_radius = 295;
	double HB_inner_radius = 175;
	double HE_start = 390;

	double x_LLP = gLLP_DecayVtx_X->at(idx_llp);
	double y_LLP = gLLP_DecayVtx_Y->at(idx_llp);
	double z_LLP = gLLP_DecayVtx_Z->at(idx_llp);
	
	float radiusLLPdecay = -999;
    if (abs(z_LLP) < HE_start) { 
		radiusLLPdecay = sqrt( pow(x_LLP,2) + pow(y_LLP,2)); // in cm
	}
	return radiusLLPdecay;
}

/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::GetDecayDistance_LLP( int idx_llp ) {
	/*
	Description: get the total distance of LLP decay
	Input: idx_llp:        LLP index (generally either 0 or 1)
	*/
	double x_LLP = gLLP_DecayVtx_X->at(idx_llp);
	double y_LLP = gLLP_DecayVtx_Y->at(idx_llp);
	double z_LLP = gLLP_DecayVtx_Z->at(idx_llp);
	
    float distance = sqrt( pow(x_LLP,2) + pow(y_LLP,2) + pow(z_LLP,2) ); // in cm

	return distance;
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
	int idx_gParticle = GetLLPDecayProductIndex( idx_llp, idx_llp_decay);

	vector<float> hbhe_matched_indices;

	for( int i=0; i<hbheRechit_E->size(); i++ ){
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
vector<float> DisplacedHcalJetAnalyzer::GetMatchedHcalRechits_LLPDecay_Overlap( int idx_llp, float deltaR_cut ){
	/*
	Description: Given a LLP, find the overlap in the matched rechits of its decy products
	Inputs: idx_llp: 		LLP index (0 or 1)
			deltaR_cut:		deltaR between hcalrechit and LLP decay product
	*/

	vector<float> matchedRechit[2];
	for (int idx_llp_decay = 0; idx_llp_decay < 2; idx_llp_decay++) {
		matchedRechit[idx_llp_decay] = GetMatchedHcalRechits_LLPDecay(idx_llp, idx_llp_decay, deltaR_cut);
	}
	vector<float> v_intersection;
	std::set_intersection( matchedRechit[0].begin(), matchedRechit[0].end(), matchedRechit[1].begin(), matchedRechit[1].end(), std::back_inserter(v_intersection));

	return v_intersection;
}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::JetIsTruthMatched( float jet_eta, float jet_phi, float deltaR_cut ){
	/* 
	Description: Delivers true/false on if there is an llp decay product matched to a reco jet object
	deltaR_cut: deltaR between jet and LLP decay prod (default: 0.4)	
	*/

	TVector3 vec_jet;
	vec_jet.SetPtEtaPhi( 235., jet_eta, jet_phi );

	for( int i_tru=0; i_tru < gLLPDecay_iParticle.size(); i_tru++ ){
		int idx_llp = gLLPDecay_iLLP.at(i_tru);

		TVector3 vec_llp;
		vec_llp.SetXYZ( gLLP_DecayVtx_X->at(idx_llp), gLLP_DecayVtx_Y->at(idx_llp), gLLP_DecayVtx_Z->at(idx_llp) );
		TVector3 vec_jet_new = vec_jet - vec_llp;

		int idx_gParticle = gLLPDecay_iParticle.at(i_tru);
		float dR_temp     = DeltaR( gParticle_Eta->at(idx_gParticle), vec_jet_new.Eta(), gParticle_Phi->at(idx_gParticle), vec_jet_new.Phi() );

		if( dR_temp < deltaR_cut ) 
			return true;
	}

	return false;
}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::LLPDecayIsTruthMatched( int idx_llp, int idx_llp_decay, float deltaR_cut ){
	/* 
	Description: Delivers true/false on if there is an llp decay product matched to a reco akt jet object
	deltaR_cut: deltaR between jet and LLP decay prod (default: 0.4)	
	*/

	TVector3 vec_llp;
	vec_llp.SetXYZ( gLLP_DecayVtx_X->at(idx_llp), gLLP_DecayVtx_Y->at(idx_llp), gLLP_DecayVtx_Z->at(idx_llp) );

	int idx_gParticle = GetLLPDecayProductIndex( idx_llp, idx_llp_decay);

	for( int i=0; i<jet_Pt->size(); i++){
		TVector3 vec_jet;
		if( jet_Pt->at(i) < 40 ) continue;
		vec_jet.SetPtEtaPhi( 235., jet_Eta->at(i), jet_Phi->at(i) );
		TVector3 vec_jet_new = vec_jet - vec_llp;

		float dR_temp = DeltaR( gParticle_Eta->at(idx_gParticle), vec_jet_new.Eta(), gParticle_Phi->at(idx_gParticle), vec_jet_new.Phi() );

		if( dR_temp < deltaR_cut ) 
			return true;
	}

	return false;

}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::LLPDecayIsTruthMatched( int idx_gLLPDecay, float deltaR_cut ){ // TODO : combine with above
	/* 
	Description: Delivers true/false on if there is an llp decay product matched to a reco akt jet object
	deltaR_cut: deltaR between jet and LLP decay prod (default: 0.4)	
	*/

	if( idx_gLLPDecay >= gLLPDecay_iLLP.size() ) return false;

	int idx_gParticle = gLLPDecay_iParticle.at(idx_gLLPDecay);
	int idx_gLLP      = gLLPDecay_iLLP.at(idx_gLLPDecay);

	TVector3 vec_llp;
	vec_llp.SetXYZ( gLLP_DecayVtx_X->at(idx_gLLP), gLLP_DecayVtx_Y->at(idx_gLLP), gLLP_DecayVtx_Z->at(idx_gLLP) );

	for( int i=0; i<jet_Pt->size(); i++){
		TVector3 vec_jet;
		vec_jet.SetPtEtaPhi( 235., jet_Eta->at(i), jet_Phi->at(i) );
		TVector3 vec_jet_new = vec_jet - vec_llp;

		float dR_temp = DeltaR( gParticle_Eta->at(idx_gParticle), vec_jet_new.Eta(), gParticle_Phi->at(idx_gParticle), vec_jet_new.Phi() );

		if( dR_temp < deltaR_cut ) 
			return true;
	}

	return false;

}

/* ====================================================================================================================== */
/*float DisplacedHcalJetAnalyzer::LLPDecayTruthMatchMaxPT( int idx_llp, int idx_llp_decay, float deltaR_cut ){
 
	//Description: Delivers max reco jet pt matched to llp decay product 
	//Inputs: eta/phi: 	reco jet eta/phi
	//		deltaR_cut: deltaR between jet and LLP decay prod (default: 0.4)	
	

	float pt_max = 0;

	TVector3 vec_llp;
	vec_llp.SetXYZ( gLLP_DecayVtx_X->at(idx_llp), gLLP_DecayVtx_Y->at(idx_llp), gLLP_DecayVtx_Z->at(idx_llp) );

	int idx_gParticle = GetLLPDecayProductIndex( idx_llp, idx_llp_decay);

	for( int i=0; i<jet_Pt->size(); i++){
		TVector3 vec_jet;
		vec_jet.SetPtEtaPhi( 235., jet_eta, jet_phi );
		vec_jet_new = vec_jet - vec_llp;

		float dR_temp = DeltaR( gParticle_Eta->at(idx_gParticle), vec_jet_new.Eta(), gParticle_Phi->at(idx_gParticle), vec_jet_new.Phi() );

		if( dR_temp < deltaR_cut && jet_Pt->at(i) > pt_max ) 
			pt_max = jet_Pt->at(i);

	}

	return pt_max;

}*/

