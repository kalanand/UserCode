//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jul  8 18:13:12 2011 by ROOT version 5.27/06b
// from TTree WJet/V+jets Tree
// found on file: /uscms_data/d2/kalanand/WjjTrees/WenuJets_DataAll_GoldenJSON_917invpb.root
//////////////////////////////////////////////////////////

#ifndef kanaelec_h
#define kanaelec_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <iostream>
#include "TLorentzVector.h"

class kanaelec {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           numPFCorJets;
   Int_t           numPFCorJetBTags;
   Float_t         JetPFCor_Et[6];
   Float_t         JetPFCor_Pt[6];
   Float_t         JetPFCor_Eta[6];
   Float_t         JetPFCor_Phi[6];
   Float_t         JetPFCor_Theta[6];
   Float_t         JetPFCor_Px[6];
   Float_t         JetPFCor_Py[6];
   Float_t         JetPFCor_Pz[6];
   Float_t         JetPFCor_E[6];
   Float_t         JetPFCor_Y[6];
   Float_t         JetPFCor_Mass[6];
   Float_t         JetPFCor_etaetaMoment[6];
   Float_t         JetPFCor_phiphiMoment[6];
   Float_t         JetPFCor_etaphiMoment[6];
   Float_t         JetPFCor_maxDistance[6];
   Int_t           JetPFCor_nConstituents[6];
   Float_t         JetPFCor_Area[6];
   Float_t         VplusPFCorJet_Mass[6];
   Float_t         JetPFCor_dphiBoson[6];
   Float_t         JetPFCor_detaBoson[6];
   Float_t         JetPFCor_dRBoson[6];
   Float_t         JetPFCor_dphiMET[6];
   Float_t         JetPFCor_Response[6];
   Float_t         JetPFCor_bDiscriminator[6];
   Float_t         JetPFCor_secVertexMass[6];
   Float_t         JetPFCor_dphiBoson2[6];
   Float_t         JetPFCor_detaBoson2[6];
   Float_t         JetPFCor_dRBoson2[6];
   Float_t         VplusPFCorJet_Mass2[6];
   Float_t         JetPFCor_Response2[6];
   Float_t         JetPFCor_ChargedHadronEnergy[6];
   Float_t         JetPFCor_ChargedHadronEnergyFrac[6];
   Float_t         JetPFCor_NeutralHadronEnergy[6];
   Float_t         JetPFCor_NeutralHadronEnergyFrac[6];
   Float_t         JetPFCor_ChargedEmEnergy[6];
   Float_t         JetPFCor_ChargedEmEnergyFrac[6];
   Float_t         JetPFCor_ChargedMuEnergy[6];
   Float_t         JetPFCor_ChargedMuEnergyFrac[6];
   Float_t         JetPFCor_NeutralEmEnergy[6];
   Float_t         JetPFCor_NeutralEmEnergyFrac[6];
   Int_t           JetPFCor_ChargedMultiplicity[6];
   Int_t           JetPFCor_NeutralMultiplicity[6];
   Int_t           JetPFCor_MuonMultiplicity[6];
   Float_t         JetPFCor_PhotonEnergy[6];
   Float_t         JetPFCor_PhotonEnergyFraction[6];
   Float_t         JetPFCor_ElectronEnergy[6];
   Float_t         JetPFCor_ElectronEnergyFraction[6];
   Float_t         JetPFCor_MuonEnergy[6];
   Float_t         JetPFCor_MuonEnergyFraction[6];
   Float_t         JetPFCor_HFHadronEnergy[6];
   Float_t         JetPFCor_HFHadronEnergyFraction[6];
   Float_t         JetPFCor_HFEMEnergy[6];
   Float_t         JetPFCor_HFEMEnergyFraction[6];
   Int_t           JetPFCor_ChargedHadronMultiplicity[6];
   Int_t           JetPFCor_NeutralHadronMultiplicity[6];
   Int_t           JetPFCor_PhotonMultiplicity[6];
   Int_t           JetPFCor_ElectronMultiplicity[6];
   Int_t           JetPFCor_HFHadronMultiplicity[6];
   Int_t           JetPFCor_HFEMMultiplicity[6];
   Float_t         MassV2j_PFCor;
   Float_t         MassV3j_PFCor;
   Float_t         MassV4j_PFCor;
   Float_t         MassV5j_PFCor;
   Float_t         MassV6j_PFCor;
   Float_t         Mass2j_PFCor;
   Float_t         Mass3j_PFCor;
   Float_t         Mass4j_PFCor;
   Float_t         Mass5j_PFCor;
   Float_t         Mass6j_PFCor;
   Float_t         cosJacksonAngleV2j_PFCor;
   Float_t         cosJacksonAngle2j_PFCor;
   Float_t         cosJacksonAngleV3j_PFCor;
   Float_t         cosJacksonAngle3j12_PFCor;
   Float_t         cosJacksonAngle3j23_PFCor;
   Float_t         cosJacksonAngle3j31_PFCor;
   Float_t         cosphiDecayPlane_PFCor;
   Float_t         cosThetaLnu_PFCor;
   Float_t         cosThetaJJ_PFCor;
   Float_t         WJetsPullPFCor;
   Float_t         cosThetaJ1HiggsCM_PFCor;
   Float_t         cosThetaJ2HiggsCM_PFCor;
   Float_t         cosThetaL1HiggsCM_PFCor;
   Float_t         cosThetaL2HiggsCM_PFCor;
   Float_t         cosThetaV1HiggsCM_PFCor;
   Float_t         cosThetaV2HiggsCM_PFCor;
   Float_t         W_mass;
   Float_t         W_mt;
   Float_t         W_px;
   Float_t         W_py;
   Float_t         W_pz;
   Float_t         W_e;
   Float_t         W_pt;
   Float_t         W_et;
   Float_t         W_eta;
   Float_t         W_phi;
   Float_t         W_vx;
   Float_t         W_vy;
   Float_t         W_vz;
   Float_t         W_y;
   Int_t           W_numTightElectrons;
   Int_t           W_numLooseElectrons;
   Float_t         W_pzNu1;
   Float_t         W_pzNu2;
   Float_t         W_electron_px;
   Float_t         W_electron_py;
   Float_t         W_electron_pz;
   Float_t         W_electron_e;
   Float_t         W_electron_pt;
   Float_t         W_electron_et;
   Float_t         W_electron_eta;
   Float_t         W_electron_theta;
   Float_t         W_electron_phi;
   Int_t           W_electron_charge;
   Float_t         W_electron_vx;
   Float_t         W_electron_vy;
   Float_t         W_electron_vz;
   Float_t         W_electron_y;
   Float_t         W_electron_trackiso;
   Float_t         W_electron_hcaliso;
   Float_t         W_electron_ecaliso;
   Int_t           W_electron_classification;
   Float_t         W_electron_sc_x;
   Float_t         W_electron_sc_y;
   Float_t         W_electron_sc_z;
   Float_t         W_electron_sc_Theta;
   Float_t         W_electron_sc_Eta;
   Float_t         W_electron_sc_Phi;
   Float_t         W_electron_sc_E;
   Float_t         W_electron_sc_px;
   Float_t         W_electron_sc_py;
   Float_t         W_electron_sc_pz;
   Float_t         W_electron_sc_Pt;
   Float_t         W_electron_sc_Et;
   Float_t         W_electron_eoverp_out;
   Float_t         W_electron_eoverp_in;
   Float_t         W_electron_numbrem;
   Float_t         W_electron_fbrem;
   Float_t         W_electron_deltaeta_in;
   Float_t         W_electron_deltaphi_in;
   Float_t         W_electron_deltaphi_out;
   Float_t         W_electron_deltaeta_out;
   Float_t         W_electron_trackmom_calo;
   Float_t         W_electron_trackmom_vtx;
   Float_t         W_electron_hovere;
   Float_t         W_electron_e9e25;
   Float_t         W_electron_sigmaetaeta;
   Float_t         W_electron_sigmaietaieta;
   Int_t           W_electron_missingHits;
   Float_t         W_electron_dist;
   Float_t         W_electron_dcot;
   Float_t         W_electron_convradius;
   Bool_t          W_electron_isWP95;
   Bool_t          W_electron_isWP80;
   Bool_t          W_electron_isWP70;
   Float_t         W_electron_d0bsp;
   Float_t         W_electron_dz000;
   Int_t           event_runNo;
   Int_t           event_evtNo;
   Int_t           event_lumi;
   Int_t           event_bunch;
   Int_t           event_nPV;
   Float_t         event_PVx[30];
   Float_t         event_PVy[30];
   Float_t         event_PVz[30];
   Float_t         event_met_calomet;
   Float_t         event_met_calosumet;
   Float_t         event_met_calometsignificance;
   Float_t         event_met_calometPhi;
   Float_t         event_met_tcmet;
   Float_t         event_met_tcsumet;
   Float_t         event_met_tcmetsignificance;
   Float_t         event_met_tcmetPhi;
   Float_t         event_met_pfmet;
   Float_t         event_met_pfsumet;
   Float_t         event_met_pfmetsignificance;
   Float_t         event_met_pfmetPhi;
   Float_t         event_fastJetRho;
   Float_t         event_RhoForLeptonIsolation;
   Float_t         event_BeamSpot_x;
   Float_t         event_BeamSpot_y;
   Float_t         event_BeamSpot_z;
   Int_t           numW;

   // List of branches
   TBranch        *b_numPFCorJets;   //!
   TBranch        *b_numPFCorJetBTags;   //!
   TBranch        *b_JetPFCor_Et;   //!
   TBranch        *b_JetPFCor_Pt;   //!
   TBranch        *b_JetPFCor_Eta;   //!
   TBranch        *b_JetPFCor_Phi;   //!
   TBranch        *b_JetPFCor_Theta;   //!
   TBranch        *b_JetPFCor_Px;   //!
   TBranch        *b_JetPFCor_Py;   //!
   TBranch        *b_JetPFCor_Pz;   //!
   TBranch        *b_JetPFCor_E;   //!
   TBranch        *b_JetPFCor_Y;   //!
   TBranch        *b_JetPFCor_Mass;   //!
   TBranch        *b_JetPFCor_etaetaMoment;   //!
   TBranch        *b_JetPFCor_phiphiMoment;   //!
   TBranch        *b_JetPFCor_etaphiMoment;   //!
   TBranch        *b_JetPFCor_maxDistance;   //!
   TBranch        *b_JetPFCor_nConstituents;   //!
   TBranch        *b_JetPFCor_Area;   //!
   TBranch        *b_VplusPFCorJet_Mass;   //!
   TBranch        *b_JetPFCor_dphiBoson;   //!
   TBranch        *b_JetPFCor_detaBoson;   //!
   TBranch        *b_JetPFCor_dRBoson;   //!
   TBranch        *b_JetPFCor_dphiMET;   //!
   TBranch        *b_JetPFCor_Response;   //!
   TBranch        *b_JetPFCor_bDiscriminator;   //!
   TBranch        *b_JetPFCor_secVertexMass;   //!
   TBranch        *b_JetPFCor_dphiBoson2;   //!
   TBranch        *b_JetPFCor_detaBoson2;   //!
   TBranch        *b_JetPFCor_dRBoson2;   //!
   TBranch        *b_VplusPFCorJet_Mass2;   //!
   TBranch        *b_JetPFCor_Response2;   //!
   TBranch        *b_JetPFCor_ChargedHadronEnergy;   //!
   TBranch        *b_JetPFCor_ChargedHadronEnergyFrac;   //!
   TBranch        *b_JetPFCor_NeutralHadronEnergy;   //!
   TBranch        *b_JetPFCor_NeutralHadronEnergyFrac;   //!
   TBranch        *b_JetPFCor_ChargedEmEnergy;   //!
   TBranch        *b_JetPFCor_ChargedEmEnergyFrac;   //!
   TBranch        *b_JetPFCor_ChargedMuEnergy;   //!
   TBranch        *b_JetPFCor_ChargedMuEnergyFrac;   //!
   TBranch        *b_JetPFCor_NeutralEmEnergy;   //!
   TBranch        *b_JetPFCor_NeutralEmEnergyFrac;   //!
   TBranch        *b_JetPFCor_ChargedMultiplicity;   //!
   TBranch        *b_JetPFCor_NeutralMultiplicity;   //!
   TBranch        *b_JetPFCor_MuonMultiplicity;   //!
   TBranch        *b_JetPFCor_PhotonEnergy;   //!
   TBranch        *b_JetPFCor_PhotonEnergyFraction;   //!
   TBranch        *b_JetPFCor_ElectronEnergy;   //!
   TBranch        *b_JetPFCor_ElectronEnergyFraction;   //!
   TBranch        *b_JetPFCor_MuonEnergy;   //!
   TBranch        *b_JetPFCor_MuonEnergyFraction;   //!
   TBranch        *b_JetPFCor_HFHadronEnergy;   //!
   TBranch        *b_JetPFCor_HFHadronEnergyFraction;   //!
   TBranch        *b_JetPFCor_HFEMEnergy;   //!
   TBranch        *b_JetPFCor_HFEMEnergyFraction;   //!
   TBranch        *b_JetPFCor_ChargedHadronMultiplicity;   //!
   TBranch        *b_JetPFCor_NeutralHadronMultiplicity;   //!
   TBranch        *b_JetPFCor_PhotonMultiplicity;   //!
   TBranch        *b_JetPFCor_ElectronMultiplicity;   //!
   TBranch        *b_JetPFCor_HFHadronMultiplicity;   //!
   TBranch        *b_JetPFCor_HFEMMultiplicity;   //!
   TBranch        *b_MassV2j_PFCor;   //!
   TBranch        *b_MassV3j_PFCor;   //!
   TBranch        *b_MassV4j_PFCor;   //!
   TBranch        *b_MassV5j_PFCor;   //!
   TBranch        *b_MassV6j_PFCor;   //!
   TBranch        *b_Mass2j_PFCor;   //!
   TBranch        *b_Mass3j_PFCor;   //!
   TBranch        *b_Mass4j_PFCor;   //!
   TBranch        *b_Mass5j_PFCor;   //!
   TBranch        *b_Mass6j_PFCor;   //!
   TBranch        *b_cosJacksonAngleV2j_PFCor;   //!
   TBranch        *b_cosJacksonAngle2j_PFCor;   //!
   TBranch        *b_cosJacksonAngleV3j_PFCor;   //!
   TBranch        *b_cosJacksonAngle3j12_PFCor;   //!
   TBranch        *b_cosJacksonAngle3j23_PFCor;   //!
   TBranch        *b_cosJacksonAngle3j31_PFCor;   //!
   TBranch        *b_cosphiDecayPlane_PFCor;   //!
   TBranch        *b_cosThetaLnu_PFCor;   //!
   TBranch        *b_cosThetaJJ_PFCor;   //!
   TBranch        *b_WJetsPullPFCor;   //!
   TBranch        *b_cosThetaJ1HiggsCM_PFCor;   //!
   TBranch        *b_cosThetaJ2HiggsCM_PFCor;   //!
   TBranch        *b_cosThetaL1HiggsCM_PFCor;   //!
   TBranch        *b_cosThetaL2HiggsCM_PFCor;   //!
   TBranch        *b_cosThetaV1HiggsCM_PFCor;   //!
   TBranch        *b_cosThetaV2HiggsCM_PFCor;   //!
   TBranch        *b_W_mass;   //!
   TBranch        *b_W_mt;   //!
   TBranch        *b_W_px;   //!
   TBranch        *b_W_py;   //!
   TBranch        *b_W_pz;   //!
   TBranch        *b_W_e;   //!
   TBranch        *b_W_pt;   //!
   TBranch        *b_W_et;   //!
   TBranch        *b_W_eta;   //!
   TBranch        *b_W_phi;   //!
   TBranch        *b_W_vx;   //!
   TBranch        *b_W_vy;   //!
   TBranch        *b_W_vz;   //!
   TBranch        *b_W_y;   //!
   TBranch        *b_W_numTightElectrons;   //!
   TBranch        *b_W_numLooseElectrons;   //!
   TBranch        *b_W_pzNu1;   //!
   TBranch        *b_W_pzNu2;   //!
   TBranch        *b_W_electron_px;   //!
   TBranch        *b_W_electron_py;   //!
   TBranch        *b_W_electron_pz;   //!
   TBranch        *b_W_electron_e;   //!
   TBranch        *b_W_electron_pt;   //!
   TBranch        *b_W_electron_et;   //!
   TBranch        *b_W_electron_eta;   //!
   TBranch        *b_W_electron_theta;   //!
   TBranch        *b_W_electron_phi;   //!
   TBranch        *b_W_electron_charge;   //!
   TBranch        *b_W_electron_vx;   //!
   TBranch        *b_W_electron_vy;   //!
   TBranch        *b_W_electron_vz;   //!
   TBranch        *b_W_electron_y;   //!
   TBranch        *b_W_electron_trackiso;   //!
   TBranch        *b_W_electron_hcaliso;   //!
   TBranch        *b_W_electron_ecaliso;   //!
   TBranch        *b_W_electron_classification;   //!
   TBranch        *b_W_electron_sc_x;   //!
   TBranch        *b_W_electron_sc_y;   //!
   TBranch        *b_W_electron_sc_z;   //!
   TBranch        *b_W_electron_sc_Theta;   //!
   TBranch        *b_W_electron_sc_Eta;   //!
   TBranch        *b_W_electron_sc_Phi;   //!
   TBranch        *b_W_electron_sc_E;   //!
   TBranch        *b_W_electron_sc_px;   //!
   TBranch        *b_W_electron_sc_py;   //!
   TBranch        *b_W_electron_sc_pz;   //!
   TBranch        *b_W_electron_sc_Pt;   //!
   TBranch        *b_W_electron_sc_Et;   //!
   TBranch        *b_W_electron_eoverp_out;   //!
   TBranch        *b_W_electron_eoverp_in;   //!
   TBranch        *b_W_electron_numbrem;   //!
   TBranch        *b_W_electron_fbrem;   //!
   TBranch        *b_W_electron_deltaeta_in;   //!
   TBranch        *b_W_electron_deltaphi_in;   //!
   TBranch        *b_W_electron_deltaphi_out;   //!
   TBranch        *b_W_electron_deltaeta_out;   //!
   TBranch        *b_W_electron_trackmom_calo;   //!
   TBranch        *b_W_electron_trackmom_vtx;   //!
   TBranch        *b_W_electron_hovere;   //!
   TBranch        *b_W_electron_e9e25;   //!
   TBranch        *b_W_electron_sigmaetaeta;   //!
   TBranch        *b_W_electron_sigmaietaieta;   //!
   TBranch        *b_W_electron_missingHits;   //!
   TBranch        *b_W_electron_dist;   //!
   TBranch        *b_W_electron_dcot;   //!
   TBranch        *b_W_electron_convradius;   //!
   TBranch        *b_W_electron_isWP95;   //!
   TBranch        *b_W_electron_isWP80;   //!
   TBranch        *b_W_electron_isWP70;   //!
   TBranch        *b_W_electron_d0bsp;   //!
   TBranch        *b_W_electron_dz000;   //!
   TBranch        *b_event_runNo;   //!
   TBranch        *b_event_evtNo;   //!
   TBranch        *b_event_lumi;   //!
   TBranch        *b_event_bunch;   //!
   TBranch        *b_event_nPV;   //!
   TBranch        *b_event_PVx;   //!
   TBranch        *b_event_PVy;   //!
   TBranch        *b_event_PVz;   //!
   TBranch        *b_event_met_calomet;   //!
   TBranch        *b_event_met_calosumet;   //!
   TBranch        *b_event_met_calometsignificance;   //!
   TBranch        *b_event_met_calometPhi;   //!
   TBranch        *b_event_met_tcmet;   //!
   TBranch        *b_event_met_tcsumet;   //!
   TBranch        *b_event_met_tcmetsignificance;   //!
   TBranch        *b_event_met_tcmetPhi;   //!
   TBranch        *b_event_met_pfmet;   //!
   TBranch        *b_event_met_pfsumet;   //!
   TBranch        *b_event_met_pfmetsignificance;   //!
   TBranch        *b_event_met_pfmetPhi;   //!
   TBranch        *b_event_fastJetRho;   //!
   TBranch        *b_event_RhoForLeptonIsolation;   //!
   TBranch        *b_event_BeamSpot_x;   //!
   TBranch        *b_event_BeamSpot_y;   //!
   TBranch        *b_event_BeamSpot_z;   //!
   TBranch        *b_numW;   //!

   kanaelec(TTree *tree=0);
   virtual ~kanaelec();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   virtual void     myana(double myflag = -999);
   virtual void     Loop(int wda, int wsp, const char * outfilename);
   virtual bool     doKinematicFit(Int_t                 fflage,
				   const TLorentzVector     mup, 
				   const TLorentzVector     nvp,
				   const TLorentzVector     ajp, 
				   const TLorentzVector     bjp, 
				   TLorentzVector     & fit_mup, 
				   TLorentzVector     & fit_nvp,
				   TLorentzVector     & fit_ajp, 
				   TLorentzVector     & fit_bjp, 
				   Float_t            & fit_chi2,
				   Int_t              & fit_NDF, 
				   Int_t              & fit_status);
};

#endif

#ifdef kanaelec_cxx
kanaelec::kanaelec(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/uscms_data/d2/kalanand/WjjTrees/WenuJets_DataAll_GoldenJSON_917invpb.root");
      if (!f) {
         f = new TFile("/uscms_data/d2/kalanand/WjjTrees/WenuJets_DataAll_GoldenJSON_917invpb.root");
      }
      tree = (TTree*)gDirectory->Get("WJet");

   }
   Init(tree);
}

kanaelec::~kanaelec()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t kanaelec::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t kanaelec::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void kanaelec::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("numPFCorJets", &numPFCorJets, &b_numPFCorJets);
   fChain->SetBranchAddress("numPFCorJetBTags", &numPFCorJetBTags, &b_numPFCorJetBTags);
   fChain->SetBranchAddress("JetPFCor_Et", JetPFCor_Et, &b_JetPFCor_Et);
   fChain->SetBranchAddress("JetPFCor_Pt", JetPFCor_Pt, &b_JetPFCor_Pt);
   fChain->SetBranchAddress("JetPFCor_Eta", JetPFCor_Eta, &b_JetPFCor_Eta);
   fChain->SetBranchAddress("JetPFCor_Phi", JetPFCor_Phi, &b_JetPFCor_Phi);
   fChain->SetBranchAddress("JetPFCor_Theta", JetPFCor_Theta, &b_JetPFCor_Theta);
   fChain->SetBranchAddress("JetPFCor_Px", JetPFCor_Px, &b_JetPFCor_Px);
   fChain->SetBranchAddress("JetPFCor_Py", JetPFCor_Py, &b_JetPFCor_Py);
   fChain->SetBranchAddress("JetPFCor_Pz", JetPFCor_Pz, &b_JetPFCor_Pz);
   fChain->SetBranchAddress("JetPFCor_E", JetPFCor_E, &b_JetPFCor_E);
   fChain->SetBranchAddress("JetPFCor_Y", JetPFCor_Y, &b_JetPFCor_Y);
   fChain->SetBranchAddress("JetPFCor_Mass", JetPFCor_Mass, &b_JetPFCor_Mass);
   fChain->SetBranchAddress("JetPFCor_etaetaMoment", JetPFCor_etaetaMoment, &b_JetPFCor_etaetaMoment);
   fChain->SetBranchAddress("JetPFCor_phiphiMoment", JetPFCor_phiphiMoment, &b_JetPFCor_phiphiMoment);
   fChain->SetBranchAddress("JetPFCor_etaphiMoment", JetPFCor_etaphiMoment, &b_JetPFCor_etaphiMoment);
   fChain->SetBranchAddress("JetPFCor_maxDistance", JetPFCor_maxDistance, &b_JetPFCor_maxDistance);
   fChain->SetBranchAddress("JetPFCor_nConstituents", JetPFCor_nConstituents, &b_JetPFCor_nConstituents);
   fChain->SetBranchAddress("JetPFCor_Area", JetPFCor_Area, &b_JetPFCor_Area);
   fChain->SetBranchAddress("VplusPFCorJet_Mass", VplusPFCorJet_Mass, &b_VplusPFCorJet_Mass);
   fChain->SetBranchAddress("JetPFCor_dphiBoson", JetPFCor_dphiBoson, &b_JetPFCor_dphiBoson);
   fChain->SetBranchAddress("JetPFCor_detaBoson", JetPFCor_detaBoson, &b_JetPFCor_detaBoson);
   fChain->SetBranchAddress("JetPFCor_dRBoson", JetPFCor_dRBoson, &b_JetPFCor_dRBoson);
   fChain->SetBranchAddress("JetPFCor_dphiMET", JetPFCor_dphiMET, &b_JetPFCor_dphiMET);
   fChain->SetBranchAddress("JetPFCor_Response", JetPFCor_Response, &b_JetPFCor_Response);
   fChain->SetBranchAddress("JetPFCor_bDiscriminator", JetPFCor_bDiscriminator, &b_JetPFCor_bDiscriminator);
   fChain->SetBranchAddress("JetPFCor_secVertexMass", JetPFCor_secVertexMass, &b_JetPFCor_secVertexMass);
   fChain->SetBranchAddress("JetPFCor_dphiBoson2", JetPFCor_dphiBoson2, &b_JetPFCor_dphiBoson2);
   fChain->SetBranchAddress("JetPFCor_detaBoson2", JetPFCor_detaBoson2, &b_JetPFCor_detaBoson2);
   fChain->SetBranchAddress("JetPFCor_dRBoson2", JetPFCor_dRBoson2, &b_JetPFCor_dRBoson2);
   fChain->SetBranchAddress("VplusPFCorJet_Mass2", VplusPFCorJet_Mass2, &b_VplusPFCorJet_Mass2);
   fChain->SetBranchAddress("JetPFCor_Response2", JetPFCor_Response2, &b_JetPFCor_Response2);
   fChain->SetBranchAddress("JetPFCor_ChargedHadronEnergy", JetPFCor_ChargedHadronEnergy, &b_JetPFCor_ChargedHadronEnergy);
   fChain->SetBranchAddress("JetPFCor_ChargedHadronEnergyFrac", JetPFCor_ChargedHadronEnergyFrac, &b_JetPFCor_ChargedHadronEnergyFrac);
   fChain->SetBranchAddress("JetPFCor_NeutralHadronEnergy", JetPFCor_NeutralHadronEnergy, &b_JetPFCor_NeutralHadronEnergy);
   fChain->SetBranchAddress("JetPFCor_NeutralHadronEnergyFrac", JetPFCor_NeutralHadronEnergyFrac, &b_JetPFCor_NeutralHadronEnergyFrac);
   fChain->SetBranchAddress("JetPFCor_ChargedEmEnergy", JetPFCor_ChargedEmEnergy, &b_JetPFCor_ChargedEmEnergy);
   fChain->SetBranchAddress("JetPFCor_ChargedEmEnergyFrac", JetPFCor_ChargedEmEnergyFrac, &b_JetPFCor_ChargedEmEnergyFrac);
   fChain->SetBranchAddress("JetPFCor_ChargedMuEnergy", JetPFCor_ChargedMuEnergy, &b_JetPFCor_ChargedMuEnergy);
   fChain->SetBranchAddress("JetPFCor_ChargedMuEnergyFrac", JetPFCor_ChargedMuEnergyFrac, &b_JetPFCor_ChargedMuEnergyFrac);
   fChain->SetBranchAddress("JetPFCor_NeutralEmEnergy", JetPFCor_NeutralEmEnergy, &b_JetPFCor_NeutralEmEnergy);
   fChain->SetBranchAddress("JetPFCor_NeutralEmEnergyFrac", JetPFCor_NeutralEmEnergyFrac, &b_JetPFCor_NeutralEmEnergyFrac);
   fChain->SetBranchAddress("JetPFCor_ChargedMultiplicity", JetPFCor_ChargedMultiplicity, &b_JetPFCor_ChargedMultiplicity);
   fChain->SetBranchAddress("JetPFCor_NeutralMultiplicity", JetPFCor_NeutralMultiplicity, &b_JetPFCor_NeutralMultiplicity);
   fChain->SetBranchAddress("JetPFCor_MuonMultiplicity", JetPFCor_MuonMultiplicity, &b_JetPFCor_MuonMultiplicity);
   fChain->SetBranchAddress("JetPFCor_PhotonEnergy", JetPFCor_PhotonEnergy, &b_JetPFCor_PhotonEnergy);
   fChain->SetBranchAddress("JetPFCor_PhotonEnergyFraction", JetPFCor_PhotonEnergyFraction, &b_JetPFCor_PhotonEnergyFraction);
   fChain->SetBranchAddress("JetPFCor_ElectronEnergy", JetPFCor_ElectronEnergy, &b_JetPFCor_ElectronEnergy);
   fChain->SetBranchAddress("JetPFCor_ElectronEnergyFraction", JetPFCor_ElectronEnergyFraction, &b_JetPFCor_ElectronEnergyFraction);
   fChain->SetBranchAddress("JetPFCor_MuonEnergy", JetPFCor_MuonEnergy, &b_JetPFCor_MuonEnergy);
   fChain->SetBranchAddress("JetPFCor_MuonEnergyFraction", JetPFCor_MuonEnergyFraction, &b_JetPFCor_MuonEnergyFraction);
   fChain->SetBranchAddress("JetPFCor_HFHadronEnergy", JetPFCor_HFHadronEnergy, &b_JetPFCor_HFHadronEnergy);
   fChain->SetBranchAddress("JetPFCor_HFHadronEnergyFraction", JetPFCor_HFHadronEnergyFraction, &b_JetPFCor_HFHadronEnergyFraction);
   fChain->SetBranchAddress("JetPFCor_HFEMEnergy", JetPFCor_HFEMEnergy, &b_JetPFCor_HFEMEnergy);
   fChain->SetBranchAddress("JetPFCor_HFEMEnergyFraction", JetPFCor_HFEMEnergyFraction, &b_JetPFCor_HFEMEnergyFraction);
   fChain->SetBranchAddress("JetPFCor_ChargedHadronMultiplicity", JetPFCor_ChargedHadronMultiplicity, &b_JetPFCor_ChargedHadronMultiplicity);
   fChain->SetBranchAddress("JetPFCor_NeutralHadronMultiplicity", JetPFCor_NeutralHadronMultiplicity, &b_JetPFCor_NeutralHadronMultiplicity);
   fChain->SetBranchAddress("JetPFCor_PhotonMultiplicity", JetPFCor_PhotonMultiplicity, &b_JetPFCor_PhotonMultiplicity);
   fChain->SetBranchAddress("JetPFCor_ElectronMultiplicity", JetPFCor_ElectronMultiplicity, &b_JetPFCor_ElectronMultiplicity);
   fChain->SetBranchAddress("JetPFCor_HFHadronMultiplicity", JetPFCor_HFHadronMultiplicity, &b_JetPFCor_HFHadronMultiplicity);
   fChain->SetBranchAddress("JetPFCor_HFEMMultiplicity", JetPFCor_HFEMMultiplicity, &b_JetPFCor_HFEMMultiplicity);
   fChain->SetBranchAddress("MassV2j_PFCor", &MassV2j_PFCor, &b_MassV2j_PFCor);
   fChain->SetBranchAddress("MassV3j_PFCor", &MassV3j_PFCor, &b_MassV3j_PFCor);
   fChain->SetBranchAddress("MassV4j_PFCor", &MassV4j_PFCor, &b_MassV4j_PFCor);
   fChain->SetBranchAddress("MassV5j_PFCor", &MassV5j_PFCor, &b_MassV5j_PFCor);
   fChain->SetBranchAddress("MassV6j_PFCor", &MassV6j_PFCor, &b_MassV6j_PFCor);
   fChain->SetBranchAddress("Mass2j_PFCor", &Mass2j_PFCor, &b_Mass2j_PFCor);
   fChain->SetBranchAddress("Mass3j_PFCor", &Mass3j_PFCor, &b_Mass3j_PFCor);
   fChain->SetBranchAddress("Mass4j_PFCor", &Mass4j_PFCor, &b_Mass4j_PFCor);
   fChain->SetBranchAddress("Mass5j_PFCor", &Mass5j_PFCor, &b_Mass5j_PFCor);
   fChain->SetBranchAddress("Mass6j_PFCor", &Mass6j_PFCor, &b_Mass6j_PFCor);
   fChain->SetBranchAddress("cosJacksonAngleV2j_PFCor", &cosJacksonAngleV2j_PFCor, &b_cosJacksonAngleV2j_PFCor);
   fChain->SetBranchAddress("cosJacksonAngle2j_PFCor", &cosJacksonAngle2j_PFCor, &b_cosJacksonAngle2j_PFCor);
   fChain->SetBranchAddress("cosJacksonAngleV3j_PFCor", &cosJacksonAngleV3j_PFCor, &b_cosJacksonAngleV3j_PFCor);
   fChain->SetBranchAddress("cosJacksonAngle3j12_PFCor", &cosJacksonAngle3j12_PFCor, &b_cosJacksonAngle3j12_PFCor);
   fChain->SetBranchAddress("cosJacksonAngle3j23_PFCor", &cosJacksonAngle3j23_PFCor, &b_cosJacksonAngle3j23_PFCor);
   fChain->SetBranchAddress("cosJacksonAngle3j31_PFCor", &cosJacksonAngle3j31_PFCor, &b_cosJacksonAngle3j31_PFCor);
   fChain->SetBranchAddress("cosphiDecayPlane_PFCor", &cosphiDecayPlane_PFCor, &b_cosphiDecayPlane_PFCor);
   fChain->SetBranchAddress("cosThetaLnu_PFCor", &cosThetaLnu_PFCor, &b_cosThetaLnu_PFCor);
   fChain->SetBranchAddress("cosThetaJJ_PFCor", &cosThetaJJ_PFCor, &b_cosThetaJJ_PFCor);
   fChain->SetBranchAddress("WJetsPullPFCor", &WJetsPullPFCor, &b_WJetsPullPFCor);
   fChain->SetBranchAddress("cosThetaJ1HiggsCM_PFCor", &cosThetaJ1HiggsCM_PFCor, &b_cosThetaJ1HiggsCM_PFCor);
   fChain->SetBranchAddress("cosThetaJ2HiggsCM_PFCor", &cosThetaJ2HiggsCM_PFCor, &b_cosThetaJ2HiggsCM_PFCor);
   fChain->SetBranchAddress("cosThetaL1HiggsCM_PFCor", &cosThetaL1HiggsCM_PFCor, &b_cosThetaL1HiggsCM_PFCor);
   fChain->SetBranchAddress("cosThetaL2HiggsCM_PFCor", &cosThetaL2HiggsCM_PFCor, &b_cosThetaL2HiggsCM_PFCor);
   fChain->SetBranchAddress("cosThetaV1HiggsCM_PFCor", &cosThetaV1HiggsCM_PFCor, &b_cosThetaV1HiggsCM_PFCor);
   fChain->SetBranchAddress("cosThetaV2HiggsCM_PFCor", &cosThetaV2HiggsCM_PFCor, &b_cosThetaV2HiggsCM_PFCor);
   fChain->SetBranchAddress("W_mass", &W_mass, &b_W_mass);
   fChain->SetBranchAddress("W_mt", &W_mt, &b_W_mt);
   fChain->SetBranchAddress("W_px", &W_px, &b_W_px);
   fChain->SetBranchAddress("W_py", &W_py, &b_W_py);
   fChain->SetBranchAddress("W_pz", &W_pz, &b_W_pz);
   fChain->SetBranchAddress("W_e", &W_e, &b_W_e);
   fChain->SetBranchAddress("W_pt", &W_pt, &b_W_pt);
   fChain->SetBranchAddress("W_et", &W_et, &b_W_et);
   fChain->SetBranchAddress("W_eta", &W_eta, &b_W_eta);
   fChain->SetBranchAddress("W_phi", &W_phi, &b_W_phi);
   fChain->SetBranchAddress("W_vx", &W_vx, &b_W_vx);
   fChain->SetBranchAddress("W_vy", &W_vy, &b_W_vy);
   fChain->SetBranchAddress("W_vz", &W_vz, &b_W_vz);
   fChain->SetBranchAddress("W_y", &W_y, &b_W_y);
   fChain->SetBranchAddress("W_numTightElectrons", &W_numTightElectrons, &b_W_numTightElectrons);
   fChain->SetBranchAddress("W_numLooseElectrons", &W_numLooseElectrons, &b_W_numLooseElectrons);
   fChain->SetBranchAddress("W_pzNu1", &W_pzNu1, &b_W_pzNu1);
   fChain->SetBranchAddress("W_pzNu2", &W_pzNu2, &b_W_pzNu2);
   fChain->SetBranchAddress("W_electron_px", &W_electron_px, &b_W_electron_px);
   fChain->SetBranchAddress("W_electron_py", &W_electron_py, &b_W_electron_py);
   fChain->SetBranchAddress("W_electron_pz", &W_electron_pz, &b_W_electron_pz);
   fChain->SetBranchAddress("W_electron_e", &W_electron_e, &b_W_electron_e);
   fChain->SetBranchAddress("W_electron_pt", &W_electron_pt, &b_W_electron_pt);
   fChain->SetBranchAddress("W_electron_et", &W_electron_et, &b_W_electron_et);
   fChain->SetBranchAddress("W_electron_eta", &W_electron_eta, &b_W_electron_eta);
   fChain->SetBranchAddress("W_electron_theta", &W_electron_theta, &b_W_electron_theta);
   fChain->SetBranchAddress("W_electron_phi", &W_electron_phi, &b_W_electron_phi);
   fChain->SetBranchAddress("W_electron_charge", &W_electron_charge, &b_W_electron_charge);
   fChain->SetBranchAddress("W_electron_vx", &W_electron_vx, &b_W_electron_vx);
   fChain->SetBranchAddress("W_electron_vy", &W_electron_vy, &b_W_electron_vy);
   fChain->SetBranchAddress("W_electron_vz", &W_electron_vz, &b_W_electron_vz);
   fChain->SetBranchAddress("W_electron_y", &W_electron_y, &b_W_electron_y);
   fChain->SetBranchAddress("W_electron_trackiso", &W_electron_trackiso, &b_W_electron_trackiso);
   fChain->SetBranchAddress("W_electron_hcaliso", &W_electron_hcaliso, &b_W_electron_hcaliso);
   fChain->SetBranchAddress("W_electron_ecaliso", &W_electron_ecaliso, &b_W_electron_ecaliso);
   fChain->SetBranchAddress("W_electron_classification", &W_electron_classification, &b_W_electron_classification);
   fChain->SetBranchAddress("W_electron_sc_x", &W_electron_sc_x, &b_W_electron_sc_x);
   fChain->SetBranchAddress("W_electron_sc_y", &W_electron_sc_y, &b_W_electron_sc_y);
   fChain->SetBranchAddress("W_electron_sc_z", &W_electron_sc_z, &b_W_electron_sc_z);
   fChain->SetBranchAddress("W_electron_sc_Theta", &W_electron_sc_Theta, &b_W_electron_sc_Theta);
   fChain->SetBranchAddress("W_electron_sc_Eta", &W_electron_sc_Eta, &b_W_electron_sc_Eta);
   fChain->SetBranchAddress("W_electron_sc_Phi", &W_electron_sc_Phi, &b_W_electron_sc_Phi);
   fChain->SetBranchAddress("W_electron_sc_E", &W_electron_sc_E, &b_W_electron_sc_E);
   fChain->SetBranchAddress("W_electron_sc_px", &W_electron_sc_px, &b_W_electron_sc_px);
   fChain->SetBranchAddress("W_electron_sc_py", &W_electron_sc_py, &b_W_electron_sc_py);
   fChain->SetBranchAddress("W_electron_sc_pz", &W_electron_sc_pz, &b_W_electron_sc_pz);
   fChain->SetBranchAddress("W_electron_sc_Pt", &W_electron_sc_Pt, &b_W_electron_sc_Pt);
   fChain->SetBranchAddress("W_electron_sc_Et", &W_electron_sc_Et, &b_W_electron_sc_Et);
   fChain->SetBranchAddress("W_electron_eoverp_out", &W_electron_eoverp_out, &b_W_electron_eoverp_out);
   fChain->SetBranchAddress("W_electron_eoverp_in", &W_electron_eoverp_in, &b_W_electron_eoverp_in);
   fChain->SetBranchAddress("W_electron_numbrem", &W_electron_numbrem, &b_W_electron_numbrem);
   fChain->SetBranchAddress("W_electron_fbrem", &W_electron_fbrem, &b_W_electron_fbrem);
   fChain->SetBranchAddress("W_electron_deltaeta_in", &W_electron_deltaeta_in, &b_W_electron_deltaeta_in);
   fChain->SetBranchAddress("W_electron_deltaphi_in", &W_electron_deltaphi_in, &b_W_electron_deltaphi_in);
   fChain->SetBranchAddress("W_electron_deltaphi_out", &W_electron_deltaphi_out, &b_W_electron_deltaphi_out);
   fChain->SetBranchAddress("W_electron_deltaeta_out", &W_electron_deltaeta_out, &b_W_electron_deltaeta_out);
   fChain->SetBranchAddress("W_electron_trackmom_calo", &W_electron_trackmom_calo, &b_W_electron_trackmom_calo);
   fChain->SetBranchAddress("W_electron_trackmom_vtx", &W_electron_trackmom_vtx, &b_W_electron_trackmom_vtx);
   fChain->SetBranchAddress("W_electron_hovere", &W_electron_hovere, &b_W_electron_hovere);
   fChain->SetBranchAddress("W_electron_e9e25", &W_electron_e9e25, &b_W_electron_e9e25);
   fChain->SetBranchAddress("W_electron_sigmaetaeta", &W_electron_sigmaetaeta, &b_W_electron_sigmaetaeta);
   fChain->SetBranchAddress("W_electron_sigmaietaieta", &W_electron_sigmaietaieta, &b_W_electron_sigmaietaieta);
   fChain->SetBranchAddress("W_electron_missingHits", &W_electron_missingHits, &b_W_electron_missingHits);
   fChain->SetBranchAddress("W_electron_dist", &W_electron_dist, &b_W_electron_dist);
   fChain->SetBranchAddress("W_electron_dcot", &W_electron_dcot, &b_W_electron_dcot);
   fChain->SetBranchAddress("W_electron_convradius", &W_electron_convradius, &b_W_electron_convradius);
   fChain->SetBranchAddress("W_electron_isWP95", &W_electron_isWP95, &b_W_electron_isWP95);
   fChain->SetBranchAddress("W_electron_isWP80", &W_electron_isWP80, &b_W_electron_isWP80);
   fChain->SetBranchAddress("W_electron_isWP70", &W_electron_isWP70, &b_W_electron_isWP70);
   fChain->SetBranchAddress("W_electron_d0bsp", &W_electron_d0bsp, &b_W_electron_d0bsp);
   fChain->SetBranchAddress("W_electron_dz000", &W_electron_dz000, &b_W_electron_dz000);
   fChain->SetBranchAddress("event_runNo", &event_runNo, &b_event_runNo);
   fChain->SetBranchAddress("event_evtNo", &event_evtNo, &b_event_evtNo);
   fChain->SetBranchAddress("event_lumi", &event_lumi, &b_event_lumi);
   fChain->SetBranchAddress("event_bunch", &event_bunch, &b_event_bunch);
   fChain->SetBranchAddress("event_nPV", &event_nPV, &b_event_nPV);
   fChain->SetBranchAddress("event_PVx", event_PVx, &b_event_PVx);
   fChain->SetBranchAddress("event_PVy", event_PVy, &b_event_PVy);
   fChain->SetBranchAddress("event_PVz", event_PVz, &b_event_PVz);
   fChain->SetBranchAddress("event_met_calomet", &event_met_calomet, &b_event_met_calomet);
   fChain->SetBranchAddress("event_met_calosumet", &event_met_calosumet, &b_event_met_calosumet);
   fChain->SetBranchAddress("event_met_calometsignificance", &event_met_calometsignificance, &b_event_met_calometsignificance);
   fChain->SetBranchAddress("event_met_calometPhi", &event_met_calometPhi, &b_event_met_calometPhi);
   fChain->SetBranchAddress("event_met_tcmet", &event_met_tcmet, &b_event_met_tcmet);
   fChain->SetBranchAddress("event_met_tcsumet", &event_met_tcsumet, &b_event_met_tcsumet);
   fChain->SetBranchAddress("event_met_tcmetsignificance", &event_met_tcmetsignificance, &b_event_met_tcmetsignificance);
   fChain->SetBranchAddress("event_met_tcmetPhi", &event_met_tcmetPhi, &b_event_met_tcmetPhi);
   fChain->SetBranchAddress("event_met_pfmet", &event_met_pfmet, &b_event_met_pfmet);
   fChain->SetBranchAddress("event_met_pfsumet", &event_met_pfsumet, &b_event_met_pfsumet);
   fChain->SetBranchAddress("event_met_pfmetsignificance", &event_met_pfmetsignificance, &b_event_met_pfmetsignificance);
   fChain->SetBranchAddress("event_met_pfmetPhi", &event_met_pfmetPhi, &b_event_met_pfmetPhi);
   fChain->SetBranchAddress("event_fastJetRho", &event_fastJetRho, &b_event_fastJetRho);
   fChain->SetBranchAddress("event_RhoForLeptonIsolation", &event_RhoForLeptonIsolation, &b_event_RhoForLeptonIsolation);
   fChain->SetBranchAddress("event_BeamSpot_x", &event_BeamSpot_x, &b_event_BeamSpot_x);
   fChain->SetBranchAddress("event_BeamSpot_y", &event_BeamSpot_y, &b_event_BeamSpot_y);
   fChain->SetBranchAddress("event_BeamSpot_z", &event_BeamSpot_z, &b_event_BeamSpot_z);
   fChain->SetBranchAddress("numW", &numW, &b_numW);
   Notify();
}

Bool_t kanaelec::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void kanaelec::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t kanaelec::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return entry=1;
}
#endif // #ifdef kanaelec_cxx
