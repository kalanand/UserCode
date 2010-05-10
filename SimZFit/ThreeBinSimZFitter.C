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
 *   A standalone macro to perform single bin simultaneos fit of 
 *   dilepton mass under Z peak. 
 *   Works with  binned data. 
 *
 * Implementation details:
 *  Uses RooFit classes.
 *  User is supposed to provide six histograms corresponding to 
 *    "tag + tag" and "tag+ Fail SC" invariant masses with 
 *     BB, EB, and EE combination.
 *  Use different signal and background PDFs for 'TT' and 'TF' cases.
 *
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 ********************************************************************/

//// The following three are the irreducible free parameters of the fit:
////        Z cross section, efficiency_barrel, efficiency_endcap.
////  Additionally, the following five nuisance parameters are floating:
////        nBkgFail_BB,  nBkgFail_EB, nBkgFail_EE, bkgGamma, bkgGammaFail.
////  If we have a lot more data, we can float three nBkgPass pars.


// ROOT
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TGraph.h>


using namespace RooFit;

// The signal & background Pdf 
RooRealVar *rooMass_;
RooAbsPdf* zVoigtianPdf_;
RooAbsPdf* zgammaintfPdf_;

RooAbsPdf* signalShapePdfBB_;
RooAbsPdf* signalShapePdfEB_;
RooAbsPdf* signalShapePdfEE_;

RooAbsPdf* signalShapeFailPdfBB_;
RooAbsPdf* signalShapeFailPdfEB_;
RooAbsPdf* signalShapeFailPdfEE_;


RooAbsPdf* bkgShapePdf_;


// Private variables needed for ZLineShape
RooRealVar*    rooZMean_;
RooRealVar*    rooZWidth_;
RooRealVar*    rooZSigma_;
RooRealVar*    rooZJ;

RooRealVar*    rooCBMean_ ;
RooRealVar*    rooCBSigma_;
RooRealVar*    rooCBAlpha_;
RooRealVar*    rooCBN_;


// Private variables needed for background shape
RooRealVar *bkgGamma_;
RooAbsPdf *rooCMSBkgPdf_;
RooRealVar *bkgGammaFail_;
RooAbsPdf *bkgShapeFailPdf_;

TCanvas *c;
TFile* Zeelineshape_file;
TH1D* th1_pass_BB;
TH1D* th1_pass_EB;
TH1D* th1_pass_EE;
RooDataHist* rdh_pass_BB;
RooDataHist* rdh_pass_EB;
RooDataHist* rdh_pass_EE;

TH1D* th1_fail_BB;
TH1D* th1_fail_EB;
TH1D* th1_fail_EE;
RooDataHist* rdh_fail_BB;
RooDataHist* rdh_fail_EB;
RooDataHist* rdh_fail_EE;



void ThreeBinSimZFitter( TH1& h_BB_pass, TH1& h_BB_fail,  
			 TH1& h_EB_pass, TH1& h_EB_fail,
			 TH1& h_EE_pass, TH1& h_EE_fail )
{

  // The fit variable - lepton invariant mass
  rooMass_ = new RooRealVar("Mass","m_{e^{+}e^{-}}", 
			    60.0, 120.0, "GeV/c^{2}");
  rooMass_->setBins(120.0);
  RooRealVar Mass = *rooMass_;


  // Make the category variable that defines the two fits,
  // namely whether the probe passes or fails the eff criteria.
  RooCategory sample("sample","") ;
  sample.defineType("BB_pass", 1) ;
  sample.defineType("BB_fail", 2) ; 
  sample.defineType("EB_pass", 3) ;
  sample.defineType("EB_fail", 4) ; 
  sample.defineType("EE_pass", 5) ;
  sample.defineType("EE_fail", 6) ; 



  gROOT->cd();

  ///////// convert Histograms into RooDataHists
  RooDataHist* data_BB_pass = new RooDataHist("data_BB_pass","data_BB_pass",
					  RooArgList(Mass), &h_BB_pass);
  RooDataHist* data_BB_fail = new RooDataHist("data_BB_fail","data_BB_fail",
					  RooArgList(Mass), &h_BB_fail);
  RooDataHist* data_EB_pass = new RooDataHist("data_EB_pass","data_EB_pass",
					  RooArgList(Mass), &h_EB_pass);
  RooDataHist* data_EB_fail = new RooDataHist("data_EB_fail","data_EB_fail",
					  RooArgList(Mass), &h_EB_fail);
  RooDataHist* data_EE_pass = new RooDataHist("data_EE_pass","data_EE_pass",
					  RooArgList(Mass), &h_EE_pass);
  RooDataHist* data_EE_fail = new RooDataHist("data_EE_fail","data_EE_fail",
					  RooArgList(Mass), &h_EE_fail);


  RooDataHist* data = new RooDataHist( "fitData","fitData",
				       RooArgList(Mass),Index(sample),
				       Import("BB_pass",*data_BB_pass),
				       Import("BB_fail",*data_BB_fail),
				       Import("EB_pass",*data_EB_pass),
				       Import("EB_fail",*data_EB_fail),
				       Import("EE_pass",*data_EE_pass),
				       Import("EE_fail",*data_EE_fail) ); 

  data->get()->Print();
  cout << "Made datahist" << endl;


  // ********** Construct signal & bkg shape PDF ********** //
  makeSignalPdf();
  cout << "Made signal pdf" << endl;

  makeBkgPdf();
  cout << "Made bkg pdf" << endl;



  // Now supply integrated luminosity in inverse picobarn
  // -->  we get this number from the CMS lumi group
  // https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki2010Data
  RooRealVar lumi("lumi","lumi", 300.0);


  // Now define Z production cross section variable (in pb) 
  RooRealVar xsec("xsec","xsec", 800., 100.0, 4000.0);


  // Define efficiency variables  
  RooRealVar eff_B("eff_B","eff_B", 0.9, 0.5, 1.0);
  RooRealVar eff_E("eff_E","eff_E", 0.9, 0.5, 1.0);


  // Now define acceptance variables --> we get these numbers from MC   
  RooRealVar acc_BB("acc_BB","acc_BB", 0.2180);
  RooRealVar acc_EB("acc_EB","acc_EB", 0.1657);
  RooRealVar acc_EE("acc_EE","acc_EE", 0.0503);



  // Define background yield variables: they are not related to each other  
  RooRealVar nBkgPass_BB("nBkgPass_BB","nBkgPass_BB", 0.0);
  RooRealVar nBkgPass_EB("nBkgPass_EB","nBkgPass_EB", 0.0);
  RooRealVar nBkgPass_EE("nBkgPass_EE","nBkgPass_EE", 0.0);

  RooRealVar nBkgFail_BB("nBkgFail_BB","nBkgFail_BB",100.,-10.,1000000000.);
  RooRealVar nBkgFail_EB("nBkgFail_EB","nBkgFail_EB", 100.,-10.,1000000000.);
  RooRealVar nBkgFail_EE("nBkgFail_EE","nBkgFail_EE", 100.,-10.,1000000000.);


  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
 //  Define signal yield variables.  
  // They are linked together by the total cross section:  e.g. 
  //          Nbb = sigma*L*Abb*eff_BB

  char* formula;
  RooArgList* args;
  formula = "lumi*xsec*acc_BB*eff_B*eff_B";
  args = new RooArgList(lumi,xsec,acc_BB,eff_B);
  RooFormulaVar nSigPass_BB("nSigPass_BB", formula, *args);
  delete args;

  formula = "lumi*xsec*acc_EB*eff_B*eff_E";
  args = new RooArgList(lumi,xsec,acc_EB,eff_B,eff_E);
  RooFormulaVar nSigPass_EB("nSigPass_EB",formula, *args);
  delete args;

  formula = "lumi*xsec*acc_EE*eff_E*eff_E";
  args = new RooArgList(lumi,xsec,acc_EE,eff_E);
  RooFormulaVar nSigPass_EE("nSigPass_EE",formula, *args);
  delete args;


  formula = "lumi*xsec*acc_BB*eff_B*(1.0-eff_B)";
  args = new RooArgList(lumi,xsec,acc_BB,eff_B);
  RooFormulaVar nSigFail_BB("nSigFail_BB",formula, *args);
  delete args;

  formula = "lumi*xsec*acc_EB*eff_B*(1.0-eff_E)";
  args = new RooArgList(lumi,xsec,acc_EB,eff_B,eff_E);
  RooFormulaVar nSigFail_EB("nSigFail_EB",formula, *args);
  delete args;

  formula = "lumi*xsec*acc_EE*eff_E*(1.0-eff_E)";
  args = new RooArgList(lumi,xsec,acc_EE,eff_E);
  RooFormulaVar nSigFail_EE("nSigFail_EE",formula, *args);
  delete args;
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

   RooArgList componentspass_BB(*signalShapePdfBB_, *bkgShapePdf_);
   RooArgList componentspass_EB(*signalShapePdfEB_, *bkgShapePdf_);
   RooArgList componentspass_EE(*signalShapePdfEE_, *bkgShapePdf_);

   RooArgList yieldspass_BB(nSigPass_BB, nBkgPass_BB);
   RooArgList yieldspass_EB(nSigPass_EB, nBkgPass_EB);
   RooArgList yieldspass_EE(nSigPass_EE, nBkgPass_EE);


   RooArgList componentsfail_BB(*signalShapeFailPdfBB_,*bkgShapeFailPdf_);
   RooArgList componentsfail_EB(*signalShapeFailPdfEB_,*bkgShapeFailPdf_);
   RooArgList componentsfail_EE(*signalShapeFailPdfEE_,*bkgShapeFailPdf_);

   RooArgList yieldsfail_BB( nSigFail_BB, nBkgFail_BB );	  
   RooArgList yieldsfail_EB(nSigFail_EB, nBkgFail_EB);	  
   RooArgList yieldsfail_EE(nSigFail_EE, nBkgFail_EE);


   RooAddPdf sumpass_BB("sumpass_BB","fixed extended sum pdf",
			componentspass_BB,yieldspass_BB);
   RooAddPdf sumpass_EB("sumpass_EB","fixed extended sum pdf",
			componentspass_EB,yieldspass_EB);
   RooAddPdf sumpass_EE("sumpass_EE","fixed extended sum pdf",
			componentspass_EE,yieldspass_EE);

   RooAddPdf sumfail_BB("sumfail_BB","fixed extended sum pdf",
			componentsfail_BB, yieldsfail_BB);
   RooAddPdf sumfail_EB("sumfail_EB","fixed extended sum pdf",
			componentsfail_EB, yieldsfail_EB);
   RooAddPdf sumfail_EE("sumfail_EE","fixed extended sum pdf",
			componentsfail_EE, yieldsfail_EE);



   // The total simultaneous fit ...
   RooSimultaneous totalPdf("totalPdf","totalPdf", sample);
   totalPdf.addPdf(sumpass_BB,"BB_pass");
   totalPdf.addPdf(sumpass_EB,"EB_pass");
   totalPdf.addPdf(sumpass_EE,"EE_pass");
   totalPdf.Print();
   totalPdf.addPdf(sumfail_BB,"BB_fail");
   totalPdf.addPdf(sumfail_EB,"EB_fail");
   totalPdf.addPdf(sumfail_EE,"EE_fail");
   totalPdf.Print();


  // ********* Do the Actual Fit ********** //  
   RooFitResult *fitResult = totalPdf.fitTo(*data, Save(true), 
					    Extended(true), 
					    PrintEvalErrors(-1), 
					    Warnings(false) );
  fitResult->Print("v");


  // ********** Make and save Canvas for the plots ********** //
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadLeftMargin(0.19);
  tdrStyle->SetPadRightMargin(0.10);
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(1.5);
  RooAbsData::ErrorType errorType = RooAbsData::SumW2;


  TString cname = TString("fit_canvas_") + h_BB_pass.GetName();
  c = new TCanvas(cname,cname,500,500);
  sample.setLabel("BB_pass");
  RooPlot* frame1 = Mass.frame();
  data_BB_pass->plotOn(frame1,RooFit::DataError(errorType));
  sumpass_BB.plotOn(frame1, ProjWData(*data_BB_pass),
  Components(*bkgShapePdf_),LineColor(kRed));
  sumpass_BB.plotOn(frame1,ProjWData(*data_BB_pass));
  frame1->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


  TString cname = TString("fit_canvas_") + h_BB_fail.GetName();
  c = new TCanvas(cname,cname,500,500);
  sample.setLabel("BB_fail");
  RooPlot* frame2 = Mass.frame();
  data_BB_fail->plotOn(frame2,RooFit::DataError(errorType));
  sumfail_BB.plotOn(frame2,ProjWData(*data_BB_fail),
  Components(*bkgShapeFailPdf_),LineColor(kRed));
  sumfail_BB.plotOn(frame2,ProjWData(*data_BB_fail));
  frame2->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


  TString cname = TString("fit_canvas_") + h_EB_pass.GetName();
  c = new TCanvas(cname,cname,500,500);
  sample.setLabel("EB_pass");
  RooPlot* frame1 = Mass.frame();
  data_EB_pass->plotOn(frame1,RooFit::DataError(errorType));
  sumpass_EB.plotOn(frame1, ProjWData(*data_EB_pass),
  Components(*bkgShapePdf_),LineColor(kRed));
  sumpass_EB.plotOn(frame1,ProjWData(*data_EB_pass));
  frame1->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


  TString cname = TString("fit_canvas_") + h_EB_fail.GetName();
  c = new TCanvas(cname,cname,500,500);
  sample.setLabel("EB_fail");
  RooPlot* frame2 = Mass.frame();
  data_EB_fail->plotOn(frame2,RooFit::DataError(errorType));
  sumfail_EB.plotOn(frame2,ProjWData(*data_EB_fail),
  Components(*bkgShapeFailPdf_),LineColor(kRed));
  sumfail_EB.plotOn(frame2,ProjWData(*data_EB_fail));
  frame2->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


 TString cname = TString("fit_canvas_") + h_EE_pass.GetName();
  c = new TCanvas(cname,cname,500,500);
  sample.setLabel("EE_pass");
  RooPlot* frame1 = Mass.frame();
  data_EE_pass->plotOn(frame1,RooFit::DataError(errorType));
  sumpass_EE.plotOn(frame1, ProjWData(*data_EE_pass),
  Components(*bkgShapePdf_),LineColor(kRed));
  sumpass_EE.plotOn(frame1,ProjWData(*data_EE_pass));
  frame1->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


  TString cname = TString("fit_canvas_") + h_EE_fail.GetName();
  c = new TCanvas(cname,cname,500,500);
  sample.setLabel("EE_fail");
  RooPlot* frame2 = Mass.frame();
  data_EE_fail->plotOn(frame2,RooFit::DataError(errorType));
  sumfail_EE.plotOn(frame2,ProjWData(*data_EE_fail),
  Components(*bkgShapeFailPdf_),LineColor(kRed));
  sumfail_EE.plotOn(frame2,ProjWData(*data_EE_fail));
  frame2->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));



  //    if(data) delete data;
  //    if(c) delete c;
}








// // ***** Function to return the signal Pdf *** //
 void makeSignalPdf() {

  // Tag+Tag selection pdf
  Zeelineshape_file =  new TFile("Zlineshapes.root", "READ");
  th1_pass_BB = (TH1D*) Zeelineshape_file->Get("pass_BB");
  th1_pass_EB = (TH1D*) Zeelineshape_file->Get("pass_BE");
  th1_pass_EE = (TH1D*) Zeelineshape_file->Get("pass_EE");

  rdh_pass_BB = new RooDataHist("rdh_pass_BB","", *rooMass_, th1_pass_BB);
  rdh_pass_EB = new RooDataHist("rdh_pass_EB","", *rooMass_, th1_pass_EB);
  rdh_pass_EE = new RooDataHist("rdh_pass_EE","", *rooMass_, th1_pass_EE);

  signalShapePdfBB_ = new RooHistPdf("signalShapePdfBB", "", RooArgSet(*rooMass_), *rdh_pass_BB);
  signalShapePdfEB_ = new RooHistPdf("signalShapePdfEB", "", RooArgSet(*rooMass_), *rdh_pass_EB);
  signalShapePdfEE_ = new RooHistPdf("signalShapePdfEE", "", RooArgSet(*rooMass_), *rdh_pass_EE);



  // Tag+Fail selection pdf
  th1_fail_BB = (TH1D*) Zeelineshape_file->Get("fail_BB");
  th1_fail_EB = (TH1D*) Zeelineshape_file->Get("fail_BE");
  th1_fail_EE = (TH1D*) Zeelineshape_file->Get("fail_EE");

  rdh_fail_BB = new RooDataHist("rdh_fail_BB","", *rooMass_, th1_fail_BB);
  rdh_fail_EB = new RooDataHist("rdh_fail_EB","", *rooMass_, th1_fail_EB);
  rdh_fail_EE = new RooDataHist("rdh_fail_EE","", *rooMass_, th1_fail_EE);


  signalShapeFailPdfBB_ = new RooHistPdf("signalShapeFailPdfBB", "", RooArgSet(*rooMass_), *rdh_fail_BB);
  signalShapeFailPdfEB_ = new RooHistPdf("signalShapeFailPdfEB", "", RooArgSet(*rooMass_), *rdh_fail_EB);
  signalShapeFailPdfEE_ = new RooHistPdf("signalShapeFailPdfEE", "", RooArgSet(*rooMass_), *rdh_fail_EE);

}








// ***** Function to return the background Pdf **** //
void makeBkgPdf()
{  
  // Background PDF variables
   bkgGamma_ = new RooRealVar("bkgGamma","bkgGamma", 0.5, -1000., 1000.);
   bkgShapePdf_ =  new RooPolynomial("bkgShapePdf","bkgShapePdf", *rooMass_, *bkgGamma_);


   bkgGammaFail_ = new RooRealVar("bkgGammaFail","bkgGammaFail",
   0.5, -1000., 1000.);
   bkgShapeFailPdf_ = new RooPolynomial("bkgShapeFailPdf","bkgShapeFailPdf", 
                      *rooMass_, *bkgGammaFail_);
}


