#include "MiniTuplePlotter.h"
#include "TString.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_HLT_Effs(){

//	string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.1/minituple_";
//	vector<string> filetags_example1 = { "v1.1_LLPskim_500k_2023_08_09", "v1.1_MCsignal_500k_2023_08_09" };

	string path = "/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Run/hists_";
	vector<string> filetags_example1 = { "HLT_17Aug_Data", "HLT_17Aug_MC" };
	
	// ----- Jet kinematics split by HLT paths passed -----//

	cout<<endl;
	cout<<" ---------- HLT Study 1: Jet kinematics split by HLT paths passed ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_example1( filetags_example1, path );
	plotter_example1.SetPlots({P_jet0_E, P_jet0_Pt, P_jet0_Eta, P_jet0_Phi}); // These "P_" variables are PlotParams structs defined in PlotParams.h
	plotter_example1.SetTreeName( "NoSel" );	// TreeName
	plotter_example1.SetOutputFileTag("HLT_v1.1"); 							// Your own special name :)

	plotter_example1.plot_norm 		  = false; 	// Default = true
	plotter_example1.plot_log  		  = false; 	// Default = true
	plotter_example1.stamp_counts 	  = true;	// Default = false. Print event count and integral in legend

	TCut Cut_None			= "";
	TCut Cut_HLTpassed1 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet35_Inclusive1PtrkShortSig5 == 1";
	TCut Cut_HLTpassed2 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1";
	TCut Cut_HLTpassed3 	= "HLT_HT240_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1";
	TCut Cut_HLTpassed4 	= "HLT_HT280_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1";
	TCut Cut_HLTpassed5 	= "HLT_HT170_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1";
	TCut Cut_HLTpassed6 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1";
	TCut Cut_HLTpassed7 	= "HLT_HT270_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1";
	TCut Cut_HLTpassed8 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet60_DisplacedTrack == 1";
	TCut Cut_HLTpassed9 	= "HLT_HT320_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1";
	TCut Cut_HLTpassed10 	= "HLT_HT420_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1";
	TCut Cut_HLTpassed11 	= "HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay0p5nsTrackless == 1";
	TCut Cut_HLTpassed12 	= "HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay1nsInclusive == 1";

	plotter_example1.SetComparisonCuts({Cut_None, Cut_HLTpassed1, Cut_HLTpassed2, Cut_HLTpassed3}); //, Cut_HLTpassed2, Cut_HLTpassed3, Cut_HLTpassed4, Cut_HLTpassed5, Cut_HLTpassed6, Cut_HLTpassed7, Cut_HLTpassed8, Cut_HLTpassed9, Cut_HLTpassed10, Cut_HLTpassed11, Cut_HLTpassed12});

	plotter_example1.Plot("ratio");				// This is what does stuff -- arguments: None/"", "ratio", "sig"/"ssqrtb"

}

