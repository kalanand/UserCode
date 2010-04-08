
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
 *   Generate toy MC of QCD lineshape and fit it.
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

void GenerateAndFit() {

  // Our observable is the invariant mass
   RooRealVar invMass("invMass", "Corrected dijet mass", 30., 600.,"GeV");

   //////////////////////////////////////////////
   // make QCD model
   RooRealVar p1("p1","p1", 4., 0., 20.) ;  
   RooRealVar p2("p2","p2", 5.3, 0., 50.) ; 
   RooRealVar p3("p3","p3", -1.5, -20., 20.) ; 

   // define QCD line shape
   RooGenericPdf qcdModel("qcdModel",
   "pow(1-invMass/7000.+p3*(invMass/7000.)*(invMass/7000.),p1)/pow(invMass,p2)",
   RooArgList(invMass,p1,p2, p3)); 
   // number of events
   RooRealVar p0("p0", "# events", 1.0, 0.0, 100000000000000.0);
   // full model
   RooAddPdf model("model","qcd",RooArgList(qcdModel), RooArgList(p0)); 

   //set the precision for the integral of the model here
   RooNumIntConfig* cfg = RooAbsReal::defaultIntegratorConfig();
   cfg->setEpsAbs(1E-4);
   cfg->setEpsRel(1E-4);
   model.setIntegratorConfig(*cfg);


  // fix the paramters for now ==> we want to generate events
  // p0.setVal(1.);
   p1.setVal(3.975);
   p2.setVal(5.302);
   p3.setVal(-1.51);

   p0.setConstant(kTRUE);
   p1.setConstant(kTRUE);
   p2.setConstant(kTRUE);
   p3.setConstant(kTRUE);


  // generate data

   RooDataSet* data = model.generate(RooArgList(invMass), 1000000);

   //   __ _ _    
   //  / _(_) |_  
   // | |_| | __| 
   // |  _| | |_  
   // |_| |_|\__| 

   p0.setConstant(kFALSE);
   p1.setConstant(kFALSE);
   p2.setConstant(kFALSE);
   p3.setConstant(kFALSE);

   model.fitTo(*data, Hesse(kFALSE),PrintLevel(-1), Range(40, 220));

   //plot data and fit 
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

   TCanvas* canvas = new TCanvas("canvas","fit to dijet mass",500,500);
   invMass.setRange(20,300);
   RooPlot* frame = invMass.frame(0,400, 50) ; 
   data->plotOn(frame ) ; 
   model.plotOn(frame, LineColor(kBlue)) ;   
   char title[100];
   cout << frame->chiSquare() << endl;
   sprintf(title, "#chi^{2}/#nu  = %.2f", frame->chiSquare());
   TLatex* tex = new TLatex(47.17749,0.4662757,title);
   tex->Draw();
   gPad->SetLogy();
   frame->GetYaxis()->SetNoExponent();
   frame->Draw() ;
}





