/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::SetHistCategories(){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::SetCategories()"<<endl;

	// histogram category corresponds to selections (jet energy, etc)
	// decides what is ultimately written
	categories = {"NoSel", "JetPt40", "PassedHLT"};

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

		// Reco Objects
		for( auto is: istring ){
			// Reco Photons
			h[cat+"pho"+is+"_pt"]  = new TH1F( Form( "%spho%s_pt",  cat.c_str(), is.c_str() ), "Photon p_{t}; pt [GeV]; Events ", NBins, 0, 1000);
			h[cat+"pho"+is+"_eta"] = new TH1F( Form( "%spho%s_eta", cat.c_str(), is.c_str() ), "Photon #eta; eta; Events", NBins, -3.2, 3.2 );
			h[cat+"pho"+is+"_phi"] = new TH1F( Form( "%spho%s_phi", cat.c_str(), is.c_str() ), "Photon #phi; phi; Events", NBins, -3.2, 3.2 );
			h[cat+"pho"+is+"_energy"] = new TH1F( Form( "%spho%s_energy", cat.c_str(), is.c_str() ), "Photon energy; energy [GeV]; Events", NBins, 0, 1000 );
			// Reco Electrons
			h[cat+"ele"+is+"_pt"]  = new TH1F( Form( "%sele%s_pt",  cat.c_str(), is.c_str() ), "Electron p_{t}; pt [GeV]; Events ", NBins, 0, 1000);
			h[cat+"ele"+is+"_eta"] = new TH1F( Form( "%sele%s_eta", cat.c_str(), is.c_str() ), "Electron #eta; eta; Events", NBins, -3.2, 3.2 );
			h[cat+"ele"+is+"_phi"] = new TH1F( Form( "%sele%s_phi", cat.c_str(), is.c_str() ), "Electron #phi; phi; Events", NBins, -3.2, 3.2 );
			h[cat+"ele"+is+"_energy"] = new TH1F( Form( "%sele%s_energy", cat.c_str(), is.c_str() ), "Electron energy; energy [GeV]; Events", NBins, 0, 1000 );
			// Reco Muons
			h[cat+"muon"+is+"_pt"]  = new TH1F( Form( "%smuon%s_pt",  cat.c_str(), is.c_str() ), "Muon p_{t}; pt [GeV]; Events ", NBins, 0, 1000);
			h[cat+"muon"+is+"_eta"] = new TH1F( Form( "%smuon%s_eta", cat.c_str(), is.c_str() ), "Muon #eta; eta; Events", NBins, -3.2, 3.2 );
			h[cat+"muon"+is+"_phi"] = new TH1F( Form( "%smuon%s_phi", cat.c_str(), is.c_str() ), "Muon #phi; phi; Events", NBins, -3.2, 3.2 );
			h[cat+"muon"+is+"_energy"] = new TH1F( Form( "%smuon%s_energy", cat.c_str(), is.c_str() ), "Muon energy; energy [GeV]; Events", NBins, 0, 1000 );
			// Reco Jets
			h[cat+"jet"+is+"_pt"]  = new TH1F( Form( "%sjet%s_pt",  cat.c_str(), is.c_str() ), "Jet p_{t}; pt [GeV]; Events ", NBins, 0, 1000);
			h[cat+"jet"+is+"_eta"] = new TH1F( Form( "%sjet%s_eta", cat.c_str(), is.c_str() ), "Jet #eta; eta; Events", NBins, -3.2, 3.2 );
			h[cat+"jet"+is+"_phi"] = new TH1F( Form( "%sjet%s_phi", cat.c_str(), is.c_str() ), "Jet #phi; phi; Events", NBins, -3.2, 3.2 );
			h[cat+"jet"+is+"_energy"] = new TH1F( Form( "%sjet%s_energy", cat.c_str(), is.c_str() ), "Jet energy; energy [GeV]; Events", NBins, 0, 1000 );
			h[cat+"jet"+is+"_energyProfile"] = new TH1F(Form("%sjet%s_energyProfile", cat.c_str(), is.c_str() ), "Energy Profile of HB Rechits Associated with Jet; HB Depth; Fraction of Energy", 6,0,6);
			h[cat+"jet"+is+"_rechitN"] = new TH1F(Form("%sjet%s_rechitN", cat.c_str(), is.c_str() ), "Number of HB Rechits Associated with Jet (#Delta R<0.4); Number of HB Rechits; Events", NBins,0,100);
			h[cat+"jet"+is+"_etaSpread"] = new TH1F(Form("%sjet%s_etaSpread", cat.c_str(), is.c_str() ), "#eta Spread of HB Rechits Associated with Jet; #eta Spread; Events", NBins,0,0.5);
			h[cat+"jet"+is+"_etaSpread_energy"] = new TH1F(Form("%sjet%s_etaSpread_energy", cat.c_str(), is.c_str() ), "Energy Weighted #eta Spread of HB Rechits Associated with Jet; Energy weighted #eta Spread; Events", NBins,0,0.5);
			h[cat+"jet"+is+"_phiSpread"] = new TH1F(Form("%sjet%s_phiSpread", cat.c_str(), is.c_str() ), "#phi Spread of HB Rechits Associated with Jet; #phi Spread; Events", NBins,0,0.5);
			h[cat+"jet"+is+"_phiSpread_energy"] = new TH1F(Form("%sjet%s_phiSpread_energy", cat.c_str(), is.c_str() ), "Energy Weighted #phi Spread of HB Rechits Associated with Jet; Energy weighted #phi Spread; Events", NBins,0,0.5);
			h2[cat+"jet"+is+"_spreadEtaPhi"] = new TH2F(Form("%sjet%s_spreadEtaPhi", cat.c_str(), is.c_str() ), "#eta #phi Spread of HB Rechits Associated with Jet (#Delta R<0.4); #eta Spread; #phi Spread", NBins,0,0.5,NBins,0,0.5);
			h2[cat+"jet"+is+"_spreadEtaPhi_energy"] = new TH2F(Form("%sjet%s_spreadEtaPhi_energy", cat.c_str(), is.c_str() ), "Energy Weighted #eta #phi Spread of HB Rechits Associated with Jet (#Delta R<0.4); #eta Spread; #phi Spread", NBins,0,0.5,NBins,0,0.5);
		}
		// HCAL rechits - not leading / subleading object as others are
		h[cat+"hbhe_eta"] = new TH1F( Form( "%shbhe_eta", cat.c_str()), "HBHE #eta; eta; Events", NBins, -3.2, 3.2 );
		h[cat+"hbhe_phi"] = new TH1F( Form( "%shbhe_phi", cat.c_str()), "HBHE #phi; phi; Events", NBins, -3.2, 3.2 );
		h[cat+"hbhe_energy"] = new TH1F( Form( "%shbhe_energy", cat.c_str()), "HBHE energy; energy [GeV]; Events", NBins, 0, 200 );
		h[cat+"hbhe_depth"] = new TH1F( Form( "%shbhe_depth", cat.c_str()), "HBHE depth; depth; Events", 8, 0, 8);
		h[cat+"hbhe_auxTDC"] = new TH1F( Form( "%shbhe_auxTDC", cat.c_str()), "HBHE aux TDC; aux TDC; Events", NBins, 0, 2000000000 );
	}
	// not split by category 
	h["gen_Xdecay"] = new TH1F("gen_Xdecay", "LLP x decay position; X Decay Vertex (cm); Events", 100, -2000, 2000);
	h["gen_Ydecay"] = new TH1F("gen_Ydecay", "LLP y decay position; Y Decay Vertex (cm); Events", 100, -2000, 2000);
	h["gen_Zdecay"] = new TH1F("gen_Zdecay", "LLP z decay position; Z Decay Vertex (cm); Events", 100, -10000, 10000); 
	h["gen_Rdecay"] = new TH1F("gen_Rdecay", "LLP radial decay position; #sqrt{x^{2} + y^{2}} Radial Decay Vertex (cm); Events", 100, 0, 2500); 
	h["gen_Ddecay"] = new TH1F("gen_Ddecay", "LLP decay position; #sqrt{x^{2} + y^{2} + z^{2}} Decay Vertex (cm); Events", 100, 0, 5000); 
	h["gen_cTau"] = new TH1F("gen_cTau", "LLP c#tau; c#tau (cm); Events", 100, 0, 1000); 
	h["gen_deltaT"] = new TH1F("gen_deltaT", "LLP #Delta t; #Delta T (ns); Events", 100, 0, 10); 

	for( auto is: istring ) {
		h["gen_rechitNpt2_"+is] = new TH1F(Form("gen_rechitNpt2_%s",is.c_str()), "Number of HB Rechits Associated with LLP (#Delta R<0.2); Number of HB Rechits; Events", 100,0,100);
		h["gen_rechitNpt4_"+is] = new TH1F(Form("gen_rechitNpt4_%s",is.c_str()), "Number of HB Rechits Associated with LLP (#Delta R<0.4); Number of HB Rechits; Events", 100,0,100);
		h["gen_rechitNpt6_"+is] = new TH1F(Form("gen_rechitNpt6_%s",is.c_str()), "Number of HB Rechits Associated with LLP (#Delta R<0.6); Number of HB Rechits; Events", 100,0,100);
		h2["gen_decay_rechitN_"+is] = new TH2F(Form("gen_decay_rechitN_%s",is.c_str()), "LLP decay position vs. number of HB rechits; Decay Radius (cm); Number of HB Rechits", 100,0,1000,100,0,100);

		h2["gen_depth_energyP_"+is] = new TH2F(Form("gen_depth_energyP_%s",is.c_str()), "Energy Profile of HB Rechits Associated with LLP; HB Depth; Fraction of Energy", 6,0,6,60,0,1.2);
		h["gen_energyP_"+is] = new TH1F(Form("gen_energyP_%s",is.c_str()), "Energy Profile of HB Rechits Associated with LLP; HB Depth; Fraction of Energy", 6,0,6);
		h["gen_energyP_HBstart_"+is] = new TH1F(Form("gen_energyP_HBstart_%s",is.c_str()), "Energy Profile of HB Rechits Associated with LLP (decaying in start of HB); HB Depth; Fraction of Energy", 6,0,6);
		h["gen_energyP_HBend_"+is] = new TH1F(Form("gen_energyP_HBend_%s",is.c_str()), "Energy Profile of HB Rechits Associated with LLP (decaying in end of HB); HB Depth; Fraction of Energy", 6,0,6);
	}
	
}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::FillHists( string cat ){

	if ( debug ) cout<<"DisplacedHcalJetAnalyzer::FillHists()"<<endl;

	// Check if category exists...
	if ( std::find(categories.begin(), categories.end(), cat) == categories.end() ) return;

	if ( debug ) cout << "  -- Running on category: " << cat << endl;

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
			h[cat+"jet"+is+"_energy"]->Fill(jet_E->at(i) );

			vector<float> rechitJet = GetMatchedHcalRechits_Jet(i, 0.4);
			vector<float> energy = GetEnergyProfile_Jet(i, 0.4);
			vector<float> spread_Eta_Phi = GetEtaPhiSpread_Jet(i, 0.4); // eta, phi (average); eta, phi (energy weighted)
			for (int depth = 0; depth < 4; depth++) {
				h[cat+"jet"+is+"_energyProfile"]->Fill(depth + 1, energy[depth]); 
				h[cat+"jet"+is+"_rechitN"]->Fill(rechitJet.size());
				h[cat+"jet"+is+"_etaSpread"]->Fill(spread_Eta_Phi[0]);
				h[cat+"jet"+is+"_etaSpread_energy"]->Fill(spread_Eta_Phi[2]);
				h[cat+"jet"+is+"_phiSpread"]->Fill(spread_Eta_Phi[1]);
				h[cat+"jet"+is+"_phiSpread_energy"]->Fill(spread_Eta_Phi[3]);
				h2[cat+"jet"+is+"_spreadEtaPhi"]->Fill(spread_Eta_Phi[0], spread_Eta_Phi[1]);
				h2[cat+"jet"+is+"_spreadEtaPhi_energy"]->Fill(spread_Eta_Phi[2], spread_Eta_Phi[3]);
			}
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
			h[cat+"pho"+is+"_energy"]->Fill(pho_E->at(i) );
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
			h[cat+"muon"+is+"_energy"]->Fill(muon_E->at(i) );
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
			h[cat+"ele"+is+"_energy"]->Fill(ele_E->at(i) );
		}
	} 
	// HCAL rechits
	if (n_hbheRechit > 0) { // GK
		for (int i=0; i < n_hbheRechit; i++) {
			h[cat+"hbhe_eta"]->Fill(hbheRechit_Eta->at(i) );
			h[cat+"hbhe_phi"]->Fill(hbheRechit_Phi->at(i) );
			h[cat+"hbhe_energy"]->Fill(hbheRechit_E->at(i) );
			h[cat+"hbhe_depth"]->Fill(hbheRechit_depth->at(i) );
			h[cat+"hbhe_auxTDC"]->Fill(hbheRechit_auxTDC->at(i) );
		}
	}

	// LLP gen information plots
	if (cat == "NoSel__") {
		if (n_gLLP > 0) { // make sure gen LLP exists 
			for (int idx_llp = 0; idx_llp < n_gLLP; idx_llp++) {
				double x_LLP = gLLP_DecayVtx_X->at(idx_llp);
				double y_LLP = gLLP_DecayVtx_Y->at(idx_llp);
				double z_LLP = gLLP_DecayVtx_Z->at(idx_llp);
				double radius = sqrt( pow(x_LLP,2) + pow(y_LLP,2) );
				double distance = sqrt( pow(x_LLP,2) + pow(y_LLP,2) + pow(z_LLP,2) );

				// positional quantities
				h["gen_Xdecay"]->Fill(x_LLP);
				h["gen_Ydecay"]->Fill(y_LLP);
				h["gen_Zdecay"]->Fill(z_LLP);
				h["gen_Rdecay"]->Fill(radius);
				h["gen_Ddecay"]->Fill(distance);
				h["gen_cTau"]->Fill(distance * (sqrt( 1 / pow(gLLP_Beta->at(idx_llp),2) - 1)));
				h["gen_deltaT"]->Fill(distance * ( 1 / gLLP_Beta->at(idx_llp) - 1) * 0.03336); // 1/c in ns / cm to give answer in ns

				// rechit multiplicity
				vector<int> n_rechit_pt2 = GetRechitMult(idx_llp, 0.2); // GetRechitMult returns rechit multiplicity associated with LLP, first daughter, second daughter
				vector<int> n_rechit_pt4 = GetRechitMult(idx_llp, 0.4);
				vector<int> n_rechit_pt6 = GetRechitMult(idx_llp, 0.6);
				float decay_radius = abs(GetDecayRadiusHB_LLP(idx_llp));
				
				// energy profile					
				vector<vector<float>> energy = GetEnergyProfile(idx_llp, 0.4); // [0] is LLP, [1] is daughter 1, [2] is daughter 2

				for (int i=0; i<3; i++) { // LLP, daughter 1, daughter 2
					string is = to_string(i);
					h["gen_rechitNpt2_"+is]->Fill(n_rechit_pt2[i]);
					h["gen_rechitNpt4_"+is]->Fill(n_rechit_pt4[i]);
					h["gen_rechitNpt6_"+is]->Fill(n_rechit_pt6[i]);
					h2["gen_decay_rechitN_"+is]->Fill(decay_radius, n_rechit_pt4[i]);

					// if (n_rechit_pt4[i] > 0) std::cout << n_rechit_pt4[i] << " = GetRechitMult(idx_llp, 0.4) for i=" << i << std::endl;
					for (int depth = 0; depth < 4; depth++) {
						h2["gen_depth_energyP_"+is]->Fill(depth + 1, energy[i][depth]);
						h["gen_energyP_"+is]->Fill(depth + 1, energy[i][depth]); // fill TH1D, weight = fractional energy at that depth
						if (decay_radius > 175 && decay_radius < 235) h["gen_energyP_HBstart_"+is]->Fill(depth + 1, energy[i][depth]);
						if (decay_radius > 235 && decay_radius < 295) h["gen_energyP_HBend_"+is]->Fill(depth + 1, energy[i][depth]); // currently all energies are 0 for these matched rechits
					}
				}
			}
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