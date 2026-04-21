// root -l /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Classifiers/MakeDNNTrainTestSigEffComparison.C+
//
// Compare signal efficiency vs DNN score between training and testing samples.
// Training samples: third_decimal < 8 (third decimal of jet0_Pt * 1000)
// Testing samples:  third_decimal >= 8
//
// Produces:
//   - Overlay of sig eff vs DNN cut for train and test
//   - Ratio (train / test) vs DNN cut -- used as signal efficiency systematic

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TGraphAsymmErrors.h>
#include <TBenchmark.h>
#include <TLorentzVector.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"

#include <filesystem>
namespace fs = std::filesystem;
#endif

//------------------------------------------------------------------------------
// getTreeFromFile
//------------------------------------------------------------------------------
TTree* getTreeFromFile_tt(const char* infname, const char* tname)
{
  TFile* inf = new TFile(infname, "read");
  assert(inf);
  TTree* t = (TTree*)inf->Get(tname);
  if (!t) {
    cout << "Cannot get Tree " << tname << " from file " << infname << endl;
  }
  assert(t);
  return t;
}

//------------------------------------------------------------------------------
// CMS stamp
//------------------------------------------------------------------------------
void StampCMS_tt(const std::string& approvaltext = "Internal", float x = 0.14, float y = 0.84, float textsize = 0.045) {
  TLatex *label_cms = new TLatex();
  label_cms->SetNDC();
  label_cms->SetTextColor(1);
  label_cms->SetTextSize(textsize);
  label_cms->DrawLatex(x, y, "CMS");

  TLatex *label_approvaltext = new TLatex();
  label_approvaltext->SetNDC();
  label_approvaltext->SetTextFont(52);
  label_approvaltext->SetTextColor(1);
  label_approvaltext->SetTextSize(textsize * 0.85);
  label_approvaltext->DrawLatex(x + 0.075, y, approvaltext.c_str());

  TLatex *label_energy = new TLatex();
  label_energy->SetNDC();
  label_energy->SetTextFont(42);
  label_energy->SetTextColor(1);
  label_energy->SetTextSize(textsize * 0.65);
  label_energy->DrawLatex(x+0.6, y, "#sqrt{s} = 13.6 TeV");
}

//------------------------------------------------------------------------------
// Build a TGraph of cumulative signal efficiency vs DNN cut value
// Efficiency at bin b = (entries with score >= bin b) / total
//------------------------------------------------------------------------------
TGraphAsymmErrors* MakeSigEffVsCutGraph(TH1F* hist, const string& name, Int_t color) {
  const UInt_t nBins = hist->GetXaxis()->GetNbins();

  double NSigTotal = 0;
  for (UInt_t q = 0; q < nBins + 2; ++q)
    NSigTotal += hist->GetBinContent(q);

  vector<double> cutValue(nBins), sigEff(nBins), errX(nBins, 0), errY(nBins, 0);

  for (UInt_t b = 1; b <= nBins; ++b) {
    double nsig = 0;
    for (UInt_t q = b; q < nBins + 2; ++q)
      nsig += hist->GetBinContent(q);

    cutValue[b-1] = hist->GetXaxis()->GetBinCenter(b);
    sigEff[b-1]   = (NSigTotal > 0) ? nsig / NSigTotal : 0;
  }

  TGraphAsymmErrors* g = new TGraphAsymmErrors(nBins, cutValue.data(), sigEff.data(),
                                               errX.data(), errX.data(), errY.data(), errY.data());
  g->SetName(name.c_str());
  g->SetTitle("");
  g->GetXaxis()->SetTitle("DNN Score Cut");
  g->GetYaxis()->SetTitle("Signal Efficiency");
  g->GetYaxis()->SetRangeUser(0, 1.1);
  g->GetXaxis()->SetLimits(-0.05, 1.05);
  g->GetYaxis()->SetTitleOffset(1.2);
  g->GetXaxis()->SetTitleOffset(1.05);
  g->SetLineColor(color);
  g->SetMarkerColor(color);
  g->SetMarkerSize(0.5);
  g->SetMarkerStyle(20);
  return g;
}

//------------------------------------------------------------------------------
// Build a TGraph of ratio (train / test) efficiency vs DNN cut value
// Both graphs must have been built from histograms with the same binning.
//------------------------------------------------------------------------------
TGraph* MakeTrainTestRatioGraph(TH1F* histTrain, TH1F* histTest, const string& name) {
  const UInt_t nBins = histTrain->GetXaxis()->GetNbins();
  assert(nBins == (UInt_t)histTest->GetXaxis()->GetNbins());

  double NTrainTotal = 0, NTestTotal = 0;
  for (UInt_t q = 0; q < nBins + 2; ++q) {
    NTrainTotal += histTrain->GetBinContent(q);
    NTestTotal  += histTest->GetBinContent(q);
  }

  vector<double> cutValue(nBins), ratio(nBins);

  for (UInt_t b = 1; b <= nBins; ++b) {
    double ntrain = 0, ntest = 0;
    for (UInt_t q = b; q < nBins + 2; ++q) {
      ntrain += histTrain->GetBinContent(q);
      ntest  += histTest->GetBinContent(q);
    }

    cutValue[b-1] = histTrain->GetXaxis()->GetBinCenter(b);

    double effTrain = (NTrainTotal > 0) ? ntrain / NTrainTotal : 0;
    double effTest  = (NTestTotal  > 0) ? ntest  / NTestTotal  : 0;

    ratio[b-1] = (effTest > 0) ? effTrain / effTest : 1.0; // default to 1 when both are 0
  }

  TGraph* g = new TGraph(nBins, cutValue.data(), ratio.data());
  g->SetName(name.c_str());
  g->SetTitle("");
  g->GetXaxis()->SetTitle("DNN Score Cut");
  g->GetYaxis()->SetTitle("Efficiency Ratio (Train / Test)");
  g->GetXaxis()->SetLimits(-0.05, 1.05);
  g->GetYaxis()->SetTitleOffset(1.2);
  g->GetXaxis()->SetTitleOffset(1.05);
  g->SetLineColor(kBlack);
  g->SetMarkerColor(kBlack);
  g->SetMarkerSize(0.5);
  g->SetMarkerStyle(20);
  return g;
}

//------------------------------------------------------------------------------
// Print efficiency ratio at a specific DNN cut value
//------------------------------------------------------------------------------
void PrintEfficiencyAtCut(TH1F* histTrain, TH1F* histTest, Double_t cutVal, const string& label) {
  const UInt_t nBins = histTrain->GetXaxis()->GetNbins();
  double NTrainTotal = 0, NTestTotal = 0;
  for (UInt_t q = 0; q < nBins + 2; ++q) {
    NTrainTotal += histTrain->GetBinContent(q);
    NTestTotal  += histTest->GetBinContent(q);
  }

  double ntrain = 0, ntest = 0;
  for (UInt_t b = 1; b <= nBins; ++b) {
    if (histTrain->GetXaxis()->GetBinCenter(b) < cutVal) continue;
    for (UInt_t q = b; q < nBins + 2; ++q) {
      ntrain += histTrain->GetBinContent(q);
      ntest  += histTest->GetBinContent(q);
    }
    break;
  }

  double effTrain = (NTrainTotal > 0) ? ntrain / NTrainTotal : 0;
  double effTest  = (NTestTotal  > 0) ? ntest  / NTestTotal  : 0;
  double r        = (effTest > 0)     ? effTrain / effTest   : -1;

  cout << label << " | DNN cut=" << cutVal
       << " | eff(train)=" << effTrain
       << " | eff(test)="  << effTest
       << " | ratio(train/test)=" << r << endl;
}

//------------------------------------------------------------------------------
// Main per-sample function
//------------------------------------------------------------------------------
void TrainTestComparison(string InputFile, string Label, string SigTree, string plotDir)
{
  // ---------------------------------------------------------------
  // Histograms -- fine binning so efficiency curve is smooth
  // ---------------------------------------------------------------
  const int nBins = 1100;
  const double xLow = -0.1, xHigh = 1.1;

  TH1F *hDepth_train = new TH1F(("hDepth_train_"+Label).c_str(), "", nBins, xLow, xHigh);
  TH1F *hDepth_test  = new TH1F(("hDepth_test_" +Label).c_str(), "", nBins, xLow, xHigh);
  TH1F *hIncl_train  = new TH1F(("hIncl_train_" +Label).c_str(), "", nBins, xLow, xHigh);
  TH1F *hIncl_test   = new TH1F(("hIncl_test_"  +Label).c_str(), "", nBins, xLow, xHigh);

  hDepth_train->Sumw2(); hDepth_test->Sumw2();
  hIncl_train->Sumw2();  hIncl_test->Sumw2();

  // ---------------------------------------------------------------
  // Signal tree
  // ---------------------------------------------------------------
  TTree *tree_sig = getTreeFromFile_tt(InputFile.c_str(), SigTree.c_str());

  float jet0_Pt, perJet_MatchedLLP_DecayR, perJet_MatchedLLP_Eta;
  float score_depth, score_incl;

  const float radius_HB1  = 177.0f;
  const float radius_HBend = 295.0f;
  const float HBeta = 1.26f;

  bool isPerJet = (SigTree.find("PerJet") != std::string::npos);

  TCut SignalSel, SignalSel_inc;
  if (!isPerJet) {
    float jet0_MatchedLLP_DecayR, jet0_Eta, jet0_isTruthMatched;
    bool jet0_DepthTagCand, jet0_InclTagCand;
    SignalSel     = Form("jet0_DepthTagCand && jet0_isTruthMatched == 1 && (jet0_MatchedLLP_DecayR >= %f && jet0_MatchedLLP_DecayR < %f) && (abs(jet0_Eta) < 1.26 && jet0_Pt > 40)", radius_HB1, radius_HBend);
    SignalSel_inc = Form("jet0_InclTagCand  && jet0_isTruthMatched == 1 && (jet0_MatchedLLP_DecayR < %f) && (abs(jet0_Eta) < 1.26 && jet0_Pt > 40)", radius_HBend);
  } else {
    SignalSel     = Form("perJet_MatchedLLP_DecayR >= %f && perJet_MatchedLLP_DecayR < %f && abs(perJet_MatchedLLP_Eta) < %f", radius_HB1, radius_HBend, HBeta);
    SignalSel_inc = Form("perJet_MatchedLLP_DecayR < %f && abs(perJet_MatchedLLP_Eta) < %f", radius_HBend, HBeta);
  }

  TFile *tmpFile = new TFile("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/Run3-HCAL-LLP-Analysis/Classifiers/temp_traintest.root", "RECREATE");
  TTree *tree_depth = tree_sig->CopyTree(SignalSel,     "", tree_sig->GetEntries(), 0);
  TTree *tree_incl  = tree_sig->CopyTree(SignalSel_inc, "", tree_sig->GetEntries(), 0);

  if (!isPerJet) {
    tree_depth->SetBranchAddress("jet0_Pt",                &jet0_Pt);
    tree_depth->SetBranchAddress("jet0_scores_depth_LLPanywhere", &score_depth); // jet0_scores_depth_hcal
    tree_incl->SetBranchAddress("jet0_Pt",                 &jet0_Pt);
    tree_incl->SetBranchAddress("jet0_scores_inc_train80", &score_incl);
  } else {
    tree_depth->SetBranchAddress("jet0_Pt",    &jet0_Pt);
    tree_depth->SetBranchAddress("scores",     &score_depth);
    tree_incl->SetBranchAddress("jet0_Pt",     &jet0_Pt);
    tree_incl->SetBranchAddress("scores_inc",  &score_incl);
  }

  // ---------------------------------------------------------------
  // Fill depth histograms
  // ---------------------------------------------------------------
  cout << "Processing depth signal (" << Label << "): " << tree_depth->GetEntries() << " entries" << endl;
  for (int i = 0; i < tree_depth->GetEntries(); ++i) {
    tree_depth->GetEntry(i);
    if (i % 100000 == 0) cout << "  depth event " << i << endl;
    int third_decimal = static_cast<int>(std::floor(jet0_Pt * 1000)) % 10;
    if (third_decimal < 8)  hDepth_train->Fill(score_depth);
    else                    hDepth_test->Fill(score_depth);
  }

  // ---------------------------------------------------------------
  // Fill inclusive histograms
  // ---------------------------------------------------------------
  cout << "Processing inclusive signal (" << Label << "): " << tree_incl->GetEntries() << " entries" << endl;
  for (int i = 0; i < tree_incl->GetEntries(); ++i) {
    tree_incl->GetEntry(i);
    if (i % 100000 == 0) cout << "  inclusive event " << i << endl;
    int third_decimal = static_cast<int>(std::floor(jet0_Pt * 1000)) % 10;
    if (third_decimal < 8)  hIncl_train->Fill(score_incl);
    else                    hIncl_test->Fill(score_incl);
  }

  tmpFile->Close();

  cout << "Train/test counts -- depth:    train=" << hDepth_train->Integral()
       << "  test=" << hDepth_test->Integral() << endl;
  cout << "Train/test counts -- inclusive: train=" << hIncl_train->Integral()
       << "  test=" << hIncl_test->Integral() << endl;

  // ---------------------------------------------------------------
  // Print efficiency ratios at working points
  // ---------------------------------------------------------------
  cout << "\n=== Depth DNN (" << Label << ") ===" << endl;
  PrintEfficiencyAtCut(hDepth_train, hDepth_test, 0.90, Label+"_depth");
  PrintEfficiencyAtCut(hDepth_train, hDepth_test, 0.95, Label+"_depth");
  PrintEfficiencyAtCut(hDepth_train, hDepth_test, 0.99, Label+"_depth");

  cout << "\n=== Inclusive DNN (" << Label << ") ===" << endl;
  PrintEfficiencyAtCut(hIncl_train, hIncl_test, 0.90, Label+"_incl");
  PrintEfficiencyAtCut(hIncl_train, hIncl_test, 0.95, Label+"_incl");
  PrintEfficiencyAtCut(hIncl_train, hIncl_test, 0.99, Label+"_incl");

  // ---------------------------------------------------------------
  // Build efficiency and ratio graphs
  // ---------------------------------------------------------------
  TGraphAsymmErrors* gDepth_train = MakeSigEffVsCutGraph(hDepth_train, "gDepth_train_"+Label, kBlue+1);
  TGraphAsymmErrors* gDepth_test  = MakeSigEffVsCutGraph(hDepth_test,  "gDepth_test_" +Label, kRed+1);
  TGraph*            gDepth_ratio = MakeTrainTestRatioGraph(hDepth_train, hDepth_test, "gDepth_ratio_"+Label);

  TGraphAsymmErrors* gIncl_train  = MakeSigEffVsCutGraph(hIncl_train, "gIncl_train_"+Label, kBlue+1);
  TGraphAsymmErrors* gIncl_test   = MakeSigEffVsCutGraph(hIncl_test,  "gIncl_test_" +Label, kRed+1);
  TGraph*            gIncl_ratio  = MakeTrainTestRatioGraph(hIncl_train, hIncl_test, "gIncl_ratio_"+Label);

  // ---------------------------------------------------------------
  // Plot: Depth DNN -- efficiency overlay + ratio
  // ---------------------------------------------------------------
  {
    TCanvas* cv = new TCanvas(("cv_depth_"+Label).c_str(), "", 800, 800);
    cv->Divide(1, 2);

    // Top pad: efficiency overlay
    cv->cd(1);
    gPad->SetPad(0, 0.3, 1, 1.0);
    gPad->SetBottomMargin(0.02);
    gPad->SetTopMargin(0.10);

    gDepth_train->GetXaxis()->SetLabelSize(0);
    gDepth_train->GetXaxis()->SetTitleSize(0);
    gDepth_train->GetYaxis()->SetTitle("Signal Efficiency");
    gDepth_train->Draw("AP");
    gDepth_test->Draw("Psame");

    TLegend* leg = new TLegend(0.12, 0.15, 0.60, 0.40);
    leg->SetBorderSize(0); leg->SetFillStyle(0); leg->SetTextSize(0.04);
    leg->AddEntry(gDepth_train, Form("Train (3rd dec. < 8) -- %s", Label.c_str()), "LP");
    leg->AddEntry(gDepth_test,  Form("Test  (3rd dec. #geq 8) -- %s", Label.c_str()), "LP");
    leg->Draw();
    StampCMS_tt("Internal", 0.14, 0.92);

    // Bottom pad: ratio
    cv->cd(2);
    gPad->SetPad(0, 0.0, 1, 0.3);
    gPad->SetTopMargin(0.03);
    gPad->SetBottomMargin(0.30);

    gDepth_ratio->GetYaxis()->SetRangeUser(0.8, 1.35);
    gDepth_ratio->GetXaxis()->SetTitle("Depth DNN Score Cut");
    gDepth_ratio->GetYaxis()->SetTitle("Train / Test");
    gDepth_ratio->GetYaxis()->SetNdivisions(505);
    gDepth_ratio->GetYaxis()->SetTitleSize(0.12);
    gDepth_ratio->GetYaxis()->SetLabelSize(0.10);
    gDepth_ratio->GetXaxis()->SetTitleSize(0.12);
    gDepth_ratio->GetXaxis()->SetLabelSize(0.10);
    gDepth_ratio->GetYaxis()->SetTitleOffset(0.45);
    gDepth_ratio->Draw("AP");

    TLine* line1 = new TLine(-0.05, 1.0, 1.05, 1.0);
    line1->SetLineStyle(2); line1->SetLineColor(kGray+1);
    line1->Draw();

    cv->SaveAs((plotDir + "TrainTest_SigEff_depth_" + Label + ".png").c_str());
    cv->SaveAs((plotDir + "TrainTest_SigEff_depth_" + Label + ".pdf").c_str());
    delete cv;
  }

  // ---------------------------------------------------------------
  // Plot: Inclusive DNN -- efficiency overlay + ratio
  // ---------------------------------------------------------------
  {
    TCanvas* cv = new TCanvas(("cv_incl_"+Label).c_str(), "", 800, 800);
    cv->Divide(1, 2);

    cv->cd(1);
    gPad->SetPad(0, 0.3, 1, 1.0);
    gPad->SetBottomMargin(0.02);
    gPad->SetTopMargin(0.10);

    gIncl_train->GetXaxis()->SetLabelSize(0);
    gIncl_train->GetXaxis()->SetTitleSize(0);
    gIncl_train->GetYaxis()->SetTitle("Signal Efficiency");
    gIncl_train->Draw("AP");
    gIncl_test->Draw("Psame");

    TLegend* leg = new TLegend(0.12, 0.15, 0.60, 0.40);
    leg->SetBorderSize(0); leg->SetFillStyle(0); leg->SetTextSize(0.04);
    leg->AddEntry(gIncl_train, Form("Train (3rd dec. < 8) -- %s", Label.c_str()), "LP");
    leg->AddEntry(gIncl_test,  Form("Test  (3rd dec. #geq 8) -- %s", Label.c_str()), "LP");
    leg->Draw();
    StampCMS_tt("Internal", 0.14, 0.92);

    cv->cd(2);
    gPad->SetPad(0, 0.0, 1, 0.3);
    gPad->SetTopMargin(0.03);
    gPad->SetBottomMargin(0.30);

    gIncl_ratio->GetYaxis()->SetRangeUser(0.8, 1.35);
    gIncl_ratio->GetXaxis()->SetTitle("Inclusive DNN Score Cut");
    gIncl_ratio->GetYaxis()->SetTitle("Train / Test");
    gIncl_ratio->GetYaxis()->SetNdivisions(505);
    gIncl_ratio->GetYaxis()->SetTitleSize(0.12);
    gIncl_ratio->GetYaxis()->SetLabelSize(0.10);
    gIncl_ratio->GetXaxis()->SetTitleSize(0.12);
    gIncl_ratio->GetXaxis()->SetLabelSize(0.10);
    gIncl_ratio->GetYaxis()->SetTitleOffset(0.45);
    gIncl_ratio->Draw("AP");

    TLine* line1 = new TLine(-0.05, 1.0, 1.05, 1.0);
    line1->SetLineStyle(2); line1->SetLineColor(kGray+1);
    line1->Draw();

    cv->SaveAs((plotDir + "TrainTest_SigEff_incl_" + Label + ".png").c_str());
    cv->SaveAs((plotDir + "TrainTest_SigEff_incl_" + Label + ".pdf").c_str());
    delete cv;
  }

  // ---------------------------------------------------------------
  // Save graphs to ROOT file for downstream use
  // ---------------------------------------------------------------
  TFile* outFile = new TFile((plotDir + "TrainTest_SigEff_" + Label + ".root").c_str(), "RECREATE");
  gDepth_train->Write(); gDepth_test->Write(); gDepth_ratio->Write();
  gIncl_train->Write();  gIncl_test->Write();  gIncl_ratio->Write();
  hDepth_train->Write(); hDepth_test->Write();
  hIncl_train->Write();  hIncl_test->Write();
  outFile->Close();

  cout << "\nOutput saved to: " << plotDir << "TrainTest_SigEff_" << Label << ".{root,png,pdf}" << endl;
}

//------------------------------------------------------------------------------
// Entry point
//------------------------------------------------------------------------------
void MakeDNNTrainTestSigEffComparison()
{
  string plotDir = "TrainTest/";
  fs::create_directories(plotDir);

  string SignalTree = "NoSel";

  vector<pair<string,string>> samples = { // initialy was with 3.16 minituples, now 5.3 with new DNN scores
    {"/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_HToSSTo4B_125_50_CTau3000_scores.root",   "125_mS50"},
    {"/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_HToSSTo4B_250_120_CTau10000_scores.root", "250_mS120"},
    {"/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_HToSSTo4B_350_160_CTau10000_scores.root", "350_mS160"},
    {"/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v5.3/minituple_HToSSTo4B_350_80_CTau500_scores.root",    "350_mX80"},
  };

  for (auto& [file, label] : samples) {
    cout << "\n========================================" << endl;
    cout << "Processing sample: " << label << endl;
    cout << "========================================" << endl;
    TrainTestComparison(file, label, SignalTree, plotDir);
  }
}
