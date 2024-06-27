#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
 
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
 
 
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/TMVAMultiClassGui.h"
 
 
using namespace TMVA;
 
void runClassification( TString dir, TString sigTagList_train, TString sigTagList_test, TString bkgTag1, TString bkgTag2, TString bkgTag3, TString bkgTag4, TString bkgTag5, TString bkgTag6, TString bkgTag7, TString tag = "" )
{
   // This loads the library
   TMVA::Tools::Instance();
 
   // to get access to the GUI and all tmva macros
   //
   //     TString tmva_dir(TString(gRootDir) + "/tmva");
   //     if(gSystem->Getenv("TMVASYS"))
   //        tmva_dir = TString(gSystem->Getenv("TMVASYS"));
   //     gROOT->SetMacroPath(tmva_dir + "/test/:" + gROOT->GetMacroPath() );
   //     gROOT->ProcessLine(".L TMVAMultiClassGui.C");
 
 
   //---------------------------------------------------------------
   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;
   Use["MLP"]             = 0;
   Use["BDTG"]            = 1; // BDT with gradient boosting
#ifdef R__HAS_TMVAGPU
   Use["DL_CPU"]          = 0;
   Use["DL_GPU"]          = 0;
#else
   Use["DL_CPU"]          = 0;
   Use["DL_GPU"]          = 0;
#endif
   Use["FDA_GA"]          = 0;
   Use["PDEFoam"]         = 0;
 
   //---------------------------------------------------------------
 
   std::cout << std::endl;
   std::cout << "==> Start TMVAMulticlass" << std::endl;
 
   // Create a new root output file.
   TString outfileName = ( "BDTWeightFilesMulticlassifier/TMVAMulticlass_" + tag + ".root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
 
   TMVA::Factory *factory = new TMVA::Factory( "TMVAMulticlass", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=multiclass" );
   TMVA::DataLoader *dataloader=new TMVA::DataLoader("BDTWeightFilesMulticlassifier");
   (TMVA::gConfig().GetIONames()).fWeightFileDir = "weights_"+tag; // save each weight file in a separate directory

   // Define input variables for BDT training.
   dataloader->AddVariable( "perJet_Eta", "perJet_Eta", "", 'F' );
   dataloader->AddVariable( "perJet_ChargedHadEFrac", "perJet_ChargedHadEFrac", "", 'F' );
   dataloader->AddVariable( "perJet_NeutralHadEFrac", "perJet_NeutralHadEFrac", "", 'F' );
   dataloader->AddVariable( "perJet_PhoEFrac", "perJet_PhoEFrac", "", 'F' );
   dataloader->AddVariable( "perJet_EleEFrac", "perJet_EleEFrac", "", 'F' );
   dataloader->AddVariable( "perJet_Track0Pt / perJet_Pt", "perJet_Track0Pt / perJet_Pt", "", 'F' );
   dataloader->AddVariable( "perJet_Track0dEta", "perJet_Track0dEta", "", 'F' );
   dataloader->AddVariable( "perJet_Track0dPhi", "perJet_Track0dPhi", "", 'F' );
   dataloader->AddVariable( "perJet_Track1Pt / perJet_Pt", "perJet_Track1Pt / perJet_Pt", "", 'F' );
   dataloader->AddVariable( "perJet_Track1dEta", "perJet_Track1dEta", "", 'F' );
   dataloader->AddVariable( "perJet_Track1dPhi", "perJet_Track1dPhi", "", 'F' );
   dataloader->AddVariable( "perJet_EnergyFrac_Depth1", "perJet_EnergyFrac_Depth1", "", 'F' );
   dataloader->AddVariable( "perJet_EnergyFrac_Depth2", "perJet_EnergyFrac_Depth2", "", 'F' );
   dataloader->AddVariable( "perJet_EnergyFrac_Depth3", "perJet_EnergyFrac_Depth3", "", 'F' );
   dataloader->AddVariable( "perJet_S_phiphi", "perJet_S_phiphi", "", 'F' );
   dataloader->AddVariable( "perJet_LeadingRechitE / perJet_E", "perJet_LeadingRechtE / perJet_E", "", 'F' );
 
    // Define Selections
   TCut selections_all = "perJet_Pt > 40 && abs(perJet_Eta) < 1.26";

   TCut selections_depth1 = "perJet_EnergyFrac_Depth1 >= 0 && perJet_EnergyFrac_Depth1 <= 1";
   TCut selections_depth2 = "perJet_EnergyFrac_Depth2 >= 0 && perJet_EnergyFrac_Depth2 <= 1";
   TCut selections_depth3 = "perJet_EnergyFrac_Depth3 >= 0 && perJet_EnergyFrac_Depth3 <= 1";
   TCut selections_depth4 = "perJet_EnergyFrac_Depth4 >= 0 && perJet_EnergyFrac_Depth4 <= 1";
   TCut selections_rechitVar1 = selections_depth1 + selections_depth2 + selections_depth3 + selections_depth4;
   TCut selections_rechitVar2 = "perJet_S_phiphi > 0 && perJet_S_phiphi < 900 && perJet_LeadingRechitE > 0 && perJet_LeadingRechitE < 900";
   TCut selections_trackVars = "perJet_Track0Pt > 0 && perJet_Track0Pt < 900 && perJet_Track0dR >= 0 && perJet_Track0dR < 1";
   TCut selections_track1Vars = "perJet_Track1Pt > 0 && perJet_Track1Pt < 900 && perJet_Track1dR >= 0 && perJet_Track1dR < 1";
   TCut selections_safety = selections_rechitVar1 + selections_rechitVar2 + selections_trackVars + selections_track1Vars; // no warnings when all variables behaved well! 
// + Cut_AnyLLP_HLT;

   TCut LLPinHCAL = "perJet_MatchedLLP_DecayR >= 177 && perJet_MatchedLLP_DecayR < 295 && abs(perJet_MatchedLLP_Eta) < 1.26";
   TCut LLPinECAL = "perJet_MatchedLLP_DecayR >= 129 && perJet_MatchedLLP_DecayR < 177 && abs(perJet_MatchedLLP_Eta) < 1.26";
   TCut LLPinHCAL12 = "perJet_MatchedLLP_DecayR >= 177 && perJet_MatchedLLP_DecayR < 214.2 && abs(perJet_MatchedLLP_Eta) < 1.26";
   TCut LLPinHCAL34 = "perJet_MatchedLLP_DecayR >= 214.2 && perJet_MatchedLLP_DecayR < 295 && abs(perJet_MatchedLLP_Eta) < 1.26";

   TCut selections_background = selections_all + selections_safety;
   TCut selections_signal = selections_all + selections_safety + LLPinHCAL;            // LLP decays in HCAL and is matched to jet
   TCut selections_signalECAL = selections_all + selections_safety + LLPinECAL;        // LLP decays in ECAL and is matched to jet
   TCut selections_signalHCAL12 = selections_all + selections_safety + LLPinHCAL12;    // LLP decays in HCAL depth 12 and is matched to jet
   TCut selections_signalHCAL34 = selections_all + selections_safety + LLPinHCAL34;    // LLP decays in HCAL depth 34 and is matched to jet

    // Read train and test data
   std::cout << "Loading files..." << std::endl;
   // Signal
   // training
   TFile *siginput(0);
   TString sigName = dir+"minituple_"+sigTagList_train+".root";
   siginput = TFile::Open( sigName ); // check if file in local directory exists
   std::cout << "--- TMVAClassification       : Using signal (training) input file: " << siginput->GetName() << std::endl;
   TTree *signalTree_temp = (TTree*)siginput->Get("PerJet_LLPmatched");
   std::cout << "opened signal file" << std::endl;
   TFile* scratchFile = TFile::Open( "scratchFile.root", "RECREATE" );
   TTree *signalTreeECAL       = (TTree*)signalTree_temp->CopyTree(selections_signalECAL);
   TTree *signalTreeHCAL12     = (TTree*)signalTree_temp->CopyTree(selections_signalHCAL12);
   TTree *signalTreeHCAL34     = (TTree*)signalTree_temp->CopyTree(selections_signalHCAL34);
   // testing
   TFile *siginput_test(0);
   TString sigName_test = dir+"minituple_"+sigTagList_test+".root";
   siginput_test = TFile::Open( sigName_test ); // check if file in local directory exists
   std::cout << "--- TMVAClassification       : Using signal (testing) input file: " << siginput_test->GetName() << std::endl;
   TTree *signalTreeTest_temp = (TTree*)siginput_test->Get("PerJet_LLPmatched");
   std::cout << "opened signal (testing) file" << std::endl;
   TFile* scratchFile_test = TFile::Open( "scratchFile_test.root", "RECREATE" );
   TTree *signalTreeECAL_test       = (TTree*)signalTreeTest_temp->CopyTree(selections_signalECAL);
   TTree *signalTreeHCAL12_test = (TTree*)signalTreeTest_temp->CopyTree(selections_signalHCAL12);
   TTree *signalTreeHCAL34_test = (TTree*)signalTreeTest_temp->CopyTree(selections_signalHCAL34);

   // Background
   // training
   TFile *bkginput(0);
   TString bkgName = dir+"minituple_"+bkgTag6+".root"; 
   bkginput = TFile::Open( bkgName ); // check if file in local directory exists
   std::cout << "--- TMVAClassification       : Using background (training, 1) input file: " << bkginput->GetName() << std::endl;
   TTree *background_temp = (TTree*)bkginput->Get("PerJet_WPlusJets");
   std::cout << "opened background file" << std::endl;
   scratchFile->cd();
   TTree *background      = (TTree*)background_temp->CopyTree(selections_background);

   // Background, for testing
   TFile *bkginput_test(0);
   TString bkgName_test = dir+"minituple_"+bkgTag2+".root"; 
   bkginput_test = TFile::Open( bkgName_test ); // check if file in local directory exists
   std::cout << "--- TMVAClassification       : Using background (testing) input file: " << bkginput_test->GetName() << std::endl;
   TTree *background_temp_test = (TTree*)bkginput_test->Get("PerJet_WPlusJets");
   std::cout << "opened background (testing) file" << std::endl;
   scratchFile->cd();
   TTree *background_test      = (TTree*)background_temp_test->CopyTree(selections_background);

   // TFile *input(0);
   // TString fname = "./tmva_example_multiclass.root";
   // if (!gSystem->AccessPathName( fname )) {
   //    input = TFile::Open( fname ); // check if file in local directory exists
   // }
   // else {
   //    TFile::SetCacheFileDir(".");
   //    input = TFile::Open("http://root.cern/files/tmva_multiclass_example.root", "CACHEREAD");
   // }
   // if (!input) {
   //    std::cout << "ERROR: could not open data file" << std::endl;
   //    exit(1);
   // }
   // std::cout << "--- TMVAMulticlass: Using input file: " << input->GetName() << std::endl;
 
   // TTree *signalTree  = (TTree*)input->Get("TreeS");
   // TTree *background0 = (TTree*)input->Get("TreeB0");
   // TTree *background1 = (TTree*)input->Get("TreeB1");
   // TTree *background2 = (TTree*)input->Get("TreeB2");
 
   gROOT->cd( outfileName+TString(":/") );
   dataloader->AddTree    (signalTreeHCAL34,"HCAL34");
   dataloader->AddTree    (signalTreeHCAL12,"HCAL12");
   dataloader->AddTree    (signalTreeECAL,"ECAL");
   dataloader->AddTree    (background,"bkg");
 
   dataloader->PrepareTrainingAndTestTree( "", "SplitMode=Random:NormMode=NumEvents:!V" );
 
   if (Use["BDTG"]) // gradient boosted decision trees
      factory->BookMethod( dataloader,  TMVA::Types::kBDT, "BDTG", "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.50:nCuts=20:MaxDepth=2");
   if (Use["MLP"]) // neural network
      factory->BookMethod( dataloader,  TMVA::Types::kMLP, "MLP", "!H:!V:NeuronType=tanh:NCycles=1000:HiddenLayers=N+5,5:TestRate=5:EstimatorType=MSE");
   if (Use["FDA_GA"]) // functional discriminant with GA minimizer
      factory->BookMethod( dataloader,  TMVA::Types::kFDA, "FDA_GA", "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );
   if (Use["PDEFoam"]) // PDE-Foam approach
      factory->BookMethod( dataloader,  TMVA::Types::kPDEFoam, "PDEFoam", "!H:!V:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T" );
 
 
   if (Use["DL_CPU"]) {
      TString layoutString("Layout=TANH|100,TANH|50,TANH|10,LINEAR");
      TString trainingStrategyString("TrainingStrategy=Optimizer=ADAM,LearningRate=1e-3,"
                                     "TestRepetitions=1,ConvergenceSteps=10,BatchSize=100,MaxEpochs=20");
      TString nnOptions("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
                        "WeightInitialization=XAVIERUNIFORM:Architecture=GPU");
      nnOptions.Append(":");
      nnOptions.Append(layoutString);
      nnOptions.Append(":");
      nnOptions.Append(trainingStrategyString);
      factory->BookMethod(dataloader, TMVA::Types::kDL, "DL_CPU", nnOptions);
   }
   if (Use["DL_GPU"]) {
      TString layoutString("Layout=TANH|100,TANH|50,TANH|10,LINEAR");
      TString trainingStrategyString("TrainingStrategy=Optimizer=ADAM,LearningRate=1e-3,"
                                     "TestRepetitions=1,ConvergenceSteps=10,BatchSize=100,MaxEpochs=20");
      TString nnOptions("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
                        "WeightInitialization=XAVIERUNIFORM:Architecture=GPU");
      nnOptions.Append(":");
      nnOptions.Append(layoutString);
      nnOptions.Append(":");
      nnOptions.Append(trainingStrategyString);
      factory->BookMethod(dataloader, TMVA::Types::kDL, "DL_GPU", nnOptions);
   }
 
 
   // Train MVAs using the set of training events
   factory->TrainAllMethods();
 
   // Evaluate all MVAs using the set of test events
   factory->TestAllMethods();
 
   // Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();
 
   // --------------------------------------------------------------
 
   // Save the output
   outputFile->Close();
 
   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAMulticlass is done!" << std::endl;
 
   delete factory;
   delete dataloader;
 
   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) TMVAMultiClassGui( outfileName );
}
 
int LLP_WPlusJets_Multiclassifier( )
{
   TString dir = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.8/";

   map<TString, TString> sigTagList_train;
   map<TString, TString> sigTagList_test;
   sigTagList_train["LLP125_MS15_ctau1000"]	   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TRAIN";   
   sigTagList_test["LLP125_MS15_ctau1000"]	   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TEST";     
   // sigTagList["LLP125_MS15_ctau3000"]	      = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau3000_13p6TeV_2024_06_03";   
   // sigTagList["LLP125_MS15_ctau10000"]	      = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau10000_13p6TeV_2024_06_03";
   // sigTagList["LLP125_MS50_ctau10000"]	      = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau10000_13p6TeV_2024_06_03";
   sigTagList_train["LLP125_MS50_ctau3000"]	   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_batch1";
   sigTagList_test["LLP125_MS50_ctau3000"]	   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_batch2";

   sigTagList_train["LLP250_MS120_ctau10000"]	= "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_06_03_batch1";
   sigTagList_test["LLP250_MS120_ctau10000"]	   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_06_03_batch2";
   
   sigTagList_train["LLP350_MS80_ctau500"]	   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_TRAIN";
   sigTagList_test["LLP350_MS80_ctau500"]	      = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_TEST";
   sigTagList_train["LLP350_MS160_ctau10000"]   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_06_03_batch1";
   sigTagList_test["LLP350_MS160_ctau10000"]    = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_06_03_batch2";

   TString bkgTag1 = "v3.8_LLPskim_Run2023Bv1_2024_06_03";
   TString bkgTag2 = "v3.8_LLPskim_Run2023Cv1_2024_06_03";
   TString bkgTag3 = "v3.8_LLPskim_Run2023Cv2_2024_06_03";
   TString bkgTag4 = "v3.8_LLPskim_Run2023Cv3_2024_06_03";
   TString bkgTag5  = "v3.8_LLPskim_Run2023Cv4_2024_06_03"; // test
   TString bkgTag6  = "v3.8_LLPskim_Run2023Dv1_2024_06_03"; // test
   TString bkgTag7 = "v3.8_LLPskim_Run2023Dv2_2024_06_03";

   // vector<string> filetag_keys_to_loop = {"LLP125_MS15", "LLP350_MS80", "LLP125_MS50", "LLP250_MS120", "LLP350_MS160", "hadd"};
   // vector<string> filetag_keys_to_loop = {"hadd"};
   vector<string> filetag_keys_to_loop = {"LLP350_MS80_ctau500"};
	for( auto key: filetag_keys_to_loop){
      cout << "TMVA training for " << key << endl;
      runClassification( dir, sigTagList_train[key], sigTagList_test[key], bkgTag1, bkgTag2, bkgTag3, bkgTag4, bkgTag5, bkgTag6, bkgTag7, key );
      cout << " " << endl;
   }

   return 0;
}