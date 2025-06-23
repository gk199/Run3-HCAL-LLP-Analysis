// root -l /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Classifiers/MakeDNNPerformancePlots_SigBkg.C+'("/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.1/minituple_v3.1_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_01_20_TEST.root","125",30,"/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.1/minituple_v3.1_LLPskim_Run2023Cv3_2024_01_20.root","W+Jets",38)'
// inputs are minituple signal file, label, color, minituple background file, label, color
// root -l /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Classifiers/MakeDNNPerformancePlots_SigBkg.C+'()'

// Adapted from https://github.com/cms-lpc-llp/llp_analyzer/blob/master/macros/ObjectStudies/MakeElectronIDMVAPerformancePlots.C by Gillian Kopp

// void MakeDNNPerformancePlots_SigBkg(string InputFile, string Label, Int_t Option, string InputFile2, string Label2, Int_t Option2)
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

#include <filesystem>
namespace fs = std::filesystem;

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

void StampCMS( const std::string & approvaltext = "Internal", float lumi=140., float x=0.14, float y=0.84, float textsize=0.045) {
  // Usage:  StampCMS("Internal", 126.5, 0.12, 0.75);
  
  textsize = 0.045;
  TLatex *label_cms = new TLatex();
  label_cms->SetNDC();
  label_cms->SetTextColor(1);
  label_cms->SetTextSize(textsize);
  label_cms->DrawLatex( x, y, "CMS" );

  TLatex *label_approvaltext = new TLatex();
  label_approvaltext->SetNDC();
  label_approvaltext->SetTextFont(52);
  label_approvaltext->SetTextColor(1);
  label_approvaltext->SetTextSize(textsize*0.85);
  label_approvaltext->DrawLatex( x+0.075, y, approvaltext.c_str() ); 

  TLatex *label_energylumi = new TLatex(); 
  label_energylumi->SetNDC();
  label_energylumi->SetTextFont(42);
  label_energylumi->SetTextColor(1);
  label_energylumi->SetTextSize(textsize*0.65 );
  label_energylumi->DrawLatex( x, y-.035, "#sqrt{s} = 13.6 TeV" ); // Just energy for now...
}

//*************************************************************************************************
//
//*************************************************************************************************
// return a TGraphAsymmErrors, sig vs bkg
TGraphAsymmErrors* MakeSigEffVsBkgEffGraph(TH1F* signalHist, TH1F* bkgHist, string name, string Label, string plotType ) {
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
  tmpSigEffVsBkgEff->SetTitle(Form("ROC Curve for LLP (mh=%s) vs. W+jets %s", Label.c_str(), plotType.c_str()));
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
TGraphAsymmErrors* MakeCurrentWPSigEffVsBkgEffGraph(Double_t signalEff, Double_t bkgEff, string name ) {
  double SigEff[1];
  double BkgEff[1];
  double SigEffErrLow[1];
  double SigEffErrHigh[1];
  double BkgEffErrLow[1];
  double BkgEffErrHigh[1];
  double NSigTotal = 0;
  double NBkgTotal = 0;
  double cutValue;

  SigEff[0] = signalEff;
  SigEffErrLow[0] = 0;
  SigEffErrHigh[0] = 0;
  BkgEff[0] = bkgEff;
  BkgEffErrLow[0] = 0;
  BkgEffErrHigh[0] = 0;

  TGraphAsymmErrors *tmpSigEffVsBkgEff = new TGraphAsymmErrors (1, BkgEff, SigEff, BkgEffErrLow, BkgEffErrHigh , SigEffErrLow, SigEffErrHigh );
  tmpSigEffVsBkgEff->SetName(name.c_str());
  tmpSigEffVsBkgEff->SetTitle("");
  tmpSigEffVsBkgEff->GetXaxis()->SetTitle("Bkg Eff");
  tmpSigEffVsBkgEff->GetYaxis()->SetTitle("Signal Eff");
  tmpSigEffVsBkgEff->GetYaxis()->SetTitleOffset(1.1);
  tmpSigEffVsBkgEff->GetXaxis()->SetTitleOffset(1.05);
  tmpSigEffVsBkgEff->SetMarkerColor(kBlack);
  tmpSigEffVsBkgEff->SetLineColor(kBlack);
  tmpSigEffVsBkgEff->SetMarkerSize(1.5);

  return tmpSigEffVsBkgEff;
}

//*************************************************************************************************
//
//*************************************************************************************************
TGraphAsymmErrors* MakeCurrentWPSigEffVsCutValueGraph(TH1F* signalHist, string name, Double_t myCutValue ) {
  const UInt_t nPoints = signalHist->GetXaxis()->GetNbins();
  double cutValue[nPoints];
  double cutValueErr[nPoints];
  double SigEff[nPoints];
  double SigEffErrLow[nPoints];
  double SigEffErrHigh[nPoints];
  double NSigTotal = 0;

  memset(cutValue, 0, sizeof cutValue);
  memset(cutValueErr, 0, sizeof cutValueErr);
  memset(SigEff, 0, sizeof SigEff);
  memset(SigEffErrLow, 0, sizeof SigEffErrLow);
  memset(SigEffErrHigh, 0, sizeof SigEffErrHigh);

  Double_t effDiff = 9999;

  for (UInt_t q=0; q < nPoints+2; ++q) {
    NSigTotal += signalHist->GetBinContent(q);
  }

  for(UInt_t b=0; b < nPoints; ++b) {
    Double_t nsig = 0;
    for (UInt_t q=b; q < nPoints+2; ++q) {
      nsig += signalHist->GetBinContent(q);
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
    cutValue[b] = signalHist->GetXaxis()->GetBinCenter(b);
    cutValueErr[b] = 0;
    // if (fabs(myCutValue - signalHist->GetXaxis()->GetBinCenter(b)) < fabs(myCutValue - cutValue[0])) {
    //   SigEff[0] = ratio;
    //   SigEffErrLow[0] = 0;
    //   SigEffErrHigh[0] = 0;
    //   cutValue[0] = signalHist->GetXaxis()->GetBinCenter(b);
    //   cutValueErr[0] = 0;
    // }
  }
  TGraphAsymmErrors *tmpSigEffVsCut = new TGraphAsymmErrors (nPoints, cutValue, SigEff, cutValueErr, cutValueErr, SigEffErrLow, SigEffErrHigh  );
  tmpSigEffVsCut->SetName(name.c_str());
  tmpSigEffVsCut->SetTitle("");
  tmpSigEffVsCut->GetXaxis()->SetTitle("DNN Cut Value");
  tmpSigEffVsCut->GetYaxis()->SetTitle("Signal Efficiency");
  tmpSigEffVsCut->GetYaxis()->SetTitleOffset(1.1);
  tmpSigEffVsCut->GetXaxis()->SetTitleOffset(1.05);
  tmpSigEffVsCut->SetMarkerColor(kBlack);
  tmpSigEffVsCut->SetLineColor(kBlack);
  tmpSigEffVsCut->SetMarkerSize(1.5);

  return tmpSigEffVsCut;
}

//*************************************************************************************************
//
//*************************************************************************************************
TGraphAsymmErrors* MakeCurrentWPSigEffVsBkgEffAtCutValueGraph(TH1F* signalHist, TH1F* bkgHist, string name, Double_t myCutValue ) {
  const UInt_t nPoints = signalHist->GetXaxis()->GetNbins();
  double cutValue[1] = {0};
  double cutValueErr[1] = {0};
  double SigEff[1] = {0};
  double SigEffErrLow[1] = {0};
  double SigEffErrHigh[1] = {0};
  double BkgEff[1] = {0};
  double BkgEffErrLow[1] = {0};
  double BkgEffErrHigh[1] = {0};
  double NSigTotal = 0;
  double NBkgTotal = 0;
  
  Double_t effDiff = 9999;

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

    Double_t ratio_bkg;
    Double_t n1_bkg = 0;
    Double_t n2_bkg = 0;
    
    n1_bkg = TMath::Nint(nbkg);
    n2_bkg = TMath::Nint(NBkgTotal);
    ratio_bkg = n1_bkg/n2_bkg;
    
    if (fabs(myCutValue - signalHist->GetXaxis()->GetBinCenter(b)) < fabs(myCutValue - cutValue[0])) {
      SigEff[0] = ratio;
      SigEffErrLow[0] = 0;
      SigEffErrHigh[0] = 0;
      BkgEff[0] = ratio_bkg;
      BkgEffErrLow[0] = 0;
      BkgEffErrHigh[0] = 0;
      cutValue[0] = signalHist->GetXaxis()->GetBinCenter(b);
      cutValueErr[0] = 0;
    }
  }
  TGraphAsymmErrors *tmpSigEffVsBkg = new TGraphAsymmErrors (1, BkgEff, SigEff, BkgEffErrLow, BkgEffErrHigh, SigEffErrLow, SigEffErrHigh  );
  tmpSigEffVsBkg->SetName(name.c_str());
  tmpSigEffVsBkg->SetTitle("");
  tmpSigEffVsBkg->GetXaxis()->SetTitle("Bkg Efficiency");
  tmpSigEffVsBkg->GetYaxis()->SetTitle("Signal Efficiency");
  tmpSigEffVsBkg->GetYaxis()->SetTitleOffset(1.1);
  tmpSigEffVsBkg->GetXaxis()->SetTitleOffset(1.05);
  tmpSigEffVsBkg->SetMarkerColor(kBlack);
  tmpSigEffVsBkg->SetLineColor(kBlack);
  tmpSigEffVsBkg->SetMarkerSize(1.5);

  return tmpSigEffVsBkg;
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

  legend = new TLegend(0.42,0.12,0.9,0.34);
  legend->SetTextSize(0.03);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);

  cv = new TCanvas("cv", "cv", 800, 600);
}

//*************************************************************************************************
//*************************************************************************************************
void BDTPerformancePlots(string InputFile, string Label, string SigTree, string InputFile2, string Label2, string BkgTree, Int_t Option, Int_t Option2, Int_t Option3, string plotType)
{  
  string label = "";
  if (Label != "") label = "_" + Label;

  string plotDir = plotType + "/";
  if (plotType == "") plotDir = "";

  //--------------------------------------------------------------------------------------------------------------
  // Histograms
  //==============================================================================================================  
  TH1F *Signal_DNN_depth = new TH1F(("Signal_DNN_depth"+label).c_str(), "LLP Signal ; DNN (trained on combination) score for LLP signal ; Number of Events ",  55000, -0.1 , 1.1);
  TH1F *Signal_DNN_inclusive = new TH1F(("Signal_DNN_depth"+label).c_str(), "LLP Signal ; Inclusive DNN (trained on combination) score for LLP signal ; Number of Events ",  55000, -0.1 , 1.1);

  TH1F *Background_DNN_depth = new TH1F(("Background_DNN_depth"+label).c_str(), "W+Jets Background ; DNN (trained on combination) score for W+jets background ; Number of Events ",  55000, -0.1 , 1.1);
  TH1F *Background_DNN_inclusive = new TH1F(("Background_DNN_inclusive"+label).c_str(), "W+Jets Background ; Inclusive DNN (trained on combination) score for W+jets background ; Number of Events ",  55000, -0.1 , 1.1);

  Double_t RealElectrons = 0;
  Double_t FakeElectrons = 0;
  Double_t RealElectronPassCSA14Tight = 0;
  Double_t FakeElectronPassCSA14Tight = 0;

  //*****************************************************************************************
  // Get signal distribution
  //*****************************************************************************************
  TTree *tree_sig = getTreeFromFile(InputFile.c_str(), SigTree.c_str());

  float LLP0_DecayR, LLP1_DecayR, LLP0_Eta, LLP1_Eta, jet0_isMatchedTo, jet0_Eta, jet0_Pt;
  float perJet_MatchedLLP_DecayR, perJet_MatchedLLP_Eta;

  float score_depth_sig, score_inclusive_sig;

  float radius_ECAL = 129;
  float radius_HB1 = 177;
  float radius_HB2 = 190.2;
  float radius_HB3 = 214.2;
  float radius_HB4 = 244.8;
  float radius_HBend = 295;
  float HBeta = 1.26;

  TCut SignalSelection = "";
  TCut SignalSelection_inc = "";
  if (SigTree.find("PerJet") == std::string::npos) { // per event tree
    tree_sig->SetBranchAddress("LLP0_DecayR", &LLP0_DecayR);
    tree_sig->SetBranchAddress("LLP1_DecayR", &LLP1_DecayR);
    tree_sig->SetBranchAddress("LLP0_Eta", &LLP0_Eta);
    tree_sig->SetBranchAddress("LLP1_Eta", &LLP1_Eta);
    tree_sig->SetBranchAddress("jet0_isMatchedTo", &jet0_isMatchedTo);
    tree_sig->SetBranchAddress("jet0_Eta", &jet0_Eta);
    tree_sig->SetBranchAddress("jet0_Pt", &jet0_Pt);
    SignalSelection = Form("((LLP0_DecayR >= %f && LLP0_DecayR < %f && abs(LLP0_Eta) <= %f && jet0_isMatchedTo == 0) || (LLP1_DecayR >= %f && LLP1_DecayR < %f && abs(LLP1_Eta) <= %f && jet0_isMatchedTo == 1)) && (abs(jet0_Eta) < 1.26 && jet0_Pt > 40)", radius_HB1, radius_HBend, HBeta, radius_HB1, radius_HBend, HBeta); // radius_HB3
    SignalSelection_inc = Form("((LLP0_DecayR < %f && abs(LLP0_Eta) <= %f && jet1_isMatchedTo == 0) || (LLP1_DecayR < %f && abs(LLP1_Eta) <= %f && jet1_isMatchedTo == 1)) && (abs(jet1_Eta) < 1.26 && jet1_Pt > 40)", radius_HBend, HBeta, radius_HBend, HBeta);
    cout << "per event tree, adding signal region cuts on LLP position and matching" << endl;
    // hopefully there is a better way to implement these cuts, had TCut errors when imported RegionCuts.h on first try
    // reduces by a factor of about 10 for mh=125
  }
  if (SigTree.find("PerJet") != std::string::npos) {
    tree_sig->SetBranchAddress("perJet_MatchedLLP_DecayR", &perJet_MatchedLLP_DecayR);
    tree_sig->SetBranchAddress("perJet_MatchedLLP_Eta", &perJet_MatchedLLP_Eta);
    SignalSelection = Form("perJet_MatchedLLP_DecayR >= %f && perJet_MatchedLLP_DecayR < %f && abs(perJet_MatchedLLP_Eta) < %f", radius_HB1, radius_HBend, 1.26);
    SignalSelection_inc = Form("perJet_MatchedLLP_DecayR < %f && abs(perJet_MatchedLLP_Eta) < %f", radius_HBend, 1.26);
    if (plotType.find("HCAL12") != std::string::npos) {
      SignalSelection = Form("perJet_MatchedLLP_DecayR >= %f && perJet_MatchedLLP_DecayR < %f && abs(perJet_MatchedLLP_Eta) < %f", radius_HB1, radius_HB3, 1.26);
      cout << "selection = HCAL depth 12 only" << endl;
    }
    if (plotType.find("HCAL34") != std::string::npos) {
      SignalSelection = Form("perJet_MatchedLLP_DecayR >= %f && perJet_MatchedLLP_DecayR < %f && abs(perJet_MatchedLLP_Eta) < %f", radius_HB3, radius_HBend, 1.26);
      cout << "selection = HCAL depth 34 only" << endl;
    }
    cout << "per jet tree, adding signal region cuts on LLP position" << endl; // this tree already requires jet pT > 40 and eta < 1.26
  }

  TFile *myReducedFile = new TFile("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Classifiers/temp.root", "RECREATE"); // preventing error "This error is symptomatic of a Tree created as a memory-resident Tree"
  TTree *tree_sig_reduced = tree_sig->CopyTree(SignalSelection, "", tree_sig->GetEntries(), 0); // no cuts needed for already jet matched tree
  TTree *tree_sig_reduced_inc = tree_sig->CopyTree(SignalSelection_inc, "", tree_sig->GetEntries(), 0);

  if (SigTree.find("PerJet") == std::string::npos) {
    tree_sig_reduced->SetBranchAddress("jet0_scores_inc_train80", &score_depth_sig);  
    tree_sig_reduced_inc->SetBranchAddress("jet0_scores_inc_train40", &score_inclusive_sig);  
  }
  if (SigTree.find("PerJet") != std::string::npos) {
    tree_sig_reduced->SetBranchAddress("scores", &score_depth_sig);  
    tree_sig_reduced_inc->SetBranchAddress("scores_inc", &score_inclusive_sig);  
    // tree_sig_reduced->SetBranchAddress(("bdtscore_hadd"+plotType+"_perJet").c_str(), &scoreHadd_sig);
  }

  cout << "Total Entries (signal): " << tree_sig_reduced->GetEntries() << "\n";
  int nentries = tree_sig_reduced->GetEntries();
  for(int ientry=0; ientry < nentries; ientry++) {       	
    tree_sig_reduced->GetEntry(ientry);
    
    if (ientry % 100000 == 0) cout << "Event " << ientry << endl;
        
    //don't evaluate performance using training events
    int third_decimal = static_cast<int>(std::floor(jet0_Pt * 1000)) % 10;
    //classify by eta and pt bins
    //Some Preselection cuts -- here is where we specify the signal region based on LLP truth information 
    // eventually need to fill with weights
    if (third_decimal >= 8) Signal_DNN_depth->Fill(score_depth_sig);
  } 
  cout << "Total Inclusive Entries (signal): " << tree_sig_reduced_inc->GetEntries() << "\n";
  int nentries_inc = tree_sig_reduced_inc->GetEntries();
  for(int ientry=0; ientry < nentries_inc; ientry++) {       	
    tree_sig_reduced_inc->GetEntry(ientry);
    if (ientry % 100000 == 0) cout << "Event " << ientry << endl;

    int third_decimal = static_cast<int>(std::floor(jet0_Pt * 1000)) % 10;

    if (third_decimal >= 4) Signal_DNN_inclusive->Fill(score_inclusive_sig);
  } 
  //*****************************************************************************************
  // Get background distribution
  //*****************************************************************************************
  TTree *tree_bkg = getTreeFromFile(InputFile2.c_str(), BkgTree.c_str()); 
  
  float score_depth_bkg, score_inclusive_bkg;
  bool Pass_WPlusJets; 

  tree_bkg->SetBranchAddress("Pass_WPlusJets", &Pass_WPlusJets);  // Set branch address for the flag

  if (SigTree.find("PerJet") == std::string::npos) {
    tree_bkg->SetBranchAddress("jet0_scores_inc_train80", &score_depth_bkg);  
    tree_bkg->SetBranchAddress("jet0_scores_inc_train40", &score_inclusive_bkg);  
    tree_bkg->SetBranchAddress("jet0_Pt", &jet0_Pt);
    // tree_bkg->SetBranchAddress("bdtscore_hadd", &scoreHadd_bkg);
  }
  if (SigTree.find("PerJet") != std::string::npos) {
    tree_bkg->SetBranchAddress("scores", &score_depth_bkg);  
    tree_bkg->SetBranchAddress("scores_inc", &score_inclusive_bkg);  
    // tree_bkg->SetBranchAddress(("bdtscore_hadd"+plotType+"_perJet").c_str(), &scoreHadd_bkg);
  }

  cout << "Total Entries (background): " << tree_bkg->GetEntries() << "\n";
  int nentries_bkg = tree_bkg->GetEntries();
  for(int ientry=0; ientry < nentries_bkg; ientry++) {       	
    tree_bkg->GetEntry(ientry);
    
    if (ientry % 100000 == 0) cout << "Event " << ientry << endl;

    if (!Pass_WPlusJets) continue; // apply W+jets selection for the background
    int third_decimal = static_cast<int>(std::floor(jet0_Pt * 1000)) % 10;

    // eventually need to fill with weights
    if (third_decimal >= 8) Background_DNN_depth->Fill(score_depth_bkg);
    if (third_decimal >= 4) Background_DNN_inclusive->Fill(score_inclusive_bkg);
  } 

  //*****************************************************************************************
  // Current Working Points
  //*****************************************************************************************
  // cout << "Efficiencies for label = " << Label << ", for depth DNN" << endl;
  // cout << "Signal efficiency at background of 0.001  : " << FindSigEffAtFixedBkgEfficiency(Signal_DNN_depth, Background_DNN_depth, 0.001) <<  endl;
  // cout << "Signal efficiency at background of 0.0001  : " << FindSigEffAtFixedBkgEfficiency(Signal_DNN_depth, Background_DNN_depth, 0.0001) <<  endl;
  // cout << "Signal efficiency at background of 0.00001  : " << FindSigEffAtFixedBkgEfficiency(Signal_DNN_depth, Background_DNN_depth, 0.00001) <<  endl;
  // cout << "Background efficiency at signal of 80% : " << FindBkgEffAtFixedSignalEfficiency(Signal_DNN_depth, Background_DNN_depth, 0.8) << endl;
  // cout << "Background efficiency at signal of 70% : " << FindBkgEffAtFixedSignalEfficiency(Signal_DNN_depth, Background_DNN_depth, 0.7) << endl;
  // cout << "Background efficiency at signal of 60% : " << FindBkgEffAtFixedSignalEfficiency(Signal_DNN_depth, Background_DNN_depth, 0.6) << endl;

  // cout << "Efficiencies for label = " << Label << ", for inclusive DNN" << endl;
  // cout << "Signal efficiency at background of 0.001  : " << FindSigEffAtFixedBkgEfficiency(Signal_DNN_inclusive, Background_DNN_inclusive, 0.001) <<  endl;
  // cout << "Signal efficiency at background of 0.0001  : " << FindSigEffAtFixedBkgEfficiency(Signal_DNN_inclusive, Background_DNN_inclusive, 0.0001) <<  endl;
  // cout << "Signal efficiency at background of 0.00001  : " << FindSigEffAtFixedBkgEfficiency(Signal_DNN_inclusive, Background_DNN_inclusive, 0.00001) <<  endl;
  // cout << "Background efficiency at signal of 80% : " << FindBkgEffAtFixedSignalEfficiency(Signal_DNN_inclusive, Background_DNN_inclusive, 0.8) << endl;
  // cout << "Background efficiency at signal of 70% : " << FindBkgEffAtFixedSignalEfficiency(Signal_DNN_inclusive, Background_DNN_inclusive, 0.7) << endl;
  // cout << "Background efficiency at signal of 60% : " << FindBkgEffAtFixedSignalEfficiency(Signal_DNN_inclusive, Background_DNN_inclusive, 0.6) << endl;

  //*****************************************************************************************
  //Find Cut with same signal efficiency
  //*****************************************************************************************
  // cout << "DNN depth Cut Value at 50% Sig Eff: " << FindCutValueAtFixedEfficiency(Signal_DNN_depth, 0.5 ) << endl;
  // cout << "DNN depth Cut Value at 50% Bkg Eff: " << FindCutValueAtFixedEfficiency(Background_DNN_depth, 0.5 ) << endl;
  // cout << "DNN inclusive Cut Value at 50% Sig Eff: " << FindCutValueAtFixedEfficiency(Signal_DNN_inclusive, 0.5 ) << endl;
  // cout << "DNN inclusive Cut Value at 50% Bkg Eff: " << FindCutValueAtFixedEfficiency(Background_DNN_inclusive, 0.5 ) << endl;

  //*****************************************************************************************
  // Make ROC curves
  //*****************************************************************************************
  // return a vector of TGraphAsymmErrors, sig vs bkg, sig vs bkg rej, sig vs inverse bkg eff
  TGraphAsymmErrors* ROChadd_sigEffBkgEff = MakeSigEffVsBkgEffGraph(Signal_DNN_depth, Background_DNN_depth, "ROC_DNN_depth_LLP_W+jets"+label, Label, plotType );
  TGraphAsymmErrors* ROChadd_sigEffBkgEff_inc = MakeSigEffVsBkgEffGraph(Signal_DNN_inclusive, Background_DNN_inclusive, "ROC_DNN_inc_LLP_W+jets"+label, Label, plotType );

  TLegend* legend_indiv;
  TCanvas* cv_indiv;
  string plotname;

  cv_indiv = new TCanvas("cv_indiv", "cv_indiv", 800, 600);

  legend_indiv = new TLegend(0.5,0.14,0.94,0.44);
  legend_indiv->SetTextSize(0.03);
  legend_indiv->SetBorderSize(0);
  legend_indiv->SetFillStyle(0);

  // plotname = Label+"_LLP_WJets_DNN";
  plotname = Label;

  //*****************************************************************************************
  //Plot ROC Curves
  //*****************************************************************************************

  ROCGraphs.push_back(ROChadd_sigEffBkgEff);
  ROCGraphs.push_back(ROChadd_sigEffBkgEff_inc);
  GraphLabels.push_back("DNN trained on LLP combination");
  GraphLabels.push_back("Inclusive DNN trained on LLP combination");
  // colors.push_back(Option);
  // colors.push_back(Option2);
  // colors.push_back(Option3);
  gStyle->SetPalette(kCandy); // (kViridis);
  //colors.push_back(TColor::GetPalette().At(0));
  colors.push_back(TColor::GetPalette().At(50));
  //colors.push_back(TColor::GetPalette().At(100));
  //colors.push_back(TColor::GetPalette().At(150));
  colors.push_back(TColor::GetPalette().At(200));
  //colors.push_back(kGray+1); //(Option3);
  PlotnameSpecific.push_back(plotname + "_trainedOnComb");
  PlotnameSpecific.push_back(plotname + "_trainedOnComb_inc");

  Double_t xmin = 0.0;
  Double_t xmax = 1.0;
  Double_t ymin = 0.0;
  Double_t ymax = 1.0;

  bool overlay = true;
  bool individual = false;
  //*****************************************************************************************

  for (UInt_t i=0; i<GraphLabels.size(); ++i) {
    legend->AddEntry(ROCGraphs[i],GraphLabels[i].c_str(), "LP");

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

    if (individual) {
      cv_indiv->cd();
      legend_indiv->AddEntry(ROCGraphs[i],GraphLabels[i].c_str(), "LP");
      ROCGraphs[i]->Draw("AP");
      legend_indiv->Draw();
      StampCMS( "Internal", 140., 0.14, 0.84, 0.045 );
      cv_indiv->SaveAs((plotDir + "Minituple_ROCGraphs_" + PlotnameSpecific[i] + ".png").c_str());
      ROCGraphs[i]->GetXaxis()->SetLimits(0.00001,1); // set non-zero lower value such that a log shows the lowest signal efficiency values too (if 0, minimum is 10^-3 below maximum)
      gPad->SetLogx();
      cv_indiv->SaveAs((plotDir + "Minituple_ROCGraphs_" + PlotnameSpecific[i] + "_logx.png").c_str());

      legend_indiv->Clear();
      gPad->SetLogx(1);
    }
  }

  float signal_eff = 0;
  string mass = "";
  if (InputFile.find("MH-125_MS-15") != std::string::npos) { // mh 125 working point, for per jet approach
    signal_eff = 0.66;
    mass = "125";
  }
  if (InputFile.find("MH-350_MS-80") != std::string::npos) { // mh 350 working point, for per jet approach
    signal_eff = 0.77;
    mass = "350";
  }
  if (InputFile.find("MH-125_MS-50") != std::string::npos) { // mh 125 working point, for per jet approach
    signal_eff = 0.58;
    mass = "125";
  }
  if (InputFile.find("MH-250_MS-120") != std::string::npos) { // mh 250 working point, for per jet approach
    signal_eff = 0.65;
    mass = "250";
  }
  if (InputFile.find("MH-350_MS-160") != std::string::npos) { // mh 125 working point, for per jet approach
    signal_eff = 0.68;
    mass = "350";
  }
  TGraphAsymmErrors* WP_jet_cutflow = MakeCurrentWPSigEffVsBkgEffGraph(signal_eff , 0.0008, "WP_jet_cutflow"+label); 
  legend->AddEntry(WP_jet_cutflow, Form("Cutflow comparison (mH = %s)", mass.c_str()), "P");
  WP_jet_cutflow->SetFillColor(kBlack);
  WP_jet_cutflow->SetMarkerColor(kBlack);
  WP_jet_cutflow->SetMarkerStyle(34);
  WP_jet_cutflow->SetMarkerSize(1.5);
  WP_jet_cutflow->Draw("Psame");  

  TGraphAsymmErrors* WP_BDTcut = MakeCurrentWPSigEffVsBkgEffAtCutValueGraph(Signal_DNN_depth, Background_DNN_depth, "Eff at fixed cut", 0.999);
  legend->AddEntry(WP_BDTcut, Form("DNN depth = 0.999 point (mH = %s)", mass.c_str()), "P");
  WP_BDTcut->SetFillColor(TColor::GetPalette().At(0));
  WP_BDTcut->SetMarkerColor(TColor::GetPalette().At(0));
  WP_BDTcut->SetMarkerStyle(34);
  WP_BDTcut->SetMarkerSize(1.5);
  WP_BDTcut->Draw("Psame"); 

  TGraphAsymmErrors* WP_BDTcut_inc = MakeCurrentWPSigEffVsBkgEffAtCutValueGraph(Signal_DNN_inclusive, Background_DNN_inclusive, "Eff at fixed cut", 0.999);
  legend->AddEntry(WP_BDTcut_inc, Form("DNN inclusive = 0.999 (mH = %s)", mass.c_str()), "P");
  WP_BDTcut_inc->SetFillColor(kBlue-5);
  WP_BDTcut_inc->SetMarkerColor(kBlue-5);
  WP_BDTcut_inc->SetMarkerStyle(34);
  WP_BDTcut_inc->SetMarkerSize(1.5);
  WP_BDTcut_inc->Draw("Psame"); 

  // overlay multiple ROC curves -- useful if comparing 125 vs. 350 for instance
  if (overlay) {
    cv->cd();
    gPad->SetLogx();
    legend->Draw();
    StampCMS( "Internal", 140., 0.14, 0.84, 0.045 );
    cv->SaveAs((plotDir + "Minituple_ROCGraphs_" + plotname + "_overlay.png").c_str());
  }
  
  // separate graph -- signal efficiency vs cut value, full curve
  TGraphAsymmErrors* WP_BDTcut_sig = MakeCurrentWPSigEffVsCutValueGraph(Signal_DNN_depth, "", 0.99);
  TGraphAsymmErrors* WP_BDTcut_bkg = MakeCurrentWPSigEffVsCutValueGraph(Background_DNN_depth, "", 0.99);
  legend->Clear(); 
  WP_BDTcut_sig->SetFillColor(colors[0]);
  WP_BDTcut_sig->SetMarkerColor(colors[0]);
  WP_BDTcut_sig->SetMarkerStyle(34);
  WP_BDTcut_sig->SetMarkerSize(1.5);
  WP_BDTcut_bkg->SetFillColor(colors[1]);
  WP_BDTcut_bkg->SetMarkerColor(colors[1]);
  WP_BDTcut_bkg->SetMarkerStyle(34);
  WP_BDTcut_bkg->SetMarkerSize(1.5);
  legend->AddEntry(WP_BDTcut_sig, "Signal Efficiency", "P");
  legend->AddEntry(WP_BDTcut_bkg, "Background Efficiency", "P");
  cv_indiv->cd();
  WP_BDTcut_sig->GetXaxis()->SetLimits(-1.1,1.1);   
  WP_BDTcut_sig->GetYaxis()->SetRangeUser(0,1.1);
  gPad->Update();
  WP_BDTcut_sig->Draw("AP"); 
  WP_BDTcut_bkg->Draw("Psame"); 
  gPad->SetLogy(0);
  legend->Draw();
  StampCMS( "Internal", 140., 0.14, 0.84, 0.045 );
  // cv_indiv->SaveAs((plotDir + "SigEffVsCutValue"+label+".png").c_str());

  //*****************************************************************************************
  // Overlay signal and background BDT scores
  //*****************************************************************************************
  THStack *hs = new THStack("hs", Form("Signal and Background DNN Scores (mH=%s) ; DNN (trained on 125) Score ; Number of Entries",Label.c_str()));
  Signal_DNN_depth->SetFillColorAlpha(kBlue, 0.35);
  Background_DNN_depth->SetFillColorAlpha(kRed, 0.35);
  Signal_DNN_depth->Rebin(50);
  Background_DNN_depth->Rebin(50);
  hs->Add(Signal_DNN_depth);
  hs->Add(Background_DNN_depth);
  cv_indiv->cd();
  hs->Draw("bar1 nostack");
  gPad->BuildLegend(0.65,0.65,0.85,0.85,"");
  StampCMS( "Internal", 140., 0.2, 0.84, 0.045 );
  cv_indiv->SaveAs((plotDir + "Minituple_DNN_depth_" + plotname + ".png").c_str());
  gPad->SetLogy();
  cv_indiv->SaveAs((plotDir + "Minituple_DNN_depth_" + plotname + "_logY.png").c_str());
  cv_indiv->Clear();

  THStack *hs2 = new THStack("hs2", Form("Signal and Background DNN Scores (mH=%s) ; DNN (trained on 350) Score ; Number of Entries",Label.c_str()));
  Signal_DNN_inclusive->SetFillColorAlpha(kBlue, 0.35);
  Background_DNN_inclusive->SetFillColorAlpha(kRed, 0.35);
  Signal_DNN_inclusive->Rebin(50);
  Background_DNN_inclusive->Rebin(50);
  hs2->Add(Signal_DNN_inclusive);
  hs2->Add(Background_DNN_inclusive);
  cv_indiv->cd();
  hs2->Draw("bar1 nostack");
  gPad->BuildLegend(0.65,0.65,0.85,0.85,"");
  StampCMS( "Internal", 140., 0.2, 0.84, 0.045 );
  gPad->SetLogy(0);
  cv_indiv->SaveAs((plotDir + "Minituple_DNN_inclusive_" + plotname + ".png").c_str());
  gPad->SetLogy();
  cv_indiv->SaveAs((plotDir + "Minituple_DNN_inclusive_" + plotname + "_logY.png").c_str());
  cv_indiv->Clear();
}


//*************************************************************************************************
//*************************************************************************************************
void MakeDNNPerformancePlots_SigBkg()
{  
  SetupPlots();

  // Signals
  string SignalTree = "NoSel";

  string Signal = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_125_50_CTau3000_batch2_scores_50percent.root";
  string SigLabel = "125_mS50";

  string Signal2 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_250_120_CTau10000_batch2_scores_50percent.root";
  string SigLabel2 = "250_mS120";

  string Signal3 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_350_160_CTau10000_batch2_scores_50percent.root";
  string SigLabel3 = "350_mS160";

  string Signal4 = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_HToSSTo4b_350_80_CTau500_scores_50percent.root";
  string SigLabel4 = "350_mX80";

  // Backgrounds
  string Background = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.15/minituple_LLPskim_2023Cv4_scores_50percent.root";
  string BkgLabel = "W+Jets";
  string BackgroundTree = "NoSel";

  int Color1 = 30;
  int Color2 = 38;
  int Color3 = 48;

  vector<string> plotType = {""}; // "_HCAL12", "_HCAL34", "_calor", "_HCAL12_calor", "_HCAL34_calor", 
  for( auto type: plotType){
    if (type != "") fs::create_directory(type);
    BDTPerformancePlots(Signal, SigLabel, SignalTree, Background, BkgLabel, BackgroundTree, Color1, Color2, Color3, type);
    SetupPlots();
    BDTPerformancePlots(Signal2, SigLabel2, SignalTree, Background, BkgLabel, BackgroundTree, Color1, Color2, Color3, type);
    SetupPlots();
    BDTPerformancePlots(Signal3, SigLabel3, SignalTree, Background, BkgLabel, BackgroundTree, Color1, Color2, Color3, type);
    SetupPlots();
    BDTPerformancePlots(Signal4, SigLabel4, SignalTree, Background, BkgLabel, BackgroundTree, Color1, Color2, Color3, type);
    SetupPlots();
  }
}