#include "../MiniTuplePlotter.h"
#include "TString.h"

// Goals of this study: DNN scores
// Mostly worked on during June 2025
// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_DNN(){

	// List where minituples are stored
	
	string path  = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_";

	vector<string> filetags = { "HToSSTo4b_125_50_CTau3000_batch2_allscores", "LLPskim_2023Cv4_allscores" }; 
	vector<string> filetags_all_data = { "LLPskim_2023Bv1_allscores", "LLPskim_2023Cv1_allscores", "LLPskim_2023Cv2_allscores", "LLPskim_2023Cv3_allscores", "LLPskim_2023Cv4_allscores", "LLPskim_2023Dv1_allscores", "LLPskim_2023Dv2_allscores" }; 

	vector<string> filetags_all_signal = {// "HToSSTo4b_125_15_CTau10000_allscores", "HToSSTo4b_125_15_CTau3000_allscores", "HToSSTo4b_125_50_CTau10000_allscores",
										"HToSSTo4b_125_50_CTau3000_batch1_allscores",
										"HToSSTo4b_125_50_CTau3000_batch2_allscores",
										"HToSSTo4b_250_120_CTau10000_batch1_allscores",
										"HToSSTo4b_250_120_CTau10000_batch2_allscores",
										"HToSSTo4b_350_160_CTau10000_batch1_allscores",
										"HToSSTo4b_350_160_CTau10000_batch2_allscores",
										"HToSSTo4b_350_80_CTau500_allscores"};

	// ----- Example 1 -----//
	// - Basic Booleans
	bool study1 = true;
	bool study2 = false;
	bool study3 = false;
	bool study4 = false;
	
	cout<<endl;
	cout<<" ---------- Study 1: Overlay LLP MC and data ---------- "<<endl;
	cout<<endl;

	if (study1) {
		class MiniTuplePlotter DNNscores( filetags, path );
		TCut Cut_jet0_inclusive 	= "jet0_InclTagCand == 1";
		TCut Cut_jet1_depth 		= "jet1_DepthTagCand == 1";  

		DNNscores.SetTreeName( "NoSel" );	// TreeName
		// pt, eta, tag candidate
		DNNscores.SetPlots({P_jet0_scores_inc_train40, P_jet0_scores_inc_train80}); // These "P_" variables are PlotParams structs defined in PlotParams.h
		DNNscores.SetOutputFileTag("DNNscores"); 						
		DNNscores.plot_norm 		  = true; 	// Default = true
		DNNscores.plot_log  		  = true; 	// Default = true
		DNNscores.SetCuts("jet0_InclTagCand == 1"); 		// Apply cuts to all events
		// DNNscores.SetComparisonCuts({Cut_jet0_inclusive, Cut_jet1_depth}); 					// Apply cuts to overlay
		// DNNscores.SetSelectiveCuts("5k", "jet0_energy > 200");								// Apply this only to filetag names that include the string "data"
		DNNscores.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );									// Manual Legend location
		DNNscores.SetLegendNames({"LLP 125,50", "Cv4"});
		DNNscores.colors = {kBlue, kOrange, kGreen+2, kRed};									// Your own colors (default kBlack + rainbow)
		DNNscores.NBins = 30; 									 								// Default = 100
		DNNscores.use_weight = true;
		DNNscores.Plot();
		DNNscores.ClearFileTrees();
		DNNscores.SetPlots({P_jet0_scores_depth_hcal, P_jet0_scores_depth_anywhere});
		DNNscores.SetCuts("jet0_DepthTagCand == 1"); 		
		DNNscores.Plot();									 								

		// pt, eta, W+jets
		DNNscores.ClearFileTrees();
		DNNscores.SetPlots({P_jet0_scores_inc_train80, P_jet0_scores_depth_hcal, P_jet0_scores_depth_anywhere});
		DNNscores.SetOutputFileTag("DNNscores_truthMatch");
		DNNscores.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5"); 		
		DNNscores.SetSelectiveCuts("CTau", "jet0_isTruthMatched == 1");
		DNNscores.SetSelectiveCuts("LLPskim", "Pass_WPlusJets");
		DNNscores.use_weight = true;
		DNNscores.Plot();

		// pt, eta, one jet inclusive tag one depth
		DNNscores.ClearFileTrees();
		DNNscores.SetPlots({P_jet0_scores_inc_train80, P_jet1_scores_depth_hcal, P_jet1_scores_depth_anywhere});
		DNNscores.SetOutputFileTag("DNNscores_jet1Depth");
		DNNscores.SetCuts("jet0_InclTagCand == 1 && jet1_DepthTagCand == 1"); 		
		DNNscores.use_weight = true;
		DNNscores.Plot();
		DNNscores.ClearFileTrees();
		DNNscores.SetPlots({P_jet1_scores_inc_train80, P_jet0_scores_depth_hcal, P_jet0_scores_depth_anywhere});
		DNNscores.SetOutputFileTag("DNNscores_jet1Incl");
		DNNscores.SetCuts("jet1_InclTagCand == 1 && jet0_DepthTagCand == 1"); 		
		DNNscores.use_weight = true;
		DNNscores.Plot();
	}

	cout<<endl;
	cout<<" ---------- Study 2: Overlay all LLP MC ---------- "<<endl;
	cout<<endl;

	if (study2) {
		class MiniTuplePlotter DNNscores_allMC( filetags_all_signal, path );
		// pt, eta, tag cand
		DNNscores_allMC.SetTreeName( "NoSel" );	// TreeName
		DNNscores_allMC.SetPlots({P_jet0_scores_inc_train40, P_jet0_scores_inc_train80, P_jet0_scores_depth_hcal, P_jet0_scores_depth_anywhere}); // These "P_" variables are PlotParams structs defined in PlotParams.h
		DNNscores_allMC.SetOutputFileTag("DNNscores_allMC"); 						
		DNNscores_allMC.plot_norm 		  = true; 	// Default = true
		DNNscores_allMC.plot_log  		  = true; 	// Default = true
		DNNscores_allMC.SetCuts("jet0_InclTagCand == 1"); 		// Apply cuts to all events
		DNNscores_allMC.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );										// Manual Legend location
		DNNscores_allMC.SetLegendNames({"125, 50", "125, 50", "250, 120", "250, 120", "350, 160", "350, 160", "350, 80"});
		DNNscores_allMC.NBins = 30; 									 								// Default = 100
		DNNscores_allMC.use_weight = true;
		DNNscores_allMC.Plot();		
		DNNscores_allMC.ClearFileTrees();
		DNNscores_allMC.SetPlots({P_jet0_scores_depth_hcal, P_jet0_scores_depth_anywhere});
		DNNscores_allMC.SetCuts("jet0_DepthTagCand == 1");
		DNNscores_allMC.Plot();	

		// pt, eta, truth match
		DNNscores_allMC.ClearFileTrees();
		DNNscores_allMC.SetPlots({P_jet0_scores_inc_train80, P_jet0_scores_depth_hcal, P_jet0_scores_depth_anywhere});
		DNNscores_allMC.SetOutputFileTag("DNNscores_allMC_truthMatch");
		DNNscores_allMC.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5 && jet0_isTruthMatched == 1"); 		
		DNNscores_allMC.use_weight = true;
		DNNscores_allMC.Plot();

		// pt, eta, tag cand, truth match
		DNNscores_allMC.ClearFileTrees();
		DNNscores_allMC.SetPlots({P_jet0_scores_inc_train80});
		DNNscores_allMC.SetOutputFileTag("DNNscores_allMC_truthMatch_InclCand");
		DNNscores_allMC.SetCuts("jet0_InclTagCand == 1 && jet0_isTruthMatched == 1"); 		
		DNNscores_allMC.use_weight = true;
		DNNscores_allMC.Plot();
		DNNscores_allMC.ClearFileTrees();
		DNNscores_allMC.SetPlots({P_jet0_scores_depth_hcal, P_jet0_scores_depth_anywhere});
		DNNscores_allMC.SetOutputFileTag("DNNscores_allMC_truthMatch_DepthCand");
		DNNscores_allMC.SetCuts("jet0_DepthTagCand == 1 && jet0_isTruthMatched == 1"); 		
		DNNscores_allMC.use_weight = true;
		DNNscores_allMC.Plot();

		// pt, eta, tag candidate, HLT
		DNNscores_allMC.ClearFileTrees();
		DNNscores_allMC.SetPlots({P_jet0_scores_inc_train80});
		DNNscores_allMC.SetOutputFileTag("DNNscores_allMC_InclCand_HLT");
		DNNscores_allMC.SetCuts("jet0_InclTagCand == 1 && Pass_HLTDisplacedJet"); 		
		DNNscores_allMC.use_weight = true;
		DNNscores_allMC.Plot();
		DNNscores_allMC.ClearFileTrees();
		DNNscores_allMC.SetPlots({P_jet0_scores_depth_hcal, P_jet0_scores_depth_anywhere});
		DNNscores_allMC.SetOutputFileTag("DNNscores_allMC_DepthCand_HLT");
		DNNscores_allMC.SetCuts("jet0_DepthTagCand == 1 && Pass_HLTDisplacedJet"); 		
		DNNscores_allMC.use_weight = true;
		DNNscores_allMC.Plot();

		// pt, eta, truth match LLP, HLT
		DNNscores_allMC.ClearFileTrees();
		DNNscores_allMC.SetPlots({P_jet0_scores_inc_train80, P_jet0_scores_depth_hcal, P_jet0_scores_depth_anywhere});
		DNNscores_allMC.SetOutputFileTag("DNNscores_allMC_truthMatch_HLT");
		DNNscores_allMC.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5 && jet0_isTruthMatched == 1 && Pass_HLTDisplacedJet"); 		
		DNNscores_allMC.use_weight = true;
		DNNscores_allMC.Plot();
	}

	cout<<endl;
	cout<<" ---------- Study 3: Overlay all data ---------- "<<endl;
	cout<<endl;

	if (study3) {
		class MiniTuplePlotter DNNscores_allData( filetags_all_data, path );
		DNNscores_allData.SetTreeName( "NoSel" );	// TreeName
		DNNscores_allData.SetPlots({P_jet0_scores_inc_train40, P_jet0_scores_inc_train80}); // These "P_" variables are PlotParams structs defined in PlotParams.h
		DNNscores_allData.SetOutputFileTag("DNNscores_allData"); 						
		DNNscores_allData.plot_norm 		  = true; 	// Default = true
		DNNscores_allData.plot_log  		  = true; 	// Default = true
		DNNscores_allData.SetCuts("jet0_InclTagCand == 1"); 		// Apply cuts to all events
		DNNscores_allData.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );										// Manual Legend location
		DNNscores_allData.SetLegendNames({"Bv1", "Cv1", "Cv2", "Cv3", "Cv4", "Dv1", "Dv2"});
		DNNscores_allData.NBins = 30; 									 								// Default = 100
		DNNscores_allData.use_weight = true;
		DNNscores_allData.Plot();		

		DNNscores_allData.ClearFileTrees();
		DNNscores_allData.SetPlots({P_jet0_scores_depth_hcal, P_jet0_scores_depth_anywhere});
		DNNscores_allData.SetOutputFileTag("DNNscores_allData");
		DNNscores_allData.SetCuts("jet0_DepthTagCand == 1"); 		
		DNNscores_allData.use_weight = true;
		DNNscores_allData.Plot();

		DNNscores_allData.ClearFileTrees();
		DNNscores_allData.SetPlots({P_jet0_scores_inc_train80, P_jet0_scores_depth_hcal, P_jet0_scores_depth_anywhere});
		DNNscores_allData.SetOutputFileTag("DNNscores_allData_WPlusJets");
		DNNscores_allData.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5 && Pass_WPlusJets"); 		
		DNNscores_allData.use_weight = true;
		DNNscores_allData.Plot();
	}

	cout<<endl;
	cout<<" ---------- Study 4: Overlay with a CR and SR focus ---------- "<<endl;
	cout<<endl;

	if (study4) {
		// focus on signal and background that we see in the analysis
		class MiniTuplePlotter DNNscores_CR_SR( filetags, path );
		DNNscores_CR_SR.SetTreeName( "NoSel" );	
		DNNscores_CR_SR.SetPlots({P_jet0_scores_inc_train80});
		DNNscores_CR_SR.SetOutputFileTag("DNNscores_CR_SR"); 						
		DNNscores_CR_SR.SetCuts("jet0_InclTagCand == 1 && Pass_HLTDisplacedJet");
		DNNscores_CR_SR.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );										// Manual Legend location
		DNNscores_CR_SR.SetLegendNames({"LLP 125,50", "Cv4"});
		DNNscores_CR_SR.colors = {kBlue, kOrange, kGreen+2, kRed};									// Your own colors (default kBlack + rainbow)
		DNNscores_CR_SR.NBins = 30; 
		DNNscores_CR_SR.use_weight = true;
		DNNscores_CR_SR.Plot();
		DNNscores_CR_SR.ClearFileTrees();
		DNNscores_CR_SR.SetPlots({P_jet1_scores_inc_train80});
		DNNscores_CR_SR.SetOutputFileTag("DNNscores_CR_SR"); 						
		DNNscores_CR_SR.SetCuts("jet1_InclTagCand == 1 && Pass_HLTDisplacedJet");
		DNNscores_CR_SR.Plot();

		DNNscores_CR_SR.ClearFileTrees();
		DNNscores_CR_SR.SetPlots({P_jet0_scores_depth_hcal, P_jet0_scores_depth_anywhere});
		DNNscores_CR_SR.SetCuts("jet0_DepthTagCand == 1 && Pass_HLTDisplacedJet"); 		
		DNNscores_CR_SR.SetSelectiveCuts("LLPskim", "jet1_scores_inc_train80 < 0.2");	// CR
		DNNscores_CR_SR.SetSelectiveCuts("CTau", "jet1_scores_inc_train80 > 0.9");	// SR
		DNNscores_CR_SR.Plot();
		DNNscores_CR_SR.ClearFileTrees();
		DNNscores_CR_SR.SetPlots({P_jet1_scores_depth_hcal, P_jet1_scores_depth_anywhere});
		DNNscores_CR_SR.SetCuts("jet1_DepthTagCand == 1 && Pass_HLTDisplacedJet"); 		
		DNNscores_CR_SR.SetSelectiveCuts("LLPskim", "jet0_scores_inc_train80 < 0.2");	// CR
		DNNscores_CR_SR.SetSelectiveCuts("CTau", "jet0_scores_inc_train80 > 0.9");	// SR
		DNNscores_CR_SR.Plot();
	}
}