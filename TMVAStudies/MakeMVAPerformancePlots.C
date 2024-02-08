// root -l /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/MakeMVAPerformancePlots.C+'("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/BDTWeightFilesTest/Test_mh350.root","350",30,"/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/BDTWeightFilesTest/Test_mh125.root","125",38)'
// inputs are output TMVA root file, label, color, repeat for second file to overlay

// root -l /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/MakeMVAPerformancePlots.C+'()'

//================================================================================================
//
//================================================================================================

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                  // access to gROOT, entry point to ROOT system
#include <TSystem.h>                // interface to OS
#include <TFile.h>                  // file handle class
#include <TTree.h>                  // class to access ntuples
#include <TBranch.h>                // class to access branches in TTree
#include <TClonesArray.h>           // ROOT array class
#include <TCanvas.h>                // class for drawing
#include <TH1F.h>                   // 1D histograms
#include <TH2F.h>                   // 2D histograms
#include <TGraphAsymmErrors.h>     
#include <TBenchmark.h>             // class to track macro running statistics
#include <TLorentzVector.h>         // 4-vector class
#include <TVector3.h>               // 3D vector class
#include <vector>                   // STL vector class
#include <iostream>                 // standard I/O
#include <iomanip>                  // functions to format standard I/O
#include <fstream>                  // functions for file I/O
#include <string>                   // C++ string class
#include <sstream>                  // class for parsing strings
#include "TLegend.h"
#include "TEfficiency.h"
#include "THStack.h"

#endif

//------------------------------------------------------------------------------
// getTreeFromFile
//------------------------------------------------------------------------------
TTree* getTreeFromFile(const char* infname, const char* tname)
{
  bool verbose = false;

  if (verbose) {
    cout << "--- Open file " << infname << endl;
  }
  
  TFile* inf = new TFile(infname,"read");
  assert(inf);

  TTree* t = (TTree*)inf->Get(tname);
  
  if (!t) {
    TDirectory *dir = (TDirectory*)inf->FindObjectAny("BDTWeightFilesTest"); // this is the TDirectory that TTree "TestTree" is under
    if (!dir) {
      cout << "Cannot get Directory BDTWeightFilesTest from file " << infname << endl;
      assert(dir);
    }
    t = (TTree*)dir->Get(tname);
  }

  if (!t) {
    cout << "Cannot get Tree with name " << tname << " from file " << infname << endl;
  }
  assert(t);

  if (verbose) {
    cout << "---\tRecovered tree " << t->GetName()
	 << " with "<< t->GetEntries() << " entries" << endl;
  }
  
  return t;
}

//*************************************************************************************************
//
//*************************************************************************************************
// return a TGraphAsymmErrors, sig vs bkg
TGraphAsymmErrors* MakeSigEffVsBkgEffGraph(TH1F* signalHist, TH1F* bkgHist, string name ) {
  //Make Met Plots
  const UInt_t nPoints = signalHist->GetXaxis()->GetNbins();
  double SigEff[nPoints];
  double BkgEff[nPoints];
  double BkgRej[nPoints];
  double BkgEffInverse[nPoints];
  double SigEffErrLow[nPoints];
  double SigEffErrHigh[nPoints];
  double BkgEffErrLow[nPoints];
  double BkgEffErrHigh[nPoints];
  double NSigTotal = 0;
  double NBkgTotal = 0;
  
  for (UInt_t q=0; q < nPoints+2; ++q) {
    NSigTotal += signalHist->GetBinContent(q);
    NBkgTotal += bkgHist->GetBinContent(q);
  }

  for(UInt_t b=0; b < nPoints; ++b) {
    Double_t nsig = 0;
    Double_t nbkg = 0;
    for (UInt_t q=b; q < nPoints+2; ++q) {
      nsig += signalHist->GetBinContent(q);
      nbkg += bkgHist->GetBinContent(q);
    }

    Double_t ratio;
    Double_t n1 = 0;
    Double_t n2 = 0;

    n1 = TMath::Nint(nsig);
    n2 = TMath::Nint(NSigTotal);
    ratio = n1/n2;

    SigEff[b] = ratio;
    SigEffErrLow[b] = 0;
    SigEffErrHigh[b] = 0;

    n1 = TMath::Nint(nbkg);
    n2 = TMath::Nint(NBkgTotal);
    ratio = n1/n2;
    BkgEff[b] = ratio;
    BkgEffErrLow[b] = 0;
    BkgEffErrHigh[b] = 0;
  }

  TGraphAsymmErrors *tmpSigEffVsBkgEff = new TGraphAsymmErrors (nPoints, BkgEff, SigEff, BkgEffErrLow, BkgEffErrHigh, SigEffErrLow, SigEffErrHigh );
  tmpSigEffVsBkgEff->SetName(name.c_str());
  tmpSigEffVsBkgEff->SetTitle("ROC Curve for LLP vs. W+jets");
  tmpSigEffVsBkgEff->GetXaxis()->SetTitle("Bkg Eff");
  tmpSigEffVsBkgEff->GetYaxis()->SetTitle("Signal Eff");
  tmpSigEffVsBkgEff->GetXaxis()->SetRangeUser(0,1);
  tmpSigEffVsBkgEff->GetYaxis()->SetTitleOffset(1.1);
  tmpSigEffVsBkgEff->GetXaxis()->SetTitleOffset(1.05);
  tmpSigEffVsBkgEff->SetMarkerSize(0.5);
  tmpSigEffVsBkgEff->SetMarkerStyle(20);

  return tmpSigEffVsBkgEff;
}

//*************************************************************************************************
//
//*************************************************************************************************
Double_t FindCutValueAtFixedEfficiency(TH1F* signalHist, Double_t targetSignalEff ) {

  Double_t targetCutValue = -9999;
  Double_t bestCurrentSignalEff = 0;
  const UInt_t nPoints = signalHist->GetXaxis()->GetNbins();
  double NSigTotal = 0;

  for (UInt_t q=0; q < nPoints+2; ++q) {
    NSigTotal += signalHist->GetBinContent(q);
  }

  for(UInt_t b=0; b < nPoints; ++b) {
    Double_t nsig = 0;
    for (UInt_t q=b; q < nPoints+2; ++q) {
      nsig += signalHist->GetBinContent(q);
    }

    Double_t ratio = nsig / NSigTotal;
//     cout << targetSignalEff << " : " << ratio << " , " << signalHist->GetXaxis()->GetBinCenter(b) << endl;

    if (fabs(targetSignalEff - ratio) < fabs(targetSignalEff - bestCurrentSignalEff)) {
      targetCutValue = signalHist->GetXaxis()->GetBinCenter(b);
      bestCurrentSignalEff = ratio;
    }
  }

  return targetCutValue;
}

//*************************************************************************************************
//
//*************************************************************************************************
Double_t FindBkgEffAtFixedSignalEfficiency(TH1F* signalHist, TH1F* bkgHist, Double_t targetSignalEff ) {

  Double_t targetBkgEff = 0;
  Double_t bestCurrentSignalEff = 0;
  const UInt_t nPoints = signalHist->GetXaxis()->GetNbins();
  double NSigTotal = 0;
  double NBkgTotal = 0;

  for (UInt_t q=0; q < nPoints+2; ++q) {
    NSigTotal += signalHist->GetBinContent(q);
    NBkgTotal += bkgHist->GetBinContent(q);
  }

  for(UInt_t b=0; b < nPoints; ++b) {
    Double_t nsig = 0;
    for (UInt_t q=b; q < nPoints+2; ++q) {
      nsig += signalHist->GetBinContent(q);
    }

    Double_t nbkg = 0;
    for (UInt_t q=b; q < nPoints+2; ++q) {
      nbkg += bkgHist->GetBinContent(q);
    }

    Double_t ratio = nsig / NSigTotal;
    Double_t bkgEff = nbkg / NBkgTotal;
//     cout << targetSignalEff << " : " << ratio << " , " << bkgEff << " : " << signalHist->GetXaxis()->GetBinCenter(b) << endl;

    if (fabs(targetSignalEff - ratio) < fabs(targetSignalEff - bestCurrentSignalEff)) {
      bestCurrentSignalEff = ratio;
      targetBkgEff = bkgEff;
    }
  }

  return targetBkgEff;
}

//*************************************************************************************************
//
//*************************************************************************************************
Double_t FindSigEffAtFixedBkgEfficiency(TH1F* signalHist, TH1F* bkgHist, Double_t targetBkgEff ) {

  Double_t targetSignalEff = 0;
  Double_t bestCurrentBkgEff = 0;
  const UInt_t nPoints = signalHist->GetXaxis()->GetNbins();
  double NSigTotal = 0;
  double NBkgTotal = 0;

  for (UInt_t q=0; q < nPoints+2; ++q) {
    NSigTotal += signalHist->GetBinContent(q);
    NBkgTotal += bkgHist->GetBinContent(q);
  }

  for(UInt_t b=0; b < nPoints; ++b) {
    Double_t nsig = 0;
    for (UInt_t q=b; q < nPoints+2; ++q) {
      nsig += signalHist->GetBinContent(q);
    }

    Double_t nbkg = 0;
    for (UInt_t q=b; q < nPoints+2; ++q) {
      nbkg += bkgHist->GetBinContent(q);
    }

    Double_t sigEff = nsig / NSigTotal;
    Double_t bkgEff = nbkg / NBkgTotal;
//     cout << targetSignalEff << " : " << ratio << " , " << bkgEff << " : " << signalHist->GetXaxis()->GetBinCenter(b) << endl;

    if (fabs(targetBkgEff - bkgEff) < fabs(targetBkgEff - bestCurrentBkgEff)) {
      bestCurrentBkgEff = bkgEff;
      targetSignalEff = sigEff;
    }
  }

  return targetSignalEff;
}

vector<TGraphAsymmErrors*> ROCGraphs;
vector<string> GraphLabels;
vector<Int_t> colors;
vector<string> PlotnameSpecific;
TLegend* legend;
TCanvas* cv;

void SetupPlots()
{
  ROCGraphs.clear();
  GraphLabels.clear();
  PlotnameSpecific.clear();

  legend = new TLegend(0.5,0.14,0.94,0.44);
  legend->SetTextSize(0.03);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);

  cv = new TCanvas("cv", "cv", 800, 600);
}

//*************************************************************************************************
//*************************************************************************************************
void BDTPerformancePlots(string InputFile, string Label, Int_t Option)
{  

  string label = "";
  if (Label != "") label = "_" + Label;

  //--------------------------------------------------------------------------------------------------------------
  // Histograms
  //==============================================================================================================  
  TH1F *Signal_MVA = new TH1F(("Signal_MVA"+label).c_str(), "LLP Signal ; MVA (BDT) score for LLP signal ; Number of Events ",  55000, -1.1 , 1.1);
  TH1F *Background_MVA = new TH1F(("Background_MVA"+label).c_str(), "W+Jets Background ; MVA (BDT) score for W+jets background ; Number of Events ",  55000, -1.1 , 1.1);

  Double_t RealElectrons = 0;
  Double_t FakeElectrons = 0;
  Double_t RealElectronPassCSA14Tight = 0;
  Double_t FakeElectronPassCSA14Tight = 0;

  //*****************************************************************************************
  //*****************************************************************************************
  TTree *tree = getTreeFromFile(InputFile.c_str(), "TestTree"); // TTree options are TestTree and TrainTree. BDTWeightFilesTest is a TDirectoryFile 

  int sig_bkg;
  // char name;
  float score;
  tree->SetBranchAddress("classID", &sig_bkg);
  // tree->SetBranchAddress("className", &name);
  tree->SetBranchAddress("BDTG", &score);

  cout << "Total Entries: " << tree->GetEntries() << "\n";
  int nentries = tree->GetEntries();
  for(int ientry=0; ientry < nentries; ientry++) {       	
    tree->GetEntry(ientry);
    
    if (ientry % 100000 == 0) cout << "Event " << ientry << endl;
        
    //don't evaluate performance using training events

    //classify by eta and pt bins

    //Some Preselection cuts
    
    //Real Electron
    // eventually need to fill with weights
	  Signal_MVA->Fill(score, sig_bkg == 0); // signal

    // //FakeElectron
	  Background_MVA->Fill(score, sig_bkg == 1);
  } 

  //*****************************************************************************************
  // Current Working Points
  //*****************************************************************************************
  cout << "Efficiencies for label = " << Label << endl;
  cout << "Signal efficiency at background of 0.001  : " << FindSigEffAtFixedBkgEfficiency(Signal_MVA, Background_MVA, 0.001) <<  endl;
  cout << "Signal efficiency at background of 0.0001  : " << FindSigEffAtFixedBkgEfficiency(Signal_MVA, Background_MVA, 0.0001) <<  endl;
  cout << "Signal efficiency at background of 0.00001  : " << FindSigEffAtFixedBkgEfficiency(Signal_MVA, Background_MVA, 0.00001) <<  endl;
  cout << "Background efficiency at signal of 80% : " << FindBkgEffAtFixedSignalEfficiency(Signal_MVA, Background_MVA, 0.8) << endl;
  cout << "Background efficiency at signal of 70% : " << FindBkgEffAtFixedSignalEfficiency(Signal_MVA, Background_MVA, 0.7) << endl;
  cout << "Background efficiency at signal of 60% : " << FindBkgEffAtFixedSignalEfficiency(Signal_MVA, Background_MVA, 0.6) << endl;

  //*****************************************************************************************
  //Find Cut with same signal efficiency
  //*****************************************************************************************
  cout << "MVA Cut Value at 50% Sig Eff: " << FindCutValueAtFixedEfficiency(Signal_MVA, 0.5 ) << endl;
  cout << "MVA Cut Value at 50% Bkg Eff: " << FindCutValueAtFixedEfficiency(Background_MVA, 0.5 ) << endl;

  //*****************************************************************************************
  // Make ROC curves
  //*****************************************************************************************
  // return a vector of TGraphAsymmErrors, sig vs bkg, sig vs bkg rej, sig vs inverse bkg eff
  TGraphAsymmErrors* ROC_sigEffBkgEff = MakeSigEffVsBkgEffGraph(Signal_MVA, Background_MVA, "ROC_MVA_LLP_W+jets"+label );

  TLegend* legend_indiv;
  TCanvas* cv_indiv;
  string plotname;

  cv_indiv = new TCanvas("cv_indiv", "cv_indiv", 800, 600);

  legend_indiv = new TLegend(0.5,0.14,0.94,0.44);
  legend_indiv->SetTextSize(0.03);
  legend_indiv->SetBorderSize(0);
  legend_indiv->SetFillStyle(0);

  plotname = "LLP_WJets_MVA"+label;

  //*****************************************************************************************
  // Overlay signal and background BDT scores
  //*****************************************************************************************
  THStack *hs = new THStack("hs", Form("Signal and Background BDT Scores (mH=%s) ; BDT Score ; Number of Entries",Label.c_str()));
  Signal_MVA->SetFillColor(kBlue);
  Background_MVA->SetFillColor(kRed);
  Signal_MVA->Rebin(20);
  Background_MVA->Rebin(20);
  hs->Add(Signal_MVA);
  hs->Add(Background_MVA);
  cv_indiv->cd();
  hs->Draw("bar1 nostack");
  gPad->BuildLegend(0.65,0.65,0.85,0.85,"");
  cv_indiv->SaveAs(("BDTscore_" + plotname + ".png").c_str());
  gPad->SetLogy();
  cv_indiv->SaveAs(("BDTscore_" + plotname + "_logY.png").c_str());
  cv_indiv->Clear();

  //*****************************************************************************************
  //Plot ROC Curves
  //*****************************************************************************************
  ROCGraphs.push_back(ROC_sigEffBkgEff);
  GraphLabels.push_back("MVA LLP (mh=" + Label + ") vs. W+jets");
  colors.push_back(Option);
  PlotnameSpecific.push_back(plotname);

  Double_t xmin = 0.0;
  Double_t xmax = 1.0;
  Double_t ymin = 0.0;
  Double_t ymax = 1.0;

  bool overlay = true;

  //*****************************************************************************************

  for (UInt_t i=0; i<GraphLabels.size(); ++i) {
    if (i == GraphLabels.size() - 1) legend->AddEntry(ROCGraphs[i],GraphLabels[i].c_str(), "LP");

    ROCGraphs[i]->SetMarkerColor(colors[i]);
    ROCGraphs[i]->SetLineColor(colors[i]);
    ROCGraphs[i]->SetMarkerSize(0.5);

    ROCGraphs[i]->GetXaxis()->SetRangeUser(xmin,xmax);    
    ROCGraphs[i]->GetYaxis()->SetRangeUser(ymin,ymax);    
    
    if (overlay) {
      cv->cd();
      if (i==0) {
        ROCGraphs[i]->Draw("AP");
      } else {
        ROCGraphs[i]->Draw("Psame");
      }
      ROCGraphs[i]->GetXaxis()->SetLimits(0.00001,1);
    }
    if (i == GraphLabels.size() - 1) {  // if want to save each ROC curve individually, only do last (most recently added) in list
      cv_indiv->cd();
      legend_indiv->AddEntry(ROCGraphs[i],GraphLabels[i].c_str(), "LP");
      ROCGraphs[i]->Draw("AP");
      legend_indiv->Draw();
      cv_indiv->SaveAs(("ROCGraphs_" + PlotnameSpecific[i] + ".png").c_str());
      ROCGraphs[i]->GetXaxis()->SetLimits(0.00001,1); // set non-zero lower value such that a log shows the lowest signal efficiency values too (if 0, minimum is 10^-3 below maximum)
      gPad->SetLogx();
      cv_indiv->SaveAs(("ROCGraphs_" + PlotnameSpecific[i] + "_logx.png").c_str());

      legend_indiv->Clear();
      gPad->SetLogx(1);
    }
  }
  
  // overlay multiple ROC curves -- useful if comparing 125 vs. 350 for instance
  if (overlay) {
    cv->cd();
    gPad->SetLogx();
    legend->Draw();
    cv->SaveAs(("ROCGraphs_" + plotname + "_overlay.png").c_str());
  }
} 

//*************************************************************************************************
//*************************************************************************************************
void MakeMVAPerformancePlots()
{  
  SetupPlots();

  // BDTPerformancePlots(InputFile, Label, Option);

  // BDTPerformancePlots("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/BDTWeightFilesTest/Test_LLP125.root", "125", 38);
  // BDTPerformancePlots("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/BDTWeightFilesTest/Test_LLP350.root", "350", 30);
  // BDTPerformancePlots("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/BDTWeightFilesTest/Test_hadd.root", "hadd", 48);
  BDTPerformancePlots("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/BDTWeightFilesTest/Test_LLP125_perJet.root", "125_perJet", 38);
  BDTPerformancePlots("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/BDTWeightFilesTest/Test_LLP350_perJet.root", "350_perJet", 30);
  BDTPerformancePlots("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/BDTWeightFilesTest/Test_hadd_perJet.root", "hadd_perJet", 48);
}