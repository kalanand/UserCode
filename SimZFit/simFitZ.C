/*******************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: Presently in the user code
 *
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *   A standalone macro to perform simultaneos fit of 
 *   dilepton mass peak from Z decay. 
 *   Works with  binned data. 
 *
 * Implementation details:
 *   Uses RooFit classes.
 *  
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 ********************************************************************/
//#include "TwoCategorySimZFitter.C"
#include "ThreeCategorySimZFitter.C"


void simFitZ(double lumi=1.34, int removeEE=0)
{
//    TCut runrange("tag_gsfEle_ecalDrivenSeed && probe_gsfEle_ecalDrivenSeed && (abs(tag_sc_eta)<1.5 && abs(probe_sc_eta)<1.5)");

//    TCut runrange2("tag_gsfEle_ecalDrivenSeed && (abs(tag_sc_eta)<1.5 && abs(probe_sc_eta)<1.5)");


   TCut runrange("tag_gsfEle_ecalDrivenSeed && probe_gsfEle_ecalDrivenSeed");
   TCut runrange2("tag_gsfEle_ecalDrivenSeed");

   //TCut runrange("");

  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadBottomMargin(0.16);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetOptStat(1001100);



////////////////////////// nominal fit

  TFile* f = new TFile("allTPtrees_1340nb.root");
  TTree* tpTree = (TTree*) f->Get("IdToHLT/fitter_tree");
  TH1F* ZmassPass = createHistogram("ZmassPass", 30);
  TH1F* ZmassFail = createHistogram("ZmassFail", 20);
  TH1F* ZmassFailBB = createHistogram("ZmassFailBB", 10);
  TH1F* ZmassFailEnd = createHistogram("ZmassFailEnd", 10);

  tpTree->Draw("pair_mass>>ZmassPass",runrange && "probe_passing","goff");
  ZmassPass->Scale(0.5);
  tpTree->Draw("pair_mass>>+ZmassPass",runrange && "!probe_passing","goff");

//   tpTree = (TTree*) f->Get("SCToGsf/fitter_tree");
//   tpTree->Draw("pair_mass>>ZmassFail",runrange2 && "!probe_passingALL","goff");

//   tdrStyle->SetOptStat(1);
//   TwoCategorySimZFitter(*ZmassPass, *ZmassFail, lumi, removeEE);



  tpTree = (TTree*) f->Get("PhotonToGsf/fitter_tree");
  TCut cutFailBB("");
//   tpTree->Draw("pair_mass>>ZmassFailBB",runrange2 && "abs(tag_sc_eta)<1.5 && abs(probe_sc_eta)<1.5 && !probe_passingALL","goff");

//   tpTree->Draw("pair_mass>>ZmassFailEnd",runrange2 && "!(abs(tag_sc_eta)<1.5 && abs(probe_sc_eta)<1.5) && !probe_passingALL","goff");

  tpTree->Draw("pair_mass>>ZmassFailBB",runrange2 && "abs(tag_sc_eta)<1.5 && abs(probe_eta)<1.5 && !probe_passingALL","goff");

  tpTree->Draw("pair_mass>>ZmassFailEnd",runrange2 && "!(abs(tag_sc_eta)<1.5 && abs(probe_eta)<1.5) && !probe_passingALL","goff");



  tdrStyle->SetOptStat(1);
  ThreeCategorySimZFitter(*ZmassPass, *ZmassFailBB, *ZmassFailEnd, lumi);



/*
  TCanvas* cpass = new TCanvas("cpass","pass",500,500);
  ZmassPass->Draw("E1");

  TCanvas* cfail = new TCanvas("cfail","fail",500,500);
  ZmassFail->Draw("E1");

*/



}



TH1F* createHistogram(char* name, int nbins=12) {
  TH1F* hist = new TH1F(name,name, nbins, 60, 120);
  hist->SetTitle("");
  hist->GetXaxis()->SetTitle("m_{ee} (GeV)");
  hist->GetYaxis()->SetTitle("number of events / 5 GeV");
  return hist;
}
