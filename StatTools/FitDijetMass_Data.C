
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
 *   Plots dijet mass distributions in data and fit to QCD lineshape 
 *
 * Implementation details:
 *   Uses RooFit and RooStat classes.
 *  
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 ********************************************************************/


#include <memory>


//  _     _ _                          
// | |   (_) |__  _ __ __ _ _ __ _   _ 
// | |   | | '_ \| '__/ _` | '__| | | |
// | |___| | |_) | | | (_| | |  | |_| |
// |_____|_|_.__/|_|  \__,_|_|   \__, |
//                               |___/ 

gSystem->Load("libRooFit") ;
using namespace RooFit;



//  __  __       _         _____                 _   _             
// |  \/  | __ _(_)_ __   |  ___|   _ _ __   ___| |_(_) ___  _ __  
// | |\/| |/ _` | | '_ \  | |_ | | | | '_ \ / __| __| |/ _ \| '_ \ 
// | |  | | (_| | | | | | |  _|| |_| | | | | (__| |_| | (_) | | | |
// |_|  |_|\__,_|_|_| |_| |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|
// 

void FitDijetMass_Data() {

  
  TFile *inf  = new TFile("MassResults_ak7calo.root");
  TH1F *hCorMass     = (TH1F*) inf->Get("DiJetMass");


  gROOT->ProcessLine(".L tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);
  gStyle->SetOptFit(1111);
  tdrStyle->SetOptStat(0); 

  
  TCanvas* c2 = new TCanvas("c2","DijetMass", 500, 500);
  TF1 *func = new TF1("func", "[0]*((1-x/7000.+[3]*(x/7000)^2)^[1])*((x)^(-1*[2]))", 50., 700.);
  func->SetLineColor(4);
  func->SetLineWidth(3);
  func->SetParameter(0,1.88e+8);
  func->SetParameter(1,3.975);
  func->SetParameter(2,5.302);
  func->SetParameter(3,-1.51);
  hCorMass->Fit("func","","",50.0, 526.0); // QCD fit
  hCorMass->SetXTitle("Corrected Dijet Mass (GeV)");
  hCorMass->SetYTitle("Events/GeV");
  hCorMass->GetYaxis()->SetTitleOffset(1.5);
  hCorMass->SetMarkerStyle(20);
  hCorMass->GetXaxis()->SetRangeUser(0.,600.);
  hCorMass->Draw("ep");
  c2->SetLogy(1);





  // Our observable is the invariant mass
  RooRealVar invMass("invMass", "Corrected dijet mass", 
		     30., 600.0, "GeV");
  RooDataHist data( "data", "", invMass, hCorMass);

   //////////////////////////////////////////////




   // make QCD model
  RooRealVar p0("p0", "# events", 600.0, 0.0, 1000000000000000000000.);
  RooRealVar p1("p1","p1", 3.975, -10., 10.) ;  
  RooRealVar p2("p2","p2", 5.302, 4., 8.) ; 
  RooRealVar p3("p3","p3", -1.51, -100., 100.) ; 



//    // define QCD line shape
  RooGenericPdf qcdModel("qcdModel", "pow(1-@0/7000.+@3*(@0/7000.)*(@0/7000.),@1)*pow(@0/7000.,-@2)",
			 RooArgList(invMass,p1,p2,p3)); 

   // full model
   RooAddPdf model("model","qcd",RooArgList(qcdModel), RooArgList(p0)); 



   //plot sig candidates, full model, and individual componenets 

   //   __ _ _    
   //  / _(_) |_  
   // | |_| | __| 
   // |  _| | |_  
   // |_| |_|\__| 


 // Important: fit integrating f(x) over ranges defined by X errors, rather
  // than taking point at center of bin

   RooFitResult* fit = model.fitTo(data, Minos(kFALSE), Extended(kTRUE),
				   SumW2Error(kFALSE),Save(kTRUE), Range(47.,526.),
				   Integrate(kTRUE) );

   // to perform chi^2 minimization fit instead
   //    RooFitResult* fit = model.chi2FitTo(data, Extended(kTRUE), 
   // 				       Save(),Range(50.,526.),Integrate(kTRUE) );

   fit->Print();


   //plot data 
   TCanvas* cdataNull = new TCanvas("cdataNull","fit to dijet mass",500,500);
   RooPlot* frame1 = invMass.frame() ; 
   data.plotOn(frame1, DataError(RooAbsData::SumW2) ) ; 
   model.plotOn(frame1, LineColor(kBlue)) ; 
   model.plotOn(frame1, VisualizeError(*fit, 1),FillColor(kYellow)) ;   
   data.plotOn(frame1, DataError(RooAbsData::SumW2) ) ; 
   model.plotOn(frame1, LineColor(kBlue)) ; 
   model.paramOn(frame1, Layout(0.4, 0.85, 0.92)); 
   TPaveText* dataPave = (TPaveText*) frame1->findObject("model_paramBox");
   dataPave->SetY1(0.77);
   gPad->SetLogy();
   frame1->GetYaxis()->SetNoExponent();
   frame1->GetYaxis()->SetRangeUser(5E-3,5E+3);
   frame1->GetYaxis()->SetTitle("Events / GeV");
   frame1->GetYaxis()->SetTitleOffset(1.35);
   frame1->SetTitle("fit to data with QCD lineshape");
   frame1->Draw() ;


    
    // S h o w   r e s i d u a l   a n d   p u l l   d i s t s
    // -------------------------------------------------------
    
   //// Construct a histogram with the residuals of the data w.r.t. the curve
   RooHist* hresid = frame1->residHist() ;
   // Create a new frame to draw the residual distribution and add the distribution to the frame
   RooPlot* frame2 = invMass.frame(Title("Residual Distribution")) ;
   frame2->addPlotable(hresid,"P") ;


    
   ///// Construct a histogram with the pulls of the data w.r.t the curve
   RooHist* hpull = frame1->pullHist() ;   
   //// Create a new frame to draw the pull distribution and add the distribution to the frame
   RooPlot* frame3 = invMass.frame(Title("Pull Distribution")) ;
   frame3->addPlotable(hpull,"P") ;


   TCanvas* cResidual = new TCanvas("cResidual","Residual Distribution",1000,500);
   cResidual->Divide(2) ;
   cResidual->cd(1) ; gPad->SetLeftMargin(0.15) ; frame1->GetYaxis()->SetTitleOffset(1.6) ; frame2->Draw() ;
   cResidual->cd(2) ; gPad->SetLeftMargin(0.15) ; frame1->GetYaxis()->SetTitleOffset(1.6) ; frame3->Draw() ;

}


