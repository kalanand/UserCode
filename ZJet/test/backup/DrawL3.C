void DrawL3(const char algorithm[100])
{
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0000);
  gStyle->SetOptFit(000); 
  gStyle->SetPalette(1);
 
  char name[100];
  //sprintf(name,"",algorithm);
  TFile *inf = new TFile("L3Graphs_ZjetResponse.root","r");
  TGraphErrors *g_Cor;
  TF1 *CorFit;
  TGraphErrors *g_Resp;
  TF1 *RespFit;
  ///////////////////////////////////////////////////////////////
  sprintf(name,"Correction_vs_CaloPt");      
  g_Cor = (TGraphErrors*)inf->Get(name);
  sprintf(name,"CorFit");
  CorFit = (TF1*)g_Cor->GetFunction(name);

  sprintf(name,"Response_vs_GenPt");      
  g_Resp = (TGraphErrors*)inf->Get(name);
  sprintf(name,"RespFit");
  RespFit = (TF1*)g_Resp->GetFunction(name);
  ////////////////////// Correction ///////////////////////////////////////
  TCanvas *c_Correction = new TCanvas("Correction","Correction",900,600);
  c_Correction->cd(); 
  gPad->SetLogx();
  sprintf(name,"L3Correction");
  g_Cor->SetTitle(name); 
  g_Cor->SetMarkerStyle(20);
  g_Cor->SetMarkerColor(1);
  g_Cor->SetLineColor(1);
  g_Cor->GetXaxis()->SetTitle("<p_{T}> (GeV)");
  g_Cor->GetYaxis()->SetTitle("L3Correction");
  g_Cor->SetMinimum(1.0);
  //g_Cor->SetMaximum(4.0);
  CorFit->SetLineColor(1);
  CorFit->SetParNames("a0","a1","a2","a3","a4");
  g_Cor->Draw("AP");
  TPaveLabel *pave = new TPaveLabel(0.4,0.75,0.6,0.85,algorithm,"NDC");
  pave->SetFillColor(0);
  pave->SetBorderSize(0);
  pave->Draw();
  ////////////////////// Response ///////////////////////////////////////
  TCanvas *c_Response = new TCanvas("Response","Response",900,600);
  c_Response->cd(); 
  gPad->SetLogx();
  sprintf(name,"Resposne");
  g_Resp->SetTitle(name); 
  g_Resp->SetMarkerStyle(20);
  g_Resp->SetMarkerColor(1);
  g_Resp->SetLineColor(1);
  g_Resp->GetXaxis()->SetTitle("<p_{T}^{gen}> (GeV)");
  g_Resp->GetYaxis()->SetTitle("Response");
  RespFit->SetLineColor(2);
  g_Resp->Draw("AP");
  TPaveLabel *pave = new TPaveLabel(0.4,0.75,0.6,0.85,algorithm,"NDC");
  pave->SetFillColor(0);
  pave->SetBorderSize(0);
  pave->Draw();
  
  /*
  TH1F *hClosure = new TH1F("Closure","Closure",500,0,1000);
  for(int i=0;i<500;i++)
    {
      double x = 5+(i+1)*2.;
      double y = Closure(x,RespFit,CorFit);
      cout<<x<<" "<<y<<" "<<Closure(x,RespFit,CorFit)<<endl;
      hClosure->SetBinContent(i+1,y);
      hClosure->SetBinError(i+1,0.);
    }
  TCanvas *can = new TCanvas("C","C",900,600);
  hClosure->Draw();
  */
}

double Closure(double x, TF1 *f1, TF1 *f2)
{
  double y1,y,tmp;
  y1 = f1->Eval(x);
  y = x*y1;
  tmp = y1*f2->Eval(y);
  return tmp;
}


