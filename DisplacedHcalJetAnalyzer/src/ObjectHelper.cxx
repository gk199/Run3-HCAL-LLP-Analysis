// Put object-related functions here (e.g. selection functions, etc)

int DisplacedHcalJetAnalyzer::GetRechitMult(Long64_t jentry, int LLP_number){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetRechitMult()"<<endl;

	fChain->GetEntry(jentry);
	Long64_t ientry = LoadTree(jentry);

	// check gen LLP decay positions if signal (in mm)
	double HB_outer_radius = 2950;
	double HB_inner_radius = 1750;
	double HE_start = 3900;

	double x_LLP = gLLP_DecayVtx_X->at(LLP_number);
	double y_LLP = gLLP_DecayVtx_Y->at(LLP_number);
	double z_LLP = gLLP_DecayVtx_Z->at(LLP_number);
	double LLP_eta = gLLP_Eta->at(LLP_number);
	double LLP_phi = gLLP_Phi->at(LLP_number);
	
	int rechitMult = 0; 

    if ((sqrt( pow(x_LLP,2) + pow(y_LLP,2)) > HB_inner_radius) && (sqrt( pow(x_LLP,2) + pow(y_LLP,2)) < HB_outer_radius) && abs(z_LLP) < HE_start) { 
		// LLP i decay is in HB - now want to find relevant HCAL rechits
		// std::cout << "found gen LLP with position (eta, phi) = " << LLP_eta << ", " << LLP_phi << "\n" << std::endl;

	    for (int j = 0; j < n_hbheRechit; j++) { // loop over HCAL rechits
            rechitMult += 1;
		    double HBHE_eta = hbheRechit_Eta->at(j);
    		double HBHE_phi = hbheRechit_Phi->at(j);
	    	double dR = deltaR(LLP_eta, LLP_phi, HBHE_eta, HBHE_phi); // note this is not a good dR matching b/c LLP eta / phi not reliable!! 
            //std::cout << "LLP eta, phi position = " << LLP_eta << ", " << LLP_phi << "; and HCAL rechit position = " << HBHE_eta << ", " << HBHE_phi << std::endl;
	        //std::cout << "LLP x, y, z position = " << x_LLP << ", " << y_LLP << ", " << z_LLP << "; and HCAL rechit position (mm) = " << hbheRechit_X->at(j)*10 << ", " << hbheRechit_Y->at(j)*10 << ", " << hbheRechit_Z->at(j)*10 << std::endl;
            std::cout << dR << std::endl;
		   	if (dR < 0.5) std::cout << "and gen LLP is matched to a HBHE rechit \n" << std::endl; 
		    if (dR < 0.5) FillHists("genMatchLLP_pt5");
		}
        if (n_hbheRechit > 0) std::cout << "rechitMult = " << rechitMult << " for the LLP " << LLP_number << " with " << n_hbheRechit << " HBHE rechits" << std::endl;
	}

	// FillOutputTrees("");

	return rechitMult;

}