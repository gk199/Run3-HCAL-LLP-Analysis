#include "../MiniTuplePlotter.h"
#include "TString.h"

// -------------------------------------------------------------------------------------------------
void MiniTuplePlotter_EtaPhiStudy(){

	// List where minituples are stored	
//	string path = "/eos/user/g/gkopp/LLP_Analysis/MiniTuples/v2.0/minituple_";
	string path = "root://cmsxrootd.fnal.gov///store/group/lpclonglived/gkopp/HCAL_LLP_Analysis/MiniTuples/v2.0/minituple_";
	vector<string> filetags_all 	= { "v2.0_LLPskimRun2023C_500k_2023_10_18", "v2.0_MC_QCD_250k_2023_10_18", "v2.0_MCsignalLLP_500k_2023_10_18"};
	vector<string> filetags_data 	= { "v2.0_LLPskimRun2023C_500k_2023_10_18" };
	vector<string> filetags_LLP 	= { "v2.0_MCsignalLLP_500k_2023_10_18" };	
	vector<string> filetags_QCD 	= { "v2.0_MC_QCD_250k_2023_10_18" };	

	vector<PlotParams> eta_phi_plots = {P_jet0_EtaSpread, P_jet0_PhiSpread, P_jet0_EtaSpread_energy, P_jet0_PhiSpread_energy, 					// eta - phi spread
										P_jet0_EtaPhiQuadSpread, P_jet0_EtaPhiQuadSpread_energy,
										P_jet0_NeutralHadEFrac, P_jet0_ChargedHadEFrac, P_jet0_PhoEFrac, P_jet0_MuonEFrac,                      // tracking based
                                        P_jet0_NeutralPhoEFrac, P_jet0_HoverE };
	
    #include "../RegionCuts.h"

	// ----- Overlay for MC signal, MC QCD, data ----- //

	cout<<endl;
	cout<<" ---------- Study 1: Eta-Phi study, comparing MC LLP, MC QCD, and LLP skim data ---------- "<<endl;
	cout<<endl;

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

	class MiniTuplePlotter plotter_overlay_rechitE( filetags_all, path );
	plotter_overlay_rechitE.SetPlots({P_jet0_EtaPhiQuadSpread_energy});
	plotter_overlay_rechitE.SetTreeName( "NoSel" );	
	plotter_overlay_rechitE.SetOutputFileTag("Overlay_v2.0_rechitE"); 	
	plotter_overlay_rechitE.plot_log_ratio    = false; 
	plotter_overlay_rechitE.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );
	plotter_overlay_rechitE.SetLegendNames({"LLP skim", "QCD MC", "LLP in Tracker: R < 10cm", "LLP in HCAL-D2"});
	plotter_overlay_rechitE.colors = { kBlack, kRed-3, kGreen+1, kGreen+3 };
	plotter_overlay_rechitE.SetCuts("jet0_Pt >= 40 && abs(jet0_Eta) <= 1.26 && jet0_LeadingRechitE > 10");
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

	// add HLT cuts for all samples
	class MiniTuplePlotter plotter_overlay2D_HLT( filetags_all, path );
	plotter_overlay2D_HLT.SetTreeName( "NoSel" );	
	plotter_overlay2D_HLT.SetOutputFileTag("Overlay_v2.0"); 	
	plotter_overlay2D_HLT.SetComparisonCuts({Cut_LLPinCR_Jet0, Cut_LLPinHCAL_Jet0}, "MCsignalLLP");
	plotter_overlay2D_HLT.SetLegendPosition( 0.6, 0.7, 0.88, 0.88 );			
	plotter_overlay2D_HLT.SetCuts("abs(jet0_Eta) <= 1.26 " + Cut_AnyLLP_HLT);
	plotter_overlay2D_HLT.SetOutputDirectory("EtaPhiStudy/2D_HLT");
	plotter_overlay2D_HLT.SetPlots2D({Hist1_Hist2(P_jet0_Pt, P_jet0_EtaSpread), Hist1_Hist2(P_jet0_Pt, P_jet0_PhiSpread), Hist1_Hist2(P_jet0_Pt, P_jet0_EtaPhiQuadSpread),
                                    Hist1_Hist2(P_jet0_AllRechitE, P_jet0_EtaSpread), Hist1_Hist2(P_jet0_AllRechitE, P_jet0_PhiSpread), Hist1_Hist2(P_jet0_AllRechitE, P_jet0_EtaPhiQuadSpread), 
                                    Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaSpread), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_PhiSpread), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaPhiQuadSpread), 
                                    Hist1_Hist2(P_jet0_Pt, P_jet0_EtaSpread_energy), Hist1_Hist2(P_jet0_Pt, P_jet0_PhiSpread_energy), Hist1_Hist2(P_jet0_Pt, P_jet0_EtaPhiQuadSpread_energy),
                                    Hist1_Hist2(P_jet0_AllRechitE, P_jet0_EtaSpread_energy), Hist1_Hist2(P_jet0_AllRechitE, P_jet0_PhiSpread_energy), Hist1_Hist2(P_jet0_AllRechitE, P_jet0_EtaPhiQuadSpread_energy),
                                    Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaSpread_energy), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_PhiSpread_energy), Hist1_Hist2(P_jet0_LeadingRechitE, P_jet0_EtaPhiQuadSpread_energy) });
	plotter_overlay2D_HLT.PlotMany2D();

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
