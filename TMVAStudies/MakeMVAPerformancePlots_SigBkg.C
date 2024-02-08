// root -l /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/MakeMVAPerformancePlots_SigBkg.C+'("/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.1/minituple_v3.1_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_01_20_TEST.root","125",30,"/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.1/minituple_v3.1_LLPskim_Run2023Cv3_2024_01_20.root","W+Jets",38)'
// inputs are minituple signal file, label, color, minituple background file, label, color
// root -l /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/MakeMVAPerformancePlots_SigBkg.C+'()'

// void MakeMVAPerformancePlots_SigBkg(string InputFile, string Label, Int_t Option, string InputFile2, string Label2, Int_t Option2)
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
// return a TGraphAsymmErrors, sig vs bkg
TGraphAsymmErrors* MakeSigEffVsBkgEffGraph(TH1F* signalHist, TH1F* bkgHist, string name, string Label ) {
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
  tmpSigEffVsBkgEff->SetTitle(Form("ROC Curve for LLP (mh=%s) vs. W+jets", Label.c_str()));
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
  //Make Met Plots
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

  legend = new TLegend(0.32,0.14,0.9,0.34);
  legend->SetTextSize(0.03);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);

  cv = new TCanvas("cv", "cv", 800, 600);
}

//*************************************************************************************************
//*************************************************************************************************
void BDTPerformancePlots(string InputFile, string Label, string SigTree, string InputFile2, string Label2, string BkgTree, Int_t Option, Int_t Option2, Int_t Option3)
{  
  string label = "";
  if (Label != "") label = "_" + Label;

  //--------------------------------------------------------------------------------------------------------------
  // Histograms
  //==============================================================================================================  
  TH1F *Signal_MVA125 = new TH1F(("Signal_MVA125"+label).c_str(), "LLP Signal ; MVA (BDT trained on 125) score for LLP signal ; Number of Events ",  5500, -1.1 , 1.1);
  TH1F *Signal_MVA350 = new TH1F(("Signal_MVA350"+label).c_str(), "LLP Signal ; MVA (BDT trained on 350) score for LLP signal ; Number of Events ",  5500, -1.1 , 1.1);
  TH1F *Signal_MVAhadd = new TH1F(("Signal_MVAhadd"+label).c_str(), "LLP Signal ; MVA (BDT trained on combination) score for LLP signal ; Number of Events ",  5500, -1.1 , 1.1);
  TH1F *Background_MVA125 = new TH1F(("Background_MVA125"+label).c_str(), "W+Jets Background ; MVA (BDT trained on 125) score for W+jets background ; Number of Events ",  5500, -1.1 , 1.1);
  TH1F *Background_MVA350 = new TH1F(("Background_MVA350"+label).c_str(), "W+Jets Background ; MVA (BDT trained on 350) score for W+jets background ; Number of Events ",  5500, -1.1 , 1.1);
  TH1F *Background_MVAhadd = new TH1F(("Background_MVAhadd"+label).c_str(), "W+Jets Background ; MVA (BDT trained on combination) score for W+jets background ; Number of Events ",  5500, -1.1 , 1.1);

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

  float score125_sig, score350_sig, scoreHadd_sig;

  float radius_HB1 = 183.6;
  float radius_HB2 = 190.2;
  float radius_HB3 = 214.2;
  float radius_HB4 = 244.8;
  float radius_HBend = 295;
  float HBeta = 1.26;

  TCut SignalSelection = "";
  if (SigTree.find("PerJet") == std::string::npos) {
    tree_sig->SetBranchAddress("LLP0_DecayR", &LLP0_DecayR);
    tree_sig->SetBranchAddress("LLP1_DecayR", &LLP1_DecayR);
    tree_sig->SetBranchAddress("LLP0_Eta", &LLP0_Eta);
    tree_sig->SetBranchAddress("LLP1_Eta", &LLP1_Eta);
    tree_sig->SetBranchAddress("jet0_isMatchedTo", &jet0_isMatchedTo);
    tree_sig->SetBranchAddress("jet0_Eta", &jet0_Eta);
    tree_sig->SetBranchAddress("jet0_Pt", &jet0_Pt);
    SignalSelection = Form("((LLP0_DecayR >= %f && LLP0_DecayR < %f && abs(LLP0_Eta) <= %f && jet0_isMatchedTo == 0) || (LLP1_DecayR >= %f && LLP1_DecayR < %f && abs(LLP1_Eta) <= %f && jet0_isMatchedTo == 1)) && (abs(jet0_Eta) < 1.26 && jet0_Pt > 40)",radius_HB1, radius_HBend, HBeta, radius_HB1, radius_HBend, HBeta);
    cout << "per event tree, adding signal region cuts on LLP position and matching" << endl;
    // hopefully there is a better way to implement these cuts, had TCut errors when imported RegionCuts.h on first try
    // reduces by a factor of about 10 for mh=125
  }
  if (SigTree.find("PerJet") != std::string::npos) {
     tree_sig->SetBranchAddress("perJet_MatchedLLP_DecayR", &perJet_MatchedLLP_DecayR);
    tree_sig->SetBranchAddress("perJet_MatchedLLP_Eta", &perJet_MatchedLLP_Eta);
    SignalSelection = "perJet_MatchedLLP_DecayR >= 183.6 && perJet_MatchedLLP_DecayR < 295 && abs(perJet_MatchedLLP_Eta) < 1.26";
    cout << "per jet tree, adding signal region cuts on LLP position" << endl; // this tree already requires jet pT > 40 and eta < 1.26
  }

  TFile *myReducedFile = new TFile("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/TMVAStudies/temp.root", "RECREATE"); // preventing error "This error is symptomatic of a Tree created as a memory-resident Tree"
  TTree *tree_sig_reduced = tree_sig->CopyTree(SignalSelection, "", tree_sig->GetEntries(), 0); // no cuts needed for already jet matched tree

  if (SigTree.find("PerJet") == std::string::npos) {
    tree_sig_reduced->SetBranchAddress("bdtscore_LLP125", &score125_sig);
    tree_sig_reduced->SetBranchAddress("bdtscore_LLP350", &score350_sig);
    tree_sig_reduced->SetBranchAddress("bdtscore_hadd", &scoreHadd_sig);
  }
  if (SigTree.find("PerJet") != std::string::npos) {
    tree_sig_reduced->SetBranchAddress("bdtscore_LLP125_perJet", &score125_sig);
    tree_sig_reduced->SetBranchAddress("bdtscore_LLP350_perJet", &score350_sig);
    tree_sig_reduced->SetBranchAddress("bdtscore_hadd_perJet", &scoreHadd_sig);
  }

  cout << "Total Entries (signal): " << tree_sig_reduced->GetEntries() << "\n";
  int nentries = tree_sig_reduced->GetEntries();
  for(int ientry=0; ientry < nentries; ientry++) {       	
    tree_sig_reduced->GetEntry(ientry);
    
    if (ientry % 100000 == 0) cout << "Event " << ientry << endl;
        
    //don't evaluate performance using training events
    //classify by eta and pt bins
    //Some Preselection cuts -- here is where we specify the signal region based on LLP truth information 
  
    // eventually need to fill with weights
	  Signal_MVA125->Fill(score125_sig);
	  Signal_MVA350->Fill(score350_sig);
	  Signal_MVAhadd->Fill(scoreHadd_sig);
  } 

  //*****************************************************************************************
  // Get background distribution
  //*****************************************************************************************
  TTree *tree_bkg = getTreeFromFile(InputFile2.c_str(), BkgTree.c_str()); 
  
  float score125_bkg;
  float score350_bkg;
  float scoreHadd_bkg;
  TCut BackgroundSelection = "";

  if (SigTree.find("PerJet") == std::string::npos) {
    tree_bkg->SetBranchAddress("bdtscore_LLP125", &score125_bkg);
    tree_bkg->SetBranchAddress("bdtscore_LLP350", &score350_bkg);
    tree_bkg->SetBranchAddress("bdtscore_hadd", &scoreHadd_bkg);

    tree_bkg->SetBranchAddress("jet0_Eta", &jet0_Eta);
    tree_bkg->SetBranchAddress("jet0_Pt", &jet0_Pt);  
    BackgroundSelection = "abs(jet0_Eta) < 1.26 && jet0_Pt > 40";
  }
  if (SigTree.find("PerJet") != std::string::npos) {
    tree_bkg->SetBranchAddress("bdtscore_LLP125_perJet", &score125_bkg);
    tree_bkg->SetBranchAddress("bdtscore_LLP350_perJet", &score350_bkg);
    tree_bkg->SetBranchAddress("bdtscore_hadd_perJet", &scoreHadd_bkg);
  }

  cout << "Total Entries (background): " << tree_bkg->GetEntries() << "\n";
  int nentries_bkg = tree_bkg->GetEntries();
  for(int ientry=0; ientry < nentries_bkg; ientry++) {       	
    tree_bkg->GetEntry(ientry);
    
    if (ientry % 100000 == 0) cout << "Event " << ientry << endl;

    // eventually need to fill with weights
	  Background_MVA125->Fill(score125_bkg);
	  Background_MVA350->Fill(score350_bkg);
    Background_MVAhadd->Fill(scoreHadd_bkg);
  } 

  //*****************************************************************************************
  // Current Working Points
  //*****************************************************************************************
  cout << "Efficiencies for label = " << Label << ", for BDT trained on 125" << endl;
  cout << "Signal efficiency at background of 0.001  : " << FindSigEffAtFixedBkgEfficiency(Signal_MVA125, Background_MVA125, 0.001) <<  endl;
  cout << "Signal efficiency at background of 0.0001  : " << FindSigEffAtFixedBkgEfficiency(Signal_MVA125, Background_MVA125, 0.0001) <<  endl;
  cout << "Signal efficiency at background of 0.00001  : " << FindSigEffAtFixedBkgEfficiency(Signal_MVA125, Background_MVA125, 0.00001) <<  endl;
  cout << "Background efficiency at signal of 80% : " << FindBkgEffAtFixedSignalEfficiency(Signal_MVA125, Background_MVA125, 0.8) << endl;
  cout << "Background efficiency at signal of 70% : " << FindBkgEffAtFixedSignalEfficiency(Signal_MVA125, Background_MVA125, 0.7) << endl;
  cout << "Background efficiency at signal of 60% : " << FindBkgEffAtFixedSignalEfficiency(Signal_MVA125, Background_MVA125, 0.6) << endl;

  cout << "Efficiencies for label = " << Label << ", for BDT trained on 350" << endl;
  cout << "Signal efficiency at background of 0.001  : " << FindSigEffAtFixedBkgEfficiency(Signal_MVA350, Background_MVA350, 0.001) <<  endl;
  cout << "Signal efficiency at background of 0.0001  : " << FindSigEffAtFixedBkgEfficiency(Signal_MVA350, Background_MVA350, 0.0001) <<  endl;
  cout << "Signal efficiency at background of 0.00001  : " << FindSigEffAtFixedBkgEfficiency(Signal_MVA350, Background_MVA350, 0.00001) <<  endl;
  cout << "Background efficiency at signal of 80% : " << FindBkgEffAtFixedSignalEfficiency(Signal_MVA350, Background_MVA350, 0.8) << endl;
  cout << "Background efficiency at signal of 70% : " << FindBkgEffAtFixedSignalEfficiency(Signal_MVA350, Background_MVA350, 0.7) << endl;
  cout << "Background efficiency at signal of 60% : " << FindBkgEffAtFixedSignalEfficiency(Signal_MVA350, Background_MVA350, 0.6) << endl;

  //*****************************************************************************************
  //Find Cut with same signal efficiency
  //*****************************************************************************************
  cout << "MVA125 Cut Value at 50% Sig Eff: " << FindCutValueAtFixedEfficiency(Signal_MVA125, 0.5 ) << endl;
  cout << "MVA125 Cut Value at 50% Bkg Eff: " << FindCutValueAtFixedEfficiency(Background_MVA125, 0.5 ) << endl;
  cout << "MVA350 Cut Value at 50% Sig Eff: " << FindCutValueAtFixedEfficiency(Signal_MVA350, 0.5 ) << endl;
  cout << "MVA350 Cut Value at 50% Bkg Eff: " << FindCutValueAtFixedEfficiency(Background_MVA350, 0.5 ) << endl;

   //*****************************************************************************************
   // Make ROC curves
   //*****************************************************************************************
   // return a vector of TGraphAsymmErrors, sig vs bkg, sig vs bkg rej, sig vs inverse bkg eff
   TGraphAsymmErrors* ROC125_sigEffBkgEff = MakeSigEffVsBkgEffGraph(Signal_MVA125, Background_MVA125, "ROC_MVA125_LLP_W+jets"+label, Label);
   TGraphAsymmErrors* ROC350_sigEffBkgEff = MakeSigEffVsBkgEffGraph(Signal_MVA350, Background_MVA350, "ROC_MVA350_LLP_W+jets"+label, Label );
   TGraphAsymmErrors* ROChadd_sigEffBkgEff = MakeSigEffVsBkgEffGraph(Signal_MVAhadd, Background_MVAhadd, "ROC_MVAhadd_LLP_W+jets"+label, Label );

  TLegend* legend_indiv;
  TCanvas* cv_indiv;
  string plotname;

  cv_indiv = new TCanvas("cv_indiv", "cv_indiv", 800, 600);

  legend_indiv = new TLegend(0.5,0.14,0.94,0.44);
  legend_indiv->SetTextSize(0.03);
  legend_indiv->SetBorderSize(0);
  legend_indiv->SetFillStyle(0);

  plotname = Label+"_LLP_WJets_MVA";

  //*****************************************************************************************
  // Overlay signal and background BDT scores
  //*****************************************************************************************
  THStack *hs = new THStack("hs", Form("Signal and Background BDT Scores (mH=%s) ; BDT (trained on 125) Score ; Number of Entries",Label.c_str()));
  Signal_MVA125->SetFillColor(kBlue);
  Background_MVA125->SetFillColor(kRed);
  Signal_MVA125->Rebin(20);
  Background_MVA125->Rebin(20);
  hs->Add(Signal_MVA125);
  hs->Add(Background_MVA125);
  cv_indiv->cd();
  hs->Draw("bar1 nostack");
  gPad->BuildLegend(0.65,0.65,0.85,0.85,"");
  cv_indiv->SaveAs(("Minituple_BDT125score_" + plotname + ".png").c_str());
  gPad->SetLogy();
  cv_indiv->SaveAs(("Minituple_BDT125score_" + plotname + "_logY.png").c_str());
  cv_indiv->Clear();

  //*****************************************************************************************
  //Plot ROC Curves
  //*****************************************************************************************

  ROCGraphs.push_back(ROC125_sigEffBkgEff);
  ROCGraphs.push_back(ROC350_sigEffBkgEff);
  ROCGraphs.push_back(ROChadd_sigEffBkgEff);
  GraphLabels.push_back("MVA trained on 125 for LLP vs. W+jets");
  GraphLabels.push_back("MVA trained on 350 for LLP vs. W+jets");
  GraphLabels.push_back("MVA trained on combination for LLP vs. W+jets");
  colors.push_back(Option);
  colors.push_back(Option2);
  colors.push_back(Option3);
  PlotnameSpecific.push_back(plotname + "_trainedOn125");
  PlotnameSpecific.push_back(plotname + "_trainedOn350");
  PlotnameSpecific.push_back(plotname + "_trainedOnComb");

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
      cv_indiv->SaveAs(("Minituple_ROCGraphs_" + PlotnameSpecific[i] + ".png").c_str());
      ROCGraphs[i]->GetXaxis()->SetLimits(0.00001,1); // set non-zero lower value such that a log shows the lowest signal efficiency values too (if 0, minimum is 10^-3 below maximum)
      gPad->SetLogx();
      cv_indiv->SaveAs(("Minituple_ROCGraphs_" + PlotnameSpecific[i] + "_logx.png").c_str());

      legend_indiv->Clear();
      gPad->SetLogx(1);
    }
  }

  // TGraphAsymmErrors* WP_event_cutflow = MakeCurrentWPSigEffVsBkgEffGraph(0.66 , 0.0009, "WP_event_cutflow"+label); // mh 125 working point, for per event approach
  // legend->AddEntry(WP_event_cutflow, "Event Cutflow WP", "P");
  // WP_event_cutflow->SetFillColor(kBlue);
  // WP_event_cutflow->SetMarkerColor(kBlue);
  // WP_event_cutflow->SetMarkerStyle(34);
  // WP_event_cutflow->SetMarkerSize(2.5);
  // WP_event_cutflow->Draw("Psame");  

  TGraphAsymmErrors* WP_jet_cutflow = MakeCurrentWPSigEffVsBkgEffGraph(0.66 , 0.0008, "WP_jet_cutflow"+label); // mh 125 working point, for per jet approach
  legend->AddEntry(WP_jet_cutflow, "Jet Cutflow WP", "P");
  WP_jet_cutflow->SetFillColor(kBlue);
  WP_jet_cutflow->SetMarkerColor(kBlue);
  WP_jet_cutflow->SetMarkerStyle(34);
  WP_jet_cutflow->SetMarkerSize(2.5);
  WP_jet_cutflow->Draw("Psame");  

  // overlay multiple ROC curves -- useful if comparing 125 vs. 350 for instance
  if (overlay) {
    cv->cd();
    gPad->SetLogx();
    legend->Draw();
    cv->SaveAs(("Minituple_ROCGraphs_" + plotname + "_overlay.png").c_str());
  }
} 

//*************************************************************************************************
//*************************************************************************************************
void MakeMVAPerformancePlots_SigBkg()
{  
  SetupPlots();

  string Signal = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_02_06_TEST.root";
  // string SigLabel = "125";
  // string SignalTree = "NoSel";
  string SigLabel = "125_perJet";
  string SignalTree = "PerJet_LLPmatched";

  // string Signal = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_02_06_TEST.root";
  // string SigLabel = "350";
  // string SignalTree = "NoSel";
  // string SigLabel = "350_perJet";
  // string SignalTree = "PerJet_LLPmatched";

  string Background = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.3/minituple_v3.3_LLPskim_Run2023_hadd_TEST.root";
  // string BkgLabel = "W+Jets";
  // string BackgroundTree = "WPlusJets";
  string BkgLabel = "W+Jets_perJet";
  string BackgroundTree = "PerJet_WPlusJets";

  int Color1 = 30;
  int Color2 = 38;
  int Color3 = 48;

  BDTPerformancePlots(Signal, SigLabel, SignalTree, Background, BkgLabel, BackgroundTree, Color1, Color2, Color3);
}