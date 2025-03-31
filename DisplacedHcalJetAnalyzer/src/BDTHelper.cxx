/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::InitializeTMVA(){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::InitializeTMVA()"<<endl;

	bdt_version = "v0.7";

	bdt_tags = {};
	/*	"LLP125_MS15_perJet",
		"LLP125_MS50_perJet",
		"LLP250_MS120_perJet",
		"LLP350_MS160_perJet",
		"LLP350_MS80_perJet",
		"hadd_perJet" 
	};*/

	vector<string> bdt_tags_booked = {};
	vector<string> bdt_variables_all;

	// Get filepath
	bool filepath_exists = false;
	vector<string> filepaths = { "BDTWeightFiles/", "../BDTWeightFiles/", "../../BDTWeightFiles/" };
	string filepath;
	for( int i=0; i<filepaths.size(); i++ ){
		if( !gSystem->AccessPathName( Form("%s", filepaths.at(i).c_str()) ) ){
			filepath = filepaths.at(i);
			filepath_exists = true;
			cout<<"\nLooking for BDT weight files in "<<filepath<<endl;
			break;
		}
	}

	if( !filepath_exists ){
		cout<<"WARNING: BDT weight directory not found! Will run without BDT..."<<endl;
		bdt_tags = {};
		return; 
	}

	for( auto bdt_tag: bdt_tags ){

		bool pass = BookTMVAReader( filepath, bdt_tag );

		if( pass == false ) {
			cout<<"WARNING: Unable to book TMVA Reader for bdt_tag "<<bdt_tag<<endl;
			continue;
		}

		bdt_tags_booked.push_back( bdt_tag );

		for( auto var: bdt_var_names[bdt_tag] ){
			bdt_variables_all.push_back( var );
		}
	}

	bdt_tags = bdt_tags_booked;
}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::BookTMVAReader( string filepath, string bdt_tag ){

	cout<<" --> "<<bdt_tag<<endl;

	string filename = Form("%s%s/weights_%s/TMVAClassification_BDTG.weights.xml", filepath.c_str(), bdt_version.c_str(), bdt_tag.c_str() );

	if( debug ) cout<<filename<<endl;

	// To fix
	//if( !gSystem->AccessPathName( Form("%s", filename.c_str()) ) ){
	//	cout<<"WARNING: BDT weight file not found for "<<bdt_tag<<endl;
	//	return false;
	//}

	// Declare TMVA Reader
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

	string jet_index = Form("%d", jet_index_int);

	for( auto var: bdt_var_names[bdt_tag] ){
		
		string var_mod = var;
		
		// It just needs to be done this way 
		if( var_mod.find("jet0") != string::npos ) var_mod.replace( var.find( "jet0" ), 4, "jet"+jet_index );			
		if( var_mod.find( "perJet" ) != string::npos ) var_mod.replace( var_mod.find( "perJet" ), 6, "jet"+jet_index );

		if( var_mod.find( "/" ) != string::npos ) {
			vector<string> var_mod_split = AdvTokenizer(var_mod, '/');
			if( var_mod_split[1].find("jet0") != string::npos ) var_mod_split[1].replace( var_mod_split[1].find( "jet0" ), 4, "perJet" );			
			if( var_mod_split[1].find( "perJet" ) != string::npos ) var_mod_split[1].replace( var_mod_split[1].find( "perJet" ), 6, "jet"+jet_index );

			bdt_vars[bdt_tag+" "+var] = tree_output_vars_float[var_mod_split[0]] / tree_output_vars_float[var_mod_split[1]];	// GK: fix for division BDT vars, such that "x/y" becomes input_var["x"] / input_var["y"]
		}
		else bdt_vars[bdt_tag+" "+var] = tree_output_vars_float[var_mod];
	
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
