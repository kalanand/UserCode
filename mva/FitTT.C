
#define MINMASS 50.0
#include <iostream>
#include <iomanip>
#include <fstream>



void FitTT(char* inFile = "CorrectEscale_ZeeCands_WP95WP95.root")
{


   TFile fin(inFile, "read");
   TTree* tree = (TTree*) fin.Get("tree");

  // The fit variable - lepton invariant mass
  RooRealVar* Zmass = new RooRealVar("mass_corrected","m_{ee}", MINMASS,120.0,"GeV/c^{2}");
  gROOT->cd();


  RooDataSet *data = new RooDataSet("data", "data", tree, *Zmass);

 // Signal pdf

//   Zeelineshape_file =  new TFile("../Zlineshapes.root", "READ");
//   TH1* histbbpass = (TH1D*) Zeelineshape_file->Get("pass_BB");
//   TH1* histebpass = (TH1D*) Zeelineshape_file->Get("pass_BE");
//   TH1* histeepass = (TH1D*) Zeelineshape_file->Get("pass_EE");
//   TH1D* th1 = (TH1D*) histbbpass->Clone("th1");
//   th1->Add(histebpass);
//   th1->Add(histeepass);



  TFile* fMC = new TFile("../allTPtrees_mc.root");
  TTree* mctree = (TTree*) fMC->Get("IdToHLT/fitter_tree");
  TH1D* th1 = new TH1D("th1","", 500, MINMASS, 120);
  mctree->Draw("mass>>th1", "","goff");

  RooDataHist* rdh = new RooDataHist("rdh","", *Zmass, th1);
  RooHistPdf* signalShape = new RooHistPdf("signalShape", "", RooArgSet(*Zmass), *rdh);
  RooRealVar* zero  = new RooRealVar("zero","", 0.0);
  RooRealVar* resolution  = new RooRealVar("resolution","",2.15748, 0.0, 5.);
  RooGaussModel* resModel = new RooGaussModel("resModel","",*Zmass, *zero, *resolution);
  RooAbsPdf* signalShapePdf  = new RooFFTConvPdf("sigModel","final signal shape", 
                       *Zmass, *signalShape, *resModel);



  // Background PDF 
  RooRealVar* bkgShape = new RooRealVar("bkgShape","bkgShape",-0.02,-10.,10.);
  RooExponential* bkgShapePdf = new RooExponential("bkgShapePdf","bkgShapePdf",*Zmass, *bkgShape);


  // Now define some efficiency/yield variables  
  RooRealVar* nSig = new RooRealVar("nSig","nSig", 4000.0, 0.0, 100000.0);
  RooRealVar* nBkg = new RooRealVar("nBkg","nBkg", 1000.0, 0.0, 10000.0);
  RooArgList components(*signalShapePdf,*bkgShapePdf);
  RooArgList yields(*nSig, *nBkg);



  // The total pdf ...
  RooAddPdf* totalPdf = new RooAddPdf("totalPdf","totalPdf",components,yields);
  totalPdf->Print();


  // ********* Do the Actual Fit ********** //  
   RooFitResult *fitResult = totalPdf->fitTo(*data, RooFit::Save(true), 
   RooFit::Extended(true), RooFit::Minos(true), RooFit::PrintLevel(1));
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
  RooAbsData::ErrorType errorType = RooAbsData::Poisson;

  TString cname = "temp_fit";
  TCanvas* c = new TCanvas(cname,cname,500,500);
  RooPlot* frame1 = Zmass->frame(MINMASS, 120, 0.5*(120-(int)MINMASS));
  frame1->SetMinimum(0);
  data->plotOn(frame1,RooFit::DataError(errorType));
  totalPdf->plotOn(frame1,RooFit::ProjWData(*data), 
  RooFit::Components(*bkgShapePdf),RooFit::LineColor(kRed));
  totalPdf->plotOn(frame1,RooFit::ProjWData(*data));
  frame1->Draw("e0");


  TPaveText *plotlabel4 = new TPaveText(0.6,0.82,0.8,0.87,"NDC");
   plotlabel4->SetTextColor(kBlack);
   plotlabel4->SetFillColor(kWhite);
   plotlabel4->SetBorderSize(0);
   plotlabel4->SetTextAlign(12);
   plotlabel4->SetTextSize(0.03);
   char temp[50];
   sprintf(temp, "Signal = %.2f #pm %.2f", nSig->getVal(), nSig->getError());
   plotlabel4->AddText(temp);
  TPaveText *plotlabel5 = new TPaveText(0.6,0.77,0.8,0.82,"NDC");
   plotlabel5->SetTextColor(kBlack);
   plotlabel5->SetFillColor(kWhite);
   plotlabel5->SetBorderSize(0);
   plotlabel5->SetTextAlign(12);
   plotlabel5->SetTextSize(0.03);
   sprintf(temp, "Bkg = %.2f #pm %.2f", nBkg->getVal(), nBkg->getError());
   plotlabel5->AddText(temp);

  plotlabel4->Draw();
  plotlabel5->Draw();
  c->SaveAs( cname + TString(".eps"));
  c->SaveAs( cname + TString(".gif"));
  c->SaveAs( cname + TString(".root"));
  cout<< "chi^2/ndf =" << frame1->chiSquare() << endl;
  // delete c;


}





