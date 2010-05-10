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


//#include "SingleBinSimZFitter.C"
#include "ThreeBinSimZFitter.C"

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

  /*
  TH1D* ZmassPass = new TH1D("ZmassPass", "", 40, 60, 120);
  TH1D* ZmassFail = new TH1D("ZmassFail", "", 40, 60, 120);
  TCut passCut =  "ePlus_HoverE<0.05 && eMinus_HoverE<0.05";
 
  t->Draw("mZee>>+ZmassPass", passCut, "goff");
  t->Draw("mZee>>+ZmassFail", !passCut, "goff");
  */


  TH1D* ZmassPass_BB = new TH1D("ZmassPass_BB", "", 40, 60, 120);
  TH1D* ZmassPass_EB = new TH1D("ZmassPass_EB", "", 40, 60, 120);
  TH1D* ZmassPass_EE = new TH1D("ZmassPass_EE", "", 40, 60, 120);

  TH1D* ZmassFail_BB = new TH1D("ZmassFail_BB", "", 40, 60, 120);
  TH1D* ZmassFail_EB = new TH1D("ZmassFail_EB", "", 40, 60, 120);
  TH1D* ZmassFail_EE = new TH1D("ZmassFail_EE", "", 40, 60, 120);


  TCut passPlus =  "(ePlus_HoverE<0.05) && ((abs(ePlusEta)<1.4442" 
    "&& ePlus_SigmaIetaIeta<0.01 && ePlus_DeltaEtaIn<0.0071)"
    "|| (abs(ePlusEta)>1.56 && ePlus_SigmaIetaIeta<0.028 && ePlus_DeltaEtaIn<0.0066))";

  TCut passMinus =  "(eMinus_HoverE<0.05) && ((abs(eMinusEta)<1.4442" 
    "&& eMinus_SigmaIetaIeta<0.01 && eMinus_DeltaEtaIn<0.0071)"
    "|| (abs(eMinusEta)>1.56 && eMinus_SigmaIetaIeta<0.028 && eMinus_DeltaEtaIn<0.0066))";


  TCut bbCut =  "abs(ePlusEta)<1.4442 && abs(eMinusEta)<1.4442";
  TCut ebCut = "(abs(ePlusEta)<1.4442 && abs(eMinusEta)>1.5660)"
    "|| (abs(eMinusEta)<1.4442 && abs(ePlusEta)>1.5660)";
  TCut eeCut =  "abs(ePlusEta)>1.5660 && abs(eMinusEta)>1.5660";
  //  TCut failCut =  "ePlus_HoverE>0.05";


  t->Draw("mZee>>+ZmassPass_BB", passPlus && passMinus && bbCut, "goff");
  t->Draw("mZee>>+ZmassPass_EB", passPlus && passMinus && ebCut, "goff");
  t->Draw("mZee>>+ZmassPass_EE", passPlus && passMinus && eeCut, "goff");

  t->Draw("mZee>>+ZmassFail_BB", !passPlus && passMinus  && bbCut, "goff");
  t->Draw("mZee>>+ZmassFail_EB", !passPlus && passMinus  && ebCut, "goff");
  t->Draw("mZee>>+ZmassFail_EE", !passPlus && passMinus  && eeCut, "goff");


  ///////////////////////////////////////
  /////////////////////////////////////

  //SingleBinSimZFitter(*ZmassPass, *ZmassFail);
  ThreeBinSimZFitter(*ZmassPass_BB, *ZmassFail_BB, *ZmassPass_EB, *ZmassFail_EB, *ZmassPass_EE, *ZmassFail_EE);
}
