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
                (30: showering nbrem=0, 31: showering nbrem=1, 
                 32: showering nbrem=2 ,33: showering nbrem=3, 3
                  4: showering nbrem>=4)
                 40: crack
      endcaps : 100: golden, 110: bigbrem, 120: narrow, 130-134: showering
               (130: showering nbrem=0, 131: showering nbrem=1, 
                132: showering nbrem=2 ,133: showering nbrem=3, 
                134: showering nbrem>=4)
   */

// user include files
#include "JetMETCorrections/ZJet/interface/ZJetAnalysis.h" 
#include "JetMETCorrections/ZJet/src/JetUtilMC.h"

#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "SimDataFormats/JetMatching/interface/MatchedPartons.h"
#include "SimDataFormats/JetMatching/interface/JetMatchedPartons.h"

#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"
#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/ElectronTkIsolation.h"
//#include "PhysicsTools/Utilities/interface/deltaR.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"



namespace cms
{
  ZJetAnalysis::ZJetAnalysis(const edm::ParameterSet& iConfig)
  {
    // get names of modules, producing object collections

    // ********** CaloJets ********** //
    vector< edm::InputTag > dCaloJets;
    dCaloJets.push_back( edm::InputTag("iterativeCone5CaloJets") );
    mInputCalo 
      = iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("srcCalo", 
								   dCaloJets);
    
    // ********** Corrected CaloJets ********** //
    vector< edm::InputTag > dCaloCorJets;
    dCaloCorJets.push_back( edm::InputTag("L3JetCorJetIcone5") );
    mInputCor  = 
      iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("srcCaloCor", 
								 dCaloCorJets);


    // ********** Tracks ********** //
    const edm::InputTag defaultTracks("generalTracks");
    trackTags_ = iConfig.getUntrackedParameter<edm::InputTag>("tracks",
							      defaultTracks);

    zee_         =  iConfig.getParameter<edm::InputTag>("zee");
    electrons_         =  iConfig.getParameter<edm::InputTag>("electrons");

    // Are we running over Monte Carlo ?
    runningOverMC_    = iConfig.getUntrackedParameter< bool >("runningOverMC",false);

    if(runningOverMC_) {

      // ********** GenJets ********** //
      vector< edm::InputTag > dGenJets;
      dGenJets.push_back( edm::InputTag("iterativeCone5GenJets") );
      mInputGen  = 
	iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("srcGen", 
								   dGenJets);
    }

    trackInfo_ = iConfig.getUntrackedParameter< bool >("trackInfo",false);

    // Jet Flavor identification
    doJetFlavorIdentification_ = 
      iConfig.getUntrackedParameter< bool >("doJetFlavorIdentification",false);
    if(doJetFlavorIdentification_) {
      sourceByValue_ = iConfig.getParameter<InputTag> ("srcByValue");
    }


    // get name of output file with histogramms
    fOutputFileName = iConfig.getUntrackedParameter<string>("HistOutFile",
							    "analysis_zjet.root"); 
  }






  ZJetAnalysis::~ZJetAnalysis()
  {   
  }




  void ZJetAnalysis::beginJob( const edm::EventSetup& iSetup)
  {
    // Open output ROOT file and TTree
    hOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ; 
    myTree = new TTree("ZJet","ZJet Tree");


    // Declare all the branches of the tree
    declareTreeBranches();

    NumRecoJets = 0;
    NumGenJets = 0;
    NumRecoTrack = 0;
    NumPart = 0;

    // create all the histograms
    createHistograms();
  }








  // ------------ method called to produce the data  ------------
  void
  ZJetAnalysis::analyze(const edm::Event& iEvent, 
			const edm::EventSetup& iSetup) {
    using namespace edm;

    setDefaultValues();

    // write HEPEVT block into file
    run = iEvent.id().run();
    event = iEvent.id().event();

    edm::Handle< double > genEventScale_;
    edm::Handle< double > genEventWeight_;
    if( runningOverMC_ ) {
      iEvent.getByLabel("genEventScale", genEventScale_);
      genEventScale = (float)(*genEventScale_);
      iEvent.getByLabel("genEventWeight", genEventWeight_);
      genEventWeight = (float)(*genEventWeight_);
    }

    // Calorimeter geometry
   edm::ESHandle<CaloGeometry> geoHandle;
   iSetup.get<CaloGeometryRecord>().get(geoHandle);
   geometry_ = geoHandle.product();
 


    // *********** Load Calo Jets ******************
    StoreCaloJetInformation (iEvent);
    StoreCorJetInformation (iEvent);


    /**  Store reconstructed  Z--> e+e-  information */
    StoreRecoZeeInformation (iEvent, iSetup);



    if( runningOverMC_ ) {

      // *********** Load Gen Jets ******************
      StoreGenJetInformation (iEvent);
      /**  Store generated  Z--> e+e-  information */
      StoreGenZeeInformation (iEvent);
    }


    myTree->Fill();

    // fill all the histograms to be saved
    fillAllHistograms();

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
    edm::Handle<reco::JetFlavourMatchingCollection> theTagByValue; 
    if(doJetFlavorIdentification_) {
      iEvent.getByLabel (sourceByValue_ , theTagByValue );   
    }   
    /****************    Jet Flavor    ***************/


    std::vector<edm::InputTag>::const_iterator ic;
    int jettype = 0;
    NumRecoJets = 0; 
    NumRecoJetAlgo = 0;

    for (ic=mInputCalo.begin(); ic!=mInputCalo.end(); ic++) {
      edm::Handle<reco::CaloJetCollection> jets;
      iEvent.getByLabel(*ic, jets);
      if (jets.isValid()) {

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

	  if(doJetFlavorIdentification_) {
	    int flavor = 100;
	    for ( JetFlavourMatchingCollection::const_iterator jfm  = 
		    theTagByValue->begin();
		  jfm != theTagByValue->end(); jfm ++ ) {
	      RefToBase<Jet> aJet  = (*jfm).first;   
	      const JetFlavour aFlav = (*jfm).second;
	      double dist = radius(aJet->eta(), aJet->phi(), 
				   (*jet).eta(), (*jet).phi());
	      if( dist < 0.0001 ) flavor = aFlav.getFlavour(); 
	    }
	    JetRecoFlavor[jettype][ij] = flavor;
	  }

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
      if (jets.isValid()) {

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
      if (jets.isValid()) {

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

  void  ZJetAnalysis::StoreRecoZeeInformation (const edm::Event& iEvent, 
					       const edm::EventSetup& iSetup) {

    edm::Handle<CandidateView> zee;
    iEvent.getByLabel(zee_, zee);

    edm::Handle<reco::GsfElectronCollection> electrons;
    iEvent.getByLabel(electrons_, electrons);

    if( zee->size()<1 || electrons->size()<2 ) return;
    const Candidate *ZeeReco = &((*zee)[0]);
    const Candidate *ep = ZeeReco->daughter( 0 );
    const Candidate *em = ZeeReco->daughter( 1 );
    const reco::GsfElectron* eplusReco(0);
    const reco::GsfElectron* eminusReco(0);


    for(size_t j = 0; j < electrons->size(); ++ j) {
      const reco::PixelMatchGsfElectron&  x = (*electrons)[j];

      // get electron ID info
      edm::Ref<reco::GsfElectronCollection> electronRef(electrons, j);
      bool id1 = isElectronId( iEvent, electronRef, "eidRobustLoose");
      bool id2 = isElectronId( iEvent, electronRef, "eidRobustTight");
      bool id3 = isElectronId( iEvent, electronRef, "eidLoose");
      bool id4 = isElectronId( iEvent, electronRef, "eidTight");
      if( overlap( x,*ep)) { 
	eplusReco  = &x; 
	isePlusRobustLoose = id1; 
	isePlusRobustTight = id2;
	isePlusLoose       = id3;
	isePlusTight       = id4; 
      }
      if( overlap( x,*em) ) { 
	eminusReco = &x; 
	iseMinusRobustLoose = id1; 
	iseMinusRobustTight = id2;
	iseMinusLoose       = id3;
	iseMinusTight       = id4; 
      }
    }


    if( ZeeReco != 0 && eplusReco != 0 && eminusReco != 0 ) { 

      ////////// e+ quantities //////////////
      ePlusCharge =eplusReco-> charge();
      ePlusVx = eplusReco->vx();
      ePlusVy = eplusReco->vy();
      ePlusVz = eplusReco->vz();
      ePlusClassification = eplusReco->classification();
      ePlus_InvEMinusInvP = 0.0;
      if(!(eplusReco->caloEnergy()==0 || eplusReco->trackMomentumAtVtx().R()==0))	    
	ePlus_InvEMinusInvP = ((1./eplusReco->caloEnergy()) - 
			       (1./eplusReco->trackMomentumAtVtx().R()));
	  
      ePlus_BremFraction  = (eplusReco->trackMomentumAtVtx().R() - 
			     eplusReco->trackMomentumOut().R());
      ePlus_DeltaEtaIn    = eplusReco->deltaEtaSuperClusterTrackAtVtx();
      ePlus_DeltaPhiIn    = eplusReco->deltaPhiSuperClusterTrackAtVtx();
      ePlus_DeltaPhiOut   = eplusReco->deltaPhiSeedClusterTrackAtCalo();
      ePlus_DeltaEtaOut   = eplusReco->deltaEtaSeedClusterTrackAtCalo();
      //Track Momentum information
      ePlus_Trackmom_calo = sqrt(eplusReco->trackMomentumAtCalo().perp2());
      ePlus_Trackmom_vtx  = sqrt(eplusReco->trackMomentumAtVtx().perp2());
      //get Hcal energy over Ecal Energy
      ePlus_HoverE = eplusReco->hadronicOverEm();	  
      //vertex info for electron
      ePlus_vx = eplusReco->vertex().x();
      ePlus_vy = eplusReco->vertex().y();
      ePlus_vz = eplusReco->vertex().z();	  
      //Transvers Impact Parameter
      ePlus_td0 = sqrt(ePlus_vx*ePlus_vx + ePlus_vy*ePlus_vy);
      ePlus_d0  = sqrt(ePlus_vx*ePlus_vx + ePlus_vy*ePlus_vy + ePlus_vz*ePlus_vz);
      ePlus_sc_no         = eplusReco->numberOfClusters();
      ePlus_EoverPout     = eplusReco->eSeedClusterOverPout();
      ePlus_EoverPin      = eplusReco->eSuperClusterOverP();

      //get SuperCluster (sc) infos
      reco::SuperClusterRef SCp = eplusReco->superCluster();
      ePlus_sc_x = SCp->x();
      ePlus_sc_y = SCp->y();
      ePlus_sc_z = SCp->z();
      ePlus_sc_R = sqrt(ePlus_sc_x*ePlus_sc_x + 
			ePlus_sc_y*ePlus_sc_y + 
			ePlus_sc_z*ePlus_sc_z);
      ePlus_sc_Rt = sqrt(ePlus_sc_x*ePlus_sc_x + 
			 ePlus_sc_y*ePlus_sc_y);
      ePlusTheta = SCp->position().Theta();
      ePlusEta   = SCp->eta();    
      ePlusPhi   = SCp->phi();
      ePlusE     = SCp->energy();
      ePluspx    = ePlusE * sin(ePlusTheta) * cos(ePlusPhi);
      ePluspy    = ePlusE * sin(ePlusTheta) * sin(ePlusPhi);
      ePluspz    = ePlusE * cos(ePlusTheta);
      ePlusPt    = ePlusE / cosh(ePlusEta);
      ePlusEt    = ePlusPt;
	  

      const edm::InputTag ebRecHit("reducedEcalRecHitsEB");
      const edm::InputTag eeRecHit("reducedEcalRecHitsEE");

      EcalClusterLazyTools lazyTools( iEvent, iSetup, ebRecHit, eeRecHit); 
      std::vector<float> vCov = lazyTools.covariances(*(SCp->seed()));
      ePlus_E9overE25       = lazyTools.e3x3(*(SCp->seed()))/
	lazyTools.e5x5(*(SCp->seed()));
      ePlus_SigmaEtaEta     = sqrt(vCov[0]);
      ePlus_SigmaEtaPhi     = sqrt(vCov[1]);
      ePlus_SigmaPhiPhi     = sqrt(vCov[2]);
	 

      //get track informations
      float dz = 0.1;
      if(trackInfo_) {
	reco::TrackRef Trp = eplusReco->track();
	ePlus_trk_eta    = Trp->eta();
	ePlus_trk_phi    = Trp->phi();
	ePlus_trk_etaout = Trp->outerEta();
	ePlus_trk_phiout = Trp->outerPhi();
	ePlus_trk_p      = Trp->p();
	ePlus_trk_pout   = Trp->outerP();
	ePlus_trk_pt     = Trp->pt();
	ePlus_trk_charge = Trp->charge();
	ePlus_trk_chi2   = Trp->chi2();
	ePlus_trk_ndof   = Trp->ndof();
	ePlus_trk_hits   = Trp->numberOfValidHits();
	ePlus_trk_d0     = Trp->d0();
	ePlus_trk_d0Err  = Trp->d0Error();
	dz = Trp->dz();
      }

      ///////////////////
      TrackIsolation( iEvent, ePlusEta, ePlusPhi, 
		      dz, 0.15, ePlus_ptiso_15, ePlus_ntrkiso_15 );
      TrackIsolation( iEvent, ePlusEta, ePlusPhi,  
		      dz, 0.20, ePlus_ptiso_20, ePlus_ntrkiso_20 );
      TrackIsolation( iEvent, ePlusEta, ePlusPhi,  
		      dz, 0.25, ePlus_ptiso_25, ePlus_ntrkiso_25 );
      TrackIsolation( iEvent, ePlusEta, ePlusPhi, 
		      dz, 0.30, ePlus_ptiso_30, ePlus_ntrkiso_30 );
      TrackIsolation( iEvent, ePlusEta, ePlusPhi, 
		      dz, 0.35, ePlus_ptiso_35, ePlus_ntrkiso_35 );
      TrackIsolation( iEvent, ePlusEta, ePlusPhi, 
		      dz, 0.40, ePlus_ptiso_40, ePlus_ntrkiso_40 );
      TrackIsolation( iEvent, ePlusEta, ePlusPhi, 
		      dz, 0.45, ePlus_ptiso_45, ePlus_ntrkiso_45 );
      TrackIsolation( iEvent, ePlusEta, ePlusPhi, 
		      dz, 0.50, ePlus_ptiso_50, ePlus_ntrkiso_50 );
      ePlus_hcaliso_10 
	= HCALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.10);
      ePlus_hcaliso_15 
	= HCALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.15);
      ePlus_hcaliso_20 
	= HCALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.20);
      ePlus_hcaliso_25 
	= HCALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.25);
      ePlus_hcaliso_30 
	= HCALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.30);
      ePlus_hcaliso_35 
	= HCALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.35);
      ePlus_hcaliso_40 
	= HCALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.40);
      ePlus_hcaliso_45 
	= HCALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.45);
      ePlus_hcaliso_50 
	= HCALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.50);
      ePlus_ecaliso_10 
	= ECALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.10);
      ePlus_ecaliso_15 
	= ECALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.15);
      ePlus_ecaliso_20 
	= ECALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.20);
      ePlus_ecaliso_25 
	= ECALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.25);
      ePlus_ecaliso_30 
	= ECALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.30);
      ePlus_ecaliso_35 
	= ECALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.35);
      ePlus_ecaliso_40 
	= ECALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.40);
      ePlus_ecaliso_45 
	= ECALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.45);
      ePlus_ecaliso_50 
	= ECALIsolation(iEvent, ePlusEta, ePlusPhi, ePlusE, 0.50);



      ////////// e- quantities //////////////
      eMinusCharge = eminusReco->charge();
      eMinusVx = eminusReco->vx();
      eMinusVy = eminusReco->vy();
      eMinusVz = eminusReco->vz();
      eMinusClassification = eminusReco->classification();
      eMinus_sc_no    = eminusReco->numberOfClusters();
      eMinus_EoverPout = eminusReco->eSeedClusterOverPout();
      eMinus_EoverPin  = eminusReco->eSuperClusterOverP();
      eMinus_InvEMinusInvP = 0.0;
      if(!(eminusReco->caloEnergy()==0 || eminusReco->trackMomentumAtVtx().R()==0))	    
	eMinus_InvEMinusInvP = ((1./eminusReco->caloEnergy()) - 
			       (1./eminusReco->trackMomentumAtVtx().R()));
	  
      eMinus_BremFraction  = (eminusReco->trackMomentumAtVtx().R() - 
			     eminusReco->trackMomentumOut().R());
      eMinus_DeltaEtaIn    = eminusReco->deltaEtaSuperClusterTrackAtVtx();
      eMinus_DeltaPhiIn    = eminusReco->deltaPhiSuperClusterTrackAtVtx();
      eMinus_DeltaPhiOut   = eminusReco->deltaPhiSeedClusterTrackAtCalo();
      eMinus_DeltaEtaOut   = eminusReco->deltaEtaSeedClusterTrackAtCalo();
      //Track Momentum information
      eMinus_Trackmom_calo = sqrt(eminusReco->trackMomentumAtCalo().perp2());
      eMinus_Trackmom_vtx  = sqrt(eminusReco->trackMomentumAtVtx().perp2());	  
      //get Hcal energy over Ecal Energy
      eMinus_HoverE = eminusReco->hadronicOverEm();	  
      //vertex info for electron
      eMinus_vx = eminusReco->vertex().x();
      eMinus_vy = eminusReco->vertex().y();
      eMinus_vz = eminusReco->vertex().z();	  
      //Transvers Impact Parameter
      eMinus_td0 = sqrt(eMinus_vx*eMinus_vx + eMinus_vy*eMinus_vy);
      eMinus_d0  = sqrt(eMinus_vx*eMinus_vx + eMinus_vy*eMinus_vy + eMinus_vz*eMinus_vz);



      //get SuperCluster (sc) infos
      reco::SuperClusterRef SCm = eminusReco->superCluster();
      eMinus_sc_x = SCm->x();
      eMinus_sc_y = SCm->y();
      eMinus_sc_z = SCm->z();
      eMinus_sc_R = sqrt(eMinus_sc_x*eMinus_sc_x + 
			 eMinus_sc_y*eMinus_sc_y + 
			 eMinus_sc_z*eMinus_sc_z);
      eMinus_sc_Rt = sqrt(eMinus_sc_x*eMinus_sc_x + 
			  eMinus_sc_y*eMinus_sc_y);
      eMinusTheta = SCm->position().Theta();
      eMinusEta = SCm->eta();    
      eMinusPhi = SCm->phi();
      eMinusE   = SCm->energy();
      eMinuspx    = eMinusE * sin(eMinusTheta) * cos(eMinusPhi);
      eMinuspy    = eMinusE * sin(eMinusTheta) * sin(eMinusPhi);
      eMinuspz    = eMinusE * cos(eMinusTheta);
      eMinusPt    = eMinusE / cosh(eMinusEta);
      eMinusEt    = eMinusPt;

      std::vector<float> vCov2 = lazyTools.covariances(*(SCm->seed()));
      ePlus_E9overE25       = lazyTools.e3x3(*(SCm->seed()))/
	lazyTools.e5x5(*(SCm->seed()));
      eMinus_SigmaEtaEta     = sqrt(vCov2[0]);
      eMinus_SigmaEtaPhi     = sqrt(vCov2[1]);
      eMinus_SigmaPhiPhi     = sqrt(vCov2[2]);
	 

      //get track informations
      if(trackInfo_) {
	reco::TrackRef Trm = eminusReco->track();
	eMinus_trk_eta    = Trm->eta();
	eMinus_trk_phi    = Trm->phi();
	eMinus_trk_etaout = Trm->outerEta();
	eMinus_trk_phiout = Trm->outerPhi();
	eMinus_trk_p      = Trm->p();
	eMinus_trk_pout   = Trm->outerP();
	eMinus_trk_pt     = Trm->pt();
	eMinus_trk_charge = Trm->charge();
	eMinus_trk_chi2   = Trm->chi2();
	eMinus_trk_ndof   = Trm->ndof();
	eMinus_trk_hits   = Trm->numberOfValidHits();
	eMinus_trk_d0     = Trm->d0();
	eMinus_trk_d0Err  = Trm->d0Error();
	dz = Trm->dz();
      }

      ///////////////////
      TrackIsolation( iEvent, eMinusEta, eMinusPhi, 
		      dz, 0.15, eMinus_ptiso_15, eMinus_ntrkiso_15 );
      TrackIsolation( iEvent, eMinusEta, eMinusPhi,  
		      dz, 0.20, eMinus_ptiso_20, eMinus_ntrkiso_20 );
      TrackIsolation( iEvent, eMinusEta, eMinusPhi,  
		      dz, 0.25, eMinus_ptiso_25, eMinus_ntrkiso_25 );
      TrackIsolation( iEvent, eMinusEta, eMinusPhi, 
		      dz, 0.30, eMinus_ptiso_30, eMinus_ntrkiso_30 );
      TrackIsolation( iEvent, eMinusEta, eMinusPhi, 
		      dz, 0.35, eMinus_ptiso_35, eMinus_ntrkiso_35 );
      TrackIsolation( iEvent, eMinusEta, eMinusPhi, 
		      dz, 0.40, eMinus_ptiso_40, eMinus_ntrkiso_40 );
      TrackIsolation( iEvent, eMinusEta, eMinusPhi, 
		      dz, 0.45, eMinus_ptiso_45, eMinus_ntrkiso_45 );
      TrackIsolation( iEvent, eMinusEta, eMinusPhi, 
		      dz, 0.50, eMinus_ptiso_50, eMinus_ntrkiso_50 );
      eMinus_hcaliso_10 
	= HCALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.10);
      eMinus_hcaliso_15 
	= HCALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.15);
      eMinus_hcaliso_20 
	= HCALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.20);
      eMinus_hcaliso_25 
	= HCALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.25);
      eMinus_hcaliso_30 
	= HCALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.30);
      eMinus_hcaliso_35 
	= HCALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.35);
      eMinus_hcaliso_40 
	= HCALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.40);
      eMinus_hcaliso_45 
	= HCALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.45);
      eMinus_hcaliso_50 
	= HCALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.50);
      eMinus_ecaliso_10 
	= ECALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.10);
      eMinus_ecaliso_15 
	= ECALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.15);
      eMinus_ecaliso_20 
	= ECALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.20);
      eMinus_ecaliso_25 
	= ECALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.25);
      eMinus_ecaliso_30 
	= ECALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.30);
      eMinus_ecaliso_35 
	= ECALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.35);
      eMinus_ecaliso_40 
	= ECALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.40);
      eMinus_ecaliso_45 
	= ECALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.45);
      eMinus_ecaliso_50 
	= ECALIsolation(iEvent, eMinusEta, eMinusPhi, eMinusE, 0.50);


      ////////// Z quantities //////////////
      mZee = ZeeReco->mass();
      Z_Eta = ZeeReco->eta();   
      Z_Phi = ZeeReco->phi();
      Z_Charge = ZeeReco->charge();
      Z_Vx = ZeeReco->vx();
      Z_Vy = ZeeReco->vy();
      Z_Vz = ZeeReco->vz();

      float Zp = sqrt( pow(ePlusE + eMinusE, 2) - pow(mZee,2) );
      float theta = ZeeReco->theta();
      Z_px = Zp * sin(theta) * cos(Z_Phi);
      Z_py = Zp * sin(theta) * sin(Z_Phi);
      Z_pz = Zp * cos(theta);
      Z_E  = ePlusE + eMinusE;
      Z_Pt = Zp / cosh(Z_Eta);
      Z_Et = Z_E / cosh(Z_Eta);
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

    if( fabs(radius(&e, &c))<0.2 && (c.charge()==e.charge()) ) return true;
    
    return false;
  }
  //--------------------------------------------










  //   //  ************* Utility: electron isolation **********************
  // calculate track isolation for different cone values
  void  ZJetAnalysis::TrackIsolation( const edm::Event& iEvent, 
				      float  eta, 
				      float phi, float maxVtxDist, float conesize, 
				      float &ptiso, int &ntrkiso) {


    ptiso = -2.0;
    ntrkiso = -1;
    if( ! trackInfo_) return;
    
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


    if(pt > 0.0 && ntrk > 0) {
      ptiso = pt;
      ntrkiso = ntrk;
    }

  }








  //   //  ************* Utility: HCAL isolation *********
  double ZJetAnalysis::HCALIsolation( const edm::Event& iEvent, float eta, 
				      float phi, float energy, float conesize ) {

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

     if(hcalEnergy <= 0.0 || energy <= 0.0) return -2.0;
     return hcalEnergy/energy;
  }






  //   //  ************* Utility: ECAL isolation *********
  double ZJetAnalysis::ECALIsolation( const edm::Event& iEvent, float eta, 
				      float phi, float energy, float conesize ) {
    
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
    

    if(ecalEnergy <= 0.0 || energy <= 0.0) return -2.0;
    return (ecalEnergy-energy)/energy;
  }
  







  //   //  ************* Utility: electron identification **********************

  bool ZJetAnalysis::isElectronId( const edm::Event& iEvent, 
				   edm::Ref<reco::GsfElectronCollection> electronRef,
				   string electronLabel) const {

    bool boolDecision = false; 

//     edm::Handle<edm::ValueMap<float> > eIDValueMap; 
//     if (iEvent.getByLabel( electronLabel, eIDValueMap)) {
//       const edm::ValueMap<float> & eIDmapL = *eIDValueMap;
//       if( eIDmapL[ electronRef ] == 1.0 ) boolDecision = true;
//     }

    return boolDecision;
  }
  //--------------------------------------------








 //   //  **** Utility: create array  of histograms to be saved in output file ***

  void ZJetAnalysis::createHistograms() {

    // e+, e- histograms
    m_HistNames1D["EmECALIsolation10"] = 
      new TH1F("EmECALIsolation10","", 200, 0.0, 0.4);
    m_HistNames1D["EmECALIsolation20"] = 
      new TH1F("EmECALIsolation20","", 200, 0.0, 0.4);
    m_HistNames1D["EmECALIsolation30"] = 
      new TH1F("EmECALIsolation30","", 200, 0.0, 0.4);
    m_HistNames1D["EmECALIsolation40"] = 
      new TH1F("EmECALIsolation40","", 200, 0.0, 0.4);
    m_HistNames1D["EmECALIsolation50"] = 
      new TH1F("EmECALIsolation50","", 200, 0.0, 0.4);
    m_HistNames1D["EmHCALIsolation10"] = 
      new TH1F("EmHCALIsolation10","", 400, -0.1, 0.1);
    m_HistNames1D["EmHCALIsolation20"] = 
      new TH1F("EmHCALIsolation20","", 400, -0.1, 0.1);
    m_HistNames1D["EmHCALIsolation30"] = 
      new TH1F("EmHCALIsolation30","", 400, -0.1, 0.1);
    m_HistNames1D["EmHCALIsolation40"] = 
      new TH1F("EmHCALIsolation40","", 400, -0.1, 0.1);
    m_HistNames1D["EmHCALIsolation50"] = 
      new TH1F("EmHCALIsolation50","", 400, -0.1, 0.1);
    m_HistNames1D["EmTrackPTIsolation10"] = 
      new TH1F("EmTrackPTIsolation10","", 60, 0.0, 3.0);
    m_HistNames1D["EmTrackPTIsolation20"] = 
      new TH1F("EmTrackPTIsolation20","", 60, 0.0, 3.0);
    m_HistNames1D["EmTrackPTIsolation30"] = 
      new TH1F("EmTrackPTIsolation30","", 60, 0.0, 3.0);
    m_HistNames1D["EmTrackPTIsolation40"] = 
      new TH1F("EmTrackPTIsolation40","", 60, 0.0, 3.0);
    m_HistNames1D["EmTrackPTIsolation50"] = 
      new TH1F("EmTrackPTIsolation50","", 60, 0.0, 3.0);
    m_HistNames1D["EmTrackNumIsolation10"] = 
      new TH1F("EmTrackNumIsolation10","", 8, 0.0, 8.0);
    m_HistNames1D["EmTrackNumIsolation20"] = 
      new TH1F("EmTrackNumIsolation20","", 8, 0.0, 8.0);
    m_HistNames1D["EmTrackNumIsolation30"] = 
      new TH1F("EmTrackNumIsolation30","", 8, 0.0, 8.0);
    m_HistNames1D["EmTrackNumIsolation40"] = 
      new TH1F("EmTrackNumIsolation40","", 8, 0.0, 8.0);
    m_HistNames1D["EmTrackNumIsolation50"] = 
      new TH1F("EmTrackNumIsolation50","", 8, 0.0, 8.0);
    m_HistNames1D["EPECALIsolation10"] = 
      new TH1F("EPECALIsolation10","", 200, 0.0, 0.4);
    m_HistNames1D["EPECALIsolation20"] = 
      new TH1F("EPECALIsolation20","", 200, 0.0, 0.4);
    m_HistNames1D["EPECALIsolation30"] = 
      new TH1F("EPECALIsolation30","", 200, 0.0, 0.4);
    m_HistNames1D["EPECALIsolation40"] = 
      new TH1F("EPECALIsolation40","", 200, 0.0, 0.4);
    m_HistNames1D["EPECALIsolation50"] = 
      new TH1F("EPECALIsolation50","", 200, 0.0, 0.4);
    m_HistNames1D["EPHCALIsolation10"] = 
      new TH1F("EPHCALIsolation10","", 400, -0.1, 0.1);
    m_HistNames1D["EPHCALIsolation20"] = 
      new TH1F("EPHCALIsolation20","", 400, -0.1, 0.1);
    m_HistNames1D["EPHCALIsolation30"] = 
      new TH1F("EPHCALIsolation30","", 400, -0.1, 0.1);
    m_HistNames1D["EPHCALIsolation40"] = 
      new TH1F("EPHCALIsolation40","", 400, -0.1, 0.1);
    m_HistNames1D["EPHCALIsolation50"] = 
      new TH1F("EPHCALIsolation50","", 400, -0.1, 0.1);
    m_HistNames1D["EPTrackPTIsolation10"] = 
      new TH1F("EPTrackPTIsolation10","", 60, 0.0, 3.0);
    m_HistNames1D["EPTrackPTIsolation20"] = 
      new TH1F("EPTrackPTIsolation20","", 60, 0.0, 3.0);
    m_HistNames1D["EPTrackPTIsolation30"] = 
      new TH1F("EPTrackPTIsolation30","", 60, 0.0, 3.0);
    m_HistNames1D["EPTrackPTIsolation40"] = 
      new TH1F("EPTrackPTIsolation40","", 60, 0.0, 3.0);
    m_HistNames1D["EPTrackPTIsolation50"] = 
      new TH1F("EPTrackPTIsolation50","", 60, 0.0, 3.0);
    m_HistNames1D["EPTrackNumIsolation10"] = 
      new TH1F("EPTrackNumIsolation10","", 8, 0.0, 8.0);
    m_HistNames1D["EPTrackNumIsolation20"] = 
      new TH1F("EPTrackNumIsolation20","", 8, 0.0, 8.0);
    m_HistNames1D["EPTrackNumIsolation30"] = 
      new TH1F("EPTrackNumIsolation30","", 8, 0.0, 8.0);
    m_HistNames1D["EPTrackNumIsolation40"] = 
      new TH1F("EPTrackNumIsolation40","", 8, 0.0, 8.0);
    m_HistNames1D["EPTrackNumIsolation50"] = 
      new TH1F("EPTrackNumIsolation50","", 8, 0.0, 8.0);
    m_HistNames1D["EmEoverP"] = new TH1F("EmEoverP","", 350, 0.0, 3.5);
    m_HistNames1D["EpEoverP"] = new TH1F("EpEoverP","", 350, 0.0, 3.5);
    m_HistNames1D["EpBremFraction"] = new TH1F("EpBremFraction","", 200, 0.0, 200.0);
    m_HistNames1D["EmBremFraction"] = new TH1F("EmBremFraction","", 200, 0.0, 200.0);
    m_HistNames1D["EpClassification"] = 
      new TH1F("EpClassification","", 141, -1.0, 140.0);
    m_HistNames1D["EpHoverE"] = new TH1F("EpHoverE","", 400, -0.1, 0.1);
    m_HistNames1D["EmHoverE"] = new TH1F("EmHoverE","", 400, -0.1, 0.1);
    m_HistNames1D["genEPpt"] = new TH1F("genEPpt","", 200, 0.0, 1000.0);
    m_HistNames1D["recoEPpt"] = new TH1F("recoEPpt","", 200, 0.0, 1000.0);
    m_HistNames1D["genEMpt"] = new TH1F("genEMpt","", 200, 0.0, 1000.0);
    m_HistNames1D["recoEMpt"] = new TH1F("recoEMpt","", 200, 0.0, 1000.0);
    m_HistNames1D["genEMptMassCut"] = new TH1F("genEMptMassCut","", 45, 0.0, 225.0);
    m_HistNames1D["recoEMptMassCut"] = 
      new TH1F("recoEMptMassCut","", 45, 0.0, 225.0);
    m_HistNames1D["genEMptGolden"] = new TH1F("genEMptGolden","", 20, 0.0, 100.0);
    m_HistNames1D["genEMptBigbrem"] = new TH1F("genEMptBigbrem","", 20, 0.0, 100.0);
    m_HistNames1D["genEMptNarrow"] = new TH1F("genEMptNarrow","", 20, 0.0, 100.0);
    m_HistNames1D["genEMptShowering"] = 
      new TH1F("genEMptShowering","", 20, 0.0, 100.0);
    m_HistNames1D["recoEMptGolden"] = new TH1F("recoEMptGolden","", 20, 0.0, 100.0);
    m_HistNames1D["recoEMptBigbrem"] = 
      new TH1F("recoEMptBigbrem","", 20, 0.0, 100.0);
    m_HistNames1D["recoEMptNarrow"] = new TH1F("recoEMptNarrow","", 20, 0.0, 100.0);
    m_HistNames1D["recoEMptShowering"] = 
      new TH1F("recoEMptShowering","", 20, 0.0, 100.0);

    // Z histograms
    m_HistNames1D["Zmass_noIso"] = new TH1F("Zmass_noIso","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_TkIso"] = new TH1F("Zmass_TkIso","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_TkECALIso"] = 
      new TH1F("Zmass_TkECALIso","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_TkECALHCALIso"] = 
      new TH1F("Zmass_TkECALHCALIso","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_ptdrtight"] = 
      new TH1F("Zmass_ptdrtight","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_ptdrmedium"] = 
      new TH1F("Zmass_ptdrmedium","", 120, 60.0, 120.0);
    m_HistNames1D["Zmass_ptdrloose"] = 
      new TH1F("Zmass_ptdrloose","", 120, 60.0, 120.0);
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








 //   //  **** Utility: fill all the histograms to be saved ***
  void ZJetAnalysis::fillAllHistograms() {

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
    if(eMinus_ptiso_15>0.0 && mZee>0.0) 
      fillHist1D("EmTrackPTIsolation10", eMinus_ptiso_15);
    if(eMinus_ptiso_25>0.0 && mZee>0.0) 
      fillHist1D("EmTrackPTIsolation20", eMinus_ptiso_25);
    if(eMinus_ptiso_30>0.0 && mZee>0.0) 
      fillHist1D("EmTrackPTIsolation30", eMinus_ptiso_30);
    if(eMinus_ptiso_40>0.0 && mZee>0.0) 
      fillHist1D("EmTrackPTIsolation40", eMinus_ptiso_40);
    if(eMinus_ptiso_50>0.0 && mZee>0.0) 
      fillHist1D("EmTrackPTIsolation50", eMinus_ptiso_50);
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
    if(ePlus_ptiso_15>0.0 && mZee>0.0) 
      fillHist1D("EPTrackPTIsolation10", ePlus_ptiso_15);
    if(ePlus_ptiso_25>0.0 && mZee>0.0) 
      fillHist1D("EPTrackPTIsolation20", ePlus_ptiso_25);
    if(ePlus_ptiso_30>0.0 && mZee>0.0) 
      fillHist1D("EPTrackPTIsolation30", ePlus_ptiso_30);
    if(ePlus_ptiso_40>0.0 && mZee>0.0) 
      fillHist1D("EPTrackPTIsolation40", ePlus_ptiso_40);
    if(ePlus_ptiso_50>0.0 && mZee>0.0) 
      fillHist1D("EPTrackPTIsolation50", ePlus_ptiso_50);
    if(mZee>0.0) {
      fillHist1D("EPTrackNumIsolation10", ePlus_ntrkiso_15);
      fillHist1D("EPTrackNumIsolation20", ePlus_ntrkiso_25);
      fillHist1D("EPTrackNumIsolation30", ePlus_ntrkiso_30);
      fillHist1D("EPTrackNumIsolation40", ePlus_ntrkiso_40);
      fillHist1D("EPTrackNumIsolation50", ePlus_ntrkiso_50);
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
      fillHist1D("EmTrackNumIsolation10", eMinus_ntrkiso_15);
      fillHist1D("EmTrackNumIsolation20", eMinus_ntrkiso_25);
      fillHist1D("EmTrackNumIsolation30", eMinus_ntrkiso_30);
      fillHist1D("EmTrackNumIsolation40", eMinus_ntrkiso_40);
      fillHist1D("EmTrackNumIsolation50", eMinus_ntrkiso_50);
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
    if(eMinus_ptiso_15 < 0.2 && ePlus_ptiso_15 < 0.2) 
      fillHist1D("Zmass_TkIso", mZee);
    if(eMinus_ptiso_15 < 0.2 && ePlus_ptiso_15 < 0.2 && 
       eMinus_ecaliso_20<0.2 && ePlus_ecaliso_20<0.2) 
      fillHist1D("Zmass_TkECALIso", mZee);
    if(eMinus_ptiso_15 < 0.2 && ePlus_ptiso_15 < 0.2 && 
       eMinus_ecaliso_20<0.2 && ePlus_ecaliso_20<0.2 &&
       fabs(eMinus_hcaliso_30)<0.1 && fabs(ePlus_hcaliso_30)<0.1) 
      fillHist1D("Zmass_TkECALHCALIso", mZee);
    if(isePlusTight & iseMinusTight) 
      fillHist1D("Zmass_ptdrtight", mZee);
    if(isePlusRobustLoose & iseMinusRobustLoose) 
      fillHist1D("Zmass_ptdrmedium", mZee);
    if(isePlusLoose & iseMinusLoose) 
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
  }








  //   //  **** Utility: default initializatied values for ntuple variables ***
void ZJetAnalysis::setDefaultValues() {

    mZee                  = -1.;
    Z_px                  = -99999.;
    Z_py                  = -99999.;
    Z_pz                  = -99999.;
    Z_E                   = -1.;
    Z_Pt                  = -1.;
    Z_Et                  = -1.;
    Z_Eta                 = -10.;
    Z_Phi                 = -10.;
    Z_Charge              = -10;
    Z_Vx                  = -10.;
    Z_Vy                  = -10.;
    Z_Vz                  = -10.;

    isePlusLoose          = false;
    isePlusTight          = false;
    isePlusRobustLoose    = false;
    isePlusRobustTight    = false;
    ePlusClassification   = -1; 
    ePluspx               = -99999.;
    ePluspy               = -99999.;
    ePluspz               = -99999.;
    ePlusE                = -1.;
    ePlusEt               = -1.;
    ePlusPt               = -1.;
    ePlusEta              = -10.;
    ePlusTheta            = -99999.;
    ePlusPhi              = -10.;
    ePlusCharge           = -10;
    ePlusVx               = -10.;
    ePlusVy               = -10.;
    ePlusVz               = -10.;
    ePlusIsolation        = -1.;
    ePlus_sc_x            = -10.;
    ePlus_sc_y            = -10.;
    ePlus_sc_z            = -10.;
    ePlus_sc_R            = -10.;
    ePlus_sc_Rt           = -10.;
    ePlus_sc_no           = -1;
    ePlus_EoverPout       = -1.;
    ePlus_EoverPin        = -1.;
    ePlus_InvEMinusInvP   = -10.;
    ePlus_BremFraction    = -1.;
    ePlus_DeltaEtaIn      = -10.;
    ePlus_DeltaPhiIn      = -10.;
    ePlus_DeltaPhiOut     = -10.;
    ePlus_DeltaEtaOut     = -10.;
    ePlus_Trackmom_calo   = -1.;
    ePlus_Trackmom_vtx    = -1.;	  
    ePlus_HoverE          = -1.;	  
    ePlus_vx              = -10.;
    ePlus_vy              = -10.;
    ePlus_vz              = -10.;	  
    ePlus_td0             = -1.;
    ePlus_d0              = -1.;	 	  
    ePlus_E9overE25       = -10.;
    ePlus_SigmaEtaEta     = -1.;
    ePlus_SigmaEtaPhi     = -1.;
    ePlus_SigmaPhiPhi     = -1.;	  
    ePlus_trk_eta         = -10.;
    ePlus_trk_phi         = -10.;
    ePlus_trk_etaout      = -10.;
    ePlus_trk_phiout      = -10.;
    ePlus_trk_p           = -1.;
    ePlus_trk_pout        = -1.;
    ePlus_trk_pt          = -1.;
    ePlus_trk_charge      = -10;
    ePlus_trk_chi2        = -1.;
    ePlus_trk_ndof        = -1;
    ePlus_trk_hits        = -1;
    ePlus_trk_d0          = -1.;
    ePlus_trk_d0Err       = -1.;
	  
    iseMinusLoose          = false;
    iseMinusTight          = false;
    iseMinusRobustLoose    = false;
    iseMinusRobustTight    = false;
    eMinusClassification  = -1;
    eMinuspx              = -99999.;
    eMinuspy              = -99999.;
    eMinuspz              = -99999.;
    eMinusE               = -1.;
    eMinusPt              = -1.;
    eMinusEt              = -1.;
    eMinusEta             = -10.;
    eMinusTheta           = -99999.;
    eMinusPhi             = -10.;
    eMinusCharge          = -10;
    eMinusVx              = -10.;
    eMinusVy              = -10.;
    eMinusVz              = -10.;
    eMinusIsolation        = -1.;
    eMinus_sc_x            = -10.;
    eMinus_sc_y            = -10.;
    eMinus_sc_z            = -10.;
    eMinus_sc_R            = -10.;
    eMinus_sc_Rt           = -10.;
    eMinus_sc_no           = -1;
    eMinus_EoverPout       = -1.;
    eMinus_EoverPin        = -1.;
    eMinus_InvEMinusInvP   = -10.;
    eMinus_BremFraction    = -1.;
    eMinus_DeltaEtaIn      = -10.;
    eMinus_DeltaPhiIn      = -10.;
    eMinus_DeltaPhiOut     = -10.;
    eMinus_DeltaEtaOut     = -10.;
    eMinus_Trackmom_calo   = -1.;
    eMinus_Trackmom_vtx    = -1.;	  
    eMinus_HoverE          = -1.;	  
    eMinus_vx              = -10.;
    eMinus_vy              = -10.;
    eMinus_vz              = -10.;	  
    eMinus_td0             = -1.;
    eMinus_d0              = -1.;	 	  
    eMinus_E9overE25       = -10.;
    eMinus_SigmaEtaEta     = -1.;
    eMinus_SigmaEtaPhi     = -1.;
    eMinus_SigmaPhiPhi     = -1.;	  
    eMinus_trk_eta         = -10.;
    eMinus_trk_phi         = -10.;
    eMinus_trk_etaout      = -10.;
    eMinus_trk_phiout      = -10.;
    eMinus_trk_p           = -1.;
    eMinus_trk_pout        = -1.;
    eMinus_trk_pt          = -1.;
    eMinus_trk_charge      = -10;
    eMinus_trk_chi2        = -1.;
    eMinus_trk_ndof        = -1;
    eMinus_trk_hits        = -1;
    eMinus_trk_d0          = -1.;
    eMinus_trk_d0Err       = -1.;
	  
    //////////////
    ePlus_ptiso_15 = -2.; 
    ePlus_ptiso_20 = -2.; 
    ePlus_ptiso_25 = -2.;
    ePlus_ptiso_30 = -2.;
    ePlus_ptiso_35 = -2.;
    ePlus_ptiso_40 = -2.;
    ePlus_ptiso_45 = -2.;
    ePlus_ptiso_50 = -2.;
    ePlus_ntrkiso_15 = -1;
    ePlus_ntrkiso_20 = -1;
    ePlus_ntrkiso_25 = -1;
    ePlus_ntrkiso_30 = -1;
    ePlus_ntrkiso_35 = -1;
    ePlus_ntrkiso_40 = -1;
    ePlus_ntrkiso_45 = -1;
    ePlus_ntrkiso_50 = -1;     
    ePlus_hcaliso_10 = -2.;
    ePlus_hcaliso_15 = -2.;
    ePlus_hcaliso_20 = -2.;
    ePlus_hcaliso_25 = -2.;
    ePlus_hcaliso_30 = -2.;
    ePlus_hcaliso_35 = -2.;
    ePlus_hcaliso_40 = -2.;
    ePlus_hcaliso_45 = -2.;
    ePlus_hcaliso_50 = -2.;
    ePlus_ecaliso_10 = -2.;
    ePlus_ecaliso_15 = -2.;
    ePlus_ecaliso_20 = -2.;
    ePlus_ecaliso_25 = -2.;
    ePlus_ecaliso_30 = -2.;
    ePlus_ecaliso_35 = -2.;
    ePlus_ecaliso_40 = -2.;
    ePlus_ecaliso_45 = -2.;
    ePlus_ecaliso_50 = -2.;

    ///////////////////
    eMinus_ptiso_15 = -2.; 
    eMinus_ptiso_20 = -2.; 
    eMinus_ptiso_25 = -2.;
    eMinus_ptiso_30 = -2.;
    eMinus_ptiso_35 = -2.;
    eMinus_ptiso_40 = -2.;
    eMinus_ptiso_45 = -2.;
    eMinus_ptiso_50 = -2.;
    eMinus_ntrkiso_15 = -1;
    eMinus_ntrkiso_20 = -1;
    eMinus_ntrkiso_25 = -1;
    eMinus_ntrkiso_30 = -1;
    eMinus_ntrkiso_35 = -1;
    eMinus_ntrkiso_40 = -1;
    eMinus_ntrkiso_45 = -1;
    eMinus_ntrkiso_50 = -1;     
    eMinus_hcaliso_10 = -2.;
    eMinus_hcaliso_15 = -2.;
    eMinus_hcaliso_20 = -2.;
    eMinus_hcaliso_25 = -2.;
    eMinus_hcaliso_30 = -2.;
    eMinus_hcaliso_35 = -2.;
    eMinus_hcaliso_40 = -2.;
    eMinus_hcaliso_45 = -2.;
    eMinus_hcaliso_50 = -2.;
    eMinus_ecaliso_10 = -2.;
    eMinus_ecaliso_15 = -2.;
    eMinus_ecaliso_20 = -2.;
    eMinus_ecaliso_25 = -2.;
    eMinus_ecaliso_30 = -2.;
    eMinus_ecaliso_35 = -2.;
    eMinus_ecaliso_40 = -2.;
    eMinus_ecaliso_45 = -2.;
    eMinus_ecaliso_50 = -2.;

    //////////////
    mZeeGen               = -1.;
    Z_pxGen               = -99999.;
    Z_pyGen               = -99999.;
    Z_pzGen               = -99999.;
    Z_EGen                = -1.;
    Z_PtGen               = -1.;
    Z_EtGen               = -1.;
    Z_EtaGen              = -10.;
    Z_PhiGen              = -10.;
    Z_VxGen               = -10.;
    Z_VyGen               = -10.;
    Z_VzGen               = -10.;
    ePlusClassificationGen= -1;
    ePluspxGen            = -99999.;
    ePluspyGen            = -99999.;
    ePluspzGen            = -99999.;
    ePlusEGen             = -1.;
    ePlusPtGen            = -1.;
    ePlusEtGen            = -1.;
    ePlusEtaGen           = -10.;
    ePlusPhiGen           = -10.;
    ePlusChargeGen        = -10;
    ePlusVxGen            = -10.;
    ePlusVyGen            = -10.;
    ePlusVzGen            = -10.;   
    eMinusClassificationGen = -1;
    eMinuspxGen           = -99999.;
    eMinuspyGen           = -99999.;
    eMinuspzGen           = -99999.;
    eMinusEGen            = -1.;
    eMinusPtGen           = -1.;
    eMinusEtGen           = -1.;
    eMinusEtaGen          = -10.;
    eMinusPhiGen          = -10.;
    eMinusChargeGen       = -10;
    eMinusVxGen           = -10.;
    eMinusVyGen           = -10.;
    eMinusVzGen           = -10.; 
}





  //   //  **** Utility: declare TTree branches for ntuple variables ***
  void ZJetAnalysis::declareTreeBranches() {

    myTree->Branch("run",  &run, "run/I");
    myTree->Branch("event",  &event, "event/I");
    myTree->Branch("eventsize", &eventsize, "eventsize/I"); 

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
    myTree->Branch("Z_Charge",    &Z_Charge,    "Z_Charge/I");
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
    myTree->Branch("ePlusTheta",   &ePlusTheta,   "ePlusTheta/F"); 
    myTree->Branch("ePlusPhi",     &ePlusPhi,     "ePlusPhi/F");
    myTree->Branch("ePlusCharge",  &ePlusCharge,  "ePlusCharge/I");
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

    myTree->Branch("ePlus_ntrkiso_15",  &ePlus_ntrkiso_15, "ePlus_ntrkiso_15/I");
    myTree->Branch("ePlus_ntrkiso_20",  &ePlus_ntrkiso_20, "ePlus_ntrkiso_20/I");
    myTree->Branch("ePlus_ntrkiso_25",  &ePlus_ntrkiso_25, "ePlus_ntrkiso_25/I");
    myTree->Branch("ePlus_ntrkiso_30",  &ePlus_ntrkiso_30, "ePlus_ntrkiso_30/I");
    myTree->Branch("ePlus_ntrkiso_35",  &ePlus_ntrkiso_35, "ePlus_ntrkiso_35/I");
    myTree->Branch("ePlus_ntrkiso_40",  &ePlus_ntrkiso_40, "ePlus_ntrkiso_40/I");
    myTree->Branch("ePlus_ntrkiso_45",  &ePlus_ntrkiso_45, "ePlus_ntrkiso_45/I");
    myTree->Branch("ePlus_ntrkiso_50",  &ePlus_ntrkiso_50, "ePlus_ntrkiso_50/I");
     
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
    myTree->Branch("isePlusLoose", &isePlusLoose,"isePlusLoose/O");
    myTree->Branch("isePlusTight",&isePlusTight,"isePlusTight/O");
    myTree->Branch("isePlusRobustLoose",&isePlusRobustLoose,"isePlusRobustLoose/O");
    myTree->Branch("isePlusRobustTight",&isePlusRobustTight,"isePlusRobustTight/O");
    myTree->Branch("ePlus_sc_x", &ePlus_sc_x, "ePlus_sc_x/F");
    myTree->Branch("ePlus_sc_y", &ePlus_sc_y, "ePlus_sc_y/F");
    myTree->Branch("ePlus_sc_z", &ePlus_sc_z, "ePlus_sc_z/F");
    myTree->Branch("ePlus_sc_R", &ePlus_sc_R, "ePlus_sc_R/F");
    myTree->Branch("ePlus_sc_Rt", &ePlus_sc_Rt, "ePlus_sc_Rt/F");
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
    myTree->Branch("ePlus_SigmaEtaPhi", &ePlus_SigmaEtaPhi, 
		   "ePlus_SigmaEtaPhi/F");
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
    myTree->Branch("eMinusTheta",  &eMinusTheta,  "eMinusTheta/F");    
    myTree->Branch("eMinusPhi",    &eMinusPhi,    "eMinusPhi/F");
    myTree->Branch("eMinusCharge", &eMinusCharge, "eMinusCharge/I");
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

    myTree->Branch("eMinus_ntrkiso_15",  &eMinus_ntrkiso_15, "eMinus_ntrkiso_15/I");
    myTree->Branch("eMinus_ntrkiso_20",  &eMinus_ntrkiso_20, "eMinus_ntrkiso_20/I");
    myTree->Branch("eMinus_ntrkiso_25",  &eMinus_ntrkiso_25, "eMinus_ntrkiso_25/I");
    myTree->Branch("eMinus_ntrkiso_30",  &eMinus_ntrkiso_30, "eMinus_ntrkiso_30/I");
    myTree->Branch("eMinus_ntrkiso_35",  &eMinus_ntrkiso_35, "eMinus_ntrkiso_35/I");
    myTree->Branch("eMinus_ntrkiso_40",  &eMinus_ntrkiso_40, "eMinus_ntrkiso_40/I");
    myTree->Branch("eMinus_ntrkiso_45",  &eMinus_ntrkiso_45, "eMinus_ntrkiso_45/I");
    myTree->Branch("eMinus_ntrkiso_50",  &eMinus_ntrkiso_50, "eMinus_ntrkiso_50/I");
     
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
    myTree->Branch("iseMinusLoose", &iseMinusLoose,"iseMinusLoose/O");
    myTree->Branch("iseMinusTight",&iseMinusTight,"iseMinusTight/O");
    myTree->Branch("iseMinusRobustLoose",&iseMinusRobustLoose,
		   "iseMinusRobustLoose/O");
    myTree->Branch("iseMinusRobustTight",&iseMinusRobustTight,
		   "iseMinusRobustTight/O");

    myTree->Branch("eMinus_sc_x", &eMinus_sc_x, "eMinus_sc_x/F");
    myTree->Branch("eMinus_sc_y", &eMinus_sc_y, "eMinus_sc_y/F");
    myTree->Branch("eMinus_sc_z", &eMinus_sc_z, "eMinus_sc_z/F");
    myTree->Branch("eMinus_sc_R", &eMinus_sc_R, "eMinus_sc_R/F");
    myTree->Branch("eMinus_sc_Rt", &eMinus_sc_Rt, "eMinus_sc_Rt/F");
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
    myTree->Branch("eMinus_SigmaEtaPhi", &eMinus_SigmaEtaPhi, 
		   "eMinus_SigmaEtaPhi/F");
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
	  
    // Jet block
    myTree->Branch("NumRecoJetAlgo", &NumRecoJetAlgo, "NumRecoJetAlgo/I");
    myTree->Branch("NumRecoJets", &NumRecoJets, "NumRecoJets/I");   
    myTree->Branch("JetRecoEt",  JetRecoEt, "JetRecoEt[20][4]/F");
    myTree->Branch("JetRecoPt",  JetRecoPt, "JetRecoPt[20][4]/F");
    myTree->Branch("JetRecoEta",  JetRecoEta, "JetRecoEta[20][4]/F");
    myTree->Branch("JetRecoPhi",  JetRecoPhi, "JetRecoPhi[20][4]/F");
    myTree->Branch("JetRecoType", JetRecoType, "JetRecoType[20][4]/I");
    if(doJetFlavorIdentification_) {
      myTree->Branch("JetRecoFlavor", JetRecoFlavor, "JetRecoFlavor[20][4]/I");
    }

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


    if( runningOverMC_ ) {

      myTree->Branch("genEventScale", &genEventScale , "genEventScale/F");
      myTree->Branch("genEventWeight", &genEventWeight , "genEventWeight/F");
      myTree->Branch("mZeeGen",     &mZeeGen,     "mZeeGen/F");
      myTree->Branch("Z_pxGen",     &Z_pxGen,     "Z_pxGen/F");
      myTree->Branch("Z_pyGen",     &Z_pyGen,     "Z_pyGen/F");
      myTree->Branch("Z_pzGen",     &Z_pzGen,     "Z_pzGen/F");
      myTree->Branch("Z_EGen",     &Z_EGen,     "Z_EGen/F");
      myTree->Branch("Z_PtGen",     &Z_PtGen,     "Z_PtGen/F");
      myTree->Branch("Z_EtGen",     &Z_EtGen,     "Z_EtGen/F");
      myTree->Branch("Z_EtaGen",     &Z_EtaGen,     "Z_EtaGen/F");    
      myTree->Branch("Z_PhiGen",     &Z_PhiGen,     "Z_PhiGen/F");
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
      myTree->Branch("ePlusChargeGen",  &ePlusChargeGen,"ePlusChargeGen/I");
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
      myTree->Branch("eMinusChargeGen",  &eMinusChargeGen,  "eMinusChargeGen/I");
      myTree->Branch("eMinusVxGen",     &eMinusVxGen,       "eMinusVxGen/F");
      myTree->Branch("eMinusVyGen",     &eMinusVyGen,       "eMinusVyGen/F");
      myTree->Branch("eMinusVzGen",     &eMinusVzGen,       "eMinusVzGen/F");
      myTree->Branch("eMinusClassificationGen", &eMinusClassificationGen, 
		     "eMinusClassificationGen/I");

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
    }

    // end of tree declaration
  }





///////////////////////////////////////////////////////////////////////////////
void ZJetAnalysis::fillHist1D(const TString& histName,const Double_t& value, 
			      const Double_t& wt) {

  std::map<TString, TH1*>::iterator hid=m_HistNames1D.find(histName);
  if (hid==m_HistNames1D.end()){
    std::cout << "%fillHist -- Could not find histogram with name: " << 
      histName << std::endl;
  }
  else{
    hid->second->Fill(value,wt);
  }
}
///////////////////////////////////////////////////////////////////////////////
  

} // namespace cms











#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
using cms::ZJetAnalysis;
DEFINE_FWK_MODULE(ZJetAnalysis);
