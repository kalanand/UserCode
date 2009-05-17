void makeEleEffPlots() {
     
  for(int i=9; i<33; i++) {  makeTable(i);  }   
}





// generic method to make a efficiency plot

void makeTable( int num )
{

  TString rootfilename;
  if(num<9) rootfilename = Form("root_electroneff_%d_eta_et.root", num);
  else if(num<17) rootfilename = Form("root_electroneff_10%d_eta_et.root", num-8);
  else if(num<25) rootfilename = Form("root_electroneff_20%d_eta_et.root", num-16);
  else rootfilename = Form("root_electroneff_30%d_eta_et.root", num-24);

  TString pname = "";
  if(num==1) pname = "WenuSelection-A1";
  if(num==2) pname = "WenuSelection-A2";
  if(num==3) pname = "WenuSelection-A3";
  if(num==4) pname = "WenuSelection-B1";
  if(num==5) pname = "WenuSelection-B2";
  if(num==6) pname = "WenuSelection-B3";
  if(num==7) pname = "ZeeSelection-A";
  if(num==8) pname = "ZeeSelection-B";
  if(num==9) pname = "WenuSelection-A1-noId-"; 
  if(num==10) pname = "WenuSelection-A2-noId-";
  if(num==11) pname = "WenuSelection-A3-noId-";
  if(num==12) pname = "WenuSelection-B1-noId-";
  if(num==13) pname = "WenuSelection-B2-noId-";
  if(num==14) pname = "WenuSelection-B3-noId-";
  if(num==15) pname = "ZeeSelection-A-noId-";
  if(num==16) pname = "ZeeSelection-B-noId-";
  if(num==17) pname = "WenuSelection-A1-noTrackIso-"; 
  if(num==18) pname = "WenuSelection-A2-noTrackIso-";
  if(num==19) pname = "WenuSelection-A3-noTrackIso-";
  if(num==20) pname = "WenuSelection-B1-noTrackIso-";
  if(num==21) pname = "WenuSelection-B2-noTrackIso-";
  if(num==22) pname = "WenuSelection-B3-noTrackIso-";
  if(num==23) pname = "ZeeSelection-A-noTrackIso-";
  if(num==24) pname = "ZeeSelection-B-noTrackIso-";
  if(num==25) pname = "WenuSelection-A1-noIso-"; 
  if(num==26) pname = "WenuSelection-A2-noIso-";
  if(num==27) pname = "WenuSelection-A3-noIso-";
  if(num==28) pname = "WenuSelection-B1-noIso-";
  if(num==29) pname = "WenuSelection-B2-noIso-";
  if(num==30) pname = "WenuSelection-B3-noIso-";
  if(num==31) pname = "ZeeSelection-A-noIso-";
  if(num==32) pname = "ZeeSelection-B-noIso-";


  TString label = TString("electroneff-") + pname;

  gROOT->ProcessLine(".L mystyle.C");
  setTDRStyle();
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.1);
  tdrStyle->SetPadTopMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);
  
  TFile* myhistos = new TFile(rootfilename); 
  TH1F* h0 = (TH1F*) myhistos->Get("sbs_eff_EtDet");
  TH1F* h1 = (TH1F*) myhistos->Get("sbs_eff_EtaDet");
  TH2F* h2 = (TH2F*) myhistos->Get("sbs_eff_EtDet_EtaDet");
  h0->SetMarkerStyle(20);
  h1->SetMarkerStyle(20);

  TAxis* xaxis = h0->GetXaxis();
  TAxis* yaxis = h0->GetYaxis();
  xaxis->SetTitle("E_{T} (GeV)");
  xaxis->SetTitleSize(0.04);
  xaxis->SetTitleOffset(1.3);
  xaxis->SetNdivisions(505);
  yaxis->SetTitle("Efficiency  ");
  yaxis->SetTitleSize(0.04);
  yaxis->SetTitleOffset(1.6);
  h0->SetMinimum(0.5);
  h0->SetMaximum(1.1);


  TAxis* xaxis = h1->GetXaxis();
  TAxis* yaxis = h1->GetYaxis();
  xaxis->SetTitle("#eta");
  xaxis->SetTitleSize(0.04);
  xaxis->SetTitleOffset(1.3);
  xaxis->SetNdivisions(505);
  yaxis->SetTitle("Efficiency  ");
  yaxis->SetTitleSize(0.04);
  yaxis->SetTitleOffset(1.6);
  h1->SetMinimum(0.5);
  h1->SetMaximum(1.1);


  TAxis* xaxis = h2->GetXaxis();
  TAxis* yaxis = h2->GetYaxis();
  xaxis->SetTitle("E_{T} (GeV)");
  xaxis->SetTitleSize(0.04);
  xaxis->SetTitleOffset(1.3);
  xaxis->SetNdivisions(505);
  yaxis->SetTitle("#eta ");
  yaxis->SetTitleSize(0.04);
  yaxis->SetTitleOffset(1.7);


  TCanvas* can0 = new TCanvas("can0", "", 500, 500);
  gStyle->SetOptStat(0);
  h0->Draw("e");
  can0->SaveAs( label+TString("et.eps") );
  can0->SaveAs( label+TString("et.gif") );
  can0->SaveAs( label+TString("et.root") );

  TCanvas* can1 = new TCanvas("can1", "", 500, 500);
  gStyle->SetOptStat(0);
  h1->Draw("e");
  can1->SaveAs( label+TString("eta.eps") );
  can1->SaveAs( label+TString("eta.gif") );
  can1->SaveAs( label+TString("eta.root") );


  tdrStyle->SetPadRightMargin(0.2);
  TCanvas* can2 = new TCanvas("can2", "", 500, 500);
  gStyle->SetOptStat(0);
  h2->Draw("colz");
  can2->SaveAs( label+TString("et_eta.eps") );
  can2->SaveAs( label+TString("et_eta.gif") );
  can2->SaveAs( label+TString("et_eta.root") );

  delete can0;
  delete can1;
  delete can2;
  delete myhistos;
}
