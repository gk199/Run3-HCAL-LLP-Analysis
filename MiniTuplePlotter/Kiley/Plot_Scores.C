#include "MiniTuplePlotter.h"

// -------------------------------------------------------------------------------------------------
void Plot_Scores(){

	string path = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.1/";
	path = "../Files/MiniTuples/v5.3/minituple_";

	vector<string> filetags_example1;

	//for( auto era: vector<string>{"2022Dv1", "2022Ev1", "2022Fv1", "2022Gv1", "2023Cv1", "2023Cv2", "2023Cv3", "2023Cv4", "2023Dv1", "2023Dv2"} ){
	
	filetags_example1 = { "data_2022Ev1_scores", "data_2023Dv2_scores", "HToSSTo4B_125_50_CTau3000_scores" }; //, "test_10k" };
	filetags_example1 = { "data_2023Dv2_scores", "HToSSTo4B_125_50_CTau3000_scores" }; //, "test_10k" };
	filetags_example1 = { "data_2022Ev1_scores", "HToSSTo4B_125_50_CTau3000_scores" }; //, "test_10k" };
	filetags_example1 = { "HToSSTo4B_125_50_CTau3000_scores", "data_2022Ev1_scores", "data_2023Dv2_scores" }; //, "test_10k" };
	filetags_example1 = { "HToSSTo4B_125_50_CTau3000_scores" }; //, "test_10k" };
	filetags_example1 = { "data_2023Dv2_scores", "HToSSTo4B_125_50_CTau3000_scores" }; //, "test_10k" };
	//filetags_example1 = { "HToSSTo4B_125_50_CTau3000_scores", "RAW_HToSSTo4B_MH125_MS50_CTau3000_2023postBPix" }; //, "test_10k" };


	TCut Cut_PassHLT = "jet0_L1trig_Matched == 1";// && Pass_HLTDisplacedJet == 1";
	TCut Cut_FailHLT = "jet0_L1trig_Matched == 0"; // && Pass_HLTDisplacedJet == 0";

	TCut Cut_Pass_PreSel = "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1"; // && jet0_DepthTagCand == 1 && jet1_InclTagCand == 1";

	vector<TCut> Cuts_jet0_DeepCSV_prob_b_binned = {

		"jet0_DeepCSV_prob_b > 0",
		"jet0_DeepCSV_prob_b > 0.1",
		"jet0_DeepCSV_prob_b > 0.2435",
		"jet0_DeepCSV_prob_b > 0.3",
		"jet0_DeepCSV_prob_b > 0.45",
		"jet0_DeepCSV_prob_b > 0.8",
	};

	vector<TCut> Cuts_jet1_DeepCSV_prob_b_binned = {

		"jet1_DeepCSV_prob_b > 0",
		"jet1_DeepCSV_prob_b > 0.1",
		"jet1_DeepCSV_prob_b > 0.2435",
		"jet1_DeepCSV_prob_b > 0.3",
		"jet1_DeepCSV_prob_b > 0.45",
		"jet1_DeepCSV_prob_b > 0.8",
	};

	// {P_jet0_scores_depth_LLPanywhere, P_jet1_scores_depth_LLPanywhere, P_jet0_scores_inc_train80, P_jet1_scores_inc_train80}

	// General //

//"(var1*(var1>0) + var2*(var2>0) + var3*(var3>0)) / "
//"((var1>0) + (var2>0) + (var3>0)) >> h(100,0,100)"

	//PlotParams P_jet0AntiAvg_scores_inc_train80 = {"(jet1_scores_inc_train80 + jet2_scores_inc_train80*(jet2_scores_inc_train80>0) + jet3_scores_inc_train80*(jet3_scores_inc_train80>0) ) / ( 1 + (jet2_scores_inc_train80>0)+(jet3_scores_inc_train80>0) )", "Average DNN Inclusive Score (No Jet0), train 80%", "DNN Inclusive 80%", 0, 1};
	//PlotParams P_jet1AntiAvg_scores_inc_train80 = {"(jet0_scores_inc_train80 + jet2_scores_inc_train80*(jet2_scores_inc_train80>0) + jet3_scores_inc_train80*(jet3_scores_inc_train80>0) ) / ( 1 + (jet2_scores_inc_train80>0)+(jet3_scores_inc_train80>0) )", "Average DNN Inclusive Score (No Jet1), train 80%", "DNN Inclusive 80%", 0, 1};

	//lotParams P_jet0AntiAvg_scores_inc_train80 = {"jet1_scores_inc_train80", "Average DNN Inclusive Score (No Jet0), train 80%", "DNN Inclusive 80%", 0, 1};
	//PlotParams P_jet1AntiAvg_scores_inc_train80 = {"jet0_scores_inc_train80", "Average DNN Inclusive Score (No Jet1), train 80%", "DNN Inclusive 80%", 0, 1};

	//PlotParams P_jet0AntiMax_scores_inc_train80 = {"max(jet1_scores_inc_train80, max(jet2_scores_inc_train80, jet3_scores_inc_train80))", "Max DNN Inclusive Score (No Jet0), train 80%", "DNN Inclusive 80%", 0, 1};
	//PlotParams P_jet0AntiMax_scores_depth_LLPanywhere = {"max(jet1_scores_depth_LLPanywhere, max(jet2_scores_depth_LLPanywhere, jet3_scores_depth_LLPanywhere))", "Average DNN Depth Score (No Jet0)", "DNN Depth", 0, 1};
	
	//Plot_jet0_scores_inc_train80 + jet2_scores_inc_train80MULTjet2_scores_inc_train80GT0 + jet3_scores_inc_train80MULTjet3_scores_inc_train80GT0 DIV 1+jet2_scores_inc_train80GT0+jet3_scores_inc_train80GT0 _norm_log

	//PlotParams P_jet0AntiAvg_scores_depth_LLPanywhere = {"(jet1_scores_depth_LLPanywhere+jet2_scores_depth_LLPanywhere*(jet2_scores_depth_LLPanywhere>0)+jet3_scores_depth_LLPanywhere*(jet3_scores_depth_LLPanywhere>0) ) / ( 1 + (jet2_scores_depth_LLPanywhere>0)+(jet3_scores_depth_LLPanywhere>0) )", "Average DNN Depth Score (No Jet0)", "DNN Depth", 0, 1};
	//PlotParams P_jet1AntiAvg_scores_depth_LLPanywhere = {"(jet0_scores_depth_LLPanywhere+jet2_scores_depth_LLPanywhere*(jet2_scores_depth_LLPanywhere>0)+jet3_scores_depth_LLPanywhere*(jet3_scores_depth_LLPanywhere>0) ) / ( 1 + (jet2_scores_depth_LLPanywhere>0)+(jet3_scores_depth_LLPanywhere>0) )", "Average DNN Depth Score (No Jet1)", "DNN Depth", 0, 1};



	vector<PlotParams> P_DNN_Training_Vars = {
		// Leading Jet
		/*P_jet0_scores_depth_LLPanywhere, P_jet0_scores_inc_train80,
		P_jet0_Eta, P_jet0_Mass, 
		P_jet0_Sphiphi, P_jet0_Setaeta, P_jet0_Setaphi, 
		P_jet0_Tracks_dR, 
		P_jet0_Track0dR, P_jet0_Track0dEta, P_jet0_Track0dPhi, P_jet0_Track1dR, P_jet0_Track1dEta, P_jet0_Track1dPhi, P_jet0_Track2dR, P_jet0_Track2dEta, P_jet0_Track2dPhi, 
		P_jet0_Track0PtFrac, P_jet0_Track1PtFrac, P_jet0_Track2PtFrac, 
		P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4, 
		P_jet0_LeadingRechitD, 
		P_jet0_LeadingRechitEFrac, P_jet0_SubLeadingRechitEFrac, P_jet0_SSubLeadingRechitEFrac,
		P_jet0_AllRechitE, 
		P_jet0_NeutralHadEFrac, P_jet0_ChargedHadEFrac, P_jet0_PhoEFrac, P_jet0_EleEFrac, P_jet0_MuonEFrac,*/

		// Subleading Jet
		P_jet1_scores_depth_LLPanywhere, P_jet1_scores_inc_train80,
		P_jet1_Eta, P_jet1_Mass, 
		P_jet1_Sphiphi, P_jet1_Setaeta, P_jet1_Setaphi, 
		P_jet1_Tracks_dR, 
		P_jet1_Track0dR, P_jet1_Track0dEta, P_jet1_Track0dPhi, P_jet1_Track1dR, P_jet1_Track1dEta, P_jet1_Track1dPhi, P_jet1_Track2dR, P_jet1_Track2dEta, P_jet1_Track2dPhi, 
		P_jet1_Track0PtFrac, P_jet1_Track1PtFrac, P_jet1_Track2PtFrac, 
		P_jet1_EnergyFrac_Depth1, P_jet1_EnergyFrac_Depth2, P_jet1_EnergyFrac_Depth3, P_jet1_EnergyFrac_Depth4, 
		P_jet1_LeadingRechitD, 
		P_jet1_LeadingRechitEFrac, P_jet1_SubLeadingRechitEFrac, P_jet1_SSubLeadingRechitEFrac,
		P_jet1_AllRechitE, 
		P_jet1_NeutralHadEFrac, P_jet1_ChargedHadEFrac, P_jet1_PhoEFrac, P_jet1_EleEFrac, P_jet0_MuonEFrac,
	};

	vector<PlotParams> P_jet0_vars_ALL = {
		P_jet0_RechitN,
		P_jet0_E,
		P_jet0_Pt,
		P_jet0_Eta,
		P_jet0_Phi,
		P_jet0_Mass,
		P_jet0_JetArea,
		P_jet0_dEta,
		P_jet0_dPhi,
		P_jet0_dPhi_v316,
		P_jet0met_dPhi,
		P_jet0_DeepCSV_prob_b,
		P_jet0_DeepCSV_prob_c,
		P_jet0_DeepCSV_prob_bb,
		P_jet0_DeepCSV_prob_udsg,
		P_jet0_EtaSpread,
		P_jet0_EtaSpread_energy,
		P_jet0_PhiSpread,
		P_jet0_PhiSpread_energy,
		P_jet0_EtaPhiQuadSpread,
		P_jet0_EtaPhiQuadSpread_energy,
		P_jet0_Setaeta,
		P_jet0_Sphiphi,
		P_jet0_Setaphi,
		P_jet0_Smajor,
		P_jet0_Sminor,
		P_jet0_SminorSmajor,
		P_jet0_TDCaverage,
		P_jet0_TDCaverage_Eweight,
		P_jet0_TDCnDelayed,
		P_jet0_DepthTowers_pt1,
		P_jet0_DepthTowers,
		P_jet0_TimingTowers_pt1,
		P_jet0_TimingTowers,
		P_jet0_FlaggedTowers_pt1,
		P_jet0_FlaggedTowers,
		P_jet0_dR_L1jet,
		P_jet0_Track0PtFrac,
		P_jet0_Track1PtFrac,
		P_jet0_Track2PtFrac,
		P_jet0_Track0Pt,
		P_jet0_Track1Pt,
		P_jet0_Track2Pt,
		P_jet0_Track0dxyToBS,
		P_jet0_Track1dxyToBS,
		P_jet0_Track2dxyToBS,
		P_jet0_Track0dxyOverErr,
		P_jet0_Track1dxyOverErr,
		P_jet0_Track2dxyOverErr,
		P_jet0_Track0dzToPV,
		P_jet0_Track1dzToPV,
		P_jet0_Track2dzToPV,
		P_jet0_Track0dzOverErr,
		P_jet0_Track1dzOverErr,
		P_jet0_Track2dzOverErr,
		P_jet0_Track0nMissingInnerHits,
		P_jet0_Track0nMissingOuterHits,
		P_jet0_Track0nPixelHits,
		P_jet0_Track0nHits,
		P_jet0_Tracks_dR,
		P_jet0_Track0dR,
		P_jet0_Track1dR,
		P_jet0_Track2dR,
		P_jet0_Track0dEta,
		P_jet0_Track1dEta,
		P_jet0_Track2dEta,
		P_jet0_Track0dPhi,
		P_jet0_Track1dPhi,
		P_jet0_Track2dPhi,
		P_jet0_PtAllTracks,
		P_jet0_PtAllPVTracks,
		P_jet0_NVertexTracks,
		P_jet0_NSelectedTracks,
		P_jet0_NSV,
		P_jet0_NSVCand,
		P_jet0_SV_x,
		P_jet0_SV_y,
		P_jet0_SV_z,
		P_jet0_SV_NTracks,
		P_jet0_SV_Mass,
		P_jet0_FlightDist2D,
		P_jet0_FlightDist2DErr,
		P_jet0_FlightDist3D,
		P_jet0_FlightDist3DErr,
		P_jet0_LeadingRechitE,
		P_jet0_SubLeadingRechitE,
		P_jet0_SSubLeadingRechitE,
		P_jet0_AllRechitE,
		P_jet0_AllRechitEFracJet,
		P_jet0_3RechitFracE,
		P_jet0_LeadingRechitEFracJet,
		P_jet0_SubLeadingRechitEFracJet,
		P_jet0_SSubLeadingRechitEFracJet,
		P_jet0_LeadingRechitEFrac,
		P_jet0_SubLeadingRechitEFrac,
		P_jet0_SSubLeadingRechitEFrac,
		P_jet0_LeadingRechitJetEDiff,
		P_jet0_LeadingSubRechitEQuad,
		P_jet0_LeadingRechitD,
		P_jet0_SubLeadingRechitD,
		P_jet0_SSubLeadingRechitD,
		P_jet0_NeutralHadEFrac,
		P_jet0_ChargedHadEFrac,
		P_jet0_PhoEFrac,
		P_jet0_EleEFrac,
		P_jet0_MuonEFrac,
		P_jet0_NeutralHadEFrac_zero,
		P_jet0_ChargedHadEFrac_zero,
		P_jet0_PhoEFrac_zero,
		P_jet0_EleEFrac_zero,
		P_jet0_MuonEFrac_zero,
		P_jet0_NeutralHadMult,
		P_jet0_ChargedHadMult,
		P_jet0_PhoMult,
		P_jet0_EleMult,
		P_jet0_HoverE,
		P_jet0_NeutralElePhoEFrac,
		P_jet0_NeutralPhoEFrac,
		P_jet0_NeutralOverChargedHad,
		P_jet0_LogNeutralOverChargedHad,
		P_jet0_EnergyFrac_Depth1,
		P_jet0_EnergyFrac_Depth2,
		P_jet0_EnergyFrac_Depth3,
		P_jet0_EnergyFrac_Depth4,
		P_jet0_HCALd1_over_d2,
		P_jet0_HCALd1_over_d3,
		P_jet0_HCALd1_over_d4,
		P_jet0_HCALd12_over_d34,
		P_jet0_isTruthMatched,
		P_jet0_isMatchedTo,
		P_jet0_MatchedLLP_DecayR,
		P_jet0_MatchedLLP_Eta,
		P_jet0_llp0_ptDiff,
		P_jet0_llp1_ptDiff,
		P_jet0_rechitN,
		P_jet0_energy,
		P_jet0_pt,
		P_jet0_energyFrac_depth1,
		P_jet0_energyFrac_depth2,
		P_jet0_energyFrac_depth3,
		P_jet0_energyFrac_depth4,
		P_jet0_etaSpread,
		P_jet0_etaSpread_energy,
		P_jet0_phiSpread,
		P_jet0_phiSpread_energy,
		P_jet0_track0Pt,
		P_jet0_track1Pt,
		P_jet0_track2Pt,
		P_jet0_bdtscoreX_LLP350_MS80_perJet,
		P_jet0_scores,
		P_jet0_scores_inc,
		P_jet0_scores_inc_train80,
		P_jet0_scores_inc_train40,
		P_jet0_scores_depth_hcal,
		P_jet0_scores_depth_LLPanywhere

	};

	vector<PlotParams> P_jet1_vars_ALL = {
		P_jet1_RechitN,
		P_jet1_E,
		P_jet1_Pt,
		P_jet1_Eta,
		P_jet1_Phi,
		P_jet1_Mass,
		P_jet1_JetArea,
		//P_jet1_dEta,
		//P_jet1_dPhi,
		//P_jet1_dPhi_v316,
		P_jet1met_dPhi,
		P_jet1_DeepCSV_prob_b,
		P_jet1_DeepCSV_prob_c,
		P_jet1_DeepCSV_prob_bb,
		P_jet1_DeepCSV_prob_udsg,
		P_jet1_EtaSpread,
		P_jet1_EtaSpread_energy,
		P_jet1_PhiSpread,
		P_jet1_PhiSpread_energy,
		P_jet1_EtaPhiQuadSpread,
		P_jet1_EtaPhiQuadSpread_energy,
		P_jet1_Setaeta,
		P_jet1_Sphiphi,
		P_jet1_Setaphi,
		P_jet1_Smajor,
		P_jet1_Sminor,
		P_jet1_SminorSmajor,
		P_jet1_TDCaverage,
		P_jet1_TDCaverage_Eweight,
		P_jet1_TDCnDelayed,
		P_jet1_DepthTowers_pt1,
		P_jet1_DepthTowers,
		P_jet1_TimingTowers_pt1,
		P_jet1_TimingTowers,
		P_jet1_FlaggedTowers_pt1,
		P_jet1_FlaggedTowers,
		P_jet1_dR_L1jet,
		P_jet1_Track0PtFrac,
		P_jet1_Track1PtFrac,
		P_jet1_Track2PtFrac,
		P_jet1_Track0Pt,
		P_jet1_Track1Pt,
		P_jet1_Track2Pt,
		P_jet1_Track0dxyToBS,
		P_jet1_Track1dxyToBS,
		P_jet1_Track2dxyToBS,
		P_jet1_Track0dxyOverErr,
		P_jet1_Track1dxyOverErr,
		P_jet1_Track2dxyOverErr,
		P_jet1_Track0dzToPV,
		P_jet1_Track1dzToPV,
		P_jet1_Track2dzToPV,
		P_jet1_Track0dzOverErr,
		P_jet1_Track1dzOverErr,
		P_jet1_Track2dzOverErr,
		P_jet1_Track0nMissingInnerHits,
		P_jet1_Track0nMissingOuterHits,
		P_jet1_Track0nPixelHits,
		P_jet1_Track0nHits,
		P_jet1_Tracks_dR,
		P_jet1_Track0dR,
		P_jet1_Track1dR,
		P_jet1_Track2dR,
		P_jet1_Track0dEta,
		P_jet1_Track1dEta,
		P_jet1_Track2dEta,
		P_jet1_Track0dPhi,
		P_jet1_Track1dPhi,
		P_jet1_Track2dPhi,
		P_jet1_PtAllTracks,
		P_jet1_PtAllPVTracks,
		P_jet1_NVertexTracks,
		P_jet1_NSelectedTracks,
		P_jet1_NSV,
		P_jet1_NSVCand,
		P_jet1_SV_x,
		P_jet1_SV_y,
		P_jet1_SV_z,
		P_jet1_SV_NTracks,
		P_jet1_SV_Mass,
		P_jet1_FlightDist2D,
		P_jet1_FlightDist2DErr,
		P_jet1_FlightDist3D,
		P_jet1_FlightDist3DErr,
		P_jet1_LeadingRechitE,
		P_jet1_SubLeadingRechitE,
		P_jet1_SSubLeadingRechitE,
		P_jet1_AllRechitE,
		P_jet1_AllRechitEFracJet,
		P_jet1_3RechitFracE,
		P_jet1_LeadingRechitEFracJet,
		P_jet1_SubLeadingRechitEFracJet,
		P_jet1_SSubLeadingRechitEFracJet,
		P_jet1_LeadingRechitEFrac,
		P_jet1_SubLeadingRechitEFrac,
		P_jet1_SSubLeadingRechitEFrac,
		P_jet1_LeadingRechitJetEDiff,
		P_jet1_LeadingSubRechitEQuad,
		P_jet1_LeadingRechitD,
		P_jet1_SubLeadingRechitD,
		P_jet1_SSubLeadingRechitD,
		P_jet1_NeutralHadEFrac,
		P_jet1_ChargedHadEFrac,
		P_jet1_PhoEFrac,
		P_jet1_EleEFrac,
		P_jet1_MuonEFrac,
		P_jet1_NeutralHadEFrac_zero,
		P_jet1_ChargedHadEFrac_zero,
		P_jet1_PhoEFrac_zero,
		P_jet1_EleEFrac_zero,
		P_jet1_MuonEFrac_zero,
		P_jet1_NeutralHadMult,
		P_jet1_ChargedHadMult,
		P_jet1_PhoMult,
		P_jet1_EleMult,
		P_jet1_HoverE,
		P_jet1_NeutralElePhoEFrac,
		P_jet1_NeutralPhoEFrac,
		P_jet1_NeutralOverChargedHad,
		P_jet1_LogNeutralOverChargedHad,
		P_jet1_EnergyFrac_Depth1,
		P_jet1_EnergyFrac_Depth2,
		P_jet1_EnergyFrac_Depth3,
		P_jet1_EnergyFrac_Depth4,
		P_jet1_HCALd1_over_d2,
		P_jet1_HCALd1_over_d3,
		P_jet1_HCALd1_over_d4,
		P_jet1_HCALd12_over_d34,
		P_jet1_isTruthMatched,
		P_jet1_isMatchedTo,
		P_jet1_MatchedLLP_DecayR,
		P_jet1_MatchedLLP_Eta,
		P_jet1_llp0_ptDiff,
		P_jet1_llp1_ptDiff,
		P_jet1_rechitN,
		P_jet1_energy,
		P_jet1_pt,
		P_jet1_energyFrac_depth1,
		P_jet1_energyFrac_depth2,
		P_jet1_energyFrac_depth3,
		P_jet1_energyFrac_depth4,
		P_jet1_etaSpread,
		P_jet1_etaSpread_energy,
		P_jet1_phiSpread,
		P_jet1_phiSpread_energy,
		P_jet1_track0Pt,
		P_jet1_track1Pt,
		P_jet1_track2Pt,
		P_jet1_bdtscoreX_LLP350_MS80_perJet,
		P_jet1_scores,
		P_jet1_scores_inc,
		P_jet1_scores_inc_train80,
		P_jet1_scores_inc_train40,
		P_jet1_scores_depth_hcal,
		P_jet1_scores_depth_LLPanywhere

	};

	PlotParams P_jet1_jet2_dPhi				= {"abs(jet1_Phi-jet2_Phi)*( abs(jet1_Phi-jet2_Phi) <= 3.14) + (abs(jet1_Phi-jet2_Phi)-2.*3.14)*( abs(jet1_Phi-jet2_Phi) > 3.14)", "Subleading-subsubleading Jet #Delta#phi", "Jet #Delta#phi", 0, 3.5 };


	// Culprits:
	// Charged hadron E frac --  
	// Leading jet to leading track dR
	// Sum HB Rechit Energy
	// Electron energy frac
	// Leading track pt/ jet pt


	if( false ) { 

		//for( auto disabled_feature: vector<string>{ "", "NoCustomRecHitAnything", "NoSAnything", "NoSEtaEta", "NoSPhiPhi", "NoSEtaPhi", "NoHcalEnergyFracs", "NoIndividualRechits", "NoTracks", "NoTrackPosition", "NoTrackPtFrac", "NoJetEnergyFracs" } ){
		for( auto category: vector<string>{ "LJDC", "SJDC" } ){
		for( auto year: vector<string>{"2022", "2023"}){

		//"NoSAnything", "NoRecHitAnything", "NoTracks", "NoContentEFrac"} ){ // "OnlyEtaMass"

			path = "../Files/MiniTuples/v5.3/minituple_";

			filetags_example1 = { "data_2022Ev1_scores"}; //, "HToSSTo4B_125_50_CTau3000_scores" }; //, "test_10k" };

			if( year == "2023" ) filetags_example1 = {"data_2023Dv2_scores"};

			MiniTuplePlotter plotter( filetags_example1, path );

			plotter.SetTreeName( "NoSel" );	// TreeName
			
			plotter.SetPlots( {P_PV} ); 

			if( category == "LJDC" ){
				plotter.SetCuts( "jet0_DepthTagCand == 1" );
				plotter.SetComparisonCuts( {"jet1_scores_inc_train80 < 0.2",  "jet1_scores_inc_train80 > 0.2 && jet1_scores_inc_train80 < 0.845", "jet1_scores_inc_train80 > 0.845" } ); 
				// "jet1_scores_inc_train80 < 0.2 && jet0_scores_depth_LLPanywhere > 0.8",
			}
			else if( category == "SJDC" ){
				plotter.SetCuts( "jet1_DepthTagCand == 1" );
				plotter.SetComparisonCuts( {"jet0_scores_inc_train80 < 0.2", "jet0_scores_inc_train80 > 0.2 && jet0_scores_inc_train80 < 0.375", "jet0_scores_inc_train80 > 0.375" } ); 
				// "jet0_scores_inc_train80 < 0.2 && jet1_scores_depth_LLPanywhere > 0.8", 
			}

			plotter.SetLegendNames({ year+" Data: CR, "+category, year+" Data: VR, "+category, year+" Data: MR, "+category}); //, "+Jet1 Inc Score > 0.9", "+Jet0 Depth Score > 0.01"});
			//year+" Data: CR (high depth score), "+category, 
			plotter.SetLegendPosition( 0.5, 0.72, 0.88, 0.88 );
			//plotter.SetSelectiveCuts("HToSSTo4B", "LLP0_DecayR > 10 && LLP1_DecayR > 10");
			plotter.colors = { kBlue-4, kGreen+2, kRed, kOrange-3,  }; //kOrange-3, kMagenta-7 };
			//plotter.colors = { kBlack, kGray, kRed, kOrange, kGreen+2, kAzure+7, kBlue-4 }; //, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7 };

			plotter.plot_log        = true; 
			plotter.stamp_counts    = true;
			plotter.plot_norm	   	= true; 	// Default = true
			plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
			//plotter.plot_cdf 		= true;	// Default = false. Cumulative distribution function 
			plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
			plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
			plotter.NBins = 35;
			plotter.output_file_tag = year+category;
			plotter.Plot("ratio");
		}
		}
	}


	if( false ) { 

		//for( auto disabled_feature: vector<string>{ "", "NoCustomRecHitAnything", "NoSAnything", "NoSEtaEta", "NoSPhiPhi", "NoSEtaPhi", "NoHcalEnergyFracs", "NoIndividualRechits", "NoTracks", "NoTrackPosition", "NoTrackPtFrac", "NoJetEnergyFracs" } ){
		for( auto disabled_feature: vector<string>{ "" } ){

		//"NoSAnything", "NoRecHitAnything", "NoTracks", "NoContentEFrac"} ){ // "OnlyEtaMass"

			path = "../Files/MiniTuples/v5.3_scores_DNNv6/minituple_";

			filetags_example1 = { 
				"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2023postBPix_Premix_NoSel_scores"+disabled_feature, // Baseline
				"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2022preEE_Premix_NoSel_scores"+disabled_feature,
 				"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2022postEE_PU60_NoSel_scores"+disabled_feature,
 				"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2022postEE_PU70_NoSel_scores"+disabled_feature
			}; 

			MiniTuplePlotter plotter( filetags_example1, path );

			plotter.SetTreeName( "NoSel" );	// TreeName
			
			plotter.SetPlots( {P_jet0_scores_depth_LLPanywhere, P_jet1_scores_inc_train80} ); 
			// /plotter.SetCuts( "jet0_Pt > 0 && abs(jet0_Eta) < 1.26"); //
			plotter.SetCuts( "jet0_DepthTagCand == 1" ); // && jet0_NeutralHadEFrac > 0.8 && jet0_ChargedHadEFrac < 0.2 && jet0_Track1Pt / jet0_Pt < 0.05 && jet0_PhoEFrac < 0.1" );
			//plotter.SetCuts( "jet0_InclTagCand == 1 && jet0_Pt > 40 && abs(jet0_Eta) < 1.26" );

			//plotter.SetPlots( P_jet1_vars_important ); plotter.SetCuts( "jet1_DepthTagCand == 1" );
			plotter.SetComparisonCuts( {""} ); //, "Pass_HLTDisplacedJet == 1"} ); //, "Pass_HLTDisplacedJet && Pass_PreSel == 1"} ); // && jet0_DepthTagCand == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_DepthTagCand == 1"} ); // "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_scores_inc_train80 > 0.9", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.1",} );
			plotter.SetLegendNames({ "Central, 2023postBPix", "Central, 2022preEE", "Central, 2022postEE PU60", "Central, 2022postEE PU70"}); //, "+Jet1 Inc Score > 0.9", "+Jet0 Depth Score > 0.01"});
			plotter.SetLegendPosition( 0.5, 0.72, 0.88, 0.88 );
			//plotter.SetSelectiveCuts("HToSSTo4B", "LLP0_DecayR > 10 && LLP1_DecayR > 10");
			//plotter.colors = { kRed, kOrange+7, kOrange-3, kMagenta-7 };
			//plotter.colors = { kBlack, kGray, kRed, kOrange, kGreen+2, kAzure+7, kBlue-4 }; //, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7 };

			plotter.plot_log        = true; 
			plotter.stamp_counts    = false;
			plotter.plot_norm	   	= true; 	// Default = true
			plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
			//plotter.plot_cdf 		= true;	// Default = false. Cumulative distribution function 
			plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
			plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
			plotter.NBins = 20;
			plotter.output_file_tag = "LJDC"+disabled_feature;
			plotter.Plot("ratio");

		}
	}

	if( false ){

		vector<PlotParams> P_jet0_vars_important = {
			/*P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_E, 
			P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4, 
			P_jet0_NeutralHadEFrac, P_jet0_ChargedHadEFrac, P_jet0_PhoEFrac, P_jet0_EleEFrac,*/
			P_jet0_scores_depth_LLPanywhere, P_jet0_scores_inc_train80, P_PV
		};


		vector<PlotParams> P_jet1_vars_important = {
			P_jet1_Pt, P_jet1_Eta, P_jet1_Phi, P_jet1_E, 
			P_jet1_EnergyFrac_Depth1, P_jet1_EnergyFrac_Depth2, P_jet1_EnergyFrac_Depth3, P_jet1_EnergyFrac_Depth4, 
			P_jet1_NeutralHadEFrac, P_jet1_ChargedHadEFrac, P_jet1_PhoEFrac, P_jet1_EleEFrac,
			P_jet1_scores_depth_LLPanywhere, P_jet1_scores_inc_train80
		};

		path = "../Files/MiniTuples/v5.3/minituple_";

		filetags_example1 = { 
			"data_2022Ev1_scores", 
			"data_2023Dv2_scores",
			"HToSSTo4B_125_50_CTau3000_scores", 
			"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2022postEE_PU60_NoSel_scores",
			"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2022postEE_PU70_NoSel_scores",
			"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2022preEE_Premix_NoSel_scores",
			"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2023postBPix_Premix_NoSel_scores"
		}; 

		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		
		plotter.SetPlots( P_jet0_vars_important ); 
		plotter.SetCuts( "jet0_Pt > 60 && abs(jet0_Eta) < 1.26 && PV > 25 && PV < 40" ); // && jet0_NeutralHadEFrac > 0.8 && jet0_ChargedHadEFrac < 0.2 && jet0_Track1Pt / jet0_Pt < 0.05 && jet0_PhoEFrac < 0.1" );
		//plotter.SetCuts( "jet0_InclTagCand == 1 && jet0_Pt > 40 && abs(jet0_Eta) < 1.26" );

		//plotter.SetPlots( P_jet1_vars_important ); plotter.SetCuts( "jet1_DepthTagCand == 1" );
		plotter.SetComparisonCuts( {""} ); //, "Pass_HLTDisplacedJet == 1"} ); //, "Pass_HLTDisplacedJet && Pass_PreSel == 1"} ); // && jet0_DepthTagCand == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_DepthTagCand == 1"} ); // "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_scores_inc_train80 > 0.9", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.1",} );
		plotter.SetLegendNames({"Data 2022", "Data 2023", "Private Sample", "Central, 2022postEE_PU60", "Central, 2022postEE_PU70", "Central, 2022preEE", "Central, 2023postBPix"}); //, "+Jet1 Inc Score > 0.9", "+Jet0 Depth Score > 0.01"});
		plotter.SetLegendPosition( 0.5, 0.72, 0.88, 0.88 );
		//plotter.SetSelectiveCuts("HToSSTo4B", "LLP0_DecayR > 10 && LLP1_DecayR > 10");
		//plotter.colors = { kRed, kOrange+7, kOrange-3, kMagenta-7 };
		plotter.colors = { kBlack, kGray, kRed, kOrange, kGreen+2, kAzure+7, kBlue-4 }; //, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7 };

		plotter.plot_log        = true; 
		plotter.stamp_counts    = false;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 20;
		plotter.output_file_tag = "";
		//plotter.Plot("");
		plotter.Plot2D(P_jet0_Setaeta, P_jet0_Sphiphi); //P_jet0_NeutralHadEFrac, P_jet0_ChargedHadEFrac);
	}

	if( false ) {

		vector<PlotParams> P_jet0_vars_important = {
			/*P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_E, 
			P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4, 
			P_jet0_NeutralHadEFrac, P_jet0_ChargedHadEFrac, P_jet0_PhoEFrac, P_jet0_EleEFrac,*/
			P_jet0_scores_depth_LLPanywhere, P_jet0_scores_inc_train80
		};


		vector<PlotParams> P_jet1_vars_important = {
			P_jet1_Pt, P_jet1_Eta, P_jet1_Phi, P_jet1_E, 
			P_jet1_EnergyFrac_Depth1, P_jet1_EnergyFrac_Depth2, P_jet1_EnergyFrac_Depth3, P_jet1_EnergyFrac_Depth4, 
			P_jet1_NeutralHadEFrac, P_jet1_ChargedHadEFrac, P_jet1_PhoEFrac, P_jet1_EleEFrac,
			P_jet1_scores_depth_LLPanywhere, P_jet1_scores_inc_train80
		};

		path = "../Files/MiniTuples/v5.3/minituple_";

		filetags_example1 = { 
			"HToSSTo4B_125_50_CTau3000_scores", 
			//"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2022postEE_PU60_NoSel_scores",
			//"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2022postEE_PU70_NoSel_scores",
			"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2022preEE_Premix_NoSel_scores",
			//"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2023postBPix_Premix_NoSel_scores"
		}; 

		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		//plotter.SetPlots( P_DNN_Training_Vars );
		plotter.SetPlots( P_DNN_Training_Vars ); 
		plotter.SetCuts( "jet0_Pt > 60 && abs(jet0_Eta) < 1.26" );

		//plotter.SetComparisonCuts( {"", "jet0_scores_depth_LLPanywhere < 0.5", "jet0_scores_depth_LLPanywhere >= 0.5"} ); //, "Pass_HLTDisplacedJet == 1"} ); //, "Pass_HLTDisplacedJet && Pass_PreSel == 1"} ); // && jet0_DepthTagCand == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_DepthTagCand == 1"} ); // "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_scores_inc_train80 > 0.9", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.1",} );
		//plotter.SetLegendNames({"Private Sample", "Private Sample, Jet0 Depth Score < 0.5", "Private Sample, Jet0 Depth Score > 0.5", "Central, 2022preEE", "Central, 2022preEE, Jet0 Depth Score < 0.5", "Central, 2022preEE, Jet0 Depth Score > 0.5"} ); 

		plotter.SetComparisonCuts( {"", "jet0_scores_inc_train80 < 0.5", "jet0_scores_inc_train80 >= 0.5"} ); //, "Pass_HLTDisplacedJet == 1"} ); //, "Pass_HLTDisplacedJet && Pass_PreSel == 1"} ); // && jet0_DepthTagCand == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_DepthTagCand == 1"} ); // "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_scores_inc_train80 > 0.9", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.1",} );
		plotter.SetLegendNames({"Private Sample", "Private Sample, Jet0 Inc Score < 0.5", "Private Sample, Jet0 Inc Score > 0.5", "Central, 2022preEE", "Central, 2022preEE, Jet0 Inc Score < 0.5", "Central, 2022preEE, Jet0 Inc Score > 0.5"} ); 


		plotter.SetComparisonCuts( {"", "jet0_scores_inc_train80 < 0.2", "jet0_scores_inc_train80 > 0.9"} ); //, "Pass_HLTDisplacedJet == 1"} ); //, "Pass_HLTDisplacedJet && Pass_PreSel == 1"} ); // && jet0_DepthTagCand == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_DepthTagCand == 1"} ); // "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_scores_inc_train80 > 0.9", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.1",} );
		plotter.SetLegendNames({"Private Sample", "Private Sample, Jet0 Inc Score < 0.2", "Private Sample, Jet0 Inc Score > 0.9", "Central, 2022preEE", "Central, 2022preEE, Jet0 Inc Score < 0.2", "Central, 2022preEE, Jet0 Inc Score > 0.9"} ); 


		//plotter.SetSelectiveCuts("HToSSTo4B", "LLP0_DecayR > 10 && LLP1_DecayR > 10");
		//plotter.colors = { kRed, kOrange+7, kOrange-3, kMagenta-7 };
		plotter.colors = {  kBlack, kBlue-4, kRed, kGray, kAzure+7, kOrange }; //, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7 };

		plotter.plot_log        = true;
		plotter.stamp_counts    = false;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 20;
		plotter.output_file_tag = "";
		plotter.Plot("");

	}

	if( false ) {

		vector<PlotParams> P_jet0_vars_important = {
			/*P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_jet0_E, 
			P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4, 
			P_jet0_NeutralHadEFrac, P_jet0_ChargedHadEFrac, P_jet0_PhoEFrac, P_jet0_EleEFrac,*/
			P_jet0_scores_depth_LLPanywhere, P_jet0_scores_inc_train80
		};


		vector<PlotParams> P_jet1_vars_important = {
			P_jet1_Pt, P_jet1_Eta, P_jet1_Phi, P_jet1_E, 
			P_jet1_EnergyFrac_Depth1, P_jet1_EnergyFrac_Depth2, P_jet1_EnergyFrac_Depth3, P_jet1_EnergyFrac_Depth4, 
			P_jet1_NeutralHadEFrac, P_jet1_ChargedHadEFrac, P_jet1_PhoEFrac, P_jet1_EleEFrac,
			P_jet1_scores_depth_LLPanywhere, P_jet1_scores_inc_train80
		};

		path = "../Files/MiniTuples/v5.3/minituple_";

		filetags_example1 = { 
			"HToSSTo4B_125_50_CTau3000_scores", 
			"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2022postEE_PU60_NoSel_scores",
			"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2022postEE_PU70_NoSel_scores",
			"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2022preEE_Premix_NoSel_scores",
			"HToSSTo4B_MH125_MS50_CTau3000_CentralFromRAW_2023postBPix_Premix_NoSel_scores"
		}; 

		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		//plotter.SetPlots( P_DNN_Training_Vars );
		plotter.SetPlots( P_DNN_Training_Vars ); 
		//plotter.SetCuts( "jet0_Pt > 60 && abs(jet0_Eta) < 1.26 && jet0_DepthTagCand == 1" );
		plotter.SetCuts( "jet1_Pt > 40 && abs(jet0_Eta) < 2") ; // && jet0_DepthTagCand == 1" );
		//plotter.SetCuts( "jet0_InclTagCand == 1 && jet0_Pt > 40 && abs(jet0_Eta) < 1.26" );

		//plotter.SetPlots( P_jet1_vars_important ); plotter.SetCuts( "jet1_DepthTagCand == 1" );
		plotter.SetComparisonCuts( {""} ); //, "Pass_HLTDisplacedJet == 1"} ); //, "Pass_HLTDisplacedJet && Pass_PreSel == 1"} ); // && jet0_DepthTagCand == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_DepthTagCand == 1"} ); // "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_scores_inc_train80 > 0.9", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.1",} );
		plotter.SetLegendNames({"Private Sample", "Central, 2022postEE_PU60", "Central, 2022postEE_PU70", "Central, 2022preEE", "Central, 2023postBPix"}); //, "+Jet1 Inc Score > 0.9", "+Jet0 Depth Score > 0.01"});
		plotter.SetLegendPosition( 0.5, 0.72, 0.88, 0.88 );
		//plotter.SetSelectiveCuts("HToSSTo4B", "LLP0_DecayR > 10 && LLP1_DecayR > 10");
		//plotter.colors = { kRed, kOrange+7, kOrange-3, kMagenta-7 };
		plotter.colors = { kBlack, kOrange, kGreen+2, kAzure+7, kBlue-4 }; //, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7 };

		plotter.plot_log        = true;
		plotter.stamp_counts    = false;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 20;
		plotter.output_file_tag = "";
		plotter.Plot("");
	}


	if( false ) {
		filetags_example1 = { "data_2023Dv2_scores" };
		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		plotter.SetPlots( { P_jet1_scores_inc_train80 } ); //{P_jet0_scores_depth_LLPanywhereLog} ); // { P_jet0_log_scores_depth_hcal } ); //P

		plotter.SetCuts( "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_DepthTagCand == 1" );
		plotter.SetComparisonCuts( Cuts_jet0_DeepCSV_prob_b_binned ); //, "Pass_HLTDisplacedJet && Pass_PreSel == 1"} ); // && jet0_DepthTagCand == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_DepthTagCand == 1"} ); // "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_scores_inc_train80 > 0.9", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.1",} );

		plotter.SetLegendNames({"LJDC Kinematics", "LJDC + lead jet b-tag score > 0.1", "LJDC + lead jet b-tag score > 0.2435", "LJDC + lead jet b-tag score > 0.3", "LJDC + lead jet b-tag score > 0.45", "LJDC + lead jet b-tag score > 0.8"}); //, "+Jet1 Inc Score > 0.9", "+Jet0 Depth Score > 0.01"});
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

	if ( false ) {

		filetags_example1 = { "data_2023Dv2_scores" };
		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		plotter.SetPlots( { P_jet0_scores_inc_train80 } ); //{P_jet0_scores_depth_LLPanywhereLog} ); // { P_jet0_log_scores_depth_hcal } ); //P

		plotter.SetCuts( "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_DepthTagCand == 1" );
		plotter.SetComparisonCuts( Cuts_jet1_DeepCSV_prob_b_binned ); //, "Pass_HLTDisplacedJet && Pass_PreSel == 1"} ); // && jet0_DepthTagCand == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_DepthTagCand == 1"} ); // "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_scores_inc_train80 > 0.9", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.1",} );

		plotter.SetLegendNames({"SJDC Kinematics", "SJDC + sublead jet b-tag score > 0.1", "SJDC + sublead jet b-tag score > 0.2435", "SJDC + sublead jet b-tag score > 0.3", "SJDC + sublead jet b-tag score > 0.45", "SJDC + sublead jet b-tag score > 0.8"}); //, "+Jet1 Inc Score > 0.9", "+Jet0 Depth Score > 0.01"});
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

	if( false ) {
		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		//plotter.SetPlots( P_DNN_Training_Vars );
		plotter.SetPlots( P_jet0_vars_ALL ); //{P_jet0_scores_depth_LLPanywhereLog} ); // { P_jet0_log_scores_depth_hcal } ); //P
		//plotter.SetCuts( "jet0_Pt > 30" ); //"jet0_log_scores_depth_hcal > 0" );
		plotter.SetComparisonCuts( {"Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_DepthTagCand == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_DepthTagCand == 1"} ); // "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_scores_inc_train80 > 0.9", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.1",} );
		plotter.SetLegendNames({"Data, Jet0 Depth Cand", "Data, Jet1 Depth Cand", "Signal, Jet0 Depth Cand", "Signal, Jet1 Depth Cand"}); //, "+Jet1 Inc Score > 0.9", "+Jet0 Depth Score > 0.01"});
		plotter.SetLegendPosition( 0.35, 0.78, 0.89, 0.89 );
		//plotter.SetSelectiveCuts("HToSSTo4B", "LLP0_DecayR > 10 && LLP1_DecayR > 10");
		//plotter.colors = { kRed, kOrange+7, kOrange-3, kMagenta-7 };
		plotter.colors = { kBlue-4, kRed, kAzure+7, kOrange+7 }; //, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7 };

		plotter.plot_log        = true;
		plotter.stamp_counts   = false;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 40;
		plotter.output_file_tag = "";
		plotter.Plot();
		//plotter.Plot2D(P_jet0_dEta, P_jet0_dPhi);
		//plotter.Plot2D(P_LLP0_DecayR, P_LLP1_DecayR);
		//plotter.Plot2D(P_LLP1_DecayR, P_jet0_scores_depth_LLPanywhere);
	}

	if( false ) {
		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		plotter.SetPlots({ P_LLP0_DecayR, P_LLP1_DecayR });
		plotter.SetCuts( "" ); //Cut_Pass_PreSel );
		//plotter.SetComparisonCuts( {"", "jet0_isMatchedTo == 0", "jet1_isMatchedTo == 0", "jet0_isMatchedTo == 0 || jet1_isMatchedTo == 0"} );
		//plotter.SetComparisonCuts( {"", "jet0_isTruthMatched == 1 && jet1_isTruthMatched == 1", "Pass_HLTDisplacedJet == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && (jet0_scores_inc_train80 > 0.9 || jet1_scores_inc_train80 > 0.9)", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && ( (jet0_scores_inc_train80 > 0.9 && jet1_scores_depth_LLPanywhere > 0.01) || (jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.01))"} );
		plotter.SetComparisonCuts( {"Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_isTruthMatched == 1 && jet1_isTruthMatched == 1"} );
		//plotter.SetLegendNames({"NoSel", "+Pass HLT", "+Pass PreSel", "+Jet0,1 Inc Score > 0.9", "+Jet1,0 Depth Score > 0.01"});
		plotter.SetLegendPosition( 0.35, 0.78, 0.89, 0.89 );
		//plotter.colors = { kRed, kOrange+7, kOrange-3, kMagenta-7 };
		plotter.colors = { kRed, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7 };

		plotter.plot_log        = false;
		plotter.stamp_counts = false;
		plotter.plot_norm	   	= false; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 40;
		//plotter.Plot();
		plotter.output_file_tag = "_truthmatched";
		plotter.Plot2D(P_LLP0_DecayR, P_LLP1_DecayR);
		//plotter.Plot2D(P_LLP1_DecayR, P_jet0_scores_depth_LLPanywhere);
	}

	if( false ) {
		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		plotter.SetPlots( P_DNN_Training_Vars );
		plotter.SetCuts( "" );

		plotter.SetComparisonCuts( {"", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_DepthTagCand == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_DepthTagCand == 1"} ); // "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet0_scores_inc_train80 > 0.9", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.1",} );
		plotter.SetLegendNames({"NoSel", "Pass HLT, PreSel", "Jet0 Depth Cand", "Jet1 Depth Cand"}); //, "+Jet1 Inc Score > 0.9", "+Jet0 Depth Score > 0.01"});
		plotter.SetLegendPosition( 0.35, 0.78, 0.89, 0.89 );
		//plotter.colors = { kRed, kOrange+7, kOrange-3, kMagenta-7 };
		plotter.colors = { kBlack, kGray+1, kBlue-4, kOrange+7 }; //, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7 };

		plotter.plot_log        = false;
		plotter.stamp_counts   = false;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 40;
		plotter.output_file_tag = "2023";
		plotter.Plot();
		//plotter.Plot2D(P_jet0_dEta, P_jet0_dPhi);
		//plotter.Plot2D(P_LLP0_DecayR, P_LLP1_DecayR);
		//plotter.Plot2D(P_LLP1_DecayR, P_jet0_scores_depth_LLPanywhere);
	}

	if( false ) {
		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		plotter.SetPlots({ P_jet0_Phi, P_jet1_Phi});// P_jet0_Pt, P_jet1_Pt, P_jet0_Eta, P_jet1_Eta, P_jet0_Phi, P_jet1_Phi });
		plotter.SetCuts( "" ); //Cut_Pass_PreSel );
		//plotter.SetComparisonCuts( {"", "jet0_isMatchedTo == 0", "jet1_isMatchedTo == 0", "jet0_isMatchedTo == 0 || jet1_isMatchedTo == 0"} );
		plotter.SetCuts( "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1" ); //Cut_Pass_PreSel );
		plotter.SetComparisonCuts( {"", "jet0_scores_inc_train80 > 0.2", "jet0_scores_depth_LLPanywhere > 0.2", "jet1_scores_inc_train80 > 0.2 && jet0_scores_depth_LLPanywhere > 0.2"}); //, "jet0_DepthTagCand", "jet1_DepthTagCand", "(jet0_DepthTagCand == 1 || jet1_DepthTagCand)"} );
		plotter.SetLegendPosition( 0.35, 0.78, 0.89, 0.89 );
		//plotter.colors = { kRed, kOrange+7, kOrange-3, kMagenta-7 };
		plotter.colors = { kBlue+3, kBlue-4, kAzure-2, kAzure+7, kRed, kOrange+7, kOrange-3, kMagenta-7 };

		plotter.plot_log        = false;
		plotter.stamp_counts   = false;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 60;
		plotter.output_file_tag = "test";
		plotter.Plot();
		//plotter.Plot2D(P_jet0_dEta, P_jet0_dPhi);
		//plotter.Plot2D(P_LLP0_DecayR, P_LLP1_DecayR);
		//plotter.Plot2D(P_LLP1_DecayR, P_jet0_scores_depth_LLPanywhere);
	}

	if( false ) {

		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		plotter.SetPlots({ P_LLP0_DecayR, P_LLP1_DecayR });
		plotter.SetCuts( "" ); //Cut_Pass_PreSel );
		//plotter.SetComparisonCuts( {"", "jet0_isMatchedTo == 0", "jet1_isMatchedTo == 0", "jet0_isMatchedTo == 0 || jet1_isMatchedTo == 0"} );
		plotter.SetComparisonCuts( {"", "Pass_HLTDisplacedJet == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && (jet0_scores_inc_train80 > 0.9 || jet1_scores_inc_train80 > 0.9)", "Pass_HLTDisplacedJet == 1 && Pass_PreSel == 1 && ( (jet0_scores_inc_train80 > 0.9 && jet1_scores_depth_LLPanywhere > 0.01) || (jet1_scores_inc_train80 > 0.9 && jet0_scores_depth_LLPanywhere > 0.01))"} );
		plotter.SetLegendNames({"NoSel", "+Pass HLT", "+Pass PreSel", "+Jet0,1 Inc Score > 0.9", "+Jet1,0 Depth Score > 0.01"});
		plotter.SetLegendPosition( 0.35, 0.78, 0.89, 0.89 );
		//plotter.colors = { kRed, kOrange+7, kOrange-3, kMagenta-7 };
		plotter.colors = { kRed, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7 };

		plotter.plot_log        = false;
		plotter.stamp_counts = true;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 40;
		plotter.Plot();
		//plotter.Plot2D(P_LLP1_DecayR, P_jet0_scores_inc_train80);
		//plotter.Plot2D(P_LLP1_DecayR, P_jet0_scores_depth_LLPanywhere);
	}

	if( false ) {

		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		//plotter.SetPlots({ P_jet0AntiMax_scores_inc_train80 });
		//plotter.SetPlots({ P_jet0AntiMax_scores_depth_LLPanywhere });
		//plotter.SetPlots({ P_jet0AntiAvg_scores_depth_LLPanywhere, P_jet1AntiAvg_scores_depth_LLPanywhere });
		//plotter.SetPlots({ P_jet0_scores_depth_LLPanywhere, P_jet1_scores_depth_LLPanywhere, P_jet2_scores_depth_LLPanywhere, P_jet3_scores_depth_LLPanywhere});
		plotter.SetCuts( "Pass_HLTDisplacedJet == 1 && jet0_isMatchedTo == 1" ); //Cut_Pass_PreSel );
		//plotter.SetComparisonCuts( {"", "jet0_DepthTagCand", "jet1_DepthTagCand", "(jet0_DepthTagCand == 1 || jet1_DepthTagCand)"} );
		plotter.SetLegendPosition( 0.35, 0.78, 0.89, 0.89 );
		plotter.colors = { kBlue+3, kBlue-4, kAzure-2, kAzure+7, kRed, kOrange+7, kOrange-3, kMagenta-7 };

		plotter.plot_log        = true;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= true;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 40;
		plotter.Plot2D(P_LLP1_DecayR, P_jet0_scores_inc_train80);
		plotter.Plot2D(P_LLP1_DecayR, P_jet0_scores_depth_LLPanywhere);
	}

	if( false ) {

		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		//plotter.SetPlots({ P_jet0_scores_inc_train80, P_jet1_scores_inc_train80, P_jet2_scores_inc_train80, P_jet3_scores_inc_train80 });
		plotter.SetPlots({ P_jet0_scores_depth_LLPanywhere, P_jet1_scores_depth_LLPanywhere, P_jet2_scores_depth_LLPanywhere, P_jet3_scores_depth_LLPanywhere});
		plotter.SetCuts( "Pass_HLTDisplacedJet == 1" ); //Cut_Pass_PreSel );
		plotter.SetComparisonCuts( {"", "jet0_DepthTagCand", "jet1_DepthTagCand", "(jet0_DepthTagCand == 1 || jet1_DepthTagCand)"} );
		plotter.SetLegendPosition( 0.35, 0.78, 0.89, 0.89 );
		plotter.colors = { kBlue+3, kBlue-4, kAzure-2, kAzure+7, kRed, kOrange+7, kOrange-3, kMagenta-7 };

		plotter.plot_log        = true;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 40;
		plotter.Plot();
	}

	if( false ) {

		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		plotter.SetPlots({P_jet1_scores_inc_train80});
		plotter.SetCuts( Cut_Pass_PreSel );
		plotter.SetComparisonCuts( {"", "jet0_DepthTagCand", "jet1_DepthTagCand", "(jet0_DepthTagCand == 1 || jet1_DepthTagCand)"} );
		plotter.SetLegendPosition( 0.35, 0.78, 0.89, 0.89 );
		plotter.colors = { kBlue+3, kBlue-4, kAzure-2, kAzure+7, kRed, kOrange+7, kOrange-3, kMagenta-7 }; // kBlack, kOrange, kGreen+2, 

		plotter.plot_log        = false;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= true;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 40;
		//plotter.Plot();
	}

	if( false ) {

		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		plotter.SetPlots({P_jet0_scores_inc_train80});
		plotter.SetCuts( Cut_Pass_PreSel );
		plotter.SetComparisonCuts( {"", "jet0_DepthTagCand", "jet1_DepthTagCand", "(jet0_DepthTagCand == 1 || jet1_DepthTagCand)"} );
		plotter.SetLegendPosition( 0.35, 0.78, 0.89, 0.89 );
		plotter.colors = { kBlue+3, kBlue-4, kAzure-2, kAzure+7, kRed, kOrange+7, kOrange-3, kMagenta-7 };

		plotter.plot_log        = false;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= true;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 40;
		//plotter.Plot();
	}

	if( false ) {

		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		plotter.SetPlots({P_jet0_scores_depth_LLPanywhere});
		plotter.SetCuts( Cut_Pass_PreSel );
		plotter.SetComparisonCuts( {"", "jet0_DepthTagCand", "jet1_DepthTagCand", "(jet0_DepthTagCand == 1 || jet1_DepthTagCand)"} );
		plotter.SetLegendPosition( 0.35, 0.78, 0.89, 0.89 );
		plotter.colors = { kBlue+3, kBlue-4, kAzure-2, kAzure+7, kRed, kOrange+7, kOrange-3, kMagenta-7 }; // kBlack, kOrange, kGreen+2, 

		plotter.plot_log        = true;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 40;
		//plotter.Plot();
	}

	// LJDC Inclusive //

	vector<TCut> Binned_jet0_scores_depth_LLPanywhere = {
		"jet0_scores_depth_LLPanywhere < 0.2", 
		"jet0_scores_depth_LLPanywhere > 0.2 && jet0_scores_depth_LLPanywhere < 0.5", 
		"jet0_scores_depth_LLPanywhere > 0.5 && jet0_scores_depth_LLPanywhere < 0.8", 
		"jet0_scores_depth_LLPanywhere > 0.8" // && jet0_scores_depth_LLPanywhere < 0.5", 
	};

	if( false ) {
		filetags_example1 = { "HToSSTo4B_125_50_CTau3000_scores" }; //, "test_10k" };

		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		plotter.SetPlots({P_jet1_scores_inc_train80});
		plotter.SetCuts( Cut_Pass_PreSel + "jet0_DepthTagCand == 1 && jet1_isTruthMatched == 1");
		plotter.SetComparisonCuts( Binned_jet0_scores_depth_LLPanywhere );
		plotter.SetLegendPosition( 0.35, 0.78, 0.89, 0.89 );
		plotter.colors = { kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed  };

		//plotter.plot_log        = false;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 30;
		plotter.Plot();
	}

	// SJDC Inclusive //

	vector<TCut> Binned_jet1_scores_depth_LLPanywhere = {
		"jet1_scores_depth_LLPanywhere < 0.2", 
		"jet1_scores_depth_LLPanywhere > 0.2 && jet1_scores_depth_LLPanywhere < 0.5", 
		"jet1_scores_depth_LLPanywhere > 0.5 && jet1_scores_depth_LLPanywhere < 0.8", 
		"jet1_scores_depth_LLPanywhere > 0.8" // && jet0_scores_depth_LLPanywhere < 0.5", 
	};

	if( false ) {
		filetags_example1 = { "HToSSTo4B_125_50_CTau3000_scores" }; //, "test_10k" };


		MiniTuplePlotter plotter( filetags_example1, path );

		plotter.SetTreeName( "NoSel" );	// TreeName
		plotter.SetPlots({P_jet0_scores_inc_train80});
		plotter.SetCuts( Cut_Pass_PreSel + "jet1_DepthTagCand == 1 && jet0_isTruthMatched == 1");
		plotter.SetComparisonCuts( Binned_jet1_scores_depth_LLPanywhere );
		plotter.SetLegendPosition( 0.35, 0.78, 0.89, 0.89 );
		plotter.colors = { kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed  };

		//plotter.plot_log        = false;
		plotter.plot_norm	   	= true; 	// Default = true
		plotter.plot_grid 	  	= true;	// Default = true. Draw grid lines on plot
		plotter.plot_cdf 		= false;	// Default = false. Cumulative distribution function 
		plotter.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
		plotter.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
		plotter.NBins = 30;
		plotter.Plot();
	}

}
