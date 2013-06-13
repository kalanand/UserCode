#include "Zprime_Py8.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <iostream>

#include "JetAnalysis.h"

Zprime_Py8::Zprime_Py8(TChain *chain) 
  : fChain(0)
  , _evtCtr(0)
  , _evtFreq(100)
  , h_nsigparts(0)
  , h_etasigparts(0)
  , h_ptsigparts(0)
  , h_nallparts(0)
  , h_etaallparts(0)
  , h_ptallparts(0)
  , h_npupparts(0)
  , h_etapupparts(0)
  , h_ptpupparts(0)
{
   if (chain == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("signal_Zprime.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("signal_Zprime.root");
      }
      f->GetObject("Zprime_Py8",chain);

   }
   Init(chain);
}

Zprime_Py8::~Zprime_Py8()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Zprime_Py8::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Zprime_Py8::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Zprime_Py8::Init(TChain *chain)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!chain) return;
   fChain = chain;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Nentry", &Nentry, &b_Nentry);
   fChain->SetBranchAddress("Npartons", &Npartons, &b_Npartons);
   fChain->SetBranchAddress("Nparticles", &Nparticles, &b_Nparticles);
   fChain->SetBranchAddress("ID", ID, &b_ID);
   fChain->SetBranchAddress("Stat", Stat, &b_Stat);
   fChain->SetBranchAddress("Charge", Charge, &b_Charge);
   fChain->SetBranchAddress("Px", Px, &b_Px);
   fChain->SetBranchAddress("Py", Py, &b_Py);
   fChain->SetBranchAddress("Pz", Pz, &b_Pz);
   fChain->SetBranchAddress("P0", P0, &b_P0);
   fChain->SetBranchAddress("Pm", Pm, &b_Pm);
   fChain->SetBranchAddress("Pt", Pt, &b_Pt);
   fChain->SetBranchAddress("Rap", Rap, &b_Rap);
   fChain->SetBranchAddress("Phi", Phi, &b_Phi);
   fChain->SetBranchAddress("Eta", Eta, &b_Eta);
   Notify();
}

Bool_t Zprime_Py8::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Zprime_Py8::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Zprime_Py8::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

bool Zprime_Py8::book()
{
  h_nsigparts = new TH1D("NSignalParts","Number of particles signal event",
			 200,0.,10000.);
  h_etasigparts = new TH1D("EtaSignalParts","Eta (signal particles)",
			   130,-6.5,6.5);
  h_ptsigparts = new TH1D("PtSignalParts","Pt (signaal particles)",
			  100,0.,100.);
  //
  h_nallparts = new TH1D("NAllParts","Number of particles full event",
			 200,0.,10000.);
  h_etaallparts = new TH1D("EtaAllParts","Eta (full particles)",
			   130,-6.5,6.5);
  h_ptallparts = new TH1D("PtAllParts","Pt (full particles)",
			  100,0.,100.);
  //
  h_npupparts = new TH1D("NPupParts","Number of particles pileup event",
			 200,0.,10000.);
  h_etapupparts = new TH1D("EtaPupParts","Eta (pileup particles)",
			   130,-6.5,6.5);
  h_ptpupparts = new TH1D("PtPupParts","Pt (pileup particles)",
			  100,0.,100.);


  createSignalPlusPUHistogramsAllAlgos(m_HistNames1D, m_HistNames2D);
  createSignalHistogramsAllAlgos(m_HistNames1D, m_HistNames2D);
  createPUHistogramsAllAlgos(m_HistNames1D, m_HistNames2D);

  return true;
}



bool Zprime_Py8::analyze(Event& pEvt, bool PUsub)
{
  _evtCtr++;

  std::vector<fastjet::PseudoJet> allParts = pEvt.pseudoJets(-1);
  std::vector<fastjet::PseudoJet> sigParts = pEvt.pseudoJets(Vertex::SIGNAL);
  std::vector<fastjet::PseudoJet> pupParts = pEvt.pseudoJets(Vertex::PILEUP);

  std::vector<fastjet::PseudoJet>::iterator fSig(sigParts.begin());
  std::vector<fastjet::PseudoJet>::iterator lSig(sigParts.end());

  std::vector<fastjet::PseudoJet>::iterator fAll(allParts.begin());
  std::vector<fastjet::PseudoJet>::iterator lAll(allParts.end());

  std::vector<fastjet::PseudoJet>::iterator fPup(pupParts.begin());
  std::vector<fastjet::PseudoJet>::iterator lPup(pupParts.end());

  double sigSize = sigParts.size();
  double allSize = allParts.size();
  double pupSize = pupParts.size();

  h_nsigparts->Fill(sigSize);
  h_nallparts->Fill(allSize);
  h_npupparts->Fill(pupSize);

  for ( ; fSig != lSig; ++fSig ) 
    {
      h_etasigparts->Fill(fSig->pseudorapidity());
      h_ptsigparts->Fill(fSig->perp());
    }

  for ( ; fAll != lAll; ++fAll ) 
    {
      h_etaallparts->Fill(fAll->pseudorapidity());
      h_ptallparts->Fill(fAll->perp());
    }

  for ( ; fPup != lPup; ++fPup ) 
    {
      h_etapupparts->Fill(fPup->pseudorapidity());
      h_ptpupparts->Fill(fPup->perp());
    }

  if ( ( _evtCtr % _evtFreq ) == 0 )
    { printf("[Zprime_py8::analyze(%p)] events analyzed %i\n",
	     (void*)&pEvt,_evtCtr); }


  analyzeJetSubstructure(pEvt, allParts, fastjet::antikt_algorithm, 0.5, PUsub, m_HistNames1D, m_HistNames2D, "all_");
  analyzeJetSubstructure(pEvt, allParts, fastjet::antikt_algorithm, 0.6, PUsub, m_HistNames1D, m_HistNames2D, "all_");
  analyzeJetSubstructure(pEvt, allParts, fastjet::antikt_algorithm, 0.7, PUsub, m_HistNames1D, m_HistNames2D, "all_");
  analyzeJetSubstructure(pEvt, allParts, fastjet::antikt_algorithm, 0.8, PUsub, m_HistNames1D, m_HistNames2D, "all_");
  analyzeJetSubstructure(pEvt, allParts, fastjet::antikt_algorithm, 1.0, PUsub, m_HistNames1D, m_HistNames2D, "all_");


  /*
  analyzeJetSubstructure(pEvt, allParts, fastjet::cambridge_algorithm, 0.8, PUsub, m_HistNames1D, m_HistNames2D, "all_");
  analyzeJetSubstructure(pEvt, allParts, fastjet::cambridge_algorithm, 1.0, PUsub, m_HistNames1D, m_HistNames2D, "all_");

  analyzeJetSubstructure(pEvt, allParts, fastjet::kt_algorithm, 0.5, PUsub, m_HistNames1D, m_HistNames2D, "all_");
  analyzeJetSubstructure(pEvt, allParts, fastjet::kt_algorithm, 0.6, PUsub, m_HistNames1D, m_HistNames2D, "all_");
  analyzeJetSubstructure(pEvt, allParts, fastjet::kt_algorithm, 0.7, PUsub, m_HistNames1D, m_HistNames2D, "all_");
  analyzeJetSubstructure(pEvt, allParts, fastjet::kt_algorithm, 0.8, PUsub, m_HistNames1D, m_HistNames2D, "all_");
  analyzeJetSubstructure(pEvt, allParts, fastjet::kt_algorithm, 1.0, PUsub, m_HistNames1D, m_HistNames2D, "all_");
   */

  ////
  analyzeJetSubstructure(pEvt, sigParts, fastjet::antikt_algorithm, 0.5, PUsub, m_HistNames1D, m_HistNames2D, "signal_");
  analyzeJetSubstructure(pEvt, sigParts, fastjet::antikt_algorithm, 0.6, PUsub, m_HistNames1D, m_HistNames2D, "signal_");
  analyzeJetSubstructure(pEvt, sigParts, fastjet::antikt_algorithm, 0.7, PUsub, m_HistNames1D, m_HistNames2D, "signal_");
  analyzeJetSubstructure(pEvt, sigParts, fastjet::antikt_algorithm, 0.8, PUsub, m_HistNames1D, m_HistNames2D, "signal_");
  analyzeJetSubstructure(pEvt, sigParts, fastjet::antikt_algorithm, 1.0, PUsub, m_HistNames1D, m_HistNames2D, "signal_");

  /*
  analyzeJetSubstructure(pEvt, sigParts, fastjet::cambridge_algorithm, 0.8, PUsub, m_HistNames1D, m_HistNames2D, "signal_");
  analyzeJetSubstructure(pEvt, sigParts, fastjet::cambridge_algorithm, 1.0, PUsub, m_HistNames1D, m_HistNames2D, "signal_");

  analyzeJetSubstructure(pEvt, sigParts, fastjet::kt_algorithm, 0.5, PUsub, m_HistNames1D, m_HistNames2D, "signal_");
  analyzeJetSubstructure(pEvt, sigParts, fastjet::kt_algorithm, 0.6, PUsub, m_HistNames1D, m_HistNames2D, "signal_");
  analyzeJetSubstructure(pEvt, sigParts, fastjet::kt_algorithm, 0.7, PUsub, m_HistNames1D, m_HistNames2D, "signal_");
  analyzeJetSubstructure(pEvt, sigParts, fastjet::kt_algorithm, 0.8, PUsub, m_HistNames1D, m_HistNames2D, "signal_");
  analyzeJetSubstructure(pEvt, sigParts, fastjet::kt_algorithm, 1.0, PUsub, m_HistNames1D, m_HistNames2D, "signal_");

  */



  /*
  ////
  analyzeJetSubstructure(pEvt, pupParts, fastjet::antikt_algorithm, 0.5, PUsub, m_HistNames1D, m_HistNames2D, "pu_");
  analyzeJetSubstructure(pEvt, pupParts, fastjet::antikt_algorithm, 0.6, PUsub, m_HistNames1D, m_HistNames2D, "pu_");
  analyzeJetSubstructure(pEvt, pupParts, fastjet::antikt_algorithm, 0.7, PUsub, m_HistNames1D, m_HistNames2D, "pu_");
  analyzeJetSubstructure(pEvt, pupParts, fastjet::antikt_algorithm, 0.8, PUsub, m_HistNames1D, m_HistNames2D, "pu_");
  analyzeJetSubstructure(pEvt, pupParts, fastjet::antikt_algorithm, 1.0, PUsub, m_HistNames1D, m_HistNames2D, "pu_");


  analyzeJetSubstructure(pEvt, pupParts, fastjet::cambridge_algorithm, 0.8, PUsub, m_HistNames1D, m_HistNames2D, "pu_");
  analyzeJetSubstructure(pEvt, pupParts, fastjet::cambridge_algorithm, 1.0, PUsub, m_HistNames1D, m_HistNames2D, "pu_");

  analyzeJetSubstructure(pEvt, pupParts, fastjet::kt_algorithm, 0.5, PUsub, m_HistNames1D, m_HistNames2D, "pu_");
  analyzeJetSubstructure(pEvt, pupParts, fastjet::kt_algorithm, 0.6, PUsub, m_HistNames1D, m_HistNames2D, "pu_");
  analyzeJetSubstructure(pEvt, pupParts, fastjet::kt_algorithm, 0.7, PUsub, m_HistNames1D, m_HistNames2D, "pu_");
  analyzeJetSubstructure(pEvt, pupParts, fastjet::kt_algorithm, 0.8, PUsub, m_HistNames1D, m_HistNames2D, "pu_");
  analyzeJetSubstructure(pEvt, pupParts, fastjet::kt_algorithm, 1.0, PUsub, m_HistNames1D, m_HistNames2D, "pu_");

  */

  return true;
}






bool Zprime_Py8::write(const std::string& outName)
{
  TFile* file = new TFile(outName.c_str(),"RECREATE");
  
  writeHist(h_nsigparts);
  writeHist(h_etasigparts);
  writeHist(h_ptsigparts);

  writeHist(h_nallparts);
  writeHist(h_etaallparts);
  writeHist(h_ptallparts);

  writeHist(h_npupparts);
  writeHist(h_etapupparts);
  writeHist(h_ptpupparts);

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


  file->Close();
  return true;

}



void Zprime_Py8::Loop(Long64_t nevts,const std::string& outName, bool PUsub)
{
  // check if chain exists
   if (fChain == 0) return;

   // number of events to be analyzed (nevts <= 0 -> all)
   Long64_t nentries = nevts > 0 ? nevts : fChain->GetEntriesFast();

   // book histograms
   printf("[Zprime_Py8::Loop(%i,\042%s\042)] book hists...",
	  (int)nevts,outName.c_str());
   book();
   printf("DONE!\n");

   // signal event store and data handler
   Event* pEvt                   = new Event(new Selectors::EtaRange(-6.,6.));
   DataHandler<Zprime_Py8>* pHdl = new DataHandler<Zprime_Py8>();
   printf("[Zprime_Py8::Loop(%i,\042%s\042)] Event at %p, DataHandler at %p\n",
	  (int)nevts,outName.c_str(),(void*)pEvt,(void*)pHdl);

   // event loop
   Long64_t iEvts = 0;
   while ( iEvts < nentries && pHdl->fillEvent(*this,*pEvt,iEvts,-1,0,
					       Vertex::SIGNAL) )
     {
       printf("[Zprime_Py8::Loop(%i,\042%s\042)] analyze event %i\n",
	      (int)nevts,outName.c_str(),(int)iEvts);
       analyze(*pEvt, PUsub); 
       pEvt->reset(); 
     }

   // write histograms
   if ( !outName.empty() && outName != "" ) { write(outName); }
}




