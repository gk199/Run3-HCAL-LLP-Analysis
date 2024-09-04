/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::SetTriggerNames( string infilepath, string hist_name ){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::SetTriggerNames()"<<endl;

	TFile *f = TFile::Open( Form("%s", infilepath.c_str() ) ); // issue reading in remote file, solved with TFile::Open instead of TFile constructor 
	
	TH1F  *h_hlt = (TH1F*) f->Get( Form( "%s", hist_name.c_str() ) );

	cout<<"Reading in triggers:"<<endl;
	int N_trig_other = 0;
	for( int i=0; i<h_hlt->GetNbinsX(); i++ ){
		string hlt_name_temp = h_hlt->GetXaxis()->GetBinLabel(i+1);
		if( hlt_name_temp.find("L1SingleLLPJet") != string::npos ) 
			cout<<" --> "<<hlt_name_temp<<endl;
		else N_trig_other++; 
		HLT_Names.push_back( hlt_name_temp );
		HLT_Indices[hlt_name_temp] = i; // Index at which to access HLT_Decision and HLT_Prescale
	}
	cout<<" --> "<<N_trig_other<<" other triggers also included"<<endl;
	
	f->Close();

}
