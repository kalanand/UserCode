#define MB_Py8_cxx
#include "MB_Py8.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>

//#include "JetAnalysis.h"



EtaRange::EtaRange(double etaMin,double etaMax)
  : _etaMin(etaMin), _etaMax(etaMax)
{}

EtaRange::~EtaRange()
{ }

bool EtaRange::accept(const fastjet::PseudoJet& pJet)
{ return pJet.pseudorapidity() > _etaMin && pJet.pseudorapidity() < _etaMax; }

void EtaRange::reset()
{ }

bool MB_Py8::GetEvent(Int_t mu,Long64_t& ientry,Event& rEvent,Int_t vtxOffset)
{
  // reset event
  rEvent.reset();
  // fill event
  return _dataHandler->fillEvent(*this,rEvent,ientry,mu,vtxOffset,Vertex::PILEUP);
}


void MB_Py8::Loop(int mu,Long64_t nevts,const std::string& outName, bool PUsub)
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

   Long64_t nentries = fChain->GetEntries();
   Long64_t ientries = 0;
   

   Event*               pEvt = new Event(new EtaRange(-6.,6.));
   DataHandler<MB_Py8>* pHdl = new DataHandler<MB_Py8>();

   if ( nevts > 0 )
     {
       Long64_t ievts(0);
       while ( ievts < ntot && pHdl->fillEvent(*this,*pEvt,ientries,mu) ) 
	 { ++ievts; ticker((Long64_t)pEvt->nVertices(),1000,ntot); 
	   analyze(*pEvt, PUsub); pEvt->reset(); }
     }
   else
     { 
       while ( pHdl->fillEvent(*this,*pEvt,ientries,mu) ) 
	 { ticker((Long64_t)pEvt->nVertices(),1000,ntot);
	   analyze(*pEvt, PUsub); pEvt->reset(); }
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

  // createHistogramsAllAlgos(m_HistNames1D, m_HistNames2D);

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


  for(std::map<TString, TH1*>::iterator 
	iter = m_HistNames1D.begin(); 
      iter != m_HistNames1D.end(); ++iter) {
    writeHist(iter->second);
    m_HistNames1D.erase(iter);
  }

  for(std::map<TString, TProfile*>::iterator 
	iter2 = m_HistNames2D.begin(); 
      iter2 != m_HistNames2D.end(); ++iter2) {
    writeHist(iter2->second);
    m_HistNames2D.erase(iter2);
  }

  outFile->Close();

  
  return true;
}

bool MB_Py8::analyze(Event& pEvt, bool PUsub)
{
  // retrieve all particles
  std::vector<fastjet::PseudoJet> pPart = pEvt.pseudoJets(-1);

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

  /*
  analyzeJetSubstructure(pEvt, pPart, fastjet::antikt_algorithm, 0.5, PUsub, m_HistNames1D, m_HistNames2D);
  analyzeJetSubstructure(pEvt, pPart, fastjet::antikt_algorithm, 0.6, PUsub, m_HistNames1D, m_HistNames2D);
  analyzeJetSubstructure(pEvt, pPart, fastjet::antikt_algorithm, 0.7, PUsub, m_HistNames1D, m_HistNames2D);
  analyzeJetSubstructure(pEvt, pPart, fastjet::antikt_algorithm, 0.8, PUsub, m_HistNames1D, m_HistNames2D);
  analyzeJetSubstructure(pEvt, pPart, fastjet::antikt_algorithm, 1.0, PUsub, m_HistNames1D, m_HistNames2D);

  analyzeJetSubstructure(pEvt, pPart, fastjet::cambridge_algorithm, 0.8, PUsub, m_HistNames1D, m_HistNames2D);
  analyzeJetSubstructure(pEvt, pPart, fastjet::cambridge_algorithm, 1.0, PUsub, m_HistNames1D, m_HistNames2D);

  analyzeJetSubstructure(pEvt, pPart, fastjet::kt_algorithm, 0.5, PUsub, m_HistNames1D, m_HistNames2D);
  analyzeJetSubstructure(pEvt, pPart, fastjet::kt_algorithm, 0.6, PUsub, m_HistNames1D, m_HistNames2D);
  analyzeJetSubstructure(pEvt, pPart, fastjet::kt_algorithm, 0.7, PUsub, m_HistNames1D, m_HistNames2D);
  analyzeJetSubstructure(pEvt, pPart, fastjet::kt_algorithm, 0.8, PUsub, m_HistNames1D, m_HistNames2D);
  analyzeJetSubstructure(pEvt, pPart, fastjet::kt_algorithm, 1.0, PUsub, m_HistNames1D, m_HistNames2D);

  */
  return true;
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
