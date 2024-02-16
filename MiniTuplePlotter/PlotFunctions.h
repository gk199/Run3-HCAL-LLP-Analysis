#include <vector>
#include <string>
//#include <map>

#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TPad.h"
#include "THStack.h"
#include "TLegend.h"
#include "TString.h"
#include <iostream>
#include <iomanip>

#include <algorithm>

#include "TGraphAsymmErrors.h"
#include <cmath>

#include "TLatex.h"

// ==========================================================================================
// Plot Appearance 
// ===========================================================================================

// -------------------------------------------------------------------------------------
void SetStyle(){
	gStyle->SetOptStat(0);
	gStyle->SetPaintTextFormat(".2f");
	//gStyle->SetLegendBorderSize(2);
	//gStyle->SetPaintTextSize(1);

	//gStyle->SetGridStyle(3);
	//gStyle->SetGridWidth(1);
	gStyle->SetGridColor(kGray+1);

	gStyle->SetCanvasColor(-1);
	gStyle->SetPadColor(-1);
	gStyle->SetFrameFillColor(-1);
	gStyle->SetHistFillColor(-1);
	gStyle->SetTitleFillColor(-1);
	gStyle->SetFillColor(-1);
	gStyle->SetFillStyle(4000);
	gStyle->SetStatStyle(0);
	gStyle->SetTitleStyle(0);
	gStyle->SetCanvasBorderSize(0);
	gStyle->SetFrameBorderSize(0);
	gStyle->SetLegendBorderSize(0);
	gStyle->SetStatBorderSize(0);
	gStyle->SetTitleBorderSize(0);
}

// -------------------------------------------------------------------------------------
void StampCMS( const std::string & approvaltext = "Internal", float lumi=140., float x=0.14, float y=0.84, float textsize=0.045) {
  // Usage:  StampCMS("Internal", 126.5, 0.12, 0.75);
  
  textsize = 0.045;
  TLatex label_cms;
  label_cms.SetNDC();
  //label_cms.SetTextFont(72);
  label_cms.SetTextColor(1);
  label_cms.SetTextSize(textsize);
  label_cms.DrawLatex( x, y, "CMS" );

  TLatex label_approvaltext;
  label_approvaltext.SetNDC();
  label_approvaltext.SetTextFont(52);
  label_approvaltext.SetTextColor(1);
  label_approvaltext.SetTextSize(textsize*0.85);
  label_approvaltext.DrawLatex( x+0.075, y, approvaltext.c_str() ); 

  TLatex label_energylumi; 
  label_energylumi.SetNDC();
  label_energylumi.SetTextFont(42);
  label_energylumi.SetTextColor(1);
  label_energylumi.SetTextSize(textsize*0.65 );
  label_energylumi.DrawLatex( x, y-.035, "#sqrt{s} = 13.6 TeV" ); // Just energy for now...
  //label_energylumi.DrawLatex( x, y-.08, Form("L = %.1f fb^{-1}", lumi) );
  //label_energylumi.DrawLatex( x+.01, y-.2, "SF = #frac{N_{SR}}{N_{CR}} = 0.88" );

}

// ==========================================================================================
// Fits 
// ===========================================================================================

// -------------------------------------------------------------------------------------------
TF1* IterativeGaussianFit( TH1F* h, float nSig = 1.5, TString fit_settings = "Q0" ) {
	// Iterative Gaussian Fit. Originally from Devin
 
	float mean = h->GetMean(), sigma = h->GetRMS();
	float dm = 999, ds = 999, epsilon = 0.0000001;
	unsigned int maxIter = 100, iter = 0;

	TF1* fit_g = new TF1("fit_g","gaus");
	h->Fit(fit_g,"Q0","",mean - nSig * sigma,mean + nSig * sigma);
	mean = fit_g->GetParameter(1);
	sigma = fit_g->GetParameter(2);

	//TFitResultPtr ;
	while ((dm > epsilon) || (ds > epsilon)) {

		h->Fit(fit_g, fit_settings,"",mean - nSig * sigma,mean + nSig * sigma);

		float prevMean = mean;
		float prevSigma = sigma;

		mean = fit_g->GetParameter(1);
		sigma = fit_g->GetParameter(2);
		//cout<<mean<<"  "<<sigma<<endl; 

		dm = fabs((mean - prevMean) / prevMean);
		ds = fabs((sigma - prevSigma) / prevSigma);

		iter++;
		if (iter > maxIter) break;
  }
  return fit_g;
}

// -------------------------------------------------------------------------------------
TF1* LorentzCauchyFit( TH1F* h, TString fit_settings = "Q0" ) {
  // Lorentz-Cauchy Fit

	TF1* fit_g = new TF1("fit_g","gaus");
	h->Fit(fit_g, "Q0");
	float scale = fit_g->GetParameter(0);
	float mean  = fit_g->GetParameter(1);
	float sigma = fit_g->GetParameter(2);  

	TF1* fit_lc = new TF1("fit_lc","[0]/(((x - [1])/[2])**2. + 1.)");
	fit_lc->SetParameters(scale, mean, sigma);
	h->Fit(fit_lc, fit_settings);
	return fit_lc;

}

// ==========================================================================================
// Cumulative Distribution Functions 
// ===========================================================================================

// -------------------------------------------------------------------------------------
TH1F* GetCDF( TH1F* h ){
	// Cumulative Distribution Function. Returns the fraction of events to the right of
	// the x-axis value
	// TODO: @Elena -- you found an indexing bug here?

	TH1F* h_cdf = (TH1F*)h->Clone();
	h_cdf->Reset();
	double integral = h->Integral();
	int NBins = h->GetNbinsX()+1;
	for( int i=1; i<=NBins; i++ ){
		//integral += h->GetBinContent(i);
		double integral_temp = h->Integral(i, NBins); //integral;
		h_cdf->SetBinContent(i, integral_temp);
	}
	return h_cdf;
}

// -------------------------------------------------------------------------------------
TH1F* GetReverseCDF( TH1F* h ){
	// Reverse Cumulative Distribution Function. Returns the fraction of events to the 
	// left of the x-axis value

	TH1F* h_cdf = (TH1F*)h->Clone();
	h_cdf->Reset();
	double integral = h->Integral();
	int NBins = h->GetNbinsX()+1;
	for( int i=1; i<NBins; i++ ){
		//integral += h->GetBinContent(i);
		double integral_temp = h->Integral(1, i); //integral;
		h_cdf->SetBinContent(i, integral_temp);
	}
	return h_cdf;
}

// ==========================================================================================
// Significance Formulas 
// ===========================================================================================

// -------------------------------------------------------------------------------------
TH1F* GetSignificanceZ( TH1F* h_s, TH1F* h_b ){

	TH1F *h_Z = (TH1F*)h_s->Clone();
	h_Z->Reset();

	for (int bin=1; bin<h_s->GetNbinsX()+1; bin++){
		double s = h_s->GetBinContent(bin);
		double b = h_b->GetBinContent(bin);
		double Z;
		if( s == 0 || b == 0 ) 
			Z = 0;
		else
			Z = sqrt( -2*( b*log(1+s/b) - s ) );

		double error = 0.0; // TODO
		h_Z->SetBinContent(bin, Z);
		h_Z->SetBinError(bin, error);
	}
	return h_Z;
}

// -------------------------------------------------------------------------------------
TH1F* DevinsFormulaZprime( TH1F* h_s, TH1F* h_b ){

	TH1F *h_Zprime = (TH1F*)h_s->Clone();
	h_Zprime->Reset();

	for (int bin=1; bin<h_s->GetNbinsX()+1; bin++){
		double s = h_s->GetBinContent(bin);
		double b = h_b->GetBinContent(bin);
		double Zprime;
		if( s <= 0 || b <= 0 ) 
			Zprime = 0;
		else{
			double Z = sqrt( -2*( b*log(1+s/b) - s ) );
			Zprime   = s/sqrt( pow(s/Z, 2.) + s + b );
		}
		double error = 0.0; // TODO
		h_Zprime->SetBinContent(bin, Zprime);
		h_Zprime->SetBinError(bin, error);
	}
	return h_Zprime;
}

// ==========================================================================================
// Other General Histogram Manipulations
// ===========================================================================================

// -------------------------------------------------------------------------------------
TH1F* GetSqrtTH1( TH1F* h ){

	TH1F *h_sqrt = (TH1F*)h->Clone();
	h_sqrt->Reset(); 
	for (int bin=1; bin<h->GetNbinsX()+1; bin++){
		double bin_content  	= h->GetBinContent(bin);
		double sqrt_bin_content = sqrt( bin_content );
		double error = 0.5*(sqrt_bin_content / bin_content)*h->GetBinError(bin);
		h_sqrt->SetBinContent(bin, sqrt_bin_content);
		h_sqrt->SetBinError(bin, error);
	}
	return h_sqrt;
}

