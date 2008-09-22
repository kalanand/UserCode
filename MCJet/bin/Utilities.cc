#include <iostream>
#include <string.h>
#include <fstream>
#include <cmath>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TStyle.h>
#include <TMath.h>
using namespace std;

void GetMPV(char name[100],TH1F* histo, TDirectory* Dir, double& peak, double& error, double& sigma, double& err_sigma);
void GetMEAN(TH1F* histo, double& peak, double& error, double& sigma);
void CalculateResponse(bool UseRatioForResponse, double x, double ex, double y, double ey, double& r, double& e);
void CalculateCorrection(bool UseRatioForResponse, double x, double ex, double y, double ey, double& c, double& e);
void Invert(TF1* f, double Min, double Max, double y, double& x);
bool HistoExists(vector<string> LIST, string hname);
int getBin(double x, vector<double> boundaries);

//////////////////////////////////////////////////////////////////////
void GetMPV(char name[100],TH1F* histo, TDirectory* Dir, double& peak, double& error, double& sigma, double& err_sigma)
{
  double norm,mean,rms,integral,lowlimit,highlimit,LowResponse,HighResponse,a;
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
  a = 1.5;
  if (integral>0)
    { 
      lowlimit = TMath::Max(LowResponse,mean-a*rms);
      highlimit= TMath::Min(mean+a*rms,HighResponse); 
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
          error = g->GetParError(1);
          err_sigma = g->GetParError(2);
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
void CalculateResponse(bool UseRatioForResponse, double x, double ex, double y, double ey, double& r, double& e)
{
  if (x>0 && fabs(y)>0)
    {
      if (UseRatioForResponse)
        {
          r = y;
          e = ey;
        }
      else
        {  
          r = (x+y)/x;
          e = fabs(r-1.)*sqrt(pow(ey/y,2)+pow(ex/x,2)); 
        }
    }
  else
    {
      r = 0;
      e = 0;
    }    
}
///////////////////////////////////////////////////////////////////////
void CalculateCorrection(bool UseRatioForResponse, double x, double ex, double y, double ey, double& c, double& e)
{
  if (x>0 && fabs(y)>0)
    {
      if (UseRatioForResponse)
        {
          c = 1./y;
          e = ey/(y*y);
        }
      else
        {  
          c = x/(x+y);
          e = (fabs(x*y)/pow(x+y,2))*sqrt(pow(ey/y,2)+pow(ex/x,2)); 
        }
    }
  else
    {
      c = 0;
      e = 0;
    }    
}
///////////////////////////////////////////////////////////////////////
bool HistoExists(vector<string> LIST, string hname)
{
  unsigned int i,N;
  bool found(false);
  N = LIST.size();
  if (N==0)
    cout<<"WARNING: empty file histogram list!!!!"<<endl;
  else
    for(i=0;i<N;i++)
     if (hname==LIST[i])
       found = true;
  if (!found)
    cout<<"Histogram: "<<hname<<" NOT FOUND!!! Check list of existing objects."<<endl;
  return found;
}
///////////////////////////////////////////////////////////////////////
int getBin(double x, vector<double> boundaries)
{
  int i;
  int n = boundaries.size()-1;
  if (n<=0) return -1;
  if (x<boundaries[0] || x>=boundaries[n])
    return -1;
  for(i=0;i<n;i++)
   {
     if (x>=boundaries[i] && x<boundaries[i+1])
       return i;
   }
  return 0; 
}
///////////////////////////////////////////////////////////////////////
