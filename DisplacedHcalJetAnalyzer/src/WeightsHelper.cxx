/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::SetWeight(string infiletag){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::SetWeight()"<<endl;
    
	// determine weights for MC to add as a new branch to the minituples
    cout << "Getting samples weight" << endl;

    bool data = false, signal = false;
    cout << infiletag << endl;
    if (infiletag.find("CTau") != string::npos ) signal = true;
    if (infiletag.find("PromptReco") != string::npos ) data = true;

    //cout << data << " = is data flag" << endl;
    //cout << signal << " = is signal flag" << endl;

	if( data ){
        weight = 1;
        cout << "weight --> " << weight << endl;
        return;
	}

    // MC
    // lumi in fb^-1
    double lumi = 1;
    // when process multiple years need to handle each lumi (CMS recorded lumi)
    double lumi_2022 = 34.76,  lumi_2023 = 28.28; // fb^-1, from brilcalc tool on 1 June 2026, agrees with HLT trigger prescale settings
    // double lumi_2022 = 38.01,  lumi_2023 = 30.12; // fb^-1, https://twiki.cern.ch/twiki/bin/view/CMSPublic/LumiPublicResults#2023_proton_proton_collisions_at
    // deal with luminosity from different MC samples, if year depedent. Now everything is 2023
    lumi = lumi_2022 + lumi_2023;

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
	weight = BRxSigma*lumi/NEvents_produced; // weight = lumi * xsec (=# higgs) * BR (=# LLP)
    // if NEvents_produced == 1, then the filetag was not found. Instead of setting a too high weight, set event weight to 1
    if ( !data && infiletag.find("CTau") != string::npos && NEvents_produced == 1) weight = 1;
	lumi_samplefrac  = lumiNum/(lumi_2022+lumi_2023);
	cout<<"  weight   --> "<<weight<<" (event-by-event weight components included later)"<<endl;

	cout<<endl;

}

/* ====================================================================================================================== */
double DisplacedHcalJetAnalyzer::GetSignalBRxSigma(string infiletag){

    if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetSignalBRxSigma()"<<endl;

    double H_LLP_to_xs = 52200; // fb // https://pdg.lbl.gov/2023/reviews/rpp2023-rev-higgs-boson.pdf

    // Find the position of "MH" and extract the mass
    string HiggsMass = infiletag.substr( infiletag.find("MH")+2,3 );
	double BRxSigma = H_LLP_to_xs;

	cout<<"  HiggsMass --> "<<HiggsMass<<endl;
	cout<<"  CrossSec --> "<<BRxSigma<<endl; // NB: BRxSigma before BR component

	// Branching Ratios //
	if( infiletag.find("HToSSTobbbb") != string::npos || infiletag.find("HToSSTo4B") != string::npos ) BRxSigma *= 0.01; // BR for H to LLP
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

    MCTag_to_NEvents["HToSSTo4B_MH125_MS50_CTau3000"] = 3992000; // 998000 in batch 1, 2994000 in batch 2
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau10000"] = 3991491; // 997090 in batch 1, 2994401 in batch 2
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau500"] = 497400; // 497400 events
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau10000"] = 3992496; // 999100 in batch 1, 2993396 in batch 2

    // Central Samples 
    MCTag_to_NEvents["HToSSTo4B_MH1000_MS450_CTau100000_PU60_2022postEE"] = 39929;
    MCTag_to_NEvents["HToSSTo4B_MH1000_MS450_CTau100000_PU70_2022postEE"] = 39929;
    MCTag_to_NEvents["HToSSTo4B_MH1000_MS450_CTau100000_Premix_2023postBPix-pythia"] = 40020;
    MCTag_to_NEvents["HToSSTo4B_MH1000_MS450_CTau100000_Premix_2022preEE"] = 39364;
    MCTag_to_NEvents["HToSSTo4B_MH1000_MS450_CTau100000_Premix_2023postBPix_pythia"] = 39944;
    MCTag_to_NEvents["HToSSTo4B_MH1000_MS450_CTau10000_PU60_2022postEE"] = 39920;
    MCTag_to_NEvents["HToSSTo4B_MH1000_MS450_CTau10000_PU70_2022postEE"] = 39920;
    MCTag_to_NEvents["HToSSTo4B_MH1000_MS450_CTau10000_Premix_2023postBPix-pythia"] = 39972;
    MCTag_to_NEvents["HToSSTo4B_MH1000_MS450_CTau10000_Premix_2022preEE"] = 38657;
    MCTag_to_NEvents["HToSSTo4B_MH1000_MS450_CTau10000_Premix_2023postBPix_pythia"] = 39930;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS12_CTau9000_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS12_CTau9000_PU70_2022postEE"] = 26000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS12_CTau9000_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS12_CTau9000_Premix_2022preEE"] = 39265;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS12_CTau9000_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS12_CTau900_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS12_CTau900_PU70_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS12_CTau900_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS12_CTau900_Premix_2022preEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS12_CTau900_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS25_CTau15000_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS25_CTau15000_PU70_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS25_CTau15000_Premix_2023postBPix-pythia"] = 0; // Should be 40000, but DNE in v5;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS25_CTau15000_Premix_2022preEE"] = 37807;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS25_CTau15000_Premix_2023postBPix_pythia"] = 39000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS25_CTau1500_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS25_CTau1500_PU70_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS25_CTau1500_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS25_CTau1500_Premix_2022preEE"] = 39275;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS25_CTau1500_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS50_CTau30000_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS50_CTau30000_PU70_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS50_CTau30000_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS50_CTau30000_Premix_2022preEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS50_CTau30000_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS50_CTau3000_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS50_CTau3000_PU70_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS50_CTau3000_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS50_CTau3000_Premix_2022preEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH125_MS50_CTau3000_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau10000_PU60_2022postEE"] = 39993;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau10000_PU70_2022postEE"] = 39993;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau10000_Premix_2023postBPix-pythia"] = 39992;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau10000_Premix_2022preEE"] = 39998;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau10000_Premix_2023postBPix_pythia"] = 39987;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau1000_PU60_2022postEE"] = 39990;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau1000_PU70_2022postEE"] = 39990;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau1000_Premix_2023postBPix-pythia"] = 0; // Should be 39994, but DNE in v5;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau1000_Premix_2022preEE"] = 34231;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau1000_Premix_2023postBPix_pythia"] = 36996;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau500_PU60_2022postEE"] = 39994;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau500_PU70_2022postEE"] = 39994;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau500_Premix_2023postBPix-pythia"] = 39994;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau500_Premix_2022preEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS120_CTau500_Premix_2023postBPix_pythia"] = 40006;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau10000_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau10000_PU70_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau10000_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau10000_Premix_2022preEE"] = 39278;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau10000_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau1000_PU60_2022postEE"] = 37000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau1000_PU70_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau1000_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau1000_Premix_2022preEE"] = 38566;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau1000_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau500_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau500_PU70_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau500_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau500_Premix_2022preEE"] = 39283;
    MCTag_to_NEvents["HToSSTo4B_MH250_MS60_CTau500_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau10000_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau10000_PU70_2022postEE"] = 39000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau10000_Premix_2023postBPix-pythia"] = 0; // Should be 40000, but DNE in v5;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau10000_Premix_2022preEE"] = 39277;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau10000_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau1000_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau1000_PU70_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau1000_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau1000_Premix_2022preEE"] = 37900;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau1000_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau500_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau500_PU70_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau500_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau500_Premix_2022preEE"] = 38590;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS160_CTau500_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau10000_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau10000_PU70_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau10000_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau10000_Premix_2022preEE"] = 36410;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau10000_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau1000_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau1000_PU70_2022postEE"] = 31000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau1000_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau1000_Premix_2022preEE"] = 34320;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau1000_Premix_2023postBPix_pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau500_PU60_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau500_PU70_2022postEE"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau500_Premix_2023postBPix-pythia"] = 40000;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau500_Premix_2022preEE"] = 39291;
    MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau500_Premix_2023postBPix_pythia"] = 40000;

    // MCTag_to_NEvents["LLP_MC_125_mX50_batch1"] = 998000;
    // MCTag_to_NEvents["LLP_MC_125_mX50_batch2"] = 2994000;
    // MCTag_to_NEvents["LLP_MC_250_mX120_batch1"] = 997090;
    // MCTag_to_NEvents["LLP_MC_250_mX120_batch2"] = 2994401;
    // MCTag_to_NEvents["HToSSTo4B_MH350_MS80_CTau500"] = 497400; 
    // MCTag_to_NEvents["LLP_MC_350_mX160__"] = 999100;
    // MCTag_to_NEvents["LLP_MC_350_mX160_batch2"] = 2993396;
    // MCTag_to_NEvents["ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV"] = 1966400;

    /*
    N events from DAS, searching with "instance=prod/phys03"
    998000 events    /HToSSTo4B_MH125_MS50_CTau3000/lpclonglived-crab_PrivateProduction_Summer23BPix_DR_step2_RECOSIM_HToSSTo4B_MH125_MS50_CTau3000_batch1_v1-6c03a81f0d97498cab5c296ab3fa9a76/USER
    2994000 events   /HToSSTo4B_MH125_MS50_CTau3000/lpclonglived-crab_PrivateProduction_Summer23BPix_DR_step2_RECOSIM_HToSSTo4B_MH125_MS50_CTau3000_batch2_v1-6c03a81f0d97498cab5c296ab3fa9a76/USER
    997090 events    /HToSSTo4B_MH250_MS120_CTau10000/lpclonglived-crab_PrivateProduction_Summer23BPix_DR_step2_RECOSIM_HToSSTo4B_MH250_MS120_CTau10000_batch1_v1-6c03a81f0d97498cab5c296ab3fa9a76/USER
    2994401 events   /HToSSTo4B_MH250_MS120_CTau10000/lpclonglived-crab_PrivateProduction_Summer23BPix_DR_step2_RECOSIM_HToSSTo4B_MH250_MS120_CTau10000_batch2_v1-6c03a81f0d97498cab5c296ab3fa9a76/USER
    497400 events    /HToSSTo4B_MH350_MS80_CTau500/lpclonglived-crab_PrivateProduction_Summer23BPix_DR_step2_RECOSIM_HToSSTo4B_MH350_MS80_CTau500_batch1_v1-6c03a81f0d97498cab5c296ab3fa9a76/USER
    999100 events    /HToSSTo4B_MH350_MS160_CTau10000/lpclonglived-crab_PrivateProduction_Summer23BPix_DR_step2_RECOSIM_HToSSTo4B_MH350_MS160_CTau10000_batch1_v1-6c03a81f0d97498cab5c296ab3fa9a76/USER
    2993396 events   /HToSSTo4B_MH350_MS160_CTau10000/lpclonglived-crab_PrivateProduction_Summer23BPix_DR_step2_RECOSIM_HToSSTo4B_MH350_MS160_CTau10000_batch2_v1-6c03a81f0d97498cab5c296ab3fa9a76/USER
    1966400 events   /ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV/lpclonglived-crab_PrivateProduction_Summer22_DR_step2_RECOSIM_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_batch1_v1-59a22edf0600a784f6c900595d24e883/USER
    /eos/uscms/store/user/lpclonglived/apresyan/privateProduction/DR/step2_RECOSIM/Run3Summer22/ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV
    */

    double NEvents_produced = MCTag_to_NEvents[infiletag];

    cout << NEvents_produced << endl;

    if (NEvents_produced < 1) {
        for(map<string,double>::iterator it = MCTag_to_NEvents.begin(); it != MCTag_to_NEvents.end(); ++it){
			string key = it->first;
			if( infiletag.find(key) != string::npos ){
				NEvents_produced = MCTag_to_NEvents[key];
				break;
			}	
        }
    }
    
    if( NEvents_produced < 1. ){
		cout<<"WARNING: Could not identify the proper NEvents value for this sample (tag "<<infiletag<<") ..."<<endl;
		NEvents_produced = 1.;
	}

	cout<<"  NEvents  --> "<<NEvents_produced<<endl;

	return NEvents_produced;
}

