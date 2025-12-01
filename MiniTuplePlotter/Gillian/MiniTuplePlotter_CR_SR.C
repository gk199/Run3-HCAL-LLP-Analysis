#include "../MiniTuplePlotter.h"
#include "TString.h"
#include "../RegionCuts.h"

// Goals of this study: use the LLP MC sample, split into different regions to understand how variables change by region. Also started overlaying LLP skim data variables.
// Mostly worked on during October - November 2023, early spring, summer 2024.
// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_CR_SR(){

	// List where minituples are stored
	
	string path_v3pt0 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_";
	string path_v3pt1 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.1/minituple_";
	string path_v3pt2 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.2/minituple_";
	string path_v3pt3 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_";
	string path_v3pt4 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.4/minituple_";
	string path_v3pt5 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.5/minituple_";
	string path_v3pt6 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.6/minituple_";
	string path_v3pt7 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.7/minituple_";
	string path 	  = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.8/minituple_";
	string path_test  = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.10/minituple_";

	map<string,vector<string>> filetags;
	filetags["LLP125_mX15"]	= { "v3.8_LLPskim_Run2023Cv4_2024_06_03", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TEST"};
	filetags["LLP350_mX80"]	= { "v3.8_LLPskim_Run2023Cv4_2024_06_03", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_TEST"};
	filetags["LLP125"]		= { "v3.8_LLPskim_Run2023Cv4_2024_06_03", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_batch2"};
	filetags["LLP250"]		= { "v3.8_LLPskim_Run2023Cv4_2024_06_03", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_06_03_batch2"};
	filetags["LLP350"]		= { "v3.8_LLPskim_Run2023Cv4_2024_06_03", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_06_03_batch2"};

	vector<string> filetags_data 		= { "v3.8_LLPskim_Run2023_HADD" };
	vector<string> filetags_data2 		= { "v3.8_LLPskim_Run2023_HADD", "v3.8_LLPskim_Run2023Cv4_2024_06_03" };
	vector<string> filetags_LLP 		= { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TEST" };	
	vector<string> filetags_all 		= { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TEST", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_TEST", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_batch2", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_06_03_batch2", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_06_03_batch2"};

	vector<string> filetags_test		= { "v3.10_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_28Jan_DepthTimingFlag_HADD"};
	vector<string> filetags_test_data	= { "v3.10_Run2023D-EXOLLPJetHCAL-v1_28Jan_DepthTimingFlag_Hadd"};

	vector<string> BDT_files 			= {"v3.8_LLPskim_Run2023_HADD", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TEST", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_TEST"};
	vector<string> filetags_all_v3pt0 	= { "v3.0_LLPskim_Run2023Bv1_2023Cv2_2023_11_23", "v2.0_MC_QCD_250k_2023_10_18", "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2023_11_23"};
	vector<string> filetags_QCD 		= { "v2.0_MC_QCD_250k_2023_10_18" };	

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
										P_jet0_Track0Pt, P_jet0_Track0dEta, P_jet0_Track0dPhi, P_jet0_Track0dR, P_jet0_Track1Pt, P_jet0_Track1dEta, P_jet0_Track1dPhi, P_jet0_Track1dR, 
										P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4,
										P_jet0_Eta, P_jet0_Phi, P_jet0_Mass,
										P_jet0_LeadingRechitE, P_jet0_LeadingRechitEFracJet, P_jet0_Sphiphi};
	vector<PlotParams> analysisVars_perJet	= { P_perJet_NeutralHadEFrac, P_perJet_ChargedHadEFrac, P_perJet_MuonEFrac, P_perJet_PhoEFrac, P_perJet_EleEFrac,
										// P_perJet_NeutralHadEFrac_zero, P_perJet_ChargedHadEFrac_zero, P_perJet_MuonEFrac_zero, P_perJet_PhoEFrac_zero, P_perJet_EleEFrac_zero,
										// P_perJet_Track0Pt, P_perJet_Track1Pt, 
										P_perJet_Track0PtFrac, P_perJet_Track1PtFrac,
										P_perJet_Track0dEta, P_perJet_Track0dPhi, P_perJet_Track0dR, P_perJet_Track1dEta, P_perJet_Track1dPhi, P_perJet_Track1dR, 
										P_perJet_EnergyFrac_Depth1, P_perJet_EnergyFrac_Depth2, P_perJet_EnergyFrac_Depth3, P_perJet_EnergyFrac_Depth4,
										P_perJet_Eta, // P_perJet_Phi, P_perJet_Mass, P_perJet_LeadingRechitE, P_perJet_AllRechitE, P_perJet_AllRechitEFracJet,
										P_perJet_LeadingRechitEFracJet, 
										P_perJet_Sphiphi}; // P_perJet_Setaeta, P_perJet_Setaphi,
										// P_perJet_Track0dzOverErr, P_perJet_Track1dzOverErr, P_perJet_Track0dxyOverErr, P_perJet_Track1dxyOverErr,
										// P_perJet_Track0dzToPV, P_perJet_Track1dzToPV, P_perJet_Track0dxyToBS, P_perJet_Track1dxyToBS,
										// P_perJet_OtherLLP_DecayR};	
										// P_jet0_Track0nMissingInnerHits, P_jet0_Track0nMissingOuterHits, P_jet0_Track0nPixelHits, P_jet0_Track0nHits,			// only in v3.2 or newer
										// P_jet0_EtaSpread, P_jet0_PhiSpread, P_jet0_EtaSpread_energy, P_jet0_PhiSpread_energy,
										// P_jet0_TDCaverage, P_jet0_TDCaverage_Eweight, P_jet0_TDCnDelayed, 

	MC_allPlots.insert(MC_allPlots.begin(), allPlots.begin(), allPlots.end());

	bool DepthFlag = false;
	bool Depth_CR_VR = false;
	bool dPhi_CR_VR = true;
	bool Skim_WJets = false;			// BDT input variables, plot LLP skim vs W+Jets selection
	bool LLP_WJets = false;				// analysis variables for LLP and W+Jets overlayed
	bool track_dR_study = false;		// track vars with diff dR cuts, dR with diff track cuts
	bool overlay_LLP = false;			// overlay analysis variables for each LLP mass point
	bool Bkg_est = false;				// look at jet tagging vars by jet eta for probability estimations

	bool LLP_all = false;				// all variables, track vars with leading track < 4 study, eta-phi spread
	bool Jet_vars = false; 				// jet kinematics without a pT cut
	bool NH_bins = false;				// bin in neutral hadron fraction
	bool Jet_bins = false;				// bin in jet pT
	bool BDT_bins = false;				// bin in BDT score
	bool Overlay_all = false;			// overlay LLP MC, QCD MC, LLP skim
	bool Matching_efficiency = false;	// LLP - jet matching efficiency
	bool perJet = false;

	// ----- Depth flag studies ---- //
	if (DepthFlag) {
		cout<<endl;
		cout<<" ---------- CR / SR Study: Depth flag in PF jets ---------- "<<endl;
		cout<<endl;

		class MiniTuplePlotter plotter_depth( filetags_test, path_test );
		plotter_depth.SetPlots({P_jet0_dR_L1jet, P_jet0_DepthTowers, P_jet0_TimingTowers, P_jet0_FlaggedTowers}); 
		plotter_depth.SetTreeNames( {"NoSel"} );	
		plotter_depth.SetOutputFileTag("Overlay_DepthTowers_v3.10");
		plotter_depth.plot_log_ratio    = false; 
		plotter_depth.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_depth.SetCuts("jet0_Pt >= 100 && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 && jet0_dR_L1jet < 0.4");
		// plotter_depth.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0}, "LLP_MC");
		// plotter_depth.SetLegendNames({"LLP in tracker <= 10cm", "LLP in tracker > 10cm", "LLP in ECAL", "LLP in HCAL, D1", "LLP in HCAL, D2", "LLP in HCAL, D34"});
		// plotter_depth.colors = { kBlack, kGray, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed };
		plotter_depth.SetComparisonCuts({Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0}, "LLP_MC");
		plotter_depth.SetLegendNames({"LLP in tracker > 10cm", "LLP in ECAL", "LLP in HCAL, D1", "LLP in HCAL, D2", "LLP in HCAL, D34"});
		plotter_depth.colors = { kGray, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed };
		plotter_depth.SetVariableBins({-0.25,0.25,0.75,1.25,1.75,2.25,2.75,3.25,3.75,4.25,4.75,5.25,5.75,6.25,6.75,7.25,7.75,8.25});
		plotter_depth.SetOutputDirectory("DepthTowers");
		//plotter_depth.plot_norm = false;
		plotter_depth.Plot();
		plotter_depth.ClearFileTrees();
		plotter_depth.SetPlots({P_jet0_DepthTowers, P_jet0_TimingTowers, P_jet0_FlaggedTowers}); 
		plotter_depth.SetOutputFileTag("Overlay_DepthTowers_v3.10_2plus");
		plotter_depth.SetVariableBins({-0.25,0.25,0.75,1.25,1.75,8});
		plotter_depth.Plot();

		// W+jets background
		class MiniTuplePlotter bkg_plotter_depth( filetags_test_data, path_test );
		bkg_plotter_depth.SetPlots({P_jet0_DepthTowers, P_jet0_TimingTowers, P_jet0_FlaggedTowers}); 
		bkg_plotter_depth.SetTreeNames( {"WPlusJets"} );	
		bkg_plotter_depth.SetOutputFileTag("Overlay_DepthTowers_v3.10_data");
		bkg_plotter_depth.plot_log_ratio    = false; 
		bkg_plotter_depth.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		bkg_plotter_depth.SetCuts("jet0_Pt >= 100 && abs(jet0_Eta) <= 1.26 && jet0_dR_L1jet < 0.4");
		bkg_plotter_depth.SetComparisonCuts({Cut_L1_LLPtriggered0, Cut_L1_LLPtriggered1}, "EXOLLPJetHCAL");
		bkg_plotter_depth.SetLegendNames({"W+jets, not triggered", "W+jets, triggered"});
		bkg_plotter_depth.colors = { kViolet+4, kMagenta-7, kRed };
		bkg_plotter_depth.SetVariableBins({-0.25,0.25,0.75,1.25,1.75,2.25,2.75,3.25,3.75,4.25,4.75,5.25,5.75,6.25,6.75,7.25,7.75,8.25});
		bkg_plotter_depth.SetOutputDirectory("DepthTowers");
		//bkg_plotter_depth.plot_norm = false;
		bkg_plotter_depth.Plot();
		bkg_plotter_depth.ClearFileTrees();
		bkg_plotter_depth.SetPlots({P_jet0_DepthTowers, P_jet0_TimingTowers, P_jet0_FlaggedTowers}); 
		bkg_plotter_depth.SetOutputFileTag("Overlay_DepthTowers_v3.10_data_2plus");
		bkg_plotter_depth.SetVariableBins({-0.25,0.25,0.75,1.25,1.75,8});
		bkg_plotter_depth.Plot();

		// plot vs LLP decay R
		class MiniTuplePlotter decay_R( filetags_test, path_test );
		decay_R.SetPlots({P_jet0_MatchedLLP_DecayR}); 
		decay_R.SetTreeNames( {"NoSel"} );	
		decay_R.SetOutputFileTag("Overlay_LLPr_v3.10");
		decay_R.plot_log_ratio    = false; 
		decay_R.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		decay_R.SetCuts("jet0_Pt >= 100 && abs(jet0_Eta) <= 1.26 && jet0_L1trig_Matched == 1 && jet0_dR_L1jet < 0.4");
		decay_R.SetComparisonCuts({Cut_0Towers, Cut_1Towers, Cut_2Towers});
		decay_R.SetLegendNames({"No flagged towers", "1 flagged tower", "2+ flagged towers"});
		decay_R.colors = { kViolet+4, kAzure+7, kMagenta-7 };
		decay_R.SetOutputDirectory("DepthTowers");
		//decay_R.plot_norm = false;
		decay_R.Plot();
		decay_R.ClearFileTrees();
		decay_R.SetOutputFileTag("Overlay_LLPr_v3.10_depth");
		decay_R.SetComparisonCuts({Cut_0DepthTowers, Cut_1DepthTowers, Cut_2DepthTowers});
		decay_R.SetLegendNames({"No flagged depth towers", "1 flagged tower", "2+ flagged towers"});		
		decay_R.Plot();
		decay_R.ClearFileTrees();
		decay_R.SetOutputFileTag("Overlay_LLPr_v3.10_timing");
		decay_R.SetComparisonCuts({Cut_0TimingTowers, Cut_1TimingTowers, Cut_2TimingTowers});
		decay_R.SetLegendNames({"No flagged timing towers", "1 flagged tower", "2+ flagged towers"});		
		decay_R.Plot();
	}
	if (Depth_CR_VR) { // July 2025
		// pT spectrum of CR and VR
		class MiniTuplePlotter CR_VR_kinematics( {"LLPskiDisplacedJet_Run2023C-EXOLLPJetHCAL-PromptReco-v4_AOD_2025_07_21_scores"}, {"/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v4.1/"} );
		CR_VR_kinematics.SetPlots({P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_dEta, P_jet0_dPhi}); 
		CR_VR_kinematics.SetTreeNames( {"NoSel"} );	
		CR_VR_kinematics.SetOutputFileTag("Overlay_DepthJet0_v4.1_data");
		CR_VR_kinematics.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		CR_VR_kinematics.SetCuts("jet0_DepthTagCand == 1 && jet0_scores_depth_LLPanywhere >= 0.95");
		CR_VR_kinematics.SetComparisonCuts({Cut_CR, Cut_VR}),
		CR_VR_kinematics.SetLegendNames({"CR", "VR"});
		CR_VR_kinematics.colors = { kViolet+4, kMagenta-7 };
		CR_VR_kinematics.SetOutputDirectory("CR_VR");
		CR_VR_kinematics.plot_log    = false; 
		CR_VR_kinematics.NBins = 30;
		CR_VR_kinematics.Plot();
		CR_VR_kinematics.ClearFileTrees();
		CR_VR_kinematics.SetOutputFileTag("Overlay_DepthJet0_anyDecay_v4.1_data");
		CR_VR_kinematics.SetCuts("jet0_DepthTagCand == 1 && jet0_scores_depth_LLPanywhere >= 0.95");
		CR_VR_kinematics.Plot();
		CR_VR_kinematics.ClearFileTrees();
		CR_VR_kinematics.SetOutputFileTag("Overlay_InclusiveJet1Bins_v4.1_data");
		CR_VR_kinematics.SetCuts("jet1_InclTagCand == 1");
		CR_VR_kinematics.SetComparisonCuts({"jet1_scores_inc_train80 >= 0 && jet1_scores_inc_train80 <= 0.2", "jet1_scores_inc_train80 > 0.2 && jet1_scores_inc_train80 <= 0.4", "jet1_scores_inc_train80 > 0.4 && jet1_scores_inc_train80 <= 0.6", "jet1_scores_inc_train80 > 0.6 && jet1_scores_inc_train80 <= 0.8"}),
		CR_VR_kinematics.SetLegendNames({"Inclusive DNN 0-0.2", "Inclusive DNN 0.2-0.4", "Inclusive DNN 0.4-0.6", "Inclusive DNN 0.6-0.8"});
		CR_VR_kinematics.Plot();

		// evaluate distributions when leading jet is inclusive and sub-leading is depth
		CR_VR_kinematics.ClearFileTrees();
		CR_VR_kinematics.SetPlots({P_jet1_Pt, P_jet1_Eta, P_jet1_Phi, P_jet0_dEta, P_jet0_dPhi}); 
		CR_VR_kinematics.SetOutputFileTag("Overlay_DepthJet1_v4.1_data");
		CR_VR_kinematics.SetCuts("jet1_DepthTagCand == 1 && jet1_scores_depth_hcal >= 0.95");
		CR_VR_kinematics.SetComparisonCuts({Cut_CR_jet0, Cut_VR_jet0}),
		CR_VR_kinematics.NBins = 30;
		CR_VR_kinematics.Plot();
		CR_VR_kinematics.ClearFileTrees();
		CR_VR_kinematics.SetOutputFileTag("Overlay_DepthJet1_anyDecay_v4.1_data");
		CR_VR_kinematics.SetCuts("jet1_DepthTagCand == 1 && jet1_scores_depth_LLPanywhere >= 0.95");
		CR_VR_kinematics.Plot();		
		CR_VR_kinematics.ClearFileTrees();
		CR_VR_kinematics.SetOutputFileTag("Overlay_InclusiveJet0Bins_v4.1_data");
		CR_VR_kinematics.SetCuts("jet0_InclTagCand == 1");
		CR_VR_kinematics.SetComparisonCuts({"jet0_scores_inc_train80 >= 0 && jet0_scores_inc_train80 <= 0.2", "jet0_scores_inc_train80 > 0.2 && jet0_scores_inc_train80 <= 0.4", "jet0_scores_inc_train80 > 0.4 && jet0_scores_inc_train80 <= 0.6", "jet0_scores_inc_train80 > 0.6 && jet0_scores_inc_train80 <= 0.8"}),
		CR_VR_kinematics.SetLegendNames({"Inclusive DNN 0-0.2", "Inclusive DNN 0.2-0.4", "Inclusive DNN 0.4-0.6", "Inclusive DNN 0.6-0.8"});
		CR_VR_kinematics.Plot();

		class MiniTuplePlotter CR_VR_kinematics_LLP( {"HToSSTo4b_125_50_CTau3000_batch2_allscores", "HToSSTo4b_250_120_CTau10000_batch2_allscores", "HToSSTo4b_350_160_CTau10000_batch2_allscores"}, {"/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_"} );
		CR_VR_kinematics_LLP.SetPlots({P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_dEta, P_jet0_dPhi}); 
		CR_VR_kinematics_LLP.SetTreeName( "NoSel" );	
		CR_VR_kinematics_LLP.SetOutputFileTag("Overlay_DepthJet0_v3.16_LLP");
		CR_VR_kinematics_LLP.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		CR_VR_kinematics_LLP.SetCuts("jet0_DepthTagCand == 1 && jet0_scores_depth_hcal >= 0.95");
		CR_VR_kinematics_LLP.SetComparisonCuts({Cut_CR, Cut_VR}),
		CR_VR_kinematics_LLP.SetLegendNames({"mH=125 CR", "mH=125 VR", "mH=250 CR", "mH=250 VR", "mH=350 CR", "mH=350 VR"});
		CR_VR_kinematics_LLP.colors = { kViolet+4, kMagenta-7 };
		CR_VR_kinematics_LLP.SetOutputDirectory("CR_VR");
		CR_VR_kinematics_LLP.plot_log    = false; 
		CR_VR_kinematics_LLP.NBins = 30;
		CR_VR_kinematics_LLP.Plot();		
	}
	if (dPhi_CR_VR) { // July 2025 

		class MiniTuplePlotter dPhi_plotter2( {"minituple_LLPskim_2023Cv1_allscores"}, {"/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v4.1/"} );
		// class MiniTuplePlotter dPhi_plotter2( {"HToSSTo4b_125_50_CTau3000_batch2_allscores", "HToSSTo4b_250_120_CTau10000_batch2_allscores", "HToSSTo4b_350_160_CTau10000_batch2_allscores", "LLPskim_2023Cv4_allscores"}, {"/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_"} );
		dPhi_plotter2.SetPlots({P_jet0_dPhi}); 
		dPhi_plotter2.SetTreeName( "NoSel" );	
		dPhi_plotter2.SetOutputFileTag("Overlay_dPhi_v4.1");
		dPhi_plotter2.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		dPhi_plotter2.SetCuts("jet0_DepthTagCand == 1 && jet0_scores_depth_LLPanywhere >= 0.2"); // && Flag_METFilters_2022_2023_PromptReco");
		dPhi_plotter2.SetComparisonCuts({Cut_CR, Cut_VR}, "LLPskim");
		// dPhi_plotter2.SetLegendNames({"mH=125", "mH=250", "mH=350", "LLP skim CR", "LLP skim VR"});
		dPhi_plotter2.SetLegendNames({"LLP skim CR", "LLP skim VR"});
		// dPhi_plotter2.colors = { 40, 45, 38, kGreen-8, kGreen-2 };
		dPhi_plotter2.colors = { 40, 45 };
		dPhi_plotter2.SetOutputDirectory("dPhi");
		dPhi_plotter2.plot_log    = false; 
		dPhi_plotter2.NBins = 30;
		dPhi_plotter2.Plot();	

		class MiniTuplePlotter dPhi_cuts( {"minituple_LLPskim_2023Cv1_allscores"}, {"/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v4.1/"} );
		// dPhi_cuts.SetPlots({P_jet, P_jet0_Pt, P_validJet, P_pho, P_ele, P_muon, P_eventHT, P_met_Pt, P_met_Phi, P_met_SumEt, P_jet0met_dPhi, P_jet1met_dPhi}); 
		dPhi_cuts.SetPlots({P_jet0_Phi, P_jet1_Phi, P_jet0met_dPhi, P_jet1met_dPhi}); 
		dPhi_cuts.SetTreeName( "NoSel" );	
		dPhi_cuts.SetOutputFileTag("Overlay_dPhi_v4.1");
		dPhi_cuts.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		dPhi_cuts.SetCuts("jet0_DepthTagCand == 1 && jet0_scores_depth_LLPanywhere >= 0.2 && abs(jet0_jet1_dPhi) < 0.2"); // && Flag_METFilters_2022_2023_PromptReco");
		dPhi_cuts.SetComparisonCuts({Cut_CR, Cut_VR}, "LLPskim");
		dPhi_cuts.SetLegendNames({"LLP skim CR", "LLP skim VR"});
		dPhi_cuts.colors = { 40, 45 };
		dPhi_cuts.SetOutputDirectory("dPhi");
		dPhi_cuts.plot_log    = false; 
		dPhi_cuts.NBins = 20;
		dPhi_cuts.Plot();	

		bool v3 = true;
		class MiniTuplePlotter dPhi_plotter( {"LLPskim_2023Cv4_allscores"}, {v3? "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.16/minituple_" 
																				: "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v4.1/minituple_"} );
		dPhi_plotter.SetTreeName( "NoSel" );	
		dPhi_plotter.SetOutputFileTag("Overlay_dPhi_v4.1"); 	
		dPhi_plotter.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );			
		if (v3) {
			dPhi_plotter.SetCuts("jet0_DepthTagCand == 1 && jet0_scores_depth_anywhere >= 0.5");
			dPhi_plotter.SetOutputFileTag("Overlay_dPhi_v3.16"); 	
		}
		else {
			dPhi_plotter.SetCuts("jet0_DepthTagCand == 1 && jet0_scores_depth_LLPanywhere >= 0.5");
		}
		dPhi_plotter.SetComparisonCuts({Cut_CR, Cut_VR});
		dPhi_plotter.SetLegendNames({"LLP skim CR", "LLP skim VR"});
		if (v3) dPhi_plotter.SetPlots2D({Hist1_Hist2(P_jet0_dPhi_v316, P_jet0_Phi), Hist1_Hist2(P_jet0_dEta, P_jet0_dPhi_v316), Hist1_Hist2(P_jet0met_dPhi, P_jet0_dPhi_v316) });
		else dPhi_plotter.SetPlots2D({Hist1_Hist2(P_jet0_dPhi, P_jet0_Phi), Hist1_Hist2(P_jet0_dEta, P_jet0_dPhi), Hist1_Hist2(P_jet0met_dPhi, P_jet0_dPhi) });
		dPhi_plotter.SetOutputDirectory("2D_dPhi_dEta");
		dPhi_plotter.PlotMany2D();
		dPhi_plotter.ClearFileTrees();
		dPhi_plotter.SetCuts("jet0_DepthTagCand == 1 " + Cut_CR);
		if (v3) {
			dPhi_plotter.SetOutputFileTag("CRratio_Overlay_dPhi_v3.16"); 	
			dPhi_plotter.SetComparisonCuts({"jet0_scores_depth_anywhere >= 0", "jet0_scores_depth_anywhere >= 0.2"});
		}
		else {
			dPhi_plotter.SetOutputFileTag("CRratio_Overlay_dPhi_v4.1"); 	
			dPhi_plotter.SetComparisonCuts({"jet0_scores_depth_LLPanywhere >= 0", "jet0_scores_depth_LLPanywhere >= 0.2"});
		}
		dPhi_plotter.PlotMany2D("ratio");
		dPhi_plotter.ClearFileTrees();
		dPhi_plotter.SetCuts("jet0_DepthTagCand == 1 " + Cut_VR);
		if (v3) {
			dPhi_plotter.SetOutputFileTag("VRratio_Overlay_dPhi_v3.16"); 	
			dPhi_plotter.SetComparisonCuts({"jet0_scores_depth_anywhere >= 0", "jet0_scores_depth_anywhere >= 0.2"});
		}
		else {
			dPhi_plotter.SetOutputFileTag("VRratio_Overlay_dPhi_v4.1"); 	
			dPhi_plotter.SetComparisonCuts({"jet0_scores_depth_LLPanywhere >= 0", "jet0_scores_depth_LLPanywhere >= 0.2"});
		}
		dPhi_plotter.PlotMany2D("ratio");
	}

	// ----- Jet kinematics in control and signal regions -----//

	if (Skim_WJets) {
		cout<<endl;
		cout<<" ---------- CR / SR Study: All BDT input variables for LLP skim and W+jets (background) ---------- "<<endl;
		cout<<endl;

		class MiniTuplePlotter plotter_BDTvars( filetags_data2, path );
		plotter_BDTvars.SetPlots(analysisVars_perJet); 
		plotter_BDTvars.SetTreeNames( {"PerJet_WPlusJets", "PerJet_NoSel"} );	
		plotter_BDTvars.SetOutputFileTag("Overlay_BDTinput_perJet_v3.8");
		plotter_BDTvars.plot_log_ratio    = false; 
		plotter_BDTvars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_BDTvars.SetLegendNames({"LLP skim - W+jets selection", "LLP skim - all"});
		plotter_BDTvars.SetCuts("perJet_Pt >= 40 && abs(perJet_Eta) <= 1.26");
		plotter_BDTvars.colors = { kGray, kViolet+4 };
		plotter_BDTvars.SetOutputDirectory("Overlay_BDTinput_perJet");
		plotter_BDTvars.Plot();
	}

	if (LLP_WJets) {
		cout<<endl;
		cout<<" ---------- CR / SR Study 0: Jet, track kinematics for LLP MC and W+jets (background) ---------- "<<endl;
		cout<<endl;
		
		// vector<string> filetag_keys_to_loop = {"LLP125", "LLP250", "LLP350", "LLP125_mX15", "LLP350_mX80"};
		vector<string> filetag_keys_to_loop = {"LLP125_mX15"};

		for( auto key: filetag_keys_to_loop){

			// // HCAL depth variables with LLP regions and data overlayed
			// class MiniTuplePlotter plotter_BDTvars( filetags[key], path );
			// plotter_BDTvars.SetPlots(analysisVars); 
			// plotter_BDTvars.SetTreeNames( {"WPlusJets", "NoSel"} );	
			// plotter_BDTvars.SetOutputFileTag("Overlay_"+key+"_v3.8");
			// plotter_BDTvars.plot_log_ratio    = false; 
			// //plotter_BDTvars.plot_cdf 		 = true;
			// //plotter_BDTvars.plot_reverse_cdf  = true;
			// plotter_BDTvars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			// plotter_BDTvars.SetLegendNames({"LLP skim - W+jets selection", "Tracker: R <= 10cm", "Tracker: R > 10cm", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
			// plotter_BDTvars.colors = { kBlack, kGray, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed };
			// plotter_BDTvars.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26");
			// plotter_BDTvars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0}, "LLP_MC");
			// //plotter_BDTvars.SetSelectiveCuts("LLPskim", "jet0_NeutralHadEFrac < 0.6");  // BLINDED with track energy fraction (jet0_Track0Pt / jet0_Pt > 0.1) or neutral hadron fraction (jet0_NeutralHadEFrac < 0.6)
			// //plotter_BDTvars.SetSelectiveCuts("LLPskim", "jet1_Pt < 30"); // for stronger W+jets selection
			// plotter_BDTvars.SetOutputDirectory("Overlay_"+key);
			// plotter_BDTvars.Plot("ratio");

			bool simple = false;
			bool reduced = false;
			bool hcal_only = true;
			// HCAL depth variables with LLP regions and data overlayed
			class MiniTuplePlotter plotter_BDTvars_perJet( filetags[key], path );
			plotter_BDTvars_perJet.SetPlots(analysisVars_perJet); 
			plotter_BDTvars_perJet.SetTreeNames( {"PerJet_WPlusJets", "PerJet_LLPmatched"} );	
			plotter_BDTvars_perJet.SetOutputFileTag("Overlay_perJet_"+key+"_v3.8");
			if (simple) plotter_BDTvars_perJet.SetOutputFileTag("SimpleOverlay_perJet_"+key+"_v3.8");
			if (reduced) plotter_BDTvars_perJet.SetOutputFileTag("ReducedOverlay_perJet_"+key+"_v3.8");
			if (hcal_only) plotter_BDTvars_perJet.SetOutputFileTag("HcalOverlay_perJet_"+key+"_v3.8");
			plotter_BDTvars_perJet.plot_log_ratio    = false; 
			//plotter_BDTvars_perJet.plot_cdf 		 = true;
			//plotter_BDTvars_perJet.plot_reverse_cdf  = true;
			plotter_BDTvars_perJet.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			plotter_BDTvars_perJet.SetLegendNames({"LLP skim - W+jets selection", "LLP in tracker <= 10cm", "LLP in tracker > 10cm", "LLP in ECAL", "LLP in HCAL, D1", "LLP in HCAL, D2", "LLP in HCAL, D34"});
			plotter_BDTvars_perJet.SetLegendNames({"Prompt background", "LLP in tracker <= 10cm", "LLP in tracker > 10cm", "LLP in ECAL", "LLP in HCAL, D1", "LLP in HCAL, D2", "LLP in HCAL, D34"});
			plotter_BDTvars_perJet.colors = { kBlack, kGray, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed };
			plotter_BDTvars_perJet.SetComparisonCuts({Cut_matchedLLPinCR, Cut_matchedLLPinTrackerNP, Cut_matchedLLPinECAL, Cut_matchedLLPinHCAL1, Cut_matchedLLPinHCAL2, Cut_matchedLLPinHCAL34}, "LLP_MC");
			if (simple) {
				plotter_BDTvars_perJet.SetLegendNames({"Prompt background", "LLP in HCAL"});
				plotter_BDTvars_perJet.colors = { kBlack, kAzure-3 };
				plotter_BDTvars_perJet.SetComparisonCuts({Cut_matchedLLPinHCAL}, "LLP_MC");
			}
			if (reduced) {
				plotter_BDTvars_perJet.SetLegendNames({"Prompt background", "LLP in tracker <= 10cm", "LLP in tracker > 10cm", "LLP in ECAL", "LLP in HCAL"});
				plotter_BDTvars_perJet.colors = { kBlack, kGray, kOrange, kGreen+2, kAzure-3 };
				plotter_BDTvars_perJet.SetComparisonCuts({Cut_matchedLLPinCR, Cut_matchedLLPinTrackerNP, Cut_matchedLLPinECAL, Cut_matchedLLPinHCAL}, "LLP_MC");
			}
			if (hcal_only) {
				plotter_BDTvars_perJet.SetLegendNames({"Prompt background", "LLP in HCAL, D1", "LLP in HCAL, D2", "LLP in HCAL, D34"});
				// plotter_BDTvars_perJet.colors = { kBlack, kAzure+7, kBlue-4, kViolet+4 };
				plotter_BDTvars_perJet.colors = { kBlack, kAzure+7, kOrange-3, kRed-7 };
				plotter_BDTvars_perJet.SetComparisonCuts({Cut_matchedLLPinHCAL1, Cut_matchedLLPinHCAL2, Cut_matchedLLPinHCAL34}, "LLP_MC");
			}
			plotter_BDTvars_perJet.SetOutputDirectory("Overlay_perJet_"+key);
			plotter_BDTvars_perJet.Plot();
		}
	}

	if (track_dR_study) {
		cout<<endl;
		cout<<" ---------- Track and dR Study ---------- "<<endl;
		cout<<endl;
		
		vector<string> filetag_keys_to_loop = {"LLP125", "LLP250", "LLP350", "LLP125_mX15", "LLP350_mX80"};

		TCut Cut_Track_dR1 	= "perJet_Track0dR < 0.1";
		TCut Cut_Track_dR2  = "perJet_Track0dR >= 0.1 && perJet_Track0dR < 0.2";
		TCut Cut_Track_dR3  = "perJet_Track0dR >= 0.2 && perJet_Track0dR < 0.3";
		TCut Cut_Track_dR4  = "perJet_Track0dR >= 0.3";

		TCut Cut_Track_2 	= "perJet_Track0Pt < 2";
		TCut Cut_Track_4  	= "perJet_Track0Pt >= 2 && perJet_Track0Pt < 4";
		TCut Cut_Track_8  	= "perJet_Track0Pt >= 4 && perJet_Track0Pt < 8";
		TCut Cut_Track_high = "perJet_Track0Pt >= 8";

		for( auto key: filetag_keys_to_loop){

			class MiniTuplePlotter plotter_track_dR( filetags[key], path );
			plotter_track_dR.SetPlots({P_perJet_Track0Pt, P_perJet_Track0PtFrac}); 
			plotter_track_dR.SetTreeNames( {"PerJet_WPlusJets", "PerJet_LLPmatched"} );	
			plotter_track_dR.SetOutputFileTag("Overlay_perJet_"+key+"_v3.8");
			plotter_track_dR.plot_log_ratio    = false; 
			//plotter_track_dR.plot_cdf 		 = true;
			//plotter_track_dR.plot_reverse_cdf  = true;
			plotter_track_dR.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			plotter_track_dR.SetLegendNames({"LLP skim - dR < 0.1", "LLP skim - 0.1 < dR < 0.2", "LLP skim - 0.2 < dR < 0.3", "LLP skim - 0.3 < dR < 0.4", "LLP MC - dR < 0.1", "LLP MC - 0.1 < dR < 0.2", "LLP MC - 0.2 < dR < 0.3", "LLP MC - 0.3 < dR < 0.4", });
			plotter_track_dR.colors = { kBlack, kGray+3, kGray+1, kGray, kGreen+2, kBlue-4, kAzure+7, kOrange };
			plotter_track_dR.SetComparisonCuts({Cut_Track_dR1, Cut_Track_dR2, Cut_Track_dR3, Cut_Track_dR4});
			plotter_track_dR.SetSelectiveCuts("LLP_MC", Cut_matchedLLPinHCAL); 
			plotter_track_dR.SetOutputDirectory("Overlay_perJet_Track_dR_"+key);
			plotter_track_dR.Plot();

			class MiniTuplePlotter plotter_dR_track( filetags[key], path );
			plotter_dR_track.SetPlots({P_perJet_Track0dR}); 
			plotter_dR_track.SetTreeNames( {"PerJet_WPlusJets", "PerJet_LLPmatched"} );	
			plotter_dR_track.SetOutputFileTag("Overlay_perJet_"+key+"_v3.8");
			plotter_dR_track.plot_log_ratio    = false; 
			plotter_dR_track.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			plotter_dR_track.SetLegendNames({"LLP skim - track pT < 2", "LLP skim - 2 < track pT < 4", "LLP skim - 4 < track pT < 8", "LLP skim - 8 < track pT", "LLP MC - track pT < 2", "LLP MC - 2 < track pT < 4", "LLP MC - 4 < track pT < 8", "LLP MC - 8 < track pT", });
			plotter_dR_track.colors = { kBlack, kGray+3, kGray+1, kGray, kGreen+2, kBlue-4, kAzure+7, kOrange };
			plotter_dR_track.SetComparisonCuts({Cut_Track_2, Cut_Track_4, Cut_Track_8, Cut_Track_high});
			plotter_dR_track.SetSelectiveCuts("LLP_MC", Cut_matchedLLPinHCAL); 
			plotter_dR_track.SetOutputDirectory("Overlay_perJet_Track_dR_"+key);
			plotter_dR_track.Plot();

			// cdf / reverse cdf to determine what fraction of jets are not matched with tracks
			class MiniTuplePlotter plotter_dR_cdf( filetags[key], path );
			plotter_dR_cdf.SetPlots({P_perJet_Track0dEta, P_perJet_Track0dPhi, P_perJet_Track0dR}); 
			plotter_dR_cdf.SetTreeNames( {"PerJet_WPlusJets", "PerJet_LLPmatched"} );	
			plotter_dR_cdf.SetOutputFileTag("Overlay_perJet_"+key+"_v3.8");
			plotter_dR_cdf.plot_log_ratio    = true;
			plotter_dR_cdf.plot_cdf  = true;
			plotter_dR_cdf.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			plotter_dR_cdf.SetLegendNames({"LLP skim - W+jets selection", "Tracker: R <= 10cm", "Tracker: R > 10cm", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
			plotter_dR_cdf.colors = { kBlack, kGray, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed };
			plotter_dR_cdf.SetComparisonCuts({Cut_matchedLLPinCR, Cut_matchedLLPinTrackerNP, Cut_matchedLLPinECAL, Cut_matchedLLPinHCAL1, Cut_matchedLLPinHCAL2, Cut_matchedLLPinHCAL34}, "LLP_MC");
			plotter_dR_cdf.SetOutputDirectory("Overlay_perJet_Track_dR_"+key);
			plotter_dR_cdf.Plot("ratio");
		}
	}

	if (overlay_LLP) {
		cout<<endl;
		cout<<" ---------- Overlay analysis variables for each LLP mass point ---------- "<<endl;
		cout<<endl;

		// HCAL depth variables with LLP regions and data overlayed
		class MiniTuplePlotter plotter_BDTvars_perJet( filetags_all, path );
		plotter_BDTvars_perJet.SetPlots(analysisVars_perJet); 
		plotter_BDTvars_perJet.SetTreeName( "PerJet_LLPmatched" );	
		plotter_BDTvars_perJet.SetOutputFileTag("Overlay_perJet_allLLP_v3.8");
		plotter_BDTvars_perJet.plot_log_ratio    = false; 
		//plotter_BDTvars_perJet.plot_cdf 		 = true;
		//plotter_BDTvars_perJet.plot_reverse_cdf  = true;
		plotter_BDTvars_perJet.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_BDTvars_perJet.SetLegendNames({"m_h = 125, m_s = 15, ctau = 1m", "m_h = 350, m_s = 80, ctau = 0.5m", "m_h = 125, m_s = 50, ctau = 3m", "m_h = 250, m_s = 120, ctau = 10m", "m_h = 350, m_s = 160, ctau = 10m"});
		plotter_BDTvars_perJet.colors = { kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed };
		plotter_BDTvars_perJet.SetCuts(Cut_matchedLLPinHCAL);
		plotter_BDTvars_perJet.SetOutputDirectory("Overlay_perJet_allLLP");
		plotter_BDTvars_perJet.Plot();
	}

	if (Bkg_est) {
		cout<<endl;
		cout<<" ---------- Background Study 1: Tagging rates by jet eta ---------- "<<endl;
		cout<<endl;

		TCut Cut_BDTscore5 	= "bdtscore_LLP350_MS80_perJet > 0.5"; 
		TCut Cut_BDTscore9 	= "bdtscore_LLP350_MS80_perJet > 0.9"; 
		TCut Cut_BDTscore99 = "bdtscore_LLP350_MS80_perJet > 0.99"; 
		TCut Cut_BDTscore999= "bdtscore_LLP350_MS80_perJet > 0.999"; 

		class MiniTuplePlotter plotter_BkgEst( filetags_data, path );
		plotter_BkgEst.SetTreeName( "PerJet_WPlusJets" );	
		plotter_BkgEst.SetComparisonCuts({Cut_None, Cut_BDTscore5, Cut_BDTscore9, Cut_BDTscore99, Cut_BDTscore999});
		plotter_BkgEst.plot_log_ratio   = true; 
		plotter_BkgEst.plot_norm 		= false;
		plotter_BkgEst.SetLegendNames({"Jets from W+Jets events", "with BDT 350 (mS 80) score > 0.5", "with BDT 350 (mS 80) score > 0.9", "with BDT 350 (mS 80) score > 0.99", "with BDT 350 (mS 80) score > 0.999"});
		plotter_BkgEst.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_BkgEst.SetOutputDirectory("BackgroundEstimations");
		plotter_BkgEst.SetOutputFileTag("Jet40_CR_SR_v3.8_MC"); 	
		plotter_BkgEst.SetPlots({P_perJet_Pt, P_perJet_Phi, P_perJet_Eta});
		plotter_BkgEst.Plot("ratio");
		plotter_BkgEst.ClearFileTrees();
		plotter_BkgEst.SetOutputFileTag("Jet40_CR_SR_v3.8_MC_variableBins"); 
		plotter_BkgEst.SetPlots({P_perJet_Pt});
		plotter_BkgEst.SetVariableBins({0,40,60,80,120,160,300});
		plotter_BkgEst.Plot("ratio");
		plotter_BkgEst.ClearFileTrees();
		plotter_BkgEst.SetPlots({P_perJet_Phi});
		plotter_BkgEst.SetVariableBins({-3.5,0,3.5});
		plotter_BkgEst.Plot("ratio");
		plotter_BkgEst.ClearFileTrees();
		plotter_BkgEst.SetPlots({P_perJet_Eta});
		plotter_BkgEst.SetVariableBins({-1.5,-1.26,-1,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1,1.26,1.5});
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
		plotter_JetVars.SetOutputFileTag("Jet40_CR_SR_v3.8_MC"); 	
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
		plotter_TrackVars.SetOutputFileTag("Jet40_Track4_CR_SR_v3.8_MC"); 	
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
		plotter_JetVars2D_etaphi.SetOutputFileTag("Jet40_CR_SR_v3.8_MC"); 	
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
		plotter_Jet0KinematicVars.SetOutputFileTag("CR_SR_v3.8_MC"); 	
		plotter_Jet0KinematicVars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0});
		plotter_Jet0KinematicVars.plot_log_ratio    = true;
		plotter_Jet0KinematicVars.SetLegendNames({"Tracker: R < 10cm", "HCAL-D1234"});
		plotter_Jet0KinematicVars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_Jet0KinematicVars.Plot("ratio");	
		plotter_Jet0KinematicVars.Plot("efficiency");	
		
		class MiniTuplePlotter plotter_Jet1KinematicVars( filetags_LLP, path );
		plotter_Jet1KinematicVars.SetPlots(jetPlots1); 
		plotter_Jet1KinematicVars.SetTreeName( "NoSel" );	
		plotter_Jet1KinematicVars.SetOutputFileTag("CR_SR_v3.8_MC"); 	
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
		class MiniTuplePlotter plotter_RechitVars_NHBinsData( filetags_data, path );
		plotter_RechitVars_NHBinsData.SetPlots(allPlots);
		plotter_RechitVars_NHBinsData.SetTreeName( "NoSel" );	
		plotter_RechitVars_NHBinsData.SetOutputFileTag("NeutralHadBins_Jet40_v3.8"); 	
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
		plotter_RechitVars_NHBins.SetOutputFileTag("NeutralHadBins_Jet40_v3.8_MC"); 	
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
		plotter_allVars_JetBinsData.SetPlots(analysisVars); 
		plotter_allVars_JetBinsData.SetTreeName( "WPlusJets" );	
		plotter_allVars_JetBinsData.SetOutputFileTag("JetpTBins_Jet40_v3.8"); 	
		plotter_allVars_JetBinsData.SetComparisonCuts({Cut_JetPt40, Cut_JetPt80, Cut_JetPt120, Cut_JetPt160, Cut_JetPt200}); 
		plotter_allVars_JetBinsData.plot_log_ratio    = false; 
		plotter_allVars_JetBinsData.SetLegendNames({"Data: Jet p_{T} 40-80 GeV", "Data: Jet p_{T} 80-120 GeV", "Data: Jet p_{T} 120-160 GeV", "Data: Jet p_{T} 160-200 GeV", "Data: Jet p_{T} 200+ GeV"});
		plotter_allVars_JetBinsData.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_allVars_JetBinsData.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26");
		plotter_allVars_JetBinsData.SetSelectiveCuts("LLPskim", "jet0_NeutralHadEFrac < 0.6");  // BLINDED with track energy fraction (jet0_Track0Pt / jet0_Pt > 0.1) or neutral hadron fraction (jet0_NeutralHadEFrac < 0.6)
		plotter_allVars_JetBinsData.SetOutputDirectory("Data_JetPtBins");
		plotter_allVars_JetBinsData.Plot();

		class MiniTuplePlotter plotter_allVars_JetBins( filetags_LLP, path );
		plotter_allVars_JetBins.SetPlots(analysisVars); 
		plotter_allVars_JetBins.SetTreeName( "NoSel" );	
		plotter_allVars_JetBins.SetOutputFileTag("JetpTBins_Jet40_v3.8_MC"); 	
		plotter_allVars_JetBins.SetComparisonCuts({Cut_JetPt40, Cut_JetPt80, Cut_JetPt120, Cut_JetPt160, Cut_JetPt200}); 
		plotter_allVars_JetBins.plot_log_ratio    = false; 
		plotter_allVars_JetBins.SetLegendNames({"Jet p_{T} 40-80 GeV", "Jet p_{T} 80-120 GeV", "Jet p_{T} 120-160 GeV", "Jet p_{T} 160-200 GeV", "Jet p_{T} 200+ GeV"});
		plotter_allVars_JetBins.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_allVars_JetBins.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26 && jet0_Track0Pt > 4");
		plotter_allVars_JetBins.SetOutputDirectory("MC_JetPtBins");
		plotter_allVars_JetBins.Plot();
	}

	if (BDT_bins) {
		// ----- Bin in BDT score ----- //

		// for mH = 350 now, BDT score for 350, 80

		cout<<endl;
		cout<<" ---------- Study 5: Bin in BDT score (MC and data) ---------- "<<endl;
		cout<<endl;

		class MiniTuplePlotter plotter_run_BDTbinsData( filetags_data, path );
		plotter_run_BDTbinsData.SetTreeName( "PerJet_WPlusJets" );	
		plotter_run_BDTbinsData.SetOutputFileTag("BDTBins_Jet40_v3.8"); 	
		plotter_run_BDTbinsData.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );			
		plotter_run_BDTbinsData.SetCuts("perJet_Pt >= 40 && abs(perJet_Eta) <= 1.26 && bdtscore_LLP350_MS80_perJet >= 0.99");
		plotter_run_BDTbinsData.SetOutputDirectory("BDTBins");
		plotter_run_BDTbinsData.SetPlots2D({Hist1_Hist2(P_bdtscore_LLP350_MS80_perJet, P_run), Hist1_Hist2(P_bdtscore_LLP350_MS80_perJet, P_eventHT)});
		plotter_run_BDTbinsData.PlotMany2D();

		class MiniTuplePlotter plotter_allVars_BDTbinsData( BDT_files, path ); 
		plotter_allVars_BDTbinsData.SetPlots(analysisVars_perJet); 
		plotter_allVars_BDTbinsData.AddPlot({P_perJet_Pt});
		plotter_allVars_BDTbinsData.AddPlot({P_run});
		plotter_allVars_BDTbinsData.SetTreeNames( {"PerJet_WPlusJets", "PerJet_LLPmatched", "PerJet_LLPmatched"} );	
		plotter_allVars_BDTbinsData.SetOutputFileTag("BDTBins_Jet40_v3.8"); 	
		plotter_allVars_BDTbinsData.SetComparisonCuts({"", Cut_BDTscorePt95, Cut_BDTscorePt99, Cut_BDTscore1}, "LLPskim"); 
		plotter_allVars_BDTbinsData.plot_log_ratio    = false; 
		plotter_allVars_BDTbinsData.SetLegendNames({"W+jets: all", "W+jets: BDT score 0.95 - 0.99", "W+jets: BDT score 0.99 to 0.999", "W+jets: BDT score 0.999 to 1", "mH=125, mS=15 (LLP in HCAL, BDT>0.99)", "mH=350, mS=80 (LLP in HCAL, BDT>0.99)"});
		plotter_allVars_BDTbinsData.colors = { kBlack,  kAzure+7, kGreen+2, kOrange, kMagenta-7, kRed  };
		plotter_allVars_BDTbinsData.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_allVars_BDTbinsData.SetCuts("perJet_Pt >= 40 && abs(perJet_Eta) <= 1.26");
		plotter_allVars_BDTbinsData.SetSelectiveCuts("LLP_MC", Cut_matchedLLPinHCAL);
		plotter_allVars_BDTbinsData.SetSelectiveCuts("LLP_MC", "bdtscore_LLP350_MS80_perJet >= 0.99"); // only look at high scoring BDT jets
		plotter_allVars_BDTbinsData.SetOutputDirectory("BDTBins");
		plotter_allVars_BDTbinsData.Plot();
	}

	if (Overlay_all) {
		// ----- Overlay for MC signal, MC QCD, data ----- //

		cout<<endl;
		cout<<" ---------- Study 6: Overlay all minituples (QCD, LLP, skim)! ---------- "<<endl;
		cout<<endl;

		class MiniTuplePlotter plotter_overlay( filetags_all_v3pt0, path_v3pt0 );
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
		class MiniTuplePlotter plotter_overlay2D( filetags_all_v3pt0, path_v3pt0 );
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

		if (perJet) {
			class MiniTuplePlotter plotter_LLP_Jet_Match( filetags_LLP, path );
			plotter_LLP_Jet_Match.SetPlots({P_perJet_MatchedLLP_DecayR, P_perJet_MatchedLLP_Eta});
			plotter_LLP_Jet_Match.SetTreeName( "PerJet_LLPmatched" );	
			plotter_LLP_Jet_Match.SetOutputFileTag("Jet40_v3.8_MC"); 	
			plotter_LLP_Jet_Match.SetComparisonCuts({"abs(perJet_Eta) <= 1.26", "abs(perJet_Eta) <= 1.26 && perJet_MatchedLLP_DecayR > 0", "abs(perJet_Eta) <= 1.26 && perJet_Pt >= 40", "abs(perJet_Eta) <= 1.26 && perJet_Pt >= 60", "abs(perJet_Eta) <= 1.26 && perJet_Pt >= 80", "abs(perJet_Eta) <= 1.26 && perJet_Pt >= 100"}); 
			plotter_LLP_Jet_Match.plot_log_ratio    = false; 
			plotter_LLP_Jet_Match.plot_norm 		= false;
			plotter_LLP_Jet_Match.SetLegendNames({"LLP0 Decay Position", "LLP0 Matched, Jet p_{T} > 0", "LLP0 Matched, Jet p_{T} > 40", "LLP0 Matched, Jet p_{T} > 60", "LLP0 Matched, Jet p_{T} > 80", "LLP0 Matched, Jet p_{T} > 100"});
			plotter_LLP_Jet_Match.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			plotter_LLP_Jet_Match.SetCuts("abs(perJet_MatchedLLP_Eta) <= 1.5"); 
			plotter_LLP_Jet_Match.SetOutputDirectory("MC_LLP_Jet_Match");
			plotter_LLP_Jet_Match.Plot("ratio");
		}

		else {
			class MiniTuplePlotter plotter_LLP_Jet_PtDiff( filetags_LLP, path );
			plotter_LLP_Jet_PtDiff.SetPlots({P_jet0_llp0_ptDiff, P_jet0_llp1_ptDiff});
			plotter_LLP_Jet_PtDiff.SetTreeName( "NoSel" );	
			plotter_LLP_Jet_PtDiff.SetOutputFileTag("Jet40_v3.8_MC"); 	
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
			plotter_LLP_Jet_Match.SetOutputFileTag("Jet40_v3.8_MC"); 	
			plotter_LLP_Jet_Match.SetComparisonCuts({"abs(jet0_Eta) <= 1.26 || abs(jet1_Eta) <= 1.26 || abs(jet2_Eta) <= 1.26 || abs(jet3_Eta) <= 1.26 || abs(jet4_Eta) <= 1.26 || abs(jet5_Eta) <= 1.26", "LLP0_isTruthMatched == 1", "LLP0_isTruthMatched_Jet40 == 1", "LLP0_isTruthMatched_Jet60 == 1", "LLP0_isTruthMatched_Jet80 == 1", "LLP0_isTruthMatched_Jet100 == 1"}); 
			plotter_LLP_Jet_Match.plot_log_ratio    = false; 
			plotter_LLP_Jet_Match.plot_norm 		= false;
			plotter_LLP_Jet_Match.SetLegendNames({"LLP0 Decay Position", "LLP0 Matched, Jet p_{T} > 0", "LLP0 Matched, Jet p_{T} > 40", "LLP0 Matched, Jet p_{T} > 60", "LLP0 Matched, Jet p_{T} > 80", "LLP0 Matched, Jet p_{T} > 100"});
			plotter_LLP_Jet_Match.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			// plotter_LLP_Jet_Match.SetCuts("LLP0_Pt > 40 && abs(LLP0_Eta) <= 1.26 && (abs(jet0_Eta) <= 1.26 || abs(jet1_Eta) <= 1.26 || abs(jet2_Eta) <= 1.26 || abs(jet3_Eta) <= 1.26 || abs(jet4_Eta) <= 1.26 || abs(jet5_Eta) <= 1.26)"); 
			plotter_LLP_Jet_Match.SetCuts("LLP0_Pt > 40 && abs(LLP0_Eta) <= 1.26"); 
			plotter_LLP_Jet_Match.SetOutputDirectory("MC_LLP_Jet_Match");
			plotter_LLP_Jet_Match.Plot("ratio");

			class MiniTuplePlotter plotter_LLP_Jet_Match_tight( filetags_LLP, path );
			plotter_LLP_Jet_Match_tight.SetPlots({P_LLP0_DecayR});
			plotter_LLP_Jet_Match_tight.SetTreeName( "NoSel" );	
			plotter_LLP_Jet_Match_tight.SetOutputFileTag("Jet40_v3.8_MC"); 	
			plotter_LLP_Jet_Match_tight.SetComparisonCuts({"abs(jet0_Eta) <= 1.26 || abs(jet1_Eta) <= 1.26 || abs(jet2_Eta) <= 1.26", "LLP0_isTruthMatched == 1 && abs(LLP0_isTruthMatched_Eta) <= 1.26", "LLP0_isTruthMatched_Jet40 == 1 && abs(LLP0_isTruthMatched_Jet40Eta) <= 1.26", "LLP0_isTruthMatched_Jet60 == 1 && abs(LLP0_isTruthMatched_Jet60Eta) <= 1.26", "LLP0_isTruthMatched_Jet80 == 1 && abs(LLP0_isTruthMatched_Jet80Eta) <= 1.26", "LLP0_isTruthMatched_Jet100 == 1 && abs(LLP0_isTruthMatched_Jet100Eta) <= 1.26"}); 
			plotter_LLP_Jet_Match_tight.plot_log_ratio    = false; 
			plotter_LLP_Jet_Match_tight.plot_norm 		= false;
			plotter_LLP_Jet_Match_tight.SetLegendNames({"LLP0 Decay Position", "LLP0 Matched, Jet p_{T} > 0", "LLP0 Matched, Jet p_{T} > 40", "LLP0 Matched, Jet p_{T} > 60", "LLP0 Matched, Jet p_{T} > 80", "LLP0 Matched, Jet p_{T} > 100"});
			plotter_LLP_Jet_Match_tight.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			plotter_LLP_Jet_Match_tight.SetCuts("LLP0_Pt > 40 && abs(LLP0_Eta) <= 1.26"); 
			plotter_LLP_Jet_Match_tight.SetOutputDirectory("MC_LLP_Jet_Match_Tight");
			plotter_LLP_Jet_Match_tight.Plot("ratio");
		}
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
