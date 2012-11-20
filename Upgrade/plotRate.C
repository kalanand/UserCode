

void plotRate() 
{
   plotRate("pt", 0., 60.);
  // plotRate("eta", -2.5, 2.5);


//   plotRate("pt", 0., 120., 0, 10);
//   plotRate("pt", 0., 120., 10, 25);
//   plotRate("pt", 0., 120., 25, 50);


//   plotRate("eta", -2.5, 2.5, 0, 10);
//   plotRate("eta", -2.5, 2.5, 10, 25);
//   plotRate("eta", -2.5, 2.5, 25, 50);

}





void plotRate(char* var, double min, double max) {
  TFile*f1 =  TFile::Open("rate_tree_muon_PU66_merged.root");
  // f1->cd("muonL1Rate");
  TTree* tree1 = (TTree*) f1->GetDirectory("muonL1Rate")->Get("Ntuple");


  // f1->cd("JetUCTRate");
  TTree* tree2 = (TTree*) f1->GetDirectory("jetUCTRate")->Get("Ntuple");
  tree1->AddFriend(tree2, "TF");



  int nBins = 100;
  if(var=="pt") nBins = (int) ((max - min) / 10);
  if(var=="eta") nBins = (int) ((max - min) / 0.5);



  ///// ----- Define your cuts ----------
  char cuts[100];
  sprintf(cuts, "pt[0]>0.&& sqrt((eta[0]-TF.eta[0])**2 + (abs(abs(phi[0]-TF.phi[0])-TMath::Pi())-TMath::Pi())**2) <2.");



  ///// ----- Create all the histograms ----------
  TH1D* allReco = new TH1D("allReco", "", nBins, min, max);
  allReco->SetLineWidth(2);
  allReco->Sumw2();
  allReco->GetYaxis()->SetTitle("Rate / Current L1 rate");
  char* xtitle = var;
  if(var=="pt") xtitle = "Muon p_{T} (GeV)";
  if(var=="eta") xtitle = "Muon #eta";
  allReco->GetXaxis()->SetTitle(xtitle);
  allReco->GetYaxis()->SetRangeUser(0., 1.);
  allReco->SetMaximum(1.);

  TH1D* isoReco = allReco->Clone("isoReco");



  TH1D* l1isoReco = allReco->Clone("l1isoReco");
  l1isoReco->SetLineColor(2);
  l1isoReco->SetMarkerColor(2);


  TH1D* l1isoReco2 = allReco->Clone("l1isoReco2");
  l1isoReco2->SetLineColor(6);
  l1isoReco2->SetMarkerColor(6);
  TString varName = TString(var);



  ///// ----- Draw from tree ----------
  tree1->Draw(varName+TString("[0]>>allReco"),cuts,"goff");
  tree1->Draw(varName+TString("[0]>>isoReco"), TString(cuts)+TString(" && abs(TF.regionPt[0]/pt[0]-1.)<0.5"),"goff");
  tree1->Draw(varName+TString("[0]>>l1isoReco"),TString(cuts)+TString(" && abs(TF.regionPt[0]/pt[0]-1.)<0.3"),"goff");
  tree1->Draw(varName+TString("[0]>>l1isoReco2"),TString(cuts)+TString(" && abs(TF.regionPt[0]/pt[0]-1.)<0.2"),"goff");

  //// ---- Now divide by allReco histogram to obtain efficiency ----
  isoReco->Divide(allReco);
  l1isoReco->Divide(allReco);
  l1isoReco2->Divide(allReco);



  // --- plot the efficiency histograms ------ 
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadLeftMargin(0.18);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(1.3);


  // for (int i=0; i<= isoReco->GetNbinsX(); ++i) {isoReco->SetBinError(i, 0.); }

  TCanvas* canEffIso = new TCanvas("canEffIso","",500,500);
  isoReco->Draw("");
  l1isoReco->Draw("esame");
  l1isoReco2->Draw("esame");
  TLegend* legend = new TLegend(0.6,0.68,0.9,0.88);
  legend->SetFillColor(0);
  legend->AddEntry(isoReco, "relIso < 0.5", "PLE");  
  legend->AddEntry(l1isoReco, "relIso < 0.3", "PLE");  
  legend->AddEntry(l1isoReco2, "relIso < 0.2", "PLE"); 
  legend->Draw();
  cmsPrelim();
  // canEffIso->SetLogy(1);

  TString plotname = TString("muonRate_")+TString(var);
  char tempst[100];

  canEffIso->SaveAs(plotname+TString(".png"));


  //--------- Finally clean up the memory -------
//   delete allReco;
//   delete isoReco;
//   delete l1isoReco;
//   delete canEffIso;
//   delete legend;
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
