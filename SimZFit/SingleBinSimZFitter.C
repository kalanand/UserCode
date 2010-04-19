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
 *    "tag + probePass" and "tag+probeFail" invariant masses.
 *  Use different signal and background PDFs for the 'probePass' 
 *    and 'probeFail' cases.   
 *
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 ********************************************************************/

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

// Private variables needed for ZLineShape
RooRealVar*    rooZMean_;
RooRealVar*    rooZWidth_;
RooRealVar*    rooZSigma_;

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


  // Now define some efficiency/yield variables  
  RooRealVar efficiency("efficiency","efficiency", 0.9, 0.0, 1.0);
  RooRealVar numSignal("numSignal","numSignal", 4000.0, -10.0, 1000000000.0);
  RooRealVar numBkgPass("numBkgPass","numBkgPass", 1000.0, -10.0, 1000000000.0);
  RooRealVar numBkgFail("numBkgFail","numBkgFail", 1000.0, -10.0, 1000000000.0);
  RooArgList components(*signalShapePdf_,*bkgShapePdf_);


   RooFormulaVar numSigPass("numSigPass","numSignal*efficiency", 
   RooArgList(numSignal,efficiency) );
   RooFormulaVar numSigFail("numSigFail","numSignal*(1.0 - efficiency)", 
   RooArgList(numSignal,efficiency) );

   RooArgList componentspass(*signalShapePdf_, *bkgShapePdf_);
   RooArgList yieldspass(numSigPass, numBkgPass);
   RooArgList componentsfail(*signalShapeFailPdf_,*bkgShapeFailPdf_);
   RooArgList yieldsfail(numSigFail, numBkgFail);	  

   RooAddPdf sumpass("sumpass","fixed extended sum pdf",
   componentspass,yieldspass);
   RooAddPdf sumfail("sumfail","fixed extended sum pdf",
   componentsfail, yieldsfail);

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

  // Signal PDF variables
  rooZMean_   = new RooRealVar("zMean","zMean", 91.1876, 87.5, 94.5);
  rooZWidth_  = new RooRealVar("zWidth","zWidth", 2.8, 0.0, 10.0);
  rooZSigma_  = new RooRealVar("zSigma","zSigma", 2.8, 0.0, 10.0);

  // Voigtian for signal component of the passing probe PDF
  signalShapePdf_ = new RooVoigtian("signalShapePdf", "signalShapePdf", 
				  *rooMass_, *rooZMean_, 
				  *rooZWidth_, *rooZSigma_);

  rooCBMean_  = new RooRealVar("cbMean","cbMean", 90., 80., 100.);
  rooCBSigma_ = new RooRealVar("cbSigma","cbSigma", 2., 0.1, 20.);
  rooCBAlpha_ = new RooRealVar("cbAlpha","cbAlpha", 0.1, 0.001, 50);
  rooCBN_     = new RooRealVar("cbN","cbN", 1., 0., 200.);

  // CB shape for signal component of the failing probe PDF
  signalShapeFailPdf_ = new RooCBShape("cbPdf","cbPdf",*rooMass_,
  *rooCBMean_, *rooCBSigma_,*rooCBAlpha_,*rooCBN_);
  
 // signalShapeFailPdf_  = rooCBPdf_;
}








// ***** Function to return the background Pdf **** //
void makeBkgPdf()
{  
  // Background PDF variables
   bkgGamma_ = new RooRealVar("bkgGamma","bkgGamma", 0.5, -10., 10.);
   bkgShapePdf_ =  new RooPolynomial("bkgShapePdf","bkgShapePdf", 
                    *rooMass_, *bkgGamma_);


   bkgGammaFail_ = new RooRealVar("bkgGammaFail","bkgGammaFail",
   0.5, -10., 10.);
   bkgShapeFailPdf_ = new RooPolynomial("bkgShapeFailPdf","bkgShapeFailPdf", 
                      *rooMass_, *bkgGammaFail_);
}


