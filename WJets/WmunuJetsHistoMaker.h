//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu May 12 21:30:22 2011 by ROOT version 5.27/06
// from TTree WJet/V+jets Tree
// found on file: Data_WenuJets_2011_178invpb_May6JSON.root
//////////////////////////////////////////////////////////

#ifndef WmunuJetsHistoMaker_h
#define WmunuJetsHistoMaker_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class WmunuJetsHistoMaker {
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
   Float_t         W_muon_px;
   Float_t         W_muon_py;
   Float_t         W_muon_pz;
   Float_t         W_muon_e;
   Float_t         W_muon_pt;
   Float_t         W_muon_et;
   Float_t         W_muon_eta;
   Float_t         W_muon_theta;
   Float_t         W_muon_phi;
   Int_t           W_muon_charge;
   Float_t         W_muon_vx;
   Float_t         W_muon_vy;
   Float_t         W_muon_vz;
   Float_t         W_muon_y;
   Float_t         W_muon_trackiso;
   Float_t         W_muon_hcaliso;
   Float_t         W_muon_ecaliso;
   Int_t           W_muon_type;
   Int_t           W_muon_numberOfChambers;
   Int_t           W_muon_numberOfMatches;
   Float_t         W_muon_d0bsp;
   Float_t         W_muon_dz000;
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
   TBranch        *b_W_muon_px;   //!
   TBranch        *b_W_muon_py;   //!
   TBranch        *b_W_muon_pz;   //!
   TBranch        *b_W_muon_e;   //!
   TBranch        *b_W_muon_pt;   //!
   TBranch        *b_W_muon_et;   //!
   TBranch        *b_W_muon_eta;   //!
   TBranch        *b_W_muon_theta;   //!
   TBranch        *b_W_muon_phi;   //!
   TBranch        *b_W_muon_charge;   //!
   TBranch        *b_W_muon_vx;   //!
   TBranch        *b_W_muon_vy;   //!
   TBranch        *b_W_muon_vz;   //!
   TBranch        *b_W_muon_y;   //!
   TBranch        *b_W_muon_trackiso;   //!
   TBranch        *b_W_muon_hcaliso;   //!
   TBranch        *b_W_muon_ecaliso;   //!
   TBranch        *b_W_muon_type;   //!
   TBranch        *b_W_muon_numberOfChambers;   //!
   TBranch        *b_W_muon_numberOfMatches;   //!
   TBranch        *b_W_muon_d0bsp;   //!
   TBranch        *b_W_muon_dz000;   //!
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

   WmunuJetsHistoMaker(TTree *tree=0);
   virtual ~WmunuJetsHistoMaker();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(int wda, int wsp, const char * outfilename);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   virtual void     myana();
};

#endif

#ifdef WmunuJetsHistoMaker_cxx
WmunuJetsHistoMaker::WmunuJetsHistoMaker(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("WmunuJets_Data2011_178invpb_May6JSON.root");
      if (!f) {
         f = new TFile("WmunuJets_Data2011_178invpb_May6JSON.root");
      }
      tree = (TTree*)gDirectory->Get("WJet");

   }
   Init(tree);
}

WmunuJetsHistoMaker::~WmunuJetsHistoMaker()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t WmunuJetsHistoMaker::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t WmunuJetsHistoMaker::LoadTree(Long64_t entry)
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

void WmunuJetsHistoMaker::Init(TTree *tree)
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
   fChain->SetBranchAddress("W_pzNu1", &W_pzNu1, &b_W_pzNu1);
   fChain->SetBranchAddress("W_pzNu2", &W_pzNu2, &b_W_pzNu2);
   fChain->SetBranchAddress("W_muon_px", &W_muon_px, &b_W_muon_px);
   fChain->SetBranchAddress("W_muon_py", &W_muon_py, &b_W_muon_py);
   fChain->SetBranchAddress("W_muon_pz", &W_muon_pz, &b_W_muon_pz);
   fChain->SetBranchAddress("W_muon_e", &W_muon_e, &b_W_muon_e);
   fChain->SetBranchAddress("W_muon_pt", &W_muon_pt, &b_W_muon_pt);
   fChain->SetBranchAddress("W_muon_et", &W_muon_et, &b_W_muon_et);
   fChain->SetBranchAddress("W_muon_eta", &W_muon_eta, &b_W_muon_eta);
   fChain->SetBranchAddress("W_muon_theta", &W_muon_theta, &b_W_muon_theta);
   fChain->SetBranchAddress("W_muon_phi", &W_muon_phi, &b_W_muon_phi);
   fChain->SetBranchAddress("W_muon_charge", &W_muon_charge, &b_W_muon_charge);
   fChain->SetBranchAddress("W_muon_vx", &W_muon_vx, &b_W_muon_vx);
   fChain->SetBranchAddress("W_muon_vy", &W_muon_vy, &b_W_muon_vy);
   fChain->SetBranchAddress("W_muon_vz", &W_muon_vz, &b_W_muon_vz);
   fChain->SetBranchAddress("W_muon_y", &W_muon_y, &b_W_muon_y);
   fChain->SetBranchAddress("W_muon_trackiso", &W_muon_trackiso, &b_W_muon_trackiso);
   fChain->SetBranchAddress("W_muon_hcaliso", &W_muon_hcaliso, &b_W_muon_hcaliso);
   fChain->SetBranchAddress("W_muon_ecaliso", &W_muon_ecaliso, &b_W_muon_ecaliso);
   fChain->SetBranchAddress("W_muon_type", &W_muon_type, &b_W_muon_type);
   fChain->SetBranchAddress("W_muon_numberOfChambers", &W_muon_numberOfChambers, &b_W_muon_numberOfChambers);
   fChain->SetBranchAddress("W_muon_numberOfMatches", &W_muon_numberOfMatches, &b_W_muon_numberOfMatches);
   fChain->SetBranchAddress("W_muon_d0bsp", &W_muon_d0bsp, &b_W_muon_d0bsp);
   fChain->SetBranchAddress("W_muon_dz000", &W_muon_dz000, &b_W_muon_dz000);
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

Bool_t WmunuJetsHistoMaker::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void WmunuJetsHistoMaker::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t WmunuJetsHistoMaker::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef WmunuJetsHistoMaker_cxx
