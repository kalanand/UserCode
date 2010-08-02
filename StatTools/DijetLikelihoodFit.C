
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
 *   signal.  Works with  unbinned data. 
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
#define intLumi 0.120 // integrated luminosity in inverse picobarn
#define xsec_low 10.0 // cross section (pb) for lower reference resonance mass
#define xsec_high 5.0 // cross section (pb) for higher reference resonance mass


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

// set the variable range here
const double minMass = 400.0;
const double maxMass = 2000.0;


//  ____                                                              
// / ___|  ___ __ _ _ __    _ __ ___  ___     _ __ ___   __ _ ___ ___ 
// \___ \ / __/ _` | '_ \  | '__/ _ \/ __|   | '_ ` _ \ / _` / __/ __|
//  ___) | (_| (_| | | | | | | |  __/\__ \_  | | | | | | (_| \__ \__ \
// |____/ \___\__,_|_| |_| |_|  \___||___(_) |_| |_| |_|\__,_|___/___/
// 

void DijetLikelihoodFit()
{

// For this demonstration, I am generating a 1 TeV signal resonance shape 
// in the following way: I generate two Gaussian lineshapes with mean 
// values 800 GeV and 1200 GeV and widths 100 GeV and 200 GeV respectively. 
// Then I use template morphing to extrapolate lineshape at 1 TeV. 
// In real life, Chiyoung/Sertac will provide us two histograms for the 
// upper and lower reference shapes (generating with Pythia) and we will 
// use template morphing to extrapolate for resonance values in between.   

   TH1D* lower_hist = new TH1D("lower_hist","",100, minMass, maxMass);
   TH1D* upper_hist = new TH1D("upper_hist","",100, minMass, maxMass);

   TRandom3 *eventGenerator = new TRandom3(123999);
   TRandom3 *eventGenerator2 = new TRandom3(534572);
   double gaus;

   for(int i=0; i<100000; ++i) {
      gaus = eventGenerator->Gaus( 800., 100. );
      lower_hist->Fill(gaus);
      gaus = eventGenerator2->Gaus( 1200., 200. );
      upper_hist->Fill(gaus);
   }

   delete eventGenerator;
   delete eventGenerator2;

   DijetLikelihoodFit( lower_hist, upper_hist, 0.5, true);
}

//  __  __       _         _____                 _   _             
// |  \/  | __ _(_)_ __   |  ___|   _ _ __   ___| |_(_) ___  _ __  
// | |\/| |/ _` | | '_ \  | |_ | | | | '_ \ / __| __| |/ _ \| '_ \ 
// | |  | | (_| | | | | | |  _|| |_| | | | | (__| |_| | (_) | | | |
// |_|  |_|\__,_|_|_| |_| |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|
// 

// Returns the Confidence Level (CL) for signal + background hypothesis. 
// Uses Wilks' theorem to translate -2 log (likelihood ratio) into a signifcance/p-value.

double DijetLikelihoodFit(TH1D* lower_hist, TH1D* upper_hist, double mAlpha=0.5, 
                bool drawFitResults=true) {

   // Our observable is the invariant mass
   RooRealVar invMass("invMass", "m_{dijet}", minMass, maxMass,"GeV");
 
   // define your signal and background models here
   // Make models for signal (q*) and background (QCD)

   // define RooDataHist from an existing histogram
   RooDataHist sigDataHistLow("sigDataHistLow","",  invMass, lower_hist, xsec_low*intLumi);
   RooDataHist sigDataHistHigh("sigDataHistHigh","",  invMass, upper_hist, xsec_high*intLumi);


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

   // Create interpolation variable: this is the distance from the lower reference mass
   RooRealVar alpha("alpha","alpha", mAlpha) ;

   // Specify sampling density on observable and interpolation variable
   invMass.setBins(100,"cache") ;
   alpha.setBins(100,"cache") ;

   // Construct interpolating pdf in (x,a) represent g1(x) at a=a_min
   // and g2(x) at a=a_max
   RooIntegralMorph sigModel("sigModel","trueSignalShape",g1,g2,invMass,alpha) ;


   // The code below implements resolution smearing due to jet energy scale.
   // The JES will shift the resonance peak position somewhat, and JER will 
   // make it broader. In order for the code below to work you must have 
   // ROOT built with FFT (which is an external package for Fast Fourier 
   // Transformation). I will work on making a small FFT binary which will 
   // obviate the need for external FFT package. 

/*
   RooIntegralMorph trueSignalShape("trueSignalShape","trueSignalShape",g1,g2,invMass,alpha) ;

   // Include effect of jet energy scale (JES) and resolution (JER)
   // JES will cause a shift in the resonance mass. Set it to 0 for the time being. 
   RooRealVar bias("bias","bias",0., -100., 100.) ; 
   // JER will cause a broadening of the resonance shape. Set the sigma to 1 for the time being.  
   RooRealVar sigma("sigma","sigma", 1., 0., 100.) ; 

   // Assume a Gaussian resolution model from JES/JER
   RooGaussModel resModel("resModel","gaussian resolution model", invMass, bias, sigma);
   invMass.setBins(1000,"cache") ;
   RooFFTConvPdf sigModel("sigModel","final signal shape", invMass, trueSignalShape, resModel);
*/
   RooRealVar Nsig("Nsig", "# signal events", 500.0, 0.0, 10000.0);


   //////////////////////////////////////////////
   // make QCD model
   RooRealVar a1("a1","a1",  4.0,   0.,  50.) ;  
   RooRealVar a2("a2","a2",  5.0,   0.,  50.) ; 
   RooRealVar a3("a3","a3",  3.0, -100., 100.) ; 
   RooGenericPdf qcdModel("qcdModel",
   "pow(1-invMass/7000.+a3*(invMass/7000.)*(invMass/7000.),a1)/pow(invMass/7000.,a2)",
   RooArgList(invMass,a1,a2, a3)); 
   RooRealVar Nbkg("Nbkg", "# background events", 10000.0, 0.0, 10000000.0);


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

 

   ////// for debug ==> to make sure that I can generate the pdf shape
   TCanvas* cmc;
   RooPlot* frame;
   RooDataSet *data;

   if(drawFitResults) {
     // generate some events to display model shape
      data = model.generate(RooArgList(invMass), 10000);
      cmc = new TCanvas("cmc","Generated signal + background shape",500,500);
      frame = invMass.frame() ; 
      data->plotOn(frame ) ; 
      model.plotOn(frame, LineColor(kBlue)) ;   
      gPad->SetLogy();
      frame->GetYaxis()->SetNoExponent();
      frame->GetYaxis()->SetRangeUser(1E-1,1E+4);
      frame->SetTitle("signal + background model");
      frame->Draw() ;
   }


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
//   bias.setConstant(kTRUE);
//   sigma.setConstant(kTRUE);

   RooFitResult* fit2 = model.fitTo(*data, Hesse(kFALSE),Minos(kTRUE),
   Save(kTRUE),PrintLevel(-1),SumW2Error(kTRUE), Extended(kTRUE));
   Double_t NLLatCondMLE= fit2->minNll();
   fit2->Print();

   //plot data and null hypothesis fit
   TCanvas* cdataNull;
   if(drawFitResults) {
      cdataNull = new TCanvas("cdataNull","fit with null hypothesis",500,500);
      frame = invMass.frame() ; 
      data->plotOn(frame ) ; 
      model.plotOn(frame, LineColor(kBlue)) ;   
      model.paramOn(frame, Layout(0.4, 0.85, 0.98), Format("NEU",AutoPrecision(1)) ); 
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
   // bias.setConstant(kFALSE);
   // sigma.setConstant(kFALSE);

   RooFitResult* fit = model.fitTo(*data,Hesse(kFALSE),Minos(kTRUE),
   Save(kTRUE),PrintLevel(-1),SumW2Error(kTRUE), Extended(kTRUE));
   fit->Print();
   Double_t NLLatMLE= fit->minNll();


   // plot data and signal hypothesis fit
   TCanvas* cdata;
   if(drawFitResults) {
      cdata = new TCanvas("cdata","fit with signal hypothesis",500,500);
      frame = invMass.frame() ; 
      data->plotOn(frame ) ;
      model.plotOn(frame) ;   
      model.plotOn(frame,Components(sigModel),LineStyle(kDashed), LineColor(kRed)) ;   
      model.plotOn(frame,Components(qcdModel),LineStyle(kDashed),LineColor(kGreen)) ;  
      model.paramOn(frame, Layout(0.4, 0.85, 0.98), Format("NEU",AutoPrecision(1)) ); 
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
   cout << "The p-value for the null hypo is " << pvalue << endl;
   cout << "The confidence level for excluding the null hypo is " << CLtoReturn << endl;
   cout << "Corresponding to a signal signifcance of " << significance << endl;
   cout << "-------------------------------------------------\n\n" << endl;

}





