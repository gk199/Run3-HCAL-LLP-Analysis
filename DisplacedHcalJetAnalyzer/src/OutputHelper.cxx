/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::DeclareOutputTrees(){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::DeclareOutputTrees()"<<endl;

	if( !save_trees ){
		treenames = {};
		return;
	}
	
	cout<<"Declaring Output Trees..."<<endl;	

	treenames = { "" }; 

	vector<string> myvars_bool = {
		"",
	};

	vector<string> myvars_int = {
		"",
	};	

	vector<string> myvars_float = {
		"",
	};


	for( auto treename: treenames ){
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
void DisplacedHcalJetAnalyzer::FillOutputTrees( string treename ){ 

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::FillOutputTrees()"<<endl;

	if ( std::find(treenames.begin(), treenames.end(), treename) == treenames.end() ) return;

	// tree_output_vars_int[""] = XYZ;

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

