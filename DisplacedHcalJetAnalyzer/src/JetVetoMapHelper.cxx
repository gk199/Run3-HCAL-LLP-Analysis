/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::IsJetInVetoRegion(int iJet) {

    if (debug) cout << "DisplacedHcalJetAnalyzer::IsJetInVetoRegion()" << endl;
    // returning true means event is rejected 

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
    if (debug) cout << "   Checking jet " << iJet << " with pt = " << pt << ", eta = " << eta << ", phi = " << phi << endl;
    if (pt < 15) return false;
    // tight lepton veto
    // JetID = (abs(eta)<=2.6 && CEMF<0.8 && CHM>0 && CHF>0.01 && NumConst>1 && NEMF<0.9 && MUF <0.8 && NHF < 0.99 )
    // note number of total constituents is NPFCands
    if ( abs(eta) > 2.6 || eleEF > 0.8 || jet_ChargedHadMult->at(iJet) <= 0 || chHadEF < 0.01 || jet_NPFCands->at(iJet) <= 1 || phoEF > 0.9 || muonEF > 0.8 || neuHadEF > 0.99 ) {
        if (debug) cout << "Jet " << iJet << " did not pass minimal selection, jet not checked against veto map" << endl;
        return false;
    }

    // (charged EM + neutral EM frac) < 0.9
    if ( (eleEF + phoEF) > 0.9 ) return false;
    if (debug) cout << "   Jet " << iJet << " passed all preselections" << endl;
    // if made it to here, have met the minimal selections. Now check if jet is in veto region

    // Vetoed channels are from ROOT file from JEC
    updateCurrentEraMap(); // try using this so we don't have to load the file for every jet! 
    if (maskMap_) {
        int bin = maskMap_->FindBin(eta, phi);
        float val = maskMap_->GetBinContent(bin);
        if (val > 0) {
            if (debug) cout << "Jet " << iJet << " with eta, phi" << eta << ", " << phi << " vetoed by jet mask map bin = " << bin << " with value = " << val << endl;
            return true; // jet is vetoed
        }
    }
    return false;  // jet is allowed
}

/* ====================================================================================================================== */
bool DisplacedHcalJetAnalyzer::PassJetVetoEvent() {

    if (debug) cout << "DisplacedHcalJetAnalyzer::PassJetVetoEvent()" << endl;

    for (int i = 0; i < jet_Pt->size(); i++) {
        if (IsJetInVetoRegion(i)) {
            if (debug) cout << "Vetoing event due to jet " << i << endl;
            return false; // reject event
        }
    }

    return true; // keep event
}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::updateCurrentEraMap() {

    if (debug) cout << "DisplacedHcalJetAnalyzer::updateCurrentEraMap()" << endl;

    // era is instead read directly from the input root file 
    // currentEra_ = *Era; // TO DO get this when era is in root file
    // currentEra_ = "Run2023D"; // temporary hardcode for testing

    // determine which veto map to use based on current era
    std::string vetoKey = eraToVetoMapKey(currentEra_);

    // look up the veto map
    auto it = vetoMaps_.find(vetoKey);
    if (it != vetoMaps_.end()) {
        maskMap_ = it->second;
    } else {
        maskMap_ = nullptr;
        if( debug ) cout<< "DisplacedHcalJetAnalyzer::updateCurrentEraMap(): No veto map found for era: " << currentEra_ << endl;
    }
}

/* ====================================================================================================================== */
TH2F* DisplacedHcalJetAnalyzer::LoadJetVetoMap(const std::string& filename) {

    if (debug) cout << "DisplacedHcalJetAnalyzer::LoadJetVetoMap()" << endl;

    TFile* f = TFile::Open(filename.c_str(), "READ"); // read only
    if (!f || f->IsZombie()) {
        if( debug ) cout<< "DisplacedHcalJetAnalyzer::LoadJetVetoMap(): Cannot open veto map file: " << filename << endl;
        return nullptr;
    }

    TH2F* hMask = (TH2F*)f->Get("jetvetomap");
    if (!hMask) {
        if( debug ) cout<<"DisplacedHcalJetAnalyzer::LoadJetVetoMap(): maskMap not found in file: " << filename << endl;
        f->Close();
        delete f;
        return nullptr;
    }

    // Clone the histogram to avoid issues when closing the file
    TH2F* h_localMask = (TH2F*)hMask->Clone();
    h_localMask->SetDirectory(0);
    f->Close();

    if (debug) cout << "DisplacedHcalJetAnalyzer::LoadJetVetoMap(): Loaded jet veto map from " << filename << endl;
    return h_localMask;
}

/* ====================================================================================================================== */
std::string DisplacedHcalJetAnalyzer::eraToVetoMapKey(const std::string& era) {
    
    if (debug) cout << "DisplacedHcalJetAnalyzer::eraToVetoMapKey()" << endl;

    // All possible era strings and the veto map key they correspond to:

    if (era == "Run2022C" || era == "Run2022D" || era == "2022preEE")
        return "Summer22_23Sep2023";

    if (era == "Run2022E" || era == "Run2022F" || era == "Run2022G" || era == "2022postEE")
        return "Summer22EE_23Sep2023";

    if (era == "Run2023C" || era == "2023preBPix")
        return "Summer23Prompt23";

    if (era == "Run2023D" || era == "2023postBPix")
        return "Summer23BPixPrompt23";

    // Default
    return "UNKNOWN";
}
/* ====================================================================================================================== */