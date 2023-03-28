/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::ProcessEvent(Long64_t jentry){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::Process()"<<endl;

	if (fChain == 0) return;
	fChain->GetEntry(jentry);
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) return;	

	count["All"]++;
	
	// GK, fill the below catergories of histograms
	if (jet_Pt->size() == 0) return; // added to avoid vector out of range if there are no jets 

	FillHists("NoSel"); 

	// check HLT results for these triggers
	int passedHLT = 0;
	for (int i = 0; i < HLT_Indices.size(); i++) {
		if (HLT_Decision->at(i) > 0) {
			if (debug) cout << HLT_Decision->at(i) << " for the trigger " << HLT_Names[i] << "\n" << endl;
			passedHLT += 1;
		}		
	}
	if (passedHLT > 0) {
		FillHists("PassedHLT");
	}

	// check jet energies
	if (jet_Pt->at(0) > 40 && jet_Pt->size() > 0) {
		FillHists("JetPt40");
	}

	// check gen LLP decay positions if signal
	double HB_outer_radius = 295;
	double HB_inner_radius = 175;

	if (n_gLLP > 0 && n_hbheRechit > 0) { // make sure gen LLP exists and HBHE rechits exist
		for (int i = 0; i < n_gLLP; i++) {
			double LLP_eta = gLLP_Eta->at(i);
			double LLP_phi = gLLP_Phi->at(i);
			if ((sqrt( pow(gLLP_DecayVtx_X->at(i),2) + pow(gLLP_DecayVtx_Y->at(i),2)) > 175) && (sqrt( pow(gLLP_DecayVtx_X->at(i),2) + pow(gLLP_DecayVtx_Y->at(i),2)) < 295) && abs(LLP_eta) < 1.4) { 
				// LLP i decay is in HB - now want to find relevant HCAL rechits

				std::cout << "found gen LLP with position (eta, phi) = " << LLP_eta << ", " << LLP_phi << "\n" << std::endl;

				for (int j = 0; j < n_hbheRechit; j++) { // loop over HCAL rechits
					double HBHE_eta = hbheRechit_Eta->at(j);
					double HBHE_phi = hbheRechit_Phi->at(j);
					double dR = deltaR(LLP_eta, LLP_phi, HBHE_eta, HBHE_phi);
					if (dR < 0.5) FillHists("genMatchLLP_pt5");
				}
			}
		}
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

