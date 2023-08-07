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

PlotParams jet0_RechitN				= {"jet0_RechitN", "Number of HCAL RecHits", "N^{HCAL}_{RecHits}", 0, 200 };
PlotParams jet0_E					= {"jet0_E", "Leading Jet Energy", "E [GeV]", 0, 250 };
PlotParams jet0_Pt					= {"jet0_Pt", "Leading Jet p_{T}", "p_{T} [GeV]", 0, 250 };
PlotParams jet0_Eta					= {"jet0_Eta", "Leading Jet #eta", "#eta", -1.5, 1.5 };
PlotParams jet0_Phi					= {"jet0_Phi", "Leading Jet #phi", "$phi", -3.2, 3.2 };
PlotParams jet0_isTruthMatched		= {"jet0_isTruthMatched", "Leading Jet is Truth Matched", "is Truth Matched", 0, 2 };
PlotParams jet0_EtaSpread			= {"jet0_EtaSpread", "Leading Jet #eta Spread", "#eta Spread", 0, 0.4 };
PlotParams jet0_EtaSpread_energy	= {"jet0_EtaSpread_energy", "Leading Jet #eta Spread Energy", "XAXIS [UNITS]", 0, 0.2 };
PlotParams jet0_PhiSpread			= {"jet0_PhiSpread", "Leading Jet #phi Spread", "#phi Spread", 0, 0.4 };
PlotParams jet0_PhiSpread_energy	= {"jet0_PhiSpread_energy", "Leading Jet #phi Spread Energy", "XAXIS [UNITS]", 0, 0.2 };
PlotParams jet0_Track0Pt			= {"jet0_Track0Pt", "Leading Jet - Leading Track p_{T}", "p_{T} [GeV]", 0, 15 };
PlotParams jet0_Track1Pt			= {"jet0_Track1Pt", "Leading Jet - Subeading Track p_{T}", "p_{T} [GeV]", 0, 5 };
PlotParams jet0_Track2Pt			= {"jet0_Track2Pt", "Leading Jet - Third leading Track p_{T}", "p_{T} [GeV]", 0, 5 };
//PlotParams jet0_track0dzToPV		= {"jet0_track0dzToPV", "Leading Jet - Leading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
//PlotParams jet0_track1dzToPV		= {"jet0_track1dzToPV", "Leading Jet - Subeading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
//PlotParams jet0_track2dzToPV		= {"jet0_track2dzToPV", "Leading Jet - Third Leading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
PlotParams jet0_EnergyFrac_Depth1	= {"jet0_EnergyFrac_Depth1", "Leading Jet Energy Fraction in HCAL Depth 1", "Energy Fraction", 0, 1 };
PlotParams jet0_EnergyFrac_Depth2	= {"jet0_EnergyFrac_Depth2", "Leading Jet Energy Fraction in HCAL Depth 2", "Energy Fraction", 0, 1 };
PlotParams jet0_EnergyFrac_Depth3	= {"jet0_EnergyFrac_Depth3", "Leading Jet Energy Fraction in HCAL Depth 3", "Energy Fraction", 0, 1 };
PlotParams jet0_EnergyFrac_Depth4	= {"jet0_EnergyFrac_Depth4", "Leading Jet Energy Fraction in HCAL Depth 4", "Energy Fraction", 0, 1 };
// below ones are for example files, v1 minituples have changed variable names as used above
PlotParams jet0_rechitN				= {"jet0_rechitN", "Number of HCAL RecHits", "N^{HCAL}_{RecHits}", 0, 200 };
PlotParams jet0_energy				= {"jet0_energy", "Leading Jet Energy", "E [GeV]", 0, 250 };
PlotParams jet0_pt					= {"jet0_pt", "Leading Jet p_{T}", "p_{T} [GeV]", 0, 250 };
PlotParams jet0_energyFrac_depth1	= {"jet0_energyFrac_depth1", "Leading Jet Energy Fraction in HCAL Depth 1", "Energy Fraction", 0, 1 };
PlotParams jet0_energyFrac_depth2	= {"jet0_energyFrac_depth2", "Leading Jet Energy Fraction in HCAL Depth 2", "Energy Fraction", 0, 1 };
PlotParams jet0_energyFrac_depth3	= {"jet0_energyFrac_depth3", "Leading Jet Energy Fraction in HCAL Depth 3", "Energy Fraction", 0, 1 };
PlotParams jet0_energyFrac_depth4	= {"jet0_energyFrac_depth4", "Leading Jet Energy Fraction in HCAL Depth 4", "Energy Fraction", 0, 1 };
PlotParams jet0_etaSpread			= {"jet0_etaSpread", "Leading Jet #eta Spread", "#eta Spread", 0, 0.4 };
PlotParams jet0_etaSpread_energy	= {"jet0_etaSpread_energy", "Leading Jet #eta Spread Energy", "XAXIS [UNITS]", 0, 0.2 };
PlotParams jet0_phiSpread			= {"jet0_phiSpread", "Leading Jet #phi Spread", "#phi Spread", 0, 0.4 };
PlotParams jet0_phiSpread_energy	= {"jet0_phiSpread_energy", "Leading Jet #phi Spread Energy", "XAXIS [UNITS]", 0, 0.2 };
PlotParams jet0_track0Pt			= {"jet0_track0Pt", "Leading Jet - Leading Track p_{T}", "p_{T} [GeV]", 0, 15 };
PlotParams jet0_track1Pt			= {"jet0_track1Pt", "Leading Jet - Subeading Track p_{T}", "p_{T} [GeV]", 0, 5 };
PlotParams jet0_track2Pt			= {"jet0_track2Pt", "Leading Jet - Third leading Track p_{T}", "p_{T} [GeV]", 0, 5 };
//PlotParams jet0_track0dzToPV		= {"jet0_track0dzToPV", "Leading Jet - Leading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
//PlotParams jet0_track1dzToPV		= {"jet0_track1dzToPV", "Leading Jet - Subeading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
//PlotParams jet0_track2dzToPV		= {"jet0_track2dzToPV", "Leading Jet - Third Leading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };