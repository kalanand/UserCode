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
 *  User is supposed to provide four histograms corresponding to 
 *    "tag + tag" invariant mass and "tag+ Fail SC" invariant masses with 
 *     BB, EB, and EE combination.
 *
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 ********************************************************************/

//// Following are the 3 irreducible free parameters of the fit:
////        Z cross section and single electron reco efficiency: eff_B, eff_E.
////  Additionally, the following 4 nuisance parameters are floating:
////        nBkgTF_BB,  nBkgTF_EB, nBkgTF_EE, bkgGamma.


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
RooAbsPdf* signalShapePdfTT_;
RooAbsPdf* signalShapePdfTF_BB_;
RooAbsPdf* signalShapePdfTF_EB_;
RooAbsPdf* signalShapePdfTF_EE_;
RooAbsPdf *bkgShapePdfTF_;


// Private global variables needed to define PDFs
RooRealVar *bkgGammaFailTF_;
TFile* Zeelineshape_file;
TH1D* th1_TF_BB;
TH1D* th1_TF_EB;
TH1D* th1_TF_EE;
RooDataHist* rdh_TF_BB;
RooDataHist* rdh_TF_EB;
RooDataHist* rdh_TF_EE;
TCanvas *c;




void FourCategorySimZFitter( TH1& h_TT, TH1& h_TF_BB, 
			     TH1& h_TF_EB, TH1& h_TF_EE, double intLumi )
{

  // The fit variable - lepton invariant mass
  rooMass_ = new RooRealVar("Mass","m_{ee}", 
			    60.0, 120.0, "GeV/c^{2}");
  rooMass_->setBins(120.0);
  RooRealVar Mass = *rooMass_;


  // Make the category variable that defines the two fits,
  // namely whether the probe passes or fails the eff criteria.
  RooCategory sample("sample","") ;
  sample.defineType("BB_pass", 1) ;
  sample.defineType("BB_fail", 2) ; 
  sample.defineType("EB_fail", 3) ; 
  sample.defineType("EE_fail", 4) ; 


  gROOT->cd();

  ///////// convert Histograms into RooDataHists
  RooDataHist* data_TT = new RooDataHist("data_TT","data_TT",
					  RooArgList(Mass), &h_TT);
  RooDataHist* data_TF_BB = new RooDataHist("data_TF_BB","data_TF_BB",
					  RooArgList(Mass), &h_TF_BB);
  RooDataHist* data_TF_EB = new RooDataHist("data_TF_EB","data_TF_EB",
					  RooArgList(Mass), &h_TF_EB);
  RooDataHist* data_TF_EE = new RooDataHist("data_TF_EE","data_TF_EE",
					  RooArgList(Mass), &h_TF_EE);


  RooDataHist* data = new RooDataHist( "fitData","fitData",
				       RooArgList(Mass),Index(sample),
				       Import("BB_pass",*data_TT),
				       Import("BB_fail",*data_TF_BB),
				       Import("EB_fail",*data_TF_EB),
				       Import("EE_fail",*data_TF_EE) ); 

  data->get()->Print();
  cout << "Made datahist" << endl;


  // ********** Construct signal & bkg shape PDF ********** //
  // Tag+Tag selection pdf
  RooDataHist* rdh_TT = new RooDataHist("rdh_TT","", *rooMass_, &h_TT);
  signalShapePdfTT_ = new RooHistPdf("signalShapePdfTT", "", RooArgSet(*rooMass_), *rdh_TT);

  makeSignalPdf();
  cout << "Made signal pdf" << endl;

  makeBkgPdf();
  cout << "Made bkg pdf" << endl;



  // Now supply integrated luminosity in inverse picobarn
  // -->  we get this number from the CMS lumi group
  // https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki2010Data
  RooRealVar lumi("lumi","lumi", intLumi);


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
  RooRealVar nBkgTF_BB("nBkgTF_BB","nBkgTF_BB",100.,-10.,100000.);
  RooRealVar nBkgTF_EB("nBkgTF_EB","nBkgTF_EB", 100.,-10.,10000.);
  RooRealVar nBkgTF_EE("nBkgTF_EE","nBkgTF_EE", 100.,-10.,10000.);


  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
 //  Define signal yield variables.  
  // They are linked together by the total cross section:  e.g. 
  //          Nbb = sigma*L*Abb*eff_BB

  char* formula;
  RooArgList* args;
  formula = "lumi*xsec*(acc_BB*eff_B*eff_B+acc_EB*eff_B*eff_E+acc_EE*eff_E*eff_E)";
  args = new RooArgList(lumi,xsec,acc_BB,acc_EB,acc_EE,eff_B,eff_E);
  RooFormulaVar nSigTT("nSigTT", formula, *args);
  delete args;


  formula = "lumi*xsec*acc_BB*eff_B*(1.0-eff_B)";
  args = new RooArgList(lumi,xsec,acc_BB,eff_B);
  RooFormulaVar nSigTF_BB("nSigTF_BB",formula, *args);
  delete args;

  formula = "lumi*xsec*acc_EB*(0.5*eff_B+0.5*eff_E-eff_B*eff_E)";
  args = new RooArgList(lumi,xsec,acc_EB,eff_B,eff_E);
  RooFormulaVar nSigTF_EB("nSigTF_EB",formula, *args);
  delete args;

  formula = "lumi*xsec*acc_EE*eff_E*(1.0-eff_E)";
  args = new RooArgList(lumi,xsec,acc_EE,eff_E);
  RooFormulaVar nSigTF_EE("nSigTF_EE",formula, *args);
  delete args;
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

   RooArgList componentsTF_BB(*signalShapePdfTF_BB_,*bkgShapePdfTF_);
   RooArgList componentsTF_EB(*signalShapePdfTF_EB_,*bkgShapePdfTF_);
   RooArgList componentsTF_EE(*signalShapePdfTF_EE_,*bkgShapePdfTF_);

   RooArgList yieldsTF_BB(nSigTF_BB, nBkgTF_BB );	  
   RooArgList yieldsTF_EB(nSigTF_EB, nBkgTF_EB);	  
   RooArgList yieldsTF_EE(nSigTF_EE, nBkgTF_EE);

   RooExtendPdf pdfTT("pdfTT","extended sum pdf", *signalShapePdfTT_, nSigTT);
   RooAddPdf pdfTF_BB("pdfTF_BB","extended sum pdf",componentsTF_BB, yieldsTF_BB);
   RooAddPdf pdfTF_EB("pdfTF_EB","extended sum pdf",componentsTF_EB, yieldsTF_EB);
   RooAddPdf pdfTF_EE("pdfTF_EE","extended sum pdf",componentsTF_EE, yieldsTF_EE);



   // The total simultaneous fit ...
   RooSimultaneous totalPdf("totalPdf","totalPdf", sample);
   totalPdf.addPdf(pdfTT,"BB_pass");
   totalPdf.Print();
   totalPdf.addPdf(pdfTF_BB,"BB_fail");
   totalPdf.addPdf(pdfTF_EB,"EB_fail");
   totalPdf.addPdf(pdfTF_EE,"EE_fail");
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


  TString cname = TString("fit_canvas_") + h_TT.GetName();
  c = new TCanvas(cname,cname,500,500);
  sample.setLabel("pass");
  RooPlot* frame1 = Mass.frame();
  data_TT->plotOn(frame1,RooFit::DataError(errorType));
  pdfTT.plotOn(frame1,ProjWData(*data_TT));
  frame1->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


  TString cname = TString("fit_canvas_") + h_TF_BB.GetName();
  c = new TCanvas(cname,cname,500,500);
  sample.setLabel("BB_fail");
  RooPlot* frame2 = Mass.frame();
  data_TF_BB->plotOn(frame2,RooFit::DataError(errorType));
  pdfTF_BB.plotOn(frame2,ProjWData(*data_TF_BB),
  Components(*bkgShapePdfTF_),LineColor(kRed));
  pdfTF_BB.plotOn(frame2,ProjWData(*data_TF_BB));
  frame2->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


  TString cname = TString("fit_canvas_") + h_TF_EB.GetName();
  c = new TCanvas(cname,cname,500,500);
  sample.setLabel("EB_fail");
  RooPlot* frame2 = Mass.frame();
  data_TF_EB->plotOn(frame2,RooFit::DataError(errorType));
  pdfTF_EB.plotOn(frame2,ProjWData(*data_TF_EB),
  Components(*bkgShapePdfTF_),LineColor(kRed));
  pdfTF_EB.plotOn(frame2,ProjWData(*data_TF_EB));
  frame2->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


  TString cname = TString("fit_canvas_") + h_TF_EE.GetName();
  c = new TCanvas(cname,cname,500,500);
  sample.setLabel("EE_fail");
  RooPlot* frame2 = Mass.frame();
  data_TF_EE->plotOn(frame2,RooFit::DataError(errorType));
  pdfTF_EE.plotOn(frame2,ProjWData(*data_TF_EE),
  Components(*bkgShapePdfTF_),LineColor(kRed));
  pdfTF_EE.plotOn(frame2,ProjWData(*data_TF_EE));
  frame2->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


  //    if(data) delete data;
  //    if(c) delete c;
}





// // ***** Function to return the signal Pdf *** //
 void makeSignalPdf() {

  // Tag+Fail selection pdf
  Zeelineshape_file =  new TFile("Zlineshapes.root", "READ");
  th1_TF_BB = (TH1D*) Zeelineshape_file->Get("fail_BB");
  th1_TF_EB = (TH1D*) Zeelineshape_file->Get("fail_BE");
  th1_TF_EE = (TH1D*) Zeelineshape_file->Get("fail_EE");

  rdh_TF_BB = new RooDataHist("rdh_TF_BB","", *rooMass_, th1_TF_BB);
  rdh_TF_EB = new RooDataHist("rdh_TF_EB","", *rooMass_, th1_TF_EB);
  rdh_TF_EE = new RooDataHist("rdh_TF_EE","", *rooMass_, th1_TF_EE);

  signalShapePdfTF_BB_ = new RooHistPdf("signalShapePdfTF_BB", "", RooArgSet(*rooMass_), *rdh_TF_BB);
  signalShapePdfTF_EB_ = new RooHistPdf("signalShapePdfTF_EB", "", RooArgSet(*rooMass_), *rdh_TF_EB);
  signalShapePdfTF_EE_ = new RooHistPdf("signalShapePdfTF_EE", "", RooArgSet(*rooMass_), *rdh_TF_EE);

}




// ***** Function to return the background Pdf **** //
void makeBkgPdf()
{  
  // Background PDF variables
   bkgGammaFailTF_ = new RooRealVar("bkgGammaFailTF","bkgGammaFailTF",-2., -20., 20.);
   bkgShapePdfTF_ = new RooExponential("bkgShapePdfTF","bkgShapePdfTF",*rooMass_, *bkgGammaFailTF_);

}
