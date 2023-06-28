/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::DeclareOutputTrees(){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::DeclareOutputTrees()"<<endl;

	if( !save_trees ){
		treenames = {};
		return;
	}

	cout<<"Declaring Output Trees..."<<endl;	

	treenames = { "NoSel" }; 

	vector<string> myvars_bool = {
		"",
	};

	vector<string> myvars_int = {
		"run","lumi","event",
		"RechitN","RechitN_1GeV","RechitN_5GeV","RechitN_10GeV",
		"TrackN", "ecalRechitN",
	};	

	for ( int i=0; i<3; i++ ) {
		myvars_int.push_back( Form("jet%d_rechitN", i) );

		if (i < 2) {
			myvars_int.push_back( Form("LLP%d_rechitN", i));
		}
	}

	vector<string> myvars_float = {
		//"",
	};

	for( int i=0; i<3; i++ ) {
		myvars_float.push_back( Form("jet%d_energy", i) );
		myvars_float.push_back( Form("jet%d_pt", i) );
		myvars_float.push_back( Form("jet%d_Eta", i) );
		myvars_float.push_back( Form("jet%d_Phi", i) );
		myvars_float.push_back( Form("jet%d_isTruthMatched", i) );

		myvars_float.push_back( Form("jet%d_etaSpread", i) );
		myvars_float.push_back( Form("jet%d_etaSpread_energy", i) );
		myvars_float.push_back( Form("jet%d_phiSpread", i) );
		myvars_float.push_back( Form("jet%d_phiSpread_energy", i) );
		myvars_float.push_back( Form("jet%d_track0Pt", i) );
		myvars_float.push_back( Form("jet%d_track1Pt", i) );
		myvars_float.push_back( Form("jet%d_track2Pt", i) );
		myvars_float.push_back( Form("jet%d_track0dzToPV", i) );
		myvars_float.push_back( Form("jet%d_track1dzToPV", i) );
		myvars_float.push_back( Form("jet%d_track2dzToPV", i) );

		for (int d=0; d<4; d++) myvars_float.push_back( Form("jet%d_energyFrac_depth%d", i, d+1) );

		if (i < 2) {
			myvars_float.push_back( Form("LLP%d_DecayR", i));
			myvars_float.push_back( Form("LLP%d_DecayX", i));
			myvars_float.push_back( Form("LLP%d_DecayY", i));
			myvars_float.push_back( Form("LLP%d_DecayZ", i));
			myvars_float.push_back( Form("LLP%d_DecayD", i));
			myvars_float.push_back( Form("LLP%d_DecayT", i));
			myvars_float.push_back( Form("LLP%d_DecayCtau", i));

			for (int d=0; d<4; d++) myvars_float.push_back( Form("LLP%d_energyFrac_depth%d", i, d+1));
			myvars_float.push_back( Form("LLP%d_Eta", i));
			myvars_float.push_back( Form("LLP%d_Phi", i));
		}

	}

	for( int i=0; i<4; i++ ) {
		myvars_float.push_back( Form("LLPDecay%d_pt", i) );
		myvars_float.push_back( Form("LLPDecay%d_eta", i) );
		myvars_float.push_back( Form("LLPDecay%d_phi", i) );
		myvars_float.push_back( Form("LLPDecay%d_E", i) );
		myvars_float.push_back( Form("LLPDecay%d_ProdX", i) );
		myvars_float.push_back( Form("LLPDecay%d_ProdY", i) );
		myvars_float.push_back( Form("LLPDecay%d_ProdZ", i) );
		myvars_float.push_back( Form("LLPDecay%d_ProdR", i) );
		myvars_float.push_back( Form("LLPDecay%d_isTruthMatched", i) );
	}

	for( auto treename: treenames ){
		tree_output[treename] = new TTree( Form("%s",treename.c_str()), Form("%s",treename.c_str()) ); 

		//for( auto var: myvars_bool )
		//	tree_output[treename]->Branch( Form("%s",var.c_str()), &tree_output_vars_bool[var] );

		for( auto var: myvars_int )
			tree_output[treename]->Branch( Form("%s",var.c_str()), &tree_output_vars_int[var] );

		for( auto var: myvars_float )
			tree_output[treename]->Branch( Form("%s",var.c_str()), &tree_output_vars_float[var] );

	}

}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::FillOutputTrees( string treename ){ 

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::FillOutputTrees()"<<endl;

	if ( std::find(treenames.begin(), treenames.end(), treename) == treenames.end() ) return;

	tree_output_vars_int["run"] 	= runNum;
	tree_output_vars_int["lumi"] 	= lumiNum;
	tree_output_vars_int["event"] 	= eventNum;
	tree_output_vars_int["RechitN"] = n_hbheRechit;
	tree_output_vars_int["TrackN"]	= n_track;
	tree_output_vars_int["ecalRechitN"] = n_ecalRechit;

	tree_output_vars_int["RechitN_1GeV"] = 0; 
	tree_output_vars_int["RechitN_5GeV"] = 0; 
	tree_output_vars_int["RechitN_10GeV"] = 0; 
	for (int i = 0; i < n_hbheRechit; i++) {
		if (hbheRechit_E->at(i) > 1) tree_output_vars_int["RechitN_1GeV"] += 1;
		if (hbheRechit_E->at(i) > 5) tree_output_vars_int["RechitN_5GeV"] += 5;
		if (hbheRechit_E->at(i) > 10) tree_output_vars_int["RechitN_10GeV"] += 10;
	}

	// jets are already sorted in jet Pt (not jet E!). Loop over first three jets, and save quantities in the ntuples
	int max_jets = std::min(3, n_jet);
	//std::cout << " " << std::endl;
	//std::cout << "Event = " << eventNum << std::endl;
	for (int i = 0; i < max_jets; i++) {
		tree_output_vars_float[Form("jet%d_energy", i)] = jet_E->at(i);
		tree_output_vars_float[Form("jet%d_pt", i)] 	= jet_Pt->at(i);
		tree_output_vars_float[Form("jet%d_Eta", i)] 	= jet_Eta->at(i);
		tree_output_vars_float[Form("jet%d_Phi", i)] 	= jet_Phi->at(i);
		tree_output_vars_float[Form("jet%d_isTruthMatched", i)] = JetIsTruthMatched( jet_Eta->at(i), jet_Phi->at(i) );

		vector<float> rechitJet = GetMatchedHcalRechits_Jet(i, 0.4);
		vector<float> energy = GetEnergyProfile_Jet(i, 0.4);
		vector<float> spread_Eta_Phi = GetEtaPhiSpread_Jet(i, 0.4); // eta, phi (average); eta, phi (energy weighted)
		
		for (int depth = 0; depth < 4; depth++) tree_output_vars_float[Form("jet%d_energyFrac_depth%d", i, depth+1)] = energy[depth]; // each fractional energy saved in different tree
		tree_output_vars_int[Form("jet%d_rechitN", i)] = rechitJet.size();
		tree_output_vars_float[Form("jet%d_etaSpread", i)] = spread_Eta_Phi[0];
		tree_output_vars_float[Form("jet%d_etaSpread_energy", i)] = spread_Eta_Phi[2];
		tree_output_vars_float[Form("jet%d_phiSpread", i)] = spread_Eta_Phi[1];
		tree_output_vars_float[Form("jet%d_phiSpread_energy", i)] = spread_Eta_Phi[3];
		
		// find three highest pT tracks matched to a jet
		vector<pair<float, float>> track_pt_PV;
		vector<uint> jet_track_index = jet_TrackIndices->at(i);
		for (int j = 0; j < jet_track_index.size(); j++) { // jet_NTracks->at(i) == jet_track_index.size()
			for (int k = 0; k < n_track; k++) { // find which generalTrack matches to jet_track_index[j]
				if (jet_track_index[j] == track_index->at(k)) {
					track_pt_PV.push_back({track_Pt->at(k), track_dzToPV->at(k)});
				}
			} 
		}	
		if (track_pt_PV.size() > 0) {
			std::sort (track_pt_PV.begin(), track_pt_PV.end(), greater<pair<float, float>>());
			for (int j = 0; j < track_pt_PV.size(); j++) if (track_pt_PV[j].second > -9999) std::cout << track_pt_PV[j].first << ", " << track_pt_PV[j].second << std::endl;

			int n_track = std::min(3, (int) jet_track_index.size());
			for (int track = 0; track < n_track; track++) {
				tree_output_vars_float[Form("jet%d_track%dPt", i, track)] = track_pt_PV[track].first;
				tree_output_vars_float[Form("jet%d_track%ddzToPV", i, track)] = track_pt_PV[track].second; 
			}
		}
		// end of track matching 
	}

	for (int i = 0; i < n_gLLP; i++) {
		float decay_radius = GetDecayRadiusHB_LLP(i); // -999 default value
		float distance = GetDecayDistance_LLP(i);
		vector<int> n_rechit_pt4 = GetRechitMult(i, 0.4); // GetRechitMult returns rechit multiplicity associated with LLP [0], first daughter, second daughter
		vector<vector<float>> energy = GetEnergyProfile(i, 0.4); // [0] is LLP, [1] is daughter 1, [2] is daughter 2

		vector<TVector3> decay_product_coords = GetLLPDecayProdCoords(i,0,vector<float>{decay_radius});  // 0 for daughter particle, just getting decay of LLP, since R is LLP decay radius
		if (decay_product_coords.size() > 0) {
			float eta = decay_product_coords.at(0).Eta();
			float phi = decay_product_coords.at(0).Phi();
			tree_output_vars_float[Form("LLP%d_Eta", i)] = eta;
			tree_output_vars_float[Form("LLP%d_Phi", i)] = phi;
		}

		tree_output_vars_int[Form("LLP%d_rechitN", i)] = n_rechit_pt4[0];
		tree_output_vars_float[Form("LLP%d_DecayR", i)] = decay_radius;

		tree_output_vars_float[Form("LLP%d_DecayX", i)] = gLLP_DecayVtx_X->at(i);
		tree_output_vars_float[Form("LLP%d_DecayY", i)] = gLLP_DecayVtx_Y->at(i);
		tree_output_vars_float[Form("LLP%d_DecayZ", i)] = gLLP_DecayVtx_Z->at(i);
		tree_output_vars_float[Form("LLP%d_DecayD", i)] = distance;
		tree_output_vars_float[Form("LLP%d_DecayT", i)] = distance * ( 1 / gLLP_Beta->at(i) - 1) * 0.03336; // 1/c in ns / cm to give answer in ns
		tree_output_vars_float[Form("LLP%d_DecayCtau", i)] = distance * (sqrt( 1 / pow(gLLP_Beta->at(i),2) - 1)); 

		if (energy[0][0] + energy[0][1] + energy[0][2] + energy[0][3] > 0) { // ensure there is positive energy in one depth
			for (int depth = 0; depth < 4; depth++) tree_output_vars_float[Form("LLP%d_energyFrac_depth%d", i, depth+1)] = energy[0][depth]; // each fractional energy saved in different tree
		}
	}

	for (int i = 0; i < gLLPDecay_iParticle.size(); i++) {
		int idx_gParticle = gLLPDecay_iParticle.at(i);
		tree_output_vars_float[Form("LLPDecay%d_pt", i)]    = gParticle_Pt->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_eta", i)]   = gParticle_Eta->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_phi", i)]   = gParticle_Phi->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_E", i)]   	= gParticle_E->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_ProdX", i)] = gParticle_ProdVtx_X->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_ProdY", i)] = gParticle_ProdVtx_Y->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_ProdZ", i)] = gParticle_ProdVtx_Z->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_ProdR", i)] = pow( pow(gParticle_ProdVtx_X->at(idx_gParticle), 2.) + pow(gParticle_ProdVtx_Y->at(idx_gParticle), 2.), 0.5 );
		tree_output_vars_float[Form("LLPDecay%d_isTruthMatched", i)] = LLPDecayIsTruthMatched( i );
	}

	tree_output[treename]->Fill();

}	

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::WriteOutputTrees(){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::WriteOutputTrees()"<<endl;

	cout<<"Writing Tree(s)..."<<endl;

	for(map<string,TTree*>::iterator it = tree_output.begin(); it != tree_output.end(); ++it){
		//AddWeightBranch( tree_output[it->first] );
		tree_output[it->first]->Write("", TObject::kOverwrite); 
	}

}
