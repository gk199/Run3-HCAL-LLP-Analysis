#include "MiniTuplePlotter.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotterExample(){

	string path = "../MiniTuples/v0.1/minituple_";
	
	vector<string> filetags_example1 = { "test_1k" }; //, "test_10k" };

	// ----- Example 1 -----//
	// - Basic Booleans

	cout<<endl;
	cout<<" ---------- EXAMPLE 1 ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_example1( filetags_example1, path );
	plotter_example1.SetPlots({jet0_energy, jet0_energy}); // These "P_" variables are PlotParams structs defined in PlotParams.h
	plotter_example1.SetTreeName( "NoSel" );
	plotter_example1.debug  		  = true; 	// Default = false
	plotter_example1.plot_norm 		  = false; 	// Default = true
	plotter_example1.plot_log  		  = false; 	// Default = true
	plotter_example1.plot_log_ratio   = false; 	// Default = false. Make bottom panel log scale
	plotter_example1.plot_log_x       = false; 	// Default = false. Plot log on the x axis
	plotter_example1.shift_center 	  = false; 	// Default = false. Shift plots to center on their average
	plotter_example1.stamp_counts 	  = true;	// Default = false. Print event count and integral in legend
	plotter_example1.plot_grid 	  	  = true;	// Default = true. Draw grid lines on plot
	plotter_example1.run_fit   		  = false;	// Default = false. Fit core & tail
	plotter_example1.plot_cdf 		  = false;	// Default = false. Cumulative distribution function 
	plotter_example1.plot_reverse_cdf = false;	// Default = false. Cumulative distribution function 
	plotter_example1.use_weight       = false;	// Default = true, which uses weight branch when drawing non-normalized hists
	plotter_example1.Plot();					// This is what does stuff -- arguments: None/"", "ratio", "sig"/"ssqrtb"

	return;

}

