#include "../MiniTuplePlotter.h"
#include "TString.h"

// September 2024, Gillian Kopp
// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_SignalContamination(){

	// List where minituples are stored
	/*
	string path = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.8.1/minituple_";
	vector<string> filetags_both = 	{ "v3.8_LLPskim_Run2023Dv1_2024_06_03", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau10000_13p6TeV_2024_06_03" };
	vector<string> filetags_data = 	{ "v3.8_LLPskim_Run2023Dv1_2024_06_03" };
	vector<string> filetags_MC = 	{ "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau10000_13p6TeV_2024_06_03" };
	*/
	string path = "/eos/user/g/gkopp/SWAN_projects/LLP_DNN_Tagger/minituple_";
	vector<string> filetags_both = 	{ "v3.9_LLPskim_Run2023Dv1_2024_10_14_scores", "v3.9_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_10_14_batch1_scores" };
	vector<string> filetags_data = 	{ "v3.9_LLPskim_Run2023Dv1_2024_10_14_scores" };
	vector<string> filetags_MC = 	{ "v3.9_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_10_14_batch1_scores" };

	#include "../RegionCuts.h"

	cout<<endl;
	cout<<" ---------- Study 1 ---------- "<<endl;
	cout<<endl;

	vector<vector<PlotParams>> plot_type = { {P_perJet_Pt}, {P_perJet_E}, {P_perJet_Eta}, {P_perJet_Phi} };

    // cuts: jet1_isTruthMatched (to LLP), jet1_L1trig_Matched (to L1), jet1_pT, jet1_Eta, jet0_bdtscoreX_LLP350_MS80_perJet

	class MiniTuplePlotter inclusiveTag( filetags_both, path );
	// inclusiveTag.SetPlots({P_jet1_bdtscoreX_LLP350_MS80_perJet, P_jet2_bdtscoreX_LLP350_MS80_perJet});
	inclusiveTag.SetPlots({P_jet1_scores_inc, P_jet2_scores_inc, P_jet1and2_scores_inc, P_jet1and2_scores});
	// inclusiveTag.SetTreeNames( {"NoSel", "PassedHLT"} );
	inclusiveTag.SetTreeNames( {"Classification", "Classification"} );
	inclusiveTag.SetOutputFileTag("HLT_v3_InclusiveTag");
	inclusiveTag.SetOutputDirectory("SignalContamination");
	inclusiveTag.plot_norm 			= false;
	inclusiveTag.plot_log_ratio   	= true;
    // need to determine which jets to look at, and need to find which jet is matched to a flagged L1 jet
	// inclusiveTag.SetSelectiveCuts("MC", Form("eventHT > %s && perJet_Pt >= %s && perJet_MatchedLLP_DecayR < 300", event_HT[0].c_str(), jet_E[0].c_str() ) ); // cut on the jet pT
	inclusiveTag.SetCuts("jet0_L1trig_Matched == 1");
	inclusiveTag.colors = { kBlack, kAzure+7, kGray+1, kViolet+4 };
	inclusiveTag.SetLegendNames({"LLP skim", "m_{H}=125 GeV, m_{S}=50 GeV, c#tau=3 m"});
	inclusiveTag.SetLegendPosition( 0.13, 0.8, 0.6, 0.98 );
	inclusiveTag.SetLegendPosition( 0.4, 0.7, 0.87, 0.88 );
	inclusiveTag.Plot("ratio");
	inclusiveTag.ClearFileTrees(); 
	// inclusiveTag.Plot2D(P_jet1_bdtscoreX_LLP350_MS80_perJet, P_jet2_bdtscoreX_LLP350_MS80_perJet);
	inclusiveTag.Plot2D(P_jet1_scores_inc, P_jet2_scores_inc);
	
}
