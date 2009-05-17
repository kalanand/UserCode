void compareEleEffPlots() {
     
  for(int i=0; i<4; i++) {  makeTable(i);  }   
}





// generic method to make a efficiency plot

void makeTable( int num )
{
//   TString rootfilename;
//   if(num<9) rootfilename = Form("root_electroneff_%d_eta_et.root", num);
//   else if(num<17) rootfilename = Form("root_electroneff_10%d_eta_et.root", num-8);
//   else if(num<25) rootfilename = Form("root_electroneff_20%d_eta_et.root", num-16);
//   else if(num<33) rootfilename = Form("root_electroneff_30%d_eta_et.root", num-24);
//   else rootfilename = Form("root_electroneff_40%d_eta_et.root", num-32);

  TString rootfilename;
  if(num==0) rootfilename = "root_electroneff_WenuOff_eta_et.root";
  if(num==1) rootfilename = "root_electroneff_WenuOn_eta_et.root";
  if(num==2) rootfilename = "root_electroneff_ZeeOff_eta_et.root";
  if(num==3) rootfilename = "root_electroneff_ZeeOn_eta_et.root";



  TString pname = "";
  if(num==0) pname = "WenuOff-";
  if(num==1) pname = "WenuOn-";
  if(num==2) pname = "ZeeOff-";
  if(num==3) pname = "ZeeOn-";



//   if(num==1) pname = "WenuSelection-A1";
//   if(num==2) pname = "WenuSelection-A2";
//   if(num==3) pname = "WenuSelection-A3";
//   if(num==4) pname = "WenuSelection-B1";
//   if(num==5) pname = "WenuSelection-B2";
//   if(num==6) pname = "WenuSelection-B3";
//   if(num==7) pname = "ZeeSelection-A";
//   if(num==8) pname = "ZeeSelection-B";
//   if(num==9) pname = "WenuSelection-A1-noId-"; 
//   if(num==10) pname = "WenuSelection-A2-noId-";
//   if(num==11) pname = "WenuSelection-A3-noId-";
//   if(num==12) pname = "WenuSelection-B1-noId-";
//   if(num==13) pname = "WenuSelection-B2-noId-";
//   if(num==14) pname = "WenuSelection-B3-noId-";
//   if(num==15) pname = "ZeeSelection-A-noId-";
//   if(num==16) pname = "ZeeSelection-B-noId-";
//   if(num==17) pname = "WenuSelection-A1-noTrackIso-"; 
//   if(num==18) pname = "WenuSelection-A2-noTrackIso-";
//   if(num==19) pname = "WenuSelection-A3-noTrackIso-";
//   if(num==20) pname = "WenuSelection-B1-noTrackIso-";
//   if(num==21) pname = "WenuSelection-B2-noTrackIso-";
//   if(num==22) pname = "WenuSelection-B3-noTrackIso-";
//   if(num==23) pname = "ZeeSelection-A-noTrackIso-";
//   if(num==24) pname = "ZeeSelection-B-noTrackIso-";
//   if(num==25) pname = "WenuSelection-A1-noIso-"; 
//   if(num==26) pname = "WenuSelection-A2-noIso-";
//   if(num==27) pname = "WenuSelection-A3-noIso-";
//   if(num==28) pname = "WenuSelection-B1-noIso-";
//   if(num==29) pname = "WenuSelection-B2-noIso-";
//   if(num==30) pname = "WenuSelection-B3-noIso-";
//   if(num==31) pname = "ZeeSelection-A-noIso-";
//   if(num==32) pname = "ZeeSelection-B-noIso-";
//   if(num==33) pname = "WenuSelection-A1-noSigmaEtaEta-";
//   if(num==34) pname = "WenuSelection-A2-noSigmaEtaEta-";
//   if(num==35) pname = "WenuSelection-A3-noSigmaEtaEta-";
//   if(num==36) pname = "WenuSelection-B1-noSigmaEtaEta-";
//   if(num==37) pname = "WenuSelection-B2-noSigmaEtaEta-";
//   if(num==38) pname = "WenuSelection-B3-noSigmaEtaEta-";
//   if(num==39) pname = "ZeeSelection-A-noSigmaEtaEta-";
//   if(num==40) pname = "ZeeSelection-B-noSigmaEtaEta-";


  TString label = TString("electroneff-") + pname;

  gROOT->ProcessLine(".L tdrstyleNew.C");
  setTDRStyle();
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.1);
  tdrStyle->SetPadTopMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);
  
  TFile* myhistos = new TFile(rootfilename); 
  TH1F* h0 = (TH1F*) myhistos->Get("sbs_eff_EtDet");
  TH1F* h1 = (TH1F*) myhistos->Get("sbs_eff_EtaDet");
  TH2F* h2 = (TH2F*) myhistos->Get("sbs_eff_EtDet_EtaDet");


//   TH1F *base = (TH1F*)f.Get("base");
//   TH1F *destination = (TH1F*)f.Get("destination");
  
//   TGraphAsymmErrors *g1 = new TGraphAsymmErrors();
//   g1->BayesDivide(destination, base, "");
//   g1->GetYaxis()->SetRangeUser(0.5, 1.05);


  h0->SetMarkerStyle(20);
  h1->SetMarkerStyle(20);
  ScaleTo10pb( *h0 );
  ScaleTo10pb( *h1 );

//   TH1F* h01 = (TH1F*) myhistos->Get("fit_eff_Et");
//   TH1F* h11 = (TH1F*) myhistos->Get("fit_eff_Eta");
//   h01->SetMarkerColor(4);
//   h01->SetLineColor(4);
//   h11->SetMarkerColor(4);
//   h11->SetLineColor(4);
//   ScaleTo10pb( *h01 );
//   ScaleTo10pb( *h11 );

  TH1F* h02 = (TH1F*) myhistos->Get("truth_eff_EtDet");
  TH1F* h12 = (TH1F*) myhistos->Get("truth_eff_EtaDet");
  h02->SetMarkerColor(2);
  h02->SetLineColor(2);
  h02->SetMarkerStyle(24);
  h12->SetMarkerColor(2);
  h12->SetLineColor(2);
  h12->SetMarkerStyle(24);
  ScaleTo10pb( *h02 );
  ScaleTo10pb( *h12 );

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


  double l1 = 0.2, l2 =0.3;
  double p1 = 0.5;
  if(num==0) p1 = 0.82;
  if(num==2) p1 = 0.4;

  TCanvas* can0 = new TCanvas("can0", "", 500, 500);
  gStyle->SetOptStat(0);
  h0->Draw("e");
  // h01->Draw("esame");
  h02->Draw("esame");
  leg_hist = new TLegend(0.3, l1, 0.8, l2);
  leg_hist->AddEntry( h0,"Sideband subtracted","P");
  // leg_hist->AddEntry( h01,"fit","le");
  leg_hist->AddEntry( h02,"Monte Carlo truth","P");
  leg_hist->SetFillColor(0);
  leg_hist->SetMargin(0.15);
  leg_hist->Draw();
  TLatex* CMS = new  TLatex();
  CMS->SetTextAlign(12);
  CMS->SetTextSize(0.04);
  CMS->SetNDC();
  CMS->DrawLatex(0.25, p1, "CMS Preliminary,     #intLdt = 10 pb^{-1}");
  can0->Update();
  can0->SaveAs( label+TString("et.eps") );
  can0->SaveAs( label+TString("et.gif") );
  can0->SaveAs( label+TString("et.root") );
  can0->SaveAs( label+TString("et.pdf") );

  p1 = 0.5;
  if(num==0 || num==2) p1 = 0.82;

  TCanvas* can1 = new TCanvas("can1", "", 500, 500);
  gStyle->SetOptStat(0);
  h1->Draw("e");
  // h11->Draw("esame");
  h12->Draw("esame");
  leg_hist = new TLegend(0.3,l1,0.8,l2);
  leg_hist->AddEntry( h1,"Sideband subtracted","P");
  // leg_hist->AddEntry( h11,"fit","le");
  leg_hist->AddEntry( h12,"Monte Carlo truth","P");
  leg_hist->SetFillColor(0);
  leg_hist->SetMargin(0.15);
  leg_hist->Draw();
  TLatex* CMS2 = new  TLatex();
  CMS2->SetTextAlign(12);
  CMS2->SetTextSize(0.04);
  CMS2->SetNDC();
  CMS2->DrawLatex(0.25,p1,"CMS Preliminary,     #intLdt = 10 pb^{-1}");
  can1->Update();
  can1->SaveAs( label+TString("eta.eps") );
  can1->SaveAs( label+TString("eta.gif") );
  can1->SaveAs( label+TString("eta.root") );
  can1->SaveAs( label+TString("eta.pdf") );


  tdrStyle->SetPadRightMargin(0.2);
  TCanvas* can2 = new TCanvas("can2", "", 500, 500);
  gStyle->SetOptStat(0);
  h2->Draw("colz");
  can2->SaveAs( label+TString("et_eta.eps") );
  can2->SaveAs( label+TString("et_eta.gif") );
  can2->SaveAs( label+TString("et_eta.root") );
  can2->SaveAs( label+TString("et_eta.pdf") );

  delete can0;
  delete can1;
  delete can2;
  delete myhistos;
}


void ScaleTo10pb( TH1& hist) {

  int bins = hist.GetNbinsX();
  double error = 0;

  for(int i=1; i<= bins; i++) {
    error = 8.0 * hist.GetBinError(i);
    // if( error > 1.0 ) error = 1.0;
    hist.SetBinError(i, error);
  }

}
