// root -l /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/MakeMVAPerformancePlots.C+'("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/BDTWeightFilesTest/Test_mh350.root","All",-1)'

// void MakeMVAPerformancePlots(string InputFile, string Label, Int_t Option)
//================================================================================================
//
// HWW selection macro
//
//  * plots distributions associated with selected events
//  * prints list of selected events from data
//  * outputs ROOT files of events passing selection for each sample, 
//    which can be processed by plotSelect.C
//
//________________________________________________________________________________________________

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
TGraphAsymmErrors* MakeSigEffVsBkgEffGraph(TH1F* signalHist, TH1F* bkgHist, string name ) {
  //Make Met Plots
  const UInt_t nPoints = signalHist->GetXaxis()->GetNbins();
  double SigEff[nPoints];
  double BkgEff[nPoints];
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
  tmpSigEffVsBkgEff->SetTitle("");
  tmpSigEffVsBkgEff->GetXaxis()->SetTitle("Bkg Eff");
  tmpSigEffVsBkgEff->GetYaxis()->SetTitle("Signal Eff");
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

//*************************************************************************************************
//*************************************************************************************************
void MakeMVAPerformancePlots(string InputFile, string Label, Int_t Option)
{  

  string label = "";
  if (Label != "") label = "_" + Label;

  //--------------------------------------------------------------------------------------------------------------
  // Histograms
  //==============================================================================================================  
  TH1F *Signal_MVA = new TH1F(("Signal_MVA"+label).c_str(), "LLP Signal ; MVA (BDT) score for LLP signal ; Number of Events ",  5500, -1.1 , 1.1);
  TH1F *Background_MVA = new TH1F(("Background_MVA"+label).c_str(), "W+Jets Background ; MVA (BDT) score for W+jets background ; Number of Events ",  5500, -1.1 , 1.1);

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
  
//   //*****************************************************************************************
//   //Current Working Points
//   //*****************************************************************************************
//   cout << "CSA14 Tight Real Electron Efficiency : " << RealElectronPassCSA14Tight << " / " << RealElectrons << " = " << RealElectronPassCSA14Tight/RealElectrons << endl;
//   cout << "CSA14 Tight Fake Electron Efficiency : " << FakeElectronPassCSA14Tight << " / " << FakeElectrons << " = " << FakeElectronPassCSA14Tight/FakeElectrons << endl;
//   TGraphAsymmErrors* ROC_CSA14TightWP = MakeCurrentWPSigEffVsBkgEffGraph(RealElectronPassCSA14Tight/RealElectrons , FakeElectronPassCSA14Tight/FakeElectrons, "ROC_CSA14TightWP"+label);

//   Double_t BkgEffCSA14Tight = FakeElectronPassCSA14Tight/FakeElectrons;
//   Double_t SigEffCSA14Tight = RealElectronPassCSA14Tight/RealElectrons;

//   cout << "**********************\n";
//   Double_t SigEffIDMVATrig_AtTightBkgEff = FindSigEffAtFixedBkgEfficiency(Signal_MVA, Background_MVA, BkgEffCSA14Tight);
//   Double_t BkgEffIDMVATrig_AtTightSigEff = FindBkgEffAtFixedSignalEfficiency(Signal_MVA, Background_MVA, SigEffCSA14Tight);
//   cout << "Signal Efficiency (wrt CSA14Tight Cut-based) for : same bkg \n";
//   cout << "IDMVATrig : " << SigEffIDMVATrig_AtTightBkgEff/SigEffCSA14Tight <<  endl;
//   cout << "Bkg Efficiency (wrt CSA14Veto Cut-based) for same sig eff \n";
//   cout << "IDMVATrig : " << BkgEffIDMVATrig_AtTightSigEff/BkgEffCSA14Tight << endl;
//   cout << "**********************\n";

//   cout << "**********************\n";
//   Double_t BkgEffIDMVATrig_AtVetoSigEff = FindBkgEffAtFixedSignalEfficiency(Signal_MVA, Background_MVA, SigEffCSA14Veto);
//   Double_t SigEffIDMVATrig_AtVetoBkgEff = FindSigEffAtFixedBkgEfficiency(Signal_MVA, Background_MVA, BkgEffCSA14Veto);
//   cout << "Sig Efficiency (wrt CSA14Veto Cut-based) for same bkg eff \n";
//   cout << "IDMVATrig : " << SigEffIDMVATrig_AtVetoBkgEff/SigEffCSA14Veto << endl;
//   cout << "Bkg Efficiency (wrt CSA14Veto Cut-based) for same sig eff \n";
//   cout << "IDMVATrig : " << BkgEffIDMVATrig_AtVetoSigEff/BkgEffCSA14Veto << endl;
//   cout << "**********************\n";

   //*****************************************************************************************
   //Make ROC curves
   //*****************************************************************************************
   TGraphAsymmErrors* ROC_MVA_LLPWjets = MakeSigEffVsBkgEffGraph(Signal_MVA, Background_MVA, "ROC_MVA_LLP_W+jets"+label );

//   //*****************************************************************************************
//   //Find Cut with same signal efficiency
//   //*****************************************************************************************
//   Double_t CutValue_IDMVATrig_SameSig = FindCutValueAtFixedEfficiency(Signal_MVA, SigEffCSA14Tight );
//   Double_t CutValue_IDMVATrig_SameBkg = FindCutValueAtFixedEfficiency(Background_MVA, BkgEffCSA14Tight );
//   cout << "IDMVATrig Cut Value @ Same Cut-Based Tight Sig Eff: " << CutValue_IDMVATrig_SameSig << endl;
//   cout << "IDMVATrig Cut Value @ Same Cut-Based Tight Bkg Eff: " << CutValue_IDMVATrig_SameBkg << endl;

  TLegend* legend;
  TCanvas* cv;
  string plotname;

  //*****************************************************************************************
  //Plot ROC Curves
  //*****************************************************************************************
  vector<TGraphAsymmErrors*> ROCGraphs;
  vector<string> GraphLabels;
  vector<Int_t> colors;

  //*****************************************************************************************
  //*****************************************************************************************
  ROCGraphs.clear();
  GraphLabels.clear();
  plotname = "LLP_WJets_MVA"+label;

  ROCGraphs.push_back(ROC_MVA_LLPWjets);
  GraphLabels.push_back("MVA LLP vs W+jets");
  colors.push_back(kBlue);

  //*****************************************************************************************
  Double_t xmin = 0.0;
  Double_t xmax = 1.0;
  Double_t ymin = 0.0;
  Double_t ymax = 1.0;

  cv = new TCanvas("cv", "cv", 800, 600);

  legend = new TLegend(0.54,0.14,0.94,0.44);
  legend->SetTextSize(0.03);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);
  for (UInt_t i=0; i<GraphLabels.size(); ++i) {
    legend->AddEntry(ROCGraphs[i],GraphLabels[i].c_str(), "LP");

    ROCGraphs[i]->SetMarkerColor(colors[i]);
    ROCGraphs[i]->SetLineColor(colors[i]);
    ROCGraphs[i]->SetMarkerSize(0.5);
   
    ROCGraphs[i]->GetXaxis()->SetRangeUser(xmin,xmax);    
    ROCGraphs[i]->GetYaxis()->SetRangeUser(ymin,ymax);    
    if (i==0) {
      ROCGraphs[i]->Draw("AP");
    } else {
      ROCGraphs[i]->Draw("Psame");
    }
  }

  // legend->AddEntry(ROC_CSA14TightWP, "CSA14Tight WP", "P");
  // ROC_CSA14TightWP->SetFillColor(kBlue);
  // ROC_CSA14TightWP->SetMarkerColor(kBlue);
  // ROC_CSA14TightWP->SetMarkerStyle(34);
  // ROC_CSA14TightWP->SetMarkerSize(2.5);
  // ROC_CSA14TightWP->Draw("Psame");

  legend->Draw();
  
  cv->SaveAs(("ROCGraphs_" + plotname + ".png").c_str());

  //*****************************************************************************************
  // Overlay signal and background BDT scores
  //*****************************************************************************************
  THStack *hs = new THStack("hs", "Signal and Background BDT Scores ; BDT Score ; Number of Entries");
  Signal_MVA->SetFillColor(kBlue);
  Background_MVA->SetFillColor(kRed);
  Signal_MVA->Rebin(20);
  Background_MVA->Rebin(20);
  hs->Add(Signal_MVA);
  hs->Add(Background_MVA);
  hs->Draw("bar1 nostack");
  gPad->BuildLegend(0.65,0.65,0.85,0.85,"");
  cv->SaveAs(("BDTscore_" + plotname + ".png").c_str());

} 