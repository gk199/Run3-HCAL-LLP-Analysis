#define DisplacedHcalJetAnalyzer_cxx

#include "../DisplacedHcalJetAnalyzer/DisplacedHcalJetAnalyzer.h"

#include "../src/Loop.cxx"
#include "../src/EventHelper.cxx"
#include "../src/TriggerHelper.cxx"
#include "../src/ObjectHelper.cxx"
#include "../src/HistHelper.cxx"
#include "../src/OutputHelper.cxx"
#include "../src/BDTHelper.cxx"
#include "../src/TruthInfoHelper.cxx"

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

	//BookTMVAReader( bdt_version );

	//MyTags jet_based = MyTags(/*event_based=*/ false, /*calor_only=*/ false);
	//DeclareTMVAReader(jet_based);
	//MyTags jet_based_calor = MyTags(/*event_based=*/ false, /*calor_only=*/ true);
	//DeclareTMVAReader(jet_based_calor);
	//MyTags event_based = MyTags(/*event_based=*/ true, /*calor_only=*/ false);
	//DeclareTMVAReader(event_based);

	return;
}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer( string infiletag = "", string infilepath = "" ){

	clock_t start_clock = clock();

	//gSystem->Load("/Users/kiley/Documents/CMS/WorkingDir/Run3-HCAL-LLP-Analysis/pugixml/pugixml_cpp.so");
	
	// ----- Get Infile ----- //
	// NEED TO FIX 
	
	if( infilepath == "" ) { // Path from base dir -- when ONLY file tag is provided

		// Using infiletag (e.g. 362085_2022-11-16_100 )
		string infilename = "";

		// Local & global paths 
		vector<string> infilepaths = {
			// Look locally
			"",
			"../NTuples/02_17_2023/", 
			// Look globally -- for files on lxplus
			// Fix path "/afs/cern.ch/work/k/kikenned/public/L1LLPJetStudies/HcalTuples/",
			// Look on cmsxrootd
			"root://cmsxrootd.fnal.gov///store/user/kikenned/JetMET/Run2022G-EXOHighMET-PromptReco-v1_RAW-RECO_20230509_041536/230509_021544/0000/", // High MET
			"root://cmsxrootd.fnal.gov///store/user/gkopp/ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV/LLP_MC_test__20230511_194008/230511_174021/0000/", // MC signal
		};

		// Get filepath
		bool filepath_exists = false;
		for( int i=0; i<infilepaths.size(); i++ ){
			if( !gSystem->AccessPathName( Form("%s%s", infilepaths.at(i).c_str(), infilename.c_str() ) ) ){
				infilepath = infilepaths.at(i)+infilename;
				filepath_exists = true;
				break;
			}
		}

		if( !filepath_exists ){
			cout<<"ERROR: could not find input file, exiting program. Checked the following file paths:"<<endl;
			for( int i=0; i<infilepaths.size(); i++ ){
				cout<<infilepaths.at(i)+infilename<<endl;
			}
			return;
		}
	}

	// ----- Read in File & Tree ----- // 

	cout<<"Reading in "<<infilepath<<endl;
	TChain *chain = new TChain("DisplacedHcalJets/Events");
	chain->Add( Form("%s", infilepath.c_str() ) );

	//TFile *f = new TFile( Form("%s", infilepath.c_str() ) );
	//cout<<"Reading Tree..."<<endl;
	//TTree *t;
	//TDirectory * dir = (TDirectory*)f->Get(Form("%s:/hcalTupleTree", infilepath.c_str()));
	//dir->GetObject("tree",t);

	// ----- Declare Class Instance ----- // 

	class DisplacedHcalJetAnalyzer AnalysisReader(chain);

	// ----- Modify Properties ----- // 

	AnalysisReader.debug		= false; 
	AnalysisReader.print_counts	= true;
	AnalysisReader.save_hists	= false;	// For output histograms
	AnalysisReader.save_trees	= true;		// For minituples
	AnalysisReader.NEvents 		= 100; //-1; 		// Run over specified number of events (set to -1 for ALL)

	// ----- Initialize ----- // 

	AnalysisReader.Initialize( infiletag, infilepath ); 
	
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
int main(int argc, char** argv) { // For running in compiled mode

	int Nargs = argc;

	if( Nargs == 2 )
		DisplacedHcalJetAnalyzer( argv[1], "" );
  
	else if( Nargs > 2 )
		DisplacedHcalJetAnalyzer( argv[1], argv[2] );

	else 
		cout<<"ERROR: Too enough arguments!"<<endl;

}
