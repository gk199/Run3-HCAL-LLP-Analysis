#include "../MiniTuplePlotter.h"
#include "TString.h"

// Goals of this study: DNN scores
// Mostly worked on during June 2025
// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_DNN(){

	// List where minituples are stored
	
	string path  = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_";

	vector<string> filetags = { "HToSSTo4b_125_50_CTau3000_batch2_scores", "LLPskim_2023Cv4_scores" }; 
	vector<string> filetags_all_signal = {//"HToSSTo4b_125_15_CTau10000_scores", "HToSSTo4b_125_15_CTau3000_scores", "HToSSTo4b_125_50_CTau10000_scores",
											"HToSSTo4b_125_50_CTau3000_batch1_scores", "HToSSTo4b_125_50_CTau3000_batch2_scores", "HToSSTo4b_250_120_CTau10000_batch1_scores",
											"HToSSTo4b_250_120_CTau10000_batch2_scores", "HToSSTo4b_350_160_CTau10000_batch1_scores", "HToSSTo4b_350_160_CTau10000_batch2_scores",
											"HToSSTo4b_350_80_CTau500_scores"};
	vector<string> filetags_all_data = { "LLPskim_2023Bv1_scores", "LLPskim_2023Cv1_scores", "LLPskim_2023Cv2_scores", "LLPskim_2023Cv3_scores", "LLPskim_2023Cv4_scores", "LLPskim_2023Dv1_scores", "LLPskim_2023Dv2_scores" }; 


	// ----- Example 1 -----//
	// - Basic Booleans

	cout<<endl;
	cout<<" ---------- EXAMPLE 1 ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter DNNscores( filetags, path );
	TCut Cut_jet0_inclusive 	= "jet0_InclTagCand == 1";
	TCut Cut_jet1_depth 		= "jet1_DepthTagCand == 1";  

	DNNscores.SetTreeName( "NoSel" );	// TreeName
	DNNscores.SetPlots({P_jet0_scores_inc_train40, P_jet0_scores_inc_train80}); // These "P_" variables are PlotParams structs defined in PlotParams.h
	DNNscores.SetOutputFileTag("DNNscores"); 						
	DNNscores.plot_norm 		  = true; 	// Default = true
	DNNscores.plot_log  		  = true; 	// Default = true
	DNNscores.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5 && jet0_InclTagCand == 1"); 		// Apply cuts to all events
	// DNNscores.SetComparisonCuts({Cut_jet0_inclusive, Cut_jet1_depth}); 					// Apply cuts to overlay
	// DNNscores.SetSelectiveCuts("5k", "jet0_energy > 200");								// Apply this only to filetag names that include the string "data"
	DNNscores.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );									// Manual Legend location
	DNNscores.SetLegendNames({"LLP 125,50", "Cv4"});
	DNNscores.colors = {kBlue, kOrange, kGreen+2, kRed};									// Your own colors (default kBlack + rainbow)
	DNNscores.NBins = 30; 									 								// Default = 100
	DNNscores.use_weight = true;
	DNNscores.Plot();									 								

	DNNscores.ClearFileTrees();
	DNNscores.SetPlots({P_jet0_scores_inc_train40, P_jet0_scores_inc_train80});
	DNNscores.SetOutputFileTag("DNNscores_truthMatch");
	DNNscores.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5"); 		
	DNNscores.SetSelectiveCuts("CTau", "jet0_isTruthMatched == 1");
	DNNscores.SetSelectiveCuts("LLPskim", "Pass_WPlusJets");
	DNNscores.use_weight = true;
	DNNscores.Plot();

	DNNscores.ClearFileTrees();
	DNNscores.SetPlots({P_jet0_scores_inc_train80});
	DNNscores.SetOutputFileTag("DNNscores_jet1Depth");
	DNNscores.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5 && jet0_InclTagCand == 1 && jet1_DepthTagCand == 1"); 		
	DNNscores.use_weight = true;
	DNNscores.Plot();

	class MiniTuplePlotter DNNscores_allMC( filetags_all_signal, path );

	DNNscores_allMC.SetTreeName( "NoSel" );	// TreeName
	DNNscores_allMC.SetPlots({P_jet0_scores_inc_train40, P_jet0_scores_inc_train80}); // These "P_" variables are PlotParams structs defined in PlotParams.h
	DNNscores_allMC.SetOutputFileTag("DNNscores_allMC"); 						
	DNNscores_allMC.plot_norm 		  = true; 	// Default = true
	DNNscores_allMC.plot_log  		  = true; 	// Default = true
	DNNscores_allMC.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5 && jet0_InclTagCand == 1"); 		// Apply cuts to all events
	// DNNscores_allMC.SetComparisonCuts({Cut_jet0_inclusive, Cut_jet1_depth}); 					// Apply cuts to overlay
	// DNNscores_allMC.SetSelectiveCuts("5k", "jet0_energy > 200");								// Apply this only to filetag names that include the string "data"
	DNNscores_allMC.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );										// Manual Legend location
	DNNscores_allMC.SetLegendNames({"125, 50", "125, 50", "250, 120", "250, 120", "350, 160", "350, 160", "350, 80"});
	DNNscores_allMC.NBins = 30; 									 								// Default = 100
	DNNscores_allMC.use_weight = true;
	DNNscores_allMC.Plot();		

	DNNscores_allMC.ClearFileTrees();
	DNNscores_allMC.SetPlots({P_jet0_scores_inc_train80});
	DNNscores_allMC.SetOutputFileTag("DNNscores_allMC_truthMatch");
	DNNscores_allMC.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5"); 		
	DNNscores_allMC.SetSelectiveCuts("CTau", "jet0_isTruthMatched == 1");
	DNNscores_allMC.SetSelectiveCuts("LLPskim", "Pass_WPlusJets");
	DNNscores_allMC.use_weight = true;
	DNNscores_allMC.Plot();

	DNNscores_allMC.ClearFileTrees();
	DNNscores_allMC.SetPlots({P_jet0_scores_inc_train80});
	DNNscores_allMC.SetOutputFileTag("DNNscores_allMC_truthMatch_InclCand");
	DNNscores_allMC.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5 && jet0_InclTagCand == 1"); 		
	DNNscores_allMC.SetSelectiveCuts("CTau", "jet0_isTruthMatched == 1");
	DNNscores_allMC.SetSelectiveCuts("LLPskim", "Pass_WPlusJets");
	DNNscores_allMC.use_weight = true;
	DNNscores_allMC.Plot();

	class MiniTuplePlotter DNNscores_allData( filetags_all_data, path );

	DNNscores_allData.SetTreeName( "NoSel" );	// TreeName
	DNNscores_allData.SetPlots({P_jet0_scores_inc_train40, P_jet0_scores_inc_train80}); // These "P_" variables are PlotParams structs defined in PlotParams.h
	DNNscores_allData.SetOutputFileTag("DNNscores_allData"); 						
	DNNscores_allData.plot_norm 		  = true; 	// Default = true
	DNNscores_allData.plot_log  		  = true; 	// Default = true
	DNNscores_allData.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5 && jet0_InclTagCand == 1"); 		// Apply cuts to all events
	// DNNscores_allData.SetComparisonCuts({Cut_jet0_inclusive, Cut_jet1_depth}); 					// Apply cuts to overlay
	// DNNscores_allData.SetSelectiveCuts("5k", "jet0_energy > 200");								// Apply this only to filetag names that include the string "data"
	DNNscores_allData.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );										// Manual Legend location
	DNNscores_allData.SetLegendNames({"Bv1", "Cv1", "Cv2", "Cv3", "Cv4", "Dv1", "Dv2"});
	DNNscores_allData.NBins = 30; 									 								// Default = 100
	DNNscores_allData.use_weight = true;
	DNNscores_allData.Plot();		

	DNNscores_allData.ClearFileTrees();
	DNNscores_allData.SetPlots({P_jet0_scores_inc_train80});
	DNNscores_allData.SetOutputFileTag("DNNscores_allData_WPlusJets");
	DNNscores_allData.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5"); 		
	DNNscores_allData.SetSelectiveCuts("CTau", "jet0_isTruthMatched == 1");
	DNNscores_allData.SetSelectiveCuts("LLPskim", "Pass_WPlusJets");
	DNNscores_allData.use_weight = true;
	DNNscores_allData.Plot();
}