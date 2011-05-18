const float IntLUMI = 210.0;

void fitWjj_mu ()
{
//    char* plotDir = "enujjPlots/";
   char* plotDir = "";

  const double WJets_scale = 24640.*1.271 *IntLUMI/14510974;
  const double WW_scale = 27.79*4.0*1.547* IntLUMI/2011760;
  const double WZ_scale =  10.4*4.0*1.547* IntLUMI/2108416;
  const double TT_scale = 65.83 * 1.4 * IntLUMI/4759744;



  TFile *file0 = TFile::Open("RD_WmunuJets_Data2011_178invpb_May6JSON.root");
  TH1F* h_jjmass = (TH1F*) file0->Get("hh_lvjj_kkmjj");
  h_jjmass->SetMarkerStyle(20);


  TFile *file1 = TFile::Open("RD_WmunuJets_Spring11MC_WJets.root");
  TH1F* h_jjmass_WJets = (TH1F*) file1->Get("hh_lvjj_kkmjj");
  h_jjmass_WJets->SetFillColor(2);

  TFile *file2 = TFile::Open("RD_WmunuJets_Spring11MC_WWtoAnything.root");
  TH1F* h_jjmass_WW_enujj = (TH1F*) file2->Get("hh_lvjj_kkmjj");
  h_jjmass_WW_enujj->SetFillColor(kOrange);

  TFile *file3 = TFile::Open("RD_WmunuJets_Spring11MC_WZtoAnything.root");
  TH1F* h_jjmass_WZ_enujj = (TH1F*) file3->Get("hh_lvjj_kkmjj");

  TFile *file4 = TFile::Open("RD_WmunuJets_Spring11MC_TTToLNu2Q2B.root");
  TH1F* h_jjmass_TT = (TH1F*) file4->Get("hh_lvjj_kkmjj");
  h_jjmass_TT->SetFillColor(3);

//////////// binning
  const int jjmass_NBINS = 45;
  const double jjmass_MIN = 40.;
  const double jjmass_MAX = 400.;
  char temp[1000];
  sprintf(temp, "Events / %d GeV", (jjmass_MAX-jjmass_MIN)/jjmass_NBINS);
  h_jjmass->GetYaxis()->SetTitle(temp);
  h_jjmass->GetXaxis()->SetRangeUser(40,220);
  h_jjmass_WJets->GetXaxis()->SetRangeUser(40,220);
  h_jjmass_WW_enujj->GetXaxis()->SetRangeUser(40,220);
  h_jjmass_TT->GetXaxis()->SetRangeUser(40,220);


  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);
  gStyle->SetOptFit(1111);
  //tdrStyle->SetOptStat("mer"); 



  TCanvas* c_jjmass = new TCanvas("c_jjmass","jj mass", 500, 500);
  h_jjmass_WJets->Scale( WJets_scale );
  h_jjmass_WW_enujj->Scale( WW_scale );
  h_jjmass_WZ_enujj->Scale( WZ_scale );
  h_jjmass_WW_enujj->Add(h_jjmass_WZ_enujj);
  h_jjmass_TT->Scale( TT_scale );
  h_jjmass->SetMinimum(0);
  h_jjmass->GetXaxis()->SetTitle("m_{jj} [GeV]");
  h_jjmass->GetXaxis()->SetTitleSize(0.06);
  h_jjmass->GetYaxis()->SetTitleSize(0.06);


// ################# Add the muon channel histograms
//   TFile* fFanMuData = new TFile("hists.root");
//   TH1D* h_data = (TH1D*) fFanMuData->Get("h_data");
//   TH1D* h_wjet = (TH1D*) fFanMuData->Get("h_wjet");
//   TH1D* h_wwwz = (TH1D*) fFanMuData->Get("h_wwwz");
//   TH1D* h_tt = (TH1D*) fFanMuData->Get("h_tt");
//   h_jjmass->Add(h_data);
//   h_jjmass_WW_enujj->Add(h_wwwz);
//   h_jjmass_WJets->Add(h_wjet);
//   h_jjmass_TT->Add(h_tt);
// #################

  h_jjmass_WJets->Add(h_jjmass_TT);
  TH1D* h_jjmass_WW_clone = h_jjmass_WW_enujj->Clone("h_jjmass_WW_clone");
  h_jjmass_WW_clone->SetLineColor(kOrange);
  h_jjmass_WW_clone->SetLineWidth(3);
  h_jjmass_WW_clone->SetFillColor(0);
  h_jjmass_WW_enujj->Add(h_jjmass_WJets);


// #################
//   h_jjmass->GetXaxis()->SetRangeUser(20., 180.);
//   h_jjmass_WW_enujj->GetXaxis()->SetRangeUser(20., 180.);
//   h_jjmass_WJets->GetXaxis()->SetRangeUser(20., 180.);
//   h_jjmass_TT->GetXaxis()->SetRangeUser(20., 180.);
// #################
  h_jjmass->SetMaximum( 1.3 * h_jjmass->GetMaximum() );
  h_jjmass->Draw("e");
  h_jjmass_WW_enujj->Draw("same");
  h_jjmass_WJets->Draw("same");
  h_jjmass_TT->Draw("same");
  h_jjmass_WW_clone->Draw("hist same");
  h_jjmass->Draw("esame");
  c_jjmass->RedrawAxis();
  TLegend* legend = new TLegend(0.55,0.7,0.89,0.89);
  legend->SetFillColor(0);
  legend->AddEntry(h_jjmass, "Data", "PLE");  
  legend->AddEntry(h_jjmass_WW_enujj, "WW/WZ#rightarrow l#nujj", "F");
  legend->AddEntry(h_jjmass_WJets, "W(#rightarrow l#nu)+jj", "F");
  legend->AddEntry(h_jjmass_TT, "t#bar{t},single top", "F");
  legend->Draw();
  cmsPrelim();
  TString plotname = TString(plotDir) + TString("jj-mass");
  c_jjmass->SaveAs( plotname + TString(".gif") );
  c_jjmass->SaveAs( plotname + TString(".eps") );
  c_jjmass->SaveAs( plotname + TString(".pdf") );
  c_jjmass->SaveAs( plotname + TString(".root") );




//////////////////////////
  TCanvas* c_jjmass_log = new TCanvas("c_jjmass_log","jj mass", 500, 500);
  h_jjmass->SetMinimum(0.18);
  h_jjmass->SetMaximum( 15. * h_jjmass->GetMaximum() );
  h_jjmass->Draw("e");
  h_jjmass_WW_enujj->Draw("same");
  h_jjmass_WJets->Draw("same");
  h_jjmass_TT->Draw("same");
  h_jjmass_WW_clone->Draw("hist same");
  h_jjmass->Draw("esame");
  c_jjmass_log->RedrawAxis();
  TLegend* legend = new TLegend(0.6,0.74,0.89,0.89);
  legend->SetFillColor(0);
  legend->AddEntry(h_jjmass, "Data", "PLE");  
  legend->AddEntry(h_jjmass_WW_enujj, "WW/WZ#rightarrow l#nujj", "F");
  legend->AddEntry(h_jjmass_WJets, "W(#rightarrow l#nu)+jj", "F");
  legend->AddEntry(h_jjmass_TT, "t#bar{t},single top", "F");
  legend->Draw();
  cmsPrelim();
  c_jjmass_log->SetLogy(1);
  plotname = TString(plotDir) + TString("jj-mass_log");
  c_jjmass_log->SaveAs( plotname + TString(".gif") );
  c_jjmass_log->SaveAs( plotname + TString(".eps") );
  c_jjmass_log->SaveAs( plotname + TString(".pdf") );
  c_jjmass_log->SaveAs( plotname + TString(".root") );
//////////////////////////

///////////////



// ################# How many good W+jj are there ?
  cout << "Number of observed events in data = " <<  h_jjmass->Integral() << endl;
  cout << "Number of W+jj events predicted from MC = " <<  h_jjmass_WJets->Integral() - h_jjmass_TT->Integral()<< endl;
  cout << "Number of ttbar + single top events predicted from MC = " << h_jjmass_TT->Integral()<< endl;
  cout << "Number of WW events predicted from MC = " <<  h_jjmass_WW_enujj->Integral() - h_jjmass_WJets->Integral()<< endl;
// #################




  TCanvas* c_jjmass_sub = new TCanvas("c_jjmass_sub","jj mass", 500, 500);
  TH1D* h_jjmass_sub = h_jjmass->Clone(" h_jjmass_sub");
  h_jjmass_sub->Add(h_jjmass_WJets, -1);

  for(int i=0; i< h_jjmass->GetNbinsX(); i++) {
     h_jjmass_sub->SetBinError(i, h_jjmass->GetBinError(i) );
  }
  h_jjmass_sub->SetMaximum( 3.0 * h_jjmass_sub->GetMaximum() );
  h_jjmass_sub->SetMinimum( 3.0 * h_jjmass_sub->GetMinimum() );
  h_jjmass_sub->Draw("e");
  h_jjmass_WW_clone->Draw("hist same");
  h_jjmass_sub->Draw("esame");
  c_jjmass_sub->RedrawAxis();
  TLegend* legend = new TLegend(0.6,0.75,0.89,0.89);
  legend->SetFillColor(0);
  legend->AddEntry(h_jjmass_sub, "Data", "PLE");  
  legend->AddEntry(h_jjmass_WW_enujj, "WW/WZ#rightarrow l#nujj", "F");
  legend->Draw();
  cmsPrelim();
  plotname = TString(plotDir) + TString("jj-mass_bgsub");
  c_jjmass_sub->SaveAs( plotname + TString(".gif") );
  c_jjmass_sub->SaveAs( plotname + TString(".eps") );
  c_jjmass_sub->SaveAs( plotname + TString(".pdf") );
  c_jjmass_sub->SaveAs( plotname + TString(".root") );
//#################




////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

 TCanvas* c_jjmass_WJets = new TCanvas("c_jjmass_WJets","WJets fit", 500, 500);
 h_jjmass_WJets->SetFillColor(0);
 h_jjmass_WJets->GetXaxis()->SetTitle("m_{jj} [GeV]");
 h_jjmass_WJets->GetXaxis()->SetTitleSize(0.06);
 h_jjmass_WJets->GetYaxis()->SetTitleSize(0.06);
 sprintf(temp, "Events / %d GeV", (jjmass_MAX-jjmass_MIN)/jjmass_NBINS);
 h_jjmass_WJets->GetYaxis()->SetTitle(temp);
 h_jjmass_WJets->Draw("e");
 cout << "WJets integral [40, 220] = " << h_jjmass_WJets->Integral() << endl;
 TF1 *func1 = new TF1("func1", "[0]*(685.5/3709.)*((1-x/7000.)^[1])/(x^[2])", 40., 1000.);
 //TF1 *func1 = new TF1("func1", "[0]*((1-x/7000.)^[1])/(x^[2])", 40., 1000.);
 func1->SetParameter(0, 10000.);
 func1->SetParameter(1, 100.0);
 func1->SetParameter(2, 1.2);
 func1->SetLineWidth(2);
 func1->SetParName(0,"Norm");
 h_jjmass_WJets->Fit("func1","LLI","",40.0, 220.0); // QCD fit
 cmsPrelim2();
 plotname = TString(plotDir) + TString("Wjets_fit");
 c_jjmass_WJets->SaveAs( plotname + TString(".gif") );
 c_jjmass_WJets->SaveAs( plotname + TString(".eps") );
 c_jjmass_WJets->SaveAs( plotname + TString(".pdf") );
 c_jjmass_WJets->SaveAs( plotname + TString(".root") );



 TCanvas* c_jjmass_WW = new TCanvas("c_jjmass_WW","WW fit", 500, 500);
 TH1F* h_jjmass_WW = (TH1F*) file2->Get("hh_lvjj_mjj");
 h_jjmass_WW->Scale( 1.3*WW_scale );
 h_jjmass_WW->GetXaxis()->SetTitle("m_{jj} [GeV]");
 h_jjmass_WW->GetXaxis()->SetTitleSize(0.06);
 h_jjmass_WW->GetYaxis()->SetTitleSize(0.06);
 h_jjmass_WW->GetXaxis()->SetRangeUser(40.0, 220.0);
 sprintf(temp, "Events / %d GeV", (jjmass_MAX-jjmass_MIN)/jjmass_NBINS);
 h_jjmass_WW->GetYaxis()->SetTitle(temp);
 h_jjmass_WW->Draw("e");
 cout << "WWW integral [40, 220] = " << h_jjmass_WW->Integral() << endl;
 TF1 *func2 = new TF1("func2", "TMath::BreitWigner(x,[0],[1])*[2]*(11310./1198.)", 40., 220.);
//TF1 *func2 = new TF1("func2", "TMath::BreitWigner(x,[0],[1])*[2]", 40., 220.);
 func2->SetParLimits(0, 75, 90);
 func2->SetParameter(1, 26.28);
 func2->SetParameter(2, 600.);
 func2->SetLineWidth(2);
 func2->SetParName(0,"BW mean");
 func2->SetParName(1,"BW sigma");
 func2->SetParName(2,"Norm");
 h_jjmass_WW->Fit("func2","LLI","",40.0, 220.0); // gaus fit
 cmsPrelim2();
 plotname = TString(plotDir) + TString("WW_fit");
 c_jjmass_WW->SaveAs( plotname + TString(".gif") );
 c_jjmass_WW->SaveAs( plotname + TString(".eps") );
 c_jjmass_WW->SaveAs( plotname + TString(".pdf") );
 c_jjmass_WW->SaveAs( plotname + TString(".root") );



 TCanvas* c_jjmass_fit = new TCanvas("c_jjmass_fit","data fit", 500, 500);
 h_jjmass->SetMaximum(650.);
 h_jjmass->SetOption("E");
 h_jjmass->Draw("e");
 TF1 *func3 = new TF1("func3", "[0]*(11310./1198.)*TMath::BreitWigner(x,76.34,29.21)+ [1]*(685.5/3709.)*((1-x/7000.)^83)/(x^1.23)", 40., 220.);


 func3->SetParameter(0, 1.0);
 func3->SetParName(0,"WW");
 func3->SetParName(1,"Wjj");
 func3->SetLineWidth(2);
 func3->SetLineColor(4);
// func3->SetParameter(1, 1.0);
 h_jjmass->Fit("func3","LLI", "", 40.0, 200.0);
 TF1 *func4 = new TF1("func4", "5.97781e+05*(685.5/3709.)*((1-x/7000.)^83)/(x^1.23)", 40., 220.);
  func4->SetLineColor(2);
  func4->SetLineStyle(2);
  func4->SetLineWidth(2);
  func4->Draw("same");
 cmsPrelim2();
 plotname = TString(plotDir) + TString("data_fit");
 c_jjmass_fit->SaveAs( plotname + TString(".gif") );
 c_jjmass_fit->SaveAs( plotname + TString(".eps") );
 c_jjmass_fit->SaveAs( plotname + TString(".pdf") );
 c_jjmass_fit->SaveAs( plotname + TString(".root") );
}






////CMS Preliminary label and lumu
void cmsPrelim()
{
   const float LUMINOSITY = IntLUMI;
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);

  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.96,"#sqrt{s} = 7 TeV");
  if (LUMINOSITY > 0.) {
    latex.SetTextAlign(31); // align right
    latex.DrawLatex(0.5,0.84,Form("#int #font[12]{L} dt = %.0f pb^{-1}",
    LUMINOSITY));
  }
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.96,"CMS preliminary 2011");
}


////CMS Preliminary label 
void cmsPrelim2()
{
   const float LUMINOSITY = IntLUMI;
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);

  if (LUMINOSITY > 0.) {
    latex.SetTextAlign(31); // align right
    latex.DrawLatex(0.7,0.64,Form("#int #font[12]{L} dt = %.0f pb^{-1}",
    LUMINOSITY));
  }
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.96,"CMS preliminary 2011");
}
