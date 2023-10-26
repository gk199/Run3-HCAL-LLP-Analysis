// -------------------------------------------------------------------------------------
struct PlotParams{
	string hist_name, title, label_x;
	double xmin, xmax;
};
typedef struct PlotParams PlotParams;

struct Hist1_Hist2_Cut{
	PlotParams Params1, Params2; 
	TCut cut;	
	Hist1_Hist2_Cut(PlotParams hist1, PlotParams hist2, TCut sel_cut)
	{
		Params1 = hist1;
		Params2 = hist2;
		cut = sel_cut;
	}
};

struct Hist1_Hist2{
	PlotParams Params1, Params2; 	
	Hist1_Hist2(PlotParams hist1, PlotParams hist2)
	{
		Params1 = hist1;
		Params2 = hist2;
	}
};

// Example:
// PlotParams P_VarName 	= {"VariableName", "Plot Title", "x label [units]", XMIN, XMAX };

// ----- Physics Variables ----- // 

// Leading Jet 
// kinematics
PlotParams P_jet0_RechitN				= {"jet0_RechitN", "Number of HCAL RecHits", "N^{HCAL}_{RecHits}", 0, 60 };
PlotParams P_jet0_E						= {"jet0_E", "Leading Jet Energy", "Jet E [GeV]", 0, 250 };
PlotParams P_jet0_Pt					= {"jet0_Pt", "Leading Jet p_{T}", "Jet p_{T} [GeV]", 0, 250 };
PlotParams P_jet0_Eta					= {"jet0_Eta", "Leading Jet #eta", "Jet #eta", -1.5, 1.5 };
PlotParams P_jet0_Phi					= {"jet0_Phi", "Leading Jet #phi", "Jet #phi", -3.2, 3.2 };
PlotParams P_jet0_Mass					= {"jet0_Mass", "Leading Jet Mass", "Jet Mass", 0, 40 };
PlotParams P_jet0_JetArea				= {"jet0_JetArea", "Leading Jet Area", "Jet Area", 0, 1 };
// spread
PlotParams P_jet0_EtaSpread				= {"jet0_EtaSpread", "Leading Jet #eta Spread", "#eta Spread", 0, 0.4 };
PlotParams P_jet0_EtaSpread_energy		= {"jet0_EtaSpread_energy", "Leading Jet #eta Spread Energy", "#eta Spread (energy weighted)", 0, 0.2 };
PlotParams P_jet0_PhiSpread				= {"jet0_PhiSpread", "Leading Jet #phi Spread", "#phi Spread", 0, 0.4 };
PlotParams P_jet0_PhiSpread_energy		= {"jet0_PhiSpread_energy", "Leading Jet #phi Spread Energy", "#phi Spread (energy weighted)", 0, 0.2 };
PlotParams P_jet0_EtaPhiQuadSpread		= {"jet0_EtaPhiQuadSpread", "Leading Jet #sqrt{#Delta#eta^{2} + #Delta#phi^{2}} Spread", "#sqrt{#Delta#eta^{2} + #Delta#phi^{2}} Spread", 0, 0.4 };
PlotParams P_jet0_EtaPhiQuadSpread_energy={"jet0_EtaPhiQuadSpread_energy", "Leading Jet #sqrt{#Delta#eta^{2} + #Delta#phi^{2}} Spread Energy", "#sqrt{#Delta#eta^{2} + #Delta#phi^{2}} Spread (energy weighted)", 0, 0.2 };
// tracks
PlotParams P_jet0_Track0PtFrac			= {"jet0_Track0Pt / jet0_Pt", "Leading Jet: Leading Track p_{T} / Jet p_{T}", "Track Energy Fraction", 0, 1 };
PlotParams P_jet0_Track0Pt				= {"jet0_Track0Pt", "Leading Jet: Leading Track p_{T}", "p_{T} [GeV]", 0, 15 };
PlotParams P_jet0_Track1Pt				= {"jet0_Track1Pt", "Leading Jet: Subeading Track p_{T}", "p_{T} [GeV]", 0, 5 };
PlotParams P_jet0_Track2Pt				= {"jet0_Track2Pt", "Leading Jet: Third leading Track p_{T}", "p_{T} [GeV]", 0, 5 };
PlotParams P_jet0_Track0dxyToBS			= {"jet0_Track0dxyToBS", "Leading Jet: Leading Track d_{xy} to BS", "d_{xy} to BS", -20, 20};
PlotParams P_jet0_Track1dxyToBS			= {"jet0_Track0dxyToBS", "Leading Jet: Subleading Track d_{xy} to BS", "d_{xy} to BS", -20, 20};
PlotParams P_jet0_Track2dxyToBS			= {"jet0_Track2dxyToBS", "Leading Jet: Third Leading Track d_{xy} to BS", "d_{xy} to BS", -20, 20};
PlotParams P_jet0_Track0dxyOverErr		= {"jet0_Track0dxyOverErr", "Leading Jet: Leading Track d_{xy} / error", "d_{xy} / error", 0, 40};
PlotParams P_jet0_Track1dxyOverErr		= {"jet0_Track0dxyOverErr", "Leading Jet: Subleading Track d_{xy} / error", "d_{xy} / error", 0, 40};
PlotParams P_jet0_Track2dxyOverErr		= {"jet0_Track2dxyOverErr", "Leading Jet: Third Leading Track d_{xy} / error", "d_{xy} / error", 0, 40};
PlotParams P_jet0_Track0dzToPV			= {"jet0_Track0dzToPV", "Leading Jet: Leading Track #DeltaZ to PV", "#DeltaZ [cm]", -40, 40 };
PlotParams P_jet0_Track1dzToPV			= {"jet0_Track1dzToPV", "Leading Jet: Subeading Track #DeltaZ to PV", "#DeltaZ [cm]", -40, 40 };
PlotParams P_jet0_Track2dzToPV			= {"jet0_Track2dzToPV", "Leading Jet: Third Leading Track #DeltaZ to PV", "#DeltaZ [cm]", -40, 40 };
PlotParams P_jet0_Track0dzOverErr		= {"jet0_Track0dzOverErr", "Leading Jet: Leading Track #DeltaZ / error", "#DeltaZ [cm] / error", 0, 100 };
PlotParams P_jet0_Track1dzOverErr		= {"jet0_Track1dzOverErr", "Leading Jet: Subeading Track #DeltaZ / error", "#DeltaZ [cm] / error", 0, 100 };
PlotParams P_jet0_Track2dzOverErr		= {"jet0_Track2dzOverErr", "Leading Jet: Third Leading Track #DeltaZ / error", "#DeltaZ [cm] / error", 0, 100 };
PlotParams P_jet0_Tracks_dR				= {"jet0_Tracks_dR", "Leading Jet: Leading Track to Sub-leading Track #DeltaR", "#DeltaR (leading, subleading track)", 0, 1.1 };
PlotParams P_jet0_Track0dR				= {"jet0_Track0dR", "Leading Jet to Leading Track #DeltaR", "#DeltaR", 0, 0.6 };
PlotParams P_jet0_Track1dR				= {"jet0_Track1dR", "Leading Jet to Subleading Track #DeltaR", "#DeltaR", 0, 0.6 };
PlotParams P_jet0_Track2dR				= {"jet0_Track2dR", "Leading Jet to Third Leading Track #DeltaR", "#DeltaR", 0, 0.6 };
PlotParams P_jet0_Track0dEta			= {"jet0_Track0dEta", "Leading Jet to Leading Track #Delta#eta", "#Delta#eta", 0, 0.6 };
PlotParams P_jet0_Track1dEta			= {"jet0_Track1dEta", "Leading Jet to Subleading Track #Delta#eta", "#Delta#eta", 0, 0.6 };
PlotParams P_jet0_Track2dEta			= {"jet0_Track2dEta", "Leading Jet to Third Leading Track #Delta#eta", "#Delta#eta", 0, 0.6 };
PlotParams P_jet0_Track0dPhi			= {"jet0_Track0dPhi", "Leading Jet to Leading Track #Delta#phi", "#Delta#phi", 0, 0.6 };
PlotParams P_jet0_Track1dPhi			= {"jet0_Track1dPhi", "Leading Jet to Subleading Track #Delta#phi", "#Delta#phi", 0, 0.6 };
PlotParams P_jet0_Track2dPhi			= {"jet0_Track2dPhi", "Leading Jet to Third Leading Track #Delta#phi", "#Delta#phi", 0, 0.6 };
// note these still need a fix in the ntupler!!!!!!!
PlotParams P_jet0_PtAllTracks			= {"jet0_PtAllTracks", "Leading Jet: p_{T} all tracks", "p_{T} all tracks", 0, 100 };
PlotParams P_jet0_PtAllPVTracks			= {"jet0_PtAllPVTracks", "Leading Jet: p_{T} all PV tracks", "p_{T} all PV tracks", 0, 100 };
PlotParams P_jet0_NVertexTracks			= {"jet0_NVertexTracks", "Leading Jet: N vertex tracks", "N vertex tracks", 0, 100 };
PlotParams P_jet0_NSelectedTracks		= {"jet0_NSelectedTracks", "Leading Jet: N selected tracks", "N selected tracks", 0, 100 };
// SV information
PlotParams P_jet0_NSV					= {"jet0_NSV", "Leading Jet: Number SV", "Number SV", 0, 6 };
PlotParams P_jet0_NSVCand				= {"jet0_NSVCand", "Leading Jet: Number SV Canidates", "Number SV Canidates", 0, 6 };
PlotParams P_jet0_SV_x					= {"jet0_SV_x", "Leading Jet: SV x", "SV x [cm]", -4, 4 };
PlotParams P_jet0_SV_y					= {"jet0_SV_y", "Leading Jet: SV y", "SV y [cm]", -4, 4 };
PlotParams P_jet0_SV_z					= {"jet0_SV_z", "Leading Jet: SV z", "SV z [cm]", -20, 20 };
PlotParams P_jet0_SV_NTracks			= {"jet0_SV_NTracks", "Leading Jet: SV Number of Tracks", "SV Number of Tracks", 0, 20 };
PlotParams P_jet0_SV_Mass				= {"jet0_SV_Mass", "Leading Jet: SV Mass", "SV Mass [GeV]", 0, 6 };
PlotParams P_jet0_FlightDist2D			= {"jet0_FlightDist2D", "Leading Jet: Flight Distance 2D", "Flight Distance 2D", 0, 5 };
PlotParams P_jet0_FlightDist2DErr		= {"jet0_FlightDist2DErr", "Leading Jet: Flight Distance 2D Error", "Flight Distance 2D Error", 0, 1.5 };
PlotParams P_jet0_FlightDist3D			= {"jet0_FlightDist3D", "Leading Jet: Flight Distance 3D", "Flight Distance 3D", 0, 5 };
PlotParams P_jet0_FlightDist3DErr		= {"jet0_FlightDist3DErr", "Leading Jet: Flight Distance 3D Error", "Flight Distance 3D Error", 0, 1.5 };
// HB rechits
PlotParams P_jet0_LeadingRechitE		= {"jet0_LeadingRechitE", "Leading Jet: Leading Rechit Energy", "Leading Rechit Energy [GeV]", 0, 100 };
PlotParams P_jet0_SubLeadingRechitE		= {"jet0_SubLeadingRechitE", "Leading Jet: Subleading Rechit Energy", "Sub-Leading Rechit Energy [GeV]", 0, 100 };
PlotParams P_jet0_SSubLeadingRechitE	= {"jet0_SSubLeadingRechitE", "Leading Jet: Third Leading Rechit Energy", "Third-Leading Rechit Energy [GeV]", 0, 100 };
PlotParams P_jet0_AllRechitE			= {"jet0_AllRechitE", "Leading Jet: Sum of HB Rechit Energy", "Sum of HB Rechit Energy [GeV]", 0, 300 };
PlotParams P_jet0_3RechitFracE			= {"(jet0_LeadingRechitE + jet0_SubLeadingRechitE + jet0_SSubLeadingRechitE) / jet0_AllRechitE", "Leading Jet: Fraction of Energy in Leading 3 Rechits", "Fraction of Energy", 0, 1 };
PlotParams P_jet0_LeadingRechitEFracJet	= {"jet0_LeadingRechitE / jet0_E", "Leading Jet: Leading Rechit Energy / Jet Energy", "Energy Fraction", 0, 1 };
PlotParams P_jet0_LeadingRechitEFrac	= {"jet0_LeadingRechitE / jet0_AllRechitE", "Leading Jet: Leading Rechit Energy / Sum of HB Rechit Energy", "Energy Fraction", 0, 1 };
PlotParams P_jet0_LeadingRechitJetEDiff	= {"jet0_E - jet0_LeadingRechitE", "Leading Jet - Leading Rechit Energy", "Energy Difference", 0, 200 };
PlotParams P_jet0_LeadingSubRechitEQuad	= {"sqrt(jet0_LeadingRechitE * jet0_LeadingRechitE + jet0_SubLeadingRechitE * jet0_SubLeadingRechitE)", "Leading Jet: #sqrt{E_{leading rechit}^{2} + E_{sub-leading rechit}^{2}}", "#sqrt{E_{leading rechit}^{2} + E_{sub-leading rechit}^{2}} [GeV]", 0, 100 };
PlotParams P_jet0_LeadingRechitD		= {"jet0_LeadingRechitD", "Leading Jet: Leading Rechit Depth", "HCAL Depth", 0, 5 };
PlotParams P_jet0_SubLeadingRechitD		= {"jet0_SubLeadingRechitD", "Leading Jet: Subleading Rechit Depth", "HCAL Depth", 0, 5 };
PlotParams P_jet0_SSubLeadingRechitD	= {"jet0_SSubLeadingRechitD", "Leading Jet: Third Leading Rechit Depth", "HCAL Depth", 0, 5 };
// energy fractions
PlotParams P_jet0_NeutralHadEFrac		= {"jet0_NeutralHadEFrac", "Leading Jet: Neutral Hadron Energy Fraction", "Neutral Hadron Energy Fraction", 0, 1 };
PlotParams P_jet0_ChargedHadEFrac		= {"jet0_ChargedHadEFrac", "Leading Jet: Charged Hadron Energy Fraction", "Charged Hadron Energy Fraction", 0, 1 };
PlotParams P_jet0_PhoEFrac				= {"jet0_PhoEFrac", "Leading Jet: Photon Energy Fraction", "Photon Energy Fraction", 0, 1 };
PlotParams P_jet0_EleEFrac				= {"jet0_EleEFrac", "Leading Jet: Electromagnetic Energy Fraction", "Electromagnetic Energy Fraction", 0, 1 };
PlotParams P_jet0_MuonEFrac				= {"jet0_MuonEFrac", "Leading Jet: Muon Energy Fraction", "Muon Energy Fraction", 0, 1 };
PlotParams P_jet0_NeutralHadMult		= {"jet0_NeutralHadMult", "Leading Jet: Neutral Hadron Multiplicity", "Neutral Hadron Multiplicity", 0, 20 };
PlotParams P_jet0_ChargedHadMult		= {"jet0_ChargedHadMult", "Leading Jet: Charged Hadron Multiplicity", "Charged Hadron Multiplicity", 0, 40 };
PlotParams P_jet0_PhoMult				= {"jet0_PhoMult", "Leading Jet: Photon Multiplicity", "Photon Multiplicity", 0, 40 };
PlotParams P_jet0_EleMult				= {"jet0_EleMult", "Leading Jet: Electron Multiplicity", "Electron Multiplicity", 0, 6 };

PlotParams P_jet0_HoverE				= {"jet0_HoverE", "Leading Jet: H/E Energy Ratio", "H/E Energy Ratio", 0, 15 };
PlotParams P_jet0_NeutralElePhoEFrac	= {"(jet0_NeutralHadEFrac+jet0_EleEFrac+jet0_PhoEFrac)", "Leading Jet: Neutral Hadron + Pho + Ele Energy Fraction", "Neutral Hadron + Pho + Ele Energy Fraction", 0, 1};
PlotParams P_jet0_NeutralPhoEFrac		= {"(jet0_NeutralHadEFrac+jet0_PhoEFrac)", "Leading Jet: Neutral Hadron + Pho Energy Fraction", "Neutral Hadron + Pho Energy Fraction", 0, 1};
PlotParams P_jet0_NeutralOverChargedHad = {"jet0_NeutralHadEFrac/jet0_ChargedHadEFrac", "Leading Jet: Neutral / Charged Hadron Energy", "Neutral / Charged Hadron Energy", 0, 100};
PlotParams P_jet0_LogNeutralOverChargedHad = {"log(jet0_NeutralHadEFrac/jet0_ChargedHadEFrac)", "Leading Jet: log(Neutral / Charged Hadron Energy)", "log(Neutral / Charged Hadron Energy)", -10, 10};
// hcal energy fractions
PlotParams P_jet0_EnergyFrac_Depth1		= {"jet0_EnergyFrac_Depth1", "Leading Jet: Energy Fraction in HCAL Depth 1", "Energy Fraction", 0, 1 };
PlotParams P_jet0_EnergyFrac_Depth2		= {"jet0_EnergyFrac_Depth2", "Leading Jet: Energy Fraction in HCAL Depth 2", "Energy Fraction", 0, 1 };
PlotParams P_jet0_EnergyFrac_Depth3		= {"jet0_EnergyFrac_Depth3", "Leading Jet: Energy Fraction in HCAL Depth 3", "Energy Fraction", 0, 1 };
PlotParams P_jet0_EnergyFrac_Depth4		= {"jet0_EnergyFrac_Depth4", "Leading Jet: Energy Fraction in HCAL Depth 4", "Energy Fraction", 0, 1 };

PlotParams P_jet0_HCALd1_over_d2		= {"jet0_EnergyFrac_Depth1/jet0_EnergyFrac_Depth2", "HCAL Energy: Depth 1 over Depth 2", "Depth 1 over Depth 2 Energy Ratio", 0, 60};
PlotParams P_jet0_HCALd1_over_d3		= {"jet0_EnergyFrac_Depth1/jet0_EnergyFrac_Depth3", "HCAL Energy: Depth 1 over Depth 3", "Depth 1 over Depth 3 Energy Ratio", 0, 60};
PlotParams P_jet0_HCALd1_over_d4		= {"jet0_EnergyFrac_Depth1/jet0_EnergyFrac_Depth4", "HCAL Energy: Depth 1 over Depth 4", "Depth 1 over Depth 4 Energy Ratio", 0, 60};
PlotParams P_jet0_HCALd12_over_d34		= {"(jet0_EnergyFrac_Depth1 + jet0_EnergyFrac_Depth2)/(jet0_EnergyFrac_Depth3 + jet0_EnergyFrac_Depth4)", "HCAL Energy: Depth 1+2 over Depth 3+4", "Depth 1+2 over Depth 3+4 Energy Ratio", 0, 100};

// truth matching
PlotParams P_jet0_isTruthMatched		= {"jet0_isTruthMatched", "Leading Jet is Truth Matched", "is Truth Matched", 0, 2 };
PlotParams P_jet0_isMatchedTo			= {"jet0_isMatchedTo", "Leading Jet is Matched to LLP", "is Matched to LLP", 0, 2 };
PlotParams P_jet0_MatchedLLP_DecayR		= {"jet0_MatchedLLP_DecayR", "Leading Jet Matched LLP Decay R", "Decay R [cm]", 0, 350 };
PlotParams P_jet0_MatchedLLP_Eta		= {"jet0_MatchedLLP_Eta", "Leading Jet Matched LLP #eta", "#eta", -1.5, 1.5 };
PlotParams P_jet0_llp0_ptDiff			= {"(jet0_Pt - LLP0_Pt) / LLP0_Pt", "Leading Jet Pt - LLP0 Pt / LLP 0 Pt", "Pt Match (LLP0)", -2, 5 };
PlotParams P_jet0_llp1_ptDiff			= {"(jet0_Pt - LLP1_Pt) / LLP1_Pt", "Leading Jet Pt - LLP1 Pt / LLP 1 Pt", "Pt Match (LLP1)", -2, 5 };

// --------------------------------------------------------------------- //
// below ones are for example files, v1 minituples have changed variable names as used above
PlotParams P_jet0_rechitN				= {"jet0_rechitN", "Number of HCAL RecHits", "N^{HCAL}_{RecHits}", 0, 60 };
PlotParams P_jet0_energy				= {"jet0_energy", "Leading Jet Energy", "E [GeV]", 0, 250 };
PlotParams P_jet0_pt					= {"jet0_pt", "Leading Jet p_{T}", "p_{T} [GeV]", 0, 250 };
PlotParams P_jet0_energyFrac_depth1		= {"jet0_energyFrac_depth1", "Leading Jet Energy Fraction in HCAL Depth 1", "Energy Fraction", 0, 1 };
PlotParams P_jet0_energyFrac_depth2		= {"jet0_energyFrac_depth2", "Leading Jet Energy Fraction in HCAL Depth 2", "Energy Fraction", 0, 1 };
PlotParams P_jet0_energyFrac_depth3		= {"jet0_energyFrac_depth3", "Leading Jet Energy Fraction in HCAL Depth 3", "Energy Fraction", 0, 1 };
PlotParams P_jet0_energyFrac_depth4		= {"jet0_energyFrac_depth4", "Leading Jet Energy Fraction in HCAL Depth 4", "Energy Fraction", 0, 1 };
PlotParams P_jet0_etaSpread				= {"jet0_etaSpread", "Leading Jet #eta Spread", "#eta Spread", 0, 0.4 };
PlotParams P_jet0_etaSpread_energy		= {"jet0_etaSpread_energy", "Leading Jet #eta Spread Energy", "#eta Spread (energy weighted)", 0, 0.2 };
PlotParams P_jet0_phiSpread				= {"jet0_phiSpread", "Leading Jet #phi Spread", "#phi Spread", 0, 0.4 };
PlotParams P_jet0_phiSpread_energy		= {"jet0_phiSpread_energy", "Leading Jet #phi Spread Energy", "#phi Spread (energy weighted)", 0, 0.2 };
PlotParams P_jet0_track0Pt				= {"jet0_track0Pt", "Leading Jet: Leading Track p_{T}", "p_{T} [GeV]", 0, 15 };
PlotParams P_jet0_track1Pt				= {"jet0_track1Pt", "Leading Jet: Subeading Track p_{T}", "p_{T} [GeV]", 0, 5 };
PlotParams P_jet0_track2Pt				= {"jet0_track2Pt", "Leading Jet: Third leading Track p_{T}", "p_{T} [GeV]", 0, 5 };
//PlotParams P_jet0_track0dzToPV		= {"jet0_track0dzToPV", "Leading Jet: Leading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
//PlotParams P_jet0_track1dzToPV		= {"jet0_track1dzToPV", "Leading Jet: Subeading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
//PlotParams P_jet0_track2dzToPV		= {"jet0_track2dzToPV", "Leading Jet: Third Leading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };

// --------------------------------------------------------------------- //
// Subleading jet
PlotParams P_jet1_E						= {"jet1_E", "Sub-Leading Jet Energy", "E [GeV] (sub-leading jet)", 0, 250 };
PlotParams P_jet1_Pt					= {"jet1_Pt", "Sub-Leading Jet p_{T}", "p_{T} [GeV] (sub-leading jet)", 0, 250 };
PlotParams P_jet1_EtaSpread				= {"jet1_EtaSpread", "Sub-Leading Jet #eta Spread", "#eta Spread", 0, 0.4 };
PlotParams P_jet1_EtaSpread_energy		= {"jet1_EtaSpread_energy", "Sub-Leading Jet #eta Spread Energy", "#eta Spread (energy weighted)", 0, 0.2 };
PlotParams P_jet1_PhiSpread				= {"jet1_PhiSpread", "Sub-Leading Jet #phi Spread", "#phi Spread", 0, 0.4 };
PlotParams P_jet1_PhiSpread_energy		= {"jet1_PhiSpread_energy", "Sub-Leading Jet #phi Spread Energy", "#phi Spread (energy weighted)", 0, 0.2 };
PlotParams P_jet1_EtaPhiQuadSpread		= {"jet1_EtaPhiQuadSpread", "Leading Jet #sqrt{#Delta#eta^2 + #Delta#phi^2} Spread", "#sqrt{#Delta#eta^2 + #Delta#phi^2} Spread", 0, 0.4 };
PlotParams P_jet1_EtaPhiQuadSpread_energy={"jet1_EtaPhiQuadSpread_energy", "Leading Jet #sqrt{#Delta#eta^2 + #Delta#phi^2} Spread Energy", "#sqrt{#Delta#eta^2 + #Delta#phi^2} Spread (energy weighted)", 0, 0.2 };
PlotParams P_jet1_NeutralHadEFrac		= {"jet1_NeutralHadEFrac", "Sub-Leading Jet: Neutral Hadron Energy Fraction", "Neutral Hadron Energy Fraction", 0, 1 };
PlotParams P_jet1_ChargedHadEFrac		= {"jet1_ChargedHadEFrac", "Sub-Leading Jet: Charged Hadron Energy Fraction", "Charged Hadron Energy Fraction", 0, 1 };
PlotParams P_jet1_PhoEFrac				= {"jet1_PhoEFrac", "Sub-Leading Jet: Photon Energy Fraction", "Photon Energy Fraction", 0, 1 };
PlotParams P_jet1_EleEFrac				= {"jet1_EleEFrac", "Sub-Leading Jet: Electromagnetic Energy Fraction", "Electromagnetic Energy Fraction", 0, 1 };
PlotParams P_jet1_MuonEFrac				= {"jet1_MuonEFrac", "Sub-Leading Jet: Muon Energy Fraction", "Muon Energy Fraction", 0, 1 };
PlotParams P_jet1_HoverE				= {"jet1_HoverE", "Sub-Leading Jet: H/E Energy Ratio", "H/E Energy Ratio", 0, 100 };
PlotParams P_jet1_NeutralOverChargedHad = {"jet1_NeutralHadEFrac/jet0_ChargedHadEFrac", "Sub-Leading Jet: Neutral / Charged Hadronic Energy Fraction", "Neutral / Charged Hadronic Energy Fraction", 0, 100};

// --------------------------------------------------------------------- //
// Event Quantities
PlotParams P_met_Pt						= {"met_Pt", "MET p_{T}", "p_{T} [GeV]", 0, 400 };
PlotParams P_met_Phi					= {"met_Phi", "MET #phi", "#phi", -3.2, 3.2 };
PlotParams P_met_SumEt					= {"met_SumEt", "MET Sum E_{T}", "E_{T} [GeV]", 0, 5000 };

// --------------------------------------------------------------------- //
// LLP variables 
// Leading LLP
PlotParams P_LLP0_Eta					= {"LLP0_Eta", "LLP0 #eta", "#eta", -1.5, 1.5 };
PlotParams P_LLP0_Phi					= {"LLP0_Phi", "LLP0 #phi", "#phi", -3.2, 3.2 };
PlotParams P_LLP0_Pt					= {"LLP0_Pt", "LLP0 p_{T}", "p_{T} [GeV]", 0, 250 };
PlotParams P_LLP0_E						= {"LLP0_E", "LLP0 Energy", "E [GeV]", 0, 250 };
PlotParams P_LLP0_Beta					= {"LLP0_Beta", "LLP0 $beta", "$beta", 0, 250 };
PlotParams P_LLP0_TravelTime			= {"LLP0_TravelTime", "LLP0 Travel Time", "Travel Time [ns]", 0, 250 };
PlotParams P_LLP0_DecayR				= {"LLP0_DecayR", "LLP0 decay radius", "Decay R [cm]", 0, 350 };
PlotParams P_LLP0_DecayX				= {"LLP0_DecayX", "LLP0 decay X position", "Decay X [cm]", -2000, 2000 };
PlotParams P_LLP0_DecayY				= {"LLP0_DecayY", "LLP0 decay Y position", "Decay Y [cm]", -2000, 2000 };
PlotParams P_LLP0_DecayZ				= {"LLP0_DecayZ", "LLP0 decay Z position", "Decay Z [cm]", -10000, 10000 };
PlotParams P_LLP0_DecayT				= {"LLP0_DecayT", "LLP0 decay time", "Decay time [ns]", 0, 10 };
PlotParams P_LLP0_DecayCtau				= {"LLP0_DecayCtau", "LLP0 c#tau", "c#tau [cm]", 0, 1000 };

PlotParams P_LLP0_dR_b_to_b				= {"LLP0_dR_b_to_b", "LLP0 dR between two decay products", "dR", 0, 2};
PlotParams P_LLP0_dEta_b_to_b			= {"LLP0_dEta_b_to_b", "LLP0 d#eta between two decay products", "d#eta", 0, 2};
PlotParams P_LLP0_dPhi_b_to_b			= {"LLP0_dPhi_b_to_b", "LLP0 d#phi between two decay products", "d#phi", 0, 2};
PlotParams P_LLP0_dR_LLP_to_b			= {"LLP0_dR_LLP_to_b", "LLP0 dR between LLP and decay products", "dR", 0, 2};

PlotParams P_LLP0_isTruthMatched_Eta	= {"LLP0_isTruthMatched_Eta", "#eta of Jet Matched to LLP0", "#eta", -1.5, 1.5};

// Sub-leading LLP
PlotParams P_LLP1_Eta					= {"LLP1_Eta", "LLP1 #eta", "#eta", -1.5, 1.5 };
PlotParams P_LLP1_Phi					= {"LLP1_Phi", "LLP1 #phi", "#phi", -3.2, 3.2 };
PlotParams P_LLP1_Pt					= {"LLP1_Pt", "LLP1 p_{T}", "p_{T} [GeV]", 0, 250 };
PlotParams P_LLP1_E						= {"LLP1_E", "LLP1 Energy", "E [GeV]", 0, 250 };
PlotParams P_LLP1_Beta					= {"LLP1_Beta", "LLP1 $beta", "$beta", 0, 250 };
PlotParams P_LLP1_TravelTime			= {"LLP1_TravelTime", "LLP1 Travel Time", "Travel Time [ns]", 0, 250 };
PlotParams P_LLP1_DecayR				= {"LLP1_DecayR", "LLP1 decay radius", "Decay R [cm]", 0, 350 };
PlotParams P_LLP1_DecayX				= {"LLP1_DecayX", "LLP1 decay X position", "Decay X [cm]", -2000, 2000 };
PlotParams P_LLP1_DecayY				= {"LLP1_DecayY", "LLP1 decay Y position", "Decay Y [cm]", -2000, 2000 };
PlotParams P_LLP1_DecayZ				= {"LLP1_DecayZ", "LLP1 decay Z position", "Decay Z [cm]", -10000, 10000 };
PlotParams P_LLP1_DecayT				= {"LLP1_DecayT", "LLP1 decay time", "Decay time [ns]", 0, 10 };
PlotParams P_LLP1_DecayCtau				= {"LLP1_DecayCtau", "LLP1 c#tau", "c#tau [cm]", 0, 1000 };