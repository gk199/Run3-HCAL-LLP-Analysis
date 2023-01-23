//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jan 23 21:39:48 2023 by ROOT version 6.24/07
// from TTree llp/selected AOD information for llp analyses
// found on file: /afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/llp_ntupler/displacedJetMuon_ntupler_small.root
//////////////////////////////////////////////////////////

#ifndef LLP_HCAL_Analyzer_h
#define LLP_HCAL_Analyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "string"
#include "vector"

class LLP_HCAL_Analyzer {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Bool_t          isData;
   Int_t           nPV;
   UInt_t          runNum;
   UInt_t          lumiNum;
   ULong64_t       eventNum;
   UInt_t          eventTime;
   Float_t         pvX;
   Float_t         pvY;
   Float_t         pvZ;
   Int_t           nPVAll;
   Float_t         pvAllX[54];   //[nPVAll]
   Float_t         pvAllY[54];   //[nPVAll]
   Float_t         pvAllZ[54];   //[nPVAll]
   Float_t         pvAllLogSumPtSq[54];   //[nPVAll]
   Float_t         pvAllSumPx[54];   //[nPVAll]
   Float_t         pvAllSumPy[54];   //[nPVAll]
   Int_t           nBunchXing;
   Int_t           BunchXing[1];   //[nBunchXing]
   Int_t           nPU[1];   //[nBunchXing]
   Float_t         nPUmean[1];   //[nBunchXing]
   Int_t           nMuons;
   Float_t         muonE[53];   //[nMuons]
   Float_t         muonPt[53];   //[nMuons]
   Float_t         muonEta[53];   //[nMuons]
   Float_t         muonPhi[53];   //[nMuons]
   Int_t           muonCharge[53];   //[nMuons]
   Bool_t          muonIsLoose[53];   //[nMuons]
   Bool_t          muonIsMedium[53];   //[nMuons]
   Bool_t          muonIsTight[53];   //[nMuons]
   Float_t         muon_pileupIso[53];   //[nMuons]
   Float_t         muon_chargedIso[53];   //[nMuons]
   Float_t         muon_photonIso[53];   //[nMuons]
   Float_t         muon_neutralHadIso[53];   //[nMuons]
   Int_t           nElectrons;
   Float_t         eleE[1];   //[nElectrons]
   Float_t         elePt[1];   //[nElectrons]
   Float_t         eleEta[1];   //[nElectrons]
   Float_t         elePhi[1];   //[nElectrons]
   Float_t         eleCharge[1];   //[nElectrons]
   Float_t         ele_HoverE[1];   //[nElectrons]
   Float_t         ele_pileupIso[1];   //[nElectrons]
   Float_t         ele_chargedIso[1];   //[nElectrons]
   Float_t         ele_photonIso[1];   //[nElectrons]
   Float_t         ele_neutralHadIso[1];   //[nElectrons]
   Int_t           nTaus;
   Float_t         tauE[1];   //[nTaus]
   Float_t         tauPt[1];   //[nTaus]
   Float_t         tauEta[1];   //[nTaus]
   Float_t         tauPhi[1];   //[nTaus]
   Bool_t          tau_IsLoose[1];   //[nTaus]
   Bool_t          tau_IsMedium[1];   //[nTaus]
   Bool_t          tau_IsTight[1];   //[nTaus]
   UInt_t          nPFCandidates;
   Int_t           PFCandidatePdgId[1041];   //[nPFCandidates]
   Float_t         PFCandidatePt[1041];   //[nPFCandidates]
   Float_t         PFCandidateEta[1041];   //[nPFCandidates]
   Float_t         PFCandidatePhi[1041];   //[nPFCandidates]
   Int_t           PFCandidateTrackIndex[1041];   //[nPFCandidates]
   Int_t           PFCandidatePVIndex[1041];   //[nPFCandidates]
   Int_t           nPhotons;
   Int_t           nPhotons_overlap;
   Float_t         phoE[1];   //[nPhotons]
   Float_t         phoPt[1];   //[nPhotons]
   Float_t         phoEta[1];   //[nPhotons]
   Float_t         phoPhi[1];   //[nPhotons]
   Float_t         pho_HoverE[1];   //[nPhotons]
   Float_t         pho_sumChargedHadronPtAllVertices[1][1000];   //[nPhotons]
   Float_t         pho_sumChargedHadronPt[1];   //[nPhotons]
   Float_t         pho_sumNeutralHadronEt[1];   //[nPhotons]
   Float_t         pho_sumPhotonEt[1];   //[nPhotons]
   Float_t         pho_ecalPFClusterIso[1];   //[nPhotons]
   Float_t         pho_hcalPFClusterIso[1];   //[nPhotons]
   Int_t           nCscWireDigis;
   Int_t           nCscStripDigis;
   Int_t           nCscSeg;
   Float_t         cscSegPhi[51];   //[nCscSeg]
   Float_t         cscSegEta[51];   //[nCscSeg]
   Float_t         cscSegX[51];   //[nCscSeg]
   Float_t         cscSegY[51];   //[nCscSeg]
   Float_t         cscSegZ[51];   //[nCscSeg]
   Float_t         cscSegT[51];   //[nCscSeg]
   Int_t           cscSegNRecHits[51];   //[nCscSeg]
   Int_t           ncscRechits;
   Float_t         cscRechitsPhi[1190];   //[ncscRechits]
   Float_t         cscRechitsEta[1190];   //[ncscRechits]
   Float_t         cscRechitsX[1190];   //[ncscRechits]
   Float_t         cscRechitsY[1190];   //[ncscRechits]
   Float_t         cscRechitsZ[1190];   //[ncscRechits]
   Float_t         cscRechitsE[1190];   //[ncscRechits]
   Int_t           nCscRechitClusters;
   Float_t         cscRechitCluster_match_cscSegCluster_minDeltaR[2];   //[nCscRechitClusters]
   Int_t           cscRechitCluster_match_cscSegCluster_index[2];   //[nCscRechitClusters]
   Float_t         cscRechitCluster_match_gParticle_minDeltaR[2];   //[nCscRechitClusters]
   Int_t           cscRechitCluster_match_gParticle_index[2];   //[nCscRechitClusters]
   Int_t           cscRechitCluster_match_gParticle_id[2];   //[nCscRechitClusters]
   Float_t         cscRechitClusterX[2];   //[nCscRechitClusters]
   Float_t         cscRechitClusterY[2];   //[nCscRechitClusters]
   Float_t         cscRechitClusterZ[2];   //[nCscRechitClusters]
   Int_t           nCscSegClusters;
   Float_t         cscSegCluster_match_gParticle_minDeltaR[2];   //[nCscSegClusters]
   Int_t           cscSegCluster_match_gParticle_index[2];   //[nCscSegClusters]
   Int_t           cscSegCluster_match_gParticle_id[2];   //[nCscSegClusters]
   Float_t         cscSegClusterX[2];   //[nCscSegClusters]
   Float_t         cscSegClusterY[2];   //[nCscSegClusters]
   Float_t         cscSegClusterZ[2];   //[nCscSegClusters]
   Int_t           nDtRechits;
   Float_t         dtRechitCorrectX[335];   //[nDtRechits]
   Float_t         dtRechitCorrectY[335];   //[nDtRechits]
   Float_t         dtRechitCorrectZ[335];   //[nDtRechits]
   Float_t         dtRechitCorrectEta[335];   //[nDtRechits]
   Float_t         dtRechitCorrectPhi[335];   //[nDtRechits]
   Int_t           nDtRechitClusters;
   Float_t         dtRechitCluster_match_gParticle_minDeltaR[1];   //[nDtRechitClusters]
   Int_t           dtRechitCluster_match_gParticle_index[1];   //[nDtRechitClusters]
   Int_t           dtRechitCluster_match_gParticle_id[1];   //[nDtRechitClusters]
   Float_t         dtRechitClusterX[1];   //[nDtRechitClusters]
   Float_t         dtRechitClusterY[1];   //[nDtRechitClusters]
   Float_t         dtRechitClusterZ[1];   //[nDtRechitClusters]
   Float_t         dtRechitClusterPhi[1];   //[nDtRechitClusters]
   Float_t         dtRechitClusterEta[1];   //[nDtRechitClusters]
   Int_t           nRpc;
   Float_t         rpcPhi[54];   //[nRpc]
   Float_t         rpcEta[54];   //[nRpc]
   Float_t         rpcX[54];   //[nRpc]
   Float_t         rpcY[54];   //[nRpc]
   Float_t         rpcZ[54];   //[nRpc]
   Int_t           nDtSeg;
   Float_t         dtSegPhi[34];   //[nDtSeg]
   Float_t         dtSegEta[34];   //[nDtSeg]
   Float_t         dtSegX[34];   //[nDtSeg]
   Float_t         dtSegY[34];   //[nDtSeg]
   Float_t         dtSegZ[34];   //[nDtSeg]
   Int_t           nDtSegClusters;
   Float_t         dtSegCluster_match_gParticle_minDeltaR[1];   //[nDtSegClusters]
   Int_t           dtSegCluster_match_gParticle_index[1];   //[nDtSegClusters]
   Int_t           dtSegCluster_match_gParticle_id[1];   //[nDtSegClusters]
   Float_t         dtSegClusterX[1];   //[nDtSegClusters]
   Float_t         dtSegClusterY[1];   //[nDtSegClusters]
   Float_t         dtSegClusterZ[1];   //[nDtSegClusters]
   Float_t         dtSegClusterPhi[1];   //[nDtSegClusters]
   Float_t         dtSegClusterEta[1];   //[nDtSegClusters]
   Int_t           nRechits;
   Float_t         ecalRechit_Eta[1];   //[nRechits]
   Float_t         ecalRechit_Phi[1];   //[nRechits]
   Float_t         ecalRechit_E[1];   //[nRechits]
   Float_t         ecalRechit_T[1];   //[nRechits]
   Int_t           nHBHERechits;
   Float_t         hbheRechit_Eta[200];   //[nHBHERechits]
   Float_t         hbheRechit_Phi[200];   //[nHBHERechits]
   Float_t         hbheRechit_E[200];   //[nHBHERechits]
   Float_t         hbheRechit_X[200];   //[nHBHERechits]
   Float_t         hbheRechit_Y[200];   //[nHBHERechits]
   Float_t         hbheRechit_Z[200];   //[nHBHERechits]
   Float_t         hbheRechit_time[200];   //[nHBHERechits]
   Float_t         hbheRechit_auxTDC[200];   //[nHBHERechits]
   Float_t         hbheRechit_timeFalling[200];   //[nHBHERechits]
   Int_t           hbheRechit_iEta[200];   //[nHBHERechits]
   Int_t           hbheRechit_iPhi[200];   //[nHBHERechits]
   Int_t           hbheRechit_depth[200];   //[nHBHERechits]
   Int_t           nTracks;
   Float_t         track_Pt[1];   //[nTracks]
   Float_t         track_Eta[1];   //[nTracks]
   Float_t         track_Phi[1];   //[nTracks]
   Int_t           track_charge[1];   //[nTracks]
   Int_t           track_bestVertexIndex[1];   //[nTracks]
   Int_t           track_nMissingInnerHits[1];   //[nTracks]
   Int_t           track_nMissingOuterHits[1];   //[nTracks]
   Int_t           track_nPixelHits[1];   //[nTracks]
   Int_t           track_angle[1];   //[nTracks]
   Int_t           track_nHits[1];   //[nTracks]
   Float_t         track_dxyToBS[1];   //[nTracks]
   Float_t         track_dxyErr[1];   //[nTracks]
   Float_t         track_dzToPV[1];   //[nTracks]
   Float_t         track_dzErr[1];   //[nTracks]
   Float_t         track_chi2[1];   //[nTracks]
   Int_t           track_ndof[1];   //[nTracks]
   Int_t           nSecondaryVertices;
   Float_t         secVtx_Pt[10];   //[nSecondaryVertices]
   Float_t         secVtx_Eta[10];   //[nSecondaryVertices]
   Float_t         secVtx_Phi[10];   //[nSecondaryVertices]
   Int_t           secVtx_charge[10];   //[nSecondaryVertices]
   Int_t           secVtx_nConstituents[10];   //[nSecondaryVertices]
   Float_t         secVtx_X[10];   //[nSecondaryVertices]
   Float_t         secVtx_Y[10];   //[nSecondaryVertices]
   Float_t         secVtx_Z[10];   //[nSecondaryVertices]
   Float_t         secVtx_Distance[10];   //[nSecondaryVertices]
   Float_t         secVtx_DistanceError[10];   //[nSecondaryVertices]
   Int_t           nJets;
   Float_t         jetE[108];   //[nJets]
   Float_t         jetPt[108];   //[nJets]
   Float_t         jetEta[108];   //[nJets]
   Float_t         jetPhi[108];   //[nJets]
   Float_t         jetCSV[108];   //[nJets]
   Float_t         jetCISV[108];   //[nJets]
   Float_t         jetCMVA[108];   //[nJets]
   Float_t         jetProbb[108];   //[nJets]
   Float_t         jetProbc[108];   //[nJets]
   Float_t         jetProbudsg[108];   //[nJets]
   Float_t         jetProbbb[108];   //[nJets]
   Float_t         jetMass[108];   //[nJets]
   Float_t         jetJetArea[108];   //[nJets]
   Float_t         jetPileupE[108];   //[nJets]
   Float_t         jetPileupId[108];   //[nJets]
   Int_t           jetPileupIdFlag[108];   //[nJets]
   Bool_t          jetPassIDLoose[108];   //[nJets]
   Bool_t          jetPassIDTight[108];   //[nJets]
   Bool_t          jetPassMuFrac[108];   //[nJets]
   Bool_t          jetPassEleFrac[108];   //[nJets]
   Int_t           jetPartonFlavor[108];   //[nJets]
   Int_t           jetHadronFlavor[108];   //[nJets]
   Float_t         jetElectronEnergyFraction[108];   //[nJets]
   Float_t         jetPhotonEnergyFraction[108];   //[nJets]
   Float_t         jetChargedHadronEnergyFraction[108];   //[nJets]
   Float_t         jetNeutralHadronEnergyFraction[108];   //[nJets]
   Float_t         jetMuonEnergyFraction[108];   //[nJets]
   Float_t         jetHOEnergyFraction[108];   //[nJets]
   Float_t         jetHFHadronEnergyFraction[108];   //[nJets]
   Float_t         jetHFEMEnergyFraction[108];   //[nJets]
   Int_t           jetChargedHadronMultiplicity[108];   //[nJets]
   Int_t           jetNeutralHadronMultiplicity[108];   //[nJets]
   Int_t           jetPhotonMultiplicity[108];   //[nJets]
   Int_t           jetElectronMultiplicity[108];   //[nJets]
   Int_t           jetMuonMultiplicity[108];   //[nJets]
   Int_t           jetNSV[108];   //[nJets]
   Int_t           jetNSVCand[108];   //[nJets]
   Int_t           jetNVertexTracks[108];   //[nJets]
   Int_t           jetNSelectedTracks[108];   //[nJets]
   Float_t         jetDRSVJet[108];   //[nJets]
   Float_t         jetFlightDist2D[108];   //[nJets]
   Float_t         jetFlightDist2DError[108];   //[nJets]
   Float_t         jetFlightDist3D[108];   //[nJets]
   Float_t         jetFlightDist3DError[108];   //[nJets]
   Float_t         jetSV_x[108];   //[nJets]
   Float_t         jetSV_y[108];   //[nJets]
   Float_t         jetSV_z[108];   //[nJets]
   Int_t           jetSVNTracks[108];   //[nJets]
   Float_t         jetSVMass[108];   //[nJets]
   Float_t         jetAllMuonPt[108];   //[nJets]
   Float_t         jetAllMuonEta[108];   //[nJets]
   Float_t         jetAllMuonPhi[108];   //[nJets]
   Float_t         jetAllMuonM[108];   //[nJets]
   Float_t         jetPtWeightedDZ[108];   //[nJets]
   Int_t           jetNRechits[108];   //[nJets]
   Float_t         jetRechitE[108];   //[nJets]
   Float_t         jetRechitT[108];   //[nJets]
   Float_t         jetRechitT_rms[108];   //[nJets]
   Float_t         jetRechitE_Error[108];   //[nJets]
   Float_t         jetRechitT_Error[108];   //[nJets]
   Float_t         jetAlphaMax[108];   //[nJets]
   Float_t         jetBetaMax[108];   //[nJets]
   Float_t         jetGammaMax_ET[108];   //[nJets]
   Float_t         jetGammaMax_EM[108];   //[nJets]
   Float_t         jetGammaMax_Hadronic[108];   //[nJets]
   Float_t         jetGammaMax[108];   //[nJets]
   Float_t         jetPtAllTracks[108];   //[nJets]
   Float_t         jetPtAllPVTracks[108];   //[nJets]
   Float_t         jetMedianTheta2D[108];   //[nJets]
   Float_t         jetMedianIP[108];   //[nJets]
   Float_t         jetMinDeltaRAllTracks[108];   //[nJets]
   Float_t         jetMinDeltaRPVTracks[108];   //[nJets]
   Float_t         jet_sig_et1[108];   //[nJets]
   Float_t         jet_sig_et2[108];   //[nJets]
   Float_t         jet_energy_frac[108];   //[nJets]
   Float_t         jetAlphaMax_wp[108];   //[nJets]
   Float_t         jetBetaMax_wp[108];   //[nJets]
   Float_t         jetGammaMax_ET_wp[108];   //[nJets]
   Float_t         jetGammaMax_EM_wp[108];   //[nJets]
   Float_t         jetGammaMax_Hadronic_wp[108];   //[nJets]
   Float_t         jetGammaMax_wp[108];   //[nJets]
   Float_t         jetPtAllTracks_wp[108];   //[nJets]
   Float_t         jetPtAllPVTracks_wp[108];   //[nJets]
   Float_t         jetMedianTheta2D_wp[108];   //[nJets]
   Float_t         jetMedianIP_wp[108];   //[nJets]
   Float_t         jetMinDeltaRAllTracks_wp[108];   //[nJets]
   Float_t         jetMinDeltaRPVTracks_wp[108];   //[nJets]
   Int_t           jetNPFCands[108];   //[nJets]
   Int_t           jetPFCandIndex[108][5000];   //[nJets]
   UInt_t          nFatJets;
   Float_t         fatJetE[1];   //[nFatJets]
   Float_t         fatJetPt[1];   //[nFatJets]
   Float_t         fatJetEta[1];   //[nFatJets]
   Float_t         fatJetPhi[1];   //[nFatJets]
   Float_t         fatJetCorrectedPt[1];   //[nFatJets]
   Float_t         fatJetPrunedM[1];   //[nFatJets]
   Float_t         fatJetTrimmedM[1];   //[nFatJets]
   Float_t         fatJetFilteredM[1];   //[nFatJets]
   Float_t         fatJetSoftDropM[1];   //[nFatJets]
   Float_t         fatJetCorrectedSoftDropM[1];   //[nFatJets]
   Float_t         fatJetUncorrectedSoftDropM[1];   //[nFatJets]
   Float_t         fatJetTau1[1];   //[nFatJets]
   Float_t         fatJetTau2[1];   //[nFatJets]
   Float_t         fatJetTau3[1];   //[nFatJets]
   Float_t         fatJetMaxSubjetCSV[1];   //[nFatJets]
   Bool_t          fatJetPassIDLoose[1];   //[nFatJets]
   Bool_t          fatJetPassIDTight[1];   //[nFatJets]
   Float_t         fatJetElectronEnergyFraction[1];   //[nFatJets]
   Float_t         fatJetPhotonEnergyFraction[1];   //[nFatJets]
   Float_t         fatJetChargedHadronEnergyFraction[1];   //[nFatJets]
   Float_t         fatJetNeutralHadronEnergyFraction[1];   //[nFatJets]
   Float_t         fatJetMuonEnergyFraction[1];   //[nFatJets]
   Float_t         fatJetHOEnergyFraction[1];   //[nFatJets]
   Float_t         fatJetHFHadronEnergyFraction[1];   //[nFatJets]
   Float_t         fatJetHFEMEnergyFraction[1];   //[nFatJets]
   Int_t           fatJetChargedHadronMultiplicity[1];   //[nFatJets]
   Int_t           fatJetNeutralHadronMultiplicity[1];   //[nFatJets]
   Int_t           fatJetPhotonMultiplicity[1];   //[nFatJets]
   Int_t           fatJetElectronMultiplicity[1];   //[nFatJets]
   Int_t           fatJetMuonMultiplicity[1];   //[nFatJets]
   Int_t           fatJetNRechits[1];   //[nFatJets]
   Float_t         fatJetRechitE[1];   //[nFatJets]
   Float_t         fatJetRechitT[1];   //[nFatJets]
   Float_t         fatJetRechitT_rms[1];   //[nFatJets]
   Float_t         fatJetRechitE_Error[1];   //[nFatJets]
   Float_t         fatJetRechitT_Error[1];   //[nFatJets]
   Float_t         fatJetAlphaMax[1];   //[nFatJets]
   Float_t         fatJetBetaMax[1];   //[nFatJets]
   Float_t         fatJetGammaMax_ET[1];   //[nFatJets]
   Float_t         fatJetGammaMax_EM[1];   //[nFatJets]
   Float_t         fatJetGammaMax_Hadronic[1];   //[nFatJets]
   Float_t         fatJetGammaMax[1];   //[nFatJets]
   Float_t         fatJetPtAllTracks[1];   //[nFatJets]
   Float_t         fatJetPtAllPVTracks[1];   //[nFatJets]
   Float_t         fatJetMedianTheta2D[1];   //[nFatJets]
   Float_t         fatJetMedianIP[1];   //[nFatJets]
   Float_t         fatJetMinDeltaRAllTracks[1];   //[nFatJets]
   Float_t         fatJetMinDeltaRPVTracks[1];   //[nFatJets]
   Int_t           fatJetNPFCands[1];   //[nFatJets]
   Int_t           fatJetPFCandIndex[1][5000];   //[nFatJets]
   Float_t         metPt;
   Float_t         metPhi;
   Float_t         sumMET;
   Float_t         metUncorrectedPt;
   Float_t         metUncorrectedPhi;
   Float_t         metType1Pt;
   Float_t         metType1Phi;
   Float_t         metPuppiPt;
   Float_t         metPuppiPhi;
   Float_t         metCaloPt;
   Float_t         metCaloPhi;
   Bool_t          HLTDecision[1201];
   Int_t           HLTPrescale[1201];
   Int_t           nGenJets;
   Float_t         genJetE[1];   //[nGenJets]
   Float_t         genJetPt[1];   //[nGenJets]
   Float_t         genJetEta[1];   //[nGenJets]
   Float_t         genJetPhi[1];   //[nGenJets]
   Float_t         genMetPtCalo;
   Float_t         genMetPhiCalo;
   Float_t         genMetPtTrue;
   Float_t         genMetPhiTrue;
   Float_t         genVertexX;
   Float_t         genVertexY;
   Float_t         genVertexZ;
   Float_t         genVertexT;
   Float_t         genWeight;
   UInt_t          genSignalProcessID;
   Float_t         genQScale;
   Float_t         genAlphaQCD;
   Float_t         genAlphaQED;
   string          *lheComments;
   vector<float>   *scaleWeights;
   vector<float>   *pdfWeights;
   vector<float>   *alphasWeights;
   Int_t           nGenParticle;
   Int_t           gParticleMotherId[1];   //[nGenParticle]
   Int_t           gParticleMotherIndex[1];   //[nGenParticle]
   Int_t           gParticleId[1];   //[nGenParticle]
   Int_t           gParticleStatus[1];   //[nGenParticle]
   Float_t         gParticleE[1];   //[nGenParticle]
   Float_t         gParticlePt[1];   //[nGenParticle]
   Float_t         gParticleEta[1];   //[nGenParticle]
   Float_t         gParticlePhi[1];   //[nGenParticle]
   Float_t         gParticleProdVertexX[1];   //[nGenParticle]
   Float_t         gParticleProdVertexY[1];   //[nGenParticle]
   Float_t         gParticleProdVertexZ[1];   //[nGenParticle]
   Float_t         gParticleDecayVertexX[1];   //[nGenParticle]
   Float_t         gParticleDecayVertexY[1];   //[nGenParticle]
   Float_t         gParticleDecayVertexZ[1];   //[nGenParticle]
   Float_t         gLLP_decay_vertex_x[2];
   Float_t         gLLP_decay_vertex_y[2];
   Float_t         gLLP_decay_vertex_z[2];
   Float_t         gLLP_beta[2];
   Float_t         gLLP_e[2];
   Float_t         gLLP_pt[2];
   Float_t         gLLP_eta[2];
   Float_t         gLLP_phi[2];
   Bool_t          gLLP_csc[2];
   Bool_t          gLLP_dt[2];
   Float_t         gLLP_travel_time[2];
   Int_t           gLLP_daughter_id[4];
   Float_t         gLLP_daughter_pt[4];
   Float_t         gLLP_daughter_eta[4];
   Float_t         gLLP_daughter_phi[4];
   Float_t         gLLP_daughter_eta_ecalcorr[4];
   Float_t         gLLP_daughter_phi_ecalcorr[4];
   Float_t         gLLP_daughter_e[4];
   Float_t         gLLP_daughter_mass[4];
   Float_t         gen_time[4];
   Float_t         gen_time_pv[4];
   Float_t         photon_travel_time[4];
   Float_t         photon_travel_time_pv[4];
   Float_t         gLLP_daughter_travel_time[4];
   Int_t           gLLP_grandaughter_id[4];
   Float_t         gLLP_grandaughter_pt[4];
   Float_t         gLLP_grandaughter_eta[4];
   Float_t         gLLP_grandaughter_phi[4];
   Float_t         gLLP_grandaughter_eta_ecalcorr[4];
   Float_t         gLLP_grandaughter_phi_ecalcorr[4];
   Float_t         gLLP_grandaughter_e[4];
   Float_t         gLLP_grandaughter_mass[4];
   Float_t         gen_time_dau[4];
   Float_t         gen_time_dau_pv[4];
   Float_t         photon_travel_time_dau[4];
   Float_t         photon_travel_time_dau_pv[4];
   Float_t         gLLP_grandaughter_travel_time[4];

   // List of branches
   TBranch        *b_isData;   //!
   TBranch        *b_nPV;   //!
   TBranch        *b_runNum;   //!
   TBranch        *b_lumiNum;   //!
   TBranch        *b_eventNum;   //!
   TBranch        *b_eventTime;   //!
   TBranch        *b_pvX;   //!
   TBranch        *b_pvY;   //!
   TBranch        *b_pvZ;   //!
   TBranch        *b_nPVAll;   //!
   TBranch        *b_pvAllX;   //!
   TBranch        *b_pvAllY;   //!
   TBranch        *b_pvAllZ;   //!
   TBranch        *b_pvAllLogSumPtSq;   //!
   TBranch        *b_pvAllSumPx;   //!
   TBranch        *b_pvAllSumPy;   //!
   TBranch        *b_nBunchXing;   //!
   TBranch        *b_BunchXing;   //!
   TBranch        *b_nPU;   //!
   TBranch        *b_nPUmean;   //!
   TBranch        *b_nMuons;   //!
   TBranch        *b_muonE;   //!
   TBranch        *b_muonPt;   //!
   TBranch        *b_muonEta;   //!
   TBranch        *b_muonPhi;   //!
   TBranch        *b_muonCharge;   //!
   TBranch        *b_muonIsLoose;   //!
   TBranch        *b_muonIsMedium;   //!
   TBranch        *b_muonIsTight;   //!
   TBranch        *b_muon_pileupIso;   //!
   TBranch        *b_muon_chargedIso;   //!
   TBranch        *b_muon_photonIso;   //!
   TBranch        *b_muon_neutralHadIso;   //!
   TBranch        *b_nElectrons;   //!
   TBranch        *b_eleE;   //!
   TBranch        *b_elePt;   //!
   TBranch        *b_eleEta;   //!
   TBranch        *b_elePhi;   //!
   TBranch        *b_eleCharge;   //!
   TBranch        *b_ele_HoverE;   //!
   TBranch        *b_ele_pileupIso;   //!
   TBranch        *b_ele_chargedIso;   //!
   TBranch        *b_ele_photonIso;   //!
   TBranch        *b_ele_neutralHadIso;   //!
   TBranch        *b_nTaus;   //!
   TBranch        *b_tauE;   //!
   TBranch        *b_tauPt;   //!
   TBranch        *b_tauEta;   //!
   TBranch        *b_tauPhi;   //!
   TBranch        *b_tau_IsLoose;   //!
   TBranch        *b_tau_IsMedium;   //!
   TBranch        *b_tau_IsTight;   //!
   TBranch        *b_nPFCandidates;   //!
   TBranch        *b_PFCandidatePdgId;   //!
   TBranch        *b_PFCandidatePt;   //!
   TBranch        *b_PFCandidateEta;   //!
   TBranch        *b_PFCandidatePhi;   //!
   TBranch        *b_PFCandidateTrackIndex;   //!
   TBranch        *b_PFCandidatePVIndex;   //!
   TBranch        *b_nPhotons;   //!
   TBranch        *b_nPhotons_overlap;   //!
   TBranch        *b_phoE;   //!
   TBranch        *b_phoPt;   //!
   TBranch        *b_phoEta;   //!
   TBranch        *b_phoPhi;   //!
   TBranch        *b_pho_HoverE;   //!
   TBranch        *b_pho_sumChargedHadronPtAllVertices;   //!
   TBranch        *b_pho_sumChargedHadronPt;   //!
   TBranch        *b_pho_sumNeutralHadronEt;   //!
   TBranch        *b_pho_sumPhotonEt;   //!
   TBranch        *b_pho_ecalPFClusterIso;   //!
   TBranch        *b_pho_hcalPFClusterIso;   //!
   TBranch        *b_nCscWireDigis;   //!
   TBranch        *b_nCscStripDigis;   //!
   TBranch        *b_nCscSeg;   //!
   TBranch        *b_cscSegPhi;   //!
   TBranch        *b_cscSegEta;   //!
   TBranch        *b_cscSegX;   //!
   TBranch        *b_cscSegY;   //!
   TBranch        *b_cscSegZ;   //!
   TBranch        *b_cscSegT;   //!
   TBranch        *b_cscSegNRecHits;   //!
   TBranch        *b_ncscRechits;   //!
   TBranch        *b_cscRechitsPhi;   //!
   TBranch        *b_cscRechitsEta;   //!
   TBranch        *b_cscRechitsX;   //!
   TBranch        *b_cscRechitsY;   //!
   TBranch        *b_cscRechitsZ;   //!
   TBranch        *b_cscRechitsE;   //!
   TBranch        *b_nCscRechitClusters;   //!
   TBranch        *b_cscRechitCluster_match_cscSegCluster_minDeltaR;   //!
   TBranch        *b_cscRechitCluster_match_cscSegCluster_index;   //!
   TBranch        *b_cscRechitCluster_match_gParticle_minDeltaR;   //!
   TBranch        *b_cscRechitCluster_match_gParticle_index;   //!
   TBranch        *b_cscRechitCluster_match_gParticle_id;   //!
   TBranch        *b_cscRechitClusterX;   //!
   TBranch        *b_cscRechitClusterY;   //!
   TBranch        *b_cscRechitClusterZ;   //!
   TBranch        *b_nCscSegClusters;   //!
   TBranch        *b_cscSegCluster_match_gParticle_minDeltaR;   //!
   TBranch        *b_cscSegCluster_match_gParticle_index;   //!
   TBranch        *b_cscSegCluster_match_gParticle_id;   //!
   TBranch        *b_cscSegClusterX;   //!
   TBranch        *b_cscSegClusterY;   //!
   TBranch        *b_cscSegClusterZ;   //!
   TBranch        *b_nDtRechits;   //!
   TBranch        *b_dtRechitCorrectX;   //!
   TBranch        *b_dtRechitCorrectY;   //!
   TBranch        *b_dtRechitCorrectZ;   //!
   TBranch        *b_dtRechitCorrectEta;   //!
   TBranch        *b_dtRechitCorrectPhi;   //!
   TBranch        *b_nDtRechitClusters;   //!
   TBranch        *b_dtRechitCluster_match_gParticle_minDeltaR;   //!
   TBranch        *b_dtRechitCluster_match_gParticle_index;   //!
   TBranch        *b_dtRechitCluster_match_gParticle_id;   //!
   TBranch        *b_dtRechitClusterX;   //!
   TBranch        *b_dtRechitClusterY;   //!
   TBranch        *b_dtRechitClusterZ;   //!
   TBranch        *b_dtRechitClusterPhi;   //!
   TBranch        *b_dtRechitClusterEta;   //!
   TBranch        *b_nRpc;   //!
   TBranch        *b_rpcPhi;   //!
   TBranch        *b_rpcEta;   //!
   TBranch        *b_rpcX;   //!
   TBranch        *b_rpcY;   //!
   TBranch        *b_rpcZ;   //!
   TBranch        *b_nDtSeg;   //!
   TBranch        *b_dtSegPhi;   //!
   TBranch        *b_dtSegEta;   //!
   TBranch        *b_dtSegX;   //!
   TBranch        *b_dtSegY;   //!
   TBranch        *b_dtSegZ;   //!
   TBranch        *b_nDtSegClusters;   //!
   TBranch        *b_dtSegCluster_match_gParticle_minDeltaR;   //!
   TBranch        *b_dtSegCluster_match_gParticle_index;   //!
   TBranch        *b_dtSegCluster_match_gParticle_id;   //!
   TBranch        *b_dtSegClusterX;   //!
   TBranch        *b_dtSegClusterY;   //!
   TBranch        *b_dtSegClusterZ;   //!
   TBranch        *b_dtSegClusterPhi;   //!
   TBranch        *b_dtSegClusterEta;   //!
   TBranch        *b_nRechits;   //!
   TBranch        *b_ecalRechit_Eta;   //!
   TBranch        *b_ecalRechit_Phi;   //!
   TBranch        *b_ecalRechit_E;   //!
   TBranch        *b_ecalRechit_T;   //!
   TBranch        *b_nHBHERechits;   //!
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
   TBranch        *b_nTracks;   //!
   TBranch        *b_track_Pt;   //!
   TBranch        *b_track_Eta;   //!
   TBranch        *b_track_Phi;   //!
   TBranch        *b_track_charge;   //!
   TBranch        *b_track_bestVertexIndex;   //!
   TBranch        *b_track_nMissingInnerHits;   //!
   TBranch        *b_track_nMissingOuterHits;   //!
   TBranch        *b_track_nPixelHits;   //!
   TBranch        *b_track_angle;   //!
   TBranch        *b_track_nHits;   //!
   TBranch        *b_track_dxyToBS;   //!
   TBranch        *b_track_dxyErr;   //!
   TBranch        *b_track_dzToPV;   //!
   TBranch        *b_track_dzErr;   //!
   TBranch        *b_track_chi2;   //!
   TBranch        *b_track_ndof;   //!
   TBranch        *b_nSecondaryVertices;   //!
   TBranch        *b_secVtx_Pt;   //!
   TBranch        *b_secVtx_Eta;   //!
   TBranch        *b_secVtx_Phi;   //!
   TBranch        *b_secVtx_charge;   //!
   TBranch        *b_secVtx_nConstituents;   //!
   TBranch        *b_secVtx_X;   //!
   TBranch        *b_secVtx_Y;   //!
   TBranch        *b_secVtx_Z;   //!
   TBranch        *b_secVtx_Distance;   //!
   TBranch        *b_secVtx_DistanceError;   //!
   TBranch        *b_nJets;   //!
   TBranch        *b_jetE;   //!
   TBranch        *b_jetPt;   //!
   TBranch        *b_jetEta;   //!
   TBranch        *b_jetPhi;   //!
   TBranch        *b_jetCSV;   //!
   TBranch        *b_jetCISV;   //!
   TBranch        *b_jetCMVA;   //!
   TBranch        *b_jetProbb;   //!
   TBranch        *b_jetProbc;   //!
   TBranch        *b_jetProbudsg;   //!
   TBranch        *b_jetProbbb;   //!
   TBranch        *b_jetMass;   //!
   TBranch        *b_jetJetArea;   //!
   TBranch        *b_jetPileupE;   //!
   TBranch        *b_jetPileupId;   //!
   TBranch        *b_jetPileupIdFlag;   //!
   TBranch        *b_jetPassIDLoose;   //!
   TBranch        *b_jetPassIDTight;   //!
   TBranch        *b_jetPassMuFrac;   //!
   TBranch        *b_jetPassEleFrac;   //!
   TBranch        *b_jetPartonFlavor;   //!
   TBranch        *b_jetHadronFlavor;   //!
   TBranch        *b_jetElectronEnergyFraction;   //!
   TBranch        *b_jetPhotonEnergyFraction;   //!
   TBranch        *b_jetChargedHadronEnergyFraction;   //!
   TBranch        *b_jetNeutralHadronEnergyFraction;   //!
   TBranch        *b_jetMuonEnergyFraction;   //!
   TBranch        *b_jetHOEnergyFraction;   //!
   TBranch        *b_jetHFHadronEnergyFraction;   //!
   TBranch        *b_jetHFEMEnergyFraction;   //!
   TBranch        *b_jetChargedHadronMultiplicity;   //!
   TBranch        *b_jetNeutralHadronMultiplicity;   //!
   TBranch        *b_jetPhotonMultiplicity;   //!
   TBranch        *b_jetElectronMultiplicity;   //!
   TBranch        *b_jetMuonMultiplicity;   //!
   TBranch        *b_jetNSV;   //!
   TBranch        *b_jetNSVCand;   //!
   TBranch        *b_jetNVertexTracks;   //!
   TBranch        *b_jetNSelectedTracks;   //!
   TBranch        *b_jetDRSVJet;   //!
   TBranch        *b_jetFlightDist2D;   //!
   TBranch        *b_jetFlightDist2DError;   //!
   TBranch        *b_jetFlightDist3D;   //!
   TBranch        *b_jetFlightDist3DError;   //!
   TBranch        *b_jetSV_x;   //!
   TBranch        *b_jetSV_y;   //!
   TBranch        *b_jetSV_z;   //!
   TBranch        *b_jetSVNTracks;   //!
   TBranch        *b_jetSVMass;   //!
   TBranch        *b_jetAllMuonPt;   //!
   TBranch        *b_jetAllMuonEta;   //!
   TBranch        *b_jetAllMuonPhi;   //!
   TBranch        *b_jetAllMuonM;   //!
   TBranch        *b_jetPtWeightedDZ;   //!
   TBranch        *b_jetNRechits;   //!
   TBranch        *b_jetRechitE;   //!
   TBranch        *b_jetRechitT;   //!
   TBranch        *b_jetRechitT_rms;   //!
   TBranch        *b_jetRechitE_Error;   //!
   TBranch        *b_jetRechitT_Error;   //!
   TBranch        *b_jetAlphaMax;   //!
   TBranch        *b_jetBetaMax;   //!
   TBranch        *b_jetGammaMax_ET;   //!
   TBranch        *b_jetGammaMax_EM;   //!
   TBranch        *b_jetGammaMax_Hadronic;   //!
   TBranch        *b_jetGammaMax;   //!
   TBranch        *b_jetPtAllTracks;   //!
   TBranch        *b_jetPtAllPVTracks;   //!
   TBranch        *b_jetMedianTheta2D;   //!
   TBranch        *b_jetMedianIP;   //!
   TBranch        *b_jetMinDeltaRAllTracks;   //!
   TBranch        *b_jetMinDeltaRPVTracks;   //!
   TBranch        *b_jet_sig_et1;   //!
   TBranch        *b_jet_sig_et2;   //!
   TBranch        *b_jet_energy_frac;   //!
   TBranch        *b_jetAlphaMax_wp;   //!
   TBranch        *b_jetBetaMax_wp;   //!
   TBranch        *b_jetGammaMax_ET_wp;   //!
   TBranch        *b_jetGammaMax_EM_wp;   //!
   TBranch        *b_jetGammaMax_Hadronic_wp;   //!
   TBranch        *b_jetGammaMax_wp;   //!
   TBranch        *b_jetPtAllTracks_wp;   //!
   TBranch        *b_jetPtAllPVTracks_wp;   //!
   TBranch        *b_jetMedianTheta2D_wp;   //!
   TBranch        *b_jetMedianIP_wp;   //!
   TBranch        *b_jetMinDeltaRAllTracks_wp;   //!
   TBranch        *b_jetMinDeltaRPVTracks_wp;   //!
   TBranch        *b_jetNPFCands;   //!
   TBranch        *b_jetPFCandIndex;   //!
   TBranch        *b_nFatJets;   //!
   TBranch        *b_fatJetE;   //!
   TBranch        *b_fatJetPt;   //!
   TBranch        *b_fatJetEta;   //!
   TBranch        *b_fatJetPhi;   //!
   TBranch        *b_fatJetCorrectedPt;   //!
   TBranch        *b_fatJetPrunedM;   //!
   TBranch        *b_fatJetTrimmedM;   //!
   TBranch        *b_fatJetFilteredM;   //!
   TBranch        *b_fatJetSoftDropM;   //!
   TBranch        *b_fatJetCorrectedSoftDropM;   //!
   TBranch        *b_fatJetUncorrectedSoftDropM;   //!
   TBranch        *b_fatJetTau1;   //!
   TBranch        *b_fatJetTau2;   //!
   TBranch        *b_fatJetTau3;   //!
   TBranch        *b_fatJetMaxSubjetCSV;   //!
   TBranch        *b_fatJetPassIDLoose;   //!
   TBranch        *b_fatJetPassIDTight;   //!
   TBranch        *b_fatJetElectronEnergyFraction;   //!
   TBranch        *b_fatJetPhotonEnergyFraction;   //!
   TBranch        *b_fatJetChargedHadronEnergyFraction;   //!
   TBranch        *b_fatJetNeutralHadronEnergyFraction;   //!
   TBranch        *b_fatJetMuonEnergyFraction;   //!
   TBranch        *b_fatJetHOEnergyFraction;   //!
   TBranch        *b_fatJetHFHadronEnergyFraction;   //!
   TBranch        *b_fatJetHFEMEnergyFraction;   //!
   TBranch        *b_fatJetChargedHadronMultiplicity;   //!
   TBranch        *b_fatJetNeutralHadronMultiplicity;   //!
   TBranch        *b_fatJetPhotonMultiplicity;   //!
   TBranch        *b_fatJetElectronMultiplicity;   //!
   TBranch        *b_fatJetMuonMultiplicity;   //!
   TBranch        *b_fatJetNRechits;   //!
   TBranch        *b_fatJetRechitE;   //!
   TBranch        *b_fatJetRechitT;   //!
   TBranch        *b_fatJetRechitT_rms;   //!
   TBranch        *b_fatJetRechitE_Error;   //!
   TBranch        *b_fatJetRechitT_Error;   //!
   TBranch        *b_fatJetAlphaMax;   //!
   TBranch        *b_fatJetBetaMax;   //!
   TBranch        *b_fatJetGammaMax_ET;   //!
   TBranch        *b_fatJetGammaMax_EM;   //!
   TBranch        *b_fatJetGammaMax_Hadronic;   //!
   TBranch        *b_fatJetGammaMax;   //!
   TBranch        *b_fatJetPtAllTracks;   //!
   TBranch        *b_fatJetPtAllPVTracks;   //!
   TBranch        *b_fatJetMedianTheta2D;   //!
   TBranch        *b_fatJetMedianIP;   //!
   TBranch        *b_fatJetMinDeltaRAllTracks;   //!
   TBranch        *b_fatJetMinDeltaRPVTracks;   //!
   TBranch        *b_fatJetNPFCands;   //!
   TBranch        *b_fatJetPFCandIndex;   //!
   TBranch        *b_metPt;   //!
   TBranch        *b_metPhi;   //!
   TBranch        *b_sumMET;   //!
   TBranch        *b_metUncorrectedPt;   //!
   TBranch        *b_metUncorrectedPhi;   //!
   TBranch        *b_metType1Pt;   //!
   TBranch        *b_metType1Phi;   //!
   TBranch        *b_metPuppiPt;   //!
   TBranch        *b_metPuppiPhi;   //!
   TBranch        *b_metCaloPt;   //!
   TBranch        *b_metCaloPhi;   //!
   TBranch        *b_HLTDecision;   //!
   TBranch        *b_HLTPrescale;   //!
   TBranch        *b_nGenJets;   //!
   TBranch        *b_genJetE;   //!
   TBranch        *b_genJetPt;   //!
   TBranch        *b_genJetEta;   //!
   TBranch        *b_genJetPhi;   //!
   TBranch        *b_genMetPtCalo;   //!
   TBranch        *b_genMetPhiCalo;   //!
   TBranch        *b_genMetPtTrue;   //!
   TBranch        *b_genMetPhiTrue;   //!
   TBranch        *b_genVertexX;   //!
   TBranch        *b_genVertexY;   //!
   TBranch        *b_genVertexZ;   //!
   TBranch        *b_genVertexT;   //!
   TBranch        *b_genWeight;   //!
   TBranch        *b_genSignalProcessID;   //!
   TBranch        *b_genQScale;   //!
   TBranch        *b_genAlphaQCD;   //!
   TBranch        *b_genAlphaQED;   //!
   TBranch        *b_lheComments;   //!
   TBranch        *b_scaleWeights;   //!
   TBranch        *b_pdfWeights;   //!
   TBranch        *b_alphasWeights;   //!
   TBranch        *b_nGenParticle;   //!
   TBranch        *b_gParticleMotherId;   //!
   TBranch        *b_gParticleMotherIndex;   //!
   TBranch        *b_gParticleId;   //!
   TBranch        *b_gParticleStatus;   //!
   TBranch        *b_gParticleE;   //!
   TBranch        *b_gParticlePt;   //!
   TBranch        *b_gParticleEta;   //!
   TBranch        *b_gParticlePhi;   //!
   TBranch        *b_gParticleProdVertexX;   //!
   TBranch        *b_gParticleProdVertexY;   //!
   TBranch        *b_gParticleProdVertexZ;   //!
   TBranch        *b_gParticleDecayVertexX;   //!
   TBranch        *b_gParticleDecayVertexY;   //!
   TBranch        *b_gParticleDecayVertexZ;   //!
   TBranch        *b_gLLP_decay_vertex_x;   //!
   TBranch        *b_gLLP_decay_vertex_y;   //!
   TBranch        *b_gLLP_decay_vertex_z;   //!
   TBranch        *b_gLLP_beta;   //!
   TBranch        *b_gLLP_e;   //!
   TBranch        *b_gLLP_pt;   //!
   TBranch        *b_gLLP_eta;   //!
   TBranch        *b_gLLP_phi;   //!
   TBranch        *b_gLLP_csc;   //!
   TBranch        *b_gLLP_dt;   //!
   TBranch        *b_gLLP_travel_time;   //!
   TBranch        *b_gLLP_daughter_id;   //!
   TBranch        *b_gLLP_daughter_pt;   //!
   TBranch        *b_gLLP_daughter_eta;   //!
   TBranch        *b_gLLP_daughter_phi;   //!
   TBranch        *b_gLLP_daughter_eta_ecalcorr;   //!
   TBranch        *b_gLLP_daughter_phi_ecalcorr;   //!
   TBranch        *b_gLLP_daughter_e;   //!
   TBranch        *b_gLLP_daughter_mass;   //!
   TBranch        *b_gen_time;   //!
   TBranch        *b_gen_time_pv;   //!
   TBranch        *b_photon_travel_time;   //!
   TBranch        *b_photon_travel_time_pv;   //!
   TBranch        *b_gLLP_daughter_travel_time;   //!
   TBranch        *b_gLLP_grandaughter_id;   //!
   TBranch        *b_gLLP_grandaughter_pt;   //!
   TBranch        *b_gLLP_grandaughter_eta;   //!
   TBranch        *b_gLLP_grandaughter_phi;   //!
   TBranch        *b_gLLP_grandaughter_eta_ecalcorr;   //!
   TBranch        *b_gLLP_grandaughter_phi_ecalcorr;   //!
   TBranch        *b_gLLP_grandaughter_e;   //!
   TBranch        *b_gLLP_grandaughter_mass;   //!
   TBranch        *b_gen_time_dau;   //!
   TBranch        *b_gen_time_dau_pv;   //!
   TBranch        *b_photon_travel_time_dau;   //!
   TBranch        *b_photon_travel_time_dau_pv;   //!
   TBranch        *b_gLLP_grandaughter_travel_time;   //!

   LLP_HCAL_Analyzer(TTree *tree=0);
   virtual ~LLP_HCAL_Analyzer();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef LLP_HCAL_Analyzer_cxx
LLP_HCAL_Analyzer::LLP_HCAL_Analyzer(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/llp_ntupler/displacedJetMuon_ntupler_small.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/llp_ntupler/displacedJetMuon_ntupler_small.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/afs/cern.ch/work/g/gkopp/2022_LLP_analysis/CMSSW_12_4_6/src/cms_lpc_llp/llp_ntupler/displacedJetMuon_ntupler_small.root:/ntuples");
      dir->GetObject("llp",tree);

   }
   Init(tree);
}

LLP_HCAL_Analyzer::~LLP_HCAL_Analyzer()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t LLP_HCAL_Analyzer::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t LLP_HCAL_Analyzer::LoadTree(Long64_t entry)
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

void LLP_HCAL_Analyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   lheComments = 0;
   scaleWeights = 0;
   pdfWeights = 0;
   alphasWeights = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("isData", &isData, &b_isData);
   fChain->SetBranchAddress("nPV", &nPV, &b_nPV);
   fChain->SetBranchAddress("runNum", &runNum, &b_runNum);
   fChain->SetBranchAddress("lumiNum", &lumiNum, &b_lumiNum);
   fChain->SetBranchAddress("eventNum", &eventNum, &b_eventNum);
   fChain->SetBranchAddress("eventTime", &eventTime, &b_eventTime);
   fChain->SetBranchAddress("pvX", &pvX, &b_pvX);
   fChain->SetBranchAddress("pvY", &pvY, &b_pvY);
   fChain->SetBranchAddress("pvZ", &pvZ, &b_pvZ);
   fChain->SetBranchAddress("nPVAll", &nPVAll, &b_nPVAll);
   fChain->SetBranchAddress("pvAllX", pvAllX, &b_pvAllX);
   fChain->SetBranchAddress("pvAllY", pvAllY, &b_pvAllY);
   fChain->SetBranchAddress("pvAllZ", pvAllZ, &b_pvAllZ);
   fChain->SetBranchAddress("pvAllLogSumPtSq", pvAllLogSumPtSq, &b_pvAllLogSumPtSq);
   fChain->SetBranchAddress("pvAllSumPx", pvAllSumPx, &b_pvAllSumPx);
   fChain->SetBranchAddress("pvAllSumPy", pvAllSumPy, &b_pvAllSumPy);
   fChain->SetBranchAddress("nBunchXing", &nBunchXing, &b_nBunchXing);
   fChain->SetBranchAddress("BunchXing", &BunchXing, &b_BunchXing);
   fChain->SetBranchAddress("nPU", &nPU, &b_nPU);
   fChain->SetBranchAddress("nPUmean", &nPUmean, &b_nPUmean);
   fChain->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
   fChain->SetBranchAddress("muonE", muonE, &b_muonE);
   fChain->SetBranchAddress("muonPt", muonPt, &b_muonPt);
   fChain->SetBranchAddress("muonEta", muonEta, &b_muonEta);
   fChain->SetBranchAddress("muonPhi", muonPhi, &b_muonPhi);
   fChain->SetBranchAddress("muonCharge", muonCharge, &b_muonCharge);
   fChain->SetBranchAddress("muonIsLoose", muonIsLoose, &b_muonIsLoose);
   fChain->SetBranchAddress("muonIsMedium", muonIsMedium, &b_muonIsMedium);
   fChain->SetBranchAddress("muonIsTight", muonIsTight, &b_muonIsTight);
   fChain->SetBranchAddress("muon_pileupIso", muon_pileupIso, &b_muon_pileupIso);
   fChain->SetBranchAddress("muon_chargedIso", muon_chargedIso, &b_muon_chargedIso);
   fChain->SetBranchAddress("muon_photonIso", muon_photonIso, &b_muon_photonIso);
   fChain->SetBranchAddress("muon_neutralHadIso", muon_neutralHadIso, &b_muon_neutralHadIso);
   fChain->SetBranchAddress("nElectrons", &nElectrons, &b_nElectrons);
   fChain->SetBranchAddress("eleE", eleE, &b_eleE);
   fChain->SetBranchAddress("elePt", elePt, &b_elePt);
   fChain->SetBranchAddress("eleEta", eleEta, &b_eleEta);
   fChain->SetBranchAddress("elePhi", elePhi, &b_elePhi);
   fChain->SetBranchAddress("eleCharge", eleCharge, &b_eleCharge);
   fChain->SetBranchAddress("ele_HoverE", ele_HoverE, &b_ele_HoverE);
   fChain->SetBranchAddress("ele_pileupIso", ele_pileupIso, &b_ele_pileupIso);
   fChain->SetBranchAddress("ele_chargedIso", ele_chargedIso, &b_ele_chargedIso);
   fChain->SetBranchAddress("ele_photonIso", ele_photonIso, &b_ele_photonIso);
   fChain->SetBranchAddress("ele_neutralHadIso", ele_neutralHadIso, &b_ele_neutralHadIso);
   fChain->SetBranchAddress("nTaus", &nTaus, &b_nTaus);
   fChain->SetBranchAddress("tauE", &tauE, &b_tauE);
   fChain->SetBranchAddress("tauPt", &tauPt, &b_tauPt);
   fChain->SetBranchAddress("tauEta", &tauEta, &b_tauEta);
   fChain->SetBranchAddress("tauPhi", &tauPhi, &b_tauPhi);
   fChain->SetBranchAddress("tau_IsLoose", &tau_IsLoose, &b_tau_IsLoose);
   fChain->SetBranchAddress("tau_IsMedium", &tau_IsMedium, &b_tau_IsMedium);
   fChain->SetBranchAddress("tau_IsTight", &tau_IsTight, &b_tau_IsTight);
   fChain->SetBranchAddress("nPFCandidates", &nPFCandidates, &b_nPFCandidates);
   fChain->SetBranchAddress("PFCandidatePdgId", PFCandidatePdgId, &b_PFCandidatePdgId);
   fChain->SetBranchAddress("PFCandidatePt", PFCandidatePt, &b_PFCandidatePt);
   fChain->SetBranchAddress("PFCandidateEta", PFCandidateEta, &b_PFCandidateEta);
   fChain->SetBranchAddress("PFCandidatePhi", PFCandidatePhi, &b_PFCandidatePhi);
   fChain->SetBranchAddress("PFCandidateTrackIndex", PFCandidateTrackIndex, &b_PFCandidateTrackIndex);
   fChain->SetBranchAddress("PFCandidatePVIndex", PFCandidatePVIndex, &b_PFCandidatePVIndex);
   fChain->SetBranchAddress("nPhotons", &nPhotons, &b_nPhotons);
   fChain->SetBranchAddress("nPhotons_overlap", &nPhotons_overlap, &b_nPhotons_overlap);
   fChain->SetBranchAddress("phoE", phoE, &b_phoE);
   fChain->SetBranchAddress("phoPt", phoPt, &b_phoPt);
   fChain->SetBranchAddress("phoEta", phoEta, &b_phoEta);
   fChain->SetBranchAddress("phoPhi", phoPhi, &b_phoPhi);
   fChain->SetBranchAddress("pho_HoverE", pho_HoverE, &b_pho_HoverE);
   fChain->SetBranchAddress("pho_sumChargedHadronPtAllVertices", pho_sumChargedHadronPtAllVertices, &b_pho_sumChargedHadronPtAllVertices);
   fChain->SetBranchAddress("pho_sumChargedHadronPt", pho_sumChargedHadronPt, &b_pho_sumChargedHadronPt);
   fChain->SetBranchAddress("pho_sumNeutralHadronEt", pho_sumNeutralHadronEt, &b_pho_sumNeutralHadronEt);
   fChain->SetBranchAddress("pho_sumPhotonEt", pho_sumPhotonEt, &b_pho_sumPhotonEt);
   fChain->SetBranchAddress("pho_ecalPFClusterIso", pho_ecalPFClusterIso, &b_pho_ecalPFClusterIso);
   fChain->SetBranchAddress("pho_hcalPFClusterIso", pho_hcalPFClusterIso, &b_pho_hcalPFClusterIso);
   fChain->SetBranchAddress("nCscWireDigis", &nCscWireDigis, &b_nCscWireDigis);
   fChain->SetBranchAddress("nCscStripDigis", &nCscStripDigis, &b_nCscStripDigis);
   fChain->SetBranchAddress("nCscSeg", &nCscSeg, &b_nCscSeg);
   fChain->SetBranchAddress("cscSegPhi", cscSegPhi, &b_cscSegPhi);
   fChain->SetBranchAddress("cscSegEta", cscSegEta, &b_cscSegEta);
   fChain->SetBranchAddress("cscSegX", cscSegX, &b_cscSegX);
   fChain->SetBranchAddress("cscSegY", cscSegY, &b_cscSegY);
   fChain->SetBranchAddress("cscSegZ", cscSegZ, &b_cscSegZ);
   fChain->SetBranchAddress("cscSegT", cscSegT, &b_cscSegT);
   fChain->SetBranchAddress("cscSegNRecHits", cscSegNRecHits, &b_cscSegNRecHits);
   fChain->SetBranchAddress("ncscRechits", &ncscRechits, &b_ncscRechits);
   fChain->SetBranchAddress("cscRechitsPhi", cscRechitsPhi, &b_cscRechitsPhi);
   fChain->SetBranchAddress("cscRechitsEta", cscRechitsEta, &b_cscRechitsEta);
   fChain->SetBranchAddress("cscRechitsX", cscRechitsX, &b_cscRechitsX);
   fChain->SetBranchAddress("cscRechitsY", cscRechitsY, &b_cscRechitsY);
   fChain->SetBranchAddress("cscRechitsZ", cscRechitsZ, &b_cscRechitsZ);
   fChain->SetBranchAddress("cscRechitsE", cscRechitsE, &b_cscRechitsE);
   fChain->SetBranchAddress("nCscRechitClusters", &nCscRechitClusters, &b_nCscRechitClusters);
   fChain->SetBranchAddress("cscRechitCluster_match_cscSegCluster_minDeltaR", cscRechitCluster_match_cscSegCluster_minDeltaR, &b_cscRechitCluster_match_cscSegCluster_minDeltaR);
   fChain->SetBranchAddress("cscRechitCluster_match_cscSegCluster_index", cscRechitCluster_match_cscSegCluster_index, &b_cscRechitCluster_match_cscSegCluster_index);
   fChain->SetBranchAddress("cscRechitCluster_match_gParticle_minDeltaR", cscRechitCluster_match_gParticle_minDeltaR, &b_cscRechitCluster_match_gParticle_minDeltaR);
   fChain->SetBranchAddress("cscRechitCluster_match_gParticle_index", cscRechitCluster_match_gParticle_index, &b_cscRechitCluster_match_gParticle_index);
   fChain->SetBranchAddress("cscRechitCluster_match_gParticle_id", cscRechitCluster_match_gParticle_id, &b_cscRechitCluster_match_gParticle_id);
   fChain->SetBranchAddress("cscRechitClusterX", cscRechitClusterX, &b_cscRechitClusterX);
   fChain->SetBranchAddress("cscRechitClusterY", cscRechitClusterY, &b_cscRechitClusterY);
   fChain->SetBranchAddress("cscRechitClusterZ", cscRechitClusterZ, &b_cscRechitClusterZ);
   fChain->SetBranchAddress("nCscSegClusters", &nCscSegClusters, &b_nCscSegClusters);
   fChain->SetBranchAddress("cscSegCluster_match_gParticle_minDeltaR", cscSegCluster_match_gParticle_minDeltaR, &b_cscSegCluster_match_gParticle_minDeltaR);
   fChain->SetBranchAddress("cscSegCluster_match_gParticle_index", cscSegCluster_match_gParticle_index, &b_cscSegCluster_match_gParticle_index);
   fChain->SetBranchAddress("cscSegCluster_match_gParticle_id", cscSegCluster_match_gParticle_id, &b_cscSegCluster_match_gParticle_id);
   fChain->SetBranchAddress("cscSegClusterX", cscSegClusterX, &b_cscSegClusterX);
   fChain->SetBranchAddress("cscSegClusterY", cscSegClusterY, &b_cscSegClusterY);
   fChain->SetBranchAddress("cscSegClusterZ", cscSegClusterZ, &b_cscSegClusterZ);
   fChain->SetBranchAddress("nDtRechits", &nDtRechits, &b_nDtRechits);
   fChain->SetBranchAddress("dtRechitCorrectX", dtRechitCorrectX, &b_dtRechitCorrectX);
   fChain->SetBranchAddress("dtRechitCorrectY", dtRechitCorrectY, &b_dtRechitCorrectY);
   fChain->SetBranchAddress("dtRechitCorrectZ", dtRechitCorrectZ, &b_dtRechitCorrectZ);
   fChain->SetBranchAddress("dtRechitCorrectEta", dtRechitCorrectEta, &b_dtRechitCorrectEta);
   fChain->SetBranchAddress("dtRechitCorrectPhi", dtRechitCorrectPhi, &b_dtRechitCorrectPhi);
   fChain->SetBranchAddress("nDtRechitClusters", &nDtRechitClusters, &b_nDtRechitClusters);
   fChain->SetBranchAddress("dtRechitCluster_match_gParticle_minDeltaR", dtRechitCluster_match_gParticle_minDeltaR, &b_dtRechitCluster_match_gParticle_minDeltaR);
   fChain->SetBranchAddress("dtRechitCluster_match_gParticle_index", dtRechitCluster_match_gParticle_index, &b_dtRechitCluster_match_gParticle_index);
   fChain->SetBranchAddress("dtRechitCluster_match_gParticle_id", dtRechitCluster_match_gParticle_id, &b_dtRechitCluster_match_gParticle_id);
   fChain->SetBranchAddress("dtRechitClusterX", dtRechitClusterX, &b_dtRechitClusterX);
   fChain->SetBranchAddress("dtRechitClusterY", dtRechitClusterY, &b_dtRechitClusterY);
   fChain->SetBranchAddress("dtRechitClusterZ", dtRechitClusterZ, &b_dtRechitClusterZ);
   fChain->SetBranchAddress("dtRechitClusterPhi", dtRechitClusterPhi, &b_dtRechitClusterPhi);
   fChain->SetBranchAddress("dtRechitClusterEta", dtRechitClusterEta, &b_dtRechitClusterEta);
   fChain->SetBranchAddress("nRpc", &nRpc, &b_nRpc);
   fChain->SetBranchAddress("rpcPhi", rpcPhi, &b_rpcPhi);
   fChain->SetBranchAddress("rpcEta", rpcEta, &b_rpcEta);
   fChain->SetBranchAddress("rpcX", rpcX, &b_rpcX);
   fChain->SetBranchAddress("rpcY", rpcY, &b_rpcY);
   fChain->SetBranchAddress("rpcZ", rpcZ, &b_rpcZ);
   fChain->SetBranchAddress("nDtSeg", &nDtSeg, &b_nDtSeg);
   fChain->SetBranchAddress("dtSegPhi", dtSegPhi, &b_dtSegPhi);
   fChain->SetBranchAddress("dtSegEta", dtSegEta, &b_dtSegEta);
   fChain->SetBranchAddress("dtSegX", dtSegX, &b_dtSegX);
   fChain->SetBranchAddress("dtSegY", dtSegY, &b_dtSegY);
   fChain->SetBranchAddress("dtSegZ", dtSegZ, &b_dtSegZ);
   fChain->SetBranchAddress("nDtSegClusters", &nDtSegClusters, &b_nDtSegClusters);
   fChain->SetBranchAddress("dtSegCluster_match_gParticle_minDeltaR", dtSegCluster_match_gParticle_minDeltaR, &b_dtSegCluster_match_gParticle_minDeltaR);
   fChain->SetBranchAddress("dtSegCluster_match_gParticle_index", dtSegCluster_match_gParticle_index, &b_dtSegCluster_match_gParticle_index);
   fChain->SetBranchAddress("dtSegCluster_match_gParticle_id", dtSegCluster_match_gParticle_id, &b_dtSegCluster_match_gParticle_id);
   fChain->SetBranchAddress("dtSegClusterX", dtSegClusterX, &b_dtSegClusterX);
   fChain->SetBranchAddress("dtSegClusterY", dtSegClusterY, &b_dtSegClusterY);
   fChain->SetBranchAddress("dtSegClusterZ", dtSegClusterZ, &b_dtSegClusterZ);
   fChain->SetBranchAddress("dtSegClusterPhi", dtSegClusterPhi, &b_dtSegClusterPhi);
   fChain->SetBranchAddress("dtSegClusterEta", dtSegClusterEta, &b_dtSegClusterEta);
   fChain->SetBranchAddress("nRechits", &nRechits, &b_nRechits);
   fChain->SetBranchAddress("ecalRechit_Eta", &ecalRechit_Eta, &b_ecalRechit_Eta);
   fChain->SetBranchAddress("ecalRechit_Phi", &ecalRechit_Phi, &b_ecalRechit_Phi);
   fChain->SetBranchAddress("ecalRechit_E", &ecalRechit_E, &b_ecalRechit_E);
   fChain->SetBranchAddress("ecalRechit_T", &ecalRechit_T, &b_ecalRechit_T);
   fChain->SetBranchAddress("nHBHERechits", &nHBHERechits, &b_nHBHERechits);
   fChain->SetBranchAddress("hbheRechit_Eta", hbheRechit_Eta, &b_hbheRechit_Eta);
   fChain->SetBranchAddress("hbheRechit_Phi", hbheRechit_Phi, &b_hbheRechit_Phi);
   fChain->SetBranchAddress("hbheRechit_E", hbheRechit_E, &b_hbheRechit_E);
   fChain->SetBranchAddress("hbheRechit_X", hbheRechit_X, &b_hbheRechit_X);
   fChain->SetBranchAddress("hbheRechit_Y", hbheRechit_Y, &b_hbheRechit_Y);
   fChain->SetBranchAddress("hbheRechit_Z", hbheRechit_Z, &b_hbheRechit_Z);
   fChain->SetBranchAddress("hbheRechit_time", hbheRechit_time, &b_hbheRechit_time);
   fChain->SetBranchAddress("hbheRechit_auxTDC", hbheRechit_auxTDC, &b_hbheRechit_auxTDC);
   fChain->SetBranchAddress("hbheRechit_timeFalling", hbheRechit_timeFalling, &b_hbheRechit_timeFalling);
   fChain->SetBranchAddress("hbheRechit_iEta", hbheRechit_iEta, &b_hbheRechit_iEta);
   fChain->SetBranchAddress("hbheRechit_iPhi", hbheRechit_iPhi, &b_hbheRechit_iPhi);
   fChain->SetBranchAddress("hbheRechit_depth", hbheRechit_depth, &b_hbheRechit_depth);
   fChain->SetBranchAddress("nTracks", &nTracks, &b_nTracks);
   fChain->SetBranchAddress("track_Pt", &track_Pt, &b_track_Pt);
   fChain->SetBranchAddress("track_Eta", &track_Eta, &b_track_Eta);
   fChain->SetBranchAddress("track_Phi", &track_Phi, &b_track_Phi);
   fChain->SetBranchAddress("track_charge", &track_charge, &b_track_charge);
   fChain->SetBranchAddress("track_bestVertexIndex", &track_bestVertexIndex, &b_track_bestVertexIndex);
   fChain->SetBranchAddress("track_nMissingInnerHits", &track_nMissingInnerHits, &b_track_nMissingInnerHits);
   fChain->SetBranchAddress("track_nMissingOuterHits", &track_nMissingOuterHits, &b_track_nMissingOuterHits);
   fChain->SetBranchAddress("track_nPixelHits", &track_nPixelHits, &b_track_nPixelHits);
   fChain->SetBranchAddress("track_angle", &track_angle, &b_track_angle);
   fChain->SetBranchAddress("track_nHits", &track_nHits, &b_track_nHits);
   fChain->SetBranchAddress("track_dxyToBS", &track_dxyToBS, &b_track_dxyToBS);
   fChain->SetBranchAddress("track_dxyErr", &track_dxyErr, &b_track_dxyErr);
   fChain->SetBranchAddress("track_dzToPV", &track_dzToPV, &b_track_dzToPV);
   fChain->SetBranchAddress("track_dzErr", &track_dzErr, &b_track_dzErr);
   fChain->SetBranchAddress("track_chi2", &track_chi2, &b_track_chi2);
   fChain->SetBranchAddress("track_ndof", &track_ndof, &b_track_ndof);
   fChain->SetBranchAddress("nSecondaryVertices", &nSecondaryVertices, &b_nSecondaryVertices);
   fChain->SetBranchAddress("secVtx_Pt", secVtx_Pt, &b_secVtx_Pt);
   fChain->SetBranchAddress("secVtx_Eta", secVtx_Eta, &b_secVtx_Eta);
   fChain->SetBranchAddress("secVtx_Phi", secVtx_Phi, &b_secVtx_Phi);
   fChain->SetBranchAddress("secVtx_charge", secVtx_charge, &b_secVtx_charge);
   fChain->SetBranchAddress("secVtx_nConstituents", secVtx_nConstituents, &b_secVtx_nConstituents);
   fChain->SetBranchAddress("secVtx_X", secVtx_X, &b_secVtx_X);
   fChain->SetBranchAddress("secVtx_Y", secVtx_Y, &b_secVtx_Y);
   fChain->SetBranchAddress("secVtx_Z", secVtx_Z, &b_secVtx_Z);
   fChain->SetBranchAddress("secVtx_Distance", secVtx_Distance, &b_secVtx_Distance);
   fChain->SetBranchAddress("secVtx_DistanceError", secVtx_DistanceError, &b_secVtx_DistanceError);
   fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
   fChain->SetBranchAddress("jetE", jetE, &b_jetE);
   fChain->SetBranchAddress("jetPt", jetPt, &b_jetPt);
   fChain->SetBranchAddress("jetEta", jetEta, &b_jetEta);
   fChain->SetBranchAddress("jetPhi", jetPhi, &b_jetPhi);
   fChain->SetBranchAddress("jetCSV", jetCSV, &b_jetCSV);
   fChain->SetBranchAddress("jetCISV", jetCISV, &b_jetCISV);
   fChain->SetBranchAddress("jetCMVA", jetCMVA, &b_jetCMVA);
   fChain->SetBranchAddress("jetProbb", jetProbb, &b_jetProbb);
   fChain->SetBranchAddress("jetProbc", jetProbc, &b_jetProbc);
   fChain->SetBranchAddress("jetProbudsg", jetProbudsg, &b_jetProbudsg);
   fChain->SetBranchAddress("jetProbbb", jetProbbb, &b_jetProbbb);
   fChain->SetBranchAddress("jetMass", jetMass, &b_jetMass);
   fChain->SetBranchAddress("jetJetArea", jetJetArea, &b_jetJetArea);
   fChain->SetBranchAddress("jetPileupE", jetPileupE, &b_jetPileupE);
   fChain->SetBranchAddress("jetPileupId", jetPileupId, &b_jetPileupId);
   fChain->SetBranchAddress("jetPileupIdFlag", jetPileupIdFlag, &b_jetPileupIdFlag);
   fChain->SetBranchAddress("jetPassIDLoose", jetPassIDLoose, &b_jetPassIDLoose);
   fChain->SetBranchAddress("jetPassIDTight", jetPassIDTight, &b_jetPassIDTight);
   fChain->SetBranchAddress("jetPassMuFrac", jetPassMuFrac, &b_jetPassMuFrac);
   fChain->SetBranchAddress("jetPassEleFrac", jetPassEleFrac, &b_jetPassEleFrac);
   fChain->SetBranchAddress("jetPartonFlavor", jetPartonFlavor, &b_jetPartonFlavor);
   fChain->SetBranchAddress("jetHadronFlavor", jetHadronFlavor, &b_jetHadronFlavor);
   fChain->SetBranchAddress("jetElectronEnergyFraction", jetElectronEnergyFraction, &b_jetElectronEnergyFraction);
   fChain->SetBranchAddress("jetPhotonEnergyFraction", jetPhotonEnergyFraction, &b_jetPhotonEnergyFraction);
   fChain->SetBranchAddress("jetChargedHadronEnergyFraction", jetChargedHadronEnergyFraction, &b_jetChargedHadronEnergyFraction);
   fChain->SetBranchAddress("jetNeutralHadronEnergyFraction", jetNeutralHadronEnergyFraction, &b_jetNeutralHadronEnergyFraction);
   fChain->SetBranchAddress("jetMuonEnergyFraction", jetMuonEnergyFraction, &b_jetMuonEnergyFraction);
   fChain->SetBranchAddress("jetHOEnergyFraction", jetHOEnergyFraction, &b_jetHOEnergyFraction);
   fChain->SetBranchAddress("jetHFHadronEnergyFraction", jetHFHadronEnergyFraction, &b_jetHFHadronEnergyFraction);
   fChain->SetBranchAddress("jetHFEMEnergyFraction", jetHFEMEnergyFraction, &b_jetHFEMEnergyFraction);
   fChain->SetBranchAddress("jetChargedHadronMultiplicity", jetChargedHadronMultiplicity, &b_jetChargedHadronMultiplicity);
   fChain->SetBranchAddress("jetNeutralHadronMultiplicity", jetNeutralHadronMultiplicity, &b_jetNeutralHadronMultiplicity);
   fChain->SetBranchAddress("jetPhotonMultiplicity", jetPhotonMultiplicity, &b_jetPhotonMultiplicity);
   fChain->SetBranchAddress("jetElectronMultiplicity", jetElectronMultiplicity, &b_jetElectronMultiplicity);
   fChain->SetBranchAddress("jetMuonMultiplicity", jetMuonMultiplicity, &b_jetMuonMultiplicity);
   fChain->SetBranchAddress("jetNSV", jetNSV, &b_jetNSV);
   fChain->SetBranchAddress("jetNSVCand", jetNSVCand, &b_jetNSVCand);
   fChain->SetBranchAddress("jetNVertexTracks", jetNVertexTracks, &b_jetNVertexTracks);
   fChain->SetBranchAddress("jetNSelectedTracks", jetNSelectedTracks, &b_jetNSelectedTracks);
   fChain->SetBranchAddress("jetDRSVJet", jetDRSVJet, &b_jetDRSVJet);
   fChain->SetBranchAddress("jetFlightDist2D", jetFlightDist2D, &b_jetFlightDist2D);
   fChain->SetBranchAddress("jetFlightDist2DError", jetFlightDist2DError, &b_jetFlightDist2DError);
   fChain->SetBranchAddress("jetFlightDist3D", jetFlightDist3D, &b_jetFlightDist3D);
   fChain->SetBranchAddress("jetFlightDist3DError", jetFlightDist3DError, &b_jetFlightDist3DError);
   fChain->SetBranchAddress("jetSV_x", jetSV_x, &b_jetSV_x);
   fChain->SetBranchAddress("jetSV_y", jetSV_y, &b_jetSV_y);
   fChain->SetBranchAddress("jetSV_z", jetSV_z, &b_jetSV_z);
   fChain->SetBranchAddress("jetSVNTracks", jetSVNTracks, &b_jetSVNTracks);
   fChain->SetBranchAddress("jetSVMass", jetSVMass, &b_jetSVMass);
   fChain->SetBranchAddress("jetAllMuonPt", jetAllMuonPt, &b_jetAllMuonPt);
   fChain->SetBranchAddress("jetAllMuonEta", jetAllMuonEta, &b_jetAllMuonEta);
   fChain->SetBranchAddress("jetAllMuonPhi", jetAllMuonPhi, &b_jetAllMuonPhi);
   fChain->SetBranchAddress("jetAllMuonM", jetAllMuonM, &b_jetAllMuonM);
   fChain->SetBranchAddress("jetPtWeightedDZ", jetPtWeightedDZ, &b_jetPtWeightedDZ);
   fChain->SetBranchAddress("jetNRechits", jetNRechits, &b_jetNRechits);
   fChain->SetBranchAddress("jetRechitE", jetRechitE, &b_jetRechitE);
   fChain->SetBranchAddress("jetRechitT", jetRechitT, &b_jetRechitT);
   fChain->SetBranchAddress("jetRechitT_rms", jetRechitT_rms, &b_jetRechitT_rms);
   fChain->SetBranchAddress("jetRechitE_Error", jetRechitE_Error, &b_jetRechitE_Error);
   fChain->SetBranchAddress("jetRechitT_Error", jetRechitT_Error, &b_jetRechitT_Error);
   fChain->SetBranchAddress("jetAlphaMax", jetAlphaMax, &b_jetAlphaMax);
   fChain->SetBranchAddress("jetBetaMax", jetBetaMax, &b_jetBetaMax);
   fChain->SetBranchAddress("jetGammaMax_ET", jetGammaMax_ET, &b_jetGammaMax_ET);
   fChain->SetBranchAddress("jetGammaMax_EM", jetGammaMax_EM, &b_jetGammaMax_EM);
   fChain->SetBranchAddress("jetGammaMax_Hadronic", jetGammaMax_Hadronic, &b_jetGammaMax_Hadronic);
   fChain->SetBranchAddress("jetGammaMax", jetGammaMax, &b_jetGammaMax);
   fChain->SetBranchAddress("jetPtAllTracks", jetPtAllTracks, &b_jetPtAllTracks);
   fChain->SetBranchAddress("jetPtAllPVTracks", jetPtAllPVTracks, &b_jetPtAllPVTracks);
   fChain->SetBranchAddress("jetMedianTheta2D", jetMedianTheta2D, &b_jetMedianTheta2D);
   fChain->SetBranchAddress("jetMedianIP", jetMedianIP, &b_jetMedianIP);
   fChain->SetBranchAddress("jetMinDeltaRAllTracks", jetMinDeltaRAllTracks, &b_jetMinDeltaRAllTracks);
   fChain->SetBranchAddress("jetMinDeltaRPVTracks", jetMinDeltaRPVTracks, &b_jetMinDeltaRPVTracks);
   fChain->SetBranchAddress("jet_sig_et1", jet_sig_et1, &b_jet_sig_et1);
   fChain->SetBranchAddress("jet_sig_et2", jet_sig_et2, &b_jet_sig_et2);
   fChain->SetBranchAddress("jet_energy_frac", jet_energy_frac, &b_jet_energy_frac);
   fChain->SetBranchAddress("jetAlphaMax_wp", jetAlphaMax_wp, &b_jetAlphaMax_wp);
   fChain->SetBranchAddress("jetBetaMax_wp", jetBetaMax_wp, &b_jetBetaMax_wp);
   fChain->SetBranchAddress("jetGammaMax_ET_wp", jetGammaMax_ET_wp, &b_jetGammaMax_ET_wp);
   fChain->SetBranchAddress("jetGammaMax_EM_wp", jetGammaMax_EM_wp, &b_jetGammaMax_EM_wp);
   fChain->SetBranchAddress("jetGammaMax_Hadronic_wp", jetGammaMax_Hadronic_wp, &b_jetGammaMax_Hadronic_wp);
   fChain->SetBranchAddress("jetGammaMax_wp", jetGammaMax_wp, &b_jetGammaMax_wp);
   fChain->SetBranchAddress("jetPtAllTracks_wp", jetPtAllTracks_wp, &b_jetPtAllTracks_wp);
   fChain->SetBranchAddress("jetPtAllPVTracks_wp", jetPtAllPVTracks_wp, &b_jetPtAllPVTracks_wp);
   fChain->SetBranchAddress("jetMedianTheta2D_wp", jetMedianTheta2D_wp, &b_jetMedianTheta2D_wp);
   fChain->SetBranchAddress("jetMedianIP_wp", jetMedianIP_wp, &b_jetMedianIP_wp);
   fChain->SetBranchAddress("jetMinDeltaRAllTracks_wp", jetMinDeltaRAllTracks_wp, &b_jetMinDeltaRAllTracks_wp);
   fChain->SetBranchAddress("jetMinDeltaRPVTracks_wp", jetMinDeltaRPVTracks_wp, &b_jetMinDeltaRPVTracks_wp);
   fChain->SetBranchAddress("jetNPFCands", jetNPFCands, &b_jetNPFCands);
   fChain->SetBranchAddress("jetPFCandIndex", jetPFCandIndex, &b_jetPFCandIndex);
   fChain->SetBranchAddress("nFatJets", &nFatJets, &b_nFatJets);
   fChain->SetBranchAddress("fatJetE", &fatJetE, &b_fatJetE);
   fChain->SetBranchAddress("fatJetPt", &fatJetPt, &b_fatJetPt);
   fChain->SetBranchAddress("fatJetEta", &fatJetEta, &b_fatJetEta);
   fChain->SetBranchAddress("fatJetPhi", &fatJetPhi, &b_fatJetPhi);
   fChain->SetBranchAddress("fatJetCorrectedPt", &fatJetCorrectedPt, &b_fatJetCorrectedPt);
   fChain->SetBranchAddress("fatJetPrunedM", &fatJetPrunedM, &b_fatJetPrunedM);
   fChain->SetBranchAddress("fatJetTrimmedM", &fatJetTrimmedM, &b_fatJetTrimmedM);
   fChain->SetBranchAddress("fatJetFilteredM", &fatJetFilteredM, &b_fatJetFilteredM);
   fChain->SetBranchAddress("fatJetSoftDropM", &fatJetSoftDropM, &b_fatJetSoftDropM);
   fChain->SetBranchAddress("fatJetCorrectedSoftDropM", &fatJetCorrectedSoftDropM, &b_fatJetCorrectedSoftDropM);
   fChain->SetBranchAddress("fatJetUncorrectedSoftDropM", &fatJetUncorrectedSoftDropM, &b_fatJetUncorrectedSoftDropM);
   fChain->SetBranchAddress("fatJetTau1", &fatJetTau1, &b_fatJetTau1);
   fChain->SetBranchAddress("fatJetTau2", &fatJetTau2, &b_fatJetTau2);
   fChain->SetBranchAddress("fatJetTau3", &fatJetTau3, &b_fatJetTau3);
   fChain->SetBranchAddress("fatJetMaxSubjetCSV", &fatJetMaxSubjetCSV, &b_fatJetMaxSubjetCSV);
   fChain->SetBranchAddress("fatJetPassIDLoose", &fatJetPassIDLoose, &b_fatJetPassIDLoose);
   fChain->SetBranchAddress("fatJetPassIDTight", &fatJetPassIDTight, &b_fatJetPassIDTight);
   fChain->SetBranchAddress("fatJetElectronEnergyFraction", &fatJetElectronEnergyFraction, &b_fatJetElectronEnergyFraction);
   fChain->SetBranchAddress("fatJetPhotonEnergyFraction", &fatJetPhotonEnergyFraction, &b_fatJetPhotonEnergyFraction);
   fChain->SetBranchAddress("fatJetChargedHadronEnergyFraction", &fatJetChargedHadronEnergyFraction, &b_fatJetChargedHadronEnergyFraction);
   fChain->SetBranchAddress("fatJetNeutralHadronEnergyFraction", &fatJetNeutralHadronEnergyFraction, &b_fatJetNeutralHadronEnergyFraction);
   fChain->SetBranchAddress("fatJetMuonEnergyFraction", &fatJetMuonEnergyFraction, &b_fatJetMuonEnergyFraction);
   fChain->SetBranchAddress("fatJetHOEnergyFraction", &fatJetHOEnergyFraction, &b_fatJetHOEnergyFraction);
   fChain->SetBranchAddress("fatJetHFHadronEnergyFraction", &fatJetHFHadronEnergyFraction, &b_fatJetHFHadronEnergyFraction);
   fChain->SetBranchAddress("fatJetHFEMEnergyFraction", &fatJetHFEMEnergyFraction, &b_fatJetHFEMEnergyFraction);
   fChain->SetBranchAddress("fatJetChargedHadronMultiplicity", &fatJetChargedHadronMultiplicity, &b_fatJetChargedHadronMultiplicity);
   fChain->SetBranchAddress("fatJetNeutralHadronMultiplicity", &fatJetNeutralHadronMultiplicity, &b_fatJetNeutralHadronMultiplicity);
   fChain->SetBranchAddress("fatJetPhotonMultiplicity", &fatJetPhotonMultiplicity, &b_fatJetPhotonMultiplicity);
   fChain->SetBranchAddress("fatJetElectronMultiplicity", &fatJetElectronMultiplicity, &b_fatJetElectronMultiplicity);
   fChain->SetBranchAddress("fatJetMuonMultiplicity", &fatJetMuonMultiplicity, &b_fatJetMuonMultiplicity);
   fChain->SetBranchAddress("fatJetNRechits", &fatJetNRechits, &b_fatJetNRechits);
   fChain->SetBranchAddress("fatJetRechitE", &fatJetRechitE, &b_fatJetRechitE);
   fChain->SetBranchAddress("fatJetRechitT", &fatJetRechitT, &b_fatJetRechitT);
   fChain->SetBranchAddress("fatJetRechitT_rms", &fatJetRechitT_rms, &b_fatJetRechitT_rms);
   fChain->SetBranchAddress("fatJetRechitE_Error", &fatJetRechitE_Error, &b_fatJetRechitE_Error);
   fChain->SetBranchAddress("fatJetRechitT_Error", &fatJetRechitT_Error, &b_fatJetRechitT_Error);
   fChain->SetBranchAddress("fatJetAlphaMax", &fatJetAlphaMax, &b_fatJetAlphaMax);
   fChain->SetBranchAddress("fatJetBetaMax", &fatJetBetaMax, &b_fatJetBetaMax);
   fChain->SetBranchAddress("fatJetGammaMax_ET", &fatJetGammaMax_ET, &b_fatJetGammaMax_ET);
   fChain->SetBranchAddress("fatJetGammaMax_EM", &fatJetGammaMax_EM, &b_fatJetGammaMax_EM);
   fChain->SetBranchAddress("fatJetGammaMax_Hadronic", &fatJetGammaMax_Hadronic, &b_fatJetGammaMax_Hadronic);
   fChain->SetBranchAddress("fatJetGammaMax", &fatJetGammaMax, &b_fatJetGammaMax);
   fChain->SetBranchAddress("fatJetPtAllTracks", &fatJetPtAllTracks, &b_fatJetPtAllTracks);
   fChain->SetBranchAddress("fatJetPtAllPVTracks", &fatJetPtAllPVTracks, &b_fatJetPtAllPVTracks);
   fChain->SetBranchAddress("fatJetMedianTheta2D", &fatJetMedianTheta2D, &b_fatJetMedianTheta2D);
   fChain->SetBranchAddress("fatJetMedianIP", &fatJetMedianIP, &b_fatJetMedianIP);
   fChain->SetBranchAddress("fatJetMinDeltaRAllTracks", &fatJetMinDeltaRAllTracks, &b_fatJetMinDeltaRAllTracks);
   fChain->SetBranchAddress("fatJetMinDeltaRPVTracks", &fatJetMinDeltaRPVTracks, &b_fatJetMinDeltaRPVTracks);
   fChain->SetBranchAddress("fatJetNPFCands", &fatJetNPFCands, &b_fatJetNPFCands);
   fChain->SetBranchAddress("fatJetPFCandIndex", &fatJetPFCandIndex, &b_fatJetPFCandIndex);
   fChain->SetBranchAddress("metPt", &metPt, &b_metPt);
   fChain->SetBranchAddress("metPhi", &metPhi, &b_metPhi);
   fChain->SetBranchAddress("sumMET", &sumMET, &b_sumMET);
   fChain->SetBranchAddress("metUncorrectedPt", &metUncorrectedPt, &b_metUncorrectedPt);
   fChain->SetBranchAddress("metUncorrectedPhi", &metUncorrectedPhi, &b_metUncorrectedPhi);
   fChain->SetBranchAddress("metType1Pt", &metType1Pt, &b_metType1Pt);
   fChain->SetBranchAddress("metType1Phi", &metType1Phi, &b_metType1Phi);
   fChain->SetBranchAddress("metPuppiPt", &metPuppiPt, &b_metPuppiPt);
   fChain->SetBranchAddress("metPuppiPhi", &metPuppiPhi, &b_metPuppiPhi);
   fChain->SetBranchAddress("metCaloPt", &metCaloPt, &b_metCaloPt);
   fChain->SetBranchAddress("metCaloPhi", &metCaloPhi, &b_metCaloPhi);
   fChain->SetBranchAddress("HLTDecision", HLTDecision, &b_HLTDecision);
   fChain->SetBranchAddress("HLTPrescale", HLTPrescale, &b_HLTPrescale);
   fChain->SetBranchAddress("nGenJets", &nGenJets, &b_nGenJets);
   fChain->SetBranchAddress("genJetE", &genJetE, &b_genJetE);
   fChain->SetBranchAddress("genJetPt", &genJetPt, &b_genJetPt);
   fChain->SetBranchAddress("genJetEta", &genJetEta, &b_genJetEta);
   fChain->SetBranchAddress("genJetPhi", &genJetPhi, &b_genJetPhi);
   fChain->SetBranchAddress("genMetPtCalo", &genMetPtCalo, &b_genMetPtCalo);
   fChain->SetBranchAddress("genMetPhiCalo", &genMetPhiCalo, &b_genMetPhiCalo);
   fChain->SetBranchAddress("genMetPtTrue", &genMetPtTrue, &b_genMetPtTrue);
   fChain->SetBranchAddress("genMetPhiTrue", &genMetPhiTrue, &b_genMetPhiTrue);
   fChain->SetBranchAddress("genVertexX", &genVertexX, &b_genVertexX);
   fChain->SetBranchAddress("genVertexY", &genVertexY, &b_genVertexY);
   fChain->SetBranchAddress("genVertexZ", &genVertexZ, &b_genVertexZ);
   fChain->SetBranchAddress("genVertexT", &genVertexT, &b_genVertexT);
   fChain->SetBranchAddress("genWeight", &genWeight, &b_genWeight);
   fChain->SetBranchAddress("genSignalProcessID", &genSignalProcessID, &b_genSignalProcessID);
   fChain->SetBranchAddress("genQScale", &genQScale, &b_genQScale);
   fChain->SetBranchAddress("genAlphaQCD", &genAlphaQCD, &b_genAlphaQCD);
   fChain->SetBranchAddress("genAlphaQED", &genAlphaQED, &b_genAlphaQED);
   fChain->SetBranchAddress("lheComments", &lheComments, &b_lheComments);
   fChain->SetBranchAddress("scaleWeights", &scaleWeights, &b_scaleWeights);
   fChain->SetBranchAddress("pdfWeights", &pdfWeights, &b_pdfWeights);
   fChain->SetBranchAddress("alphasWeights", &alphasWeights, &b_alphasWeights);
   fChain->SetBranchAddress("nGenParticle", &nGenParticle, &b_nGenParticle);
   fChain->SetBranchAddress("gParticleMotherId", &gParticleMotherId, &b_gParticleMotherId);
   fChain->SetBranchAddress("gParticleMotherIndex", &gParticleMotherIndex, &b_gParticleMotherIndex);
   fChain->SetBranchAddress("gParticleId", &gParticleId, &b_gParticleId);
   fChain->SetBranchAddress("gParticleStatus", &gParticleStatus, &b_gParticleStatus);
   fChain->SetBranchAddress("gParticleE", &gParticleE, &b_gParticleE);
   fChain->SetBranchAddress("gParticlePt", &gParticlePt, &b_gParticlePt);
   fChain->SetBranchAddress("gParticleEta", &gParticleEta, &b_gParticleEta);
   fChain->SetBranchAddress("gParticlePhi", &gParticlePhi, &b_gParticlePhi);
   fChain->SetBranchAddress("gParticleProdVertexX", &gParticleProdVertexX, &b_gParticleProdVertexX);
   fChain->SetBranchAddress("gParticleProdVertexY", &gParticleProdVertexY, &b_gParticleProdVertexY);
   fChain->SetBranchAddress("gParticleProdVertexZ", &gParticleProdVertexZ, &b_gParticleProdVertexZ);
   fChain->SetBranchAddress("gParticleDecayVertexX", &gParticleDecayVertexX, &b_gParticleDecayVertexX);
   fChain->SetBranchAddress("gParticleDecayVertexY", &gParticleDecayVertexY, &b_gParticleDecayVertexY);
   fChain->SetBranchAddress("gParticleDecayVertexZ", &gParticleDecayVertexZ, &b_gParticleDecayVertexZ);
   fChain->SetBranchAddress("gLLP_decay_vertex_x", gLLP_decay_vertex_x, &b_gLLP_decay_vertex_x);
   fChain->SetBranchAddress("gLLP_decay_vertex_y", gLLP_decay_vertex_y, &b_gLLP_decay_vertex_y);
   fChain->SetBranchAddress("gLLP_decay_vertex_z", gLLP_decay_vertex_z, &b_gLLP_decay_vertex_z);
   fChain->SetBranchAddress("gLLP_beta", gLLP_beta, &b_gLLP_beta);
   fChain->SetBranchAddress("gLLP_e", gLLP_e, &b_gLLP_e);
   fChain->SetBranchAddress("gLLP_pt", gLLP_pt, &b_gLLP_pt);
   fChain->SetBranchAddress("gLLP_eta", gLLP_eta, &b_gLLP_eta);
   fChain->SetBranchAddress("gLLP_phi", gLLP_phi, &b_gLLP_phi);
   fChain->SetBranchAddress("gLLP_csc", gLLP_csc, &b_gLLP_csc);
   fChain->SetBranchAddress("gLLP_dt", gLLP_dt, &b_gLLP_dt);
   fChain->SetBranchAddress("gLLP_travel_time", gLLP_travel_time, &b_gLLP_travel_time);
   fChain->SetBranchAddress("gLLP_daughter_id", gLLP_daughter_id, &b_gLLP_daughter_id);
   fChain->SetBranchAddress("gLLP_daughter_pt", gLLP_daughter_pt, &b_gLLP_daughter_pt);
   fChain->SetBranchAddress("gLLP_daughter_eta", gLLP_daughter_eta, &b_gLLP_daughter_eta);
   fChain->SetBranchAddress("gLLP_daughter_phi", gLLP_daughter_phi, &b_gLLP_daughter_phi);
   fChain->SetBranchAddress("gLLP_daughter_eta_ecalcorr", gLLP_daughter_eta_ecalcorr, &b_gLLP_daughter_eta_ecalcorr);
   fChain->SetBranchAddress("gLLP_daughter_phi_ecalcorr", gLLP_daughter_phi_ecalcorr, &b_gLLP_daughter_phi_ecalcorr);
   fChain->SetBranchAddress("gLLP_daughter_e", gLLP_daughter_e, &b_gLLP_daughter_e);
   fChain->SetBranchAddress("gLLP_daughter_mass", gLLP_daughter_mass, &b_gLLP_daughter_mass);
   fChain->SetBranchAddress("gen_time", gen_time, &b_gen_time);
   fChain->SetBranchAddress("gen_time_pv", gen_time_pv, &b_gen_time_pv);
   fChain->SetBranchAddress("photon_travel_time", photon_travel_time, &b_photon_travel_time);
   fChain->SetBranchAddress("photon_travel_time_pv", photon_travel_time_pv, &b_photon_travel_time_pv);
   fChain->SetBranchAddress("gLLP_daughter_travel_time", gLLP_daughter_travel_time, &b_gLLP_daughter_travel_time);
   fChain->SetBranchAddress("gLLP_grandaughter_id", gLLP_grandaughter_id, &b_gLLP_grandaughter_id);
   fChain->SetBranchAddress("gLLP_grandaughter_pt", gLLP_grandaughter_pt, &b_gLLP_grandaughter_pt);
   fChain->SetBranchAddress("gLLP_grandaughter_eta", gLLP_grandaughter_eta, &b_gLLP_grandaughter_eta);
   fChain->SetBranchAddress("gLLP_grandaughter_phi", gLLP_grandaughter_phi, &b_gLLP_grandaughter_phi);
   fChain->SetBranchAddress("gLLP_grandaughter_eta_ecalcorr", gLLP_grandaughter_eta_ecalcorr, &b_gLLP_grandaughter_eta_ecalcorr);
   fChain->SetBranchAddress("gLLP_grandaughter_phi_ecalcorr", gLLP_grandaughter_phi_ecalcorr, &b_gLLP_grandaughter_phi_ecalcorr);
   fChain->SetBranchAddress("gLLP_grandaughter_e", gLLP_grandaughter_e, &b_gLLP_grandaughter_e);
   fChain->SetBranchAddress("gLLP_grandaughter_mass", gLLP_grandaughter_mass, &b_gLLP_grandaughter_mass);
   fChain->SetBranchAddress("gen_time_dau", gen_time_dau, &b_gen_time_dau);
   fChain->SetBranchAddress("gen_time_dau_pv", gen_time_dau_pv, &b_gen_time_dau_pv);
   fChain->SetBranchAddress("photon_travel_time_dau", photon_travel_time_dau, &b_photon_travel_time_dau);
   fChain->SetBranchAddress("photon_travel_time_dau_pv", photon_travel_time_dau_pv, &b_photon_travel_time_dau_pv);
   fChain->SetBranchAddress("gLLP_grandaughter_travel_time", gLLP_grandaughter_travel_time, &b_gLLP_grandaughter_travel_time);
   Notify();
}

Bool_t LLP_HCAL_Analyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void LLP_HCAL_Analyzer::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t LLP_HCAL_Analyzer::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef LLP_HCAL_Analyzer_cxx
