// -*- c++ -*-
//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jan 14 12:34:36 2013 by ROOT version 5.34/04
// from TTree MB_Py8/MinimumBias root tree
// found on file: /SharedDisk/BOOST2012/data.BOOST2012/softQCD_13TeV/float/10_19/pythia8_mb10_00000.root
//////////////////////////////////////////////////////////

#ifndef MB_Py8_h
#define MB_Py8_h

#include "Event.h"
//#include "IDataFiller.h"
//#include "IDataAnalyzer.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <map>
#include <string>


class MB_Py8 {
public :
   TChain         *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           Nentry;
   Int_t           Npartons;
   Int_t           Nparticles;
   Int_t           ID[1000000];   //[Nentry]
   Int_t           Stat[1000000];   //[Nentry]
   Float_t         Charge[1000000];   //[Nentry]
   Float_t         Px[1000000];   //[Nentry]
   Float_t         Py[1000000];   //[Nentry]
   Float_t         Pz[1000000];   //[Nentry]
   Float_t         P0[1000000];   //[Nentry]
   Float_t         Pm[1000000];   //[Nentry]
   Float_t         Pt[1000000];   //[Nentry]
   Float_t         Rap[1000000];   //[Nentry]
   Float_t         Phi[1000000];   //[Nentry]
   Float_t         Eta[1000000];   //[Nentry]

   // List of branches
   TBranch        *b_Nentry;   //!
   TBranch        *b_Npartons;   //!
   TBranch        *b_Nparticles;   //!
   TBranch        *b_ID;   //!
   TBranch        *b_Stat;   //!
   TBranch        *b_Charge;   //!
   TBranch        *b_Px;   //!
   TBranch        *b_Py;   //!
   TBranch        *b_Pz;   //!
   TBranch        *b_P0;   //!
   TBranch        *b_Pm;   //!
   TBranch        *b_Pt;   //!
   TBranch        *b_Rap;   //!
   TBranch        *b_Phi;   //!
  TBranch        *b_Eta;   //!

  MB_Py8(int mu=0,TChain *tree=0);
  virtual ~MB_Py8();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TChain *tree);
  template <typename T>
  void SetBranchAddress( const char* name, const T* x, TBranch** ptr);
  virtual void     Loop(int mu=1,Long64_t nevts=-1,
			const std::string& outName="",bool PUsub=false);
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);

  virtual bool     GetEvent(Int_t mu,Long64_t& ientry,
			    Event& rEvent,Int_t vtxOffset=0);
protected:

  Long64_t _procEvts;
  Long64_t _singleEvts;

  DataHandler<MB_Py8>* _dataHandler;

  std::map<TString, TH1*> m_HistNames1D;
  std::map<TString, TProfile*> m_HistNames2D;

  //////////////////////
  // Event quantities //
  //////////////////////

  TH1D* h_mu;

  //////////////////////////////////
  // Particle flow and kinematics //
  //////////////////////////////////

  TH1D* h_rap;
  TH1D* h_eta;
  TH1D* h_perp;

  TH1D* h_rap_charged;
  TH1D* h_eta_charged;
  TH1D* h_perp_charged;

public:

  bool book();
  bool write(const std::string& outName="");
  bool analyze(Event& pEvt, bool PUsub);
  bool ticker(Long64_t nVtx,Long64_t nfreq,Long64_t nevts);

  template<class H>
  void writeHist(H* pHist)
  { 
    if ( pHist->GetEntries() != 0 )
      {
	printf("[MB_py8::writeHist(%p)] - INFO - write histogram <%s/%s>\n",
	       (void*)pHist,pHist->GetName(),pHist->GetTitle());
	pHist->Write();
      }
    else
      {
	printf("[MB_py8::writeHist(%p)] - WARN - histogram <%s/%s> empty.\n",
	       (void*)pHist,pHist->GetName(),pHist->GetTitle());
      }
  }

};


class EtaRange : virtual public IFinalStateSelector
{
public:
  EtaRange(double etaMin=-6.,double etaMax=6.);
  virtual ~EtaRange();

  virtual bool accept(const fastjet::PseudoJet& pJet);
  virtual void reset();
  
private:

  double _etaMin;
  double _etaMax;
};

#endif

#ifdef MB_Py8_cxx
MB_Py8::MB_Py8(int mu,TChain *tree) 
  : fChain(0)
  , _procEvts(0)
  , _singleEvts(0)
  , _dataHandler(new DataHandler<MB_Py8>())
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/SharedDisk/BOOST2012/data.BOOST2012/softQCD_13TeV/float/10_19/pythia8_mb10_00000.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/SharedDisk/BOOST2012/data.BOOST2012/softQCD_13TeV/float/10_19/pythia8_mb10_00000.root");
      }
      f->GetObject("MB_Py8",tree);

   }
   Init(tree);
}

MB_Py8::~MB_Py8()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MB_Py8::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MB_Py8::LoadTree(Long64_t entry)
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

void MB_Py8::Init(TChain *tree)
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






Bool_t MB_Py8::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MB_Py8::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MB_Py8::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MB_Py8_cxx
