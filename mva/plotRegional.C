{

 TFile* fData = new TFile("../ZeeEvents_PromptReco_35invpb_WP80.root");
 TTree* tpTree = (TTree*) fData->Get("tree");
 
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadBottomMargin(0.16);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetOptStat("mer");
  tdrStyle->SetStatStyle(1001);
  tdrStyle->SetStatX(0.9);
  tdrStyle->SetStatY(0.9);
  tdrStyle->SetStatW(0.25);


////////// ******* Z mass histogram ********************** //////
  const int ZMASSBINS = 30;
  const double ZMASSMIN = 60.;
  const double ZMASSMAX = 120.;

 TH1D* dataZmassBB = new TH1D("dataZmassBB","BB",ZMASSBINS, ZMASSMIN, ZMASSMAX);
 dataZmassBB->SetLineWidth(2);
 TAxis* xaxis = dataZmassBB->GetXaxis();
 TAxis* yaxis = dataZmassBB->GetYaxis();
 xaxis->SetNdivisions(510);
 xaxis->SetTitle("m_{ee} [GeV]");
 char temp[50];
 sprintf(temp, "Number of events / %d GeV", 
 (int) (dataZmassBB->GetBinWidth(0)));
 yaxis->SetTitle(temp);

 TH1D* dataZmassEB = dataZmassBB->Clone("dataZmassEB");
 TH1D* dataZmassEE = dataZmassBB->Clone("dataZmassEE");



////////// ******* Trigger efficiency histogram ********************** //////
  const int EPTSBINS = 8;
  const double EPTSMIN = 20.;
  const double EPTSMAX = 100.;
 TH1D* effPt = new TH1D("effPt","BB",EPTSBINS, EPTSMIN, EPTSMAX);
 effPt->SetLineWidth(2);
 TAxis* xaxis = effPt->GetXaxis();
 TAxis* yaxis = effPt->GetYaxis();
 xaxis->SetNdivisions(510);
 xaxis->SetTitle("E_{T} [GeV]");
 char temp[50];
 sprintf(temp, "Number of events / %d GeV",(int) (effPt->GetBinWidth(0)));
 yaxis->SetTitle(temp);
 TH1D* effPtDenom = effPt->Clone("effPtDenom");
 TH1D* effPtB = effPt->Clone("effPtB");
 TH1D* effPtE = effPt->Clone("effPtE");
 TH1D* effPtBDenom = effPt->Clone("effPtBDenom");
 TH1D* effPtEDenom = effPt->Clone("effPtEDenom");





////////// ******* plot from data ********************** //////
  tpTree->Draw("mass>>dataZmassBB", "tag_gsfEle_isEB>0 && probe_gsfEle_isEB>0","goff");
  tpTree->Draw("mass>>dataZmassEB", "(tag_gsfEle_isEB>0 && probe_gsfEle_isEE>0) || (tag_gsfEle_isEE>0 && probe_gsfEle_isEB>0)","goff");
  tpTree->Draw("mass>>dataZmassEE", "tag_gsfEle_isEE>0 && probe_gsfEle_isEE>0","goff");
  tpTree->Draw("probe_sc_et>>effPtDenom", "","goff");
  tpTree->Draw("tag_sc_et>>+effPtDenom", "","goff");
  tpTree->Draw("probe_sc_et>>effPt", "probe_passing","goff");
  tpTree->Draw("tag_sc_et>>+effPt", "tag_passingHLT","goff");

  tpTree->Draw("probe_sc_et>>effPtBDenom", "probe_gsfEle_isEB>0","goff");
  tpTree->Draw("tag_sc_et>>+effPtBDenom", "tag_gsfEle_isEB>0","goff");
  tpTree->Draw("probe_sc_et>>effPtB", "probe_gsfEle_isEB>0 && probe_passing","goff");
  tpTree->Draw("tag_sc_et>>+effPtB", "tag_gsfEle_isEB>0 && tag_passingHLT","goff");

  tpTree->Draw("probe_sc_et>>effPtEDenom", "probe_gsfEle_isEE>0","goff");
  tpTree->Draw("tag_sc_et>>+effPtEDenom", "tag_gsfEle_isEE>0","goff");
  tpTree->Draw("probe_sc_et>>effPtE", "probe_gsfEle_isEE>0 && probe_passing","goff");
  tpTree->Draw("tag_sc_et>>+effPtE", "tag_gsfEle_isEE>0 && tag_passingHLT","goff");



  TEfficiency* hltEffVsPt =  new TEfficiency( *effPt, *effPtDenom);
  hltEffVsPt->SetConfidenceLevel(0.67);
  hltEffVsPt->SetStatisticOption(TEfficiency::kFCP);

  TEfficiency* hltEffVsPtB =  new TEfficiency( *effPtB, *effPtBDenom);
  hltEffVsPtB->SetConfidenceLevel(0.67);
  hltEffVsPtB->SetStatisticOption(TEfficiency::kFCP);
  hltEffVsPtB->SetLineColor(4);
  hltEffVsPtB->SetMarkerColor(4);


  TEfficiency* hltEffVsPtE =  new TEfficiency( *effPtE, *effPtEDenom);
  hltEffVsPtE->SetConfidenceLevel(0.67);
  hltEffVsPtE->SetStatisticOption(TEfficiency::kFCP);
  hltEffVsPtE->SetLineColor(2);
  hltEffVsPtE->SetMarkerColor(2);



/*
  TCanvas* mZeeBB = new TCanvas("mZeeBB","BB",500, 500);
  dataZmassBB->Draw();
  dataZmassBB->Draw("esame");
  gPad->SetGridx();
  mZeeBB->SaveAs("mZee_EBEB.eps");


  TCanvas* mZeeEB = new TCanvas("mZeeEB","EB",500, 500);
  dataZmassEB->Draw();
  dataZmassEB->Draw("esame");
  gPad->SetGridx();
  mZeeEB->SaveAs("mZee_EBEE.eps");


  TCanvas* mZeeEE = new TCanvas("mZeeEE","EE",500, 500);
  dataZmassEE->Draw();
  dataZmassEE->Draw("esame");
  gPad->SetGridx();
  mZeeEE->SaveAs("mZee_EEEE.eps");
*/


  TCanvas* effHLTVsPt = new TCanvas("effHLTVsPt","HLT eff",500, 500);
  hltEffVsPtE->SetTitle(";E_{T} [GeV]; Efficiency");
  hltEffVsPtE->Draw("AP");
  hltEffVsPt->Draw("Psame");
  hltEffVsPtB->Draw("Psame");
  hltEffVsPt->Draw("Psame");
  TLegend* leg2 = new TLegend(0.3,0.2,0.5,0.45);
  leg2->AddEntry(hltEffVsPt,"all","P");
  leg2->AddEntry(hltEffVsPtB,"EB","P");
  leg2->AddEntry(hltEffVsPtE,"EE","P");
  leg2->Draw("same");


}
