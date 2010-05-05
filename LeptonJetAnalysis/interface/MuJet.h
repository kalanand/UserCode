/*****************************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: ElectroWeakAnalysis/VPlusJets
 *
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *   Search for leptojet in muon channel: 
 *          fill interesting variables into a specified TTree
 *   Can work with jets in RECO/AOD/PAT data formats.
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 *****************************************************************************/

#ifndef MuJet_h
#define MuJet_h

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h" 
#include "FWCore/Framework/interface/Event.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"


namespace ewk {

  class MuJet : public edm::EDAnalyzer {
  public:
    explicit MuJet(const edm::ParameterSet iConfig );
    ~MuJet();
    
    void beginJob();
    void endJob() ;
    
    /// To be called once per event to fill the values for jets
    void analyze(const edm::Event&, const edm::EventSetup& iSetup);
    

  protected:

    /// To be called once per event, to initialize variable values to default
    virtual void init();
    /// Helper function for main constructor
    void SetBranches(); 
    void SetBranch( float* x, std::string name );
    void SetBranch( int* x, std::string name );
    void SetBranch( bool* x, std::string name );
    double ComputeJetSize(int* nCount);
    TTree* tree_;
    edm::InputTag mInputSeedMuon;
    edm::InputTag mInputTrackMuons;

    // Set maximum number of muons in the MuonJet to 20.
    static const int nMuMax = 20;
    std::string fOutputFileName;
    TFile* hOutputFile;

  private:
    // private data members
    float mujetMass;
    float mujetPt;
    float mujetP;
    float mujetPx;
    float mujetPy;
    float mujetPz;
    float mujetRapidity;
    float mujetEta;
    float mujetPhi;
    float mujetTheta;
    int mujetCharge;
    int mujetNmuonsIn01;
    int mujetNmuonsIn02;
    int mujetNmuonsIn03;
    float mujetSize;
    int mujetNumberOfMuons;

    /// muon type - type of the algorithm that reconstructed this muon
    /// multiple algorithms can reconstruct the same muon
    /// GlobalMuon = 1; TrackerMuon = 2; StandAloneMuon = 3; CaloMuon = 4;
    int seedType; 
    int seedCharge;
    /// number of chambers
    int seed_numberOfChambers;
   /// get number of chambers with matched segments
    int seed_numberOfMatches;

    int muType[nMuMax];
    int muCharge[nMuMax];
    int mu_numberOfChambers[nMuMax];
    int mu_numberOfMatches[nMuMax];

    float seedpx;
    float seedpy;
    float seedpz;
    float seedE;
    float seedEt;
    float seedPt;
    float seedEta;
    float seedTheta;
    float seedPhi;
    float seedVx;
    float seedVy;
    float seedVz;
    float seedY;
    float seed_trackiso03;
    float seed_hcaliso03;
    float seed_ecaliso03;
    float seed_trackiso05;
    float seed_hcaliso05;
    float seed_ecaliso05;


    ///////////////////
    float mupx[nMuMax];
    float mupy[nMuMax];
    float mupz[nMuMax];
    float muE[nMuMax];
    float muPt[nMuMax];
    float muEt[nMuMax];
    float muEta[nMuMax];
    float muTheta[nMuMax];
    float muPhi[nMuMax];
    float muVx[nMuMax];
    float muVy[nMuMax];
    float muVz[nMuMax];
    float muY[nMuMax];
    float mu_trackiso03[nMuMax];
    float mu_hcaliso03[nMuMax];
    float mu_ecaliso03[nMuMax];
    float mu_trackiso05[nMuMax];
    float mu_hcaliso05[nMuMax];
    float mu_ecaliso05[nMuMax];

    float muDR[nMuMax];
  };

} //namespace

#endif


