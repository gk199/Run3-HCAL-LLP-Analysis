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

PlotParams P_jet0_rechitN				= {"jet0_rechitN", "Number of HCAL RecHits", "N^{HCAL}_{RecHits}", 0, 200 };
PlotParams P_jet0_energy				= {"jet0_energy", "Leading Jet Energy", "E [GeV]", 0, 250 };
PlotParams P_jet0_pt					= {"jet0_pt", "Leading Jet p_{T}", "p_{T} [GeV]", 0, 250 };
PlotParams P_jet0_Eta					= {"jet0_Eta", "Leading Jet #eta", "#eta", -1.5, 1.5 };
PlotParams P_jet0_Phi					= {"jet0_Phi", "Leading Jet #phi", "$phi", -3.2, 3.2 };
PlotParams P_jet0_isTruthMatched		= {"jet0_isTruthMatched", "Leading Jet is Truth Matched", "is Truth Matched", 0, 2 };
PlotParams P_jet0_etaSpread			= {"jet0_etaSpread", "Leading Jet #eta Spread", "#eta Spread", 0, 0.4 };
PlotParams P_jet0_etaSpread_energy	= {"jet0_etaSpread_energy", "Leading Jet #eta Spread Energy", "XAXIS [UNITS]", 0, 0.2 };
PlotParams P_jet0_phiSpread			= {"jet0_phiSpread", "Leading Jet #phi Spread", "#phi Spread", 0, 0.4 };
PlotParams P_jet0_phiSpread_energy	= {"jet0_phiSpread_energy", "Leading Jet #phi Spread Energy", "XAXIS [UNITS]", 0, 0.2 };
PlotParams P_jet0_track0Pt			= {"jet0_track0Pt", "Leading Jet - Leading Track p_{T}", "p_{T} [GeV]", 0, 15 };
PlotParams P_jet0_track1Pt			= {"jet0_track1Pt", "Leading Jet - Subeading Track p_{T}", "p_{T} [GeV]", 0, 5 };
PlotParams P_jet0_track2Pt			= {"jet0_track2Pt", "Leading Jet - Third leading Track p_{T}", "p_{T} [GeV]", 0, 5 };
//PlotParams P_jet0_track0dzToPV		= {"jet0_track0dzToPV", "Leading Jet - Leading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
//PlotParams P_jet0_track1dzToPV		= {"jet0_track1dzToPV", "Leading Jet - Subeading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
//PlotParams P_jet0_track2dzToPV		= {"jet0_track2dzToPV", "Leading Jet - Third Leading Track #DeltaZ to PV", "#DeltaZ [cm]", MIN, MAX };
PlotParams P_jet0_energyFrac_depth1	= {"jet0_energyFrac_depth1", "Leading Jet Energy Fraction in HCAL Depth 1", "Energy Fraction", 0, 1 };
PlotParams P_jet0_energyFrac_depth2	= {"jet0_energyFrac_depth2", "Leading Jet Energy Fraction in HCAL Depth 2", "Energy Fraction", 0, 1 };
PlotParams P_jet0_energyFrac_depth3	= {"jet0_energyFrac_depth3", "Leading Jet Energy Fraction in HCAL Depth 3", "Energy Fraction", 0, 1 };
PlotParams P_jet0_energyFrac_depth4	= {"jet0_energyFrac_depth4", "Leading Jet Energy Fraction in HCAL Depth 4", "Energy Fraction", 0, 1 };