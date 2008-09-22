#include <iostream>
#include <string.h>
#include <fstream>
#include <cmath>
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
using namespace std;

void GetMPV(char name[100],TH1F* histo, TDirectory* Dir, double& peak, double& error, double& sigma, double& err_sigma)
{
  double norm,mean,rms,integral,lowlimit,highlimit,LowResponse,HighResponse;
  int k;
 
  LowResponse = histo->GetXaxis()->GetXmin();
  HighResponse = histo->GetXaxis()->GetXmax();
  Dir->cd();
  TF1 *g;
  TStyle *myStyle = new TStyle("mystyle","mystyle");
  myStyle->Reset();
  myStyle->SetOptFit(1111);
  myStyle->SetOptStat(2200);
  myStyle->SetStatColor(0);
  myStyle->SetTitleFillColor(0);
  myStyle->cd(); 
  integral = histo->Integral();
  mean = histo->GetMean();
  rms = histo->GetRMS();
 
  if (integral>0)
    { 
      lowlimit = TMath::Max(LowResponse,mean-2*rms);
      highlimit= TMath::Min(mean+2*rms,HighResponse); 
      norm = histo->GetMaximumStored();
      peak = mean;
      sigma = rms;
      for (k=0; k<3; k++)
       {
         g = new TF1("g","gaus",lowlimit, highlimit);
         g->SetParNames("N","#mu","#sigma");
         g->SetParameter(0,norm);
         g->SetParameter(1,peak);
         g->SetParameter(2,sigma);
         lowlimit = TMath::Max(LowResponse,peak-1.5*sigma);
         highlimit= TMath::Min(peak+1.5*sigma,HighResponse); 
         g->SetRange(lowlimit,highlimit);
         histo->Fit(g,"RQ");
         norm = g->GetParameter(0);
         peak = g->GetParameter(1);
         error= g->GetParError(1);
         err_sigma = g->GetParError(2);
         sigma = g->GetParameter(2);  
       }
      
      if (g->GetNDF()<0)
        {
          cout<<"FIT FAILURE: histogram "<<name<<"...Using MEAN and RMS."<<endl;
          peak = mean;
          sigma = rms;
          error = histo->GetMeanError();
          err_sigma = histo->GetRMSError();
        }
    }
  else
    {
      peak = 0;
      sigma = 0;
      error = 0;
      err_sigma = 0;
    }
  histo->Write();
}
//////////////////////////////////////////////////////////////////////
void GetClosureMPV(char name[100],TH1F* histo, TDirectory* Dir, double& peak, double& error, double& sigma, double& err_sigma)
{
  double norm,mean,rms,integral,lowlimit,highlimit,LowResponse,HighResponse;
  int k;
  double a = 1;
  LowResponse = histo->GetXaxis()->GetXmin();
  HighResponse = histo->GetXaxis()->GetXmax();
  Dir->cd();
  TF1 *g;
  TStyle *myStyle = new TStyle("mystyle","mystyle");
  myStyle->Reset();
  myStyle->SetOptFit(1111);
  myStyle->SetOptStat(2200);
  myStyle->SetStatColor(0);
  myStyle->SetTitleFillColor(0);
  myStyle->cd(); 
  integral = histo->Integral();
  mean = histo->GetMean();
  rms = histo->GetRMS();
  
  if (integral>0)
    { 
      lowlimit = TMath::Max(LowResponse,mean-1.5*rms);
      highlimit= TMath::Min(mean+1.5*rms,HighResponse); 
      norm = histo->GetMaximumStored();
      peak = mean;
      sigma = rms;
      for (k=0; k<3; k++)
       {
         g = new TF1("g","gaus",lowlimit, highlimit);
         g->SetParNames("N","#mu","#sigma");
         g->SetParameter(0,norm);
         g->SetParameter(1,peak);
         g->SetParameter(2,sigma);
         lowlimit = TMath::Max(LowResponse,peak-a*sigma);
         highlimit= TMath::Min(peak+a*sigma,HighResponse); 
         g->SetRange(lowlimit,highlimit);
         histo->Fit(g,"RQ");
         norm = g->GetParameter(0);
         peak = g->GetParameter(1);
         sigma = g->GetParameter(2);  
       }
      if (g->GetNDF()>5)
        {
          peak = g->GetParameter(1);
          sigma = g->GetParameter(2);
          error = histo->GetMeanError();
          err_sigma = histo->GetRMSError();
        }
      else
        {
          cout<<"FIT FAILURE: histogram "<<name<<"...Using MEAN and RMS."<<endl;
          peak = mean;
          sigma = rms;
          error = histo->GetMeanError();
          err_sigma = histo->GetRMSError();
        }
    }
  else
    {
      peak = 0;
      sigma = 0;
      error = 0;
      err_sigma = 0;
    }
  histo->Write();
}
//////////////////////////////////////////////////////////////////////
void GetMEAN(TH1F* histo, double& peak, double& error, double& sigma)
{
  double N = histo->Integral();
  if (N>2)
    {
      peak  = histo->GetMean();
      sigma = histo->GetRMS();
      error = histo->GetMeanError();
    }
  else
    {
      peak = 0;
      sigma = 0;
      error = 0; 
    }  
}
///////////////////////////////////////////////////////////////////////
void CalculateResponse(double GenPt, double eGenPt, double DPt, double eDPt, double& r, double& e)
{
  if (GenPt>0 && fabs(DPt)>0)
    {
      r = DPt;
      e = eDPt; 
    }
  else
    {
      r = 0;
      e = 0;
    }    
}
///////////////////////////////////////////////////////////////////////
void CalculateCorrection(double CaloPt, double eCaloPt, double DPt, double eDPt, double& c, double& e)
{
  if (CaloPt>0 && fabs(DPt)>0)
    {
      c = 1.0/DPt;
      e = c*c*eDPt; 
    }
  else
    {
      c = 0;
      e = 0;
    }    
}
///////////////////////////////////////////////////////////////////////
void Invert(TF1* f, double Min, double Max, double y, double& x)
{
  int i=0;
  double tmp,r; 
  x = y;
  for(i=0;i<50;i++)
   {     
     if (x<Min)
       r = f->Eval(Min);
     if (x>Max)   
       r = f->Eval(Max);
     if (x>=Min || x<=Max)
       r = f->Eval(x);
     tmp = y/r;
     if (tmp<1)// Math constraint due to the presence of log(pt) in function f.
       {
         x = 0;
         break;
       }
     else
       x = tmp;
   }     
}
