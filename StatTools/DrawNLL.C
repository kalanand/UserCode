
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
 *   A standalone macro to plot -log (Likelihood) distribution as 
 *   a function of signal cross section for a bump hunting experiment. 
 *   The null hypothesis (say, QCD or Standard Model predictions) is a 
 *   a smooth function. 
 *   Works with  binned data. 
 *
 * Implementation details:
 *   Uses RooFit and RooStat classes.
 *   User is supposed to provide histogram of signal shape.
 *   Alternatively, user can specify analytic shape. 
 *  
 * History:
 *   
 *
 * Copyright (C) 2010 FNAL 
 ********************************************************************/


/* qstar cross section values as a function of mass
mass: 700., 800., 900., 1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000.
xsec: 356., 194., 111., 65.29, 39.56, 24.49, 15.42, 9.85,  6.37,  4.16,  2.74,  1.82,  1.21,  0.81 
*/

//  _   _                _               
// | | | | ___  __ _  __| | ___ _ __ ___ 
// | |_| |/ _ \/ _` |/ _` |/ _ \ '__/ __|
// |  _  |  __/ (_| | (_| |  __/ |  \__ \
// |_| |_|\___|\__,_|\__,_|\___|_|  |___/
// 


#include <memory>
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

const double minMass = 400.0;
const double maxMass = 2000.0;

// Read input file to get signal template and data
TFile *inputFile = TFile::Open( "/uscms_data/d2/sertac/forKalanand/"
				"Smooth_QCD_Background_withFit_at1pb_and_Resonance_Shape.root", "READ");
// Read histograms to get QCD background template. 
TH1D* hQCD = (TH1D *) inputFile->Get( "hQCD_CS_at_1pb" );




//  __  __       _         _____                 _   _             
// |  \/  | __ _(_)_ __   |  ___|   _ _ __   ___| |_(_) ___  _ __  
// | |\/| |/ _` | | '_ \  | |_ | | | | '_ \ / __| __| |/ _ \| '_ \ 
// | |  | | (_| | | | | | |  _|| |_| | | | | (__| |_| | (_) | | | |
// |_|  |_|\__,_|_|_| |_| |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|
// 

void DrawNLL(const double mResonance = 1000, const double xsection = 65.29) {

   hQCD->GetXaxis()->SetRangeUser(minMass, maxMass);
   // Our observable is the invariant mass
   RooRealVar invMass("invMass", "M_{Zjet}", minMass, maxMass,"GeV");
   RooDataHist data( "data", "", invMass, hQCD);

   // define your signal and background models here
   TString sig_hist_name = Form("h_Resonance_%d", (int) mResonance);
   TH1D* hist1 = (TH1D *) inputFile->Get( sig_hist_name );
   RooDataHist sigHist("sigHist","",  invMass, hist1);
   RooHistPdf sigModel("sigModel","sigModel", invMass,  sigHist);
   RooRealVar Nsig("Nsig", "# signal events", hist1->Integral());


   //////////////////////////////////////////////
   // make QCD model
   RooConstVar a0("a0","a0", 1.88e+16) ;
   RooConstVar a1("a1","a1", 3.975) ;  
   RooConstVar a2("a2","a2", 5.302) ; 
   RooConstVar a3("a3","a3", -1.51) ; 
   RooGenericPdf qcdModel("qcdModel", "@4*pow(1-@0/7000.+@3*(@0/7000.)*(@0/7000.),@1)*pow(@0,-@2)",
			  RooArgList(invMass,a1,a2,a3, a0)); 
   RooRealVar Nbkg("Nbkg", "# background events", hQCD->Integral() );


   //////////////////////////////////////////////
   // combined model
   // Introduce alpha: the signal strength in units of the expectation.  
   // eg. alpha = 1 is the SM, alpha = 0 is no signal, alpha=2 is 2x the SM
   RooRealVar alpha("alpha","cross-section ratio", 1., 0., 100.);
   RooFormulaVar ns("ns","alpha*Nsig",RooArgList(alpha,Nsig));

   // full model
   RooAddPdf model("model","sig+qcd background",RooArgList(sigModel,qcdModel),
   RooArgList(ns, Nbkg)); 



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




  // Plot data and QCD lineshape
  // ----------------------------------------------


   TCanvas* cdataNull = new TCanvas("cdataNull","data and QCD lineshape",500,500);
   RooPlot* frame = invMass.frame() ; 
   data.plotOn(frame ) ; 
   qcdModel.plotOn(frame, LineColor(kBlue), Normalization(0.65)) ;   
   // setting normalization by hand: 
   // should be ratio of QCD function integral in range [400, 2000] / integral of the data histogram 
   gPad->SetLogy();
   frame->GetXaxis()->SetNdivisions(505);
   frame->GetYaxis()->SetNoExponent();
   frame->GetYaxis()->SetRangeUser(1E-3,1E+4);
   frame->SetTitle("fit to data with null hypothesis");
   frame->Draw() ;




  // S c a n   - l o g ( L )   v s   cross section
  // ----------------------------------------------


   ///// plot NLL
   RooAbsReal* nll = model.createNLL( data) ;
   float mMU[10000];
   float mNLL[10000];

   for (int j=0; j<10000; j++) {
     alpha.setVal(0.01*j);
     mMU[j] = xsection * alpha.getVal();
     mNLL[j] = nll->getVal();
   }

   TGraph* nllGraph = new TGraph(10000, mMU, mNLL);
   nllGraph->SetMarkerColor(4);
   nllGraph->SetMarkerSize(0.5);
   nllGraph->GetXaxis()->SetTitle("cross section (pb)");
   nllGraph->GetYaxis()->SetTitle("-log (Likelihood)");
   TCanvas* cnll = new TCanvas("cnll","nll",500,500);
   nllGraph->Draw("AP");
   gPad->SetLogx();

}





