#define DisplacedHcalJetAnalyzer_cxx

#include "../DisplacedHcalJetAnalyzer/DisplacedHcalJetAnalyzer.h"

#include "../src/Loop.cxx"
#include "../src/EventHelper.cxx"
#include "../src/TriggerHelper.cxx"
#include "../src/ObjectHelper.cxx"
#include "../src/HcalRechitHelper.cxx"
#include "../src/HistHelper.cxx"
#include "../src/OutputHelper.cxx"
#include "../src/BDTHelper.cxx"
#include "../src/TruthInfoHelper.cxx"
#include "../src/WeightsHelper.cxx"
#include "../src/JetVetoMapHelper.cxx"

// gSystem->Load("/Users/kiley/Documents/CMS/WorkingDir/Run3-HCAL-LLP-Analysis/pugixml/pugixml_cpp.so");


/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::Initialize( string infiletag, string infilepath ){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::Initialize()"<<endl;

	// Initialize trigger names from NEvents_HLT histogram
	SetTriggerNames( infilepath, "DisplacedHcalJets/NEvents_HLT");

	// Set Categories
	SetHistCategories();

	// Initialize TMVA Reader
	InitializeTMVA(); 

	// Lifetime Reweighting
	InitializeLifetimeReweighting( infilepath );

	// Weights for MC
	SetWeight( infilepath ); // and then added as a branch in OutputHelper.cxx

	return;
}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer( string infiletag = "", vector<string> infilepaths = {} ){

	clock_t start_clock = clock();

	cout<<"\n ----- INITIALIZING ----- \n"<<endl;


	//gSystem->Load("/Users/kiley/Documents/CMS/WorkingDir/Run3-HCAL-LLP-Analysis/pugixml/pugixml_cpp.so");
	
	// ----- Read in File & Tree ----- // 

	cout<<"\nAdding the following files to chain:"<<endl;

	TChain *chain = new TChain("DisplacedHcalJets/Events");
    chain->SetCacheSize(50 * 1024 * 1024); // 50 MB
    chain->SetCacheLearnEntries(10);

	int n_files = 0;
	for( auto infilepath: infilepaths ){
		if( gSystem->AccessPathName( Form("%s", infilepath.c_str() ) ) )
			cout<<"WARNING: Could not find input file: "<<infilepath<<endl;

		cout<<infilepath<<endl;
		chain->Add( Form("%s", infilepath.c_str() ) );
		n_files++;
	}

	if( n_files == 0 ){
		cout<<"ERROR: Could not find any input files. Exiting program."<<endl;
		return;
	}

	//TFile *f = new TFile( Form("%s", infilepath.c_str() ) );
	//cout<<"Reading Tree..."<<endl;
	//TTree *t;
	//TDirectory * dir = (TDirectory*)f->Get(Form("%s:/hcalTupleTree", infilepath.c_str()));
	//dir->GetObject("tree",t);

	// ----- Declare Class Instance ----- // 

	class DisplacedHcalJetAnalyzer AnalysisReader(chain);

	// ----- Modify Properties ----- // 

	AnalysisReader.debug		= true; 
	AnalysisReader.print_counts	= true;
	AnalysisReader.save_hists	= false;	// For output histograms
	AnalysisReader.save_trees	= true;		// For minituples
	AnalysisReader.NEvents 		= -1; 		// Run over specified number of events (set to -1 for ALL)
	AnalysisReader.randomGenerator->SetSeed(0);

	// ----- Load Jet Veto Maps ----- //
	// these are from https://cms-jerc.web.cern.ch/Recommendations/#jet-veto-maps
	// AnalysisReader.vetoMaps_["Summer22_23Sep2023"]     = AnalysisReader.LoadJetVetoMap("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_13_2_0/src/cms_lpc_llp/Run3-HCAL-LLP-NTupler/data/JEC_JER/JECDatabase/jet_veto_maps/Summer22_23Sep2023/Summer22_23Sep2023_RunCD_v1.root");
	// copied to local directory, as these files are also transfered with condor. Currently in Run/ directory
	AnalysisReader.vetoMaps_["Summer22_23Sep2023"]     = AnalysisReader.LoadJetVetoMap("Summer22_23Sep2023_RunCD_v1.root");
    AnalysisReader.vetoMaps_["Summer22EE_23Sep2023"]   = AnalysisReader.LoadJetVetoMap("Summer22EE_23Sep2023_RunEFG_v1.root");
    AnalysisReader.vetoMaps_["Summer23Prompt23"]       = AnalysisReader.LoadJetVetoMap("Summer23Prompt23_RunC_v1.root");
    AnalysisReader.vetoMaps_["Summer23BPixPrompt23"]   = AnalysisReader.LoadJetVetoMap("Summer23BPixPrompt23_RunD_v1.root");

	// ----- Initialize ----- // 


	AnalysisReader.Initialize( infiletag, infilepaths.at(0) ); 
	
	//TString outfilename = Form( "minituple_%s.root", infiletag.c_str() ); // Not yet
	TString outfilename = Form( "minituple_%s.root", infiletag.c_str() );
	TFile *fout = new TFile( outfilename, "RECREATE" );

	// ----- Run!!! ----- // 

	AnalysisReader.Loop(); // Runs over number of events specified by Nevents (above)

	cout<<"File written to: "<<outfilename<<endl;
	fout->Close();

	std::cout<<"--------------------------------------------------------"<<endl;
	double duration_sec = (clock()-start_clock)/(double)CLOCKS_PER_SEC;
	if( duration_sec < 60 )
		std::cout<<"\n\t PROGRAM DURATION = "<<duration_sec<<" sec\n"<<endl;
	else
		std::cout<<"\n\t PROGRAM DURATION = "<<duration_sec/60.<<" min\n"<<endl;
	std::cout<<"--------------------------------------------------------"<<endl;	

}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer( string infiletag = "", string infilepath = "" ){

	vector<string> infilepaths = { infilepath };
	DisplacedHcalJetAnalyzer( infiletag, infilepaths );

}

/* ====================================================================================================================== */
int main(int argc, char** argv) { // For running in compiled mode

	int Nargs = argc;

	if( Nargs > 2 ){
	    vector<string> infilepaths;
	    for( int i = 2; i < Nargs; i++ ) infilepaths.push_back( argv[i] ); 

	    DisplacedHcalJetAnalyzer( argv[1], infilepaths );

	} else {
		cout<<"ERROR: Not enough arguments!"<<endl;
	}

}
