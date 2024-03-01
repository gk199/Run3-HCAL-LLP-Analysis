/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::DeclareTMVAReader( MyTags bdt_tag_info ){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::DeclareTMVAReader()"<<endl;

	vector<string> bdt_tags = bdt_tag_info.bdt_tags();
	
	if( !save_bdtscores ) bdt_tags.clear();

	cout<<"Reading in BDT Weight Files..."<<endl;

	for( auto bdt_tag: bdt_tags ){

		// Get filepath
		bool filepath_exists = false;
		vector<string> filepaths = { "BDTWeightFiles/", "../BDTWeightFiles/", "../../BDTWeightFiles/" };
		string filepath;
		for( int i=0; i<filepaths.size(); i++ ){
			if( !gSystem->AccessPathName( Form("%s", filepaths.at(i).c_str()) ) ){
				filepath = filepaths.at(i);
				filepath_exists = true;
				cout<<"Looking for BDT weight files in "<<filepath<<endl;
				break;
			}
		}

		string filename = Form("%s/v0.6/weights_%s/TMVAClassification_BDTG.weights.xml", filepath.c_str(), bdt_tag.c_str() );

		// Declare TMVA Reader
		cout<<"  --> "<<bdt_tag<<" from  "<<filename<<endl;
		bdt_reader[bdt_tag] = new TMVA::Reader( "!Color:!Silent", debug );

		// Read in variables // TODO: AUTOMATE

		bdt_var_names[bdt_tag] = bdt_tag_info.bdt_var_names();

		// TODO bdt_var_names[bdt_tag] = GetBDTVariableNamesXML( filename, false );

		for( auto bdt_var_name: bdt_var_names[bdt_tag] ){
			bdt_vars[bdt_tag+" "+bdt_var_name] = 0;
			bdt_reader[bdt_tag]->AddVariable( Form("%s", bdt_var_name.c_str()), &bdt_vars[bdt_tag+" "+bdt_var_name] );
		}

		// Read in spectators (to do... remove!)


		vector<string> bdt_var_spectators = {
			// "LLP0_Pt",
			// "LLP0_E",
			// "LLP0_Beta",
			// "LLP0_TravelTime",
			// "LLP0_DecayR",
			// "LLP0_DecayX",
			// "LLP0_DecayY",
			// "LLP0_DecayZ"
		};

		// TODO bdt_var_names[bdt_tag] = GetBDTVariableNamesXML( filename, true );

		for( auto bdt_var_name: bdt_var_spectators ){
			bdt_reader[bdt_tag]->AddSpectator( Form("%s", bdt_var_name.c_str()), &bdt_vars[bdt_tag+" "+bdt_var_name] );
		}

		bdt_reader[bdt_tag]->BookMVA("BDT", Form("%s", filename.c_str() ) );
		
	}
}

/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::GetBDTScores(string bdt_tag){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetBDTScores()"<<endl;

	// Check if bdt tag exists...	
	if (!MyTags::isValidTag(bdt_tag)) { // BDT tag not found
		return -999.9;
	}

//	if( std::find(bdt_tags.begin(), bdt_tags.end(), bdt_tag) == bdt_tags.end() ) return -999.9;

	for( auto bdt_var_name: bdt_var_names[bdt_tag] ){
		if (bdt_tag.find("perJet") != std::string::npos) {
			bdt_vars[bdt_tag+" "+bdt_var_name] = jet_tree_output_vars_float[bdt_var_name];
		}
		else {
			bdt_vars[bdt_tag+" "+bdt_var_name] = tree_output_vars_float[bdt_var_name];
		}
	}

	return bdt_reader[bdt_tag]->EvaluateMVA("BDT");

}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::EventValidForBDTEval(){ // TO IMPLEMENT

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::EventValidForBDTEval()"<<endl;

	return true;
}

/* ====================================================================================================================== */
vector<string> DisplacedHcalJetAnalyzer::GetBDTVariableNamesXML( string filepath, bool isSpectator ){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetBDTVariableNamesXML()"<<endl;

	/*TODO 
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file("tree.xml");

	std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;
	

	pugi::xml_document xml_temp;
	cout<<filepath<<endl;
	xml_temp.load_file( filepath );

	auto rotary = xml_temp.root();
	auto name = rotary
        .select_single_node("//MethodSetup/Variables/text()")
        .node();

    cout<<name.value()<<endl;*/

    vector<string> bdt_var_names_temp;
    return bdt_var_names_temp;

}
