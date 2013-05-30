#define MB_Py8_cxx
#include "MB_Py8.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>

#include "Event.h"
#include "TLorentzVector.h"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"
#include "fastjet/tools/MassDropTagger.hh"
#include "fastjet/GhostedAreaSpec.hh"
#include "fastjet/contrib/NjettinessPlugin.hh"
#include "fastjet/contrib/Nsubjettiness.hh"
#include<fastjet/Selector.hh>
#include<fastjet/tools/JetMedianBackgroundEstimator.hh>
#include<fastjet/tools/Subtractor.hh>
#include<fastjet/contrib/GenericSubtractor.hh>

using namespace fastjet;
using namespace fastjet::contrib;


EtaRange::EtaRange(double etaMin,double etaMax)
  : _etaMin(etaMin), _etaMax(etaMax)
{}

EtaRange::~EtaRange()
{ }

bool EtaRange::accept(const fastjet::PseudoJet& pJet)
{ return pJet.pseudorapidity() > _etaMin && pJet.pseudorapidity() < _etaMax; }

void EtaRange::reset()
{ }


void MB_Py8::Loop(int mu,Long64_t nevts,const std::string& outName, MB_Py8* sigData, bool PUsub)
{

   if (fChain == 0) return;

   Long64_t ntot(0);
   if ( nevts > 0 ) 
     { ntot = nevts; }
   else
     { Long64_t nall = (Long64_t)fChain->GetEntries();
       Long64_t nmu  = (Long64_t)mu;
       ntot = nall/nmu; }
   printf("[MB_Py8::Loop(...)] - INFO - form and analyze %i pileup events (may be approximate!)\n",
	  ntot);

   book();

   // Long64_t nentries = fChain->GetEntries();
   Long64_t ientries = 0;
   
   Event*               pEvt = new Event(new EtaRange(-6.,6.));
   DataHandler<MB_Py8>* pHdl = new DataHandler<MB_Py8>();


   // ------- Now get the signal -------------
   Event*               sigEvt = new Event(new EtaRange(-6.,6.));
   DataHandler<MB_Py8>* sigHdl = new DataHandler<MB_Py8>();


   if ( nevts > 0 )
     {
       Long64_t ievts(0);
       while ( ievts < ntot && pHdl->fillEvent(*this,*pEvt,ientries,mu) && 
	       sigHdl->fillEvent(*sigData,*sigEvt,ientries,0)) 
	 { ++ievts; ticker((Long64_t)pEvt->nVertices(),1000,ntot); 
	   analyze(*pEvt, *sigEvt, PUsub); pEvt->reset(); sigEvt->reset();}
     }
   else 
     { 
       while ( pHdl->fillEvent(*this,*pEvt,ientries,mu)  && 
	       sigHdl->fillEvent(*sigData,*sigEvt,ientries,0)) 
	 { ticker((Long64_t)pEvt->nVertices(),1000,ntot);
	   analyze(*pEvt, *sigEvt, PUsub); pEvt->reset(); sigEvt->reset();}
     }  

   //
   if ( !(outName.empty() || outName == "") ) { write(outName); }
}

bool MB_Py8::book()
{
  // event distributions
  h_mu = new TH1D("NumberInteractions","Number of interactions in event",
		  100,0.,250.);

  // particle flow
  int nEtaBins(140);
  double etaMin(-7.);
  double etaMax(7.);
  h_rap = new TH1D("PartAllRap","Particle rapidity",nEtaBins,etaMin,etaMax);
  h_eta = new TH1D("PartAllEta","Particle pseudorapidity",
		   nEtaBins,etaMin,etaMax);
  h_rap_charged = new TH1D("PartChgRap","Charged particle rapidity",
			   nEtaBins,etaMin,etaMax);
  h_eta_charged = new TH1D("PartChgEta","Charged particle pseudorapidity",
			   nEtaBins,etaMin,etaMax);
  int nPtBins(500);
  double ptMin(0.);
  double ptMax(100.);
  h_perp = new TH1D("PartAllPt","Particle pT",nPtBins,ptMin,ptMax);
  h_perp_charged = new TH1D("PartChgPt","Charged patricle pt",
			    nPtBins,ptMin,ptMax);


  int nJetMassBins(1000);
  double JetMassMin(0.);
  double JetMassMax(10000.);

  int nJetPtBins(1000);
  double JetPtMin(0.);
  double JetPtMax(10000);


  //////// ------------- AK5 ----------------------------------------------
  h_ak5_numJets     = new TH1D("ak5_numJets","",20, -0.5, 19.5);
  h_ak5_jetmass     = new TH1D("ak5_jetmass","",nJetMassBins, JetMassMin, JetMassMax); 
  h_ak5_jetpt       = new TH1D("ak5_jetpt","",nJetPtBins, JetPtMin, JetPtMax); 
  h_ak5_jetarea     = new TH1D("ak5_jetarea","", 100, 0., 10.);		
  h_ak5_tau2tau1    = new TH1D("ak5_tau2tau1","", 100, 0., 1.);
  h_ak5_tau3tau2    = (TH1D*) h_ak5_tau2tau1->Clone("ak5_tau3tau2"); 
  h_ak5_jetmass_tr  = (TH1D*) h_ak5_jetmass->Clone("ak5_jetmass_tr"); 
  h_ak5_jetpt_tr    = (TH1D*) h_ak5_jetpt->Clone("ak5_jetpt_tr"); 
  h_ak5_jetarea_tr  = (TH1D*) h_ak5_jetarea->Clone("ak5_jetarea_tr"); 		 
  h_ak5_jetmass_ft  = (TH1D*) h_ak5_jetmass->Clone("ak5_jetmass_ft"); 
  h_ak5_jetpt_ft    = (TH1D*) h_ak5_jetpt->Clone("ak5_jetpt_ft"); 
  h_ak5_jetarea_ft  = (TH1D*) h_ak5_jetarea->Clone("ak5_jetarea_ft"); 		 
  h_ak5_jetmass_pr  = (TH1D*) h_ak5_jetmass->Clone("ak5_jetmass_pr");
  h_ak5_jetpt_pr    = (TH1D*) h_ak5_jetpt->Clone("ak5_jetpt_pr"); 
  h_ak5_jetarea_pr  = (TH1D*) h_ak5_jetarea->Clone("ak5_jetarea_pr"); 
  h_ak5_massdrop_pr = new TH1D("ak5_massdrop_pr","", 200, 0., 2.);	

  hprof_ak5_jetmass_jetpt  = new TProfile("ak5_jetmass_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  hprof_ak5_jetmass_tr_jetpt_tr  = (TProfile*) hprof_ak5_jetmass_jetpt->Clone("ak5_jetmass_tr_jetpt_tr");
  hprof_ak5_jetmass_ft_jetpt_ft  = (TProfile*) hprof_ak5_jetmass_jetpt->Clone("ak5_jetmass_ft_jetpt_ft");
  hprof_ak5_jetmass_pr_jetpt_pr  = (TProfile*) hprof_ak5_jetmass_jetpt->Clone("ak5_jetmass_pr_jetpt_pr");
  hprof_ak5_tau2tau1_jetpt  = new TProfile("ak5_tau2tau1_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, 0.0, 1.0);



  //////// ------------- AK6 ----------------------------------------------
  h_ak6_numJets     = new TH1D("ak6_numJets","",20, -0.5, 19.5);
  h_ak6_jetmass     = new TH1D("ak6_jetmass","",nJetMassBins, JetMassMin, JetMassMax); 
  h_ak6_jetpt       = new TH1D("ak6_jetpt","",nJetPtBins, JetPtMin, JetPtMax); 
  h_ak6_jetarea     = new TH1D("ak6_jetarea","", 100, 0., 10.);	
  h_ak6_tau3tau2    = (TH1D*) h_ak5_tau2tau1->Clone("ak6_tau3tau2");
  h_ak6_tau2tau1    = (TH1D*) h_ak5_tau2tau1->Clone("ak6_tau2tau1");		 
  h_ak6_jetmass_tr  = (TH1D*) h_ak6_jetmass->Clone("ak6_jetmass_tr"); 
  h_ak6_jetpt_tr    = (TH1D*) h_ak6_jetpt->Clone("ak6_jetpt_tr"); 
  h_ak6_jetarea_tr  = (TH1D*) h_ak6_jetarea->Clone("ak6_jetarea_tr"); 		 
  h_ak6_jetmass_ft  = (TH1D*) h_ak6_jetmass->Clone("ak6_jetmass_ft"); 
  h_ak6_jetpt_ft    = (TH1D*) h_ak6_jetpt->Clone("ak6_jetpt_ft"); 
  h_ak6_jetarea_ft  = (TH1D*) h_ak6_jetarea->Clone("ak6_jetarea_ft"); 		 
  h_ak6_jetmass_pr  = (TH1D*) h_ak6_jetmass->Clone("ak6_jetmass_pr");
  h_ak6_jetpt_pr    = (TH1D*) h_ak6_jetpt->Clone("ak6_jetpt_pr"); 
  h_ak6_jetarea_pr  = (TH1D*) h_ak6_jetarea->Clone("ak6_jetarea_pr"); 
  h_ak6_massdrop_pr = new TH1D("ak6_massdrop_pr","", 200, 0., 2.);	

  hprof_ak6_jetmass_jetpt  = new TProfile("ak6_jetmass_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  hprof_ak6_jetmass_tr_jetpt_tr  = (TProfile*) hprof_ak6_jetmass_jetpt->Clone("ak6_jetmass_tr_jetpt_tr");
  hprof_ak6_jetmass_ft_jetpt_ft  = (TProfile*) hprof_ak6_jetmass_jetpt->Clone("ak6_jetmass_ft_jetpt_ft");
  hprof_ak6_jetmass_pr_jetpt_pr  = (TProfile*) hprof_ak6_jetmass_jetpt->Clone("ak6_jetmass_pr_jetpt_pr");
  hprof_ak6_tau2tau1_jetpt  = new TProfile("ak6_tau2tau1_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, 0.0, 1.0);

  //////// ------------- AK7 ----------------------------------------------
  h_ak7_numJets     = new TH1D("ak7_numJets","",20, -0.5, 19.5);
  h_ak7_jetmass     = new TH1D("ak7_jetmass","",nJetMassBins, JetMassMin, JetMassMax); 
  h_ak7_jetpt       = new TH1D("ak7_jetpt","",nJetPtBins, JetPtMin, JetPtMax); 
  h_ak7_jetarea     = new TH1D("ak7_jetarea","", 100, 0., 10.);
  h_ak7_tau3tau2    = (TH1D*) h_ak5_tau2tau1->Clone("ak7_tau3tau2");
  h_ak7_tau2tau1    = (TH1D*) h_ak5_tau2tau1->Clone("ak7_tau2tau1");				 
  h_ak7_jetmass_tr  = (TH1D*) h_ak7_jetmass->Clone("ak7_jetmass_tr"); 
  h_ak7_jetpt_tr    = (TH1D*) h_ak7_jetpt->Clone("ak7_jetpt_tr"); 
  h_ak7_jetarea_tr  = (TH1D*) h_ak7_jetarea->Clone("ak7_jetarea_tr"); 		 
  h_ak7_jetmass_ft  = (TH1D*) h_ak7_jetmass->Clone("ak7_jetmass_ft"); 
  h_ak7_jetpt_ft    = (TH1D*) h_ak7_jetpt->Clone("ak7_jetpt_ft"); 
  h_ak7_jetarea_ft  = (TH1D*) h_ak7_jetarea->Clone("ak7_jetarea_ft"); 		 
  h_ak7_jetmass_pr  = (TH1D*) h_ak7_jetmass->Clone("ak7_jetmass_pr");
  h_ak7_jetpt_pr    = (TH1D*) h_ak7_jetpt->Clone("ak7_jetpt_pr"); 
  h_ak7_jetarea_pr  = (TH1D*) h_ak7_jetarea->Clone("ak7_jetarea_pr"); 
  h_ak7_massdrop_pr = new TH1D("ak7_massdrop_pr","", 200, 0., 2.);	

  hprof_ak7_jetmass_jetpt  = new TProfile("ak7_jetmass_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  hprof_ak7_jetmass_tr_jetpt_tr  = (TProfile*) hprof_ak7_jetmass_jetpt->Clone("ak7_jetmass_tr_jetpt_tr");
  hprof_ak7_jetmass_ft_jetpt_ft  = (TProfile*) hprof_ak7_jetmass_jetpt->Clone("ak7_jetmass_ft_jetpt_ft");
  hprof_ak7_jetmass_pr_jetpt_pr  = (TProfile*) hprof_ak7_jetmass_jetpt->Clone("ak7_jetmass_pr_jetpt_pr");
  hprof_ak7_tau2tau1_jetpt  = new TProfile("ak7_tau2tau1_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, 0.0, 1.0);

  //////// ------------- AK8 ----------------------------------------------
  h_ak8_numJets     = new TH1D("ak8_numJets","",20, -0.5, 19.5);
  h_ak8_jetmass     = new TH1D("ak8_jetmass","",nJetMassBins, JetMassMin, JetMassMax); 
  h_ak8_jetpt       = new TH1D("ak8_jetpt","",nJetPtBins, JetPtMin, JetPtMax); 
  h_ak8_jetarea     = new TH1D("ak8_jetarea","", 100, 0., 10.);
  h_ak8_tau3tau2    = (TH1D*) h_ak5_tau2tau1->Clone("ak8_tau3tau2");
  h_ak8_tau2tau1    = (TH1D*) h_ak5_tau2tau1->Clone("ak8_tau2tau1");				 
  h_ak8_jetmass_tr  = (TH1D*) h_ak8_jetmass->Clone("ak8_jetmass_tr"); 
  h_ak8_jetpt_tr    = (TH1D*) h_ak8_jetpt->Clone("ak8_jetpt_tr"); 
  h_ak8_jetarea_tr  = (TH1D*) h_ak8_jetarea->Clone("ak8_jetarea_tr"); 		 
  h_ak8_jetmass_ft  = (TH1D*) h_ak8_jetmass->Clone("ak8_jetmass_ft"); 
  h_ak8_jetpt_ft    = (TH1D*) h_ak8_jetpt->Clone("ak8_jetpt_ft"); 
  h_ak8_jetarea_ft  = (TH1D*) h_ak8_jetarea->Clone("ak8_jetarea_ft"); 		 
  h_ak8_jetmass_pr  = (TH1D*) h_ak8_jetmass->Clone("ak8_jetmass_pr");
  h_ak8_jetpt_pr    = (TH1D*) h_ak8_jetpt->Clone("ak8_jetpt_pr"); 
  h_ak8_jetarea_pr  = (TH1D*) h_ak8_jetarea->Clone("ak8_jetarea_pr"); 
  h_ak8_massdrop_pr = new TH1D("ak8_massdrop_pr","", 200, 0., 2.);	

  hprof_ak8_jetmass_jetpt  = new TProfile("ak8_jetmass_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  hprof_ak8_jetmass_tr_jetpt_tr  = (TProfile*) hprof_ak8_jetmass_jetpt->Clone("ak8_jetmass_tr_jetpt_tr");
  hprof_ak8_jetmass_ft_jetpt_ft  = (TProfile*) hprof_ak8_jetmass_jetpt->Clone("ak8_jetmass_ft_jetpt_ft");
  hprof_ak8_jetmass_pr_jetpt_pr  = (TProfile*) hprof_ak8_jetmass_jetpt->Clone("ak8_jetmass_pr_jetpt_pr");
  hprof_ak8_tau2tau1_jetpt  = new TProfile("ak8_tau2tau1_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, 0.0, 1.0);


  //////// ------------- CA8 ----------------------------------------------
  h_ca8_numJets     = new TH1D("ca8_numJets","",20, -0.5, 19.5);
  h_ca8_jetmass     = new TH1D("ca8_jetmass","",nJetMassBins, JetMassMin, JetMassMax); 
  h_ca8_jetpt       = new TH1D("ca8_jetpt","",nJetPtBins, JetPtMin, JetPtMax); 
  h_ca8_jetarea     = new TH1D("ca8_jetarea","", 100, 0., 10.);
  h_ca8_tau3tau2    = (TH1D*) h_ak5_tau2tau1->Clone("ca8_tau3tau2");
  h_ca8_tau2tau1    = (TH1D*) h_ak5_tau2tau1->Clone("ca8_tau2tau1");				 
  h_ca8_jetmass_tr  = (TH1D*) h_ca8_jetmass->Clone("ca8_jetmass_tr"); 
  h_ca8_jetpt_tr    = (TH1D*) h_ca8_jetpt->Clone("ca8_jetpt_tr"); 
  h_ca8_jetarea_tr  = (TH1D*) h_ca8_jetarea->Clone("ca8_jetarea_tr"); 		 
  h_ca8_jetmass_ft  = (TH1D*) h_ca8_jetmass->Clone("ca8_jetmass_ft"); 
  h_ca8_jetpt_ft    = (TH1D*) h_ca8_jetpt->Clone("ca8_jetpt_ft"); 
  h_ca8_jetarea_ft  = (TH1D*) h_ca8_jetarea->Clone("ca8_jetarea_ft"); 		 
  h_ca8_jetmass_pr  = (TH1D*) h_ca8_jetmass->Clone("ca8_jetmass_pr");
  h_ca8_jetpt_pr    = (TH1D*) h_ca8_jetpt->Clone("ca8_jetpt_pr"); 
  h_ca8_jetarea_pr  = (TH1D*) h_ca8_jetarea->Clone("ca8_jetarea_pr"); 
  h_ca8_massdrop_pr = new TH1D("ca8_massdrop_pr","", 200, 0., 2.);	

  hprof_ca8_jetmass_jetpt  = new TProfile("ca8_jetmass_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  hprof_ca8_jetmass_tr_jetpt_tr  = (TProfile*) hprof_ca8_jetmass_jetpt->Clone("ca8_jetmass_tr_jetpt_tr");
  hprof_ca8_jetmass_ft_jetpt_ft  = (TProfile*) hprof_ca8_jetmass_jetpt->Clone("ca8_jetmass_ft_jetpt_ft");
  hprof_ca8_jetmass_pr_jetpt_pr  = (TProfile*) hprof_ca8_jetmass_jetpt->Clone("ca8_jetmass_pr_jetpt_pr");
  hprof_ca8_tau2tau1_jetpt  = new TProfile("ca8_tau2tau1_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, 0.0, 1.0);


  //////// ------------- CA10 ----------------------------------------------
  h_ca10_numJets     = new TH1D("ca10_numJets","",20, -0.5, 19.5);
  h_ca10_jetmass     = new TH1D("ca10_jetmass","",nJetMassBins, JetMassMin, JetMassMax); 
  h_ca10_jetpt       = new TH1D("ca10_jetpt","",nJetPtBins, JetPtMin, JetPtMax); 
  h_ca10_jetarea     = new TH1D("ca10_jetarea","", 100, 0., 10.);
  h_ca10_tau3tau2    = (TH1D*) h_ak5_tau2tau1->Clone("ca10_tau3tau2");
  h_ca10_tau2tau1    = (TH1D*) h_ak5_tau2tau1->Clone("ca10_tau2tau1");				 
  h_ca10_jetmass_tr  = (TH1D*) h_ca10_jetmass->Clone("ca10_jetmass_tr"); 
  h_ca10_jetpt_tr    = (TH1D*) h_ca10_jetpt->Clone("ca10_jetpt_tr"); 
  h_ca10_jetarea_tr  = (TH1D*) h_ca10_jetarea->Clone("ca10_jetarea_tr"); 		 
  h_ca10_jetmass_ft  = (TH1D*) h_ca10_jetmass->Clone("ca10_jetmass_ft"); 
  h_ca10_jetpt_ft    = (TH1D*) h_ca10_jetpt->Clone("ca10_jetpt_ft"); 
  h_ca10_jetarea_ft  = (TH1D*) h_ca10_jetarea->Clone("ca10_jetarea_ft"); 		 
  h_ca10_jetmass_pr  = (TH1D*) h_ca10_jetmass->Clone("ca10_jetmass_pr");
  h_ca10_jetpt_pr    = (TH1D*) h_ca10_jetpt->Clone("ca10_jetpt_pr"); 
  h_ca10_jetarea_pr  = (TH1D*) h_ca10_jetarea->Clone("ca10_jetarea_pr"); 
  h_ca10_massdrop_pr = new TH1D("ca10_massdrop_pr","", 200, 0., 2.);	

  hprof_ca10_jetmass_jetpt  = new TProfile("ca10_jetmass_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  hprof_ca10_jetmass_tr_jetpt_tr  = (TProfile*) hprof_ca10_jetmass_jetpt->Clone("ca10_jetmass_tr_jetpt_tr");
  hprof_ca10_jetmass_ft_jetpt_ft  = (TProfile*) hprof_ca10_jetmass_jetpt->Clone("ca10_jetmass_ft_jetpt_ft");
  hprof_ca10_jetmass_pr_jetpt_pr  = (TProfile*) hprof_ca10_jetmass_jetpt->Clone("ca10_jetmass_pr_jetpt_pr");
  hprof_ca10_tau2tau1_jetpt  = new TProfile("ca10_tau2tau1_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, 0.0, 1.0);


  //////// ------------- KT5 ----------------------------------------------
  h_kt5_numJets     = new TH1D("kt5_numJets","",20, -0.5, 19.5);
  h_kt5_jetmass     = new TH1D("kt5_jetmass","",nJetMassBins, JetMassMin, JetMassMax); 
  h_kt5_jetpt       = new TH1D("kt5_jetpt","",nJetPtBins, JetPtMin, JetPtMax); 
  h_kt5_jetarea     = new TH1D("kt5_jetarea","", 100, 0., 10.);		
  h_kt5_tau2tau1    = new TH1D("kt5_tau2tau1","", 100, 0., 1.);
  h_kt5_tau3tau2    = (TH1D*) h_kt5_tau2tau1->Clone("kt5_tau3tau2"); 
  h_kt5_jetmass_tr  = (TH1D*) h_kt5_jetmass->Clone("kt5_jetmass_tr"); 
  h_kt5_jetpt_tr    = (TH1D*) h_kt5_jetpt->Clone("kt5_jetpt_tr"); 
  h_kt5_jetarea_tr  = (TH1D*) h_kt5_jetarea->Clone("kt5_jetarea_tr"); 		 
  h_kt5_jetmass_ft  = (TH1D*) h_kt5_jetmass->Clone("kt5_jetmass_ft"); 
  h_kt5_jetpt_ft    = (TH1D*) h_kt5_jetpt->Clone("kt5_jetpt_ft"); 
  h_kt5_jetarea_ft  = (TH1D*) h_kt5_jetarea->Clone("kt5_jetarea_ft"); 		 
  h_kt5_jetmass_pr  = (TH1D*) h_kt5_jetmass->Clone("kt5_jetmass_pr");
  h_kt5_jetpt_pr    = (TH1D*) h_kt5_jetpt->Clone("kt5_jetpt_pr"); 
  h_kt5_jetarea_pr  = (TH1D*) h_kt5_jetarea->Clone("kt5_jetarea_pr"); 
  h_kt5_massdrop_pr = new TH1D("kt5_massdrop_pr","", 200, 0., 2.);	

  hprof_kt5_jetmass_jetpt  = new TProfile("kt5_jetmass_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  hprof_kt5_jetmass_tr_jetpt_tr  = (TProfile*) hprof_kt5_jetmass_jetpt->Clone("kt5_jetmass_tr_jetpt_tr");
  hprof_kt5_jetmass_ft_jetpt_ft  = (TProfile*) hprof_kt5_jetmass_jetpt->Clone("kt5_jetmass_ft_jetpt_ft");
  hprof_kt5_jetmass_pr_jetpt_pr  = (TProfile*) hprof_kt5_jetmass_jetpt->Clone("kt5_jetmass_pr_jetpt_pr");
  hprof_kt5_tau2tau1_jetpt  = new TProfile("kt5_tau2tau1_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, 0.0, 1.0);



  //////// ------------- KT6 ----------------------------------------------
  h_kt6_numJets     = new TH1D("kt6_numJets","",20, -0.5, 19.5);
  h_kt6_jetmass     = new TH1D("kt6_jetmass","",nJetMassBins, JetMassMin, JetMassMax); 
  h_kt6_jetpt       = new TH1D("kt6_jetpt","",nJetPtBins, JetPtMin, JetPtMax); 
  h_kt6_jetarea     = new TH1D("kt6_jetarea","", 100, 0., 10.);	
  h_kt6_tau3tau2    = (TH1D*) h_kt5_tau2tau1->Clone("kt6_tau3tau2");
  h_kt6_tau2tau1    = (TH1D*) h_kt5_tau2tau1->Clone("kt6_tau2tau1");		 
  h_kt6_jetmass_tr  = (TH1D*) h_kt6_jetmass->Clone("kt6_jetmass_tr"); 
  h_kt6_jetpt_tr    = (TH1D*) h_kt6_jetpt->Clone("kt6_jetpt_tr"); 
  h_kt6_jetarea_tr  = (TH1D*) h_kt6_jetarea->Clone("kt6_jetarea_tr"); 		 
  h_kt6_jetmass_ft  = (TH1D*) h_kt6_jetmass->Clone("kt6_jetmass_ft"); 
  h_kt6_jetpt_ft    = (TH1D*) h_kt6_jetpt->Clone("kt6_jetpt_ft"); 
  h_kt6_jetarea_ft  = (TH1D*) h_kt6_jetarea->Clone("kt6_jetarea_ft"); 		 
  h_kt6_jetmass_pr  = (TH1D*) h_kt6_jetmass->Clone("kt6_jetmass_pr");
  h_kt6_jetpt_pr    = (TH1D*) h_kt6_jetpt->Clone("kt6_jetpt_pr"); 
  h_kt6_jetarea_pr  = (TH1D*) h_kt6_jetarea->Clone("kt6_jetarea_pr"); 
  h_kt6_massdrop_pr = new TH1D("kt6_massdrop_pr","", 200, 0., 2.);	

  hprof_kt6_jetmass_jetpt  = new TProfile("kt6_jetmass_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  hprof_kt6_jetmass_tr_jetpt_tr  = (TProfile*) hprof_kt6_jetmass_jetpt->Clone("kt6_jetmass_tr_jetpt_tr");
  hprof_kt6_jetmass_ft_jetpt_ft  = (TProfile*) hprof_kt6_jetmass_jetpt->Clone("kt6_jetmass_ft_jetpt_ft");
  hprof_kt6_jetmass_pr_jetpt_pr  = (TProfile*) hprof_kt6_jetmass_jetpt->Clone("kt6_jetmass_pr_jetpt_pr");
  hprof_kt6_tau2tau1_jetpt  = new TProfile("kt6_tau2tau1_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, 0.0, 1.0);

  //////// ------------- KT7 ----------------------------------------------
  h_kt7_numJets     = new TH1D("kt7_numJets","",20, -0.5, 19.5);
  h_kt7_jetmass     = new TH1D("kt7_jetmass","",nJetMassBins, JetMassMin, JetMassMax); 
  h_kt7_jetpt       = new TH1D("kt7_jetpt","",nJetPtBins, JetPtMin, JetPtMax); 
  h_kt7_jetarea     = new TH1D("kt7_jetarea","", 100, 0., 10.);
  h_kt7_tau3tau2    = (TH1D*) h_kt5_tau2tau1->Clone("kt7_tau3tau2");
  h_kt7_tau2tau1    = (TH1D*) h_kt5_tau2tau1->Clone("kt7_tau2tau1");				 
  h_kt7_jetmass_tr  = (TH1D*) h_kt7_jetmass->Clone("kt7_jetmass_tr"); 
  h_kt7_jetpt_tr    = (TH1D*) h_kt7_jetpt->Clone("kt7_jetpt_tr"); 
  h_kt7_jetarea_tr  = (TH1D*) h_kt7_jetarea->Clone("kt7_jetarea_tr"); 		 
  h_kt7_jetmass_ft  = (TH1D*) h_kt7_jetmass->Clone("kt7_jetmass_ft"); 
  h_kt7_jetpt_ft    = (TH1D*) h_kt7_jetpt->Clone("kt7_jetpt_ft"); 
  h_kt7_jetarea_ft  = (TH1D*) h_kt7_jetarea->Clone("kt7_jetarea_ft"); 		 
  h_kt7_jetmass_pr  = (TH1D*) h_kt7_jetmass->Clone("kt7_jetmass_pr");
  h_kt7_jetpt_pr    = (TH1D*) h_kt7_jetpt->Clone("kt7_jetpt_pr"); 
  h_kt7_jetarea_pr  = (TH1D*) h_kt7_jetarea->Clone("kt7_jetarea_pr"); 
  h_kt7_massdrop_pr = new TH1D("kt7_massdrop_pr","", 200, 0., 2.);	

  hprof_kt7_jetmass_jetpt  = new TProfile("kt7_jetmass_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  hprof_kt7_jetmass_tr_jetpt_tr  = (TProfile*) hprof_kt7_jetmass_jetpt->Clone("kt7_jetmass_tr_jetpt_tr");
  hprof_kt7_jetmass_ft_jetpt_ft  = (TProfile*) hprof_kt7_jetmass_jetpt->Clone("kt7_jetmass_ft_jetpt_ft");
  hprof_kt7_jetmass_pr_jetpt_pr  = (TProfile*) hprof_kt7_jetmass_jetpt->Clone("kt7_jetmass_pr_jetpt_pr");
  hprof_kt7_tau2tau1_jetpt  = new TProfile("kt7_tau2tau1_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, 0.0, 1.0);

  //////// ------------- KT8 ----------------------------------------------
  h_kt8_numJets     = new TH1D("kt8_numJets","",20, -0.5, 19.5);
  h_kt8_jetmass     = new TH1D("kt8_jetmass","",nJetMassBins, JetMassMin, JetMassMax); 
  h_kt8_jetpt       = new TH1D("kt8_jetpt","",nJetPtBins, JetPtMin, JetPtMax); 
  h_kt8_jetarea     = new TH1D("kt8_jetarea","", 100, 0., 10.);
  h_kt8_tau3tau2    = (TH1D*) h_kt5_tau2tau1->Clone("kt8_tau3tau2");
  h_kt8_tau2tau1    = (TH1D*) h_kt5_tau2tau1->Clone("kt8_tau2tau1");				 
  h_kt8_jetmass_tr  = (TH1D*) h_kt8_jetmass->Clone("kt8_jetmass_tr"); 
  h_kt8_jetpt_tr    = (TH1D*) h_kt8_jetpt->Clone("kt8_jetpt_tr"); 
  h_kt8_jetarea_tr  = (TH1D*) h_kt8_jetarea->Clone("kt8_jetarea_tr"); 		 
  h_kt8_jetmass_ft  = (TH1D*) h_kt8_jetmass->Clone("kt8_jetmass_ft"); 
  h_kt8_jetpt_ft    = (TH1D*) h_kt8_jetpt->Clone("kt8_jetpt_ft"); 
  h_kt8_jetarea_ft  = (TH1D*) h_kt8_jetarea->Clone("kt8_jetarea_ft"); 		 
  h_kt8_jetmass_pr  = (TH1D*) h_kt8_jetmass->Clone("kt8_jetmass_pr");
  h_kt8_jetpt_pr    = (TH1D*) h_kt8_jetpt->Clone("kt8_jetpt_pr"); 
  h_kt8_jetarea_pr  = (TH1D*) h_kt8_jetarea->Clone("kt8_jetarea_pr"); 
  h_kt8_massdrop_pr = new TH1D("kt8_massdrop_pr","", 200, 0., 2.);	

  hprof_kt8_jetmass_jetpt  = new TProfile("kt8_jetmass_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, JetMassMin, JetMassMax);
  hprof_kt8_jetmass_tr_jetpt_tr  = (TProfile*) hprof_kt8_jetmass_jetpt->Clone("kt8_jetmass_tr_jetpt_tr");
  hprof_kt8_jetmass_ft_jetpt_ft  = (TProfile*) hprof_kt8_jetmass_jetpt->Clone("kt8_jetmass_ft_jetpt_ft");
  hprof_kt8_jetmass_pr_jetpt_pr  = (TProfile*) hprof_kt8_jetmass_jetpt->Clone("kt8_jetmass_pr_jetpt_pr");
  hprof_kt8_tau2tau1_jetpt  = new TProfile("kt8_tau2tau1_jetpt",
					  "Profile of mass versus pt", 
					  nJetPtBins, JetPtMin, JetPtMax, 0.0, 1.0);




  return true;
}


bool MB_Py8::write(const std::string& outName)
{
  TFile* outFile = new TFile(outName.c_str(),"RECREATE");

  writeHist(h_mu);
  writeHist(h_rap);
  writeHist(h_eta);
  writeHist(h_perp);
  writeHist(h_rap_charged);
  writeHist(h_eta_charged);
  writeHist(h_perp_charged);

  //////// ------------- AK5 -------------------
  writeHist(h_ak5_numJets);
  writeHist(h_ak5_jetmass); 
  writeHist(h_ak5_jetpt); 
  writeHist(h_ak5_jetarea);
  writeHist(h_ak5_tau2tau1);
  writeHist(h_ak5_tau3tau2);	 
  writeHist(h_ak5_jetmass_tr); 
  writeHist(h_ak5_jetpt_tr); 
  writeHist(h_ak5_jetarea_tr); 		 
  writeHist(h_ak5_jetmass_ft); 
  writeHist(h_ak5_jetpt_ft); 
  writeHist(h_ak5_jetarea_ft); 			 
  writeHist(h_ak5_jetmass_pr);
  writeHist(h_ak5_jetpt_pr); 
  writeHist(h_ak5_jetarea_pr); 
  writeHist(h_ak5_massdrop_pr);
  writeHist(hprof_ak5_jetmass_jetpt);
  writeHist(hprof_ak5_jetmass_tr_jetpt_tr);
  writeHist(hprof_ak5_jetmass_ft_jetpt_ft);
  writeHist(hprof_ak5_jetmass_pr_jetpt_pr);
  writeHist(hprof_ak5_tau2tau1_jetpt);

  //////// ------------- AK6 -------------------
  writeHist(h_ak6_numJets);
  writeHist(h_ak6_jetmass); 
  writeHist(h_ak6_jetpt); 
  writeHist(h_ak6_jetarea);
  writeHist(h_ak6_tau2tau1);
  writeHist(h_ak6_tau3tau2);			 
  writeHist(h_ak6_jetmass_tr); 
  writeHist(h_ak6_jetpt_tr); 
  writeHist(h_ak6_jetarea_tr); 		 
  writeHist(h_ak6_jetmass_ft); 
  writeHist(h_ak6_jetpt_ft); 
  writeHist(h_ak6_jetarea_ft); 			 
  writeHist(h_ak6_jetmass_pr);
  writeHist(h_ak6_jetpt_pr); 
  writeHist(h_ak6_jetarea_pr); 
  writeHist(h_ak6_massdrop_pr);
  writeHist(hprof_ak6_jetmass_jetpt);
  writeHist(hprof_ak6_jetmass_tr_jetpt_tr);
  writeHist(hprof_ak6_jetmass_ft_jetpt_ft);
  writeHist(hprof_ak6_jetmass_pr_jetpt_pr);
  writeHist(hprof_ak6_tau2tau1_jetpt);

  //////// ------------- AK7 -------------------
  writeHist(h_ak7_numJets);
  writeHist(h_ak7_jetmass); 
  writeHist(h_ak7_jetpt); 
  writeHist(h_ak7_jetarea);
  writeHist(h_ak7_tau2tau1);
  writeHist(h_ak7_tau3tau2);			 
  writeHist(h_ak7_jetmass_tr); 
  writeHist(h_ak7_jetpt_tr); 
  writeHist(h_ak7_jetarea_tr); 		 
  writeHist(h_ak7_jetmass_ft); 
  writeHist(h_ak7_jetpt_ft); 
  writeHist(h_ak7_jetarea_ft); 			 
  writeHist(h_ak7_jetmass_pr);
  writeHist(h_ak7_jetpt_pr); 
  writeHist(h_ak7_jetarea_pr); 
  writeHist(h_ak7_massdrop_pr);
  writeHist(hprof_ak7_jetmass_jetpt);
  writeHist(hprof_ak7_jetmass_tr_jetpt_tr);
  writeHist(hprof_ak7_jetmass_ft_jetpt_ft);
  writeHist(hprof_ak7_jetmass_pr_jetpt_pr);
  writeHist(hprof_ak7_tau2tau1_jetpt);


  //////// ------------- AK8 -------------------
  writeHist(h_ak8_numJets);
  writeHist(h_ak8_jetmass); 
  writeHist(h_ak8_jetpt); 
  writeHist(h_ak8_jetarea);
  writeHist(h_ak8_tau2tau1);
  writeHist(h_ak8_tau3tau2);			 
  writeHist(h_ak8_jetmass_tr); 
  writeHist(h_ak8_jetpt_tr); 
  writeHist(h_ak8_jetarea_tr); 		 
  writeHist(h_ak8_jetmass_ft); 
  writeHist(h_ak8_jetpt_ft); 
  writeHist(h_ak8_jetarea_ft); 			 
  writeHist(h_ak8_jetmass_pr);
  writeHist(h_ak8_jetpt_pr); 
  writeHist(h_ak8_jetarea_pr); 
  writeHist(h_ak8_massdrop_pr);
  writeHist(hprof_ak8_jetmass_jetpt);
  writeHist(hprof_ak8_jetmass_tr_jetpt_tr);
  writeHist(hprof_ak8_jetmass_ft_jetpt_ft);
  writeHist(hprof_ak8_jetmass_pr_jetpt_pr);
  writeHist(hprof_ak8_tau2tau1_jetpt);

  //////// ------------- CA8 -------------------
  writeHist(h_ca8_numJets);
  writeHist(h_ca8_jetmass); 
  writeHist(h_ca8_jetpt); 
  writeHist(h_ca8_jetarea);
  writeHist(h_ca8_tau2tau1);
  writeHist(h_ca8_tau3tau2);			 
  writeHist(h_ca8_jetmass_tr); 
  writeHist(h_ca8_jetpt_tr); 
  writeHist(h_ca8_jetarea_tr); 		 
  writeHist(h_ca8_jetmass_ft); 
  writeHist(h_ca8_jetpt_ft); 
  writeHist(h_ca8_jetarea_ft); 			 
  writeHist(h_ca8_jetmass_pr);
  writeHist(h_ca8_jetpt_pr); 
  writeHist(h_ca8_jetarea_pr); 
  writeHist(h_ca8_massdrop_pr);
  writeHist(hprof_ca8_jetmass_jetpt);
  writeHist(hprof_ca8_jetmass_tr_jetpt_tr);
  writeHist(hprof_ca8_jetmass_ft_jetpt_ft);
  writeHist(hprof_ca8_jetmass_pr_jetpt_pr);
  writeHist(hprof_ca8_tau2tau1_jetpt);

  //////// ------------- CA10 -------------------
  writeHist(h_ca10_numJets);
  writeHist(h_ca10_jetmass); 
  writeHist(h_ca10_jetpt); 
  writeHist(h_ca10_jetarea);
  writeHist(h_ca10_tau2tau1);
  writeHist(h_ca10_tau3tau2);			 
  writeHist(h_ca10_jetmass_tr); 
  writeHist(h_ca10_jetpt_tr); 
  writeHist(h_ca10_jetarea_tr); 		 
  writeHist(h_ca10_jetmass_ft); 
  writeHist(h_ca10_jetpt_ft); 
  writeHist(h_ca10_jetarea_ft); 			 
  writeHist(h_ca10_jetmass_pr);
  writeHist(h_ca10_jetpt_pr); 
  writeHist(h_ca10_jetarea_pr); 
  writeHist(h_ca10_massdrop_pr);
  writeHist(hprof_ca10_jetmass_jetpt);
  writeHist(hprof_ca10_jetmass_tr_jetpt_tr);
  writeHist(hprof_ca10_jetmass_ft_jetpt_ft);
  writeHist(hprof_ca10_jetmass_pr_jetpt_pr);
  writeHist(hprof_ca10_tau2tau1_jetpt);

  //////// ------------- KT5 -------------------
  writeHist(h_kt5_numJets);
  writeHist(h_kt5_jetmass); 
  writeHist(h_kt5_jetpt); 
  writeHist(h_kt5_jetarea);
  writeHist(h_kt5_tau2tau1);
  writeHist(h_kt5_tau3tau2);	 
  writeHist(h_kt5_jetmass_tr); 
  writeHist(h_kt5_jetpt_tr); 
  writeHist(h_kt5_jetarea_tr); 		 
  writeHist(h_kt5_jetmass_ft); 
  writeHist(h_kt5_jetpt_ft); 
  writeHist(h_kt5_jetarea_ft); 			 
  writeHist(h_kt5_jetmass_pr);
  writeHist(h_kt5_jetpt_pr); 
  writeHist(h_kt5_jetarea_pr); 
  writeHist(h_kt5_massdrop_pr);
  writeHist(hprof_kt5_jetmass_jetpt);
  writeHist(hprof_kt5_jetmass_tr_jetpt_tr);
  writeHist(hprof_kt5_jetmass_ft_jetpt_ft);
  writeHist(hprof_kt5_jetmass_pr_jetpt_pr);
  writeHist(hprof_kt5_tau2tau1_jetpt);

  //////// ------------- KT6 -------------------
  writeHist(h_kt6_numJets);
  writeHist(h_kt6_jetmass); 
  writeHist(h_kt6_jetpt); 
  writeHist(h_kt6_jetarea);
  writeHist(h_kt6_tau2tau1);
  writeHist(h_kt6_tau3tau2);			 
  writeHist(h_kt6_jetmass_tr); 
  writeHist(h_kt6_jetpt_tr); 
  writeHist(h_kt6_jetarea_tr); 		 
  writeHist(h_kt6_jetmass_ft); 
  writeHist(h_kt6_jetpt_ft); 
  writeHist(h_kt6_jetarea_ft); 			 
  writeHist(h_kt6_jetmass_pr);
  writeHist(h_kt6_jetpt_pr); 
  writeHist(h_kt6_jetarea_pr); 
  writeHist(h_kt6_massdrop_pr);
  writeHist(hprof_kt6_jetmass_jetpt);
  writeHist(hprof_kt6_jetmass_tr_jetpt_tr);
  writeHist(hprof_kt6_jetmass_ft_jetpt_ft);
  writeHist(hprof_kt6_jetmass_pr_jetpt_pr);
  writeHist(hprof_kt6_tau2tau1_jetpt);

  //////// ------------- KT7 -------------------
  writeHist(h_kt7_numJets);
  writeHist(h_kt7_jetmass); 
  writeHist(h_kt7_jetpt); 
  writeHist(h_kt7_jetarea);
  writeHist(h_kt7_tau2tau1);
  writeHist(h_kt7_tau3tau2);			 
  writeHist(h_kt7_jetmass_tr); 
  writeHist(h_kt7_jetpt_tr); 
  writeHist(h_kt7_jetarea_tr); 		 
  writeHist(h_kt7_jetmass_ft); 
  writeHist(h_kt7_jetpt_ft); 
  writeHist(h_kt7_jetarea_ft); 			 
  writeHist(h_kt7_jetmass_pr);
  writeHist(h_kt7_jetpt_pr); 
  writeHist(h_kt7_jetarea_pr); 
  writeHist(h_kt7_massdrop_pr);
  writeHist(hprof_kt7_jetmass_jetpt);
  writeHist(hprof_kt7_jetmass_tr_jetpt_tr);
  writeHist(hprof_kt7_jetmass_ft_jetpt_ft);
  writeHist(hprof_kt7_jetmass_pr_jetpt_pr);
  writeHist(hprof_kt7_tau2tau1_jetpt);


  //////// ------------- KT8 -------------------
  writeHist(h_kt8_numJets);
  writeHist(h_kt8_jetmass); 
  writeHist(h_kt8_jetpt); 
  writeHist(h_kt8_jetarea);
  writeHist(h_kt8_tau2tau1);
  writeHist(h_kt8_tau3tau2);			 
  writeHist(h_kt8_jetmass_tr); 
  writeHist(h_kt8_jetpt_tr); 
  writeHist(h_kt8_jetarea_tr); 		 
  writeHist(h_kt8_jetmass_ft); 
  writeHist(h_kt8_jetpt_ft); 
  writeHist(h_kt8_jetarea_ft); 			 
  writeHist(h_kt8_jetmass_pr);
  writeHist(h_kt8_jetpt_pr); 
  writeHist(h_kt8_jetarea_pr); 
  writeHist(h_kt8_massdrop_pr);
  writeHist(hprof_kt8_jetmass_jetpt);
  writeHist(hprof_kt8_jetmass_tr_jetpt_tr);
  writeHist(hprof_kt8_jetmass_ft_jetpt_ft);
  writeHist(hprof_kt8_jetmass_pr_jetpt_pr);
  writeHist(hprof_kt8_tau2tau1_jetpt);

  outFile->Close();

  return true;
}

bool MB_Py8::analyze(Event& pEvt, Event& sigEvt, bool PUsub)
{
  // retrieve all particles
  std::vector<fastjet::PseudoJet> pPart = pEvt.pseudoJets(-1);
  std::vector<fastjet::PseudoJet> sigPart = sigEvt.pseudoJets(-1);
  pPart.insert(pPart.end(), sigPart.begin(), sigPart.end());


  // event distributions
  h_mu->Fill((double)pEvt.nVertices());
  std::vector<fastjet::PseudoJet>::const_iterator fPart(pPart.begin());
  std::vector<fastjet::PseudoJet>::const_iterator lPart(pPart.end());
  for ( ; fPart != lPart; ++fPart )
    {
      h_rap->Fill(fPart->rap());
      h_eta->Fill(fPart->pseudorapidity());
      h_perp->Fill(fPart->perp());
      if ( Event::isCharged(*fPart) )
	{ h_rap_charged->Fill(fPart->rap());
	  h_eta_charged->Fill(fPart->pseudorapidity());
	  h_perp_charged->Fill(fPart->perp());
	}
    }

  analyzeJetSubstructure(pPart, fastjet::antikt_algorithm, 0.5, PUsub);
  analyzeJetSubstructure(pPart, fastjet::antikt_algorithm, 0.6, PUsub);
  analyzeJetSubstructure(pPart, fastjet::antikt_algorithm, 0.7, PUsub);
  analyzeJetSubstructure(pPart, fastjet::antikt_algorithm, 0.8, PUsub);
  analyzeJetSubstructure(pPart, fastjet::cambridge_algorithm, 0.8, PUsub);
  analyzeJetSubstructure(pPart, fastjet::cambridge_algorithm, 1.0, PUsub);
  analyzeJetSubstructure(pPart, fastjet::kt_algorithm, 0.5, PUsub);
  analyzeJetSubstructure(pPart, fastjet::kt_algorithm, 0.6, PUsub);
  analyzeJetSubstructure(pPart, fastjet::kt_algorithm, 0.7, PUsub);
  analyzeJetSubstructure(pPart, fastjet::kt_algorithm, 0.8, PUsub);

  return true;
}




void MB_Py8::analyzeJetSubstructure(std::vector<fastjet::PseudoJet> pPart, 
				    fastjet::JetAlgorithm jetAlgo, double mJetRadius, 
				    bool PUsub) {

  TH1D* numJets; 
  TH1D* jetmass; 
  TH1D* jetpt; 
  TH1D* jetarea; 
  TH1D* tau2tau1;
  TH1D* tau3tau2;
  TH1D* jetmass_tr; 
  TH1D* jetpt_tr; 
  TH1D* jetarea_tr;
  TH1D* jetmass_ft; 
  TH1D* jetpt_ft; 
  TH1D* jetarea_ft;
  TH1D* jetmass_pr; 
  TH1D* jetpt_pr; 
  TH1D* jetarea_pr; 
  TH1D* massdrop_pr; 
  TProfile* jetmass_jetpt;
  TProfile* jetmass_tr_jetpt_tr;
  TProfile* jetmass_ft_jetpt_ft; 
  TProfile* jetmass_pr_jetpt_pr;
  TProfile* tau2tau1_jetpt;

  if(jetAlgo == fastjet::antikt_algorithm && fabs(mJetRadius-0.5)<0.001) {
    numJets                    = h_ak5_numJets; 
    jetmass                    = h_ak5_jetmass; 
    jetpt                      = h_ak5_jetpt; 
    jetarea                    = h_ak5_jetarea; 
    tau2tau1                = h_ak5_tau2tau1;
    tau3tau2                = h_ak5_tau3tau2;
    jetmass_tr                 = h_ak5_jetmass_tr; 
    jetpt_tr                   = h_ak5_jetpt_tr; 
    jetarea_tr                 = h_ak5_jetarea_tr;
    jetmass_ft                 = h_ak5_jetmass_ft; 
    jetpt_ft                   = h_ak5_jetpt_ft; 
    jetarea_ft                 = h_ak5_jetarea_ft;
    jetmass_pr                 = h_ak5_jetmass_pr; 
    jetpt_pr                   = h_ak5_jetpt_pr; 
    jetarea_pr                 = h_ak5_jetarea_pr; 
    massdrop_pr                = h_ak5_massdrop_pr; 
    jetmass_jetpt              = hprof_ak5_jetmass_jetpt;
    jetmass_tr_jetpt_tr        = hprof_ak5_jetmass_tr_jetpt_tr;
    jetmass_ft_jetpt_ft        = hprof_ak5_jetmass_ft_jetpt_ft; 
    jetmass_pr_jetpt_pr        = hprof_ak5_jetmass_pr_jetpt_pr;
    tau2tau1_jetpt             = hprof_ak5_tau2tau1_jetpt;
  }
  if(jetAlgo == fastjet::antikt_algorithm && fabs(mJetRadius-0.6)<0.001) {
    numJets                    = h_ak6_numJets; 
    jetmass                    = h_ak6_jetmass; 
    jetpt                      = h_ak6_jetpt; 
    jetarea                    = h_ak6_jetarea; 
    tau2tau1                = h_ak6_tau2tau1;
    tau3tau2                = h_ak6_tau3tau2;
    jetmass_tr                 = h_ak6_jetmass_tr; 
    jetpt_tr                   = h_ak6_jetpt_tr; 
    jetarea_tr                 = h_ak6_jetarea_tr;
    jetmass_ft                 = h_ak6_jetmass_ft; 
    jetpt_ft                   = h_ak6_jetpt_ft; 
    jetarea_ft                 = h_ak6_jetarea_ft;
    jetmass_pr                 = h_ak6_jetmass_pr; 
    jetpt_pr                   = h_ak6_jetpt_pr; 
    jetarea_pr                 = h_ak6_jetarea_pr; 
    massdrop_pr                = h_ak6_massdrop_pr; 
    jetmass_jetpt              = hprof_ak6_jetmass_jetpt;
    jetmass_tr_jetpt_tr        = hprof_ak6_jetmass_tr_jetpt_tr;
    jetmass_ft_jetpt_ft        = hprof_ak6_jetmass_ft_jetpt_ft; 
    jetmass_pr_jetpt_pr        = hprof_ak6_jetmass_pr_jetpt_pr;
    tau2tau1_jetpt             = hprof_ak6_tau2tau1_jetpt;
  }
  if(jetAlgo == fastjet::antikt_algorithm && fabs(mJetRadius-0.7)<0.001) {
    numJets                    = h_ak7_numJets; 
    jetmass                    = h_ak7_jetmass; 
    jetpt                      = h_ak7_jetpt; 
    jetarea                    = h_ak7_jetarea; 
    tau2tau1                = h_ak7_tau2tau1;
    tau3tau2                = h_ak7_tau3tau2;
    jetmass_tr                 = h_ak7_jetmass_tr; 
    jetpt_tr                   = h_ak7_jetpt_tr; 
    jetarea_tr                 = h_ak7_jetarea_tr;
    jetmass_ft                 = h_ak7_jetmass_ft; 
    jetpt_ft                   = h_ak7_jetpt_ft; 
    jetarea_ft                 = h_ak7_jetarea_ft;
    jetmass_pr                 = h_ak7_jetmass_pr; 
    jetpt_pr                   = h_ak7_jetpt_pr; 
    jetarea_pr                 = h_ak7_jetarea_pr; 
    massdrop_pr                = h_ak7_massdrop_pr; 
    jetmass_jetpt              = hprof_ak7_jetmass_jetpt;
    jetmass_tr_jetpt_tr        = hprof_ak7_jetmass_tr_jetpt_tr;
    jetmass_ft_jetpt_ft        = hprof_ak7_jetmass_ft_jetpt_ft; 
    jetmass_pr_jetpt_pr        = hprof_ak7_jetmass_pr_jetpt_pr;
    tau2tau1_jetpt             = hprof_ak7_tau2tau1_jetpt;
  }
  if(jetAlgo == fastjet::antikt_algorithm && fabs(mJetRadius-0.8)<0.001) {
    numJets                    = h_ak8_numJets; 
    jetmass                    = h_ak8_jetmass; 
    jetpt                      = h_ak8_jetpt; 
    jetarea                    = h_ak8_jetarea; 
    tau2tau1                = h_ak8_tau2tau1;
    tau3tau2                = h_ak8_tau3tau2;
    jetmass_tr                 = h_ak8_jetmass_tr; 
    jetpt_tr                   = h_ak8_jetpt_tr; 
    jetarea_tr                 = h_ak8_jetarea_tr;
    jetmass_ft                 = h_ak8_jetmass_ft; 
    jetpt_ft                   = h_ak8_jetpt_ft; 
    jetarea_ft                 = h_ak8_jetarea_ft;
    jetmass_pr                 = h_ak8_jetmass_pr; 
    jetpt_pr                   = h_ak8_jetpt_pr; 
    jetarea_pr                 = h_ak8_jetarea_pr; 
    massdrop_pr                = h_ak8_massdrop_pr; 
    jetmass_jetpt              = hprof_ak8_jetmass_jetpt;
    jetmass_tr_jetpt_tr        = hprof_ak8_jetmass_tr_jetpt_tr;
    jetmass_ft_jetpt_ft        = hprof_ak8_jetmass_ft_jetpt_ft; 
    jetmass_pr_jetpt_pr        = hprof_ak8_jetmass_pr_jetpt_pr;
    tau2tau1_jetpt             = hprof_ak8_tau2tau1_jetpt;
  }
  if(jetAlgo == fastjet::cambridge_algorithm && fabs(mJetRadius-0.8)<0.001) {
    numJets                    = h_ca8_numJets; 
    jetmass                    = h_ca8_jetmass; 
    jetpt                      = h_ca8_jetpt; 
    jetarea                    = h_ca8_jetarea; 
    tau2tau1                = h_ca8_tau2tau1;
    tau3tau2                = h_ca8_tau3tau2;
    jetmass_tr                 = h_ca8_jetmass_tr; 
    jetpt_tr                   = h_ca8_jetpt_tr; 
    jetarea_tr                 = h_ca8_jetarea_tr;
    jetmass_ft                 = h_ca8_jetmass_ft; 
    jetpt_ft                   = h_ca8_jetpt_ft; 
    jetarea_ft                 = h_ca8_jetarea_ft;
    jetmass_pr                 = h_ca8_jetmass_pr; 
    jetpt_pr                   = h_ca8_jetpt_pr; 
    jetarea_pr                 = h_ca8_jetarea_pr; 
    massdrop_pr                = h_ca8_massdrop_pr; 
    jetmass_jetpt              = hprof_ca8_jetmass_jetpt;
    jetmass_tr_jetpt_tr        = hprof_ca8_jetmass_tr_jetpt_tr;
    jetmass_ft_jetpt_ft        = hprof_ca8_jetmass_ft_jetpt_ft; 
    jetmass_pr_jetpt_pr        = hprof_ca8_jetmass_pr_jetpt_pr;
    tau2tau1_jetpt             = hprof_ca8_tau2tau1_jetpt;
  }
  if(jetAlgo == fastjet::cambridge_algorithm && fabs(mJetRadius-1.0)<0.001) {
    numJets                    = h_ca10_numJets; 
    jetmass                    = h_ca10_jetmass; 
    jetpt                      = h_ca10_jetpt; 
    jetarea                    = h_ca10_jetarea; 
    tau2tau1                = h_ca10_tau2tau1;
    tau3tau2                = h_ca10_tau3tau2;
    jetmass_tr                 = h_ca10_jetmass_tr; 
    jetpt_tr                   = h_ca10_jetpt_tr; 
    jetarea_tr                 = h_ca10_jetarea_tr;
    jetmass_ft                 = h_ca10_jetmass_ft; 
    jetpt_ft                   = h_ca10_jetpt_ft; 
    jetarea_ft                 = h_ca10_jetarea_ft;
    jetmass_pr                 = h_ca10_jetmass_pr; 
    jetpt_pr                   = h_ca10_jetpt_pr; 
    jetarea_pr                 = h_ca10_jetarea_pr; 
    massdrop_pr                = h_ca10_massdrop_pr; 
    jetmass_jetpt              = hprof_ca10_jetmass_jetpt;
    jetmass_tr_jetpt_tr        = hprof_ca10_jetmass_tr_jetpt_tr;
    jetmass_ft_jetpt_ft        = hprof_ca10_jetmass_ft_jetpt_ft; 
    jetmass_pr_jetpt_pr        = hprof_ca10_jetmass_pr_jetpt_pr;
    tau2tau1_jetpt             = hprof_ca10_tau2tau1_jetpt;
  }
  if(jetAlgo == fastjet::kt_algorithm && fabs(mJetRadius-0.5)<0.001) {
    numJets                    = h_kt5_numJets; 
    jetmass                    = h_kt5_jetmass; 
    jetpt                      = h_kt5_jetpt; 
    jetarea                    = h_kt5_jetarea; 
    tau2tau1                = h_kt5_tau2tau1;
    tau3tau2                = h_kt5_tau3tau2;
    jetmass_tr                 = h_kt5_jetmass_tr; 
    jetpt_tr                   = h_kt5_jetpt_tr; 
    jetarea_tr                 = h_kt5_jetarea_tr;
    jetmass_ft                 = h_kt5_jetmass_ft; 
    jetpt_ft                   = h_kt5_jetpt_ft; 
    jetarea_ft                 = h_kt5_jetarea_ft;
    jetmass_pr                 = h_kt5_jetmass_pr; 
    jetpt_pr                   = h_kt5_jetpt_pr; 
    jetarea_pr                 = h_kt5_jetarea_pr; 
    massdrop_pr                = h_kt5_massdrop_pr; 
    jetmass_jetpt              = hprof_kt5_jetmass_jetpt;
    jetmass_tr_jetpt_tr        = hprof_kt5_jetmass_tr_jetpt_tr;
    jetmass_ft_jetpt_ft        = hprof_kt5_jetmass_ft_jetpt_ft; 
    jetmass_pr_jetpt_pr        = hprof_kt5_jetmass_pr_jetpt_pr;
    tau2tau1_jetpt             = hprof_kt5_tau2tau1_jetpt;
  }
  if(jetAlgo == fastjet::kt_algorithm && fabs(mJetRadius-0.6)<0.001) {
    numJets                    = h_kt6_numJets; 
    jetmass                    = h_kt6_jetmass; 
    jetpt                      = h_kt6_jetpt; 
    jetarea                    = h_kt6_jetarea; 
    tau2tau1                = h_kt6_tau2tau1;
    tau3tau2                = h_kt6_tau3tau2;
    jetmass_tr                 = h_kt6_jetmass_tr; 
    jetpt_tr                   = h_kt6_jetpt_tr; 
    jetarea_tr                 = h_kt6_jetarea_tr;
    jetmass_ft                 = h_kt6_jetmass_ft; 
    jetpt_ft                   = h_kt6_jetpt_ft; 
    jetarea_ft                 = h_kt6_jetarea_ft;
    jetmass_pr                 = h_kt6_jetmass_pr; 
    jetpt_pr                   = h_kt6_jetpt_pr; 
    jetarea_pr                 = h_kt6_jetarea_pr; 
    massdrop_pr                = h_kt6_massdrop_pr; 
    jetmass_jetpt              = hprof_kt6_jetmass_jetpt;
    jetmass_tr_jetpt_tr        = hprof_kt6_jetmass_tr_jetpt_tr;
    jetmass_ft_jetpt_ft        = hprof_kt6_jetmass_ft_jetpt_ft; 
    jetmass_pr_jetpt_pr        = hprof_kt6_jetmass_pr_jetpt_pr;
    tau2tau1_jetpt             = hprof_kt6_tau2tau1_jetpt;
  }
  if(jetAlgo == fastjet::kt_algorithm && fabs(mJetRadius-0.7)<0.001) {
    numJets                    = h_kt7_numJets; 
    jetmass                    = h_kt7_jetmass; 
    jetpt                      = h_kt7_jetpt; 
    jetarea                    = h_kt7_jetarea; 
    tau2tau1                = h_kt7_tau2tau1;
    tau3tau2                = h_kt7_tau3tau2;
    jetmass_tr                 = h_kt7_jetmass_tr; 
    jetpt_tr                   = h_kt7_jetpt_tr; 
    jetarea_tr                 = h_kt7_jetarea_tr;
    jetmass_ft                 = h_kt7_jetmass_ft; 
    jetpt_ft                   = h_kt7_jetpt_ft; 
    jetarea_ft                 = h_kt7_jetarea_ft;
    jetmass_pr                 = h_kt7_jetmass_pr; 
    jetpt_pr                   = h_kt7_jetpt_pr; 
    jetarea_pr                 = h_kt7_jetarea_pr; 
    massdrop_pr                = h_kt7_massdrop_pr; 
    jetmass_jetpt              = hprof_kt7_jetmass_jetpt;
    jetmass_tr_jetpt_tr        = hprof_kt7_jetmass_tr_jetpt_tr;
    jetmass_ft_jetpt_ft        = hprof_kt7_jetmass_ft_jetpt_ft; 
    jetmass_pr_jetpt_pr        = hprof_kt7_jetmass_pr_jetpt_pr;
    tau2tau1_jetpt             = hprof_kt7_tau2tau1_jetpt;
  }
  if(jetAlgo == fastjet::kt_algorithm && fabs(mJetRadius-0.8)<0.001) {
    numJets                    = h_kt8_numJets; 
    jetmass                    = h_kt8_jetmass; 
    jetpt                      = h_kt8_jetpt; 
    jetarea                    = h_kt8_jetarea; 
    tau2tau1                = h_kt8_tau2tau1;
    tau3tau2                = h_kt8_tau3tau2;
    jetmass_tr                 = h_kt8_jetmass_tr; 
    jetpt_tr                   = h_kt8_jetpt_tr; 
    jetarea_tr                 = h_kt8_jetarea_tr;
    jetmass_ft                 = h_kt8_jetmass_ft; 
    jetpt_ft                   = h_kt8_jetpt_ft; 
    jetarea_ft                 = h_kt8_jetarea_ft;
    jetmass_pr                 = h_kt8_jetmass_pr; 
    jetpt_pr                   = h_kt8_jetpt_pr; 
    jetarea_pr                 = h_kt8_jetarea_pr; 
    massdrop_pr                = h_kt8_massdrop_pr; 
    jetmass_jetpt              = hprof_kt8_jetmass_jetpt;
    jetmass_tr_jetpt_tr        = hprof_kt8_jetmass_tr_jetpt_tr;
    jetmass_ft_jetpt_ft        = hprof_kt8_jetmass_ft_jetpt_ft; 
    jetmass_pr_jetpt_pr        = hprof_kt8_jetmass_pr_jetpt_pr;
    tau2tau1_jetpt             = hprof_kt8_tau2tau1_jetpt;
  }

  // do re-clustering
  fastjet::JetDefinition jetDef(jetAlgo, mJetRadius);

  int activeAreaRepeats = 1;
  double ghostArea = 0.01;
  double ghostEtaMax = 4.0;
  double jetPtMin = 100.0;


  fastjet::GhostedAreaSpec fjActiveArea(ghostEtaMax,activeAreaRepeats,ghostArea);
  fjActiveArea.set_fj2_placement(true);
  fastjet::AreaDefinition fjAreaDefinition( fastjet::active_area_explicit_ghosts, fjActiveArea );
  fastjet::ClusterSequenceArea thisClustering(pPart, jetDef, fjAreaDefinition);
    


  // ------- pileup subtraction stuff --------------
  fastjet::JetDefinition jet_def_for_rho(fastjet::kt_algorithm, 0.4);
  fastjet::Selector rho_range = fastjet::SelectorAbsRapMax(ghostEtaMax);
  // the estimation of the background itself
  // fastjet::JetMedianBackgroundEstimator bge_rho(rho_range, jet_def_for_rho, fjAreaDefinition);
  fastjet::ClusterSequenceArea puClustering(pPart, jet_def_for_rho, fjAreaDefinition);
  fastjet::JetMedianBackgroundEstimator bge_rho(rho_range, puClustering);

  // for Pythia8(4C) minbias events, we've studied the rapidity 
  // dependence so we can easily correct for it by applying
  // (Gregory Soyez has a reference for that if needed)
  fastjet::BackgroundRescalingYPolynomial rescaling(1.05114, 0, -0.0236079, 0, 2.22773e-05);
  bge_rho.set_rescaling_class(&rescaling);
  fastjet::Subtractor subtractor(&bge_rho);
  std::vector<fastjet::PseudoJet> out_jets = sorted_by_pt(thisClustering.inclusive_jets(jetPtMin));
  if(PUsub) out_jets = subtractor(out_jets);


  // define groomers
  fastjet::Filter trimmer( fastjet::Filter(fastjet::JetDefinition(fastjet::kt_algorithm, 0.2), fastjet::SelectorPtFractionMin(0.03)));
  fastjet::Filter filter( fastjet::Filter(fastjet::JetDefinition(fastjet::cambridge_algorithm, 0.3), fastjet::SelectorNHardest(3)));
  fastjet::Pruner pruner(fastjet::cambridge_algorithm, 0.1, 0.5);
    
  std::vector<fastjet::Transformer const *> transformers;
  transformers.push_back(&trimmer);
  transformers.push_back(&filter);
  transformers.push_back(&pruner);
    
  // Defining Nsubjettiness parameters
  double beta = 1.0; // power for angular dependence, e.g. beta = 1 --> linear k-means, beta = 2 --> quadratic/classic k-means
  double R0 = mJetRadius; // Characteristic jet radius for normalization	      
  double Rcut = mJetRadius; // maximum R particles can be from axis to be included in jet	      
    
  numJets->Fill( (int) (out_jets.size()));
  const int NUM_JET_MAX = 1;

  for (unsigned j = 0; j < out_jets.size() && int(j)<NUM_JET_MAX; j++) {
                
    jetmass->Fill( out_jets.at(j).m() );
    jetpt->Fill( out_jets.at(j).pt() );
//     jeteta[j] = out_jets.at(j).eta();
//     jetphi[j] = out_jets.at(j).phi();
//     jete[j]   = out_jets.at(j).energy();
    jetarea->Fill( out_jets.at(j).area() );


    float m_tau2tau1 = -1.0; 
    float m_tau3tau2 = -1.0; 
    Nsubjettiness( out_jets.at(j), mJetRadius, m_tau2tau1, m_tau3tau2);
    tau2tau1->Fill( m_tau2tau1);
    tau3tau2->Fill( m_tau3tau2);
    jetmass_jetpt-> Fill( out_jets.at(j).pt(), out_jets.at(j).m(), 1);
    tau2tau1_jetpt-> Fill( out_jets.at(j).pt(), m_tau2tau1, 1);

    // pruning, trimming, filtering  -------------
    int transctr = 0;
    for ( std::vector<fastjet::Transformer const *>::const_iterator 
	    itransf = transformers.begin(), itransfEnd = transformers.end(); 
	  itransf != itransfEnd; ++itransf ) {  
            
      fastjet::PseudoJet transformedJet = out_jets.at(j);
      transformedJet = (**itransf)(transformedJet);           
      if (transctr == 0){ // trimmed
	jetmass_tr->Fill( transformedJet.m() );
	jetpt_tr->Fill( transformedJet.pt() );
	jetarea_tr->Fill( transformedJet.area() );
	jetmass_tr_jetpt_tr-> Fill( transformedJet.pt(), transformedJet.m(), 1);
      }
      else if (transctr == 1){ // filtered
	jetmass_ft->Fill( transformedJet.m() );
	jetpt_ft->Fill( transformedJet.pt() );
	jetarea_ft->Fill( transformedJet.area() );  
	jetmass_ft_jetpt_ft-> Fill( transformedJet.pt(), transformedJet.m(), 1);    
      }
      else if (transctr == 2){ // pruned
	jetmass_pr->Fill( transformedJet.m() );
	jetpt_pr->Fill( transformedJet.pt() );
	jetarea_pr->Fill( transformedJet.area() );                      
	massdrop_pr->Fill( massDrop(transformedJet)) ;
	jetmass_pr_jetpt_pr-> Fill( transformedJet.pt(), transformedJet.m(), 1);
      }                
      else{ std::cout << "error in number of transformers" << std::endl;}                    
      transctr++;
    } //  close itransf for loop   
        
  }//close jet loop

}






float MB_Py8::massDrop(fastjet::PseudoJet& transformedJet) {

  float massdrop = -1.0;
  
  //decompose into requested number of subjets:
  if (transformedJet.constituents().size() > 1){
    int nsubjetstokeep = 2;
    std::vector<fastjet::PseudoJet> subjets = 
      transformedJet.associated_cluster_sequence()->exclusive_subjets(transformedJet, nsubjetstokeep);    
                    
    TLorentzVector sj1( subjets.at(0).px(),subjets.at(0).py(),subjets.at(0).pz(),subjets.at(0).e());
    TLorentzVector sj2( subjets.at(1).px(),subjets.at(1).py(),subjets.at(1).pz(),subjets.at(1).e());   
    TLorentzVector fullj = sj1 + sj2;
                    
    if (subjets.at(0).m() >= subjets.at(1).m())
      massdrop = subjets.at(0).m()/transformedJet.m();                        
    else massdrop = subjets.at(1).m()/transformedJet.m();    
  }
  return massdrop;
}




void MB_Py8::Nsubjettiness(fastjet::PseudoJet& jet, double mJetRadius, float& tau2tau1, float& tau3tau2) {

  double beta = 1.0;// power for angular dependence, e.g. beta = 1 --> linear k-means, beta = 2 --> quadratic/classic k-means
  double R0 = mJetRadius; // Characteristic jet radius for normalization	      
  double Rcut = mJetRadius; // maximum R particles can be from axis to be included in jet	

  fastjet::contrib::Nsubjettiness nSub1KT(1, Njettiness::onepass_kt_axes, beta, R0, Rcut);
  float tau1 = nSub1KT(jet);
  fastjet::contrib::Nsubjettiness nSub2KT(2, Njettiness::onepass_kt_axes, beta, R0, Rcut);
  float tau2 = nSub2KT(jet);
  fastjet::contrib::Nsubjettiness nSub3KT(3, Njettiness::onepass_kt_axes, beta, R0, Rcut);
  float tau3 = nSub3KT(jet);
  fastjet::contrib::Nsubjettiness nSub4KT(4, Njettiness::onepass_kt_axes, beta, R0, Rcut);
  float tau4 = nSub4KT(jet);
  tau2tau1 = tau2/tau1;
  tau3tau2 = tau3/tau2;
}










bool MB_Py8::ticker(Long64_t nVtx,Long64_t nfreq,Long64_t ntot)
{ 
  _singleEvts += nVtx;
  ++_procEvts;
  if ( _procEvts % nfreq == 0 || _singleEvts % 1000 == 0 )
    { printf("[MB_Py8::ticker(%3i,%i,%i)] - INFO - %9i out of %9i events read (%7.3f%%) (scanned %12i single minbias events)\n",
	     nVtx,nfreq,ntot,_procEvts,ntot,(double)_procEvts/(double)ntot*100,
	     _singleEvts);
      return true;
    }
  return false;
}

//   In a ROOT session, you can do:
//      Root > .L MB_Py8.C
//      Root > MB_Py8 t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
