// -------------------------------------------------------------------------------------
struct PlotParams{
	string hist_name, title, label_x;
	double xmin, xmax;
};
typedef struct PlotParams PlotParams;

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
PlotParams P_jet0_EtaSpread_energy		= {"jet0_EtaSpread_energy", "Leading Jet #eta Spread Energy", "XAXIS [UNITS]", 0, 0.2 };
PlotParams P_jet0_PhiSpread				= {"jet0_PhiSpread", "Leading Jet #phi Spread", "#phi Spread", 0, 0.4 };
PlotParams P_jet0_PhiSpread_energy		= {"jet0_PhiSpread_energy", "Leading Jet #phi Spread Energy", "XAXIS [UNITS]", 0, 0.2 };
PlotParams P_jet0_Track0Pt				= {"jet0_Track0Pt", "Leading Jet - Leading Track p_{T}", "p_{T} [GeV]", 0, 15 };
PlotParams P_jet0_Track1Pt				= {"jet0_Track1Pt", "Leading Jet - Subeading Track p_{T}", "p_{T} [GeV]", 0, 5 };
PlotParams P_jet0_Track2Pt				= {"jet0_Track2Pt", "Leading Jet - Third leading Track p_{T}", "p_{T} [GeV]", 0, 5 };
//PlotParams P_jet0_Track0dzToPV		= {"jet0_Track0dzToPV", "Leading Jet - Leading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
//PlotParams P_jet0_Track1dzToPV		= {"jet0_Track1dzToPV", "Leading Jet - Subeading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
//PlotParams P_jet0_Track2dzToPV		= {"jet0_Track2dzToPV", "Leading Jet - Third Leading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
// below ones are for example files, v1 minituples have changed variable names as used above
PlotParams P_jet0_rechitN				= {"jet0_rechitN", "Number of HCAL RecHits", "N^{HCAL}_{RecHits}", 0, 200 };
PlotParams P_jet0_energy				= {"jet0_energy", "Leading Jet Energy", "E [GeV]", 0, 250 };
PlotParams P_jet0_pt					= {"jet0_pt", "Leading Jet p_{T}", "p_{T} [GeV]", 0, 250 };
PlotParams P_jet0_energyFrac_depth1		= {"jet0_energyFrac_depth1", "Leading Jet Energy Fraction in HCAL Depth 1", "Energy Fraction", 0, 1 };
PlotParams P_jet0_energyFrac_depth2		= {"jet0_energyFrac_depth2", "Leading Jet Energy Fraction in HCAL Depth 2", "Energy Fraction", 0, 1 };
PlotParams P_jet0_energyFrac_depth3		= {"jet0_energyFrac_depth3", "Leading Jet Energy Fraction in HCAL Depth 3", "Energy Fraction", 0, 1 };
PlotParams P_jet0_energyFrac_depth4		= {"jet0_energyFrac_depth4", "Leading Jet Energy Fraction in HCAL Depth 4", "Energy Fraction", 0, 1 };
PlotParams P_jet0_etaSpread				= {"jet0_etaSpread", "Leading Jet #eta Spread", "#eta Spread", 0, 0.4 };
PlotParams P_jet0_etaSpread_energy		= {"jet0_etaSpread_energy", "Leading Jet #eta Spread Energy", "XAXIS [UNITS]", 0, 0.2 };
PlotParams P_jet0_phiSpread				= {"jet0_phiSpread", "Leading Jet #phi Spread", "#phi Spread", 0, 0.4 };
PlotParams P_jet0_phiSpread_energy		= {"jet0_phiSpread_energy", "Leading Jet #phi Spread Energy", "XAXIS [UNITS]", 0, 0.2 };
PlotParams P_jet0_track0Pt				= {"jet0_track0Pt", "Leading Jet - Leading Track p_{T}", "p_{T} [GeV]", 0, 15 };
PlotParams P_jet0_track1Pt				= {"jet0_track1Pt", "Leading Jet - Subeading Track p_{T}", "p_{T} [GeV]", 0, 5 };
PlotParams P_jet0_track2Pt				= {"jet0_track2Pt", "Leading Jet - Third leading Track p_{T}", "p_{T} [GeV]", 0, 5 };
//PlotParams P_jet0_track0dzToPV		= {"jet0_track0dzToPV", "Leading Jet - Leading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
//PlotParams P_jet0_track1dzToPV		= {"jet0_track1dzToPV", "Leading Jet - Subeading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
//PlotParams P_jet0_track2dzToPV		= {"jet0_track2dzToPV", "Leading Jet - Third Leading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };


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


