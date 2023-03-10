#define DisplacedHcalJetAnalyzer_cxx

using namespace std;

#include "../DisplacedHcalJetAnalyzer/DisplacedHcalJetAnalyzer.h"

#include "../src/Loop.cxx"
#include "../src/EventHelper.cxx"
#include "../src/HistHelper.cxx"
#include "../src/OutputHelper.cxx"

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::Initialize( string infiletag ){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::Initialize()"<<endl;

	SetHistCategories();

	return;
}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer( string infiletag = "", string infilepath = "" ){

	clock_t start_clock = clock();
	
	// ----- Get Infile ----- //
	// NEED TO FIX 
	
	if( infilepath == "" ) { // Path from base dir -- when ONLY file tag is provided

		// Using infiletag (e.g. 362085_2022-11-16_100 )
		string infilename = "";

		// Local & global paths 
		vector<string> infilepaths = {
			// Look locally
			"",
			"../NTuples/02_17_2023", 
			// Look globally -- for files on lxplus
			// Fix path "/afs/cern.ch/work/k/kikenned/public/L1LLPJetStudies/HcalTuples/",
			// Look on cmsxrootd
			// Fix path "root://cmsxrootd.fnal.gov///store/user/kikenned/JetMET/Run2022F-v1_RAW_20221117_002820/221116_232834/0000/", // Run 362085, produced on 2022-11-16
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
	AnalysisReader.save_hists	= true;		// For output histograms
	AnalysisReader.save_trees	= false;	// For minituples
	AnalysisReader.NEvents 		= -1; 		// Run over specified number of events (set to -1 for ALL)

	// ----- Initialize ----- // 

	AnalysisReader.Initialize( infiletag ); 
	
	//TString outfilename = Form( "minituple_%s.root", infiletag.c_str() ); // Not yet
	TString outfilename = Form( "hists_%s.root", infiletag.c_str() );
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