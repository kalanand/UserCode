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
  {1.0,         1.0,        225.0,       1.0},  //  0 For data 160329-163759
  {24640,       1.,        14510974,          1.21},  //  1 WJetsToLNu_TuneZ2_7TeV-madgraph-tauola: 1.271
  {65.83,       1.,        4759744,           1.22},  //  2 TTToLNu2Q2B_7TeV-powheg-pythia6: 1.4
  {10.4*4.0,    1.0,        2108416,          1.547},  //  3 WZtoAnything_TuneZ2_7TeV-pythia6-tauola
  {27.79*4.0,   1.0,        2011760,          1.547},  //  4 WWtoAnything_TuneZ2_7TeV-pythia6-tauola
  {6.0*2.0,     1.0,        109992,            1.000},  //  5 GluGluToHToWWToLNuQQ_M-200_7TeV-powheg-pythia6
  {3.5*2.0,     1.0,        106432,            1.000}   //  6 GluGluToHToWWToLNuQQ_M-250_7TeV-powheg-pythia6
};

static const char *names[]= { 
  "Data",
  "WJets",
  "TT",
  "WZ",
  "WW",
  "mH=200 X~2",
  "mH=250 X~2"
};

static const char *rootnames[]= { 
  "Data",
  "WJets",
  "TT",
  "WZ",
  "WW",
  "mH=200 X~2",
  "mH=250 X~2"
};

static const char *filenames[] = {
  "Histo_Data_WenuJets_225invpb.root",
  "Histo_MC_WJets_enujj_Spring11.root",
  "Histo_MC_TT_enujj_Spring2011.root",
  "Histo_MC_WZ_enujj_Spring11.root",
  "Histo_MC_WW_enujj_Spring11.root",
  "Histo_MC_HWW_200GeV_enujj_Spring2011.root",
  "Histo_MC_HWW_250GeV_enujj_Spring2011.root"
};
