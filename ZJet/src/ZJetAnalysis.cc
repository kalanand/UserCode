  // particle behaviour


   /** Jet flavor classification.
        if ( Flavour == 1 ) : Down 
	if ( Flavour == 2 ) : Up 
	if ( Flavour == 3 ) : Strange 
	if ( Flavour == 4 ) : Charm 
	if ( Flavour == 5 ) : Bottom 
	if ( Flavour == 21 ): Gluon
   */


   /** The electron classification.
      barrel  :   0: golden,  10: bigbrem,  20: narrow, 30-34: showering, 
                (30: showering nbrem=0, 31: showering nbrem=1, 32: showering nbrem=2 ,33: showering nbrem=3, 34: showering nbrem>=4)
                 40: crack
      endcaps : 100: golden, 110: bigbrem, 120: narrow, 130-134: showering
               (130: showering nbrem=0, 131: showering nbrem=1, 132: showering nbrem=2 ,133: showering nbrem=3, 134: showering nbrem>=4)
   */

// user include files
#include "JetMETCorrections/ZJet/interface/ZJetAnalysis.h" 
#include "JetMETCorrections/ZJet/src/JetUtilMC.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/ESHandle.h" 
#include "FWCore/Framework/interface/EventSetup.h" 
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/DetId/interface/DetId.h" 
#include "DataFormats/JetReco/interface/CaloJetCollection.h" 
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/OverlapChecker.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Common/interface/AssociationVector.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"
#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"
#include <Math/GenVector/VectorUtil.h>
#include "TFile.h"
#include "TTree.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "RecoEgamma/EgammaTools/interface/ECALPositionCalculator.h"
/////////
#include "RecoEgamma/EgammaIsolationAlgos/interface/ElectronTkIsolation.h"


typedef edm::AssociationVector<reco::CandidateRefProd, 
			       std::vector<double> > IsolationCollection;

namespace cms
{
  ZJetAnalysis::ZJetAnalysis(const edm::ParameterSet& iConfig)
  {
    // get names of modules, producing object collections
    mInputCalo = iConfig.getParameter<std::vector<edm::InputTag> >("srcCalo");
    mInputGen  = iConfig.getParameter<std::vector<edm::InputTag> >("srcGen");
    mInputCor  = iConfig.getParameter<std::vector<edm::InputTag> >("srcCaloCor");

    trackTags_ = iConfig.getParameter<edm::InputTag>("tracks");
    zee_         =  iConfig.getParameter<edm::InputTag>("zee");
    myName       =  iConfig.getParameter<std::string> ("textout");


    StoreParticleInformation_ = 
      iConfig.getUntrackedParameter<bool>( "StoreParticleInformation", false);
    StoreTrackInformation_=iConfig.getUntrackedParameter<bool>("StoreTrackInformation", false);

    
    // Jet Flavor identification
    flavourMatchOptionf 
      = iConfig.getUntrackedParameter<std::string>("flavourMatchOption", "genParticle");
    const edm::InputTag flavourSource("tagJet");
    if (flavourMatchOptionf == "fastMC") {
      flavourSourcef = iConfig.getUntrackedParameter<edm::InputTag>("flavourSource", flavourSource);
    } else if (flavourMatchOptionf == "hepMC") {
      jetIdParameters_ = 
	iConfig.getParameter<edm::ParameterSet>("jetIdParameters");
      jfi = JetFlavourIdentifier(jetIdParameters_);
    } else if (flavourMatchOptionf == "genParticle") {
      flavourSourcef = iConfig.getUntrackedParameter<edm::InputTag> ("flavourSource", flavourSource);
    }


    // No missing input allowed !!!!
    allowMissingInputs_ 
      = iConfig.getUntrackedParameter<bool>("AllowMissingInputs",false);
    // get name of output file with histogramms
    fOutputFileName = iConfig.getUntrackedParameter<string>("HistOutFile","analysis_zjet.root"); 
  }






  ZJetAnalysis::~ZJetAnalysis()
  {   
  }




  void ZJetAnalysis::beginJob( const edm::EventSetup& iSetup)
  {
    hOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ; 
    myTree = new TTree("ZJet","ZJet Tree");

    ///////////////////////  //////////////////////////////////////////////   

    myTree->Branch("run",  &run, "run/I");
    myTree->Branch("event",  &event, "event/I");
    myTree->Branch("eventsize", &eventsize, "eventsize/I"); 
    myTree->Branch("genEventScale", &genEventScale , "genEventScale/F");
    myTree->Branch("genEventWeight", &genEventWeight , "genEventWeight/F");

    NumRecoJets = 0;
    NumGenJets = 0;
    NumRecoTrack = 0;
    NumPart = 0;

    // Z-->ee block
    myTree->Branch("nZee",        &nZee,        "nZee/I");   
    myTree->Branch("mZee",        &mZee,        "mZee/F");  
    myTree->Branch("Z_px",        &Z_px,        "Z_px/F");
    myTree->Branch("Z_py",        &Z_py,        "Z_py/F");
    myTree->Branch("Z_pz",        &Z_pz,        "Z_pz/F");
    myTree->Branch("Z_E",         &Z_E,         "Z_E/F");
    myTree->Branch("Z_Pt",        &Z_Pt,        "Z_Pt/F");
    myTree->Branch("Z_Et",        &Z_Et,        "Z_Et/F");
    myTree->Branch("Z_Eta",       &Z_Eta,       "Z_Eta/F");    
    myTree->Branch("Z_Phi",       &Z_Phi,       "Z_Phi/F");
    myTree->Branch("Z_Charge",    &Z_Charge,    "Z_Charge/F");
    myTree->Branch("Z_Vx",        &Z_Vx,        "Z_Vx/F");
    myTree->Branch("Z_Vy",        &Z_Vy,        "Z_Vy/F");
    myTree->Branch("Z_Vz",        &Z_Vz,        "Z_Vz/F");

    myTree->Branch("ePluspx",      &ePluspx,      "ePluspx/F");
    myTree->Branch("ePluspy",      &ePluspy,      "ePluspy/F");
    myTree->Branch("ePluspz",      &ePluspz,      "ePluspz/F");
    myTree->Branch("ePlusE",       &ePlusE,       "ePlusE/F");
    myTree->Branch("ePlusPt",      &ePlusPt,      "ePlusPt/F");
    myTree->Branch("ePlusEt",      &ePlusEt,      "ePlusEt/F");
    myTree->Branch("ePlusEta",     &ePlusEta,     "ePlusEta/F");    
    myTree->Branch("ePlusPhi",     &ePlusPhi,     "ePlusPhi/F");
    myTree->Branch("ePlusCharge",  &ePlusCharge,  "ePlusCharge/F");
    myTree->Branch("ePlusVx",      &ePlusVx,      "ePlusVx/F");
    myTree->Branch("ePlusVy",      &ePlusVy,      "ePlusVy/F");
    myTree->Branch("ePlusVz",      &ePlusVz,      "ePlusVz/F");
    myTree->Branch("ePlusIsolation", &ePlusIsolation, "ePlusIsolation/F");


    //////////////
    myTree->Branch("ePlus_ptiso_15",      &ePlus_ptiso_15, "ePlus_ptiso_15/F");
    myTree->Branch("ePlus_ptiso_20",      &ePlus_ptiso_20, "ePlus_ptiso_20/F"); 
    myTree->Branch("ePlus_ptiso_25",      &ePlus_ptiso_25, "ePlus_ptiso_25/F");
    myTree->Branch("ePlus_ptiso_30",      &ePlus_ptiso_30, "ePlus_ptiso_30/F");
    myTree->Branch("ePlus_ptiso_35",      &ePlus_ptiso_35, "ePlus_ptiso_35/F");
    myTree->Branch("ePlus_ptiso_40",      &ePlus_ptiso_40, "ePlus_ptiso_40/F");
    myTree->Branch("ePlus_ptiso_45",      &ePlus_ptiso_45, "ePlus_ptiso_45/F");
    myTree->Branch("ePlus_ptiso_50",      &ePlus_ptiso_50, "ePlus_ptiso_50/F");

    myTree->Branch("ePlus_ptisoatecal_15", &ePlus_ptisoatecal_15, "ePlus_ptisoatecal_15/F");
    myTree->Branch("ePlus_ptisoatecal_20", &ePlus_ptisoatecal_20, "ePlus_ptisoatecal_20/F");
    myTree->Branch("ePlus_ptisoatecal_25", &ePlus_ptisoatecal_25, "ePlus_ptisoatecal_25/F");
    myTree->Branch("ePlus_ptisoatecal_30", &ePlus_ptisoatecal_30, "ePlus_ptisoatecal_30/F");
    myTree->Branch("ePlus_ptisoatecal_35", &ePlus_ptisoatecal_35, "ePlus_ptisoatecal_35/F");
    myTree->Branch("ePlus_ptisoatecal_40", &ePlus_ptisoatecal_40, "ePlus_ptisoatecal_40/F");
    myTree->Branch("ePlus_ptisoatecal_45", &ePlus_ptisoatecal_45, "ePlus_ptisoatecal_45/F");
    myTree->Branch("ePlus_ptisoatecal_50", &ePlus_ptisoatecal_50, "ePlus_ptisoatecal_50/F");

    myTree->Branch("ePlus_ntrkiso_15",  &ePlus_ntrkiso_15, "ePlus_ntrkiso_15/I");
    myTree->Branch("ePlus_ntrkiso_20",  &ePlus_ntrkiso_20, "ePlus_ntrkiso_20/I");
    myTree->Branch("ePlus_ntrkiso_25",  &ePlus_ntrkiso_25, "ePlus_ntrkiso_25/I");
    myTree->Branch("ePlus_ntrkiso_30",  &ePlus_ntrkiso_30, "ePlus_ntrkiso_30/I");
    myTree->Branch("ePlus_ntrkiso_35",  &ePlus_ntrkiso_35, "ePlus_ntrkiso_35/I");
    myTree->Branch("ePlus_ntrkiso_40",  &ePlus_ntrkiso_40, "ePlus_ntrkiso_40/I");
    myTree->Branch("ePlus_ntrkiso_45",  &ePlus_ntrkiso_45, "ePlus_ntrkiso_45/I");
    myTree->Branch("ePlus_ntrkiso_50",  &ePlus_ntrkiso_50, "ePlus_ntrkiso_50/I");
     
    myTree->Branch("ePlus_ntrkisoatecal_15",  &ePlus_ntrkisoatecal_15, "ePlus_ntrkisoatecal_15/I");
    myTree->Branch("ePlus_ntrkisoatecal_20",  &ePlus_ntrkisoatecal_20, "ePlus_ntrkisoatecal_20/I");
    myTree->Branch("ePlus_ntrkisoatecal_25",  &ePlus_ntrkisoatecal_25, "ePlus_ntrkisoatecal_25/I");
    myTree->Branch("ePlus_ntrkisoatecal_30",  &ePlus_ntrkisoatecal_30, "ePlus_ntrkisoatecal_30/I");
    myTree->Branch("ePlus_ntrkisoatecal_35",  &ePlus_ntrkisoatecal_35, "ePlus_ntrkisoatecal_35/I");
    myTree->Branch("ePlus_ntrkisoatecal_40",  &ePlus_ntrkisoatecal_40, "ePlus_ntrkisoatecal_40/I");
    myTree->Branch("ePlus_ntrkisoatecal_45",  &ePlus_ntrkisoatecal_45, "ePlus_ntrkisoatecal_45/I");
    myTree->Branch("ePlus_ntrkisoatecal_50",  &ePlus_ntrkisoatecal_50, "ePlus_ntrkisoatecal_50/I");

    myTree->Branch("ePlus_hcaliso_10",  &ePlus_hcaliso_10, "ePlus_hcaliso_10/F");
    myTree->Branch("ePlus_hcaliso_15",  &ePlus_hcaliso_15, "ePlus_hcaliso_15/F");
    myTree->Branch("ePlus_hcaliso_20",  &ePlus_hcaliso_20, "ePlus_hcaliso_20/F");
    myTree->Branch("ePlus_hcaliso_25",  &ePlus_hcaliso_25, "ePlus_hcaliso_25/F");
    myTree->Branch("ePlus_hcaliso_30",  &ePlus_hcaliso_30, "ePlus_hcaliso_30/F");
    myTree->Branch("ePlus_hcaliso_35",  &ePlus_hcaliso_35, "ePlus_hcaliso_35/F");
    myTree->Branch("ePlus_hcaliso_40",  &ePlus_hcaliso_40, "ePlus_hcaliso_40/F");
    myTree->Branch("ePlus_hcaliso_45",  &ePlus_hcaliso_45, "ePlus_hcaliso_45/F");
    myTree->Branch("ePlus_hcaliso_50",  &ePlus_hcaliso_50, "ePlus_hcaliso_50/F");

    myTree->Branch("ePlus_ecaliso_10",  &ePlus_ecaliso_10, "ePlus_ecaliso_10/F");
    myTree->Branch("ePlus_ecaliso_15",  &ePlus_ecaliso_15, "ePlus_ecaliso_15/F");
    myTree->Branch("ePlus_ecaliso_20",  &ePlus_ecaliso_20, "ePlus_ecaliso_20/F");
    myTree->Branch("ePlus_ecaliso_25",  &ePlus_ecaliso_25, "ePlus_ecaliso_25/F");
    myTree->Branch("ePlus_ecaliso_30",  &ePlus_ecaliso_30, "ePlus_ecaliso_30/F");
    myTree->Branch("ePlus_ecaliso_35",  &ePlus_ecaliso_35, "ePlus_ecaliso_35/F");
    myTree->Branch("ePlus_ecaliso_40",  &ePlus_ecaliso_40, "ePlus_ecaliso_40/F");
    myTree->Branch("ePlus_ecaliso_45",  &ePlus_ecaliso_45, "ePlus_ecaliso_45/F");
    myTree->Branch("ePlus_ecaliso_50",  &ePlus_ecaliso_50, "ePlus_ecaliso_50/F");
    ///////////////////





    myTree->Branch("ePlusClassification", &ePlusClassification, 
		   "ePlusClassification/I");
    myTree->Branch("isePlusLoosePTDR",     &isePlusLoosePTDR,     
		   "isePlusLoosePTDR/O");
    myTree->Branch("isePlusMediumPTDR",    &isePlusMediumPTDR,    
		   "isePlusMediumPTDR/O");
    myTree->Branch("isePlusTightPTDR",     &isePlusTightPTDR,     
		   "isePlusTightPTDR/O");
    myTree->Branch("isePlusLooseCutBased", &isePlusLooseCutBased, 
		   "isePlusLooseCutBased/O");
    myTree->Branch("isePlusRobustCutBased",&isePlusRobustCutBased,
		   "isePlusRobustCutBased/O");
    myTree->Branch("isePlusTightCutBased", &isePlusTightCutBased, 
		   "isePlusTightCutBased/O");

    myTree->Branch("ePlus_sc_x", &ePlus_sc_x, "ePlus_sc_x/F");
    myTree->Branch("ePlus_sc_y", &ePlus_sc_y, "ePlus_sc_y/F");
    myTree->Branch("ePlus_sc_z", &ePlus_sc_z, "ePlus_sc_z/F");
    myTree->Branch("ePlus_sc_R", &ePlus_sc_R, "ePlus_sc_R/F");
    myTree->Branch("ePlus_sc_Rt", &ePlus_sc_Rt, "ePlus_sc_Rt/F");
    myTree->Branch("ePlus_sc_eta", &ePlus_sc_eta, "ePlus_sc_eta/F");
    myTree->Branch("ePlus_sc_phi", &ePlus_sc_phi, "ePlus_sc_phi/F");
    myTree->Branch("ePlus_sc_theta", &ePlus_sc_theta, "ePlus_sc_theta/F");
    myTree->Branch("ePlus_sc_et", &ePlus_sc_et, "ePlus_sc_et/F");
    myTree->Branch("ePlus_sc_e", &ePlus_sc_e, "ePlus_sc_e/F");
    myTree->Branch("ePlus_sc_no", &ePlus_sc_no, "ePlus_sc_no/I");
    myTree->Branch("ePlus_EoverPout", &ePlus_EoverPout, "ePlus_EoverPout/F");
    myTree->Branch("ePlus_EoverPin", &ePlus_EoverPin, "ePlus_EoverPin/F");
    myTree->Branch("ePlus_InvEMinusInvP", &ePlus_InvEMinusInvP, 
		   "ePlus_InvEMinusInvP/F");
    myTree->Branch("ePlus_BremFraction", &ePlus_BremFraction, 
		   "ePlus_BremFraction/F");
    myTree->Branch("ePlus_DeltaEtaIn",&ePlus_DeltaEtaIn, "ePlus_DeltaEtaIn/F");
    myTree->Branch("ePlus_DeltaPhiIn",&ePlus_DeltaPhiIn, "ePlus_DeltaPhiIn/F");
    myTree->Branch("ePlus_DeltaPhiOut", &ePlus_DeltaPhiOut, 
		   "ePlus_DeltaPhiOut/F");
    myTree->Branch("ePlus_DeltaEtaOut", &ePlus_DeltaEtaOut, 
		   "ePlus_DeltaEtaOut/F");
    myTree->Branch("ePlus_Trackmom_calo", &ePlus_Trackmom_calo, 
		   "ePlus_Trackmom_calo/F");
    myTree->Branch("ePlus_Trackmom_vtx", &ePlus_Trackmom_vtx, 
		   "ePlus_Trackmom_vtx/F");	  
    myTree->Branch("ePlus_HoverE", &ePlus_HoverE, "ePlus_HoverE/F");	  
    myTree->Branch("ePlus_vx", &ePlus_vx, "ePlus_vx/F");
    myTree->Branch("ePlus_vy", &ePlus_vy, "ePlus_vy/F");
    myTree->Branch("ePlus_vz", &ePlus_vz, "ePlus_vz/F");	  
    myTree->Branch("ePlus_td0", &ePlus_td0, "ePlus_td0/F");
    myTree->Branch("ePlus_d0", &ePlus_d0, "ePlus_d0/F");	 	  
    myTree->Branch("ePlus_E9overE25", &ePlus_E9overE25, "ePlus_E9overE25/F");
    myTree->Branch("ePlus_SigmaEtaEta", &ePlus_SigmaEtaEta, 
		   "ePlus_SigmaEtaEta/F");
    myTree->Branch("ePlus_SigmaPhiPhi", &ePlus_SigmaPhiPhi, 
		   "ePlus_SigmaPhiPhi/F");	  
    myTree->Branch("ePlus_trk_eta", &ePlus_trk_eta, "ePlus_trk_eta/F");
    myTree->Branch("ePlus_trk_phi", &ePlus_trk_phi, "ePlus_trk_phi/F");
    myTree->Branch("ePlus_trk_etaout",&ePlus_trk_etaout, "ePlus_trk_etaout/F");
    myTree->Branch("ePlus_trk_phiout",&ePlus_trk_phiout, "ePlus_trk_phiout/F");
    myTree->Branch("ePlus_trk_p", &ePlus_trk_p, "ePlus_trk_p/F");
    myTree->Branch("ePlus_trk_pout", &ePlus_trk_pout, "ePlus_trk_pout/F");
    myTree->Branch("ePlus_trk_pt", &ePlus_trk_pt, "ePlus_trk_pt/F");
    myTree->Branch("ePlus_trk_charge",&ePlus_trk_charge, "ePlus_trk_charge/I");
    myTree->Branch("ePlus_trk_chi2", &ePlus_trk_chi2, "ePlus_trk_chi2/F");
    myTree->Branch("ePlus_trk_ndof", &ePlus_trk_ndof, "ePlus_trk_ndof/I");
    myTree->Branch("ePlus_trk_hits", &ePlus_trk_hits, "ePlus_trk_hits/I");
    myTree->Branch("ePlus_trk_d0", &ePlus_trk_d0, "ePlus_trk_d0/F");
    myTree->Branch("ePlus_trk_d0Err", &ePlus_trk_d0Err, "ePlus_trk_d0Err/F");
	  


    myTree->Branch("eMinuspx",     &eMinuspx,     "eMinuspx/F");
    myTree->Branch("eMinuspy",     &eMinuspy,     "eMinuspy/F");
    myTree->Branch("eMinuspz",     &eMinuspz,     "eMinuspz/F");
    myTree->Branch("eMinusE",      &eMinusE,      "eMinusE/F");
    myTree->Branch("eMinusPt",     &eMinusPt,     "eMinusPt/F");
    myTree->Branch("eMinusEt",     &eMinusEt,     "eMinusEt/F");
    myTree->Branch("eMinusEta",    &eMinusEta,    "eMinusEta/F");    
    myTree->Branch("eMinusPhi",    &eMinusPhi,    "eMinusPhi/F");
    myTree->Branch("eMinusCharge", &eMinusCharge, "eMinusCharge/F");
    myTree->Branch("eMinusVx",     &eMinusVx,     "eMinusVx/F");
    myTree->Branch("eMinusVy",     &eMinusVy,     "eMinusVy/F");
    myTree->Branch("eMinusVz",     &eMinusVz,     "eMinusVz/F");
    myTree->Branch("eMinusIsolation", &eMinusIsolation, "eMinusIsolation/F");



    //////////////
    myTree->Branch("eMinus_ptiso_15",   &eMinus_ptiso_15, "eMinus_ptiso_15/F");
    myTree->Branch("eMinus_ptiso_20",   &eMinus_ptiso_20, "eMinus_ptiso_20/F"); 
    myTree->Branch("eMinus_ptiso_25",   &eMinus_ptiso_25, "eMinus_ptiso_25/F");
    myTree->Branch("eMinus_ptiso_30",   &eMinus_ptiso_30, "eMinus_ptiso_30/F");
    myTree->Branch("eMinus_ptiso_35",   &eMinus_ptiso_35, "eMinus_ptiso_35/F");
    myTree->Branch("eMinus_ptiso_40",   &eMinus_ptiso_40, "eMinus_ptiso_40/F");
    myTree->Branch("eMinus_ptiso_45",   &eMinus_ptiso_45, "eMinus_ptiso_45/F");
    myTree->Branch("eMinus_ptiso_50",   &eMinus_ptiso_50, "eMinus_ptiso_50/F");

    myTree->Branch("eMinus_ptisoatecal_15", &eMinus_ptisoatecal_15, "eMinus_ptisoatecal_15/F");
    myTree->Branch("eMinus_ptisoatecal_20", &eMinus_ptisoatecal_20, "eMinus_ptisoatecal_20/F");
    myTree->Branch("eMinus_ptisoatecal_25", &eMinus_ptisoatecal_25, "eMinus_ptisoatecal_25/F");
    myTree->Branch("eMinus_ptisoatecal_30", &eMinus_ptisoatecal_30, "eMinus_ptisoatecal_30/F");
    myTree->Branch("eMinus_ptisoatecal_35", &eMinus_ptisoatecal_35, "eMinus_ptisoatecal_35/F");
    myTree->Branch("eMinus_ptisoatecal_40", &eMinus_ptisoatecal_40, "eMinus_ptisoatecal_40/F");
    myTree->Branch("eMinus_ptisoatecal_45", &eMinus_ptisoatecal_45, "eMinus_ptisoatecal_45/F");
    myTree->Branch("eMinus_ptisoatecal_50", &eMinus_ptisoatecal_50, "eMinus_ptisoatecal_50/F");

    myTree->Branch("eMinus_ntrkiso_15",  &eMinus_ntrkiso_15, "eMinus_ntrkiso_15/I");
    myTree->Branch("eMinus_ntrkiso_20",  &eMinus_ntrkiso_20, "eMinus_ntrkiso_20/I");
    myTree->Branch("eMinus_ntrkiso_25",  &eMinus_ntrkiso_25, "eMinus_ntrkiso_25/I");
    myTree->Branch("eMinus_ntrkiso_30",  &eMinus_ntrkiso_30, "eMinus_ntrkiso_30/I");
    myTree->Branch("eMinus_ntrkiso_35",  &eMinus_ntrkiso_35, "eMinus_ntrkiso_35/I");
    myTree->Branch("eMinus_ntrkiso_40",  &eMinus_ntrkiso_40, "eMinus_ntrkiso_40/I");
    myTree->Branch("eMinus_ntrkiso_45",  &eMinus_ntrkiso_45, "eMinus_ntrkiso_45/I");
    myTree->Branch("eMinus_ntrkiso_50",  &eMinus_ntrkiso_50, "eMinus_ntrkiso_50/I");
     
    myTree->Branch("eMinus_ntrkisoatecal_15",  &eMinus_ntrkisoatecal_15, "eMinus_ntrkisoatecal_15/I");
    myTree->Branch("eMinus_ntrkisoatecal_20",  &eMinus_ntrkisoatecal_20, "eMinus_ntrkisoatecal_20/I");
    myTree->Branch("eMinus_ntrkisoatecal_25",  &eMinus_ntrkisoatecal_25, "eMinus_ntrkisoatecal_25/I");
    myTree->Branch("eMinus_ntrkisoatecal_30",  &eMinus_ntrkisoatecal_30, "eMinus_ntrkisoatecal_30/I");
    myTree->Branch("eMinus_ntrkisoatecal_35",  &eMinus_ntrkisoatecal_35, "eMinus_ntrkisoatecal_35/I");
    myTree->Branch("eMinus_ntrkisoatecal_40",  &eMinus_ntrkisoatecal_40, "eMinus_ntrkisoatecal_40/I");
    myTree->Branch("eMinus_ntrkisoatecal_45",  &eMinus_ntrkisoatecal_45, "eMinus_ntrkisoatecal_45/I");
    myTree->Branch("eMinus_ntrkisoatecal_50",  &eMinus_ntrkisoatecal_50, "eMinus_ntrkisoatecal_50/I");

    myTree->Branch("eMinus_hcaliso_10",  &eMinus_hcaliso_10, "eMinus_hcaliso_10/F");
    myTree->Branch("eMinus_hcaliso_15",  &eMinus_hcaliso_15, "eMinus_hcaliso_15/F");
    myTree->Branch("eMinus_hcaliso_20",  &eMinus_hcaliso_20, "eMinus_hcaliso_20/F");
    myTree->Branch("eMinus_hcaliso_25",  &eMinus_hcaliso_25, "eMinus_hcaliso_25/F");
    myTree->Branch("eMinus_hcaliso_30",  &eMinus_hcaliso_30, "eMinus_hcaliso_30/F");
    myTree->Branch("eMinus_hcaliso_35",  &eMinus_hcaliso_35, "eMinus_hcaliso_35/F");
    myTree->Branch("eMinus_hcaliso_40",  &eMinus_hcaliso_40, "eMinus_hcaliso_40/F");
    myTree->Branch("eMinus_hcaliso_45",  &eMinus_hcaliso_45, "eMinus_hcaliso_45/F");
    myTree->Branch("eMinus_hcaliso_50",  &eMinus_hcaliso_50, "eMinus_hcaliso_50/F");

    myTree->Branch("eMinus_ecaliso_10",  &eMinus_ecaliso_10, "eMinus_ecaliso_10/F");
    myTree->Branch("eMinus_ecaliso_15",  &eMinus_ecaliso_15, "eMinus_ecaliso_15/F");
    myTree->Branch("eMinus_ecaliso_20",  &eMinus_ecaliso_20, "eMinus_ecaliso_20/F");
    myTree->Branch("eMinus_ecaliso_25",  &eMinus_ecaliso_25, "eMinus_ecaliso_25/F");
    myTree->Branch("eMinus_ecaliso_30",  &eMinus_ecaliso_30, "eMinus_ecaliso_30/F");
    myTree->Branch("eMinus_ecaliso_35",  &eMinus_ecaliso_35, "eMinus_ecaliso_35/F");
    myTree->Branch("eMinus_ecaliso_40",  &eMinus_ecaliso_40, "eMinus_ecaliso_40/F");
    myTree->Branch("eMinus_ecaliso_45",  &eMinus_ecaliso_45, "eMinus_ecaliso_45/F");
    myTree->Branch("eMinus_ecaliso_50",  &eMinus_ecaliso_50, "eMinus_ecaliso_50/F");
    ///////////////////



    myTree->Branch("eMinusClassification", &eMinusClassification, 
		   "eMinusClassification/I");
    myTree->Branch("iseMinusLoosePTDR",     &iseMinusLoosePTDR,     
		   "iseMinusLoosePTDR/O");
    myTree->Branch("iseMinusMediumPTDR",    &iseMinusMediumPTDR,    
		   "iseMinusMediumPTDR/O");
    myTree->Branch("iseMinusTightPTDR",     &iseMinusTightPTDR,     
		   "iseMinusTightPTDR/O");
    myTree->Branch("iseMinusLooseCutBased", &iseMinusLooseCutBased, 
		   "iseMinusLooseCutBased/O");
    myTree->Branch("iseMinusRobustCutBased",&iseMinusRobustCutBased,
		   "iseMinusRobustCutBased/O");
    myTree->Branch("iseMinusTightCutBased", &iseMinusTightCutBased, 
		   "iseMinusTightCutBased/O");

    myTree->Branch("eMinus_sc_x", &eMinus_sc_x, "eMinus_sc_x/F");
    myTree->Branch("eMinus_sc_y", &eMinus_sc_y, "eMinus_sc_y/F");
    myTree->Branch("eMinus_sc_z", &eMinus_sc_z, "eMinus_sc_z/F");
    myTree->Branch("eMinus_sc_R", &eMinus_sc_R, "eMinus_sc_R/F");
    myTree->Branch("eMinus_sc_Rt", &eMinus_sc_Rt, "eMinus_sc_Rt/F");
    myTree->Branch("eMinus_sc_eta", &eMinus_sc_eta, "eMinus_sc_eta/F");
    myTree->Branch("eMinus_sc_phi", &eMinus_sc_phi, "eMinus_sc_phi/F");
    myTree->Branch("eMinus_sc_theta", &eMinus_sc_theta, "eMinus_sc_theta/F");
    myTree->Branch("eMinus_sc_et", &eMinus_sc_et, "eMinus_sc_et/F");
    myTree->Branch("eMinus_sc_e", &eMinus_sc_e, "eMinus_sc_e/F");
    myTree->Branch("eMinus_sc_no", &eMinus_sc_no, "eMinus_sc_no/I");
    myTree->Branch("eMinus_EoverPout", &eMinus_EoverPout, 
		   "eMinus_EoverPout/F");
    myTree->Branch("eMinus_EoverPin", &eMinus_EoverPin, "eMinus_EoverPin/F");
    myTree->Branch("eMinus_InvEMinusInvP", &eMinus_InvEMinusInvP, 
		   "eMinus_InvEMinusInvP/F");
    myTree->Branch("eMinus_BremFraction", &eMinus_BremFraction, 
		   "eMinus_BremFraction/F");
    myTree->Branch("eMinus_DeltaEtaIn",&eMinus_DeltaEtaIn, 
		   "eMinus_DeltaEtaIn/F");
    myTree->Branch("eMinus_DeltaPhiIn",&eMinus_DeltaPhiIn, 
		   "eMinus_DeltaPhiIn/F");
    myTree->Branch("eMinus_DeltaPhiOut", &eMinus_DeltaPhiOut, 
		   "eMinus_DeltaPhiOut/F");
    myTree->Branch("eMinus_DeltaEtaOut", &eMinus_DeltaEtaOut, 
		   "eMinus_DeltaEtaOut/F");
    myTree->Branch("eMinus_Trackmom_calo", &eMinus_Trackmom_calo, 
		   "eMinus_Trackmom_calo/F");
    myTree->Branch("eMinus_Trackmom_vtx", &eMinus_Trackmom_vtx, 
		   "eMinus_Trackmom_vtx/F");	  
    myTree->Branch("eMinus_HoverE", &eMinus_HoverE, "eMinus_HoverE/F");	  
    myTree->Branch("eMinus_vx", &eMinus_vx, "eMinus_vx/F");
    myTree->Branch("eMinus_vy", &eMinus_vy, "eMinus_vy/F");
    myTree->Branch("eMinus_vz", &eMinus_vz, "eMinus_vz/F");	  
    myTree->Branch("eMinus_td0", &eMinus_td0, "eMinus_td0/F");
    myTree->Branch("eMinus_d0", &eMinus_d0, "eMinus_d0/F");	 	  
    myTree->Branch("eMinus_E9overE25", &eMinus_E9overE25, 
		   "eMinus_E9overE25/F");
    myTree->Branch("eMinus_SigmaEtaEta", &eMinus_SigmaEtaEta, 
		   "eMinus_SigmaEtaEta/F");
    myTree->Branch("eMinus_SigmaPhiPhi", &eMinus_SigmaPhiPhi, 
		   "eMinus_SigmaPhiPhi/F");	  
    myTree->Branch("eMinus_trk_eta", &eMinus_trk_eta, "eMinus_trk_eta/F");
    myTree->Branch("eMinus_trk_phi", &eMinus_trk_phi, "eMinus_trk_phi/F");
    myTree->Branch("eMinus_trk_etaout",&eMinus_trk_etaout, 
		   "eMinus_trk_etaout/F");
    myTree->Branch("eMinus_trk_phiout",&eMinus_trk_phiout, 
		   "eMinus_trk_phiout/F");
    myTree->Branch("eMinus_trk_p", &eMinus_trk_p, "eMinus_trk_p/F");
    myTree->Branch("eMinus_trk_pout", &eMinus_trk_pout, "eMinus_trk_pout/F");
    myTree->Branch("eMinus_trk_pt", &eMinus_trk_pt, "eMinus_trk_pt/F");
    myTree->Branch("eMinus_trk_charge",&eMinus_trk_charge, 
		   "eMinus_trk_charge/I");
    myTree->Branch("eMinus_trk_chi2", &eMinus_trk_chi2, "eMinus_trk_chi2/F");
    myTree->Branch("eMinus_trk_ndof", &eMinus_trk_ndof, "eMinus_trk_ndof/I");
    myTree->Branch("eMinus_trk_hits", &eMinus_trk_hits, "eMinus_trk_hits/I");
    myTree->Branch("eMinus_trk_d0", &eMinus_trk_d0, "eMinus_trk_d0/F");
    myTree->Branch("eMinus_trk_d0Err", &eMinus_trk_d0Err, 
		   "eMinus_trk_d0Err/F");
	  

    myTree->Branch("mZeeGen",     &mZeeGen,     "mZeeGen/F");
    myTree->Branch("Z_pxGen",     &Z_pxGen,     "Z_pxGen/F");
    myTree->Branch("Z_pyGen",     &Z_pyGen,     "Z_pyGen/F");
    myTree->Branch("Z_pzGen",     &Z_pzGen,     "Z_pzGen/F");
    myTree->Branch("Z_EGen",     &Z_EGen,     "Z_EGen/F");
    myTree->Branch("Z_PtGen",     &Z_PtGen,     "Z_PtGen/F");
    myTree->Branch("Z_EtGen",     &Z_EtGen,     "Z_EtGen/F");
    myTree->Branch("Z_EtaGen",     &Z_EtaGen,     "Z_EtaGen/F");    
    myTree->Branch("Z_PhiGen",     &Z_PhiGen,     "Z_PhiGen/F");
    myTree->Branch("Z_ChargeGen",  &Z_ChargeGen,     "Z_ChargeGen/F");
    myTree->Branch("Z_VxGen",     &Z_VxGen,     "Z_VxGen/F");
    myTree->Branch("Z_VyGen",     &Z_VyGen,     "Z_VyGen/F");
    myTree->Branch("Z_VzGen",     &Z_VzGen,     "Z_VzGen/F");

    myTree->Branch("ePluspxGen",     &ePluspxGen,     "ePluspxGen/F");
    myTree->Branch("ePluspyGen",     &ePluspyGen,     "ePluspyGen/F");
    myTree->Branch("ePluspzGen",     &ePluspzGen,     "ePluspzGen/F");
    myTree->Branch("ePlusEGen",     &ePlusEGen,       "ePlusEGen/F");
    myTree->Branch("ePlusPtGen",     &ePlusPtGen,     "ePlusPtGen/F");
    myTree->Branch("ePlusEtGen",     &ePlusEtGen,     "ePlusEtGen/F");    
    myTree->Branch("ePlusEtaGen",       &ePlusEtaGen, "ePlusEtaGen/F");    
    myTree->Branch("ePlusPhiGen",     &ePlusPhiGen,   "ePlusPhiGen/F");
    myTree->Branch("ePlusChargeGen",  &ePlusChargeGen,"ePlusChargeGen/F");
    myTree->Branch("ePlusVxGen",     &ePlusVxGen,     "ePlusVxGen/F");
    myTree->Branch("ePlusVyGen",     &ePlusVyGen,     "ePlusVyGen/F");
    myTree->Branch("ePlusVzGen",     &ePlusVzGen,     "ePlusVzGen/F");
    myTree->Branch("ePlusClassificationGen", &ePlusClassificationGen, 
		   "ePlusClassificationGen/I");      

    myTree->Branch("eMinuspxGen",     &eMinuspxGen,     "eMinuspxGen/F");
    myTree->Branch("eMinuspyGen",     &eMinuspyGen,     "eMinuspyGen/F");
    myTree->Branch("eMinuspzGen",     &eMinuspzGen,     "eMinuspzGen/F");
    myTree->Branch("eMinusEGen",      &eMinusEGen,      "eMinusEGen/F");
    myTree->Branch("eMinusPtGen",     &eMinusPtGen,     "eMinusPtGen/F");
    myTree->Branch("eMinusEtGen",     &eMinusEtGen,     "eMinusEtGen/F");
    myTree->Branch("eMinusEtaGen",       &eMinusEtaGen,   "eMinusEtaGen/F");
    myTree->Branch("eMinusPhiGen",     &eMinusPhiGen,     "eMinusPhiGen/F");
    myTree->Branch("eMinusChargeGen",  &eMinusChargeGen,  "eMinusChargeGen/F");
    myTree->Branch("eMinusVxGen",     &eMinusVxGen,       "eMinusVxGen/F");
    myTree->Branch("eMinusVyGen",     &eMinusVyGen,       "eMinusVyGen/F");
    myTree->Branch("eMinusVzGen",     &eMinusVzGen,       "eMinusVzGen/F");
    myTree->Branch("eMinusClassificationGen", &eMinusClassificationGen, 
		   "eMinusClassificationGen/I");


    // Jet block
    myTree->Branch("NumRecoJetAlgo", &NumRecoJetAlgo, "NumRecoJetAlgo/I");
    myTree->Branch("NumRecoJets", &NumRecoJets, "NumRecoJets/I");   
    myTree->Branch("JetRecoEt",  JetRecoEt, "JetRecoEt[20][4]/F");
    myTree->Branch("JetRecoPt",  JetRecoPt, "JetRecoPt[20][4]/F");
    myTree->Branch("JetRecoEta",  JetRecoEta, "JetRecoEta[20][4]/F");
    myTree->Branch("JetRecoPhi",  JetRecoPhi, "JetRecoPhi[20][4]/F");
    myTree->Branch("JetRecoType", JetRecoType, "JetRecoType[20][4]/I");
    myTree->Branch("JetRecoFlavor", JetRecoFlavor, "JetRecoFlavor[20][4]/I");


    /** Returns the maximum energy deposited in ECAL towers*/
    myTree->Branch("JetRecoMaxEInEmTowers",  JetRecoMaxEInEmTowers, 
		   "JetRecoMaxEInEmTowers[20][4]/F");
    /** Returns the maximum energy deposited in HCAL towers*/
    myTree->Branch("JetRecoMaxEInHadTowers",  JetRecoMaxEInHadTowers, 
		   "JetRecoMaxEInHadTowers[20][4]/F");
    /** Returns the jet hadronic energy fraction*/
    myTree->Branch("JetRecoEnergyFractionHadronic", 
		   JetRecoEnergyFractionHadronic, 
		   "JetRecoEnergyFractionHadronic[20][4]/F");
    /** Returns the jet electromagnetic energy fraction*/
    myTree->Branch("JetRecoEmEnergyFraction",  JetRecoEmEnergyFraction, 
		   "JetRecoEmEnergyFraction[20][4]/F");
    /** Returns the jet hadronic energy in HB*/ 
    myTree->Branch("JetRecoHadEnergyInHB",  JetRecoHadEnergyInHB, 
		   "JetRecoHadEnergyInHB[20][4]/F");
    /** Returns the jet hadronic energy in HO*/
    myTree->Branch("JetRecoHadEnergyInHO",  JetRecoHadEnergyInHO, 
		   "JetRecoHadEnergyInHO[20][4]/F");
    /** Returns the jet hadronic energy in HE*/
    myTree->Branch("JetRecoHadEnergyInHE",  JetRecoHadEnergyInHE, 
		   "JetRecoHadEnergyInHE[20][4]/F");
    /** Returns the jet hadronic energy in HF*/
    myTree->Branch("JetRecoHadEnergyInHF",  JetRecoHadEnergyInHF, 
		   "JetRecoHadEnergyInHF[20][4]/F");
    /** Returns the jet electromagnetic energy in EB*/
    myTree->Branch("JetRecoEmEnergyInEB",  JetRecoEmEnergyInEB, 
		   "JetRecoEmEnergyInEB[20][4]/F");
    /** Returns the jet electromagnetic energy in EE*/
    myTree->Branch("JetRecoEmEnergyInEE",  JetRecoEmEnergyInEE, 
		   "JetRecoEmEnergyInEE[20][4]/F");
    /** Returns the jet electromagnetic energy extracted from HF*/
    myTree->Branch("JetRecoEmEnergyInHF",  JetRecoEmEnergyInHF, 
		   "JetRecoEmEnergyInHF[20][4]/F");
    /** Returns area of contributing towers */
    myTree->Branch("JetRecoTowersArea",  JetRecoTowersArea, 
		   "JetRecoTowersArea[20][4]/F");
    /** Number of constituents carrying a 90% of the total Jet energy*/
    myTree->Branch("JetRecoN90",  JetRecoN90, "JetRecoN90[20][4]/I");
    /** Number of constituents carrying a 60% of the total Jet energy*/
    myTree->Branch("JetRecoN60",  JetRecoN60, "JetRecoN60[20][4]/I");



    myTree->Branch("NumGenJetAlgo", &NumGenJetAlgo, "NumGenJetAlgo/I");
    myTree->Branch("NumGenJets", &NumGenJets, "NumGenJets/I");   
    myTree->Branch("JetGenEt",  JetGenEt, "JetGenEt[20][4]/F");
    myTree->Branch("JetGenPt",  JetGenPt, "JetGenPt[20][4]/F");
    myTree->Branch("JetGenEta",  JetGenEta, "JetGenEta[20][4]/F");
    myTree->Branch("JetGenPhi",  JetGenPhi, "JetGenPhi[20][4]/F");
    myTree->Branch("JetGenType", JetGenType, "JetGenType[20][4]/I");
    /** Returns energy of electromagnetic particles*/
    myTree->Branch("JetGenEmEnergy", JetGenEmEnergy,
		   "JetGenEmEnergy[20][4]/F");
    /** Returns energy of hadronic particles*/
    myTree->Branch("JetGenHadEnergy",  JetGenHadEnergy, 
		   "JetGenHadEnergy[20][4]/F");
    /** Returns invisible energy*/
    myTree->Branch("JetGenInvisibleEnergy",  JetGenInvisibleEnergy, 
		   "JetGenInvisibleEnergy[20][4]/F");
    /** Returns other energy (undecayed Sigmas etc.)*/
    myTree->Branch("JetGenAuxiliaryEnergy",  JetGenAuxiliaryEnergy, 
		   "JetGenAuxiliaryEnergy[20][4]/F");


    // Corrected CaloJet
    myTree->Branch("JetCorEt",  JetCorEt, "JetCorEt[20][4]/F");
    myTree->Branch("JetCorPt",  JetCorPt, "JetCorPt[20][4]/F");
    myTree->Branch("JetCorEta",  JetCorEta, "JetCorEta[20][4]/F");
    myTree->Branch("JetCorPhi",  JetCorPhi, "JetCorPhi[20][4]/F");
    myTree->Branch("JetCorEnergyFractionHadronic", 
		   JetCorEnergyFractionHadronic, 
		   "JetCorEnergyFractionHadronic[20][4]/F");
    myTree->Branch("JetCorEmEnergyFraction",  JetCorEmEnergyFraction, 
		   "JetCorEmEnergyFraction[20][4]/F");


    // Tracks block
    if(StoreTrackInformation_) {
      myTree->Branch("NumRecoTrack", &NumRecoTrack, "NumRecoTrack/I");
      myTree->Branch("TrackRecoP", TrackRecoP, "TrackRecoP[NumRecoTrack]/F");
      myTree->Branch("TrackRecoPx",TrackRecoPx, "TrackRecoPx[NumRecoTrack]/F");
      myTree->Branch("TrackRecoPy",TrackRecoPy, "TrackRecoPy[NumRecoTrack]/F");
      myTree->Branch("TrackRecoPz",TrackRecoPz, "TrackRecoPz[NumRecoTrack]/F");
      myTree->Branch("TrackRecoVx",TrackRecoVx, "TrackRecoVx[NumRecoTrack]/F");
      myTree->Branch("TrackRecoVy",TrackRecoVy, "TrackRecoVy[NumRecoTrack]/F");
      myTree->Branch("TrackRecoVz",TrackRecoVz, "TrackRecoVz[NumRecoTrack]/F");
      myTree->Branch("TrackRecoPt",TrackRecoPt, "TrackRecoPt[NumRecoTrack]/F");
      myTree->Branch("TrackRecoEta",TrackRecoEta,
		     "TrackRecoEta[NumRecoTrack]/F");
      myTree->Branch("TrackRecoPhi",TrackRecoPhi,
		     "TrackRecoPhi[NumRecoTrack]/F");
    }



    // Particle block
    if(StoreParticleInformation_) {
      myTree->Branch("NumPart", &NumPart, "NumPart/I"); 
      myTree->Branch("partStatus", partStatus, "partStatus[NumPart]/I");     
      myTree->Branch("partPDGId", partPDGId, "partPDGId[NumPart]/I");
      myTree->Branch("partMother1", partMother1, "partMother1[NumPart]/I");
      myTree->Branch("partpx",  partpx, "partpx[NumPart]/F");
      myTree->Branch("partpy",  partpy, "partpy[NumPart]/F");
      myTree->Branch("partpz",  partpz, "partpz[NumPart]/F");
      myTree->Branch("parte",  parte, "parte[NumPart]/F");
      myTree->Branch("partm",  partm, "partm[NumPart]/F");
      myTree->Branch("partvx",  partvx, "partvx[NumPart]/F");
      myTree->Branch("partvy",  partvy, "partvy[NumPart]/F");
      myTree->Branch("partvz",  partvz, "partvz[NumPart]/F");
      myTree->Branch("partvt",  partvt, "partvt[NumPart]/F");
    }
    // end of tree declaration



    //##################################
    // e+, e- histograms

    m_HistNames1D["EmECALIsolation10"] = new TH1F("EmECALIsolation10","", 200, 0.0, 0.4);
    m_HistNames1D["EmECALIsolation20"] = new TH1F("EmECALIsolation20","", 200, 0.0, 0.4);
    m_HistNames1D["EmECALIsolation30"] = new TH1F("EmECALIsolation30","", 200, 0.0, 0.4);
    m_HistNames1D["EmECALIsolation40"] = new TH1F("EmECALIsolation40","", 200, 0.0, 0.4);
    m_HistNames1D["EmECALIsolation50"] = new TH1F("EmECALIsolation50","", 200, 0.0, 0.4);
    m_HistNames1D["EmHCALIsolation10"] = new TH1F("EmHCALIsolation10","", 400, -0.1, 0.1);
    m_HistNames1D["EmHCALIsolation20"] = new TH1F("EmHCALIsolation20","", 400, -0.1, 0.1);
    m_HistNames1D["EmHCALIsolation30"] = new TH1F("EmHCALIsolation30","", 400, -0.1, 0.1);
    m_HistNames1D["EmHCALIsolation40"] = new TH1F("EmHCALIsolation40","", 400, -0.1, 0.1);
    m_HistNames1D["EmHCALIsolation50"] = new TH1F("EmHCALIsolation50","", 400, -0.1, 0.1);
    m_HistNames1D["EmTrackPTIsolation10"] = new TH1F("EmTrackPTIsolation10","", 60, 0.0, 3.0);
    m_HistNames1D["EmTrackPTIsolation20"] = new TH1F("EmTrackPTIsolation20","", 60, 0.0, 3.0);
    m_HistNames1D["EmTrackPTIsolation30"] = new TH1F("EmTrackPTIsolation30","", 60, 0.0, 3.0);
    m_HistNames1D["EmTrackPTIsolation40"] = new TH1F("EmTrackPTIsolation40","", 60, 0.0, 3.0);
    m_HistNames1D["EmTrackPTIsolation50"] = new TH1F("EmTrackPTIsolation50","", 60, 0.0, 3.0);
    m_HistNames1D["EmTrackNumIsolation10"] = new TH1F("EmTrackNumIsolation10","", 8, 0.0, 8.0);
    m_HistNames1D["EmTrackNumIsolation20"] = new TH1F("EmTrackNumIsolation20","", 8, 0.0, 8.0);
    m_HistNames1D["EmTrackNumIsolation30"] = new TH1F("EmTrackNumIsolation30","", 8, 0.0, 8.0);
    m_HistNames1D["EmTrackNumIsolation40"] = new TH1F("EmTrackNumIsolation40","", 8, 0.0, 8.0);
    m_HistNames1D["EmTrackNumIsolation50"] = new TH1F("EmTrackNumIsolation50","", 8, 0.0, 8.0);
    m_HistNames1D["EPECALIsolation10"] = new TH1F("EPECALIsolation10","", 200, 0.0, 0.4);
    m_HistNames1D["EPECALIsolation20"] = new TH1F("EPECALIsolation20","", 200, 0.0, 0.4);
    m_HistNames1D["EPECALIsolation30"] = new TH1F("EPECALIsolation30","", 200, 0.0, 0.4);
    m_HistNames1D["EPECALIsolation40"] = new TH1F("EPECALIsolation40","", 200, 0.0, 0.4);
    m_HistNames1D["EPECALIsolation50"] = new TH1F("EPECALIsolation50","", 200, 0.0, 0.4);
    m_HistNames1D["EPHCALIsolation10"] = new TH1F("EPHCALIsolation10","", 400, -0.1, 0.1);
    m_HistNames1D["EPHCALIsolation20"] = new TH1F("EPHCALIsolation20","", 400, -0.1, 0.1);
    m_HistNames1D["EPHCALIsolation30"] = new TH1F("EPHCALIsolation30","", 400, -0.1, 0.1);
    m_HistNames1D["EPHCALIsolation40"] = new TH1F("EPHCALIsolation40","", 400, -0.1, 0.1);
    m_HistNames1D["EPHCALIsolation50"] = new TH1F("EPHCALIsolation50","", 400, -0.1, 0.1);
    m_HistNames1D["EPTrackPTIsolation10"] = new TH1F("EPTrackPTIsolation10","", 60, 0.0, 3.0);
    m_HistNames1D["EPTrackPTIsolation20"] = new TH1F("EPTrackPTIsolation20","", 60, 0.0, 3.0);
    m_HistNames1D["EPTrackPTIsolation30"] = new TH1F("EPTrackPTIsolation30","", 60, 0.0, 3.0);
    m_HistNames1D["EPTrackPTIsolation40"] = new TH1F("EPTrackPTIsolation40","", 60, 0.0, 3.0);
    m_HistNames1D["EPTrackPTIsolation50"] = new TH1F("EPTrackPTIsolation50","", 60, 0.0, 3.0);
    m_HistNames1D["EPTrackNumIsolation10"] = new TH1F("EPTrackNumIsolation10","", 8, 0.0, 8.0);
    m_HistNames1D["EPTrackNumIsolation20"] = new TH1F("EPTrackNumIsolation20","", 8, 0.0, 8.0);
    m_HistNames1D["EPTrackNumIsolation30"] = new TH1F("EPTrackNumIsolation30","", 8, 0.0, 8.0);
    m_HistNames1D["EPTrackNumIsolation40"] = new TH1F("EPTrackNumIsolation40","", 8, 0.0, 8.0);
    m_HistNames1D["EPTrackNumIsolation50"] = new TH1F("EPTrackNumIsolation50","", 8, 0.0, 8.0);
    m_HistNames1D["EmEoverP"] = new TH1F("EmEoverP","", 350, 0.0, 3.5);
    m_HistNames1D["EpEoverP"] = new TH1F("EpEoverP","", 350, 0.0, 3.5);
    m_HistNames1D["EpBremFraction"] = new TH1F("EpBremFraction","", 200, 0.0, 200.0);
    m_HistNames1D["EmBremFraction"] = new TH1F("EmBremFraction","", 200, 0.0, 200.0);
    m_HistNames1D["EpClassification"] = new TH1F("EpClassification","", 141, -1.0, 140.0);
    m_HistNames1D["EpHoverE"] = new TH1F("EpHoverE","", 400, -0.1, 0.1);
    m_HistNames1D["EmHoverE"] = new TH1F("EmHoverE","", 400, -0.1, 0.1);
    m_HistNames1D["genEPpt"] = new TH1F("genEPpt","", 200, 0.0, 1000.0);
    m_HistNames1D["recoEPpt"] = new TH1F("recoEPpt","", 200, 0.0, 1000.0);
    m_HistNames1D["genEMpt"] = new TH1F("genEMpt","", 200, 0.0, 1000.0);
    m_HistNames1D["recoEMpt"] = new TH1F("recoEMpt","", 200, 0.0, 1000.0);
    m_HistNames1D["genEMptMassCut"] = new TH1F("genEMptMassCut","", 45, 0.0, 225.0);
    m_HistNames1D["recoEMptMassCut"] = new TH1F("recoEMptMassCut","", 45, 0.0, 225.0);
    m_HistNames1D["genEMptGolden"] = new TH1F("genEMptGolden","", 20, 0.0, 100.0);
    m_HistNames1D["genEMptBigbrem"] = new TH1F("genEMptBigbrem","", 20, 0.0, 100.0);
    m_HistNames1D["genEMptNarrow"] = new TH1F("genEMptNarrow","", 20, 0.0, 100.0);
    m_HistNames1D["genEMptShowering"] = new TH1F("genEMptShowering","", 20, 0.0, 100.0);
    m_HistNames1D["recoEMptGolden"] = new TH1F("recoEMptGolden","", 20, 0.0, 100.0);
    m_HistNames1D["recoEMptBigbrem"] = new TH1F("recoEMptBigbrem","", 20, 0.0, 100.0);
    m_HistNames1D["recoEMptNarrow"] = new TH1F("recoEMptNarrow","", 20, 0.0, 100.0);
    m_HistNames1D["recoEMptShowering"] = new TH1F("recoEMptShowering","", 20, 0.0, 100.0);


    // Z histograms
    m_HistNames1D["Zmass_noIso"] = new TH1F("Zmass_noIso","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_TkIso"] = new TH1F("Zmass_TkIso","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_TkECALIso"] = new TH1F("Zmass_TkECALIso","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_TkECALHCALIso"] = new TH1F("Zmass_TkECALHCALIso","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_ptdrtight"] = new TH1F("Zmass_ptdrtight","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_ptdrmedium"] = new TH1F("Zmass_ptdrmedium","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_ptdrloose"] = new TH1F("Zmass_ptdrloose","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_BB"] = new TH1F("Zmass_BB","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_BE"] = new TH1F("Zmass_BE","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_EE"] = new TH1F("Zmass_EE","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_golden"] = new TH1F("Zmass_golden","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_bigbrem"] = new TH1F("Zmass_bigbrem","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_narrow"] = new TH1F("Zmass_narrow","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_shower"] = new TH1F("Zmass_shower","", 120, 60.0, 120.0);
    m_HistNames1D["genZpt"] = new TH1F("genZpt","", 200, 0.0, 1000.0);
    m_HistNames1D["recoZpt"] = new TH1F("recoZpt","", 200, 0.0, 1000.0);
    m_HistNames1D["genZptm"] = new TH1F("genZptm","", 200, 0.0, 1000.0);
    m_HistNames1D["recoZptm"] = new TH1F("recoZptm","", 200, 0.0, 1000.0);
    m_HistNames1D["genZmass"] = new TH1F("genZmass","", 120, 60.0, 120.0);
    m_HistNames1D["recoZmass"] = new TH1F("recoZmass","", 120, 60.0, 120.0);

  }





  // ------------ method called to produce the data  ------------
  void
  ZJetAnalysis::analyze(const edm::Event& iEvent, 
			const edm::EventSetup& iSetup) {
    using namespace edm;

    //default initializatied values
    
    mZee                  = -99999.;
    Z_px                  = -99999.;
    Z_py                  = -99999.;
    Z_pz                  = -99999.;
    Z_E                   = -99999.;
    Z_Pt                  = -99999.;
    Z_Et                  = -99999.;
    Z_Eta                 = -99999.;
    Z_Phi                 = -99999.;
    Z_Charge              = -99999.;
    Z_Vx                  = -99999.;
    Z_Vy                  = -99999.;
    Z_Vz                  = -99999.;

    mZeeGen               = -99999.;
    Z_pxGen               = -99999.;
    Z_pyGen               = -99999.;
    Z_pzGen               = -99999.;
    Z_EGen                = -99999.;
    Z_PtGen               = -99999.;
    Z_EtGen               = -99999.;
    Z_EtaGen              = -99999.;
    Z_PhiGen              = -99999.;
    Z_ChargeGen           = -99999.;
    Z_VxGen               = -99999.;
    Z_VyGen               = -99999.;
    Z_VzGen               = -99999.;

    isePlusLoosePTDR      = false;
    isePlusMediumPTDR     = false;
    isePlusTightPTDR      = false;
    isePlusLooseCutBased  = false;
    isePlusRobustCutBased = false;
    isePlusTightCutBased  = false;
    ePlusClassification   = -1; 
    ePluspx               = -99999.;
    ePluspy               = -99999.;
    ePluspz               = -99999.;
    ePlusE                = -99999.;
    ePlusEt               = -99999.;
    ePlusPt               = -99999.;
    ePlusEta              = -99999.;
    ePlusPhi              = -99999.;
    ePlusCharge           = -99999.;
    ePlusVx               = -99999.;
    ePlusVy               = -99999.;
    ePlusVz               = -99999.;
    ePlusIsolation        = -99999.;

    ePlus_sc_x            = -99999.;
    ePlus_sc_y            = -99999.;
    ePlus_sc_z            = -99999.;
    ePlus_sc_R            = -99999.;
    ePlus_sc_Rt           = -99999.;
    ePlus_sc_eta          = -99999.;
    ePlus_sc_phi          = -99999.;
    ePlus_sc_theta        = -99999.;
    ePlus_sc_et           = -99999.;
    ePlus_sc_e            = -99999.;
    ePlus_sc_no           = -1;
    ePlus_EoverPout       = -99999.;
    ePlus_EoverPin        = -99999.;
    ePlus_InvEMinusInvP   = -99999.;
    ePlus_BremFraction    = -99999.;
    ePlus_DeltaEtaIn      = -99999.;
    ePlus_DeltaPhiIn      = -99999.;
    ePlus_DeltaPhiOut     = -99999.;
    ePlus_DeltaEtaOut     = -99999.;
    ePlus_Trackmom_calo   = -99999.;
    ePlus_Trackmom_vtx    = -99999.;	  
    ePlus_HoverE          = -99999.;	  
    ePlus_vx              = -99999.;
    ePlus_vy              = -99999.;
    ePlus_vz              = -99999.;	  
    ePlus_td0             = -99999.;
    ePlus_d0              = -99999.;	 	  
    ePlus_E9overE25       = -99999.;
    ePlus_SigmaEtaEta     = -99999.;
    ePlus_SigmaPhiPhi     = -99999.;	  
    ePlus_trk_eta         = -99999.;
    ePlus_trk_phi         = -99999.;
    ePlus_trk_etaout      = -99999.;
    ePlus_trk_phiout      = -99999.;
    ePlus_trk_p           = -99999.;
    ePlus_trk_pout        = -99999.;
    ePlus_trk_pt          = -99999.;
    ePlus_trk_charge      = -99999;
    ePlus_trk_chi2        = -99999.;
    ePlus_trk_ndof        = -99999.;
    ePlus_trk_hits        = -1;
    ePlus_trk_d0          = -99999.;
    ePlus_trk_d0Err       = -99999.;
	  

    ePlusClassificationGen= -1;
    ePluspxGen            = -99999.;
    ePluspyGen            = -99999.;
    ePluspzGen            = -99999.;
    ePlusEGen             = -99999.;
    ePlusPtGen            = -99999.;
    ePlusEtGen            = -99999.;
    ePlusEtaGen           = -99999.;
    ePlusPhiGen           = -99999.;
    ePlusChargeGen        = -99999.;
    ePlusVxGen            = -99999.;
    ePlusVyGen            = -99999.;
    ePlusVzGen            = -99999.;   
  
    iseMinusLoosePTDR      = false;
    iseMinusMediumPTDR     = false;
    iseMinusTightPTDR      = false;
    iseMinusLooseCutBased  = false;
    iseMinusRobustCutBased = false;
    iseMinusTightCutBased  = false;
    eMinusClassification  = -1;
    eMinuspx              = -99999.;
    eMinuspy              = -99999.;
    eMinuspz              = -99999.;
    eMinusE               = -99999.;
    eMinusPt              = -99999.;
    eMinusEt              = -99999.;
    eMinusEta             = -99999.;
    eMinusPhi             = -99999.;
    eMinusCharge          = -99999.;
    eMinusVx              = -99999.;
    eMinusVy              = -99999.;
    eMinusVz              = -99999.;
    eMinusIsolation        = -99999.;

    eMinus_sc_x            = -99999.;
    eMinus_sc_y            = -99999.;
    eMinus_sc_z            = -99999.;
    eMinus_sc_R            = -99999.;
    eMinus_sc_Rt           = -99999.;
    eMinus_sc_eta          = -99999.;
    eMinus_sc_phi          = -99999.;
    eMinus_sc_theta        = -99999.;
    eMinus_sc_et           = -99999.;
    eMinus_sc_e            = -99999.;
    eMinus_sc_no           = -1;
    eMinus_EoverPout       = -99999.;
    eMinus_EoverPin        = -99999.;
    eMinus_InvEMinusInvP   = -99999.;
    eMinus_BremFraction    = -99999.;
    eMinus_DeltaEtaIn      = -99999.;
    eMinus_DeltaPhiIn      = -99999.;
    eMinus_DeltaPhiOut     = -99999.;
    eMinus_DeltaEtaOut     = -99999.;
    eMinus_Trackmom_calo   = -99999.;
    eMinus_Trackmom_vtx    = -99999.;	  
    eMinus_HoverE          = -99999.;	  
    eMinus_vx              = -99999.;
    eMinus_vy              = -99999.;
    eMinus_vz              = -99999.;	  
    eMinus_td0             = -99999.;
    eMinus_d0              = -99999.;	 	  
    eMinus_E9overE25       = -99999.;
    eMinus_SigmaEtaEta     = -99999.;
    eMinus_SigmaPhiPhi     = -99999.;	  
    eMinus_trk_eta         = -99999.;
    eMinus_trk_phi         = -99999.;
    eMinus_trk_etaout      = -99999.;
    eMinus_trk_phiout      = -99999.;
    eMinus_trk_p           = -99999.;
    eMinus_trk_pout        = -99999.;
    eMinus_trk_pt          = -99999.;
    eMinus_trk_charge      = -99999;
    eMinus_trk_chi2        = -99999.;
    eMinus_trk_ndof        = -99999.;
    eMinus_trk_hits        = -1;
    eMinus_trk_d0          = -99999.;
    eMinus_trk_d0Err       = -99999.;
	  
    eMinusClassificationGen = -1;
    eMinuspxGen           = -99999.;
    eMinuspyGen           = -99999.;
    eMinuspzGen           = -99999.;
    eMinusEGen            = -99999.;
    eMinusPtGen           = -99999.;
    eMinusEtGen           = -99999.;
    eMinusEtaGen          = -99999.;
    eMinusPhiGen          = -99999.;
    eMinusChargeGen       = -99999.;
    eMinusVxGen           = -99999.;
    eMinusVyGen           = -99999.;
    eMinusVzGen           = -99999.;


    // write HEPEVT block into file
    run = iEvent.id().run();
    event = iEvent.id().event();

    edm::Handle< double > genEventScale_;
    iEvent.getByLabel("genEventScale", genEventScale_);
    genEventScale = (float)(*genEventScale_);

    edm::Handle< double > genEventWeight_;
    iEvent.getByLabel("genEventWeight", genEventWeight_);
    genEventWeight = (float)(*genEventWeight_);


    // Calorimeter geometry
   edm::ESHandle<CaloGeometry> geoHandle;
   iSetup.get<IdealGeometryRecord>().get(geoHandle);
   geometry_ = geoHandle.product();
 


    // ********* Store Generated Particle Level Information **********
    if(StoreParticleInformation_) {
      StoreParticleInformation (iEvent);
    }



    // *********** Load Calo Jets ******************
    StoreCaloJetInformation (iEvent);
    StoreCorJetInformation (iEvent);


    // *********** Load Gen Jets ******************
    StoreGenJetInformation (iEvent);


    /**  Store reconstructed  Z--> e+e-  information */
    StoreRecoZeeInformation (iEvent);


    /**  Store generated  Z--> e+e-  information */
    StoreGenZeeInformation (iEvent);


    //  ************* Load Tracks ****************
    if(StoreTrackInformation_) {
      StoreTrackInformation (iEvent);
    }


    myTree->Fill();



   //##################################
    // Fill all the histograms now

    if(eMinus_ecaliso_10>0.0 && mZee>0.0) 
      fillHist1D("EmECALIsolation10", eMinus_ecaliso_10);
    if(eMinus_ecaliso_20>0.0 && mZee>0.0) 
      fillHist1D("EmECALIsolation20", eMinus_ecaliso_20);
    if(eMinus_ecaliso_30>0.0 && mZee>0.0) 
      fillHist1D("EmECALIsolation30", eMinus_ecaliso_30);
    if(eMinus_ecaliso_40>0.0 && mZee>0.0) 
      fillHist1D("EmECALIsolation40", eMinus_ecaliso_40);
    if(eMinus_ecaliso_50>0.0 && mZee>0.0) 
      fillHist1D("EmECALIsolation50", eMinus_ecaliso_50);
    if(eMinus_hcaliso_10>-1.0 && mZee>0.0) 
      fillHist1D("EmHCALIsolation10", eMinus_hcaliso_10);
    if(eMinus_hcaliso_20>-1.0 && mZee>0.0) 
      fillHist1D("EmHCALIsolation20", eMinus_hcaliso_20);
    if(eMinus_hcaliso_30>-1.0 && mZee>0.0) 
      fillHist1D("EmHCALIsolation30", eMinus_hcaliso_30);
    if(eMinus_hcaliso_40>-1.0 && mZee>0.0) 
      fillHist1D("EmHCALIsolation40", eMinus_hcaliso_40);
    if(eMinus_hcaliso_50>-1.0 && mZee>0.0) 
      fillHist1D("EmHCALIsolation50", eMinus_hcaliso_50);
    if(eMinus_ptisoatecal_15>0.0 && mZee>0.0) 
      fillHist1D("EmTrackPTIsolation10", eMinus_ptisoatecal_15);
    if(eMinus_ptisoatecal_25>0.0 && mZee>0.0) 
      fillHist1D("EmTrackPTIsolation20", eMinus_ptisoatecal_25);
    if(eMinus_ptisoatecal_30>0.0 && mZee>0.0) 
      fillHist1D("EmTrackPTIsolation30", eMinus_ptisoatecal_30);
    if(eMinus_ptisoatecal_40>0.0 && mZee>0.0) 
      fillHist1D("EmTrackPTIsolation40", eMinus_ptisoatecal_40);
    if(eMinus_ptisoatecal_50>0.0 && mZee>0.0) 
      fillHist1D("EmTrackPTIsolation50", eMinus_ptisoatecal_50);
    if(ePlus_ecaliso_10>0.0 && mZee>0.0) 
      fillHist1D("EPECALIsolation10", ePlus_ecaliso_10);
    if(ePlus_ecaliso_20>0.0 && mZee>0.0) 
      fillHist1D("EPECALIsolation20", ePlus_ecaliso_20);
    if(ePlus_ecaliso_30>0.0 && mZee>0.0) 
      fillHist1D("EPECALIsolation30", ePlus_ecaliso_30);
    if(ePlus_ecaliso_40>0.0 && mZee>0.0) 
      fillHist1D("EPECALIsolation40", ePlus_ecaliso_40);
    if(ePlus_ecaliso_50>0.0 && mZee>0.0) 
      fillHist1D("EPECALIsolation50", ePlus_ecaliso_50);
    if(ePlus_hcaliso_10>-1.0 && mZee>0.0) 
      fillHist1D("EPHCALIsolation10", ePlus_hcaliso_10);
    if(ePlus_hcaliso_20>-1.0 && mZee>0.0) 
      fillHist1D("EPHCALIsolation20", ePlus_hcaliso_20);
    if(ePlus_hcaliso_30>-1.0 && mZee>0.0) 
      fillHist1D("EPHCALIsolation30", ePlus_hcaliso_30);
    if(ePlus_hcaliso_40>-1.0 && mZee>0.0) 
      fillHist1D("EPHCALIsolation40", ePlus_hcaliso_40);
    if(ePlus_hcaliso_50>-1.0 && mZee>0.0) 
      fillHist1D("EPHCALIsolation50", ePlus_hcaliso_50);
    if(ePlus_ptisoatecal_15>0.0 && mZee>0.0) 
      fillHist1D("EPTrackPTIsolation10", ePlus_ptisoatecal_15);
    if(ePlus_ptisoatecal_25>0.0 && mZee>0.0) 
      fillHist1D("EPTrackPTIsolation20", ePlus_ptisoatecal_25);
    if(ePlus_ptisoatecal_30>0.0 && mZee>0.0) 
      fillHist1D("EPTrackPTIsolation30", ePlus_ptisoatecal_30);
    if(ePlus_ptisoatecal_40>0.0 && mZee>0.0) 
      fillHist1D("EPTrackPTIsolation40", ePlus_ptisoatecal_40);
    if(ePlus_ptisoatecal_50>0.0 && mZee>0.0) 
      fillHist1D("EPTrackPTIsolation50", ePlus_ptisoatecal_50);
    if(mZee>0.0) {
      fillHist1D("EPTrackNumIsolation10", ePlus_ntrkisoatecal_15);
      fillHist1D("EPTrackNumIsolation20", ePlus_ntrkisoatecal_25);
      fillHist1D("EPTrackNumIsolation30", ePlus_ntrkisoatecal_30);
      fillHist1D("EPTrackNumIsolation40", ePlus_ntrkisoatecal_40);
      fillHist1D("EPTrackNumIsolation50", ePlus_ntrkisoatecal_50);
      fillHist1D("EpBremFraction", ePlus_BremFraction);
      fillHist1D("EmBremFraction", eMinus_BremFraction);
      fillHist1D("EpHoverE", ePlus_HoverE);
      fillHist1D("EmHoverE", eMinus_HoverE);
      fillHist1D("genEPpt", ePlusPtGen);
      fillHist1D("recoEPpt", ePlusPt);
      fillHist1D("genEMpt", eMinusPtGen);
      fillHist1D("recoEMpt", eMinusPt);
      fillHist1D("genZpt", Z_PtGen);
      fillHist1D("recoZpt", Z_Pt);
      fillHist1D("recoZmass", mZee);
      fillHist1D("genZmass", mZeeGen);
      fillHist1D("EmTrackNumIsolation10", eMinus_ntrkisoatecal_15);
      fillHist1D("EmTrackNumIsolation20", eMinus_ntrkisoatecal_25);
      fillHist1D("EmTrackNumIsolation30", eMinus_ntrkisoatecal_30);
      fillHist1D("EmTrackNumIsolation40", eMinus_ntrkisoatecal_40);
      fillHist1D("EmTrackNumIsolation50", eMinus_ntrkisoatecal_50);
    }
    fillHist1D("EmEoverP", eMinus_EoverPin);
    fillHist1D("EpEoverP", ePlus_EoverPin);
    if(ePlusClassification != -1 && mZee>0.0) 
      fillHist1D("EpClassification", ePlusClassification);

    if(fabs(mZee-91.2)<2.5) 
      fillHist1D("genEMptMassCut", eMinusPtGen);
    if(fabs(mZee-91.2)<2.5) 
      fillHist1D("recoEMptMassCut", eMinusPt);
    if(fabs(mZee-91.2)<2.5 && eMinusClassification % 100 == 0) 
      fillHist1D("genEMptGolden", eMinusPtGen);
    if(fabs(mZee-91.2)<2.5 && eMinusClassification % 100 == 10) 
      fillHist1D("genEMptBigbrem", eMinusPtGen);
    if(fabs(mZee-91.2)<2.5 && eMinusClassification % 100 == 20) 
      fillHist1D("genEMptNarrow", eMinusPtGen);
    if(fabs(mZee-91.2)<2.5 && eMinusClassification % 100 > 29 && 
       eMinusClassification % 100 <35) 
      fillHist1D("genEMptShowering", eMinusPtGen); 
    if(fabs(mZee-91.2)<2.5 && eMinusClassification % 100 == 0) 
      fillHist1D("recoEMptGolden", eMinusPt);
    if(fabs(mZee-91.2)<2.5 && eMinusClassification % 100 == 10) 
      fillHist1D("recoEMptBigbrem", eMinusPt);
    if(fabs(mZee-91.2)<2.5 && eMinusClassification % 100 == 20) 
      fillHist1D("recoEMptNarrow", eMinusPt);
    if(fabs(mZee-91.2)<2.5 && eMinusClassification % 100 > 29 && 
       eMinusClassification % 100 <35) 
      fillHist1D("recoEMptShowering", eMinusPt); 


    fillHist1D("Zmass_noIso", mZee);
    if(eMinus_ptisoatecal_15 < 0.2 && ePlus_ptisoatecal_15 < 0.2) 
      fillHist1D("Zmass_TkIso", mZee);
    if(eMinus_ptisoatecal_15 < 0.2 && 
       ePlus_ptisoatecal_15 < 0.2 && 
       eMinus_ecaliso_20<0.2 && ePlus_ecaliso_20<0.2) 
      fillHist1D("Zmass_TkECALIso", mZee);
    if(eMinus_ptisoatecal_15 < 0.2 && 
       ePlus_ptisoatecal_15 < 0.2 && 
       eMinus_ecaliso_20<0.2 && ePlus_ecaliso_20<0.2 &&
       fabs(eMinus_hcaliso_30)<0.1 && 
       fabs(ePlus_hcaliso_30)<0.1) 
      fillHist1D("Zmass_TkECALHCALIso", mZee);
    if(isePlusTightPTDR & iseMinusTightPTDR) 
      fillHist1D("Zmass_ptdrtight", mZee);
    if(isePlusMediumPTDR & iseMinusMediumPTDR) 
      fillHist1D("Zmass_ptdrmedium", mZee);
    if(isePlusLoosePTDR & iseMinusLoosePTDR) 
      fillHist1D("Zmass_ptdrloose", mZee);


    if(ePlusClassification>-1 && ePlusClassification<99 && 
       eMinusClassification>-1 && eMinusClassification<99) 
      fillHist1D("Zmass_BB", mZee);
    if((ePlusClassification>-1 && ePlusClassification<99 && 
	eMinusClassification>99) || (eMinusClassification>-1 && 
				     eMinusClassification<99 && 
				     ePlusClassification>99)) 
      fillHist1D("Zmass_BE", mZee);
    if(ePlusClassification>99 && eMinusClassification>99) 
      fillHist1D("Zmass_EE", mZee);
    if(ePlusClassification % 100 == 0 && 
       eMinusClassification % 100 == 0) 
      fillHist1D("Zmass_golden",  mZee );
    if(ePlusClassification % 100 == 10 && 
       eMinusClassification % 100 == 10) 
      fillHist1D("Zmass_bigbrem",  mZee ); 
    if(ePlusClassification % 100 == 20 && 
       eMinusClassification % 100 == 20) 
      fillHist1D("Zmass_narrow",  mZee );
    if(ePlusClassification %100 > 29 && ePlusClassification %100 < 35 && 
       eMinusClassification % 100 > 29 && eMinusClassification % 100 < 35) 
      fillHist1D("Zmass_shower",  mZee );
    if(fabs(mZee-91.2)<2.5 && ePlusPt>20.0 && eMinusPt>20.0) 
      fillHist1D("genZptm", Z_PtGen);
    if(fabs(mZee-91.2)<2.5 && ePlusPt>20.0 && eMinusPt>20.0) 
      fillHist1D("recoZptm", Z_Pt);

  } // analyze method






  void ZJetAnalysis::endJob()
  {

    // cout << "===== Start writing user histograms =====" << endl;
    hOutputFile->SetCompressionLevel(2);
    //  hOutputFile->cd();
    //  myTree->Write();
    hOutputFile->Write();
    delete myTree;
    for(std::map<TString, TH1*>::iterator 
	  iter = m_HistNames1D.begin(); 
	iter != m_HistNames1D.end(); ++iter) {
      m_HistNames1D.erase(iter);
    }

    hOutputFile->Close() ;
    delete hOutputFile;

    //  cout << "===== End writing user histograms =======" << endl; 
  }






  //
  // member functions
  //

  // ***************** Load Calo Jets ************************* 

  void  ZJetAnalysis::StoreCaloJetInformation (const edm::Event& iEvent) {

    /****************    Jet Flavor    ***************/
    edm::Handle<JetFlavourMatchingCollection> jetMC;
    if (flavourMatchOptionf == "fastMC") {
      // initialize flavour identifiers
      iEvent.getByLabel(flavourSourcef, jetMC);
      for(JetFlavourMatchingCollection::const_iterator iter =
	    jetMC->begin(); iter != jetMC->end(); iter++)
	flavoursMapf.insert(*iter);
    } 
    else if (flavourMatchOptionf == "hepMC") {
      jfi.readEvent(iEvent);    
    } 
    else if (flavourMatchOptionf == "genParticle") {
      iEvent.getByLabel (flavourSourcef, theJetPartonMapf);
    }
    /****************    Jet Flavor    ***************/


    std::vector<edm::InputTag>::const_iterator ic;
    int jettype = 0;
    NumRecoJets = 0; 
    NumRecoJetAlgo = 0;

    for (ic=mInputCalo.begin(); ic!=mInputCalo.end(); ic++) {
      edm::Handle<reco::CaloJetCollection> jets;
      iEvent.getByLabel(*ic, jets);
      if (!jets.isValid()) {
	// can't find it!
	if (!allowMissingInputs_) {
	  *jets;  // will throw the proper exception
	}
      } else {
	reco::CaloJetCollection::const_iterator jet = jets->begin ();

	if(jets->size() < 1) continue; 
	int ij = 0;
	for (; jet != jets->end (); jet++) {
	  if( !(ij < NUM_JET_MAX) ) continue; 

	  JetRecoEt[jettype][ij] = (*jet).et();
	  JetRecoPt[jettype][ij] = (*jet).pt();
	  JetRecoEta[jettype][ij] = (*jet).eta();
	  JetRecoPhi[jettype][ij] = (*jet).phi();

	  JetRecoMaxEInEmTowers[jettype][ij] = (*jet).maxEInEmTowers();
	  JetRecoMaxEInHadTowers[jettype][ij] = (*jet).maxEInHadTowers();
	  JetRecoEnergyFractionHadronic[jettype][ij] 
	    = (*jet).energyFractionHadronic();
	  JetRecoEmEnergyFraction[jettype][ij] 
	    = (*jet).emEnergyFraction();
	  JetRecoHadEnergyInHB[jettype][ij] = (*jet).hadEnergyInHB();
	  JetRecoHadEnergyInHO[jettype][ij] = (*jet).hadEnergyInHO();
	  JetRecoHadEnergyInHE[jettype][ij] = (*jet).hadEnergyInHE();
	  JetRecoHadEnergyInHF[jettype][ij] = (*jet).hadEnergyInHF();
	  JetRecoEmEnergyInEB[jettype][ij] = (*jet).emEnergyInEB();
	  JetRecoEmEnergyInEE[jettype][ij] = (*jet).emEnergyInEE();
	  JetRecoEmEnergyInHF[jettype][ij] = (*jet).emEnergyInHF();
	  JetRecoTowersArea[jettype][ij] = (*jet).towersArea();
	  JetRecoN90[jettype][ij] = (*jet).n90();
	  JetRecoN60[jettype][ij] = (*jet).n60();
	  JetRecoType[jettype][ij] = jettype;
	  JetRecoFlavor[jettype][ij] = getMatchedParton(*jet).flavour();
	  ij++;	    
	}
	NumRecoJets = ij;
        jettype++;
      }
    }
    NumRecoJetAlgo = jettype;
  }
  // --------------------------------------------------  


 // ***************** Load Corrected Jets ************************* 

  void  ZJetAnalysis::StoreCorJetInformation (const edm::Event& iEvent) {

    std::vector<edm::InputTag>::const_iterator ic;
    int jettype = 0;

    for (ic=mInputCor.begin(); ic!=mInputCor.end(); ic++) {
      edm::Handle<reco::CaloJetCollection> jets;
      iEvent.getByLabel(*ic, jets);
      if (!jets.isValid()) {
	// can't find it!
	if (!allowMissingInputs_) {
	  *jets;  // will throw the proper exception
	}
      } else {
	reco::CaloJetCollection::const_iterator jet = jets->begin ();

	if(jets->size() < 1) continue; 
	int ij = 0;
	for (; jet != jets->end (); jet++) {
	  if( !(ij < NUM_JET_MAX) ) continue; 

	  JetCorEt[jettype][ij] = (*jet).et();
	  JetCorPt[jettype][ij] = (*jet).pt();
	  JetCorEta[jettype][ij] = (*jet).eta();
	  JetCorPhi[jettype][ij] = (*jet).phi();

	  JetCorEnergyFractionHadronic[jettype][ij] 
	    = (*jet).energyFractionHadronic();
	  JetCorEmEnergyFraction[jettype][ij] 
	    = (*jet).emEnergyFraction();
	  ij++;	    
	}
        jettype++;
      }
    }
  }
  // --------------------------------------------------  




  // *********** Load Gen Jets ******************

  void  ZJetAnalysis::StoreGenJetInformation (const edm::Event& iEvent) {
    std::vector<edm::InputTag>::const_iterator ic;
    int jettype = 0;
    NumGenJets = 0;
    NumGenJetAlgo = 0;

    for (ic=mInputGen.begin(); ic!=mInputGen.end(); ic++) {
      edm::Handle<reco::GenJetCollection> jets;
      iEvent.getByLabel(*ic, jets);
      if (!jets.isValid()) {
	// can't find it!
	if (!allowMissingInputs_) {
	  *jets;  // will throw the proper exception
	}
      } else {
	reco::GenJetCollection::const_iterator jet = jets->begin ();

	if(jets->size() < 1) continue;
	int ij = 0;
	for (; jet != jets->end (); jet++) {
	  if( !(ij < NUM_JET_MAX) ) continue;    
	  JetGenEt[jettype][ij] = (*jet).et();
	  JetGenPt[jettype][ij] = (*jet).pt();
	  JetGenEta[jettype][ij] = (*jet).eta();
	  JetGenPhi[jettype][ij] = (*jet).phi();
	  JetGenType[jettype][ij] = jettype;
	  JetGenEmEnergy[jettype][ij] = (*jet).emEnergy();
	  JetGenHadEnergy[jettype][ij] = (*jet).hadEnergy();
	  JetGenInvisibleEnergy[jettype][ij] = (*jet).invisibleEnergy();
	  JetGenAuxiliaryEnergy[jettype][ij] = (*jet).auxiliaryEnergy();
	  ij++;
	}
	NumGenJets = ij;
	
	jettype++;	     
      }
    }
    NumGenJetAlgo = jettype;
  }
  // --------------------------------------------------  





    /* ******************************************************** */
    /**  Store Reconstructed    Z--> e+e-    information ****** */
    /* ******************************************************** */

  void  ZJetAnalysis::StoreRecoZeeInformation (const edm::Event& iEvent) {

    Handle<reco::CandidateCollection> zee;
    iEvent.getByLabel(zee_, zee);
    nZee = zee->size();
    if( nZee < 1 ) return;


    const Candidate *ZeeReco(0);
    const Candidate *eplusReco(0);
    const Candidate *eminusReco(0);

    for(size_t i = 0; i < zee->size(); ++ i) {
      ZeeReco = &((*zee)[i]);
     
      size_t nZdau = 0;
      if(!(ZeeReco==0)) nZdau = ZeeReco->numberOfDaughters();  
      if(nZdau>1) {
	for(size_t j = 0; j < nZdau; ++ j) {
	  const Candidate *d = ZeeReco->daughter( j );
	  if(!(d==0)) {
	    double dcharge = d->charge();
	    if (dcharge>0.0) eplusReco  = d;
	    if (dcharge<0.0) eminusReco = d;
	  }
	}
      }

      if( eplusReco != 0 && eminusReco != 0 ) { 
     
	////////// e+ quantities //////////////
	reco::SuperClusterRef SCp 
	  = eplusReco->get<SuperClusterRef>();
	ePlusCharge =eplusReco-> charge();
	ePlusVx = eplusReco->vx();
	ePlusVy = eplusReco->vy();
	ePlusVz = eplusReco->vz();

	float theta = SCp->position().Theta();
	ePlusEta = SCp->eta();    
	ePlusPhi = SCp->phi();
	ePlusE   = SCp->energy();
	ePluspx    = ePlusE * sin(theta) * cos(ePlusPhi);
	ePluspy    = ePlusE * sin(theta) * sin(ePlusPhi);
	ePluspz    = ePlusE * cos(theta);
	ePlusPt    = ePlusE / cosh(ePlusEta);
	ePlusEt    = ePlusPt;


	////////// e- quantities //////////////
	reco::SuperClusterRef SCm 
	  = eminusReco->get<SuperClusterRef>();
	eMinusCharge = eminusReco->charge();
	eMinusVx = eminusReco->vx();
	eMinusVy = eminusReco->vy();
	eMinusVz = eminusReco->vz();

	theta = SCm->position().Theta();
	eMinusEta = SCm->eta();    
	eMinusPhi = SCm->phi();
	eMinusE   = SCm->energy();
	eMinuspx    = eMinusE * sin(theta) * cos(eMinusPhi);
	eMinuspy    = eMinusE * sin(theta) * sin(eMinusPhi);
	eMinuspz    = eMinusE * cos(theta);
	eMinusPt    = eMinusE / cosh(eMinusEta);
	eMinusEt    = eMinusPt;

	////////// Z quantities //////////////
	mZee = ZeeReco->mass();
	Z_Eta = ZeeReco->eta();   
	Z_Phi = ZeeReco->phi();
	Z_Charge = ZeeReco->charge();
	Z_Vx = ZeeReco->vx();
	Z_Vy = ZeeReco->vy();
	Z_Vz = ZeeReco->vz();

	float Zp = sqrt( pow(ePlusE + eMinusE, 2) - pow(mZee,2) );
	theta = ZeeReco->theta();
	Z_px = Zp * sin(theta) * cos(Z_Phi);
	Z_py = Zp * sin(theta) * sin(Z_Phi);
	Z_pz = Zp * cos(theta);
	Z_E  = ePlusE + eMinusE;
	Z_Pt = Zp / cosh(Z_Eta);
	Z_Et = Z_E / cosh(Z_Eta);
      }
    }


    //get reco electron collection
    Handle<reco::GsfElectronCollection> electron;
    iEvent.getByLabel("pixelMatchGsfElectrons", electron);
    
    //get Barrel track collection from GsfTracks
    Handle<GsfTrackCollection> gsf_trks_B;
    iEvent.getByLabel("pixelMatchGsfFitBarrel",gsf_trks_B);
    const GsfTrackCollection * gsf_B_TracksData;
    gsf_B_TracksData = gsf_trks_B.product();
    
    //get Endcap track collection from GsfTracks
    Handle<GsfTrackCollection> gsf_trks_E;
    iEvent.getByLabel("pixelMatchGsfFitEndcap",gsf_trks_E);
    const GsfTrackCollection * gsf_E_TracksData;
    gsf_E_TracksData = gsf_trks_E.product();
  


    size_t eSize = (const int) electron->size();
    if(eSize<1 || eSize>19) return;
    float sc_e[20];


    for(size_t j = 0; j < eSize; ++ j) {
      const reco::PixelMatchGsfElectron&  x = (*electron)[j];
      

      // Remove the duplicate Electrons - Twins
      sc_e[j] = x.superCluster()->energy();
      bool test = false;
      if(j>0) {
	for(unsigned int twins = 0; twins < j; twins++) {	    
	  if(sc_e[twins] == sc_e[j]) test = true;
	}
      }
      if(test) continue;



      // get electron ID info
      int classfn = x.classification();
      bool isLoosePTDR  = isElectronId( iEvent, x, 0, "loose");
      bool isMediumPTDR = isElectronId( iEvent, x, 0, "medium");
      bool isTightPTDR  = isElectronId( iEvent, x, 0, "tight");
      bool isLooseCutBased  = isElectronId( iEvent, x, 1, "loose");
      bool isRobustCutBased = isElectronId( iEvent, x, 1, "robust");
      bool isTightCutBased  = isElectronId( iEvent, x, 1, "tight");



      //get SuperCluster (sc) infos
      float sc_x = x.superCluster()->x();
      float sc_y = x.superCluster()->y();
      float sc_z = x.superCluster()->z();
      float sc_R   = sqrt(sc_x*sc_x + sc_y*sc_y + sc_z*sc_z);
      float sc_Rt  = sqrt(sc_x*sc_x + sc_y*sc_y);

      float sc_eta        = x.superCluster()->eta();
      float sc_phi        = x.superCluster()->phi();
      float sc_theta      = x.superCluster()->position().Theta();
      float sc_et         = x.superCluster()->energy()*(sc_Rt/sc_R);
      float sc_e          = x.superCluster()->energy();
      int sc_no           = x.numberOfClusters();
      float EoverPout     = x.eSeedClusterOverPout();
      float EoverPin      = x.eSuperClusterOverP();
      float InvEMinusInvP = 0.0;
      if(!(x.caloEnergy()==0 || x.trackMomentumAtVtx().R()==0))	    
	InvEMinusInvP = ((1./x.caloEnergy()) - 
			 (1./x.trackMomentumAtVtx().R()));
	  
      float BremFraction  = (x.trackMomentumAtVtx().R() - 
			     x.trackMomentumOut().R());
      float DeltaEtaIn    = x.deltaEtaSuperClusterTrackAtVtx();
      float DeltaPhiIn    = x.deltaPhiSuperClusterTrackAtVtx();
      float DeltaPhiOut   = x.deltaPhiSeedClusterTrackAtCalo();
      float DeltaEtaOut   = x.deltaEtaSeedClusterTrackAtCalo();



	  //Track Momentum information
      float Trackmom_calo = sqrt(x.trackMomentumAtCalo().perp2());
      float Trackmom_vtx  = sqrt(x.trackMomentumAtVtx().perp2());
	  
	  //get Hcal energy over Ecal Energy
      float HoverE = x.hadronicOverEm();
	  
	  //vertex info for electron
      float vx = x.vertex().x();
      float vy = x.vertex().y();
      float vz = x.vertex().z();
	  

	  //Transvers Impact Parameter
      float td0 = sqrt(vx*vx + vy*vy);
      float d0  = sqrt(vx*vx + vy*vy + vz*vz);
	 
 
	  //get ClusterShape Information
      const reco::ClusterShapeRef& shapeRef = getClusterShape(&x, iEvent);  
      float E9overE25   = shapeRef->e3x3()/shapeRef->e5x5();
      float SigmaEtaEta = shapeRef->covEtaEta();
      float SigmaPhiPhi = shapeRef->covPhiPhi();
	  
	  
      //helpvariable
      float tmpTrackDeltaR = 999.9;
      
      //get track informations
      float trk_eta    = -99999.;
      float trk_phi    = -99999.;
      float trk_etaout = -99999.;
      float trk_phiout = -99999.;
      float trk_p      = -99999.;
      float trk_pout   = -99999.;
      float trk_pt     = -99999.;
      int   trk_charge = -99999;
      float trk_chi2   = -99999.;
      float trk_ndof   = -99999.;
      int   trk_hits   = -1;
      float trk_d0     = -99999.;
      float trk_d0Err  = -99999.;


      GsfTrackCollection::const_iterator track_B;
      for(track_B = gsf_B_TracksData->begin(); 
	  track_B != gsf_B_TracksData->end(); ++track_B) {
	float trk_eta_tmp = track_B->eta();
	float trk_phi_tmp = track_B->phi();
	float deltaR = radius(trk_eta_tmp,trk_phi_tmp,x.eta(),x.phi());
	
	if( deltaR < 0.15 && deltaR<tmpTrackDeltaR) {
	  trk_eta    = track_B->eta();
	  trk_phi    = track_B->phi();
	  trk_etaout = track_B->outerEta();
	  trk_phiout = track_B->outerPhi();
	  trk_p      = track_B->p();
	  trk_pout   = track_B->outerP();
	  trk_pt     = track_B->pt();
	  trk_charge = track_B->charge();
	  trk_chi2   = track_B->chi2();
	  trk_ndof   = track_B->ndof();
	  trk_hits   = track_B->numberOfValidHits();
	  trk_d0     = track_B->d0();
	  trk_d0Err  = track_B->d0Error();
	    
	  tmpTrackDeltaR = deltaR;
	}
      }
      
      //get endcap tracks
      GsfTrackCollection::const_iterator track_E;
      for(track_E = gsf_E_TracksData->begin();
	  track_E != gsf_E_TracksData->end(); ++track_E) {
	float trk_eta_tmp = track_E->eta();
	float trk_phi_tmp = track_E->phi();
	float deltaR = radius(trk_eta_tmp,trk_phi_tmp,x.eta(),x.phi());
	
	if( deltaR < 0.15 && deltaR<tmpTrackDeltaR) {
	  trk_eta    = track_E->eta();
	  trk_phi    = track_E->phi();
	  trk_etaout = track_E->outerEta();
	  trk_phiout = track_E->outerPhi();
	  trk_p      = track_E->p();
	  trk_pout   = track_E->outerP();
	  trk_pt     = track_E->pt();
	  trk_charge = track_E->charge();
	  trk_chi2   = track_E->chi2();
	  trk_ndof   = track_E->ndof();
	  trk_hits   = track_E->numberOfValidHits();
	  trk_d0     = track_E->d0();
	  trk_d0Err  = track_E->d0Error();
	    
	  tmpTrackDeltaR = deltaR;
	}
      }
	  


    //////////////////////////////////////////
    //////////    ISOLATION      ////////////
    //////////////////////////////////////////
    float ptiso_15 = -1.0; 
    float ptiso_20 = -1.0; 
    float ptiso_25 = -1.0;
    float ptiso_30 = -1.0;
    float ptiso_35 = -1.0;
    float ptiso_40 = -1.0;
    float ptiso_45 = -1.0;
    float ptiso_50 = -1.0;

    float ptisoatecal_15 = -1.0;
    float ptisoatecal_20 = -1.0;
    float ptisoatecal_25 = -1.0;
    float ptisoatecal_30 = -1.0;
    float ptisoatecal_35 = -1.0;
    float ptisoatecal_40 = -1.0;
    float ptisoatecal_45 = -1.0;
    float ptisoatecal_50 = -1.0;

    int ntrkiso_15 = -1;
    int ntrkiso_20 = -1;
    int ntrkiso_25 = -1;
    int ntrkiso_30 = -1;
    int ntrkiso_35 = -1;
    int ntrkiso_40 = -1;
    int ntrkiso_45 = -1;
    int ntrkiso_50 = -1;
     
    int ntrkisoatecal_15 = -1;
    int ntrkisoatecal_20 = -1;
    int ntrkisoatecal_25 = -1;
    int ntrkisoatecal_30 = -1;
    int ntrkisoatecal_35 = -1;
    int ntrkisoatecal_40 = -1;
    int ntrkisoatecal_45 = -1;
    int ntrkisoatecal_50 = -1;

    float hcaliso_10 = -1.0;
    float hcaliso_15 = -1.0;
    float hcaliso_20 = -1.0;
    float hcaliso_25 = -1.0;
    float hcaliso_30 = -1.0;
    float hcaliso_35 = -1.0;
    float hcaliso_40 = -1.0;
    float hcaliso_45 = -1.0;
    float hcaliso_50 = -1.0;

    float ecaliso_10 = -1.0;
    float ecaliso_15 = -1.0;
    float ecaliso_20 = -1.0;
    float ecaliso_25 = -1.0;
    float ecaliso_30 = -1.0;
    float ecaliso_35 = -1.0;
    float ecaliso_40 = -1.0;
    float ecaliso_45 = -1.0;
    float ecaliso_50 = -1.0;
    ///////////////////
    float dz = x.gsfTrack()->dz();
    TrackIsolation( iEvent, false, sc_eta, sc_phi, dz, 0.15, ptiso_15, ntrkiso_15 );
    TrackIsolation( iEvent, false, sc_eta, sc_phi, dz, 0.20, ptiso_20, ntrkiso_20 );
    TrackIsolation( iEvent, false, sc_eta, sc_phi, dz, 0.25, ptiso_25, ntrkiso_25 );
    TrackIsolation( iEvent, false, sc_eta, sc_phi, dz, 0.30, ptiso_30, ntrkiso_30 );
    TrackIsolation( iEvent, false, sc_eta, sc_phi, dz, 0.35, ptiso_35, ntrkiso_35 );
    TrackIsolation( iEvent, false, sc_eta, sc_phi, dz, 0.40, ptiso_40, ntrkiso_40 );
    TrackIsolation( iEvent, false, sc_eta, sc_phi, dz, 0.45, ptiso_45, ntrkiso_45 );
    TrackIsolation( iEvent, false, sc_eta, sc_phi, dz, 0.50, ptiso_50, ntrkiso_50 );
    TrackIsolation( iEvent, true,  sc_eta, sc_phi, dz, 0.15, ptisoatecal_15, ntrkisoatecal_15 );
    TrackIsolation( iEvent, true,  sc_eta, sc_phi, dz, 0.20, ptisoatecal_20, ntrkisoatecal_20 );
    TrackIsolation( iEvent, true,  sc_eta, sc_phi, dz, 0.25, ptisoatecal_25, ntrkisoatecal_25 );
    TrackIsolation( iEvent, true,  sc_eta, sc_phi, dz, 0.30, ptisoatecal_30, ntrkisoatecal_30 );
    TrackIsolation( iEvent, true,  sc_eta, sc_phi, dz, 0.35, ptisoatecal_35, ntrkisoatecal_35 );
    TrackIsolation( iEvent, true,  sc_eta, sc_phi, dz, 0.40, ptisoatecal_40, ntrkisoatecal_40 );
    TrackIsolation( iEvent, true,  sc_eta, sc_phi, dz, 0.45, ptisoatecal_45, ntrkisoatecal_45 );
    TrackIsolation( iEvent, true,  sc_eta, sc_phi, dz, 0.50, ptisoatecal_50, ntrkisoatecal_50 );
    
    if(sc_e != 0.0) {
      hcaliso_10 = HCALIsolation(iEvent, sc_eta, sc_phi, 0.10) / sc_e;
      hcaliso_15 = HCALIsolation(iEvent, sc_eta, sc_phi, 0.15) / sc_e;
      hcaliso_20 = HCALIsolation(iEvent, sc_eta, sc_phi, 0.20) / sc_e;
      hcaliso_25 = HCALIsolation(iEvent, sc_eta, sc_phi, 0.25) / sc_e;
      hcaliso_30 = HCALIsolation(iEvent, sc_eta, sc_phi, 0.30) / sc_e;
      hcaliso_35 = HCALIsolation(iEvent, sc_eta, sc_phi, 0.35) / sc_e;
      hcaliso_40 = HCALIsolation(iEvent, sc_eta, sc_phi, 0.40) / sc_e;
      hcaliso_45 = HCALIsolation(iEvent, sc_eta, sc_phi, 0.45) / sc_e;
      hcaliso_50 = HCALIsolation(iEvent, sc_eta, sc_phi, 0.50) / sc_e;

      ecaliso_10 = ( ECALIsolation(iEvent, sc_eta, sc_phi, 0.10) - sc_e ) / sc_e;
      ecaliso_15 = ( ECALIsolation(iEvent, sc_eta, sc_phi, 0.15) - sc_e ) / sc_e;
      ecaliso_20 = ( ECALIsolation(iEvent, sc_eta, sc_phi, 0.20) - sc_e ) / sc_e;
      ecaliso_25 = ( ECALIsolation(iEvent, sc_eta, sc_phi, 0.25) - sc_e ) / sc_e;
      ecaliso_30 = ( ECALIsolation(iEvent, sc_eta, sc_phi, 0.30) - sc_e ) / sc_e;
      ecaliso_35 = ( ECALIsolation(iEvent, sc_eta, sc_phi, 0.35) - sc_e ) / sc_e;
      ecaliso_40 = ( ECALIsolation(iEvent, sc_eta, sc_phi, 0.40) - sc_e ) / sc_e;
      ecaliso_45 = ( ECALIsolation(iEvent, sc_eta, sc_phi, 0.45) - sc_e ) / sc_e;
      ecaliso_50 = ( ECALIsolation(iEvent, sc_eta, sc_phi, 0.50) - sc_e ) / sc_e;
    }

      if( overlap( x,*eplusReco) ) { 
	ePlusClassification = classfn;
	isePlusLoosePTDR      = isLoosePTDR;
	isePlusMediumPTDR     = isMediumPTDR;
	isePlusTightPTDR      = isTightPTDR;
	isePlusLooseCutBased  = isLooseCutBased;
	isePlusRobustCutBased = isRobustCutBased;
	isePlusTightCutBased  = isTightCutBased;

	ePlus_sc_x            = sc_x;
	ePlus_sc_y            = sc_y;
	ePlus_sc_z            = sc_z;
	ePlus_sc_R            = sc_R;
	ePlus_sc_Rt           = sc_Rt;
	ePlus_sc_eta          = sc_eta;
	ePlus_sc_phi          = sc_phi;
	ePlus_sc_theta        = sc_theta;
	ePlus_sc_et           = sc_et;
	ePlus_sc_e            = sc_e;
	ePlus_sc_no           = sc_no;
	ePlus_EoverPout       = EoverPout;
	ePlus_EoverPin        = EoverPin;
	ePlus_InvEMinusInvP   = InvEMinusInvP;
	ePlus_BremFraction    = BremFraction;
	ePlus_DeltaEtaIn      = DeltaEtaIn;
	ePlus_DeltaPhiIn      = DeltaPhiIn;
	ePlus_DeltaPhiOut     = DeltaPhiOut;
	ePlus_DeltaEtaOut     = DeltaEtaOut;
	ePlus_Trackmom_calo   = Trackmom_calo;
	ePlus_Trackmom_vtx    = Trackmom_vtx;	  
	ePlus_HoverE          = HoverE;	  
	ePlus_vx              = vx;
	ePlus_vy              = vy;
	ePlus_vz              = vz;	  
	ePlus_td0             = td0;
	ePlus_d0              = d0;	 	  
	ePlus_E9overE25       = E9overE25;
	ePlus_SigmaEtaEta     = SigmaEtaEta;
	ePlus_SigmaPhiPhi     = SigmaPhiPhi;	  
	ePlus_trk_eta         = trk_eta;
	ePlus_trk_phi         = trk_phi;
	ePlus_trk_etaout      = trk_etaout;
	ePlus_trk_phiout      = trk_phiout;
	ePlus_trk_p           = trk_p;
	ePlus_trk_pout        = trk_pout;
	ePlus_trk_pt          = trk_pt;
	ePlus_trk_charge      = trk_charge;
	ePlus_trk_chi2        = trk_chi2;
	ePlus_trk_ndof        = trk_ndof;
	ePlus_trk_hits        = trk_hits;
	ePlus_trk_d0          = trk_d0;
	ePlus_trk_d0Err       = trk_d0Err;

	//////////////////////
	ePlus_ptiso_15 = ptiso_15/sc_et; 
	ePlus_ptiso_20 = ptiso_20/sc_et; 
	ePlus_ptiso_25 = ptiso_25/sc_et;
	ePlus_ptiso_30 = ptiso_30/sc_et;
	ePlus_ptiso_35 = ptiso_35/sc_et;
	ePlus_ptiso_40 = ptiso_40/sc_et;
	ePlus_ptiso_45 = ptiso_45/sc_et;
	ePlus_ptiso_50 = ptiso_50/sc_et;

	ePlus_ptisoatecal_15 = ptisoatecal_15/sc_et;
	ePlus_ptisoatecal_20 = ptisoatecal_20/sc_et;
	ePlus_ptisoatecal_25 = ptisoatecal_25/sc_et;
	ePlus_ptisoatecal_30 = ptisoatecal_30/sc_et;
	ePlus_ptisoatecal_35 = ptisoatecal_35/sc_et;
	ePlus_ptisoatecal_40 = ptisoatecal_40/sc_et;
	ePlus_ptisoatecal_45 = ptisoatecal_45/sc_et;
	ePlus_ptisoatecal_50 = ptisoatecal_50/sc_et;

	ePlus_ntrkiso_15 = ntrkiso_15;
	ePlus_ntrkiso_20 = ntrkiso_20;
	ePlus_ntrkiso_25 = ntrkiso_25;
	ePlus_ntrkiso_30 = ntrkiso_30;
	ePlus_ntrkiso_35 = ntrkiso_35;
	ePlus_ntrkiso_40 = ntrkiso_40;
	ePlus_ntrkiso_45 = ntrkiso_45;
	ePlus_ntrkiso_50 = ntrkiso_50;
     
	ePlus_ntrkisoatecal_15 = ntrkisoatecal_15;
	ePlus_ntrkisoatecal_20 = ntrkisoatecal_20;
	ePlus_ntrkisoatecal_25 = ntrkisoatecal_25;
	ePlus_ntrkisoatecal_30 = ntrkisoatecal_30;
	ePlus_ntrkisoatecal_35 = ntrkisoatecal_35;
	ePlus_ntrkisoatecal_40 = ntrkisoatecal_40;
	ePlus_ntrkisoatecal_45 = ntrkisoatecal_45;
	ePlus_ntrkisoatecal_50 = ntrkisoatecal_50;

	ePlus_hcaliso_10 = hcaliso_10;
	ePlus_hcaliso_15 = hcaliso_15;
	ePlus_hcaliso_20 = hcaliso_20;
	ePlus_hcaliso_25 = hcaliso_25;
	ePlus_hcaliso_30 = hcaliso_30;
	ePlus_hcaliso_35 = hcaliso_35;
	ePlus_hcaliso_40 = hcaliso_40;
	ePlus_hcaliso_45 = hcaliso_45;
	ePlus_hcaliso_50 = hcaliso_50;

	ePlus_ecaliso_10 = ecaliso_10;
	ePlus_ecaliso_15 = ecaliso_15;
	ePlus_ecaliso_20 = ecaliso_20;
	ePlus_ecaliso_25 = ecaliso_25;
	ePlus_ecaliso_30 = ecaliso_30;
	ePlus_ecaliso_35 = ecaliso_35;
	ePlus_ecaliso_40 = ecaliso_40;
	ePlus_ecaliso_45 = ecaliso_45;
	ePlus_ecaliso_50 = ecaliso_50;	  
      }


	  
      if( overlap( x,*eminusReco) ) { 
	eMinusClassification = classfn;
	iseMinusLoosePTDR      = isLoosePTDR;
	iseMinusMediumPTDR     = isMediumPTDR;
	iseMinusTightPTDR      = isTightPTDR;
	iseMinusLooseCutBased  = isLooseCutBased;
	iseMinusRobustCutBased = isRobustCutBased;
	iseMinusTightCutBased  = isTightCutBased;

	eMinus_sc_x            = sc_x;
	eMinus_sc_y            = sc_y;
	eMinus_sc_z            = sc_z;
	eMinus_sc_R            = sc_R;
	eMinus_sc_Rt           = sc_Rt;
	eMinus_sc_eta          = sc_eta;
	eMinus_sc_phi          = sc_phi;
	eMinus_sc_theta        = sc_theta;
	eMinus_sc_et           = sc_et;
	eMinus_sc_e            = sc_e;
	eMinus_sc_no           = sc_no;
	eMinus_EoverPout       = EoverPout;
	eMinus_EoverPin        = EoverPin;
	eMinus_InvEMinusInvP   = InvEMinusInvP;
	eMinus_BremFraction    = BremFraction;
	eMinus_DeltaEtaIn      = DeltaEtaIn;
	eMinus_DeltaPhiIn      = DeltaPhiIn;
	eMinus_DeltaPhiOut     = DeltaPhiOut;
	eMinus_DeltaEtaOut     = DeltaEtaOut;
	eMinus_Trackmom_calo   = Trackmom_calo;
	eMinus_Trackmom_vtx    = Trackmom_vtx;	  
	eMinus_HoverE          = HoverE;	  
	eMinus_vx              = vx;
	eMinus_vy              = vy;
	eMinus_vz              = vz;	  
	eMinus_td0             = td0;
	eMinus_d0              = d0;	 	  
	eMinus_E9overE25       = E9overE25;
	eMinus_SigmaEtaEta     = SigmaEtaEta;
	eMinus_SigmaPhiPhi     = SigmaPhiPhi;	  
	eMinus_trk_eta         = trk_eta;
	eMinus_trk_phi         = trk_phi;
	eMinus_trk_etaout      = trk_etaout;
	eMinus_trk_phiout      = trk_phiout;
	eMinus_trk_p           = trk_p;
	eMinus_trk_pout        = trk_pout;
	eMinus_trk_pt          = trk_pt;
	eMinus_trk_charge      = trk_charge;
	eMinus_trk_chi2        = trk_chi2;
	eMinus_trk_ndof        = trk_ndof;
	eMinus_trk_hits        = trk_hits;
	eMinus_trk_d0          = trk_d0;
	eMinus_trk_d0Err       = trk_d0Err;

	//////////////////////
	eMinus_ptiso_15 = ptiso_15/sc_et; 
	eMinus_ptiso_20 = ptiso_20/sc_et; 
	eMinus_ptiso_25 = ptiso_25/sc_et;
	eMinus_ptiso_30 = ptiso_30/sc_et;
	eMinus_ptiso_35 = ptiso_35/sc_et;
	eMinus_ptiso_40 = ptiso_40/sc_et;
	eMinus_ptiso_45 = ptiso_45/sc_et;
	eMinus_ptiso_50 = ptiso_50/sc_et;

	eMinus_ptisoatecal_15 = ptisoatecal_15/sc_et;
	eMinus_ptisoatecal_20 = ptisoatecal_20/sc_et;
	eMinus_ptisoatecal_25 = ptisoatecal_25/sc_et;
	eMinus_ptisoatecal_30 = ptisoatecal_30/sc_et;
	eMinus_ptisoatecal_35 = ptisoatecal_35/sc_et;
	eMinus_ptisoatecal_40 = ptisoatecal_40/sc_et;
	eMinus_ptisoatecal_45 = ptisoatecal_45/sc_et;
	eMinus_ptisoatecal_50 = ptisoatecal_50/sc_et;

	eMinus_ntrkiso_15 = ntrkiso_15;
	eMinus_ntrkiso_20 = ntrkiso_20;
	eMinus_ntrkiso_25 = ntrkiso_25;
	eMinus_ntrkiso_30 = ntrkiso_30;
	eMinus_ntrkiso_35 = ntrkiso_35;
	eMinus_ntrkiso_40 = ntrkiso_40;
	eMinus_ntrkiso_45 = ntrkiso_45;
	eMinus_ntrkiso_50 = ntrkiso_50;
     
	eMinus_ntrkisoatecal_15 = ntrkisoatecal_15;
	eMinus_ntrkisoatecal_20 = ntrkisoatecal_20;
	eMinus_ntrkisoatecal_25 = ntrkisoatecal_25;
	eMinus_ntrkisoatecal_30 = ntrkisoatecal_30;
	eMinus_ntrkisoatecal_35 = ntrkisoatecal_35;
	eMinus_ntrkisoatecal_40 = ntrkisoatecal_40;
	eMinus_ntrkisoatecal_45 = ntrkisoatecal_45;
	eMinus_ntrkisoatecal_50 = ntrkisoatecal_50;

	eMinus_hcaliso_10 = hcaliso_10;
	eMinus_hcaliso_15 = hcaliso_15;
	eMinus_hcaliso_20 = hcaliso_20;
	eMinus_hcaliso_25 = hcaliso_25;
	eMinus_hcaliso_30 = hcaliso_30;
	eMinus_hcaliso_35 = hcaliso_35;
	eMinus_hcaliso_40 = hcaliso_40;
	eMinus_hcaliso_45 = hcaliso_45;
	eMinus_hcaliso_50 = hcaliso_50;

	eMinus_ecaliso_10 = ecaliso_10;
	eMinus_ecaliso_15 = ecaliso_15;
	eMinus_ecaliso_20 = ecaliso_20;
	eMinus_ecaliso_25 = ecaliso_25;
	eMinus_ecaliso_30 = ecaliso_30;
	eMinus_ecaliso_35 = ecaliso_35;
	eMinus_ecaliso_40 = ecaliso_40;
	eMinus_ecaliso_45 = ecaliso_45;
	eMinus_ecaliso_50 = ecaliso_50;		  
      }

    }
  }
  // ---------------------------------







  /* ******************************************************** */
    /**  Store generated  Z--> e+e-  information */
    /* ******************************************************** */

  void  ZJetAnalysis::StoreGenZeeInformation (const edm::Event& iEvent) {

    Handle<reco::CandidateCollection> genParticles;
    iEvent.getByLabel("genParticleCandidates", genParticles);

    size_t nZGen = genParticles->size();
    if( nZGen < 1 ) return;

    const Candidate *ZeeGen(0);
    const Candidate *eplusGen(0);
    const Candidate *eminusGen(0);


    for(size_t i = 0; i < nZGen; ++ i) {

      // Look for Z-->e+e-
      ZeeGen = &((*genParticles)[i]);
      size_t nZdau = 0;
      if(!(ZeeGen==0)) nZdau = ZeeGen->numberOfDaughters();


      if( nZdau<1 || !(ZeeGen->pdgId()==23) ) continue;

      for(size_t j = 0; j < nZdau; ++ j) {
	const Candidate *d = ZeeGen->daughter( j );
	if(!(d==0)) {
	  if (d->pdgId()==-11)  eplusGen  = d;
	  if (d->pdgId()==11) eminusGen = d;
	}
      }

      if( eplusGen != 0 && eminusGen != 0 ) {
	mZeeGen = ZeeGen->mass();
	Z_pxGen = ZeeGen->px();
	Z_pyGen = ZeeGen->py();
	Z_pzGen = ZeeGen->pz();
	Z_EGen  = ZeeGen->energy();
	Z_PtGen = ZeeGen->pt();
	Z_EtGen = ZeeGen->et();
	Z_EtaGen = ZeeGen->eta();   
	Z_PhiGen = ZeeGen->phi();
	Z_ChargeGen = ZeeGen->charge();
	Z_VxGen = ZeeGen->vx();
	Z_VyGen = ZeeGen->vy();
	Z_VzGen = ZeeGen->vz();
      
	ePluspxGen    = eplusGen->px();
	ePluspyGen    = eplusGen->py();
	ePluspzGen    = eplusGen->pz();
	ePlusEGen     = eplusGen->energy();
	ePlusPtGen    = eplusGen->pt();
	ePlusEtGen    = eplusGen->et();
	ePlusEtaGen = eplusGen->eta();    
	ePlusPhiGen = eplusGen->phi();
	ePlusChargeGen =eplusGen-> charge();
	ePlusVxGen = eplusGen->vx();
	ePlusVyGen = eplusGen->vy();
	ePlusVzGen = eplusGen->vz();

	eMinuspxGen   = eminusGen->px();
	eMinuspyGen   = eminusGen->py();
	eMinuspzGen   = eminusGen->pz();
	eMinusEGen    = eminusGen->energy();
	eMinusPtGen   = eminusGen->pt();
	eMinusEtGen   = eminusGen->et();
	eMinusEtaGen = eminusGen->eta();    
	eMinusPhiGen = eminusGen->phi();
	eMinusChargeGen = eminusGen->charge();
	eMinusVxGen = eminusGen->vx();
	eMinusVyGen = eminusGen->vy();
	eMinusVzGen = eminusGen->vz();
      }
    }


    Handle<reco::GsfElectronCollection> electron;
    iEvent.getByLabel("pixelMatchGsfElectrons", electron);

    for(size_t j = 0; j < electron->size(); ++ j) {
      const reco::PixelMatchGsfElectron&  x = (*electron)[j];
      int classfn = x.classification();
      if( overlap( x,*eplusGen) ) ePlusClassificationGen = classfn;
      if( overlap( x,*eminusGen) ) eMinusClassificationGen = classfn;
    }
  }
  // ---------------------------------








  //  ************* Utility: check overlap **********************
  bool ZJetAnalysis::overlap( const reco::GsfElectron & e, 
			      const Candidate & c) const {
    
    double deltaR  = radius(&e, &c);

    if( fabs(deltaR) < 0.3 && (c.charge()==e.charge()) ) return true;
    
    return false;
  }
  //--------------------------------------------







//   //  ************* Utility: get jet flavor **********************


  JetFlavour ZJetAnalysis::getMatchedParton(const reco::CaloJet &jet) {

    JetFlavour jetFlavour;
  
    if (flavourMatchOptionf == "fastMC") {
      jetFlavour.underlyingParton4Vec(jet.p4());
    } 
    else if (flavourMatchOptionf == "hepMC") {    
      jetFlavour = jfi.identifyBasedOnPartons(jet);  
    } 
    else if (flavourMatchOptionf == "genParticle") {

      for( reco::CandMatchMap::const_iterator f  = theJetPartonMapf->begin();
	   f != theJetPartonMapf->end(); f++) {
	const reco::Candidate *theJetInTheMatchMap = &*(f->key);
	const reco::Candidate *theMatchedParton    = &*(f->val);


	if(theJetInTheMatchMap->hasMasterClone ()) {
	  const reco::CaloJet* theMasterClone = 
	    dynamic_cast<const reco::CaloJet*>
	    (theJetInTheMatchMap->masterClone().get());
	
	  if( fabs(theMasterClone->phi() - jet.phi()) < 1.e-5 &&
	      fabs(theMasterClone->eta() - jet.eta())< 1.e-5 ){

	    jetFlavour.flavour(abs(theMatchedParton->pdgId()));
	    jetFlavour.underlyingParton4Vec(theMatchedParton->p4());
	    return jetFlavour;
	  }
	}
      }  
    }
  
    return jetFlavour;
  }








  //   //  ************* Utility: electron isolation **********************
  // calculate track isolation for different cone values
  void  ZJetAnalysis::TrackIsolation( const edm::Event& iEvent, 
				      bool useECAL, float  eta, 
				      float phi, float maxVtxDist, float conesize, 
				      float &ptiso, int &ntrkiso) {

    double pt = 0.0;
    int ntrk = 0;

    Handle<TrackCollection> tracks;
    iEvent.getByLabel(trackTags_,tracks);

    for(TrackCollection::const_iterator itTrack = tracks->begin();
	itTrack != tracks->end();  ++itTrack) {

      if ( itTrack->pt() < 1.5 ) continue ;  
      if (fabs( itTrack->dz() - maxVtxDist ) > 0.1 ) continue;
      
      double etaTrack = itTrack->innerMomentum().eta();  
      double phiTrack = itTrack->innerMomentum().phi();  
     

      if( useECAL ) {
	const math::XYZVector tempvec = itTrack->innerMomentum();
	const math::XYZPoint temppos = itTrack->innerPosition();
	int tempcharge = itTrack->charge();     
	ECALPositionCalculator tempcalculator;      
	
	phiTrack = tempcalculator.ecalPhi(tempvec,temppos,tempcharge);
	etaTrack = tempcalculator.ecalEta(tempvec,temppos);
      }


      double deltaPhi = phiTrack - phi;
      double deltaEta = etaTrack - eta;

      if(deltaPhi > Geom::pi()) deltaPhi -= 2.*Geom::pi();
      if(deltaPhi < -Geom::pi()) deltaPhi += 2.*Geom::pi();
      double deltaR = std::sqrt(deltaEta*deltaEta+deltaPhi*deltaPhi);
       
      if(deltaR < conesize && deltaR > 0.02)  {
	pt  += sqrt(itTrack->innerMomentum().Mag2()); 
	ntrk++; 
      }  
    }


    if(pt != 0.0 && ntrk != 0) {
      ptiso = pt;
      ntrkiso = ntrk;
    }

  }








  //   //  ************* Utility: HCAL isolation *********
  double ZJetAnalysis::HCALIsolation( const edm::Event& iEvent, float eta, 
				      float phi, float conesize ) {

     double hcalEnergy = 0.0;
     Handle<HBHERecHitCollection> hbhe;
     iEvent.getByLabel("hbhereco", hbhe);
     const HBHERecHitMetaCollection mhbhe(*hbhe);

     CaloConeSelector selector(conesize, geometry_, DetId::Hcal);
     std::auto_ptr<CaloRecHitMetaCollectionV> selected 
       = selector.select(eta,phi,mhbhe);

     for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); 
	  hit != selected->end(); ++hit) { 
       hcalEnergy += hit->energy();
     }

     if(hcalEnergy == 0.0) return -1.0;
     return hcalEnergy;
  }






  //   //  ************* Utility: ECAL isolation *********
  double ZJetAnalysis::ECALIsolation( const edm::Event& iEvent, float eta, 
				      float phi, float conesize ) {
    
    double ecalEnergy = 0;
    Handle<EBRecHitCollection> ecalhits;
    iEvent.getByLabel( "ecalRecHit", "EcalRecHitsEB", ecalhits);
    const EcalRecHitMetaCollection mecalhits(*ecalhits);    
    
    CaloConeSelector selector(conesize, geometry_, DetId::Ecal);
    std::auto_ptr<CaloRecHitMetaCollectionV> selected 
      = selector.select(eta,phi,mecalhits);
    
    for (CaloRecHitMetaCollectionV::const_iterator hit=selected->begin(); 
	 hit != selected->end(); ++hit) { 
      ecalEnergy += hit->energy();
    }
    

    if(ecalEnergy == 0.0) return -1.0;
    return ecalEnergy;
  }
  







  //   //  ************* Utility: electron identification **********************

// algo==0::PTDR ElectronID:     criterion == "tight", "medium", "loose"
// algo==1::CutBased ElectronID: criterion == "tight", "robust", "loose"


  bool ZJetAnalysis::isElectronId( const edm::Event& iEvent, 
				   const reco::PixelMatchGsfElectron& 
				   electron, int algo, 
				   std::string criterion) const {

    bool boolDecision = false; 

    if(algo==0) boolDecision = PTDRElectronID_result( electron, 
						      iEvent, criterion);
    if(algo==1) boolDecision = CutBasedElectronID_result( electron, 
							  iEvent, criterion);
    return boolDecision;
  }
  //--------------------------------------------












//   //  ************* electron identification:: PTDR **********************
  bool 
  ZJetAnalysis::PTDRElectronID_result(const reco::PixelMatchGsfElectron& 
				      electron, const edm::Event& e, 
				      std::string quality) const {

  if (electron.classification()==40) return false;

  int useEoverPIn[3]      = { 0, 1, 0 };
  int useDeltaEtaIn[3]    = { 1, 1, 1 };
  int useDeltaPhiIn[3]    = { 1, 1, 1 };
  int useHoverE[3]        = { 1, 1, 1 };
  int useE9overE25[3]     = { 1, 1, 1 };
  int useEoverPOut[3]     = { 1, 1, 1 };
  int useDeltaPhiOut[3]   = { 0, 1, 1 };
  int useInvEMinusInvP[3] = { 0, 0, 0 };
  int useBremFraction[3]  = { 0, 0, 0 };
  int useSigmaEtaEta[3]   = { 0, 1, 1 };
  int useSigmaPhiPhi[3]   = { 0, 1, 0 };

  double EoverPInMax[3][8] = { { 999.,  999.,  999.,  999.,  999.,  999.,  
				 999.,  999.  },
			       { 1.3,   1.2,   1.3,   999.,  999.,  999.,  
				 999.,  999.  },
			       { 999.,  999.,  999.,  999.,  999.,  999.,  
				 999.,  999.  } };
  double EoverPInMin[3][8] = {{ 0.,    0.,    0.,    0.,    0.,    0.,    
				0.,    0.    },
			      { 0.9,   0.9,   0.9,   0.6,   0.9,   0.9,   
				0.9,   0.7   },
			      { 0.,    0.,    0.,    0.,    0.,    0.,    
				0.,    0.    }};
  double deltaEtaIn[3][8] = {{ 0.008, 0.008, 0.008, 0.009, 0.008, 0.008, 
			       0.008, 0.009 },
			     { 0.004, 0.006, 0.005, 0.007, 0.007, 0.008, 
			       0.007, 0.008 },
			     { 0.004, 0.004, 0.004, 0.005, 0.005, 0.005, 
			       0.005, 0.005 }};
  double deltaPhiIn[3][8] = {{ 0.06,  0.06,  0.06,  0.08,  0.06,  0.06,  
			       0.06,  0.09  },
			     { 0.04,  0.07,  0.04,  0.08,  0.06,  0.07,  
			       0.06,  0.07  },
			     { 0.02,  0.03,  0.02,  0.04,  0.04,  0.04,  
			       0.04,  0.05  }};
  double HoverE[3][8] = {{ 0.09,  0.06,  0.07,  0.12,  0.09,  0.06,  0.07,  
			   0.12  },
			 { 0.06,  0.05,  0.06,  0.14,  0.1,   0.1,   0.1,  
			   0.12  },
			 { 0.05,  0.05,  0.05,  0.05,  0.07,  0.07,  0.07,  
			   0.07  }};
  double E9overE25[3][8] = {{ 0.7,   0.70,  0.7,   0.5,   0.8,   0.8,   
			      0.8,   0.5   },
			    { 0.7,   0.75,  0.8,   0.,    0.85,  0.75,  
			      0.8,   0.    },
			    { 0.8,   0.65,  0.75,  0.65,  0.8,   0.7,   
			      0.7,   0.65  }};
  double EoverPOutMax[3][8] = {{ 2.5,   999.,  2.2,   999.,  2.5,   999.,  
				 2.2,   999.  },
			       { 2.5,   999.,  999.,  999.,  2.,    999.,  
				 999.,  999.  },
			       { 999,   999.,  999.,  999.,  999.,  999.,  
				 999.,  999.  }};
  double EoverPOutMin[3][8] = {{ 0.6,   1.7,   0.9,   0.5,   0.6,   1.7,  
				 0.9,   0.5   },
			       { 0.6,   1.8,   1.,    0.75,  0.6,   1.5,   
				 1.,    0.8   },
			       { 0.6,   0.75,  0.75,  0.75,  0.5,   0.8, 
				 0.5,   0.8   }};
  double deltaPhiOut[3][8] = {{ 0.011, 999.,  999.,  999.,  0.02,  999., 
				999.,  999.  },
			      { 0.011, 999.,  999.,  999.,  0.02,  999., 
				999.,  999.  },
			      { 0.02,  999.,  0.02,  999.,  0.02,  999., 
				0.02,  999.  }};
  double invEMinusInvP[3][8] = {{ 0.02,  0.02,  0.02,  0.02,  0.02,  0.02, 
				  0.02,  0.02  },
				{ 0.02,  0.02,  0.02,  0.02,  0.02,  0.02, 
				  0.02,  0.02  },
				{ 0.02,  0.02,  0.02,  0.02,  0.02,  0.02,
				  0.02,  0.02  }};
  double bremFraction[3][8] = {{ 0.,    0.1,   0.1,   0.1,   0.,    0.2, 
				 0.2,   0.2   },
			       { 0.,    0.1,   0.1,   0.1,   0.,    0.2, 
				 0.2,   0.2   },
			       { 0.,    0.1,   0.1,   0.1,   0.,    0.2, 
				 0.2,   0.2   }};
  double sigmaEtaEtaMax[3][8] = {{ 999.,  999.,  999.,  999.,  999.,  999., 
				   999.,  999.  },
				 { 0.011, 0.011, 0.011, 0.011, 0.022, 0.022,
				   0.022, 0.3   },
				 { 0.011, 0.011, 0.011, 0.011, 0.03,  0.03,
				   0.03,  0.022 }};
  double sigmaEtaEtaMin[3][8] = {{ 0.,    0.,    0.,    0.,    0.,    0.,  
				   0.,    0.    },
				 { 0.005, 0.005, 0.005, 0.005, 0.008, 0.008,
				   0.008, 0.	},
				 { 0.005, 0.005, 0.005, 0.005, 0.008, 0.008,
				   0.008, 0.008 }};
  double sigmaPhiPhiMax[3][8] = {{ 999.,  999.,  999.,  999.,  999.,  999., 
				   999.,  999.  },
				 { 0.015, 999.,  999.,  999.,  0.02,  999., 
				   999.,  999.  },
				 { 999.,  999.,  999.,  999.,  999.,  999.,
				   999.,  999.  }};
  double sigmaPhiPhiMin[3][8] = {{ 0.,    0.,    0.,    0.,    0.,    0., 
				   0.,    0.    },
				 { 0.005, 0.,    0.,    0.,    0.,    0.,  
				   0.,    0.    },
				 { 0.,    0.,    0.,    0.,    0.,    0.,  
				   0.,    0.    }};

  int variables_ = 0;

  if (quality=="loose") {
    variables_ = 0 ;
    }

    if (quality=="medium") {
      variables_ = 1 ;
    }
    
  if (quality=="tight") {
    variables_ = 2 ;
    }

  //determine which element of the cut arrays in electronId.cfi to read
  //depending on the electron classification
  int icut=0;
  switch (electron.classification()) {
  case 0: icut=0; break;
  case 10: icut=1; break;
  case 20: icut=2; break;
  case 30: icut=3; break;
  case 31: icut=3; break;
  case 32: icut=3; break;
  case 33: icut=3; break;
  case 34: icut=3; break;
  case 100: icut=4; break;
  case 110: icut=5; break;
  case 120: icut=6; break;
  case 130: icut=7; break;
  case 131: icut=7; break;
  case 132: icut=7; break;
  case 133: icut=7; break;
  case 134: icut=7; break;
  default:
    edm::LogError("CutBasedElectronID") << 
      "Error: unrecognized electron classification ";
    break;
  }
  
  if (useEoverPIn[variables_]) {
    double value = electron.eSuperClusterOverP();
    double maxcut = EoverPInMax[variables_][icut];
    double mincut = EoverPInMin[variables_][icut];
    if (value<mincut || value>maxcut) return false;
  }

  if (useDeltaEtaIn[variables_]) {
    double value = electron.deltaEtaSuperClusterTrackAtVtx();
    double maxcut = deltaEtaIn[variables_][icut];
    if (fabs(value)>maxcut) return false;
  }

  if (useDeltaPhiIn[variables_]) {
    double value = electron.deltaPhiSuperClusterTrackAtVtx();
    double maxcut = deltaPhiIn[variables_][icut];
    if (fabs(value)>maxcut) return false;
  }

  if (useHoverE[variables_]) {
    double value = electron.hadronicOverEm();
    double maxcut = HoverE[variables_][icut];
    if (fabs(value)>maxcut) return false;
  }

  if (useEoverPOut[variables_]) {
    double value = electron.eSeedClusterOverPout();
    double maxcut = EoverPOutMax[variables_][icut];
    double mincut = EoverPOutMin[variables_][icut];
    if (value<mincut || value>maxcut) return false;
  }

  if (useDeltaPhiOut[variables_]) {
    double value = electron.deltaPhiSeedClusterTrackAtCalo();
    double maxcut = deltaPhiOut[variables_][icut];
    if (fabs(value)>maxcut) return false;
  }

  if (useInvEMinusInvP[variables_]) {
    double value = (1./electron.caloEnergy()) -
      (1./electron.trackMomentumAtVtx().R());
    double maxcut = invEMinusInvP[variables_][icut];
    if (value>maxcut) return false;
  }

  if (useBremFraction[variables_]) {
    double value = electron.trackMomentumAtVtx().R() -
      electron.trackMomentumOut().R();
    double mincut = bremFraction[variables_][icut];
    if (value<mincut) return false;
  }

  const reco::ClusterShapeRef& shapeRef = getClusterShape(&electron,e);

  if (useE9overE25[variables_]) {
    double value = shapeRef->e3x3()/shapeRef->e5x5();
    double mincut = E9overE25[variables_][icut];
    if (fabs(value)<mincut) return false;
  }

  if (useSigmaEtaEta[variables_]) {
    double value = shapeRef->covEtaEta();
    double maxcut = sigmaEtaEtaMax[variables_][icut];
    double mincut = sigmaEtaEtaMin[variables_][icut];
    if (sqrt(value)<mincut || sqrt(value)>maxcut) return false;
  }

  if (useSigmaPhiPhi[variables_]) {
    double value = shapeRef->covPhiPhi();
    double mincut = sigmaPhiPhiMin[variables_][icut];
    double maxcut = sigmaPhiPhiMax[variables_][icut];
    if (sqrt(value)<mincut || sqrt(value)>maxcut) return false;
  }

  return true;

  }
  //--------------------------------------------



//   //  ********* electron identification:: Cut Based **************
  bool 
  ZJetAnalysis::CutBasedElectronID_result(const reco::PixelMatchGsfElectron& 
					  electron, const edm::Event& e, 
					  std::string quality)  const {


    double eta = fabs(electron.p4().Eta());
    const reco::ClusterShapeRef& shapeRef = getClusterShape(&electron, e);
  
    double eOverP = electron.eSuperClusterOverP();
    double eSeed = electron.superCluster()->seed()->energy();
    double pin  = electron.trackMomentumAtVtx().R();   
    double eSeedOverPin = eSeed/pin; 
    double pout = electron.trackMomentumOut().R(); 
    double fBrem = (pin-pout)/pin;
  
    double hOverE = electron.hadronicOverEm();
    double sigmaee = sqrt(shapeRef->covEtaEta());
    double deltaPhiIn = electron.deltaPhiSuperClusterTrackAtVtx();
    double deltaEtaIn = electron.deltaEtaSuperClusterTrackAtVtx();
  
    int eb;
    if (eta < 1.479) eb = 0;
    else {
      eb = 1; 
      sigmaee = sigmaee - 0.02*(fabs(eta) - 2.3); 
      //correct sigmaetaeta dependence on eta in endcap
    }

    
    // ROBUST Selection
    if (quality == "robust") {

      // hoe, sigmaEtaEta, dPhiIn, dEtaIn
      double cut[4];
      if (eta < 1.479) { 
	cut[0] = 0.115; 
	cut[1] = 0.0140;
	cut[2] = 0.090; 
	cut[3] = 0.0090; 
      }
      else {
	cut[0] = 0.150;
	cut[1] = 0.0275;
	cut[2] = 0.092;
	cut[3] = 0.0105; 
      }

      if (hOverE > cut[0]) return false;    
      if (sigmaee > cut[1]) return false;    
      if (fabs(deltaPhiIn) > cut[2]) return false;    
      if (fabs(deltaEtaIn) > cut[3]) return false;        
      return true;
    }
  
    int cat = electron.classification();

    // TIGHT Selection
    if (quality == "tight") {
    
      if ((eOverP < 0.8) && (fBrem < 0.2)) return false;
    
      if (eOverP < 0.9*(1-fBrem)) return false;
    
      double cut1[8] = {0.05, 0.042,  0.045,  0., 0.055,   0.037,  0.050,  0.};
      if (hOverE > cut1[cat+4*eb]) return false;    
    
      double cut2[8] = {0.0125, 0.011,  0.01, 0., 0.0265,  0.0252, 0.026, 0.};
      if (sigmaee > cut2[cat+4*eb]) return false;    
    

      double cut3[8] = {0.032, 0.016, 0.0525,0.09, 0.025, 0.035, 0.065, 0.092};
      if (eOverP < 1.5) { if(fabs(deltaPhiIn) > cut3[cat+4*eb]) return false;} 
      else { if (fabs(deltaPhiIn) > cut3[3+4*eb]) return false; }
      
      double cut4[8] = { 0.0055, 0.0030, 0.0065, 0.,0.0060, 0.0055, 0.0075,0.};
      if (fabs(deltaEtaIn) > cut4[cat+4*eb]) return false;    
    
      double cut5[8] = {0.24, 0.94,   0.11,   0., 0.32,    0.83,   0.,  0.};
      if (eSeedOverPin < cut5[cat+4*eb]) return false;  

      double cut6[8] = {99999., 99999., 99999., 99999., 
			99999., 99999., 99999., 99999.};
      if (eSeedOverPin > cut6[cat+4*eb]) return false;  

      return true;
    }
  
    // LOOSE Selection
    if (quality == "loose") {
    
      if ((eOverP < 0.8) && (fBrem < 0.2)) return false;
    
      double cut7[8] = {0.115, 0.10, 0.055,  0., 0.145,   0.12,   0.150,  0.}; 
      if (hOverE > cut7[cat+4*eb]) return false;    
    
      double cut8[8] = {0.0140, 0.0120, 0.0115, 0.,0.0275, 0.0265, 0.0265, 0.};
      if (sigmaee > cut8[cat+4*eb]) return false;    
    

      double cut9[8] = {0.05, 0.025, 0.053, 0.09, 0.07, 0.03, 0.092,  0.092};
      if (eOverP < 1.5) { if(fabs(deltaPhiIn) > cut9[cat+4*eb]) return false;} 
      else { if (fabs(deltaPhiIn) > cut9[3+4*eb]) return false; }
    

      double cut10[8] = {0.009, 0.0045, 0.0085, 0.,0.0105, 0.0068, 0.010, 0.}; 
      if (fabs(deltaEtaIn) > cut10[cat+4*eb]) return false;    
    
      double cut11[8] = {0.11, 0.91, 0.11,   0.,  0., 0.85,   0.,   0.};
      if (eSeedOverPin < cut11[cat+4*eb]) return false;  
    
      return true; 
    }
  
    return false;
  }
  //--------------------------------------------







  //  ************* Utility:ClusterShapeRef **********************
  const reco::ClusterShapeRef& 
  ZJetAnalysis::getClusterShape(const reco::PixelMatchGsfElectron* electron, 
				const edm::Event& e) const 
  {
    // Get association maps linking BasicClusters to ClusterShape.
    edm::Handle<reco::BasicClusterShapeAssociationCollection> 
      clusterShapeHandle;
    if (electron->classification()<100) {
      e.getByLabel("hybridSuperClusters","hybridShapeAssoc",
		   clusterShapeHandle);
    } else {
      e.getByLabel("islandBasicClusters","islandEndcapShapeAssoc", 
		   clusterShapeHandle);
    }
    // Find entry in map corresponding to seed BasicCluster of SuperCluster
    reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr;
    seedShpItr = clusterShapeHandle->find(electron->superCluster()->seed());
    const reco::ClusterShapeRef& seedShapeRef = seedShpItr->val;
    return seedShapeRef;
  }
  //--------------------------------------------










  //  ************* Load Tracks **********************
  void  ZJetAnalysis::StoreTrackInformation (const edm::Event& iEvent) {
    edm::Handle<reco::TrackCollection> tracks;
    iEvent.getByLabel(trackTags_, tracks);

    reco::TrackCollection::const_iterator trk;
    int iTracks = 0;
    for ( trk = tracks->begin(); trk != tracks->end(); ++trk){
      TrackRecoP[iTracks] = trk->p();
      TrackRecoPx[iTracks] = trk->px();
      TrackRecoPy[iTracks] = trk->py();
      TrackRecoPz[iTracks] = trk->pz();
      TrackRecoVx[iTracks] = trk->px();
      TrackRecoVy[iTracks] = trk->py();
      TrackRecoVz[iTracks] = trk->pz();
      TrackRecoPt[iTracks] = trk->pt();
      TrackRecoEta[iTracks] = trk->eta();
      TrackRecoPhi[iTracks] = trk->phi();
      iTracks++;
    }
    NumRecoTrack = iTracks;
  }
  // ---------------------------------------------






    // ***************** View Particle Level Information ******************

  void  ZJetAnalysis::StoreParticleInformation (const edm::Event& iEvent) {

    // Load generator information
    int nevhep = 0;
    vector<HepMC::GenParticle*> theGenPart;
    vector<HepMC::GenParticle*> theGenPartAll;
    map<HepMC::GenParticle*,int> theGenPartPair;
  
    vector< Handle< HepMCProduct > > EvtHandles ;
    iEvent.getManyByType( EvtHandles ) ;
    eventsize = 0;

    if (EvtHandles.empty()) {
      // can't find it!
      if (!allowMissingInputs_){
	throw edm::Exception(errors::ProductNotFound,"NoMatch")
	  << "getManyByType: no products found of specified type\n";
      }
    } else {
      eventsize =  EvtHandles.size();
      for (int i=0; i<eventsize; i++ ) {
      
	if ( EvtHandles[i].isValid() ) {
	  const HepMC::GenEvent* Evt = EvtHandles[i]->GetEvent() ;
	
	  for (HepMC::GenEvent::particle_const_iterator
		 Part = Evt->particles_begin() ; Part!=Evt->particles_end(); Part++ ) {
	    double Eta = -log(tan(((*Part)->momentum()).theta()/2.));
	  
	    if ( EvtHandles[i].provenance()->moduleLabel() == "source" ) {
	      nevhep++;
	      theGenPartPair[(*Part)] = nevhep;   
	      theGenPartAll.push_back(*Part);
	    
	      // Analysis of the particles in event	      
	      if( (*Part)->status()!=1 ) continue;
	      if ( !((*Part)->pdg_id()==23 || abs((*Part)->pdg_id())==11) ) continue;
	      if(fabs(Eta) > 2.8 ) continue;
	      if(((*Part)->momentum()).perp() <5. ) continue;
	    
	      theGenPart.push_back(*Part);	    
	    }
	    else {
	      cout<<" Label "<<EvtHandles[i].provenance()->moduleLabel()<<endl;
	    }
	  }
	
	}
      }
    }
    //  ------------ " We filled theGenPart" ---------------------
  
    int ihep = 0;
    int motherline1 = 0;
    int motherline2 = 0;
    int dauthline1 = 0;
    int dauthline2 = 0;
 
    for(vector<HepMC::GenParticle*>::iterator Part = theGenPartAll.begin(); 
	Part != theGenPartAll.end(); Part++) {
      ihep++;
      // If there is a mother
      motherline1 = 0;
      motherline2 = 0;
      dauthline1 = 0;
      dauthline2 = 0;
      double vx=0.; 
      double vy=0.;
      double vz=0.; 
      double vt=0.;
    
      if((*Part)->production_vertex()) {        
	if(theGenPartPair.find(*((*Part)->production_vertex()->particles_in_const_begin())) != theGenPartPair.end() ) 
	  motherline1 = (*theGenPartPair.find(*((*Part)->production_vertex()->particles_in_const_begin())) ).second;
	HepMC::GenVertex::particles_in_const_iterator inend = (*Part)->production_vertex()->particles_in_const_end();
	inend--;
	if(theGenPartPair.find( *(inend) ) != theGenPartPair.end()) {
	  motherline2 = (*theGenPartPair.find(  *(inend) )).second;
	} else  motherline2 = 0;
      
	vx = (*Part)->production_vertex()->position().x();
	vy = (*Part)->production_vertex()->position().y();
	vy = (*Part)->production_vertex()->position().z();
	vt = (*Part)->production_vertex()->position().t();	
      }
    
    
      if((*Part)->end_vertex()) {
	if(theGenPartPair.find(*((*Part)->end_vertex()->particles_out_const_begin())) != theGenPartPair.end()) 
	  dauthline1 = (*theGenPartPair.find(*((*Part)->end_vertex()->particles_out_const_begin()))).second;
      
	HepMC::GenVertex::particles_in_const_iterator inend = (*Part)->end_vertex()->particles_out_const_end();
	inend--; 
	if(theGenPartPair.find(*(inend)) != theGenPartPair.end()) dauthline2 = (*theGenPartPair.find(*(inend))).second;
      }
    

      if(ihep-1<4000) {
	partStatus[ihep-1] = (*Part)->status();
	partPDGId[ihep-1] = (*Part)->pdg_id();
	partMother1[ihep-1] = motherline1;
	partpx[ihep-1] = (*Part)->momentum().px();
	partpy[ihep-1] = (*Part)->momentum().py();
	partpz[ihep-1] = (*Part)->momentum().pz();
	parte[ihep-1] = (*Part)->momentum().e();
	partm[ihep-1] = (*Part)->momentum().m();
	partvx[ihep-1] = (float)vx;
	partvy[ihep-1] = (float)vy;
	partvz[ihep-1] = (float)vz;
	partvt[ihep-1] = (float)vt;
	NumPart = ihep;  
      } 
    }
  }     
  // --------------------------------------------------  



////////////////////////////////////////////////////////////////////////////////////////
void ZJetAnalysis::fillHist1D(const TString& histName,const Double_t& value, const Double_t& wt) {

  std::map<TString, TH1*>::iterator hid=m_HistNames1D.find(histName);
  if (hid==m_HistNames1D.end()){
    std::cout << "%fillHist -- Could not find histogram with name: " << histName << std::endl;
  }
  else{
    hid->second->Fill(value,wt);
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
  
} // namespace cms







#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
using cms::ZJetAnalysis;
DEFINE_FWK_MODULE(ZJetAnalysis);
