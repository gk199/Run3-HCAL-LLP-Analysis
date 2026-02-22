/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::SetTriggerNames(){ //} string infilepath, string hist_name ){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::SetTriggerNames()"<<endl;

	vector<string> triggerPathNames;

	// Copied from NTupler
	triggerPathNames.push_back("HLT_CaloMET60_DTCluster50");
	triggerPathNames.push_back("HLT_CaloMET60_DTClusterNoMB1S50");
	triggerPathNames.push_back("HLT_HT170_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack");
	triggerPathNames.push_back("HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay0p5nsTrackless");
	triggerPathNames.push_back("HLT_HT200_L1SingleLLPJet_DelayedJet40_DoubleDelay1nsInclusive");
	triggerPathNames.push_back("HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay1nsTrackless");
	triggerPathNames.push_back("HLT_HT200_L1SingleLLPJet_DelayedJet40_SingleDelay2nsInclusive");
	triggerPathNames.push_back("HLT_HT200_L1SingleLLPJet_DisplacedDijet35_Inclusive1PtrkShortSig5");
	triggerPathNames.push_back("HLT_HT200_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack");
	triggerPathNames.push_back("HLT_HT200_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5");
	triggerPathNames.push_back("HLT_HT200_L1SingleLLPJet_DisplacedDijet60_DisplacedTrack");
	triggerPathNames.push_back("HLT_HT240_L1SingleLLPacedDijet40_Inclusive1PtrkShortSig5");
	triggerPathNames.push_back("HLT_HT270_L1SingleLLPJet_DisplacedDijet40_DisplacedTrack");
	triggerPathNames.push_back("HLT_HT280_L1SingleLLPJet_DisplacedDijet40_Inclusive1PtrkShortSig5");
	triggerPathNames.push_back("HLT_HT320_L1SingleLLPJet_DisplacedDijet60_Inclusive");
	triggerPathNames.push_back("HLT_HT320_L1SingleLLPJet_DisplacedDijet60_Inclusive");
	triggerPathNames.push_back("HLT_HT200_L1SingleLLPJet_PFJet60_NeutralHadronFrac0p7");
	triggerPathNames.push_back("HLT_HT200_L1SingleLLPJet_PFJet60_NeutralHadronFrac0p8");
	triggerPathNames.push_back("HLT_HT350_DelayedJet40_SingleDelay3nsInclusive");
	triggerPathNames.push_back("HLT_HT350_DelayedJet40_SingleDelay3p25nsInclusive");
	triggerPathNames.push_back("HLT_HT350_DelayedJet40_SingleDelay3p5nsInclusive");
	triggerPathNames.push_back("HLT_HT350");
	triggerPathNames.push_back("HLT_HT400_DisplacedDijet40_DisplacedTrack");
	triggerPathNames.push_back("HLT_HT420_L1SingleLLPJet_DisplacedDijet60_Inclusive");
	triggerPathNames.push_back("HLT_HT425");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_DoubleDelay0p5nsInclusive");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_DoubleDelay0p5nsTrackless");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_DoubleDelay0p75nsTrackless");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_DoubleDelay1nsInclusive");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_DoubleDelay1nsTrackless");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_DoubleDelay1p25nsInclusive");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_DoubleDelay1p5nsInclusive");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_SingleDelay0p5nsInclusive");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_SingleDelay0p5nsTrackless");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_SingleDelay1nsInclusive");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_SingleDelay1nsTrackless");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_SingleDelay1p25nsTrackless");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_SingleDelay1p5nsInclusive");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_SingleDelay1p5nsTrackless");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_SingleDelay2nsInclusive");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_SingleDelay2p25nsInclusive");
	triggerPathNames.push_back("HLT_HT430_DelayedJet40_SingleDelay2p5nsInclusive");
	triggerPathNames.push_back("HLT_HT550_DisplacedDijet60_Inclusive");
	triggerPathNames.push_back("HLT_L1MET_DTCluster50");
	triggerPathNames.push_back("HLT_L1MET_DTClusterNoMB1S50");
	triggerPathNames.push_back("HLT_L1Mu6HT240");
	triggerPathNames.push_back("HLT_L1SingleLLPJet");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_DoubleDelay0p5nsTrackless");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_DoubleDelay0p75nsInclusive");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_DoubleDelay1nsTrackless");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_DoubleDelay1p25nsInclusive");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_DoubleDelay1p25nsTrackless");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_DoubleDelay1p5nsInclusive");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_DoubleDelay1p5nsTrackless");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_DoubleDelay1p75nsInclusive");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_SingleDelay2p5nsTrackless");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_SingleDelay2p75nsTrackless");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_SingleDelay3nsTrackless");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_SingleDelay3p5nsInclusive");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_SingleDelay3p75nsInclusive");
	triggerPathNames.push_back("HLT_L1Tau_DelayedJet40_SingleDelay4nsInclusive");
	triggerPathNames.push_back("HLT_Mu6HT240_DisplacedDijet30_Inclusive1PtrkShortSig5_DisplacedLoose");
	triggerPathNames.push_back("HLT_Mu6HT240_DisplacedDijet35_Inclusive0PtrkShortSig5");
	triggerPathNames.push_back("HLT_Mu6HT240_DisplacedDijet35_Inclusive1PtrkShortSig5_DisplacedLoose");
	triggerPathNames.push_back("HLT_Mu6HT240_DisplacedDijet40_Inclusive0PtrkShortSig5");
	triggerPathNames.push_back("HLT_Mu6HT240_DisplacedDijet40_Inclusive1PtrkShortSig5_DisplacedLoose");
	triggerPathNames.push_back("HLT_Mu6HT240_DisplacedDijet45_Inclusive0PtrkShortSig5");
	triggerPathNames.push_back("HLT_Mu6HT240_DisplacedDijet50_Inclusive0PtrkShortSig5");
	triggerPathNames.push_back("HLT_PFJet200_TimeGt2p5ns");
	triggerPathNames.push_back("HLT_PFJet200_TimeLtNeg2p5ns");
	
	// Set Global Variables
	HLT_Names = triggerPathNames;

	cout<<"Reading in "<<triggerPathNames.size()<<" HLT paths, including:"<<endl;

	for( int i=0; i<triggerPathNames.size(); i++ ){
		string hlt_name_temp = triggerPathNames.at(i);

		if( hlt_name_temp.find("L1SingleLLPJet") != std::string::npos){
			cout<<" --> "<<hlt_name_temp<<endl;
		}

		HLT_Indices[hlt_name_temp] = i;
		
	}

}
