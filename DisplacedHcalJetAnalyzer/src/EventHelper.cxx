/* ====================================================================================================================== */
float DisplacedHcalJetAnalyzer::GetEventRuntime( clock_t clock_start, Long64_t init_entry, Long64_t current_entry ){
	//if( debug ) cout<<"DisplacedHcalJetAnalyzer::GetEventRuntime()"<<endl;
	double duration_sec = (clock()-clock_start)/(double)CLOCKS_PER_SEC;
	float events_per_sec = (current_entry - init_entry)/duration_sec; 
	return events_per_sec;
}

/* ====================================================================================================================== */
void DisplacedHcalJetAnalyzer::ResetGlobalEventVars(){

	if ( debug ) cout<<"DisplacedHcalJetAnalyzer::ResetGlobalEventVars()"<<endl;

	gLLPDecay_iLLP.clear();
	gLLPDecay_iParticle.clear();
	map_gLLP_to_gParticle_indices.clear();

}
