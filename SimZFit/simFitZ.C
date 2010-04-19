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


#include "SingleBinSimZFitter.C"

void simFitZ()
{
  TString inputFileName = "/uscms_data/d2/kalanand/trash/"
     "Summer09-7TeV-ZeeJets-Pt_80_120.root";

  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.1);
  tdrStyle->SetPadTopMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);


  TFile* f = new TFile(inputFileName);
  TTree* t = (TTree*) f->Get("ZJet");
  TH1D* ZmassPass = new TH1D("ZmassPass", "", 40, 60, 120);
  TH1D* ZmassFail = new TH1D("ZmassFail", "", 40, 60, 120);
  TCut passCut =  "ePlus_HoverE<0.05 && eMinus_HoverE<0.05";
 
  t->Draw("mZee>>+ZmassPass", passCut, "goff");
  t->Draw("mZee>>+ZmassFail", !passCut, "goff");



  ///////////////////////////////////////
  /////////////////////////////////////

  SingleBinSimZFitter(*ZmassPass, *ZmassFail);
}
