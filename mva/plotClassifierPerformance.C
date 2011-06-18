void plotClassifierPerformance() {
  const int nBins = 500;
  TFile fsig("signal.root");
  TTree* tsig = fsig.Get("tree");
  TFile fbkg("background.root");
  TTree* tbkg = fbkg.Get("tree");


  int denom1 = (int)tsig->Draw("mass","","goff");
  int denom0 = (int)tbkg->Draw("mass","","goff");
  
  float effLH[nBins];
  float misLH[nBins];
  float adaLH[nBins];
  
  float effBDT[nBins];
  float misBDT[nBins];
  float adaBDT[nBins];

  TString strg;
  int num=0;
  double x=0;






////////////// Likelihood ///////////////////////////
  for (int cbin=0; cbin<nBins; cbin++) {
     //x = (float)(cbin*3)/ (float) nBins;
     x = 0.96+0.04*(float)(cbin)/ (float) nBins;
    adaLH[cbin] = x;
    strg =  Form("probe_myLikelihood>%f", x);
    TCut aCut(strg);
    num = (int) tsig->Draw("mass", aCut,"goff");
    effLH[cbin] = (float)num/(float)denom1;
    num = (int) tbkg->Draw("mass", aCut,"goff");
    misLH[cbin] = 1.0 - (float)num/(float)denom0;
  }
  
  

////////////// BDT ///////////////////////////
  for (int cbin=0; cbin<nBins; cbin++) {
     x = (float)(cbin*3)/ (float) nBins;
     x = -0.8+(float)(cbin)/ (float) nBins;
    adaBDT[cbin] = x;
    strg =  Form("probe_myBDTresponse>%f", x);
    TCut aCut(strg);
    num = (int) tsig->Draw("mass", aCut,"goff");
    effBDT[cbin] = (float)num/(float)denom1;
    num = (int) tbkg->Draw("mass", aCut,"goff");
    misBDT[cbin] = 1.0 - (float)num/(float)denom0;
  }




////////////// CIC ///////////////////////////
  float effCIC[6];
  float misCIC[6];
  effCIC[0]=(float) tsig->Draw("mass", "probe_passConvRej && probe_isCiCVeryLoose","goff")/denom1;
  effCIC[1]=(float) tsig->Draw("mass", "probe_passConvRej && probe_isCiCLoose","goff")/denom1;
  effCIC[2]=(float) tsig->Draw("mass", "probe_passConvRej && probe_isCiCMedium","goff")/denom1;
  effCIC[3]=(float) tsig->Draw("mass", "probe_passConvRej && probe_isCiCTight","goff")/denom1;
  effCIC[4]=(float) tsig->Draw("mass", "probe_passConvRej && probe_isCiCSuperTight","goff")/denom1;
  effCIC[5]=(float) tsig->Draw("mass", "probe_passConvRej && probe_isCiCHyperTight","goff")/denom1;
  misCIC[0]=1.0 - (float) tbkg->Draw("mass", "probe_isCiCVeryLoose","goff")/denom0;
  misCIC[1]=1.0 - (float) tbkg->Draw("mass", "probe_isCiCLoose","goff")/denom0;
  misCIC[2]=1.0 - (float) tbkg->Draw("mass", "probe_isCiCMedium","goff")/denom0;
  misCIC[3]=1.0 - (float) tbkg->Draw("mass", "probe_isCiCTight","goff")/denom0;
  misCIC[4]=1.0 - (float) tbkg->Draw("mass", "probe_isCiCSuperTight","goff")/denom0;
  misCIC[5]=1.0 - (float) tbkg->Draw("mass", "probe_isCiCHyperTight","goff")/denom0;



////////////// VBTF WP ///////////////////////////
  float effWP[5];
  float misWP[5];
  effWP[0]=(float) tsig->Draw("mass", "probe_isWP95","goff")/denom1;
  effWP[1]=(float) tsig->Draw("mass", "probe_isWP90","goff")/denom1;
  effWP[2]=(float) tsig->Draw("mass", "probe_isWP80","goff")/denom1;
  effWP[3]=(float) tsig->Draw("mass", "probe_isWP70","goff")/denom1;
  effWP[4]=(float) tsig->Draw("mass", "probe_isWP60","goff")/denom1;
  misWP[0]=1.0 - (float) tbkg->Draw("mass", "probe_isWP95","goff")/denom0;
  misWP[1]=1.0 - (float) tbkg->Draw("mass", "probe_isWP90","goff")/denom0;
  misWP[2]=1.0 - (float) tbkg->Draw("mass", "probe_isWP80","goff")/denom0;
  misWP[3]=1.0 - (float) tbkg->Draw("mass", "probe_isWP70","goff")/denom0;
  misWP[4]=1.0 - (float) tbkg->Draw("mass", "probe_isWP60","goff")/denom0;



 
  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadBottomMargin(0.16);
  tdrStyle->SetLegendBorderSize(0);


/* --------------------------------------------------------

  TGraph* perform    = new TGraph(nBins, effLH, misLH);
  perform->SetMarkerColor(2);
  perform->SetMarkerStyle(21);
  TGraph* performBDT    = new TGraph(nBins, effBDT, misBDT);
  performBDT->SetMarkerColor(4);
  performBDT->SetMarkerStyle(21);
//   TGraph* performCIC    = new TGraph(6, effCIC, misCIC);
//   performCIC->SetMarkerColor(1);
//   performCIC->SetMarkerStyle(21);
  TGraph* performWP    = new TGraph(6, effWP, misWP);
  performWP->SetMarkerColor(1);
  performWP->SetMarkerStyle(21);



  TCanvas* percan = new TCanvas("percan","");
  performWP->GetXaxis()->SetTitle("Efficiency");
  performWP->GetYaxis()->SetTitle("Bkg Rejection    ");
  performWP->GetYaxis()->SetRangeUser(0.9, 1.02);
  performWP->GetXaxis()->SetRangeUser(0.58, 1.0005);
  performWP->SetMarkerSize(1.4);
  performWP->SetTitle("");
  performWP->Draw("AP");
  perform->Draw("P");
  performBDT->Draw("P");


//   TString outfilename = "perform-";
//   percan->SaveAs( outfilename + TString(".root"));
//   percan->SaveAs( outfilename + TString(".eps"));
//   percan->SaveAs( outfilename + TString(".gif"));
//   delete percan;
//   delete perform;


 -------------------------------------------------------- */








  TGraph* perform1    = new TGraph(nBins, adaBDT, effBDT);
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


  TGraph* perform2    = new TGraph(nBins, adaBDT, misBDT);
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

}

