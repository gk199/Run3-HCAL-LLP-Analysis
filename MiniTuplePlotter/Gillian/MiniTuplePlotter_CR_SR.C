#include "../MiniTuplePlotter.h"
#include "TString.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_CR_SR(){


	// List where minituples are stored
	string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.3/minituple_";
	vector<string> filetags_both 	= { "v1.3_LLPskim_500k_2023_09_22", "v1.3_MCsignal_500k_2023_09_22" };
	vector<string> filetags_data 	= { "v1.3_LLPskim_500k_2023_09_22" };
	vector<string> filetags_MC 		= { "v1.3_MCsignal_500k_2023_09_22" };

	vector<PlotParams> jetPlots		= {P_jet0_E, P_jet0_Pt}; // dont apply a 40 GeV jet cut on these
	vector<PlotParams> allPlots 	= {P_jet0_E, P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_RechitN, P_jet0_Mass, P_jet0_JetArea,							// jet kinematics
										P_jet0_EtaSpread, P_jet0_PhiSpread, P_jet0_EtaSpread_energy, P_jet0_PhiSpread_energy, 								// eta - phi spread
										P_jet0_EtaPhiQuadSpread, P_jet0_EtaPhiQuadSpread_energy,
										P_jet0_Track0Pt, P_jet0_Track0PtFrac, P_jet0_Track0dR, 					 											// track vars
										P_jet0_Track0dEta, P_jet0_Track0dPhi, P_jet0_Tracks_dR, 
										P_jet0_Track0dzToPV, P_jet0_Track0dxyToBS, P_jet0_Track0dzOverErr, P_jet0_Track0dxyOverErr,
										P_jet0_NSV, P_jet0_NSVCand, P_jet0_SV_x, P_jet0_SV_y, P_jet0_SV_z, P_jet0_SV_NTracks, P_jet0_SV_Mass, 				// SV
//										P_jet0_FlightDist2D, P_jet0_FlightDist2DErr, P_jet0_FlightDist3D, P_jet0_FlightDist3DErr,
										P_jet0_LeadingRechitE, P_jet0_SubLeadingRechitE, P_jet0_SSubLeadingRechitE, P_jet0_AllRechitE, P_jet0_3RechitFracE,	// HB rechits
										P_jet0_LeadingRechitD, P_jet0_SubLeadingRechitD, P_jet0_SSubLeadingRechitD, 
										P_jet0_LeadingRechitEFrac, P_jet0_LeadingRechitJetEDiff, 
										P_jet0_NeutralHadEFrac, P_jet0_ChargedHadEFrac, P_jet0_PhoEFrac, P_jet0_MuonEFrac,
//										P_jet0_EleEFrac, P_jet0_NeutralElePhoEFrac, 																		// need fix in ntupler!!!
										P_jet0_NeutralPhoEFrac, P_jet0_HoverE, P_jet0_NeutralOverChargedHad, P_jet0_LogNeutralOverChargedHad,				// H/E and neutral / charged hadron
										P_jet0_NeutralHadMult, P_jet0_ChargedHadMult, P_jet0_PhoMult, P_jet0_EleMult, 
										P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4,				// depth ratios
										P_jet0_HCALd1_over_d2, P_jet0_HCALd1_over_d3, P_jet0_HCALd1_over_d4, P_jet0_HCALd12_over_d34 };

	#include "../RegionCuts.h"

	// ----- Jet kinematics in control and signal regions -----//

	cout<<endl;
	cout<<" ---------- CR / SR Study 1: Jet kinematics ---------- "<<endl;
	cout<<endl;

	// leading jet, all jet variables
	class MiniTuplePlotter plotter_JetVars( filetags_MC, path );
	plotter_JetVars.SetPlots(allPlots);
	plotter_JetVars.SetTreeName( "NoSel" );	
	plotter_JetVars.SetOutputFileTag("CR_SR_v1.3_MC"); 	
	plotter_JetVars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0}); // , Cut_LLPinHCAL4_Jet0});
	plotter_JetVars.plot_log_ratio    = false; 
	plotter_JetVars.SetLegendNames({"Prompt", "Tracker-NonPrompt", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
	plotter_JetVars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	//plotter_JetVars.plot_cdf = true;
	//plotter_JetVars.plot_reverse_cdf = true;
	plotter_JetVars.SetCuts("jet0_E > 40");  // 40 GeV jet cut on all plots! 
	plotter_JetVars.SetOutputDirectory("CR_SR");
	plotter_JetVars.Plot("ratio");

	// 2D spread of eta - phi, for leading and subleading jet
	class MiniTuplePlotter plotter_JetVars2D_etaphi( filetags_MC, path );
	plotter_JetVars2D_etaphi.SetTreeName( "NoSel" );	
	plotter_JetVars2D_etaphi.SetOutputFileTag("CR_SR_v1.3_MC"); 	
	plotter_JetVars2D_etaphi.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0});
	plotter_JetVars2D_etaphi.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );			
	plotter_JetVars2D_etaphi.SetCuts("jet0_E > 40");
	plotter_JetVars2D_etaphi.SetOutputDirectory("2D");
    plotter_JetVars2D_etaphi.SetPlots2D({Hist1_Hist2(P_jet0_EtaSpread, P_jet0_PhiSpread), Hist1_Hist2(P_jet0_EtaSpread_energy, P_jet0_PhiSpread_energy), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_SubLeadingRechitE) });
	plotter_JetVars2D_etaphi.PlotMany2D();
	plotter_JetVars2D_etaphi.SetComparisonCuts({Cut_LLPinCR_Jet1, Cut_LLPinHCAL_Jet1});
	plotter_JetVars2D_etaphi.SetCuts("jet1_E > 40");
    plotter_JetVars2D_etaphi.SetPlots2D({Hist1_Hist2(P_jet1_EtaSpread, P_jet1_PhiSpread), Hist1_Hist2(P_jet1_EtaSpread_energy, P_jet1_PhiSpread_energy) });
	plotter_JetVars2D_etaphi.ClearFileTrees();
	plotter_JetVars2D_etaphi.PlotMany2D();

	// sub-leading jet, energy and pT distributions
	class MiniTuplePlotter plotter_Jet1Vars2D_etaphi( filetags_MC, path );
	plotter_Jet1Vars2D_etaphi.SetPlots({P_jet1_E, P_jet1_Pt}); 
	plotter_Jet1Vars2D_etaphi.SetTreeName( "NoSel" );	
	plotter_Jet1Vars2D_etaphi.SetOutputFileTag("CR_SR_v1.3_MC"); 	
	plotter_Jet1Vars2D_etaphi.SetComparisonCuts({Cut_LLPinCR_Jet1, Cut_LLPinHCAL_Jet1});
	plotter_Jet1Vars2D_etaphi.plot_log_ratio    = true;
	plotter_Jet1Vars2D_etaphi.SetLegendNames({"Prompt", "HCAL-D1234"});
	plotter_Jet1Vars2D_etaphi.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_Jet1Vars2D_etaphi.Plot("ratio");

	// ----- Cut based selections -----//

	cout<<endl;
	cout<<" ---------- CR / SR Study 2: Cut based selections. Cut on neutral hadron energy fraction ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_CutBased( filetags_MC, path );
	plotter_CutBased.SetPlots(allPlots);
	plotter_CutBased.SetTreeName( "NoSel" );	
	plotter_CutBased.SetOutputFileTag("CR_SR_NeutralHadCut_v1.3_MC"); 	
	plotter_CutBased.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0}); // , Cut_LLPinHCAL4_Jet0});
	plotter_CutBased.plot_log_ratio    = false; 
	plotter_CutBased.SetLegendNames({"Prompt", "Tracker-NonPrompt", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
	plotter_CutBased.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_CutBased.SetCuts("jet0_NeutralHadEFrac > 0.6 && jet0_E > 40"); 
	plotter_CutBased.SetOutputDirectory("CR_SR_NeutralHadCut");
	plotter_CutBased.Plot("ratio");
}
