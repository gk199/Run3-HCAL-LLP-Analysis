/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::SetTriggerNames( string infilepath, string hist_name ){

	if( debug ) cout<<"DisplacedHcalJetAnalyzer::SetTriggerNames()"<<endl;

	TFile *f = new TFile( Form("%s", infilepath.c_str() ) );
	TH1F  *h_hlt = (TH1F*) f->Get( Form( "%s", hist_name.c_str() ) );

	cout<<"Reading in triggers:"<<endl;
	for( int i=0; i<h_hlt->GetNbinsX(); i++ ){
		string hlt_name_temp = h_hlt->GetXaxis()->GetBinLabel(i+1);
		cout<<" --> "<<hlt_name_temp<<endl;
		HLT_Names.push_back( hlt_name_temp );
		HLT_Indices[hlt_name_temp] = i; // Index at which to access HLT_Decision and HLT_Prescale
	}
	f->Close();

}
