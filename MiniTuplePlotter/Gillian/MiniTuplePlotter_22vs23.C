#include "../MiniTuplePlotter.h"
#include "TString.h"
#include "../RegionCuts.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_22vs23(){

	string path = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.1/minituple_";
	
	vector<string> filetags = { "data_2022Dv1_scores", "data_2022Ev1_scores", "data_2022Fv1_scores", "data_2022Gv1_scores", 
								"data_2023Cv1_scores", "data_2023Cv2_scores", "data_2023Cv3_scores", "data_2023Cv4_scores", "data_2023Dv1_scores", "data_2023Dv2_scores"}; 

	vector<string> filetags_small = { "data_2022Gv1_scores", "data_2023Cv4_scores"}; 

	// ----- Flavor tagging probabilities -----//

	cout<<endl;
	cout<<" ---------- Flavor tagging probabilities by DNN scores ---------- "<<endl;
	cout<<endl;

	bool plot_all = false; // Set to true to plot all variables, false to just plot DNN scores in CR/VR.
	if (plot_all) {
		class MiniTuplePlotter kinematics( filetags, path );

		kinematics.SetTreeName( "NoSel" );	
		kinematics.SetPlots({P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_DeepCSV_prob_b, 
							P_jet0_DepthTowers, P_jet0_TimingTowers, P_jet0_FlaggedTowers, P_jet0_dR_L1jet, 
							P_jet0_Setaeta, P_jet0_Sphiphi, P_jet0_Setaphi,
							P_jet0_Track0PtFrac, 
							P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4,
							P_jet0_TDCaverage, P_jet0_TDCaverage_Eweight, P_jet0_TDCnDelayed, 
							P_jet0_NeutralHadEFrac, P_jet0_ChargedHadEFrac, P_jet0_PhoEFrac, P_jet0_EleEFrac, P_jet0_MuonEFrac,
							P_jet0_LeadingRechitEFracJet, P_jet0_AllRechitE,
							P_jet0_scores_inc_train80, P_jet1_scores_inc_train80, // P_jet0_scores_depth_LLPanywhere, P_jet1_scores_depth_LLPanywhere,
						}); 
		kinematics.SetOutputFileTag("22vs23"); 	
		kinematics.SetOutputDirectory("2022vs2023");
	//	kinematics.SetCuts(""); 	
	//	kinematics.SetComparisonCuts({Cut_CR, Cut_VR}); 	
		kinematics.SetLegendNames({"Data 2022 D", "Data 2022 E", "Data 2022 F", "Data 2022 G", 
								"Data 2023 Cv1", "Data 2023 Cv2", "Data 2023 Cv3", "Data 2023 Cv4", "Data 2023 Dv1", "Data 2023 Dv2"});
		kinematics.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
		kinematics.colors = {kViolet+3, kViolet-7, kViolet-8, kViolet-9, 
							kGreen+4, kGreen-1, kGreen-2, kGreen-6, kGreen-8, kGreen-10};
			// kViolet-8, kSpring-6};
		kinematics.NBins = 50; 		
		kinematics.Plot("ratio");
	}

	// ---------------------------------------------------------------------- //
	class MiniTuplePlotter CR_only( filetags, path );

	CR_only.SetTreeName( "NoSel" );	
	CR_only.SetPlots({P_jet0_scores_depth_LLPanywhere,
					P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_DeepCSV_prob_b, P_jet0_NeutralHadEFrac,
					P_jet1_scores_inc_train80,
				}); 
	CR_only.SetOutputFileTag("22vs23_CR"); 
	CR_only.SetOutputDirectory("2022vs2023/VR_CR_study");
	CR_only.SetCuts(Cut_CR + " jet0_DepthTagCand == 1"); 	
	CR_only.SetLegendNames({"Data 2022 D", "Data 2022 E", "Data 2022 F", "Data 2022 G", 
							"Data 2023 Cv1", "Data 2023 Cv2", "Data 2023 Cv3", "Data 2023 Cv4", "Data 2023 Dv1", "Data 2023 Dv2"});
	CR_only.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
	CR_only.colors = {kViolet+3, kViolet-7, kViolet-8, kViolet-9, 
						kGreen+4, kGreen-1, kGreen-2, kGreen-6, kGreen-8, kGreen-10};
	CR_only.NBins = 50; 		
	CR_only.Plot("ratio");

	class MiniTuplePlotter CR1_only( filetags, path );

	CR1_only.SetTreeName( "NoSel" );	
	CR1_only.SetPlots({P_jet1_scores_depth_LLPanywhere,
					P_jet1_Pt, P_jet1_Eta, P_jet1_Phi, P_jet1_DeepCSV_prob_b, P_jet1_NeutralHadEFrac,
					P_jet0_scores_inc_train80,
				}); 
	CR1_only.SetOutputFileTag("22vs23_CR"); 
	CR1_only.SetOutputDirectory("2022vs2023/VR_CR_study");
	CR1_only.SetCuts(Cut_CR_jet0 + " jet1_DepthTagCand == 1"); 	
	CR1_only.SetLegendNames({"Data 2022 D", "Data 2022 E", "Data 2022 F", "Data 2022 G", 
							"Data 2023 Cv1", "Data 2023 Cv2", "Data 2023 Cv3", "Data 2023 Cv4", "Data 2023 Dv1", "Data 2023 Dv2"});
	CR1_only.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
	CR1_only.colors = {kViolet+3, kViolet-7, kViolet-8, kViolet-9, 
						kGreen+4, kGreen-1, kGreen-2, kGreen-6, kGreen-8, kGreen-10};
	CR1_only.NBins = 50; 		
	CR1_only.Plot("ratio");

	// ---------------------------------------------------------------------- //
	class MiniTuplePlotter VR_only( filetags, path );

	VR_only.SetTreeName( "NoSel" );	
	VR_only.SetPlots({P_jet0_scores_depth_LLPanywhere,
					P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_DeepCSV_prob_b, P_jet0_NeutralHadEFrac,
					P_jet1_scores_inc_train80,
				}); 
	VR_only.SetOutputFileTag("22vs23_VR"); 
	VR_only.SetOutputDirectory("2022vs2023/VR_CR_study");
	VR_only.SetCuts(Cut_VR + " jet0_DepthTagCand == 1"); 	
	VR_only.SetLegendNames({"Data 2022 D", "Data 2022 E", "Data 2022 F", "Data 2022 G", 
							"Data 2023 Cv1", "Data 2023 Cv2", "Data 2023 Cv3", "Data 2023 Cv4", "Data 2023 Dv1", "Data 2023 Dv2"});
	VR_only.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
	VR_only.colors = {kViolet+3, kViolet-7, kViolet-8, kViolet-9, 
						kGreen+4, kGreen-1, kGreen-2, kGreen-6, kGreen-8, kGreen-10};
	VR_only.NBins = 50; 		
	VR_only.Plot("ratio");

	class MiniTuplePlotter VR1_only( filetags, path );

	VR1_only.SetTreeName( "NoSel" );	
	VR1_only.SetPlots({P_jet1_scores_depth_LLPanywhere,
					P_jet1_Pt, P_jet1_Eta, P_jet1_Phi, P_jet1_DeepCSV_prob_b, P_jet1_NeutralHadEFrac,
					P_jet0_scores_inc_train80,
					}); 
	VR1_only.SetOutputFileTag("22vs23_VR"); 
	VR1_only.SetOutputDirectory("2022vs2023/VR_CR_study");
	VR1_only.SetCuts(Cut_VR_jet0 + " jet1_DepthTagCand == 1"); 	
	VR1_only.SetLegendNames({"Data 2022 D", "Data 2022 E", "Data 2022 F", "Data 2022 G", 
							"Data 2023 Cv1", "Data 2023 Cv2", "Data 2023 Cv3", "Data 2023 Cv4", "Data 2023 Dv1", "Data 2023 Dv2"});
	VR1_only.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
	VR1_only.colors = {kViolet+3, kViolet-7, kViolet-8, kViolet-9, 
						kGreen+4, kGreen-1, kGreen-2, kGreen-6, kGreen-8, kGreen-10};
	VR1_only.NBins = 50; 		
	VR1_only.Plot("ratio");

	// ---------------------------------------------------------------------- //

	class MiniTuplePlotter b_tag_study( filetags_small, path );

	b_tag_study.SetTreeName( "NoSel" );	
	b_tag_study.SetPlots({P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_DeepCSV_prob_b, P_jet0_NeutralHadEFrac,
					P_jet1_scores_inc_train80,
					}); 
	b_tag_study.SetOutputFileTag("22vs23_b_tag"); 
	b_tag_study.SetOutputDirectory("2022vs2023/b_tag_study");
	b_tag_study.SetCuts(Cut_VR + " jet0_DepthTagCand == 1"); 	// jet 0 is depth tagged, jet 1 is inclusive tagged
	b_tag_study.SetComparisonCuts({"jet0_DeepCSV_prob_b < 0.2435", "jet0_DeepCSV_prob_b >= 0.2435"}); 	
	b_tag_study.SetLegendNames({"Data 2022 G, not b-tagged", "Data 2022 G, b-tagged", "Data 2023 Cv4, not b-tagged", "Data 2023 Cv4, b-tagged"});
	b_tag_study.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
	b_tag_study.colors = {kViolet+3, kViolet-8, kGreen-1, kGreen-6};
	b_tag_study.NBins = 50; 		
	b_tag_study.Plot("ratio");

	class MiniTuplePlotter b_tag_study_subleading( filetags_small, path );

	b_tag_study_subleading.SetTreeName( "NoSel" );	
	b_tag_study_subleading.SetPlots({P_jet1_Pt, P_jet1_Eta, P_jet1_Phi, P_jet1_DeepCSV_prob_b, P_jet1_NeutralHadEFrac,
					P_jet0_scores_inc_train80,
					}); 
	b_tag_study_subleading.SetOutputFileTag("22vs23_b_tag"); 
	b_tag_study_subleading.SetOutputDirectory("2022vs2023/b_tag_study");
	b_tag_study_subleading.SetCuts(Cut_VR_jet0 + " jet1_DepthTagCand == 1"); 	// jet 0 is inclusive tagged, jet 1 is depth tagged
	b_tag_study_subleading.SetComparisonCuts({"jet1_DeepCSV_prob_b < 0.2435", "jet1_DeepCSV_prob_b >= 0.2435"}); 	
	b_tag_study_subleading.SetLegendNames({"Data 2022 G, not b-tagged", "Data 2022 G, b-tagged", "Data 2023 Cv4, not b-tagged", "Data 2023 Cv4, b-tagged"});
	b_tag_study_subleading.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );	
	b_tag_study_subleading.colors = {kViolet+3, kViolet-8, kGreen-1, kGreen-6};
	b_tag_study_subleading.NBins = 50; 		
	b_tag_study_subleading.Plot("ratio");

}