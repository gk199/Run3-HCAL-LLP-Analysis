#include "../MiniTuplePlotter.h"
#include "TString.h"
#include "../RegionCuts.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_22vs23(){

	string path = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.0/minituple_";
	
	vector<string> filetags = { "data_2022Dv1_scores", "data_2022Ev1_scores", "data_2022Fv1_scores", "data_2022Gv1_scores", 
								"data_2023Cv1_scores", "data_2023Cv2_scores", "data_2023Cv3_scores", "data_2023Cv4_scores", "data_2023Dv1_scores", "data_2023Dv2_scores"}; 

	// ----- Flavor tagging probabilities -----//

	cout<<endl;
	cout<<" ---------- Flavor tagging probabilities by DNN scores ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter kinematics( filetags, path );

	kinematics.SetTreeName( "NoSel" );	
	kinematics.SetPlots({P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_DeepCSV_prob_b, 
                        P_jet0_DepthTowers, P_jet0_TimingTowers, P_jet0_FlaggedTowers, P_jet0_dR_L1jet, 
                        P_jet0_Setaeta, P_jet0_Sphiphi, P_jet0_Setaphi,
                        P_jet0_Track0PtFrac, 
                        P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4,
                        P_jet0_TDCaverage, P_jet0_TDCaverage_Eweight, P_jet0_TDCnDelayed, 
                        P_jet0_NeutralHadEFrac, P_jet0_ChargedHadEFrac, P_jet0_PhoEFrac, P_jet0_EleEFrac, P_jet0_MuonEFrac,
                        P_jet0_LeadingRechitEFracJet, P_jet0_AllRechitE}); 
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

