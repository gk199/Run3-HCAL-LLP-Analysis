TString OR		   	= " || ";
TString AND 		= " && ";

// Setup cuts on HLT paths passed
TCut Cut_HLTpassedHT350 = "HLT_HT350 == 1";

TCut Cut_None			= "";
TCut Cut_HLTpassed1 	= "HLT_L1SingleLLPJet == 1"; // monitoring trigger // not in v1 ntuples yet
TCut Cut_HLTpassed2 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1";
TCut Cut_HLTpassed3 	= "HLT_HT240_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1"; 
TCut Cut_HLTpassed4 	= "HLT_HT280_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1"; // end of group 1 of triggers 
TCut Cut_HLTpassed5 	= "HLT_HT170_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1"; 
TCut Cut_HLTpassed6 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1";
TCut Cut_HLTpassed7 	= "HLT_HT270_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1";
TCut Cut_HLTpassed8 	= "HLT_HT200_L1SingleLLPJet_DisplacedDijet60_DisplacedTrack == 1"; // end of group 2 of triggers
TCut Cut_HLTpassed9 	= "HLT_HT320_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1";
TCut Cut_HLTpassed10 	= "HLT_HT420_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1"; // end of group 3 of triggers
TCut Cut_HLTpassed11 	= "HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay0p5nsTrackless == 1";
TCut Cut_HLTpassed12 	= "HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay1nsInclusive == 1"; 
TCut Cut_HLTpassed13 	= "HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay1nsTrackless == 1";
TCut Cut_HLTpassed14 	= "HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay2nsInclusive == 1"; // end of group 4 of triggers

TCut Cut_AnyLLP_HLT     = "HLT_HT200_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT240_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT280_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT170_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT270_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet60_DisplacedTrack == 1 || HLT_HT320_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 || HLT_HT420_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay0p5nsTrackless == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay1nsInclusive == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay1nsTrackless == 1 || HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay2nsInclusive == 1";
TCut Cut_AnyLLP_HLT_noECAL     = "HLT_HT200_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT240_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT280_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5 == 1 || HLT_HT170_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT270_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT200_L1SingleLLPJet_DisplacedDijet60_DisplacedTrack == 1 || HLT_HT320_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 || HLT_HT420_L1SingleLLPJet_DisplacedDijet60_Inclusive == 1 ";

TCut Cut_Run2_HLTs      = "HLT_HT400_DisplacedDijet40_DisplacedTrack == 1 || HLT_HT430_DelayedJet40_DoubleDelay0p5nsInclusive == 1 || HLT_HT430_DelayedJet40_DoubleDelay0p5nsTrackless == 1 || HLT_HT430_DelayedJet40_DoubleDelay0p75nsTrackless == 1 || HLT_HT430_DelayedJet40_DoubleDelay1nsInclusive == 1 || HLT_HT430_DelayedJet40_DoubleDelay1nsTrackless == 1 || HLT_HT430_DelayedJet40_DoubleDelay1p25nsInclusive == 1 || HLT_HT430_DelayedJet40_DoubleDelay1p5nsInclusive == 1 || HLT_HT430_DelayedJet40_SingleDelay0p5nsInclusive == 1 || HLT_HT430_DelayedJet40_SingleDelay0p5nsTrackless == 1 || HLT_HT430_DelayedJet40_SingleDelay1nsInclusive == 1 || HLT_HT430_DelayedJet40_SingleDelay1nsTrackless == 1 || HLT_HT430_DelayedJet40_SingleDelay1p25nsTrackless == 1 || HLT_HT430_DelayedJet40_SingleDelay1p5nsInclusive == 1 || HLT_HT430_DelayedJet40_SingleDelay1p5nsTrackless == 1 || HLT_HT430_DelayedJet40_SingleDelay2nsInclusive == 1 || HLT_HT430_DelayedJet40_SingleDelay2p25nsInclusive == 1 || HLT_HT430_DelayedJet40_SingleDelay2p5nsInclusive == 1 || HLT_HT550_DisplacedDijet60_Inclusive == 1 || HLT_HT350_DelayedJet40_SingleDelay3nsInclusive == 1 || HLT_HT350_DelayedJet40_SingleDelay3p25nsInclusive == 1 || HLT_HT350_DelayedJet40_SingleDelay3p5nsInclusive == 1";

TCut Cut_DisplacedJetHLT_OR = Cut_AnyLLP_HLT_noECAL || Cut_Run2_HLTs;

TCut Cut_HLT_dijet_1promptTrack         = Cut_HLTpassed4 || Cut_HLTpassed3 || Cut_HLTpassed2;                            // displaced dijet with at most 1 associated prompt track
TCut Cut_HLT_dijet_1displacedTrack      = Cut_HLTpassed5 || Cut_HLTpassed6 || Cut_HLTpassed7 || Cut_HLTpassed8;          // displaced dijet, one displaced track
TCut Cut_HLT_dijet_2promptTrack         = Cut_HLTpassed9 || Cut_HLTpassed10;                                             // displaced dijet, allow for 2 prompt tracks (higher jet and HT energies)
TCut Cut_HLT_delayedjet_ECALtiming      = Cut_HLTpassed11 || Cut_HLTpassed12 || Cut_HLTpassed13 || Cut_HLTpassed14;      // delayed jet, ECAL timing

// Setup cuts on jet matching results
TCut Cut_Jet0_isTruthMatched	= "jet0_isTruthMatched == 1";
TCut Cut_Jet1_isTruthMatched	= "jet1_isTruthMatched == 1";

// Setup cuts on LLP decay position
float radius_tracker[2]  = {0, 129};
float radius_ECAL[2]     = {129, 177}; // 48cm of ECAL
float radius_depth1[2]   = {177, 190.2};
float radius_depth2[2]   = {190.2, 214.2};
float radius_depth3[2]   = {214.2, 244.8};
float radius_depth4[2]   = {244.8, 295};
float radius_all[2]      = {0,9999};
float radius_inHCAL[2]   = {177, 295};
float radius_depth12[2]  = {177, 214.2};
float radius_depth34[2]  = {214.2, 295};

float HBeta = 1.26;
int z_pos = 425; // depth 1 and 2 have a z position < 388, depth 3 and 4 have z position < 425cm

// use prompt LLP as CR for now
float controlRegion = 10.0;
TString LLP0inCR 	= Form("(LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		controlRegion, HBeta);
TString LLP1inCR 	= Form("(LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		controlRegion, HBeta);

TString LLP0inSR 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR <= %0.1f && abs(LLP0_Eta) <= %f)", 		controlRegion, radius_depth34[1], HBeta);
TString LLP1inSR 	= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR <= %0.1f && abs(LLP1_Eta) <= %f)", 		controlRegion, radius_depth34[1], HBeta);

// LLP decaying further in detector is SR
TString LLP0inHCAL 	    = Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 	    radius_inHCAL[0],   radius_inHCAL[1],   HBeta);
TString LLP1inHCAL 	    = Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_inHCAL[0],   radius_inHCAL[1],   HBeta);

TString LLP0inTracker 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_tracker[0], 	radius_tracker[1], 	HBeta);
TString LLP0inTrackerNP = Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		controlRegion, 	    radius_tracker[1], 	HBeta);
TString LLP0inECAL 		= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_ECAL[0], 	radius_ECAL[1], 	HBeta);
TString LLP0inHCAL_d1 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_depth1[0], 	radius_depth1[1], 	HBeta);
TString LLP0inHCAL_d2 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_depth2[0], 	radius_depth2[1], 	HBeta);
TString LLP0inHCAL_d3 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_depth3[0], 	radius_depth3[1], 	HBeta);
TString LLP0inHCAL_d4 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_depth4[0], 	radius_depth4[1], 	HBeta);
TString LLP0inHCAL_d12 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_depth12[0], 	radius_depth12[1], 	HBeta);
TString LLP0inHCAL_d34 	= Form("(LLP0_DecayR >= %0.1f && LLP0_DecayR < %0.1f && abs(LLP0_Eta) <= %f)", 		radius_depth34[0], 	radius_depth34[1], 	HBeta);

TString LLP1inTracker 	= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_tracker[0], 	radius_tracker[1], 	HBeta);
TString LLP1inTrackerNP = Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		controlRegion, 	    radius_tracker[1], 	HBeta);
TString LLP1inECAL 		= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_ECAL[0], 	radius_ECAL[1], 	HBeta);
TString LLP1inHCAL_d1 	= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_depth1[0], 	radius_depth1[1], 	HBeta);
TString LLP1inHCAL_d2 	= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_depth2[0], 	radius_depth2[1], 	HBeta);
TString LLP1inHCAL_d3 	= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_depth3[0], 	radius_depth3[1], 	HBeta);
TString LLP1inHCAL_d4 	= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_depth4[0], 	radius_depth4[1], 	HBeta);
TString LLP1inHCAL_d12 	= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_depth12[0], 	radius_depth12[1], 	HBeta);
TString LLP1inHCAL_d34 	= Form("(LLP1_DecayR >= %0.1f && LLP1_DecayR < %0.1f && abs(LLP1_Eta) <= %f)", 		radius_depth34[0], 	radius_depth34[1], 	HBeta);

// determine which LLP the two leading jets are matched to
TString Jet0_LLP0 		= Form("jet0_isMatchedTo == 0");
TString Jet0_LLP1 		= Form("jet0_isMatchedTo == 1");
TString Jet1_LLP0 		= Form("jet1_isMatchedTo == 0");
TString Jet1_LLP1 		= Form("jet1_isMatchedTo == 1");
TString Jet2_LLP0 		= Form("jet2_isMatchedTo == 0");
TString Jet2_LLP1 		= Form("jet2_isMatchedTo == 1");
TString Jet3_LLP0 		= Form("jet3_isMatchedTo == 0");
TString Jet3_LLP1 		= Form("jet3_isMatchedTo == 1");
TString Jet4_LLP0 		= Form("jet4_isMatchedTo == 0");
TString Jet4_LLP1 		= Form("jet4_isMatchedTo == 1");
TString Jet5_LLP0 		= Form("jet5_isMatchedTo == 0");
TString Jet5_LLP1 		= Form("jet5_isMatchedTo == 1");

TCut Cut_LLPinHCAL 	= (LLP0inHCAL + OR + LLP1inHCAL).Data();
TCut Cut_LLP0inHCAL = LLP0inHCAL.Data();
TCut Cut_LLP1inHCAL = LLP1inHCAL.Data();

// at least one of LLPs is in region of interest
TCut Cut_LLPinTracker	= (LLP0inTracker + OR + LLP1inTracker).Data();
TCut Cut_LLP0inTracker 	= LLP0inTracker.Data();
TCut Cut_LLPinTrackerNP	= (LLP0inTrackerNP + OR + LLP1inTrackerNP).Data();
TCut Cut_LLP0inTrackerNP= LLP0inTrackerNP.Data();
TCut Cut_LLPinECAL 		= (LLP0inECAL + OR + LLP1inECAL).Data();
TCut Cut_LLP0inECAL 	= LLP0inECAL.Data();
TCut Cut_LLPinHCAL_d1 	= (LLP0inHCAL_d1 + OR + LLP1inHCAL_d1).Data();
TCut Cut_LLP0inHCAL_d1  = LLP0inHCAL_d1.Data();
TCut Cut_LLPinHCAL_d2 	= (LLP0inHCAL_d2 + OR + LLP1inHCAL_d2).Data();
TCut Cut_LLP0inHCAL_d2  = LLP0inHCAL_d2.Data();
TCut Cut_LLPinHCAL_d3 	= (LLP0inHCAL_d3 + OR + LLP1inHCAL_d3).Data();
TCut Cut_LLP0inHCAL_d3 	= LLP0inHCAL_d3.Data();
TCut Cut_LLPinHCAL_d4 	= (LLP0inHCAL_d4 + OR + LLP1inHCAL_d4).Data();
TCut Cut_LLP0inHCAL_d4 	= LLP0inHCAL_d4.Data();
TCut Cut_LLPinHCAL_d12 	= (LLP0inHCAL_d12 + OR + LLP1inHCAL_d12).Data();
TCut Cut_LLP0inHCAL_d12 = LLP0inHCAL_d12.Data();
TCut Cut_LLPinHCAL_d34 	= (LLP0inHCAL_d34 + OR + LLP1inHCAL_d34).Data();
TCut Cut_LLP0inHCAL_d34 = LLP0inHCAL_d34.Data();

TCut Cut_LLPinCR	    = (LLP0inCR + OR + LLP1inCR).Data();
TCut Cut_LLP0inCR       = (LLP0inCR).Data();

// LLP is in region of interest, and matched to a jet
TCut Cut_LLPinTracker_Jet0	= 	( "(" + LLP0inTracker + AND + Jet0_LLP0 + ")" + 	OR + "(" + LLP1inTracker + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinTracker_Jet1	= 	( "(" + LLP0inTracker + AND + Jet1_LLP0 + ")" + 	OR + "(" + LLP1inTracker + AND + Jet1_LLP1 + ")" ).Data();
TCut Cut_LLPinTrackerNP_Jet0= 	( "(" + LLP0inTrackerNP + AND + Jet0_LLP0 + ")" + 	OR + "(" + LLP1inTrackerNP + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinTrackerNP_Jet1= 	( "(" + LLP0inTrackerNP + AND + Jet1_LLP0 + ")" + 	OR + "(" + LLP1inTrackerNP + AND + Jet1_LLP1 + ")" ).Data();
TCut Cut_LLPinECAL_Jet0	    = 	( "(" + LLP0inECAL + AND + Jet0_LLP0 + ")" + 		OR + "(" + LLP1inECAL + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinECAL_Jet1	    = 	( "(" + LLP0inECAL + AND + Jet1_LLP0 + ")" + 		OR + "(" + LLP1inECAL + AND + Jet1_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL1_Jet0	= 	( "(" + LLP0inHCAL_d1 + AND + Jet0_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d1 + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL1_Jet1	= 	( "(" + LLP0inHCAL_d1 + AND + Jet1_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d1 + AND + Jet1_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL2_Jet0	= 	( "(" + LLP0inHCAL_d2 + AND + Jet0_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d2 + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL2_Jet1	= 	( "(" + LLP0inHCAL_d2 + AND + Jet1_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d2 + AND + Jet1_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL3_Jet0	= 	( "(" + LLP0inHCAL_d3 + AND + Jet0_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d3 + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL3_Jet1	= 	( "(" + LLP0inHCAL_d3 + AND + Jet1_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d3 + AND + Jet1_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL4_Jet0	= 	( "(" + LLP0inHCAL_d4 + AND + Jet0_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d4 + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL4_Jet1	= 	( "(" + LLP0inHCAL_d4 + AND + Jet1_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d4 + AND + Jet1_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL_Jet0	    = 	( "(" + LLP0inHCAL + AND + Jet0_LLP0 + ")" + 	    OR + "(" + LLP1inHCAL + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL_Jet1	    = 	( "(" + LLP0inHCAL + AND + Jet1_LLP0 + ")" + 	    OR + "(" + LLP1inHCAL + AND + Jet1_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL12_Jet0	= 	( "(" + LLP0inHCAL_d12 + AND + Jet0_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d12 + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL12_Jet1	= 	( "(" + LLP0inHCAL_d12 + AND + Jet1_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d12 + AND + Jet1_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL34_Jet0	= 	( "(" + LLP0inHCAL_d34 + AND + Jet0_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d34 + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL34_Jet1	= 	( "(" + LLP0inHCAL_d34 + AND + Jet1_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d34 + AND + Jet1_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL34_Jet2	= 	( "(" + LLP0inHCAL_d34 + AND + Jet2_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d34 + AND + Jet2_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL34_Jet3	= 	( "(" + LLP0inHCAL_d34 + AND + Jet3_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d34 + AND + Jet3_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL34_Jet4	= 	( "(" + LLP0inHCAL_d34 + AND + Jet4_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d34 + AND + Jet4_LLP1 + ")" ).Data();
TCut Cut_LLPinHCAL34_Jet5	= 	( "(" + LLP0inHCAL_d34 + AND + Jet5_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d34 + AND + Jet5_LLP1 + ")" ).Data();

// below jet cut is 100 for hard cut plots
TCut Cut_LLPinHCAL34_AnyJet = ( "( jet0_Pt > 0" + AND + "( (" + LLP0inHCAL_d34 + AND + Jet0_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d34 + AND + Jet0_LLP1 + ") ) )" + OR + 
                                "( jet1_Pt > 0" + AND + "( (" + LLP0inHCAL_d34 + AND + Jet1_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d34 + AND + Jet1_LLP1 + ") ) )" + OR + 
                                "( jet2_Pt > 0" + AND + "( (" + LLP0inHCAL_d34 + AND + Jet2_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d34 + AND + Jet2_LLP1 + ") ) )" + OR + 
                                "( jet3_Pt > 0" + AND + "( (" + LLP0inHCAL_d34 + AND + Jet3_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d34 + AND + Jet3_LLP1 + ") ) )" + OR + 
                                "( jet4_Pt > 0" + AND + "( (" + LLP0inHCAL_d34 + AND + Jet4_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d34 + AND + Jet4_LLP1 + ") ) )" + OR + 
                                "( jet5_Pt > 0" + AND + "( (" + LLP0inHCAL_d34 + AND + Jet5_LLP0 + ")" + 	OR + "(" + LLP1inHCAL_d34 + AND + Jet5_LLP1 + ") ) )" ).Data();



TCut Cut_LLPinHCAL123_Jet0	= 	( "( (" + LLP0inHCAL_d2 + OR + LLP0inHCAL_d34 + ")" + AND + Jet0_LLP0 + ")" + 	OR + "( (" + LLP1inHCAL_d2 + OR + LLP1inHCAL_d34 + ")" + AND + Jet0_LLP1 + ")" ).Data();

TCut Cut_LLPinCR_Jet0	    = 	( "(" + LLP0inCR + AND + Jet0_LLP0 + ")" + 	        OR + "(" + LLP1inCR + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinCR_Jet1	    = 	( "(" + LLP0inCR + AND + Jet1_LLP0 + ")" + 	        OR + "(" + LLP1inCR + AND + Jet1_LLP1 + ")" ).Data();

TCut Cut_LLPinCalo_Jet0	    = 	( "((" + LLP0inECAL + OR + LLP0inHCAL + ")" + AND + Jet0_LLP0 + ")" + 	        OR + "((" + LLP1inECAL + OR + LLP1inHCAL + ")" + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinSR_Jet0	    = 	( "(" + LLP0inSR + AND + Jet0_LLP0 + ")" + 	        OR + "(" + LLP1inSR + AND + Jet0_LLP1 + ")" ).Data();
TCut Cut_LLPinSR            = (LLP0inTrackerNP + OR + LLP0inECAL + OR + LLP0inHCAL + OR + LLP1inTrackerNP + OR + LLP1inECAL + OR + LLP1inHCAL).Data();

// Save these cuts in a map paired with a string so they can be iterated over, to easily make same plot with lots of different cuts  :) 
// used in MiniTuplePlotter_HLT_Effs.C
map<TCut, string> LLP_Cuts; // either LLP position
LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinTracker, "LLPinTracker"));
LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinECAL, "LLPinECAL"));
LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinHCAL_d1, "LLPinHCAL_depth1"));
LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinHCAL_d2, "LLPinHCAL_depth2"));
LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinHCAL_d3, "LLPinHCAL_depth3"));
LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinHCAL_d4, "LLPinHCAL_depth4"));
LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinHCAL_d12, "LLPinHCAL_depth12"));
LLP_Cuts.insert(pair<TCut, string>(Cut_LLPinHCAL_d34, "LLPinHCAL_depth34"));

map<TCut, string> LLP0_Cuts; // leading LLP position
LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inTracker, "LLP0inTracker"));
LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inECAL, "LLP0inECAL"));
LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inHCAL_d1, "LLP0inHCAL_depth1"));
LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inHCAL_d2, "LLP0inHCAL_depth2"));
LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inHCAL_d3, "LLP0inHCAL_depth3"));
LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inHCAL_d4, "LLP0inHCAL_depth4"));
LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inHCAL_d12, "LLP0inHCAL_depth12"));
LLP0_Cuts.insert(pair<TCut, string>(Cut_LLP0inHCAL_d34, "LLP0inHCAL_depth34"));

map<tuple<TCut, TCut>, string> JetMatchedToLLP; // jet (0,1) is matched to a LLP in the given region // tuple could be used for more cuts as well, accessing done below
JetMatchedToLLP.insert(pair< tuple<TCut, TCut>, string> (make_tuple(Cut_LLPinTracker_Jet0, Cut_LLPinTracker_Jet1), "Matched_LLPinTracker"));
JetMatchedToLLP.insert(pair< tuple<TCut, TCut>, string> (make_tuple(Cut_LLPinECAL_Jet0, Cut_LLPinECAL_Jet1), "Matched_LLPinECAL"));
JetMatchedToLLP.insert(pair< tuple<TCut, TCut>, string> (make_tuple(Cut_LLPinHCAL1_Jet0, Cut_LLPinHCAL1_Jet1), "Matched_LLPinHCAL_d1"));
JetMatchedToLLP.insert(pair< tuple<TCut, TCut>, string> (make_tuple(Cut_LLPinHCAL2_Jet0, Cut_LLPinHCAL2_Jet1), "Matched_LLPinHCAL_d2"));
JetMatchedToLLP.insert(pair< tuple<TCut, TCut>, string> (make_tuple(Cut_LLPinHCAL3_Jet0, Cut_LLPinHCAL3_Jet1), "Matched_LLPinHCAL_d3"));
JetMatchedToLLP.insert(pair< tuple<TCut, TCut>, string> (make_tuple(Cut_LLPinHCAL4_Jet0, Cut_LLPinHCAL4_Jet1), "Matched_LLPinHCAL_d4"));
JetMatchedToLLP.insert(pair< tuple<TCut, TCut>, string> (make_tuple(Cut_LLPinHCAL12_Jet0, Cut_LLPinHCAL12_Jet1), "Matched_LLPinHCAL_d12"));
JetMatchedToLLP.insert(pair< tuple<TCut, TCut>, string> (make_tuple(Cut_LLPinHCAL34_Jet0, Cut_LLPinHCAL34_Jet1), "Matched_LLPinHCAL_d34"));

map<TCut, string>::iterator it;
map<tuple<TCut, TCut>, string>::iterator it_double;


// cuts for bins of neutral hadron fraction
TCut Cut_NHadpt1 	= "jet0_NeutralHadEFrac < 0.1";
TCut Cut_NHadpt2 	= "jet0_NeutralHadEFrac >= 0.1 && jet0_NeutralHadEFrac < 0.2";
TCut Cut_NHadpt3 	= "jet0_NeutralHadEFrac >= 0.2 && jet0_NeutralHadEFrac < 0.3";
TCut Cut_NHadpt4 	= "jet0_NeutralHadEFrac >= 0.3 && jet0_NeutralHadEFrac < 0.4";
TCut Cut_NHadpt6 	= "jet0_NeutralHadEFrac >= 0.4 && jet0_NeutralHadEFrac < 0.6";
TCut Cut_NHad1 	    = "jet0_NeutralHadEFrac >= 0.6";

// cuts for bins of jet pT
TCut Cut_JetPt40 	= "jet0_Pt >= 40 && jet0_Pt < 80";
TCut Cut_JetPt80 	= "jet0_Pt >= 80 && jet0_Pt < 120";
TCut Cut_JetPt120 	= "jet0_Pt >= 120 && jet0_Pt < 160";
TCut Cut_JetPt160 	= "jet0_Pt >= 160 && jet0_Pt < 200";
TCut Cut_JetPt200 	= "jet0_Pt >= 200";


// cuts for bins of BDT score
TCut Cut_BDTscorePt9    = "bdtscore_LLP350_MS80_perJet >= 0.9 && bdtscore_LLP350_MS80_perJet < 0.95"; // "bdtscore_125GeV >= -1 && bdtscore_125GeV < 0";
TCut Cut_BDTscorePt95 	= "bdtscore_LLP350_MS80_perJet >= 0.95 && bdtscore_LLP350_MS80_perJet < 0.99";
TCut Cut_BDTscorePt99 	= "bdtscore_LLP350_MS80_perJet >= 0.99 && bdtscore_LLP350_MS80_perJet < 0.999";
TCut Cut_BDTscore1   	= "bdtscore_LLP350_MS80_perJet >= 0.999";

// --------------------------------------------------------------------- //
// ---------------------------- per jet cuts --------------------------- //
// --------------------------------------------------------------------- //
TString matchedLLPinCR             = Form("perJet_MatchedLLP_DecayR < %0.1f && abs(perJet_MatchedLLP_Eta) < %f", controlRegion, HBeta);
TString matchedLLPinTrackerNP      = Form("perJet_MatchedLLP_DecayR >= %0.1f && perJet_MatchedLLP_DecayR < %0.1f && abs(perJet_MatchedLLP_Eta) < %f",  controlRegion, radius_tracker[1], HBeta);
TString matchedLLPinECAL           = Form("perJet_MatchedLLP_DecayR >= %0.1f && perJet_MatchedLLP_DecayR < %0.1f && abs(perJet_MatchedLLP_Eta) < %f",  radius_ECAL[0], radius_ECAL[1], HBeta);
TString matchedLLPinHCAL           = Form("perJet_MatchedLLP_DecayR >= %0.1f && perJet_MatchedLLP_DecayR < %0.1f && abs(perJet_MatchedLLP_Eta) < %f",  radius_depth1[0], radius_depth34[1], HBeta);
TString matchedLLPinHCAL1           = Form("perJet_MatchedLLP_DecayR >= %0.1f && perJet_MatchedLLP_DecayR < %0.1f && abs(perJet_MatchedLLP_Eta) < %f",  radius_depth1[0], radius_depth1[1], HBeta);
TString matchedLLPinHCAL2          = Form("perJet_MatchedLLP_DecayR >= %0.1f && perJet_MatchedLLP_DecayR < %0.1f && abs(perJet_MatchedLLP_Eta) < %f",  radius_depth2[0], radius_depth2[1], HBeta);
TString matchedLLPinHCAL34         = Form("perJet_MatchedLLP_DecayR >= %0.1f && perJet_MatchedLLP_DecayR < %0.1f && abs(perJet_MatchedLLP_Eta) < %f",  radius_depth34[0], radius_depth34[1], HBeta);

TString eventHT250                  = Form("eventHT > 0"); //250 for hard cut plots

TCut Cut_matchedLLPinCR             = matchedLLPinCR.Data();
TCut Cut_matchedLLPinTrackerNP      = matchedLLPinTrackerNP.Data();
TCut Cut_matchedLLPinECAL           = matchedLLPinECAL.Data();
TCut Cut_matchedLLPinHCAL           = matchedLLPinHCAL.Data();
TCut Cut_matchedLLPinHCAL1          = matchedLLPinHCAL1.Data();
TCut Cut_matchedLLPinHCAL2          = matchedLLPinHCAL2.Data();
TCut Cut_matchedLLPinHCAL34         = matchedLLPinHCAL34.Data();

TCut Cut_matchedLLPinHCAL34_eventHT250 = (matchedLLPinHCAL34 + AND + eventHT250).Data();

TCut Cut_L1_LLPtriggered1 = Form("jet0_L1trig_Matched == 1");
TCut Cut_L1_LLPtriggered0 = Form("jet0_L1trig_Matched == 0");

TCut Cut_0DepthTowers = Form("jet0_DepthTowers == 0");
TCut Cut_1DepthTowers = Form("jet0_DepthTowers == 1");
TCut Cut_2DepthTowers = Form("jet0_DepthTowers >= 2");

TCut Cut_0TimingTowers = Form("jet0_TimingTowers == 0");
TCut Cut_1TimingTowers = Form("jet0_TimingTowers == 1");
TCut Cut_2TimingTowers = Form("jet0_TimingTowers >= 2");

TCut Cut_0Towers = Form("jet0_TimingTowers + jet0_DepthTowers == 0");
TCut Cut_1Towers = Form("jet0_TimingTowers + jet0_DepthTowers == 1");
TCut Cut_2Towers = Form("jet0_TimingTowers + jet0_DepthTowers >= 2");

TCut Cut_CR = Form("jet1_scores_inc_train80 >= 0 && jet1_scores_inc_train80 <= 0.2 && jet1_InclTagCand == 1");
TCut Cut_VR = Form("jet1_scores_inc_train80 > 0.2 && jet1_scores_inc_train80 <= 0.9 && jet1_InclTagCand == 1");