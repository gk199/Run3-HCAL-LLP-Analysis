#include "MiniTuplePlotter.h"
#include "TString.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_CR_SR(){


	// List where minituples are stored
	string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.2/minituple_";
	vector<string> filetags_both = 	{ "v1.2_LLPskim_500k_2023_08_31", "v1.2_MCsignal_500k_2023_08_31" };
	vector<string> filetags_data = 	{ "v1.2_LLPskim_500k_2023_08_31" };
	vector<string> filetags_MC = 	{ "v1.2_MCsignal_500k_2023_08_31" };
	vector<string> filetags_MC_CR_SR = { "v1.2_CR_MC_500k_2023_08_31copy", "v1.2_MCsignal_500k_2023_08_31" };

	#include "RegionCuts.h"

	// ----- Jet kinematics in control and signal regions -----//

	cout<<endl;
	cout<<" ---------- CR / SR Study 1: Jet kinematics ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_JetVars( filetags_MC, path );
	plotter_JetVars.SetPlots({P_jet0_E, P_jet0_Pt, P_jet0_Eta, P_jet0_Phi, 
								P_jet0_Track0Pt, P_jet0_Track0dzToPV, P_jet0_Track0dxyToBS, P_jet0_Track0dzOverErr, P_jet0_Track0dxyOverErr, 
								P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3, P_jet0_EnergyFrac_Depth4 } );
	plotter_JetVars.SetTreeName( "NoSel" );	
	plotter_JetVars.SetOutputFileTag("CR_SR_v1.2_MC"); 	
	// plotter_JetVars.SetSelectiveCuts("MCsignal", Cut_LLPinHCAL_Jet0); // region for LLP decay // if have separate CR and SR filetags
	// plotter_JetVars.SetSelectiveCuts("CR_MC", Cut_LLPinCR_Jet0); // region for LLP decay
	plotter_JetVars.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinECAL_Jet0, Cut_LLPinHCAL12_Jet0, Cut_LLPinHCAL3_Jet0}); // , Cut_LLPinHCAL4_Jet0});
	plotter_JetVars.plot_log_ratio    = true; 	// Default = false. Make bottom panel log scale
	plotter_JetVars.SetLegendManual( 0.35, 0.65, 0.9, 0.9 );					// Manual Legend location
	plotter_JetVars.Plot("ratio");				// This is what does stuff -- arguments: None/"", "ratio", "sig"/"ssqrtb"

	class MiniTuplePlotter plotter_JetVars2D_etaphi( filetags_MC, path );
	plotter_JetVars2D_etaphi.SetTreeName( "NoSel" );	
	plotter_JetVars2D_etaphi.SetOutputFileTag("CR_SR_v1.2_MC"); 	
	plotter_JetVars2D_etaphi.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0});
	plotter_JetVars2D_etaphi.SetLegendManual( 0.35, 0.65, 0.9, 0.9 );				
    plotter_JetVars2D_etaphi.Plot2D(P_jet0_EtaSpread, P_jet0_PhiSpread);
    plotter_JetVars2D_etaphi.Plot2D(P_jet0_EtaSpread_energy, P_jet0_PhiSpread_energy);

	class MiniTuplePlotter plotter_JetVars2( filetags_MC, path );
	plotter_JetVars2.SetPlots({P_jet1_E, P_jet1_Pt}); 
	plotter_JetVars2.SetTreeName( "NoSel" );	
	plotter_JetVars2.SetOutputFileTag("CR_SR_v1.2_MC"); 	
	// plotter_JetVars2.SetSelectiveCuts("MCsignal", Cut_LLPinHCAL_Jet1); // region for LLP decay
	// plotter_JetVars2.SetSelectiveCuts("CR_MC", Cut_LLPinCR_Jet1); // region for LLP decay
	plotter_JetVars2.SetComparisonCuts({Cut_LLPinCR_Jet1, Cut_LLPinHCAL_Jet1});
	plotter_JetVars2.plot_log_ratio    = true;
	plotter_JetVars2.SetLegendManual( 0.35, 0.65, 0.9, 0.9 );
	plotter_JetVars2.Plot("ratio");
    plotter_JetVars2.Plot2D(P_jet1_EtaSpread, P_jet1_PhiSpread);
    plotter_JetVars2.Plot2D(P_jet1_EtaSpread_energy, P_jet1_PhiSpread_energy);

}
