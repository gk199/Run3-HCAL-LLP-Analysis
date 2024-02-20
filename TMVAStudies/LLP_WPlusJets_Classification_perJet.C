/// \file
/// \ingroup tutorial_tmva
/// \notebook -nodraw
/// This macro provides examples for the training and testing of the
/// TMVA classifiers.
///
/// As input data is used a toy-MC sample consisting of four Gaussian-distributed
/// and linearly correlated input variables.
/// The methods to be used can be switched on and off by means of booleans, or
/// via the prompt command, for example:
///
///     root -l ./TMVAClassification.C\(\"Fisher,Likelihood\"\)
///
/// (note that the backslashes are mandatory)
/// If no method given, a default set of classifiers is used.
/// The output file "TMVA.root" can be analysed with the use of dedicated
/// macros (simply say: root -l <macro.C>), which can be conveniently
/// invoked through a GUI that will appear at the end of the run of this macro.
/// Launch the GUI via the command:
///
///     root -l ./TMVAGui.C
///
/// You can also compile and run the example with the following commands
///
///     make
///     ./TMVAClassification <Methods>
///
/// where: `<Methods> = "method1 method2"` are the TMVA classifier names
/// example:
///
///     ./TMVAClassification Fisher LikelihoodPCA BDT
///
/// If no method given, a default set is of classifiers is used
///
/// - Project   : TMVA - a ROOT-integrated toolkit for multivariate data analysis
/// - Package   : TMVA
/// - Root Macro: TMVAClassification
///
/// \macro_output
/// \macro_code
/// \author Andreas Hoecker


#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

using namespace TMVA;

int runClassification( TString dir, TString sigTag, TString sigTag_test, TString bkgTag, TString tag )
{
   // Setup cuts on LLP decay position. Include header file of all cuts potentially needed
   #include "../MiniTuplePlotter/RegionCuts.h"

   // The explicit loading of the shared libTMVA is done in TMVAlogon.C, defined in .rootrc
   // if you use your private .rootrc, or run from a different directory, please copy the
   // corresponding lines from .rootrc

   // Methods to be processed can be given as an argument; use format:
   //
   //     mylinux~> root -l TMVAClassification.C\(\"myMethod1,myMethod2,myMethod3\"\)

   //---------------------------------------------------------------
   // This loads the library

   std::cout << "Loading library" << std::endl;

   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   // Cut optimisation
   Use["Cuts"]            = 0;
   Use["CutsD"]           = 0;
   Use["CutsPCA"]         = 0;
   Use["CutsGA"]          = 0;
   Use["CutsSA"]          = 0;
   //
   // 1-dimensional likelihood ("naive Bayes estimator")
   Use["Likelihood"]      = 0;
   Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
   Use["LikelihoodPCA"]   = 0; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
   Use["LikelihoodKDE"]   = 0;
   Use["LikelihoodMIX"]   = 0;
   //
   // Mutidimensional likelihood and Nearest-Neighbour methods
   Use["PDERS"]           = 0;
   Use["PDERSD"]          = 0;
   Use["PDERSPCA"]        = 0;
   Use["PDEFoam"]         = 0;
   Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
   Use["KNN"]             = 0; // k-nearest neighbour method
   //
   // Linear Discriminant Analysis
   Use["LD"]              = 0; // Linear Discriminant identical to Fisher
   Use["Fisher"]          = 0;
   Use["FisherG"]         = 0;
   Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
   Use["HMatrix"]         = 0;
   //
   // Function Discriminant analysis
   Use["FDA_GA"]          = 0; // minimisation of user-defined function using Genetics Algorithm
   Use["FDA_SA"]          = 0;
   Use["FDA_MC"]          = 0;
   Use["FDA_MT"]          = 0;
   Use["FDA_GAMT"]        = 0;
   Use["FDA_MCMT"]        = 0;
   //
   // Neural Networks (all are feed-forward Multilayer Perceptrons)
   Use["MLP"]             = 0; // Recommended ANN
   Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
   Use["MLPBNN"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
   Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
   Use["TMlpANN"]         = 0; // ROOT's own ANN
#ifdef R__HAS_TMVAGPU
   Use["DNN_GPU"]         = 0; // CUDA-accelerated DNN training.
#else
   Use["DNN_GPU"]         = 0;
#endif

#ifdef R__HAS_TMVACPU
   Use["DNN_CPU"]         = 0; // Multi-core accelerated DNN.
#else
   Use["DNN_CPU"]         = 0;
#endif
   //
   // Support Vector Machine
   Use["SVM"]             = 0;
   //
   // Boosted Decision Trees
   Use["BDT"]             = 0; // uses Adaptive Boost
   Use["BDTG"]            = 1; // uses Gradient Boost
   Use["BDTB"]            = 0; // uses Bagging
   Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
   Use["BDTF"]            = 0; // allow usage of fisher discriminant for node splitting
   //
   // Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
   Use["RuleFit"]         = 0;
   // ---------------------------------------------------------------

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification" << std::endl;

   // Select methods (don't look at this code - not of interest)
   // if (myMethodList != "") {
   //    for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

   //    std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
   //    for (UInt_t i=0; i<mlist.size(); i++) {
   //       std::string regMethod(mlist[i]);

   //       if (Use.find(regMethod) == Use.end()) {
   //          std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
   //          for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
   //          std::cout << std::endl;
   //          return 1;
   //       }
   //       Use[regMethod] = 1;
   //    }
   // }

   // --------------------------------------------------------------------------------------------------
   // READ IN FILES 
   // --------------------------------------------------------------------------------------------------

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

   TCut LLPinHCAL = "perJet_MatchedLLP_DecayR >= 183.6 && perJet_MatchedLLP_DecayR < 295 && abs(perJet_MatchedLLP_Eta) < 1.26";
   TCut LLPinECAL = "perJet_MatchedLLP_DecayR >= 161.6 && perJet_MatchedLLP_DecayR < 183.6 && abs(perJet_MatchedLLP_Eta) < 1.26";
   TCut LLPinHCAL12 = "perJet_MatchedLLP_DecayR >= 183.6 && perJet_MatchedLLP_DecayR < 214.2 && abs(perJet_MatchedLLP_Eta) < 1.26";
   TCut LLPinHCAL34 = "perJet_MatchedLLP_DecayR >= 214.2 && perJet_MatchedLLP_DecayR < 295 && abs(perJet_MatchedLLP_Eta) < 1.26";

   TCut selections_background = selections_all + selections_safety;
   TCut selections_signal = selections_all + selections_safety + LLPinHCAL;            // LLP decays in HCAL and is matched to jet
   TCut selections_signalECAL = selections_all + selections_safety + LLPinECAL;        // LLP decays in ECAL and is matched to jet
   TCut selections_signalHCAL12 = selections_all + selections_safety + LLPinHCAL12;    // LLP decays in HCAL depth 12 and is matched to jet
   TCut selections_signalHCAL34 = selections_all + selections_safety + LLPinHCAL34;    // LLP decays in HCAL depth 34 and is matched to jet

   // Read training and test data (it is also possible to use ASCII format as input -> see TMVA Users Guide)
   std::cout << "Loading files..." << std::endl;

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   TString outfileName( "BDTWeightFilesTest/Test_" + tag +"_perJet.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
   
   // Signal // 

   TFile *siginput(0);
   TString sigName = dir+"minituple_"+sigTag+".root";
   siginput = TFile::Open( sigName ); // check if file in local directory exists
   std::cout << "--- TMVAClassification       : Using signal (training) input file: " << siginput->GetName() << std::endl;
   TTree *signalTree_temp = (TTree*)siginput->Get("PerJet_LLPmatched");
   std::cout << "opened signal file" << std::endl;
   TFile* scratchFile = TFile::Open( "scratchFile.root", "RECREATE" );
   TTree *signalTree       = (TTree*)signalTree_temp->CopyTree(selections_signal);

   TTree *signalTreeHCAL12 = (TTree*)signalTree_temp->CopyTree(selections_signalHCAL12);
   TTree *signalTreeHCAL34 = (TTree*)signalTree_temp->CopyTree(selections_signalHCAL34);

   // Signal, for testing
   TFile *siginput_test(0);
   TString sigName_test = dir+"minituple_"+sigTag_test+".root";
   siginput_test = TFile::Open( sigName_test ); // check if file in local directory exists
   std::cout << "--- TMVAClassification       : Using signal (testing) input file: " << siginput_test->GetName() << std::endl;
   TTree *signalTreeTest_temp = (TTree*)siginput_test->Get("PerJet_LLPmatched");
   std::cout << "opened signal (testing) file" << std::endl;
   TFile* scratchFile_test = TFile::Open( "scratchFile_test.root", "RECREATE" );
   TTree *signalTree_test       = (TTree*)signalTreeTest_temp->CopyTree(selections_signal);

   TTree *signalTreeHCAL12_test = (TTree*)signalTreeTest_temp->CopyTree(selections_signalHCAL12);
   TTree *signalTreeHCAL34_test = (TTree*)signalTreeTest_temp->CopyTree(selections_signalHCAL34);

   // Register the training and test trees

   TFile *bkginput(0);
   TString bkgName = dir+"minituple_"+bkgTag+".root"; 
   bkginput = TFile::Open( bkgName ); // check if file in local directory exists
   std::cout << "--- TMVAClassification       : Using background input file: " << bkginput->GetName() << std::endl;
   TTree *background_temp = (TTree*)bkginput->Get("PerJet_WPlusJets");
   std::cout << "opened background file" << std::endl;
   scratchFile->cd();
   TTree *background      = (TTree*)background_temp->CopyTree(selections_background);

   outputFile->cd();

   // Create the factory object. Later you can choose the methods
   // whose performance you'd like to investigate. The factory is
   // the only TMVA object you have to interact with
   //
   // The first argument is the base of the name of all the
   // weightfiles in the directory weight/
   //
   // The second argument is the output file for the training results
   // All TMVA output can be suppressed by removing the "!" (not) in
   // front of the "Silent" argument in the option string
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

   TMVA::DataLoader *dataloader=new TMVA::DataLoader("BDTWeightFilesTest");
   // If you wish to modify default settings
   // (please check "src/Config.h" to see all available global options)
   //
   //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
   //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";
   (TMVA::gConfig().GetIONames()).fWeightFileDir = "weights_"+tag+"_perJet"; // save each weight file in a separate directory

   // Define the input variables that shall be used for the MVA training
   // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
   // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]

   //dataloader->AddVariable( "bdtscore",      "bdtscore", "", 'F' );

   // jet kinematics // *************************
   dataloader->AddVariable( "perJet_Eta", "perJet_Eta", "", 'F' );
   // dataloader->AddVariable( "perJet_Phi", "perJet_Phi", "", 'F' );                                 // removing because there is a difference in pi and -pi in data (W+jets)
   // dataloader->AddVariable( "perJet_Pt", "perJet_Pt", "GeV", 'F' );
   // dataloader->AddVariable( "perJet_E", "perJet_E", "GeV", 'F' );
   // track-based variables // *************************
   // dataloader->AddVariable( "perJet_ChargedHadEFrac", "perJet_ChargedHadEFrac", "", 'F' );
   // dataloader->AddVariable( "perJet_NeutralHadEFrac", "perJet_NeutralHadEFrac", "", 'F' );
   // //dataloader->AddVariable( "perJet_MuonEFrac", "perJet_MuonEFrac", "", 'F' );
   // dataloader->AddVariable( "perJet_PhoEFrac", "perJet_PhoEFrac", "", 'F' );
   // dataloader->AddVariable( "perJet_EleEFrac", "perJet_EleEFrac", "", 'F' );
   // dataloader->AddVariable( "perJet_Track0Pt / perJet_Pt", "perJet_Track0Pt / perJet_Pt", "", 'F' );
   // // dataloader->AddVariable( "perJet_Track0dR", "perJet_Track0dR", "", 'F' );
   // dataloader->AddVariable( "perJet_Track0dEta", "perJet_Track0dEta", "", 'F' );
   // dataloader->AddVariable( "perJet_Track0dPhi", "perJet_Track0dPhi", "", 'F' );
   // dataloader->AddVariable( "perJet_Track1Pt / perJet_Pt", "perJet_Track1Pt / perJet_Pt", "", 'F' );
   // // dataloader->AddVariable( "perJet_Track1dR", "perJet_Track1dR", "", 'F' );
   // dataloader->AddVariable( "perJet_Track1dEta", "perJet_Track1dEta", "", 'F' );
   // dataloader->AddVariable( "perJet_Track1dPhi", "perJet_Track1dPhi", "", 'F' );
   //dataloader->AddVariable( "perJet_Track1Pt", "perJet_Track1Pt", "GeV", 'F' );
   //dataloader->AddVariable( "perJet_Track2Pt", "perJet_Track2Pt", "GeV", 'F' );
   //dataloader->AddVariable( "perJet_HoverE", "perJet_HoverE", "", 'F' );
   // rechit-based variables // *************************
   dataloader->AddVariable( "perJet_EnergyFrac_Depth1", "perJet_EnergyFrac_Depth1", "", 'F' );
   dataloader->AddVariable( "perJet_EnergyFrac_Depth2", "perJet_EnergyFrac_Depth2", "", 'F' );
   dataloader->AddVariable( "perJet_EnergyFrac_Depth3", "perJet_EnergyFrac_Depth3", "", 'F' );
   //dataloader->AddVariable( "perJet_EnergyFrac_Depth4", "perJet_EnergyFrac_Depth4", "", 'F' );       // redundant! 
   dataloader->AddVariable( "perJet_S_phiphi", "perJet_S_phiphi", "", 'F' );
   dataloader->AddVariable( "perJet_LeadingRechitE / perJet_E", "perJet_LeadingRechtE / perJet_E", "", 'F' );

   // You can add so-called "Spectator variables", which are not used in the MVA training,
   // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
   // input variables, the response values of all trained MVAs, and the spectator variables

   // removing spectator variables 
   // dataloader->AddSpectator( "LLP0_Pt", "LLP0_Pt", "GeV", 'F' );
   // dataloader->AddSpectator( "LLP0_E", "LLP0_E", "GeV", 'F' );
   // dataloader->AddSpectator( "LLP0_Beta", "LLP0_Beta", "", 'F' );
   // dataloader->AddSpectator( "LLP0_TravelTime", "LLP0_TravelTime", "ns", 'F' );
   // dataloader->AddSpectator( "LLP0_DecayR", "LLP0_DecayR", "cm", 'F' );
   // dataloader->AddSpectator( "LLP0_DecayX", "LLP0_DecayX", "cm", 'F' );
   // dataloader->AddSpectator( "LLP0_DecayY", "LLP0_DecayY", "cm", 'F' );
   // dataloader->AddSpectator( "LLP0_DecayZ", "LLP0_DecayZ", "cm", 'F' );

   // global event weights per tree (see below for setting event-wise weights)
   // Double_t signalTestWeight     = 1.0;
   Double_t signalWeight     = 1.0;
   Double_t backgroundWeight = 1.0;

   // You can add an arbitrary number of signal or background trees
   dataloader->AddSignalTree     ( signalTree,      signalWeight );
   dataloader->AddSignalTree     ( signalTree_test, signalWeight,  "Test" );
   // dataloader->AddSignalTree     ( signalTreeHCAL12,     signalWeight );
   // dataloader->AddSignalTree     ( signalTreeHCAL12_test,     signalWeight, "Test" );
   // dataloader->AddSignalTree     ( signalTreeHCAL34,     signalWeight );
   // dataloader->AddSignalTree     ( signalTreeHCAL34_test,     signalWeight, "Test" );
   dataloader->AddBackgroundTree ( background, backgroundWeight );

   // To give different trees for training and testing, do as follows:
   //
   //dataloader->AddSignalTree( signalTrainingTree, signalTrainWeight);
   //dataloader->AddSignalTree( signalTestTree1,     signalWeight,  "Test" );
   //dataloader->AddSignalTree( signalTestTree2,     signalWeight,  "Test" );
   //dataloader->AddSignalTree( signalTestTree3,     signalWeight,  "Test" );

   // Use the following code instead of the above two or four lines to add signal and background
   // training and test events "by hand"
   // NOTE that in this case one should not give expressions (such as "var1+var2") in the input
   //      variable definition, but simply compute the expression before adding the event
   // ```cpp
   // // --- begin ----------------------------------------------------------
   // int numVars = 11;
   // std::vector<Double_t> vars( numVars ); // vector has size of number of input variables
   // Double_t  treevars[numVars], weight;
   // 
   // std::vector<string> var_names;
   // var_names.push_back("ph1_E");
   // var_names.push_back("ph2_E");
   // var_names.push_back("ph1_maxEcell_E");
   // var_names.push_back("ph2_maxEcell_E");
   // var_names.push_back("2DSliceRZ_Vr");
   // var_names.push_back("2DSliceRZ_Vz");
   // var_names.push_back("MET_met");
   // var_names.push_back("Hcand_pt");
   // var_names.push_back("dR_ph");
   // var_names.push_back("dPhi_ph");
   // var_names.push_back("dEta_ph");

   // // Signal
   // signalTree->SetBranchAddress( "weight", &weight );
   // for (UInt_t ivar=0; ivar<numVars; ivar++) signalTree->SetBranchAddress( Form( "%s", var_names[ivar].c_str() ), &(treevars[ivar]) );
   // for (UInt_t i=0; i<signalTree->GetEntries(); i++) {
   //    signalTree->GetEntry(i);
   //    for (UInt_t ivar=0; ivar<numVars; ivar++) vars[ivar] = treevars[ivar];
   //    // add training and test events; here: first half is training, second is testing
   //    // note that the weight can also be event-wise
   //    if (i < signalTree->GetEntries()/2.0) dataloader->AddSignalTrainingEvent( vars, signalWeight*weight );
   //    else                              dataloader->AddSignalTestEvent    ( vars, signalWeight*weight );
   // }
   // 
   // // Background (has event weights)
   // background->SetBranchAddress( "weight", &weight );
   // for (UInt_t ivar=0; ivar<numVars; ivar++) background->SetBranchAddress( Form( "%s", var_names[ivar].c_str() ), &(treevars[ivar]) );
   // for (UInt_t i=0; i<background->GetEntries(); i++) {
   //    background->GetEntry(i);
   //    for (UInt_t ivar=0; ivar<numVars; ivar++) vars[ivar] = treevars[ivar];
   //    // add training and test events; here: first half is training, second is testing
   //    // note that the weight can also be event-wise
   //    if (i < background->GetEntries()/2) dataloader->AddBackgroundTrainingEvent( vars, backgroundWeight*weight );
   //    else                                dataloader->AddBackgroundTestEvent    ( vars, backgroundWeight*weight );
   // }
   // // --- end ------------------------------------------------------------
   // ```
   // End of tree registration

   // Set individual event weights (the variables must exist in the original TTree)
   // -  for signal    : `dataloader->SetSignalWeightExpression    ("weight1*weight2");`
   // -  for background: `dataloader->SetBackgroundWeightExpression("weight1*weight2");`
  
   // dataloader->SetBackgroundWeightExpression( "weight" );
   // dataloader->SetSignalWeightExpression    ("weight_bdt");

   // Apply additional cuts on the signal and background samples (can be different)
   // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   //TCut mycuta; // = "abs(2DSliceRZ_Vr) < 1050 && abs(2DSliceRZ_Vz) < 3000"; // && ph_detector_region == \"BB\"";
   //TCut mycutb = "2DSliceRZ_Vr>0 && abs(ph1_t)<12 && abs(ph2_t)<12 && Hcand_M>60"; // for example: TCut mycutb = "abs(var1)<0.5";
   TCut mycuts = ""; //mycuta && mycutb;
   TCut mycutb = "";
   // Tell the dataloader how to use the training and testing events
   //
   // If no numbers of events are given, half of the events in the tree are used
   // for training, and the other half for testing:
   //
   dataloader->PrepareTrainingAndTestTree( mycuts, "SplitMode=random:!V" );
   //
   // To also specify the number of testing events, use:
   //
   //    dataloader->PrepareTrainingAndTestTree( mycut,
   //         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );
   //    dataloader->PrepareTrainingAndTestTree( mycuts, mycutb,
   //                                     "nTrain_Signal=1000:nTrain_Background=1000:SplitMode=Random:NormMode=NumEvents:!V" );
   // dataloader->PrepareTrainingAndTestTree( mycuts, mycutb, "nTrain_Signal=29000:nTrain_Background=24000:SplitMode=Random:NormMode=NumEvents:!V" );

   // ### Book MVA methods
   //
   // Please lookup the various method configuration options in the corresponding cxx files, eg:
   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

   // Cut optimisation
   if (Use["Cuts"])
      factory->BookMethod( dataloader, TMVA::Types::kCuts, "Cuts",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

   if (Use["CutsD"])
      factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsD",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );

   if (Use["CutsPCA"])
      factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsPCA",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );

   if (Use["CutsGA"])
      factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsGA",
                           "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );

   if (Use["CutsSA"])
      factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsSA",
                           "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

   // Likelihood ("naive Bayes estimator")
   if (Use["Likelihood"])
      factory->BookMethod( dataloader, TMVA::Types::kLikelihood, "Likelihood",
                           "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );

   // Decorrelated likelihood
   if (Use["LikelihoodD"])
      factory->BookMethod( dataloader, TMVA::Types::kLikelihood, "LikelihoodD",
                           "!H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=Decorrelate" );

   // PCA-transformed likelihood
   if (Use["LikelihoodPCA"])
      factory->BookMethod( dataloader, TMVA::Types::kLikelihood, "LikelihoodPCA",
                           "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA" );

   // Use a kernel density estimator to approximate the PDFs
   if (Use["LikelihoodKDE"])
      factory->BookMethod( dataloader, TMVA::Types::kLikelihood, "LikelihoodKDE",
                           "!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" );

   // Use a variable-dependent mix of splines and kernel density estimator
   if (Use["LikelihoodMIX"])
      factory->BookMethod( dataloader, TMVA::Types::kLikelihood, "LikelihoodMIX",
                           "!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" );

   // Test the multi-dimensional probability density estimator
   // here are the options strings for the MinMax and RMS methods, respectively:
   //
   //      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );
   //      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );
   if (Use["PDERS"])
      factory->BookMethod( dataloader, TMVA::Types::kPDERS, "PDERS",
                           "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );

   if (Use["PDERSD"])
      factory->BookMethod( dataloader, TMVA::Types::kPDERS, "PDERSD",
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate" );

   if (Use["PDERSPCA"])
      factory->BookMethod( dataloader, TMVA::Types::kPDERS, "PDERSPCA",
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA" );

   // Multi-dimensional likelihood estimator using self-adapting phase-space binning
   if (Use["PDEFoam"])
      factory->BookMethod( dataloader, TMVA::Types::kPDEFoam, "PDEFoam",
                           "!H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T" );

   if (Use["PDEFoamBoost"])
      factory->BookMethod( dataloader, TMVA::Types::kPDEFoam, "PDEFoamBoost",
                           "!H:!V:Boost_Num=30:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4:UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=500:nBin=20:Nmin=400:Kernel=None:Compress=T" );

   // K-Nearest Neighbour classifier (KNN)
   if (Use["KNN"])
      factory->BookMethod( dataloader, TMVA::Types::kKNN, "KNN",
                           "H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );

   // H-Matrix (chi2-squared) method
   if (Use["HMatrix"])
      factory->BookMethod( dataloader, TMVA::Types::kHMatrix, "HMatrix", "!H:!V:VarTransform=None" );

   // Linear discriminant (same as Fisher discriminant)
   if (Use["LD"])
      factory->BookMethod( dataloader, TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   // Fisher discriminant (same as LD)
   if (Use["Fisher"])
      factory->BookMethod( dataloader, TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   // Fisher with Gauss-transformed input variables
   if (Use["FisherG"])
      factory->BookMethod( dataloader, TMVA::Types::kFisher, "FisherG", "H:!V:VarTransform=Gauss" );

   // Composite classifier: ensemble (tree) of boosted Fisher classifiers
   if (Use["BoostedFisher"])
      factory->BookMethod( dataloader, TMVA::Types::kFisher, "BoostedFisher",
                           "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2:!Boost_DetailedMonitoring" );

   // Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
   if (Use["FDA_MC"])
      factory->BookMethod( dataloader, TMVA::Types::kFDA, "FDA_MC",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1" );

   if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod( dataloader, TMVA::Types::kFDA, "FDA_GA",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=100:Cycles=2:Steps=5:Trim=True:SaveBestGen=1" );

   if (Use["FDA_SA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod( dataloader, TMVA::Types::kFDA, "FDA_SA",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

   if (Use["FDA_MT"])
      factory->BookMethod( dataloader, TMVA::Types::kFDA, "FDA_MT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );

   if (Use["FDA_GAMT"])
      factory->BookMethod( dataloader, TMVA::Types::kFDA, "FDA_GAMT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );

   if (Use["FDA_MCMT"])
      factory->BookMethod( dataloader, TMVA::Types::kFDA, "FDA_MCMT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20" );

   // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
   if (Use["MLP"])
      factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );

   if (Use["MLPBFGS"])
      factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:!UseRegulator" );

   if (Use["MLPBNN"])
      factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLPBNN", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=60:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators


   // Multi-architecture DNN implementation.
   if (Use["DNN_CPU"] or Use["DNN_GPU"]) {
      // General layout.
      TString layoutString ("Layout=TANH|128,TANH|128,TANH|128,LINEAR");

      // Training strategies.
      TString training0("LearningRate=1e-2,Momentum=0.9,Repetitions=1,"
                        "ConvergenceSteps=30,BatchSize=256,TestRepetitions=10,"
                        "WeightDecay=1e-4,Regularization=None,"
                        "DropConfig=0.0+0.5+0.5+0.5, Multithreading=True");
      TString training1("LearningRate=1e-2,Momentum=0.9,Repetitions=1,"
                        "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                        "WeightDecay=1e-4,Regularization=L2,"
                        "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
      TString training2("LearningRate=1e-3,Momentum=0.0,Repetitions=1,"
                        "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                        "WeightDecay=1e-4,Regularization=L2,"
                        "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
      TString trainingStrategyString ("TrainingStrategy=");
      trainingStrategyString += training0 + "|" + training1 + "|" + training2;

      // General Options.
      TString dnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
                          "WeightInitialization=XAVIERUNIFORM");
      dnnOptions.Append (":"); dnnOptions.Append (layoutString);
      dnnOptions.Append (":"); dnnOptions.Append (trainingStrategyString);

      // Cuda implementation.
      if (Use["DNN_GPU"]) {
         TString gpuOptions = dnnOptions + ":Architecture=GPU";
         factory->BookMethod(dataloader, TMVA::Types::kDL, "DNN_GPU", gpuOptions);
      }
      // Multi-core CPU implementation.
      if (Use["DNN_CPU"]) {
         TString cpuOptions = dnnOptions + ":Architecture=CPU";
         factory->BookMethod(dataloader, TMVA::Types::kDL, "DNN_CPU", cpuOptions);
      }
   }

   // CF(Clermont-Ferrand)ANN
   if (Use["CFMlpANN"])
      factory->BookMethod( dataloader, TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N"  ); // n_cycles:#nodes:#nodes:...

   // Tmlp(Root)ANN
   if (Use["TMlpANN"])
      factory->BookMethod( dataloader, TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); // n_cycles:#nodes:#nodes:...

   // Support Vector Machine
   if (Use["SVM"])
      factory->BookMethod( dataloader, TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );

   // Boosted Decision Trees
   if (Use["BDTG"]) // Gradient Boost
      factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG",
                           "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );

   if (Use["BDT"])  // Adaptive Boost
      factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

   if (Use["BDTB"]) // Bagging
      factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTB",
                           "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20" );

   if (Use["BDTD"]) // Decorrelation + Adaptive Boost
      factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTD",
                           "!H:!V:NTrees=400:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );

   if (Use["BDTF"])  // Allow Using Fisher discriminant in node splitting for (strong) linearly correlated variables
      factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTF",
                           "!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20" );

   // RuleFit -- TMVA implementation of Friedman's method
   if (Use["RuleFit"])
      factory->BookMethod( dataloader, TMVA::Types::kRuleFit, "RuleFit",
                           "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );

   // For an example of the category classifier usage, see: TMVAClassificationCategory
   //
   // --------------------------------------------------------------------------------------------------
   //  Now you can optimize the setting (configuration) of the MVAs using the set of training events
   // STILL EXPERIMENTAL and only implemented for BDT's !
   //
   //     factory->OptimizeAllMethods("SigEffAtBkg0.01","Scan");
   //     factory->OptimizeAllMethods("ROCIntegral","FitGA");
   //
   // --------------------------------------------------------------------------------------------------

   // Now you can tell the factory to train, test, and evaluate the MVAs
   //
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
   std::cout << "==> TMVAClassification is done!" << std::endl;

   delete factory;
   delete dataloader;
   // Launch the GUI for the root macros
   //if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );

   return 0;
}

int LLP_WPlusJets_Classification_perJet()
{
   // Select methods (don't look at this code - not of interest)
   //std::cout << "print" << std::endl;

   map<TString, TString> sigTagList;
   sigTagList["LLP125_MS15"]	   = "v3.4_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_02_16_TRAIN";
   sigTagList["LLP350_MS80"]	   = "v3.4_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_02_16_TRAIN";
   sigTagList["LLP125_MS50"]	   = "v3.4_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_02_16_batch1";
   sigTagList["LLP250_MS120"]    = "v3.4_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_02_16_batch1";
   sigTagList["LLP350_MS160"]    = "v3.4_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_02_16_batch1";
   sigTagList["hadd"]            = "v3.4_LLP_MC_ggH_HToSSTobbbb_MH-HADD_TRAIN-batch1";

   map<TString, TString> sigTagList_test;
   sigTagList_test["LLP125_MS15"]	   = "v3.4_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-15_CTau1000_13p6TeV_2024_02_16_TEST";
   sigTagList_test["LLP350_MS80"]	   = "v3.4_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-80_CTau500_13p6TeV_2024_02_16_TEST";
   sigTagList_test["LLP125_MS50"]	   = "v3.4_LLP_MC_ggH_HToSSTobbbb_MH-125_MS-50_CTau3000_13p6TeV_2024_02_16_batch2";
   sigTagList_test["LLP250_MS120"]     = "v3.4_LLP_MC_ggH_HToSSTobbbb_MH-250_MS-120_CTau10000_13p6TeV_2024_02_16_batch2";
   sigTagList_test["LLP350_MS160"]     = "v3.4_LLP_MC_ggH_HToSSTobbbb_MH-350_MS-160_CTau10000_13p6TeV_2024_02_16_batch2";
   sigTagList_test["hadd"]             = "v3.4_LLP_MC_ggH_HToSSTobbbb_MH-HADD_TEST-batch2";

   TString bkgTag = "v3.4_LLPskim_Run2023Dv1_2024_02_16";

   TString dir = "/eos/cms/store/group/phys_exotica/HCAL_LLP/MiniTuples/v3.4/";

   vector<string> filetag_keys_to_loop = {"LLP125_MS15", "LLP350_MS80", "LLP125_MS50", "LLP250_MS120", "LLP350_MS160", "hadd"};
	for( auto key: filetag_keys_to_loop){
      cout << "TMVA training for " << key << ", per jet " << endl;
      runClassification(dir, sigTagList[key], sigTagList_test[key], bkgTag, key+"_calor"); // key + "xxx" will be the name of the weights file. Make sure this agrees with the signal selection cuts!
      cout << " " << endl;
   }
   return 0;
}

