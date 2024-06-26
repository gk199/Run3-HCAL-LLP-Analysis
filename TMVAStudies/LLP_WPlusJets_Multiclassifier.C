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
 
void runClassification( TString dir, TString sigTagList_train, TString sigTagList_test, TString bkgTag1_train, TString bkgTag2_train, TString bkgTag3_train, TString bkgTag4_train, TString bkgTag5_test, TString bkgTag6_test, TString bkgTag7_train, TString tag = "" )
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
   dataloader->AddVariable( "var1", 'F' );
   dataloader->AddVariable( "var2", "Variable 2", "", 'F' );
   dataloader->AddVariable( "var3", "Variable 3", "units", 'F' );
   dataloader->AddVariable( "var4", "Variable 4", "units", 'F' );
 
    // Read train and test data
   std::cout << "Loading files..." << std::endl;
   // Signal
   // Background

   TFile *input(0);
   TString fname = "./tmva_example_multiclass.root";
   if (!gSystem->AccessPathName( fname )) {
      input = TFile::Open( fname ); // check if file in local directory exists
   }
   else {
      TFile::SetCacheFileDir(".");
      input = TFile::Open("http://root.cern/files/tmva_multiclass_example.root", "CACHEREAD");
   }
   if (!input) {
      std::cout << "ERROR: could not open data file" << std::endl;
      exit(1);
   }
   std::cout << "--- TMVAMulticlass: Using input file: " << input->GetName() << std::endl;
 
   TTree *signalTree  = (TTree*)input->Get("TreeS");
   TTree *background0 = (TTree*)input->Get("TreeB0");
   TTree *background1 = (TTree*)input->Get("TreeB1");
   TTree *background2 = (TTree*)input->Get("TreeB2");
 
   gROOT->cd( outfileName+TString(":/") );
   dataloader->AddTree    (signalTree,"Signal");
   dataloader->AddTree    (background0,"bg0");
   dataloader->AddTree    (background1,"bg1");
   dataloader->AddTree    (background2,"bg2");
 
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
   sigTagList_train["LLP125_MS15_ctau1000"]	   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TRAIN.root";   
   sigTagList_test["LLP125_MS15_ctau1000"]	   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_06_03_TEST.root";     
   // sigTagList["LLP125_MS15_ctau3000"]	      = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau3000_13p6TeV_2024_06_03.root";   
   // sigTagList["LLP125_MS15_ctau10000"]	      = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau10000_13p6TeV_2024_06_03.root";
   // sigTagList["LLP125_MS50_ctau10000"]	      = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau10000_13p6TeV_2024_06_03.root";
   sigTagList_train["LLP125_MS50_ctau3000"]	   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_batch1.root";
   sigTagList_test["LLP125_MS50_ctau3000"]	   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_06_03_batch2.root";

   sigTagList_train["LLP250_MS120_ctau10000"]	= "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_06_03_batch1.root";
   sigTagList_test["LLP250_MS120_ctau10000"]	   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_06_03_batch2.root";
   
   sigTagList_train["LLP350_MS80_ctau500"]	   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_TRAIN.root";
   sigTagList_test["LLP350_MS80_ctau500"]	      = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_06_03_TEST.root";
   sigTagList_train["LLP350_MS160_ctau10000"]   = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_06_03_batch1.root";
   sigTagList_test["LLP350_MS160_ctau10000"]    = "v3.8_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_06_03_batch2.root";

   TString bkgTag1_train = "v3.8_LLPskim_Run2023Bv1_2024_06_03.root";
   TString bkgTag2_train = "v3.8_LLPskim_Run2023Cv1_2024_06_03.root";
   TString bkgTag3_train = "v3.8_LLPskim_Run2023Cv2_2024_06_03.root";
   TString bkgTag4_train = "v3.8_LLPskim_Run2023Cv3_2024_06_03.root";
   TString bkgTag5_test  = "v3.8_LLPskim_Run2023Cv4_2024_06_03.root";
   TString bkgTag6_test  = "v3.8_LLPskim_Run2023Dv1_2024_06_03.root";
   TString bkgTag7_train = "v3.8_LLPskim_Run2023Dv2_2024_06_03.root";

   // vector<string> filetag_keys_to_loop = {"LLP125_MS15", "LLP350_MS80", "LLP125_MS50", "LLP250_MS120", "LLP350_MS160", "hadd"};
   // vector<string> filetag_keys_to_loop = {"hadd"};
   vector<string> filetag_keys_to_loop = {"LLP350_MS80_ctau500"};
	for( auto key: filetag_keys_to_loop){
      cout << "TMVA training for " << key << endl;
      runClassification( dir, sigTagList_train[key], sigTagList_test[key], bkgTag1_train, bkgTag2_train, bkgTag3_train, bkgTag4_train, bkgTag5_test, bkgTag6_test, bkgTag7_train, key );
      cout << " " << endl;
   }

   return 0;
}