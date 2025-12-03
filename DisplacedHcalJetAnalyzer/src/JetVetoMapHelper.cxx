/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::IsJetInVetoRegion(int iJet) {

    if (debug) cout << "DisplacedHcalJetAnalyzer::IsJetInVetoRegion()" << endl;

    const float eta = jet_Eta->at(iJet);
    const float phi = jet_Phi->at(iJet);
    const float pt  = jet_Pt->at(iJet);

    const float eleEF = jet_EleEFrac->at(iJet);
    const float phoEF = jet_PhoEFrac->at(iJet);
    const float muonEF = jet_MuonEFrac->at(iJet);
    const float chHadEF = jet_ChargedHadEFrac->at(iJet);
    const float neuHadEF = jet_NeutralHadEFrac->at(iJet);

    // Minimal selection: jet pT > 15, tightLepVeto, (charged EM frac + neutral EM frac) < 0.9
    // https://cms-jerc.web.cern.ch/Recommendations/#2023_1 

    // Low-pt jets
    if (pt < 15) return true;
    // tight lepton veto
    if ( eleEF > 0.8 || muonEF > 0.8 ) {
    // if ( abs(eta) > 2.6 || eleEF > 0.8 || jet_ChargedHadMult->at(iJet) <= 0 || chHadEF < 0.01 || phoEF > 0.9 || muonEF > 0.8 || neuHadEF > 0.99 )
    // note second option is missing requirement that number of total constituents is > 1 -- don't have avaliable 
        return true;
    }
    // (charged EM + neutral EM frac) < 0.9
    if ( (eleEF + phoEF) > 0.9 ) return true;

    // Vetoed channels are from ROOT file from JEC
    updateCurrentEraMap(); // try using this so we don't have to load the file for every jet! 
    if (maskMap_) {
        int bin = maskMap_->FindBin(eta, phi);
        float val = maskMap_=>GetBinContent(bin);
        if (val > 0) {
            if (debug) cout << "Jet " << iJet << " vetoed by jet mask map bin = " << bin << " with value = " << val << endl;
        }
        return true; // jet is vetoed
    }

    return false;  // jet is allowed
}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::PassJetVetoEvent() {

    if (debug) cout << "DisplacedHcalJetAnalyzer::PassJetVetoEvent()" << endl;

    for (int i = 0; i < nJets; i++) {
        if (IsJetInVetoRegion(i)) {
            if (debug) cout << "Vetoing event due to jet " << i << endl;
            return false; // reject event
        }
    }

    return true; // keep event
}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::PassJetVetoJet(int iJet) {

    if (debug) cout << "DisplacedHcalJetAnalyzer::PassJetVetoJet()" << endl;
                         << iJet << ")" << std::endl;

    return !IsJetInVetoRegion(iJet);   // keep jet only if not in veto region
}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::updateCurrentEraMap() {

    if (debug) cout << "DisplacedHcalJetAnalyzer::updateCurrentEraMap()" << endl;

    currentEra_ = getEraForRun(runNumber);

    if (vetoMaps_.count(currentEra_))
        maskMap_ = vetoMaps_[currentEra_];
    else
        maskMap_ = nullptr;
}

/* ====================================================================================================================== */
TH2F DisplacedHcalJetAnalyzer::LoadJetVetoMap(std::string filename) {

    if (debug) cout << "DisplacedHcalJetAnalyzer::LoadJetVetoMap()" << endl;

    TFile* f = TFile::Open(filename.c_str());
    if (!f || f->IsZombie())
        if( debug ) cout<< "DisplacedHcalJetAnalyzer::LoadJetVetoMap(): Cannot open veto map file: " << filename << endl;

    TH2F* hMask = (TH2F*)f->Get("jetvetomap");
    if (!hMask)
        if( debug ) cout<<"DisplacedHcalJetAnalyzer::LoadJetVetoMap(): maskMap not found in file: " << filename << endl;

    maskMap_ = (TH2F*)hMask->Clone("jetvetomap_local");
    maskMap_->SetDirectory(0);
    f->Close();

    if (debug) cout << "DisplacedHcalJetAnalyzer::LoadJetVetoMap(): Loaded jet veto map from " << filename << endl;
    return maskMap_;
}

/* ====================================================================================================================== */
string DisplacedHcalJetAnalyzer::getEraForRun(int run) {
    if (run >= 355000 && run < 357900) return "Summer22_23Sep2023";
    if (run >= 357900 && run < 359000) return "Summer22EE_23Sep2023";
    if (run >= 365000 && run < 366500) return "Summer23Prompt23";
    if (run >= 366500) return "Summer23BPixPrompt23";
    return "UNKNOWN";
}


// still need to integrate...

    vetoMaps_["Summer22_23Sep2023"]     = LoadJetVetoMap("Summer22_23Sep2023_RunCD_v1.root");
    vetoMaps_["Summer22EE_23Sep2023"]   = LoadJetVetoMap("Summer22EE_23Sep2023_RunEFG_v1.root");
    vetoMaps_["Summer23Prompt23"]       = LoadJetVetoMap("Summer23Prompt23_RunC_v1.root");
    vetoMaps_["Summer23BPixPrompt23"]   = LoadJetVetoMap("Summer23BPixPrompt23_RunD_v1.root");


