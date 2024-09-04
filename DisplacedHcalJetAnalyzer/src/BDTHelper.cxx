/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::InitializeTMVA(){

	bdt_version = "v0.7";
	bdt_tags = { "LLP350_MS80_perJet" };

	vector<string> bdt_tags_booked = {};
	//map<string,bool> bdt_perJet;
	vector<string> bdt_variables_all;

	for( auto bdt_tag: bdt_tags ){

		//bdt_perJet[bdt_tag] = false;
		//if( bdt_tag.find( "perJet" ) != string::npos ) bdt_perJet[bdt_tag] = true;

		bool pass = BookTMVAReader( bdt_tag );

		if( pass == false ) {
			cout<<"WARNING: Unable to book TMVA Reader for bdt_tag "<<bdt_tag<<" -- file not found?"<<endl;
			continue;
		}

		bdt_tags_booked.push_back( bdt_tag );

		for( auto var: bdt_var_names[bdt_tag] ){
			bdt_variables_all.push_back( var );
		}

	}

}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::BookTMVAReader( string bdt_tag ){

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

	if( !filepath_exists ) return false; 

	string filename = Form("%s%s/weights_%s/TMVAClassification_BDTG.weights.xml", filepath.c_str(), bdt_version.c_str(), bdt_tag.c_str() );
	
	// Declare TMVA Reader
	cout<<"  --> "<<bdt_tag<<" from  "<<filename<<endl;
	bdt_reader[bdt_tag] = new TMVA::Reader( "!Color:!Silent", debug );

	// Read in Variables (Automated!)
	
	bdt_var_names[bdt_tag] = GetBDTVariableNamesXML( filename );

	for( auto bdt_var_name: bdt_var_names[bdt_tag] ){
		bdt_vars[bdt_tag+" "+bdt_var_name] = 0;
		bdt_reader[bdt_tag]->AddVariable( Form("%s", bdt_var_name.c_str()), &bdt_vars[bdt_tag+" "+bdt_var_name] );
	}

	bdt_reader[bdt_tag]->BookMVA("BDT", Form("%s", filename.c_str() ) );

	return true; 

}

/* ====================================================================================================================== */
// A quick way to split strings separated via any character delimiter (https://www.geeksforgeeks.org/how-to-split-a-string-in-cc-python-and-java/)
vector<string> AdvTokenizer(string s, char del)
{
	vector<string> split_string = {};
	stringstream ss(s);
	string word;
	while (!ss.eof()) {
		getline(ss, word, del);
		split_string.push_back(word);
	}
	return split_string;
}

/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::GetBDTScores( string bdt_tag, int jet_index_int ){ //, map<string,Float_t> input_vars, string jet_index ){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetBDTScores()"<<endl;

	// Check if bdt tag exists...	
	//if (!MyTags::isValidTag(bdt_tag)) { // BDT tag not found
	//	return -999.9;
	//}

	string jet_index = Form("%d", jet_index_int);

	for( auto var: bdt_var_names[bdt_tag] ){
		
		if( jet_index == "-1" ){ 
			if (var.find( "/" ) != string::npos ) { 
				vector<string> split_string = AdvTokenizer(var, '/');
				bdt_vars[bdt_tag+" "+var] = tree_output_vars_float[split_string[0]] / tree_output_vars_float[split_string[1]];	// GK: fix for division BDT vars, such that "x/y" becomes input_var["x"] / input_var["y"]
			}
			else bdt_vars[bdt_tag+" "+var] = tree_output_vars_float[var];
		} else {
			string var_mod = var;
			var_mod.replace( var.find( "perJet" ), 6, "jet"+jet_index );
			if (var_mod.find( "perJet" ) != string::npos ) var_mod.replace( var_mod.find( "perJet" ), 6, "jet"+jet_index );
			if (var_mod.find( "/" ) != string::npos ) {
				vector<string> split_string = AdvTokenizer(var_mod, '/');
				bdt_vars[bdt_tag+" "+var] = tree_output_vars_float[split_string[0]] / tree_output_vars_float[split_string[1]];	// GK: fix for division BDT vars, such that "x/y" becomes input_var["x"] / input_var["y"]
			}
			else bdt_vars[bdt_tag+" "+var] = tree_output_vars_float[var_mod];
		}
	}

	return bdt_reader[bdt_tag]->EvaluateMVA("BDT");
}

/* ====================================================================================================================== */
vector<string> DisplacedHcalJetAnalyzer::GetBDTVariableNamesXML( string filename ){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetBDTVariableNamesXML()"<<endl;

	// From: https://root.cern/doc/master/RReader_8hxx_source.html#l00037

	// Parse XML file and find root node

	TXMLEngine xml;
	auto xmldoc = xml.ParseFile( filename.c_str() );

	// Get Variable Names

	vector<string> bdt_var_names_temp;

	auto mainNode = xml.DocGetRootElement(xmldoc);
	for (auto node = xml.GetChild(mainNode); node; node = xml.GetNext(node)) {
			const auto nodeName = std::string(xml.GetNodeName(node));
			if (nodeName.compare("Variables") == 0) {
					for (auto thisNode = xml.GetChild(node); thisNode; thisNode = xml.GetNext(thisNode)) {
							// cout<<std::atoi(xml.GetAttr(thisNode, "VarIndex"))<<"  "<<xml.GetAttr(thisNode, "Title")<<"  "<<xml.GetAttr(thisNode, "Expression")<<endl;
							bdt_var_names_temp.push_back( xml.GetAttr(thisNode, "Expression") );
							//cout<<xml.GetAttr(thisNode, "Expression")<<endl;
					}
			}
	}

	return bdt_var_names_temp;

}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::EventValidForBDTEval(){ // TO IMPLEMENT
 
	if( debug ) cout<<"DisplacedHcalJetAnalyzer::EventValidForBDTEval()"<<endl;

	return true;
}
