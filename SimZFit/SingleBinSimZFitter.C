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
 *  User is supposed to provide two histograms corresponding to 
 *    "tag + tag" and "tag + Fail SC" invariant masses.
 *  Use different signal and background PDFs for the 'TT' and 'TF' cases. 
 *
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 ********************************************************************/


//// The following two are the irreducible free parameters of the fit:
////        Z cross section and single electron reconstruction efficiency.
////  Additionally, the following two nuisance parameters are floating:
////        nBkgFail, bkgGammaFail.
////  If we have a lot more data, we can float nBkgPass parameter.


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
RooAbsPdf* signalShapePdf_;
RooAbsPdf* signalShapeFailPdf_;
RooAbsPdf* bkgShapePdf_;
RooAbsPdf *bkgShapeFailPdf_;


// Private global variables needed to define PDFs
RooRealVar *bkgGamma_;
RooAbsPdf *rooCMSBkgPdf_;
RooRealVar *bkgGammaFail_;
TCanvas *c;
TFile* Zeelineshape_file;
TH1D* th1_pass;
RooDataHist* rdh_pass;
TH1D* th1_fail;
RooDataHist* rdh_fail;



void SingleBinSimZFitter( TH1& hist1, TH1& hist2 )
{

  // The fit variable - lepton invariant mass
  rooMass_ = new RooRealVar("Mass","m_{e^{+}e^{-}}", 
			    60.0, 120.0, "GeV/c^{2}");
  rooMass_->setBins(120.0);
  RooRealVar Mass = *rooMass_;


  // Make the category variable that defines the two fits,
  // namely whether the probe passes or fails the eff criteria.
  RooCategory sample("sample","") ;
  sample.defineType("pass", 1) ;
  sample.defineType("fail", 0) ; 


  gROOT->cd();

  RooDataHist* dataPass = new RooDataHist("dataPass","dataPass",
					  RooArgList(Mass), &hist1);
  RooDataHist* dataFail = new RooDataHist("dataFail","dataFail",
					  RooArgList(Mass), &hist2);

  RooDataHist* data = new RooDataHist("fitData","fitData",
                                      RooArgList(Mass),Index(sample),
                                      Import("pass",*dataPass),
                                      Import("fail",*dataFail)); 

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


  // Define efficiency variable 
  RooRealVar eff("eff","eff", 0.9, 0.5, 1.0);


  // Now define acceptance variables --> we get these numbers from MC   
  RooRealVar acc("acc","acc", 0.4340);


  // Define background yield variables: they are not related to each other  
  RooRealVar nBkgPass("nBkgPass","nBkgPass", 0.0);
  RooRealVar nBkgFail("nBkgFail","nBkgFail",100.,-10.,1000000000.);



  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
 //  Define signal yield variables.  
  // They are linked together by the total cross section:  e.g. 
  //          N = sigma*L*A*eff

  RooFormulaVar nSigPass("nSigPass", "lumi*xsec*acc*eff*eff", 
			 RooArgList(lumi,xsec,acc,eff) );

  RooFormulaVar nSigFail("nSigFail","lumi*xsec*acc*eff*(1.0-eff)", 
			 RooArgList(lumi,xsec,acc,eff) );

  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

   RooArgList componentspass(*signalShapePdf_, *bkgShapePdf_);
   RooArgList yieldspass(nSigPass, nBkgPass);
   RooArgList componentsfail(*signalShapeFailPdf_,*bkgShapeFailPdf_);
   RooArgList yieldsfail(nSigFail, nBkgFail);	  

   RooAddPdf sumpass("sumpass","extended sum pdf",componentspass,yieldspass);
   RooAddPdf sumfail("sumfail","extended sum pdf",componentsfail, yieldsfail);


   // The total simultaneous fit ...
   RooSimultaneous totalPdf("totalPdf","totalPdf", sample);
   totalPdf.addPdf(sumpass,"pass");
   totalPdf.Print();
   totalPdf.addPdf(sumfail,"fail");
   totalPdf.Print();



  // ********* Do the Actual Fit ********** //  
  RooFitResult *fitResult = totalPdf.fitTo(*data, Save(true), Extended(true));
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


  TString cname = TString("fit_canvas_") + hist1.GetName();
  c = new TCanvas(cname,cname,500,500);
  sample.setLabel("pass");
  RooPlot* frame1 = Mass.frame();
  dataPass->plotOn(frame1,RooFit::DataError(errorType));
  sumpass.plotOn(frame1, ProjWData(*dataPass),
  Components(*bkgShapePdf_),LineColor(kRed));
  sumpass.plotOn(frame1,ProjWData(*dataPass));
  frame1->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));




  TString cname = TString("fit_canvas_") + hist2.GetName();
  c = new TCanvas(cname,cname,500,500);
  sample.setLabel("fail");
  RooPlot* frame2 = Mass.frame();
  dataFail->plotOn(frame2,RooFit::DataError(errorType));
  sumfail.plotOn(frame2,ProjWData(*dataFail),
  Components(*bkgShapeFailPdf_),LineColor(kRed));
  sumfail.plotOn(frame2,ProjWData(*dataFail));
  frame2->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


  //    if(data) delete data;
  //    if(c) delete c;
}








// ***** Function to return the signal Pdf *** //
void makeSignalPdf()
{
  // Tag+Tag selection pdf
  Zeelineshape_file =  new TFile("Zlineshapes.root", "READ");
  TH1D* th1_pass_BB = (TH1D*) Zeelineshape_file->Get("pass_BB");
  TH1D* th1_pass_EB = (TH1D*) Zeelineshape_file->Get("pass_BE");
  TH1D* th1_pass_EE = (TH1D*) Zeelineshape_file->Get("pass_EE");

  th1_pass = (TH1D*) th1_pass_BB->Clone("th1_pass");
  th1_pass->Add(th1_pass_EB);
  th1_pass->Add(th1_pass_EE);


  rdh_pass = new RooDataHist("rdh_pass","", *rooMass_, th1_pass);
  signalShapePdf_ = new RooHistPdf("signalShapePdf","",RooArgSet(*rooMass_),*rdh_pass);



  // Tag+Fail selection pdf
  TH1D* th1_fail_BB = (TH1D*) Zeelineshape_file->Get("fail_BB");
  TH1D* th1_fail_EB = (TH1D*) Zeelineshape_file->Get("fail_BE");
  TH1D* th1_fail_EE = (TH1D*) Zeelineshape_file->Get("fail_EE");

  th1_fail = (TH1D*) th1_fail_BB->Clone("th1_fail");
  th1_fail->Add(th1_fail_EB);
  th1_fail->Add(th1_fail_EE);

  rdh_fail = new RooDataHist("rdh_fail","", *rooMass_, th1_fail);
  signalShapeFailPdf_ = new RooHistPdf("signalShapeFailPdf", "", 
				       RooArgSet(*rooMass_), *rdh_fail);
}








// ***** Function to return the background Pdf **** //
void makeBkgPdf()
{  
  // Background PDF variables
  // ************* Fix the background shape PDF to 0 for the TT component
   bkgGamma_ = new RooRealVar("bkgGamma","bkgGamma", 0.0);
   bkgShapePdf_ =  new RooPolynomial("bkgShapePdf","bkgShapePdf", 
                    *rooMass_, *bkgGamma_);


   bkgGammaFail_ = new RooRealVar("bkgGammaFail","bkgGammaFail",
   0.5, -1000., 1000.);
   bkgShapeFailPdf_ = new RooPolynomial("bkgShapeFailPdf","bkgShapeFailPdf", 
                      *rooMass_, *bkgGammaFail_);
}


