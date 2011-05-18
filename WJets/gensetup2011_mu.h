#include <iostream>
#include <iomanip>
#include "stdlib.h"
#include "math.h"
#include <fstream>

#include "TROOT.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include "TTree.h"
#include "TF1.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TString.h"
#include "THStack.h"
#include "TLatex.h"

static const int TOTAL_COMPONENTS  = 7;

static const double xsecs[][4] ={ 
  //x pb-1, filter eff,     events,         k-factor 
   {1.0,         1.0,        2.1e+08,   1.0E6},  //  0 Certified Data For data 160329-163759
  //{1.0,         1.0,        1.783779e+08,   1.0E6},  //  0 Certified Data For data 160329-163759
  //{1.0,         1.0,        2.306580e+08,   1.0E6},  //  0 DCSOnly For data 160329-163759
  {24640,       1.0,        14510974,       1.271},  //  1 WJetsToLNu_TuneZ2_7TeV-madgraph-tauola
  {65.83,       1.0,        4759744,        1.400},  //  2 TTToLNu2Q2B_7TeV-powheg-pythia6
  {10.4*4.0,    1.0,        2108416,        1.547},  //  3 WZtoAnything_TuneZ2_7TeV-pythia6-tauola
  {27.79*4.0,   1.0,        2011760,        1.547},  //  4 WWtoAnything_TuneZ2_7TeV-pythia6-tauola
  {44.000,     1.0,        109992,         1.000},  //  5 GluGluToHToWWToLNuQQ_M-180_7TeV-powheg-pythia6
  {20.000,     1.0,        106432,         1.000}   //  6 GluGluToHToWWToLNuQQ_M-300_7TeV-powheg-pythia6
};

static const char *names[]= { 
  "Data",
  "WJets",
  "TT",
  "WZ",
  "WW",
  "mH=180 X~2",
  "mH=300 X~2"
};

static const char *rootnames[]= { 
  "Data",
  "WJets",
  "TT",
  "WZ",
  "WW",
  "mH=180 X~2",
  "mH=300 X~2"
};

static const char *filenames[] = {
  "RD_WmunuJets_Data2011_178invpb_May6JSON.root",
  //"RD_WmunuJets_Data2011_231invpb_May9DCS.root",
  "RD_WmunuJets_Spring11MC_WJets.root",
  "RD_WmunuJets_Spring11MC_TTToLNu2Q2B.root",
  "RD_WmunuJets_Spring11MC_WZtoAnything.root",
  "RD_WmunuJets_Spring11MC_WWtoAnything.root",
  "RD_WmunuJets_Spring11MC_WWToLNuQQ_M-180.root",
  "RD_WmunuJets_Spring11MC_WWToLNuQQ_M-300.root"
};
