/* ====================================================================================================================== */
vector<TVector3> DisplacedHcalJetAnalyzer::GetLLPDecayProdCoords(int idx_llp, int idx_llp_decay, vector<float> intersection_depths){
	/* 
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
		std::cout << gParticle_ParentId->at(i) << " = gParticle_ParentId->at(i)" << std::endl;
		if( abs(gParticle_ParentId->at(i)) != 9000006) continue; // require parent is LLP
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
    // if ((sqrt( pow(x_LLP,2) + pow(y_LLP,2)) > HB_inner_radius) && (sqrt( pow(x_LLP,2) + pow(y_LLP,2)) < HB_outer_radius) && abs(z_LLP) < HE_start) { 
    // if ((sqrt( pow(x_LLP,2) + pow(y_LLP,2)) < HB_outer_radius) && abs(z_LLP) < HE_start) { 
    if (abs(z_LLP) < HE_start) { 
		radiusLLPdecay = sqrt( pow(x_LLP,2) + pow(y_LLP,2)); // in cm
	}
	return radiusLLPdecay;
}

/* ====================================================================================================================== */
vector<float> DisplacedHcalJetAnalyzer::GetMatchedHcalRechits_LLPDecay( int idx_llp, int idx_llp_decay, float deltaR_cut ){
	/* 
	Description: Delivers vector of indices of matched hcal rechits (in hbheRechit)
	Inputs: idx_llp: 		LLP index (generally either 0 or 1)
			idx_llp_decay: 	LLP decay product index (generally either 0 or 1)
			deltaR: 		deltaR between hcalrechit and LLP decay prod (default: 0.4)
	*/

	vector<float> HBDepths = { 181.05, 190.04, 214.16, 244.76 };

	vector<TVector3> decay_product_coords = GetLLPDecayProdCoords( idx_llp, idx_llp_decay, HBDepths );

	vector<float> hbhe_matched_indices;

	for( int i=0; i<decay_product_coords.size(); i++ ){

		for( int j=0; j<hbheRechit_E->size(); j++ ){

			if( i+1 != hbheRechit_depth->at(j) ) continue; // Only allow correct depth
		
			float dR_temp = DeltaR( decay_product_coords.at(i).Eta(), hbheRechit_Eta->at(j), decay_product_coords.at(i).Phi(), hbheRechit_Phi->at(j) );

			if( dR_temp < deltaR_cut ) hbhe_matched_indices.push_back( j );
		}
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
bool DisplacedHcalJetAnalyzer::IsTruthMatchedLLPDecay_HcalRechit( int idx_hbheRechit, float deltaR_cut ){
	/* 
	Description: Delivers true/false on if there is an llp decay product consistent with a given hbhe rechit
	Inputs: idx_hbheRechit: hbhe rechit index
			deltaR: 		deltaR between hcalrechit and LLP decay prod (default: 0.4)	
	TODO: 	- add which LLP / gParticle 
	*/

	if( hbheRechit_E->size() < idx_hbheRechit+1 ) return false;

	float depth_value_temp = pow( pow( hbheRechit_X->at(idx_hbheRechit), 2.) + pow( hbheRechit_Y->at(idx_hbheRechit), 2.), 0.5);

	for( int i_llp=0; i_llp < gLLP_Pt->size(); i_llp++ ){
		for( int i_llp_decay=0; i_llp_decay < 2; i_llp_decay++ ){
			vector<TVector3> decay_product_coords = GetLLPDecayProdCoords( i_llp, i_llp_decay, vector<float>{depth_value_temp} );

			if (decay_product_coords.size() == 0) continue;

			float dR_temp = DeltaR( decay_product_coords.at(0).Eta(), hbheRechit_Eta->at(idx_hbheRechit), decay_product_coords.at(0).Phi(), hbheRechit_Phi->at(idx_hbheRechit) );

			if( dR_temp < deltaR_cut ) return true;

		}
	}

	return false; 

}


