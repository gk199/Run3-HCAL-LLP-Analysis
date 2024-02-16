#include "../MiniTuplePlotter.h"
#include "TString.h"

// Goals of this study: use the LLP MC sample, split into different regions to understand how variables change by region. Also started overlaying LLP skim data variables.
// Mostly worked on during October - November 2023.
// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_CR_SR(){

	// List where minituples are stored
	
	// string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v3.0/minituple_";
	string path = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_";
	string path_v3pt1 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.1/minituple_";
	string path_v3pt2 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.2/minituple_";
	string path_v3pt3 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_";

	map<string,vector<string>> filetags;
	// filetags["LLP125"]	= { "v3.0_LLPskim_Run2023Bv1_2023Cv2_2023_11_23", "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2023_11_23"};
	// filetags["LLP350"]	= { "v3.0_LLPskim_Run2023Bv1_2023Cv2_2023_11_23", "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2023_11_29"};

	filetags["LLP125"]	= { "v3.3_LLPskim_Run2023_hadd_TEST", "v3.3_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_02_06_TEST"};
	filetags["LLP350"]	= { "v3.3_LLPskim_Run2023_hadd_TEST", "v3.3_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_02_06_TEST"};

	vector<string> filetags_all 	= { "v3.0_LLPskim_Run2023Bv1_2023Cv2_2023_11_23", "v2.0_MC_QCD_250k_2023_10_18", "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2023_11_23"};
	vector<string> filetags_data 	= { "v3.0_LLPskim_Run2023Bv1_2023Cv2_2023_11_23" };
	vector<string> filetags_LLP 	= { "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2023_11_23" };	
	vector<string> filetags_QCD 	= { "v2.0_MC_QCD_250k_2023_10_18" };	

	vector<string> filetags_data_v3pt1 	= { "v3.1_LLPskim_Run2023Cv3_2024_01_20" };
	vector<string> filetags_LLP_v3pt1 	= { "v3.1_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_01_20_TEST" };	
	//vector<string> filetags_LLP_v3pt1 	= { "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_01_20_TEST" };
	vector<string> filetags_data_v3pt3 	= { "v3.3_LLPskim_Run2023_hadd_TEST" };
	vector<string> filetags_LLP_v3pt3	= { "v3.3_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_02_06_TEST" };
	//vector<string> filetags_LLP_v3pt3 	= { "v3.3_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_02_06_TEST" };

	vector<PlotParams> jetPlots0	= {P_jet0_E, P_jet0_Pt}; 																				// dont apply a 40 GeV jet cut on these
	vector<PlotParams> jetPlots1	= {P_jet1_E, P_jet1_Pt}; 																				// dont apply a 40 GeV jet cut on these
	vector<PlotParams> allPlots 	= {P_jet0_E, P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_RechitN, P_jet0_Mass, P_jet0_JetArea,							// jet kinematics
										P_jet0_EtaSpread, P_jet0_PhiSpread, P_jet0_EtaSpread_energy, P_jet0_PhiSpread_energy, 								// eta - phi spread
										P_jet0_EtaPhiQuadSpread, P_jet0_EtaPhiQuadSpread_energy,
										P_jet0_Track0Pt, P_jet0_Track0PtFrac, P_jet0_Track0dR, 					 											// track vars
										P_jet0_Track0dEta, P_jet0_Track0dPhi, P_jet0_Tracks_dR, 
										P_jet0_MatchedLLP_DecayR, P_jet0_MatchedLLP_Eta,
//										P_jet0_Smajor, P_jet0_Sminor, P_jet0_SminorSmajor, 
//										P_jet0_Track0dzToPV, P_jet0_Track0dxyToBS, P_jet0_Track0dzOverErr, P_jet0_Track0dxyOverErr,
//										P_jet0_NSV, P_jet0_NSVCand, P_jet0_SV_x, P_jet0_SV_y, P_jet0_SV_z, P_jet0_SV_NTracks, P_jet0_SV_Mass, 				// SV
//										P_jet0_FlightDist2D, P_jet0_FlightDist2DErr, P_jet0_FlightDist3D, P_jet0_FlightDist3DErr,
										P_jet0_LeadingRechitE, P_jet0_SubLeadingRechitE, P_jet0_SSubLeadingRechitE, P_jet0_AllRechitE, P_jet0_3RechitFracE,	// HB rechits
										P_jet0_LeadingRechitD, P_jet0_SubLeadingRechitD, P_jet0_SSubLeadingRechitD, 
										P_jet0_LeadingRechitEFrac, P_jet0_LeadingRechitEFracJet, P_jet0_LeadingRechitJetEDiff, 
										P_jet0_LeadingSubRechitEQuad, 
										P_jet0_NeutralHadEFrac, P_jet0_ChargedHadEFrac, P_jet0_PhoEFrac, P_jet0_MuonEFrac,									// tracking based
//										P_jet0_EleEFrac, P_jet0_NeutralElePhoEFrac, 																		// need fix in ntupler!!!
										P_jet0_NeutralPhoEFrac, P_jet0_HoverE, P_jet0_NeutralOverChargedHad, P_jet0_LogNeutralOverChargedHad,				// H/E and neutral / charged hadron
										P_jet0_NeutralHadMult, P_jet0_ChargedHadMult, P_jet0_PhoMult, P_jet0_EleMult, 
										P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4,				// depth ratios
										P_jet0_HCALd1_over_d2, P_jet0_HCALd1_over_d3, P_jet0_HCALd1_over_d4, P_jet0_HCALd12_over_d34 };
	vector<PlotParams> MC_allPlots	= { P_LLP0_dR_b_to_b, P_LLP0_dEta_b_to_b, P_LLP0_dPhi_b_to_b, P_LLP0_dR_LLP_to_b };										// LLP decay kinematic variables
	vector<PlotParams> trackVars	= {	P_jet0_Track0Pt, P_jet0_Track0PtFrac, P_jet0_Track0dR, 					 											// track vars
										P_jet0_Track0dEta, P_jet0_Track0dPhi, P_jet0_Tracks_dR}; 
	vector<PlotParams> rechitVars 	= {P_jet0_LeadingRechitE, P_jet0_SubLeadingRechitE, P_jet0_SSubLeadingRechitE, P_jet0_AllRechitE, P_jet0_3RechitFracE,	// HB rechits
										P_jet0_LeadingRechitD, P_jet0_SubLeadingRechitD, P_jet0_SSubLeadingRechitD, 
										P_jet0_LeadingRechitEFrac, P_jet0_LeadingRechitEFracJet, P_jet0_LeadingRechitJetEDiff, 
										P_jet0_LeadingSubRechitEQuad, P_jet0_EtaPhiQuadSpread, P_jet0_EtaPhiQuadSpread_energy,
										P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4 };			// depth ratios
	vector<PlotParams> analysisVars	= { P_jet0_NeutralHadEFrac, P_jet0_ChargedHadEFrac, P_jet0_MuonEFrac, P_jet0_PhoEFrac, P_jet0_EleEFrac,
										//P_jet0_NeutralHadEFrac_zero, P_jet0_ChargedHadEFrac_zero, P_jet0_MuonEFrac_zero, P_jet0_PhoEFrac_zero, P_jet0_EleEFrac_zero,
										P_jet0_Track0Pt, P_jet0_Track0dEta, P_jet0_Track0dPhi, P_jet0_Track1Pt, P_jet0_Track1dEta, P_jet0_Track1dPhi, 
										P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4,
										P_jet0_Eta, P_jet0_Phi, P_jet0_Mass,
										P_jet0_LeadingRechitE, P_jet0_LeadingRechitEFracJet, P_jet0_Sphiphi};
	vector<PlotParams> analysisVars_perJet	= { P_perJet_NeutralHadEFrac, P_perJet_ChargedHadEFrac, P_perJet_MuonEFrac, P_perJet_PhoEFrac, P_perJet_EleEFrac,
										// P_perJet_NeutralHadEFrac_zero, P_perJet_ChargedHadEFrac_zero, P_perJet_MuonEFrac_zero, P_perJet_PhoEFrac_zero, P_perJet_EleEFrac_zero,
										P_perJet_Track0Pt, P_perJet_Track0dEta, P_perJet_Track0dPhi, P_perJet_Track1Pt, P_perJet_Track1dEta, P_perJet_Track1dPhi, 
										P_perJet_EnergyFrac_Depth1, P_perJet_EnergyFrac_Depth2, P_perJet_EnergyFrac_Depth3, P_perJet_EnergyFrac_Depth4,
										P_perJet_Eta, P_perJet_Phi, P_perJet_Mass,
										P_perJet_LeadingRechitE, P_perJet_AllRechitE, P_perJet_LeadingRechitEFracJet, 
										P_perJet_Sphiphi, P_perJet_Setaeta, P_perJet_Setaphi};				 																					// only in newest v3.0 minituples
										// P_jet0_Track0nMissingInnerHits, P_jet0_Track0nMissingOuterHits, P_jet0_Track0nPixelHits, P_jet0_Track0nHits,			// only in v3.2 or newer
										// P_jet0_EtaSpread, P_jet0_PhiSpread, P_jet0_EtaSpread_energy, P_jet0_PhiSpread_energy,
										// P_jet0_TDCaverage, P_jet0_TDCaverage_Eweight, P_jet0_TDCnDelayed, 

	MC_allPlots.insert(MC_allPlots.begin(), allPlots.begin(), allPlots.end());

	#include "../RegionCuts.h"

	bool LLP_WJets = true;				// analysis variables for LLP and W+Jets overlayed
	bool Bkg_est = false;				// look at jet tagging vars by jet eta for probability estimations
	bool LLP_all = false;				// all variables, track vars with leading track < 4 study, eta-phi spread
	bool Jet_vars = false; 				// jet kinematics without a pT cut
	bool NH_bins = false;				// bin in neutral hadron fraction
	bool Jet_bins = false;				// bin in jet pT
	bool BDT_bins = false;				// bin in BDT score
	bool Overlay_all = false;			// overlay LLP MC, QCD MC, LLP skim
	bool Matching_efficiency = false;	// LLP - jet matching efficiency

	// ----- Jet kinematics in control and signal regions -----//

	if (LLP_WJets) {
		cout<<endl;
		cout<<" ---------- CR / SR Study 0: Jet, track kinematics for LLP MC and W+jets (background) ---------- "<<endl;
		cout<<endl;
		
		vector<string> filetag_keys_to_loop = {"LLP125", "LLP350"};

		for( auto key: filetag_keys_to_loop){

			// HCAL depth variables with LLP regions and data overlayed
			class MiniTuplePlotter plotter_BDTvars( filetags[key], path_v3pt3 );
			plotter_BDTvars.SetPlots(analysisVars); 
			plotter_BDTvars.SetTreeNames( {"WPlusJets", "NoSel"} );	
			plotter_BDTvars.SetOutputFileTag("Overlay_"+key+"_v3.3");
			plotter_BDTvars.plot_log_ratio    = false; 
			//plotter_BDTvars.plot_cdf 		 = true;
			//plotter_BDTvars.plot_reverse_cdf  = true;
			plotter_BDTvars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			plotter_BDTvars.SetLegendNames({"LLP skim - W+jets selection", "Tracker: R <= 10cm", "Tracker: R > 10cm", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
			plotter_BDTvars.colors = { kBlack, kGray, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed };
			plotter_BDTvars.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26");
			plotter_BDTvars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0}, "LLP_MC");
			//plotter_BDTvars.SetSelectiveCuts("LLPskim", "jet0_NeutralHadEFrac < 0.6");  // BLINDED with track energy fraction (jet0_Track0Pt / jet0_Pt > 0.1) or neutral hadron fraction (jet0_NeutralHadEFrac < 0.6)
			//plotter_BDTvars.SetSelectiveCuts("LLPskim", "jet1_Pt < 30"); // for stronger W+jets selection
			plotter_BDTvars.SetOutputDirectory("Overlay_"+key);
			plotter_BDTvars.Plot("ratio");

			// HCAL depth variables with LLP regions and data overlayed
			class MiniTuplePlotter plotter_BDTvars_perJet( filetags[key], path_v3pt3 );
			plotter_BDTvars_perJet.SetPlots(analysisVars_perJet); 
			plotter_BDTvars_perJet.SetTreeNames( {"PerJet_WPlusJets", "PerJet_LLPmatched"} );	
			plotter_BDTvars_perJet.SetOutputFileTag("Overlay_perJet_"+key+"_v3.3");
			plotter_BDTvars_perJet.plot_log_ratio    = false; 
			//plotter_BDTvars_perJet.plot_cdf 		 = true;
			//plotter_BDTvars_perJet.plot_reverse_cdf  = true;
			plotter_BDTvars_perJet.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			plotter_BDTvars_perJet.SetLegendNames({"LLP skim - W+jets selection", "Tracker: R <= 10cm", "Tracker: R > 10cm", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
			plotter_BDTvars_perJet.colors = { kBlack, kGray, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed };
			plotter_BDTvars_perJet.SetComparisonCuts({Cut_matchedLLPinCR, Cut_matchedLLPinTrackerNP, Cut_matchedLLPinECAL, Cut_matchedLLPinHCAL1, Cut_matchedLLPinHCAL2, Cut_matchedLLPinHCAL34}, "LLP_MC");
			plotter_BDTvars_perJet.SetOutputDirectory("Overlay_perJet_"+key);
			plotter_BDTvars_perJet.Plot("ratio");
		}
	}

	if (Bkg_est) {
		cout<<endl;
		cout<<" ---------- Background Study 1: Tagging rates by jet eta ---------- "<<endl;
		cout<<endl;

		TCut Cut_BDTscore0 	= "bdtscore_LLP125_perJet > 0"; 
		TCut Cut_BDTscore 	= "bdtscore_LLP125_perJet > 0.5"; 

		// leading jet, all jet variables
		class MiniTuplePlotter plotter_BkgEst( filetags_data_v3pt3, path_v3pt3 );
		plotter_BkgEst.SetPlots({P_perJet_Eta, P_perJet_Phi, P_perJet_Pt});
		plotter_BkgEst.SetTreeName( "PerJet_WPlusJets" );	
		plotter_BkgEst.SetOutputFileTag("Jet40_CR_SR_v3.3_MC"); 	
		plotter_BkgEst.SetComparisonCuts({Cut_None, Cut_BDTscore0, Cut_BDTscore});
		plotter_BkgEst.plot_log_ratio   = true; 
		plotter_BkgEst.plot_norm 		= false;
		plotter_BkgEst.SetLegendNames({"Jets from W+Jets events", "with BDT 125 score > 0", "with BDT 125 score > 0.5"});
		plotter_BkgEst.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		//plotter_BkgEst.plot_cdf = true;
		//plotter_BkgEst.plot_reverse_cdf = true;
		plotter_BkgEst.SetOutputDirectory("BackgroundEstimations");
		plotter_BkgEst.Plot("ratio");
	}

	if (LLP_all) {
		cout<<endl;
		cout<<" ---------- CR / SR Study 1: Jet, track kinematics ---------- "<<endl;
		cout<<endl;

		// leading jet, all jet variables
		class MiniTuplePlotter plotter_JetVars( filetags_LLP, path );
		plotter_JetVars.SetPlots(MC_allPlots);
		plotter_JetVars.SetTreeName( "NoSel" );	
		plotter_JetVars.SetOutputFileTag("Jet40_CR_SR_v3.0_MC"); 	
		plotter_JetVars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0});
		plotter_JetVars.plot_log_ratio    = false; 
		plotter_JetVars.SetLegendNames({"Tracker: R < 10cm", "Tracker: R > 10cm", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
		plotter_JetVars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		//plotter_JetVars.plot_cdf = true;
		//plotter_JetVars.plot_reverse_cdf = true;
		plotter_JetVars.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26");  // 40 GeV jet cut on all plots! Require jet is in HB (1.26) 
		plotter_JetVars.SetOutputDirectory("MC_CR_SR");
		plotter_JetVars.Plot("ratio");

		// track variables, with leading track < 4 Gev (signal region)
		class MiniTuplePlotter plotter_TrackVars( filetags_LLP, path );
		plotter_TrackVars.SetPlots(trackVars); 
		plotter_TrackVars.SetTreeName( "NoSel" );	
		plotter_TrackVars.SetOutputFileTag("Jet40_Track4_CR_SR_v3.0_MC"); 	
		plotter_TrackVars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0});
		plotter_TrackVars.plot_log_ratio    = false; 
		plotter_TrackVars.SetLegendNames({"Tracker: R < 10cm", "Tracker: R > 10cm", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
		plotter_TrackVars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_TrackVars.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26 && jet0_Track0Pt < 4");
		plotter_TrackVars.SetOutputDirectory("MC_CR_SR_TrackStudy");
		plotter_TrackVars.Plot("ratio");

		// 2D spread of eta - phi, for leading and subleading jet
		class MiniTuplePlotter plotter_JetVars2D_etaphi( filetags_LLP, path );
		plotter_JetVars2D_etaphi.SetTreeName( "NoSel" );	
		plotter_JetVars2D_etaphi.SetOutputFileTag("Jet40_CR_SR_v3.0_MC"); 	
		plotter_JetVars2D_etaphi.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0});
		plotter_JetVars2D_etaphi.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );			
		plotter_JetVars2D_etaphi.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26");
		plotter_JetVars2D_etaphi.SetOutputDirectory("2D");
		plotter_JetVars2D_etaphi.SetPlots2D({Hist1_Hist2(P_jet0_EtaSpread, P_jet0_PhiSpread), Hist1_Hist2(P_jet0_EtaSpread_energy, P_jet0_PhiSpread_energy), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_SubLeadingRechitE) });
		plotter_JetVars2D_etaphi.PlotMany2D();
		plotter_JetVars2D_etaphi.SetCuts("jet1_Pt >= 40 && abs(jet1_Eta) <= 1.26");
		plotter_JetVars2D_etaphi.SetPlots2D({Hist1_Hist2(P_jet1_EtaSpread, P_jet1_PhiSpread), Hist1_Hist2(P_jet1_EtaSpread_energy, P_jet1_PhiSpread_energy) });
		plotter_JetVars2D_etaphi.ClearFileTrees();
		plotter_JetVars2D_etaphi.PlotMany2D();
	}

	if (Jet_vars) {
		// ----- Full jet kinematics for leading and sub-leading jet, energy and pT distributions with no pt cuts to see full distribution (simple, no cuts) -----//

		cout<<endl;
		cout<<" ---------- CR / SR Study 2: Simple distributions of jet kinematics without a pT cut ---------- "<<endl;
		cout<<endl;

		class MiniTuplePlotter plotter_Jet0KinematicVars( filetags_LLP, path );
		plotter_Jet0KinematicVars.SetPlots(jetPlots0); 
		plotter_Jet0KinematicVars.SetTreeName( "NoSel" );	
		plotter_Jet0KinematicVars.SetOutputFileTag("CR_SR_v3.0_MC"); 	
		plotter_Jet0KinematicVars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0});
		plotter_Jet0KinematicVars.plot_log_ratio    = true;
		plotter_Jet0KinematicVars.SetLegendNames({"Tracker: R < 10cm", "HCAL-D1234"});
		plotter_Jet0KinematicVars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_Jet0KinematicVars.Plot("ratio");	
		
		class MiniTuplePlotter plotter_Jet1KinematicVars( filetags_LLP, path );
		plotter_Jet1KinematicVars.SetPlots(jetPlots1); 
		plotter_Jet1KinematicVars.SetTreeName( "NoSel" );	
		plotter_Jet1KinematicVars.SetOutputFileTag("CR_SR_v3.0_MC"); 	
		plotter_Jet1KinematicVars.SetComparisonCuts({Cut_LLPinCR_Jet1, Cut_LLPinHCAL_Jet1});
		plotter_Jet1KinematicVars.plot_log_ratio    = true;
		plotter_Jet1KinematicVars.SetLegendNames({"Tracker: R < 10cm", "HCAL-D1234"});
		plotter_Jet1KinematicVars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_Jet1KinematicVars.Plot("ratio");
	}

	if (NH_bins) {
		// ----- Bin in neutral hadron fraction -----//

		cout<<endl;
		cout<<" ---------- Study 3: Bin in neutral hadron fraction (MC and data) ---------- "<<endl;
		cout<<endl;

		// this will overlay data and LLP MC with the same cuts -- plot might be too messy! Could be useful in future with good color scheme (dark / light) for dedicated plots
		// class MiniTuplePlotter plotter_RechitVars_NHBinsData( filetags_all, path );
		class MiniTuplePlotter plotter_RechitVars_NHBinsData( filetags_data, path );
		plotter_RechitVars_NHBinsData.SetPlots(allPlots);
		plotter_RechitVars_NHBinsData.SetTreeName( "NoSel" );	
		plotter_RechitVars_NHBinsData.SetOutputFileTag("NeutralHadBins_Jet40_v3.0"); 	
		plotter_RechitVars_NHBinsData.SetComparisonCuts({Cut_NHadpt1, Cut_NHadpt2, Cut_NHadpt3, Cut_NHadpt4, Cut_NHadpt6, Cut_NHad1}); 
		plotter_RechitVars_NHBinsData.plot_log_ratio    = false; 
		plotter_RechitVars_NHBinsData.SetLegendNames({"Data: Neutral Hadron 0-0.1", "Data: Neutral Hadron 0.1-0.2", "Data: Neutral Hadron 0.2-0.3", "Data: Neutral Hadron 0.3-0.4", "Data: Neutral Hadron 0.4-0.6", "Data: Neutral Hadron 0.6-1"});
		plotter_RechitVars_NHBinsData.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_RechitVars_NHBinsData.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26");
		plotter_RechitVars_NHBinsData.SetSelectiveCuts("LLPskim", "jet0_NeutralHadEFrac < 0.6");  // BLINDED with track energy fraction (jet0_Track0Pt / jet0_Pt > 0.1) or neutral hadron fraction (jet0_NeutralHadEFrac < 0.6)
		plotter_RechitVars_NHBinsData.SetOutputDirectory("Data_NeutralHadBins");
		plotter_RechitVars_NHBinsData.Plot("ratio");

		class MiniTuplePlotter plotter_RechitVars_NHBins( filetags_LLP, path );
		plotter_RechitVars_NHBins.SetPlots(MC_allPlots);
		plotter_RechitVars_NHBins.SetTreeName( "NoSel" );	
		plotter_RechitVars_NHBins.SetOutputFileTag("NeutralHadBins_Jet40_v3.0_MC"); 	
		plotter_RechitVars_NHBins.SetComparisonCuts({Cut_NHadpt1, Cut_NHadpt2, Cut_NHadpt3, Cut_NHadpt4, Cut_NHadpt6, Cut_NHad1}); 
		plotter_RechitVars_NHBins.plot_log_ratio    = false; 
		plotter_RechitVars_NHBins.SetLegendNames({"Neutral Hadron 0-0.1", "Neutral Hadron 0.1-0.2", "Neutral Hadron 0.2-0.3", "Neutral Hadron 0.3-0.4", "Neutral Hadron 0.4-0.6", "Neutral Hadron 0.6-1"});
		plotter_RechitVars_NHBins.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_RechitVars_NHBins.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26");
		plotter_RechitVars_NHBins.SetOutputDirectory("MC_NeutralHadBins");
		plotter_RechitVars_NHBins.Plot("ratio");
	}

	if (Jet_bins) {
		// ----- Bin in jet pT ----- //

		cout<<endl;
		cout<<" ---------- Study 4: Bin in jet pT (MC and data) ---------- "<<endl;
		cout<<endl;

		class MiniTuplePlotter plotter_allVars_JetBinsData( filetags_data, path );
		plotter_allVars_JetBinsData.SetPlots(analysisVars); //allPlots);
		plotter_allVars_JetBinsData.SetTreeName( "WPlusJets" );	
		plotter_allVars_JetBinsData.SetOutputFileTag("JetpTBins_Jet40_v3.0"); 	
		plotter_allVars_JetBinsData.SetComparisonCuts({Cut_JetPt40, Cut_JetPt80, Cut_JetPt120, Cut_JetPt160, Cut_JetPt200}); 
		plotter_allVars_JetBinsData.plot_log_ratio    = false; 
		plotter_allVars_JetBinsData.SetLegendNames({"Data: Jet p_{T} 40-80 GeV", "Data: Jet p_{T} 80-120 GeV", "Data: Jet p_{T} 120-160 GeV", "Data: Jet p_{T} 160-200 GeV", "Data: Jet p_{T} 200+ GeV"});
		plotter_allVars_JetBinsData.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_allVars_JetBinsData.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26");
		plotter_allVars_JetBinsData.SetSelectiveCuts("LLPskim", "jet0_NeutralHadEFrac < 0.6");  // BLINDED with track energy fraction (jet0_Track0Pt / jet0_Pt > 0.1) or neutral hadron fraction (jet0_NeutralHadEFrac < 0.6)
		plotter_allVars_JetBinsData.SetOutputDirectory("Data_JetPtBins");
		plotter_allVars_JetBinsData.Plot("ratio");

		class MiniTuplePlotter plotter_allVars_JetBins( filetags_LLP, path );
		plotter_allVars_JetBins.SetPlots(analysisVars); // {P_jet0_Track0PtFrac}); //MC_allPlots);
		plotter_allVars_JetBins.SetTreeName( "NoSel" );	
		plotter_allVars_JetBins.SetOutputFileTag("JetpTBins_Jet40_v3.0_MC"); 	
		plotter_allVars_JetBins.SetComparisonCuts({Cut_JetPt40, Cut_JetPt80, Cut_JetPt120, Cut_JetPt160, Cut_JetPt200}); 
		plotter_allVars_JetBins.plot_log_ratio    = false; 
		plotter_allVars_JetBins.SetLegendNames({"Jet p_{T} 40-80 GeV", "Jet p_{T} 80-120 GeV", "Jet p_{T} 120-160 GeV", "Jet p_{T} 160-200 GeV", "Jet p_{T} 200+ GeV"});
		plotter_allVars_JetBins.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_allVars_JetBins.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26 && jet0_Track0Pt > 4");
		plotter_allVars_JetBins.SetOutputDirectory("MC_JetPtBins");
		plotter_allVars_JetBins.Plot("ratio");
	}

	if (BDT_bins) {
		// ----- Bin in BDT score ----- //

		// for mH = 125 now, BDT score for 125

		cout<<endl;
		cout<<" ---------- Study 5: Bin in BDT score (MC and data) ---------- "<<endl;
		cout<<endl;

		class MiniTuplePlotter plotter_allVars_BDTbinsData( filetags_data_v3pt1, path_v3pt1 );
		plotter_allVars_BDTbinsData.SetPlots(analysisVars); //allPlots);
		plotter_allVars_BDTbinsData.AddPlot({P_jet0_Pt});
		plotter_allVars_BDTbinsData.SetTreeName( "WPlusJets" );	
		plotter_allVars_BDTbinsData.SetOutputFileTag("BDTBins_Jet40_v3.1"); 	
		plotter_allVars_BDTbinsData.SetComparisonCuts({Cut_BDTscoreNPt99, Cut_BDTscoreNPt98, Cut_BDTscoreNPt95, Cut_BDTscore0, Cut_BDTscorePos}); 
		plotter_allVars_BDTbinsData.plot_log_ratio    = false; 
		plotter_allVars_BDTbinsData.SetLegendNames({"Data: BDT score -1 to -0.99", "Data: BDT score -0.99 to -0.98", "Data: BDT score -0.98 to -0.95", "Data: BDT score -0.95 to 0", "Data: BDT score 0 to 1"});
		plotter_allVars_BDTbinsData.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_allVars_BDTbinsData.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26");
		plotter_allVars_BDTbinsData.SetSelectiveCuts("LLPskim", "jet0_NeutralHadEFrac < 0.6");  // BLINDED with track energy fraction (jet0_Track0Pt / jet0_Pt > 0.1) or neutral hadron fraction (jet0_NeutralHadEFrac < 0.6)
		plotter_allVars_BDTbinsData.SetOutputDirectory("Data_BDTBins");
		plotter_allVars_BDTbinsData.Plot("ratio");

		class MiniTuplePlotter plotter_allVars_BDTBins( filetags_LLP_v3pt1, path_v3pt1 );
		plotter_allVars_BDTBins.SetPlots(analysisVars); // {P_jet0_Track0PtFrac}); //MC_allPlots);
		plotter_allVars_BDTBins.AddPlot({P_jet0_Pt});
		plotter_allVars_BDTBins.SetTreeName( "NoSel" );	
		plotter_allVars_BDTBins.SetOutputFileTag("BDTBins_Jet40_v3.1_MC"); 	
		plotter_allVars_BDTBins.SetComparisonCuts({Cut_BDTscoreN, Cut_BDTscorePt5, Cut_BDTscorePt9, Cut_BDTscorePt95, Cut_BDTscore1}); 
		plotter_allVars_BDTBins.plot_log_ratio    = false; 
		plotter_allVars_BDTBins.SetLegendNames({"BDT score -1 to 0", "BDT score 0 to 0.5", "BDT score 0.5 to 0.9", "BDT score 0.9 to 0.95", "BDT score 0.95 to 1"});
		plotter_allVars_BDTBins.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_allVars_BDTBins.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26 && jet0_Track0Pt > 4");
		plotter_allVars_BDTBins.SetSelectiveCuts("LLP_MC", Cut_LLPinHCAL_Jet0);
		plotter_allVars_BDTBins.SetOutputDirectory("MC_BDTBins");
		plotter_allVars_BDTBins.Plot("ratio");
	}

	if (Overlay_all) {
		// ----- Overlay for MC signal, MC QCD, data ----- //

		cout<<endl;
		cout<<" ---------- Study 6: Overlay all minituples (QCD, LLP, skim)! ---------- "<<endl;
		cout<<endl;

		class MiniTuplePlotter plotter_overlay( filetags_all, path );
		plotter_overlay.SetPlots(analysisVars);
		plotter_overlay.SetTreeName( "NoSel" );	
		plotter_overlay.SetOutputFileTag("Overlay_v3.0"); 	
		plotter_overlay.plot_log_ratio    = false; 
		plotter_overlay.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_overlay.SetLegendNames({"LLP skim", "QCD MC", "LLP in Tracker: R < 10cm", "LLP in HCAL-D2"});
		plotter_overlay.colors = { kBlack, kRed-3, kGreen+1, kGreen+3 };
		plotter_overlay.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26");
		plotter_overlay.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL2_Jet0}, "MCsignalLLP");
		// plotter_overlay.SetSelectiveCuts("LLPskim", "jet0_NeutralHadEFrac < 0.6");  // BLINDED with track energy fraction (jet0_Track0Pt / jet0_Pt > 0.1) or neutral hadron fraction (jet0_NeutralHadEFrac < 0.6)
		plotter_overlay.SetOutputDirectory("Overlay");
		plotter_overlay.Plot("ratio");

		// 2D spread of eta - phi, for leading and subleading jet
		class MiniTuplePlotter plotter_overlay2D( filetags_all, path );
		plotter_overlay2D.SetTreeName( "NoSel" );	
		plotter_overlay2D.SetOutputFileTag("Overlay_v3.0"); 	
		plotter_overlay2D.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0}, "MCsignalLLP");
		plotter_overlay2D.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );			
		plotter_overlay2D.SetCuts("abs(jet0_Eta) <= 1.26");
		plotter_overlay2D.SetOutputDirectory("Overlay/2D");
		plotter_overlay2D.SetPlots2D({Hist1_Hist2(P_jet0_Pt, P_jet0_EtaPhiQuadSpread), Hist1_Hist2(P_jet0_Pt, P_jet0_EtaPhiQuadSpread_energy) });
		plotter_overlay2D.PlotMany2D();
	}

	if (Matching_efficiency) {
		// ----- LLP to jet matching efficiencies -----//

		cout<<endl;
		cout<<" ---------- MC Study 7: LLP - jet match (efficiency plots) ---------- "<<endl;
		cout<<endl;

		class MiniTuplePlotter plotter_LLP_Jet_PtDiff( filetags_LLP, path );
		plotter_LLP_Jet_PtDiff.SetPlots({P_jet0_llp0_ptDiff, P_jet0_llp1_ptDiff});
		plotter_LLP_Jet_PtDiff.SetTreeName( "NoSel" );	
		plotter_LLP_Jet_PtDiff.SetOutputFileTag("Jet40_v3.0_MC"); 	
		plotter_LLP_Jet_PtDiff.SetComparisonCuts({Cut_LLP0inCR, Cut_LLP0inTrackerNP, Cut_LLP0inECAL, Cut_LLPinHCAL_d1, Cut_LLP0inHCAL_d2, Cut_LLP0inHCAL_d34}); 
		plotter_LLP_Jet_PtDiff.plot_log_ratio    = false; 
		plotter_LLP_Jet_PtDiff.SetLegendNames({"LLP0: Tracker: R < 10cm", "LLP0: Tracker: R > 10cm", "LLP0: ECAL", "LLP0: HCAL-D1", "LLP0: HCAL-D2", "LLP0: HCAL-D34"});
		plotter_LLP_Jet_PtDiff.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_LLP_Jet_PtDiff.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26 && jet0_isMatchedTo == 0");  
		plotter_LLP_Jet_PtDiff.SetOutputDirectory("MC_LLP_Jet_Match");
		plotter_LLP_Jet_PtDiff.Plot();	

		class MiniTuplePlotter plotter_LLP_Jet_Match( filetags_LLP, path );
		plotter_LLP_Jet_Match.SetPlots({P_LLP0_DecayR, P_LLP0_isTruthMatched_Eta});
		plotter_LLP_Jet_Match.SetTreeName( "NoSel" );	
		plotter_LLP_Jet_Match.SetOutputFileTag("Jet40_v3.0_MC"); 	
		plotter_LLP_Jet_Match.SetComparisonCuts({"abs(jet0_Eta) <= 1.26 || abs(jet1_Eta) <= 1.26 || abs(jet2_Eta) <= 1.26", "LLP0_isTruthMatched == 1", "LLP0_isTruthMatched_Jet40 == 1", "LLP0_isTruthMatched_Jet60 == 1", "LLP0_isTruthMatched_Jet80 == 1", "LLP0_isTruthMatched_Jet100 == 1"}); 
		plotter_LLP_Jet_Match.plot_log_ratio    = false; 
		plotter_LLP_Jet_Match.plot_norm 		= false;
		plotter_LLP_Jet_Match.SetLegendNames({"LLP0 Decay Position", "LLP0 Matched, Jet p_{T} > 0", "LLP0 Matched, Jet p_{T} > 40", "LLP0 Matched, Jet p_{T} > 60", "LLP0 Matched, Jet p_{T} > 80", "LLP0 Matched, Jet p_{T} > 100"});
		plotter_LLP_Jet_Match.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_LLP_Jet_Match.SetCuts("LLP0_Pt > 40 && abs(LLP0_Eta) <= 1.26"); 
		plotter_LLP_Jet_Match.SetOutputDirectory("MC_LLP_Jet_Match");
		plotter_LLP_Jet_Match.Plot("ratio");

		class MiniTuplePlotter plotter_LLP_Jet_Match_tight( filetags_LLP, path );
		plotter_LLP_Jet_Match_tight.SetPlots({P_LLP0_DecayR});
		plotter_LLP_Jet_Match_tight.SetTreeName( "NoSel" );	
		plotter_LLP_Jet_Match_tight.SetOutputFileTag("Jet40_v3.0_MC"); 	
		plotter_LLP_Jet_Match_tight.SetComparisonCuts({"abs(jet0_Eta) <= 1.26 || abs(jet1_Eta) <= 1.26 || abs(jet2_Eta) <= 1.26", "LLP0_isTruthMatched == 1 && abs(LLP0_isTruthMatched_Eta) <= 1.26", "LLP0_isTruthMatched_Jet40 == 1 && abs(LLP0_isTruthMatched_Jet40Eta) <= 1.26", "LLP0_isTruthMatched_Jet60 == 1 && abs(LLP0_isTruthMatched_Jet60Eta) <= 1.26", "LLP0_isTruthMatched_Jet80 == 1 && abs(LLP0_isTruthMatched_Jet80Eta) <= 1.26", "LLP0_isTruthMatched_Jet100 == 1 && abs(LLP0_isTruthMatched_Jet100Eta) <= 1.26"}); 
		plotter_LLP_Jet_Match_tight.plot_log_ratio    = false; 
		plotter_LLP_Jet_Match_tight.plot_norm 		= false;
		plotter_LLP_Jet_Match_tight.SetLegendNames({"LLP0 Decay Position", "LLP0 Matched, Jet p_{T} > 0", "LLP0 Matched, Jet p_{T} > 40", "LLP0 Matched, Jet p_{T} > 60", "LLP0 Matched, Jet p_{T} > 80", "LLP0 Matched, Jet p_{T} > 100"});
		plotter_LLP_Jet_Match_tight.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_LLP_Jet_Match_tight.SetCuts("LLP0_Pt > 40 && abs(LLP0_Eta) <= 1.26"); 
		plotter_LLP_Jet_Match_tight.SetOutputDirectory("MC_LLP_Jet_Match_Tight");
		plotter_LLP_Jet_Match_tight.Plot("ratio");
	}

	// ARCHIVE

	// ----- Study low energy leading rechits in signal region -----//
	// This study was used to show that the low energy rechits (giving a double peak distribution) were coming from jets in HE. Now add jet eta restrictions on all plots, so this study is redundant

	// cout<<endl;
	// cout<<" ---------- MC Study 6: Study low energy leading rechits in signal region ---------- "<<endl;
	// cout<<endl;

	// leading jet rechit variables, with eta cut on jet
	// class MiniTuplePlotter plotter_RechitVars( filetags_LLP, path );
	// plotter_RechitVars.SetPlots(rechitVars);
	// plotter_RechitVars.SetTreeName( "NoSel" );	
	// plotter_RechitVars.SetOutputFileTag("Jet40_JetHBtight_CR_SR_v3.0_MC"); 	
	// plotter_RechitVars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0});
	// plotter_RechitVars.plot_log_ratio    = false; 
	// plotter_RechitVars.SetLegendNames({"Tracker: R < 10cm", "Tracker: R > 10cm", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
	// plotter_RechitVars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	// plotter_RechitVars.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) < 1.25");  // 40 GeV jet cut on all plots! 
	// plotter_RechitVars.SetOutputDirectory("MC_CR_SR_LeadingRechitStudy");
	// plotter_RechitVars.Plot("ratio");

	// class MiniTuplePlotter plotter_LRechit_Bins( filetags_LLP, path );
	// plotter_LRechit_Bins.SetPlots(allPlots);
	// plotter_LRechit_Bins.SetTreeName( "NoSel" );	
	// plotter_LRechit_Bins.SetOutputFileTag("LRechit8_Jet40_v3.0_MC"); 	
	// plotter_LRechit_Bins.SetComparisonCuts({"jet0_LeadingRechitE < 8", Cut_LLPinHCAL34_Jet0 + "jet0_LeadingRechitE < 8", "jet0_LeadingRechitE >= 8"}); 
	// plotter_LRechit_Bins.plot_log_ratio    = false; 
	// plotter_LRechit_Bins.SetLegendNames({"Leading Rechit E < 8 GeV", "Leading Rechit E < 8 GeV and LLP in HCALd34", "Leading Rechit E >= 8 GeV"});
	// plotter_LRechit_Bins.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	// plotter_LRechit_Bins.SetCuts("jet0_Pt >= 40" + Cut_LLPinHCAL_Jet0);  // 40 GeV jet cut on all plots!
	// plotter_LRechit_Bins.SetOutputDirectory("MC_LeadingRechitStudy8GeV");
	// plotter_LRechit_Bins.Plot();

	// class MiniTuplePlotter plotter_LRechit_Bins2D( filetags_LLP, path );
	// plotter_LRechit_Bins2D.SetTreeName( "NoSel" );	
	// plotter_LRechit_Bins2D.SetOutputFileTag("LRechit8_Jet40_v3.0_MC"); 	
	// plotter_LRechit_Bins2D.SetComparisonCuts({"jet0_LeadingRechitE < 8", Cut_LLPinHCAL34_Jet0 + "jet0_LeadingRechitE < 8", "jet0_LeadingRechitE >= 8"}); 
	// plotter_LRechit_Bins2D.SetLegendNames({"Leading Rechit E < 8 GeV", "LLP in HCALd34 and Leading Rechit E < 8 GeV ", "Leading Rechit E >= 8 GeV"});
	// plotter_LRechit_Bins2D.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	// plotter_LRechit_Bins2D.SetCuts("jet0_Pt >= 40" + Cut_LLPinHCAL_Jet0);  // 40 GeV jet cut on all plots!
	// plotter_LRechit_Bins2D.SetOutputDirectory("MC_LeadingRechitStudy8GeV");
	// plotter_LRechit_Bins2D.Plot2D(P_jet0_MatchedLLP_Eta, P_jet0_MatchedLLP_DecayR);

	// ----- Cut based selections -- cut on neutral hadron fraction -----//
	// binning in neutral hadron fraction is more informative for now! 
	
	// cout<<endl;
	// cout<<" ---------- CR / SR Study 2: Cut based selections. Cut on neutral hadron energy fraction ---------- "<<endl;
	// cout<<endl;

	// class MiniTuplePlotter plotter_CutBased( filetags_LLP, path );
	// plotter_CutBased.SetPlots(allPlots);
	// plotter_CutBased.SetTreeName( "NoSel" );	
	// plotter_CutBased.SetOutputFileTag("NeutralHadpt6_Jet40_CR_SR_v3.0_MC"); 	
	// plotter_CutBased.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0}); // , Cut_LLPinHCAL4_Jet0});
	// plotter_CutBased.plot_log_ratio    = false; 
	// plotter_CutBased.SetLegendNames({"Tracker: R < 10cm", "Tracker: R > 10cm", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
	// plotter_CutBased.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	// plotter_CutBased.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26 && jet0_NeutralHadEFrac > 0.6"); 
	// plotter_CutBased.SetOutputDirectory("MC_CR_SR_NeutralHadCut");
	// plotter_CutBased.Plot("ratio"); 
}
