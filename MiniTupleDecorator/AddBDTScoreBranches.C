#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TPad.h"
#include "THStack.h"
#include "TLegend.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"

/* ====================================================================================================================== */
// Global Inputs 

bool debug = false;

// ----- Input and Output Paths ----- //

string basepath = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/";
string vIN 		= "v3.7";
string vOUT		= vIN+".1";

string infiledir  = basepath + vIN;
string outfiledir = basepath + vOUT;

// ----- Selections ----- //

TCut cuts = ""; 

// ----- Lifetime Reweighting ----- //
vector<string> list_lifetime_rw_str = { "100", "300", "1000", "3000", "10000", "30000", "100000" };

// ----- BDT Globals ----- //

string bdt_version = "v0.7";

vector<string> bdt_tags = { "LLP350_MS80_perJet" }; //{ "LLP125", "LLP125_perJet" }; //{ "LLP125", "LLP350", "hadd",  "LLP125_perJet", "LLP350_perJet", "hadd_perJet" };
map<string,TMVA::Reader*> bdt_reader;
map<string,vector<string>> bdt_var_names;
map<string,Float_t> bdt_vars;

/* ====================================================================================================================== */
float GetCTauFromInfiletag( string infiletag ){

	if( debug ) cout<<"Getting CTau From infiletag..."<<endl;

	// Start String
	string str_first = "CTau";
	string infiletag_partial = infiletag.substr( infiletag.find(str_first)+str_first.length(), infiletag.length() );

	// End String (either the following _ or .)
	vector<string> vecstr_last = {".", "_"};
	unsigned last = infiletag.length();

	for( auto str_last: vecstr_last ){
		if( infiletag_partial.find(str_last) == string::npos ) continue;
		unsigned last_temp = infiletag_partial.find(str_last);
		if( last_temp < last ) last = last_temp;
	}

	float lifetime = std::stof( infiletag_partial.substr(0,last))*0.1; // 0.1 to convert from mm to cm

	cout<<"Reading in signal lifetime as: "<<lifetime<<" cm..."<<endl;

	return lifetime;

}

/* ====================================================================================================================== */
Float_t GetLifetimeReweight( float ctau_sample, map<string,Float_t> input_vars, float ctau_target ){

	Float_t ctau_llp0 = input_vars["LLP0_DecayCtau"]; //LLP0_DecayCtau"];
    Float_t ctau_llp1 = input_vars["LLP1_DecayCtau"];

	Float_t reweight_llp0 = pow ( ctau_sample / ctau_target, 1 ) * exp( -ctau_llp0 * ( 1.0/ctau_target - 1.0/ctau_sample ) );
    Float_t reweight_llp1 = pow ( ctau_sample / ctau_target, 1 ) * exp( -ctau_llp1 * ( 1.0/ctau_target - 1.0/ctau_sample ) );
    return reweight_llp0*reweight_llp1;

}

/* ====================================================================================================================== */
vector<string> GetBDTVariableNamesXML( string filename ){
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
                        }
                }
        }

    return bdt_var_names_temp;

}

/* ====================================================================================================================== */
bool BookTMVAReader( string bdt_tag ){

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
float GetBDTScore( string bdt_tag, map<string,Float_t> input_vars, string jet_index ){

	for( auto var: bdt_var_names[bdt_tag] ){
		
		if( jet_index == "-1" ){ 
			if (var.find( "/" ) != string::npos ) { 
				vector<string> split_string = AdvTokenizer(var, '/');
				bdt_vars[bdt_tag+" "+var] = input_vars[split_string[0]] / input_vars[split_string[1]];	// GK: fix for division BDT vars, such that "x/y" becomes input_var["x"] / input_var["y"]
			}
			else bdt_vars[bdt_tag+" "+var] = input_vars[var];
		} else {
			string var_mod = var;
			var_mod.replace( var.find( "perJet" ), 6, "jet"+jet_index );
			if (var_mod.find( "perJet" ) != string::npos ) var_mod.replace( var_mod.find( "perJet" ), 6, "jet"+jet_index );
			if (var_mod.find( "/" ) != string::npos ) {
				vector<string> split_string = AdvTokenizer(var_mod, '/');
				bdt_vars[bdt_tag+" "+var] = input_vars[split_string[0]] / input_vars[split_string[1]];	// GK: fix for division BDT vars, such that "x/y" becomes input_var["x"] / input_var["y"]
			}
			else bdt_vars[bdt_tag+" "+var] = input_vars[var_mod];
		}
	}

	return bdt_reader[bdt_tag]->EvaluateMVA("BDT");
}

/* ====================================================================================================================== */
void AddBranchesToTree( TTree* tree, bool tree_perJet, float signal_ctau ){

	// ----- Define Input Variables & Branches ----- //

	vector<string> input_variable_names = {};

	// Book TMVA Reader //
	
	vector<string> bdt_tags_booked = {};
	map<string,bool> bdt_perJet;

	for( auto bdt_tag: bdt_tags ){ 

		bdt_perJet[bdt_tag] = false;
                if( bdt_tag.find( "perJet" ) != string::npos ) bdt_perJet[bdt_tag] = true;

		// Don't run event-level bdt on jet level tree
		if( tree_perJet && !bdt_perJet[bdt_tag] ) continue; 

		bool pass = BookTMVAReader( bdt_tag );
		if( pass == false ) {
			cout<<"WARNING: Unable to book TMVA Reader for bdt_tag "<<bdt_tag<<" -- file not found?"<<endl;
			continue;
		}

		bdt_tags_booked.push_back( bdt_tag );
		
		for( auto var: bdt_var_names[bdt_tag] ){

			// Tree / bdt agreement (event, event) and (perJet, perJet)
			if( tree_perJet == bdt_perJet[bdt_tag] ){
				if (var.find( "/" ) != string::npos ) {	// GK: added if statement to deal with division. push back "x" and push back "y" when x/y is input to BDT
					vector<string> split_string = AdvTokenizer(var, '/');
					input_variable_names.push_back(split_string[0]);
					input_variable_names.push_back(split_string[1]);
				}
				else input_variable_names.push_back( var );
				continue;
			} 
			
		    // Run jet-level bdt on event-level
			for( auto i_jet: vector<string>{ "0", "1", "2", "3", "4", "5" } ){
				string var_mod = var;
				var_mod.replace( var.find( "perJet" ), 6, "jet"+i_jet ); 
				if (var_mod.find( "perJet" ) != string::npos ) var_mod.replace( var_mod.find( "perJet" ), 6, "jet"+i_jet );
				if (var_mod.find( "/" ) != string::npos ) { // GK: GK: added if statement to deal with division. push back "x" and push back "y" when x/y is input to BDT
					vector<string> split_string = AdvTokenizer(var_mod, '/');
					input_variable_names.push_back(split_string[0]);
					input_variable_names.push_back(split_string[1]);
				}
				else input_variable_names.push_back( var_mod );
			}
		}
	}

	// Add Lifetime Reweighting branches //
	input_variable_names.push_back( "LLP0_DecayCtau" );
        input_variable_names.push_back( "LLP1_DecayCtau" );	

	// Make input_variable_names unique
	sort( input_variable_names.begin(), input_variable_names.end() );
	input_variable_names.erase(std::unique(input_variable_names.begin(), input_variable_names.end()), input_variable_names.end());

	// Declare Input Branches //
	
	map<string,Float_t> input_vars;
	map<string,TBranch*> input_branches;

	for( auto input_var: input_variable_names ){
		tree->SetBranchAddress( Form("%s", input_var.c_str()), &input_vars[input_var], &input_branches[input_var] );
	}

	// ----- Define Output Variables & Branches ----- //

	vector<string> output_variable_names = {};

	for( auto bdt_tag: bdt_tags_booked ){
		if( tree_perJet == bdt_perJet[bdt_tag] ){
			output_variable_names.push_back( "bdtscoreX_"+bdt_tag );
			continue;
		}
		for( auto i_jet: vector<string>{ "0", "1", "2", "3", "4", "5" } ){ // first 6 jets are saved
			output_variable_names.push_back( "jet"+i_jet+"_bdtscoreX_"+bdt_tag );
		}
	}

	for( auto lt_rw: list_lifetime_rw_str ) output_variable_names.push_back( "weight_ctau_"+lt_rw );

	// ----- Decorate Tree ----- //

	map<string,Float_t> output_vars;
	map<string,TBranch*> output_branches;

	for( auto output_var: output_variable_names ){
		output_branches[output_var] = tree->Branch(Form("%s", output_var.c_str()), &output_vars[output_var] );
	}

	Long64_t NEntries = tree->GetEntries();
	cout<<"Looping over "<<NEntries<<" events"<<endl;

	for (Long64_t jentry = 0; jentry < NEntries; jentry++) {

		tree->GetEntry(jentry);

		if( jentry % 100000 == 0 )
			cout<<"Processing Entry "<<jentry<<endl;

		// BDT Scores

		for( auto bdt_tag: bdt_tags_booked ){
			if( tree_perJet == bdt_perJet[bdt_tag] ){
				output_vars["bdtscoreX_"+bdt_tag] = GetBDTScore( bdt_tag, input_vars, "-1" );
				continue;
			}
			for( auto i_jet: vector<string>{ "0", "1", "2", "3", "4", "5" } ){
				// float jet0_Pt;
				// tree->SetBranchAddress( "jet0_Pt", &jet0_Pt );
				// cout << jet0_Pt << " jet 0 pT, with BDT score " << GetBDTScore( bdt_tag, input_vars, i_jet ) << endl;
				output_vars["jet"+i_jet+"_bdtscoreX_"+bdt_tag] = GetBDTScore( bdt_tag, input_vars, i_jet ); 
			}
		}

		// Lifetime Reweighting // 
		for( auto lt_rw: list_lifetime_rw_str ){
			if( signal_ctau > 0 ){
				output_vars["weight_ctau_"+lt_rw] = GetLifetimeReweight( signal_ctau, input_vars, std::stof(lt_rw) ); 
			} else{
				output_vars["weight_ctau_"+lt_rw] = input_vars["weight"];
			}
		}

		// Write Branches // 

		for( auto output_var: output_variable_names )
			output_branches[output_var]->Fill();
	
		// if( jentry == 100 ) break;
	}

	// Write Tree

	tree->Write("", TObject::kOverwrite); 

}

/* ====================================================================================================================== */
void AddTreesToFile( string infiletag, vector<string> treenames ){

        cout<<endl;
        cout<<" ----- DECORATING "<<infiletag<<" (vIN = "<<vIN<<", vOUT = "<<vOUT<<") ----- "<<endl;
        cout<<endl;

        clock_t start_clock = clock();

        // ----- Infile / Outfile ----- //

        bool isData = false;
        bool isSignal = false;
        bool isBkgMC = false;
        if( infiletag.find("Run2023") != string::npos ) isData = true;
        if( infiletag.find("CTau") != string::npos ) isSignal = true; 
        if( !isData && !isSignal ) isBkgMC = true;

        // Get Signal Lifetime
        float signal_ctau = -1;
        if( isSignal ) signal_ctau = GetCTauFromInfiletag( infiletag );

        // Infile
        TString infilepath = Form( "%s/minituple_%s.root", infiledir.c_str(), infiletag.c_str() );
        cout<<"Reading in "<<infilepath<<endl;
        TFile *f = new TFile( infilepath, "READ" );

        cout<<" -> isData:   "<<isData<<endl;
        cout<<" -> isSignal: "<<isSignal<<endl;
        cout<<" -> isBkgMC: "<<isBkgMC<<endl;

        map<string,TTree*> trees_input;
        map<string,Long64_t> NEntries_input;
        for( auto treename: treenames ){
                trees_input[treename] = (TTree*) f->Get( Form( "%s", treename.c_str() ) );
                NEntries_input[treename] = trees_input[treename]->GetEntries();
        }

        // Outfile
        TString outfilename = Form( "%s/minituple_%s.root", outfiledir.c_str(), infiletag.c_str() );
        TFile *fout = new TFile(outfilename, "RECREATE");

        for( auto treename: treenames ){
                cout<<"\n ----- Running Over: "<<treename<<" ----- \n"<<endl;
                cout<<"Cloning Tree ... (this step could take approx "<<0.0001*NEntries_input[treename]<<" s)"<<endl;
				        //TFile *fscratch = new TFile("minituple_scratch.root", "RECREATE"); // preventing basket WriteBuffer failed -- but causes actual tree to not be filled! need to debug
                TTree *tree = (TTree*) trees_input[treename]->CloneTree();
                cout<<" -> Processing time: "<<(clock()-start_clock)/(double)CLOCKS_PER_SEC<<" s"<<endl;
		
		            bool tree_perJet = false;
		            if( treename.find( "PerJet" ) != string::npos ) tree_perJet = true;
          
                fout->cd();
                AddBranchesToTree( tree, tree_perJet, signal_ctau );
        }

        cout<<"File written to: "<<outfilename<<endl;
        fout->Close();

        double duration_sec = (clock()-start_clock)/(double)CLOCKS_PER_SEC;
        std::cout<<" --> Run Time = "<<duration_sec<<" sec\n"<<endl;

}

/* ====================================================================================================================== */
void AddBDTScoreBranches( string infiletag, vector<string> treenames ){

	clock_t start_clock = clock();

	//AddTreesToFile( "test", vector<string>{"NoSel","WPlusJets", "PerJet_LLPmatched", "PerJet_NoSel", "PerJet_WPlusJets"} );

	// AddTreesToFile( "job2_8_output_97", vector<string>{ "NoSel" } );

	// AddTreesToFile( "v3.7_LLPskim_Run2023Bv1_2024_03_14", vector<string>{ "NoSel", "WPlusJets" } );
	// AddTreesToFile( "v3.7_LLPskim_Run2023Cv1_2024_03_14", vector<string>{ "NoSel", "WPlusJets" } );
	// AddTreesToFile( "v3.7_LLPskim_Run2023Cv2_2024_03_14", vector<string>{ "NoSel", "WPlusJets" } );
	// AddTreesToFile( "v3.7_LLPskim_Run2023Cv3_2024_03_14", vector<string>{ "NoSel", "WPlusJets" } );
	// AddTreesToFile( "v3.7_LLPskim_Run2023Cv4_2024_03_14", vector<string>{ "NoSel", "WPlusJets" } );
	// AddTreesToFile( "v3.7_LLPskim_Run2023Dv1_2024_03_14", vector<string>{ "NoSel", "WPlusJets" } );
	// AddTreesToFile( "v3.7_LLPskim_Run2023Dv2_2024_03_14", vector<string>{ "NoSel", "WPlusJets" } );

	// AddTreesToFile( "v3.7_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_03_14_TEST", vector<string>{ "NoSel" } );
	// AddTreesToFile( "v3.7_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_03_14_TRAIN", vector<string>{ "NoSel" } );
	// AddTreesToFile( "v3.7_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_03_14_batch1", vector<string>{ "NoSel" } );
	// AddTreesToFile( "v3.7_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_03_14_batch2", vector<string>{ "NoSel" } );
	// AddTreesToFile( "v3.7_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_03_14_batch1", vector<string>{ "NoSel" } );
	// AddTreesToFile( "v3.7_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_03_14_batch2", vector<string>{ "NoSel" } );
	// AddTreesToFile( "v3.7_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_03_14_batch1", vector<string>{ "NoSel" } );
	// AddTreesToFile( "v3.7_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_03_14_batch2", vector<string>{ "NoSel" } );
	// AddTreesToFile( "v3.7_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_03_14_TEST", vector<string>{ "NoSel" } );
	// AddTreesToFile( "v3.7_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_03_14_TRAIN", vector<string>{ "NoSel" } );

    //AddTreesToFile( "HToSSTobbbb_MH-125_MS-50_CTau10000", 1000, vector<string>{ "NoSel" } ); 
    //AddTreesToFile( "HToSSTobbbb_MH-125_MS-50_CTau3000", 300, vector<string>{ "NoSel" } ); 

   	//AddTreesToFile( "HToSSTobbbb_MH-125_MS-15_CTau10000", vector<string>{ "NoSel" } ); 
    //AddTreesToFile( "HToSSTobbbb_MH-125_MS-15_CTau3000", 300, vector<string>{ "NoSel" } ); 
    //AddTreesToFile( "HToSSTobbbb_MH-125_MS-15_CTau1000", 100, vector<string>{ "NoSel" } ); 

	AddTreesToFile( infiletag, treenames );

	std::cout<<"--------------------------------------------------------"<<endl;
	double duration_sec = (clock()-start_clock)/(double)CLOCKS_PER_SEC;
	if( duration_sec < 60 )
		std::cout<<"\n\t PROGRAM DURATION = "<<duration_sec<<" sec\n"<<endl;
	else
		std::cout<<"\n\t PROGRAM DURATION = "<<duration_sec/60.<<" min\n"<<endl;
	std::cout<<"--------------------------------------------------------"<<endl;	

}