#ifndef ZJetAnalysis_h
#define ZJetAnalysis_h
// system include files
#include <memory>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <Math/GenVector/VectorUtil.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/EventSetup.h" 

#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/JetReco/interface/CaloJet.h"  
#include "DataFormats/JetReco/interface/GenJet.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

class TFile;
class TTree;
class CaloGeometry;

using namespace edm;
using namespace std;
using namespace reco;

//
// class decleration
//
namespace cms
{
  class ZJetAnalysis : public edm::EDAnalyzer {
  public:
    explicit ZJetAnalysis(const edm::ParameterSet&);
    ~ZJetAnalysis();

    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void beginJob(const edm::EventSetup& ) ;
    virtual void endJob() ;
    virtual void StoreCaloJetInformation (const edm::Event& iEvent);
    virtual void StoreCorJetInformation (const edm::Event& iEvent);
    virtual void StoreGenJetInformation (const edm::Event& iEvent);
    virtual void StoreRecoZeeInformation (const edm::Event& iEvent, 
					  const edm::EventSetup& iSetup);
    virtual void StoreGenZeeInformation (const edm::Event& iEvent);
    virtual bool overlap( const reco::GsfElectron & e, const Candidate & c) const;
    virtual bool isElectronId( const edm::Event& iEvent, 
			       edm::Ref<reco::GsfElectronCollection> electronRef,
			       string electronLabel) const;
    virtual void  TrackIsolation( const edm::Event& iEvent, 
				  float eta, float phi, float maxVtxDist, float conesize, 
				  float &ptiso, int &ntrkiso);
    virtual double HCALIsolation( const edm::Event& iEvent, float eta, 
				  float phi, float energy, float conesize );
    virtual double ECALIsolation( const edm::Event& iEvent, float eta, 
				  float phi, float energy, float conesize );
    virtual void fillHist1D(const TString& histName,const Double_t& value, const Double_t& wt=1.0);
    virtual void setDefaultValues();
    virtual void declareTreeBranches();
    virtual void createHistograms();
    virtual void fillAllHistograms();

  private:
    // ----------member data ---------------------------
    // names of modules, producing object collections
     
    std::vector<edm::InputTag> mInputCalo;
    std::vector<edm::InputTag> mInputGen;
    std::vector<edm::InputTag> mInputCor;
    edm::InputTag zee_;
    edm::InputTag electrons_;
    edm::InputTag trackTags_; 
    edm::InputTag sourceByValue_;
    bool runningOverMC_;
    bool doJetFlavorIdentification_;
    bool trackInfo_;

    const CaloGeometry* geometry_;
    const JetCorrector* corrector_L3; 
    std::string L3JetCorrectionService;


    // stuff for histogramms
    //  output file name with histograms
    std::string fOutputFileName ;
    TFile*      hOutputFile ;
    TTree *     myTree;
    std::map<TString, TH1*> m_HistNames1D;


    int NumRecoJets;
    int NumRecoJetAlgo;
    int NumGenJets;
    int NumGenJetAlgo;
    int NumRecoTrack;
    int NumPart;
    int nZee;
    int run;
    int event; 
    int eventsize;
    float genEventScale;
    float genEventWeight;

    static const int NUM_ALGO_MAX = 20;
    static const int NUM_JET_MAX = 4;

    float JetRecoEt[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoPt[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoEta[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoPhi[NUM_ALGO_MAX][NUM_JET_MAX];
    int   JetRecoType[NUM_ALGO_MAX][NUM_JET_MAX];
    int   JetRecoFlavor[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoMaxEInEmTowers[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoMaxEInHadTowers[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoEnergyFractionHadronic[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoEmEnergyFraction[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoHadEnergyInHB[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoHadEnergyInHO[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoHadEnergyInHE[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoHadEnergyInHF[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoEmEnergyInEB[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoEmEnergyInEE[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoEmEnergyInHF[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetRecoTowersArea[NUM_ALGO_MAX][NUM_JET_MAX];
    int   JetRecoN90[NUM_ALGO_MAX][NUM_JET_MAX]; 
    int   JetRecoN60[NUM_ALGO_MAX][NUM_JET_MAX];

    float JetGenEt[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetGenPt[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetGenEta[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetGenPhi[NUM_ALGO_MAX][NUM_JET_MAX];
    int   JetGenType[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetGenEmEnergy[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetGenHadEnergy[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetGenInvisibleEnergy[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetGenAuxiliaryEnergy[NUM_ALGO_MAX][NUM_JET_MAX];

    float JetCorEt[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetCorPt[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetCorEta[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetCorPhi[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetCorEnergyFractionHadronic[NUM_ALGO_MAX][NUM_JET_MAX];
    float JetCorEmEnergyFraction[NUM_ALGO_MAX][NUM_JET_MAX];


    float mZee;
    float Z_px;
    float Z_py;
    float Z_pz;
    float Z_E;
    float Z_Pt;
    float Z_Et;
    float Z_Eta;
    float Z_Phi;
    int Z_Charge;
    float Z_Vx;
    float Z_Vy;
    float Z_Vz;

    float mZeeGen;
    float Z_pxGen;
    float Z_pyGen;
    float Z_pzGen;
    float Z_EGen;
    float Z_PtGen;
    float Z_EtGen;
    float Z_EtaGen;
    float Z_PhiGen;
    float Z_VxGen;
    float Z_VyGen;
    float Z_VzGen;

    bool isePlusLoose;
    bool isePlusTight;
    bool isePlusRobustLoose;
    bool isePlusRobustTight;

    int ePlusClassification; 
    float ePluspx;
    float ePluspy;
    float ePluspz;
    float ePlusE;
    float ePlusEt;
    float ePlusPt;
    float ePlusEta;
    float ePlusTheta;
    float ePlusPhi;
    int   ePlusCharge;
    float ePlusVx;
    float ePlusVy;
    float ePlusVz;
    float ePlusIsolation;

    //////////////
    float ePlus_ptiso_15; 
    float ePlus_ptiso_20; 
    float ePlus_ptiso_25;
    float ePlus_ptiso_30;
    float ePlus_ptiso_35;
    float ePlus_ptiso_40;
    float ePlus_ptiso_45;
    float ePlus_ptiso_50;

    int ePlus_ntrkiso_15;
    int ePlus_ntrkiso_20;
    int ePlus_ntrkiso_25;
    int ePlus_ntrkiso_30;
    int ePlus_ntrkiso_35;
    int ePlus_ntrkiso_40;
    int ePlus_ntrkiso_45;
    int ePlus_ntrkiso_50;
     
    float ePlus_hcaliso_10;
    float ePlus_hcaliso_15;
    float ePlus_hcaliso_20;
    float ePlus_hcaliso_25;
    float ePlus_hcaliso_30;
    float ePlus_hcaliso_35;
    float ePlus_hcaliso_40;
    float ePlus_hcaliso_45;
    float ePlus_hcaliso_50;

    float ePlus_ecaliso_10;
    float ePlus_ecaliso_15;
    float ePlus_ecaliso_20;
    float ePlus_ecaliso_25;
    float ePlus_ecaliso_30;
    float ePlus_ecaliso_35;
    float ePlus_ecaliso_40;
    float ePlus_ecaliso_45;
    float ePlus_ecaliso_50;
    ///////////////////

    float ePlus_sc_x;
    float ePlus_sc_y;
    float ePlus_sc_z;
    float ePlus_sc_R;
    float ePlus_sc_Rt;
    int   ePlus_sc_no;
    float ePlus_EoverPout;
    float ePlus_EoverPin;
    float ePlus_InvEMinusInvP;
    float ePlus_BremFraction;
    float ePlus_DeltaEtaIn;
    float ePlus_DeltaPhiIn;
    float ePlus_DeltaPhiOut;
    float ePlus_DeltaEtaOut;
    float ePlus_Trackmom_calo;
    float ePlus_Trackmom_vtx;	  
    float ePlus_HoverE;	  
    float ePlus_vx;
    float ePlus_vy;
    float ePlus_vz;	  
    float ePlus_td0;
    float ePlus_d0;	 	  
    float ePlus_E9overE25;
    float ePlus_SigmaEtaEta;
    float ePlus_SigmaEtaPhi;
    float ePlus_SigmaPhiPhi;
	  
    float ePlus_trk_eta;
    float ePlus_trk_phi;
    float ePlus_trk_etaout;
    float ePlus_trk_phiout;
    float ePlus_trk_p;
    float ePlus_trk_pout;
    float ePlus_trk_pt;
    int   ePlus_trk_charge;
    float ePlus_trk_chi2;
    float ePlus_trk_ndof;
    int   ePlus_trk_hits;
    float ePlus_trk_d0;
    float ePlus_trk_d0Err;
	  


    int ePlusClassificationGen;
    float ePluspxGen;
    float ePluspyGen;
    float ePluspzGen;
    float ePlusEGen;
    float ePlusPtGen;
    float ePlusEtGen;
    float ePlusEtaGen;
    float ePlusPhiGen;
    int   ePlusChargeGen;
    float ePlusVxGen;
    float ePlusVyGen;
    float ePlusVzGen;   
  
    bool iseMinusLoose;
    bool iseMinusTight;
    bool iseMinusRobustLoose;
    bool iseMinusRobustTight;

    int eMinusClassification;
    float eMinuspx;
    float eMinuspy;
    float eMinuspz;
    float eMinusE;
    float eMinusPt;
    float eMinusEt;
    float eMinusEta;
    float eMinusTheta;
    float eMinusPhi;
    int   eMinusCharge;
    float eMinusVx;
    float eMinusVy;
    float eMinusVz;
    float eMinusIsolation;

    //////////////
    float eMinus_ptiso_15; 
    float eMinus_ptiso_20; 
    float eMinus_ptiso_25;
    float eMinus_ptiso_30;
    float eMinus_ptiso_35;
    float eMinus_ptiso_40;
    float eMinus_ptiso_45;
    float eMinus_ptiso_50;

    int eMinus_ntrkiso_15;
    int eMinus_ntrkiso_20;
    int eMinus_ntrkiso_25;
    int eMinus_ntrkiso_30;
    int eMinus_ntrkiso_35;
    int eMinus_ntrkiso_40;
    int eMinus_ntrkiso_45;
    int eMinus_ntrkiso_50;
     
    float eMinus_hcaliso_10;
    float eMinus_hcaliso_15;
    float eMinus_hcaliso_20;
    float eMinus_hcaliso_25;
    float eMinus_hcaliso_30;
    float eMinus_hcaliso_35;
    float eMinus_hcaliso_40;
    float eMinus_hcaliso_45;
    float eMinus_hcaliso_50;

    float eMinus_ecaliso_10;
    float eMinus_ecaliso_15;
    float eMinus_ecaliso_20;
    float eMinus_ecaliso_25;
    float eMinus_ecaliso_30;
    float eMinus_ecaliso_35;
    float eMinus_ecaliso_40;
    float eMinus_ecaliso_45;
    float eMinus_ecaliso_50;
    ///////////////////

    float eMinus_sc_x;
    float eMinus_sc_y;
    float eMinus_sc_z;
    float eMinus_sc_R;
    float eMinus_sc_Rt;
    int   eMinus_sc_no;
    float eMinus_EoverPout;
    float eMinus_EoverPin;
    float eMinus_InvEMinusInvP;
    float eMinus_BremFraction;
    float eMinus_DeltaEtaIn;
    float eMinus_DeltaPhiIn;
    float eMinus_DeltaPhiOut;
    float eMinus_DeltaEtaOut;

    float eMinus_Trackmom_calo;
    float eMinus_Trackmom_vtx;
	  
    float eMinus_HoverE;	  
    float eMinus_vx;
    float eMinus_vy;
    float eMinus_vz;	  
    float eMinus_td0;
    float eMinus_d0;
	 
	  
    float eMinus_E9overE25;
    float eMinus_SigmaEtaEta;
    float eMinus_SigmaEtaPhi;
    float eMinus_SigmaPhiPhi;
	  
    float eMinus_trk_eta;
    float eMinus_trk_phi;
    float eMinus_trk_etaout;
    float eMinus_trk_phiout;
    float eMinus_trk_p;
    float eMinus_trk_pout;
    float eMinus_trk_pt;
    int   eMinus_trk_charge;
    float eMinus_trk_chi2;
    float eMinus_trk_ndof;
    int   eMinus_trk_hits;
    float eMinus_trk_d0;
    float eMinus_trk_d0Err;
	  
    int eMinusClassificationGen;
    float eMinuspxGen;
    float eMinuspyGen;
    float eMinuspzGen;
    float eMinusEGen;
    float eMinusPtGen;
    float eMinusEtGen;
    float eMinusEtaGen;
    float eMinusPhiGen;
    int   eMinusChargeGen;
    float eMinusVxGen;
    float eMinusVyGen;
    float eMinusVzGen;


    TH1F* EmECALIsolation10;
    TH1F* EmECALIsolation20;
    TH1F* EmECALIsolation30;
    TH1F* EmECALIsolation40;
    TH1F* EmECALIsolation50;
    TH1F* EmHCALIsolation10;
    TH1F* EmHCALIsolation20;
    TH1F* EmHCALIsolation30;
    TH1F* EmHCALIsolation40;
    TH1F* EmHCALIsolation50;
    TH1F* EmTrackPTIsolation10;
    TH1F* EmTrackPTIsolation20;
    TH1F* EmTrackPTIsolation30;
    TH1F* EmTrackPTIsolation40;
    TH1F* EmTrackPTIsolation50;
    TH1F* EmTrackNumIsolation10;
    TH1F* EmTrackNumIsolation20;
    TH1F* EmTrackNumIsolation30;
    TH1F* EmTrackNumIsolation40;
    TH1F* EmTrackNumIsolation50;
    TH1F* EPECALIsolation10;
    TH1F* EPECALIsolation20;
    TH1F* EPECALIsolation30;
    TH1F* EPECALIsolation40;
    TH1F* EPECALIsolation50;
    TH1F* EPHCALIsolation10;
    TH1F* EPHCALIsolation20;
    TH1F* EPHCALIsolation30;
    TH1F* EPHCALIsolation40;
    TH1F* EPHCALIsolation50;
    TH1F* EPTrackPTIsolation10;
    TH1F* EPTrackPTIsolation20;
    TH1F* EPTrackPTIsolation30;
    TH1F* EPTrackPTIsolation40;
    TH1F* EPTrackPTIsolation50;
    TH1F* EPTrackNumIsolation10;
    TH1F* EPTrackNumIsolation20;
    TH1F* EPTrackNumIsolation30;
    TH1F* EPTrackNumIsolation40;
    TH1F* EPTrackNumIsolation50;
    TH1F* EmEoverP;
    TH1F* EpEoverP;
    TH1F* EpBremFraction;
    TH1F* EmBremFraction;
    TH1F* EpClassification;
    TH1F* EpHoverE;
    TH1F* EmHoverE;
    TH1F* genEPpt;
    TH1F* recoEPpt;
    TH1F* genEMpt;
    TH1F* recoEMpt;
    TH1F* genEMptMassCut;
    TH1F* recoEMptMassCut;
    TH1F* genEMptGolden;
    TH1F* genEMptBigbrem;
    TH1F* genEMptNarrow;
    TH1F* genEMptShowering;
    TH1F* recoEMptGolden;
    TH1F* recoEMptBigbrem;
    TH1F* recoEMptNarrow;
    TH1F* recoEMptShowering;
    TH1F* Zmass_noIso;
    TH1F* Zmass_TkIso;
    TH1F* Zmass_TkECALIso;
    TH1F* Zmass_TkECALHCALIso;
    TH1F* Zmass_ptdrtight;
    TH1F* Zmass_ptdrmedium;
    TH1F* Zmass_ptdrloose;
    TH1F* Zmass_BB;
    TH1F* Zmass_BE;
    TH1F* Zmass_EE;
    TH1F* Zmass_golden;
    TH1F* Zmass_bigbrem;
    TH1F* Zmass_narrow;
    TH1F* Zmass_shower;
    TH1F* genZpt;
    TH1F* recoZpt;
    TH1F* genZptm;
    TH1F* recoZptm;
    TH1F* genZmass;
    TH1F* recoZmass;

  };
}
#endif
