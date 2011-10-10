/*******************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: Presently in the user code
 *
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *   Kellen McGee, Johns Hopkins
 *   Kristina Krylova, Buffalo
 *
 * Description:
 *
 * Implementation details:
 *  - Uses standard ROOT classes.
 *  - Script takes the root files for the normal and 
 *    anomalous couplings as inputs, and compares their distributions.
 *
 * History:
 *  - Kristina+Kellen wrote the initial macro to plot 
 *    kinematic distributions for various aTGC values.
 *  - Kalanand made several changes and made it compilable
 *    to improve the execution time.
 *
 * Copyright (C) 2011 FNAL 
 ********************************************************************/


#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include <stdio.h>
#include <string.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCut.h>
#include <TStyle.h>
#include <TString.h> 
#include <TLegend.h>
#include <TPaveText.h>
#include <TLatex.h>
#include <iostream>

using namespace std; 


const string inputDirPath = "/uscms_data/d3/kkrylova/MCFM/Bin/";
const string f0Name = "WW_NLO_norm";
string f1Name;
string f2Name;
string f3Name;
string f4Name;
string f5Name;
string f6Name;
const string treeName = "h300";

TFile* f0;
TFile* f1;
TFile* f2;
TFile* f3;
TFile* f4;
TFile* f5;
TFile* f6;

TTree* tree0;
TTree* tree1;
TTree* tree2;
TTree* tree3;
TTree* tree4;
TTree* tree5;
TTree* tree6;


const string var1 = "sqrt(px5*px5+py5*py5)";
const string histName1 = "Jet1pT";
const string title1 = "Jet 1 P_{T} [GeV]";

const string var2 = "sqrt(px4*px4+py4*py4)";
const string histName2 = "LeptonpT";
const string title2 = "Lepton P_{T} [GeV]";


const string var3 = "sqrt((px3+px4)*(px3+px4)+(py3+py4)*(py3+py4))";
const string histName3 = "LeptonicWpT";
const string title3 = "Lept. W P_{T} [GeV]";


const string var4 = "sqrt((px5+px6)*(px5+px6)+(py5+py6)*(py5+py6))";
const string histName4 = "HadronicWpT";
const string title4 = "Hadromic W P_{T} [GeV]";

const string var5 = "sqrt((E3+E4+E5+E6)*(E3+E4+E5+E6)-((px3+px4+px5+px6)*(px3+px4+px5+px6)+(py3+py4+py5+py6)*(py3+py4+py5+py6)+(pz3+pz4+pz5+pz6)*(pz3+pz4+pz5+pz6)))";
const string histName5 = "Mass4Body";
const string title5 = "M_{lnujj} [GeV]";


/////////// define all member functions here first ////////////////
TH1D* aesthetics( TH1D* hist, Int_t col_num);
void title_binning( const char title[], int bins, 
		    float min, float max );
TLegend* legend( TH1D* hist1, TH1D* hist2, TH1D* hist3, 
		 TH1D* hist4, TH1D* hist5, 
		 TH1D* hist6, TH1D*hist7);
void scaling( TH1D* hist, Int_t dm_max, bool doTruncation=false );
TH1D* draw_hist( Int_t varIndex, char* name, TTree* tree, 
		 Int_t color, bool doYaxisTitle=false );
TH1D* get_ratio( TH1D* hist_denominator, TH1D* hist_numerator);
void  MakeComparisonPlot( Int_t varIndex, bool doLogY=false );





////////////// primary function //////////////////////////
void anomalousTGC(string coupling="Delta_K_gamma") {
  f1Name = coupling+"-0.15d0";
  f2Name = coupling+"-0.1d0";
  f3Name = coupling+"-0.05d0";
  f4Name = coupling+"0.05d0";
  f5Name = coupling+"0.1d0";
  f6Name = coupling+"0.15d0";

  f0 = new TFile( (inputDirPath+f0Name+"/"+f0Name+".root").c_str(), "read");
  tree0 = (TTree*)f0->Get(treeName.c_str());
  f1 = new TFile( (inputDirPath+f1Name+"/"+f1Name+".root").c_str(), "read");
  tree1 = (TTree*)f1->Get(treeName.c_str());
  f2 = new TFile( (inputDirPath+f2Name+"/"+f2Name+".root").c_str(), "read");
  tree2 = (TTree*)f2->Get(treeName.c_str());
  f3 = new TFile( (inputDirPath+f3Name+"/"+f3Name+".root").c_str(), "read");
  tree3 = (TTree*)f3->Get(treeName.c_str());
  f4 = new TFile( (inputDirPath+f4Name+"/"+f4Name+".root").c_str(), "read");
  tree4 = (TTree*)f4->Get(treeName.c_str());
  f5 = new TFile( (inputDirPath+f5Name+"/"+f5Name+".root").c_str(), "read");
  tree5 = (TTree*)f5->Get(treeName.c_str());
  f6 = new TFile( (inputDirPath+f6Name+"/"+f6Name+".root").c_str(), "read");
  tree6 = (TTree*)f6->Get(treeName.c_str());


  ///////////// Print out the tree names and number of entries etc. /////////////

  cout << f0Name.c_str() << " : "  << treeName.c_str() << " : " << tree0->GetEntries() << endl;
  cout << f1Name.c_str() << " : "  << treeName.c_str() << " : " << tree1->GetEntries() << endl;
  cout << f2Name.c_str() << " : "  << treeName.c_str() << " : " << tree2->GetEntries() << endl;
  cout << f3Name.c_str() << " : "  << treeName.c_str() << " : " << tree3->GetEntries() << endl;
  cout << f4Name.c_str() << " : "  << treeName.c_str() << " : " << tree4->GetEntries() << endl;
  cout << f5Name.c_str() << " : "  << treeName.c_str() << " : " << tree5->GetEntries() << endl;
  cout << f6Name.c_str() << " : "  << treeName.c_str() << " : " << tree6->GetEntries() << endl;
  //////////////////////////////////////////////////////////////////////////////////


  // Filling histograms
  MakeComparisonPlot(1, true);
//   MakeComparisonPlot(2, true);
//   MakeComparisonPlot(3, true);
//   MakeComparisonPlot(4, true);
//   MakeComparisonPlot(5, true);

}








TH1D* aesthetics( TH1D* hist, Int_t col_num){
  hist->SetLineColor(col_num);
  hist->SetMarkerColor(col_num);
  hist->GetYaxis()->SetTitleOffset(1.15);
  return hist;
} // aesthetics




void title_binning( const char title[], int bins, float min, float max ) {
    sprintf((char*) title, (char*) "Number of Events / %.1f GeV", ((max - min)/bins) );
  }





TLegend* legend( TH1D* hist1, TH1D* hist2, TH1D* hist3, TH1D* hist4, TH1D* hist5, TH1D* hist6,
		 TH1D*hist7){
  Double_t x1=0.7, y1=0.59, x2=x1+0.2, y2=y1+0.3;
  TLegend *l = new TLegend( x1, y1, x2, y2 );
  l->AddEntry(hist1, "SM", "lp");
  l->AddEntry(hist2, "#Delta#kappa #gamma -0.15", "lp");
  l->AddEntry(hist3, "#Delta#kappa #gamma -0.10", "lp");
  l->AddEntry(hist4, "#Delta#kappa #gamma -0.05", "lp");
  l->AddEntry(hist5, "#Delta#kappa #gamma 0.05", "lp");
  l->AddEntry(hist6, "#Delta#kappa #gamma 0.10", "lp");
  l->AddEntry(hist7, "#Delta#kappa #gamma 0.15", "lp");
  l->SetFillColor(0);
  return l;
} // legend







void scaling( TH1D* hist, Int_t dm_max, bool doTruncation){

  if(doTruncation) hist->GetXaxis()->SetRangeUser( 0.1, dm_max ); 
  Double_t scale = 1.0;
  Double_t integral = hist->Integral();
  if(integral !=0.0) scale=1.0/integral;
  hist->Scale(scale);
} // scaling






TH1D* draw_hist(  Int_t varIndex, char* name, TTree* tree, 
		  Int_t color, bool doYaxisTitle ){

  string variable;
  string plotname;
  string axis_label;
  Int_t bins; 
  Float_t dm_min; 
  Float_t dm_max;

  //////// define the histogram binning etc.////////////
  if(varIndex==1) {
    variable = var1;
    plotname = histName1;
    axis_label = title1;
  }
  else if(varIndex==2) {
    variable = var2;
    plotname = histName2;
    axis_label = title2;
  }
  else if(varIndex==3) {
    variable = var3;
    plotname = histName3;
    axis_label = title3;
  }
  else if(varIndex==4) {
    variable = var4;
    plotname = histName4;
    axis_label = title4;
  }
  else if(varIndex==5) {
    variable = var5;
    plotname = histName5;
    axis_label = title5;
  }
  else return 0;

  if(varIndex==5) {
    bins = 30; 
    dm_min = 150.; 
    dm_max = 500.;
  }
  else {
    bins = 30; 
    dm_min = 50.; 
    dm_max = 500.;
  }

  //////////// now make histogram ////////////////////////
  TString histname = TString(plotname.c_str()) + 
    TString(varIndex) + TString(name);
  TH1D* hist = new TH1D(histname, "", bins, dm_min, dm_max);
  aesthetics( hist, color );
  hist->Sumw2();
  tree->Draw( TString(variable.c_str())+TString(">>")+histname, "", "goff");
  scaling( hist, dm_max );
  hist->GetXaxis()->SetTitle(axis_label.c_str());
  if (doYaxisTitle){
    hist->SetNdivisions(505);
    const char title[100]="";
    title_binning(title, bins, dm_min, dm_max);
    hist->GetYaxis()->SetTitle(title);
  } // if
  return hist;
} // draw_hist






TH1D* get_ratio( TH1D* hist_denominator, TH1D* hist_numerator)
{
  TString histname = TString(hist_numerator->GetName()) + TString("_ratio");
  TH1D* hist = (TH1D*) hist_numerator->Clone(histname);
  hist->Divide( hist_denominator);
  hist->GetYaxis()->SetMoreLogLabels(true);
  hist->GetYaxis()->SetNoExponent(true);
  hist->GetYaxis()->SetTitle("Ratio");
  return hist;
} // get_ratio



void  MakeComparisonPlot( Int_t varIndex, bool doLogY ){

  string plotname;
  if(varIndex==1) plotname = histName1;
  else if(varIndex==2) plotname = histName2;
  else if(varIndex==3) plotname = histName3;
  else if(varIndex==4) plotname = histName4;
  else if(varIndex==5) plotname = histName5;
  else return;


  // get and draw the basic graphs for comparisons with anomalous couplings
  TH1D* hist1 = draw_hist( varIndex, (char*)"-0.0",  tree0, 1, true );
  TH1D* hist2 = draw_hist( varIndex, (char*)"-0.15", tree1, 2, false );
  TH1D* hist3 = draw_hist( varIndex, (char*)"-0.10", tree2, 4, false );
  TH1D* hist4 = draw_hist( varIndex, (char*)"-0.05", tree3, 6, false );
  TH1D* hist5 = draw_hist( varIndex, (char*)"0.05",  tree4, 7, false );
  TH1D* hist6 = draw_hist( varIndex, (char*)"0.10",  tree5, 8, false );
  TH1D* hist7 = draw_hist( varIndex, (char*)"0.15",  tree6, 9, false );


  TCanvas* c1 = new TCanvas( ("can1_"+plotname).c_str(), plotname.c_str(), 500, 500);
  gStyle->SetOptStat(0);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.16);
  //gStyle->SetPadRightMargin(0.28);

  //  if(doLogY) hist1->SetMinimum(0.00001);
  hist1->Draw("c");
  hist2->Draw("csame");
  hist3->Draw("csame");
  hist4->Draw("csame");
  hist5->Draw("csame");
  hist6->Draw("csame");
  hist7->Draw("csame");
  if(doLogY) c1->SetLogy();
  // Draw the legend
  TLegend* l = legend( hist1, hist2, hist3, hist4, hist5, hist6, hist7 );
  l->Draw();
  c1->Print( (plotname+".pdf").c_str() );

  // get the ratio between between the normal and each of the individual anomalous couplings

  TH1D* ratio_hist1 = get_ratio( hist1, hist2 );
  TH1D* ratio_hist2 = get_ratio( hist1, hist3 );
  TH1D* ratio_hist3 = get_ratio( hist1, hist4 );
  TH1D* ratio_hist4 = get_ratio( hist1, hist5 );
  TH1D* ratio_hist5 = get_ratio( hist1, hist6 );
  TH1D* ratio_hist6 = get_ratio( hist1, hist7 );
 

  TCanvas* c2 = new TCanvas( ("can2_"+plotname).c_str(), plotname.c_str(), 500, 500);
  ratio_hist1->SetLineColor(2);
  ratio_hist2->SetLineColor(4);
  ratio_hist3->SetLineColor(6);
  ratio_hist4->SetLineColor(7);
  ratio_hist5->SetLineColor(8);
  ratio_hist6->SetLineColor(9);
  

  ratio_hist1->Draw("c");
  ratio_hist2->Draw("csame");
  ratio_hist3->Draw("csame");
  ratio_hist4->Draw("csame");
  ratio_hist5->Draw("csame");
  ratio_hist6->Draw("csame");
  if(doLogY) c2->SetLogy();
  // Draw the legend
  TLegend* l2 = legend( hist1, hist2, hist3, hist4, hist5, hist6, hist7 );
  l2->Draw();
  c2->Print( (plotname+"_ratio.pdf").c_str() );


  delete c1;
  delete c2;
} // MakeComparisonPlot


