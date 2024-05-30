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

	vector<string> myvars_bool = {};
	
	for (int i = 0; i < HLT_Indices.size(); i++) {
		myvars_bool.push_back(HLT_Names[i]);
	}

	// Add Event Variables //
	vector<string> myvars_int = {
		"run","lumi","event","jet","validJet","muon","ele","pho",
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
	MyTags event_based = MyTags(/*event_based=*/ true, /*calor_only=*/ false);
	for( auto bdt_tag: event_based.bdt_tags() ){
		myvars_float.push_back( Form("bdtscore_%s", bdt_tag.c_str()) );
	}

	// Add Physics Variables //

	for( int i=0; i<6; i++ ) {
		if (i < 3) {
			myvars_float.push_back( Form("l1jet%d_Pt", i) );
			myvars_float.push_back( Form("l1jet%d_E", i) );
			myvars_float.push_back( Form("l1jet%d_Eta", i) );
			myvars_float.push_back( Form("l1jet%d_Phi", i) );
			myvars_float.push_back( Form("l1jet%d_hwQual", i) );
		}

		myvars_float.push_back( Form("jet%d_Pt", i) );
		myvars_float.push_back( Form("jet%d_Eta", i) );
		myvars_float.push_back( Form("jet%d_Phi", i) );
		myvars_float.push_back( Form("jet%d_E", i) );
		myvars_float.push_back( Form("jet%d_Mass", i) );
		myvars_float.push_back( Form("jet%d_JetArea", i) );

		myvars_int.push_back( Form("jet%d_L1trig_Matched", i) );
		// myvars_int.push_back( Form("jet%d_Index", i) );
		myvars_float.push_back( Form("jet%d_dR_L1jet", i) );

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
		myvars_float.push_back( Form("jet%d_Timeavg", i) );

		for (int t=0; t<3; t++) {
			myvars_float.push_back( Form("jet%d_Track%dPt", i, t) );
			myvars_float.push_back( Form("jet%d_Track%ddzToPV", i, t) );
			myvars_float.push_back( Form("jet%d_Track%ddxyToBS", i, t) );
			myvars_float.push_back( Form("jet%d_Track%ddzOverErr", i, t) );
			myvars_float.push_back( Form("jet%d_Track%ddxyOverErr", i, t) );
			myvars_float.push_back( Form("jet%d_Track%dnMissingInnerHits", i, t) );
			myvars_float.push_back( Form("jet%d_Track%dnMissingOuterHits", i, t) );
			myvars_float.push_back( Form("jet%d_Track%dnPixelHits", i, t) );
			myvars_float.push_back( Form("jet%d_Track%dnHits", i, t) );
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
void DisplacedHcalJetAnalyzer::DeclareOutputJetTrees(){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::DeclareOutputJetTrees()"<<endl;

	if( !save_trees ){
		cout<<"  NOTE: 'save_trees' is set to false. Will not run over ANY tree categories..."<<endl;
		jet_treenames = {};
		return;
	}

	cout<<"Declaring Output Trees..."<<endl;	

	jet_treenames = { "PerJet_NoSel", "PerJet_WPlusJets", "PerJet_LLPmatched" }; 

	vector<string> myvars_bool = {};
	for (int i = 0; i < HLT_Indices.size(); i++) {
		myvars_bool.push_back(HLT_Names[i]);
	}

	// Add Event Variables //
	vector<string> myvars_int = {
		"run","lumi","event","jet","muon","ele","pho",
		"jetIndex"
	};	

	vector<string> myvars_float = {"eventHT"};

	// Add BDT Scores //
	MyTags jet_based = MyTags(/*event_based=*/ false, /*calor_only=*/ false);
	for( auto bdt_tag: jet_based.bdt_tags() ){
		myvars_float.push_back( Form("bdtscore_%s", bdt_tag.c_str()) );
	}
	MyTags jet_based_calor = MyTags(/*event_based=*/ false, /*calor_only=*/ true);
	for( auto bdt_tag: jet_based_calor.bdt_tags() ){
		myvars_float.push_back( Form("bdtscore_%s", bdt_tag.c_str()) );
	}

	// Add Physics Variables //
	// consider adding L1 jet information
	myvars_float.push_back("perJet_E");
	myvars_float.push_back("perJet_Pt");
	myvars_float.push_back("perJet_Eta");
	myvars_float.push_back("perJet_Phi");
	myvars_float.push_back("perJet_Mass");
	myvars_float.push_back("perJet_Area");
	myvars_float.push_back("perJet_ChargedHadEFrac");
	myvars_float.push_back("perJet_NeutralHadEFrac");
	myvars_float.push_back("perJet_PhoEFrac");
	myvars_float.push_back("perJet_EleEFrac");
	myvars_float.push_back("perJet_MuonEFrac");

	myvars_float.push_back("perJet_PtAllTracks");
	myvars_float.push_back("perJet_PtAllPVTracks");

	myvars_float.push_back("perJet_isTruthMatched");
	myvars_float.push_back("perJet_isMatchedTo");
	myvars_float.push_back("perJet_MatchedLLP_DecayZ");
	myvars_float.push_back("perJet_MatchedLLP_DecayR");
	myvars_float.push_back("perJet_MatchedLLP_TravelTime");
	myvars_float.push_back("perJet_OtherLLP_DecayR");
	myvars_float.push_back("perJet_MatchedLLP_Eta");

	myvars_float.push_back("perJet_S_phiphi");
	myvars_float.push_back("perJet_S_etaeta");
	myvars_float.push_back("perJet_S_etaphi");

	myvars_float.push_back("perJet_Tracks_dR");

	for (int t=0; t<3; t++) {
		myvars_float.push_back( Form("perJet_Track%dPt", t) );
		myvars_float.push_back( Form("perJet_Track%ddR", t) );
		myvars_float.push_back( Form("perJet_Track%ddEta", t) );
		myvars_float.push_back( Form("perJet_Track%ddPhi", t) );

		myvars_float.push_back( Form("perJet_Track%ddzToPV", t) );
		myvars_float.push_back( Form("perJet_Track%ddxyToBS", t) );
		myvars_float.push_back( Form("perJet_Track%ddzOverErr", t) );
		myvars_float.push_back( Form("perJet_Track%ddxyOverErr", t) );
	}

	for (int d=0; d<4; d++) myvars_float.push_back( Form("perJet_EnergyFrac_Depth%d", d+1) );

	myvars_float.push_back("perJet_LeadingRechitE" );
	myvars_float.push_back("perJet_SubLeadingRechitE" );
	myvars_float.push_back("perJet_SSubLeadingRechitE" );
	myvars_float.push_back("perJet_AllRechitE" );
	myvars_int.push_back("perJet_LeadingRechitD" );

	cout<<"Creating new trees for the following:"<<endl;
	if( jet_treenames.size() == 0 ) cout<<"WARNING: No jet treenames specified!"<<endl;
	for( auto treename: jet_treenames ){
		cout<<"  --> "<<treename<<endl;

		jet_tree_output[treename] = new TTree( Form("%s",treename.c_str()), Form("%s",treename.c_str()) ); 

		for( auto var: myvars_bool )
			jet_tree_output[treename]->Branch( Form("%s",var.c_str()), &jet_tree_output_vars_bool[var] );

		for( auto var: myvars_int )
			jet_tree_output[treename]->Branch( Form("%s",var.c_str()), &jet_tree_output_vars_int[var] );

		for( auto var: myvars_float )
			jet_tree_output[treename]->Branch( Form("%s",var.c_str()), &jet_tree_output_vars_float[var] );
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

	for( const auto &pair : jet_tree_output_vars_bool )
		jet_tree_output_vars_bool[pair.first] = false;
	
	for( const auto &pair : jet_tree_output_vars_int )
		jet_tree_output_vars_int[pair.first] = -9999;

	for( const auto &pair : jet_tree_output_vars_float )
		jet_tree_output_vars_float[pair.first] = -9999.9;

	for( const auto &pair : jet_tree_output_vars_string )
		jet_tree_output_vars_string[pair.first] = "";

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
void DisplacedHcalJetAnalyzer::FillOutputTrees( string treename ){ 
	// for the output trees that are filled on a per event basis

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::FillOutputTrees()"<<endl;

	if ( std::find(treenames.begin(), treenames.end(), treename) == treenames.end() ) return;

	ResetOutputBranches( treename );

	tree_output_vars_int["run"] 	= runNum;
	tree_output_vars_int["lumi"] 	= lumiNum;
	tree_output_vars_int["event"] 	= eventNum;
	tree_output_vars_int["jet"]		= n_jet;
	tree_output_vars_int["ele"]		= n_ele;
	tree_output_vars_int["muon"]	= n_muon;
	tree_output_vars_int["pho"]		= n_pho;
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
		if (hbheRechit_auxTDC->at(i) <= 2) tree_output_vars_int["HBHE_Rechit_auxTDC"] += hbheRechit_auxTDC->at(i);			// auxTDC is already unmasked in ntupler (v1) to give TDC in SOI
	}

	int max_l1jets = std::min(3, n_l1jet);
	for (int i = 0; i < max_l1jets; i++) {
		tree_output_vars_float[Form("l1jet%d_Pt", i)]		= l1jet_Pt->at(i);
		tree_output_vars_float[Form("l1jet%d_E", i)]		= l1jet_E->at(i);
		tree_output_vars_float[Form("l1jet%d_Eta", i)]		= l1jet_Eta->at(i);
		tree_output_vars_float[Form("l1jet%d_Phi", i)]		= l1jet_Phi->at(i);
		tree_output_vars_float[Form("l1jet%d_hwQual", i)]	= l1jet_hwQual->at(i);
	}

	// jets are already sorted in jet Pt (not jet E!). Loop over first three reco jets, and save quantities in the ntuples
	int valid_jet = 0;
	for (int i = 0; i < n_jet; i++) {
		if (jet_Pt->at(i) < 40 || abs(jet_Eta->at(i)) > 1.26) continue; // not interested if low energy (< 40) or in HE (eta > 1.26) 
		if (valid_jet >= 6) continue; // below output variables are only designed to be used for first 12 jets

		tree_output_vars_int[Form("jet%d_Index", valid_jet)]	= i; // needed if only filling for valid_jet
		
		float dR = 999.9;
		float L1trig = -999.9;
		for (int j = 0; j < max_l1jets; j++) { // loop over L1 jets to determine if a reco jet is matched to jet that passed L1
			float dR_to_L1 = DeltaR( jet_Eta->at(i), l1jet_Eta->at(j), jet_Phi->at(i), l1jet_Phi->at(j) );
			if (dR_to_L1 < dR) { // if matched, save the dR and whether the L1 jet is triggered by HCAL LLP
				dR = dR_to_L1;
				L1trig = l1jet_hwQual->at(j);
			}
		}
		tree_output_vars_float[Form("jet%d_dR_L1jet", valid_jet)] = dR;
		if (dR < 0.4) tree_output_vars_int[Form("jet%d_L1trig_Matched", valid_jet)] = L1trig;
		tree_output_vars_float[Form("jet%d_Pt", valid_jet)] 	= jet_Pt->at(i);
		tree_output_vars_float[Form("jet%d_Eta", valid_jet)] 	= jet_Eta->at(i);
		tree_output_vars_float[Form("jet%d_Phi", valid_jet)] 	= jet_Phi->at(i);
		tree_output_vars_float[Form("jet%d_E", valid_jet)] 		= jet_E->at(i);
		tree_output_vars_float[Form("jet%d_Mass", valid_jet)] 	= jet_Mass->at(i);
		tree_output_vars_float[Form("jet%d_JetArea", valid_jet)]= jet_JetArea->at(i);

		tree_output_vars_float[Form("jet%d_ChargedHadEFrac", valid_jet)] 		= jet_ChargedHadEFrac->at(i);
		tree_output_vars_float[Form("jet%d_NeutralHadEFrac", valid_jet)] 		= jet_NeutralHadEFrac->at(i);
		tree_output_vars_float[Form("jet%d_PhoEFrac", valid_jet)] 				= jet_PhoEFrac->at(i);
		tree_output_vars_float[Form("jet%d_EleEFrac", valid_jet)] 				= jet_EleEFrac->at(i);
		tree_output_vars_float[Form("jet%d_MuonEFrac", valid_jet)] 				= jet_MuonEFrac->at(i);
		tree_output_vars_float[Form("jet%d_HoverE", valid_jet)] 				= (jet_ChargedHadEFrac->at(i)) / (jet_NeutralHadEFrac->at(i) + jet_PhoEFrac->at(i) + jet_EleEFrac->at(i));

		tree_output_vars_int[Form("jet%d_ChargedHadMult", valid_jet)] 			= jet_ChargedHadMult->at(i);
		tree_output_vars_int[Form("jet%d_NeutralHadMult", valid_jet)] 			= jet_NeutralHadMult->at(i);
		tree_output_vars_int[Form("jet%d_PhoMult", valid_jet)] 					= jet_PhoMult->at(i);
		tree_output_vars_int[Form("jet%d_EleMult", valid_jet)] 					= jet_EleMult->at(i);
		tree_output_vars_int[Form("jet%d_MuonMult", valid_jet)] 				= jet_MuonMult->at(i);

		tree_output_vars_float[Form("jet%d_PtAllTracks", valid_jet)] 			= jet_PtAllTracks->at(i);
		tree_output_vars_float[Form("jet%d_PtAllPVTracks", valid_jet)] 			= jet_PtAllPVTracks->at(i);
		tree_output_vars_int[Form("jet%d_NVertexTracks", valid_jet)] 			= jet_NVertexTracks->at(i);
		tree_output_vars_int[Form("jet%d_NSelectedTracks", valid_jet)] 			= jet_NSelectedTracks->at(i);
		tree_output_vars_int[Form("jet%d_NSV", valid_jet)] 						= jet_NSV->at(i);
		tree_output_vars_int[Form("jet%d_NSVCand", valid_jet)] 					= jet_NSVCand->at(i);
		tree_output_vars_float[Form("jet%d_SV_x", valid_jet)] 					= jet_SV_x->at(i);
		tree_output_vars_float[Form("jet%d_SV_y", valid_jet)] 					= jet_SV_y->at(i);
		tree_output_vars_float[Form("jet%d_SV_z", valid_jet)] 					= jet_SV_z->at(i);
		tree_output_vars_int[Form("jet%d_SV_NTracks", valid_jet)] 				= jet_SV_NTracks->at(i);
		tree_output_vars_float[Form("jet%d_SV_Mass", valid_jet)] 				= jet_SV_Mass->at(i);
		tree_output_vars_float[Form("jet%d_SV_DRJet", valid_jet)] 				= jet_SV_DRJet->at(i);
		tree_output_vars_float[Form("jet%d_FlightDist2D", valid_jet)] 			= jet_FlightDist2D->at(i);
		tree_output_vars_float[Form("jet%d_FlightDist2DErr", valid_jet)] 		= jet_FlightDist2DErr->at(i);
		tree_output_vars_float[Form("jet%d_FlightDist3D", valid_jet)] 			= jet_FlightDist3D->at(i);
		tree_output_vars_float[Form("jet%d_FlightDist3DErr", valid_jet)] 		= jet_FlightDist3DErr->at(i);

		tree_output_vars_float[Form("jet%d_isTruthMatched", valid_jet)] = 0;
		vector<float> matchedInfo = JetIsMatchedTo( jet_Eta->at(i), jet_Phi->at(i) );
		float matchedLLP = matchedInfo[0];
		float matchedDR = matchedInfo[1];
		if (matchedLLP > -1) { // if jet is matched to a LLP or LLP decay product
			tree_output_vars_float[Form("jet%d_isTruthMatched", valid_jet)] = 1;
			tree_output_vars_float[Form("jet%d_isMatchedTo", valid_jet)] = matchedLLP;
			tree_output_vars_float[Form("jet%d_isMatchedWithDR", valid_jet)] = matchedDR;
			tree_output_vars_float[Form("jet%d_MatchedLLP_DecayR", valid_jet)] = gLLP_DecayVtx_R.at(matchedLLP); // what is the decay R for the LLP matched to this jet?
			tree_output_vars_float[Form("jet%d_MatchedLLP_Eta", valid_jet)] = gLLP_Eta->at(matchedLLP); // what is the decay eta for the LLP matched to this jet?
		}

		vector<float> rechitJet = GetMatchedHcalRechits_Jet(i, 0.4);
		vector<float> energy = GetEnergyProfile_Jet(i, 0.4);
		vector<float> spread_Eta_Phi = GetEtaPhiSpread_Jet(i, 0.4); // eta, phi (average); eta, phi (energy weighted); S_ee, S_pp, S_ep
		vector<pair<float,int>> energeticRechits = Get3RechitE_Jet(i, 0.4); // three highest rechit energies in the , and total energy
		vector<float> TDC_TDCenergy = GetTDCavg_Jet(i, 0.4); // TDC average, energy weighted TDC

		for (int depth = 0; depth < 4; depth++) tree_output_vars_float[Form("jet%d_EnergyFrac_Depth%d", valid_jet, depth+1)] = energy[depth]; // each fractional energy saved in different tree
		tree_output_vars_float[Form("jet%d_LeadingRechitE", valid_jet)] 			= energeticRechits[0].first;
		tree_output_vars_float[Form("jet%d_SubLeadingRechitE", valid_jet)] 			= energeticRechits[1].first;
		tree_output_vars_float[Form("jet%d_SSubLeadingRechitE", valid_jet)] 		= energeticRechits[2].first;
		tree_output_vars_float[Form("jet%d_AllRechitE", valid_jet)] 				= energeticRechits[3].first;
		tree_output_vars_int[Form("jet%d_LeadingRechitD", valid_jet)] 				= energeticRechits[0].second;
		tree_output_vars_int[Form("jet%d_SubLeadingRechitD", valid_jet)] 			= energeticRechits[1].second;
		tree_output_vars_int[Form("jet%d_SSubLeadingRechitD", valid_jet)] 			= energeticRechits[2].second;
		tree_output_vars_int[Form("jet%d_RechitN", valid_jet)] 						= rechitJet.size();
		tree_output_vars_float[Form("jet%d_EtaSpread", valid_jet)] 					= spread_Eta_Phi[0];
		tree_output_vars_float[Form("jet%d_EtaSpread_energy", valid_jet)] 			= spread_Eta_Phi[2];
		tree_output_vars_float[Form("jet%d_PhiSpread", valid_jet)] 					= spread_Eta_Phi[1];
		tree_output_vars_float[Form("jet%d_PhiSpread_energy", valid_jet)] 			= spread_Eta_Phi[3];
		tree_output_vars_float[Form("jet%d_EtaPhiQuadSpread", valid_jet)] 			= sqrt(spread_Eta_Phi[0] * spread_Eta_Phi[0] + spread_Eta_Phi[1] * spread_Eta_Phi[1]);
		tree_output_vars_float[Form("jet%d_EtaPhiQuadSpread_energy", valid_jet)] 	= sqrt(spread_Eta_Phi[2] * spread_Eta_Phi[2] + spread_Eta_Phi[3] * spread_Eta_Phi[3]);
		tree_output_vars_float[Form("jet%d_S_etaeta", valid_jet)]					= spread_Eta_Phi[4];
		tree_output_vars_float[Form("jet%d_S_phiphi", valid_jet)]					= spread_Eta_Phi[5];
		tree_output_vars_float[Form("jet%d_S_etaphi", valid_jet)]					= spread_Eta_Phi[6];

		tree_output_vars_float[Form("jet%d_TDCavg", valid_jet)] = TDC_TDCenergy[0];
		tree_output_vars_float[Form("jet%d_TDCavg_energyWeight", valid_jet)] = TDC_TDCenergy[1];
		tree_output_vars_float[Form("jet%d_TDCnDelayed", valid_jet)] = TDC_TDCenergy[2];
		tree_output_vars_float[Form("jet%d_Timeavg", valid_jet)] = TDC_TDCenergy[3];
		
		// find three highest pT tracks matched to a jet, and save the generalTrack index for use later (in dzToPV and dzyToBS)
		vector<uint> jet_track_index = jet_TrackIndices->at(i);
		vector<pair<float, float>> track_pt_index = TrackMatcher(i, jet_track_index);

		for (int track = 0; track < 3; track++) {
			tree_output_vars_float[Form("jet%d_Track%dPt", valid_jet, track)] = 0; // default value for track pT (in case no tracks are matched)
			tree_output_vars_float[Form("jet%d_Track%ddR", valid_jet, track)] = 0.5; // default value for track dR (in case no tracks are matched)
			tree_output_vars_float[Form("jet%d_Track%ddEta", valid_jet, track)] = 0.5;
			tree_output_vars_float[Form("jet%d_Track%ddPhi", valid_jet, track)] = 0.5;
		}
		if (track_pt_index.size() > 0) {
			std::sort (track_pt_index.begin(), track_pt_index.end(), greater<pair<float, float>>()); // sort to find highest pt tracks
			// int n_track = std::min(3, (int) jet_track_index.size());
			int n_track = jet_track_index.size();
			int valid_tracks = 0;
			for (int track = 0; track < n_track; track++) {
				int track_num = track_pt_index[track].second;
				if (DeltaR( jet_Eta->at(i), track_Eta->at(track_num), jet_Phi->at(i), track_Phi->at(track_num) ) > 0.4 ) continue; 			// require track is closer to jet than ntuples enforce
				// could put requirement here that track matches with PV
				valid_tracks += 1; 
				if (valid_tracks > 3) continue;			
				tree_output_vars_float[Form("jet%d_Track%dPt", valid_jet, track)] 			= track_pt_index[track].first;
				tree_output_vars_float[Form("jet%d_Track%ddzToPV", valid_jet, track)] 		= track_dzToPV->at(track_num); 
				tree_output_vars_float[Form("jet%d_Track%ddxyToBS", valid_jet, track)] 		= track_dxyToBS->at(track_num); 
				tree_output_vars_float[Form("jet%d_Track%ddzOverErr", valid_jet, track)]	= track_dzToPV->at(track_num) / track_dzErr->at(track_num); 
				tree_output_vars_float[Form("jet%d_Track%ddxyOverErr", valid_jet, track)] 	= track_dxyToBS->at(track_num) / track_dxyErr->at(track_num); 

				tree_output_vars_float[Form("jet%d_Track%dnMissingInnerHits", valid_jet, track)] 	= track_nMissingInnerHits->at(track_num);
				tree_output_vars_float[Form("jet%d_Track%dnMissingOuterHits", valid_jet, track)] 	= track_nMissingOuterHits->at(track_num); 
				tree_output_vars_float[Form("jet%d_Track%dnPixelHits", valid_jet, track)] 			= track_nPixelHits->at(track_num); 
				tree_output_vars_float[Form("jet%d_Track%dnHits", valid_jet, track)] 				= track_nHits->at(track_num); 

				tree_output_vars_float[Form("jet%d_Track%ddR", valid_jet, track)] 			= DeltaR( jet_Eta->at(i), track_Eta->at(track_num), jet_Phi->at(i), track_Phi->at(track_num) ); 
				tree_output_vars_float[Form("jet%d_Track%ddEta", valid_jet, track)] 		= jet_Eta->at(i) - track_Eta->at(track_num);
				tree_output_vars_float[Form("jet%d_Track%ddPhi", valid_jet, track)] 		= deltaPhi( jet_Phi->at(i), track_Phi->at(track_num) ); 
				if (track == 1) {															// dR for two leading tracks
					tree_output_vars_float[Form("jet%d_Tracks_dR", valid_jet)] 				= DeltaR( track_Eta->at(track_pt_index[0].second), track_Eta->at(track_num), track_Phi->at(track_pt_index[0].second), track_Phi->at(track_num) );
				}
			}
		} // end of track matching 
		valid_jet += 1;
	}
	tree_output_vars_int["validJet"]	= valid_jet;

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
		tree_output_vars_float[Form("LLP%d_DecayT", i)] = distance * ( 1 / gLLP_Beta->at(i) ) * 0.03336; // 1/c in ns / cm to give answer in ns
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

	MyTags event_based = MyTags(/*event_based=*/ true, /*calor_only=*/ false);
	if( EventValidForBDTEval() ){
		for( const auto & bdt_tag: event_based.bdt_tags() ) {
			tree_output_vars_float[Form("bdtscore_%s", bdt_tag.c_str())] = GetBDTScores(bdt_tag);
		}
	} 

	tree_output[treename]->Fill();
	
	if( debug ) cout<<"DONE DisplacedHcalJetAnalyzer::FillOutputTrees()"<<endl;

}	

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::FillOutputJetTrees( string treename, int jetIndex ){ 
	// for the output trees that are filled on a per jet basis

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::FillOutputJetTrees()"<<endl;

	if ( std::find(jet_treenames.begin(), jet_treenames.end(), treename) == jet_treenames.end() ) return;

	ResetOutputBranches( treename );

	jet_tree_output_vars_int["run"] 		= runNum;
	jet_tree_output_vars_int["lumi"] 		= lumiNum;
	jet_tree_output_vars_int["event"] 		= eventNum;
	jet_tree_output_vars_int["jet"]			= n_jet;
	jet_tree_output_vars_int["ele"]			= n_ele;
	jet_tree_output_vars_int["muon"]		= n_muon;
	jet_tree_output_vars_int["pho"]			= n_pho;
	jet_tree_output_vars_float["eventHT"]   = EventHT();

	for (int i = 0; i < HLT_Indices.size(); i++) { 
		tree_output_vars_bool[HLT_Names[i]] = HLT_Decision->at(i);
	}

	jet_tree_output_vars_float["perJet_E"] 			= jet_E->at(jetIndex);
	jet_tree_output_vars_float["perJet_Pt"] 		= jet_Pt->at(jetIndex);
	jet_tree_output_vars_float["perJet_Eta"] 		= jet_Eta->at(jetIndex);
	jet_tree_output_vars_float["perJet_Phi"] 		= jet_Phi->at(jetIndex);
	jet_tree_output_vars_float["perJet_Mass"] 		= jet_Mass->at(jetIndex);
	jet_tree_output_vars_float["perJet_Area"] 		= jet_JetArea->at(jetIndex);
	jet_tree_output_vars_float["perJet_ChargedHadEFrac"] 	= jet_ChargedHadEFrac->at(jetIndex);
	jet_tree_output_vars_float["perJet_NeutralHadEFrac"] 	= jet_NeutralHadEFrac->at(jetIndex);
	jet_tree_output_vars_float["perJet_PhoEFrac"] 			= jet_PhoEFrac->at(jetIndex);
	jet_tree_output_vars_float["perJet_EleEFrac"] 			= jet_EleEFrac->at(jetIndex);
	jet_tree_output_vars_float["perJet_MuonEFrac"] 			= jet_MuonEFrac->at(jetIndex);

	jet_tree_output_vars_float["perJet_PtAllTracks"] 	= jet_PtAllTracks->at(jetIndex);
	jet_tree_output_vars_float["perJet_PtAllPVTracks"] 	= jet_PtAllPVTracks->at(jetIndex);
	
	jet_tree_output_vars_float["perJet_isTruthMatched"] 	= 0;
	vector<float> matchedInfo = JetIsMatchedTo( jet_Eta->at(jetIndex), jet_Phi->at(jetIndex) );
	float matchedLLP = matchedInfo[0];
	float otherLLP = -1;
	if (matchedLLP == 1) otherLLP = 0;
	if (matchedLLP == 0) otherLLP = 1;
	float matchedDR = matchedInfo[1];
	if (matchedLLP > -1) { // if jet is matched to a LLP or LLP decay product
		jet_tree_output_vars_float["perJet_isTruthMatched"] = 1;
		jet_tree_output_vars_float["perJet_isMatchedTo"] = matchedLLP;
		jet_tree_output_vars_float["perJet_MatchedLLP_DecayZ"] = gLLP_DecayVtx_Z->at(matchedLLP); // what is the decay Z for the LLP matched to this jet?
		jet_tree_output_vars_float["perJet_MatchedLLP_DecayR"] = gLLP_DecayVtx_R.at(matchedLLP); // what is the decay R for the LLP matched to this jet?
		jet_tree_output_vars_float["perJet_MatchedLLP_TravelTime"] = gLLP_TravelTime->at(matchedLLP); // what is the travel time for the LLP matched to this jet?
		jet_tree_output_vars_float["perJet_OtherLLP_DecayR"] = gLLP_DecayVtx_R.at(otherLLP); // what is the decay R for the LLP matched to this jet?
		jet_tree_output_vars_float["perJet_MatchedLLP_Eta"] = gLLP_Eta->at(matchedLLP); // what is the decay eta for the LLP matched to this jet?
	}

	vector<float> energy = GetEnergyProfile_Jet(jetIndex, 0.4);
	vector<float> spread_Eta_Phi = GetEtaPhiSpread_Jet(jetIndex, 0.4); // eta, phi (average); eta, phi (energy weighted); S_ee, S_pp, S_ep
	vector<pair<float,int>> energeticRechits = Get3RechitE_Jet(jetIndex, 0.4); // three highest rechit energies in the jet, and total energy

	jet_tree_output_vars_float["perJet_S_etaeta"] 		= spread_Eta_Phi[4];
	jet_tree_output_vars_float["perJet_S_phiphi"] 		= spread_Eta_Phi[5];
	jet_tree_output_vars_float["perJet_S_etaphi"] 		= spread_Eta_Phi[6];	

	for (int depth = 0; depth < 4; depth++) jet_tree_output_vars_float[Form("perJet_EnergyFrac_Depth%d", depth+1)] = energy[depth]; // each fractional energy saved in different tree
	jet_tree_output_vars_float["perJet_LeadingRechitE"] 		= energeticRechits[0].first;
	jet_tree_output_vars_float["perJet_SubLeadingRechitE"] 		= energeticRechits[1].first;
	jet_tree_output_vars_float["perJet_SSubLeadingRechitE"] 	= energeticRechits[2].first;
	jet_tree_output_vars_float["perJet_AllRechitE"] 			= energeticRechits[3].first;
	jet_tree_output_vars_int["perJet_LeadingRechitD"] 			= energeticRechits[0].second;
	
	// find three highest pT tracks matched to a jet, and save the generalTrack index for use later (in dzToPV and dzyToBS)
	vector<uint> jet_track_index = jet_TrackIndices->at(jetIndex);
	vector<pair<float, float>> track_pt_index = TrackMatcher(jetIndex, jet_track_index);

	for (int track = 0; track < 3; track++) {
		jet_tree_output_vars_float[Form("perJet_Track%dPt", track)] = 0; // default value for track pT (in case no tracks are matched)
		jet_tree_output_vars_float[Form("perJet_Track%ddR", track)] = 0.5; // default value for track dR (in case no tracks are matched)
		jet_tree_output_vars_float[Form("perJet_Track%ddEta", track)] = 0.5;
		jet_tree_output_vars_float[Form("perJet_Track%ddPhi", track)] = 0.5;
	}
	if (track_pt_index.size() > 0) {
		std::sort (track_pt_index.begin(), track_pt_index.end(), greater<pair<float, float>>()); // sort to find highest pt tracks
//		int n_track = std::min(3, (int) jet_track_index.size());
		int n_track = jet_track_index.size();
		int valid_tracks = 0;
		for (int track = 0; track < n_track; track++) {
			int track_num = track_pt_index[track].second;
			if (DeltaR( jet_Eta->at(jetIndex), track_Eta->at(track_num), jet_Phi->at(jetIndex), track_Phi->at(track_num) ) > 0.4 ) continue; 			// require track is closer to jet than ntuples enforce
			// could put requirement here that track matches with PV
			valid_tracks += 1; 
			if (valid_tracks > 3) continue;
			jet_tree_output_vars_float[Form("perJet_Track%dPt", track)] 			= track_pt_index[track].first;
			jet_tree_output_vars_float[Form("perJet_Track%ddzToPV", track)] 		= track_dzToPV->at(track_num); 
			jet_tree_output_vars_float[Form("perJet_Track%ddxyToBS", track)] 		= track_dxyToBS->at(track_num); 
			jet_tree_output_vars_float[Form("perJet_Track%ddzOverErr", track)]		= track_dzToPV->at(track_num) / track_dzErr->at(track_num); 
			jet_tree_output_vars_float[Form("perJet_Track%ddxyOverErr", track)] 	= track_dxyToBS->at(track_num) / track_dxyErr->at(track_num); 

			jet_tree_output_vars_float[Form( "perJet_Track%ddR", track) ] 		= DeltaR( jet_Eta->at(jetIndex), track_Eta->at(track_num), jet_Phi->at(jetIndex), track_Phi->at(track_num) ); 
			jet_tree_output_vars_float[Form( "perJet_Track%ddEta", track) ] 	= jet_Eta->at(jetIndex) - track_Eta->at(track_num);
			jet_tree_output_vars_float[Form( "perJet_Track%ddPhi", track) ] 	= deltaPhi( jet_Phi->at(jetIndex), track_Phi->at(track_num) ); 
			if (track == 1) {									// dR for two leading tracks
				jet_tree_output_vars_float["perJet_Tracks_dR"] 	= DeltaR( track_Eta->at(track_pt_index[0].second), track_Eta->at(track_num), track_Phi->at(track_pt_index[0].second), track_Phi->at(track_num) );
			}
		}
	} // end of track matching 

	MyTags jet_based = MyTags(/*event_based=*/ false, /*calor_only=*/ false);
	if( EventValidForBDTEval() ){
		for(const auto & bdt_tag: jet_based.bdt_tags() ) {
			jet_tree_output_vars_float[Form("bdtscore_%s", bdt_tag.c_str())] = GetBDTScores(bdt_tag);
		}
	}

	MyTags jet_based_calor = MyTags(/*event_based=*/ false, /*calor_only=*/ true);
	if( EventValidForBDTEval() ){
		for(const auto & bdt_tag: jet_based_calor.bdt_tags() ) {
			jet_tree_output_vars_float[Form("bdtscore_%s", bdt_tag.c_str())] = GetBDTScores(bdt_tag);
		}
	}

	jet_tree_output[treename]->Fill();
	
	if( debug ) cout<<"DONE DisplacedHcalJetAnalyzer::FillOutputJetTrees()"<<endl;

}	

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::WriteOutputTrees(){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::WriteOutputTrees()"<<endl;

	cout<<"Writing Tree(s)..."<<endl;

	for(map<string,TTree*>::iterator it = tree_output.begin(); it != tree_output.end(); ++it){
		//AddWeightBranch( tree_output[it->first] );
		tree_output[it->first]->Write("", TObject::kOverwrite); 
	}
	for(map<string,TTree*>::iterator it = jet_tree_output.begin(); it != jet_tree_output.end(); ++it){
		jet_tree_output[it->first]->Write("", TObject::kOverwrite); 
	}

}

