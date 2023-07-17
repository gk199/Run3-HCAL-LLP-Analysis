#include "PlotParams.h"
#include "PlotFunctions.h"

// -------------------------------------------------------------------------------------
class MiniTuplePlotter{
public :

	bool debug = false;

	// Inputs
	vector<string> filetags;
	vector<string> treenames; // Expert Only, default blank. Still needs to be finalized & validated
	vector<string> filetags_treenames;
	map<string, TTree*> trees;
	bool use_single_treename = true;
	TString TreeName = "NoSel";

	vector<string> hist_tags;	
	string infile_path;
	string infile_ext;
	vector<PlotParams> PlotParamsList;
	TString MiniTupleVersion = "";

	string output_file_tag = ""; 

	// Cuts
	TCut cuts_all;
	vector<TCut> cuts_compare = {""};
	map<string,TCut> selective_cuts;

	// Plot Options
	vector<Color_t> colors    = { kBlack, kMagenta-7, kRed, kOrange, kGreen+2, kAzure+7, kBlue+2 };
	vector<Style_t> linestyle = { kSolid, kSolid, kSolid,  kSolid, kSolid, kSolid, kSolid };
	bool plot_log  = true; 
	bool plot_log_x = false; 
	bool plot_log_ratio = false; // set to true if you want a log ratio or s/sqrtb panel
	bool plot_norm = true;
	bool plot_cdf  = false;
	bool plot_reverse_cdf  = false;
	bool plot_grid = true; 
	bool manual_legend = false;
	bool stamp_counts = false; 
	bool shift_center = false; 
	double scale_data = 1;
	double legx1, legx2, legy1, legy2;

	Int_t NBins = 100; 
	bool set_variable_bins = false;
	vector<Double_t> Bins_variable;

	// Fit to write to legend
	bool run_fit    = false;
	bool run_fit2D  = false;
	string fit_type = "igf_sigma tailfrac";

	// Reweighting
	bool use_weight = true;
	bool run_reweight = false; 
	PlotParams P_reweight_to;
	TH1F* h_reweight; 
	bool run_reweight2D = false; 
	PlotParams P_reweight_to_1;
	PlotParams P_reweight_to_2;
	TH2F* h2_reweight;
		
	// Significance formula
	string significance_formula = "ssqrtb";

	// Outputs
	//TFile *fout;

	// -------------------------------------------------------------------------------------
	MiniTuplePlotter(	vector<string> IN_filetags, 
						string IN_infile_path 	= "../MiniTuples/v1.0/minituple_", 
						string IN_infile_ext 	= ".root",
						TString IN_MiniTupleVersion = "" ){

		SetStyle();

		filetags 	= IN_filetags;
		infile_path = IN_infile_path;
		infile_ext 	= IN_infile_ext;

		SetTreeName( "PostSel" ); // Default

		// Get MiniTuple Version

		if( IN_MiniTupleVersion != "" ){
			MiniTupleVersion = IN_MiniTupleVersion;
		} else {
			TString infile_path_tstr = Form("%s", infile_path.c_str());
			TObjArray *tarr = (TObjArray*)infile_path_tstr.Tokenize("/");
			for( Int_t i=0; i<tarr->GetEntries(); i++){
				TString element = ((TObjString *)(tarr->At(i)))->String();
				if( element.Contains("v") ){
					MiniTupleVersion = element;
					break;
				}
			}
		}	
		cout<<"NOTE: Reading in MiniTupleVersion as \""<<MiniTupleVersion<<"\""<<endl;
		cout<<"    --> If this is incorrect please fix it manually when you declare the instance of the plotter :)"<<endl;

		//fout = new TFile( "Figures/Figures_test.root", "RECREATE");

	}

	// -------------------------------------------------------------------------------------
	~MiniTuplePlotter(){
		//fout->Close();
	}

	// -------------------------------------------------------------------------------------
	void SetTreeName( string IN_treename ){

		if( debug) cout<<"MiniTuplePlotter::SetTreeNames()"<<endl;

		treenames.clear();

		for( int i=0; i<filetags.size(); i++ )
			treenames.push_back( IN_treename );

		use_single_treename = true;

	}

	// -------------------------------------------------------------------------------------
	void SetTreeNames( vector<string> IN_treenames ){

		if( debug) cout<<"MiniTuplePlotter::ReadFromMultipleTrees()"<<endl;

		if( treenames.size() != filetags.size() ){

			cout<<"WARNING: Number of treenames != number of input filetags -- they must be 1-to-1"<<endl;

			if( IN_treenames.size() > 0 ){
				cout<<"         --> Will only read from: "<<IN_treenames.at(0)<<endl;
				SetTreeName( IN_treenames.at(0) );
			} else if( treenames.size() > 0 ){
				cout<<"         --> Will only read from: "<<treenames.at(0)<<endl;
				SetTreeName( treenames.at(0) );
			} else { 
				cout<<"         --> Will only read from: PostSel"<<endl;
				SetTreeName( "PostSel" );				
			}
			return;
		}

		use_single_treename = false; 

		treenames = IN_treenames;

	}

	// -------------------------------------------------------------------------------------
	string GetFiletagTreename( string filetag, string treename){

		if( debug) cout<<"MiniTuplePlotter::GetFiletagTreename()"<<endl;

		return filetag+" | "+treename;
	}

	// -------------------------------------------------------------------------------------
	bool GetTree(string filetag, string treename ){

		if( debug) cout<<"MiniTuplePlotter::GetTree()"<<endl;

		string filetag_treename = GetFiletagTreename( filetag, treename);

		for(map<string,TTree*>::iterator it = trees.begin(); it != trees.end(); ++it){
			if( it->first == filetag_treename ){
				cout<<"NOTE: tree "<<treename<<" associated with "<<filetag<<" already exists."<<endl;
				return true;
			}
		}

		TString filename = Form("%s%s%s", infile_path.c_str(), filetag.c_str(), infile_ext.c_str() );

		if( gSystem->AccessPathName(filename) ){
			cout<<"ERROR: File "<<filename<<" does not exist"<<endl;
			return false;
		}

		cout<<"Reading in "<<filename<<endl;
		TFile* file = new TFile( filename, "READ");

		if( !file->GetListOfKeys()->Contains( Form("%s", treename.c_str()) ) ){
			cout<<"ERROR: Tree "<<treename<<" does not exist in "<<filename<<endl;
			return false;
		}

		TTree* tree_temp = (TTree*)file->Get( Form("%s", treename.c_str()) );
		trees[filetag_treename] = (TTree*)tree_temp->Clone(); 

		return true;

	}

	// -------------------------------------------------------------------------------------
	void GetTrees(){
		if( debug) cout<<"MiniTuplePlotter::GetTrees()"<<endl;

		bool trees_ok = false;
		for( int i=0; i<filetags.size(); i++ ){
			if( GetTree(filetags[i], treenames[i]) ){
				trees_ok = true;
				filetags_treenames.push_back( GetFiletagTreename( filetags[i], treenames[i]) );
			}
		}

		if( !trees_ok ) cout<<"ERROR: Input files or trees do not exist. Check input file paths & parameters.."<<endl;
	}

	// -------------------------------------------------------------------------------------
	string GetFitInfo(TH1F *h, string units = ""){
		if( debug) cout<<"MiniTuplePlotter::GetFitInfo()"<<endl;

		string fit_info_str = "";

		float mean, sigma; 
		if( fit_type.find("igf") != string::npos || fit_type.find("tailfrac") != string::npos ){
			TF1* corefit = IterativeGaussianFit( h );
			float scale  = corefit->GetParameter(0);
			mean   = corefit->GetParameter(1);
			sigma  = corefit->GetParameter(2); 

			if( fit_type.find("igf_sigma") != string::npos )
				fit_info_str += Form("sigma = %.3f %s, ", sigma, units.c_str() );
			if( fit_type.find("igf_mean") != string::npos )
				fit_info_str += Form("mean = %.2f %s, ", mean, units.c_str() );
		}

		if( fit_type.find("tail_spread") != string::npos ){
			TF1* tailfit = LorentzCauchyFit( h ); 
			float spread = tailfit->GetParameter(2); 
			fit_info_str += Form("tail_spread = %.2f %s, ", spread, units.c_str() );
		}

		if( fit_type.find("tailfrac") != string::npos ){
			float frac_outside_1p5sigma;
			int b_neg = h->GetXaxis()->FindBin(mean-sigma*1.5); 
			int b_pos = h->GetXaxis()->FindBin(mean+sigma*1.5); 
			float n_neg = h->Integral( 0, b_neg);
			float n_pos = h->Integral( b_pos, h->GetNbinsX()+1);
			frac_outside_1p5sigma = (n_neg + n_pos) / h->Integral();
			fit_info_str += Form("tailfrac = %.2f %s, ", frac_outside_1p5sigma, units.c_str() );
		}

		if( fit_type.find("exp_decay") != string::npos ){

			TF1* fit = new TF1("fit_expdecay","expo"); // exponential decay
			h->Fit( fit, "Q");
			float scale  = fit->GetParameter(0);
			float ctau   = -1/fit->GetParameter(1);
			float c      = 300.0; // mm/ns
			double tau   = ctau / c ;
			//fit_info_str += Form("c#tau = %.2f mm, #tau = %.2f ns, ", ctau, tau );
			fit_info_str += Form("decaylen = %.2f %s", ctau, units.c_str() );

		}	

		if( fit_type.find("fixed_gaus") != string::npos ){

			TF1* fit = new TF1("fit_fixedgaus","[0]*exp(-0.5*(x/[1])^2)"); // exponential decay
			h->Fit( fit, "Q");
			float scale  = fit->GetParameter(0);
			float sigma  = fit->GetParameter(1);
			fit_info_str += Form("fixedgaus: %.2f %.2f", scale,sigma );

		}			

		return fit_info_str;
	}

	// -------------------------------------------------------------------------------------
	void SetLegendManual( double legx1_temp, double legx2_temp, double legy1_temp, double legy2_temp ){
		if( debug) cout<<"MiniTuplePlotter::SetLegendManual()"<<endl;

		legx1 = legx1_temp;
		legx2 = legx2_temp;
		legy1 = legy1_temp;
		legy2 = legy2_temp;
		manual_legend = true;
	}

	// -------------------------------------------------------------------------------------
	void SetOutputFileTag(string output_file_tag_tmp){
		if( debug) cout<<"MiniTuplePlotter::SetOutputFileTag()"<<endl;

		output_file_tag = output_file_tag_tmp;
	}

	// -------------------------------------------------------------------------------------
	TString GetOutputFileName( PlotParams myPlotParams, string plot_type = ""){
		if( debug) cout<<"MiniTuplePlotter::GetOutputFileName()"<<endl;		

		string output_file_name = "Plot"+plot_type;

		if( plot_type == "sig" ) 
			output_file_name += "_"+significance_formula;

		output_file_name += "_"+myPlotParams.hist_name;

		if( plot_norm ) output_file_name += "_norm";
		if( plot_log  ) output_file_name += "_log";		
		if( plot_cdf  ) output_file_name += "_cdf";
		if( plot_reverse_cdf ) output_file_name += "_Rcdf";
		if( run_reweight ) output_file_name += "_RW";
		if( run_reweight2D ) output_file_name += "_RW2D";
		if( output_file_tag != "" ) output_file_name += "_"+output_file_tag;

		TString output_file_name_temp = Form("%s",output_file_name.c_str() );
		output_file_name_temp.ReplaceAll("(", "");
		output_file_name_temp.ReplaceAll(")", "");
		output_file_name_temp.ReplaceAll("**", "POW");
		output_file_name_temp.ReplaceAll("*", "MULT");
		output_file_name_temp.ReplaceAll("/", "DIV");	
		output_file_name_temp.ReplaceAll(">", "GT");	
		output_file_name_temp.ReplaceAll("<", "LT");		

		return output_file_name_temp;
	} 	

	// -------------------------------------------------------------------------------------
	void SetPlots(vector<PlotParams> myPlotParamsList){
		if( debug) cout<<"MiniTuplePlotter::SetPlots()"<<endl;		

		PlotParamsList = myPlotParamsList;
	} 

	// -------------------------------------------------------------------------------------
	void AddPlot( PlotParams myPlotParams ){
		if( debug) cout<<"MiniTuplePlotter::AddPlot()"<<endl;	

		PlotParamsList.push_back( myPlotParams );
	}

	// -------------------------------------------------------------------------------------
	void SetCuts( TCut new_cut ){
		if( debug) cout<<"MiniTuplePlotter::SetCuts()"<<endl;		

		cuts_all = new_cut;	
	}

	// -------------------------------------------------------------------------------------
	void AddCut( TCut new_cut ){
		if( debug) cout<<"MiniTuplePlotter::AddCut()"<<endl;

		cuts_all = cuts_all && new_cut;
	}

	// -------------------------------------------------------------------------------------
	void SetComparisonCuts( vector<TCut> cuts_compare_input ){
		if( debug) cout<<"MiniTuplePlotter::SetComparisonCuts()"<<endl;		

		if( cuts_compare_input.size() == 0 )
			cuts_compare = {""};
		else
			cuts_compare = cuts_compare_input; 
	}	

	// -------------------------------------------------------------------------------------
	void ApplySelectiveCuts( string match_filetag_or_treename, TCut cut ){
		if( debug) cout<<"MiniTuplePlotter::ApplySelectiveCuts()"<<endl;
		// filetag_or_treename can be either -- there is string matching

		for( auto filetag_treename: filetags_treenames ){
			if( filetag_treename.find( match_filetag_or_treename ) != string::npos ){
				selective_cuts[filetag_treename] = selective_cuts[filetag_treename] && cut;
			}
		}	
	}

	// -------------------------------------------------------------------------------------
	TString GetBetterCutTitle( TCut cut ){
		if( debug) cout<<"MiniTuplePlotter::GetBetterCutTitle()"<<endl;		

		TString title = cut.GetTitle(); 
		title.ReplaceAll("ph_detector_region == ", "");
		title.ReplaceAll("el_detector_region == ", "");
		title.ReplaceAll("ph_timing_cat == ", "");
		title.ReplaceAll("el_timing_cat == ", "");		
		title.ReplaceAll("\"", "");
		title.ReplaceAll("/", "DIV");
		title.ReplaceAll("(", "");
		title.ReplaceAll(")", "");
		return title;
	}

	// -------------------------------------------------------------------------------------
	TString FormatMyString( string my_string ){
		if( debug) cout<<"MiniTuplePlotter::GetBetterCutTitle()"<<endl;		

		TString my_TString = Form("%s", my_string.c_str());	
		my_TString.ReplaceAll(" ", "");
		my_TString.ReplaceAll("(", "");
		my_TString.ReplaceAll(")", "");

		return my_TString;
	}

	// -------------------------------------------------------------------------------------
	void StampCuts( float x=0.14, float y=0.84, float textsize=0.03, TString stamp_string="") {
		if( debug) cout<<"MiniTuplePlotter::StampCuts()"<<endl;		

		TLatex stamp_text;
		stamp_text.SetNDC();
		//stamp_text.SetTextFont(42);
		stamp_text.SetTextFont(42);
		stamp_text.SetTextColor(kGray+1);
		stamp_text.SetTextSize(textsize);

		string treename = "";
		if( use_single_treename ) treename = treenames[0];

		if( stamp_string == "" )
			stamp_text.DrawLatex( x, y, Form( MiniTupleVersion+" %s -- "+GetBetterCutTitle(cuts_all), treename.c_str() ) );
		else 
			stamp_text.DrawLatex( x, y, stamp_string );

	}

	// -------------------------------------------------------------------------------------
	void SetVariableBins( vector<Double_t> Bins_variable_temp ){
		if( debug) cout<<"MiniTuplePlotter::SetVariableBins()"<<endl;		

		set_variable_bins = true;
		Bins_variable = Bins_variable_temp;

	}

	// -------------------------------------------------------------------------------------
	void RunReweighting1D(PlotParams target_PlotParams, string target_filetag, string target_treename, TCut target_cut){
		if( debug) cout<<"MiniTuplePlotter::RunReweighting1D()"<<endl;		

		// Get Tree
		if( !GetTree(target_filetag, target_treename) ) {
			cout<<"ERROR: Could not access target file or tree for reweighting."<<endl;
			return;
		}

		run_reweight   = true; 
		run_reweight2D = false; 
		P_reweight_to  = target_PlotParams;

		string target_filetag_treename = GetFiletagTreename( target_filetag, target_treename);
		TH1F* h_temp = (TH1F*)GetHist1D( target_PlotParams, target_filetag_treename, target_cut, "RW_Target_");
		h_reweight   = (TH1F*)h_temp->Clone(); 

		cout<<" --> Target Reweighting Histogram has "<<h_reweight->GetEntries()<<" entries"<<endl;
		if( h_reweight->GetEntries() == 0 )
			cout<<"ERROR: Reweighting histogram is empty. This will cause the program to crash."<<endl;

		TCanvas *c_reweight = new TCanvas("c_reweight", "c_reweight", 1600, 1200);

		h_reweight->Draw("HIST SAME E");
		if( plot_log ) c_reweight->SetLogy();
		StampCuts( 0.1, 0.91, 0.015, Form("FileRel "+MiniTupleVersion+", Target Filetag: %s, Target Tree: %s, Target Cuts: "+GetBetterCutTitle(target_cut), target_treename.c_str(), target_filetag.c_str() ) );
		c_reweight->SaveAs( Form("Plots/ReweightTo__%s__%s__%s__%s.png", target_filetag.c_str(), target_treename.c_str(), target_PlotParams.hist_name.c_str(), output_file_tag.c_str() ) );
		delete c_reweight; 
	}

	// -------------------------------------------------------------------------------------
	void RunReweighting2D(PlotParams target_PlotParams_1, PlotParams target_PlotParams_2, string target_filetag, string target_treename, TCut target_cut ){

		if( debug) cout<<"MiniTuplePlotter::RunReweighting2D()"<<endl;	

		// Get Tree
		if( !GetTree(target_filetag, target_treename) ) {
			cout<<"ERROR: Could not access target file or tree for reweighting."<<endl;
			return;
		}

		run_reweight2D  = true; 
		run_reweight    = false; 
		P_reweight_to_1 = target_PlotParams_1;
		P_reweight_to_2 = target_PlotParams_2;

		string target_filetag_treename = GetFiletagTreename( target_filetag, target_treename);
		TH2F* h_temp = (TH2F*)GetHist2D( target_PlotParams_1, target_PlotParams_2, target_filetag_treename, target_cut, "RW2D_Target_");
		h2_reweight  = (TH2F*)h_temp->Clone(); 

		cout<<" --> Target 2D Reweighting Histogram has "<<h2_reweight->GetEntries()<<" entries"<<endl;
		if( h2_reweight->GetEntries() == 0 )
			cout<<"ERROR: Reweighting histogram is empty. This will cause the program to crash."<<endl;

		TCanvas *c_reweight = new TCanvas("c_reweight", "c_reweight", 1600, 1200);

		h2_reweight->Draw("colz TEXT");
		if( plot_log ){ c_reweight->SetLogx(); c_reweight->SetLogy(); }
		StampCuts( 0.1, 0.91, 0.015, Form("FileRel "+MiniTupleVersion+", Target Filetag: %s, Target Tree: %s, Target Cuts: "+GetBetterCutTitle(target_cut), target_treename.c_str(), target_filetag.c_str() ) );
		c_reweight->SaveAs( Form("Plots/ReweightTo__%s__%s__%s__%s.png", target_filetag.c_str(), target_PlotParams_1.hist_name.c_str(), target_PlotParams_2.hist_name.c_str(), output_file_tag.c_str() ) );
		delete c_reweight;
	}	

	// -------------------------------------------------------------------------------------
	TH1F* GetHist1DReweighted( PlotParams myPlotParams, string filetag_treename, TCut cut_compare, double manual_shift = 0.0 ){
		if( debug) cout<<"MiniTuplePlotter::GetHist1DReweighted()"<<endl;		

		string hist_name = myPlotParams.hist_name;
		string label_x 	 = myPlotParams.label_x;
		double xmin 	 = myPlotParams.xmin;
		double xmax 	 = myPlotParams.xmax;

		TH2F* h2 = (TH2F*)GetHist2D( myPlotParams, P_reweight_to, filetag_treename, cut_compare, "RW_", manual_shift );

		if( shift_center && manual_shift == 0.0 ) NBins *= 1000;
		TH1F* h  = (TH1F*)GetHist1D( myPlotParams, filetag_treename, cut_compare );
		h->Reset(); 
		if( shift_center && manual_shift == 0.0 ) NBins /= 1000;

		TH1F* h_SFs = (TH1F*)h_reweight->Clone(); 
		h_SFs->Reset();

		int NbinsY = h2->GetNbinsY();
		for( int ib = 1; ib < NbinsY+1; ib++ ){
			TH1F* h_slice = (TH1F*)h2->ProjectionX("h_slice", ib, ib);

			double N_RW_from = h_slice->Integral();
			double N_RW_to   = h_reweight->GetBinContent(ib);
			double SF = N_RW_to/N_RW_from;

			double Nentries_RW_from = h_slice->GetEntries();
			double Nentries_RW_to   = h_reweight->GetBinError(ib);			
			double SF_err = SF*( pow( Nentries_RW_from, 0.5 )/Nentries_RW_from + pow( Nentries_RW_to, 0.5 )/Nentries_RW_to );

			if( N_RW_from == 0 || N_RW_to == 0 ){
				SF = 0;
				SF_err = 0;
			}

			h_SFs->SetBinContent( ib, SF );
			h_SFs->SetBinError(ib, SF_err );

			h_slice->Scale( SF );

			h->Add( h_slice );
		}

		if( shift_center && manual_shift == 0.0 ){
			double shift = h->GetMean();
			h = GetHist1DReweighted( myPlotParams, filetag_treename, cut_compare, shift );
		}

		/*
		TCanvas *c_sf = new TCanvas();
		h_SFs->Draw();
		*/

		return h;
	}

	// -------------------------------------------------------------------------------------
	TH1F* GetHist1DReweighted2D( PlotParams myPlotParams, string filetag_treename, TCut cut_compare ){
		if( debug) cout<<"MiniTuplePlotter::GetHist1DReweighted()"<<endl;		

		string hist_name = myPlotParams.hist_name;
		string label_x 	 = myPlotParams.label_x;
		double xmin 	 = myPlotParams.xmin;
		double xmax 	 = myPlotParams.xmax;

		TH3F* h3 = (TH3F*)GetHist3D( myPlotParams, P_reweight_to_1, P_reweight_to_2, filetag_treename, cut_compare, "RW2D_" );
		TH1F* h  = (TH1F*)GetHist1D( myPlotParams, filetag_treename, cut_compare );
		h->Reset(); 
		//TH1F* h_SFs = (TH1F*)h->Clone(); 

		int NbinsY = h3->GetNbinsY();
		int NbinsZ = h3->GetNbinsZ();

		int rebinning_factor_y = 1;
		int rebinning_factor_z = 1;
		int N_emptybins = 0;

		bool shifty, shiftx;

		for( int iby = 1; iby < NbinsY+1; iby+=rebinning_factor_y ){
			for( int ibz = 1; ibz < NbinsZ+1; ibz+=rebinning_factor_z ){

				TH1F* h_slice;
				double SF, SF_err;

				// Standard
				while( true ){

					h_slice = (TH1F*)h3->ProjectionX("h_slice", iby, iby+rebinning_factor_y-1, ibz, ibz+rebinning_factor_z-1);

					double N_RW_from = h_slice->Integral();
					double N_RW_to   = h2_reweight->Integral(iby,iby+rebinning_factor_y-1,ibz,ibz+rebinning_factor_z-1);
					SF = N_RW_to/N_RW_from;

					double Nentries_RW_from = h_slice->GetEntries();
					double Nentries_RW_to   = h2_reweight->GetBinError(iby,ibz);			
					SF_err = SF*( pow( Nentries_RW_from, 0.5 )/Nentries_RW_from + pow( Nentries_RW_to, 0.5 )/Nentries_RW_to );

					if( N_RW_from == N_RW_to ){ // for when both are zero
						SF = 1;
						SF_err = 0;
					} else if( N_RW_from == 0 ||  N_RW_to == 0 ){
						N_emptybins++;
						SF = 0;
						SF_err = 0;
					}
					break;

				}

				// If one but not both of the values is zero:
				/*if( N_RW_from == 0 ||  N_RW_to == 0 && N_RW_from != N_RW_to ){

					while( true ){
						rebinning_factor+=1;
						TH1F* h_slice = (TH1F*)h3->ProjectionX("h_slice", iby, iby+rebinning_factor-1, ibz, ibz+rebinning_factor-1);
						double N_RW_from = h_slice->Integral();
						double N_RW_to   = h2_reweight->GetBinContent(iby,ibz);
					}

				}*/

				//h_SFs->SetBinContent( ib, SF );
				//h_SFs->SetBinError(ib, SF_err );

				h_slice->Scale( SF );

				h->Add( h_slice );

			}
		}

		if( N_emptybins != 0 ) 
			cout<<"WARNING: There were "<<N_emptybins<<" empty bins in 2D RW"<<endl;

		return h;

	}	

	// -------------------------------------------------------------------------------------
	TH1F* GetHist1D( PlotParams myPlotParams, string filetag_treename, TCut cut_compare, string hist_tag_prepend = ""){
		if( debug) cout<<"MiniTuplePlotter::GetHist1D()"<<endl;		

		string hist_name = myPlotParams.hist_name;
		string label_x 	 = myPlotParams.label_x;
		double xmin 	 = myPlotParams.xmin;
		double xmax 	 = myPlotParams.xmax;

		if( cut_compare == "-1" )
			hist_name = "-1*"+myPlotParams.hist_name;

		TCanvas *c_temp = new TCanvas();

		TString hist_name_full = FormatMyString( Form("%s%s__%s__"+GetBetterCutTitle( cut_compare ), hist_tag_prepend.c_str(), filetag_treename.c_str(), hist_name.c_str()  ) );

		cout<<"-> Getting "<<hist_name_full<<endl;

		TH1F *h_temp;

		if( set_variable_bins && hist_tag_prepend.find("RW") == string::npos ){
			const int NBins_temp = Bins_variable.size() - 1;
			Double_t Bins_temp[NBins_temp+1];
			std::copy(Bins_variable.begin(), Bins_variable.end(), Bins_temp );
			h_temp = new TH1F( hist_name_full, "", NBins_temp, Bins_temp); 		
		} else if( hist_tag_prepend.find("RW") != string::npos && hist_name.find("maxEcell_E") != string::npos ){
			Int_t NBins_temp = 13;
			Double_t Bins_temp[14] = {10, 15, 20, 25, 30, 35, 40, 50, 60, 70, 80, 100, 150, 250};
			h_temp = new TH1F( hist_name_full, "", NBins_temp, Bins_temp); 	
		} else {
			h_temp = new TH1F( hist_name_full, "", NBins, xmin, xmax); 	
		}

		TCut cut_weight  = Form( "weight" ); 
		TCut cut_total   = cut_weight * (cuts_all && cut_compare && selective_cuts[filetag_treename]);
		if( !use_weight ) cut_total = (cuts_all && cut_compare && selective_cuts[filetag_treename]);

		if( hist_tag_prepend.find("RW") != string::npos && hist_tag_prepend.find("Target") != string::npos ){
			cut_total = cut_weight * (cut_compare && selective_cuts[filetag_treename]);
			if( !use_weight ) cut_total = (cut_compare && selective_cuts[filetag_treename]);
		}

		double shift = 0.0;
		if( shift_center && !run_reweight ){ //&& hist_name.find("_t") != string::npos ){

			TH1F *h_finelybinned = new TH1F( "Fine_"+hist_name_full, "", NBins*10000, xmin*3., xmax*3.); 
			trees[filetag_treename]->Draw( Form( "%s >> Fine_"+hist_name_full, hist_name.c_str() ), cut_total , "");
			double mean = h_finelybinned->GetMean(); 	
			shift += mean;

			cout<<"  - pre-iteration mean = "<<mean<<endl;
		}

		if( use_weight ){
			if( debug ) cout<<Form( "%s - %f*(weight/weight) >> "+hist_name_full, hist_name.c_str(), shift )<<endl;
			trees[filetag_treename]->Draw( Form( "%s - %f*(weight/weight) >> "+hist_name_full, hist_name.c_str(), shift ), cut_total , "");
		} else {
			if( debug ) cout<<Form( "%s >> "+hist_name_full, hist_name.c_str() )<<endl;
			trees[filetag_treename]->Draw( Form( "%s >> "+hist_name_full, hist_name.c_str() ), cut_total , "");			
		}
		

		TH1F *h = (TH1F*)h_temp->Clone(); 

		if( shift_center && !run_reweight ) 
			cout<<"  - post-shift mean = "<<h->GetMean()<<endl;

		cout<<"   NEvents = "<<h->GetEntries()<<endl;

		if( filetag_treename.find("data") != string::npos && scale_data != 1 ){
			cout<<"Scaling data by "<<scale_data<<endl;
			h->Scale(scale_data);
		}

		return h;
	}

	// -------------------------------------------------------------------------------------
	TH2F* GetHist2D( PlotParams myPlotParams_x, PlotParams myPlotParams_y, string filetag_treename, TCut cut_compare, string hist_tag_prepend = "", double manual_shift = 0.0 ){
		if( debug) cout<<"MiniTuplePlotter::GetHist2D()"<<endl;		

		string hist_name_x = myPlotParams_x.hist_name;
		string label_x 	   = myPlotParams_x.label_x;
		double xmin 	   = myPlotParams_x.xmin;
		double xmax 	   = myPlotParams_x.xmax;

		string hist_name_y = myPlotParams_y.hist_name;
		string label_y 	   = myPlotParams_y.label_x;
		double ymin 	   = myPlotParams_y.xmin;
		double ymax 	   = myPlotParams_y.xmax;

		Int_t NBinsX = NBins, NBinsY = NBins;
		if( shift_center && manual_shift == 0.0 ){
			NBinsX *= 1000;
			xmin *= 3; 
			xmax *= 3;
		}

		TCanvas *c_temp 	 = new TCanvas();
				
		TString hist_name_full = FormatMyString( Form("%s__%s__%s__"+GetBetterCutTitle( cut_compare ), filetag_treename.c_str(), hist_name_x.c_str(), hist_name_y.c_str() ) );
		string hist_title_axes = Form("; %s; %s", label_x.c_str(), label_y.c_str() );

		TH2F *h2_temp;
		// MaxEcellRW
		if( hist_tag_prepend.find("RW_") != string::npos && hist_name_y.find("maxEcell_E") != string::npos ){
			Int_t NBins_temp = 13;
			Double_t Bins_temp[14] = {10, 15, 20, 25, 30, 35, 40, 50, 60, 70, 80, 100, 150, 500};
			h2_temp = new TH2F( hist_name_full , Form("%s", hist_title_axes.c_str()), NBinsX, xmin, xmax, NBins_temp, Bins_temp); 					
		} else if( hist_tag_prepend.find("RW2D_") != string::npos && hist_name_x.find("maxEcell_E") != string::npos && hist_name_y.find("maxEcell_E") != string::npos ){
			Int_t NBins_temp = 6;
			Double_t Bins_temp[7] = {10, 15, 20, 30, 50, 80, 500};
			h2_temp = new TH2F( hist_name_full , Form("%s", hist_title_axes.c_str()), NBins_temp, Bins_temp, NBins_temp, Bins_temp);
		} else {
			h2_temp = new TH2F( hist_name_full , Form("%s", hist_title_axes.c_str()), NBinsX, xmin, xmax, NBinsY, ymin, ymax); 					
		}

		TCut cut_weight  = Form( "weight" ); 
		TCut cut_total   = cut_weight * (cuts_all && cut_compare && selective_cuts[filetag_treename]);

		if( hist_tag_prepend.find("RW") != string::npos && hist_tag_prepend.find("Target") != string::npos )
			cut_total = cut_weight * (cut_compare && selective_cuts[filetag_treename]);

		trees[filetag_treename]->Draw( Form( "%s:(%s - %f*(weight/weight)) >> "+hist_name_full,  hist_name_y.c_str(), hist_name_x.c_str(), manual_shift ), cut_total , "");
		TH2F *h2 = (TH2F*)h2_temp->Clone(); 

		return h2;
	}  

	// -------------------------------------------------------------------------------------
	TH3F* GetHist3D( PlotParams myPlotParams_x, PlotParams myPlotParams_y, PlotParams myPlotParams_z, string filetag_treename, TCut cut_compare, string hist_tag_prepend = ""){
		if( debug) cout<<"MiniTuplePlotter::GetHist2D()"<<endl;		

		string hist_name_x = myPlotParams_x.hist_name;
		string label_x 	   = myPlotParams_x.label_x;
		double xmin 	   = myPlotParams_x.xmin;
		double xmax 	   = myPlotParams_x.xmax;

		string hist_name_y = myPlotParams_y.hist_name;
		string label_y 	   = myPlotParams_y.label_x;
		double ymin 	   = myPlotParams_y.xmin;
		double ymax 	   = myPlotParams_y.xmax;

		string hist_name_z = myPlotParams_z.hist_name;
		string label_z 	   = myPlotParams_z.label_x;
		double zmin 	   = myPlotParams_z.xmin;
		double zmax 	   = myPlotParams_z.xmax;

		TCanvas *c_temp 	 = new TCanvas();
				
		TString hist_name_full = FormatMyString( Form("%s__%s__%s__%s__"+GetBetterCutTitle( cut_compare ), filetag_treename.c_str(), hist_name_x.c_str(), hist_name_y.c_str(), hist_name_z.c_str() ) );
		string hist_title_axes = Form("; %s; %s; %s", label_x.c_str(), label_y.c_str(), label_z.c_str() );
		TH3F *h3_temp;

		if( hist_tag_prepend.find("RW2D_") != string::npos && hist_name_y.find("maxEcell_E") != string::npos && hist_name_z.find("maxEcell_E") != string::npos ){
			Int_t NBins_temp = 6;
			Double_t Bins_temp[7] = {10, 15, 20, 30, 50, 80, 500};

			Int_t NBins_temp_X = NBins;
			Double_t Bins_temp_X[NBins_temp_X+1];
			for( int i=0; i<NBins_temp_X+1; i++ ) Bins_temp_X[i] = xmin + double(i*(xmax-xmin)/NBins);

			h3_temp = new TH3F( hist_name_full , Form("%s", hist_title_axes.c_str()), NBins_temp_X, Bins_temp_X, NBins_temp, Bins_temp, NBins_temp, Bins_temp);			
			//cout<<"HERE!!!!!!! 3d"<<endl;	
		} else {
			h3_temp = new TH3F( hist_name_full , Form("%s", hist_title_axes.c_str()), NBins, xmin, xmax, NBins, ymin, ymax, NBins, zmin, zmax); 				
		}	

		TCut cut_weight  = Form( "weight" ); 
		TCut cut_total   = cut_weight * (cuts_all && cut_compare && selective_cuts[filetag_treename]);

		if( hist_tag_prepend.find("RW") != string::npos && hist_tag_prepend.find("Target") != string::npos )
			cut_total = cut_weight * (cut_compare && selective_cuts[filetag_treename]);

		trees[filetag_treename]->Draw( Form( "%s:%s:%s >> "+hist_name_full, hist_name_z.c_str(), hist_name_y.c_str(), hist_name_x.c_str() ), cut_total , "");
		TH3F *h3 = (TH3F*)h3_temp->Clone(); 
		
		return h3;
	}		

	// -------------------------------------------------------------------------------------
	map<string,TH1F*> GetHists( PlotParams myPlotParams ){
		if( debug ) cout<<"MiniTuplePlotter::GetHists()"<<endl;

		map<string,TH1F*> hists; 
		hist_tags.clear(); 

		int i = 0;
		for( auto filetag_treename: filetags_treenames ){
			for( auto cut_compare: cuts_compare ){

				TH1F* h;

				if( run_reweight2D ){
					h = (TH1F*)GetHist1DReweighted2D( myPlotParams, filetag_treename, cut_compare);
				} else if( run_reweight ){
					h = (TH1F*)GetHist1DReweighted( myPlotParams, filetag_treename, cut_compare);
				} else {
					h = (TH1F*)GetHist1D( myPlotParams, filetag_treename, cut_compare);
				}

				string hist_tag = Form( "%s "+GetBetterCutTitle( selective_cuts[filetag_treename] )+" "+GetBetterCutTitle( cut_compare ), filetag_treename.c_str() );
				hist_tags.push_back( hist_tag );
				hists[hist_tag] = h;
				i++;
			}
		}
		return hists; 
	}

	// -------------------------------------------------------------------------------------
	THStack* GetStackHist( map<string,TH1F*> hists, PlotParams myPlotParams ){ 
		if( debug) cout<<"MiniTuplePlotter::GetStackHist()"<<endl;		

		string label_y = "Events / Bin";
		if( plot_norm ) label_y = "A.U.";
		if( plot_cdf  ) label_y = "CDF";
		if( plot_reverse_cdf ) label_y = "Reverse CDF";

		THStack* hs = new THStack(Form( "hs_%s", myPlotParams.hist_name.c_str() ), Form("%s; %s; %s", myPlotParams.title.c_str(), myPlotParams.label_x.c_str(), label_y.c_str() ));

		int i = -1;
		for( auto hist_tag: hist_tags ){
			i++;

			TH1F *h = (TH1F*)hists[hist_tag]->Clone();

			string fit_info_str = "";
			if( run_fit ){
				fit_info_str = GetFitInfo( h );
			}	

			if( plot_norm )
				h->Scale( 1./h->Integral() );

			string legend_name = hist_tag;
			if( stamp_counts ){
				//legend_name = Form("%s (Mean=%.2f)", hist_tag.c_str(), h->GetMean());
				legend_name = Form("%s (NE=%.f,Int=%.2f)", hist_tag.c_str(), h->GetEntries(), h->Integral() );
				cout<<legend_name<<endl;
			}

			if( plot_cdf )
				h = GetCDF( h ); 
			else if( plot_reverse_cdf )
				h = GetReverseCDF( h ); 

			if( plot_log ) h->SetMaximum( h->GetMaximum()*20. );
			else  		   h->SetMaximum( h->GetMaximum()*1.25 );

			h->SetLineColor( colors[i] );
			h->SetLineStyle( linestyle[i] );
			h->SetLineWidth(4);
			h->SetFillColor(0);

			if( run_fit ) 
				h->SetName( Form("#splitline{%s}{%s}", legend_name.c_str(), fit_info_str.c_str() ) );	
			else 
				h->SetName( Form("%s", legend_name.c_str()) );	

			hs->Add( h );	
		}

		return hs; 
	}	

	// -------------------------------------------------------------------------------------
	THStack* GetStackRatio( map<string,TH1F*> hists, PlotParams myPlotParams, string filetag_treename_divisor = "" ){
		if( debug) cout<<"MiniTuplePlotter::GetStackRatio()"<<endl;		

		if( filetag_treename_divisor == "" ) 
			filetag_treename_divisor = hist_tags[0];

		string label_y = Form( "Hist / %s", filetag_treename_divisor.c_str() );
		if( plot_norm ) label_y += " (norm)";

		THStack* hs = new THStack(Form( "hs_%s", myPlotParams.hist_name.c_str() ), Form(" ; %s; %s", myPlotParams.label_x.c_str(), label_y.c_str() ));

		TH1F *h_divisor = (TH1F*)hists[filetag_treename_divisor]->Clone();
		if( plot_norm )
			h_divisor->Scale( 1./h_divisor->Integral() );

		if( plot_cdf )
			h_divisor = GetCDF( h_divisor ); 
		else if( plot_reverse_cdf )
			h_divisor = GetReverseCDF( h_divisor ); 	

		int i = -1;
		for( auto hist_tag: hist_tags ){
			i++;

			TH1F *h = (TH1F*)hists[hist_tag]->Clone();

			if( plot_norm )
				h->Scale( 1./h->Integral() );

			if( plot_cdf )
				h = GetCDF( h ); 
			else if( plot_reverse_cdf )
				h = GetReverseCDF( h ); 	

			h->SetMaximum( 2. );
			if( plot_log_ratio ) h->SetMaximum( 10. );


			h->Divide( h_divisor );
			h->SetLineColor( colors[i] );
			h->SetLineStyle( linestyle[i] );
			h->SetLineWidth(4);
			h->SetFillColor(0);
			h->SetName( Form("%s", hist_tag.c_str() ) );
			hs->Add( h );
		}

		return hs; 
	}

	// -------------------------------------------------------------------------------------
	THStack* GetStackSignificance( map<string,TH1F*> hists, PlotParams myPlotParams, string filetag_treename_divisor = "" ){
		if( debug) cout<<"MiniTuplePlotter::GetStackSignificance()"<<endl;		

		string label_y;
		if( significance_formula == "ssqrtb" ) 
			label_y = Form("Signal CDF / sqrt(data CDF)");
		else if( significance_formula == "Z" )
			label_y = Form("Significance Z");
		else if( significance_formula == "Zprime" )
			label_y = Form("Significance Z'");		
		else if( significance_formula == "ssqrtb_sigeff" )
			label_y = Form("Eff_{S} * #frac{S}{#sqrt(data)}");
		else{
			cout<<"ERROR: Invalid significance_formula."<<endl;
			THStack* hs_empty;
			return hs_empty;
		}

		THStack* hs = new THStack(Form( "hs_%s", myPlotParams.hist_name.c_str() ), Form(" ; %s; %s", myPlotParams.label_x.c_str(), label_y.c_str() ));

		if( filetag_treename_divisor == "" ) filetag_treename_divisor = hist_tags[0];

		TH1F *h_b_temp 	  = (TH1F*)hists[filetag_treename_divisor]->Clone();
		TH1F *h_cdf_b 	  = (TH1F*) GetCDF( h_b_temp );
		TH1F *h_cdf_sqrtb = (TH1F*) GetSqrtTH1( h_cdf_b );

		int i_f = -1;
		for( auto hist_tag: hist_tags ){
			i_f++;

			if( hist_tag == filetag_treename_divisor ) continue;

			TH1F *h_s_temp 	  = (TH1F*) hists[hist_tag]->Clone();
			TH1F *h_cdf_s 	  = (TH1F*) GetCDF( h_s_temp );
			TH1F *h_cdf_sqrts = (TH1F*) GetSqrtTH1( h_cdf_s );
			TH1F *h_cdf_seff  = (TH1F*) h_cdf_s->Clone();
			h_cdf_seff->Scale( 1./h_cdf_seff->GetBinContent(1) );

			// S/Sqrt(b) (default)
			TH1F *h_ssqrtb = (TH1F*)h_cdf_s->Clone(); 
			h_ssqrtb->Divide( h_cdf_sqrtb );			

			TH1F* h;
			if( significance_formula == "ssqrtb"){
				h = (TH1F*) h_ssqrtb->Clone();
			} else if( significance_formula == "Z"){ // Significance
				TH1F *h_Z = (TH1F*) GetSignificanceZ( h_cdf_s, h_cdf_b);
				h = (TH1F*) h_Z->Clone();
			} else if( significance_formula == "Zprime"){ // Devin's Formula
				TH1F *h_Zprime = (TH1F*) DevinsFormulaZprime( h_cdf_s, h_cdf_b);
				h = (TH1F*) h_Zprime->Clone();				
			} else if ( significance_formula == "ssqrtb_sigeff" ){ // sig_eff * S/Sqrt(b)
				TH1F *h_sigeff_ssqrtb = (TH1F*)h_ssqrtb->Clone(); 
				h_sigeff_ssqrtb->Multiply( h_cdf_seff );				
				h = (TH1F*) h_sigeff_ssqrtb->Clone();
			}
			
			h->SetLineColor( colors[i_f] );
			h->SetLineStyle( linestyle[i_f] );
			h->SetFillColor(0);
			h->SetLineWidth(4);

			double max_x = h->GetXaxis()->GetBinCenter( h->GetMaximumBin() );
			double max_y = h->GetMaximum(); 
			h->SetName( Form("%.3f , %.1f ", max_y, max_x ) );

			if( plot_log ) h->SetMaximum( h->GetMaximum()*10. );
			else  		   h->SetMaximum( h->GetMaximum()*1.25 );	

			hs->Add( h );
		}

		return hs; 

	}

	// -------------------------------------------------------------------------------------
	void Plot( string plot_type = "", string filetag_treename_divisor = "" ){
		if( debug) cout<<"MiniTuplePlotter::Plot()"<<endl;		

		GetTrees();

		for( auto PlotParams_temp: PlotParamsList ){

			map<string,TH1F*> hists = GetHists( PlotParams_temp );

			TCanvas *myCanvas;

			if( plot_type == "" ) 
				myCanvas = new TCanvas("c", "c", 1600, 1200);
			else {
				myCanvas = new TCanvas("c", "c", 1200, 1600);	
				myCanvas->Divide(0,2);
			}

			THStack* hs = GetStackHist( hists, PlotParams_temp );
			myCanvas->cd(1);
			hs->Draw("nostack"); 

			if( manual_legend )
				gPad->BuildLegend(legx1,legx2,legy1,legy2,"");
			else
				gPad->BuildLegend(0.55,0.65,0.9,0.9,"");
			

			StampCMS( "Internal", 140., 0.14, 0.84, 0.045 );
			StampCuts( 0.1, 0.91, 0.015 );			

			if( plot_type == "ratio" ){
				myCanvas->cd(2);
				if( plot_log_ratio ) gPad->SetLogy(); 	
				if( plot_log_x ) gPad->SetLogx(); 
				THStack* hs_ratio = GetStackRatio( hists, PlotParams_temp, filetag_treename_divisor );	
				hs_ratio->Draw("nostack");
				myCanvas->cd(1);
			}
			else if( plot_type == "sig" || plot_type == "ssqrtb" ){
				myCanvas->cd(2);
				if( plot_log ) gPad->SetLogy(); 	
				if( plot_log_x ) gPad->SetLogx(); 					
				THStack* hs_ssqrtb = GetStackSignificance( hists, PlotParams_temp, filetag_treename_divisor );
				hs_ssqrtb->Draw("nostack");
				gPad->BuildLegend(0.6,0.65,0.9,0.9,"Max Significance, Cut Value:");
				myCanvas->cd(1);
			}

			myCanvas->cd(1);
			if( plot_log )
				gPad->SetLogy(); 

			if( plot_log_x )
				gPad->SetLogx(); 

			if( plot_grid ){
				myCanvas->SetGrid();
				if( plot_type != "" ){
					myCanvas->cd(1); gPad->SetGrid();
					myCanvas->cd(2); gPad->SetGrid();
				}
			}

			TString output_file_name = GetOutputFileName(PlotParams_temp, plot_type);
			//fout->cd();
			//myCanvas->Write();
			myCanvas->SaveAs( "Plots/"+output_file_name+".png", "png" );
			delete myCanvas;

		}

	}

	// -------------------------------------------------------------------------------------
	void Plot2D( PlotParams myPlotParams_x, PlotParams myPlotParams_y ){
		if( debug) cout<<"MiniTuplePlotter::Plot2D()"<<endl;		

		GetTrees();
		SetStyle();

		for( auto filetag_treename: filetags_treenames ){
			for( auto cut_compare: cuts_compare ){

				TH2F* h2 = (TH2F*)GetHist2D( myPlotParams_x, myPlotParams_y, filetag_treename, cut_compare );

				TCanvas *myCanvas = new TCanvas("c", "c", 1300, 1200);
				myCanvas->SetRightMargin(0.14);
				myCanvas->SetLeftMargin(0.14);
				myCanvas->SetTopMargin(0.12);
				myCanvas->SetBottomMargin(0.12);

				if( plot_log ) 
					gPad->SetLogz();

				if( plot_norm )
					h2->Scale(1./h2->Integral());

				if( stamp_counts )
					h2->Draw("colz TEXT");
				else
					h2->Draw("colz");

				//StampCMS( "Internal", 140., 0.14, 0.84, 0.045 );
				StampCuts( 0.12, 0.91, 0.02);

				TF1* fitline = new TF1("fitline", "[0]*x+[1]");
				TLatex fittext;
				float m, b, corr;
				if( run_fit2D ){

					//h2->Fit( fitline );
					m = fitline->GetParameter(0);
					b = fitline->GetParameter(1);
					cout<<"slope = "<<m<<endl;
					cout<<"y-int = "<<b<<endl;

					corr = h2->GetCorrelationFactor();
					cout<<"correlation factor = "<<corr<<endl;
				
					//fitline->Draw("SAME"); 
					double stamp_x = 0, stamp_y = 0;
					if( myPlotParams_x.xmin < 0) stamp_x = myPlotParams_x.xmin*0.9;
					else 						 stamp_x = myPlotParams_x.xmin*1.1;
					stamp_y = myPlotParams_y.xmax*0.9;

					fittext.SetTextSize(.03);
					//fittext.DrawLatex(-4,4, Form("#splitline{Fit: y = %.4f * x + %.4f}{Correlation Factor = %.4f}", m, b, corr ));
					fittext.DrawLatex(stamp_x, stamp_y, Form("Correlation Factor = %.4f", corr ));
					fittext.Draw("SAME");
				}

				string output_file_name = myPlotParams_y.hist_name + "_vs_" + myPlotParams_x.hist_name; // GetOutputFileName(myPlotParams_x);
				myCanvas->SaveAs( Form( "Plots/Plot2D_%s_%s_"+GetBetterCutTitle(cut_compare)+"_%s.png", filetag_treename.c_str(), output_file_name.c_str(), output_file_tag.c_str() ) );

			}
		}

	}

	// -------------------------------------------------------------------------------------
	double GetSignalFiletagMass(string filetag){ 
		return stod( filetag.substr(filetag.find("_")+1,-1).substr(0,filetag.find("_")-1));
	}

	// -------------------------------------------------------------------------------------
	string GetSignalFiletagLifetime(string filetag){
		return filetag.substr(filetag.find("_")+1,-1).substr(0,filetag.find("_")-1);
	}

	// -------------------------------------------------------------------------------------
	void PlotStaticHistogram( string hist_name ){

		SetStyle();

		TCanvas* canv = new TCanvas("c", "c", 1200, 800);
		TLegend* leg  = new TLegend(.18,.18,.45,.45);
		THStack* hs; 

		float ymin = 1e-5;
		float ymax = 5;

		for( int i=0; i<filetags.size(); i++){

			string filetag  = filetags[i];
			TString filename = Form("%s%s%s", infile_path.c_str(), filetag.c_str(), infile_ext.c_str() );
			TFile *file = new TFile( filename, "READ");

			TH1F* h_temp = (TH1F*)file->Get( Form("%s", hist_name.c_str() ) );

			TH1F *h = (TH1F*) h_temp->Clone();

			if( plot_norm ){
				h->Scale(1./h->GetBinContent(1));
				h->SetAxisRange(1e-3,5,"Y");
			} else {
				if( h->GetMaximum() > ymax ) ymax = h->GetMaximum();
				if( h->GetMaximum() < ymin && h->GetMaximum() != 0 ) ymin = h->GetMaximum();			
				h->SetAxisRange(ymin,ymax*10,"Y");
			}
			
			h->SetLineColor( colors[i] );
			h->SetLineStyle( linestyle[i] );
			
			h->Draw("same hist");
			leg->AddEntry(h, Form("%s", filetag.c_str() ) );

			//if( i == 0 )
			//	hs = new THStack(Form( "hs_%s", hist_name.c_str() ), Form( h->GetTitle()+";"+h->GetXaxis()->GetTitle()+";"+h->GetYaxis()->GetTitle() ) );

			//hs->Add( h );

		}

		if( plot_grid )
			canv->SetGrid();

		canv->SetLogy();

		StampCMS( "Internal", 140., 0.14, 0.84, 0.045 );
		leg->Draw();

		/*
		if( manual_legend )
			gPad->BuildLegend(legx1,legx2,legy1,legy2,"");
		else
			gPad->BuildLegend(0.55,0.65,0.9,0.9,"");	
		*/
		canv->SaveAs( Form( "Plots/Plot_%s_%s.png", hist_name.c_str(), output_file_tag.c_str() ) );
		delete canv;

	}

	// -------------------------------------------------------------------------------------
	void PlotExtendedCutflow( string hist_name, vector<TCut> cuts = {}, vector<float> yrange = {}, bool fraction_passed = false){

		SetStyle();
		GetTrees();

		TCanvas* canv = new TCanvas("c", "c", 1200, 800);
		TLegend* leg = new TLegend(.7,.55,.9,.9);
		THStack* hs; 

		float ymin, ymax;
		if( yrange.size() < 2 ){
			ymin = 1e-5;
			ymax = 5;
		}

		Int_t NCuts = cuts.size();

		// Temporarily disable cuts_all and NBins (re-enabled later)
		TCut cuts_all_temp = cuts_all; 	cuts_all = "";
		Int_t NBins_temp = NBins; 		NBins = 1;
		
		PlotParams myDummyPlotParam = {"weight", "Dummy", "Dummy", 0., 100. };

		for( int i=0; i<filetags.size(); i++){

			TString filename = Form("%s%s%s", infile_path.c_str(), filetags[i].c_str(), infile_ext.c_str() );
			TFile *file = new TFile( filename, "READ");

			TH1F* h_temp = (TH1F*)file->Get( Form("%s", hist_name.c_str() ) );
			Int_t NBinsCutflow = h_temp->GetNbinsX();

			// Define NTuple-to-MiniTuple Cutflow Histogram
			string filetag_treename = GetFiletagTreename( filetags[i], treenames[i] );
			TString hname  = Form( "%s", filetag_treename.c_str() );
			TString htitle = Form( "%s;%s;%s", h_temp->GetTitle(), h_temp->GetXaxis()->GetTitle(), h_temp->GetXaxis()->GetTitle() );
			TH1F* h_cutflow_temp = new TH1F( hname, htitle, NBinsCutflow+NCuts, 0, NBinsCutflow+NCuts );
			TH1F* h = (TH1F*) h_cutflow_temp->Clone(); 

			// Fill New Cutflow
			TCut cumulative_cut = "";
			vector<Double_t> hist_values;
			for( Int_t j=1; j<=NBinsCutflow+NCuts; j++ ){
				if( j <= NBinsCutflow ){ // Previous
					//cout<<j<<" -- 1"<<endl;
					h->GetXaxis()->SetBinLabel(j, h_temp->GetXaxis()->GetBinLabel(j) );
					h->SetBinContent(j, h_temp->GetBinContent(j) ); 
					h->SetBinError(j, h_temp->GetBinError(j) ); 
				} else{ // New cuts !
					//cout<<j<<" -- 2"<<endl;
					cumulative_cut += cuts[j-NBinsCutflow-1];
					TH1F* h_bin_temp = (TH1F*)GetHist1D( myDummyPlotParam, filetag_treename, cumulative_cut);
					TH1F* h_bin = (TH1F*) h_bin_temp->Clone(); 					
					Double_t NEvents_temp_err = 0.0;
					Double_t NEvents_temp = h_bin->IntegralAndError(0,2,NEvents_temp_err);

					h->GetXaxis()->SetBinLabel(j, GetBetterCutTitle( cuts[j-NBinsCutflow-1]) );
					h->SetBinContent(j, NEvents_temp );
					h->SetBinError(j, NEvents_temp_err );
				}
				hist_values.push_back( h->GetBinContent(j) );
			}
			
			if( fraction_passed ) {
				h->SetBinContent(1, 1. );
				h->SetBinError( 1, 0.0 );	
				for( Int_t j=2; j<=NBinsCutflow+NCuts; j++ ){
					Double_t myvalue =hist_values[j-1]/hist_values[j-2];
					//cout<<myvalue<<endl;
					h->SetBinContent(j, myvalue );
					h->SetBinError( j, 0.0 );
				}

			}

			if( plot_norm )
				h->Scale(1./h->GetBinContent(1));

			//	h->SetAxisRange(1e-5,2,"Y");
			//} else {
			if( yrange.size() < 2  ){
				if( h->GetMaximum() > ymax ) ymax = h->GetMaximum();
				if( h->GetMaximum() < ymin && h->GetMaximum() != 0 ) ymin = h->GetMaximum();			
				if( plot_log ) h->SetAxisRange(ymin+1e-6,ymax*10,"Y");
				else           h->SetAxisRange(ymin,ymax*1.4,"Y");
			}
			//}

			h->SetLineColor( colors[i] );
			h->SetLineStyle( linestyle[i] );
			h->SetFillColor( 0 );
			h->SetLineWidth( 3 );

			canv->cd();
			h->Draw("same hist");
			leg->AddEntry(h, Form("%s", filetags[i].c_str() ) );

		}

		if( plot_grid )
			canv->SetGrid();

		if( plot_log )
			canv->SetLogy();
		
		StampCMS( "Internal", 140., 0.14, 0.84, 0.045 );
		leg->Draw();
		
		if( fraction_passed )
			canv->SaveAs( Form( "Plots/PlotExtendedCutflowByFractionPass_%s_%s.png", hist_name.c_str(), output_file_tag.c_str() ) );
		else
			canv->SaveAs( Form( "Plots/PlotExtendedCutflow_%s_%s.png", hist_name.c_str(), output_file_tag.c_str() ) );

			
		delete canv;

		cuts_all = cuts_all_temp;
		NBins = NBins_temp;

	}	

	// -------------------------------------------------------------------------------------
	void GetValues(string hist_name, bool print = true){ // TODO
		if( debug) cout<<"MiniTuplePlotter::PrintValues()"<<endl;		

		map<string,vector<double>> values;

		if( hist_name == "Selection" ){	
			values["x"] = {};
			values["MediumIDEff"] = {};
			values["LooseIDEff"] = {};
			values["BothIDEff"] = {};
			values["PassZeeSel_OrthogMed"] = {};
			values["PassZeeSel_OrthogLoose"] = {};
		}

		for( auto filetag: filetags ){

			TString filename = Form("%s%s%s", infile_path.c_str(), filetag.c_str(), infile_ext.c_str() );
			TFile* file;

			if( !gSystem->AccessPathName(filename) ){
				//cout<<"Reading in "<<filename<<endl;
				file = new TFile( filename, "READ");
			} else{
				cout<<"ERROR: File "<<filename<<" does not exist"<<endl;
			}

			TH1F* h = (TH1F*)file->Get( Form("%s", hist_name.c_str() ) );

			if( hist_name == "Selection" ){

				float NTotal = h->GetBinContent(1);
				if( print ){
					cout<<filetag<<"\t";
					cout<<h->GetBinContent(2)/NTotal<<"\t";
					cout<<h->GetBinContent(3)/NTotal<<"\t";
					cout<<h->GetBinContent(4)/NTotal<<"\t";
					cout<<endl;
				}

				values["x"].push_back( GetSignalFiletagMass(filetag)  );
				values["MediumIDEff"].push_back( h->GetBinContent(2)/NTotal );
				values["LooseIDEff"].push_back( h->GetBinContent(3)/NTotal );
				values["BothIDEff"].push_back( h->GetBinContent(4)/NTotal );
				values["PassZeeSel_OrthogMed"].push_back( h->GetBinContent(6)/NTotal );
				values["PassZeeSel_OrthogLoose"].push_back( h->GetBinContent(7)/NTotal );
			}			

		}

		cout<<"x_vals = [";
		for( int i = 0; i<filetags.size(); i++)
			cout<<values["x"][i]<<",";
		cout<<"]"<<endl;


		cout<<"y_vals['BothID'] = [";
		for( int i = 0; i<filetags.size(); i++)
			cout<<values["BothIDEff"][i]<<",";
		cout<<"]"<<endl;	

		cout<<"y_vals['MediumID'] = [";
		for( int i = 0; i<filetags.size(); i++)
			cout<<values["MediumIDEff"][i]<<",";
		cout<<"]"<<endl;

		cout<<"y_vals['LooseID'] = [";
		for( int i = 0; i<filetags.size(); i++)
			cout<<values["LooseIDEff"][i]<<",";
		cout<<"]"<<endl;		
		
		cout<<"y_vals['PassZeeSel_OrthogMed'] = [";
		for( int i = 0; i<filetags.size(); i++)
			cout<<values["PassZeeSel_OrthogMed"][i]<<",";
		cout<<"]"<<endl;	

		cout<<"y_vals['PassZeeSel_OrthogLoose'] = [";
		for( int i = 0; i<filetags.size(); i++)
			cout<<values["PassZeeSel_OrthogLoose"][i]<<",";
		cout<<"]"<<endl;			

	}

};
