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
 *   Plots muon Level-1 trigger turnon curve  
 *
 * Implementation details:
 *   Simple ROOT macro to plot L1 turnon curve.
 *   Match L1 muon to UCT15 calo deposit to compute isolation.
 *
 * History:
 *   
 *
 * Copyright (C) 2012 FNAL 
 ********************************************************************/


void plotTurnonCurve() 
{
  plotTurnonCurve("Pt", 16., 100.);
  // plotTurnonCurve("Eta", -2.5, 2.5);


//   plotTurnonCurve("Pt", 0., 120., 0, 10);
//   plotTurnonCurve("Pt", 0., 120., 10, 25);
//   plotTurnonCurve("Pt", 0., 120., 25, 50);


//   plotTurnonCurve("Eta", -2.5, 2.5, 0, 10);
//   plotTurnonCurve("Eta", -2.5, 2.5, 10, 25);
//   plotTurnonCurve("Eta", -2.5, 2.5, 25, 50);

}





void plotTurnonCurve(char* var, double min, double max, int NPVmin=0, int NPVmax=200) {
  TFile*f1 =  TFile::Open("efficiency_tree_Zmumu_merged.root");
  f1->cd("muonEfficiency");
  TTree* tree1 = (TTree*) gDirectory->Get("Ntuple");

  int nBins = 100;
  if(var=="Pt") nBins = (int) ((max - min) / 2);
  if(var=="Eta") nBins = (int) ((max - min) / 0.5);



  ///// ----- Define your cuts ----------
  char cuts[100];
  sprintf(cuts, "nPVs>%d && nPVs<%d && (abs(l1gDR)>0.2 || (l1gRegionEt/recoPt<0.5))", NPVmin, NPVmax);


  ///// ----- Create all the histograms ----------
  TH1D* allReco = new TH1D("allReco", "", nBins, min, max);
  allReco->SetLineWidth(2);
  allReco->Sumw2();

  TH1D* l1Pt10 = allReco->Clone("l1Pt10");
  if(var=="Pt") l1Pt10->GetYaxis()->SetRangeUser(0.3, 1.1);
  if(var=="Eta") l1Pt10->GetYaxis()->SetRangeUser(0.55, 1.05);

  l1Pt10->GetYaxis()->SetTitle("L1 efficiency");
  char* xtitle = var;
  if(var=="Pt") xtitle = "Offline muon p_{T} (GeV)";
  if(var=="Eta") xtitle = "Offline muon #eta";
  l1Pt10->GetXaxis()->SetTitle(xtitle);


  TH1D* l1Pt15 = allReco->Clone("l1Pt15");
  l1Pt15->SetLineColor(4);
  l1Pt15->SetMarkerColor(4);


  TH1D* l1Pt20 = allReco->Clone("l1Pt20");
  l1Pt20->SetLineColor(7);
  l1Pt20->SetMarkerColor(7);

  TH1D* l1Pt25 = allReco->Clone("l1Pt25");
  l1Pt25->SetLineColor(6);
  l1Pt25->SetMarkerColor(6);


  TH1D* l1Pt30 = allReco->Clone("l1Pt30");
  l1Pt30->SetLineColor(2);
  l1Pt30->SetMarkerColor(2);
  TString varName = TString("reco") + TString(var);




  ///// ----- Draw from tree ----------
  tree1->Draw(varName+TString(">>allReco"),cuts,"goff");
  tree1->Draw(varName+TString(">>l1Pt10"), TString(cuts)+TString(" && l1Pt>10."),"goff");
  tree1->Draw(varName+TString(">>l1Pt15"),TString(cuts)+TString(" && l1Pt>15."),"goff");
  tree1->Draw(varName+TString(">>l1Pt20"),TString(cuts)+TString(" && l1Pt>20."),"goff");
  tree1->Draw(varName+TString(">>l1Pt25"),TString(cuts)+TString(" && l1Pt>25."),"goff");
  tree1->Draw(varName+TString(">>l1Pt30"),TString(cuts)+TString(" && l1Pt>30."),"goff");


  //// ---- Now divide by allReco histogram to obtain efficiency ----
  l1Pt10->Divide(allReco);
  l1Pt15->Divide(allReco);
  l1Pt20->Divide(allReco);
  l1Pt25->Divide(allReco);
  l1Pt30->Divide(allReco);


  // --- plot the efficiency histograms ------ 
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadLeftMargin(0.18);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(1.3);


  TCanvas* canEffIso = new TCanvas("canEffIso","",500,500);
  l1Pt10->Draw("e");
  l1Pt10->Draw("hist same");
  l1Pt15->Draw("esame");
  l1Pt15->Draw("hist same");
  l1Pt20->Draw("esame");
  l1Pt20->Draw("hist same");
  l1Pt25->Draw("esame");
  l1Pt25->Draw("hist same");
  l1Pt30->Draw("esame");
  l1Pt30->Draw("hist same");


  TLegend* legend = new TLegend(0.5,0.22,0.9,0.45);
  legend->SetFillColor(0);
  legend->AddEntry(l1Pt10, "L1 p_{T} > 10 GeV", "PLE");  
  legend->AddEntry(l1Pt15, "L1 p_{T} > 15 GeV", "PLE");  
  legend->AddEntry(l1Pt20, "L1 p_{T} > 20 GeV", "PLE"); 
  legend->AddEntry(l1Pt25, "L1 p_{T} > 25 GeV", "PLE"); 
  legend->AddEntry(l1Pt30, "L1 p_{T} > 30 GeV", "PLE"); 
  legend->Draw();
  cmsPrelim();
  canEffIso->SetGridx();
  canEffIso->SetGridy();


  TString plotname = TString("muonTurnonCurve_")+TString(var);
  char tempst[100];
  sprintf(tempst, "nPV_%d-%d", NPVmin, NPVmax);
  if(!(NPVmin==0 && NPVmax==200)) plotname += TString(tempst);

  canEffIso->SaveAs(plotname+TString(".png"));
  canEffIso->SaveAs(plotname+TString(".pdf"));


  //--------- Finally clean up the memory -------
  delete allReco;
  delete l1Pt10;
  delete l1Pt15;
  delete l1Pt20;
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
