#include <iostream>
#include <string.h>
#include <fstream>
#include <cmath>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include "Functions.h"
#include "Functions.cc"
#include "UserDefinedData.h"
using namespace std;

int main()
{
 
/////////////////////////////////////////////////////////////////////////
 char name[100];
 int j;
 double e,mRBarrel,eRBarrel,sRBarrel,seRBarrel,r,c;
 double mCaloPt,eCaloPt,sCaloPt,mGenPt,eGenPt,sGenPt;
 TFile *inf;
 TFile *outf; 
 TH1F *Response; 
 TH1F *Correction;
 TH1F *MeanGenPt;
 TH1F *MeanCaloPt; 
 TH1F *h;
 inf = new TFile(JetResponseFilename,"r"); 
 outf = new TFile(FitterOutputFilename,"RECREATE");
 TDirectory *dir_Response = (TDirectory*)outf->mkdir("fittedResponse");
 sprintf(name,"Response");
 Response = new TH1F(name,name,NGenPtBins-1,GenPt);
 sprintf(name,"Correction");
 Correction = new TH1F(name,name,NGenPtBins-1,GenPt);
 MeanGenPt = new TH1F("MeanGenPt","MeanGenPt",NGenPtBins-1,GenPt); 
 MeanCaloPt = new TH1F("MeanCaloPt","MeanCaloPt",NGenPtBins-1,GenPt);
 for (j=0; j<NGenPtBins; j++)//loop over GenPt bins
   {  
     cout<<"GenJetPt Bin: ["<<GenPt[j]<<","<<GenPt[j+1]<<"] GeV"<<endl; 
     sprintf(name,"recoZpt_%d_%d", (int) GenPt[j], (int) GenPt[j+1] );
     h = (TH1F*)inf->Get(name);
     GetMEAN(h,mGenPt,eGenPt,sGenPt);
     sprintf(name,"caloJetpt_%d_%d", (int) GenPt[j], (int) GenPt[j+1] );
     h = (TH1F*)inf->Get(name);
     GetMEAN(h,mCaloPt,eCaloPt,sCaloPt);
     sprintf(name,"responseHistReco_%d_%d", (int) GenPt[j], (int) GenPt[j+1] );
     h = (TH1F*)inf->Get(name);
     GetMPV(name,h,dir_Response,mRBarrel,eRBarrel,sRBarrel,seRBarrel);  
     ///////////////// GenPt ///////////////////////////////////
     MeanGenPt->SetBinContent(j+1,mGenPt);
     MeanGenPt->SetBinError(j+1,eGenPt);
     ///////////////// CaloPt ///////////////////////////////////
     MeanCaloPt->SetBinContent(j+1,mCaloPt);
     MeanCaloPt->SetBinError(j+1,eCaloPt);
     ////////////////// Absolute response //////////////////////////
     CalculateResponse(mGenPt,eGenPt,mRBarrel,eRBarrel,r,e);
     Response->SetBinContent(j+1,r);
     Response->SetBinError(j+1,e);
     ////////////////// Absolute correction //////////////////////////
     CalculateCorrection(mCaloPt,eCaloPt,mRBarrel,eRBarrel,c,e);
     Correction->SetBinContent(j+1,c);
     Correction->SetBinError(j+1,e);
     /////////////////////////////////////////////////////////////////
   }// end of GenPt loop  
   ////////////////////// writing ///////////////////////////////
   outf->cd();
   MeanGenPt->Write();
   MeanCaloPt->Write();
   Response->Write();
   Correction->Write();
   outf->Close();  
}
