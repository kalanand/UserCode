
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

const int numEventsToGenerate = 1000000;

const int nMassBins = 33;
double massBoundaries[nMassBins+1] = { 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 
                                       1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 
                                       1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 
                                       2895, 3019};

//  __  __       _         _____                 _   _             
// |  \/  | __ _(_)_ __   |  ___|   _ _ __   ___| |_(_) ___  _ __  
// | |\/| |/ _` | | '_ \  | |_ | | | | '_ \ / __| __| |/ _ \| '_ \ 
// | |  | | (_| | | | | | |  _|| |_| | | | | (__| |_| | (_) | | | |
// |_|  |_|\__,_|_|_| |_| |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|
// 

void GenerateAndFit() {

  TFile* fout = new TFile("QCDshape-generated.root","recreate");

  // Our observable is the invariant mass
  RooRealVar invMass("invMass", "Corrected dijet mass", 
  massBoundaries[0], massBoundaries[nMassBins],"GeV");


  // set realistic mass range for generation and fitting
  //invMass.setRange( 520.0, 3019.0 );

   //////////////////////////////////////////////
   // make QCD model
   RooRealVar p1("p1","p1", 3.975) ;  
   RooRealVar p2("p2","p2", 5.302) ; 
   RooRealVar p3("p3","p3", -1.51) ; 

   // define QCD line shape
   RooGenericPdf qcdModel("qcdModel",
   "pow(1-invMass/7000.+p3*(invMass/7000.)*(invMass/7000.),p1)/pow(invMass,p2)",
   RooArgList(invMass,p1,p2, p3)); 

  // generate data
   RooDataSet* data = qcdModel.generate(RooArgList(invMass), numEventsToGenerate);

   //////////////////////////////////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////////
   // the following is needed only if you want to perform a binned LH fit
   // create binned data 
   TTree* tree = data->tree();
   TH1D* hist =  new TH1D("hist", "", nMassBins, massBoundaries);
   tree->Draw("invMass>>hist", "", "goff");
   RooDataHist binneddata( "binneddata", "", invMass, hist);
   //////////////////////////////////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////////


   //   __ _ _    
   //  / _(_) |_  
   // | |_| | __| 
   // |  _| | |_  
   // |_| |_|\__| 

   // number of events
   RooRealVar p0("p0", "# events", 10000.0, 0., 1.0e+6);
   // full model
   RooAddPdf model("model","qcd",RooArgList(qcdModel), RooArgList(p0)); 

   //set the precision for the integral of the model here
   RooNumIntConfig* cfg = RooAbsReal::defaultIntegratorConfig();
   cfg->setEpsAbs(1E-5);
   cfg->setEpsRel(1E-5);
   model.setIntegratorConfig(*cfg);

   p1.setConstant(kFALSE);
   p1.setRange(4., 6.) ; 
   
   p2.setConstant(kFALSE);
   p2.setRange(4.5, 5.5) ; 
   p3.setConstant(kFALSE);
   p3.setRange(-20., -1.) ; 

   ////// unbinned likelihood fit
     // RooFitResult* fit = model.fitTo( *data, PrintLevel(-1),Save(kTRUE));

   ////// binned likelihood fit
   RooFitResult* fit = model.fitTo(binneddata,SumW2Error(kTRUE), PrintLevel(-1),Save(kTRUE));

   fit->Print();
   Double_t NLLatMLE = fit->minNll();

   //plot data and fit 
   gROOT->ProcessLine(".L tdrstyle.C");
   setTDRStyle();
   tdrStyle->SetPadRightMargin(0.08);


   TCanvas* canvas = new TCanvas("canvas","fit to dijet mass",500,500);
   RooPlot* frame = invMass.frame() ; 
   /////// unbinned likelihood fit
   //data.plotOn(frame ) ; 
   /////// unbinned likelihood fit
   binneddata.plotOn(frame ) ; 
   model.plotOn(frame, LineColor(kBlue)) ;   
   frame->Draw() ;
   frame->SetMinimum(0.2);
   frame->SetMaximum(1e+07);
   frame->GetYaxis()->SetTitle("Events/bin");
   gPad->SetLogy();
   char title[100];
   cout << "chi^2 of the fit is: " << frame->chiSquare() << endl;
   sprintf(title, "NLL = %.1E,  #chi^{2}/#nu  = %.2f/%d", NLLatMLE, frame->chiSquare(), nMassBins);
   TLatex tex;
   tex.SetNDC();
   tex.SetTextAlign(12);
   tex.SetTextSize(0.04);
   tex.DrawLatex(0.35,0.87, title);
   canvas->SaveAs("myCanvas.root");
   canvas->SaveAs("myCanvas.eps");
   canvas->SaveAs("myCanvas.gif");

   fout->cd();
   hist->Write();

   ////// delete all created objects to avoid memory leak //////
   // delete hist;
   // fout->Close();
   // delete fout;
   // delete canvas;
   // delete data;
}





