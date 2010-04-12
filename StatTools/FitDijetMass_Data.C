
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



  // Our observable is the invariant mass
  RooRealVar invMass("invMass", "Corrected dijet mass", 
		     45., 600.0, "GeV");
  RooDataHist data( "data", "", invMass, hCorMass);

   //////////////////////////////////////////////
   // make QCD model
  RooRealVar p1("p1","p1", 2.975, 1., 200.) ;  
  RooRealVar p2("p2","p2", 5., 4.2, 6.) ; 
  RooRealVar p3("p3","p3", 0., -1., 1.) ; 


   // define QCD line shape
   RooGenericPdf qcdModel("qcdModel",
   "pow(1-invMass/7000.+p3*(invMass/7000.)*(invMass/7000.),p1)/pow(invMass,p2)",
   RooArgList(invMass,p1,p2, p3)); 

   RooRealVar p0("p0", "# events", 500.0, 0.0, 10000.0);
   // full model
   RooAddPdf model("model","qcd",RooArgList(qcdModel), RooArgList(p0)); 




   //plot sig candidates, full model, and individual componenets 
   gROOT->ProcessLine(".L tdrstyle.C");
   setTDRStyle();
   tdrStyle->SetErrorX(0.5);
   tdrStyle->SetPadLeftMargin(0.18);
   tdrStyle->SetPadRightMargin(0.08);
   tdrStyle->SetLegendBorderSize(0);
   tdrStyle->SetTitleYOffset(1.3);
   gStyle->SetOptFit(1111);
   tdrStyle->SetOptFile(1111);
   tdrStyle->SetOptStat(0); 
   tdrStyle->SetStatColor(1);


   //   __ _ _    
   //  / _(_) |_  
   // | |_| | __| 
   // |  _| | |_  
   // |_| |_|\__| 


   RooFitResult* fit = model.fitTo(data, Hesse(kFALSE),Minos(kTRUE),
	       PrintLevel(-1),SumW2Error(kTRUE), Extended(kTRUE),Save(kTRUE));

   fit->Print();

   //plot data 
   TCanvas* cdataNull = new TCanvas("cdataNull","fit to dijet mass",500,500);
   RooPlot* frame = invMass.frame() ; 
   data.plotOn(frame ) ; 
   model.plotOn(frame, LineColor(kBlue)) ;   
   model.paramOn(frame, Layout(0.4, 0.85, 0.92)); 
   TPaveText* dataPave = (TPaveText*) frame->findObject("model_paramBox");
   dataPave->SetY1(0.77);
   gPad->SetLogy();
   frame->GetYaxis()->SetNoExponent();
   frame->GetYaxis()->SetRangeUser(1E-3,1E+3);
   frame->SetTitle("fit to data with null hypothesis");
   frame->Draw() ;

   tex = new TLatex(47.17749,0.4662757,"#sqrt{s}=7 TeV");
   tex->SetTextSize(0.04);
   tex->Draw();
   tex = new TLatex(48.54541,0.5273968,"|Jet #eta|<1.3");
   tex->SetTextSize(0.04);
   tex->Draw();
   tex = new TLatex(50.54541,0.5273968,"AK7Calo");
   tex->SetTextSize(0.04);
   tex->Draw();
   tex = new TLatex(52.54541,0.5273968,"P_{t}>20 GeV");
   tex->SetTextSize(0.04);
   tex->Draw() ;
}





