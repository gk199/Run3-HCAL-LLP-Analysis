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

string basepath = "../MiniTuples/";
string vIN 		= "v3.4";
string vOUT		= vIN+".1";

string infiledir  = basepath + vIN;
string outfiledir = basepath + vOUT;

// ----- Selections ----- //

TCut cuts = ""; 

// ----- Lifetime Reweighting ----- //

vector<string> list_lifetime_rw_str = { "0.5", "1", "2", "4", "5", "6", "10", "15", "20", "25", "30", "40", "50" };
vector<double> list_lifetime_rw     = {  0.5 ,  1.,  2.,  4.,  5.,  6.,  10.,  15.,  20.,  25.,  30.,  40.,  50. };

// ----- BDT Globals ----- //

string bdt_version = "v0.4";

vector<string> bdt_tags = { "LLP350", "LLP125" };
map<string,TMVA::Reader*> bdt_reader;
map<string,vector<string>> bdt_var_names;
map<string,Float_t> bdt_vars;

/* ====================================================================================================================== */
Float_t GetLifetimeReweight( string filetag, double event_weight, double tau_event, double tau_target ){

	// TODO: adapt to displaced jets samples

	double signal_lifetime = -1;
	// TO DO: READ IN from filetag
	cout<<"Reading signal lifetime as "<<signal_lifetime<<endl;

	double lt_rw = event_weight * pow ( signal_lifetime / tau_target, 0.5 ) * exp( -tau_event * ( 1.0/tau_target - 1.0/signal_lifetime ) );
	//double lt_rw = event_weight * pow ( signal_lifetime / tau_target, 1.0 ) * exp( -tau_event * ( 1.0/tau_target - 1.0/signal_lifetime ) );

	return lt_rw;

	/* Safety checks for low stats...
	if( lt_rw > 1 && filetag.find("_100_") == string::npos ) {
		return 1.0;
	} else if ( lt_rw > 10 && lt_rw/event_weight > 50 && filetag.find("_100_") != string::npos ){
		return 50*event_weight;
	} else {
		return lt_rw;
	}*/

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
float GetBDTScore( string bdt_tag, map<string,Float_t> input_vars ){

	for( auto bdt_var_name: bdt_var_names[bdt_tag] ){
		bdt_vars[bdt_tag+" "+bdt_var_name] = input_vars[bdt_var_name];
	}

	return bdt_reader[bdt_tag]->EvaluateMVA("BDT");
}

/* ====================================================================================================================== */
void AddBranchesToTree( TTree* tree ){

	// ----- Define Input Variables & Branches ----- //

	vector<string> input_variable_names = { // As read from MiniTuple
		//"jet0_Pt",
	};

	// Book TMVA Reader //
	
	for( auto bdt_tag: bdt_tags ){ 

		bool pass = BookTMVAReader( bdt_tag );
		if( pass == false ) cout<<"WARNING: Unable to book TMVA Reader for bdt_tag "<<bdt_tag<<" -- file not found?"<<endl;
		
		for( auto var: bdt_var_names[bdt_tag] ){
			input_variable_names.push_back( var );
		}
	}

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

	for( auto bdt_tag: bdt_tags ){
		output_variable_names.push_back( "bdtscoreX_"+bdt_tag );
	}

	//for( auto lt_rw: list_lifetime_rw_str ) 
	//	output_variable_names.push_back( "lifetime_rw_to_"+lt_rw );

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

		for( auto bdt_tag: bdt_tags ){
			output_vars["bdtscoreX_"+bdt_tag] = GetBDTScore( bdt_tag, input_vars );
		}

		// Lifetime Reweighting // 

		/*for( int i=0; i<list_lifetime_rw.size(); i++ ){
			if( isSignal ){
				output_vars["lifetime_rw_to_"+list_lifetime_rw_str.at(i)] = GetLifetimeReweight( infiletag, input_vars["weight"], input_vars["tau_Hino"], list_lifetime_rw.at(i) );
			} else{
				output_vars["lifetime_rw_to_"+list_lifetime_rw_str.at(i)] = input_vars["weight"];
			}
		}*/

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
        if( infiletag.find("data") != string::npos ) isData = true;
        if( infiletag.find("CTau") != string::npos ) isSignal = true; // TODO: FIX 
        if( !isData && !isSignal ) isBkgMC = true;

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
                TTree *tree = (TTree*) trees_input[treename]->CloneTree();
                cout<<" -> Processing time: "<<(clock()-start_clock)/(double)CLOCKS_PER_SEC<<" s"<<endl;

                fout->cd();
                AddBranchesToTree( tree );
        }

        cout<<"File written to: "<<outfilename<<endl;
        fout->Close();

        double duration_sec = (clock()-start_clock)/(double)CLOCKS_PER_SEC;
        std::cout<<" --> Run Time = "<<duration_sec<<" sec\n"<<endl;

}

/* ====================================================================================================================== */
void AddBDTScoreBranches(){

	clock_t start_clock = clock();

	AddTreesToFile( "test", vector<string>{"NoSel","WPlusJets"} );

	std::cout<<"--------------------------------------------------------"<<endl;
	double duration_sec = (clock()-start_clock)/(double)CLOCKS_PER_SEC;
	if( duration_sec < 60 )
		std::cout<<"\n\t PROGRAM DURATION = "<<duration_sec<<" sec\n"<<endl;
	else
		std::cout<<"\n\t PROGRAM DURATION = "<<duration_sec/60.<<" min\n"<<endl;
	std::cout<<"--------------------------------------------------------"<<endl;	

}

