#include "../MiniTuplePlotter.h"
#include "TString.h"

// Goals of this study: understand prompt and displaced track multiplicity in signal MC jets.
// Make 2D plots of:
//   - jet pT vs number of prompt tracks
//   - event HT vs number of prompt tracks
//   - jet pT vs number of displaced tracks
//   - event HT vs number of displaced tracks
//
// Prompt track:    jet*_Track*dxyToBS < 0.1 cm
// Displaced track: jet*_Track*dxyOverErr > 5 AND jet*_Track*dxyToBS > 0.05 cm
//
// Three tracks per jet are stored (Track0, Track1, Track2); counts are formed by summing booleans.
// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_PromptTracks(){

	string path    = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.8/minituple_";
	string path_v5 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.1/minituple_";

	// Set to true to run only over a representative subset (faster);
	// set to false to run over all files.
	bool quickRun = true;

	// Data (v5.1)
	vector<string> filetags_data_2022 = quickRun ? vector<string>{ "data_2022Gv1_scores" }
	                                              : vector<string>{ "data_2022Dv1_scores", "data_2022Ev1_scores", "data_2022Fv1_scores", "data_2022Gv1_scores" };
	vector<string> filetags_data_2023 = quickRun ? vector<string>{ "data_2023Cv4_scores" }
	                                              : vector<string>{ "data_2023Cv1_scores", "data_2023Cv2_scores", "data_2023Cv3_scores", "data_2023Cv4_scores", "data_2023Dv1_scores", "data_2023Dv2_scores" };

	// Signal MC (v3.8)
	map<string,vector<string>> filetags;
	filetags["LLP350_80_ctau500mm"]  = { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_rerun"};
	filetags["LLP125_50_ctau3m"]     = { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_combined"};
	filetags["LLP125_50_ctau10m"]    = { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau10000_13p6TeV_2024_06_03"};
	filetags["LLP125_15_ctau3m"]     = { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau3000_13p6TeV_2024_06_03"};
	filetags["LLP250_120_ctau10m"]   = { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_06_03_batch1"};
	filetags["LLP350_160_ctau10m"]   = { "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_06_03_batch1"};

	vector<string> filetag_keys_to_loop = quickRun ? vector<string>{ "LLP125_50_ctau3m" }
	                                                : vector<string>{ "LLP350_80_ctau500mm", "LLP125_50_ctau3m", "LLP250_120_ctau10m", "LLP350_160_ctau10m" };

	#include "../RegionCuts.h"

	// ------------------------------------------------------------------------------------------
	// Track count expressions (leading jet, jet0)
	// Three tracks per jet are stored: Track0, Track1, Track2
	// Prompt:   dxyToBS < 0.1 cm
	// Displaced: dxyOverErr > 5 AND dxyToBS > 0.05 cm
	// ------------------------------------------------------------------------------------------
	PlotParams P_jet0_nPromptTracks = {
		"(jet0_Track0dxyToBS < 0.1) + (jet0_Track1dxyToBS < 0.1) + (jet0_Track2dxyToBS < 0.1)",
		"Leading Jet: Number of Prompt Tracks",
		"N prompt tracks (d_{xy} to BS < 0.1 cm)", -0.5, 3.5
	};

	PlotParams P_jet0_nDisplacedTracks = {
		"(jet0_Track0dxyOverErr > 5 && jet0_Track0dxyToBS > 0.05) + (jet0_Track1dxyOverErr > 5 && jet0_Track1dxyToBS > 0.05) + (jet0_Track2dxyOverErr > 5 && jet0_Track2dxyToBS > 0.05)",
		"Leading Jet: Number of Displaced Tracks",
		"N displaced tracks (d_{xy}/err > 5, d_{xy} to BS > 0.05 cm)", -0.5, 3.5
	};

	vector<string> jet_E    = {"100"};
	vector<string> event_HT = {"250"};

	// ------------------------------------------------------------------------------------------
	cout<<endl;
	cout<<" ---------- Prompt Tracks Study 1: jet E and event HT vs N prompt/displaced tracks, by mass point ---------- "<<endl;
	cout<<endl;

	for( auto key : filetag_keys_to_loop ){
		class MiniTuplePlotter track_plotter( filetags[key], path );
		track_plotter.SetTreeName( "NoSel" );
		track_plotter.SetOutputFileTag("PromptTracks_v3.8_"+key);
		track_plotter.SetOutputDirectory("PromptTracks");
		track_plotter.plot_log  = true;
		track_plotter.plot_norm = false;
		track_plotter.SetCuts( Form("jet0_L1trig_Matched == 1 && abs(jet0_Eta) <= 1.26") );
		track_plotter.SetPlots2D({
			Hist1_Hist2(P_jet0_Pt,  P_jet0_nPromptTracks),
			Hist1_Hist2(P_eventHT,  P_jet0_nPromptTracks),
			Hist1_Hist2(P_jet0_Pt,  P_jet0_nDisplacedTracks),
			Hist1_Hist2(P_eventHT,  P_jet0_nDisplacedTracks)
		});
		track_plotter.PlotMany2D();
	}

	// ------------------------------------------------------------------------------------------
	cout<<endl;
	cout<<" ---------- Prompt Tracks Study 2: MH-125 MS-50 CTau3m, LLP in HCAL ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_MC_LLPinHCAL( filetags["LLP125_50_ctau3m"], path );
	plotter_MC_LLPinHCAL.SetTreeName( "NoSel" );
	plotter_MC_LLPinHCAL.SetOutputFileTag("PromptTracks_v3.8_MH125_MS50_CTau3m_LLPinHCAL");
	plotter_MC_LLPinHCAL.SetOutputDirectory("PromptTracks");
	plotter_MC_LLPinHCAL.plot_log  = true;
	plotter_MC_LLPinHCAL.plot_norm = false;
	plotter_MC_LLPinHCAL.SetCuts( Form("jet0_L1trig_Matched == 1 && abs(jet0_Eta) <= 1.26") );
	plotter_MC_LLPinHCAL.SetComparisonCuts({Cut_LLPinHCAL});
	plotter_MC_LLPinHCAL.SetPlots2D({
		Hist1_Hist2(P_jet0_Pt,  P_jet0_nPromptTracks),
		Hist1_Hist2(P_eventHT,  P_jet0_nPromptTracks),
		Hist1_Hist2(P_jet0_Pt,  P_jet0_nDisplacedTracks),
		Hist1_Hist2(P_eventHT,  P_jet0_nDisplacedTracks)
	});
	plotter_MC_LLPinHCAL.PlotMany2D();

	// ------------------------------------------------------------------------------------------
	cout<<endl;
	cout<<" ---------- Prompt Tracks Study 3: Data 2022 -- jet E and event HT vs N prompt/displaced tracks ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_data_2022( filetags_data_2022, path_v5 );
	plotter_data_2022.SetTreeName( "NoSel" );
	plotter_data_2022.SetOutputFileTag("PromptTracks_v5.1_2022");
	plotter_data_2022.SetOutputDirectory("PromptTracks");
	plotter_data_2022.plot_log  = true;
	plotter_data_2022.plot_norm = false;
	plotter_data_2022.SetCuts( Form("jet0_L1trig_Matched == 1 && abs(jet0_Eta) <= 1.26") );
	plotter_data_2022.SetPlots2D({
		Hist1_Hist2(P_jet0_Pt,  P_jet0_nPromptTracks),
		Hist1_Hist2(P_eventHT,  P_jet0_nPromptTracks),
		Hist1_Hist2(P_jet0_Pt,  P_jet0_nDisplacedTracks),
		Hist1_Hist2(P_eventHT,  P_jet0_nDisplacedTracks)
	});
	plotter_data_2022.PlotMany2D();

	// ------------------------------------------------------------------------------------------
	cout<<endl;
	cout<<" ---------- Prompt Tracks Study 4: Data 2023 -- jet E and event HT vs N prompt/displaced tracks ---------- "<<endl;
	cout<<endl;

	class MiniTuplePlotter plotter_data_2023( filetags_data_2023, path_v5 );
	plotter_data_2023.SetTreeName( "NoSel" );
	plotter_data_2023.SetOutputFileTag("PromptTracks_v5.1_2023");
	plotter_data_2023.SetOutputDirectory("PromptTracks");
	plotter_data_2023.plot_log  = true;
	plotter_data_2023.plot_norm = false;
	plotter_data_2023.SetCuts( Form("jet0_L1trig_Matched == 1 && abs(jet0_Eta) <= 1.26") );
	plotter_data_2023.SetPlots2D({
		Hist1_Hist2(P_jet0_Pt,  P_jet0_nPromptTracks),
		Hist1_Hist2(P_eventHT,  P_jet0_nPromptTracks),
		Hist1_Hist2(P_jet0_Pt,  P_jet0_nDisplacedTracks),
		Hist1_Hist2(P_eventHT,  P_jet0_nDisplacedTracks)
	});
	plotter_data_2023.PlotMany2D();

}
