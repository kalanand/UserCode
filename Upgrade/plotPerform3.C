void plotPerform3() {
  for(int i=0; i<4; i++) {
    plotPerformMomBin(i);
  }
}



void plotPerformMomBin(int hypo=0) {

  TString sigCut;
  TString bkgCut;
  TString sigVar;
  if(hypo==0) {
    sigCut = "Vars.classification==0";
    bkgCut = "Vars.classification==1";
    sigVar = "Vars.multi0";
  }
  if(hypo==1) {
    sigCut = "Vars.classification==1";
    bkgCut = "Vars.classification==0";
    sigVar = "Vars.multi1";
  }
  if(hypo==2) {
    sigCut = "Vars.classification==2";
    bkgCut = "Vars.classification==0";
    sigVar = "Vars.multi2";
  }
  if(hypo==3) {
    sigCut = "Vars.classification==3";
    bkgCut = "Vars.classification==1";
    sigVar = "Vars.multi3";
  }
  
  
  TCut Cut1(sigCut);
  TCut Cut0(bkgCut);

  const int nBins = 200;


  TFile f("sprdir/multiclass_val.root");
  TTree* t = f.Get("ClassRecord");


  int denom1 = (int)t->Draw(sigVar,Cut1,"goff");
  int denom0 = (int)t->Draw(sigVar,Cut0,"goff");
  
  float eff[nBins];
  float mis[nBins];
  float ada[nBins];
  
  for (int cbin=0; cbin<nBins; cbin++) {
    double x = (float)(cbin*3)/ (float) nBins;
    ada[cbin] = x;
    TString strg =  Form("Vars.multi%d<%f", hypo,x);
    TCut aCut(strg);
    int sgnl = (int) t->Draw(sigVar,(Cut1) && (aCut),"goff");
    eff[cbin] = (float)sgnl/(float)denom1;
    int bkgd = (int) t->Draw(sigVar,(Cut0) && (aCut),"goff");
    mis[cbin] = 1.0 - (float)bkgd/(float)denom0;
  }
  
  
  TGraph* perform    = new TGraph(nBins, eff, mis);
  perform->SetMarkerColor(2);
  perform->SetMarkerStyle(21);
  perform->GetXaxis()->SetTitle("Efficiency");
  perform->GetYaxis()->SetTitle("Bkg Rejection    ");
  perform->SetTitle("");
  TCanvas* percan = new TCanvas("percan","");
  perform->Draw("AP");
  TString outfilename = "perform-";
  outfilename += hypo;
  percan->SaveAs( outfilename + TString(".root"));
  percan->SaveAs( outfilename + TString(".eps"));
  percan->SaveAs( outfilename + TString(".gif"));
  delete percan;
  delete perform;


  TGraph* perform1    = new TGraph(nBins, ada, eff);
  perform1->GetXaxis()->SetTitle("output");
  perform1->GetYaxis()->SetTitle("efficiency    ");
  perform1->SetTitle("");
  TCanvas* percan1 = new TCanvas("percan1","");
  perform1->Draw("AP");
  TString outfilename = "eff-";
  outfilename += hypo;
  percan1->SaveAs( outfilename + TString(".gif"));
  percan1->SaveAs( outfilename + TString(".root"));
  delete percan1;
  delete perform1;


  TGraph* perform2    = new TGraph(nBins, ada, mis);
  perform2->GetXaxis()->SetTitle("output");
  perform2->GetYaxis()->SetTitle("Bkg Rej.    ");
  perform2->SetTitle("");
  TCanvas* percan2 = new TCanvas("percan2","");
  perform2->Draw("AP");
  TString outfilename = "pur-";
  outfilename += hypo;
  percan2->SaveAs( outfilename + TString(".gif"));
  percan2->SaveAs( outfilename + TString(".root"));
  delete percan2;
  delete perform2;
}

