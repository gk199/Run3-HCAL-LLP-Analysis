#include "../MiniTuplePlotter.h"
#include "TString.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_CR_SR(){


	// List where minituples are stored
	string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.3/minituple_";
	vector<string> filetags_both 	= { "v1.3_LLPskim_500k_2023_09_28", "v1.3_MCsignal_500k_2023_09_29" };
	vector<string> filetags_data 	= { "v1.3_LLPskim_500k_2023_09_28" };
	vector<string> filetags_MC 		= { "v1.3_MCsignal_500k_2023_09_29" };

	vector<PlotParams> jetPlots0	= {P_jet0_E, P_jet0_Pt}; // dont apply a 40 GeV jet cut on these
	vector<PlotParams> jetPlots1	= {P_jet1_E, P_jet1_Pt}; // dont apply a 40 GeV jet cut on these
	vector<PlotParams> allPlots 	= {P_jet0_E, P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_RechitN, P_jet0_Mass, P_jet0_JetArea,							// jet kinematics
										P_jet0_EtaSpread, P_jet0_PhiSpread, P_jet0_EtaSpread_energy, P_jet0_PhiSpread_energy, 								// eta - phi spread
										P_jet0_EtaPhiQuadSpread, P_jet0_EtaPhiQuadSpread_energy,
										P_jet0_Track0Pt, P_jet0_Track0PtFrac, P_jet0_Track0dR, 					 											// track vars
										P_jet0_Track0dEta, P_jet0_Track0dPhi, P_jet0_Tracks_dR, 
										P_jet0_MatchedLLP_DecayR, P_jet0_MatchedLLP_Eta,
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
	vector<PlotParams> trackVars	= {	P_jet0_Track0Pt, P_jet0_Track0PtFrac, P_jet0_Track0dR, 					 											// track vars
										P_jet0_Track0dEta, P_jet0_Track0dPhi, P_jet0_Tracks_dR}; 
//										P_jet0_Track0dzToPV, P_jet0_Track0dxyToBS, P_jet0_Track0dzOverErr, P_jet0_Track0dxyOverErr };
	vector<PlotParams> rechitVars 	= {P_jet0_LeadingRechitE, P_jet0_SubLeadingRechitE, P_jet0_SSubLeadingRechitE, P_jet0_AllRechitE, P_jet0_3RechitFracE,	// HB rechits
										P_jet0_LeadingRechitD, P_jet0_SubLeadingRechitD, P_jet0_SSubLeadingRechitD, 
										P_jet0_LeadingRechitEFrac, P_jet0_LeadingRechitEFracJet, P_jet0_LeadingRechitJetEDiff, 
										P_jet0_LeadingSubRechitEQuad, P_jet0_EtaPhiQuadSpread, P_jet0_EtaPhiQuadSpread_energy,
										P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4 };			// depth ratios
	
	#include "../RegionCuts.h"

	// ----- Jet kinematics in control and signal regions -----//

	cout<<endl;
	cout<<" ---------- CR / SR Study 1: Jet, track kinematics ---------- "<<endl;
	cout<<endl;

	// leading jet, all jet variables
	class MiniTuplePlotter plotter_JetVars( filetags_MC, path );
	plotter_JetVars.SetPlots(allPlots);
	plotter_JetVars.SetTreeName( "NoSel" );	
	plotter_JetVars.SetOutputFileTag("Jet40_CR_SR_v1.3_MC"); 	
	plotter_JetVars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0}); // , Cut_LLPinHCAL4_Jet0});
	plotter_JetVars.plot_log_ratio    = false; 
	plotter_JetVars.SetLegendNames({"Tracker: R < 10cm", "Tracker: R > 10cm", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
	plotter_JetVars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	//plotter_JetVars.plot_cdf = true;
	//plotter_JetVars.plot_reverse_cdf = true;
	plotter_JetVars.SetCuts("jet0_Pt >= 40");  // 40 GeV jet cut on all plots! 
	plotter_JetVars.SetOutputDirectory("MC_CR_SR");
	plotter_JetVars.Plot("ratio");

	// track variables, with leading track < 4 Gev (signal region)
	class MiniTuplePlotter plotter_TrackVars( filetags_MC, path );
	plotter_TrackVars.SetPlots(trackVars); 
	plotter_TrackVars.SetTreeName( "NoSel" );	
	plotter_TrackVars.SetOutputFileTag("Jet40_Track4_CR_SR_v1.3_MC"); 	
	plotter_TrackVars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0}); // , Cut_LLPinHCAL4_Jet0});
	plotter_TrackVars.plot_log_ratio    = false; 
	plotter_TrackVars.SetLegendNames({"Tracker: R < 10cm", "Tracker: R > 10cm", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
	plotter_TrackVars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_TrackVars.SetCuts("jet0_Pt >= 40 && jet0_Track0Pt < 4");
	plotter_TrackVars.SetOutputDirectory("MC_CR_SR_TrackStudy");
	plotter_TrackVars.Plot("ratio");

	// leading jet leading rechit energies, with eta cut on jet
	class MiniTuplePlotter plotter_RechitVars( filetags_MC, path );
	plotter_RechitVars.SetPlots(rechitVars);
	plotter_RechitVars.SetTreeName( "NoSel" );	
	plotter_RechitVars.SetOutputFileTag("Jet40_JetHBtight_CR_SR_v1.3_MC"); 	
	plotter_RechitVars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0}); // , Cut_LLPinHCAL4_Jet0});
	plotter_RechitVars.plot_log_ratio    = false; 
	plotter_RechitVars.SetLegendNames({"Tracker: R < 10cm", "Tracker: R > 10cm", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
	plotter_RechitVars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_RechitVars.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) < 1.25");  // 40 GeV jet cut on all plots! 
	plotter_RechitVars.SetOutputDirectory("MC_CR_SR_LeadingRechitStudy");
	plotter_RechitVars.Plot("ratio");

	// 2D spread of eta - phi, for leading and subleading jet
	class MiniTuplePlotter plotter_JetVars2D_etaphi( filetags_MC, path );
	plotter_JetVars2D_etaphi.SetTreeName( "NoSel" );	
	plotter_JetVars2D_etaphi.SetOutputFileTag("Jet40_CR_SR_v1.3_MC"); 	
	plotter_JetVars2D_etaphi.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0});
	plotter_JetVars2D_etaphi.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );			
	plotter_JetVars2D_etaphi.SetCuts("jet0_Pt >= 40");
	plotter_JetVars2D_etaphi.SetOutputDirectory("2D");
    plotter_JetVars2D_etaphi.SetPlots2D({Hist1_Hist2(P_jet0_EtaSpread, P_jet0_PhiSpread), Hist1_Hist2(P_jet0_EtaSpread_energy, P_jet0_PhiSpread_energy), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_SubLeadingRechitE) });
	plotter_JetVars2D_etaphi.PlotMany2D();
	plotter_JetVars2D_etaphi.SetComparisonCuts({Cut_LLPinCR_Jet1, Cut_LLPinHCAL_Jet1});
	plotter_JetVars2D_etaphi.SetCuts("jet1_Pt > 40");
    plotter_JetVars2D_etaphi.SetPlots2D({Hist1_Hist2(P_jet1_EtaSpread, P_jet1_PhiSpread), Hist1_Hist2(P_jet1_EtaSpread_energy, P_jet1_PhiSpread_energy) });
	plotter_JetVars2D_etaphi.ClearFileTrees();
	plotter_JetVars2D_etaphi.PlotMany2D();

	// ----- Cut based selections -- cut on neutral hadron fraction -----//
	
	cout<<endl;
	cout<<" ---------- CR / SR Study 2: Cut based selections. Cut on neutral hadron energy fraction ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_CutBased( filetags_MC, path );
	plotter_CutBased.SetPlots(allPlots);
	plotter_CutBased.SetTreeName( "NoSel" );	
	plotter_CutBased.SetOutputFileTag("NeutralHadpt6_Jet40_CR_SR_v1.3_MC"); 	
	plotter_CutBased.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinTrackerNP_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL34_Jet0}); // , Cut_LLPinHCAL4_Jet0});
	plotter_CutBased.plot_log_ratio    = false; 
	plotter_CutBased.SetLegendNames({"Tracker: R < 10cm", "Tracker: R > 10cm", "ECAL", "HCAL-D1", "HCAL-D2", "HCAL-D34"});
	plotter_CutBased.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_CutBased.SetCuts("jet0_Pt >= 40 && jet0_NeutralHadEFrac > 0.6"); 
	plotter_CutBased.SetOutputDirectory("MC_CR_SR_NeutralHadCut");
	plotter_CutBased.Plot("ratio"); 

	// ----- Full jet kinematics (simple, no cuts) -----//

	cout<<endl;
	cout<<" ---------- CR / SR Study 3: Simple distributions of jet kinematics without a pT cut ---------- "<<endl;
	cout<<endl;

	// leading and sub-leading jet, energy and pT distributions with no pt cuts to see full distribution
	class MiniTuplePlotter plotter_Jet0KinematicVars( filetags_MC, path );
	plotter_Jet0KinematicVars.SetPlots(jetPlots0); 
	plotter_Jet0KinematicVars.SetTreeName( "NoSel" );	
	plotter_Jet0KinematicVars.SetOutputFileTag("CR_SR_v1.3_MC"); 	
	plotter_Jet0KinematicVars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0});
	plotter_Jet0KinematicVars.plot_log_ratio    = true;
	plotter_Jet0KinematicVars.SetLegendNames({"Tracker: R < 10cm", "HCAL-D1234"});
	plotter_Jet0KinematicVars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_Jet0KinematicVars.Plot("ratio");	
	
	class MiniTuplePlotter plotter_Jet1KinematicVars( filetags_MC, path );
	plotter_Jet1KinematicVars.SetPlots(jetPlots1); 
	plotter_Jet1KinematicVars.SetTreeName( "NoSel" );	
	plotter_Jet1KinematicVars.SetOutputFileTag("CR_SR_v1.3_MC"); 	
	plotter_Jet1KinematicVars.SetComparisonCuts({Cut_LLPinCR_Jet1, Cut_LLPinHCAL_Jet1});
	plotter_Jet1KinematicVars.plot_log_ratio    = true;
	plotter_Jet1KinematicVars.SetLegendNames({"Tracker: R < 10cm", "HCAL-D1234"});
	plotter_Jet1KinematicVars.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_Jet1KinematicVars.Plot("ratio");

	// ----- Bin in neutral hadron fraction or HB rechit quantity -----//

	cout<<endl;
	cout<<" ---------- Data Study 4: Bin in neutral hadron fraction (data) ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_RechitVars_NHBins( filetags_data, path );
	plotter_RechitVars_NHBins.SetPlots(allPlots); //rechitVars);
	plotter_RechitVars_NHBins.SetTreeName( "NoSel" );	
	plotter_RechitVars_NHBins.SetOutputFileTag("NeutralHadBins_Jet40_v1.3_DataBLINDED"); 	
	plotter_RechitVars_NHBins.SetComparisonCuts({Cut_NHadpt1, Cut_NHadpt2, Cut_NHadpt3, Cut_NHadpt4, Cut_NHadpt6}); 
	plotter_RechitVars_NHBins.plot_log_ratio    = false; 
	plotter_RechitVars_NHBins.SetLegendNames({"Data - Neutral Hadron 0-0.1", "Data - Neutral Hadron 0.1-0.2", "Data - Neutral Hadron 0.2-0.3", "Data - Neutral Hadron 0.3-0.4", "Data - Neutral Hadron 0.4-0.6"});
	plotter_RechitVars_NHBins.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_RechitVars_NHBins.SetCuts("jet0_Pt >= 40 && jet0_NeutralHadEFrac < 0.6");  // 40 GeV jet cut on all plots!  BLINDED with track energy fraction (jet0_Track0Pt / jet0_Pt > 0.1) or neutral hadron fraction (jet0_NeutralHadEFrac < 0.6)
	plotter_RechitVars_NHBins.SetOutputDirectory("Data_NeutralHadBins");
	plotter_RechitVars_NHBins.Plot("ratio");

	// for MC and data, bin in jet pT

	cout<<endl;
	cout<<" ---------- Data Study 5: Bin in jet pT (MC, and data) ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_allVars_JetBinsData( filetags_data, path );
	plotter_allVars_JetBinsData.SetPlots(allPlots);
	plotter_allVars_JetBinsData.SetTreeName( "NoSel" );	
	plotter_allVars_JetBinsData.SetOutputFileTag("JetpTBins_Jet40_v1.3_DataBLINDED"); 	
	plotter_allVars_JetBinsData.SetComparisonCuts({Cut_JetPt40, Cut_JetPt80, Cut_JetPt120, Cut_JetPt160, Cut_JetPt200}); 
	plotter_allVars_JetBinsData.plot_log_ratio    = false; 
	plotter_allVars_JetBinsData.SetLegendNames({"Data - Jet p_{T} 40-80 GeV", "Data - Jet p_{T} 80-120 GeV", "Data - Jet p_{T} 120-160 GeV", "Data - Jet p_{T} 160-200 GeV", "Data - Jet p_{T} 200+ GeV"});
	plotter_allVars_JetBinsData.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_allVars_JetBinsData.SetCuts("jet0_Pt >= 40 && jet0_NeutralHadEFrac < 0.6");  // 40 GeV jet cut on all plots!  BLINDED with track energy fraction (jet0_Track0Pt / jet0_Pt > 0.1) or neutral hadron fraction (jet0_NeutralHadEFrac < 0.6)
	plotter_allVars_JetBinsData.SetOutputDirectory("Data_JetPtBins");
	plotter_allVars_JetBinsData.Plot("ratio");

	class MiniTuplePlotter plotter_allVars_JetBins( filetags_MC, path );
	plotter_allVars_JetBins.SetPlots(allPlots);
	plotter_allVars_JetBins.SetTreeName( "NoSel" );	
	plotter_allVars_JetBins.SetOutputFileTag("JetpTBins_Jet40_v1.3_MC"); 	
	plotter_allVars_JetBins.SetComparisonCuts({Cut_JetPt40, Cut_JetPt80, Cut_JetPt120, Cut_JetPt160, Cut_JetPt200}); 
	plotter_allVars_JetBins.plot_log_ratio    = false; 
	plotter_allVars_JetBins.SetLegendNames({"Jet p_{T} 40-80 GeV", "Jet p_{T} 80-120 GeV", "Jet p_{T} 120-160 GeV", "Jet p_{T} 160-200 GeV", "Jet p_{T} 200+ GeV"});
	plotter_allVars_JetBins.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_allVars_JetBins.SetCuts("jet0_Pt >= 40");  // 40 GeV jet cut on all plots!
	plotter_allVars_JetBins.SetOutputDirectory("MC_JetPtBins");
	plotter_allVars_JetBins.Plot("ratio");

	// ----- Study low energy leading rechits in signal region -----//
	
	cout<<endl;
	cout<<" ---------- MC Study 6: Study low energy leading rechits in signal region ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_LRechit_Bins( filetags_MC, path );
	plotter_LRechit_Bins.SetPlots(allPlots);
	plotter_LRechit_Bins.SetTreeName( "NoSel" );	
	plotter_LRechit_Bins.SetOutputFileTag("LRechit8_Jet40_v1.3_MC"); 	
	plotter_LRechit_Bins.SetComparisonCuts({"jet0_LeadingRechitE < 8", Cut_LLPinHCAL34_Jet0 + "jet0_LeadingRechitE < 8", "jet0_LeadingRechitE >= 8"}); 
	plotter_LRechit_Bins.plot_log_ratio    = false; 
	plotter_LRechit_Bins.SetLegendNames({"Leading Rechit E < 8 GeV", "Leading Rechit E < 8 GeV and LLP in HCALd34", "Leading Rechit E >= 8 GeV"});
	plotter_LRechit_Bins.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_LRechit_Bins.SetCuts("jet0_Pt >= 40" + Cut_LLPinHCAL_Jet0);  // 40 GeV jet cut on all plots!
	plotter_LRechit_Bins.SetOutputDirectory("MC_LeadingRechitStudy8GeV");
	plotter_LRechit_Bins.Plot();

	class MiniTuplePlotter plotter_LRechit_Bins2D( filetags_MC, path );
	//plotter_LRechit_Bins2D.SetPlots(allPlots);
	plotter_LRechit_Bins2D.SetTreeName( "NoSel" );	
	plotter_LRechit_Bins2D.SetOutputFileTag("LRechit8_Jet40_v1.3_MC"); 	
	plotter_LRechit_Bins2D.SetComparisonCuts({"jet0_LeadingRechitE < 8", Cut_LLPinHCAL34_Jet0 + "jet0_LeadingRechitE < 8", "jet0_LeadingRechitE >= 8"}); 
	plotter_LRechit_Bins2D.SetLegendNames({"Leading Rechit E < 8 GeV", "LLP in HCALd34 and Leading Rechit E < 8 GeV ", "Leading Rechit E >= 8 GeV"});
	plotter_LRechit_Bins2D.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_LRechit_Bins2D.SetCuts("jet0_Pt >= 40" + Cut_LLPinHCAL_Jet0);  // 40 GeV jet cut on all plots!
	plotter_LRechit_Bins2D.SetOutputDirectory("MC_LeadingRechitStudy8GeV");
	plotter_LRechit_Bins2D.Plot2D(P_jet0_MatchedLLP_Eta, P_jet0_MatchedLLP_DecayR);

	cout<<endl;
	cout<<" ---------- MC Study 7: LLP - jet match (efficiency plots) ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_LLP_Jet_PtDiff( filetags_MC, path );
	plotter_LLP_Jet_PtDiff.SetPlots({P_jet0_llp0_ptDiff});
	plotter_LLP_Jet_PtDiff.SetTreeName( "NoSel" );	
	plotter_LLP_Jet_PtDiff.SetOutputFileTag("Jet40_v1.3_MC"); 	
	plotter_LLP_Jet_PtDiff.SetComparisonCuts({Cut_LLP0inCR, Cut_LLP0inTrackerNP, Cut_LLP0inECAL, Cut_LLPinHCAL_d1, Cut_LLP0inHCAL_d2, Cut_LLP0inHCAL_d34}); 
	plotter_LLP_Jet_PtDiff.plot_log_ratio    = false; 
	plotter_LLP_Jet_PtDiff.SetLegendNames({"LLP0: Tracker: R < 10cm", "LLP0: Tracker: R > 10cm", "LLP0: ECAL", "LLP0: HCAL-D1", "LLP0: HCAL-D2", "LLP0: HCAL-D34"});
	plotter_LLP_Jet_PtDiff.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_LLP_Jet_PtDiff.SetCuts("jet0_Pt >= 40 && jet0_isMatchedTo == 0");  
	plotter_LLP_Jet_PtDiff.SetOutputDirectory("MC_LLP_Jet_Match");
	plotter_LLP_Jet_PtDiff.Plot();	

	class MiniTuplePlotter plotter_LLP_Jet_Match( filetags_MC, path );
	plotter_LLP_Jet_Match.SetPlots({P_LLP0_DecayR});
	plotter_LLP_Jet_Match.SetTreeName( "NoSel" );	
	plotter_LLP_Jet_Match.SetOutputFileTag("Jet40_v1.3_MC"); 	
	plotter_LLP_Jet_Match.SetComparisonCuts({"", "LLP0_isTruthMatched == 1", "LLP0_isTruthMatched_Jet40 == 1", "LLP0_isTruthMatched_Jet60 == 1", "LLP0_isTruthMatched_Jet80 == 1", "LLP0_isTruthMatched_Jet100 == 1"}); 
	plotter_LLP_Jet_Match.plot_log_ratio    = false; 
	plotter_LLP_Jet_Match.plot_norm 		= false;
	plotter_LLP_Jet_Match.SetLegendNames({"LLP0 Decay Position", "LLP0 Matched, Jet p_{T} > 0", "LLP0 Matched, Jet p_{T} > 40", "LLP0 Matched, Jet p_{T} > 60", "LLP0 Matched, Jet p_{T} > 80", "LLP0 Matched, Jet p_{T} > 100"});
	plotter_LLP_Jet_Match.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_LLP_Jet_Match.SetCuts("LLP0_Pt > 40 && abs(LLP0_Eta) < 1.4"); 
	plotter_LLP_Jet_Match.SetOutputDirectory("MC_LLP_Jet_Match");
	plotter_LLP_Jet_Match.Plot("ratio");

	class MiniTuplePlotter plotter_LLP_Jet_Match_tight( filetags_MC, path );
	plotter_LLP_Jet_Match_tight.SetPlots({P_LLP0_DecayR});
	plotter_LLP_Jet_Match_tight.SetTreeName( "NoSel" );	
	plotter_LLP_Jet_Match_tight.SetOutputFileTag("Jet40_v1.3_MC"); 	
	plotter_LLP_Jet_Match_tight.SetComparisonCuts({"", "LLP0_isTruthMatched == 1", "LLP0_isTruthMatched_Jet40 == 1", "LLP0_isTruthMatched_Jet60 == 1", "LLP0_isTruthMatched_Jet80 == 1", "LLP0_isTruthMatched_Jet100 == 1"}); 
	plotter_LLP_Jet_Match_tight.plot_log_ratio    = false; 
	plotter_LLP_Jet_Match_tight.plot_norm 		= false;
	plotter_LLP_Jet_Match_tight.SetLegendNames({"LLP0 Decay Position", "LLP0 Matched, Jet p_{T} > 0", "LLP0 Matched, Jet p_{T} > 40", "LLP0 Matched, Jet p_{T} > 60", "LLP0 Matched, Jet p_{T} > 80", "LLP0 Matched, Jet p_{T} > 100"});
	plotter_LLP_Jet_Match_tight.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_LLP_Jet_Match_tight.SetCuts("LLP0_Pt > 40 && abs(LLP0_Eta) < 0.8"); 
	plotter_LLP_Jet_Match_tight.SetOutputDirectory("MC_LLP_Jet_Match_Tight");
	plotter_LLP_Jet_Match_tight.Plot("ratio");

}
