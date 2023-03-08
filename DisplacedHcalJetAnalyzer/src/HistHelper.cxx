/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::SetHistCategories(){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::SetCategories()"<<endl;

	// histogram category corresponds to selections (jet energy, etc)
	// decides what is ultimately written
	categories = {"NoSel", "JetPt40"};

	if( !save_hists ){
		cout<<"  NOTE: 'save_hists' is set to false. Will not run over ANY histogram categories..."<<endl;
		categories = {};
	}

}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::BookHists(){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::BookHists()"<<endl;
	
	cout<<"Booking Histograms..."<<endl;

	int NBins = 240;
	int NBinsPull = NBins*2;
	int NBins2D = NBins/2; // 96	

	// ----- Cutflow ----- // 
	// E.g. h["Cutflow_"+photonIDtype+ph_det_region_temp] = new TH1F( hname_temp , Form( "Cutflow: Analysis Selection %s ; ; Events", ph_det_region_temp.c_str() ), 9, -1, 8 );

	// ----- Triggers ----- // 

	/*triggers = { "g35_loose_g25_loose", // 2015-16 Data!
				 "g35_medium_g25_medium_L12EM20VH", "2g50_loose_L12EM20VH", // 2017-2018 Data!
	}; 

	cout<<"Booking Trigger Hists for the following:"<<endl;
	for( auto trig: triggers ) cout<<"  --> "<<trig<<endl;  

	const int Ntriggers = triggers.size();
	h["PassTrig"] = new TH1F( Form( "%sPassTrig", hname_preprend.c_str() ), "PassTrig ; ; Events", Ntriggers+1, 0, Ntriggers+1 );
	h["PassTrig"]->GetXaxis()->SetBinLabel(1, "All" );
	h["PassTrigPostSel"] = new TH1F( Form( "%sPassTrigPostSel", hname_preprend.c_str() ), "PassTrig and PostSelection; ; Events", Ntriggers+1, 0, Ntriggers+1 );
	h["PassTrigPostSel"]->GetXaxis()->SetBinLabel(1, "Only PostSelection" );

	if( Ntriggers == 0 ) cout<<"WARNING: No triggers specified!"<<endl;
	for( int i=0; i<Ntriggers; i++ ){
		string trig = triggers.at(i);
		h["PassTrig"]->GetXaxis()->SetBinLabel((Int_t)i+2, Form("Pass %s", trig.c_str()) );
		h["PassTrigPostSel"]->GetXaxis()->SetBinLabel((Int_t)i+2, Form("Pass %s", trig.c_str()) );
	}*/

	// --------------------------------------------------------------------------------------------
	// Histogram CATEGORIES 
	// Loop over categories, book identical histograms in each category

	cout<<"Booking Categorized Hists for the following:"<<endl;
	if( categories.size() == 0 ) cout<<"WARNING: No categories specified!"<<endl;

	// For creating general histograms for all, leading and subleading objects
	vector<string> istring = {"0", "1", "2"};

	for( auto cat: categories ){

		cout<<"  --> "<<cat<<endl;

		cat += "__";

		// 1D //

		// --------------------------------------------------------------------------------------------
		// Truth: General (any form of MC)      

		//if( !isData ){

			// Truth Vertex
			//h[cat+"TVx"]  = new TH1F( Form( "%sTVx",  cat.c_str() ), "; TVx [mm]; Events ", NBins, -2000, 2000);

		//}

		//if( isSignal ){

		//	// Object Kinematics            
		//	vector<string> istring = {"", "1", "2"};
		//	for( auto is: istring ){                
		//		h[cat+"trugrv"+is+"_pt"]  = new TH1F( Form( "%strugrv%s_pt",  cat.c_str(), is.c_str() ), "pt; pt [GeV]; Events ", NBins, 0, 1000);
		//		h[cat+"trugrv"+is+"_eta"] = new TH1F( Form( "%strugrv%s_eta", cat.c_str(), is.c_str() ), "eta; eta; Events", NBins, -3.2, 3.2 );
		//		h[cat+"trugrv"+is+"_phi"] = new TH1F( Form( "%strugrv%s_phi", cat.c_str(), is.c_str() ), "phi; phi; Events", NBins, -3.2, 3.2 );
		//	}

		// --------------------------------------------------------------------------------------------
		// Reco-Level     

		// Reco Objects
		for( auto is: istring ){
			// Reco Photons
			h[cat+"pho"+is+"_pt"]  = new TH1F( Form( "%spho%s_pt",  cat.c_str(), is.c_str() ), "pt; pt [GeV]; Events ", NBins, 0, 1000);
			h[cat+"pho"+is+"_eta"] = new TH1F( Form( "%spho%s_eta", cat.c_str(), is.c_str() ), "eta; eta; Events", NBins, -3.2, 3.2 );
			h[cat+"pho"+is+"_phi"] = new TH1F( Form( "%spho%s_phi", cat.c_str(), is.c_str() ), "phi; phi; Events", NBins, -3.2, 3.2 );
			// Reco Electrons
			h[cat+"ele"+is+"_pt"]  = new TH1F( Form( "%sele%s_pt",  cat.c_str(), is.c_str() ), "pt; pt [GeV]; Events ", NBins, 0, 1000);
			h[cat+"ele"+is+"_eta"] = new TH1F( Form( "%sele%s_eta", cat.c_str(), is.c_str() ), "eta; eta; Events", NBins, -3.2, 3.2 );
			h[cat+"ele"+is+"_phi"] = new TH1F( Form( "%sele%s_phi", cat.c_str(), is.c_str() ), "phi; phi; Events", NBins, -3.2, 3.2 );
			// Reco Muons
			h[cat+"muon"+is+"_pt"]  = new TH1F( Form( "%smuon%s_pt",  cat.c_str(), is.c_str() ), "pt; pt [GeV]; Events ", NBins, 0, 1000);
			h[cat+"muon"+is+"_eta"] = new TH1F( Form( "%smuon%s_eta", cat.c_str(), is.c_str() ), "eta; eta; Events", NBins, -3.2, 3.2 );
			h[cat+"muon"+is+"_phi"] = new TH1F( Form( "%smuon%s_phi", cat.c_str(), is.c_str() ), "phi; phi; Events", NBins, -3.2, 3.2 );            
			// Reco Jets
			h[cat+"jet"+is+"_pt"]  = new TH1F( Form( "%sjet%s_pt",  cat.c_str(), is.c_str() ), "pt; pt [GeV]; Events ", NBins, 0, 1000);
			h[cat+"jet"+is+"_eta"] = new TH1F( Form( "%sjet%s_eta", cat.c_str(), is.c_str() ), "eta; eta; Events", NBins, -3.2, 3.2 );
			h[cat+"jet"+is+"_phi"] = new TH1F( Form( "%sjet%s_phi", cat.c_str(), is.c_str() ), "phi; phi; Events", NBins, -3.2, 3.2 );
		}

	}

}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::FillHists( string cat ){

	if ( debug ) cout<<"DisplacedHcalJetAnalyzer::FillHists()"<<endl;

	// Check if category exists...
	if ( std::find(categories.begin(), categories.end(), cat) == categories.end() ) return;

	if ( debug ) cout<<"  -- Running on category: "<<cat<<endl;

	cat+= "__";

	if ( isData && blind_data ) return;

	// Reco jets
	if ( debug ) cout << "  -- N Jets = " << n_jet << endl;
	if (n_jet > 0) { // GK
		for (int i=0; i < n_jet; i++) {
			if (i > 2) continue;
			string is = to_string(i);
			h[cat+"jet"+is+"_pt"]->Fill(jet_Pt->at(i) );
			h[cat+"jet"+is+"_eta"]->Fill(jet_Eta->at(i) );
			h[cat+"jet"+is+"_phi"]->Fill(jet_Phi->at(i) );
		}
	} 
	// Reco photons
	if (n_pho > 0) { // GK
		for (int i=0; i < n_pho; i++) {
			if (i > 2) continue;
			string is = to_string(i);
			h[cat+"pho"+is+"_pt"]->Fill(pho_Pt->at(i) );
			h[cat+"pho"+is+"_eta"]->Fill(pho_Eta->at(i) );
			h[cat+"pho"+is+"_phi"]->Fill(pho_Phi->at(i) );
		}
	} 
	// Reco muons
	if (n_muon > 0) { // GK
		for (int i=0; i < n_muon; i++) {
			if (i > 2) continue;
			string is = to_string(i);
			h[cat+"muon"+is+"_pt"]->Fill(muon_Pt->at(i) );
			h[cat+"muon"+is+"_eta"]->Fill(muon_Eta->at(i) );
			h[cat+"muon"+is+"_phi"]->Fill(muon_Phi->at(i) );
		}
	} 
	// Reco electrons
	if (n_ele > 0) { // GK
		for (int i=0; i < n_ele; i++) {
			if (i > 2) continue;
			string is = to_string(i);
			h[cat+"ele"+is+"_pt"]->Fill(ele_Pt->at(i) );
			h[cat+"ele"+is+"_eta"]->Fill(ele_Eta->at(i) );
			h[cat+"ele"+is+"_phi"]->Fill(ele_Phi->at(i) );
		}
	} 
}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::WriteHists(){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::WriteHists()"<<endl;

	cout<<"Writing Histograms..."<<endl;

	for(map<string,TH1F*>::iterator it = h.begin(); it != h.end(); ++it)
		h[it->first]->Write(); 

	for(map<string,TProfile*>::iterator it = hp.begin(); it != hp.end(); ++it) 
		hp[it->first]->Write(); 

	for(map<string,TH2F*>::iterator it = h2.begin(); it != h2.end(); ++it) 
		h2[it->first]->Write();

}