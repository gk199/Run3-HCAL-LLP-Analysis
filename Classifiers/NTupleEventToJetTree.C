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

/*
string basepath = "inputs/root/";
string vIN 		= "v0";
string vOUT		= vIN+".1";

string infiledir  = basepath + vIN;
string outfiledir = basepath + vOUT;
*/

// ----- Selections ----- //

TCut cuts = ""; 


/* ====================================================================================================================== */
float DeltaPhi( float phi1, float phi2){

 	float dphi = phi2 - phi1;
 	float dphiX = dphi;
 	while( dphi > 3.14159 ) dphi -= 2*3.14159; // PBC
 	
 	return dphi;
}

/* ====================================================================================================================== */
float DeltaR( float eta1, float eta2, float phi1, float phi2){

 	float deta = fabs(eta2 - eta1);
 	float dphi = fabs(phi2 - phi1);
 	if( dphi > 3.14159 ) dphi -= 2*3.14159;

 	float dR = sqrt( pow(deta, 2.) + pow(dphi, 2.) );
 	return dR;
}

/* ====================================================================================================================== */
int PassLLPJetMatching( int i_jet, map<string,vector<Float_t>*> input_vars_vec ){

	int i_llp_match = -1;
	float dR_min = 0.4;

	for( int i_llp=0; i_llp<input_vars_vec["gLLP_Pt"]->size(); i_llp++){

		if( fabs(input_vars_vec["gLLP_Eta"]->at(i_llp)) > 1.4 ) continue;
		if( sqrt( pow(input_vars_vec["gLLP_DecayVtx_X"]->at(i_llp), 2.) + pow(input_vars_vec["gLLP_DecayVtx_Y"]->at(i_llp), 2.) ) > 290 ) continue;
		if( sqrt( pow(input_vars_vec["gLLP_DecayVtx_X"]->at(i_llp), 2.) + pow(input_vars_vec["gLLP_DecayVtx_Y"]->at(i_llp), 2.) ) < 180 ) continue;
					
		float dR_temp = DeltaR(input_vars_vec["gLLP_Eta"]->at(i_llp), input_vars_vec["jet_Eta"]->at(i_jet), input_vars_vec["gLLP_Phi"]->at(i_llp), input_vars_vec["jet_Phi"]->at(i_jet) );
		if( dR_temp > dR_min ) continue;

		dR_min = dR_temp;
		i_llp_match = i_llp;
	}

	return i_llp_match;

}

/* ====================================================================================================================== */
bool PassWPlusJetsSelection( map<string,Float_t> input_vars_float, map<string,vector<Float_t>*> input_vars_vec ) {
	// given a event, determine if it is a W + jets event, based off of lepton and jet selections

	if( debug ) cout<<"    - Getting MET vars..."<<endl;

	if( input_vars_float["met_Pt"] < 30 ) return false;										// should this be met pT or eT?

	if( debug ) cout<<"    - Pass MET > 30"<<endl;

	string lep_found = "";

	map<string,bool> pass_selection;
	pass_selection["ele"]  = false;
	pass_selection["muon"] = false;

	map<string,float> phiVectorSum;
	phiVectorSum["ele"]  = -99;
	phiVectorSum["muon"] = -99;

	map<string,int> n_lep_over20;
	n_lep_over20["ele"]  = 0;
	n_lep_over20["muon"] = 0;

	// ----- Check Lepton ----- //

	// Lepton must be over 20 GeV, eta < 2.4, tight ID, isolated, and select the first (highest pT) lepton passing these requriements. Count number of leptons over 20 GeV
	
	for( auto lep: vector<string>{"ele", "muon"}){

		for (int i = 0; i < input_vars_vec[lep+"_Pt"]->size(); i++) {
			// ID & Iso
			if( lep == "ele" ){
				if( input_vars_vec["ele_passCutBasedIDTight"]->at(i) != 1 ) continue;
				//if ( ! IsElectronIsolatedTight(i) ) continue;	
			} else if( lep == "muon" ){
				if( input_vars_vec["muon_IsTight"]->at(i) != 1 ) continue; 
				// if ( ! IsMuonIsolatedTight(i) ) continue; 
			}

			float transverseM = sqrt( 2 * input_vars_vec[lep+"_Pt"]->at(i) * input_vars_float["met_Pt"] * (1 - cos( input_vars_vec[lep+"_Phi"]->at(i) - input_vars_float["met_Phi"] ) ) );

			if( input_vars_vec[lep+"_Pt"]->at(i) > 20 ) n_lep_over20[lep]++;
			if( input_vars_vec[lep+"_Pt"]->at(i) > 20 && abs(input_vars_vec[lep+"_Eta"]->at(i)) < 2.4 && transverseM > 55 && pass_selection[lep] == false) {
				pass_selection[lep] = true;

				float x_vector = input_vars_vec[lep+"_Pt"]->at(i) * cos(input_vars_vec[lep+"_Phi"]->at(i)) + input_vars_float["met_Pt"] * cos(input_vars_float["met_Phi"]);
				float y_vector = input_vars_vec[lep+"_Pt"]->at(i) * sin(input_vars_vec[lep+"_Phi"]->at(i)) + input_vars_float["met_Pt"] * sin(input_vars_float["met_Phi"]);
				phiVectorSum[lep] = atan2(y_vector, x_vector);
				lep_found = lep;
			}

		}

	}

	if( pass_selection["ele"] == pass_selection["muon"] ) return false;		// reject events with none or multiple leptons passing
	if( n_lep_over20["ele"] + n_lep_over20["muon"] > 1 )  return false;		// reject events with additional leptons over 20 GeV

	// ----- Check Jets ----- //

	double jet_lepton_dPhi = -999; 
	for( int i = 0; i < input_vars_vec["jet_Pt"]->size(); i++) { 	

		if( input_vars_vec["jet_Pt"]->at(i) > 30 && abs(input_vars_vec["jet_Eta"]->at(i)) < 1.26 ){
			jet_lepton_dPhi = DeltaR(0,0, input_vars_vec["jet_Phi"]->at(i), phiVectorSum[lep_found] );

			if ( abs(jet_lepton_dPhi) > 2 ) return true;
		}
	}

	return false;

}

/* ====================================================================================================================== */
void SkimEventsMakeJetTree( string infilepath, string outfilepath, string treename, bool isData, bool isSignal ){

	cout<<endl;
	cout<<" ----- DECORATING "<<infilepath<<endl; //" (vIN = "<<vIN<<", vOUT = "<<vOUT<<") ----- "<<endl;
	cout<<endl;

	clock_t start_clock = clock();

	// ----- Infile / Outfile ----- //

	cout<<" -> isData:   "<<isData<<endl;
	cout<<" -> isSignal: "<<isSignal<<endl;
	//cout<<" -> isBkgMC: "<<isBkgMC<<endl;

	// TODO: FIX
	//bool isData = false;
	//bool isSignal = false;
	// bool isBkgMC = false;
	//if( infiletag.find("data") != string::npos ) isData = true;
	//if( infiletag.find("CTau") != string::npos ) isSignal = true; // TODO: FIX 
	//if( !isData && !isSignal ) isBkgMC = true;

	// ----- Define Input Variables & Branches ----- //

	// Infile //

	//TString infilepath = Form( "%s/ntuple_%s.root", infiledir.c_str(), infiletag.c_str() );
	cout<<"Reading in "<<infilepath<<endl;
	TFile *infile = TFile::Open( Form( "%s", infilepath.c_str()), "READ" );

	TTree* tree_in = (TTree*) infile->Get( Form( "%s", treename.c_str() ) );
	/*map<string,TTree*> trees_input;
	map<string,Long64_t> NEntries_input;
	for( auto treename: treenames ){
		trees_input[treename] = (TTree*) f->Get( Form( "%s", treename.c_str() ) );
		NEntries_input[treename] = trees_input[treename]->GetEntries();
	}*/

	// Input Branches //

	vector<string> input_variable_names_uint = {
		"runNum", "lumiNum", //"eventNum",
	};

	vector<string> input_variable_names_int = {
		"n_PV", "n_PVCand", "n_hbheRechit", "n_jet", "n_ele", "n_muon", "n_pho", "n_gLLP", 
	};

	vector<string> input_variable_names_float = {
		"met_Pt", "met_Phi",
	};

	vector<string> input_variable_names_vec = {
		"jet_Pt", "jet_Eta", "jet_Phi",
		"ele_Pt", "ele_Eta", "ele_Phi", "ele_passCutBasedIDTight",
		"muon_Pt", "muon_Eta", "muon_Phi", "muon_IsTight",
		"gLLP_Pt", "gLLP_Eta", "gLLP_Phi", "gLLP_DecayVtx_X",  "gLLP_DecayVtx_Y",  "gLLP_DecayVtx_Z",
		"hbheRechit_E", "hbheRechit_Eta", "hbheRechit_Phi", "hbheRechit_iEta", "hbheRechit_iPhi", "hbheRechit_depth",
	};

	map<string,UInt_t> 					input_vars_uint;
	map<string,Int_t> 					input_vars_int;
	map<string,Float_t> 				input_vars_float;
	map<string,vector<Float_t>*> 		input_vars_vec;
	//map<string,vector<vector<Float_t>>> input_vars_vecvec;
	map<string,TBranch*> input_branches;

	for( auto input_var: input_variable_names_uint ){
		tree_in->SetBranchAddress( Form("%s", input_var.c_str()), &input_vars_uint[input_var], &input_branches[input_var] );
	}

	for( auto input_var: input_variable_names_int ){
		tree_in->SetBranchAddress( Form("%s", input_var.c_str()), &input_vars_int[input_var], &input_branches[input_var] );
	}

	for( auto input_var: input_variable_names_float ){
		tree_in->SetBranchAddress( Form("%s", input_var.c_str()), &input_vars_float[input_var], &input_branches[input_var] );
	}

	for( auto input_var: input_variable_names_vec ){
		tree_in->SetBranchAddress( Form("%s", input_var.c_str()), &input_vars_vec[input_var], &input_branches[input_var] );
	}

	// ----- Define Output Variables & Branches ----- //

	// Outfile // 

	//TString outfilepath = Form( "%s/ntuple_%s.root", outfiledir.c_str(), infiletag.c_str() );
	TFile *outfile = new TFile( Form( "%s", outfilepath.c_str()), "RECREATE" );

	TTree *tree_out = new TTree("JetHCalRechits", "JetHCalRechits");

	outfile->cd();

	// Output Branches //

	vector<string> output_variable_names_uint = input_variable_names_uint;

	vector<string> output_variable_names_int  = input_variable_names_int;
	output_variable_names_int.push_back( "jet_index" );
	output_variable_names_int.push_back( "gLLP_index" );


	vector<string> output_variable_names_float = {
		"jet_Pt", "jet_Eta", "jet_Phi", 
		"gLLP_Pt",  "gLLP_Eta", "gLLP_DecayVtx_R", "gLLP_DecayVtx_Z",
	};

	vector<string> output_variable_names_vec = {
		"rechit_E", "rechit_depth", 
		"rechit_Eta", "rechit_Phi", 
		"rechit_iEta", "rechit_iPhi", 
		"rechit_relEta", "rechit_relPhi", 
		"rechit_reliEta", "rechit_reliPhi", 
	};

	vector<string> output_variable_names_all;

	map<string,UInt_t> 			output_vars_uint;
	map<string,Int_t> 			output_vars_int;
	map<string,Float_t> 		output_vars_float;
	map<string,vector<Float_t>> output_vars_vec;

	map<string,TBranch*> output_branches;

	for( auto output_var: output_variable_names_uint ){
		output_branches[output_var] = tree_out->Branch(Form("%s", output_var.c_str()), &output_vars_uint[output_var] );
		output_variable_names_all.push_back(output_var);
	}

	for( auto output_var: output_variable_names_int ){
		output_branches[output_var] = tree_out->Branch(Form("%s", output_var.c_str()), &output_vars_int[output_var] );
		output_variable_names_all.push_back(output_var);
	}

	for( auto output_var: output_variable_names_float ){
		output_branches[output_var] = tree_out->Branch(Form("%s", output_var.c_str()), &output_vars_float[output_var] );
		output_variable_names_all.push_back(output_var);
	}

	for( auto output_var: output_variable_names_vec ){
		output_branches[output_var] = tree_out->Branch(Form("%s", output_var.c_str()), &output_vars_vec[output_var] );
		output_variable_names_all.push_back(output_var);
	}

	//for( auto treename: treenames ){
	cout<<"\n ----- Running Over: "<<treename<<" ----- \n"<<endl;
	//cout<<"Cloning Tree ... (this step could take approx "<<0.0001*NEntries_input[treename]<<" s)"<<endl;
	//TTree *tree = (TTree*) trees_input[treename]->CloneTree(500); //, "runNum < 10");
	//TTree *tree = new TTree();

	Long64_t NEntries = tree_in->GetEntries();
	cout<<"Looping over "<<NEntries<<" events"<<endl;

	for (Long64_t tentry = 0; tentry < NEntries; tentry++) {

		tree_in->GetEntry(tentry);

		if( tentry % 1000 == 0 )
			cout<<"Processing Entry "<<tentry<<endl;

		// ----- Event Selection ----- //

		if( isSignal ){

			// 
			for( int i_llp=0; i_llp<input_vars_vec["gLLP_Pt"]->size(); i_llp++){

				if( fabs(input_vars_vec["gLLP_Eta"]->at(i_llp)) > 1.4 ) continue;
				if( sqrt( pow(input_vars_vec["gLLP_DecayVtx_X"]->at(i_llp), 2.) + pow(input_vars_vec["gLLP_DecayVtx_Y"]->at(i_llp), 2.) ) > 290 ) continue;
				if( sqrt( pow(input_vars_vec["gLLP_DecayVtx_X"]->at(i_llp), 2.) + pow(input_vars_vec["gLLP_DecayVtx_Y"]->at(i_llp), 2.) ) < 180 ) continue;
			}


		}
		if ( isData ) { // W+Jets Selection
			if( debug ) cout<<"  - Checking PassWPlusJetsSelection..."<<endl;
			if( !PassWPlusJetsSelection( input_vars_float, input_vars_vec) ) continue;
		}

		// ----- Loop Over Jets ----- //

		for( int i_jet=0; i_jet<input_vars_vec["jet_Pt"]->size(); i_jet++ ){
			if( debug ) cout<<"Running over jet "<<i_jet<<", pt = "<<input_vars_vec["jet_Pt"]->at(i_jet)<<endl;
		
			// Jet Object Selection //

			if( input_vars_vec["jet_Pt"]->at(i_jet) < 40 || fabs( input_vars_vec["jet_Eta"]->at(i_jet) ) > 1.24 || i_jet > 2 ) continue;

			if( debug ) cout<<"  - Pass jet object selection..."<<endl;

			// LLP Matching for Signal //

			if( isSignal ){
				if( debug ) cout<<"  - Checking PassLLPJetMatching..."<<endl;

				int i_llp = PassLLPJetMatching( i_jet, input_vars_vec );
				if( i_llp < 0 ) continue;

				output_vars_int["gLLP_index"] = i_llp;
				output_vars_float["gLLP_Pt"]  = input_vars_vec["gLLP_Pt"]->at(i_llp);
				output_vars_float["gLLP_Eta"] = input_vars_vec["gLLP_Eta"]->at(i_llp);
				output_vars_float["gLLP_DecayVtx_R"] = sqrt( pow(input_vars_vec["gLLP_DecayVtx_X"]->at(i_llp), 2.) + pow(input_vars_vec["gLLP_DecayVtx_Y"]->at(i_llp), 2.) );
				output_vars_float["gLLP_DecayVtx_Z"] = input_vars_vec["gLLP_DecayVtx_Z"]->at(i_llp);

			} 

			if( debug ) cout<<"  - Pass event selection, running HCalRecHit"<<endl;

			// HCalRecHit Matching // 

			for( auto var: output_variable_names_vec )
				output_vars_vec[var].clear();

			for( int i_rechit=0; i_rechit<input_vars_vec["hbheRechit_E"]->size(); i_rechit++){
				if( DeltaR(input_vars_vec["hbheRechit_Eta"]->at(i_rechit), input_vars_vec["jet_Eta"]->at(i_jet), input_vars_vec["hbheRechit_Phi"]->at(i_rechit), input_vars_vec["jet_Phi"]->at(i_jet) ) > 0.4 ) continue;

				// Set Variables

				output_vars_vec["rechit_E"].push_back( input_vars_vec["hbheRechit_E"]->at(i_rechit) );
				output_vars_vec["rechit_depth"].push_back( input_vars_vec["hbheRechit_depth"]->at(i_rechit) );

				output_vars_vec["rechit_Eta"].push_back( input_vars_vec["hbheRechit_Eta"]->at(i_rechit) );
				output_vars_vec["rechit_Phi"].push_back( input_vars_vec["hbheRechit_Phi"]->at(i_rechit) );

				output_vars_vec["rechit_iEta"].push_back( input_vars_vec["hbheRechit_iEta"]->at(i_rechit) );
				output_vars_vec["rechit_iPhi"].push_back( input_vars_vec["hbheRechit_iPhi"]->at(i_rechit) );

				//float relEta, relPhi, reliEta, reliPhi;
				float relEta = input_vars_vec["hbheRechit_Eta"]->at(i_rechit) - input_vars_vec["jet_Eta"]->at(i_jet);
				float relPhi = DeltaPhi( input_vars_vec["hbheRechit_Phi"]->at(i_rechit), input_vars_vec["jet_Phi"]->at(i_jet)); //DeltaR(0, 0, input_vars_vec["jet_Phi"]->at(i_jet), input_vars_vec["hbheRechit_Phi"]->at(i_rechit));

				output_vars_vec["rechit_relEta"].push_back( relEta );
				output_vars_vec["rechit_relPhi"].push_back( relPhi );

				float iEtaBin = 0.087;
				float iPhiBin = 0.087266; // = 2pi/72
				float reliEta = floor(relEta/iEtaBin+5);
				float reliPhi = floor(relPhi/iPhiBin+5);

				if( debug ) cout<<relEta<<" -> "<<reliEta<<"  |  "<<relPhi<<" -> "<<reliPhi<<endl;

				output_vars_vec["rechit_reliEta"].push_back( reliEta );
				output_vars_vec["rechit_reliPhi"].push_back( reliPhi );

			}

			output_vars_int["jet_index"] = i_jet;
			output_vars_float["jet_Pt"]  = input_vars_vec["jet_Pt"]->at(i_jet);
			output_vars_float["jet_Eta"] = input_vars_vec["jet_Eta"]->at(i_jet);
			output_vars_float["jet_Phi"] = input_vars_vec["jet_Phi"]->at(i_jet);

			for( auto var: output_variable_names_uint ) output_vars_uint[var] = input_vars_uint[var];
			for( auto var: output_variable_names_int  ) output_vars_int[var]  = input_vars_int[var];
			
			// Write Branches // 

			tree_out->Fill();

		}

		if( tentry == 10000 ) break;
	}

	// ----- Write Tree ---- // 

	tree_out->Write("", TObject::kOverwrite); 

	cout<<" -> Processing time: "<<(clock()-start_clock)/(double)CLOCKS_PER_SEC<<" s"<<endl;

	cout<<"File written to: "<<outfilepath<<endl;
	outfile->Close();

	double duration_sec = (clock()-start_clock)/(double)CLOCKS_PER_SEC;
	std::cout<<" --> Run Time = "<<duration_sec<<" sec\n"<<endl;

}

/* ====================================================================================================================== */
void NTupleEventToJetTree2(string infilepath = "", string outfilepath = "" ){
    
	cout << "here3" << endl;
	clock_t start_clock = clock();

	//SkimEventsMakeJetTree( "HToSSTo4B_MH250_MS120_CTau10000_output_101", "DisplacedHcalJets/Events", false, true );
	//SkimEventsMakeJetTree( "Run2023D-EXOLLPJetHCAL-PromptReco-v2_output_1-15", "DisplacedHcalJets/Events", true, false );
	//SkimEventsMakeJetTree( "../../Run3-HCAL-LLP-Analysis/NTuples/2024-02-29/ntuple_Run2023D-EXOLLPJetHCAL-PromptReco-v2_output_1-15.root", "test.root", "DisplacedHcalJets/Events", true, false );
	//SkimEventsMakeJetTree( "../../Run3-HCAL-LLP-Analysis/NTuples/2024-02-29/ntuple_HToSSTo4B_MH250_MS120_CTau10000_output_101.root", "testSignal.root", "DisplacedHcalJets/Events", false, true );


	if( infilepath.find( "Run202") != string::npos ) // For Data
		SkimEventsMakeJetTree( infilepath, outfilepath, "DisplacedHcalJets/Events", true, false );
	else // For Signal
		SkimEventsMakeJetTree( infilepath, outfilepath, "DisplacedHcalJets/Events", false, true );

	std::cout<<"--------------------------------------------------------"<<endl;
	double duration_sec = (clock()-start_clock)/(double)CLOCKS_PER_SEC;
	if( duration_sec < 60 )
		std::cout<<"\n\t PROGRAM DURATION = "<<duration_sec<<" sec\n"<<endl;
	else
		std::cout<<"\n\t PROGRAM DURATION = "<<duration_sec/60.<<" min\n"<<endl;
	std::cout<<"--------------------------------------------------------"<<endl;	

}

/* ====================================================================================================================== */
int main(int argc, char** argv) { // For running in compiled mode
	  
	if( argc > 2 ){
		cout << "heree" << endl;        
		NTupleEventToJetTree2( argv[1], argv[2] );}

	else {
		cout<<"ERROR: Not enough arguments!"<<endl;
	}    
}


