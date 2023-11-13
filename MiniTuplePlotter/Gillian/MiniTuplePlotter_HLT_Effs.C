#include "../MiniTuplePlotter.h"
#include "TString.h"

// Goals of this study: use the LLP MC sample, understand efficiency of each HLT passed, as a function of various kinematic (jet) variables
// Mostly worked on during September.
// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_HLT_Effs(){

	// List where minituples are stored
	string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.2/minituple_";
	vector<string> filetags_both = 	{ "v1.2_LLPskim_500k_2023_08_31", "v1.2_MCsignal_500k_2023_08_31" };
	vector<string> filetags_data = 	{ "v1.2_LLPskim_500k_2023_08_31" };
	vector<string> filetags_MC = 	{ "v1.2_MCsignal_500k_2023_08_31" };

	#include "../RegionCuts.h"

	// ----- Jet kinematics split by HLT paths passed -----//

	cout<<endl;
	cout<<" ---------- HLT Study 1: Jet kinematics split by HLT paths passed ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_HLTeff( filetags_MC, path );
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

		class MiniTuplePlotter plotter_HLTeff2( filetags_MC, path );
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
		class MiniTuplePlotter plotter_HLTeff3( filetags_MC, path );
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
		
		class MiniTuplePlotter plotter_HLTeff4( filetags_MC, path );
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

		class MiniTuplePlotter plotter_LLP( filetags_MC, path );
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

	class MiniTuplePlotter plotter_disp( filetags_MC, path );
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
		class MiniTuplePlotter jet_dist( filetags_MC, path );
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
		class MiniTuplePlotter LLP_dist( filetags_MC, path );
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
