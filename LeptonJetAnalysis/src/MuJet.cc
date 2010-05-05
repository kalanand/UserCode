/*****************************************************************************
 * Project: CMS detector at the CERN
 *
 * class: LeptonJetAnalysis/MuJet
 *
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *   Valerie Halyo, Princeton  - valerieh@princeton.edu
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
#include "ElectroWeakAnalysis/VPlusJets/interface/MuJet.h"
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
  SetBranch( &mujetNmuonsIn01, "mujet_numMuonsInDR02");
  SetBranch( &mujetNmuonsIn03, "mujet_numMuonsInDR03");
  SetBranch( &mujetSize,        "mujetSize");
  SetBranch( &mujetNumberOfMuons, "mujetNumberOfMuons");

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
  SetBranch( &seed_trackiso03,      seedLabel+"_trackiso03" );
  SetBranch( &seed_hcaliso03,       seedLabel+"_hcaliso03" );
  SetBranch( &seed_ecaliso03,       seedLabel+"_ecaliso03" );
  SetBranch( &seed_trackiso05,      seedLabel+"_trackiso05" );
  SetBranch( &seed_hcaliso05,       seedLabel+"_hcaliso05" );
  SetBranch( &seed_ecaliso05,       seedLabel+"_ecaliso05" );
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
  SetBranch( &(mu_trackiso03[0]),      lept+"_trackiso03" );
  SetBranch( &(mu_hcaliso03[0]),       lept+"_hcaliso03" );
  SetBranch( &(mu_ecaliso03[0]),       lept+"_ecaliso03");
  SetBranch( &(mu_trackiso05[0]),      lept+"_trackiso05" );
  SetBranch( &(mu_hcaliso05[0]),       lept+"_hcaliso05" );
  SetBranch( &(mu_ecaliso05[0]),       lept+"_ecaliso05");
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
  mujetNmuonsIn02 = 0;
  mujetNmuonsIn03 = 0;
  mujetSize = 0.0;
  mujetNumberOfMuons = 0;

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
  seed_trackiso03     = 500.0;
  seed_hcaliso03      = 500.0;
  seed_ecaliso03      = 500.0;
  seed_trackiso05     = 500.0;
  seed_hcaliso05      = 500.0;
  seed_ecaliso05      = 500.0;


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
    mu_trackiso03[i]    = 500.0;
    mu_hcaliso03[i]     = 500.0;
    mu_ecaliso03[i]     = 500.0;
    mu_trackiso05[i]    = 500.0;
    mu_hcaliso05[i]     = 500.0;
    mu_ecaliso05[i]     = 500.0;
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
  

  // define occupancy array
  int nCountDR[26];
  for(int i=0; i<26; ++i) nCountDR[i] = 0.0;
  // index 0: count up to dR = 0.1
  // index 1: count up to dR = 0.11
  // index 2: count up to dR = 0.12
  // index 3: count up to dR = 0.13
  // ....
  // index 25: count up to dR = 0.35


  double dRval = 0.0;

  for (edm::View<reco::Muon>::const_iterator mu = trackMuons->begin (); 
       mu != trackMuons->end (); ++mu) {

    // ******** Require muon pt > 1 GeV.
    if( mu->pt() < 1.0 ) continue;
    dRval = reco::deltaR( mu->eta(), mu->phi(), seed->eta(), seed->phi() );

  // *******  Make sure not to count the seed itself: dR>0.005
    if( dRval < 0.005 ) continue;

    // Increment the appropriate occupancy elements 
    for(int i=0; i<26; ++i) {
      if( dRval < (0.1 + 0.01*i) ) nCountDR[i]++;
    }
  }

  if( nCountDR[0]==0 ) return; // ****** no muon found


  // ******* Now apply our algorithm $$$$$$$$$$
  double mujetSize = ComputeJetSize(nCountDR); 
  mujetNmuonsIn01 = nCountDR[0];
  mujetNmuonsIn02 = nCountDR[10];
  mujetNmuonsIn03 = nCountDR[20];

  ////////// fill seed variables //////////////

    seedCharge           = seed->charge();
    seedVx               = seed->vx();
    seedVy               = seed->vy();
    seedVz               = seed->vz();
    seedY                = seed->rapidity();
    /// isolation 
    seed_trackiso03       = seed->isolationR03().sumPt;
    seed_ecaliso03        = seed->isolationR03().emEt;
    seed_hcaliso03        = seed->isolationR03().hadEt;
    seed_trackiso05       = seed->isolationR05().sumPt;
    seed_ecaliso05        = seed->isolationR05().emEt;
    seed_hcaliso05        = seed->isolationR05().hadEt;

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
      
      dRval = reco::deltaR((float)mu->eta(), (float)mu->phi(), 
			   seed->eta(), seed->phi());	
      
      if( !( dRval>0.005 && dRval<mujetSize && i<nMuMax) ) continue;
      
      ++i;
      muDR[i]              = dRval;
      muCharge[i]          = mu->charge();
      muVx[i]              = mu->vx();
      muVy[i]              = mu->vy();
      muVz[i]              = mu->vz();
      muY[i]               = mu->rapidity();
      /// isolation 
      mu_trackiso03[i]       = mu->isolationR03().sumPt;
      mu_ecaliso03[i]        = mu->isolationR03().emEt;
      mu_hcaliso03[i]        = mu->isolationR03().hadEt;
    
      mu_trackiso05[i]       = mu->isolationR05().sumPt;
      mu_ecaliso05[i]        = mu->isolationR05().emEt;
      mu_hcaliso05[i]        = mu->isolationR05().hadEt;
    
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


    mujetNumberOfMuons = i; 

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

}




////////////////// compute jet size given the occupancy array ///////////////////

// Now count the number of muons in dR of cone 0.1 from the seed.  
// -- if we do not find any muon then STOP => Nothing to do 
// -- if we find a muon then increment the cone size by 0.01 and count again
//   * found a new muon ? => increment dR in the steps of 0.01 and continue the iteration
//   * didn't find new muon ? => search in dR=0.11, then in dR=0.12, and finally in dR=0.15
//        => still no new muon ? => STOP and report "muon jet size" = 0.1
//        => if found a new muon in either of the previous three steps, then 
//           increment dR by 0.01 and repeat the above process. Report the jet size.
// -- finally, stop at dR=0.33 in any case. [we want to go up to cone size 0.3 
//                                            + 10% for resolution]   

double ewk::MuJet::ComputeJetSize(int* nCount) {

  double jetSize = 0.1;
  double tempjetSize = 0.1;
  unsigned int i =0;
  unsigned int ilast =0;


  while( i < (sizeof(nCount)/sizeof(nCount[0]) - 5) ) {

    tempjetSize = 0.1 + 0.01*i;

    if( nCount[i]==nCount[i+1] && nCount[i]==nCount[i+2] && 
	nCount[i]==nCount[i+5] ) { 
      if( nCount[i]>nCount[ilast] ) jetSize = tempjetSize;
      ilast = i;
      i += 5;       
    }
    else { 
      ilast = i;
      ++i; 
    }
  }

  return jetSize;

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
