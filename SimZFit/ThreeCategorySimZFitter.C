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
////        Z cross section and single electron efficiency: eff_B, eff_E.
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
RooAbsPdf* signalShapePdfTF_BB_;
RooAbsPdf* signalShapePdfTF_End_;
RooAbsPdf *bkgShapePdfTF_BB_;
RooAbsPdf *bkgShapePdfTF_End_;

RooGaussModel* resModelTT_;
RooAbsPdf* signalModelTT_BB_;
RooAbsPdf* signalModelTT_End_;
RooGaussModel* resModelTF_BB_;
RooGaussModel* resModelTF_End_;

RooAbsPdf* signalModelTF_BB_;
RooAbsPdf* signalModelTF_End_;

// Private global variables needed to define PDFs
RooRealVar *bkgGammaFailTF_BB_;
RooRealVar *bkgGammaFailTF_End_;
RooRealVar* Mean_;
RooRealVar* Width_;
RooRealVar* Resolution_;
RooRealVar* Mean2_;
RooRealVar* Width2_;
RooRealVar* Resolution2_;

//RooRealVar* shiftTT_;
RooRealVar* resoTT_;
//RooRealVar* shiftTF_BB_;
RooRealVar* resoTF_BB_;
//RooRealVar* shiftTF_End_;
RooRealVar* resoTF_End_;


TFile* Zeelineshape_file;
TH1D* th1_TT;
TH1D* th1_TF;
TH1D* histbb;
RooDataHist* rdh_TT;
RooDataHist* rdh_TF_BB;
RooDataHist* rdh_TF_End;
TCanvas *c;

RooRealVar* massShiftTT;
RooRealVar* massShiftTF_BB;
RooRealVar* massShiftTF_End;

RooFormulaVar* shiftedMassTT;
RooFormulaVar* shiftedMassTF_BB;
RooFormulaVar* shiftedMassTF_End;



void ThreeCategorySimZFitter( TH1& h_TT, TH1& h_TF_BB, TH1& h_TF_End, 
                            double intLumi)
{
  // The fit variable - lepton invariant mass
  rooMass_ = new RooRealVar("Mass","m_{ee}",60.0, 120.0, "GeV/c^{2}");
  rooMass_->setBins(120.0);
  RooRealVar Mass = *rooMass_;

  // Make the category variable that defines the two fits,
  // namely whether the probe passes or fails the eff criteria.
  RooCategory sample("sample","") ;
  sample.defineType("BB_pass", 1) ;
  sample.defineType("BB_fail", 2) ; 
  sample.defineType("End_fail", 3) ; 


  gROOT->cd();

  ///////// convert Histograms into RooDataHists
  RooDataHist* data_TT = new RooDataHist("data_TT","data_TT",
					  RooArgList(Mass), &h_TT);
  RooDataHist* data_TF_BB = new RooDataHist("data_TF_BB","data_TF_BB",
					  RooArgList(Mass), &h_TF_BB);
  RooDataHist* data_TF_End = new RooDataHist("data_TF_End","data_TF_End",
					  RooArgList(Mass), &h_TF_End);

  RooDataHist* data = new RooDataHist( "fitData","fitData",
				       RooArgList(Mass),Index(sample),
				       Import("BB_pass",*data_TT),
				       Import("BB_fail",*data_TF_BB),
				       Import("End_fail",*data_TF_End) ); 

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
  RooRealVar xsec("xsec","xsec", 912.187, 200.0, 2000.0);


  // Define efficiency variables  
  RooRealVar eff_B("eff_B","eff_B", 0.966370,  0.0, 1.2);
  RooRealVar eff_E("eff_E","eff_E", 0.851405, 0.0, 1.2);



  // Now define acceptance variables --> we get these numbers from MC   
  RooRealVar acc_BB("acc_BB","acc_BB", 0.2253);
  RooRealVar acc_EB("acc_EB","acc_EB", 0.1625);
  RooRealVar acc_EE("acc_EE","acc_EE", 0.0479);


  // Define background yield variables: they are not related to each other  
  RooRealVar nBkgTF_BB("nBkgTF_BB","nBkgTF_BB", 10.,-10.,1000.);
  RooRealVar nBkgTF_End("nBkgTF_End","nBkgTF_End", 23.,-10.,1000.);

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
 //  Define signal yield variables.  
  // They are linked together by the total cross section:  e.g. 
  //          Nbb = sigma*L*Abb*effB

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

  formula = "lumi*xsec*(acc_EB*(0.5*eff_B+0.5*eff_E-eff_B*eff_E)+(acc_EE*eff_E*(1.0-eff_E)))";
  args = new RooArgList(lumi,xsec,acc_EB,acc_EE,eff_B,eff_E);
  RooFormulaVar nSigTF_End("nSigTF_EB",formula, *args);
  delete args;

  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

   RooArgList componentsTF_BB(*signalShapePdfTF_BB_,*bkgShapePdfTF_BB_);
   RooArgList componentsTF_End(*signalShapePdfTF_End_,*bkgShapePdfTF_End_);

   RooArgList yieldsTF_BB(nSigTF_BB, nBkgTF_BB );	  
   RooArgList yieldsTF_End(nSigTF_End, nBkgTF_End);	  

   RooExtendPdf pdfTT("pdfTT","extended sum pdf", *signalShapePdfTT_, nSigTT);
   RooAddPdf pdfTF_BB("pdfTF_BB","extended sum pdf",componentsTF_BB, yieldsTF_BB);
   RooAddPdf pdfTF_End("pdfTF_End","extended sum pdf",componentsTF_End, yieldsTF_End);


   // The total simultaneous fit ...
   RooSimultaneous totalPdf("totalPdf","totalPdf", sample);
   totalPdf.addPdf(pdfTT,"BB_pass");
   totalPdf.Print();
   totalPdf.addPdf(pdfTF_BB,"BB_fail");
   totalPdf.addPdf(pdfTF_End,"End_fail");
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

   double efficiency = acc_BB.getVal()*eff_B.getVal()**2 + 
      acc_EB.getVal()*eff_B.getVal()*eff_E.getVal() + 
      acc_EE.getVal()*eff_B.getVal()**2; 
   double nsig = lumi.getVal()*xsec.getVal()*efficiency;
   double effBxsecCorr = fitResult->correlation(eff_B, xsec);
   double effExsecCorr = fitResult->correlation(eff_E, xsec);
   double effBECorr = fitResult->correlation(eff_B, eff_E);
   double nsigerr = GetNerrTT(eff_B.getVal(), eff_B.getError(), 
                              eff_E.getVal(), eff_E.getError(), 
                              xsec.getVal(), xsec.getError(), 
                              effBxsecCorr, effExsecCorr, 
                              effBECorr, intLumi);
   sprintf(temp, "Signal = %.1f #pm %.1f", nsig, nsigerr);
   plotlabel4->AddText(temp);
   TPaveText *plotlabel41 = new TPaveText(0.6,0.82,0.8,0.87,"NDC");
   plotlabel41->SetTextColor(kBlack);
   plotlabel41->SetFillColor(kWhite);
   plotlabel41->SetBorderSize(0);
   plotlabel41->SetTextAlign(12);
   plotlabel41->SetTextSize(0.03);
   sprintf(temp, "Shift = %.2f #pm %.2f", massShiftTT->getVal(), massShiftTT->getError() );
   plotlabel41->AddText(temp);
   TPaveText *plotlabel5 = new TPaveText(0.6,0.77,0.8,0.82,"NDC");
   plotlabel5->SetTextColor(kBlack);
   plotlabel5->SetFillColor(kWhite);
   plotlabel5->SetBorderSize(0);
   plotlabel5->SetTextAlign(12);
   plotlabel5->SetTextSize(0.03);
   sprintf(temp, "#epsilon_{EB} = %.3f #pm %.3f", eff_B.getVal(), eff_B.getError() );
   plotlabel5->AddText(temp);
  TPaveText *plotlabel6 = new TPaveText(0.6,0.72,0.8,0.77,"NDC");
   plotlabel6->SetTextColor(kBlack);
   plotlabel6->SetFillColor(kWhite);
   plotlabel6->SetBorderSize(0);
   plotlabel6->SetTextAlign(12);
   plotlabel6->SetTextSize(0.03);
   sprintf(temp, "#epsilon_{EE} = %.3f #pm %.3f", eff_E.getVal(), eff_E.getError() );
   plotlabel6->AddText(temp);
  TPaveText *plotlabel7 = new TPaveText(0.6,0.67,0.8,0.72,"NDC");
   plotlabel7->SetTextColor(kBlack);
   plotlabel7->SetFillColor(kWhite);
   plotlabel7->SetBorderSize(0);
   plotlabel7->SetTextAlign(12);
   plotlabel7->SetTextSize(0.03);
   sprintf(temp, "#sigma = %.1f #pm %.1f pb", xsec.getVal(), xsec.getError());
   plotlabel7->AddText(temp);
  plotlabel->Draw();
  plotlabel2->Draw();
  plotlabel3->Draw();
  plotlabel4->Draw();
  plotlabel41->Draw();
  plotlabel5->Draw();
  plotlabel6->Draw();
  plotlabel7->Draw();
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));
  c->SaveAs( cname + TString(".png"));
  c->SaveAs( cname + TString(".C"));


  TString cname = Form("Zmass_TF_BB%dnb", (int)(1000*intLumi) );
  c = new TCanvas(cname,cname,500,500);
  RooPlot* frame2 = Mass.frame();
  data_TF_BB->plotOn(frame2,RooFit::DataError(errorType));
  pdfTF_BB.plotOn(frame2,ProjWData(*data_TF_BB),
  Components(*bkgShapePdfTF_BB_),LineColor(kRed));
  pdfTF_BB.plotOn(frame2,ProjWData(*data_TF_BB),
  Components(*signalShapePdfTF_BB_),DrawOption("LF"),FillStyle(1001),FillColor(kOrange),VLines());
  pdfTF_BB.plotOn(frame2,ProjWData(*data_TF_BB));
  data_TF_BB->plotOn(frame2,RooFit::DataError(errorType));
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
   double nsig = eff_B.getVal()*(1.0-eff_B.getVal())*
   xsec.getVal()*acc_BB.getVal()*lumi.getVal();
   double xsecFrErr = xsec.getError()/xsec.getVal();
   double effFrErr = eff_B.getError()*(1.-2.*eff_B.getVal()) / 
      (1.-eff_B.getVal())/eff_B.getVal();
   double effxsecCorr = fitResult->correlation(eff_B, xsec);
   double nsigerr = sqrt(effFrErr**2 +xsecFrErr**2 + 
   2.0*effxsecCorr*xsecFrErr*effFrErr)*nsig;
   sprintf(temp, "Signal = %.2f #pm %.2f", nsig, nsigerr);
   plotlabel4->AddText(temp);
   TPaveText *plotlabel41 = new TPaveText(0.6,0.82,0.8,0.87,"NDC");
   plotlabel41->SetTextColor(kBlack);
   plotlabel41->SetFillColor(kWhite);
   plotlabel41->SetBorderSize(0);
   plotlabel41->SetTextAlign(12);
   plotlabel41->SetTextSize(0.03);
   sprintf(temp, "Shift = %.2f #pm %.2f", massShiftTF_BB->getVal(), massShiftTF_BB->getError() );
   plotlabel41->AddText(temp);
  TPaveText *plotlabel5 = new TPaveText(0.6,0.77,0.8,0.82,"NDC");
   plotlabel5->SetTextColor(kBlack);
   plotlabel5->SetFillColor(kWhite);
   plotlabel5->SetBorderSize(0);
   plotlabel5->SetTextAlign(12);
   plotlabel5->SetTextSize(0.03);
   sprintf(temp, "Bkg = %.2f #pm %.2f", nBkgTF_BB.getVal(), nBkgTF_BB.getError());
   plotlabel5->AddText(temp);
  TPaveText *plotlabel6 = new TPaveText(0.6,0.72,0.8,0.77,"NDC");
   plotlabel6->SetTextColor(kBlack);
   plotlabel6->SetFillColor(kWhite);
   plotlabel6->SetBorderSize(0);
   plotlabel6->SetTextAlign(12);
   plotlabel6->SetTextSize(0.03);
   sprintf(temp, "#epsilon_{EB} = %.3f #pm %.3f", eff_B.getVal(), eff_B.getError() );
   plotlabel6->AddText(temp);
  TPaveText *plotlabel7 = new TPaveText(0.6,0.67,0.8,0.72,"NDC");
   plotlabel7->SetTextColor(kBlack);
   plotlabel7->SetFillColor(kWhite);
   plotlabel7->SetBorderSize(0);
   plotlabel7->SetTextAlign(12);
   plotlabel7->SetTextSize(0.03);
   sprintf(temp, "#epsilon_{EE} = %.3f #pm %.3f", eff_E.getVal(), eff_E.getError() );
   plotlabel7->AddText(temp);

  plotlabel->Draw();
  plotlabel2->Draw();
  plotlabel3->Draw();
  plotlabel4->Draw();
  plotlabel41->Draw();
  plotlabel5->Draw();
  plotlabel6->Draw();
  plotlabel7->Draw();

  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));
  c->SaveAs( cname + TString(".png"));
  c->SaveAs( cname + TString(".C"));


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

  TString cname = Form("Zmass_TF_EndCaps%dnb", (int)(1000*intLumi) );
  c = new TCanvas(cname,cname,500,500);
  RooPlot* frame3 = Mass.frame();
  data_TF_End->plotOn(frame3,RooFit::DataError(errorType));
  pdfTF_End.plotOn(frame3,ProjWData(*data_TF_End),
  Components(*bkgShapePdfTF_End_),LineColor(kRed));
  pdfTF_End.plotOn(frame3,ProjWData(*data_TF_End),
  Components(*signalShapePdfTF_End_),DrawOption("LF"),FillStyle(1001),FillColor(kOrange),VLines());
  pdfTF_End.plotOn(frame3,ProjWData(*data_TF_End));
  data_TF_End->plotOn(frame3,RooFit::DataError(errorType));
  frame3->SetMinimum(0);
  frame3->Draw("e0");
  frame3->GetYaxis()->SetNdivisions(505);
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
   nsig = GetNTFEndcaps(eff_B.getVal(), eff_E.getVal(), xsec.getVal(), intLumi);
   nsigerr = GetNerrTFEndcaps(eff_B.getVal(), eff_B.getError(), eff_E.getVal(), 
                              eff_E.getError(), xsec.getVal(), xsec.getError(), 
                              effBxsecCorr, effExsecCorr, effBECorr, intLumi);
   sprintf(temp, "Signal = %.2f #pm %.2f", nsig, nsigerr);
   plotlabel4->AddText(temp);
   TPaveText *plotlabel41 = new TPaveText(0.6,0.82,0.8,0.87,"NDC");
   plotlabel41->SetTextColor(kBlack);
   plotlabel41->SetFillColor(kWhite);
   plotlabel41->SetBorderSize(0);
   plotlabel41->SetTextAlign(12);
   plotlabel41->SetTextSize(0.03);
   sprintf(temp, "Shift = %.2f #pm %.2f", massShiftTF_End->getVal(), massShiftTF_End->getError() );
   plotlabel41->AddText(temp);
  TPaveText *plotlabel5 = new TPaveText(0.6,0.77,0.8,0.82,"NDC");
   plotlabel5->SetTextColor(kBlack);
   plotlabel5->SetFillColor(kWhite);
   plotlabel5->SetBorderSize(0);
   plotlabel5->SetTextAlign(12);
   plotlabel5->SetTextSize(0.03);
   sprintf(temp, "Bkg = %.2f #pm %.2f", nBkgTF_End.getVal(), nBkgTF_End.getError());
   plotlabel5->AddText(temp);
  TPaveText *plotlabel6 = new TPaveText(0.6,0.72,0.8,0.77,"NDC");
   plotlabel6->SetTextColor(kBlack);
   plotlabel6->SetFillColor(kWhite);
   plotlabel6->SetBorderSize(0);
   plotlabel6->SetTextAlign(12);
   plotlabel6->SetTextSize(0.03);
   sprintf(temp, "#epsilon_{EB} = %.3f #pm %.3f", eff_B.getVal(), eff_B.getError() );
   plotlabel6->AddText(temp);
  TPaveText *plotlabel7 = new TPaveText(0.6,0.67,0.8,0.72,"NDC");
   plotlabel7->SetTextColor(kBlack);
   plotlabel7->SetFillColor(kWhite);
   plotlabel7->SetBorderSize(0);
   plotlabel7->SetTextAlign(12);
   plotlabel7->SetTextSize(0.03);
   sprintf(temp, "#epsilon_{EE} = %.3f #pm %.3f", eff_E.getVal(), eff_E.getError() );
   plotlabel7->AddText(temp);

  plotlabel->Draw();
  plotlabel2->Draw();
  plotlabel3->Draw();
  plotlabel4->Draw();
  plotlabel41->Draw();
  plotlabel5->Draw();
  plotlabel6->Draw();
  plotlabel7->Draw();

  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));
  c->SaveAs( cname + TString(".png"));
  c->SaveAs( cname + TString(".C"));




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

  massShiftTT = new RooRealVar("massShiftTT","",-0.54483.);//, -3., 3.);
  massShiftTF_BB = new RooRealVar("massShiftTF_BB","",-1.12);//, -5., 5.);
  massShiftTF_End = new RooRealVar("massShiftTF_End","",-1.4332);//, -5., 5.);


  shiftedMassTT = new RooFormulaVar("shiftedMassTT", 
  "@0-@1", RooArgSet( *rooMass_, *massShiftTT) );
  shiftedMassTF_BB = new RooFormulaVar("shiftedMassTF_BB", 
  "@0-@1", RooArgSet( *rooMass_, *massShiftTF_BB) );
  shiftedMassTF_End = new RooFormulaVar("shiftedMassTF_BB", 
  "@0-@1", RooArgSet( *rooMass_, *massShiftTF_End) );


  rdh_TT = new RooDataHist("rdh_TT","", *rooMass_, th1_TT);
  signalModelTT_ = new RooHistPdf("signalModelTT", "",
                   RooArgSet(*shiftedMassTT), RooArgList(*rooMass_), *rdh_TT);
  resoTT_  = new RooRealVar("resoTT","resoTT",2.15748, 0.0, 5.);
  resModelTT_ = new RooGaussModel("resModelTT","gaussian resolution model", 
                *rooMass_, *massShiftTT, *resoTT_);
  signalShapePdfTT_  = new RooFFTConvPdf("sigModel","final signal shape", 
                       *shiftedMassTT, *rooMass_, *signalModelTT_, *resModelTT_);


 // Tag+Fail selection pdf
  TH1* histbb = (TH1D*) Zeelineshape_file->Get("fail_BB");
  TH1* histeb = (TH1D*) Zeelineshape_file->Get("fail_BE");
  TH1* histee = (TH1D*) Zeelineshape_file->Get("fail_EE");

  th1_TF = (TH1D*) histeb->Clone("th1_TF");
  th1_TF->Add(histee);

  rdh_TF_BB_ = new RooDataHist("rdh_TF_BB","", *rooMass_, histbb);
  signalModelTF_BB_ = new RooHistPdf("signalModelTF_BB", "", 
                    RooArgSet(*shiftedMassTF_BB), RooArgList(*rooMass_), *rdh_TF_BB_);
  resoTF_BB_  = new RooRealVar("resoTF_BB","resoTF_BB",0.075291, 0.0, 5.);
  resModelTF_BB_ = new RooGaussModel("resModelTF_BB","gaussian resolution model", 
                *rooMass_, *massShiftTF_BB, *resoTF_BB_);
  signalShapePdfTF_BB_  = new RooFFTConvPdf("signalShapePdfTF_BB","final signal shape", 
                       *shiftedMassTF_BB, *rooMass_, *signalModelTF_BB_, *resModelTF_BB_);

//////////////////////////
  rdh_TF_End_ = new RooDataHist("rdh_TF_End","", *rooMass_, th1_TF);
  signalModelTF_End_ = new RooHistPdf("signalModelTF_End", "", 
                    RooArgSet(*shiftedMassTF_End), RooArgList(*rooMass_), *rdh_TF_End_);
  resoTF_End_  = new RooRealVar("resoTF_End","resoTF_End",0.031977, 0.0, 5.);
  resModelTF_End_ = new RooGaussModel("resModelTF_End","gaussian resolution model", 
                *rooMass_, *massShiftTF_End, *resoTF_End_);
  signalShapePdfTF_End_  = new RooFFTConvPdf("signalShapePdfTF_End","final signal shape", 
                       *shiftedMassTF_End, *rooMass_, *signalModelTF_End_, *resModelTF_End_);

}




// ***** Function to return the background Pdf **** //
void makeBkgPdf()
{  
  // Background PDF variables
   bkgGammaFailTF_BB_ = new RooRealVar("bkgGammaFailTF_BB","",-0.0160475, -10., 10.);
   bkgShapePdfTF_BB_ = new RooExponential("bkgShapePdfTF_BB","",*rooMass_, *bkgGammaFailTF_BB_);

   bkgGammaFailTF_End_ = new RooRealVar("bkgGammaFailTF_End","",-0.0101220, -10., 10.);
   bkgShapePdfTF_End_ = new RooExponential("bkgShapePdfTF_End","",*rooMass_, *bkgGammaFailTF_End_);
}



double GetNerrTT(double eB, double eBerr, double eE, double eEerr, 
                 double xsec, double xsecerr, double corrEBX, 
                 double corrEEX, double corrEBEE, double lumi) {

   double T1 = xsecerr/xsec;
   double eff = 0.225*eB*eB + 0.1625*eB*eE + 0.0479*eE*eE;
   double T2 = (2.0*0.2253*eB + 0.1625*eE) * eBerr / eff;
   double T3 = (2.0*0.0479*eE + 0.1625*eB) * eEerr / eff;

   return lumi * xsec * eff * 
      sqrt(T1**2+T2**2+2.0*T1*T2*corrEBX+T3**2+2.0*T1*T3*corrEEX+2.0*T2*T3*corrEBEE);
}





double GetNTFEndcaps(double eB, double eE, double xsec, double lumi) {

   return lumi*xsec*( 0.1625*(0.5*eB+0.5*eE-eB**2) + 0.0479*eE*(1.-eE) );
}



double GetNerrTFEndcaps(double eB, double eBerr, double eE, double eEerr, 
                        double xsec, double xsecerr, double corrEBX, 
                        double corrEEX, double corrEBEE, double lumi) {

   double T1 = xsecerr/xsec;
   double eff = 0.1625*(0.5*eB+0.5*eE-eB**2) + 0.0479*eE*(1.-eE);
   double T2 = 0.1625*0.5 * eBerr / eff;
   double T3 = (0.12915 - 2.0*0.2104*eE) *eEerr / eff;

   return lumi * xsec * eff * 
      sqrt(T1**2+T2**2+2.0*T1*T2*corrEBX+T3**2+2.0*T1*T3*corrEEX+2.0*T2*T3*corrEBEE);
}
