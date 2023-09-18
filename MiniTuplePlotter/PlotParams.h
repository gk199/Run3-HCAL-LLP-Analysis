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

PlotParams P_jet0_RechitN				= {"jet0_RechitN", "Number of HCAL RecHits", "N^{HCAL}_{RecHits}", 0, 200 };
PlotParams P_jet0_E						= {"jet0_E", "Leading Jet Energy", "E [GeV]", 0, 250 };
PlotParams P_jet0_Pt					= {"jet0_Pt", "Leading Jet p_{T}", "p_{T} [GeV]", 0, 250 };
PlotParams P_jet0_Eta					= {"jet0_Eta", "Leading Jet #eta", "#eta", -1.5, 1.5 };
PlotParams P_jet0_Phi					= {"jet0_Phi", "Leading Jet #phi", "$phi", -3.2, 3.2 };
PlotParams P_jet0_EnergyFrac_Depth1		= {"jet0_EnergyFrac_Depth1", "Leading Jet Energy Fraction in HCAL Depth 1", "Energy Fraction", 0, 1 };
PlotParams P_jet0_EnergyFrac_Depth2		= {"jet0_EnergyFrac_Depth2", "Leading Jet Energy Fraction in HCAL Depth 2", "Energy Fraction", 0, 1 };
PlotParams P_jet0_EnergyFrac_Depth3		= {"jet0_EnergyFrac_Depth3", "Leading Jet Energy Fraction in HCAL Depth 3", "Energy Fraction", 0, 1 };
PlotParams P_jet0_EnergyFrac_Depth4		= {"jet0_EnergyFrac_Depth4", "Leading Jet Energy Fraction in HCAL Depth 4", "Energy Fraction", 0, 1 };
PlotParams P_jet0_isTruthMatched		= {"jet0_isTruthMatched", "Leading Jet is Truth Matched", "is Truth Matched", 0, 2 };
PlotParams P_jet0_EtaSpread				= {"jet0_EtaSpread", "Leading Jet #eta Spread", "#eta Spread", 0, 0.4 };
PlotParams P_jet0_EtaSpread_energy		= {"jet0_EtaSpread_energy", "Leading Jet #eta Spread Energy", "#eta Spread (energy weighted)", 0, 0.2 };
PlotParams P_jet0_PhiSpread				= {"jet0_PhiSpread", "Leading Jet #phi Spread", "#phi Spread", 0, 0.4 };
PlotParams P_jet0_PhiSpread_energy		= {"jet0_PhiSpread_energy", "Leading Jet #phi Spread Energy", "#phi Spread (energy weighted)", 0, 0.2 };
PlotParams P_jet0_EtaPhiQuadSpread		= {"jet0_EtaPhiQuadSpread", "Leading Jet #sqrt{#eta^{2} + #phi^{2}} Spread", "#sqrt{#eta^{2} + #phi^{2}} Spread", 0, 0.4 };
PlotParams P_jet0_EtaPhiQuadSpread_energy={"jet0_EtaPhiQuadSpread_energy", "Leading Jet #sqrt{#eta^{2} + #phi^{2}} Spread Energy", "#sqrt{#eta^{2} + #phi^{2}} Spread (energy weighted)", 0, 0.2 };
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
PlotParams P_jet0_NeutralHadEFrac		= {"jet0_NeutralHadEFrac", "Leading Jet Neutral Hadron Energy Fraction", "Neutral Hadron Energy Fraction", 0, 1 };
PlotParams P_jet0_ChargedHadEFrac		= {"jet0_ChargedHadEFrac", "Leading Jet Charged Hadron Energy Fraction", "Charged Hadron Energy Fraction", 0, 1 };
PlotParams P_jet0_PhoEFrac				= {"jet0_PhoEFrac", "Leading Jet Photon Energy Fraction", "Photon Energy Fraction", 0, 1 };
PlotParams P_jet0_EleEFrac				= {"jet0_EleEFrac", "Leading Jet Electromagnetic Energy Fraction", "Electromagnetic Energy Fraction", 0, 1 };
PlotParams P_jet0_MuonEFrac				= {"jet0_MuonEFrac", "Leading Jet Muon Energy Fraction", "Muon Energy Fraction", 0, 1 };
PlotParams P_jet0_HoverE				= {"jet0_HoverE", "Leading Jet H/E Energy Ratio", "H/E Energy Ratio", 0, 40 };

// below ones are for example files, v1 minituples have changed variable names as used above
PlotParams P_jet0_rechitN				= {"jet0_rechitN", "Number of HCAL RecHits", "N^{HCAL}_{RecHits}", 0, 200 };
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

// Subleading jet
PlotParams P_jet1_E						= {"jet1_E", "Sub-Leading Jet Energy", "E [GeV] (sub-leading jet)", 0, 250 };
PlotParams P_jet1_Pt					= {"jet1_Pt", "Sub-Leading Jet p_{T}", "p_{T} [GeV] (sub-leading jet)", 0, 250 };
PlotParams P_jet1_EtaSpread				= {"jet1_EtaSpread", "Sub-Leading Jet #eta Spread", "#eta Spread", 0, 0.4 };
PlotParams P_jet1_EtaSpread_energy		= {"jet1_EtaSpread_energy", "Sub-Leading Jet #eta Spread Energy", "#eta Spread (energy weighted)", 0, 0.2 };
PlotParams P_jet1_PhiSpread				= {"jet1_PhiSpread", "Sub-Leading Jet #phi Spread", "#phi Spread", 0, 0.4 };
PlotParams P_jet1_PhiSpread_energy		= {"jet1_PhiSpread_energy", "Sub-Leading Jet #phi Spread Energy", "#phi Spread (energy weighted)", 0, 0.2 };
PlotParams P_jet1_EtaPhiQuadSpread		= {"jet1_EtaPhiQuadSpread", "Leading Jet #sqrt{#eta^2 + #phi^2} Spread", "#sqrt{#eta^2 + #phi^2} Spread", 0, 0.4 };
PlotParams P_jet1_EtaPhiQuadSpread_energy={"jet1_EtaPhiQuadSpread_energy", "Leading Jet #sqrt{#eta^2 + #phi^2} Spread Energy", "#sqrt{#eta^2 + #phi^2} Spread (energy weighted)", 0, 0.2 };
PlotParams P_jet1_NeutralHadEFrac		= {"jet1_NeutralHadEFrac", "Sub-Leading Jet Neutral Hadron Energy Fraction", "Neutral Hadron Energy Fraction", 0, 1 };
PlotParams P_jet1_ChargedHadEFrac		= {"jet1_ChargedHadEFrac", "Sub-Leading Jet Charged Hadron Energy Fraction", "Charged Hadron Energy Fraction", 0, 1 };
PlotParams P_jet1_PhoEFrac				= {"jet1_PhoEFrac", "Sub-Leading Jet Photon Energy Fraction", "Photon Energy Fraction", 0, 1 };
PlotParams P_jet1_EleEFrac				= {"jet1_EleEFrac", "Sub-Leading Jet Electromagnetic Energy Fraction", "Electromagnetic Energy Fraction", 0, 1 };
PlotParams P_jet1_MuonEFrac				= {"jet1_MuonEFrac", "Sub-Leading Jet Muon Energy Fraction", "Muon Energy Fraction", 0, 1 };
PlotParams P_jet1_HoverE				= {"jet1_HoverE", "Sub-Leading Jet H/E Energy Ratio", "H/E Energy Ratio", 0, 100 };

// Event Quantities
PlotParams P_met_Pt						= {"met_Pt", "MET p_{T}", "p_{T} [GeV]", 0, 400 };
PlotParams P_met_Phi					= {"met_Phi", "MET #phi", "#phi", -3.2, 3.2 };
PlotParams P_met_SumEt					= {"met_SumEt", "MET Sum E_{T}", "E_{T} [GeV]", 0, 5000 };

// LLP variables 
// Leading LLP
PlotParams P_LLP0_Eta					= {"LLP0_Eta", "LLP0 #eta", "#eta", -1.5, 1.5 };
PlotParams P_LLP0_Phi					= {"LLP0_Phi", "LLP0 #phi", "$phi", -3.2, 3.2 };
PlotParams P_LLP0_Pt					= {"LLP0_Pt", "LLP0 p_{T}", "p_{T} [GeV]", 0, 250 };
PlotParams P_LLP0_E						= {"LLP0_E", "LLP0 Energy", "E [GeV]", 0, 250 };
PlotParams P_LLP0_Beta					= {"LLP0_Beta", "LLP0 $beta", "$beta", 0, 250 };
PlotParams P_LLP0_TravelTime			= {"LLP0_TravelTime", "LLP0 Travel Time", "Travel Time [ns]", 0, 250 };
PlotParams P_LLP0_DecayR				= {"LLP0_DecayR", "LLP0 decay radius", "Decay R [cm]", 0, 400 };
PlotParams P_LLP0_DecayX				= {"LLP0_DecayX", "LLP0 decay X position", "Decay X [cm]", -2000, 2000 };
PlotParams P_LLP0_DecayY				= {"LLP0_DecayY", "LLP0 decay Y position", "Decay Y [cm]", -2000, 2000 };
PlotParams P_LLP0_DecayZ				= {"LLP0_DecayZ", "LLP0 decay Z position", "Decay Z [cm]", -10000, 10000 };
PlotParams P_LLP0_DecayT				= {"LLP0_DecayT", "LLP0 decay time", "Decay time [ns]", 0, 10 };
PlotParams P_LLP0_DecayCtau				= {"LLP0_DecayCtau", "LLP0 c#tau", "c#tau [cm]", 0, 1000 };

// Sub-leading LLP
PlotParams P_LLP1_Eta					= {"LLP1_Eta", "LLP1 #eta", "#eta", -1.5, 1.5 };
PlotParams P_LLP1_Phi					= {"LLP1_Phi", "LLP1 #phi", "$phi", -3.2, 3.2 };
PlotParams P_LLP1_Pt					= {"LLP1_Pt", "LLP1 p_{T}", "p_{T} [GeV]", 0, 250 };
PlotParams P_LLP1_E						= {"LLP1_E", "LLP1 Energy", "E [GeV]", 0, 250 };
PlotParams P_LLP1_Beta					= {"LLP1_Beta", "LLP1 $beta", "$beta", 0, 250 };
PlotParams P_LLP1_TravelTime			= {"LLP1_TravelTime", "LLP1 Travel Time", "Travel Time [ns]", 0, 250 };
PlotParams P_LLP1_DecayR				= {"LLP1_DecayR", "LLP1 decay radius", "Decay R [cm]", 0, 400 };
PlotParams P_LLP1_DecayX				= {"LLP1_DecayX", "LLP1 decay X position", "Decay X [cm]", -2000, 2000 };
PlotParams P_LLP1_DecayY				= {"LLP1_DecayY", "LLP1 decay Y position", "Decay Y [cm]", -2000, 2000 };
PlotParams P_LLP1_DecayZ				= {"LLP1_DecayZ", "LLP1 decay Z position", "Decay Z [cm]", -10000, 10000 };
PlotParams P_LLP1_DecayT				= {"LLP1_DecayT", "LLP1 decay time", "Decay time [ns]", 0, 10 };
PlotParams P_LLP1_DecayCtau				= {"LLP1_DecayCtau", "LLP1 c#tau", "c#tau [cm]", 0, 1000 };