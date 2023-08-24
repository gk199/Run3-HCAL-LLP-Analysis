#include "MiniTuplePlotter.h"
#include "TString.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_HLT_Effs(){

	// List where minituples are stored
	string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.1/minituple_";
	vector<string> filetags_both = { "v1.1_LLPskim_500k_2023_08_22", "v1.1_MCsignal_500k_2023_08_22" };
	vector<string> filetags_data = { "v1.1_LLPskim_500k_2023_08_22" };
	vector<string> filetags_MC = { "v1.1_MCsignal_500k_2023_08_23" };

	// Setup cuts on HLT paths passed
	TCut Cut_None			= "";
	TCut Cut_HLTpassed1 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet35_Inclusive1PtrkShortSig5 == 1"; 
	TCut Cut_HLTpassed2 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1";
	TCut Cut_HLTpassed3 	= "HLT_HT240_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1"; // not in v1 ntuples yet
	TCut Cut_HLTpassed4 	= "HLT_HT280_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1"; // end of group 1 of triggers // not in v1 ntuples yet
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
	TCut Cut_HLTpassed15 	= "HLT_L1SingleLLPJet == 1"; // monitoring trigger // not in v1 ntuples yet

	// Setup cuts on LLP decay position
	float radius_tracker[2]  = {0, 161.6};
    float radius_ECAL[2]     = {161.6, 183.6}; // 22cm of ECAL
    float radius_depth1[2]   = {183.6, 190.2};
    float radius_depth2[2]   = {190.2, 214.2};
    float radius_depth3[2]   = {214.2, 244.8};
    float radius_depth4[2]   = {244.8, 295};
    float radius_all[2]      = {0,9999};
    float radius_inHCAL[2]   = {183.6, 295};
    float radius_depth12[2]  = {183.6, 214.2};
    float radius_depth34[2]  = {214.2, 295};

	float HBeta = 1.4;
	int z_pos = 425; // depth 1 and 2 have a z position < 388, depth 3 and 4 have z position < 425cm

	TString LLP0inHCAL 	= Form("(LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_inHCAL[1], HBeta);
	TString LLP1inHCAL 	= Form("(LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_inHCAL[1], HBeta);

	TString LLP0inTracker 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_tracker[0], 	radius_tracker[1], 	HBeta);
	TString LLP0inECAL 		= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_ECAL[0], 	radius_ECAL[1], 	HBeta);
	TString LLP0inHCAL_d12 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_depth12[0], 	radius_depth12[1], 	HBeta);
	TString LLP0inHCAL_d3 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_depth3[0], 	radius_depth3[1], 	HBeta);
	TString LLP0inHCAL_d4 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_depth4[0], 	radius_depth4[1], 	HBeta);
	
	TString LLP1inTracker 	= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_tracker[0], 	radius_tracker[1], 	HBeta);
	TString LLP1inECAL 		= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_ECAL[0], 	radius_ECAL[1], 	HBeta);
	TString LLP1inHCAL_d12 	= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_depth12[0], 	radius_depth12[1], 	HBeta);
	TString LLP1inHCAL_d3 	= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_depth3[0], 	radius_depth3[1], 	HBeta);
	TString LLP1inHCAL_d4 	= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_depth4[0], 	radius_depth4[1], 	HBeta);

	TString OR		   	= (" || ");
	
	TCut Cut_LLPinHCAL 	= (LLP0inHCAL + OR + LLP1inHCAL).Data();
	TCut Cut_LLP0inHCAL = LLP0inHCAL.Data();
	TCut Cut_LLP1inHCAL = LLP1inHCAL.Data();

	TCut Cut_LLPinTracker	= (LLP0inTracker + OR + LLP1inTracker).Data();
	TCut Cut_LLP0inTracker 	= LLP0inTracker.Data();
	TCut Cut_LLPinECAL 		= (LLP0inECAL + OR + LLP1inECAL).Data();
	TCut Cut_LLP0inECAL 	= LLP0inECAL.Data();
	TCut Cut_LLPinHCAL_d12 	= (LLP0inHCAL_d12 + OR + LLP1inHCAL_d12).Data();
	TCut Cut_LLP0inHCAL_d12 = LLP0inHCAL_d12.Data();
	TCut Cut_LLPinHCAL_d3 	= (LLP0inHCAL_d3 + OR + LLP1inHCAL_d3).Data();
	TCut Cut_LLP0inHCAL_d3 	= LLP0inHCAL_d3.Data();
	TCut Cut_LLPinHCAL_d4 	= (LLP0inHCAL_d4 + OR + LLP1inHCAL_d4).Data();
	TCut Cut_LLP0inHCAL_d4 	= LLP0inHCAL_d4.Data();

	// Save these cuts in a map paired with a string so they can be iterated over, to easily make same plot with lots of different cuts  :) 
	map<TCut, string> LLP_Cuts;
  	LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinTracker, "LLPinTracker"));
  	LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinECAL, "LLPinECAL"));
  	LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinHCAL_d12, "LLPinHCAL_depth12"));
  	LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinHCAL_d3, "LLPinHCAL_depth3"));
  	LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinHCAL_d4, "LLPinHCAL_depth4"));

	map<TCut, string> LLP0_Cuts;
  	LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inTracker, "LLP0inTracker"));
  	LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inECAL, "LLP0inECAL"));
  	LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inHCAL_d12, "LLP0inHCAL_depth12"));
  	LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inHCAL_d3, "LLP0inHCAL_depth3"));
  	LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inHCAL_d4, "LLP0inHCAL_depth4"));

	map<TCut, string>::iterator it;
	for (it = LLP_Cuts.begin(); it != LLP_Cuts.end(); it++) {
		cout << it->second << " ";
		cout << it->first << " ";
		cout << endl;
	}

	// ----- Jet kinematics split by HLT paths passed -----//

	cout<<endl;
	cout<<" ---------- HLT Study 1: Jet kinematics split by HLT paths passed ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_HLTeff( filetags_MC, path );
	plotter_HLTeff.SetPlots({P_jet0_E, P_jet0_Pt, P_jet1_E, P_jet1_Pt, P_jet0_Eta, P_jet0_Phi}); // These "P_" variables are PlotParams structs defined in PlotParams.h
	plotter_HLTeff.SetTreeName( "NoSel" );	// TreeName
	plotter_HLTeff.SetOutputFileTag("HLT_v1.1_MC"); 							// Your own special name :)

	plotter_HLTeff.plot_norm 		  = false; 	// Default = true
	plotter_HLTeff.plot_log_ratio   = true; 	// Default = false. Make bottom panel log scale
	plotter_HLTeff.SetLegendManual( 0.35, 0.65, 0.9, 0.9 );					// Manual Legend location
	
	plotter_HLTeff.SetComparisonCuts({Cut_None, Cut_HLTpassed2, Cut_HLTpassed5, Cut_HLTpassed9, Cut_HLTpassed11}); 
	plotter_HLTeff.Plot("ratio");				// This is what does stuff -- arguments: None/"", "ratio", "sig"/"ssqrtb"


	// ----- Jet kinematics split by HLT paths passed -- require that LLP decays in/before HCAL -----//

	cout<<endl;
	cout<<" ---------- HLT Study 2: Jet kinematics split by HLT paths passed -- require that LLP decays in/before HCAL ---------- "<<endl;
	cout<<endl;

	for (it = LLP_Cuts.begin(); it != LLP_Cuts.end(); it++) {

		class MiniTuplePlotter plotter_HLTeff2( filetags_MC, path );
		plotter_HLTeff2.SetPlots({P_jet0_Pt, P_jet1_Pt, P_met_Pt}); 
		plotter_HLTeff2.SetTreeName( "NoSel" );
		plotter_HLTeff2.SetOutputFileTag("HLT_v1.1_MC_" + it->second);

		plotter_HLTeff2.plot_norm 		  = false;
		plotter_HLTeff2.plot_log_ratio   = true; 
		plotter_HLTeff2.SetLegendManual( 0.35, 0.65, 0.9, 0.9 );

		plotter_HLTeff2.SetComparisonCuts({Cut_None, Cut_HLTpassed2, Cut_HLTpassed5, Cut_HLTpassed9, Cut_HLTpassed11}); 
		plotter_HLTeff2.SetSelectiveCuts("MC", it->first);
		plotter_HLTeff2.Selection(it->second); // print which selection is made on the plot
		plotter_HLTeff2.Plot("ratio");
	}

	// ----- LLP kinematics -- HLT Efficiency split by LLP displacement and energy -----//

	cout<<endl;
	cout<<" ---------- HLT Study 3: LLP kinematics -- HLT efficiency split by LLP displacement and energy ---------- "<<endl;
	cout<<endl;

	for (it = LLP0_Cuts.begin(); it != LLP0_Cuts.end(); it++) {

		class MiniTuplePlotter plotter_LLP( filetags_MC, path );
		plotter_LLP.SetPlots({P_LLP0_Pt, P_LLP0_E}); 
		plotter_LLP.SetTreeName( "NoSel" );	
		plotter_LLP.SetOutputFileTag("HLT_v1.1_MC_" + it->second); 							

		plotter_LLP.plot_norm 		  = false; 	
		plotter_LLP.plot_log_ratio   = true; 	
		plotter_LLP.SetLegendManual( 0.35, 0.65, 0.9, 0.9 );

		plotter_LLP.SetComparisonCuts({Cut_None, Cut_HLTpassed2, Cut_HLTpassed5, Cut_HLTpassed9, Cut_HLTpassed11}); 
		plotter_LLP.SetSelectiveCuts("MC", it->first);
		plotter_LLP.Selection(it->second); // print which selection is made on the plot
		plotter_LLP.Plot("ratio");	
	}

	class MiniTuplePlotter plotter_disp( filetags_MC, path );
	plotter_disp.SetPlots({P_LLP0_DecayR}); 
	plotter_disp.SetTreeName( "NoSel" );	
	plotter_disp.SetOutputFileTag("HLT_v1.1_MC_LLP0inHCAL"); 							

	plotter_disp.plot_norm 		  = false; 	
	plotter_disp.plot_log_ratio   = true; 	
	plotter_disp.SetLegendManual( 0.35, 0.65, 0.9, 0.9 );

	plotter_disp.SetComparisonCuts({Cut_None, Cut_HLTpassed2, Cut_HLTpassed5, Cut_HLTpassed9, Cut_HLTpassed11}); 
	plotter_disp.SetSelectiveCuts("MC", Cut_LLP0inHCAL);
	plotter_disp.Plot("ratio");	

}
