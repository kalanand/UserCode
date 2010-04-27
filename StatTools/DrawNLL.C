
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
   RooRealVar alpha("alpha","cross-section ratio", 1., -100., 100.);
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
   // should be ratio of QCD function integral in range [400, 2000] / integral of data histogram 
   gPad->SetLogy();
   frame->GetXaxis()->SetNdivisions(505);
   frame->GetYaxis()->SetNoExponent();
   frame->GetYaxis()->SetRangeUser(1E-3,1E+4);
   frame->SetTitle("fit to data with null hypothesis");
   frame->Draw() ;




  // S c a n   - l o g ( L )   v s   cross section
  // ----------------------------------------------


   ///// compute Likelihood, NLL
   const int nPoints = 200;
   const float eStep  = 0.05;
   RooAbsReal* nll = model.createNLL( data) ;
   float mMU[nPoints];
   float mNLL[nPoints];
   float mLH[nPoints];
   float mUnNormLH[nPoints];
   alpha.setVal(0.0);
   float mNLL0 = nll->getVal();

   for (int j=0; j<nPoints; j++) {
     alpha.setVal( eStep*(j) );
     mMU[j] = xsection * alpha.getVal();
     mUnNormLH[j]  = exp( - nll->getVal() + mNLL0 );
   }

   // unnormalized likelihood => we need to compute its integral 
   // and normalize to unity
   TGraph* unlhGraph = new TGraph(nPoints, mMU, mUnNormLH);
   double intg = Integral(*unlhGraph);

   cout<< "un-normalized likelihood integral = " << intg << endl;

   for (int j=0; j<nPoints; j++) {
     mNLL[j] =  -log( mUnNormLH[j] / intg );
     mLH[j]  = mUnNormLH[j] / intg;
   }



   // TGraph of Likelihood vs cross section 
   TGraph* lhGraph = new TGraph(nPoints, mMU, mLH);
   lhGraph->SetMarkerColor(4);
   lhGraph->SetMarkerSize(0.5);
   lhGraph->GetXaxis()->SetTitle("cross section (pb)");
   lhGraph->GetYaxis()->SetTitle("Likelihood");


   cout<< "normalized likelihood integral = " << Integral(*lhGraph) << endl;

   // TGraph of NLL vs cross section  
   TGraph* nllGraph = new TGraph(nPoints, mMU, mNLL);
   nllGraph->SetMarkerColor(4);
   nllGraph->SetMarkerSize(0.5);
   nllGraph->GetXaxis()->SetTitle("cross section (pb)");
   nllGraph->GetYaxis()->SetTitle("-log (Likelihood)");


   // Draw the likelihood graphs
   TCanvas* cnll = new TCanvas("cnll","nll",500,500);
   nllGraph->Draw("AP");
   TCanvas* clh = new TCanvas("clh","lh",500,500);
   lhGraph->Draw("AP");
 

}





//______________________________________________________________________________
Double_t Integral(TGraph& graph, Int_t first=0, Int_t last=-1) const
{
   // Integrate the TGraph data within a given (index) range
   // NB: if last=-1 (default) last is set to the last point.
   //     if (first <0) the first point (0) is taken.
   //   : The graph segments should not intersect.
   //Method:
   // There are many ways to calculate the surface of a polygon. It all depends on what kind of data
   // you have to deal with. The most evident solution would be to divide the polygon in triangles and
   // calculate the surface of them. But this can quickly become complicated as you will have to test
   // every segments of every triangles and check if they are intersecting with a current polygon's
   // segment or if it goes outside the polygon. Many calculations that would lead to many problems...
   //      The solution (implemented by R.Brun)
   // Fortunately for us, there is a simple way to solve this problem, as long as the polygon's
   // segments don't intersect.
   // It takes the x coordinate of the current vertex and multiply it by the y coordinate of the next
   // vertex. Then it subtracts from it the result of the y coordinate of the current vertex multiplied
   // by the x coordinate of the next vertex. Then divide the result by 2 to get the surface/area.
   //      Sources
   //      http://forums.wolfram.com/mathgroup/archive/1998/Mar/msg00462.html
   //      http://stackoverflow.com/questions/451426/how-do-i-calculate-the-surface-area-of-a-2d-polygon


  Int_t fNpoints = graph.GetN();
  Double_t* fX = graph.GetX();
  Double_t* fY = graph.GetY();

   if (first < 0) first = 0;
   if (last < 0) last = fNpoints-1;
   if(last >= fNpoints) last = fNpoints-1;
   if (first >= last) return 0;
   Int_t np = last-first+1;
   Double_t sum = 0.0;
   for(Int_t i=first;i<=last;i++) {
      Int_t j = first + (i-first+1)%np;
      sum += TMath::Abs(fX[i]*fY[j]);
      sum -= TMath::Abs(fY[i]*fX[j]);
   }
   return 0.5*sum;
}
