//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Feb 17 20:24:46 2023 by ROOT version 6.24/07
// Kiley Kennedy
//////////////////////////////////////////////////////////

#ifndef DisplacedHcalJetAnalyzer_h
#define DisplacedHcalJetAnalyzer_h

#include <TROOT.h>
#include <TSystem.h>
#include <TChain.h>
#include <TFile.h>

//#pragma link C++ class vector<vector<int>>+;

#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TF2.h>
#include <TGraph2D.h>
#include <TRandom.h>
#include <TLorentzVector.h>
#include <TVector3.h>

#include <TStyle.h>
#include <TCanvas.h>

#include <TMath.h>

//#include "Math/GSLMinimizer.h"
//#include "Minuit2/Minuit2Minimizer.h"
//#include <Math/Functor.h>
//#include <TMVA/Reader.h>

#include <vector>
#include<algorithm>
#ifdef __ROOTCLING__
#pragma link C++ class vector<vector <float> >+;
#pragma link C++ class vector<vector <int> >+;
#endif

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "map"
#include "iostream"
#include "iomanip" 


class DisplacedHcalJetAnalyzer {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // ----- Settings ----- //

   bool debug         = false; 
   bool print_counts  = false; 
   bool save_hists    = true;
   bool save_trees    = true;
   bool blind_data    = false;

   float weight = 1;

   Long64_t NEvents   = -1; 
   Long64_t NEvents_HLT = -1;

   // Numbers for cutflow
   map<string, Double32_t> count; //Long64_t

   // ----- My Hists ----- //

   vector<string> categories;
   map<string, TH1F*> h;
   map<string, TH2F*> h2;
   map<string, TProfile*> hp;

   // ----- My Output Tree ----- //
   
   vector<string> treenames;
   map<string,TTree*>   tree_output;
   map<string,bool>     tree_output_vars_bool;  
   map<string,int>      tree_output_vars_int;  
   map<string,float>    tree_output_vars_float;  
   map<string,string>   tree_output_vars_string;

   // ----- Globals ----- //

   vector<string> HLT_Names;
   map<string, int> HLT_Indices;

   vector<int> gLLPDecay_iLLP;
   vector<int> gLLPDecay_iParticle;
   vector<vector<int>> map_gLLP_to_gParticle_indices;

   vector<float> gLLP_DecayVtx_R;
   vector<float> gLLP_DecayVtx_Mag;

   // ----- Variables ----- //

   // Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Bool_t          isData;
   UInt_t          runNum;
   UInt_t          lumiNum;
   ULong64_t       eventNum;
   UInt_t          eventTime;
   Int_t           n_PV;
   Float_t         PV_X;
   Float_t         PV_Y;
   Float_t         PV_Z;
   Int_t           n_PVCand;
   vector<float>   *PVCand_X;
   vector<float>   *PVCand_Y;
   vector<float>   *PVCand_Z;
   vector<float>   *PVCand_LogSumPtSq;
   vector<float>   *PVCand_SumPx;
   vector<float>   *PVCand_SumPy;
   vector<float>   *PVTrack_Pt;
   vector<float>   *PVTrack_Eta;
   vector<float>   *PVTrack_Phi;
   vector<bool>    *HLT_Decision;
   vector<int>     *HLT_Prescale;
   Float_t         met_Pt;
   Float_t         met_Phi;
   Float_t         met_SumEt;
   Int_t           n_ele;
   vector<float>   *ele_Pt;
   vector<float>   *ele_Eta;
   vector<float>   *ele_Phi;
   vector<float>   *ele_E;
   vector<float>   *ele_Charge;
   vector<float>   *ele_d0;
   vector<float>   *ele_dZ;
   vector<float>   *ele_dEta;
   vector<float>   *ele_dPhi;
   vector<bool>    *ele_passCutBasedIDLoose;
   vector<bool>    *ele_passCutBasedIDMedium;
   vector<bool>    *ele_passCutBasedIDTight;
   vector<vector<unsigned int> > *ele_EcalRechitIDs;
   vector<vector<unsigned int> > *ele_EcalRechitIndices;
   vector<unsigned int> *ele_SeedRechitID;
   vector<unsigned int> *ele_SeedRechitIndex;
   Int_t           n_muon;
   vector<float>   *muon_Pt;
   vector<float>   *muon_Eta;
   vector<float>   *muon_Phi;
   vector<float>   *muon_E;
   vector<int>     *muon_Charge;
   vector<float>   *muon_d0;
   vector<float>   *muon_dZ;
   vector<bool>    *muon_IsLoose;
   vector<bool>    *muon_IsMedium;
   vector<bool>    *muon_IsTight;
   Int_t           n_tau;
   vector<float>   *tau_E;
   vector<float>   *tau_Pt;
   vector<float>   *tau_Eta;
   vector<float>   *tau_Phi;
   vector<bool>    *tau_IsLoose;
   vector<bool>    *tau_IsMedium;
   vector<bool>    *tau_IsTight;
   Int_t           n_pho;
   vector<float>   *pho_Pt;
   vector<float>   *pho_Eta;
   vector<float>   *pho_Phi;
   vector<float>   *pho_E;
   vector<float>   *pho_sminor;
   vector<float>   *pho_smajor;
   vector<int>     *pho_convType;
   vector<float>   *pho_convTrkZ;
   vector<bool>    *pho_passCutBasedIDLoose;
   vector<bool>    *pho_passCutBasedIDMedium;
   vector<bool>    *pho_passCutBasedIDTight;
   vector<unsigned int> *pho_SeedRechitID;
   vector<unsigned int> *pho_SeedRechitIndex;
   vector<vector<unsigned int> > *pho_EcalRechitIDs;
   vector<vector<unsigned int> > *pho_EcalRechitIndices;
   Int_t           n_l1jet;
   vector<float>   *l1jet_Pt;
   vector<float>   *l1jet_Eta;
   vector<float>   *l1jet_Phi;
   vector<float>   *l1jet_E;
   vector<float>   *l1jet_hwQual;
   Int_t           n_jet;
   vector<float>   *jet_Pt;
   vector<float>   *jet_Eta;
   vector<float>   *jet_Phi;
   vector<float>   *jet_E;
   vector<float>   *jet_Mass;
   vector<float>   *jet_JetArea;
   vector<float>   *jet_ChargedHadEFrac;
   vector<float>   *jet_NeutralHadEFrac;
   vector<float>   *jet_PhoEFrac;
   vector<float>   *jet_EleEFrac;
   vector<float>   *jet_MuonEFrac;
   vector<int>     *jet_ChargedHadMult;
   vector<int>     *jet_NeutralHadMult;
   vector<int>     *jet_PhoMult;
   vector<int>     *jet_EleMult;
   vector<int>     *jet_MuonMult;
   vector<float>   *jet_PtAllTracks;
   vector<float>   *jet_PtAllPVTracks;
   vector<int>     *jet_NVertexTracks;
   vector<int>     *jet_NSelectedTracks;
   vector<int>     *jet_NSV;
   vector<int>     *jet_NSVCand;
   vector<float>   *jet_SV_x;
   vector<float>   *jet_SV_y;
   vector<float>   *jet_SV_z;
   vector<int>     *jet_SV_NTracks;
   vector<float>   *jet_SV_Mass;
   vector<float>   *jet_SV_DRJet;
   vector<float>   *jet_FlightDist2D;
   vector<float>   *jet_FlightDist2DErr;
   vector<float>   *jet_FlightDist3D;
   vector<float>   *jet_FlightDist3DErr;
   vector<bool>    *jet_PassIDLoose;
   vector<bool>    *jet_PassIDTight;
   vector<int>     *jet_PartonFlavor;
   vector<int>     *jet_HadronFlavor;
   vector<float>   *jet_PtAllTracks_wp;
   vector<float>   *jet_PtAllPVTracks_wp;
   vector<float>   *jet_MedianIP_wp;
   vector<float>   *jet_MinDeltaRAllTracks_wp;
   vector<float>   *jet_MinDeltaRPVTracks_wp;
   vector<unsigned int> *jet_NTracks;
   vector<vector<unsigned int> > *jet_TrackIndices;
   vector<unsigned int> *jet_NPFCands;
   vector<vector<unsigned int> > *jet_PFCandIndices;
   vector<unsigned int> *jet_SeedHcalRechitID;
   vector<unsigned int> *jet_SeedHcalRechitIndex;
   vector<vector<unsigned int> > *jet_EcalRechitIDs;
   vector<vector<unsigned int> > *jet_EcalRechitIndices;
   vector<vector<unsigned int> > *jet_HcalRechitIDs;
   vector<vector<unsigned int> > *jet_HcalRechitIndices;
   Int_t           n_calojet;
   vector<float>   *calojet_Pt;
   vector<float>   *calojet_Eta;
   vector<float>   *calojet_Phi;
   vector<float>   *calojet_E;
   vector<float>   *calojet_Mass;
   vector<int>     *calojet_NPFCands;
   vector<vector<int> > *calojet_PFCandIndices;
   vector<unsigned int> *calojet_SeedHcalRechitID;
   vector<unsigned int> *calojet_SeedHcalRechitIndex;
   vector<vector<unsigned int> > *calojet_EcalRechitIDs;
   vector<vector<unsigned int> > *calojet_EcalRechitIndices;
   vector<vector<unsigned int> > *calojet_HcalRechitIDs;
   vector<vector<unsigned int> > *calojet_HcalRechitIndices;
   Int_t           n_LRJet;
   vector<float>   *LRJet_Pt;
   vector<float>   *LRJet_Eta;
   vector<float>   *LRJet_Phi;
   vector<float>   *LRJet_E;
   vector<float>   *LRJet_TrimmedM;
   vector<float>   *LRJet_PrunedM;
   vector<float>   *LRJet_FilteredM;
   vector<float>   *LRJet_SoftDropM;
   vector<float>   *LRJet_JetArea;
   vector<float>   *LRJet_PtAllTracks;
   vector<float>   *LRJet_PtAllPVTracks;
   vector<int>     *LRJet_NVertexTracks;
   vector<int>     *LRJet_NSelectedTracks;
   vector<float>   *LRJet_ChargedHadEFrac;
   vector<float>   *LRJet_NeutralHadEFrac;
   vector<float>   *LRJet_EleEFrac;
   vector<float>   *LRJet_PhoEFrac;
   vector<int>     *LRJet_EleMult;
   vector<int>     *LRJet_PhoMult;
   vector<int>     *LRJet_MuonMult;
   vector<int>     *LRJet_NSV;
   vector<int>     *LRJet_NSVCand;
   vector<float>   *LRJet_SV_x;
   vector<float>   *LRJet_SV_y;
   vector<float>   *LRJet_SV_z;
   vector<int>     *LRJet_SV_NTracks;
   vector<float>   *LRJet_SV_Mass;
   vector<float>   *LRJet_SV_DRJet;
   vector<float>   *LRJet_FlightDist2D;
   vector<float>   *LRJet_FlightDist2DErr;
   vector<float>   *LRJet_FlightDist3D;
   vector<float>   *LRJet_FlightDist3DErr;
   vector<bool>    *LRJet_PassIDLoose;
   vector<bool>    *LRJet_PassIDTight;
   vector<int>     *LRJet_NPFCands;
   vector<vector<int> > *LRJet_PFCandIndices;
   vector<unsigned int> *LRJet_SeedHcalRechitID;
   vector<unsigned int> *LRJet_SeedHcalRechitIndex;
   vector<vector<unsigned int> > *LRJet_EcalRechitIDs;
   vector<vector<unsigned int> > *LRJet_EcalRechitIndices;
   vector<vector<unsigned int> > *LRJet_HcalRechitIDs;
   vector<vector<unsigned int> > *LRJet_HcalRechitIndices;
   Int_t           n_caloLRJet;
   vector<float>   *caloLRJet_Pt;
   vector<float>   *caloLRJet_Eta;
   vector<float>   *caloLRJet_Phi;
   vector<float>   *caloLRJet_E;
   vector<float>   *caloLRJet_Mass;
   vector<int>     *caloLRJet_NPFCands;
   vector<vector<int> > *caloLRJet_PFCandIndices;
   vector<unsigned int> *caloLRJet_SeedHcalRechitID;
   vector<unsigned int> *caloLRJet_SeedHcalRechitIndex;
   vector<vector<unsigned int> > *caloLRJet_EcalRechitIDs;
   vector<vector<unsigned int> > *caloLRJet_EcalRechitIndices;
   vector<vector<unsigned int> > *caloLRJet_HcalRechitIDs;
   vector<vector<unsigned int> > *caloLRJet_HcalRechitIndices;
   Int_t           n_track;
   vector<float>   *track_Pt;
   vector<float>   *track_Eta;
   vector<float>   *track_Phi;
   vector<int>     *track_Charge;
   vector<int>     *track_bestVertexIndex;
   vector<int>     *track_nMissingInnerHits;
   vector<int>     *track_nMissingOuterHits;
   vector<int>     *track_nPixelHits;
   vector<int>     *track_nHits;
   vector<float>   *track_angle;
   vector<float>   *track_dxyToBS;
   vector<float>   *track_dxyErr;
   vector<float>   *track_dzToPV;
   vector<float>   *track_dzErr;
   vector<float>   *track_chi2;
   vector<int>     *track_ndof;
   vector<int>     *track_index;
   Int_t           n_secVtx;
   vector<float>   *secVtx_Pt;
   vector<float>   *secVtx_Eta;
   vector<float>   *secVtx_Phi;
   vector<int>     *secVtx_Charge;
   vector<int>     *secVtx_nConstituents;
   vector<float>   *secVtx_X;
   vector<float>   *secVtx_Y;
   vector<float>   *secVtx_Z;
   vector<float>   *secVtx_Distance;
   vector<float>   *secVtx_DistanceError;
   Int_t           n_PFCand;
   vector<float>   *PFCand_Pt;
   vector<float>   *PFCand_Eta;
   vector<float>   *PFCand_Phi;
   vector<int>     *PFCand_PdgId;
   vector<int>     *PFCand_TrackIndex;
   vector<int>     *PFCand_GeneralTrackIndex;
   vector<int>     *PFCand_PVIndex;
   Int_t           n_ecalRechit;
   Int_t           n_hbheRechit;
   vector<float>   *hbheRechit_Eta;
   vector<float>   *hbheRechit_Phi;
   vector<float>   *hbheRechit_E;
   vector<float>   *hbheRechit_X;
   vector<float>   *hbheRechit_Y;
   vector<float>   *hbheRechit_Z;
   vector<float>   *hbheRechit_time;
   vector<float>   *hbheRechit_auxTDC;
   vector<float>   *hbheRechit_timeFalling;
   vector<int>     *hbheRechit_iEta;
   vector<int>     *hbheRechit_iPhi;
   vector<int>     *hbheRechit_depth;
   Int_t           n_hoRechit;
   vector<float>   *hoRechit_Eta;
   vector<float>   *hoRechit_Phi;
   vector<float>   *hoRechit_E;
   vector<float>   *hoRechit_X;
   vector<float>   *hoRechit_Y;
   vector<float>   *hoRechit_Z;
   vector<float>   *hoRechit_T;
   Int_t           n_gParticle;
   vector<int>     *gParticle_ParentIndex;
   vector<int>     *gParticle_ParentId;
   vector<int>     *gParticle_Status;
   vector<int>     *gParticle_Id;
   vector<float>   *gParticle_Pt;
   vector<float>   *gParticle_Px;
   vector<float>   *gParticle_Py;
   vector<float>   *gParticle_Pz;
   vector<float>   *gParticle_Eta;
   vector<float>   *gParticle_Phi;
   vector<float>   *gParticle_E;
   vector<float>   *gParticle_ProdVtx_X;
   vector<float>   *gParticle_ProdVtx_Y;
   vector<float>   *gParticle_ProdVtx_Z;
   Int_t           n_gLLP;
   vector<float>   *gLLP_Pt;
   vector<float>   *gLLP_Eta;
   vector<float>   *gLLP_Phi;
   vector<float>   *gLLP_E;
   vector<float>   *gLLP_Beta;
   vector<float>   *gLLP_TravelTime;
   vector<float>   *gLLP_DecayVtx_X;
   vector<float>   *gLLP_DecayVtx_Y;
   vector<float>   *gLLP_DecayVtx_Z;
   vector<float>   *gLLP_ProdVtx_X;
   vector<float>   *gLLP_ProdVtx_Y;
   vector<float>   *gLLP_ProdVtx_Z;

   // List of branches
   TBranch        *b_isData;   //!
   TBranch        *b_runNum;   //!
   TBranch        *b_lumiNum;   //!
   TBranch        *b_eventNum;   //!
   TBranch        *b_eventTime;   //!
   TBranch        *b_n_PV;   //!
   TBranch        *b_PV_X;   //!
   TBranch        *b_PV_Y;   //!
   TBranch        *b_PV_Z;   //!
   TBranch        *b_n_PVCand;   //!
   TBranch        *b_PVCand_X;   //!
   TBranch        *b_PVCand_Y;   //!
   TBranch        *b_PVCand_Z;   //!
   TBranch        *b_PVCand_LogSumPtSq;   //!
   TBranch        *b_PVCand_SumPx;   //!
   TBranch        *b_PVCand_SumPy;   //!
   TBranch        *b_PVTrack_Pt;   //!
   TBranch        *b_PVTrack_Eta;   //!
   TBranch        *b_PVTrack_Phi;   //!
   TBranch        *b_HLT_Decision;   //!
   TBranch        *b_HLT_Prescale;   //!
   TBranch        *b_met_Pt;   //!
   TBranch        *b_met_Phi;   //!
   TBranch        *b_met_SumEt;   //!
   TBranch        *b_n_ele;   //!
   TBranch        *b_ele_Pt;   //!
   TBranch        *b_ele_Eta;   //!
   TBranch        *b_ele_Phi;   //!
   TBranch        *b_ele_E;   //!
   TBranch        *b_ele_Charge;   //!
   TBranch        *b_ele_d0;   //!
   TBranch        *b_ele_dZ;   //!
   TBranch        *b_ele_dEta;   //!
   TBranch        *b_ele_dPhi;   //!
   TBranch        *b_ele_passCutBasedIDLoose;   //!
   TBranch        *b_ele_passCutBasedIDMedium;   //!
   TBranch        *b_ele_passCutBasedIDTight;   //!
   TBranch        *b_ele_EcalRechitIDs;   //!
   TBranch        *b_ele_EcalRechitIndices;   //!
   TBranch        *b_ele_SeedRechitID;   //!
   TBranch        *b_ele_SeedRechitIndex;   //!
   TBranch        *b_n_muon;   //!
   TBranch        *b_muon_Pt;   //!
   TBranch        *b_muon_Eta;   //!
   TBranch        *b_muon_Phi;   //!
   TBranch        *b_muon_E;   //!
   TBranch        *b_muon_Charge;   //!
   TBranch        *b_muon_d0;   //!
   TBranch        *b_muon_dZ;   //!
   TBranch        *b_muon_IsLoose;   //!
   TBranch        *b_muon_IsMedium;   //!
   TBranch        *b_muon_IsTight;   //!
   TBranch        *b_n_tau;   //!
   TBranch        *b_tau_E;   //!
   TBranch        *b_tau_Pt;   //!
   TBranch        *b_tau_Eta;   //!
   TBranch        *b_tau_Phi;   //!
   TBranch        *b_tau_IsLoose;   //!
   TBranch        *b_tau_IsMedium;   //!
   TBranch        *b_tau_IsTight;   //!
   TBranch        *b_n_pho;   //!
   TBranch        *b_pho_Pt;   //!
   TBranch        *b_pho_Eta;   //!
   TBranch        *b_pho_Phi;   //!
   TBranch        *b_pho_E;   //!
   TBranch        *b_pho_sminor;   //!
   TBranch        *b_pho_smajor;   //!
   TBranch        *b_pho_convType;   //!
   TBranch        *b_pho_convTrkZ;   //!
   TBranch        *b_pho_passCutBasedIDLoose;   //!
   TBranch        *b_pho_passCutBasedIDMedium;   //!
   TBranch        *b_pho_passCutBasedIDTight;   //!
   TBranch        *b_pho_SeedRechitID;   //!
   TBranch        *b_pho_SeedRechitIndex;   //!
   TBranch        *b_pho_EcalRechitIDs;   //!
   TBranch        *b_pho_EcalRechitIndices;   //!
   TBranch        *b_n_l1jet;
   TBranch        *b_l1jet_Pt;
   TBranch        *b_l1jet_Eta;
   TBranch        *b_l1jet_Phi;
   TBranch        *b_l1jet_E;
   TBranch        *b_l1jet_hwQual;
   TBranch        *b_n_jet;   //!
   TBranch        *b_jet_Pt;   //!
   TBranch        *b_jet_Eta;   //!
   TBranch        *b_jet_Phi;   //!
   TBranch        *b_jet_E;   //!
   TBranch        *b_jet_Mass;   //!
   TBranch        *b_jet_JetArea;   //!
   TBranch        *b_jet_ChargedHadEFrac;   //!
   TBranch        *b_jet_NeutralHadEFrac;   //!
   TBranch        *b_jet_PhoEFrac;   //!
   TBranch        *b_jet_EleEFrac;   //!
   TBranch        *b_jet_MuonEFrac;   //!
   TBranch        *b_jet_ChargedHadMult;   //!
   TBranch        *b_jet_NeutralHadMult;   //!
   TBranch        *b_jet_PhoMult;   //!
   TBranch        *b_jet_EleMult;   //!
   TBranch        *b_jet_MuonMult;   //!
   TBranch        *b_jet_PtAllTracks;   //!
   TBranch        *b_jet_PtAllPVTracks;   //!
   TBranch        *b_jet_NVertexTracks;   //!
   TBranch        *b_jet_NSelectedTracks;   //!
   TBranch        *b_jet_NSV;   //!
   TBranch        *b_jet_NSVCand;   //!
   TBranch        *b_jet_SV_x;   //!
   TBranch        *b_jet_SV_y;   //!
   TBranch        *b_jet_SV_z;   //!
   TBranch        *b_jet_SV_NTracks;   //!
   TBranch        *b_jet_SV_Mass;   //!
   TBranch        *b_jet_SV_DRJet;   //!
   TBranch        *b_jet_FlightDist2D;   //!
   TBranch        *b_jet_FlightDist2DErr;   //!
   TBranch        *b_jet_FlightDist3D;   //!
   TBranch        *b_jet_FlightDist3DErr;   //!
   TBranch        *b_jet_PassIDLoose;   //!
   TBranch        *b_jet_PassIDTight;   //!
   TBranch        *b_jet_PartonFlavor;   //!
   TBranch        *b_jet_HadronFlavor;   //!
   TBranch        *b_jet_PtAllTracks_wp;   //!
   TBranch        *b_jet_PtAllPVTracks_wp;   //!
   TBranch        *b_jet_MedianIP_wp;   //!
   TBranch        *b_jet_MinDeltaRAllTracks_wp;   //!
   TBranch        *b_jet_MinDeltaRPVTracks_wp;   //!
   TBranch        *b_jet_NTracks;   //!
   TBranch        *b_jet_TrackIndices;   //!
   TBranch        *b_jet_NPFCands;   //!
   TBranch        *b_jet_PFCandIndices;   //!
   TBranch        *b_jet_SeedHcalRechitID;   //!
   TBranch        *b_jet_SeedHcalRechitIndex;   //!
   TBranch        *b_jet_EcalRechitIDs;   //!
   TBranch        *b_jet_EcalRechitIndices;   //!
   TBranch        *b_jet_HcalRechitIDs;   //!
   TBranch        *b_jet_HcalRechitIndices;   //!
   TBranch        *b_n_calojet;   //!
   TBranch        *b_calojet_Pt;   //!
   TBranch        *b_calojet_Eta;   //!
   TBranch        *b_calojet_Phi;   //!
   TBranch        *b_calojet_E;   //!
   TBranch        *b_calojet_Mass;   //!
   TBranch        *b_calojet_NPFCands;   //!
   TBranch        *b_calojet_PFCandIndices;   //!
   TBranch        *b_calojet_SeedHcalRechitID;   //!
   TBranch        *b_calojet_SeedHcalRechitIndex;   //!
   TBranch        *b_calojet_EcalRechitIDs;   //!
   TBranch        *b_calojet_EcalRechitIndices;   //!
   TBranch        *b_calojet_HcalRechitIDs;   //!
   TBranch        *b_calojet_HcalRechitIndices;   //!
   TBranch        *b_n_LRJet;   //!
   TBranch        *b_LRJet_Pt;   //!
   TBranch        *b_LRJet_Eta;   //!
   TBranch        *b_LRJet_Phi;   //!
   TBranch        *b_LRJet_E;   //!
   TBranch        *b_LRJet_TrimmedM;   //!
   TBranch        *b_LRJet_PrunedM;   //!
   TBranch        *b_LRJet_FilteredM;   //!
   TBranch        *b_LRJet_SoftDropM;   //!
   TBranch        *b_LRJet_JetArea;   //!
   TBranch        *b_LRJet_PtAllTracks;   //!
   TBranch        *b_LRJet_PtAllPVTracks;   //!
   TBranch        *b_LRJet_NVertexTracks;   //!
   TBranch        *b_LRJet_NSelectedTracks;   //!
   TBranch        *b_LRJet_ChargedHadEFrac;   //!
   TBranch        *b_LRJet_NeutralHadEFrac;   //!
   TBranch        *b_LRJet_EleEFrac;   //!
   TBranch        *b_LRJet_PhoEFrac;   //!
   TBranch        *b_LRJet_EleMult;   //!
   TBranch        *b_LRJet_PhoMult;   //!
   TBranch        *b_LRJet_MuonMult;   //!
   TBranch        *b_LRJet_NSV;   //!
   TBranch        *b_LRJet_NSVCand;   //!
   TBranch        *b_LRJet_SV_x;   //!
   TBranch        *b_LRJet_SV_y;   //!
   TBranch        *b_LRJet_SV_z;   //!
   TBranch        *b_LRJet_SV_NTracks;   //!
   TBranch        *b_LRJet_SV_Mass;   //!
   TBranch        *b_LRJet_SV_DRJet;   //!
   TBranch        *b_LRJet_FlightDist2D;   //!
   TBranch        *b_LRJet_FlightDist2DErr;   //!
   TBranch        *b_LRJet_FlightDist3D;   //!
   TBranch        *b_LRJet_FlightDist3DErr;   //!
   TBranch        *b_LRJet_PassIDLoose;   //!
   TBranch        *b_LRJet_PassIDTight;   //!
   TBranch        *b_LRJet_NPFCands;   //!
   TBranch        *b_LRJet_PFCandIndices;   //!
   TBranch        *b_LRJet_SeedHcalRechitID;   //!
   TBranch        *b_LRJet_SeedHcalRechitIndex;   //!
   TBranch        *b_LRJet_EcalRechitIDs;   //!
   TBranch        *b_LRJet_EcalRechitIndices;   //!
   TBranch        *b_LRJet_HcalRechitIDs;   //!
   TBranch        *b_LRJet_HcalRechitIndices;   //!
   TBranch        *b_n_caloLRJet;   //!
   TBranch        *b_caloLRJet_Pt;   //!
   TBranch        *b_caloLRJet_Eta;   //!
   TBranch        *b_caloLRJet_Phi;   //!
   TBranch        *b_caloLRJet_E;   //!
   TBranch        *b_caloLRJet_Mass;   //!
   TBranch        *b_caloLRJet_NPFCands;   //!
   TBranch        *b_caloLRJet_PFCandIndices;   //!
   TBranch        *b_caloLRJet_SeedHcalRechitID;   //!
   TBranch        *b_caloLRJet_SeedHcalRechitIndex;   //!
   TBranch        *b_caloLRJet_EcalRechitIDs;   //!
   TBranch        *b_caloLRJet_EcalRechitIndices;   //!
   TBranch        *b_caloLRJet_HcalRechitIDs;   //!
   TBranch        *b_caloLRJet_HcalRechitIndices;   //!
   TBranch        *b_n_track;   //!
   TBranch        *b_track_Pt;   //!
   TBranch        *b_track_Eta;   //!
   TBranch        *b_track_Phi;   //!
   TBranch        *b_track_Charge;   //!
   TBranch        *b_track_bestVertexIndex;   //!
   TBranch        *b_track_nMissingInnerHits;   //!
   TBranch        *b_track_nMissingOuterHits;   //!
   TBranch        *b_track_nPixelHits;   //!
   TBranch        *b_track_nHits;   //!
   TBranch        *b_track_angle;   //!
   TBranch        *b_track_dxyToBS;   //!
   TBranch        *b_track_dxyErr;   //!
   TBranch        *b_track_dzToPV;   //!
   TBranch        *b_track_dzErr;   //!
   TBranch        *b_track_chi2;   //!
   TBranch        *b_track_ndof;   //!
   TBranch        *b_track_index;   //!
   TBranch        *b_n_secVtx;   //!
   TBranch        *b_secVtx_Pt;   //!
   TBranch        *b_secVtx_Eta;   //!
   TBranch        *b_secVtx_Phi;   //!
   TBranch        *b_secVtx_Charge;   //!
   TBranch        *b_secVtx_nConstituents;   //!
   TBranch        *b_secVtx_X;   //!
   TBranch        *b_secVtx_Y;   //!
   TBranch        *b_secVtx_Z;   //!
   TBranch        *b_secVtx_Distance;   //!
   TBranch        *b_secVtx_DistanceError;   //!
   TBranch        *b_n_PFCand;   //!
   TBranch        *b_PFCand_Pt;   //!
   TBranch        *b_PFCand_Eta;   //!
   TBranch        *b_PFCand_Phi;   //!
   TBranch        *b_PFCand_PdgId;   //!
   TBranch        *b_PFCand_TrackIndex;   //!
   TBranch        *b_PFCand_GeneralTrackIndex;   //!
   TBranch        *b_PFCand_PVIndex;   //!
   TBranch        *b_n_ecalRechit;   //!
   TBranch        *b_n_hbheRechit;   //!
   TBranch        *b_hbheRechit_Eta;   //!
   TBranch        *b_hbheRechit_Phi;   //!
   TBranch        *b_hbheRechit_E;   //!
   TBranch        *b_hbheRechit_X;   //!
   TBranch        *b_hbheRechit_Y;   //!
   TBranch        *b_hbheRechit_Z;   //!
   TBranch        *b_hbheRechit_time;   //!
   TBranch        *b_hbheRechit_auxTDC;   //!
   TBranch        *b_hbheRechit_timeFalling;   //!
   TBranch        *b_hbheRechit_iEta;   //!
   TBranch        *b_hbheRechit_iPhi;   //!
   TBranch        *b_hbheRechit_depth;   //!
   TBranch        *b_n_hoRechit;   //!
   TBranch        *b_hoRechit_Eta;   //!
   TBranch        *b_hoRechit_Phi;   //!
   TBranch        *b_hoRechit_E;   //!
   TBranch        *b_hoRechit_X;   //!
   TBranch        *b_hoRechit_Y;   //!
   TBranch        *b_hoRechit_Z;   //!
   TBranch        *b_hoRechit_T;   //!
   TBranch        *b_n_gParticle;   //!
   TBranch        *b_gParticle_ParentIndex;   //!
   TBranch        *b_gParticle_ParentId;   //!
   TBranch        *b_gParticle_Status;   //!
   TBranch        *b_gParticle_Id;   //!
   TBranch        *b_gParticle_Pt;   //!
   TBranch        *b_gParticle_Px;   //!
   TBranch        *b_gParticle_Py;   //!
   TBranch        *b_gParticle_Pz;   //!
   TBranch        *b_gParticle_Eta;   //!
   TBranch        *b_gParticle_Phi;   //!
   TBranch        *b_gParticle_E;   //!
   TBranch        *b_gParticle_ProdVtx_X;   //!
   TBranch        *b_gParticle_ProdVtx_Y;   //!
   TBranch        *b_gParticle_ProdVtx_Z;   //!
   TBranch        *b_n_gLLP;   //! 
   TBranch        *b_gLLP_Pt;   //!
   TBranch        *b_gLLP_Eta;   //!
   TBranch        *b_gLLP_Phi;   //!
   TBranch        *b_gLLP_E;   //!
   TBranch        *b_gLLP_Beta;   //!
   TBranch        *b_gLLP_TravelTime;   //!
   TBranch        *b_gLLP_DecayVtx_X;   //!
   TBranch        *b_gLLP_DecayVtx_Y;   //!
   TBranch        *b_gLLP_DecayVtx_Z;   //!
   TBranch        *b_gLLP_ProdVtx_X;   //!
   TBranch        *b_gLLP_ProdVtx_Y;   //!
   TBranch        *b_gLLP_ProdVtx_Z;   //!

   DisplacedHcalJetAnalyzer(TTree *tree=0);
   virtual ~DisplacedHcalJetAnalyzer();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   // DisplacedHcalJetAnalyzer.C
   virtual void   Initialize( string infiletag, string infilepath );
   // Loop.cxx
   virtual void   Loop();
   virtual void   ProcessEvent( Long64_t jentry );
   // TriggerHelper.cxx
   virtual void   SetTriggerNames( string infilepath, string hist_name );
   // Object Helper.cxx
   virtual float  DeltaR( float eta1, float eta2, float phi1, float phi2);
   virtual double deltaPhi( double phi1, double phi2);
   // virtual double deltaR( double eta1, double phi1, double eta2, double phi2);
   virtual vector<int> GetRechitMult( int idx_llp, float deltaR_cut );
   virtual vector<vector<float>> GetEnergyProfile( int idx_llp, float deltaR_cut );
   virtual vector<float> GetMatchedHcalRechits_Jet( int idx_jet, float deltaR_cut );
   virtual vector<float> GetEnergyProfile_Jet( int idx_jet, float deltaR_cut );
   virtual vector<pair<float,int>> Get3RechitE_Jet( int idx_jet, float deltaR_cut );
   virtual vector<float> GetEtaPhiSpread_Jet( int idx_jet, float deltaR_cut );
   virtual vector<float> GetTDCavg_Jet( int idx_jet, float deltaR_cut );
   // TruthInfoHelper.cxx
   virtual void   SetLLPVariables();
   virtual vector<float> GetMatchedHcalRechits_LLPDecay( int idx_llp, int idx_llp_decay, float deltaR_cut=0.4 );
   virtual vector<float> GetMatchedHcalRechits_LLP( int idx_llp, float deltaR_cut=0.4 );
   virtual float  DeltaR_LLP_b( int idx_llp, int idx_llp_decay);
   virtual vector<float> JetIsMatchedTo( float jet_eta, float jet_phi, float deltaR_cut=0.4 );
   virtual pair<bool,float> LLPDecayIsTruthMatched_LLP_b( int idx_gLLP, int idx_gParticle, float jetPt_cut=0, float deltaR_cut=0.4 );
   virtual pair<bool,float> LLPIsTruthMatched( int idx_gLLPDecay, float jetPt_cut=0, float deltaR_cut=0.4 );
   virtual vector<TVector3> GetLLPDecayProdCoords(int idx_llp, int idx_llp_decay, vector<float> intersection_depths); // Deprecated
   // EventHelper.cxx
   virtual float  GetEventRuntime( clock_t clock_start, Long64_t init_entry, Long64_t current_entry );
   virtual void   ResetGlobalEventVars();
   // OutputHelper.cxx
   virtual void   DeclareOutputTrees();
   virtual void   ResetOutputBranches( string treename );
   virtual void   FillOutputTrees( string treename );
   virtual void   WriteOutputTrees();
   virtual void   SetHistCategories();
   virtual void   BookHists();
   virtual void   FillHists(string cat = "");
   virtual void   FillTriggerMatchHists(string cat = "");
   virtual void   WriteHists();   
};

#endif

#ifdef DisplacedHcalJetAnalyzer_cxx
DisplacedHcalJetAnalyzer::DisplacedHcalJetAnalyzer(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      cout<<"Error: Tree not specified"<<endl;
      return;
   }

   Init(tree);
}

DisplacedHcalJetAnalyzer::~DisplacedHcalJetAnalyzer()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t DisplacedHcalJetAnalyzer::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t DisplacedHcalJetAnalyzer::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void DisplacedHcalJetAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   PVCand_X = 0;
   PVCand_Y = 0;
   PVCand_Z = 0;
   PVCand_LogSumPtSq = 0;
   PVCand_SumPx = 0;
   PVCand_SumPy = 0;
   PVTrack_Pt = 0;
   PVTrack_Eta = 0;
   PVTrack_Phi = 0;
   HLT_Decision = 0;
   HLT_Prescale = 0;
   ele_Pt = 0;
   ele_Eta = 0;
   ele_Phi = 0;
   ele_E = 0;
   ele_Charge = 0;
   ele_d0 = 0;
   ele_dZ = 0;
   ele_dEta = 0;
   ele_dPhi = 0;
   ele_passCutBasedIDLoose = 0;
   ele_passCutBasedIDMedium = 0;
   ele_passCutBasedIDTight = 0;
   ele_EcalRechitIDs = 0;
   ele_EcalRechitIndices = 0;
   ele_SeedRechitID = 0;
   ele_SeedRechitIndex = 0;
   muon_Pt = 0;
   muon_Eta = 0;
   muon_Phi = 0;
   muon_E = 0;
   muon_Charge = 0;
   muon_d0 = 0;
   muon_dZ = 0;
   muon_IsLoose = 0;
   muon_IsMedium = 0;
   muon_IsTight = 0;
   tau_E = 0;
   tau_Pt = 0;
   tau_Eta = 0;
   tau_Phi = 0;
   tau_IsLoose = 0;
   tau_IsMedium = 0;
   tau_IsTight = 0;
   pho_Pt = 0;
   pho_Eta = 0;
   pho_Phi = 0;
   pho_E = 0;
   pho_sminor = 0;
   pho_smajor = 0;
   pho_convType = 0;
   pho_convTrkZ = 0;
   pho_passCutBasedIDLoose = 0;
   pho_passCutBasedIDMedium = 0;
   pho_passCutBasedIDTight = 0;
   pho_SeedRechitID = 0;
   pho_SeedRechitIndex = 0;
   pho_EcalRechitIDs = 0;
   pho_EcalRechitIndices = 0;
   l1jet_Pt = 0;
   l1jet_Eta = 0;
   l1jet_Phi = 0;
   l1jet_E = 0;
   l1jet_hwQual = 0;
   jet_Pt = 0;
   jet_Eta = 0;
   jet_Phi = 0;
   jet_E = 0;
   jet_Mass = 0;
   jet_JetArea = 0;
   jet_ChargedHadEFrac = 0;
   jet_NeutralHadEFrac = 0;
   jet_PhoEFrac = 0;
   jet_EleEFrac = 0;
   jet_MuonEFrac = 0;
   jet_ChargedHadMult = 0;
   jet_NeutralHadMult = 0;
   jet_PhoMult = 0;
   jet_EleMult = 0;
   jet_MuonMult = 0;
   jet_PtAllTracks = 0;
   jet_PtAllPVTracks = 0;
   jet_NVertexTracks = 0;
   jet_NSelectedTracks = 0;
   jet_NSV = 0;
   jet_NSVCand = 0;
   jet_SV_x = 0;
   jet_SV_y = 0;
   jet_SV_z = 0;
   jet_SV_NTracks = 0;
   jet_SV_Mass = 0;
   jet_SV_DRJet = 0;
   jet_FlightDist2D = 0;
   jet_FlightDist2DErr = 0;
   jet_FlightDist3D = 0;
   jet_FlightDist3DErr = 0;
   jet_PassIDLoose = 0;
   jet_PassIDTight = 0;
   jet_PartonFlavor = 0;
   jet_HadronFlavor = 0;
   jet_PtAllTracks_wp = 0;
   jet_PtAllPVTracks_wp = 0;
   jet_MedianIP_wp = 0;
   jet_MinDeltaRAllTracks_wp = 0;
   jet_MinDeltaRPVTracks_wp = 0;
   jet_NTracks = 0;
   jet_TrackIndices = 0;
   jet_NPFCands = 0;
   jet_PFCandIndices = 0;
   jet_SeedHcalRechitID = 0;
   jet_SeedHcalRechitIndex = 0;
   jet_EcalRechitIDs = 0;
   jet_EcalRechitIndices = 0;
   jet_HcalRechitIDs = 0;
   jet_HcalRechitIndices = 0;
   calojet_Pt = 0;
   calojet_Eta = 0;
   calojet_Phi = 0;
   calojet_E = 0;
   calojet_Mass = 0;
   calojet_NPFCands = 0;
   calojet_PFCandIndices = 0;
   calojet_SeedHcalRechitID = 0;
   calojet_SeedHcalRechitIndex = 0;
   calojet_EcalRechitIDs = 0;
   calojet_EcalRechitIndices = 0;
   calojet_HcalRechitIDs = 0;
   calojet_HcalRechitIndices = 0;
   LRJet_Pt = 0;
   LRJet_Eta = 0;
   LRJet_Phi = 0;
   LRJet_E = 0;
   LRJet_TrimmedM = 0;
   LRJet_PrunedM = 0;
   LRJet_FilteredM = 0;
   LRJet_SoftDropM = 0;
   LRJet_JetArea = 0;
   LRJet_PtAllTracks = 0;
   LRJet_PtAllPVTracks = 0;
   LRJet_NVertexTracks = 0;
   LRJet_NSelectedTracks = 0;
   LRJet_ChargedHadEFrac = 0;
   LRJet_NeutralHadEFrac = 0;
   LRJet_EleEFrac = 0;
   LRJet_PhoEFrac = 0;
   LRJet_EleMult = 0;
   LRJet_PhoMult = 0;
   LRJet_MuonMult = 0;
   LRJet_NSV = 0;
   LRJet_NSVCand = 0;
   LRJet_SV_x = 0;
   LRJet_SV_y = 0;
   LRJet_SV_z = 0;
   LRJet_SV_NTracks = 0;
   LRJet_SV_Mass = 0;
   LRJet_SV_DRJet = 0;
   LRJet_FlightDist2D = 0;
   LRJet_FlightDist2DErr = 0;
   LRJet_FlightDist3D = 0;
   LRJet_FlightDist3DErr = 0;
   LRJet_PassIDLoose = 0;
   LRJet_PassIDTight = 0;
   LRJet_NPFCands = 0;
   LRJet_PFCandIndices = 0;
   LRJet_SeedHcalRechitID = 0;
   LRJet_SeedHcalRechitIndex = 0;
   LRJet_EcalRechitIDs = 0;
   LRJet_EcalRechitIndices = 0;
   LRJet_HcalRechitIDs = 0;
   LRJet_HcalRechitIndices = 0;
   caloLRJet_Pt = 0;
   caloLRJet_Eta = 0;
   caloLRJet_Phi = 0;
   caloLRJet_E = 0;
   caloLRJet_Mass = 0;
   caloLRJet_NPFCands = 0;
   caloLRJet_PFCandIndices = 0;
   caloLRJet_SeedHcalRechitID = 0;
   caloLRJet_SeedHcalRechitIndex = 0;
   caloLRJet_EcalRechitIDs = 0;
   caloLRJet_EcalRechitIndices = 0;
   caloLRJet_HcalRechitIDs = 0;
   caloLRJet_HcalRechitIndices = 0;
   track_Pt = 0;
   track_Eta = 0;
   track_Phi = 0;
   track_Charge = 0;
   track_bestVertexIndex = 0;
   track_nMissingInnerHits = 0;
   track_nMissingOuterHits = 0;
   track_nPixelHits = 0;
   track_nHits = 0;
   track_angle = 0;
   track_dxyToBS = 0;
   track_dxyErr = 0;
   track_dzToPV = 0;
   track_dzErr = 0;
   track_chi2 = 0;
   track_ndof = 0;
   track_index = 0;
   secVtx_Pt = 0;
   secVtx_Eta = 0;
   secVtx_Phi = 0;
   secVtx_Charge = 0;
   secVtx_nConstituents = 0;
   secVtx_X = 0;
   secVtx_Y = 0;
   secVtx_Z = 0;
   secVtx_Distance = 0;
   secVtx_DistanceError = 0;
   PFCand_Pt = 0;
   PFCand_Eta = 0;
   PFCand_Phi = 0;
   PFCand_PdgId = 0;
   PFCand_TrackIndex = 0;
   PFCand_GeneralTrackIndex = 0;
   PFCand_PVIndex = 0;
   hbheRechit_Eta = 0;
   hbheRechit_Phi = 0;
   hbheRechit_E = 0;
   hbheRechit_X = 0;
   hbheRechit_Y = 0;
   hbheRechit_Z = 0;
   hbheRechit_time = 0;
   hbheRechit_auxTDC = 0;
   hbheRechit_timeFalling = 0;
   hbheRechit_iEta = 0;
   hbheRechit_iPhi = 0;
   hbheRechit_depth = 0;
   hoRechit_Eta = 0;
   hoRechit_Phi = 0;
   hoRechit_E = 0;
   hoRechit_X = 0;
   hoRechit_Y = 0;
   hoRechit_Z = 0;
   hoRechit_T = 0;
   gParticle_ParentIndex = 0;
   gParticle_ParentId = 0;
   gParticle_Status = 0;
   gParticle_Id = 0;
   gParticle_Pt = 0;
   gParticle_Px = 0;
   gParticle_Py = 0;
   gParticle_Pz = 0;
   gParticle_Eta = 0;
   gParticle_Phi = 0;
   gParticle_E = 0;
   gParticle_ProdVtx_X = 0;
   gParticle_ProdVtx_Y = 0;
   gParticle_ProdVtx_Z = 0;
   gLLP_Pt = 0;
   gLLP_Eta = 0;
   gLLP_Phi = 0;
   gLLP_E = 0;
   gLLP_Beta = 0;
   gLLP_TravelTime = 0;
   gLLP_DecayVtx_X = 0;
   gLLP_DecayVtx_Y = 0;
   gLLP_DecayVtx_Z = 0;
   gLLP_ProdVtx_X = 0;
   gLLP_ProdVtx_Y = 0;
   gLLP_ProdVtx_Z = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("isData", &isData, &b_isData);
   fChain->SetBranchAddress("runNum", &runNum, &b_runNum);
   fChain->SetBranchAddress("lumiNum", &lumiNum, &b_lumiNum);
   fChain->SetBranchAddress("eventNum", &eventNum, &b_eventNum);
   fChain->SetBranchAddress("eventTime", &eventTime, &b_eventTime);
   fChain->SetBranchAddress("n_PV", &n_PV, &b_n_PV);
   fChain->SetBranchAddress("PV_X", &PV_X, &b_PV_X);
   fChain->SetBranchAddress("PV_Y", &PV_Y, &b_PV_Y);
   fChain->SetBranchAddress("PV_Z", &PV_Z, &b_PV_Z);
   fChain->SetBranchAddress("n_PVCand", &n_PVCand, &b_n_PVCand);
   fChain->SetBranchAddress("PVCand_X", &PVCand_X, &b_PVCand_X);
   fChain->SetBranchAddress("PVCand_Y", &PVCand_Y, &b_PVCand_Y);
   fChain->SetBranchAddress("PVCand_Z", &PVCand_Z, &b_PVCand_Z);
   fChain->SetBranchAddress("PVCand_LogSumPtSq", &PVCand_LogSumPtSq, &b_PVCand_LogSumPtSq);
   fChain->SetBranchAddress("PVCand_SumPx", &PVCand_SumPx, &b_PVCand_SumPx);
   fChain->SetBranchAddress("PVCand_SumPy", &PVCand_SumPy, &b_PVCand_SumPy);
//    fChain->SetBranchAddress("n_PVCand", &n_PVCand, &b_n_PVCand);
   fChain->SetBranchAddress("PVTrack_Pt", &PVTrack_Pt, &b_PVTrack_Pt);
   fChain->SetBranchAddress("PVTrack_Eta", &PVTrack_Eta, &b_PVTrack_Eta);
   fChain->SetBranchAddress("PVTrack_Phi", &PVTrack_Phi, &b_PVTrack_Phi);
   fChain->SetBranchAddress("HLT_Decision", &HLT_Decision, &b_HLT_Decision);
   fChain->SetBranchAddress("HLT_Prescale", &HLT_Prescale, &b_HLT_Prescale);
   fChain->SetBranchAddress("met_Pt", &met_Pt, &b_met_Pt);
   fChain->SetBranchAddress("met_Phi", &met_Phi, &b_met_Phi);
   fChain->SetBranchAddress("met_SumEt", &met_SumEt, &b_met_SumEt);
   fChain->SetBranchAddress("n_ele", &n_ele, &b_n_ele);
   fChain->SetBranchAddress("ele_Pt", &ele_Pt, &b_ele_Pt);
   fChain->SetBranchAddress("ele_Eta", &ele_Eta, &b_ele_Eta);
   fChain->SetBranchAddress("ele_Phi", &ele_Phi, &b_ele_Phi);
   fChain->SetBranchAddress("ele_E", &ele_E, &b_ele_E);
   fChain->SetBranchAddress("ele_Charge", &ele_Charge, &b_ele_Charge);
   fChain->SetBranchAddress("ele_d0", &ele_d0, &b_ele_d0);
   fChain->SetBranchAddress("ele_dZ", &ele_dZ, &b_ele_dZ);
   fChain->SetBranchAddress("ele_dEta", &ele_dEta, &b_ele_dEta);
   fChain->SetBranchAddress("ele_dPhi", &ele_dPhi, &b_ele_dPhi);
   fChain->SetBranchAddress("ele_passCutBasedIDLoose", &ele_passCutBasedIDLoose, &b_ele_passCutBasedIDLoose);
   fChain->SetBranchAddress("ele_passCutBasedIDMedium", &ele_passCutBasedIDMedium, &b_ele_passCutBasedIDMedium);
   fChain->SetBranchAddress("ele_passCutBasedIDTight", &ele_passCutBasedIDTight, &b_ele_passCutBasedIDTight);
   fChain->SetBranchAddress("ele_EcalRechitIDs", &ele_EcalRechitIDs, &b_ele_EcalRechitIDs);
   fChain->SetBranchAddress("ele_EcalRechitIndices", &ele_EcalRechitIndices, &b_ele_EcalRechitIndices);
   fChain->SetBranchAddress("ele_SeedRechitID", &ele_SeedRechitID, &b_ele_SeedRechitID);
   fChain->SetBranchAddress("ele_SeedRechitIndex", &ele_SeedRechitIndex, &b_ele_SeedRechitIndex);
   fChain->SetBranchAddress("n_muon", &n_muon, &b_n_muon);
   fChain->SetBranchAddress("muon_Pt", &muon_Pt, &b_muon_Pt);
   fChain->SetBranchAddress("muon_Eta", &muon_Eta, &b_muon_Eta);
   fChain->SetBranchAddress("muon_Phi", &muon_Phi, &b_muon_Phi);
   fChain->SetBranchAddress("muon_E", &muon_E, &b_muon_E);
   fChain->SetBranchAddress("muon_Charge", &muon_Charge, &b_muon_Charge);
   fChain->SetBranchAddress("muon_d0", &muon_d0, &b_muon_d0);
   fChain->SetBranchAddress("muon_dZ", &muon_dZ, &b_muon_dZ);
   fChain->SetBranchAddress("muon_IsLoose", &muon_IsLoose, &b_muon_IsLoose);
   fChain->SetBranchAddress("muon_IsMedium", &muon_IsMedium, &b_muon_IsMedium);
   fChain->SetBranchAddress("muon_IsTight", &muon_IsTight, &b_muon_IsTight);
   fChain->SetBranchAddress("n_tau", &n_tau, &b_n_tau);
   fChain->SetBranchAddress("tau_E", &tau_E, &b_tau_E);
   fChain->SetBranchAddress("tau_Pt", &tau_Pt, &b_tau_Pt);
   fChain->SetBranchAddress("tau_Eta", &tau_Eta, &b_tau_Eta);
   fChain->SetBranchAddress("tau_Phi", &tau_Phi, &b_tau_Phi);
   fChain->SetBranchAddress("tau_IsLoose", &tau_IsLoose, &b_tau_IsLoose);
   fChain->SetBranchAddress("tau_IsMedium", &tau_IsMedium, &b_tau_IsMedium);
   fChain->SetBranchAddress("tau_IsTight", &tau_IsTight, &b_tau_IsTight);
   fChain->SetBranchAddress("n_pho", &n_pho, &b_n_pho);
   fChain->SetBranchAddress("pho_Pt", &pho_Pt, &b_pho_Pt);
   fChain->SetBranchAddress("pho_Eta", &pho_Eta, &b_pho_Eta);
   fChain->SetBranchAddress("pho_Phi", &pho_Phi, &b_pho_Phi);
   fChain->SetBranchAddress("pho_E", &pho_E, &b_pho_E);
   fChain->SetBranchAddress("pho_sminor", &pho_sminor, &b_pho_sminor);
   fChain->SetBranchAddress("pho_smajor", &pho_smajor, &b_pho_smajor);
   fChain->SetBranchAddress("pho_convType", &pho_convType, &b_pho_convType);
   fChain->SetBranchAddress("pho_convTrkZ", &pho_convTrkZ, &b_pho_convTrkZ);
   fChain->SetBranchAddress("pho_passCutBasedIDLoose", &pho_passCutBasedIDLoose, &b_pho_passCutBasedIDLoose);
   fChain->SetBranchAddress("pho_passCutBasedIDMedium", &pho_passCutBasedIDMedium, &b_pho_passCutBasedIDMedium);
   fChain->SetBranchAddress("pho_passCutBasedIDTight", &pho_passCutBasedIDTight, &b_pho_passCutBasedIDTight);
   fChain->SetBranchAddress("pho_SeedRechitID", &pho_SeedRechitID, &b_pho_SeedRechitID);
   fChain->SetBranchAddress("pho_SeedRechitIndex", &pho_SeedRechitIndex, &b_pho_SeedRechitIndex);
   fChain->SetBranchAddress("pho_EcalRechitIDs", &pho_EcalRechitIDs, &b_pho_EcalRechitIDs);
   fChain->SetBranchAddress("pho_EcalRechitIndices", &pho_EcalRechitIndices, &b_pho_EcalRechitIndices);
   fChain->SetBranchAddress("n_l1jet", &n_l1jet, &b_n_l1jet);
   fChain->SetBranchAddress("l1jet_Pt", &l1jet_Pt, &b_l1jet_Pt);
   fChain->SetBranchAddress("l1jet_Eta", &l1jet_Eta, &b_l1jet_Eta);
   fChain->SetBranchAddress("l1jet_Phi", &l1jet_Phi, &b_l1jet_Phi);
   fChain->SetBranchAddress("l1jet_E", &l1jet_E, &b_l1jet_E);
   fChain->SetBranchAddress("l1jet_hwQual", &l1jet_hwQual, &b_l1jet_hwQual);
   fChain->SetBranchAddress("n_jet", &n_jet, &b_n_jet);
   fChain->SetBranchAddress("jet_Pt", &jet_Pt, &b_jet_Pt);
   fChain->SetBranchAddress("jet_Eta", &jet_Eta, &b_jet_Eta);
   fChain->SetBranchAddress("jet_Phi", &jet_Phi, &b_jet_Phi);
   fChain->SetBranchAddress("jet_E", &jet_E, &b_jet_E);
   fChain->SetBranchAddress("jet_Mass", &jet_Mass, &b_jet_Mass);
   fChain->SetBranchAddress("jet_JetArea", &jet_JetArea, &b_jet_JetArea);
   fChain->SetBranchAddress("jet_ChargedHadEFrac", &jet_ChargedHadEFrac, &b_jet_ChargedHadEFrac);
   fChain->SetBranchAddress("jet_NeutralHadEFrac", &jet_NeutralHadEFrac, &b_jet_NeutralHadEFrac);
   fChain->SetBranchAddress("jet_PhoEFrac", &jet_PhoEFrac, &b_jet_PhoEFrac);
   fChain->SetBranchAddress("jet_EleEFrac", &jet_EleEFrac, &b_jet_EleEFrac);
   fChain->SetBranchAddress("jet_MuonEFrac", &jet_MuonEFrac, &b_jet_MuonEFrac);
   fChain->SetBranchAddress("jet_ChargedHadMult", &jet_ChargedHadMult, &b_jet_ChargedHadMult);
   fChain->SetBranchAddress("jet_NeutralHadMult", &jet_NeutralHadMult, &b_jet_NeutralHadMult);
   fChain->SetBranchAddress("jet_PhoMult", &jet_PhoMult, &b_jet_PhoMult);
   fChain->SetBranchAddress("jet_EleMult", &jet_EleMult, &b_jet_EleMult);
   fChain->SetBranchAddress("jet_MuonMult", &jet_MuonMult, &b_jet_MuonMult);
   fChain->SetBranchAddress("jet_PtAllTracks", &jet_PtAllTracks, &b_jet_PtAllTracks);
   fChain->SetBranchAddress("jet_PtAllPVTracks", &jet_PtAllPVTracks, &b_jet_PtAllPVTracks);
   fChain->SetBranchAddress("jet_NVertexTracks", &jet_NVertexTracks, &b_jet_NVertexTracks);
   fChain->SetBranchAddress("jet_NSelectedTracks", &jet_NSelectedTracks, &b_jet_NSelectedTracks);
   fChain->SetBranchAddress("jet_NSV", &jet_NSV, &b_jet_NSV);
   fChain->SetBranchAddress("jet_NSVCand", &jet_NSVCand, &b_jet_NSVCand);
   fChain->SetBranchAddress("jet_SV_x", &jet_SV_x, &b_jet_SV_x);
   fChain->SetBranchAddress("jet_SV_y", &jet_SV_y, &b_jet_SV_y);
   fChain->SetBranchAddress("jet_SV_z", &jet_SV_z, &b_jet_SV_z);
   fChain->SetBranchAddress("jet_SV_NTracks", &jet_SV_NTracks, &b_jet_SV_NTracks);
   fChain->SetBranchAddress("jet_SV_Mass", &jet_SV_Mass, &b_jet_SV_Mass);
   fChain->SetBranchAddress("jet_SV_DRJet", &jet_SV_DRJet, &b_jet_SV_DRJet);
   fChain->SetBranchAddress("jet_FlightDist2D", &jet_FlightDist2D, &b_jet_FlightDist2D);
   fChain->SetBranchAddress("jet_FlightDist2DErr", &jet_FlightDist2DErr, &b_jet_FlightDist2DErr);
   fChain->SetBranchAddress("jet_FlightDist3D", &jet_FlightDist3D, &b_jet_FlightDist3D);
   fChain->SetBranchAddress("jet_FlightDist3DErr", &jet_FlightDist3DErr, &b_jet_FlightDist3DErr);
   fChain->SetBranchAddress("jet_PassIDLoose", &jet_PassIDLoose, &b_jet_PassIDLoose);
   fChain->SetBranchAddress("jet_PassIDTight", &jet_PassIDTight, &b_jet_PassIDTight);
   fChain->SetBranchAddress("jet_PartonFlavor", &jet_PartonFlavor, &b_jet_PartonFlavor);
   fChain->SetBranchAddress("jet_HadronFlavor", &jet_HadronFlavor, &b_jet_HadronFlavor);
   fChain->SetBranchAddress("jet_PtAllTracks_wp", &jet_PtAllTracks_wp, &b_jet_PtAllTracks_wp);
   fChain->SetBranchAddress("jet_PtAllPVTracks_wp", &jet_PtAllPVTracks_wp, &b_jet_PtAllPVTracks_wp);
   fChain->SetBranchAddress("jet_MedianIP_wp", &jet_MedianIP_wp, &b_jet_MedianIP_wp);
   fChain->SetBranchAddress("jet_MinDeltaRAllTracks_wp", &jet_MinDeltaRAllTracks_wp, &b_jet_MinDeltaRAllTracks_wp);
   fChain->SetBranchAddress("jet_MinDeltaRPVTracks_wp", &jet_MinDeltaRPVTracks_wp, &b_jet_MinDeltaRPVTracks_wp);
   fChain->SetBranchAddress("jet_NTracks", &jet_NTracks, &b_jet_NTracks);
   fChain->SetBranchAddress("jet_TrackIndices", &jet_TrackIndices, &b_jet_TrackIndices);
   fChain->SetBranchAddress("jet_NPFCands", &jet_NPFCands, &b_jet_NPFCands);
   fChain->SetBranchAddress("jet_PFCandIndices", &jet_PFCandIndices, &b_jet_PFCandIndices);
   fChain->SetBranchAddress("jet_SeedHcalRechitID", &jet_SeedHcalRechitID, &b_jet_SeedHcalRechitID);
   fChain->SetBranchAddress("jet_SeedHcalRechitIndex", &jet_SeedHcalRechitIndex, &b_jet_SeedHcalRechitIndex);
   fChain->SetBranchAddress("jet_EcalRechitIDs", &jet_EcalRechitIDs, &b_jet_EcalRechitIDs);
   fChain->SetBranchAddress("jet_EcalRechitIndices", &jet_EcalRechitIndices, &b_jet_EcalRechitIndices);
   fChain->SetBranchAddress("jet_HcalRechitIDs", &jet_HcalRechitIDs, &b_jet_HcalRechitIDs);
   fChain->SetBranchAddress("jet_HcalRechitIndices", &jet_HcalRechitIndices, &b_jet_HcalRechitIndices);
   fChain->SetBranchAddress("n_calojet", &n_calojet, &b_n_calojet);
   fChain->SetBranchAddress("calojet_Pt", &calojet_Pt, &b_calojet_Pt);
   fChain->SetBranchAddress("calojet_Eta", &calojet_Eta, &b_calojet_Eta);
   fChain->SetBranchAddress("calojet_Phi", &calojet_Phi, &b_calojet_Phi);
   fChain->SetBranchAddress("calojet_E", &calojet_E, &b_calojet_E);
   fChain->SetBranchAddress("calojet_Mass", &calojet_Mass, &b_calojet_Mass);
   fChain->SetBranchAddress("calojet_NPFCands", &calojet_NPFCands, &b_calojet_NPFCands);
   fChain->SetBranchAddress("calojet_PFCandIndices", &calojet_PFCandIndices, &b_calojet_PFCandIndices);
   fChain->SetBranchAddress("calojet_SeedHcalRechitID", &calojet_SeedHcalRechitID, &b_calojet_SeedHcalRechitID);
   fChain->SetBranchAddress("calojet_SeedHcalRechitIndex", &calojet_SeedHcalRechitIndex, &b_calojet_SeedHcalRechitIndex);
   fChain->SetBranchAddress("calojet_EcalRechitIDs", &calojet_EcalRechitIDs, &b_calojet_EcalRechitIDs);
   fChain->SetBranchAddress("calojet_EcalRechitIndices", &calojet_EcalRechitIndices, &b_calojet_EcalRechitIndices);
   fChain->SetBranchAddress("calojet_HcalRechitIDs", &calojet_HcalRechitIDs, &b_calojet_HcalRechitIDs);
   fChain->SetBranchAddress("calojet_HcalRechitIndices", &calojet_HcalRechitIndices, &b_calojet_HcalRechitIndices);
   fChain->SetBranchAddress("n_LRJet", &n_LRJet, &b_n_LRJet);
   fChain->SetBranchAddress("LRJet_Pt", &LRJet_Pt, &b_LRJet_Pt);
   fChain->SetBranchAddress("LRJet_Eta", &LRJet_Eta, &b_LRJet_Eta);
   fChain->SetBranchAddress("LRJet_Phi", &LRJet_Phi, &b_LRJet_Phi);
   fChain->SetBranchAddress("LRJet_E", &LRJet_E, &b_LRJet_E);
   fChain->SetBranchAddress("LRJet_TrimmedM", &LRJet_TrimmedM, &b_LRJet_TrimmedM);
   fChain->SetBranchAddress("LRJet_PrunedM", &LRJet_PrunedM, &b_LRJet_PrunedM);
   fChain->SetBranchAddress("LRJet_FilteredM", &LRJet_FilteredM, &b_LRJet_FilteredM);
   fChain->SetBranchAddress("LRJet_SoftDropM", &LRJet_SoftDropM, &b_LRJet_SoftDropM);
   fChain->SetBranchAddress("LRJet_JetArea", &LRJet_JetArea, &b_LRJet_JetArea);
   fChain->SetBranchAddress("LRJet_PtAllTracks", &LRJet_PtAllTracks, &b_LRJet_PtAllTracks);
   fChain->SetBranchAddress("LRJet_PtAllPVTracks", &LRJet_PtAllPVTracks, &b_LRJet_PtAllPVTracks);
   fChain->SetBranchAddress("LRJet_NVertexTracks", &LRJet_NVertexTracks, &b_LRJet_NVertexTracks);
   fChain->SetBranchAddress("LRJet_NSelectedTracks", &LRJet_NSelectedTracks, &b_LRJet_NSelectedTracks);
   fChain->SetBranchAddress("LRJet_ChargedHadEFrac", &LRJet_ChargedHadEFrac, &b_LRJet_ChargedHadEFrac);
   fChain->SetBranchAddress("LRJet_NeutralHadEFrac", &LRJet_NeutralHadEFrac, &b_LRJet_NeutralHadEFrac);
   fChain->SetBranchAddress("LRJet_EleEFrac", &LRJet_EleEFrac, &b_LRJet_EleEFrac);
   fChain->SetBranchAddress("LRJet_PhoEFrac", &LRJet_PhoEFrac, &b_LRJet_PhoEFrac);
   fChain->SetBranchAddress("LRJet_EleMult", &LRJet_EleMult, &b_LRJet_EleMult);
   fChain->SetBranchAddress("LRJet_PhoMult", &LRJet_PhoMult, &b_LRJet_PhoMult);
   fChain->SetBranchAddress("LRJet_MuonMult", &LRJet_MuonMult, &b_LRJet_MuonMult);
   fChain->SetBranchAddress("LRJet_NSV", &LRJet_NSV, &b_LRJet_NSV);
   fChain->SetBranchAddress("LRJet_NSVCand", &LRJet_NSVCand, &b_LRJet_NSVCand);
   fChain->SetBranchAddress("LRJet_SV_x", &LRJet_SV_x, &b_LRJet_SV_x);
   fChain->SetBranchAddress("LRJet_SV_y", &LRJet_SV_y, &b_LRJet_SV_y);
   fChain->SetBranchAddress("LRJet_SV_z", &LRJet_SV_z, &b_LRJet_SV_z);
   fChain->SetBranchAddress("LRJet_SV_NTracks", &LRJet_SV_NTracks, &b_LRJet_SV_NTracks);
   fChain->SetBranchAddress("LRJet_SV_Mass", &LRJet_SV_Mass, &b_LRJet_SV_Mass);
   fChain->SetBranchAddress("LRJet_SV_DRJet", &LRJet_SV_DRJet, &b_LRJet_SV_DRJet);
   fChain->SetBranchAddress("LRJet_FlightDist2D", &LRJet_FlightDist2D, &b_LRJet_FlightDist2D);
   fChain->SetBranchAddress("LRJet_FlightDist2DErr", &LRJet_FlightDist2DErr, &b_LRJet_FlightDist2DErr);
   fChain->SetBranchAddress("LRJet_FlightDist3D", &LRJet_FlightDist3D, &b_LRJet_FlightDist3D);
   fChain->SetBranchAddress("LRJet_FlightDist3DErr", &LRJet_FlightDist3DErr, &b_LRJet_FlightDist3DErr);
   fChain->SetBranchAddress("LRJet_PassIDLoose", &LRJet_PassIDLoose, &b_LRJet_PassIDLoose);
   fChain->SetBranchAddress("LRJet_PassIDTight", &LRJet_PassIDTight, &b_LRJet_PassIDTight);
   fChain->SetBranchAddress("LRJet_NPFCands", &LRJet_NPFCands, &b_LRJet_NPFCands);
   fChain->SetBranchAddress("LRJet_PFCandIndices", &LRJet_PFCandIndices, &b_LRJet_PFCandIndices);
   fChain->SetBranchAddress("LRJet_SeedHcalRechitID", &LRJet_SeedHcalRechitID, &b_LRJet_SeedHcalRechitID);
   fChain->SetBranchAddress("LRJet_SeedHcalRechitIndex", &LRJet_SeedHcalRechitIndex, &b_LRJet_SeedHcalRechitIndex);
   fChain->SetBranchAddress("LRJet_EcalRechitIDs", &LRJet_EcalRechitIDs, &b_LRJet_EcalRechitIDs);
   fChain->SetBranchAddress("LRJet_EcalRechitIndices", &LRJet_EcalRechitIndices, &b_LRJet_EcalRechitIndices);
   fChain->SetBranchAddress("LRJet_HcalRechitIDs", &LRJet_HcalRechitIDs, &b_LRJet_HcalRechitIDs);
   fChain->SetBranchAddress("LRJet_HcalRechitIndices", &LRJet_HcalRechitIndices, &b_LRJet_HcalRechitIndices);
   fChain->SetBranchAddress("n_caloLRJet", &n_caloLRJet, &b_n_caloLRJet);
   fChain->SetBranchAddress("caloLRJet_Pt", &caloLRJet_Pt, &b_caloLRJet_Pt);
   fChain->SetBranchAddress("caloLRJet_Eta", &caloLRJet_Eta, &b_caloLRJet_Eta);
   fChain->SetBranchAddress("caloLRJet_Phi", &caloLRJet_Phi, &b_caloLRJet_Phi);
   fChain->SetBranchAddress("caloLRJet_E", &caloLRJet_E, &b_caloLRJet_E);
   fChain->SetBranchAddress("caloLRJet_Mass", &caloLRJet_Mass, &b_caloLRJet_Mass);
   fChain->SetBranchAddress("caloLRJet_NPFCands", &caloLRJet_NPFCands, &b_caloLRJet_NPFCands);
   fChain->SetBranchAddress("caloLRJet_PFCandIndices", &caloLRJet_PFCandIndices, &b_caloLRJet_PFCandIndices);
   fChain->SetBranchAddress("caloLRJet_SeedHcalRechitID", &caloLRJet_SeedHcalRechitID, &b_caloLRJet_SeedHcalRechitID);
   fChain->SetBranchAddress("caloLRJet_SeedHcalRechitIndex", &caloLRJet_SeedHcalRechitIndex, &b_caloLRJet_SeedHcalRechitIndex);
   fChain->SetBranchAddress("caloLRJet_EcalRechitIDs", &caloLRJet_EcalRechitIDs, &b_caloLRJet_EcalRechitIDs);
   fChain->SetBranchAddress("caloLRJet_EcalRechitIndices", &caloLRJet_EcalRechitIndices, &b_caloLRJet_EcalRechitIndices);
   fChain->SetBranchAddress("caloLRJet_HcalRechitIDs", &caloLRJet_HcalRechitIDs, &b_caloLRJet_HcalRechitIDs);
   fChain->SetBranchAddress("caloLRJet_HcalRechitIndices", &caloLRJet_HcalRechitIndices, &b_caloLRJet_HcalRechitIndices);
   fChain->SetBranchAddress("n_track", &n_track, &b_n_track);
   fChain->SetBranchAddress("track_Pt", &track_Pt, &b_track_Pt);
   fChain->SetBranchAddress("track_Eta", &track_Eta, &b_track_Eta);
   fChain->SetBranchAddress("track_Phi", &track_Phi, &b_track_Phi);
   fChain->SetBranchAddress("track_Charge", &track_Charge, &b_track_Charge);
   fChain->SetBranchAddress("track_bestVertexIndex", &track_bestVertexIndex, &b_track_bestVertexIndex);
   fChain->SetBranchAddress("track_nMissingInnerHits", &track_nMissingInnerHits, &b_track_nMissingInnerHits);
   fChain->SetBranchAddress("track_nMissingOuterHits", &track_nMissingOuterHits, &b_track_nMissingOuterHits);
   fChain->SetBranchAddress("track_nPixelHits", &track_nPixelHits, &b_track_nPixelHits);
   fChain->SetBranchAddress("track_nHits", &track_nHits, &b_track_nHits);
   fChain->SetBranchAddress("track_angle", &track_angle, &b_track_angle);
   fChain->SetBranchAddress("track_dxyToBS", &track_dxyToBS, &b_track_dxyToBS);
   fChain->SetBranchAddress("track_dxyErr", &track_dxyErr, &b_track_dxyErr);
   fChain->SetBranchAddress("track_dzToPV", &track_dzToPV, &b_track_dzToPV);
   fChain->SetBranchAddress("track_dzErr", &track_dzErr, &b_track_dzErr);
   fChain->SetBranchAddress("track_chi2", &track_chi2, &b_track_chi2);
   fChain->SetBranchAddress("track_ndof", &track_ndof, &b_track_ndof);
   fChain->SetBranchAddress("track_index", &track_index, &b_track_index);
   fChain->SetBranchAddress("n_secVtx", &n_secVtx, &b_n_secVtx);
   fChain->SetBranchAddress("secVtx_Pt", &secVtx_Pt, &b_secVtx_Pt);
   fChain->SetBranchAddress("secVtx_Eta", &secVtx_Eta, &b_secVtx_Eta);
   fChain->SetBranchAddress("secVtx_Phi", &secVtx_Phi, &b_secVtx_Phi);
   fChain->SetBranchAddress("secVtx_Charge", &secVtx_Charge, &b_secVtx_Charge);
   fChain->SetBranchAddress("secVtx_nConstituents", &secVtx_nConstituents, &b_secVtx_nConstituents);
   fChain->SetBranchAddress("secVtx_X", &secVtx_X, &b_secVtx_X);
   fChain->SetBranchAddress("secVtx_Y", &secVtx_Y, &b_secVtx_Y);
   fChain->SetBranchAddress("secVtx_Z", &secVtx_Z, &b_secVtx_Z);
   fChain->SetBranchAddress("secVtx_Distance", &secVtx_Distance, &b_secVtx_Distance);
   fChain->SetBranchAddress("secVtx_DistanceError", &secVtx_DistanceError, &b_secVtx_DistanceError);
   fChain->SetBranchAddress("n_PFCand", &n_PFCand, &b_n_PFCand);
   fChain->SetBranchAddress("PFCand_Pt", &PFCand_Pt, &b_PFCand_Pt);
   fChain->SetBranchAddress("PFCand_Eta", &PFCand_Eta, &b_PFCand_Eta);
   fChain->SetBranchAddress("PFCand_Phi", &PFCand_Phi, &b_PFCand_Phi);
   fChain->SetBranchAddress("PFCand_PdgId", &PFCand_PdgId, &b_PFCand_PdgId);
   fChain->SetBranchAddress("PFCand_TrackIndex", &PFCand_TrackIndex, &b_PFCand_TrackIndex);
   fChain->SetBranchAddress("PFCand_GeneralTrackIndex", &PFCand_GeneralTrackIndex, &b_PFCand_GeneralTrackIndex);
   fChain->SetBranchAddress("PFCand_PVIndex", &PFCand_PVIndex, &b_PFCand_PVIndex);
   fChain->SetBranchAddress("n_ecalRechit", &n_ecalRechit, &b_n_ecalRechit);
   fChain->SetBranchAddress("n_hbheRechit", &n_hbheRechit, &b_n_hbheRechit);
   fChain->SetBranchAddress("hbheRechit_Eta", &hbheRechit_Eta, &b_hbheRechit_Eta);
   fChain->SetBranchAddress("hbheRechit_Phi", &hbheRechit_Phi, &b_hbheRechit_Phi);
   fChain->SetBranchAddress("hbheRechit_E", &hbheRechit_E, &b_hbheRechit_E);
   fChain->SetBranchAddress("hbheRechit_X", &hbheRechit_X, &b_hbheRechit_X);
   fChain->SetBranchAddress("hbheRechit_Y", &hbheRechit_Y, &b_hbheRechit_Y);
   fChain->SetBranchAddress("hbheRechit_Z", &hbheRechit_Z, &b_hbheRechit_Z);
   fChain->SetBranchAddress("hbheRechit_time", &hbheRechit_time, &b_hbheRechit_time);
   fChain->SetBranchAddress("hbheRechit_auxTDC", &hbheRechit_auxTDC, &b_hbheRechit_auxTDC);
   fChain->SetBranchAddress("hbheRechit_timeFalling", &hbheRechit_timeFalling, &b_hbheRechit_timeFalling);
   fChain->SetBranchAddress("hbheRechit_iEta", &hbheRechit_iEta, &b_hbheRechit_iEta);
   fChain->SetBranchAddress("hbheRechit_iPhi", &hbheRechit_iPhi, &b_hbheRechit_iPhi);
   fChain->SetBranchAddress("hbheRechit_depth", &hbheRechit_depth, &b_hbheRechit_depth);
   fChain->SetBranchAddress("n_hoRechit", &n_hoRechit, &b_n_hoRechit);
   fChain->SetBranchAddress("hoRechit_Eta", &hoRechit_Eta, &b_hoRechit_Eta);
   fChain->SetBranchAddress("hoRechit_Phi", &hoRechit_Phi, &b_hoRechit_Phi);
   fChain->SetBranchAddress("hoRechit_E", &hoRechit_E, &b_hoRechit_E);
   fChain->SetBranchAddress("hoRechit_X", &hoRechit_X, &b_hoRechit_X);
   fChain->SetBranchAddress("hoRechit_Y", &hoRechit_Y, &b_hoRechit_Y);
   fChain->SetBranchAddress("hoRechit_Z", &hoRechit_Z, &b_hoRechit_Z);
   fChain->SetBranchAddress("hoRechit_T", &hoRechit_T, &b_hoRechit_T);
   fChain->SetBranchAddress("n_gParticle", &n_gParticle, &b_n_gParticle);
   fChain->SetBranchAddress("gParticle_ParentIndex", &gParticle_ParentIndex, &b_gParticle_ParentIndex);
   fChain->SetBranchAddress("gParticle_ParentId", &gParticle_ParentId, &b_gParticle_ParentId);
   fChain->SetBranchAddress("gParticle_Status", &gParticle_Status, &b_gParticle_Status);
   fChain->SetBranchAddress("gParticle_Id", &gParticle_Id, &b_gParticle_Id);
   fChain->SetBranchAddress("gParticle_Pt", &
      gParticle_Pt, &b_gParticle_Pt);
   fChain->SetBranchAddress("gParticle_Px", &gParticle_Px, &b_gParticle_Px);
   fChain->SetBranchAddress("gParticle_Py", &gParticle_Py, &b_gParticle_Py);
   fChain->SetBranchAddress("gParticle_Pz", &gParticle_Pz, &b_gParticle_Pz);
   fChain->SetBranchAddress("gParticle_Eta", &gParticle_Eta, &b_gParticle_Eta);
   fChain->SetBranchAddress("gParticle_Phi", &gParticle_Phi, &b_gParticle_Phi);
   fChain->SetBranchAddress("gParticle_E", &gParticle_E, &b_gParticle_E);
   fChain->SetBranchAddress("gParticle_ProdVtx_X", &gParticle_ProdVtx_X, &b_gParticle_ProdVtx_X);
   fChain->SetBranchAddress("gParticle_ProdVtx_Y", &gParticle_ProdVtx_Y, &b_gParticle_ProdVtx_Y);
   fChain->SetBranchAddress("gParticle_ProdVtx_Z", &gParticle_ProdVtx_Z, &b_gParticle_ProdVtx_Z);
   fChain->SetBranchAddress("n_gLLP", &n_gLLP, &b_n_gLLP);
   fChain->SetBranchAddress("gLLP_Pt", &gLLP_Pt, &b_gLLP_Pt);
   fChain->SetBranchAddress("gLLP_Eta", &gLLP_Eta, &b_gLLP_Eta);
   fChain->SetBranchAddress("gLLP_Phi", &gLLP_Phi, &b_gLLP_Phi);
   fChain->SetBranchAddress("gLLP_E", &gLLP_E, &b_gLLP_E);
   fChain->SetBranchAddress("gLLP_Beta", &gLLP_Beta, &b_gLLP_Beta);
   fChain->SetBranchAddress("gLLP_TravelTime", &gLLP_TravelTime, &b_gLLP_TravelTime);
   fChain->SetBranchAddress("gLLP_DecayVtx_X", &gLLP_DecayVtx_X, &b_gLLP_DecayVtx_X);
   fChain->SetBranchAddress("gLLP_DecayVtx_Y", &gLLP_DecayVtx_Y, &b_gLLP_DecayVtx_Y);
   fChain->SetBranchAddress("gLLP_DecayVtx_Z", &gLLP_DecayVtx_Z, &b_gLLP_DecayVtx_Z);
   fChain->SetBranchAddress("gLLP_ProdVtx_X", &gLLP_ProdVtx_X, &b_gLLP_ProdVtx_X);
   fChain->SetBranchAddress("gLLP_ProdVtx_Y", &gLLP_ProdVtx_Y, &b_gLLP_ProdVtx_Y);
   fChain->SetBranchAddress("gLLP_ProdVtx_Z", &gLLP_ProdVtx_Z, &b_gLLP_ProdVtx_Z);
   Notify();
}

Bool_t DisplacedHcalJetAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void DisplacedHcalJetAnalyzer::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t DisplacedHcalJetAnalyzer::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

#endif // #ifdef DisplacedHcalJetAnalyzer_cxx
