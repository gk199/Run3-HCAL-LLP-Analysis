/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::ProcessEvent(Long64_t jentry){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::Process()"<<endl;

	if (fChain == 0) return;
	fChain->GetEntry(jentry);
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) return;	

	count["All"]++;
	
	// GK, fill the below catergories of histograms
	FillHists("NoSel"); 

	if (jet_Pt->at(0) > 40) {
		FillHists("JetPt40");
	}
	// FillOutputTrees("");

	return;

}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::Loop(){

	// Get Number of entries from global variable Nevents

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::Loop()"<<endl;

	cout<<"\n ----- PRE-PROCESSING ----- \n"<<endl;	

	// ----- Book Histograms, Declare Output Trees, Initialize event counters ----- // 

	if( save_hists ) BookHists();

	if( save_trees ) DeclareOutputTrees();

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

