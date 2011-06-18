{

   const double etCut = 30.0;
   const int nBINS = 200;
   const double xMAX = 70.0;


   TFile *file0 = TFile::Open("tree_Wenu_withBDT.root");
   TTree* tree = (TTree*) file0.Get("tree");

   TH1D* hMET_bdt = new TH1D("hMET_bdt", "", nBINS, 0.0, xMAX);
   TH1D* hMET_wp80 = new TH1D("hMET_wp80", "", nBINS, 0.0, xMAX);
   TH1D* hMET_CicHTight = new TH1D("hMET_CicHTight", "", nBINS, 0.0, xMAX);
   TH1D* hMET_CicSTight = new TH1D("hMET_CicSTight", "", nBINS, 0.0, xMAX);
   TH1D* hMET_LH = new TH1D("hMET_LH", "", nBINS, 0.0, xMAX);

   hMET_bdt->SetLineColor(4);
   hMET_CicHTight->SetLineColor(2);
   char tit[50];
   sprintf(tit, "events / %.2f GeV", xMAX/nBINS);
   TAxis* xaxis = hMET_CicSTight->GetXaxis();
   TAxis* yaxis = hMET_CicSTight->GetYaxis();
   xaxis->SetTitle("pf met [GeV]");
   xaxis->SetTitleSize(0.05);
   xaxis->SetLabelSize(0.05);
   yaxis->SetTitle(tit);
   yaxis->SetTitleOffset(1.6);
   yaxis->SetTitleSize(0.05);
   hMET_CicSTight->SetLineColor(2);
   hMET_LH->SetLineColor(3);
   hMET_CicHTight->SetLineStyle(2);
   hMET_CicHTight->SetLineWidth(2);


//    TCut lhcut("((probe_gsfEle_numberOfBrems==0 && probe_gsfEle_isEB && probe_eidLikelihood>0.592) || (probe_gsfEle_numberOfBrems>0 && probe_gsfEle_isEB && probe_eidLikelihood>0.732) || (probe_gsfEle_numberOfBrems==0 && probe_gsfEle_isEE && probe_eidLikelihood>0.741) || (probe_gsfEle_numberOfBrems>0 && probe_gsfEle_isEE && probe_eidLikelihood>0.919))");

   TCut lhcut("((probe_gsfEle_numberOfBrems==0 && probe_gsfEle_isEB && probe_eidLikelihood>0.5) || (probe_gsfEle_numberOfBrems>0 && probe_gsfEle_isEB && probe_eidLikelihood>0.7) || (probe_gsfEle_numberOfBrems==0 && probe_gsfEle_isEE && probe_eidLikelihood>0.6) || (probe_gsfEle_numberOfBrems>0 && probe_gsfEle_isEE && probe_eidLikelihood>0.9))");


   char temp[50];
   sprintf( temp, "probe_gsfEle_et>%f", etCut);
   TCut kin(temp);

   tree->Draw("event_met_pfmet>>hMET_bdt", kin && "probe_myBDT>0.027","goff");
   tree->Draw("event_met_pfmet>>hMET_wp80", kin && "probe_isWP80 && !(abs(probe_dcot)<0.02 && abs(probe_dist)<0.02)", "goff");
   tree->Draw("event_met_pfmet>>hMET_CicSTight", kin && "probe_isCicSuperTight", 
   "goff");
   tree->Draw("event_met_pfmet>>hMET_CicHTight", kin && "probe_isCicHyperTight1", "goff");
   tree->Draw("event_met_pfmet>>hMET_LH", kin && lhcut && "!(abs(probe_dcot)<0.02 && abs(probe_dist)<0.02) && probe_gsfEle_missingHits==0 && ((probe_gsfEle_trackiso + probe_gsfEle_ecaliso + probe_gsfEle_hcaliso)/probe_gsfEle_et -1.)<0.055","goff");


  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadBottomMargin(0.16);
  tdrStyle->SetLegendBorderSize(0);

  TCanvas* can = new TCanvas("can","",500, 500);
  hMET_CicSTight->Draw();
  hMET_LH->Draw("same");
  hMET_CicHTight->Draw("same");
  hMET_wp80->Draw("same");
  hMET_bdt->Draw("same");
  TLegend* leg = new TLegend(0.6,0.7,0.9,0.9);
  leg->AddEntry( hMET_wp80, "WP 80","L");
  leg->AddEntry( hMET_CicSTight, "CiC Super Tight","L");
  leg->AddEntry( hMET_CicHTight, "CiC Hyper Tight 1","L");
  leg->AddEntry( hMET_LH, "Likelihood","L");
  leg->AddEntry( hMET_bdt, "KM","L");
  leg->SetMargin(0.15);
  leg->SetFillColor(0);
  leg->Draw();
  sprintf(temp,"Wenu_met_Ele%dGeV.eps", (int) etCut);
  can->SaveAs(temp);
  sprintf(temp,"Wenu_met_Ele%dGeV.gif", (int) etCut);
  can->SaveAs(temp);
  sprintf(temp,"Wenu_met_Ele%dGeV.root", (int) etCut);
  can->SaveAs(temp);
}
