#include "MiniTuplePlotter.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotterExample(){

	string path = "ExampleFiles/v0.1/minituple_";
	
	vector<string> filetags_example1 = { "test_1k", "test_5k" }; //, "test_10k" };

	// ----- Example 1 -----//
	// - Basic Booleans

	cout<<endl;
	cout<<" ---------- EXAMPLE 1 ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_example1( filetags_example1, path );

	plotter_example1.SetTreeName( "NoSel" );	// TreeName
	plotter_example1.SetPlots({P_jet0_energy, P_jet0_pt, P_jet0_Eta, P_jet0_Phi}); // These "P_" variables are PlotParams structs defined in PlotParams.h
	plotter_example1.SetOutputFileTag("mycoolplot"); 						// Your own special name :)
	plotter_example1.debug  		  = true; 	// Default = false
	plotter_example1.plot_norm 		  = true; 	// Default = true
	plotter_example1.plot_log  		  = false; 	// Default = true
	plotter_example1.plot_log_ratio   = false; 	// Default = false. Make bottom panel log scale
	plotter_example1.plot_log_x       = false; 	// Default = false. Plot log on the x axis
	plotter_example1.stamp_counts 	  = true;	// Default = false. Print event count and integral in legend
	plotter_example1.plot_grid 	  	  = true;	// Default = true. Draw grid lines on plot
	plotter_example1.run_fit   		  = true;	// Default = false. Fit core & tail
	plotter_example1.fit_type         = "exp_decay";
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
	TCut Cut_DecayTk 	= "LLP0_DecayR < 161.6 && abs(LLP0_DecayZ) < 300";
	TCut Cut_DecayHCAL 	= "LLP0_DecayR > 183.6 && LLP0_DecayR < 295 && abs(LLP0_DecayZ) < 376"; // Approx 

	class MiniTuplePlotter plotter_example2( filetags_example2, path );

	plotter_example2.SetPlots({ P_jet0_energyFrac_depth1, P_jet0_energyFrac_depth2, P_jet0_energyFrac_depth3}); 
	plotter_example2.AddPlot( P_jet0_energyFrac_depth4 );
	plotter_example2.SetTreeNames( {"NoSel", "NoSel"} ); 					// Multiple Tree Names -- number must match number of input files (1:1)
	plotter_example2.SetCuts("jet0_pt > 40 && abs(jet0_Eta) < 1.5"); 		// Apply cuts to all events
	plotter_example2.SetComparisonCuts({Cut_DecayTk, Cut_DecayHCAL}); 		// Apply cuts to overlay
	plotter_example2.SetSelectiveCuts("5k", "jet0_energy > 200");			// Apply this only to filetag names that include the string "data"
	plotter_example2.SetOutputFileTag("mycoolplot"); 						// Your own special name :)
	plotter_example2.SetLegendManual( 0.35, 0.6, 0.9, 0.9 );				// Manual Legend location
	plotter_example2.colors = {kOrange, kOrange, kGreen+2, kGreen+2};		// Your own colors (default kBlack + rainbow)
	plotter_example2.linestyle = {kDotted, kDashed, kDotted, kDashed}; 		// Your own linestyle (default kSolid)
	plotter_example2.NBins = 30; 									 		// Default = 100
	plotter_example2.Plot("ratio");									 		// Ratio panel

	// ----- Other Current Featues ----- //

	// - Fitting
	//		plotter_example.run_fit  = true;
	//		plotter_example.fit_type = "igf_sigma igf_mean"; 	// Options (you can combine them): "igf", "igf_sigma", "igf_mean", "tailfrac", "tail_spread", "exp_decay", "fixed_gaus";
	// - Plotting static histograms, like cutflows: 
	// 		plotter_example.PlotStaticHistogram("NoSel__jet0_energy");
	// - 2D Plotting:
	// 		plotter_example.Plot2D(jet0_energy, jet0_pt); X vs. Y
	// - Variable Bins -- BUT These are the same for all plots in a given instance of calling Plot()
	// 		plotter_example.SetVariableBins( {0, 10, 20, 30, 50, 75, 100, 150, 200, 300, 500} );

	// ----- Other Potential Featues ----- //

	// - N-1 Plots ("sig"/"ssqrtb") -- could add
	// - 1D/2D Reweighting -- could add
	// - Plot extended cutflow -- cutflow + additional dynamic cuts

}

