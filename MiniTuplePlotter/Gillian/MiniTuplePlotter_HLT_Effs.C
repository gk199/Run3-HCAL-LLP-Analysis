#include "../MiniTuplePlotter.h"
#include "TString.h"

// Goals of this study: use the LLP MC sample, understand efficiency of each HLT passed, as a function of various kinematic (jet) variables
// Mostly worked on during September.
// Returning to in December 2023 to compare mH 125 and 350 GeV samples
// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_HLT_Effs(){

	// List where minituples are stored
	// string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.2/minituple_";
	// vector<string> filetags_both = 	{ "v1.2_LLPskim_500k_2023_08_31", "v1.2_MCsignal_500k_2023_08_31" };
	// vector<string> filetags_data = 	{ "v1.2_LLPskim_500k_2023_08_31" };
	// vector<string> filetags_MC = 	{ "v1.2_MCsignal_500k_2023_08_31" };

	string path_v3 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.0/minituple_";
	string path = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.6/minituple_";

	map<string,vector<string>> filetags;
	// filetags["LLP125"]	= { "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2023_11_23"};
	// filetags["LLP350"]	= { "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2023_11_29"};
	filetags["LLP125_mX15"]	= { "v3.6_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_03_02_TEST"};
	filetags["LLP350_mX80"]	= { "v3.6_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_03_02_TEST"};
	filetags["LLP125"]		= { "v3.6_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_03_02_batch2"};
	filetags["LLP250"]		= { "v3.6_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_03_02_batch2"};
	filetags["LLP350"]		= { "v3.6_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_03_02_batch2"};

	vector<string> filetags_LLP125	= { "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2023_11_23"};
	vector<string> filetags_LLP350	= { "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2023_11_29"};
	vector<string> filetags_LLP_all	= { "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2023_11_23", "v3.0_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2023_11_29"};

	#include "../RegionCuts.h"

	bool September = false;

	// December 2023 studies
	cout<<endl;
	cout<<" ---------- HLT Efficiency Study 1: Jet kinematics split by HLT paths passed ---------- "<<endl;
	cout<<endl;

	// to do -- make a new version that is jet based!

	vector<string> filetag_keys_to_loop = {"LLP125", "LLP250", "LLP350", "LLP125_mX15", "LLP350_mX80"};

	vector<string> jet_E = {"60", "100"};
	vector<string> event_HT = {"200", "170"};

	for( auto key: filetag_keys_to_loop){

		for( int i = 0; i < jet_E.size(); i++ ){

			// with a jet pT cut, plot efficiency vs LLP displacement in R, Z, eta, travel time
			class MiniTuplePlotter eff_LLPdisplacement( filetags[key], path );
			eff_LLPdisplacement.SetPlots({P_LLP0_DecayR, P_LLP0_DecayZ, P_LLP0_Eta, P_LLP0_TravelTime});
//			eff_LLPdisplacement.SetPlots({P_perJet_MatchedLLP_DecayR, P_perJet_MatchedLLP_DecayZ, P_perJet_MatchedLLP_Eta, P_perJet_MatchedLLP_TravelTime});
//			eff_LLPdisplacement.SetPlots({P_perJet_MatchedLLP_DecayR, P_perJet_MatchedLLP_Eta});
			eff_LLPdisplacement.SetTreeName( "NoSel" );
//			eff_LLPdisplacement.SetTreeName( "PerJet_LLPmatched" );
			eff_LLPdisplacement.SetOutputFileTag("HLT_v3_MC_"+key+"_jetE"+jet_E[i]);
			eff_LLPdisplacement.SetOutputDirectory("HLT_Efficiencies");
			eff_LLPdisplacement.plot_norm 			= false;
			eff_LLPdisplacement.plot_log_ratio   	= true;
			eff_LLPdisplacement.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
			eff_LLPdisplacement.SetSelectiveCuts("MC", Form("eventHT > %s && ( (jet0_isMatchedTo == 0 && jet0_Pt >= %s) || (jet1_isMatchedTo == 0 && jet1_Pt >= %s) || (jet2_isMatchedTo == 0 && jet2_Pt >= %s) ) ", event_HT[i].c_str(), jet_E[i].c_str(), jet_E[i].c_str(), jet_E[i].c_str() ) ); // make sure that LLP 0 is matched to jet, and cut on the jet pT
//			eff_LLPdisplacement.SetSelectiveCuts("MC", Form("eventHT > %s && perJet_Pt >= %s ", event_HT[i].c_str(), jet_E[i].c_str() ) ); // cut on the jet pT
			eff_LLPdisplacement.SetComparisonCuts({Cut_None, Cut_HLTpassed1, Cut_AnyLLP_HLT}); 
			eff_LLPdisplacement.SetLegendNames({"No cuts", "Monitoring HLT passed", "LLP HLT passed"});
			eff_LLPdisplacement.Plot("ratio");
			eff_LLPdisplacement.ClearFileTrees(); 														// reset, and cut on each HLT group
			eff_LLPdisplacement.SetOutputFileTag("HLT_v3_MC_"+key+"_jetE"+jet_E[i]+"_HLTsplit");
			eff_LLPdisplacement.SetComparisonCuts({Cut_HLTpassed1, Cut_AnyLLP_HLT, Cut_HLTpassed2, Cut_HLTpassed9, Cut_HLTpassed5, Cut_HLTpassed11}); 
			eff_LLPdisplacement.SetLegendNames({"Monitoring HLT passed", "LLP HLT passed", "Displaced dijet, <=1 prompt track", "Displaced dijet, <=2 prompt tracks", "Displaced dijet, 1 displaced track", "Delayed jet, ECAL jet timing"});
			eff_LLPdisplacement.Plot("ratio");
		}

		// cut on LLP displacement, plot efficiency vs jet pT, E, eta, phi, event HTT
		class MiniTuplePlotter plotter_HLTeffMC( filetags[key], path );
		plotter_HLTeffMC.SetPlots({P_jet0_E, P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, P_eventHT});
//		plotter_HLTeffMC.SetPlots({P_perJet_E, P_perJet_Pt, P_perJet_Eta, P_perJet_Phi, P_eventHT});
		plotter_HLTeffMC.SetTreeName( "NoSel" ); // "PerJet_LLPmatched"
		plotter_HLTeffMC.SetOutputFileTag("HLT_v3_MC_"+key);
		plotter_HLTeffMC.SetOutputDirectory("HLT_Efficiencies");
		plotter_HLTeffMC.plot_norm 			= false;
		plotter_HLTeffMC.plot_log_ratio   	= true;
		plotter_HLTeffMC.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
		plotter_HLTeffMC.SetSelectiveCuts("MC", Cut_LLPinHCAL123_Jet0); // Cut_matchedLLPinHCAL);		// region for LLP decay, and require LLP is matched to jet 0
		plotter_HLTeffMC.SetComparisonCuts({Cut_None, Cut_HLTpassed1, Cut_AnyLLP_HLT}); 
		plotter_HLTeffMC.SetLegendNames({"No cuts", "Monitoring HLT passed", "LLP HLT passed"});
		plotter_HLTeffMC.Plot("ratio");
		plotter_HLTeffMC.ClearFileTrees(); 																// reset, and cut on each HLT group.
		plotter_HLTeffMC.SetOutputFileTag("HLT_v3_MC_"+key+"_HLTsplit");
		plotter_HLTeffMC.SetComparisonCuts({Cut_HLTpassed1, Cut_AnyLLP_HLT, Cut_HLTpassed2, Cut_HLTpassed9, Cut_HLTpassed5, Cut_HLTpassed11}); 
		plotter_HLTeffMC.SetLegendNames({"Monitoring HLT passed", "LLP HLT passed", "Displaced dijet, <=1 prompt track", "Displaced dijet, <=2 prompt tracks", "Displaced dijet, 1 displaced track", "Delayed jet, ECAL jet timing"});
		plotter_HLTeffMC.Plot("ratio");

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
