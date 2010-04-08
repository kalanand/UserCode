
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
 *   A standalone macro to compute signal significance for a bump 
 *   hunting experiment. Computes confidence level for rejecting 
 *   null hypothesis (say, QCD or Standard Model predictions) as a 
 *   function of the cross section and/or mass of the new physics 
 *   signal.  Works with  binned data. 
 *
 * Implementation details:
 *   Uses RooFit and RooStat classes.
 *   Uses template morphing to extrapolate signal shape.
 *   User is supposed to provide histograms of signal shape at two 
 *   end points, e.g., two different masses. Alternatively, user 
 *   can specify analytic shapes. 
 *   The QCD/ SM background shape can be obtained from a 
 *   histogram or a parametric function.
 *  
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 ********************************************************************/


//  _   _                _               
// | | | | ___  __ _  __| | ___ _ __ ___ 
// | |_| |/ _ \/ _` |/ _` |/ _ \ '__/ __|
// |  _  |  __/ (_| | (_| |  __/ |  \__ \
// |_| |_|\___|\__,_|\__,_|\___|_|  |___/
// 

// #ifndef __CINT__
// #include "RooGlobalFunc.h"
// #endif
// #include "RooDataSet.h"
// #include "RooRealVar.h"
// #include "RooGaussian.h"
// #include "RooAddPdf.h"
// #include "RooProdPdf.h"
// #include "RooAddition.h"
// #include "RooProduct.h"
// #include "TCanvas.h"
// #include "RooAbsPdf.h"
// #include "RooFit.h"
// #include "RooFitResult.h"
// #include "RooPlot.h"
// #include "RooAbsArg.h"
// #include "RooWorkspace.h"
// #include "RooStats/ProfileLikelihoodCalculator.h"
// #include "RooStats/HypoTestResult.h"
// #include <string>

#include <memory>


//  _     _ _                          
// | |   (_) |__  _ __ __ _ _ __ _   _ 
// | |   | | '_ \| '__/ _` | '__| | | |
// | |___| | |_) | | | (_| | |  | |_| |
// |_____|_|_.__/|_|  \__,_|_|   \__, |
//                               |___/ 

// use this order for safety on library loading
gSystem->Load("libRooFit") ;
gSystem->Load("libRooStats") ;
using namespace RooFit;
using namespace RooStats;


//  _____ _              _   ____                                _                
// |  ___(_)_  _____  __| | |  _ \ __ _ _ __ __ _ _ __ ___   ___| |_ ___ _ __ __ _ 
// | |_  | \ \/ / _ \/ _` | | |_) / _` | '__/ _` | '_ ` _ \ / _ \ __/ _ \ '__/ _ _|
// |  _| | |>  <  __/ (_| | |  __/ (_| | | | (_| | | | | | |  __/ ||  __/ |  \__ \
// |_|   |_/_/\_\___|\__,_| |_|   \__,_|_|  \__,_|_| |_| |_|\___|\__\___|_|  |___/
// 

//// set integrated luminosity in inverse picoparn 
///            => needed to normalize signal, bkg levels
//// and variable mass range ==> for fit 
const double integ_lumi = 10.;
const char* histogramName = "mZJet2_Cor";
const double minMass = 250.0;
const double maxMass = 3000.0;

//////// cross sections for above masses at sqrt{s} = 10 TeV
const double xsec0 = 14.17;
const double xsec1 = 0.6176;
const double xsec2 = 0.0117; 

//  ____  _        ___   ____  _           _   _ _     _            
// / ___|(_) __ _ ( _ ) | __ )| | ____ _  | | | (_)___| |_ ___  ___ 
// \___ \| |/ _` |/ _ \/\  _ \| |/ / _` | | |_| | / __| __/ _ \/ __|
//  ___) | | (_| | (_>  < |_) |   < (_| | |  _  | \__ \ || (_) \__ \
// |____/|_|\__, |\___/\/____/|_|\_\__, | |_| |_|_|___/\__\___/|___/
//          |___/                  |___/                            

/////////////////////////////////////////////
// Read histograms to get q* signal template. 
///// The q* signal were generated at 500 GeV, 1TeV, 2 TeV
TFile *inFile0 = TFile::Open( "signal_500.root", "READ");
TH1D* hist0 = (TH1D *) inFile0->Get( histogramName );
TFile *inFile1 = TFile::Open( "signal_1000.root", "READ");
TH1D* hist1 = (TH1D *) inFile1->Get( histogramName );
TFile *inFile2 = TFile::Open( "signal_2000.root", "READ");
TH1D* hist2 = (TH1D *) inFile2->Get( histogramName );

// Read histograms to get QCD background template. 
TFile *inputFile = TFile::Open( "background.root", "READ");
TH1D* hQCD = (TH1D *) inputFile->Get( histogramName );



//  ____                                                              
// / ___|  ___ __ _ _ __    _ __ ___  ___     _ __ ___   __ _ ___ ___ 
// \___ \ / __/ _` | '_ \  | '__/ _ \/ __|   | '_ ` _ \ / _` / __/ __|
//  ___) | (_| (_| | | | | | | |  __/\__ \_  | | | | | | (_| \__ \__ \
// |____/ \___\__,_|_| |_| |_|  \___||___(_) |_| |_| |_|\__,_|___/___/
// 

void SignificanceCalculatorBinned()
{
   float x[16];
   float CL[16];
   for (int i=0; i<16; i++) {
      x[i] = 100.0 * (i+5);
      CL[i] = SignificanceCalculatorBinned(x[i], false);
   }

   TGraph* clGraph = new TGraph(16, x, CL);
   clGraph->GetXaxis()->SetTitle("m_{q*} (GeV)");
   clGraph->GetYaxis()->SetTitle("Confidence Limit to exclude null hypo");
   TCanvas* ccl = new TCanvas("ccl","cl",500,500);
   clGraph->Draw("AP");

}

//  __  __       _         _____                 _   _             
// |  \/  | __ _(_)_ __   |  ___|   _ _ __   ___| |_(_) ___  _ __  
// | |\/| |/ _` | | '_ \  | |_ | | | | '_ \ / __| __| |/ _ \| '_ \ 
// | |  | | (_| | | | | | |  _|| |_| | | | | (__| |_| | (_) | | | |
// |_|  |_|\__,_|_|_| |_| |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|
// 

double SignificanceCalculatorBinned(double mass, bool drawFitResults=true) {

   hQCD->GetXaxis()->SetRangeUser(minMass, maxMass);
   // Our observable is the invariant mass
   RooRealVar invMass("invMass", "M_{Zjet}", minMass, maxMass,"GeV");
   RooDataHist data( "data", "", invMass, hQCD);

   // define your signal and background models here
   // Make models for signal (q*) and background (QCD: Z+jets)

   // Define Lower and Upper end point histograms for signal shape
   TH1D* lower_hist; 
   TH1D* upper_hist;
   double lower_weight, upper_weight;
   double mAlpha = 0.0;

   if(mass <= 1000.0) {
      lower_hist = hist0;
      upper_hist = hist1;
      lower_weight = xsec0 * integ_lumi;
      upper_weight = xsec1 * integ_lumi;
      mAlpha = (mass-500.0) / (1000.0 - 500.0);
   }
   else if(mass <= 2000.0) {
      lower_hist = hist1;
      upper_hist = hist2;
      lower_weight = xsec1 * integ_lumi;
      upper_weight = xsec2 * integ_lumi;
      mAlpha = (mass-1000.0) / (2000.0 - 1000.0);
   }

   // for the time being setting by hand, because my histograms are 
   // already scaled for 100 pb^-1
   lower_weight = 0.1;
   upper_weight = 0.1;

   //invMass.setRange( TMath::Max(minMass, 0.5*mass), TMath::Min( maxMass, 1.5*mass) );


   // define RooDataHist from an existing histogram
   RooDataHist sigDataHistLow("sigDataHistLow","",  invMass, lower_hist, lower_weight);
   RooDataHist sigDataHistHigh("sigDataHistHigh","",  invMass, upper_hist, upper_weight);


   // Lower end point shape: a histogram shaped p.d.f.
   RooHistPdf g1("g1", "", RooArgSet(invMass), sigDataHistLow);


   // Upper end point shape: also a histogram shaped p.d.f.
   RooHistPdf g2("g2", "", RooArgSet(invMass), sigDataHistHigh);


   //  _                       _       _                                    _     
   // | |_ ___ _ __ ___  _ __ | | __ _| |_ ___   _ __ ___   ___  _ __ _ __ | |__  
   // | __/ _ \ '_ ` _ \| '_ \| |/ _` | __/ _ \ | '_ ` _ \ / _ \| '__| '_ \| '_ \ 
   // | ||  __/ | | | | | |_) | | (_| | ||  __/ | | | | | | (_) | |  | |_) | | | |
   //  \__\___|_| |_| |_| .__/|_|\__,_|\__\___| |_| |_| |_|\___/|_|  | .__/|_| |_|
   //                   |_|                                          |_|          

   // C r e a t e   i n t e r p o l a t i n g   p d f  ("template morphing")
   // -----------------------------------------------------------------------

   // Create interpolation variable
   RooRealVar alpha("alpha","alpha", 1.0 - mAlpha) ;


   // Specify sampling density on observable and interpolation variable
   invMass.setBins(100,"cache") ;
   alpha.setBins(100,"cache") ;

   // Construct interpolating pdf in (x,a) represent g1(x) at a=a_min
   // and g2(x) at a=a_max
   RooIntegralMorph sigModel("sigModel","sigModel",g1,g2,invMass,alpha) ;
   RooRealVar Nsig("Nsig", "# signal events", 0.0, 0.0, 10000000.0);


   //////////////////////////////////////////////
   // make QCD model
   RooRealVar a1("a1","a1", -16.63, -50., 50.) ;  
   RooRealVar a2("a2","a2", 15.3, 0., 50.) ; 
   RooRealVar a3("a3","a3", 3.0, -10., 3.8) ; 
   RooGenericPdf qcdModel("qcdModel",
   "pow(1-invMass/10000.,a1)/pow(invMass/10000.,a2+a3*log10(invMass/10000.))",
   RooArgList(invMass,a1,a2, a3)); 
   RooRealVar Nbkg("Nbkg", "# background events", 10.0, 0.0, 10000000.0);


   //////////////////////////////////////////////
   // combined model
   // Introduce mu: the signal strength in units of the expectation.  
   // eg. mu = 1 is the SM, mu = 0 is no signal, mu=2 is 2x the SM
   //  RooRealVar mu("mu","signal strength in units of SM expectation",0.01,0.,1.) ; 


   // full model
   RooAddPdf model("model","sig+qcd background",RooArgList(sigModel,qcdModel),
   RooArgList(Nsig, Nbkg)); 


//set the precision for the integral of the model here
   RooNumIntConfig* cfg = RooAbsReal::defaultIntegratorConfig();
   cfg->setEpsAbs(1E-4);
   cfg->setEpsRel(1E-4);
   model.setIntegratorConfig(*cfg);


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

/*   

   ////// for debug ==> to make sure that I can generate the pdf shape
   TCanvas* cmc;
   RooPlot* frame;
   RooDataSet *toydata;

   if(drawFitResults) {
     // generate some events to display model shape
      toydata = model.generate(RooArgList(invMass), 10000);
      cmc = new TCanvas("cmc","signal + background shape",500,500);
      frame = invMass.frame() ; 
      toydata->plotOn(frame ) ; 
      model.plotOn(frame, LineColor(kBlue)) ;   
      gPad->SetLogy();
      frame->GetYaxis()->SetNoExponent();
      frame->GetYaxis()->SetRangeUser(1E-1,1E+4);
      frame->SetTitle("signal + background model");
      frame->Draw() ;
   }
*/


   // Main interface to get a HypoTestResult.
   // It does two fits:
   // the first lets the null parameters float, so it's a maximum likelihood estimate
   // the second is to the null (fixing null parameters to their specified values): 
   // eg. a conditional maximum likelihood
   // the ratio of the likelihood at the conditional MLE to the MLE is the profile 
   // likelihood ratio.
   // Wilks' theorem is used to get p-values 
   
   //   __ _ _     _                      _ _ 
   //  / _(_) |_  | |_ ___    _ __  _   _| | |
   // | |_| | __| | __/ _ \  | '_ \| | | | | |
   // |  _| | |_  | || (_) | | | | | |_| | | |
   // |_| |_|\__|  \__\___/  |_| |_|\__,_|_|_|


   // here we explicitly fit for the null hypothesis
   Nsig.setVal(0.0);
   Nsig.setConstant(kTRUE);
   RooFitResult* fit2 = model.fitTo(data, Hesse(kFALSE),Minos(kTRUE),
   Save(kTRUE),PrintLevel(-1),SumW2Error(kTRUE), Extended(kTRUE));
   Double_t NLLatCondMLE= fit2->minNll();
   fit2->Print();

   //plot data and null hypothesis fit
   TCanvas* cdataNull;
   if(drawFitResults) {
      cdataNull = new TCanvas("cdataNull","fit with null hypothesis",500,500);
      frame = invMass.frame() ; 
      data.plotOn(frame ) ; 
      model.plotOn(frame, LineColor(kBlue)) ;   
      model.paramOn(frame, Layout(0.4, 0.85, 0.92)); 
      gPad->SetLogy();
      frame->GetYaxis()->SetNoExponent();
      frame->GetYaxis()->SetRangeUser(1E-3,1E+4);
      frame->SetTitle("fit to data with null hypothesis");
      frame->Draw() ;
   }


   //   __ _ _     _          ____        ____    _                        
   //  / _(_) |_  | |_ ___   / ___|   _  | __ )  | |__  _   _ _ __   ___   
   // | |_| | __| | __/ _ \  \___ \ _| |_|  _ \  | '_ \| | | | '_ \ / _ \  
   // |  _| | |_  | || (_) |  ___) |_   _| |_) | | | | | |_| | |_) | (_) | 
   // |_| |_|\__|  \__\___/  |____/  |_| |____/  |_| |_|\__, | .__/ \___(_)
   //                                                   |___/|_|           

   // calculate MLE for the signal hypothesis
   Nsig.setConstant(kFALSE);
   RooFitResult* fit = model.fitTo(data,Hesse(kFALSE),Minos(kTRUE),
   Save(kTRUE),PrintLevel(-1),SumW2Error(kTRUE), Extended(kTRUE));
   fit->Print();
   Double_t NLLatMLE= fit->minNll();


   // plot data and signal hypothesis fit
   TCanvas* cdata;
   if(drawFitResults) {
      cdata = new TCanvas("cdata","fit with signal hypothesis",500,500);
      frame = invMass.frame() ; 
      data.plotOn(frame ) ;
      model.plotOn(frame) ;   
      model.plotOn(frame,Components(sigModel),LineStyle(kDashed), LineColor(kRed)) ;   
      model.plotOn(frame,Components(qcdModel),LineStyle(kDashed),LineColor(kGreen)) ;  
      model.paramOn(frame, Layout(0.4, 0.85, 0.92)); 
      gPad->SetLogy();
      frame->GetYaxis()->SetNoExponent();
      frame->GetYaxis()->SetRangeUser(1E-3,1E+4);
      frame->SetTitle("fit to data with signal hypothesis");
      frame->Draw() ;
   }


   //  ____  _             _  __ _                          
   // / ___|(_) __ _ _ __ (_)/ _(_) ___ __ _ _ __   ___ ___ 
   // \___ \| |/ _` | '_ \| | |_| |/ __/ _` | '_ \ / __/ _ \
   //  ___) | | (_| | | | | |  _| | (_| (_| | | | | (_|  __/
   // |____/|_|\__, |_| |_|_|_| |_|\___\__,_|_| |_|\___\___|
   //          |___/                                        


   // Use Wilks' theorem to translate -2 log lambda into a signifcance/p-value
   double CLtoReturn = 0.0; // confidence level for rejecting null hypothesis
   double pvalue = 1.0;     // p-value for the null hypothesis
   double significance = 0.0; // will use Wilks' theorem


   HypoTestResult* htr; 
   if( NLLatCondMLE > NLLatMLE ) {
      htr = new HypoTestResult("ProfileLRHypoTestResult",
      SignificanceToPValue(sqrt( 2*(NLLatCondMLE-NLLatMLE))), 0 );
 
      CLtoReturn   = htr->CLb();
      pvalue       = htr->NullPValue();
      significance = htr->Significance();
   }
   cout << "-------------------------------------------------" << endl;
   cout << "The resonance mass considered = " << mass << endl;
   cout << "The p-value for the null hypo is " << pvalue << endl;
   cout << "The confidence level for excluding the null hypo is " << CLtoReturn << endl;
   cout << "Corresponding to a signal signifcance of " << significance << endl;
   cout << "-------------------------------------------------\n\n" << endl;


   //        _       _      ____ _     
   //  _ __ | | ___ | |_   / ___| |    
   // | '_ \| |/ _ \| __| | |   | |    
   // | |_) | | (_) | |_  | |___| |___ 
   // | .__/|_|\___/ \__|  \____|_____|
   // |_|                              

   ///// compute NLL
   RooAbsReal* nll = model.createNLL( data, Extended()) ;
   RooAbsReal* pll = nll->createProfile(Nsig) ;
   pll->addOwnedComponents(*nll) ;  // to avoid memory leak

   TCanvas* cnll;
   RooPlot* frame1;
   
   if(drawFitResults) {
      cnll = new TCanvas("cnll","nll",500,500);
      frame1 = Nsig.frame(Bins(10),Range( 0.01*Nsig.getVal(),10*Nsig.getVal()) ) ;
      nll->plotOn(frame1) ;
      pll->plotOn(frame1,LineColor(kRed)) ;
      gPad->SetLogx();
   }
   return CLtoReturn;
}





