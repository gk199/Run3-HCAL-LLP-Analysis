/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::SetWeight(string infiletag){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::SetWeight()"<<endl;

	// determine weights for MC to add as a new branch to the minituples
    cout << "Getting samples weight" << endl;

	if( isData ){
        weight == 1;

        cout << "weight --> " << weight << endl;

        return;
	}

    // when process multiple years will need to handle each lumi
    // 2023: CMS recorded 30.12 fb^-1
    // 2022: CMS recorded 38.01 fb^-1

    // MC
    // lumi in fb^-1
    double lumi = 1;
    // deal with luminosity from different MC samples (how??)

    // NEvents
    double NEvents_minituples = GetNEventsProduced(infiletag);
    if (fChain == 0) return;
    NEvents = fChain->GetEntriesFast();
    double NEvents_Ntuple = NEvents;

    // Cross section
    double BRxSigma = 1;

    if (isSignal) {
        BRxSigma = GetSignalBRxSigma(infiletag);
    } else {
        // would also deal with MC background processes here, like W+jets or Z->mu, to get BRxSigma
        weight = 1.0;
    }
	
    // Weight for each event
	weight 	    	 = BRxSigma*lumi/NEvents_AOD;
	weight_unskimmed = BRxSigma*lumi/NEvents_NTuple;
	lumi_samplefrac  = lumi/(lumi_mc16a+lumi_mc16d+lumi_mc16e);
	cout<<"  weight   --> "<<weight<<" (event-by-event weight components included later)"<<endl;

	cout<<endl;

}

/* ====================================================================================================================== */
double DisplacedHcalJetAnalyzer::GetSignalBRxSigma(string infiletag){

    if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetSignalBRxSigma()"<<endl;

    map<string,double> H_LLP_to_xs;
    H_LLP_to_xs["125"] = 48500; // fb
    H_LLP_to_xs["250"] = 48500;
    H_LLP_to_xs["350"] = 48500;

    string HiggsMass = infiletag.substr( infiletag.find("MH-")+26,3 );
	double BRxSigma = H_LLP_to_xs[HiggsMass];

	cout<<"  HiggsMass --> "<<HiggsMass<<endl;
	cout<<"  CrossSec --> "<<BRxSigma<<endl; // NB: BRxSigma before BR component

	// Branching Ratios //

	if( infiletag.find("LLP_MC_ggH_HToSSTobbbb_MH") != string::npos ) BRxSigma *= 0.01; // BR for H to LLP
	else { 
		weight = 1.0;
		cout<<"WARNING: Could not identify the proper weight value for this signal sample..."<<endl;
	}	

	cout<<"  BRxSigma --> "<<BRxSigma<<endl;

	return BRxSigma;
}

/* ====================================================================================================================== */
double DisplacedHcalJetAnalyzer::GetNEventsProduced(string infiletag){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetNEventsProduced()"<<endl;

    map<string, double> MCTag_to_NEvents;
    MCTag_to_NEvents["ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV"] = 1000;

    double NEvents_minituples = MCTag_to_NEvents[infiletag];

    cout << NEvents_minituples << endl;

    if (NEvents_minituples < 1) {
        for(map<string,double>::iterator it = MCTag_to_NEvents.begin(); it != MCTag_to_NEvents.end(); ++it){
			string key = it->first;
			if( infiletag.find(key) != string::npos ){
				NEvents_minituples = MCTag_to_NEvents[key];
				break;
			}	
        }
    }
    
    if( NEvents_minituples < 1. ){
		cout<<"WARNING: Could not identify the proper NEvents value for this sample (tag"<<infiletag<<") ..."<<endl;
		NEvents_minituples = 1.;
	}

	cout<<"  NEvents  --> "<<NEvents_minituples<<endl;

	return NEvents_minituples;
}