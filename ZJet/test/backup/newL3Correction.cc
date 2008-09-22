#include <iostream>
#include <fstream>
#include <string.h>
#include <cmath>
#include <iomanip>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TMath.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include "UserDefinedData.h"
#include "Functions.h"
#include "Functions.cc"
using namespace std;

int main()
{
 char name[100];
 int i,auxi_cor,auxi_resp;
 double Correction[NGenPtBins],CorrectionError[NGenPtBins],Response[NGenPtBins],ResponseError[NGenPtBins];
 double xCaloPt[NGenPtBins],exCaloPt[NGenPtBins],xGenPt[NGenPtBins],exGenPt[NGenPtBins];
 double MinCaloPt,MaxCaloPt,MinGenPt,MaxGenPt;
 double cor,e_cor,resp,e_resp; 
 TFile *inf;
 TFile *outf;
 TH1F *hCor,*hResp,*hGen,*hCalo;
 TF1 *CorFit;
 TF1 *RespFit;
 TGraphErrors *g_Cor;
 TGraphErrors *g_Resp;
 ofstream L3CorrectionFile,L3ResponseFile;
 L3CorrectionFile.open(L3CorrectionFilename);
 L3ResponseFile.open(L3ResponseFilename);
 inf = new TFile(FitterOutputFilename,"r");
 ///////////// CaloPt /////////////////////////////////////
 sprintf(name,"MeanCaloPt");
 hCalo = (TH1F*)inf->Get(name);
 ///////////// GenPt /////////////////////////////////////
 sprintf(name,"MeanGenPt");
 hGen = (TH1F*)inf->Get(name);
 ///////////////////////// Correction //////////////////////////////////////////////
 sprintf(name,"Correction");
 hCor = (TH1F*)inf->Get(name);
 sprintf(name,"Response");
 hResp = (TH1F*)inf->Get(name);
 auxi_cor=0;
 auxi_resp=0;
 cout<<"GenPt bin"<<setw(12)<<"<CaloPt>"<<setw(12)<<"Correction"<<setw(12)<<"Error"<<setw(12)<<"<GenPt>"<<setw(12)<<"Response"<<setw(12)<<"Error"<<endl; 
 for(i=0;i<NGenPtBins;i++)
   { 
     cor = hCor->GetBinContent(i+1);
     e_cor = hCor->GetBinError(i+1);
     cout<<"["<<GenPt[i]<<","<<GenPt[i+1]<<"]"<<setw(12)<<hCalo->GetBinContent(i+1)<<setw(12)<<cor<<setw(12)<<e_cor;
     resp = hResp->GetBinContent(i+1);
     e_resp = hResp->GetBinError(i+1);
     cout<<setw(12)<<hGen->GetBinContent(i+1)<<setw(12)<<resp<<setw(12)<<e_resp<<endl;
     if (cor>0 && e_cor>0.000001 && e_cor<0.2)
       {    
         Correction[auxi_cor] = cor;
         CorrectionError[auxi_cor] = e_cor;
         xCaloPt[auxi_cor] = hCalo->GetBinContent(i+1);
         exCaloPt[auxi_cor] = hCalo->GetBinError(i+1);
         auxi_cor++;
       }
     if (resp>0 && e_resp>0.000001 && e_resp<0.2)
       {    
         Response[auxi_resp] = resp;
         ResponseError[auxi_resp] = e_resp;
         xGenPt[auxi_resp] = hGen->GetBinContent(i+1);
         exGenPt[auxi_resp] = hGen->GetBinError(i+1);
         auxi_resp++;
       }
   }
 g_Cor = new TGraphErrors(auxi_cor,xCaloPt,Correction,exCaloPt,CorrectionError);
 sprintf(name,"CorFit");
 CorFit = new TF1(name,"[0]+[1]/(pow(log10(x),[2])+[3])",xCaloPt[0],xCaloPt[auxi_cor-1]); 
 CorFit->SetParameter(0,1.);
 CorFit->SetParameter(1,7.);
 CorFit->SetParameter(2,4.);
 CorFit->SetParameter(3,4.);
 cout<<"Fitting correction in the range: "<<xCaloPt[0]<<" "<<xCaloPt[auxi_cor-1]<<endl;
 g_Cor->Fit(CorFit,"RQ");

 g_Resp = new TGraphErrors(auxi_resp,xGenPt,Response,exGenPt,ResponseError);
 sprintf(name,"RespFit");
 RespFit = new TF1(name,"[0]-[1]/(pow(log10(x),[2])+[3])+[4]/x",xGenPt[0],xGenPt[auxi_resp-1]); 
 RespFit->SetParameter(0,1.);
 RespFit->SetParameter(1,1.);
 RespFit->SetParameter(2,1.);
 RespFit->SetParameter(3,1.);
 RespFit->SetParameter(4,1.);
 cout<<"Fitting response in the range: "<<xGenPt[0]<<" "<<xGenPt[auxi_resp-1]<<endl;
 g_Resp->Fit(RespFit,"RQ");
 /////////////////////////////////////////////////////////
 MinCaloPt = xCaloPt[0];
 MaxCaloPt = xCaloPt[NGenPtBins-1]; 
 MinGenPt = xGenPt[0];
 MaxGenPt = xGenPt[NGenPtBins-1]; 
 //////////////////////////// Writing ///////////////////////////////////////////// 
 L3CorrectionFile.setf(ios::left);
 L3CorrectionFile <<setw(12)<<-5.191
                     <<setw(12)<<5.191
                     <<setw(12)<<(int)6 
                     <<setw(12)<<MinCaloPt
                     <<setw(12)<<MaxCaloPt
                     <<setw(12)<<CorFit->GetParameter(0)
                     <<setw(12)<<CorFit->GetParameter(1)
                     <<setw(12)<<CorFit->GetParameter(2)
                     <<setw(12)<<CorFit->GetParameter(3);
 L3CorrectionFile.close(); 

 L3ResponseFile.setf(ios::left);
 L3ResponseFile <<setw(12)<<RespFit->GetParameter(0)
                   <<setw(12)<<RespFit->GetParameter(1)
                   <<setw(12)<<RespFit->GetParameter(2)
                   <<setw(12)<<RespFit->GetParameter(3)
                   <<setw(12)<<RespFit->GetParameter(4);
 L3ResponseFile.close(); 

 outf = new TFile(L3OutputROOTFilename,"RECREATE");  
 sprintf(name,"Correction_vs_CaloPt");    
 g_Cor->Write(name);
 sprintf(name,"Response_vs_GenPt");    
 g_Resp->Write(name);
 outf->Close();
}
