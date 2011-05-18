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
  {1.0,         1.0,        3.38503e+07 ,   1.0E6},  //  0 For data 160329-163759
  {24640,       1.0,        15161497,       1.271},  //  1 WJetsToLNu_TuneZ2_7TeV-madgraph-tauola
  {94.3,        1.0,        1099550,        1.670},  //  2 TT_TuneZ2_7TeV-pythia6-tauola
  {10.4*4.0,    1.0,        1894752,        1.547},  //  3 WZtoAnything_TuneZ2_7TeV-pythia6-tauola
  {27.79*4.0,   1.0,        1715680,        1.547},  //  4 WWtoAnything_TuneZ2_7TeV-pythia6-tauola
  {220.000,     1.0,        109992,         1.000},  //  5 GluGluToHToWWToLNuQQ_M-180_7TeV-powheg-pythia6
  {100.000,     1.0,        106432,         1.000}   //  6 GluGluToHToWWToLNuQQ_M-300_7TeV-powheg-pythia6
};

static const char *names[]= { 
  "Data",
  "WJets",
  "TT",
  "WZ",
  "WW",
  "HM180 X~10",
  "HM300 X~10"
};

static const char *rootnames[]= { 
  "Data",
  "WJets",
  "TT",
  "WZ",
  "WW",
  "HM180 X~10",
  "HM300 X~10"
};

static const char *filenames[] = {
  "/uscms_data/d2/yangf/ana/WuvWjj/Plots/Kana/RD_WmunuJets_Data2010.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Plots/Kana/RD_WmunuJets_Winter10MC_WJets.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Plots/Kana/RD_WmunuJets_Winter10MC_TTPythia.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Plots/Kana/RD_WmunuJets_Winter10MC_WZtoAnything.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Plots/Kana/RD_WmunuJets_Winter10MC_WWtoAnything.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Plots/Kana/RD_WmunuJets_Spring11MC_WWToLNuQQ_M-180.root",
  "/uscms_data/d2/yangf/ana/WuvWjj/Plots/Kana/RD_WmunuJets_Spring11MC_WWToLNuQQ_M-300.root"
};
