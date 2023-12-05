/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::DeclareOutputTrees(){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::DeclareOutputTrees()"<<endl;

	if( !save_trees ){
		cout<<"  NOTE: 'save_trees' is set to false. Will not run over ANY tree categories..."<<endl;
		treenames = {};
		return;
	}

	cout<<"Declaring Output Trees..."<<endl;	

	treenames = { "NoSel", "WPlusJets" }; 

	vector<string> myvars_bool = {
		//"",
	};
	for (int i = 0; i < HLT_Indices.size(); i++) {
		myvars_bool.push_back(HLT_Names[i]);
	}

	vector<string> myvars_int = {
		"run","lumi","event",
		"RechitN","RechitN_1GeV","RechitN_5GeV","RechitN_10GeV",
		"TrackN", "ecalRechitN", "HBHE_Rechit_auxTDC"
	};	

	for ( int i=0; i<3; i++ ) {
		myvars_int.push_back( Form("jet%d_RechitN", i) );

		if (i < 2) {
			myvars_int.push_back( Form("LLP%d_RechitN", i));
		}
	}

	vector<string> myvars_float = {
		"met_Pt", "met_Phi", "met_SumEt", "eventHT"
	};

	// Add BDT Scores //
	for( auto bdt_tag: bdt_tags ){
		myvars_float.push_back( Form("bdtscore_%s", bdt_tag.c_str()) );
	}

	// Add Physics Variables //

	for( int i=0; i<3; i++ ) {
		myvars_float.push_back( Form("l1jet%d_Pt", i) );
		myvars_float.push_back( Form("l1jet%d_E", i) );
		myvars_float.push_back( Form("l1jet%d_Eta", i) );
		myvars_float.push_back( Form("l1jet%d_Phi", i) );
		myvars_float.push_back( Form("l1jet%d_hwQual", i) );
		
		myvars_float.push_back( Form("jet%d_Pt", i) );
		myvars_float.push_back( Form("jet%d_Eta", i) );
		myvars_float.push_back( Form("jet%d_Phi", i) );
		myvars_float.push_back( Form("jet%d_E", i) );
		myvars_float.push_back( Form("jet%d_Mass", i) );
		myvars_float.push_back( Form("jet%d_JetArea", i) );

		myvars_float.push_back( Form("jet%d_ChargedHadEFrac", i) );
		myvars_float.push_back( Form("jet%d_NeutralHadEFrac", i) );
		myvars_float.push_back( Form("jet%d_PhoEFrac", i) );
		myvars_float.push_back( Form("jet%d_EleEFrac", i) );
		myvars_float.push_back( Form("jet%d_MuonEFrac", i) );
		myvars_float.push_back( Form("jet%d_HoverE", i) );

		myvars_int.push_back( Form("jet%d_ChargedHadMult", i) );
		myvars_int.push_back( Form("jet%d_NeutralHadMult", i) );
		myvars_int.push_back( Form("jet%d_PhoMult", i) );
		myvars_int.push_back( Form("jet%d_EleMult", i) );
		myvars_int.push_back( Form("jet%d_MuonMult", i) );

		myvars_float.push_back( Form("jet%d_PtAllTracks", i) );
		myvars_float.push_back( Form("jet%d_PtAllPVTracks", i) );
		myvars_int.push_back( Form("jet%d_NVertexTracks", i) );
		myvars_int.push_back( Form("jet%d_NSelectedTracks", i) );
		myvars_int.push_back( Form("jet%d_NSV", i) );
		myvars_int.push_back( Form("jet%d_NSVCand", i) );
		myvars_float.push_back( Form("jet%d_SV_x", i) );
		myvars_float.push_back( Form("jet%d_SV_y", i) );
		myvars_float.push_back( Form("jet%d_SV_z", i) );
		myvars_int.push_back( Form("jet%d_SV_NTracks", i) );
		myvars_float.push_back( Form("jet%d_SV_Mass", i) );
		myvars_float.push_back( Form("jet%d_SV_DRJet", i) );
		myvars_float.push_back( Form("jet%d_FlightDist2D", i) );
		myvars_float.push_back( Form("jet%d_FlightDist2DErr", i) );
		myvars_float.push_back( Form("jet%d_FlightDist3D", i) );
		myvars_float.push_back( Form("jet%d_FlightDist3DErr", i) );

		myvars_float.push_back( Form("jet%d_isTruthMatched", i) );
		myvars_float.push_back( Form("jet%d_isMatchedTo", i) );
		myvars_float.push_back( Form("jet%d_isMatchedWithDR", i) );
		myvars_float.push_back( Form("jet%d_MatchedLLP_DecayR", i) );
		myvars_float.push_back( Form("jet%d_MatchedLLP_Eta", i) );

		myvars_float.push_back( Form("jet%d_EtaSpread", i) );
		myvars_float.push_back( Form("jet%d_EtaSpread_energy", i) );
		myvars_float.push_back( Form("jet%d_PhiSpread", i) );
		myvars_float.push_back( Form("jet%d_PhiSpread_energy", i) );
		myvars_float.push_back( Form("jet%d_EtaPhiQuadSpread", i) );
		myvars_float.push_back( Form("jet%d_EtaPhiQuadSpread_energy", i) );
		myvars_float.push_back( Form("jet%d_S_etaeta", i) );
		myvars_float.push_back( Form("jet%d_S_phiphi", i) );
		myvars_float.push_back( Form("jet%d_S_etaphi", i) );
		myvars_float.push_back( Form("jet%d_TDCavg", i) );
		myvars_float.push_back( Form("jet%d_TDCavg_energyWeight", i) );
		myvars_float.push_back( Form("jet%d_TDCnDelayed", i) );
		myvars_float.push_back( Form("jet%d_Track0Pt", i) );
		myvars_float.push_back( Form("jet%d_Track1Pt", i) );
		myvars_float.push_back( Form("jet%d_Track2Pt", i) );

		for (int t=0; t<3; t++) {
			myvars_float.push_back( Form("jet%d_Track%ddzToPV", i, t) );
			myvars_float.push_back( Form("jet%d_Track%ddxyToBS", i, t) );
			myvars_float.push_back( Form("jet%d_Track%ddzOverErr", i, t) );
			myvars_float.push_back( Form("jet%d_Track%ddxyOverErr", i, t) );
			myvars_float.push_back( Form("jet%d_Track%ddR", i, t) );
			myvars_float.push_back( Form("jet%d_Track%ddEta", i, t) );
			myvars_float.push_back( Form("jet%d_Track%ddPhi", i, t) );
		}
		myvars_float.push_back( Form("jet%d_Tracks_dR", i) );

		for (int d=0; d<4; d++) myvars_float.push_back( Form("jet%d_EnergyFrac_Depth%d", i, d+1) );

		myvars_float.push_back(Form("jet%d_LeadingRechitE", i) );
		myvars_float.push_back(Form("jet%d_SubLeadingRechitE", i) );
		myvars_float.push_back(Form("jet%d_SSubLeadingRechitE", i) );
		myvars_float.push_back(Form("jet%d_AllRechitE", i) );
		myvars_int.push_back(Form("jet%d_LeadingRechitD", i) );
		myvars_int.push_back(Form("jet%d_SubLeadingRechitD", i) );
		myvars_int.push_back(Form("jet%d_SSubLeadingRechitD", i) );

		if (i < 2) {
			myvars_float.push_back( Form("LLP%d_Pt", i));
			myvars_float.push_back( Form("LLP%d_E", i));
			myvars_float.push_back( Form("LLP%d_Beta", i));
			myvars_float.push_back( Form("LLP%d_TravelTime", i));

			myvars_float.push_back( Form("LLP%d_DecayR", i));
			myvars_float.push_back( Form("LLP%d_DecayX", i));
			myvars_float.push_back( Form("LLP%d_DecayY", i));
			myvars_float.push_back( Form("LLP%d_DecayZ", i));
			myvars_float.push_back( Form("LLP%d_DecayD", i));
			myvars_float.push_back( Form("LLP%d_DecayT", i));
			myvars_float.push_back( Form("LLP%d_DecayCtau", i));

			myvars_float.push_back( Form("LLP%dDecay_RechitEnergy20GeV", i));
			myvars_float.push_back( Form("LLP%dDecay_RechitEnergy", i));
			myvars_float.push_back( Form("LLP%d_RechitEnergy20GeV", i));
			myvars_float.push_back( Form("LLP%d_RechitEnergy", i));

			for (int d=0; d<4; d++) myvars_float.push_back( Form("LLP%d_EnergyFrac_Depth%d", i, d+1));
			for (int d=0; d<4; d++) myvars_float.push_back( Form("LLP%d_EnergyFracLLP_Depth%d", i, d+1));
			myvars_float.push_back( Form("LLP%d_Eta", i));
			myvars_float.push_back( Form("LLP%d_Phi", i));
			myvars_float.push_back( Form("LLP%d_dR_LLP_to_b", i));
			myvars_float.push_back( Form("LLP%d_dR_b_to_b", i));
			myvars_float.push_back( Form("LLP%d_dEta_b_to_b", i));
			myvars_float.push_back( Form("LLP%d_dPhi_b_to_b", i));
		}

	}

	for( int i=0; i<4; i++ ) {
		myvars_float.push_back( Form("LLPDecay%d_Pt", i) );
		myvars_float.push_back( Form("LLPDecay%d_Eta", i) );
		myvars_float.push_back( Form("LLPDecay%d_Phi", i) );
		myvars_float.push_back( Form("LLPDecay%d_E", i) );
		myvars_float.push_back( Form("LLPDecay%d_ProdX", i) );
		myvars_float.push_back( Form("LLPDecay%d_ProdY", i) );
		myvars_float.push_back( Form("LLPDecay%d_ProdZ", i) );
		myvars_float.push_back( Form("LLPDecay%d_ProdR", i) );
		myvars_float.push_back( Form("LLPDecay%d_isTruthMatched", i) );
		myvars_float.push_back( Form("LLPDecay%d_isTruthMatched_Eta", i) );
		if (i < 2) {
			myvars_float.push_back( Form("LLP%d_isTruthMatched", i) );
			myvars_float.push_back( Form("LLP%d_isTruthMatched_Jet40", i) );
			myvars_float.push_back( Form("LLP%d_isTruthMatched_Jet60", i) );
			myvars_float.push_back( Form("LLP%d_isTruthMatched_Jet80", i) );
			myvars_float.push_back( Form("LLP%d_isTruthMatched_Jet100", i) );
			myvars_float.push_back( Form("LLP%d_isTruthMatched_Eta", i) );
			myvars_float.push_back( Form("LLP%d_isTruthMatched_Jet40Eta", i) );
			myvars_float.push_back( Form("LLP%d_isTruthMatched_Jet60Eta", i) );
			myvars_float.push_back( Form("LLP%d_isTruthMatched_Jet80Eta", i) );
			myvars_float.push_back( Form("LLP%d_isTruthMatched_Jet100Eta", i) );
		}
	}

	cout<<"Creating new trees for the following:"<<endl;
	if( treenames.size() == 0 ) cout<<"WARNING: No treenames specified!"<<endl;
	for( auto treename: treenames ){
		cout<<"  --> "<<treename<<endl;

		tree_output[treename] = new TTree( Form("%s",treename.c_str()), Form("%s",treename.c_str()) ); 

		for( auto var: myvars_bool )
			tree_output[treename]->Branch( Form("%s",var.c_str()), &tree_output_vars_bool[var] );

		for( auto var: myvars_int )
			tree_output[treename]->Branch( Form("%s",var.c_str()), &tree_output_vars_int[var] );

		for( auto var: myvars_float )
			tree_output[treename]->Branch( Form("%s",var.c_str()), &tree_output_vars_float[var] );

	}

}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::ResetOutputBranches( string treename ){ 

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::ResetOutputTrees()"<<endl;

	for( const auto &pair : tree_output_vars_bool )
		tree_output_vars_bool[pair.first] = false;
	
	for( const auto &pair : tree_output_vars_int )
		tree_output_vars_int[pair.first] = -9999;

	for( const auto &pair : tree_output_vars_float )
		tree_output_vars_float[pair.first] = -9999.9;

	for( const auto &pair : tree_output_vars_string )
		tree_output_vars_string[pair.first] = "";

}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::FillOutputTrees( string treename ){ 

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::FillOutputTrees()"<<endl;

	if ( std::find(treenames.begin(), treenames.end(), treename) == treenames.end() ) return;

	ResetOutputBranches( treename );

	tree_output_vars_int["run"] 	= runNum;
	tree_output_vars_int["lumi"] 	= lumiNum;
	tree_output_vars_int["event"] 	= eventNum;
	tree_output_vars_int["RechitN"] = n_hbheRechit;
	tree_output_vars_int["TrackN"]	= n_track;
	tree_output_vars_int["ecalRechitN"] = n_ecalRechit;

	tree_output_vars_float["met_Pt"]	= met_Pt;
	tree_output_vars_float["met_Phi"]	= met_Phi;
	tree_output_vars_float["met_SumEt"]	= met_SumEt;

	tree_output_vars_float["eventHT"]   = EventHT();

	for (int i = 0; i < HLT_Indices.size(); i++) { 
		tree_output_vars_bool[HLT_Names[i]] = HLT_Decision->at(i);
	}

	tree_output_vars_int["RechitN_1GeV"] = 0; 
	tree_output_vars_int["RechitN_5GeV"] = 0; 
	tree_output_vars_int["RechitN_10GeV"] = 0; 
	for (int i = 0; i < n_hbheRechit; i++) {
		if (hbheRechit_E->at(i) > 1) tree_output_vars_int["RechitN_1GeV"] += 1;
		if (hbheRechit_E->at(i) > 5) tree_output_vars_int["RechitN_5GeV"] += 5;
		if (hbheRechit_E->at(i) > 10) tree_output_vars_int["RechitN_10GeV"] += 10;
		tree_output_vars_int["HBHE_Rechit_auxTDC"] = hbheRechit_auxTDC->at(i);			// auxTDC is already unmasked in ntupler (v1) to give TDC in SOI
	}

	int max_l1jets = std::min(3, n_l1jet);
	for (int i = 0; i < max_l1jets; i++) {
		tree_output_vars_float[Form("l1jet%d_Pt", i)]		= l1jet_Pt->at(i);
		tree_output_vars_float[Form("l1jet%d_E", i)]		= l1jet_E->at(i);
		tree_output_vars_float[Form("l1jet%d_Eta", i)]		= l1jet_Eta->at(i);
		tree_output_vars_float[Form("l1jet%d_Phi", i)]		= l1jet_Phi->at(i);
		tree_output_vars_float[Form("l1jet%d_hwQual", i)]	= l1jet_hwQual->at(i);
	}

	// jets are already sorted in jet Pt (not jet E!). Loop over first three jets, and save quantities in the ntuples
	int max_jets = std::min(3, n_jet);
	//std::cout << " " << std::endl;
	//std::cout << "Event = " << eventNum << std::endl;
	for (int i = 0; i < max_jets; i++) {
		tree_output_vars_float[Form("jet%d_Pt", i)] 	= jet_Pt->at(i);
		tree_output_vars_float[Form("jet%d_Eta", i)] 	= jet_Eta->at(i);
		tree_output_vars_float[Form("jet%d_Phi", i)] 	= jet_Phi->at(i);
		tree_output_vars_float[Form("jet%d_E", i)] 		= jet_E->at(i);
		tree_output_vars_float[Form("jet%d_Mass", i)] 	= jet_Mass->at(i);
		tree_output_vars_float[Form("jet%d_JetArea", i)]= jet_JetArea->at(i);

		tree_output_vars_float[Form("jet%d_ChargedHadEFrac", i)] 		= jet_ChargedHadEFrac->at(i);
		tree_output_vars_float[Form("jet%d_NeutralHadEFrac", i)] 		= jet_NeutralHadEFrac->at(i);
		tree_output_vars_float[Form("jet%d_PhoEFrac", i)] 				= jet_PhoEFrac->at(i);
		tree_output_vars_float[Form("jet%d_EleFrac", i)] 				= jet_EleEFrac->at(i);
		tree_output_vars_float[Form("jet%d_MuonEFrac", i)] 				= jet_MuonEFrac->at(i);
		tree_output_vars_float[Form("jet%d_HoverE", i)] 				= (jet_ChargedHadEFrac->at(i)) / (jet_NeutralHadEFrac->at(i) + jet_PhoEFrac->at(i) + jet_EleEFrac->at(i));

		tree_output_vars_int[Form("jet%d_ChargedHadMult", i)] 			= jet_ChargedHadMult->at(i);
		tree_output_vars_int[Form("jet%d_NeutralHadMult", i)] 			= jet_NeutralHadMult->at(i);
		tree_output_vars_int[Form("jet%d_PhoMult", i)] 					= jet_PhoMult->at(i);
		tree_output_vars_int[Form("jet%d_EleMult", i)] 					= jet_EleMult->at(i);
		tree_output_vars_int[Form("jet%d_MuonMult", i)] 				= jet_MuonMult->at(i);

		tree_output_vars_float[Form("jet%d_PtAllTracks", i)] 			= jet_PtAllTracks->at(i);
		tree_output_vars_float[Form("jet%d_PtAllPVTracks", i)] 			= jet_PtAllPVTracks->at(i);
		tree_output_vars_int[Form("jet%d_NVertexTracks", i)] 			= jet_NVertexTracks->at(i);
		tree_output_vars_int[Form("jet%d_NSelectedTracks", i)] 			= jet_NSelectedTracks->at(i);
		tree_output_vars_int[Form("jet%d_NSV", i)] 						= jet_NSV->at(i);
		tree_output_vars_int[Form("jet%d_NSVCand", i)] 					= jet_NSVCand->at(i);
		tree_output_vars_float[Form("jet%d_SV_x", i)] 					= jet_SV_x->at(i);
		tree_output_vars_float[Form("jet%d_SV_y", i)] 					= jet_SV_y->at(i);
		tree_output_vars_float[Form("jet%d_SV_z", i)] 					= jet_SV_z->at(i);
		tree_output_vars_int[Form("jet%d_SV_NTracks", i)] 				= jet_SV_NTracks->at(i);
		tree_output_vars_float[Form("jet%d_SV_Mass", i)] 				= jet_SV_Mass->at(i);
		tree_output_vars_float[Form("jet%d_SV_DRJet", i)] 				= jet_SV_DRJet->at(i);
		tree_output_vars_float[Form("jet%d_FlightDist2D", i)] 			= jet_FlightDist2D->at(i);
		tree_output_vars_float[Form("jet%d_FlightDist2DErr", i)] 		= jet_FlightDist2DErr->at(i);
		tree_output_vars_float[Form("jet%d_FlightDist3D", i)] 			= jet_FlightDist3D->at(i);
		tree_output_vars_float[Form("jet%d_FlightDist3DErr", i)] 		= jet_FlightDist3DErr->at(i);

		tree_output_vars_float[Form("jet%d_isTruthMatched", i)] = 0;
		vector<float> matchedInfo = JetIsMatchedTo( jet_Eta->at(i), jet_Phi->at(i) );
		float matchedLLP = matchedInfo[0];
		float matchedDR = matchedInfo[1];
		if (matchedLLP > -1) { // if jet is matched to a LLP or LLP decay product
			tree_output_vars_float[Form("jet%d_isTruthMatched", i)] = 1;
			tree_output_vars_float[Form("jet%d_isMatchedTo",i)] = matchedLLP;
			tree_output_vars_float[Form("jet%d_isMatchedWithDR",i)] = matchedDR;
			tree_output_vars_float[Form("jet%d_MatchedLLP_DecayR",i)] = gLLP_DecayVtx_R.at(matchedLLP); // what is the decay R for the LLP matched to this jet?
			tree_output_vars_float[Form("jet%d_MatchedLLP_Eta",i)] = gLLP_Eta->at(matchedLLP); // what is the decay eta for the LLP matched to this jet?
		}

		vector<float> rechitJet = GetMatchedHcalRechits_Jet(i, 0.4);
		vector<float> energy = GetEnergyProfile_Jet(i, 0.4);
		vector<float> spread_Eta_Phi = GetEtaPhiSpread_Jet(i, 0.4); // eta, phi (average); eta, phi (energy weighted); S_ee, S_pp, S_ep
		vector<pair<float,int>> energeticRechits = Get3RechitE_Jet(i, 0.4); // three highest rechit energies in the , and total energy
		vector<float> TDC_TDCenergy = GetTDCavg_Jet(i, 0.4); // TDC average, energy weighted TDC

		for (int depth = 0; depth < 4; depth++) tree_output_vars_float[Form("jet%d_EnergyFrac_Depth%d", i, depth+1)] = energy[depth]; // each fractional energy saved in different tree
		tree_output_vars_float[Form("jet%d_LeadingRechitE", i)] 			= energeticRechits[0].first;
		tree_output_vars_float[Form("jet%d_SubLeadingRechitE", i)] 			= energeticRechits[1].first;
		tree_output_vars_float[Form("jet%d_SSubLeadingRechitE", i)] 		= energeticRechits[2].first;
		tree_output_vars_float[Form("jet%d_AllRechitE", i)] 				= energeticRechits[3].first;
		tree_output_vars_int[Form("jet%d_LeadingRechitD", i)] 				= energeticRechits[0].second;
		tree_output_vars_int[Form("jet%d_SubLeadingRechitD", i)] 			= energeticRechits[1].second;
		tree_output_vars_int[Form("jet%d_SSubLeadingRechitD", i)] 			= energeticRechits[2].second;
		tree_output_vars_int[Form("jet%d_RechitN", i)] 						= rechitJet.size();
		tree_output_vars_float[Form("jet%d_EtaSpread", i)] 					= spread_Eta_Phi[0];
		tree_output_vars_float[Form("jet%d_EtaSpread_energy", i)] 			= spread_Eta_Phi[2];
		tree_output_vars_float[Form("jet%d_PhiSpread", i)] 					= spread_Eta_Phi[1];
		tree_output_vars_float[Form("jet%d_PhiSpread_energy", i)] 			= spread_Eta_Phi[3];
		tree_output_vars_float[Form("jet%d_EtaPhiQuadSpread", i)] 			= sqrt(spread_Eta_Phi[0] * spread_Eta_Phi[0] + spread_Eta_Phi[1] * spread_Eta_Phi[1]);
		tree_output_vars_float[Form("jet%d_EtaPhiQuadSpread_energy", i)] 	= sqrt(spread_Eta_Phi[2] * spread_Eta_Phi[2] + spread_Eta_Phi[3] * spread_Eta_Phi[3]);
		tree_output_vars_float[Form("jet%d_S_etaeta", i)]					= spread_Eta_Phi[4];
		tree_output_vars_float[Form("jet%d_S_phiphi", i)]					= spread_Eta_Phi[5];
		tree_output_vars_float[Form("jet%d_S_etaphi", i)]					= spread_Eta_Phi[6];

		tree_output_vars_float[Form("jet%d_TDCavg", i)] = TDC_TDCenergy[0];
		tree_output_vars_float[Form("jet%d_TDCavg_energyWeight", i)] = TDC_TDCenergy[1];
		tree_output_vars_float[Form("jet%d_TDCnDelayed", i)] = TDC_TDCenergy[2];
		
		// find three highest pT tracks matched to a jet, and save the generalTrack index for use later (in dzToPV and dzyToBS)
		vector<pair<float, float>> track_pt_index;
		vector<uint> jet_track_index = jet_TrackIndices->at(i);
		for (int j = 0; j < jet_track_index.size(); j++) { // jet_NTracks->at(i) == jet_track_index.size()
			for (int k = 0; k < n_track; k++) { // find which generalTrack matches to jet_track_index[j]
				if (jet_track_index[j] == track_index->at(k)) {
					track_pt_index.push_back({track_Pt->at(k), k});
					// std::cout << track_pt_index[j].first << ", " << track_pt_index[j].second << " before sorting, and dzToPV = " << track_dzToPV->at(k) << std::endl;
				}
			} 
		}	
		if (track_pt_index.size() > 0) {
			std::sort (track_pt_index.begin(), track_pt_index.end(), greater<pair<float, float>>()); // sort to find highest pt tracks
			int n_track = std::min(3, (int) jet_track_index.size());
			for (int track = 0; track < n_track; track++) {
				tree_output_vars_float[Form("jet%d_Track%dPt", i, track)] 			= track_pt_index[track].first;
				int track_num = track_pt_index[track].second;
				tree_output_vars_float[Form("jet%d_Track%ddzToPV", i, track)] 		= track_dzToPV->at(track_num); 
				tree_output_vars_float[Form("jet%d_Track%ddxyToBS", i, track)] 		= track_dxyToBS->at(track_num); 
				tree_output_vars_float[Form("jet%d_Track%ddzOverErr", i, track)]	= track_dzToPV->at(track_num) / track_dzErr->at(track_num); 
				tree_output_vars_float[Form("jet%d_Track%ddxyOverErr", i, track)] 	= track_dxyToBS->at(track_num) / track_dxyErr->at(track_num); 
				tree_output_vars_float[Form("jet%d_Track%ddR", i, track)] 			= DeltaR( jet_Eta->at(i), track_Eta->at(track_num), jet_Phi->at(i), track_Phi->at(track_num) ); 
				tree_output_vars_float[Form("jet%d_Track%ddEta", i, track)] 		= fabs(jet_Eta->at(i) - track_Eta->at(track_num));
				tree_output_vars_float[Form("jet%d_Track%ddPhi", i, track)] 		= fabs(deltaPhi( jet_Phi->at(i), track_Phi->at(track_num) )); 
				if (track == 1) {													// dR for two leading tracks
					tree_output_vars_float[Form("jet%d_Tracks_dR", i)] 				= DeltaR( track_Eta->at(track_pt_index[0].second), track_Eta->at(track_num), track_Phi->at(track_pt_index[0].second), track_Phi->at(track_num) );
				}
			}
		} // end of track matching 
	}

	for (int i = 0; i < n_gLLP; i++) {
		float decay_radius = gLLP_DecayVtx_R.at(i); // GetDecayRadiusHB_LLP(i); // -999 default value
		float distance = gLLP_DecayVtx_Mag.at(i); //GetDecayDistance_LLP(i);
		vector<int> n_rechit_pt4 = GetRechitMult(i, 0.4); // GetRechitMult returns rechit multiplicity associated with LLP [0], first daughter [1], second daughter [2]
		vector<vector<float>> energy = GetEnergyProfile(i, 0.4); // [0] is LLP, [1] is daughter 1, [2] is daughter 2, [3] is LLP only (not with decay products considered). [4] is total energies (LLP, daughter1, daughter2, LLP no decay prods)

		vector<TVector3> decay_product_coords = GetLLPDecayProdCoords(i,0,vector<float>{decay_radius});  // 0 for daughter particle, just getting decay of LLP, since R is LLP decay radius
		if (decay_product_coords.size() > 0) {
			float eta = decay_product_coords.at(0).Eta();
			float phi = decay_product_coords.at(0).Phi();
			tree_output_vars_float[Form("LLP%d_Eta", i)] = eta;
			tree_output_vars_float[Form("LLP%d_Phi", i)] = phi;
		}

		tree_output_vars_int[Form("LLP%d_RechitN", i)] = n_rechit_pt4[0];
		tree_output_vars_float[Form("LLP%d_DecayR", i)] = decay_radius;

		tree_output_vars_float[Form("LLP%d_Pt", i)] = gLLP_Pt->at(i);
		tree_output_vars_float[Form("LLP%d_E", i)] = gLLP_E->at(i);
		tree_output_vars_float[Form("LLP%d_Beta", i)] = gLLP_Beta->at(i);
		tree_output_vars_float[Form("LLP%d_TravelTime", i)] = gLLP_TravelTime->at(i);
		tree_output_vars_float[Form("LLP%d_DecayX", i)] = gLLP_DecayVtx_X->at(i);
		tree_output_vars_float[Form("LLP%d_DecayY", i)] = gLLP_DecayVtx_Y->at(i);
		tree_output_vars_float[Form("LLP%d_DecayZ", i)] = gLLP_DecayVtx_Z->at(i);
		tree_output_vars_float[Form("LLP%d_DecayD", i)] = distance;
		tree_output_vars_float[Form("LLP%d_DecayT", i)] = distance * ( 1 / gLLP_Beta->at(i) - 1) * 0.03336; // 1/c in ns / cm to give answer in ns
		tree_output_vars_float[Form("LLP%d_DecayCtau", i)] = distance * (sqrt( 1 / pow(gLLP_Beta->at(i),2) - 1)); 
		for (int b = 0; b < 2; b++) tree_output_vars_float[Form("LLP%d_dR_LLP_to_b", i)] = DeltaR_LLP_b(i, b); 
		tree_output_vars_float[Form("LLP%d_dR_b_to_b", i)] = DeltaR_b(i);
		tree_output_vars_float[Form("LLP%d_dEta_b_to_b", i)] = DeltaEta_Phi_b(i)[0];
		tree_output_vars_float[Form("LLP%d_dPhi_b_to_b", i)] = DeltaEta_Phi_b(i)[1];

		if (energy[0][0] + energy[0][1] + energy[0][2] + energy[0][3] > 0) { // ensure there is positive energy in one depth
			for (int depth = 0; depth < 4; depth++) tree_output_vars_float[Form("LLP%d_EnergyFrac_Depth%d", i, depth+1)] = energy[0][depth]; // LLP from b matching. each fractional energy saved in different tree
		}
		if (energy[3][0] + energy[3][1] + energy[3][2] + energy[3][3] > 0) { // ensure there is positive energy in one depth
			for (int depth = 0; depth < 4; depth++) tree_output_vars_float[Form("LLP%d_EnergyFracLLP_Depth%d", i, depth+1)] = energy[3][depth]; // LLP only matching. each fractional energy saved in different tree
		}

		if (energy[4][0] > 20) tree_output_vars_float[Form("LLP%dDecay_RechitEnergy20GeV", i)] = 1;
		tree_output_vars_float[Form("LLP%dDecay_RechitEnergy", i)] = energy[4][0];
		if (energy[4][3] > 20) tree_output_vars_float[Form("LLP%d_RechitEnergy20GeV", i)] = 1;
		tree_output_vars_float[Form("LLP%d_RechitEnergy", i)] = energy[4][3];
	}

	for (int i = 0; i < gLLPDecay_iParticle.size(); i++) { // size is 4
		int idx_gParticle = gLLPDecay_iParticle.at(i);
		tree_output_vars_float[Form("LLPDecay%d_Pt", i)]    = gParticle_Pt->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_Eta", i)]   = gParticle_Eta->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_Phi", i)]   = gParticle_Phi->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_E", i)]   	= gParticle_E->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_ProdX", i)] = gParticle_ProdVtx_X->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_ProdY", i)] = gParticle_ProdVtx_Y->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_ProdZ", i)] = gParticle_ProdVtx_Z->at(idx_gParticle);
		tree_output_vars_float[Form("LLPDecay%d_ProdR", i)] = pow( pow(gParticle_ProdVtx_X->at(idx_gParticle), 2.) + pow(gParticle_ProdVtx_Y->at(idx_gParticle), 2.), 0.5 );
		tree_output_vars_float[Form("LLPDecay%d_isTruthMatched", i)] = LLPIsTruthMatched( i ).first;
		tree_output_vars_float[Form("LLPDecay%d_isTruthMatched_Eta", i)] = LLPIsTruthMatched( i ).second;

		tree_output_vars_float[Form("LLP%d_isTruthMatched", gLLPDecay_iLLP.at(i))] = LLPIsTruthMatched( i ).first; // gLLPDecay_iLLP.at( i ) tells which LLP this comes from, and gLLPDecay_iLLP.at(i) details correspond to i in n_gLLP loop above
		tree_output_vars_float[Form("LLP%d_isTruthMatched_Eta", gLLPDecay_iLLP.at(i))] = LLPIsTruthMatched( i ).second;
		tree_output_vars_float[Form("LLP%d_isTruthMatched_Jet40", gLLPDecay_iLLP.at(i))] = LLPIsTruthMatched( i, 40 ).first; 
		tree_output_vars_float[Form("LLP%d_isTruthMatched_Jet40Eta", gLLPDecay_iLLP.at(i))] = LLPIsTruthMatched( i, 40 ).second;
		tree_output_vars_float[Form("LLP%d_isTruthMatched_Jet60", gLLPDecay_iLLP.at(i))] = LLPIsTruthMatched( i, 60 ).first; 
		tree_output_vars_float[Form("LLP%d_isTruthMatched_Jet60Eta", gLLPDecay_iLLP.at(i))] = LLPIsTruthMatched( i, 60 ).second;
		tree_output_vars_float[Form("LLP%d_isTruthMatched_Jet80", gLLPDecay_iLLP.at(i))] = LLPIsTruthMatched( i, 80 ).first; 
		tree_output_vars_float[Form("LLP%d_isTruthMatched_Jet80Eta", gLLPDecay_iLLP.at(i))] = LLPIsTruthMatched( i, 80 ).second;
		tree_output_vars_float[Form("LLP%d_isTruthMatched_Jet100", gLLPDecay_iLLP.at(i))] = LLPIsTruthMatched( i, 100 ).first; 
		tree_output_vars_float[Form("LLP%d_isTruthMatched_Jet100Eta", gLLPDecay_iLLP.at(i))] = LLPIsTruthMatched( i, 100 ).second;
	}

	if( EventValidForBDTEval() ){
		for( auto bdt_tag: bdt_tags ) tree_output_vars_float[Form("bdtscore_%s", bdt_tag.c_str())] = GetBDTScores(bdt_tag);
	} 

	tree_output[treename]->Fill();
	
	if( debug ) cout<<"DONE DisplacedHcalJetAnalyzer::FillOutputTrees()"<<endl;

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
