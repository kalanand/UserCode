{
   const double etCut = 30.0;
   const int nBINS = 30;
   const double xMAX = 120.0;
   const double xMIN = 60.0;


   TFile *file0 = TFile::Open("test.root");
   TTree* tree = (TTree*) file0.Get("tree");

   TH1D* hMee_bdt = new TH1D("hMee_bdt", "", nBINS, xMIN, xMAX);
   TH1D* hMee_wp80 = new TH1D("hMee_wp80", "", nBINS, xMIN, xMAX);
   TH1D* hMee_CicHTight = new TH1D("hMee_CicHTight", "", nBINS, xMIN, xMAX);
   TH1D* hMee_CicSTight = new TH1D("hMee_CicSTight", "", nBINS, xMIN, xMAX);
   TH1D* hMee_LH = new TH1D("hMee_LH", "", nBINS, xMIN, xMAX);

   hMee_wp80->SetLineWidth(2);
   hMee_bdt->SetLineColor(4);
   hMee_bdt->SetLineWidth(2);
   hMee_CicHTight->SetLineColor(2);
   hMee_CicSTight->SetLineWidth(2);
   char tit[50];
   sprintf(tit, "events / %.1f GeV", (xMAX-xMIN)/nBINS);
   TAxis* xaxis = hMee_LH->GetXaxis();
   TAxis* yaxis = hMee_LH->GetYaxis();
   xaxis->SetTitle("m_{ee} [GeV]");
   xaxis->SetTitleSize(0.05);
   xaxis->SetLabelSize(0.05);
   yaxis->SetTitle(tit);
   yaxis->SetTitleOffset(1.6);
   yaxis->SetTitleSize(0.05);
   hMee_LH->SetMinimum(10);
   hMee_CicSTight->SetLineColor(2);
   hMee_LH->SetLineColor(3);
   hMee_LH->SetLineWidth(2);
   hMee_CicHTight->SetLineStyle(2);
   hMee_CicHTight->SetLineWidth(2);


  TCut lhcut("((tag_gsfEle_numberOfBrems==0 && tag_gsfEle_isEB && tag_eidLikelihood>0.85) || (tag_gsfEle_numberOfBrems>0 && tag_gsfEle_isEB && tag_eidLikelihood>0.9) || (tag_gsfEle_numberOfBrems==0 && tag_gsfEle_isEE && tag_eidLikelihood>0.99) || (tag_gsfEle_numberOfBrems>0 && tag_gsfEle_isEE && tag_eidLikelihood>0.99))");

//    TCut lhcut("((tag_gsfEle_numberOfBrems==0 && tag_gsfEle_isEB && tag_eidLikelihood>0.5) || (tag_gsfEle_numberOfBrems>0 && tag_gsfEle_isEB && tag_eidLikelihood>0.7) || (tag_gsfEle_numberOfBrems==0 && tag_gsfEle_isEE && tag_eidLikelihood>0.6) || (tag_gsfEle_numberOfBrems>0 && tag_gsfEle_isEE && tag_eidLikelihood>0.9))");


   char temp[50];
   sprintf( temp, "tag_sc_et>20.0 && probe_sc_et>%f && probe_isWP80==0", etCut);
   TCut kin(temp);

   tree->Draw("mass>>hMee_bdt", kin && "tag_myBDT>0.05","goff");
   tree->Draw("mass>>hMee_wp80", kin && "tag_isWP80 && !(abs(tag_dcot)<0.02 && abs(tag_dist)<0.02)", "goff");
   tree->Draw("mass>>hMee_CicSTight", kin && "tag_isCicSuperTight", 
   "goff");
   tree->Draw("mass>>hMee_CicHTight", kin && "tag_isCicHyperTight1", "goff");
   tree->Draw("mass>>hMee_LH", kin && lhcut && "!(abs(tag_dcot)<0.02 && abs(tag_dist)<0.02) && tag_gsfEle_missingHits==0 && ((tag_gsfEle_trackiso + tag_gsfEle_ecaliso + tag_gsfEle_hcaliso)/tag_gsfEle_et -1.)<0.0", "goff");



  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadBottomMargin(0.16);
  tdrStyle->SetLegendBorderSize(0);

  TCanvas* can = new TCanvas("can","",500, 500);
  hMee_LH->Draw();
  hMee_CicHTight->Draw("same");
  hMee_wp80->Draw("same");
  hMee_bdt->Draw("same");
  hMee_CicSTight->Draw("same");
  hMee_wp80->Draw("same");
  TLegend* leg = new TLegend(0.6,0.7,0.9,0.9);
  leg->AddEntry( hMee_wp80, "WP 80","L");
  leg->AddEntry( hMee_CicSTight, "CiC Super Tight","L");
  leg->AddEntry( hMee_CicHTight, "CiC Hyper Tight 1","L");
  leg->AddEntry( hMee_LH, "Likelihood","L");
  leg->AddEntry( hMee_bdt, "KM","L");
  leg->SetMargin(0.15);
  leg->SetFillColor(0);
  leg->Draw();
  can->SetLogy();
  sprintf(temp,"TagSCmass_%dGeV.eps", (int) etCut);
  can->SaveAs(temp);
  sprintf(temp,"TagSCmass_%dGeV.gif", (int) etCut);
  can->SaveAs(temp);
  sprintf(temp,"TagSCmass_%dGeV.root", (int) etCut);
  can->SaveAs(temp);
}
