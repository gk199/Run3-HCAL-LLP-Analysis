#include "MiniTuplePlotter.h"

// -------------------------------------------------------------------------------------------------
void Plot_CalRatio(){

	string path = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.1/";
	path = "../Files/NanoMiniTuples/2026-04-29/minituple_";

	vector<string> filetags;
	
	filetags = { "Files_ParkingLLP0_2025F_NANOAOD_temp", "Files_ParkingLLP0_2026B_NANOAOD_temp", "Files_ParkingLLP0_2026C_NANOAOD_temp"};
	
	// "HLT_HT200_L1SingleLLPJet_PFJet60_NeutralHadronFrac0p7"
	// P_jet0_NeutralHadEFrac

	if( true ) {

		MiniTuplePlotter plotter( filetags, path );

		plotter.SetTreeName( "PassL1" );
		plotter.SetPlots( { P_jet0_NeutralHadEFrac } );

		//plotter.SetCuts( "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_DepthTagCand == 1" );
		plotter.SetComparisonCuts( {"", "HLT_HT200_L1SingleLLPJet_PFJet60_NeutralHadronFrac0p7 == 1"} ); //, "Pass_HLTDisplacedJet && Pass_PreSel == 1"} ); // && jet0_DepthTagCand == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_DepthTagCand == 1"} ); // "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_scores_inc_train80 > 0.9", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.1",} );

		//plotter.SetLegendNames({"LJDC Kinematics", "LJDC + lead jet b-tag score > 0.1", "LJDC + lead jet b-tag score > 0.2435", "LJDC + lead jet b-tag score > 0.3", "LJDC + lead jet b-tag score > 0.45", "LJDC + lead jet b-tag score > 0.8"}); //, "+Jet1 Inc Score > 0.9", "+Jet0 Depth Score > 0.01"});
		plotter.SetLegendPosition( 0.4, 0.6, 0.88, 0.88 );

		plotter.plot_log        = true;
		plotter.stamp_counts   = false;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 40;
		plotter.output_file_tag = "";
		plotter.Plot("ratio");
	}

}
