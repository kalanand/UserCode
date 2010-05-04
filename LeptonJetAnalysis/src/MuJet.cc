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

// CMS includes
#include "DataFormats/MuonReco/interface/Muon.h"  
#include "TMath.h" 
#include "TLorentzVector.h"

// Header file
#include "ElectroWeakAnalysis/LeptonJetAnalysis/interface/MuJet.h"
#include "DataFormats/Math/interface/deltaR.h" // reco::deltaR

ewk::MuJet::MuJet(const edm::ParameterSet iConfig)
{
  // ********** Input parameters ********** //
  mInputSeedMuon = iConfig.getParameter<edm::InputTag>("SeedMuon"); 
  mInputTrackMuons   = iConfig.getParameter<edm::InputTag>("TrackMuons");
  fOutputFileName = iConfig.getParameter<std::string>("OutputFile");
}



// destructor:does nothing
ewk::MuJet::~MuJet() {}



void ewk::MuJet::beginJob() {

  hOutputFile = new TFile( fOutputFileName.c_str(), "RECREATE" );

  // initiliaze the tree
  tree_     = new TTree("MuJet","MuJet Tree");

  // Declare all the branches of the tree
  SetBranches();
}





void ewk::MuJet::endJob()
{
  hOutputFile->SetCompressionLevel(2);
  hOutputFile->cd();
  tree_->Write();

  delete tree_;
  hOutputFile->Close();
  delete hOutputFile;
}




void ewk::MuJet::SetBranches()
{
  // Declare jet branches
  std::string seedLabel = "seed";
  std::string lept = "mu";


  SetBranch( &mujetMass, "mujet_Mass");
  SetBranch( &mujetPt   , "mujet_Pt");
  SetBranch( &mujetP    , "mujet_P");
  SetBranch( &mujetPx    , "mujet_Px");
  SetBranch( &mujetPy    , "mujet_Py");
  SetBranch( &mujetPz    , "mujet_Pz");
  SetBranch( &mujetRapidity, "mujet_Rapidity");
  SetBranch( &mujetEta    , "mujet_Eta");
  SetBranch( &mujetPhi    , "mujet_Phi");
  SetBranch( &mujetTheta    , "mujet_Theta");
  SetBranch( &mujetCharge   , "mujet_Charge");
  SetBranch( &mujetNmuonsIn01, "mujet_numMuonsInDR01");
  SetBranch( &mujetNmuonsIn03, "mujet_numMuonsInDR03");


  ///////////////////////////////////////////////
  SetBranch( &seedpx,             seedLabel+"_px" );
  SetBranch( &seedpy,             seedLabel+"_py" );
  SetBranch( &seedpz,             seedLabel+"_pz" );
  SetBranch( &seedE,              seedLabel+"_e" );
  SetBranch( &seedPt,             seedLabel+"_pt" );
  SetBranch( &seedEt,             seedLabel+"_et" );
  SetBranch( &seedEta,            seedLabel+"_eta" ); 
  SetBranch( &seedTheta,          seedLabel+"_theta" ); 
  SetBranch( &seedPhi,            seedLabel+"_phi" );
  SetBranch( &seedCharge,         seedLabel+"_charge" );
  SetBranch( &seedVx,             seedLabel+"_vx" );
  SetBranch( &seedVy,             seedLabel+"_vy" );
  SetBranch( &seedVz,             seedLabel+"_vz" );
  SetBranch( &seedY,              seedLabel+"_y" );
  SetBranch( &seed_trackiso,      seedLabel+"_trackiso" );
  SetBranch( &seed_hcaliso,       seedLabel+"_hcaliso" );
  SetBranch( &seed_ecaliso,       seedLabel+"_ecaliso" );
  SetBranch( &seedType, seedLabel+"_type" );
  SetBranch( &seed_numberOfChambers, seedLabel+"_numberOfChambers" );
  SetBranch( &seed_numberOfMatches,  seedLabel+"_numberOfMatches" );	  
  ////////////////////////////////////////////////////////
	  
  SetBranch( &(mupx[0]),             lept+"_px" );
  SetBranch( &(mupy[0]),             lept+"_py" );
  SetBranch( &(mupz[0]),             lept+"_pz" );
  SetBranch( &(muE[0]),              lept+"_e" );
  SetBranch( &(muPt[0]),             lept+"_pt" );
  SetBranch( &(muEt[0]),             lept+"_et" );
  SetBranch( &(muEta[0]),            lept+"_eta" ); 
  SetBranch( &(muTheta[0]),          lept+"_theta" );    
  SetBranch( &(muPhi[0]),            lept+"_phi" );
  SetBranch( &(muCharge[0]),         lept+"_charge" );
  SetBranch( &(muVx[0]),             lept+"_vx" );
  SetBranch( &(muVy[0]),             lept+"_vy" );
  SetBranch( &(muVz[0]),             lept+"_vz" );
  SetBranch( &(muY[0]),              lept+"_y" );
  SetBranch( &(mu_trackiso[0]),      lept+"_trackiso" );
  SetBranch( &(mu_hcaliso[0]),       lept+"_hcaliso" );
  SetBranch( &(mu_ecaliso[0]),       lept+"_ecaliso");
  SetBranch( &(muType[0]), lept+"_type" );
  SetBranch( &(mu_numberOfChambers[0]), lept+"_numberOfChambers" );
  SetBranch( &(mu_numberOfMatches[0]),  lept+"_numberOfMatches" );
  SetBranch( &(muDR[0]),  lept+"_dRFromSeed" );
	  
}
/////////////////////////////////////////////////////////////////////////






void ewk::MuJet::init()   
{
  // initialize private data members

  mujetMass = -1.;
  mujetPt   = -1.;
  mujetP    = -1.;
  mujetPx    = -99999.;
  mujetPy    = -99999.;
  mujetPz    = -99999.;
  mujetRapidity    = -10.;
  mujetEta    = -10.;
  mujetPhi    = -10.;
  mujetTheta    = -99999.;
  mujetCharge   = -10;
  mujetNmuonsIn01 = 0;
  mujetNmuonsIn03 = 0;

  seedType   = -1; 
  seedCharge           = -10;
  seedpx               = -99999.;
  seedpy               = -99999.;
  seedpz               = -99999.;
  seedE                = -1.;
  seedEt               = -1.;
  seedPt               = -1.;
  seedEta              = -10.;
  seedTheta            = -99999.;
  seedPhi              = -10.;
  seedVx               = -10.;
  seedVy               = -10.;
  seedVz               = -10.;
  seedY                = -10.;
  seed_numberOfChambers   = -10.;
  seed_numberOfMatches    = -1.;
  seed_trackiso     = 500.0;
  seed_hcaliso      = 500.0;
  seed_ecaliso      = 500.0;

  //////////////
  for(int i=0; i< nMuMax; ++i) {
    muType[i]  = -1;
    muCharge[i]          = -10;	  
    mupx[i]              = -99999.;
    mupy[i]              = -99999.;
    mupz[i]              = -99999.;
    muE[i]               = -1.;
    muPt[i]              = -1.;
    muEt[i]              = -1.;
    muEta[i]             = -10.;
    muTheta[i]           = -99999.;
    muPhi[i]             = -10.;
    muVx[i]              = -10.;
    muVy[i]              = -10.;
    muVz[i]              = -10.;
    muY[i]               = -10.;
    mu_numberOfChambers[i]   = -10.;
    mu_numberOfMatches[i]    = -1.;
    mu_trackiso[i]    = 500.0;
    mu_hcaliso[i]     = 500.0;
    mu_ecaliso[i]     = 500.0;
    muDR[i]     = 500.0;
  }
  // initialization done
}






void ewk::MuJet::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // protection
  if( (tree_==0) )  return;

  // first initialize to the default values
  init();

  edm::Handle<edm::View<reco::Muon> > seedmuon;
  iEvent.getByLabel( mInputSeedMuon, seedmuon);
  if( seedmuon->size()<1 ) return; // Nothing to fill
  

  // seed is the highest pt muon in this collection.
  // require seed pt > 10 GeV
  const reco::Muon *seed = &((*seedmuon)[0]); 
  if( seed == 0) return;
  if(seed->pt() < 10.0 ) return;


  edm::Handle<edm::View<reco::Muon> > trackMuons;
  iEvent.getByLabel( mInputTrackMuons, trackMuons);
  if( trackMuons->size()<1 ) return; // Nothing to fill
  

  // Now count the number of muons in cone 0.1 and 0.33 
  // around the seed. Also, require muon pt > 1 GeV.
  // make sure not to count the seed itself: dr > 0.01 

  int muCountDR01 =0;
  int muCountDR03 =0;
  for (edm::View<reco::Muon>::const_iterator mu = trackMuons->begin (); 
       mu != trackMuons->end (); ++mu) {

    if( mu->pt() < 1.0 ) continue;

    double dRval = reco::deltaR((float)mu->eta(), (float)mu->phi(), 
				seed->eta(), seed->phi());	
    if ( dRval > 0.01 && dRval < 0.1) muCountDR01++;
    if( muCountDR01>0 && dRval > 0.01 && dRval < 0.33 ) muCountDR03++;
  }

  if(muCountDR03<1) return; // no other muon found



  ////////// fill seed variables //////////////

    seedCharge           = seed->charge();
    seedVx               = seed->vx();
    seedVy               = seed->vy();
    seedVz               = seed->vz();
    seedY                = seed->rapidity();
    /// isolation 
    seed_trackiso       = seed->isolationR03().sumPt;
    seed_ecaliso        = seed->isolationR03().emEt;
    seed_hcaliso        = seed->isolationR03().hadEt;

    seedType  = seed->type();
    seed_numberOfChambers  = seed->numberOfChambers();      
    seed_numberOfMatches   = seed->numberOfMatches();
    seedTheta          = seed->theta();
    seedEta            = seed->eta();    
    seedPhi            = seed->phi();
    seedE              = seed->energy();
    seedpx             = seed->px();
    seedpy             = seed->py();
    seedpz             = seed->pz();
    seedPt             = seed->pt();
    seedEt             = seed->et();	  


  ////////// fill muon related quantities //////////////

    // vector sum of momentum and charge
    TLorentzVector MujetP4( 0, 0, 0, 0);
    mujetCharge = 0.0;

  int i =0;
  for (edm::View<reco::Muon>::const_iterator mu = trackMuons->begin (); 
       mu != trackMuons->end (); ++mu) {

    double dRval = reco::deltaR((float)mu->eta(), (float)mu->phi(), 
				seed->eta(), seed->phi());	
    
    if( !( dRval>0.01 && dRval<0.33 && i<nMuMax) ) continue;
    
    ++i;
    muDR[i]              = dRval;
    muCharge[i]          = mu->charge();
    muVx[i]              = mu->vx();
    muVy[i]              = mu->vy();
    muVz[i]              = mu->vz();
    muY[i]               = mu->rapidity();
    /// isolation 
    mu_trackiso[i]       = mu->isolationR03().sumPt;
    mu_ecaliso[i]        = mu->isolationR03().emEt;
    mu_hcaliso[i]        = mu->isolationR03().hadEt;
    
    muType[i]            = mu->type();
    mu_numberOfChambers[i] = mu->numberOfChambers();
    mu_numberOfMatches[i]  = mu->numberOfMatches();
    muTheta[i]          = mu->theta();
    muEta[i]            = mu->eta();    
    muPhi[i]            = mu->phi();
    muE[i]              = mu->energy();
    mupx[i]             = mu->px();
    mupy[i]             = mu->py();
    mupz[i]             = mu->pz();
    muPt[i]             = mu->pt();
    muEt[i]             = mu->et();

    MujetP4 += TLorentzVector( mu->px(), mu->py(), mu->pz(), mu->energy() ); 
    mujetCharge += mu->charge();
  }

  mujetMass = MujetP4.M();
  mujetPt   = MujetP4.Perp();
  mujetP    = MujetP4.Mag();

  mujetPx    = MujetP4.Px();
  mujetPy    = MujetP4.Py();
  mujetPz    = MujetP4.Pz();

  mujetRapidity    = MujetP4.Rapidity();
  mujetEta    = MujetP4.PseudoRapidity();
  mujetPhi    = MujetP4.Phi();
  mujetTheta    = MujetP4.Theta();
  mujetNmuonsIn01 = muCountDR01;
  mujetNmuonsIn03 = muCountDR03;
}





////////////////// utilities, helpers ///////////////////
 
void ewk::MuJet::SetBranch( float* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"/F").c_str() );
}


void ewk::MuJet::SetBranch( int* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"/I").c_str() );
}


void ewk::MuJet::SetBranch( bool* x, std::string name)
{
  tree_->Branch( name.c_str(), x, ( name+"/O").c_str() );
}


// declare this class as a plugin
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
using ewk::MuJet;
DEFINE_FWK_MODULE(MuJet);
