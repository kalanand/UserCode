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
#include "TwoCategorySimZFitter.C"
//#include "ThreeCategorySimZFitter.C"
//#include "FourCategorySimZFitter.C"


void simFitZ()
{
  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetLegendBorderSize(0);

  TFile* f = new TFile("mikhail/lineshapesC.root");
  TH1D* ZmassPass = (TH1D*) f->Get("passALL");
  TH1D* ZmassFail = (TH1D*) f->Get("failALL");
  TH1D* ZmassFail_BB = (TH1D*) f->Get("fail_BB");
  TH1D* ZmassFail_E = (TH1D*) f->Get("fail_E");
  TH1D* ZmassFail_EB = (TH1D*) f->Get("fail_BE");
  TH1D* ZmassFail_EE = (TH1D*) f->Get("fail_EE");

  double lumi = 0.3;
  ScaleToLumi(*ZmassPass, lumi);
  ScaleToLumi(*ZmassFail, lumi);
  ScaleToLumi(*ZmassFail_BB, lumi);
  ScaleToLumi(*ZmassFail_EB, lumi);
  ScaleToLumi(*ZmassFail_E, lumi);
  ScaleToLumi(*ZmassFail_EE, lumi);

  TwoCategorySimZFitter(*ZmassPass, *ZmassFail, lumi);

  // ThreeCategorySimZFitter(*ZmassPass, *ZmassFail_BB, *ZmassFail_E, lumi);
  // FourCategorySimZFitter(*ZmassPass, *ZmassFail_BB, *ZmassFail_EB, 
  //                        *ZmassFail_EE, lumi);
}



void ScaleToLumi( TH1& hist, double lumi) {

  int bins = hist.GetNbinsX();
  double entry = 0;

  for(int i=0; i< bins; i++) {
     entry = lumi*hist.GetBinContent(i+1);
     hist.SetBinContent(i+1,  entry);
     hist.SetBinError(i+1, sqrt(entry) );
  }

}
