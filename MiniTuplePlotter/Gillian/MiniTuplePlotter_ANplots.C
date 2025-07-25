#include "../MiniTuplePlotter.h"
#include "TString.h"
#include "../RegionCuts.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_ANplots(){

	string path_v3 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.14/minituple_"; // last one with per jet tree
	string path_v4 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v4.1/";
	
	vector<string> filetags_v4 = { "DisplacedJet_Run2023C-EXOLLPJetHCAL-PromptReco-v1_AOD_2025_07_21_scores" }; 

	map<string,vector<string>> filetags_v3;
	filetags_v3["LLP125_mX15"]			= { "LLPskim_2023Cv4", "HToSSTo4b_125_15_CTau10000"};
	filetags_v3["LLP125_mX15_ctau3m"]	= { "LLPskim_2023Cv4", "HToSSTo4b_125_15_CTau3000"};
	filetags_v3["LLP125"]				= { "LLPskim_2023Cv4", "HToSSTo4b_125_50_CTau3000_batch2"};
	filetags_v3["LLP125_ctau10m"]		= { "LLPskim_2023Cv4", "HToSSTo4b_125_50_CTau10000"};
	filetags_v3["LLP250"]				= { "LLPskim_2023Cv4", "HToSSTo4b_250_120_CTau10000_batch2"};
	filetags_v3["LLP350_mX80"]			= { "LLPskim_2023Cv4", "HToSSTo4b_350_80_CTau500"};
	filetags_v3["LLP350"]				= { "LLPskim_2023Cv4", "HToSSTo4b_350_160_CTau10000_batch2"};

	vector<PlotParams> analysisVars_perJet_HCALdepth = {P_perJet_EnergyFrac_Depth1, P_perJet_EnergyFrac_Depth2, P_perJet_EnergyFrac_Depth3, P_perJet_EnergyFrac_Depth4};

	vector<PlotParams> analysisVars_perJet_jets	= {P_perJet_NeutralHadEFrac, P_perJet_ChargedHadEFrac, P_perJet_MuonEFrac, P_perJet_PhoEFrac, P_perJet_EleEFrac,
										P_perJet_Track0PtFrac, P_perJet_Track1PtFrac, P_perJet_Track0dR, P_perJet_Track1dR, 
										P_perJet_LeadingRechitEFracJet, P_perJet_Sphiphi};

	cout<<endl;
	cout<<" ---------- Plots of energy fraction in each of HCAL depths, by LLP decay position ---------- "<<endl;
	cout<<endl;
		
	// vector<string> filetag_keys_to_loop = {"LLP125_mX15", "LLP125_mX15_ctau3m", "LLP125", "LLP125_ctau10m", "LLP250", "LLP350", "LLP350_mX80"};
	vector<string> filetag_keys_to_loop = {"LLP125_mX15"};

	for( auto key: filetag_keys_to_loop){

		bool simple = false;
		bool reduced = true; // run for all other plots
		bool hcal_only = false; // hcal depths
		// HCAL depth variables with LLP regions and data overlayed
		class MiniTuplePlotter HCALdepths( filetags_v3[key], path_v3 );
		HCALdepths.SetPlots(analysisVars_perJet_HCALdepth); 
		HCALdepths.SetTreeName( "PerJet_NoSel" );	
		HCALdepths.SetOutputFileTag("Overlay_perJet_"+key+"_v3.14");
		if (simple) HCALdepths.SetOutputFileTag("SimpleOverlay_perJet_"+key+"_v3.14");
		if (reduced) HCALdepths.SetOutputFileTag("ReducedOverlay_perJet_"+key+"_v3.14");
		if (hcal_only) HCALdepths.SetOutputFileTag("HcalOverlay_perJet_"+key+"_v3.14");
		HCALdepths.plot_log_ratio    = false; 
		HCALdepths.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		HCALdepths.SetLegendNames({"LLP skim - W+jets selection", "LLP in tracker <= 10cm", "LLP in tracker > 10cm", "LLP in ECAL", "LLP in HCAL, D1", "LLP in HCAL, D2", "LLP in HCAL, D34"});
		HCALdepths.SetLegendNames({"Prompt background", "LLP in tracker <= 10cm", "LLP in tracker > 10cm", "LLP in ECAL", "LLP in HCAL, D1", "LLP in HCAL, D2", "LLP in HCAL, D34"});
		HCALdepths.colors = { kBlack, kGray, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed };
		HCALdepths.SetComparisonCuts({Cut_matchedLLPinCR, Cut_matchedLLPinTrackerNP, Cut_matchedLLPinECAL, Cut_matchedLLPinHCAL1, Cut_matchedLLPinHCAL2, Cut_matchedLLPinHCAL34}, "HToSSTo4b");
		HCALdepths.SetSelectiveCuts("LLPskim", "Pass_WPlusJets");
		HCALdepths.SetSelectiveCuts("HToSSTo4b", "Pass_LLPMatched");
		if (simple) {
			HCALdepths.SetLegendNames({"Prompt background", "LLP in HCAL"});
			HCALdepths.colors = { kBlack, kAzure-3 };
			HCALdepths.SetComparisonCuts({Cut_matchedLLPinHCAL}, "HToSSTo4b");
		}
		if (reduced) {
			HCALdepths.SetPlots(analysisVars_perJet_jets); 
			HCALdepths.SetLegendNames({"Prompt background", "LLP in tracker <= 10cm", "LLP in tracker > 10cm", "LLP in ECAL", "LLP in HCAL"});
			HCALdepths.colors = { kBlack, kGray, kOrange, kGreen+2, kAzure-3 };
			HCALdepths.SetComparisonCuts({Cut_matchedLLPinCR, Cut_matchedLLPinTrackerNP, Cut_matchedLLPinECAL, Cut_matchedLLPinHCAL}, "HToSSTo4b");
		}
		if (hcal_only) {
			HCALdepths.SetLegendNames({"Prompt background", "LLP in HCAL, D1", "LLP in HCAL, D2", "LLP in HCAL, D34"});
			HCALdepths.colors = { kBlack, kAzure+7, kOrange-3, kRed-7 };
			HCALdepths.SetComparisonCuts({Cut_matchedLLPinHCAL1, Cut_matchedLLPinHCAL2, Cut_matchedLLPinHCAL34}, "HToSSTo4b");
		}
		HCALdepths.SetOutputDirectory("Overlay_perJet_"+key);
		HCALdepths.Plot();
	}

}

