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
	};	
	for ( int i=0; i<3; i++ ) {
		myvars_int.push_back( Form("jet%d_rechitN", i) );
	}

	vector<string> myvars_float = {
		"",
	};

	for( int i=0; i<3; i++ ) {
		myvars_float.push_back( Form("jet%d_energy", i) );
		myvars_float.push_back( Form("jet%d_pt", i) );
		myvars_float.push_back( Form("jet%d_Eta", i) );
		myvars_float.push_back( Form("jet%d_Phi", i) );

		myvars_float.push_back( Form("jet%d_etaSpread", i) );
		myvars_float.push_back( Form("jet%d_etaSpread_energy", i) );
		myvars_float.push_back( Form("jet%d_phiSpread", i) );
		myvars_float.push_back( Form("jet%d_phiSpread_energy", i) );

		if (i < 2) {
			myvars_float.push_back( Form("gLLP%d_DecayX", i));
			myvars_float.push_back( Form("gLLP%d_DecayY", i));
			myvars_float.push_back( Form("gLLP%d_DecayZ", i));
		}
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

	tree_output_vars_int["RechitN_1GeV"] = 0; 
	tree_output_vars_int["RechitN_5GeV"] = 0; 
	tree_output_vars_int["RechitN_10GeV"] = 0; 
	for (int i = 0; i < n_hbheRechit; i++) {
		if (hbheRechit_E->at(i) > 1) tree_output_vars_int["RechitN_1GeV"] += 1;
		if (hbheRechit_E->at(i) > 5) tree_output_vars_int["RechitN_5GeV"] += 5;
		if (hbheRechit_E->at(i) > 10) tree_output_vars_int["RechitN_10GeV"] += 10;
	}

	int max = std::min(3, n_jet);
	for (int i = 0; i < max; i++) {
		tree_output_vars_float[Form("jet%d_energy", i)] = jet_E->at(i);
		tree_output_vars_float[Form("jet%d_pt", i)] 	= jet_Pt->at(i);
		tree_output_vars_float[Form("jet%d_Eta", i)] 	= jet_Eta->at(i);
		tree_output_vars_float[Form("jet%d_Phi", i)] 	= jet_Phi->at(i);

		vector<float> rechitJet = GetMatchedHcalRechits_Jet(i, 0.4);
		vector<float> energy = GetEnergyProfile_Jet(i, 0.4);
		vector<float> spread_Eta_Phi = GetEtaPhiSpread_Jet(i, 0.4); // eta, phi (average); eta, phi (energy weighted)
		// for (int depth = 0; depth < 4; depth++) h["jet"+i+"_energyProfile"]->Fill(depth + 1, energy[depth]); // figuring out how to save still
		tree_output_vars_int[Form("jet%d_rechitN", i)] = rechitJet.size();
		tree_output_vars_float[Form("jet%d_etaSpread", i)] = spread_Eta_Phi[0];
		tree_output_vars_float[Form("jet%d_etaSpread_energy", i)] = spread_Eta_Phi[2];
		tree_output_vars_float[Form("jet%d_phiSpread", i)] = spread_Eta_Phi[1];
		tree_output_vars_float[Form("jet%d_phiSpread_energy", i)] = spread_Eta_Phi[3];
	}
	for (int i = 0; i < n_gLLP; i++) {
		tree_output_vars_float[Form("gLLP%d_DecayX", i)] = gLLP_DecayVtx_X->at(i);
		tree_output_vars_float[Form("gLLP%d_DecayY", i)] = gLLP_DecayVtx_Y->at(i);
		tree_output_vars_float[Form("gLLP%d_DecayZ", i)] = gLLP_DecayVtx_Z->at(i);
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
