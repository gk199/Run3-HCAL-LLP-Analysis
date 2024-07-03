/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::ProcessEvent(Long64_t jentry){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::Process()"<<endl;

	if (fChain == 0) return;
	fChain->GetEntry(jentry);
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) return;	

	// ----- Reset and Set Global Variables ----- //

	ResetGlobalEventVars();

	SetLLPVariables();

	count["All"]++;
	
	if (jet_Pt->size() == 0) return; // added to avoid vector out of range if there are no jets -- issue on signal file 

	// check HLT results for these triggers
	// note that this is not just the HCAL based LLP triggers, but all triggers saved in "triggerPathNames" in Run3-HCAL-LLP-NTupler/plugins/DisplacedHcalJetNTuplizer.cc, which is much larger now! 
	// so add an extra check that the name we are looking at is the HLT L1 HCAL monitoring path
	int passedHLT = 0;
	for (int i = 0; i < HLT_Indices.size(); i++) {
		if (HLT_Decision->at(i) > 0) {
			if (debug) cout << HLT_Decision->at(i) << " for the trigger " << HLT_Names[i] << "\n" << endl;
			if (HLT_Names[i] == "HLT_L1SingleLLPJet") passedHLT += 1;
		}		
	}
	
	// Fill the below catergories of histograms
	FillHists("NoSel");

	if (passedHLT > 0) {
		FillHists("PassedHLT");
	}

	if (jet_Pt->at(0) > 40 && abs(jet_Eta->at(0)) <= 1.26) { // check leading jet energy and eta
		FillHists("JetPt40");
	}

	bool WPlusJetsEvent = false;
	bool NoLeptonEvent = false;
	if (PassWPlusJetsSelection()) WPlusJetsEvent = true;
	if (PassLeptonVeto()) NoLeptonEvent = true;

	// Fill jet based output trees in minituples
	for (int i = 0; i < jet_Pt->size(); i++) {
		if (jet_Pt->at(i) > 40 && abs(jet_Eta->at(i)) <= 1.26) { // this is the standard requirement
		// if (jet_Pt->at(i) >= 0 && abs(jet_Eta->at(i)) <= 1.26) { // edited requirement to make jet pT turn on plot without a 40 GeV cut

			FillOutputJetTrees("PerJet_NoSel", i);
			if (passedHLT > 0) FillOutputJetTrees("PerJet_PassedHLT", i);
			vector<float> matchedInfo = JetIsMatchedTo( jet_Eta->at(i), jet_Phi->at(i) );
			if (matchedInfo[0] > -1) { 					// if jet is matched to a LLP or LLP decay product
				FillOutputJetTrees("PerJet_LLPmatched", i);
			}
			if (WPlusJetsEvent) {
				float phiSeparation = deltaPhi(jet_Phi->at(i), WPlusJets_leptonPhi);
				if ( abs(phiSeparation) > 2 ) FillOutputJetTrees("PerJet_WPlusJets", i);
			}
			if (NoLeptonEvent) {
				FillOutputJetTrees("PerJet_NoLepton", i);
			}
		}
	}

	// Fill event based output trees in minituples
	FillOutputTrees("NoSel");
	if (passedHLT > 0) FillOutputTrees("PassedHLT");
	if (WPlusJetsEvent && abs(deltaPhi(jet_Phi->at(0), WPlusJets_leptonPhi)) > 2) FillOutputTrees("WPlusJets");
	if (NoLeptonEvent) FillOutputTrees("NoLepton");

	return;

}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::Loop(){

	// Get Number of entries from global variable Nevents

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::Loop()"<<endl;

	cout<<"\n ----- PRE-PROCESSING ----- \n"<<endl;	

	// ----- Book Histograms, Declare Output Trees, Initialize event counters ----- // 

	if( save_hists ) BookHists();

	if( save_trees ) {
		DeclareOutputJetTrees();
		DeclareOutputTrees();
	}

	count["All"] = 0;

	// ----- Initialize Values ----- // 


	// ----- Set up & run loop ----- // 

	if (fChain == 0) return;

	Long64_t nentries = NEvents; 
	if( NEvents < 0 || NEvents > fChain->GetEntries() ) nentries = fChain->GetEntries();

	Long64_t init_entry = 0; //0; 

	cout<<"\n ----- RUNNING ----- \n"<<endl;
	cout<<"Number of events = "<<nentries<<endl;

	clock_t clock_start = clock();

	Long64_t nbytes = 0, nb = 0;
	Long64_t nevents_processed = 0; 
	for (Long64_t jentry=init_entry; jentry<nentries;jentry++) {

		//nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;

		bool print_jentry = false; 

		if( jentry < 10 ) print_jentry = true;
		else if( jentry < 100  && jentry % 10 == 0    ) print_jentry = true;
		else if( jentry < 1000 && jentry % 100 == 0   ) print_jentry = true;
		else if( jentry % 100 == 0  ) print_jentry = true;
		//else if( jentry < 1e5  && jentry % 10000 == 0 ) print_jentry = true;
		//else if( jentry % 100000 == 0 ) print_jentry = true;

		if( print_jentry && !debug ){
			float event_timing = GetEventRuntime(clock_start, init_entry, jentry); 
			cout<<"Processing Event: "<<jentry<<"\t |  ";
			cout<<"Rate: "<<std::fixed<<setprecision(2)<<event_timing<<" events/sec\t |  ";
			cout<<"Estimated Time Remaining: "<<(nentries-jentry)/event_timing<<" sec \t\t \r"<<flush;
		} 

		if( debug ) cout<<"\n Processing Event "<<jentry<<" "<<endl;
		
		ProcessEvent(jentry);

		nevents_processed++;

	}

	cout<<endl;

	cout<<"Processed "<<nevents_processed<<" entries\t | ";
	cout<<"Cumulative Event Loop Rate: "<<std::fixed<<setprecision(2)<<GetEventRuntime(clock_start, init_entry, nentries)<<" events/sec \n"<<endl;

	cout<<"\n ----- POST-PROCESSING ----- \n"<<endl;	


	// ----- Print event counters ----- // 

	if( print_counts ){
		cout<<"EVENT COUNTS: "<<endl;
		for(map<string,Double32_t>::iterator it = count.begin(); it != count.end(); ++it) 
			cout<<it->first<<"\t"<<count[it->first]<<endl;
	}

	// ----- Write Trees ----- // 

	if( save_trees ) WriteOutputTrees();

	// ----- Write Histograms ----- // 

	// Always write cutflow & trigger combinatorics hists
	if( save_hists ) WriteHists();

}

