#include "../MiniTuplePlotter.h"
#include "TString.h"
#include "../RegionCuts.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_FlavorTag(){

	string path = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v4.1/minituple_";
	
	vector<string> filetags = { "LLPskim_2023Cv4_allscores" }; 

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
	flavor_tags_depth.SetComparisonCuts({"jet0_scores_depth_LLPanywhere < 0.01", "jet0_scores_depth_LLPanywhere >= 0.01 && jet0_scores_depth_LLPanywhere < 0.05", "jet0_scores_depth_LLPanywhere >= 0.05 && jet0_scores_depth_LLPanywhere < 0.2",  "jet0_scores_depth_LLPanywhere >= 0.2 && jet0_scores_depth_LLPanywhere < 0.9"}); 	
	flavor_tags_depth.SetLegendNames({"LLP skim, depth DNN < 0.01", "LLP skim, depth DNN 0.01-0.05", "LLP skim, depth DNN 0.05-0.2", "LLP skim, depth DNN 0.2-0.9"});
	flavor_tags_depth.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
	flavor_tags_depth.colors = {kRed-3, kSpring-6, kBlue-3, kOrange};
	flavor_tags_depth.NBins = 50; 		
	flavor_tags_depth.Plot("ratio");

	class MiniTuplePlotter flavor_tags_inc( filetags, path );

	flavor_tags_inc.SetTreeName( "NoSel" );	
	flavor_tags_inc.SetPlots({P_jet0_DeepCSV_prob_b, P_jet0_DeepCSV_prob_c, P_jet0_DeepCSV_prob_bb, P_jet0_DeepCSV_prob_udsg}); 
	flavor_tags_inc.SetOutputFileTag("flavor_inclDNN_v4.1"); 	
	flavor_tags_inc.SetOutputDirectory("flavorTags");
	flavor_tags_inc.SetCuts("jet0_InclTagCand == 1"); 	
	flavor_tags_inc.SetComparisonCuts({"jet0_scores_inc_train80 < 0.01", "jet0_scores_inc_train80 >= 0.01 && jet0_scores_inc_train80 < 0.05", "jet0_scores_inc_train80 >= 0.05 && jet0_scores_inc_train80 < 0.2",  "jet0_scores_inc_train80 >= 0.2 && jet0_scores_inc_train80 < 0.9"}); 	
	flavor_tags_inc.SetLegendNames({"LLP skim, inclusive DNN < 0.01", "LLP skim, inclusive DNN 0.01-0.05", "LLP skim, inclusive DNN 0.05-0.2", "LLP skim, inclusive DNN 0.2-0.9"});
	flavor_tags_inc.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
	flavor_tags_inc.colors = {kRed-3, kSpring-6, kBlue-3, kOrange};
	flavor_tags_inc.NBins = 50; 		
	flavor_tags_inc.Plot("ratio");

	class MiniTuplePlotter scores_by_flavor( filetags, path );

	scores_by_flavor.SetTreeName( "NoSel" );	
	scores_by_flavor.SetPlots({P_jet0_scores_inc_train80}); 
	scores_by_flavor.SetOutputFileTag("score_by_flavor_v4.1"); 	
	scores_by_flavor.SetOutputDirectory("flavorTags");
	scores_by_flavor.SetCuts("jet0_InclTagCand == 1"); 	
	scores_by_flavor.SetComparisonCuts({"jet0_DeepCSV_prob_b >= 0.2435", "jet0_DeepCSV_prob_c >= 0.102", "jet0_DeepCSV_prob_bb >= 0.2435", "jet0_DeepCSV_prob_udsg > 0.8"}); 	
	scores_by_flavor.SetLegendNames({"b tagged", "c tagged", "bb tagged", "light flavor tagged"});
	scores_by_flavor.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
	scores_by_flavor.colors = {kRed-3, kSpring-6, kBlue-3, kOrange};
	scores_by_flavor.NBins = 50; 		
	scores_by_flavor.Plot("ratio");
	scores_by_flavor.ClearFileTrees();
	scores_by_flavor.SetPlots({P_jet0_scores_depth_LLPanywhere});  
	scores_by_flavor.SetCuts("jet0_DepthTagCand == 1");
	scores_by_flavor.Plot("ratio");
}

