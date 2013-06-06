// -*- c++ -*-
//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri May 31 11:39:45 2013 by ROOT version 5.34/05
// from TTree Zprime_Py8/Zprime to ttbar ROOT tree
// found on file: signal_Zprime.root
//////////////////////////////////////////////////////////

#ifndef Zprime_Py8_h
#define Zprime_Py8_h

#include "Selectors.h"
#include "Event.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <map>
#include <string>
#include <cstdio>


class Zprime_Py8 {
public :
  TChain         *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
  Int_t           Nentry;
  Int_t           Npartons;
  Int_t           Nparticles;
  Int_t           ID[100000];   //[Nentry]
  Int_t           Stat[100000];   //[Nentry]
  Double_t        Charge[100000];   //[Nentry]
  Double_t        Px[100000];   //[Nentry]
  Double_t        Py[100000];   //[Nentry]
  Double_t        Pz[100000];   //[Nentry]
  Double_t        P0[100000];   //[Nentry]
  Double_t        Pm[100000];   //[Nentry]
  Double_t        Pt[100000];   //[Nentry]
  Double_t        Rap[100000];   //[Nentry]
  Double_t        Phi[100000];   //[Nentry]
  Double_t        Eta[100000];   //[Nentry]

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

  Zprime_Py8(TChain *chain=0);
  virtual ~Zprime_Py8();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TChain *chain);
  virtual void     Loop(Long64_t nevts=-1,const std::string& outName="", bool PUsub=false);
  template<class T>
  void             Loop(T& pileupServer,Int_t mu,Long64_t nevts=-1,
			const std::string& outName="",
			const std::string& pupName="", bool PUsub=false);
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);

public:

  bool book();
  bool write(const std::string& outName="");
  bool analyze(Event& pEvt, bool PUsub);

  //  bool ticker(Long64_t nVtx,Long64_t nfreq,Long64_t nevts);
  template<class H>
  void writeHist(H* pHist);


private:

 
  std::map<TString, TH1*> m_HistNames1D;
  std::map<TString, TProfile*> m_HistNames2D;




  int _evtCtr;
  int _evtFreq;

  TH1D* h_nsigparts;
  TH1D* h_etasigparts;
  TH1D* h_ptsigparts;

  TH1D* h_nallparts;
  TH1D* h_etaallparts;
  TH1D* h_ptallparts;

  TH1D* h_npupparts;
  TH1D* h_etapupparts;
  TH1D* h_ptpupparts;

  
};

#include "Zprime_Py8.icc"

#endif
