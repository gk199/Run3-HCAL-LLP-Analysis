#include "MiniTuplePlotter.h"
#include "TString.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_HLT_Effs(){

	string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.1/minituple_";
	vector<string> filetags_both = { "v1.1_LLPskim_500k_2023_08_17", "v1.1_MCsignal_500k_2023_08_17" };
	vector<string> filetags_data = { "v1.1_LLPskim_500k_2023_08_17" };
	vector<string> filetags_MC = { "v1.1_MCsignal_500k_2023_08_17" };
	
	// ----- Jet kinematics split by HLT paths passed -----//

	cout<<endl;
	cout<<" ---------- HLT Study 1: Jet kinematics split by HLT paths passed ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_example1( filetags_MC, path );
	plotter_example1.SetPlots({P_jet0_E, P_jet0_Pt, P_jet0_Eta, P_jet0_Phi}); // These "P_" variables are PlotParams structs defined in PlotParams.h
	plotter_example1.SetTreeName( "NoSel" );	// TreeName
	plotter_example1.SetOutputFileTag("HLT_v1.1_MC"); 							// Your own special name :)

	plotter_example1.plot_norm 		  = false; 	// Default = true
//	plotter_example1.plot_log  		  = false; 	// Default = true
//	plotter_example1.stamp_counts 	  = true;	// Default = false. Print event count and integral in legend
	plotter_example1.plot_log_ratio   = true; 	// Default = false. Make bottom panel log scale
	plotter_example1.SetLegendManual( 0.35, 0.65, 0.9, 0.9 );					// Manual Legend location

	TCut Cut_None			= "";
	TCut Cut_HLTpassed1 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet35_Inclusive1PtrkShortSig5 == 1"; 
	TCut Cut_HLTpassed2 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1";
	TCut Cut_HLTpassed3 	= "HLT_HT240_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1";
	TCut Cut_HLTpassed4 	= "HLT_HT280_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1"; // end of group 1 of triggers
	TCut Cut_HLTpassed5 	= "HLT_HT170_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1"; 
	TCut Cut_HLTpassed6 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1";
	TCut Cut_HLTpassed7 	= "HLT_HT270_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1";
	TCut Cut_HLTpassed8 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet60_DisplacedTrack == 1"; // end of group 2 of triggers
	TCut Cut_HLTpassed9 	= "HLT_HT320_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1";
	TCut Cut_HLTpassed10 	= "HLT_HT420_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1"; // end of group 3 of triggers
	TCut Cut_HLTpassed11 	= "HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay0p5nsTrackless == 1";
	TCut Cut_HLTpassed12 	= "HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay1nsInclusive == 1"; 
	TCut Cut_HLTpassed13 	= "HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay1nsTrackless == 1";
	TCut Cut_HLTpassed14 	= "HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay2nsInclusive == 1"; // end of group 4 of triggers
	TCut Cut_HLTpassed15 	= "HLT_L1SingleLLPJet == 1"; // monitoring trigger

	plotter_example1.SetComparisonCuts({Cut_None, Cut_HLTpassed2, Cut_HLTpassed5, Cut_HLTpassed9, Cut_HLTpassed11}); //, Cut_HLTpassed2, Cut_HLTpassed3, Cut_HLTpassed4, Cut_HLTpassed5, Cut_HLTpassed6, Cut_HLTpassed7, Cut_HLTpassed8, Cut_HLTpassed9, Cut_HLTpassed10, Cut_HLTpassed11, Cut_HLTpassed12});

	plotter_example1.Plot("ratio");				// This is what does stuff -- arguments: None/"", "ratio", "sig"/"ssqrtb"

}
