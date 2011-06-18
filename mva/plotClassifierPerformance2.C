{
 



////////////// BDT ///////////////////////////
  TFile* f2 = TFile::Open("plots_BDT/rejBvsS.root");
  c = (TCanvas*) f2.Get("c");
  TH1F* performBDT = c->GetPrimitive("MVA_BDT_rejBvsS");
  performBDT->SetLineColor(4);



////////////// CIC ///////////////////////////
  TFile fTest("TMVA_BDT.root");
  TTree* tTest = (TTree*) fTest.Get("TestTree");
  float effCIC[9];
  float misCIC[9];
  TH1D* htemp = new TH1D("htemp","", 1000, -0.02, 1.02);
  TString sigcut = "isSignal";
  TString bkgcut = "isBackground";
  tTest->Draw("(eidVeryLoose==15)>>htemp",sigcut,"goff");
  effCIC[0]=htemp->GetMean();
  tTest->Draw("(eidLoose==15)>>htemp",sigcut,"goff");
  effCIC[1]=htemp->GetMean();
  tTest->Draw("(eidMedium==15)>>htemp",sigcut,"goff");
  effCIC[2]=htemp->GetMean();
  tTest->Draw("(eidTight==15)>>htemp",sigcut,"goff");
  effCIC[3]=htemp->GetMean();
  tTest->Draw("(eidSuperTight==15)>>htemp",sigcut,"goff");
  effCIC[4]=htemp->GetMean();
  tTest->Draw("(eidHyperTight1==15)>>htemp",sigcut,"goff");
  effCIC[5]=htemp->GetMean();
  tTest->Draw("(eidHyperTight2==15)>>htemp",sigcut,"goff");
  effCIC[6]=htemp->GetMean();
  tTest->Draw("(eidHyperTight3==15)>>htemp",sigcut,"goff");
  effCIC[7]=htemp->GetMean();
  tTest->Draw("(eidHyperTight4==15)>>htemp",sigcut,"goff");
  effCIC[8]=htemp->GetMean();



  tTest->Draw("(eidVeryLoose==15)>>htemp",bkgcut,"goff");
  misCIC[0]=1.0 - htemp->GetMean();
  tTest->Draw("(eidLoose==15)>>htemp",bkgcut,"goff");
  misCIC[1]=1.0 - htemp->GetMean();
  tTest->Draw("(eidMedium==15)>>htemp",bkgcut,"goff");
  misCIC[2]=1.0 - htemp->GetMean();
  tTest->Draw("(eidTight==15)>>htemp",bkgcut,"goff");
  misCIC[3]=1.0 - htemp->GetMean();
  tTest->Draw("(eidSuperTight==15)>>htemp",bkgcut,"goff");
  misCIC[4]=1.0 - htemp->GetMean();
  tTest->Draw("(eidHyperTight1==15)>>htemp",bkgcut,"goff");
  misCIC[5]=1.0 - htemp->GetMean();
  tTest->Draw("(eidHyperTight2==15)>>htemp",bkgcut,"goff");
  misCIC[6]=1.0 - htemp->GetMean();
  tTest->Draw("(eidHyperTight3==15)>>htemp",bkgcut,"goff");
  misCIC[7]=1.0 - htemp->GetMean();
  tTest->Draw("(eidHyperTight4==15)>>htemp",bkgcut,"goff");
  misCIC[8]=1.0 - htemp->GetMean();


////////////// VBTF WP ///////////////////////////
  float effWP[6];
  float misWP[6];
  tTest->Draw("isWP95>>htemp",sigcut,"goff");
  effWP[0]=htemp->GetMean();
  tTest->Draw("isWP90>>htemp",sigcut,"goff");
  effWP[1]=htemp->GetMean();

  tTest->Draw("isWP85>>htemp",sigcut,"goff");
  effWP[2]=htemp->GetMean();
  tTest->Draw("isWP80>>htemp",sigcut,"goff");
  effWP[3]=htemp->GetMean();
  tTest->Draw("isWP70>>htemp",sigcut,"goff");
  effWP[4]=htemp->GetMean();
  tTest->Draw("isWP60>>htemp",sigcut,"goff");
  effWP[5]=htemp->GetMean();

  tTest->Draw("isWP95>>htemp",bkgcut,"goff");
  misWP[0]=1.0 - htemp->GetMean();
  tTest->Draw("isWP90>>htemp",bkgcut,"goff");
  misWP[1]=1.0 - htemp->GetMean();
  tTest->Draw("isWP85>>htemp",bkgcut,"goff");
  misWP[2]=1.0 - htemp->GetMean();
  tTest->Draw("isWP80>>htemp",bkgcut,"goff");
  misWP[3]=1.0 - htemp->GetMean();
  tTest->Draw("isWP70>>htemp",bkgcut,"goff");
  misWP[4]=1.0 - htemp->GetMean();
  tTest->Draw("isWP60>>htemp",bkgcut,"goff");
  misWP[5]=1.0 - htemp->GetMean();

////////////// VBTF WPNew ///////////////////////////
  float effWPNew[6];
  float misWPNew[6];
  tTest->Draw("isWP95New>>htemp",sigcut,"goff");
  effWPNew[0]=htemp->GetMean();
  tTest->Draw("isWP90New>>htemp",sigcut,"goff");
  effWPNew[1]=htemp->GetMean();
  tTest->Draw("isWP85New>>htemp",sigcut,"goff");
  effWPNew[2]=htemp->GetMean();
  tTest->Draw("isWP80New>>htemp",sigcut,"goff");
  effWPNew[3]=htemp->GetMean();
  tTest->Draw("isWP70New>>htemp",sigcut,"goff");
  effWPNew[4]=htemp->GetMean();
  tTest->Draw("isWP60New>>htemp",sigcut,"goff");
  effWPNew[5]=htemp->GetMean();

  tTest->Draw("isWP95New>>htemp",bkgcut,"goff");
  misWPNew[0]=1.0 - htemp->GetMean();
  tTest->Draw("isWP90New>>htemp",bkgcut,"goff");
  misWPNew[1]=1.0 - htemp->GetMean();
  tTest->Draw("isWP85New>>htemp",bkgcut,"goff");
  misWPNew[2]=1.0 - htemp->GetMean();
  tTest->Draw("isWP80New>>htemp",bkgcut,"goff");
  misWPNew[3]=1.0 - htemp->GetMean();
  tTest->Draw("isWP70New>>htemp",bkgcut,"goff");
  misWPNew[4]=1.0 - htemp->GetMean();
  tTest->Draw("isWP60New>>htemp",bkgcut,"goff");
  misWPNew[5]=1.0 - htemp->GetMean();

// ////////////// Likelihood ///////////////////////////
   const int nBins = 50;
  int denom1 = (int)tTest->Draw("mass","isSignal","goff");
  int denom0 = (int)tTest->Draw("mass","isBackground","goff");
  
  float effLH[nBins];
  float misLH[nBins];
  float adaLH[nBins];
  
  TString strg;
  int num=0;
  double x=0;

  for (int cbin=0; cbin<nBins; cbin++) {
     x = (float)(cbin)/ (float) nBins;
    adaLH[cbin] = x;
    strg =  Form("isSignal && eidLikelihood>%f", x);
    num = (int)  tTest->Draw("mass", strg,"goff");
    effLH[cbin] = (float)num/(float)denom1;
    strg =  Form("isBackground && !(abs(dcot)<0.02 && abs(dist)<0.02) && misHits==0 && ((trackIso+ecalIso+hcalIso)/et-1.)<0.0 && bremFraction < 0.5 && eidLikelihood>%f", x);
    num = (int) tTest->Draw("mass", strg,"goff");
    misLH[cbin] = 1.0 - (float)num/(float)denom0;
  }


//////////////////////////////////////////////////////////


  TGraph* performCIC    = new TGraph(9, effCIC, misCIC);
  performCIC->SetMarkerColor(2);
  performCIC->SetMarkerStyle(20);
  performCIC->SetMarkerSize(1.2);
  TGraph* performWP    = new TGraph(6, effWP, misWP);
  performWP->SetMarkerColor(1);
  performWP->SetMarkerStyle(21);
  TGraph* performWPNew = new TGraph(6, effWPNew, misWPNew);
  performWPNew->SetMarkerColor(1);
  performWPNew->SetMarkerStyle(25);
  TGraph* performLH    = new TGraph(nBins, effLH, misLH);
  performLH->SetLineColor(6);
  performLH->SetLineWidth(2);
 

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetPadLeftMargin(0.19);
  tdrStyle->SetPadBottomMargin(0.16);
  tdrStyle->SetLegendBorderSize(0);


  const double purMIN = 0.965;
  const double purMAX = 1.0035;


  TCanvas* percan = new TCanvas("percan","");
  performCIC->GetXaxis()->SetTitle("Efficiency");
  performCIC->GetYaxis()->SetTitle("Bkg Rejection    ");
  performCIC->GetYaxis()->SetTitleOffset(1.5);
  performCIC->GetYaxis()->SetRangeUser(purMIN, purMAX);
  performCIC->GetXaxis()->SetRangeUser(0.2, 1.01);

  performCIC->SetTitle("");
  performCIC->Draw("ap");
  performBDT->Draw("csame");
  performWPNew->Draw("psame");
  performWP->Draw("psame");
  performLH->Draw("csame");
  percan->SetGridx();
  percan->SetGridy();



   TLatex *tex = new TLatex(0.3041382,0.9704085,"Cuts in Categories (Matteo)");
   tex->SetTextColor(2);
   tex->SetTextSize(0.03846154);
   tex->SetLineWidth(2);
   tex->Draw();
   tex = new TLatex(0.3094229,0.9725385,"Boosted Decision Tree (KM)");
   tex->SetTextColor(4);
   tex->SetTextSize(0.03496503);
   tex->SetLineWidth(2);
   tex->Draw();
   tex = new TLatex(0.3058998,0.9683637,"Likelihood (Emanuele)");
   tex->SetTextColor(6);
   tex->SetTextSize(0.03496503);
   tex->SetLineWidth(2);
   tex->Draw();
   percan->Modified();
   percan->cd();
   percan->SetSelected(percan);
   





  TString outfilename = "perform-2";
  percan->SaveAs( outfilename + TString(".root"));
  percan->SaveAs( outfilename + TString(".eps"));
  percan->SaveAs( outfilename + TString(".gif"));
//   delete percan;
//   delete perform;










/*
  TGraph* perform1    = new TGraph(nBins, adaLH, effLH);
  perform1->GetXaxis()->SetTitle("Classifier output");
  perform1->GetYaxis()->SetTitle("Efficiency    ");
  perform1->SetTitle("");
  TCanvas* percan1 = new TCanvas("percan1","");
  perform1->Draw("AP");
//   TString outfilename = "eff-";
//   percan1->SaveAs( outfilename + TString(".gif"));
//   percan1->SaveAs( outfilename + TString(".root"));
//   delete percan1;
//   delete perform1;


  TGraph* perform2    = new TGraph(nBins, adaLH, misLH);
  perform2->GetXaxis()->SetTitle("Classifier output");
  perform2->GetYaxis()->SetTitle("Bkg Rej.    ");
  perform2->SetTitle("");
  TCanvas* percan2 = new TCanvas("percan2","");
  perform2->Draw("AP");
//   TString outfilename = "pur-";
//   percan2->SaveAs( outfilename + TString(".gif"));
//   percan2->SaveAs( outfilename + TString(".root"));
//   delete percan2;
//   delete perform2;
*/
}

