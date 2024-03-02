#include "../MiniTuplePlotter.h"
#include "TString.h"

// Goals of this study: familiarize with framework, set up studies, and overlay energy fractions in each HCAL depth. In later studies, most selections / cuts are done using "RegionCuts.h"
// Mostly worked on during August.
// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_Jet_EnergyFrac(){

	string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v1.2/minituple_";
	vector<string> filetags_example1 = { "v1.2_LLPskim_500k_2023_08_30", "v1.2_MCsignal_500k_2023_08_30" };

	// ----- Example 1 -----//
	// - Basic Booleans

	cout<<endl;
	cout<<" ---------- EXAMPLE 1 ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_example1( filetags_example1, path );
	plotter_example1.SetPlots({P_jet0_E, P_jet0_Pt, P_jet0_Eta, P_jet0_Phi}); // These "P_" variables are PlotParams structs defined in PlotParams.h
	plotter_example1.SetTreeName( "NoSel" );	// TreeName
	plotter_example1.SetOutputFileTag("v1.2"); 							// Your own special name :)
	plotter_example1.debug  		  = false; 	// Default = false
	plotter_example1.plot_norm 		  = false; 	// Default = true
	plotter_example1.plot_log  		  = false; 	// Default = true
	plotter_example1.plot_log_ratio   = false; 	// Default = false. Make bottom panel log scale
	plotter_example1.plot_log_x       = false; 	// Default = false. Plot log on the x axis
	plotter_example1.stamp_counts 	  = true;	// Default = false. Print event count and integral in legend
	plotter_example1.plot_grid 	  	  = true;	// Default = true. Draw grid lines on plot
	plotter_example1.run_fit   		  = false;	// Default = false. Fit core & tail
	plotter_example1.plot_cdf 		  = false;	// Default = false. Cumulative distribution function 
	plotter_example1.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
	plotter_example1.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
	plotter_example1.Plot("ratio");				// This is what does stuff -- arguments: None/"", "ratio", "sig"/"ssqrtb"

	// ----- Example 2 -----//
	// - Adding Plots
	// - Setting cuts/selections
	// - Setting output file tag
	// - Aesthetics: Legend Location, NBins, colors, linestyles

	cout<<endl;
	cout<<" ---------- EXAMPLE 2 ---------- "<<endl;
	cout<<endl;
	
	vector<string> filetags_example2 = filetags_example1;

	float radius_preECAL[2]  = {0, 129};
    float radius_ECAL[2]     = {129, 177}; // 48cm of ECAL
    float radius_depth1[2]   = {177, 190.2};
    float radius_depth2[2]   = {190.2, 214.2};
    float radius_depth3[2]   = {214.2, 244.8};
    float radius_depth4[2]   = {244.8, 295};
    float radius_all[2]      = {0,9999};
    float radius_inHCAL[2]   = {177, 295};
    float radius_depth12[2]  = {177, 214.2};
    float radius_depth34[2]  = {214.2, 295};

	int z_pos = 425;
	float HBeta = 1.4;
	// depth 1 and 2 have a z position < 388, dpeth 3 and 4 have z position < 425cm

	TString preECAL = Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) < %f && jet0_isMatchedTo == 0)", 		radius_preECAL[0], radius_preECAL[1], HBeta);
	preECAL 		+= Form(" || (LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) < %f && jet0_isMatchedTo == 1)", 	radius_preECAL[0], radius_preECAL[1], HBeta);
	preECAL 		+= Form(" || (LLP0_Eta < -9999 && LLP0_Phi < -9999 && LLP0_DecayR < -9999 && jet0_isMatchedTo == -1)"); // hack for now to let data pass the cuts, want to improve later
	TString ECAL 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) < %f && jet0_isMatchedTo == 0)", 		radius_ECAL[0], radius_ECAL[1], HBeta);
	ECAL 			+= Form(" || (LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) < %f && jet0_isMatchedTo == 1)", 	radius_ECAL[0], radius_ECAL[1], HBeta);
	TString depth12 = Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) < %f && jet0_isMatchedTo == 0)", 		radius_depth12[0], radius_depth12[1], HBeta);
	depth12 		+= Form(" || (LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) < %f && jet0_isMatchedTo == 1)", 	radius_depth12[0], radius_depth12[1], HBeta);
	TString depth34 = Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) < %f && jet0_isMatchedTo == 0)", 		radius_depth34[0], radius_depth34[1], HBeta);
	depth34 		+= Form(" || (LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) < %f && jet0_isMatchedTo == 1)", 	radius_depth34[0], radius_depth34[1], HBeta);
	TString depth1	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) < %f && jet0_isMatchedTo == 0)", 		radius_depth1[0], radius_depth1[1], HBeta);
	depth1 			+= Form(" || (LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) < %f && jet0_isMatchedTo == 1)", 		radius_depth1[0], radius_depth1[1], HBeta);
	TString depth2	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) < %f && jet0_isMatchedTo == 0)", 		radius_depth2[0], radius_depth2[1], HBeta);
	depth2 			+= Form(" || (LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) < %f && jet0_isMatchedTo == 1)", 		radius_depth2[0], radius_depth2[1], HBeta);
	TString depth3	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) < %f && jet0_isMatchedTo == 0)", 		radius_depth3[0], radius_depth3[1], HBeta);
	depth3 			+= Form(" || (LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) < %f && jet0_isMatchedTo == 1)", 		radius_depth3[0], radius_depth3[1], HBeta);
	TString depth4	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) < %f && jet0_isMatchedTo == 0)", 		radius_depth4[0], radius_depth4[1], HBeta);
	depth4 			+= Form(" || (LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) < %f && jet0_isMatchedTo == 1)", 		radius_depth4[0], radius_depth4[1], HBeta);
	// jet0_isMatchedTo will be 0 or 1 depending which LLP is matched to 

	TCut Cut_Decay_preECAL 	= preECAL.Data();
	TCut Cut_Decay_ECAL 	= ECAL.Data();;
	TCut Cut_Decay_depth12 	= depth12.Data();
	TCut Cut_Decay_depth34 	= depth34.Data();
	TCut Cut_Decay_depth1 	= depth1.Data();
	TCut Cut_Decay_depth2 	= depth2.Data();
	TCut Cut_Decay_depth3 	= depth3.Data();
	TCut Cut_Decay_depth4 	= depth4.Data();

	class MiniTuplePlotter plotter_example2( filetags_example2, path );
	plotter_example2.debug = false; 
	plotter_example2.SetPlots({P_jet0_EnergyFrac_Depth1, P_jet0_EnergyFrac_Depth2, P_jet0_EnergyFrac_Depth3}); 
	plotter_example2.AddPlot( P_jet0_EnergyFrac_Depth4 );
	plotter_example2.SetTreeNames( {"NoSel", "NoSel"} ); 						// Multiple Tree Names -- number must match number of input files (1:1)
	plotter_example2.SetCuts("jet0_Pt > 40 && abs(jet0_Eta) < 1.5"); 			// Apply cuts to all events
	plotter_example2.SetComparisonCuts({Cut_Decay_preECAL, Cut_Decay_depth12}); // Apply cuts to overlay
	plotter_example2.SetSelectiveCuts("MC", "");			// Apply this only to filetag names that include the string "data"
	plotter_example2.SetOutputFileTag("v1.2"); 							// Your own special name :)
	plotter_example2.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );						// Manual Legend location
	plotter_example2.colors = {kOrange, kOrange, kGreen+2, kGreen+2};			// Your own colors (default kBlack + rainbow)
	plotter_example2.linestyle = {kDotted, kDashed, kDotted, kDashed}; 			// Your own linestyle (default kSolid)
	plotter_example2.NBins = 30; 									 			// Default = 100
	plotter_example2.Plot("ratio");									 			// Ratio panel

	// ----- Other Current Featues ----- //

	// - Fitting
	//		plotter_example.run_fit  = true;
	//		plotter_example.fit_type = "igf_sigma igf_mean"; 	// Options (you can combine them): "igf", "igf_sigma", "igf_mean", "tailfrac", "tail_spread", "exp_decay", "fixed_gaus";
	// - Plotting static histograms, like cutflows: 
	// 		plotter_example.PlotStaticHistogram("NoSel__jet0_energy");
	// - 2D Plotting:
	// 		plotter_example.Plot2D(jet0_energy, jet0_Pt); X vs. Y
	// - Variable Bins -- BUT These are the same for all plots in a given instance of calling Plot()
	// 		plotter_example.SetVariableBins( {0, 10, 20, 30, 50, 75, 100, 150, 200, 300, 500} );

	// ----- Other Potential Featues ----- //

	// - N-1 Plots ("sig"/"ssqrtb") -- could add
	// - 1D/2D Reweighting -- could add
	// - Plot extended cutflow -- cutflow + additional dynamic cuts

}

