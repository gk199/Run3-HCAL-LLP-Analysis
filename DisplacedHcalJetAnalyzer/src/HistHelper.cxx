/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::SetHistCategories(){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::SetCategories()"<<endl;

	categories = {"PreSel"};

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
	vector<string> istring = {"", "1", "2"};

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
			// Reco Photons //
			h[cat+"ph"+is+"_pt"]  = new TH1F( Form( "%sph%s_pt",  cat.c_str(), is.c_str() ), "pt; pt [GeV]; Events ", NBins, 0, 1000);
			h[cat+"ph"+is+"_eta"] = new TH1F( Form( "%sph%s_eta", cat.c_str(), is.c_str() ), "eta; eta; Events", NBins, -3.2, 3.2 );
			h[cat+"ph"+is+"_phi"] = new TH1F( Form( "%sph%s_phi", cat.c_str(), is.c_str() ), "phi; phi; Events", NBins, -3.2, 3.2 );
			// Reco Electrons
			h[cat+"el"+is+"_pt"]  = new TH1F( Form( "%sel%s_pt",  cat.c_str(), is.c_str() ), "pt; pt [GeV]; Events ", NBins, 0, 1000);
			h[cat+"el"+is+"_eta"] = new TH1F( Form( "%sel%s_eta", cat.c_str(), is.c_str() ), "eta; eta; Events", NBins, -3.2, 3.2 );
			h[cat+"el"+is+"_phi"] = new TH1F( Form( "%sel%s_phi", cat.c_str(), is.c_str() ), "phi; phi; Events", NBins, -3.2, 3.2 );
			// Reco Muons
			h[cat+"mu"+is+"_pt"]  = new TH1F( Form( "%smu%s_pt",  cat.c_str(), is.c_str() ), "pt; pt [GeV]; Events ", NBins, 0, 1000);
			h[cat+"mu"+is+"_eta"] = new TH1F( Form( "%smu%s_eta", cat.c_str(), is.c_str() ), "eta; eta; Events", NBins, -3.2, 3.2 );
			h[cat+"mu"+is+"_phi"] = new TH1F( Form( "%smu%s_phi", cat.c_str(), is.c_str() ), "phi; phi; Events", NBins, -3.2, 3.2 );            
			// Reco Jets
			h[cat+"jet"+is+"_pt"]  = new TH1F( Form( "%sjet%s_pt",  cat.c_str(), is.c_str() ), "pt; pt [GeV]; Events ", NBins, 0, 1000);
			h[cat+"jet"+is+"_eta"] = new TH1F( Form( "%sjet%s_eta", cat.c_str(), is.c_str() ), "eta; eta; Events", NBins, -3.2, 3.2 );
			h[cat+"jet"+is+"_phi"] = new TH1F( Form( "%sjet%s_phi", cat.c_str(), is.c_str() ), "phi; phi; Events", NBins, -3.2, 3.2 );
		}


	}

}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::FillHists( string cat ){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::FillHists()"<<endl;

	// Check if category exists...
	if ( std::find(categories.begin(), categories.end(), cat) == categories.end() ) return;

	if( debug ) cout<<"  -- Running on category: "<<cat<<endl;

	cat+= "__";

	if( isData && blind_data ) return;

	/*if( debug ) cout<<"  -- N photons = "<<ph_i.size()<<endl;
	h[cat+"N_ph"]->Fill( ph_i.size(), weight );
	for( int i = 0; i<ph_i.size(); i++ ){
		string is = Form("%d", i+1);
		int ii = ph_i.at(i);
		// All Photons
		h[cat+"ph_pt"]->Fill( ph_pt->at(ii)/GeV, weight ); //ph_pt_corr.at(i)/GeV  ); //ph_pt->at(i)/GeV ); //ph_pt_corr.at(i)/GeV  );
		h[cat+"ph_eta"]->Fill( ph_eta->at(ii), weight );
		h[cat+"ph_phi"]->Fill( ph_phi->at(ii), weight );
		h[cat+"ph_E"]->Fill( ph_caloCluster_e->at(ii), weight );
	}*/

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