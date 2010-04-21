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
RooAbsPdf* zVoigtianPdf_;
RooAbsPdf* zgammaintfPdf_;
RooAbsPdf* signalShapePdf_;
RooAbsPdf* signalShapeFailPdf_;
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


  // Now define some efficiency/yield variables  
  RooRealVar efficiency_BB("efficiency_BB","efficiency_BB", 0.9, 0.0, 1.0);
  RooRealVar efficiency_EB("efficiency_EB","efficiency_EB", 0.9, 0.0, 1.0);
  RooRealVar efficiency_EE("efficiency_EE","efficiency_EE", 0.9, 0.0, 1.0);


  RooRealVar numSignal_BB("numSignal_BB","numSignal_BB", 4000.0, -10.0, 1000000000.0);
  RooRealVar numSignal_EB("numSignal_EB","numSignal_EB", 4000.0, -10.0, 1000000000.0);
  RooRealVar numSignal_EE("numSignal_EE","numSignal_EE", 4000.0, -10.0, 1000000000.0);


  RooRealVar numBkgPass_BB("numBkgPass_BB","numBkgPass_BB", 1000.0, -10.0, 1000000000.0);
  RooRealVar numBkgPass_EB("numBkgPass_EB","numBkgPass_EB", 1000.0, -10.0, 1000000000.0);
  RooRealVar numBkgPass_EE("numBkgPass_EE","numBkgPass_EE", 1000.0, -10.0, 1000000000.0);


  RooRealVar numBkgFail_BB("numBkgFail_BB","numBkgFail_BB", 1000.0, -10.0, 1000000000.0);
  RooRealVar numBkgFail_EB("numBkgFail_EB","numBkgFail_EB", 1000.0, -10.0, 1000000000.0);
  RooRealVar numBkgFail_EE("numBkgFail_EE","numBkgFail_EE", 1000.0, -10.0, 1000000000.0);

  // RooArgList components(*signalShapePdf_,*bkgShapePdf_);

   RooFormulaVar numSigPass_BB("numSigPass_BB","numSignal_BB*efficiency_BB",
			       RooArgList(numSignal_BB,efficiency_BB));
   RooFormulaVar numSigPass_EB("numSigPass_EB","numSignal_EB*efficiency_EB",
			       RooArgList(numSignal_EB,efficiency_EB));
   RooFormulaVar numSigPass_EE("numSigPass_EE","numSignal_EE*efficiency_EE",
			       RooArgList(numSignal_EE,efficiency_EE));


   RooFormulaVar numSigFail_BB("numSigFail_BB","numSignal_BB*(1.0-efficiency_BB)",
			       RooArgList(numSignal_BB,efficiency_BB) );
   RooFormulaVar numSigFail_EB("numSigFail_EB","numSignal_EB*(1.0-efficiency_EB)",
			       RooArgList(numSignal_EB,efficiency_EB) );
   RooFormulaVar numSigFail_EE("numSigFail_EE","numSignal_EE*(1.0-efficiency_EE)",
			       RooArgList(numSignal_EE,efficiency_EE) );


   RooArgList componentspass_BB(*signalShapePdf_, *bkgShapePdf_);
   RooArgList componentspass_EB(*signalShapePdf_, *bkgShapePdf_);
   RooArgList componentspass_EE(*signalShapePdf_, *bkgShapePdf_);

   RooArgList yieldspass_BB(numSigPass_BB, numBkgPass_BB);
   RooArgList yieldspass_EB(numSigPass_EB, numBkgPass_EB);
   RooArgList yieldspass_EE(numSigPass_EE, numBkgPass_EE);


   RooArgList componentsfail_BB(*signalShapeFailPdf_,*bkgShapeFailPdf_);
   RooArgList componentsfail_EB(*signalShapeFailPdf_,*bkgShapeFailPdf_);
   RooArgList componentsfail_EE(*signalShapeFailPdf_,*bkgShapeFailPdf_);

   RooArgList yieldsfail_BB( numSigFail_BB, numBkgFail_BB );	  
   RooArgList yieldsfail_EB(numSigFail_EB, numBkgFail_EB);	  
   RooArgList yieldsfail_EE(numSigFail_EE, numBkgFail_EE);


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








// ***** Function to return the signal Pdf *** //
void makeSignalPdf()
{

  // Signal PDF variables
  rooZMean_   = new RooRealVar("zMean","zMean", 91.1876, 88., 94.);
  rooZWidth_  = new RooRealVar("zWidth","zWidth", 2.8, 0.0, 10.0);
  rooZSigma_  = new RooRealVar("zSigma","zSigma", 2.8, 0.0, 10.0);
  rooZJ_  = new RooRealVar("z_gam_intf","", 0.01, 0.0, 1.0);


  // Voigtian for signal component of the passing probe PDF
  zVoigtianPdf_ = new RooVoigtian("zVoigtianPdf", "zVoigtianPdf", 
				  *rooMass_, *rooZMean_, 
				  *rooZWidth_, *rooZSigma_);

  // Tord Riemann formula::::
  // Z lineshape = [ (s/M_Z^2)*(1+J) - J] . BW, where J = Z--gamma int = 0.07 
  

  zgammaintfPdf_ = new RooGenericPdf("zgammaintfPdf_",
				     "@0*@0*1.07/(91.1876*91.1876)-0.07",
				     RooArgList(*rooMass_));
  signalShapePdf_ = new RooProdPdf("signalShapePdf","signalShapePdf",
				   *zVoigtianPdf_, *zgammaintfPdf_);



  rooCBMean_  = new RooRealVar("cbMean","cbMean", 90., 80., 100.);
  rooCBSigma_ = new RooRealVar("cbSigma","cbSigma", 2., 0.1, 200.);
  rooCBAlpha_ = new RooRealVar("cbAlpha","cbAlpha", 0.1, 0.001, 200);
  rooCBN_     = new RooRealVar("cbN","cbN", 1., 0., 2000.);

  // CB shape for signal component of the failing probe PDF
  signalShapeFailPdf_ = new RooCBShape("cbPdf","cbPdf",*rooMass_,
  *rooCBMean_, *rooCBSigma_,*rooCBAlpha_,*rooCBN_);
  
 // signalShapeFailPdf_  = rooCBPdf_;
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


