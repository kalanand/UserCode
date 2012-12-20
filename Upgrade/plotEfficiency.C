/*******************************************************************
 * Project: CMS detector at the CERN
 *
 * Package: Presently in the user code
 *
 *
 * Authors:
 *
 *   Kalanand Mishra, Fermilab - kalanand@fnal.gov
 *
 * Description:
 *   Plots muon Level-1 trigger efficiency  
 *
 * Implementation details:
 *   Simple ROOT macro to plot L1 efficiency.
 *   Match L1 muon to UCT15 calo deposit to compute isolation.
 *
 * History:
 *   
 *
 * Copyright (C) 2012 FNAL 
 ********************************************************************/


void plotEfficiency() 
{
  plotEfficiency("Pt", 0., 120.);
  plotEfficiency("Eta", -2.5, 2.5);


  plotEfficiency("Pt", 0., 120., 14);
  plotEfficiency("Pt", 0., 120., 18);
  plotEfficiency("Pt", 0., 120., 24);


  plotEfficiency("Eta", -2.5, 2.5, 14);
  plotEfficiency("Eta", -2.5, 2.5, 18);
  plotEfficiency("Eta", -2.5, 2.5, 24);

}





void plotEfficiency(char* var, double min, double max, int l1PtMin=0) {
  TFile*f1 =  TFile::Open("efficiency_tree_Zmumu_merged.root");
  f1->cd("muonEfficiency");
  TTree* tree1 = (TTree*) gDirectory->Get("Ntuple");

  int nBins = 100;
  if(var=="Pt") nBins = (int) ((max - min) / 5);
  if(var=="Eta") nBins = (int) ((max - min) / 0.2);



  ///// ----- Define your cuts ----------
  char l1PtMinStr[100];
  sprintf(l1PtMinStr, "l1Pt>%d", l1PtMin);
  TCut l1PtMinCut(l1PtMinStr);
  TCut cutNPV0("(nPVs>-1)");
  TCut cutNPV1("(nPVs>=0 && nPVs<15)");
  TCut cutNPV2("(nPVs>=15 && nPVs<25)");
  TCut cutNPV3("(nPVs>=25 && nPVs<200)");
  TCut l1isoCut("(abs(l1gDR)>0.2 || (l1gRegionEt/l1Pt<0.5))");


  ///// ----- Create all the histograms ----------
  TH1D* allReco = new TH1D("allReco", "", nBins, min, max);
  allReco->SetLineWidth(2);
  allReco->Sumw2();
  TH1D* allRecoNPV1 = allReco->Clone("allRecoNPV1");
  TH1D* allRecoNPV2 = allReco->Clone("allRecoNPV2");
  TH1D* allRecoNPV3 = allReco->Clone("allRecoNPV3");



  TH1D* l1iso = allReco->Clone("l1iso");
  TH1D* l1isoNPV1 = allReco->Clone("l1isoNPV1");
  TH1D* l1isoNPV2 = allReco->Clone("l1isoNPV2");
  TH1D* l1isoNPV3 = allReco->Clone("l1isoNPV3");


  l1isoNPV1->SetLineColor(4);
  l1isoNPV1->SetMarkerColor(4);
  l1isoNPV2->SetLineColor(6);
  l1isoNPV2->SetMarkerColor(6);
  l1isoNPV3->SetLineColor(2);
  l1isoNPV3->SetMarkerColor(2);

  l1iso->GetYaxis()->SetRangeUser(0.5, 1.1);
  l1iso->GetYaxis()->SetTitle("L1 Efficiency");
  char* xtitle = var;
  if(var=="Pt") xtitle = "Offline muon p_{T} (GeV)";
  if(var=="Eta") xtitle = "Offline muon #eta";
  l1iso->GetXaxis()->SetTitle(xtitle);





  ///// ----- Draw from tree ----------
  TString varName = TString("reco") + TString(var);
  tree1->Draw(varName+TString(">>allReco"),cutNPV0,"goff");
  tree1->Draw(varName+TString(">>allRecoNPV1"),cutNPV1,"goff");
  tree1->Draw(varName+TString(">>allRecoNPV2"),cutNPV2,"goff");
  tree1->Draw(varName+TString(">>allRecoNPV3"),cutNPV3,"goff");

  tree1->Draw(varName+TString(">>l1iso"),l1PtMinCut && cutNPV0 && l1isoCut,"goff");
  tree1->Draw(varName+TString(">>l1isoNPV1"),l1PtMinCut && cutNPV1 && l1isoCut,"goff");
  tree1->Draw(varName+TString(">>l1isoNPV2"),l1PtMinCut && cutNPV2 && l1isoCut,"goff");
  tree1->Draw(varName+TString(">>l1isoNPV3"),l1PtMinCut && cutNPV3 && l1isoCut,"goff");




  //// ---- Now divide by allReco histogram to obtain efficiency ----
  l1iso->Divide(allReco);
  l1isoNPV1->Divide(allRecoNPV1);
  l1isoNPV2->Divide(allRecoNPV2);
  l1isoNPV3->Divide(allRecoNPV3);


  // --- plot the efficiency histograms ------ 
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadLeftMargin(0.18);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(1.3);


  TCanvas* canEffIso = new TCanvas("canEffIso","",500,500);
  l1iso->SetFillColor(5);
  l1iso->Draw("e3");
  l1iso->Draw("esame");
  l1isoNPV1->Draw("psame");
  l1isoNPV2->Draw("psame");
  l1isoNPV3->Draw("psame");
  l1iso->Draw("esame");

  TLegend* legend = new TLegend(0.45,0.15,0.9,0.45);
  legend->SetFillColor(0);
  legend->AddEntry(l1iso, "All NPV", "PLE");  
  legend->AddEntry(l1isoNPV1, "NPV: 0-15", "PLE");  
  legend->AddEntry(l1isoNPV2, "NPV: 15-25", "PLE");  
  legend->AddEntry(l1isoNPV3, "NPV: 25-above", "PLE"); 
  legend->Draw();
  cmsPrelim();

  TString plotname = TString("muonEfficiency_")+TString(var);
  char tempst[100];
  sprintf(tempst, "_l1Pt_%d", l1PtMin);
  plotname += TString(tempst);
  canEffIso->SetGridy();
  canEffIso->SaveAs(plotname+TString(".png"));
  canEffIso->SaveAs(plotname+TString(".pdf"));


  //--------- Finally clean up the memory -------
  delete allReco;
  delete allRecoNPV1;
  delete allRecoNPV2;
  delete allRecoNPV3;
  delete l1iso;
  delete l1isoNPV1;
  delete l1isoNPV2;
  delete l1isoNPV3;

  delete canEffIso;
  delete legend;



}






////CMS Preliminary label and lumu
void cmsPrelim()
{
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);

  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.96,"#sqrt{s} = 8 TeV");
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.96,"CMS preliminary 2012");
}
