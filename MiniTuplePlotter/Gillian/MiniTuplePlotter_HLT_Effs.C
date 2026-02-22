#include "../MiniTuplePlotter.h"
#include "TString.h"

// Goals of this study: use the LLP MC sample, understand efficiency of each HLT passed, as a function of various kinematic (jet) variables
// Mostly worked on during September.
// Returning to in December 2023 to compare mH 125 and 350 GeV samples
// Used in June / July 2024 for L1 approval plots
// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_HLT_Effs(){

	// List where minituples are stored
	// string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.2/minituple_";
	// vector<string> filetags_both = 	{ "v1.2_LLPskim_500k_2023_08_31", "v1.2_MCsignal_500k_2023_08_31" };
	// vector<string> filetags_data = 	{ "v1.2_LLPskim_500k_2023_08_31" };
	// vector<string> filetags_MC = 	{ "v1.2_MCsignal_500k_2023_08_31" };

	string path_v3 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_";
	string path = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.8/minituple_";

	map<string,vector<string>> filetags;
	filetags["LLP125"]	= { "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2023_11_23"};
	// filetags["LLP350"]	= { "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2023_11_29"};

	filetags["LLP350_80_ctau500mm"]	= { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_rerun"}; //{ "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_TEST"};
	filetags["LLP125_50_ctau10m"]	= { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau10000_13p6TeV_2024_06_03"};
	filetags["LLP1000_450_ctau100m"] = { "MC_MH-1000_MFF-450_CTau-100000mm_TuneCP5_13p6TeV_Nov15_depthTagger"};
	filetags["LLP1000_450_ctau10m"] = { "MC_MH-1000_MFF-450_CTau-10000mm_TuneCP5_13p6TeV_Nov15_depthTagger"};

	filetags["LLP125_15_ctau10m"]	= { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau10000_13p6TeV_2024_06_03"};
	filetags["LLP125_15_ctau1m"]	= { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TEST"};
	filetags["LLP125_15_ctau3m"]	= { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau3000_13p6TeV_2024_06_03"};
	filetags["LLP125_50_ctau3m"]	= { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_combined"};
	// filetags["LLP125_50_ctau3m"]	= { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_batch1"};
	filetags["LLP250_120_ctau10m"]	= { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_06_03_batch1"};
	filetags["LLP350_160_ctau10m"]	= { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_06_03_batch1"};
	
	// filetags["LLP125_mX15"]	= { "v3.6_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_03_02_TEST"};
	// filetags["LLP350_mX80"]	= { "v3.6_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_03_02_TEST"};
	// filetags["LLP125"]		= { "v3.6_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_03_02_batch2"};
	// filetags["LLP250"]		= { "v3.6_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_03_02_batch2"};
	// filetags["LLP350"]		= { "v3.6_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_03_02_batch2"};

	vector<string> filetags_LLP125	= { "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2023_11_23"};
	vector<string> filetags_LLP350	= { "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2023_11_29"};
	vector<string> filetags_LLP_all	= { "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2023_11_23", "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2023_11_29"};

	#include "../RegionCuts.h"

	bool September = false;

	bool HLT_OR = false;
	bool acceptance = true;
	bool HLT_only = true;

	// December 2023 studies
	cout<<endl;
	cout<<" ---------- HLT Efficiency Study 1: Jet kinematics split by HLT paths passed ---------- "<<endl;
	cout<<endl;

	// to do -- make a new version that is jet based!

	vector<string> filetag_keys_to_loop = {"LLP350_80_ctau500mm", "LLP125_50_ctau3m", "LLP1000_450_ctau100m", "LLP1000_450_ctau10m"};
	// vector<string> filetag_keys_to_loop = {"LLP350_80_ctau500mm", "LLP125_15_ctau3m", "LLP125_50_ctau10m", "LLP125_15_ctau10m", "LLP125_15_ctau1m", "LLP125_50_ctau3m", "LLP250_120_ctau10m", "LLP350_160_ctau10m"};

	vector<string> jet_E = {"100"}; //{"60", "100"};
	vector<string> event_HT = {"250"}; // {"200", "170"};

	vector<vector<double>> variable_bins = { //{0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 290, 330, 390, 490, 610, 760, 950, 1200},
											{40, 43, 46, 49, 52, 55, 58, 61, 64, 67, 70, 73, 76, 79, 82, 86, 90, 95, 100, 110, 124, 140, 160, 185, 215, 250},
											{40, 43, 46, 49, 52, 55, 58, 61, 64, 67, 70, 73, 76, 79, 82, 86, 90, 95, 100, 110, 124, 140, 160, 185, 215, 250},
											{-1.26, -1.16, -1.06, -0.96, -0.86, -0.76, -0.66, -0.56, -0.46, -0.36, -0.26, -0.16, -0.06, 0.06, 0.16, 0.26, 0.36, 0.46, 0.56, 0.66, 0.76, 0.86, 0.96, 1.06, 1.16, 1.26},
											{-3.15, -3, -2.85, -2.7, -2.55, -2.4, -2.25, -2.1, -1.95, -1.8, -1.65, -1.5, -1.35, -1.2, -1.05, -0.9, -0.75, -0.6, -0.45, -0.3, -0.15, 0, 0.15, 0.3, 0.45, 0.6, 0.75, 0.9, 1.05, 1.2, 1.35, 1.5, 1.65, 1.8, 1.95, 2.1, 2.25, 2.4, 2.55, 2.7, 2.85, 3.0, 3.15} };
	vector<vector<PlotParams>> plot_type = { // {P_eventHT}, 
											{P_perJet_Pt}, {P_perJet_E}, {P_perJet_Eta}, {P_perJet_Phi} };

	// overlay each mass point
	class MiniTuplePlotter overlay_LLPdisplacement( { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_rerun", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_combined"}, path );
	overlay_LLPdisplacement.SetPlots({P_perJet_MatchedLLP_DecayR});
	overlay_LLPdisplacement.SetTreeName( "PerJet_LLPmatched" );
	overlay_LLPdisplacement.SetOutputFileTag("HLT_v3_MC_jetE"+jet_E[0]+"_L1effs");
	overlay_LLPdisplacement.SetOutputDirectory("HLT_Efficiencies_31Jan");
	overlay_LLPdisplacement.plot_norm 			= false;
	overlay_LLPdisplacement.plot_log_ratio   	= true;
	overlay_LLPdisplacement.plot_grid			= false;
	overlay_LLPdisplacement.SetLegendPosition( 0.12, 0.6, 0.55, 0.95 );
	overlay_LLPdisplacement.SetSelectiveCuts("MC", Form("eventHT > %s && perJet_Pt >= %s && perJet_MatchedLLP_DecayR < 300", event_HT[0].c_str(), jet_E[0].c_str() ) ); // cut on the jet pT
	overlay_LLPdisplacement.SetComparisonCuts({Cut_None, Cut_HLTpassed1});
	overlay_LLPdisplacement.colors = { kBlack, kAzure+7, kGray+1, kViolet+4 };
	overlay_LLPdisplacement.SetLegendNames({"No cuts", "#splitline{m_{H} = 350 GeV,}{m_{S} = 80 GeV, c#tau = 0.5 m}", "No cuts", "#splitline{m_{H} = 125 GeV,}{m_{S} = 50 GeV, c#tau = 3 m}"});
	overlay_LLPdisplacement.SetVariableBins( {0,50,100,150,175,190,200,208,215,222,229,236,243,250,257,264,271,278,285,292,300} );
	overlay_LLPdisplacement.Plot("efficiency", "", {"0","0","0"}, true);
	overlay_LLPdisplacement.ClearFileTrees(); 
	overlay_LLPdisplacement.SetOutputFileTag("HLT_v3_MC_jetE"+jet_E[0]+"_HLTeffs");
	overlay_LLPdisplacement.SetLegendPosition( 0.15, 0.5, 0.53, 0.68 );
	overlay_LLPdisplacement.SetComparisonCuts({Cut_HLTpassed1, Cut_AnyLLP_HLT_noECAL});
	overlay_LLPdisplacement.SetLegendNames({"L1", "m_{H}=350 GeV, HLT efficiency", "L1", "m_{H}=125 GeV, HLT efficiency"});
	overlay_LLPdisplacement.Plot("efficiency", "", {"0","0","0"}, true);

	class MiniTuplePlotter overlay_HT( { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_rerun", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_combined"}, path );
	overlay_HT.SetPlots({P_eventHT});
	overlay_HT.SetTreeName( "NoSel" ); 
	overlay_HT.SetOutputFileTag("HLT_v3_MC_eventHT_L1effs");
	overlay_HT.SetOutputDirectory("HLT_Efficiencies_31Jan");
	overlay_HT.plot_norm 			= false;
	overlay_HT.plot_log_ratio   	= true;
	overlay_HT.plot_grid			= false;
	overlay_HT.SetLegendPosition( 0.32, 0.55, 0.65, 0.73 ); // -0.1 to move legend left
	overlay_HT.SetSelectiveCuts("MC", Cut_LLPinHCAL34_AnyJet); // jet is matched to LLP in HCAL 34 and jet pT > 100 GeV
	overlay_HT.SetComparisonCuts({Cut_None, Cut_HLTpassed1});
	overlay_HT.colors = { kBlack, kAzure+7, kGray+1, kViolet+4 };
	overlay_HT.SetLegendNames({"No cuts", "m_{H} = 350 GeV, m_{S} = 80 GeV, c#tau = 0.5 m", "No cuts", "m_{H} = 125 GeV, m_{S} = 50 GeV, c#tau = 3 m"});
	overlay_HT.SetVariableBins( {40, 60, 80, 100, 120, 140, 160, 190, 220, 250, 290, 330, 390, 490, 640, 800, 1000, 1200} );
	overlay_HT.SetVariableBins( {40, 80, 120, 145, 185, 220, 250, 290, 330, 390, 490, 640, 800, 1000, 1200} );
	overlay_HT.Plot("efficiency", "", {"0","0","0"}, true);
	overlay_HT.ClearFileTrees(); 
	overlay_HT.SetOutputFileTag("HLT_v3_MC_eventHT_HLTeffs");
	overlay_HT.SetLegendPosition( 0.15, 0.9, 0.53, 1.08 );
	overlay_HT.SetComparisonCuts({Cut_HLTpassed1, Cut_AnyLLP_HLT_noECAL});
	overlay_HT.SetLegendNames({"L1", "m_{H}=350 GeV, HLT efficiency", "L1", "m_{H}=125 GeV, HLT efficiency"});
	overlay_HT.Plot("efficiency", "", {"0","0","0"}, true);

	class MiniTuplePlotter overlay_jet( { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_noPerJetPt_2024_06_30", "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_noPerJetPt_2024_06_30"}, path );
	overlay_jet.SetPlots({P_perJet_Pt});
	overlay_jet.SetTreeName( "PerJet_LLPmatched" );
	overlay_jet.SetOutputFileTag("HLT_v3_MC_L1effs");
	overlay_jet.SetOutputDirectory("HLT_Efficiencies_31Jan");
	overlay_jet.plot_norm 			= false;
	overlay_jet.plot_log_ratio   	= true;
	overlay_jet.plot_grid			= false;
	overlay_jet.SetLegendPosition( 0.32, 0.55, 0.65, 0.73 );
	overlay_jet.SetVariableBins( {40, 46, 52, 58, 64, 70, 76, 82, 88, 94, 100, 110, 124, 140, 160, 185, 215, 250} );
	overlay_jet.SetVariableBins( {0, 10, 26, 40, 54, 63, 72, 81, 90, 100, 110, 120, 140, 165, 200, 250, 300} );	
	overlay_jet.SetSelectiveCuts("MC", Cut_matchedLLPinHCAL34_eventHT250);		// region for LLP decay, and require LLP is matched to jet 0, and event HT 250
	overlay_jet.SetComparisonCuts({Cut_None, Cut_HLTpassed1}); 
	overlay_jet.colors = { kBlack, kAzure+7, kGray+1, kViolet+4 };
	overlay_jet.SetLegendNames({"No cuts", "m_{H} = 350 GeV, m_{S} = 80 GeV, c#tau = 0.5 m", "No cuts", "m_{H} = 125 GeV, m_{S} = 50 GeV, c#tau = 3 m"});
	overlay_jet.Plot("efficiency", "", {"0","0","0"}, true);
	overlay_jet.ClearFileTrees(); 
	overlay_jet.SetOutputFileTag("HLT_v3_MC_HLTeffs");
	overlay_jet.SetComparisonCuts({Cut_HLTpassed1, Cut_AnyLLP_HLT_noECAL});
	overlay_jet.SetLegendPosition( 0.5, 0.9, 0.88, 1.08 ); 
	overlay_jet.SetLegendNames({"L1", "m_{H}=350 GeV, HLT efficiency", "L1", "m_{H}=125 GeV, HLT efficiency"});
	overlay_jet.Plot("efficiency", "", {"0","0","0"}, true);

	// overlay L1 and HLT results
	for( auto key: filetag_keys_to_loop){

		// parse the string to determine input mass and lifetime
		string s = key;
		string delimiter = "_";
		vector<string> remove = {"LLP","ctau"};
		size_t pos = 0;
		vector<string> mass_lifetime = {"0","0","0"};

		// remove LLP, ctau
		for( auto pattern: remove) {
			std::string::size_type i = s.find(pattern);
			while (i != std::string::npos) {
				s.erase(i, pattern.length());
				i = s.find(pattern, i);
			}
		}
		//std::cout << s << std::endl;
		int type = 0;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			string token = s.substr(0, pos);
			std::cout << token << std::endl;
			mass_lifetime[type] = token;
			type += 1;
			s.erase(0, pos + delimiter.length());
		}
		mass_lifetime[type] = s;
		std::cout << s << std::endl;

		for( int i = 0; i < jet_E.size(); i++ ){

			// with a jet pT cut, plot efficiency vs LLP displacement in R, Z, eta, travel time
			class MiniTuplePlotter eff_LLPdisplacement( filetags[key], path );
//			eff_LLPdisplacement.SetPlots({P_LLP0_DecayR, P_LLP0_DecayZ, P_LLP0_Eta, P_LLP0_TravelTime});
			eff_LLPdisplacement.SetPlots({P_perJet_MatchedLLP_DecayR, P_perJet_MatchedLLP_DecayZ, P_perJet_MatchedLLP_Eta, P_perJet_MatchedLLP_TravelTime});
//			eff_LLPdisplacement.SetTreeName( "NoSel" );
			eff_LLPdisplacement.SetTreeName( "PerJet_LLPmatched" );
			eff_LLPdisplacement.SetOutputFileTag("HLT_v3_MC_"+key+"_jetE"+jet_E[i]);
			eff_LLPdisplacement.SetOutputDirectory("HLT_Efficiencies_31Jan");
			eff_LLPdisplacement.plot_norm 			= false;
			eff_LLPdisplacement.plot_log_ratio   	= true;
			eff_LLPdisplacement.SetLegendPosition( 0.15, 0.9, 0.43, 1.08 );
//			eff_LLPdisplacement.SetSelectiveCuts("MC", Form("eventHT > %s && ( (jet0_isMatchedTo == 0 && jet0_Pt >= %s) || (jet1_isMatchedTo == 0 && jet1_Pt >= %s) || (jet2_isMatchedTo == 0 && jet2_Pt >= %s) ) ", event_HT[i].c_str(), jet_E[i].c_str(), jet_E[i].c_str(), jet_E[i].c_str() ) ); // make sure that LLP 0 is matched to jet, and cut on the jet pT
			eff_LLPdisplacement.SetSelectiveCuts("MC", Form("eventHT > %s && perJet_Pt >= %s && perJet_MatchedLLP_DecayR < 300", event_HT[i].c_str(), jet_E[i].c_str() ) ); // cut on the jet pT
			std::cout << key << std::endl;
			if (HLT_OR) {
				std::cout << "comparing to general displaced jet paths" << std::endl;
				eff_LLPdisplacement.SetComparisonCuts({Cut_None, Cut_HLTpassed1, Cut_Run2_HLTs, Cut_DisplacedJetHLT_OR});
				eff_LLPdisplacement.colors = { kBlack, kAzure+7, kGray+1, kViolet+4 };
				if (acceptance) eff_LLPdisplacement.SetLegendNames({"No cuts", "L1 acceptance", "HLT baseline acceptance", "HLT combined acceptance"});
				else eff_LLPdisplacement.SetLegendNames({"No cuts", "L1 efficiency", "HLT baseline efficiency", "HLT combined efficiency"});
				eff_LLPdisplacement.SetLegendPosition( 0.15, 0.85, 0.5, 1.08 );
			}
			else {
				// eff_LLPdisplacement.SetComparisonCuts({Cut_None, Cut_HLTpassed1, Cut_AnyLLP_HLT_noECAL}); 
				eff_LLPdisplacement.SetComparisonCuts({Cut_None, Cut_HLTpassed1, Cut_AnyLLP_HLT}); 
				if (HLT_only) eff_LLPdisplacement.SetComparisonCuts({Cut_None, Cut_AnyLLP_HLT}); 
				eff_LLPdisplacement.colors = { kBlack, kAzure+7, kViolet+4 };
				if (acceptance) {
					eff_LLPdisplacement.SetLegendNames({"No cuts", "L1 acceptance", "HLT acceptance"});
					eff_LLPdisplacement.colors = { kBlack, kCyan+2, kMagenta-2 };
					if (HLT_only) {
						eff_LLPdisplacement.SetLegendNames({"No cuts", "HLT acceptance"});
						eff_LLPdisplacement.colors = { kBlack, kMagenta-2 };
					}
				}
				else eff_LLPdisplacement.SetLegendNames({"No cuts", "L1 efficiency", "HLT efficiency"});
			}
			if (key == "LLP125_50_ctau3m") eff_LLPdisplacement.SetVariableBins( {0,50,100,150,175,190,200,210,220,230,240,250,260,270,280,290,300} );
			// eff_LLPdisplacement.colors = { kWhite, kOrange, kGreen+2 }; // to just see trigger efficiency 
			// eff_LLPdisplacement.Plot("ratio");
			if (acceptance) eff_LLPdisplacement.Plot("acceptance", "", mass_lifetime);
			else eff_LLPdisplacement.Plot("efficiency", "", mass_lifetime);
			eff_LLPdisplacement.ClearFileTrees(); 														// reset, and cut on each HLT group
			eff_LLPdisplacement.SetOutputFileTag("HLT_v3_MC_"+key+"_jetE"+jet_E[i]+"_HLTsplit");
			eff_LLPdisplacement.colors = { kBlack, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed  };
			eff_LLPdisplacement.SetComparisonCuts({Cut_HLTpassed1, Cut_AnyLLP_HLT, Cut_HLTpassed2, Cut_HLTpassed9, Cut_HLTpassed5, Cut_HLTpassed11}); 
			eff_LLPdisplacement.SetLegendNames({"Monitoring HLT passed", "LLP HLT passed", "Displaced dijet, <=1 prompt track", "Displaced dijet, <=2 prompt tracks", "Displaced dijet, 1 displaced track", "Delayed jet, ECAL jet timing"});
			eff_LLPdisplacement.Plot("ratio");
		}

		// per event now, cut on LLP displacement (Cut_LLPinHCAL34_AnyJet), plot efficiency vs event HTT
		class MiniTuplePlotter plotter_HT( filetags[key], path );
		plotter_HT.SetPlots({P_eventHT});
		plotter_HT.SetTreeName( "NoSel" ); 
		plotter_HT.SetOutputFileTag("HLT_v3_MC_"+key);
		plotter_HT.SetOutputDirectory("HLT_Efficiencies_31Jan");
		plotter_HT.plot_norm 			= false;
		plotter_HT.plot_log_ratio   	= true;
		plotter_HT.SetVariableBins( {0, 20, 40, 60, 80, 100, 120, 140, 160, 190, 220, 250, 290, 330, 390, 490, 610, 760, 950, 1200} );
		plotter_HT.SetLegendPosition( 0.6, 0.85, 0.88, 1.03 );
		plotter_HT.SetSelectiveCuts("MC", Cut_LLPinHCAL34_AnyJet); // jet is matched to LLP in HCAL 34 and jet pT > 100 GeV
		if (HLT_OR) {
			std::cout << "comparing to general displaced jet paths" << std::endl;
			plotter_HT.SetComparisonCuts({Cut_None, Cut_HLTpassed1, Cut_Run2_HLTs, Cut_DisplacedJetHLT_OR});
			plotter_HT.colors = { kBlack, kAzure+7, kGray+1, kViolet+4 };
			plotter_HT.SetLegendNames({"No cuts", "L1 acceptance", "HLT baseline acceptance", "HLT combined acceptance"});
			plotter_HT.SetLegendPosition( 0.55, 0.8, 0.88, 1.03 );
			if (key == "LLP350_80_ctau500mm") plotter_HT.SetLegendPosition( 0.55, 0.55, 0.88, 0.78 ); 
		}
		else {
			plotter_HT.SetComparisonCuts({Cut_None, Cut_HLTpassed1, Cut_AnyLLP_HLT}); 
			plotter_HT.colors = { kBlack, kAzure+7, kViolet+4 };
			if (acceptance) {
				plotter_HT.SetLegendNames({"No cuts", "L1 acceptance", "HLT acceptance"});
				plotter_HT.colors = { kBlack, kCyan+2, kMagenta-2 };
			}
			else plotter_HT.SetLegendNames({"No cuts", "L1 efficiency", "HLT efficiency"});
		}
		if (acceptance) plotter_HT.Plot("acceptance", "", mass_lifetime); // now need to pass other arguments! 
		else plotter_HT.Plot("efficiency", "", mass_lifetime); // now need to pass other arguments! 
		plotter_HT.ClearFileTrees(); 																// reset, and cut on each HLT group.
		plotter_HT.SetOutputFileTag("HLT_v3_MC_"+key+"_HLTsplit");
		plotter_HT.colors = { kBlack, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed  };
		plotter_HT.SetComparisonCuts({Cut_HLTpassed1, Cut_AnyLLP_HLT, Cut_HLTpassed2, Cut_HLTpassed9, Cut_HLTpassed5, Cut_HLTpassed11}); 
		plotter_HT.SetLegendNames({"Monitoring HLT passed", "LLP HLT passed", "Displaced dijet, <=1 prompt track", "Displaced dijet, <=2 prompt tracks", "Displaced dijet, 1 displaced track", "Delayed jet, ECAL jet timing"});
		plotter_HT.Plot("ratio");

		for( int i = 0; i < variable_bins.size(); i++ ) {
			// cut on LLP displacement, plot efficiency vs jet pT, E, eta, phi, event HTT
			class MiniTuplePlotter plotter_HLTeffMC( filetags[key], path );
	//		plotter_HLTeffMC.SetPlots({P_jet0_E, P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_eventHT});
	//		plotter_HLTeffMC.SetPlots({P_perJet_E, P_perJet_Pt, P_perJet_Eta, P_perJet_Phi, P_eventHT});
			plotter_HLTeffMC.SetPlots(plot_type[i]);
	//		plotter_HLTeffMC.SetTreeName( "NoSel" ); 
			plotter_HLTeffMC.SetTreeName( "PerJet_LLPmatched" );
			plotter_HLTeffMC.SetOutputFileTag("HLT_v3_MC_"+key);
			plotter_HLTeffMC.SetOutputDirectory("HLT_Efficiencies_31Jan");
			plotter_HLTeffMC.plot_norm 			= false;
			plotter_HLTeffMC.plot_log_ratio   	= true;
			plotter_HLTeffMC.SetLegendPosition( 0.6, 0.75, 0.88, 0.88 ); // works when efficiency legend uses -0.45 for x (upper left) or -0.3 for y (middle right)
			plotter_HLTeffMC.SetVariableBins( variable_bins[i] );
			if (key == "LLP125_50_ctau3m" && i==0 ) plotter_HLTeffMC.SetVariableBins( {40, 46, 52, 58, 64, 70, 76, 82, 88, 94, 100, 110, 124, 140, 160, 185, 215, 250} );
	//		plotter_HLTeffMC.SetSelectiveCuts("MC", Cut_LLPinHCAL123_Jet0);
	//		plotter_HLTeffMC.SetSelectiveCuts("MC", Cut_matchedLLPinHCAL34);		// region for LLP decay, and require LLP is matched to jet 0
			plotter_HLTeffMC.SetSelectiveCuts("MC", Cut_matchedLLPinHCAL34_eventHT250);		// region for LLP decay, and require LLP is matched to jet 0, and event HT 250
			if (key == "LLP125_50_ctau3m_____") {
				std::cout << "expect to cut on HT 350 " << std::endl;
				plotter_HLTeffMC.SetComparisonCuts({Cut_None, Cut_HLTpassed1, Cut_AnyLLP_HLT_noECAL, Cut_HLTpassedHT350}); 
				plotter_HLTeffMC.colors = { kBlack, kAzure+7, kViolet+4, kGray };
				if (acceptance) plotter_HLTeffMC.SetLegendNames({"No cuts", "L1 acceptance", "HLT acceptance", "HT 350 acceptance"});
				else plotter_HLTeffMC.SetLegendNames({"No cuts", "L1 efficiency", "HLT efficiency", "HT 350 efficiency"});
			}
			else if (HLT_OR) {
				std::cout << "comparing to general displaced jet paths" << std::endl;
				plotter_HLTeffMC.SetComparisonCuts({Cut_None, Cut_HLTpassed1, Cut_Run2_HLTs, Cut_DisplacedJetHLT_OR});
				plotter_HLTeffMC.colors = { kBlack, kAzure+7, kGray+1, kViolet+4 };
				if (acceptance) plotter_HLTeffMC.SetLegendNames({"No cuts", "L1 acceptance", "HLT baseline acceptance", "HLT combined acceptance"});
				else plotter_HLTeffMC.SetLegendNames({"No cuts", "L1 efficiency", "HLT baseline efficiency", "HLT combined efficiency"});
				plotter_HLTeffMC.SetLegendPosition( 0.55, 0.8, 0.88, 1.0 ); 
				if (key == "LLP350_80_ctau500mm") plotter_HLTeffMC.SetLegendPosition( 0.55, 0.9, 0.88, 1.1 ); 
			}
			else {
				plotter_HLTeffMC.SetLegendPosition( 0.6, 0.85, 0.88, 1.03 );
				if (i==0) plotter_HLTeffMC.SetVariableBins( {40, 55, 65, 75, 85, 95, 105, 115, 125, 140, 160, 185, 215, 250} );
				plotter_HLTeffMC.SetComparisonCuts({Cut_None, Cut_HLTpassed1, Cut_AnyLLP_HLT}); 
				plotter_HLTeffMC.colors = { kBlack, kAzure+7, kViolet+4 };
				if (acceptance) {
					plotter_HLTeffMC.colors = { kBlack, kCyan+2, kMagenta-2 };
					plotter_HLTeffMC.SetLegendNames({"No cuts", "L1 acceptance", "HLT acceptance"});
				}
				else plotter_HLTeffMC.SetLegendNames({"No cuts", "L1 efficiency", "HLT efficiency"});
			}
			// plotter_HLTeffMC.Plot("ratio");
			if (acceptance) plotter_HLTeffMC.Plot("acceptance", "", mass_lifetime); // now need to pass other arguments! 
			else plotter_HLTeffMC.Plot("efficiency", "", mass_lifetime); 
			plotter_HLTeffMC.ClearFileTrees(); 																// reset, and cut on each HLT group.
			plotter_HLTeffMC.SetOutputFileTag("HLT_v3_MC_"+key+"_HLTsplit");
			plotter_HLTeffMC.colors = { kBlack, kOrange, kGreen+2, kAzure+7, kBlue-4, kViolet+4, kMagenta-7, kRed  };
			plotter_HLTeffMC.SetComparisonCuts({Cut_HLTpassed1, Cut_AnyLLP_HLT, Cut_HLTpassed2, Cut_HLTpassed9, Cut_HLTpassed5, Cut_HLTpassed11}); 
			plotter_HLTeffMC.SetLegendNames({"Monitoring HLT passed", "LLP HLT passed", "Displaced dijet, <=1 prompt track", "Displaced dijet, <=2 prompt tracks", "Displaced dijet, 1 displaced track", "Delayed jet, ECAL jet timing"});
			plotter_HLTeffMC.Plot("ratio");
		}
	}

	if (September) {
		// September 2023 Studies

		// ----- Jet kinematics split by HLT paths passed -----//

		cout<<endl;
		cout<<" ---------- HLT Study 1: Jet kinematics split by HLT paths passed ---------- "<<endl;
		cout<<endl;

		class MiniTuplePlotter plotter_HLTeff( filetags_LLP125, path_v3 );
		plotter_HLTeff.SetPlots({P_jet0_E, P_jet0_Pt, P_jet1_E, P_jet1_Pt, P_jet0_Eta, P_jet0_Phi}); // These "P_" variables are PlotParams structs defined in PlotParams.h
		plotter_HLTeff.SetTreeName( "NoSel" );	// TreeName
		plotter_HLTeff.SetOutputFileTag("HLT_v1.2_MC"); 							// Your own special name :)

		plotter_HLTeff.plot_norm 		  = false; 	// Default = true
		plotter_HLTeff.plot_log_ratio   = true; 	// Default = false. Make bottom panel log scale
		plotter_HLTeff.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );					// Manual Legend location
		
		plotter_HLTeff.SetComparisonCuts({Cut_None, Cut_HLTpassed2, Cut_HLTpassed5, Cut_HLTpassed9, Cut_HLTpassed11}); 
		plotter_HLTeff.Plot("ratio");				// This is what does stuff -- arguments: None/"", "ratio", "sig"/"ssqrtb"


		// ----- Jet kinematics split by HLT paths passed -- require that LLP decays in/before HCAL -----//

		cout<<endl;
		cout<<" ---------- HLT Study 2: Jet kinematics split by HLT paths passed -- require that LLP decays in/before HCAL ---------- "<<endl;
		cout<<endl;

		for (it = LLP_Cuts.begin(); it != LLP_Cuts.end(); it++) {

			class MiniTuplePlotter plotter_HLTeff2( filetags_LLP125, path_v3 );
			plotter_HLTeff2.SetPlots({P_met_Pt}); 
			plotter_HLTeff2.SetTreeName( "NoSel" );
			plotter_HLTeff2.SetOutputFileTag("HLT_v1.2_MC_" + it->second);
			plotter_HLTeff2.plot_norm 		  = false;
			plotter_HLTeff2.plot_log_ratio   = true; 
			plotter_HLTeff2.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			plotter_HLTeff2.SetComparisonCuts({Cut_None, Cut_HLTpassed2, Cut_HLTpassed5, Cut_HLTpassed9, Cut_HLTpassed11}); 
			plotter_HLTeff2.SetSelectiveCuts("MC", it->first); // region for LLP decay
			plotter_HLTeff2.Selection(it->second); // print which selection is made on the plot
			plotter_HLTeff2.Plot("ratio");
		}
		
		for (it_double = JetMatchedToLLP.begin(); it_double != JetMatchedToLLP.end(); it_double++) {
			// do jet plots with gen matching requirement 
			class MiniTuplePlotter plotter_HLTeff3( filetags_LLP125, path_v3 );
			plotter_HLTeff3.SetPlots({P_jet0_Pt}); 
			plotter_HLTeff3.SetTreeName( "NoSel" );
			plotter_HLTeff3.SetOutputFileTag("HLT_v1.2_MC_" + it_double->second);
			plotter_HLTeff3.plot_norm 		  = false;
			plotter_HLTeff3.plot_log_ratio   = true; 
			plotter_HLTeff3.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			plotter_HLTeff3.SetComparisonCuts({Cut_None, Cut_HLTpassed2, Cut_HLTpassed5, Cut_HLTpassed9, Cut_HLTpassed11}); 
			plotter_HLTeff3.SetSelectiveCuts("MC", get<0>(it_double->first));// jet matched + region for LLP decay
			plotter_HLTeff3.Selection(it_double->second); // print which selection is made on the plot
			plotter_HLTeff3.Plot("ratio");
			
			class MiniTuplePlotter plotter_HLTeff4( filetags_LLP125, path_v3 );
			plotter_HLTeff4.SetPlots({P_jet1_Pt}); 
			plotter_HLTeff4.SetTreeName( "NoSel" );
			plotter_HLTeff4.SetOutputFileTag("HLT_v1.2_MC_" + it_double->second);
			plotter_HLTeff4.plot_norm 		  = false;
			plotter_HLTeff4.plot_log_ratio   = true; 
			plotter_HLTeff4.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			plotter_HLTeff4.SetComparisonCuts({Cut_None, Cut_HLTpassed2, Cut_HLTpassed5, Cut_HLTpassed9, Cut_HLTpassed11}); 
			plotter_HLTeff4.SetSelectiveCuts("MC", get<1>(it_double->first)); // jet matched + region for LLP decay
			plotter_HLTeff4.Selection(it_double->second); // print which selection is made on the plot
			plotter_HLTeff4.Plot("ratio");
		}

		// ----- LLP kinematics -- HLT Efficiency split by LLP displacement and energy -----//

		cout<<endl;
		cout<<" ---------- HLT Study 3: LLP kinematics -- HLT efficiency split by LLP displacement and energy ---------- "<<endl;
		cout<<endl;

		for (it = LLP0_Cuts.begin(); it != LLP0_Cuts.end(); it++) {

			class MiniTuplePlotter plotter_LLP( filetags_LLP125, path_v3 );
			plotter_LLP.SetPlots({P_LLP0_Pt, P_LLP0_E}); 
			plotter_LLP.SetTreeName( "NoSel" );	
			plotter_LLP.SetOutputFileTag("HLT_v1.2_MC_" + it->second); 							

			plotter_LLP.plot_norm 		  = false; 	
			plotter_LLP.plot_log_ratio   = true; 	
			plotter_LLP.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );

			plotter_LLP.SetComparisonCuts({Cut_None, Cut_HLTpassed2, Cut_HLTpassed5, Cut_HLTpassed9, Cut_HLTpassed11}); 
			plotter_LLP.SetSelectiveCuts("MC", it->first);
			plotter_LLP.Selection(it->second); // print which selection is made on the plot
			plotter_LLP.Plot("ratio");	
		}

		class MiniTuplePlotter plotter_disp( filetags_LLP125, path_v3 );
		plotter_disp.SetPlots({P_LLP0_DecayR}); 
		plotter_disp.SetTreeName( "NoSel" );	
		plotter_disp.SetOutputFileTag("HLT_v1.2_MC_LLP0inHCAL"); 
		plotter_disp.plot_norm 		  = false; 	
		plotter_disp.plot_log_ratio   = true; 	
		plotter_disp.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_disp.SetComparisonCuts({Cut_None, Cut_HLTpassed2, Cut_HLTpassed5, Cut_HLTpassed9, Cut_HLTpassed11}); 
		plotter_disp.SetSelectiveCuts("MC", Cut_LLP0inHCAL);
		plotter_disp.Plot("ratio");	


		// ----- Jet Characteristics -- If HLT passed, what are distributions in leading, subleading jets? -----//

		cout<<endl;
		cout<<" ----- HLT Study 4: Jet Characteristics -- If HLT passed, what are distributions in leading, subleading jets? ----- "<<endl;
		cout<<endl;

		// set up 2D plots, and associated cuts. Save in a std::vector of the Hist1_Hist2_Cut structure defined in PlotParams so these can be iterated over
		std::vector<Hist1_Hist2_Cut> Jet1_Jet2;
		Jet1_Jet2.push_back(Hist1_Hist2_Cut(P_jet0_EtaSpread, P_jet0_PhiSpread, "jet0_isMatchedTo >= 0"));
		Jet1_Jet2.push_back(Hist1_Hist2_Cut(P_jet0_EtaSpread_energy, P_jet0_PhiSpread_energy, "jet0_isMatchedTo >= 0"));
		Jet1_Jet2.push_back(Hist1_Hist2_Cut(P_jet1_EtaSpread, P_jet1_PhiSpread, "jet1_isMatchedTo >= 0"));
		Jet1_Jet2.push_back(Hist1_Hist2_Cut(P_jet1_EtaSpread_energy, P_jet1_PhiSpread_energy, "jet1_isMatchedTo >= 0"));
		Jet1_Jet2.push_back(Hist1_Hist2_Cut(P_jet0_Pt, P_jet1_Pt, "jet0_isMatchedTo >= 0 && jet1_isMatchedTo >= 0"));

		for (auto group = Jet1_Jet2.begin(); group != Jet1_Jet2.end(); group++) { // iterate over the vector defined above
			class MiniTuplePlotter jet_dist( filetags_LLP125, path_v3 );
			jet_dist.SetTreeName( "NoSel" );	
			jet_dist.SetOutputFileTag("HLT_v1.2_MC_jetDist"); 							
			jet_dist.plot_norm 		  = false; 	
			jet_dist.plot_log_ratio   = true; 	
			jet_dist.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			jet_dist.SetComparisonCuts({Cut_None, Cut_HLTpassed2}); 
			jet_dist.SetSelectiveCuts("MC", group->cut);
			cout << group->cut << endl;
			jet_dist.Plot2D(group->Params1, group->Params2);
		}

		// add jet energy distribution plots too (but likely 1D plots instead of 2D)

		// ----- LLP Characteristics -- If HLT passed, what are distributions in leading, subleading LLP? -----//

		cout<<endl;
		cout<<" ----- HLT Study 5: LLP Characteristics -- If HLT passed, what are distributions in leading, subleading LLP? ----- "<<endl;
		cout<<endl;

		int jet_pt[4] = {0, 40, 70, 100}; // jet pT categories
		for (int i = 0; i < 4; i++) {
			class MiniTuplePlotter LLP_dist( filetags_LLP125, path_v3 );
			LLP_dist.SetTreeName( "NoSel" );	
			LLP_dist.SetOutputFileTag("HLT_v1.2_MC_LLPdist_jetPT" + to_string(jet_pt[i])); 							
			LLP_dist.plot_norm 		  = false; 	
			LLP_dist.plot_log_ratio   = true; 	
			LLP_dist.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			LLP_dist.SetComparisonCuts({Cut_None, Cut_HLTpassed2}); 
			// require jet is matched to a LLP, and then cut on jet energy
			TString cut = Form("(jet0_Pt > %d && jet0_isMatchedTo >= 0)", jet_pt[i]);
			TCut jet_cut = cut.Data();
			LLP_dist.SetSelectiveCuts("MC", jet_cut);  
			LLP_dist.Selection(Form("jet0_Pt > %d", jet_pt[i])); // not written on plot yet...
			LLP_dist.Plot2D(P_LLP0_DecayR, P_LLP1_DecayR);	
			LLP_dist.Plot2D(P_LLP0_DecayZ, P_LLP1_DecayZ);	
		}
	}
	
}
