
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

const bool drawFitResults=true;

// set the variable range here
const double minMass = 354.0;
const double maxMass = 3019.0;

const int nMassBins = 38;
double massBoundaries[nMassBins+1] = 
{354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 
 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 
 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 
 2775, 2895, 3019};

// const int nMassBins = 61;
// double massBoundaries[nMassBins+1] = 
// {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 
//  220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 
//  740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 
//  1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 
//  2775, 2895, 3019};


//  ____                                                              
// / ___|  ___ __ _ _ __    _ __ ___  ___     _ __ ___   __ _ ___ ___ 
// \___ \ / __/ _` | '_ \  | '__/ _ \/ __|   | '_ ` _ \ / _` / __/ __|
//  ___) | (_| (_| | | | | | | |  __/\__ \_  | | | | | | (_| \__ \__ \
// |____/ \___\__,_|_| |_| |_|  \___||___(_) |_| |_| |_|\__,_|___/___/
// 

void DijetLikelihoodFit()
{
   double xs[21];

   /*
   for(int i=0; i<16; ++i) {
     cout << "#############################################" << endl;
     cout << "Resonance mass = " << 500.+100.*i << endl;
     
     xs[i] = DijetLikelihoodFit( 500.+100.*i, 0.0, 0.0, true);
     
     cout << "#############################################" << endl;
   }
   */

   DijetLikelihoodFit( 700., 0., 0., true);

}



//  __  __       _         _____                 _   _             
// |  \/  | __ _(_)_ __   |  ___|   _ _ __   ___| |_(_) ___  _ __  
// | |\/| |/ _` | | '_ \  | |_ | | | | '_ \ / __| __| |/ _ \| '_ \ 
// | |  | | (_| | | | | | |  _|| |_| | | | | (__| |_| | (_) | | | |
// |_|  |_|\__,_|_|_| |_| |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|
// 

// Returns the Confidence Level (CL) for signal + background hypothesis. 
// Uses Wilks' theorem to translate -2 log (likelihood ratio) into a signifcance/p-value.

double DijetLikelihoodFit(double resMass, double shiftInPeakFromJES=0.0, 
                          double shiftInPeakFromJER=0.0, 
                          bool doQCDFit = false) {

   RooBinning fullRange( nMassBins, massBoundaries, "fullRange");

   // Our observable is the invariant mass
   RooRealVar invMass("invMass", "m_{dijet}", minMass, maxMass,"GeV");
   invMass.setBinning(fullRange, "fullRange");
   TCanvas* cmc;
   RooPlot* frame;
   RooDataSet *data = RooDataSet::read("120nbm1_Dijet_Mass.txt", RooArgList(invMass));

  char title[100];


   // define your signal and background models here
   // Make models for signal (q*) and background (QCD)

   // define RooDataHist from an existing histogram
  int mMin=500, mMax=3500;
   double xsec_low = 0.0; // cross section (pb) for lower reference resonance mass
   double xsec_high = 0.0; // cross section (pb) for higher reference resonance mass
   double mAlpha = 0.0;

   if(resMass<500 || resMass>3500) return -1;
   else if(resMass<700) { mMin=500; mMax=700; xsec_low=0.1472E+04; xsec_high=0.3562E+03; }
   else if(resMass<1200) { mMin=700; mMax=1200; xsec_low=0.3562E+03; xsec_high=0.2449E+02; }
   else if(resMass<2000) { mMin=1200; mMax=2000; xsec_low=0.2449E+02; xsec_high=0.8122E+00; }
   else { mMin=2000; mMax=3500; xsec_low=0.8122E+00; xsec_high=0.2364E-02; }

   TFile* finput = new TFile("dijetResonanceShape.root","read");
   sprintf(title, "h_Djj_cor_%d", mMin);
   TH1D* lower_hist = (TH1D*) finput->Get(title);
   sprintf(title, "h_Djj_cor_%d", mMax);
   TH1D* upper_hist = (TH1D*) finput->Get(title);

   SetProperXRangeInDijetResonanceMassHistogram( *lower_hist, mMin);
   SetProperXRangeInDijetResonanceMassHistogram( *upper_hist, mMax);


   RooDataHist sigDataHistLow("sigDataHistLow","",  RooArgSet(invMass), lower_hist, intLumi*xsec_low);
   RooDataHist sigDataHistHigh("sigDataHistHigh","",  RooArgSet(invMass), upper_hist, intLumi*xsec_high);

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

   // Create interpolation variable: this is the distance from the higher reference mass
   RooRealVar upperRefMass("upperRefMass","mMax", (double) mMax);
   RooRealVar lowerRefMass("lowerRefMass","mMin", (double) mMin);
   RooRealVar m0("m0","m0", resMass) ;

   // The code below implements shift in resonance peak position due to jet energy scale.
  RooRealVar deltam("deltam","deltam", shiftInPeakFromJES) ;
  RooFormulaVar alpha("alpha","(@1-@2+@3)/(@1-@0)", 
                      RooArgList(lowerRefMass, upperRefMass, m0, deltam)) ;

   // Construct interpolating pdf in (x,a) represent g1(x) at a=a_min
   // and g2(x) at a=a_max
  RooIntegralMorph sigModel0("sigModel0","Signal Shape before resolution",g1,g2,invMass,alpha) ;

  // The code below implements effect of jet energy smearing
  RooRealVar deltasigma("deltasigma","deltasigma", 0.5 + shiftInPeakFromJER/resMass) ;
  RooIntegralMorph sigModel("sigModel","Signal Shape after resolution",
                             sigModel0, sigModel0, invMass, deltasigma) ;


   // Specify sampling density on observable and interpolation variable
   invMass.setBins(100,"cache") ;

   RooRealVar luminosity("luminosity","integrated luminosity", intLumi) ;
   RooRealVar SigXS("SigXS", "Cross Section #times BR #times Acc. (pb)", 100.0, 0., 2000.);
   RooProduct Nsig("Nsig", "# signal events", RooArgSet(luminosity, SigXS)) ;



   //////////////////////////////////////////////
   // make QCD model
   RooRealVar a1("a1","a1",  1.8533e-07,   0.,  10.) ;  
   RooRealVar a2("a2","a2",  3.4204e+00,   0.,  10.) ; 
   RooRealVar a3("a3","a3",  5.8554e-01,   0.,  10.) ; 
//    RooGenericPdf qcdModel("qcdModel",
//    "pow(1-invMass/7000.+a3*(invMass/7000.)**2,a1)/pow(invMass,a2)",
   RooGenericPdf qcdModel("qcdModel",
   "pow(1-invMass/7000.,a1)/pow(invMass,a2+a3*log(invMass/7000.))",
   RooArgList(invMass,a1,a2, a3)); 

   RooRealVar Nbkg("Nbkg", "# background events", data->numEntries(), 0., 10000000.);


   //////////////////////////////////////////////
   // combined model
   // full model
   RooAddPdf model("model","sig+qcd background",RooArgList(sigModel,qcdModel),
   RooArgList(Nsig, Nbkg)); 


//set the precision for the integral of the model here
//   RooNumIntConfig* cfg = RooAbsReal::defaultIntegratorConfig();
//   cfg->setEpsAbs(1E-6);
//   cfg->setEpsRel(1E-6);
//   model.setIntegratorConfig(*cfg);


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
//   Nsig.setVal(0.0);
//   Nsig.setConstant(kTRUE);
//   bias.setConstant(kTRUE);
//   sigma.setConstant(kTRUE);

   SigXS.setVal(0.0);
   SigXS.setConstant(kTRUE);

   RooFitResult* fit2; 
   Double_t NLLatCondMLE;

   if(doQCDFit) {
      fit2 = model.fitTo(*data, Hesse(kFALSE),Minos(kTRUE),
      Save(kTRUE),PrintLevel(-1),SumW2Error(kTRUE), Extended(kTRUE));
      NLLatCondMLE= fit2->minNll();
      fit2->Print();
   }
   else 
      NLLatCondMLE = -2659.31;

   cout << "NLL with NULL hypothesis = " << NLLatCondMLE << endl;


   //plot data and null hypothesis fit
   TCanvas* cdataNull;
   if(drawFitResults) {
      cdataNull = new TCanvas("cdataNull","fit with null hypothesis",500,500);
      frame = invMass.frame() ; 
      data->plotOn(frame, Binning("fullRange"), DataError(RooAbsData::Poisson) ) ; 
      model.plotOn(frame, LineColor(kBlue)) ;   
      model.paramOn(frame, Layout(0.43, 0.88, 0.92), Format("NEU",AutoPrecision(1)) ); 
      gPad->SetLogy();
      frame->GetXaxis()->SetRangeUser(360,2500);
      frame->GetYaxis()->SetNoExponent();
      frame->GetYaxis()->SetRangeUser(1E-2,1E+4);
      frame->GetYaxis()->SetTitle("Events / bin");
      frame->GetYaxis()->SetTitleOffset(1.5);
      frame->SetTitle("fit to data with null hypothesis");
      frame->Draw() ;
      sprintf(title, "#chi^{2}/#nu = %.2f", frame->chiSquare());
      TLatex tex;
      tex.SetNDC();
      tex.SetTextAlign(12);
      tex.SetTextSize(0.04);
      tex.DrawLatex(0.22,0.88, title);
      cdataNull->SaveAs("plot_dijetmass_fitNULL.gif");
      cdataNull->SaveAs("plot_dijetmass_fitNULL.eps");
      cdataNull->SaveAs("plot_dijetmass_fitNULL.root");


    // S h o w   r e s i d u a l   a n d   p u l l   d i s t s  
      RooHist* hresid = frame->residHist() ;
      RooPlot* frame2 = invMass.frame(Title("Residual Distribution")) ;
      frame2->addPlotable(hresid,"P") ;    
      RooHist* hpull = frame->pullHist() ;   
      RooPlot* frame3 = invMass.frame(Title("Pull Distribution")) ;
      frame3->addPlotable(hpull,"P") ;

      TCanvas* cResidualNULL = new TCanvas("cResidualNULL","Residual Distribution",1000,500);
      cResidualNULL->Divide(2) ;
      cResidualNULL->cd(1) ; 
      gPad->SetLeftMargin(0.15) ; 
      frame2->GetXaxis()->SetRangeUser(360,2500);
      frame2->GetYaxis()->SetTitleOffset(1.3) ; 
      frame2->GetYaxis()->SetTitle("Residual = Data - Fit");
      frame2->Draw() ;
      cResidualNULL->cd(2) ; 
      gPad->SetLeftMargin(0.15) ; 
      frame3->GetXaxis()->SetRangeUser(360,2500);
      frame3->GetYaxis()->SetTitleOffset(1) ; 
      frame3->GetYaxis()->SetTitle("Pull = (Data - Fit) / Error");
      frame3->Draw() ;
      cResidualNULL->SaveAs("plot_dijetmass_residual_NULL.gif");
      cResidualNULL->SaveAs("plot_dijetmass_residual_NULL.eps");
      cResidualNULL->SaveAs("plot_dijetmass_residual_NULL.root");
   }


   //   __ _ _     _          ____        ____    _                        
   //  / _(_) |_  | |_ ___   / ___|   _  | __ )  | |__  _   _ _ __   ___   
   // | |_| | __| | __/ _ \  \___ \ _| |_|  _ \  | '_ \| | | | '_ \ / _ \  
   // |  _| | |_  | || (_) |  ___) |_   _| |_) | | | | | |_| | |_) | (_) | 
   // |_| |_|\__|  \__\___/  |____/  |_| |____/  |_| |_|\__, | .__/ \___(_)
   //                                                   |___/|_|           

   // calculate MLE for the signal hypothesis
 
   SigXS.setConstant(kFALSE);

   RooFitResult* fit = model.fitTo(*data,Hesse(kFALSE),Minos(kTRUE),
   Save(kTRUE),PrintLevel(-1),SumW2Error(kTRUE), Extended(kTRUE));
   fit->Print();
   Double_t NLLatMLE= fit->minNll();
   cout << "NLL with S+B hypothesis = " << NLLatMLE << endl;

   // plot data and signal hypothesis fit
   TCanvas* cdata;
   TCanvas* cNLL;
   if(drawFitResults) {
      cdata = new TCanvas("cdata","fit with signal hypothesis",500,500);
      frame = invMass.frame() ; 
      data->plotOn(frame, Binning("fullRange"), DataError(RooAbsData::Poisson) ) ;
      model.plotOn(frame) ;   
      model.plotOn(frame,Components(sigModel),LineStyle(kDashed), LineColor(kRed)) ;   
      model.plotOn(frame,Components(qcdModel),LineStyle(kDashed),LineColor(kBlack)) ;  
      model.paramOn(frame, Layout(0.43, 0.88, 0.92), Format("NEU",AutoPrecision(1)) ); 
      gPad->SetLogy();
      frame->GetXaxis()->SetRangeUser(360,2500);
      frame->GetYaxis()->SetNoExponent();
      frame->GetYaxis()->SetRangeUser(1E-2,1E+4);
      frame->GetYaxis()->SetTitleOffset(1.5);
      frame->GetYaxis()->SetTitle("Events / bin");
      frame->SetTitle("fit to data with signal hypothesis");
      frame->Draw() ;
      sprintf(title, "#chi^{2}/#nu = %.2f", frame->chiSquare());
      TLatex tex;
      tex.SetNDC();
      tex.SetTextAlign(12);
      tex.SetTextSize(0.04);
      tex.DrawLatex(0.22,0.88, title);
      sprintf(title, "plot_dijetmass_fitSplusB_%d.gif", (int) resMass);
      cdata->SaveAs(title);
      sprintf(title, "plot_dijetmass_fitSplusB_%d.eps", (int) resMass);
      cdata->SaveAs(title);
      sprintf(title, "plot_dijetmass_fitSplusB_%d.root", (int) resMass);
      cdata->SaveAs(title);


    // S h o w   r e s i d u a l   a n d   p u l l   d i s t s
    // -------------------------------------------------------
    
   //// Construct a histogram with the residuals of the data w.r.t. the curve
      RooHist* hresid = frame->residHist() ;
      // Create a new frame to draw the residual distribution 
      // and add the distribution to the frame
      RooPlot* frame2 = invMass.frame(Title("Residual Distribution")) ;
      frame2->addPlotable(hresid,"P") ;    

      ///// Construct a histogram with the pulls of the data w.r.t the curve
      RooHist* hpull = frame->pullHist() ;   

      //// Create a new frame to draw the pull distribution 
      //// and add the distribution to the frame
      RooPlot* frame3 = invMass.frame(Title("Pull Distribution")) ;
      frame3->addPlotable(hpull,"P") ;


      TCanvas* cResidual = new TCanvas("cResidual","Residual Distribution",1000,500);
      cResidual->Divide(2) ;
      cResidual->cd(1) ; 
      gPad->SetLeftMargin(0.15) ; 
      frame2->GetXaxis()->SetRangeUser(360,2500);
      frame2->GetYaxis()->SetTitleOffset(1.3) ; 
      frame2->GetYaxis()->SetTitle("Residual = Data - Fit");
      frame2->Draw() ;
      cResidual->cd(2) ; 
      gPad->SetLeftMargin(0.15) ; 
      frame3->GetYaxis()->SetTitleOffset(1) ; 
      frame3->GetXaxis()->SetRangeUser(360,2500);
      frame3->GetYaxis()->SetTitle("Pull = (Data - Fit) / Error");
      frame3->Draw() ;
      sprintf(title, "plot_dijetmass_residualSplusB_%d.gif", (int) resMass);
      cResidual->SaveAs(title);
      sprintf(title, "plot_dijetmass_residualSplusB_%d.eps", (int) resMass);
      cResidual->SaveAs(title);
      sprintf(title, "plot_dijetmass_residualSplusB_%d.root", (int) resMass);
      cResidual->SaveAs(title);


      // Draw likelihood as a function of signal cross section
      cNLL = new TCanvas("cNLL","Posterior PDF",500,500);
      RooAbsReal* nll = model.createNLL(*data) ;
      RooRealVar nllMin("nllMin","nllMin",NLLatMLE);
      RooAbsReal* lh = new RooFormulaVar("lh", "exp(-@0+@1)", RooArgList(*nll, nllMin));
      RooPlot* framea = SigXS.frame(0,1000,100) ;
      lh->plotOn(framea) ;
      framea->GetYaxis()->SetTitle("Likelihood / Max Likelihood");
      framea->GetXaxis()->SetNdivisions(505);
      framea->Draw() ;
      sprintf(title, "plot_dijetmass_PLLxsec_%d.gif", (int) resMass);
      cNLL->SaveAs(title);
      sprintf(title, "plot_dijetmass_PLLxsec_%d.eps", (int) resMass);
      cNLL->SaveAs(title);
      sprintf(title, "plot_dijetmass_PLLxsec_%d.root", (int) resMass);
      cNLL->SaveAs(title);
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

   return significance;
}








// set x = m/m0 in the range to 0.3 -- 1.3
void SetProperXRangeInDijetResonanceMassHistogram(TH1& hist, double mass) {

   const unsigned int numBins = hist.GetNbinsX();
   double x;

   for( int i=0; i<numBins; ++i) {
      x = hist.GetBinCenter(i+1);
      if(x/mass<0.28 || x/mass>1.32) {
         hist.SetBinContent(i+1, 0.0);
         hist.SetBinError(i+1, 0.0);
      }
   }

}
