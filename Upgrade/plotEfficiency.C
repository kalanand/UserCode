

void plotEfficiency() 
{
  plotEfficiency("Pt", 0., 120.);
  plotEfficiency("Eta", -2.5, 2.5);


  plotEfficiency("Pt", 0., 120., 0, 10);
  plotEfficiency("Pt", 0., 120., 10, 25);
  plotEfficiency("Pt", 0., 120., 25, 50);


  plotEfficiency("Eta", -2.5, 2.5, 0, 10);
  plotEfficiency("Eta", -2.5, 2.5, 10, 25);
  plotEfficiency("Eta", -2.5, 2.5, 25, 50);

}





void plotEfficiency(char* var, double min, double max, int NPVmin=0, int NPVmax=50) {
  TFile*f1 =  TFile::Open("efficiency_tree_Zmumu_merged.root");
  f1->cd("muonEfficiency");
  TTree* tree1 = (TTree*) gDirectory->Get("Ntuple");

  int nBins = 100;
  if(var=="Pt") nBins = (int) ((max - min) / 5);
  if(var=="Eta") nBins = (int) ((max - min) / 0.2);



  ///// ----- Define your cuts ----------
  char cuts[100];
  sprintf(cuts, "nPVs>%d && nPVs<%d", NPVmin, NPVmax);


  ///// ----- Create all the histograms ----------
  TH1D* allReco = new TH1D("allReco", "", nBins, min, max);
  allReco->SetLineWidth(2);
  allReco->Sumw2();

  TH1D* isoReco = allReco->Clone("isoReco");
  isoReco->GetYaxis()->SetRangeUser(0.5, 1.1);
  isoReco->GetYaxis()->SetTitle("Efficiency");
  char* xtitle = var;
  if(var=="Pt") xtitle = "Muon p_{T} (GeV)";
  if(var=="Eta") xtitle = "Muon #eta";
  isoReco->GetXaxis()->SetTitle(xtitle);


  TH1D* l1isoReco = allReco->Clone("l1isoReco");
  l1isoReco->SetLineColor(2);
  l1isoReco->SetMarkerColor(2);
  TString varName = TString("reco") + TString(var);


  ///// ----- Draw from tree ----------
  tree1->Draw(varName+TString(">>allReco"),cuts,"goff");
  tree1->Draw(varName+TString(">>isoReco"), TString(cuts)+TString(" && dr04PFIsoCombinedRel<0.2"),"goff");
  tree1->Draw(varName+TString(">>l1isoReco"),TString(cuts)+TString(" && abs(l1gDR)>0.3"),"goff");


  //// ---- Now divide by allReco histogram to obtain efficiency ----
  isoReco->Divide(allReco);
  l1isoReco->Divide(allReco);



  // --- plot the efficiency histograms ------ 
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadLeftMargin(0.18);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(1.3);


  TCanvas* canEffIso = new TCanvas("canEffIso","",500,500);
  isoReco->Draw("e");
  l1isoReco->Draw("esame");
  TLegend* legend = new TLegend(0.4,0.25,0.9,0.45);
  legend->SetFillColor(0);
  legend->AddEntry(isoReco, "Offline Pf Iso (<0.2)", "PLE");  
  legend->AddEntry(l1isoReco, "Online L1 Iso (dR>0.3)", "PLE");  
  legend->Draw();
  cmsPrelim();

  TString plotname = TString("muonEfficiency_")+TString(var);
  char tempst[100];
  sprintf(tempst, "nPV_%d-%d", NPVmin, NPVmax);
  if(!(NPVmin==0 && NPVmax==50)) plotname += TString(tempst);

  canEffIso->SaveAs(plotname+TString(".png"));


  //--------- Finally clean up the memory -------
  delete allReco;
  delete isoReco;
  delete l1isoReco;
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
