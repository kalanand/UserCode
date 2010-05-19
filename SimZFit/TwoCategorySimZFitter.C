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
 *    BB and BE+EE combinations
 *
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 ********************************************************************/

//// Following are the 3 irreducible free parameters of the fit:
////        Z cross section and single electron reco efficiency: eff_B, eff_E.
////  Additionally, the following 3 nuisance parameters are floating:
////        nBkgTF_BB,  nBkgTF_Endcap, bkgGamma.


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
RooAbsPdf* signalShapePdfTF_;
RooAbsPdf *bkgShapePdfTF_;

// Private global variables needed to define PDFs
RooRealVar *bkgGammaFailTF_;
TFile* Zeelineshape_file;
TH1D* th1_TF;
RooDataHist* rdh_TF;
TCanvas *c;




void TwoCategorySimZFitter( TH1& h_TT, TH1& h_TF, double intLumi)
{
  // The fit variable - lepton invariant mass
  rooMass_ = new RooRealVar("Mass","m_{ee}",60.0, 120.0, "GeV/c^{2}");
  rooMass_->setBins(120.0);
  RooRealVar Mass = *rooMass_;

  // Make the category variable that defines the two fits,
  // namely whether the probe passes or fails the eff criteria.
  RooCategory sample("sample","") ;
  sample.defineType("TT", 1) ;
  sample.defineType("TF", 2) ; 


  gROOT->cd();

  ///////// convert Histograms into RooDataHists
  RooDataHist* data_TT = new RooDataHist("data_TT","data_TT",
					  RooArgList(Mass), &h_TT);
  RooDataHist* data_TF = new RooDataHist("data_TF","data_TF",
					  RooArgList(Mass), &h_TF);

  RooDataHist* data = new RooDataHist( "fitData","fitData",
				       RooArgList(Mass),Index(sample),
				       Import("TT",*data_TT), Import("TF",*data_TF) ); 

  data->get()->Print();
  cout << "Made datahist" << endl;


  // ********** Construct signal & bkg shape PDF ********** //
  // Tag+Tag selection pdf
  RooDataHist* rdh_TT = new RooDataHist("rdh_TT","", *rooMass_, &h_TT);
  signalShapePdfTT_ = new RooHistPdf("signalShapePdfTT", "",
                          RooArgSet(*rooMass_), *rdh_TT);

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
  RooRealVar nBkgTF("nBkgTF","nBkgTF",10.,-10.,1000.);
 
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

  formula = "lumi*xsec*(acc_BB*eff_B*(1.0-eff_B)+acc_EB*(0.5*eff_B+0.5*eff_E-eff_B*eff_E)+(acc_EE*eff_E*(1.0-eff_E)))";
  args = new RooArgList(lumi,xsec,acc_BB,acc_EB,acc_EE,eff_B,eff_E);
  RooFormulaVar nSigTF("nSigTF",formula, *args);
  delete args;

  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

   RooArgList componentsTF(*signalShapePdfTF_,*bkgShapePdfTF_);
   RooArgList yieldsTF(nSigTF, nBkgTF );	  

   RooExtendPdf pdfTT("pdfTT","extended sum pdf", *signalShapePdfTT_, nSigTT);
   RooAddPdf pdfTF("pdfTF","extended sum pdf",componentsTF, yieldsTF);


   // The total simultaneous fit ...
   RooSimultaneous totalPdf("totalPdf","totalPdf", sample);
   totalPdf.addPdf(pdfTT,"TT");
   totalPdf.Print();
   totalPdf.addPdf(pdfTF,"TF");
   totalPdf.Print();


  // ********* Do the Actual Fit ********** //  
   RooFitResult *fitResult = totalPdf.fitTo(*data, Save(true), 
					    Extended(true),PrintEvalErrors(-1),Warnings(false) );
  fitResult->Print("v");

   ///// generate toy MC ----> need for events with integer weight in each bin
 
  RooDataSet *toy1 = pdfTT.generate(RooArgSet(Mass), (int) (h_TT.Integral()) ) ;
  RooDataSet *toy2 = pdfTF.generate(RooArgSet(Mass), (int) (h_TF.Integral()) ) ;

  RooDataHist* toydata1 = new RooDataHist("toydata1","binned toy data",
  RooArgSet(*rooMass_),*toy1) ;
  RooDataHist* toydata2 = new RooDataHist("toydata2","binned toy data",
  RooArgSet(*rooMass_),*toy2) ;

  RooDataHist* toydata = new RooDataHist( "toydata","toyata",
				       RooArgList(Mass),Index(sample),
				       Import("TT",*toydata1), Import("TF",*toydata2) ); 
  fitResult = totalPdf.fitTo(*toydata, Save(true), 
  Extended(true),PrintEvalErrors(-1),Warnings(false) );

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


  TString cname = Form("Zmass_TT_%dnb_TwoCatFit", (int)(1000*intLumi) );
  c = new TCanvas(cname,cname,500,500);
  RooPlot* frame1 = Mass.frame();
  // data_TT->plotOn(frame1,RooFit::DataError(errorType));
  toydata1->plotOn(frame1,RooFit::DataError(errorType));
  // pdfTT.plotOn(frame1,ProjWData(*data_TT));
  pdfTT.plotOn(frame1,ProjWData(*toydata1));
  frame1->SetMinimum(0);
  frame1->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


  TString cname = Form("Zmass_TF_%dnb_TwoCatFit", (int)(1000*intLumi) );
  c = new TCanvas(cname,cname,500,500);
  RooPlot* frame2 = Mass.frame();
  // data_TF->plotOn(frame2,RooFit::DataError(errorType));
  toydata2->plotOn(frame2,RooFit::DataError(errorType));
  //pdfTF.plotOn(frame2,ProjWData(*data_TF),
  //Components(*bkgShapePdfTF_),LineColor(kRed));
  //pdfTF.plotOn(frame2,ProjWData(*data_TF));
  pdfTF.plotOn(frame2,ProjWData(*toydata2),
  Components(*bkgShapePdfTF_),LineColor(kRed));
  pdfTF.plotOn(frame2,ProjWData(*toydata2));
  frame2->SetMinimum(0);
  frame2->Draw("e0");
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));


/*
  RooMCStudy toymc( totalPdf, RooArgSet(*rooMass_, sample), Binned(kTRUE), 
  FitModel(totalPdf), Extended(),FitOptions(Extended()));
  toymc.generateAndFit(1000,numEventsToGenerate);

  RooPlot* plot1 = toymc.plotPull( xsec, -4., 4., 24);
  plot1->SetTitle("");
  plot1->GetXaxis()->SetTitle("cross section pull");
  TCanvas *c2 = new TCanvas("c2","Pull distribution of cross section",500,500);
  plot1->Draw();
*/


  //    if(data) delete data;
  //    if(c) delete c;
}





// // ***** Function to return the signal Pdf *** //
 void makeSignalPdf() {

  // Tag+Fail selection pdf
  Zeelineshape_file =  new TFile("Zlineshapes.root", "READ");
  TH1* histbb = (TH1D*) Zeelineshape_file->Get("fail_BB");
  TH1* histeb = (TH1D*) Zeelineshape_file->Get("fail_BE");
  TH1* histee = (TH1D*) Zeelineshape_file->Get("fail_EE");

  th1_TF = (TH1D*) histbb->Clone("th1_TF");
  th1_TF->Add(histeb);
  th1_TF->Add(histee);

  rdh_TF = new RooDataHist("rdh_TF","", *rooMass_, th1_TF);
  signalShapePdfTF_ = new RooHistPdf("signalShapePdfTF", "", RooArgSet(*rooMass_), *rdh_TF);
}




// ***** Function to return the background Pdf **** //
void makeBkgPdf()
{  
  // Background PDF variables
   bkgGammaFailTF_ = new RooRealVar("bkgGammaFailTF","bkgGammaFailTF",-0.2, -20., 20.);
   bkgShapePdfTF_ = new RooExponential("bkgShapePdfTF","bkgShapePdfTF",*rooMass_, *bkgGammaFailTF_);
}

