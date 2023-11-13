#include "../MiniTuplePlotter.h"
#include "TString.h"

// Goals of this study: understand why the eta-phi distributions look different between LLP "prompt < 10cm" and QCD MC + LLP skim. Mostly worked on last two weeks of October. 
// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_EtaPhiStudy(){

	// List where minituples are stored	
	string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v2.0/minituple_";
//	string path = "root://cmsxrootd.fnal.gov///store/group/lpclonglived/gkopp/HCAL_LLP_Analysis/MiniTuples/v2.0/minituple_";
	vector<string> filetags_all 	= { "v2.0_LLPskimRun2023C_500k_2023_10_18", "v2.0_MC_QCD_250k_2023_10_18", "v2.0_MCsignalLLP_500k_2023_10_18"};
	vector<string> filetags_data 	= { "v2.0_LLPskimRun2023C_500k_2023_10_18" };
	vector<string> filetags_LLP 	= { "v2.0_MCsignalLLP_500k_2023_10_18" };	
	vector<string> filetags_QCD 	= { "v2.0_MC_QCD_250k_2023_10_18" };	

	vector<PlotParams> eta_phi_plots = {P_jet0_EtaSpread, P_jet0_PhiSpread, P_jet0_EtaSpread_energy, P_jet0_PhiSpread_energy, 					// eta - phi spread
										P_jet0_EtaPhiQuadSpread, P_jet0_EtaPhiQuadSpread_energy,
										P_jet0_NeutralHadEFrac, P_jet0_ChargedHadEFrac, P_jet0_PhoEFrac, P_jet0_MuonEFrac,                      // tracking based
                                        P_jet0_NeutralPhoEFrac, P_jet0_HoverE };
	
    #include "../RegionCuts.h"
	
	cout<<endl;
	cout<<" ---------- Study 1: 2D eta-Phi study, comparing MC LLP, MC QCD, and LLP skim data. If a HLT is passed, make many 2D distributions to see correlations (jet energy, etc) ---------- "<<endl;
	cout<<endl;

	// ----- Overlay for MC signal, MC QCD, data ----- //

	// add HLT cuts for all samples
	class MiniTuplePlotter plotter_overlay2D_HLT( filetags_all, path );
	plotter_overlay2D_HLT.SetTreeName( "NoSel" );	
	plotter_overlay2D_HLT.SetOutputFileTag("Overlay_v2.0"); 	
	plotter_overlay2D_HLT.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );			
	plotter_overlay2D_HLT.SetCuts("abs(jet0_Eta) <= 1.26 " + Cut_AnyLLP_HLT);
	//plotter_overlay2D_HLT.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0}, "MCsignalLLP");
	//plotter_overlay2D_HLT.SetOutputDirectory("EtaPhiStudy/2D_HLT");
	plotter_overlay2D_HLT.SetPlots2D({Hist1_Hist2(P_jet0_Pt, P_jet0_EtaSpread), Hist1_Hist2(P_jet0_Pt, P_jet0_PhiSpread), Hist1_Hist2(P_jet0_Pt, P_jet0_EtaPhiQuadSpread),
                                    Hist1_Hist2(P_jet0_AllRechitE, P_jet0_EtaSpread), Hist1_Hist2(P_jet0_AllRechitE, P_jet0_PhiSpread), Hist1_Hist2(P_jet0_AllRechitE, P_jet0_EtaPhiQuadSpread), 
                                    Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaSpread), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_PhiSpread), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaPhiQuadSpread), 
                                    Hist1_Hist2(P_jet0_Pt, P_jet0_EtaSpread_energy), Hist1_Hist2(P_jet0_Pt, P_jet0_PhiSpread_energy), Hist1_Hist2(P_jet0_Pt, P_jet0_EtaPhiQuadSpread_energy),
                                    Hist1_Hist2(P_jet0_AllRechitE, P_jet0_EtaSpread_energy), Hist1_Hist2(P_jet0_AllRechitE, P_jet0_PhiSpread_energy), Hist1_Hist2(P_jet0_AllRechitE, P_jet0_EtaPhiQuadSpread_energy),
                                    Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaSpread_energy), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_PhiSpread_energy), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaPhiQuadSpread_energy) });
	//plotter_overlay2D_HLT.PlotMany2D();
	//plotter_overlay2D_HLT.ClearFileTrees();
	plotter_overlay2D_HLT.SetOutputDirectory("EtaPhiStudy/2D_HLT_Split");
	plotter_overlay2D_HLT.SetSelectiveCuts("MCsignalLLP", Cut_LLPinHCAL_Jet0);
	plotter_overlay2D_HLT.SetComparisonCuts( {Cut_HLT_dijet_1promptTrack,		// displaced dijet with at most 1 associated prompt track, "HLT_HT280"
						Cut_HLT_dijet_1displacedTrack,							// displaced dijet, one displaced track, "HLT_HT170"
						Cut_HLT_dijet_2promptTrack,								// displaced dijet, allow for 2 prompt tracks (higher jet and HT energies), "HLT_HT320"
						Cut_HLT_delayedjet_ECALtiming} );						// delayed jet with ECAL timing delays, "HLT_HT200"
	plotter_overlay2D_HLT.PlotMany2D();


	cout<<endl;
	cout<<" ---------- Study 2: HLT study, comparing MC LLP, MC QCD, and LLP skim data in 4 different HLT groupings ---------- "<<endl;
	cout<<endl;

	// How do the different types of HLT path affect the distributions? 
	class MiniTuplePlotter plotter_overlay_HLT( filetags_all, path );
	plotter_overlay_HLT.SetPlots({P_jet0_EtaPhiQuadSpread_energy});
	plotter_overlay_HLT.SetTreeName( "NoSel" );	
	plotter_overlay_HLT.SetOutputFileTag("Overlay_v2.0_HLT_dijet_1promptTrack"); 	
	plotter_overlay_HLT.plot_log_ratio    = false; 
	plotter_overlay_HLT.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_overlay_HLT.SetLegendNames({"LLP skim", "QCD MC", "LLP in Tracker: R < 10cm", "LLP in HCAL"});
	plotter_overlay_HLT.colors = { kBlack, kRed-3, kGreen+1, kGreen+3 };
	plotter_overlay_HLT.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26 " + Cut_HLT_dijet_1promptTrack); // displaced dijet with at most 1 associated prompt track, "HLT_HT280"
	plotter_overlay_HLT.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0}, "MCsignalLLP");
	plotter_overlay_HLT.SetOutputDirectory("EtaPhiStudy");
	plotter_overlay_HLT.Plot("ratio");	

	class MiniTuplePlotter plotter_overlay_HLT2( filetags_all, path );
	plotter_overlay_HLT2.SetPlots({P_jet0_EtaPhiQuadSpread_energy});
	plotter_overlay_HLT2.SetTreeName( "NoSel" );	
	plotter_overlay_HLT2.SetOutputFileTag("Overlay_v2.0_HLT_dijet_2promptTrack"); 	
	plotter_overlay_HLT2.plot_log_ratio    = false; 
	plotter_overlay_HLT2.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_overlay_HLT2.SetLegendNames({"LLP skim", "QCD MC", "LLP in Tracker: R < 10cm", "LLP in HCAL"});
	plotter_overlay_HLT2.colors = { kBlack, kRed-3, kGreen+1, kGreen+3 };
	plotter_overlay_HLT2.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26 " + Cut_HLT_dijet_2promptTrack); // displaced dijet, allow for 2 prompt tracks (higher jet and HT energies), "HLT_HT320"
	plotter_overlay_HLT2.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0}, "MCsignalLLP");
	plotter_overlay_HLT2.SetOutputDirectory("EtaPhiStudy");
	plotter_overlay_HLT2.Plot("ratio");	
	
	class MiniTuplePlotter plotter_overlay_HLT3( filetags_all, path );
	plotter_overlay_HLT3.SetPlots({P_jet0_EtaPhiQuadSpread_energy});
	plotter_overlay_HLT3.SetTreeName( "NoSel" );	
	plotter_overlay_HLT3.SetOutputFileTag("Overlay_v2.0_HLT_dijet_1displacedTrack"); 	
	plotter_overlay_HLT3.plot_log_ratio    = false; 
	plotter_overlay_HLT3.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_overlay_HLT3.SetLegendNames({"LLP skim", "QCD MC", "LLP in Tracker: R < 10cm", "LLP in HCAL"});
	plotter_overlay_HLT3.colors = { kBlack, kRed-3, kGreen+1, kGreen+3 };
	plotter_overlay_HLT3.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26 " + Cut_HLT_dijet_1displacedTrack); // displaced dijet, one displaced track, "HLT_HT170"
	plotter_overlay_HLT3.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0}, "MCsignalLLP");
	plotter_overlay_HLT3.SetOutputDirectory("EtaPhiStudy");
	plotter_overlay_HLT3.Plot("ratio");	

	class MiniTuplePlotter plotter_overlay_HLT4( filetags_all, path );
	plotter_overlay_HLT4.SetPlots({P_jet0_EtaPhiQuadSpread_energy});
	plotter_overlay_HLT4.SetTreeName( "NoSel" );	
	plotter_overlay_HLT4.SetOutputFileTag("Overlay_v2.0_HLT_delayedjet_ECALtiming"); 	
	plotter_overlay_HLT4.plot_log_ratio    = false; 
	plotter_overlay_HLT4.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_overlay_HLT4.SetLegendNames({"LLP skim", "QCD MC", "LLP in Tracker: R < 10cm", "LLP in HCAL"});
	plotter_overlay_HLT4.colors = { kBlack, kRed-3, kGreen+1, kGreen+3 };
	plotter_overlay_HLT4.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26 " + Cut_HLT_delayedjet_ECALtiming); // delayed jet with ECAL timing delays, "HLT_HT200"
	plotter_overlay_HLT4.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0}, "MCsignalLLP");
	plotter_overlay_HLT4.SetOutputDirectory("EtaPhiStudy");
	plotter_overlay_HLT4.Plot("ratio");	

	cout<<endl;
	cout<<" ---------- Study 3: Eta-Phi study, comparing MC LLP, MC QCD, and LLP skim data ---------- "<<endl;
	cout<<endl;

	// many plots, wit each file (LLP MC split by decay position)
	class MiniTuplePlotter plotter_overlay( filetags_all, path );
	plotter_overlay.SetPlots(eta_phi_plots);
	plotter_overlay.SetTreeName( "NoSel" );	
	plotter_overlay.SetOutputFileTag("Overlay_v2.0"); 	
	plotter_overlay.plot_log_ratio    = false; 
	plotter_overlay.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_overlay.SetLegendNames({"LLP skim", "QCD MC", "LLP in Tracker: R < 10cm", "LLP in HCAL-D2"});
	plotter_overlay.colors = { kBlack, kRed-3, kGreen+1, kGreen+3 };
	plotter_overlay.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26");
	plotter_overlay.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL2_Jet0}, "MCsignalLLP");
	// plotter_overlay.SetSelectiveCuts("LLPskim", "jet0_NeutralHadEFrac < 0.6");  // BLINDED with track energy fraction (jet0_Track0Pt / jet0_Pt > 0.1) or neutral hadron fraction (jet0_NeutralHadEFrac < 0.6)
	plotter_overlay.SetOutputDirectory("EtaPhiStudy");
	plotter_overlay.Plot("ratio");

	// check adding a cut on the leading rechit energy
	class MiniTuplePlotter plotter_overlay_rechitE( filetags_all, path );
	plotter_overlay_rechitE.SetPlots({P_jet0_EtaPhiQuadSpread_energy});
	plotter_overlay_rechitE.SetTreeName( "NoSel" );	
	plotter_overlay_rechitE.SetOutputFileTag("Overlay_v2.0_rechitE"); 	
	plotter_overlay_rechitE.plot_log_ratio    = false; 
	plotter_overlay_rechitE.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_overlay_rechitE.SetLegendNames({"LLP skim", "QCD MC", "LLP in Tracker: R < 10cm", "LLP in HCAL-D2"});
	plotter_overlay_rechitE.colors = { kBlack, kRed-3, kGreen+1, kGreen+3 };
	plotter_overlay_rechitE.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26 && jet0_LeadingRechitE > 15");
	plotter_overlay_rechitE.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL2_Jet0}, "MCsignalLLP");
	plotter_overlay_rechitE.SetOutputDirectory("EtaPhiStudy");
	plotter_overlay_rechitE.Plot("ratio");

	// 2D spread of eta - phi 
	class MiniTuplePlotter plotter_overlay2D( filetags_all, path );
	plotter_overlay2D.SetTreeName( "NoSel" );	
	plotter_overlay2D.SetOutputFileTag("Overlay_v2.0"); 	
	plotter_overlay2D.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0}, "MCsignalLLP");
	plotter_overlay2D.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );			
	plotter_overlay2D.SetCuts("abs(jet0_Eta) <= 1.26");
	plotter_overlay2D.SetOutputDirectory("EtaPhiStudy/2D");
	plotter_overlay2D.SetPlots2D({Hist1_Hist2(P_jet0_Pt, P_jet0_EtaSpread), Hist1_Hist2(P_jet0_Pt, P_jet0_PhiSpread), Hist1_Hist2(P_jet0_Pt, P_jet0_EtaPhiQuadSpread),
                                    Hist1_Hist2(P_jet0_AllRechitE, P_jet0_EtaSpread), Hist1_Hist2(P_jet0_AllRechitE, P_jet0_PhiSpread), Hist1_Hist2(P_jet0_AllRechitE, P_jet0_EtaPhiQuadSpread), 
                                    Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaSpread), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_PhiSpread), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaPhiQuadSpread), 
                                    Hist1_Hist2(P_jet0_Pt, P_jet0_EtaSpread_energy), Hist1_Hist2(P_jet0_Pt, P_jet0_PhiSpread_energy), Hist1_Hist2(P_jet0_Pt, P_jet0_EtaPhiQuadSpread_energy),
                                    Hist1_Hist2(P_jet0_AllRechitE, P_jet0_EtaSpread_energy), Hist1_Hist2(P_jet0_AllRechitE, P_jet0_PhiSpread_energy), Hist1_Hist2(P_jet0_AllRechitE, P_jet0_EtaPhiQuadSpread_energy),
                                    Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaSpread_energy), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_PhiSpread_energy), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaPhiQuadSpread_energy) });
	plotter_overlay2D.PlotMany2D();

	// how does photon fraction change result?
	class MiniTuplePlotter plotter_overlay2D_pho( filetags_all, path );
	plotter_overlay2D_pho.SetTreeName( "NoSel" );	
	plotter_overlay2D_pho.SetOutputFileTag("Overlay_v2.0"); 	
	plotter_overlay2D_pho.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0}, "MCsignalLLP");
	plotter_overlay2D_pho.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );			
	plotter_overlay2D_pho.SetCuts("abs(jet0_Eta) <= 1.26 && jet0_PhoEFrac > 0.9");
	plotter_overlay2D_pho.SetOutputDirectory("EtaPhiStudy/2D_energy_phoCut");
	plotter_overlay2D_pho.SetPlots2D({Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaPhiQuadSpread_energy), 
                                    Hist1_Hist2(P_jet0_Pt, P_jet0_EtaPhiQuadSpread_energy) });
	plotter_overlay2D_pho.PlotMany2D();
}
