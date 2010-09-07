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

RooGaussModel* resModelTT_;
RooAbsPdf* signalModelTT_;
RooGaussModel* resModelTF_;
RooAbsPdf* signalModelTF_;

// Private global variables needed to define PDFs
RooRealVar *bkgGammaFailTF_;
RooRealVar* Mean_;
RooRealVar* Width_;
RooRealVar* Resolution_;
RooRealVar* Mean2_;
RooRealVar* Width2_;
RooRealVar* Resolution2_;

RooRealVar* shiftTT_;
RooRealVar* resoTT_;
RooRealVar* shiftTF_;
RooRealVar* resoTF_;

TFile* Zeelineshape_file;
TH1D* th1_TT;
TH1D* th1_TF;
RooDataHist* rdh_TT;
RooDataHist* rdh_TF;
TCanvas *c;

RooRealVar* massShiftTT;
RooRealVar* massShiftTF;
RooFormulaVar* shiftedMassTT;
RooFormulaVar* shiftedMassTF;



void TwoCategorySimZFitter( TH1& h_TT, TH1& h_TF, 
                            double intLumi, int removeEE )
{
  // The fit variable - lepton invariant mass
  rooMass_ = new RooRealVar("Mass","m_{ee}",60.0, 120.0, "GeV/c^{2}");
  rooMass_->setBins(20.0);
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
  makeSignalPdf();
  cout << "Made signal pdf" << endl;
  makeBkgPdf();
  cout << "Made bkg pdf" << endl;

  // Now supply integrated luminosity in inverse picobarn
  // -->  we get this number from the CMS lumi group
  // https://twiki.cern.ch/twiki/bin/view/CMS/LumiWiki2010Data
  RooRealVar lumi("lumi","lumi", intLumi);


  // Now define Z production cross section variable (in pb) 
  RooRealVar xsec("xsec","xsec", 1300., 400.0, 2000.0);


  // Define efficiency variables  
  RooRealVar eff("eff","eff", 0.9, 0.5, 1.0);

  // Now define acceptance variables --> we get these numbers from MC   
//   RooRealVar acc_BB("acc_BB","acc_BB", 0.2253);
//   RooRealVar acc_EB("acc_EB","acc_EB", 0.1625);
//   RooRealVar acc_EE("acc_EE","acc_EE", 0.0479);

  double ACCEPTANCE=0.4357;
  if(removeEE==1) ACCEPTANCE= 0.3878;
  if(removeEE==2) ACCEPTANCE= 0.2253;
  RooRealVar acc("acc","acc", ACCEPTANCE);

  // Define background yield variables: they are not related to each other  
  RooRealVar nBkgTF("nBkgTF","nBkgTF",10.,-10.,100000.);
 
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
 //  Define signal yield variables.  
  // They are linked together by the total cross section:  e.g. 
  //          Nbb = sigma*L*Abb*effB

  char* formula;
  RooArgList* args;
  formula = "lumi*xsec*acc*eff*eff";
  args = new RooArgList(lumi,xsec,acc,eff);
  RooFormulaVar nSigTT("nSigTT", formula, *args);
  delete args;

  formula = "lumi*xsec*acc*eff*(1.0-eff)";
  args = new RooArgList(lumi,xsec,acc,eff);
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
					    Extended(), Minos(),
                        PrintEvalErrors(-1),Warnings(false) );
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


  TString cname = Form("Zmass_TT_%dnb", (int)(1000*intLumi) );
  c = new TCanvas(cname,cname,500,500);
  RooPlot* frame1 = Mass.frame();
  data_TT->plotOn(frame1,RooFit::DataError(errorType));
  pdfTT.plotOn(frame1,ProjWData(*data_TT));
  frame1->SetMinimum(0);
  frame1->Draw("e0");
  TPaveText *plotlabel = new TPaveText(0.23,0.87,0.43,0.92,"NDC");
   plotlabel->SetTextColor(kBlack);
   plotlabel->SetFillColor(kWhite);
   plotlabel->SetBorderSize(0);
   plotlabel->SetTextAlign(12);
   plotlabel->SetTextSize(0.03);
   plotlabel->AddText("CMS Preliminary 2010");
  TPaveText *plotlabel2 = new TPaveText(0.23,0.82,0.43,0.87,"NDC");
   plotlabel2->SetTextColor(kBlack);
   plotlabel2->SetFillColor(kWhite);
   plotlabel2->SetBorderSize(0);
   plotlabel2->SetTextAlign(12);
   plotlabel2->SetTextSize(0.03);
   plotlabel2->AddText("#sqrt{s} = 7 TeV");
  TPaveText *plotlabel3 = new TPaveText(0.23,0.75,0.43,0.80,"NDC");
   plotlabel3->SetTextColor(kBlack);
   plotlabel3->SetFillColor(kWhite);
   plotlabel3->SetBorderSize(0);
   plotlabel3->SetTextAlign(12);
   plotlabel3->SetTextSize(0.03);
  char temp[100];
  sprintf(temp, "%.1f", intLumi);
  plotlabel3->AddText((string("#int#font[12]{L}dt = ") + 
  temp + string(" pb^{ -1}")).c_str());
  TPaveText *plotlabel4 = new TPaveText(0.6,0.87,0.8,0.92,"NDC");
   plotlabel4->SetTextColor(kBlack);
   plotlabel4->SetFillColor(kWhite);
   plotlabel4->SetBorderSize(0);
   plotlabel4->SetTextAlign(12);
   plotlabel4->SetTextSize(0.03);
   double nsig = eff.getVal()*xsec.getVal()*acc.getVal()*lumi.getVal();
   double xsecFrErr = xsec.getError()/xsec.getVal();
   double effFrErr = eff.getError()/eff.getVal();
   double effxsecCorr = fitResult->correlation(eff, xsec);
   double nsigerr = sqrt(effFrErr**2 +xsecFrErr**2 + 
   2.0*effxsecCorr*xsecFrErr*effFrErr)*nsig;
   sprintf(temp, "Signal = %.2f #pm %.2f", nsig, nsigerr);
   plotlabel4->AddText(temp);
  TPaveText *plotlabel5 = new TPaveText(0.6,0.82,0.8,0.87,"NDC");
   plotlabel5->SetTextColor(kBlack);
   plotlabel5->SetFillColor(kWhite);
   plotlabel5->SetBorderSize(0);
   plotlabel5->SetTextAlign(12);
   plotlabel5->SetTextSize(0.03);
   sprintf(temp, "#epsilon = %.4f #pm %.4f", eff.getVal(), eff.getError());
   plotlabel5->AddText(temp);
  TPaveText *plotlabel6 = new TPaveText(0.6,0.77,0.8,0.82,"NDC");
   plotlabel6->SetTextColor(kBlack);
   plotlabel6->SetFillColor(kWhite);
   plotlabel6->SetBorderSize(0);
   plotlabel6->SetTextAlign(12);
   plotlabel6->SetTextSize(0.03);
   sprintf(temp, "#sigma = %.1f #pm %.1f pb", xsec.getVal(), xsec.getError());
   plotlabel6->AddText(temp);

  plotlabel->Draw();
  plotlabel2->Draw();
  plotlabel3->Draw();
  plotlabel4->Draw();
  plotlabel5->Draw();
  plotlabel6->Draw();
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));
  c->SaveAs( cname + TString(".png"));
  c->SaveAs( cname + TString(".C"));


  TString cname = Form("Zmass_TF_%dnb", (int)(1000*intLumi) );
  c = new TCanvas(cname,cname,500,500);
  RooPlot* frame2 = Mass.frame();
  data_TF->plotOn(frame2,RooFit::DataError(errorType));
  pdfTF.plotOn(frame2,ProjWData(*data_TF),
  Components(*bkgShapePdfTF_),LineColor(kRed));
  pdfTF.plotOn(frame2,ProjWData(*data_TF));
  frame2->SetMinimum(0);
  frame2->Draw("e0");
  frame2->GetYaxis()->SetNdivisions(505);
  TPaveText *plotlabel = new TPaveText(0.23,0.87,0.43,0.92,"NDC");
   plotlabel->SetTextColor(kBlack);
   plotlabel->SetFillColor(kWhite);
   plotlabel->SetBorderSize(0);
   plotlabel->SetTextAlign(12);
   plotlabel->SetTextSize(0.03);
   plotlabel->AddText("CMS Preliminary 2010");
  TPaveText *plotlabel2 = new TPaveText(0.23,0.82,0.43,0.87,"NDC");
   plotlabel2->SetTextColor(kBlack);
   plotlabel2->SetFillColor(kWhite);
   plotlabel2->SetBorderSize(0);
   plotlabel2->SetTextAlign(12);
   plotlabel2->SetTextSize(0.03);
   plotlabel2->AddText("#sqrt{s} = 7 TeV");
  TPaveText *plotlabel3 = new TPaveText(0.23,0.75,0.43,0.80,"NDC");
   plotlabel3->SetTextColor(kBlack);
   plotlabel3->SetFillColor(kWhite);
   plotlabel3->SetBorderSize(0);
   plotlabel3->SetTextAlign(12);
   plotlabel3->SetTextSize(0.03);
  char temp[100];
  sprintf(temp, "%.1f", intLumi);
  plotlabel3->AddText((string("#int#font[12]{L}dt = ") + 
  temp + string(" pb^{ -1}")).c_str());
  TPaveText *plotlabel4 = new TPaveText(0.6,0.87,0.8,0.92,"NDC");
   plotlabel4->SetTextColor(kBlack);
   plotlabel4->SetFillColor(kWhite);
   plotlabel4->SetBorderSize(0);
   plotlabel4->SetTextAlign(12);
   plotlabel4->SetTextSize(0.03);
   double nsig = (1.0-eff.getVal())*xsec.getVal()*acc.getVal()*lumi.getVal();
   double xsecFrErr = xsec.getError()/xsec.getVal();
   double effFrErr = eff.getError()/(1.0-eff.getVal());
   double effxsecCorr = fitResult->correlation(eff, xsec);
   double nsigerr = sqrt(effFrErr**2 +xsecFrErr**2 + 
   2.0*effxsecCorr*xsecFrErr*effFrErr)*nsig;
   sprintf(temp, "Signal = %.2f #pm %.2f", nsig, nsigerr);
   plotlabel4->AddText(temp);
  TPaveText *plotlabel5 = new TPaveText(0.6,0.82,0.8,0.87,"NDC");
   plotlabel5->SetTextColor(kBlack);
   plotlabel5->SetFillColor(kWhite);
   plotlabel5->SetBorderSize(0);
   plotlabel5->SetTextAlign(12);
   plotlabel5->SetTextSize(0.03);
   sprintf(temp, "Bkg = %.2f #pm %.2f", nBkgTF.getVal(), nBkgTF.getError());
   plotlabel5->AddText(temp);
  TPaveText *plotlabel6 = new TPaveText(0.6,0.77,0.8,0.82,"NDC");
   plotlabel6->SetTextColor(kBlack);
   plotlabel6->SetFillColor(kWhite);
   plotlabel6->SetBorderSize(0);
   plotlabel6->SetTextAlign(12);
   plotlabel6->SetTextSize(0.03);
   sprintf(temp, "#epsilon = %.4f #pm %.4f", eff.getVal(), eff.getError());
   plotlabel6->AddText(temp);

  plotlabel->Draw();
  plotlabel2->Draw();
  plotlabel3->Draw();
  plotlabel4->Draw();
  plotlabel5->Draw();
  plotlabel6->Draw();

  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));
  c->SaveAs( cname + TString(".png"));
  c->SaveAs( cname + TString(".C"));

/*
  RooMCStudy toymc( totalPdf, RooArgSet(Mass, sample), Binned(kTRUE), 
  FitModel(totalPdf), Extended(),FitOptions(Extended(), Minos()));
  int numEventsToGenerate = (int)(h_TT.Integral()+h_TF.Integral());
  toymc.generateAndFit(1000,numEventsToGenerate);

  RooPlot* plot1 = toymc.plotPull( xsec, -4., 4., 24);
  plot1->SetTitle("");
  plot1->GetXaxis()->SetTitle("cross section pull");
  TString cname = Form("pull_crosssection_%dnb", (int)(1000*intLumi));
  TCanvas *c2 = new TCanvas(cname,"Pull distribution of cross section",500,500);
  plot1->Draw();
  c2->SaveAs( cname + TString(".eps"));
  c2->SaveAs( cname + TString(".gif"));
  c2->SaveAs( cname + TString(".root"));




  RooPlot* plot2 = toymc.plotError( eff, 0., 0.2, 20);
  plot2->SetTitle("");
  plot2->GetXaxis()->SetTitle("Uncertainty in efficiency");
  TString cname = Form("error_efficiency_%dnb", (int)(1000*intLumi));
  TCanvas *c3 = new TCanvas(cname,"Uncertainty in efficiency",500,500);
  plot2->Draw();
  c3->SaveAs( cname + TString(".eps"));
  c3->SaveAs( cname + TString(".gif"));
  c3->SaveAs( cname + TString(".root"));



  RooPlot* plot3 = toymc.plotError( xsec, 0., 650., 65);
  plot3->SetTitle("");
  plot3->GetXaxis()->SetTitle("Uncertainty in cross section (pb)");
  TString cname = Form("error_crosssection_%dnb", (int)(1000*intLumi));
  TCanvas *c4 = new TCanvas(cname,"Uncertainty in cross section",500,500);
  plot3->Draw();
  c4->SaveAs( cname + TString(".eps"));
  c4->SaveAs( cname + TString(".gif"));
  c4->SaveAs( cname + TString(".root"));

*/

  //    if(data) delete data;
  //    if(c) delete c;
}





// // ***** Function to return the signal Pdf *** //
 void makeSignalPdf() {




 // Tag+Tag selection pdf
  Zeelineshape_file =  new TFile("Zlineshapes.root", "READ");

  // Tag+Tag selection pdf
  TH1* histbbpass = (TH1D*) Zeelineshape_file->Get("pass_BB");
  TH1* histebpass = (TH1D*) Zeelineshape_file->Get("pass_BE");
  TH1* histeepass = (TH1D*) Zeelineshape_file->Get("pass_EE");

  th1_TT = (TH1D*) histbbpass->Clone("th1_TT");
  th1_TT->Add(histebpass);
  th1_TT->Add(histeepass);

  massShiftTT = new RooRealVar("massShiftTT","",-1., -10., 10.);
  massShiftTF = new RooRealVar("massShiftTF","",-3., -10., 10.);

  shiftedMassTT = new RooFormulaVar("shiftedMassTT", 
  "@0+@1", RooArgSet( *rooMass_, *massShiftTT) );
  shiftedMassTF = new RooFormulaVar("shiftedMassTF", 
  "@0+@1", RooArgSet( *rooMass_, *massShiftTF) );


  rdh_TT = new RooDataHist("rdh_TT","", *rooMass_, th1_TT);

//   signalShapePdfTT_ = new RooHistPdf("signalShapePdfTT", "",
//                           RooArgSet(*rooMass_), *rdh_TT);

  signalModelTT_ = new RooHistPdf("signalModelTT", "",
                   RooArgSet(*shiftedMassTT), RooArgList(*rooMass_), *rdh_TT);

  shiftTT_ = new RooRealVar("shiftTT","shiftTT",0.0);
  resoTT_  = new RooRealVar("resoTT","resoTT",2.36, 0.5, 5.);
  resModelTT_ = new RooGaussModel("resModelTT","gaussian resolution model", 
                *rooMass_, *massShiftTT, *resoTT_);

  signalShapePdfTT_  = new RooFFTConvPdf("sigModel","final signal shape", 
                       *shiftedMassTT, *rooMass_, *signalModelTT_, *resModelTT_);


 // Tag+Fail selection pdf
  TH1* histbb = (TH1D*) Zeelineshape_file->Get("fail_BB");
  TH1* histeb = (TH1D*) Zeelineshape_file->Get("fail_BE");
  TH1* histee = (TH1D*) Zeelineshape_file->Get("fail_EE");

  th1_TF = (TH1D*) histbb->Clone("th1_TF");
  th1_TF->Add(histeb);
  th1_TF->Add(histee);

  rdh_TF = new RooDataHist("rdh_TF","", *rooMass_, th1_TF);
//   signalShapePdfTF_ = new RooHistPdf("signalShapePdfTF", "", RooArgSet(*rooMass_), *rdh_TF);

  signalModelTF_ = new RooHistPdf("signalModelTF", "", 
                    RooArgSet(*shiftedMassTF), RooArgList(*rooMass_), *rdh_TF);


  shiftTF_ = new RooRealVar("shiftTF","shiftTF",0.0);
  resoTF_  = new RooRealVar("resoTF","resoTF",0.5, 0.01, 5.);
  resModelTF_ = new RooGaussModel("resModelTF","gaussian resolution model", 
                *rooMass_, *massShiftTF, *resoTF_);

  signalShapePdfTF_  = new RooFFTConvPdf("sigModel","final signal shape", 
                       *shiftedMassTF, *rooMass_, *signalModelTF_, *resModelTF_);

/*
    Mean_   = new RooRealVar("Mean","Mean", 91.2, 85., 95.);
    Resolution_  = new RooRealVar("Resolution","Resolution", 4.0, 1.0, 10.0);
      // Voigtian
    signalShapePdfTT_ = new RooVoigtian("signalShapePdfTT", "", 
    *rooMass_, *Mean_, *Width_, *Resolution_);


    Mean2_   = new RooRealVar("Mean2","Mean2", 91.2, 80., 95.);
    Width_  = new RooRealVar("Width","Width", 2.5);
    Resolution2_  = new RooRealVar("Resolution2","Resolution2", 5.0, 1.0, 10.0);
  signalShapePdfTF_ = new RooVoigtian("signalShapePdfTF", "", 
  *rooMass_, *Mean2_, *Width_, *Resolution2_);
*/
}




// ***** Function to return the background Pdf **** //
void makeBkgPdf()
{  
  // Background PDF variables
   bkgGammaFailTF_ = new RooRealVar("bkgGammaFailTF","bkgGammaFailTF",-0.008, -10., 0.);
   bkgShapePdfTF_ = new RooExponential("bkgShapePdfTF","bkgShapePdfTF",*rooMass_, *bkgGammaFailTF_);
}

