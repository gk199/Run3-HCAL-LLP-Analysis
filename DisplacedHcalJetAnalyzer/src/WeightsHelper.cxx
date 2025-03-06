/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::SetWeight(string infiletag){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::SetWeight()"<<endl;
    
	// determine weights for MC to add as a new branch to the minituples
    cout << "Getting samples weight" << endl;

    bool data = false, signal = false;
    cout << infiletag << endl;
    if (infiletag.find("CTau") != string::npos ) signal = true;
    if (infiletag.find("PromptReco") != string::npos ) data = true;

    cout << data << " = is data flag" << endl;
    cout << signal << " = is signal flag" << endl;

	if( data ){
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
    double lumi_2022 = 38.01,  lumi_2023 = 30.12; // fb^-1, https://twiki.cern.ch/twiki/bin/view/CMSPublic/LumiPublicResults#2023_proton_proton_collisions_at
    // deal with luminosity from different MC samples, if year depedent. Now everything is 2023
    lumi = lumi_2023;

    // NEvents
    double NEvents_produced = GetNEventsProduced(infiletag);
    if (fChain == 0) return;
    double NEvents_Ntuple = fChain->GetEntriesFast();

    // Cross section
    double BRxSigma = 1;

    if ( !data && infiletag.find("CTau") != string::npos ) { // isSignal
        BRxSigma = GetSignalBRxSigma(infiletag);
    } else {
        // would also deal with MC background processes here, like W+jets or Z->mu, to get BRxSigma
        weight = 1.0;
    }
	
    // Weight for each event
	// weight 	    	 = BRxSigma*lumi/NEvents_produced;
	weight = BRxSigma*lumi/NEvents_Ntuple;
	lumi_samplefrac  = lumi/(lumi_2022+lumi_2023);
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

    // Find the position of "MH" and extract the mass
    cout << infiletag << endl;
    string HiggsMass = infiletag.substr( infiletag.find("MH")+2,3 );
    cout << HiggsMass << endl;
	double BRxSigma = H_LLP_to_xs[HiggsMass];

	cout<<"  HiggsMass --> "<<HiggsMass<<endl;
	cout<<"  CrossSec --> "<<BRxSigma<<endl; // NB: BRxSigma before BR component

	// Branching Ratios //

	if( infiletag.find("LLP_MC_ggH_HToSSTobbbb_MH") != string::npos && infiletag.find("HToSSTo4B") != string::npos ) BRxSigma *= 0.01; // BR for H to LLP
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
    MCTag_to_NEvents["ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV"] = 292437; // nevents in TEST v3.8 NoSel

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