#include "../MiniTuplePlotter.h"
#include "TString.h"
#include "../RegionCuts.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_FlavorTag(){

	string path = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v4.1/";
	
	vector<string> filetags = { "DisplacedJet_Run2023C-EXOLLPJetHCAL-PromptReco-v1_AOD_2025_07_21_scores" }; 

	// ----- Flavor tagging probabilities -----//

	cout<<endl;
	cout<<" ---------- Flavor tagging probabilities by DNN scores ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter flavor_tags( filetags, path );

	flavor_tags.SetTreeName( "NoSel" );	
	flavor_tags.SetPlots({P_jet0_DeepCSV_prob_b, P_jet0_DeepCSV_prob_c, P_jet0_DeepCSV_prob_bb, P_jet0_DeepCSV_prob_udsg}); 
	flavor_tags.SetOutputFileTag("flavor_v4.1"); 	
	flavor_tags.SetOutputDirectory("flavorTags");
	flavor_tags.SetCuts("jet0_DepthTagCand == 1 && jet0_scores_depth_LLPanywhere > 0.1"); 	
	flavor_tags.SetComparisonCuts({Cut_CR, Cut_VR}); 	
	flavor_tags.SetLegendNames({"LLP skim CR", "LLP skim VR"});
	flavor_tags.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
	flavor_tags.colors = {kViolet-8, kSpring-6};
	flavor_tags.NBins = 50; 		
	flavor_tags.Plot("ratio");

	class MiniTuplePlotter flavor_tags_depth( filetags, path );

	flavor_tags_depth.SetTreeName( "NoSel" );	
	flavor_tags_depth.SetPlots({P_jet0_DeepCSV_prob_b, P_jet0_DeepCSV_prob_c, P_jet0_DeepCSV_prob_bb, P_jet0_DeepCSV_prob_udsg}); 
	flavor_tags_depth.SetOutputFileTag("flavor_depthDNN_v4.1"); 	
	flavor_tags_depth.SetOutputDirectory("flavorTags");
	flavor_tags_depth.SetCuts("jet0_DepthTagCand == 1"); 	
	flavor_tags_depth.SetComparisonCuts({"jet0_scores_depth_LLPanywhere < 0.2", "jet0_scores_depth_LLPanywhere >= 0.2 && jet0_scores_depth_LLPanywhere < 0.5", "jet0_scores_depth_LLPanywhere >= 0.5 && jet0_scores_depth_LLPanywhere < 0.9"}); 	
	flavor_tags_depth.SetLegendNames({"LLP skim, depth DNN < 0.2", "LLP skim, depth DNN 0.2-0.5", "LLP skim, depth DNN 0.5-0.9"});
	flavor_tags_depth.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
	flavor_tags_depth.colors = {kRed-3, kSpring-6, kBlue-3};
	flavor_tags_depth.NBins = 50; 		
	flavor_tags_depth.Plot("ratio");

	class MiniTuplePlotter flavor_tags_inc( filetags, path );

	flavor_tags_inc.SetTreeName( "NoSel" );	
	flavor_tags_inc.SetPlots({P_jet0_DeepCSV_prob_b, P_jet0_DeepCSV_prob_c, P_jet0_DeepCSV_prob_bb, P_jet0_DeepCSV_prob_udsg}); 
	flavor_tags_inc.SetOutputFileTag("flavor_inclDNN_v4.1"); 	
	flavor_tags_inc.SetOutputDirectory("flavorTags");
	flavor_tags_inc.SetCuts("jet0_InclTagCand == 1"); 	
	flavor_tags_inc.SetComparisonCuts({"jet0_scores_inc_train80 < 0.2", "jet0_scores_inc_train80 >= 0.2 && jet0_scores_inc_train80 < 0.5", "jet0_scores_inc_train80 >= 0.5 && jet0_scores_inc_train80 < 0.9"}); 	
	flavor_tags_inc.SetLegendNames({"LLP skim, inclusive DNN < 0.2", "LLP skim, inclusive DNN 0.2-0.5", "LLP skim, inclusive DNN 0.5-0.9"});
	flavor_tags_inc.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
	flavor_tags_inc.colors = {kRed-3, kSpring-6, kBlue-3};
	flavor_tags_inc.NBins = 50; 		
	flavor_tags_inc.Plot("ratio");
}

