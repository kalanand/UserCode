
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
 *   Plots dijet mass distributions in data and MC and fits them to QCD lineshape 
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


//  _____ _              _   ____                                _                
// |  ___(_)_  _____  __| | |  _ \ __ _ _ __ __ _ _ __ ___   ___| |_ ___ _ __ __ _ 
// | |_  | \ \/ / _ \/ _` | | |_) / _` | '__/ _` | '_ ` _ \ / _ \ __/ _ \ '__/ _ _|
// |  _| | |>  <  __/ (_| | |  __/ (_| | | | (_| | | | | | |  __/ ||  __/ |  \__ \
// |_|   |_/_/\_\___|\__,_| |_|   \__,_|_|  \__,_|_| |_| |_|\___|\__\___|_|  |___/
// 

const int nMassBins = 29;
double massBoundaries[nMassBins+1] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 
				      74, 88, 103, 119, 137, 156, 176, 197, 
				      220, 244, 270, 296, 325, 354, 386, 419, 
				      453, 489, 526, 565};

TCut GoodEvent = "eventVar.RunNo == 132440 || eventVar.RunNo == 132473"
  " || eventVar.RunNo == 132599 || eventVar.RunNo == 132601 ||"
  " eventVar.RunNo == 132602 || eventVar.RunNo == 132605";
TCut eventCut ="eventVar.PVntracks>3 && abs(eventVar.PVz)<15"
  " && jet1Var.corPt>20. && eventVar.njets>1 && jet2Var.corPt>20."
  " && abs(abs(jet1Var.phi - jet2Var.phi)-3.1416)<1.";
   
TCut jetidCut = "jet1Var.emf>0.01 && jet2Var.emf>0.01 && jet1Var.n90hits>1"
  " && jet2Var.n90hits>1 && jet1Var.fHPD<0.98 && jet2Var.fHPD<0.98";

TCut eta_he = "abs(jet1Var.eta)<1.3 && abs(jet2Var.eta)<1.3";


//  __  __       _         _____                 _   _             
// |  \/  | __ _(_)_ __   |  ___|   _ _ __   ___| |_(_) ___  _ __  
// | |\/| |/ _` | | '_ \  | |_ | | | | '_ \ / __| __| |/ _ \| '_ \ 
// | |  | | (_| | | | | | |  _|| |_| | | | | (__| |_| | (_) | | | |
// |_|  |_|\__,_|_|_| |_| |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|
// 

void FitDijetMass_Data() {

  
  TFile *inf  = new TFile("/uscms_data/d2/sertac/data/7TeVCollisions/Data/"
			  "ProcessedTree_ak7calo_till_R132605.root");
  TFile *mcinf   = new TFile("/uscms_data/d2/kkousour/7TeV/MinBias/"
			  "ProcessedTree_ak7calo_MinBias.root");


  //-------------- Book Histograms ----------------------
 
  TH1F *hCorMass     = new TH1F("hCorMass","DijetMass",nMassBins,massBoundaries);
  TH1F *hCorMass_mc  = new TH1F("hCorMass_mc","DijetMass",nMassBins,massBoundaries);
 
  TTree* tr = (TTree*) inf->Get("tr");
  TTree* tr_mc = (TTree*) mcinf->Get("tr");
  tr->Draw("eventVar.corMass>>hCorMass",eventCut && GoodEvent && jetidCut && eta_he,"goff");
  tr_mc->Draw("eventVar.corMass>>hCorMass_mc",eventCut && jetidCut && eta_he,"goff");


  // set right normalization
  double n_mc = hCorMass_mc->GetEntries();
  double n_data = hCorMass->GetEntries();
   
  hCorMass_mc->Scale(n_data/n_mc);
  hCorMass_mc->SetFillColor(19);
  hCorMass_mc->SetLineColor(2);
  hCorMass_mc->SetMarkerColor(2);
  hCorMass_mc->SetXTitle("Corrected Dijet Mass (GeV)");
  hCorMass_mc->SetYTitle("Events / (1 GeV)");
  hCorMass->SetMarkerStyle(20);
  hCorMass_mc->GetXaxis()->SetRangeUser(0..,600.);
    

  for(int i=1; i<30; i++){
    hCorMass->SetBinContent(i+1, hCorMass->GetBinContent(i+1) / hCorMass->GetBinWidth(i+1));
    hCorMass->SetBinError(i+1, hCorMass->GetBinError(i+1) / hCorMass->GetBinWidth(i+1));

    hCorMass_mc->SetBinContent(i+1, hCorMass_mc->GetBinContent(i+1) / hCorMass_mc->GetBinWidth(i+1));
    hCorMass_mc->SetBinError(i+1, hCorMass_mc->GetBinError(i+1) / hCorMass_mc->GetBinWidth(i+1));

  } 



  // Our observable is the invariant mass
   RooRealVar invMass("invMass", "Corrected dijet mass", 1., 565.,"GeV");
   //  RooRealVar invMassmc("invMassmc", "Corrected dijet mass", 1., 565.,"GeV");
   RooDataHist data( "data", "", invMass, hCorMass);
   RooDataHist mc( "mc", "", invMass, hCorMass_mc);

   //////////////////////////////////////////////
   // make QCD model
   RooRealVar p1("p1","p1", -16.63, -50., 50.) ;  
   RooRealVar p2("p2","p2", 15.3, 0., 50.) ; 
   RooRealVar p3("p3","p3", 3.0, -10., 3.8) ; 
   RooGenericPdf qcdModel("qcdModel",
   "pow(1-invMass/7000.,p1)/pow(invMass/7000.,p2+p3*log10(invMass/7000.))",
   RooArgList(invMass,p1,p2, p3)); 
   RooRealVar p0("p0", "# events", 10.0, 0.0, 10000000.0);
   // full model
   RooAddPdf model("model","qcd",RooArgList(qcdModel), RooArgList(p0)); 



   //////////////////////////////////////////////
   // Repeat PDF definition for MC
   RooRealVar p1mc("p1mc","p1", -16.63, -50., 50.) ;  
   RooRealVar p2mc("p2mc","p2", 15.3, 0., 50.) ; 
   RooRealVar p3mc("p3mc","p3", 3.0, -10., 3.8) ; 
   RooGenericPdf qcdModelmc("qcdModelmc",
   "pow(1-invMass/7000.,p1mc)/pow(invMass/7000.,p2mc+p3mc*log10(invMass/7000.))",
   RooArgList(invMass,p1mc,p2mc, p3mc)); 
   RooRealVar p0mc("p0mc", "# events", 10.0, 0.0, 10000000.0);
   RooAddPdf modelmc("modelmc","qcd",RooArgList(qcdModelmc), RooArgList(p0mc)); 



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


   model.fitTo(data, Hesse(kFALSE),Minos(kTRUE),
	       PrintLevel(-1),SumW2Error(kTRUE), Extended(kTRUE), 
	       Range(60, 500) );

   modelmc.fitTo(mc, Hesse(kFALSE),Minos(kTRUE),
		 PrintLevel(-1),SumW2Error(kTRUE), Extended(kTRUE), 
		 Range(60, 500) );



//    TCanvas* mcNull = new TCanvas("mcNull","dummy canvas for MC",500,500);
//    RooPlot* framemc = invMass.frame() ; 
//    mc.plotOn(framemc ) ; 
//    modelmc.plotOn(framemc, LineColor(kRed)) ;
//    framemc->Draw() ;
//    framemc->GetYaxis()->SetRangeUser(1E-3,1E+3);
//    gPad->SetLogy();


   //plot data 
   TCanvas* cdataNull = new TCanvas("cdataNull","fit to dijet mass",500,500);
   RooPlot* frame = invMass.frame() ; 
   frame->addTH1(hCorMass_mc, "hist");
   frame->addObject( &modelmc, "");
   data.plotOn(frame ) ; 
   model.plotOn(frame, LineColor(kBlue)) ;   
   model.paramOn(frame, Layout(0.4, 0.85, 0.92)); 
   TPaveText* dataPave = (TPaveText*) frame->findObject("model_paramBox");
   dataPave->SetY1(0.77);
   modelmc.plotOn(frame, LineColor(kRed)) ;
   modelmc.paramOn(frame, Layout(0.4, 0.65, 0.77)); 
   mcPave->SetY1(0.62);
   TPaveText* mcPave = (TPaveText*) frame->findObject("modelmc_paramBox");
   mcPave->SetTextColor(2);
   mcPave->SetLineColor(2);
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





