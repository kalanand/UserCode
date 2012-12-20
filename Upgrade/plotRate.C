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
 *   Plots muon Level-1 trigger rate 
 *
 * Implementation details:
 *   Simple ROOT macro to plot L1 rate.
 *   Match L1 muon to UCT15 calo deposit to compute isolation.
 *   User can subtract pileup from L1 isolation deposit if desired.
 *
 * History:
 *   
 *
 * Copyright (C) 2012 FNAL 
 ********************************************************************/


void plotRate() 
{
   //plotRate("pt", 0., 150.);
   plotRate("eta", -2.5, 2.5);

}





void plotRate(char* var, double min, double max, bool applyPUsub=false) {
   TFile*f1 =  TFile::Open("rate_tree_muon_Run2012C-Commissioning.root");
   // ----- get muon tree first -----------
   TTree* tree1 = (TTree*) f1->GetDirectory("muonL1Rate")->Get("Ntuple");
   
   
   // ---- get UCT15 jet tree ----------------
   TTree* tree2 = (TTree*) f1->GetDirectory("jetUCTRate")->Get("Ntuple");
   
   
   // ---- define histogram binning ----------------
   int nBins = 100;
   if(var=="pt") nBins = (int) ((max - min) / 5);
   if(var=="eta") nBins = (int) ((max - min) / 0.5);



   ///// ----- Create all the histograms ----------
   //----- starting with the current/non-isolated trigger rate ----
   TH1D* denominator = new TH1D("denominator", "", nBins, min, max);
   denominator->SetLineWidth(2);
   denominator->Sumw2();
   denominator->GetYaxis()->SetTitle("Rate / Current L1 rate");
   char* xtitle = var;
   if(var=="pt") xtitle = "L1 muon p_{T} (GeV)";
   if(var=="eta") xtitle = "L1 muon #eta";
   denominator->GetXaxis()->SetTitle(xtitle);
   denominator->GetYaxis()->SetRangeUser(0., 1.);
   denominator->SetMaximum(1.);
   denominator->GetXaxis()->SetLabelSize(0.05);
   denominator->GetXaxis()->SetTitleSize(0.05);
   denominator->GetYaxis()->SetLabelSize(0.05);
   denominator->GetYaxis()->SetTitleSize(0.05);
   denominator->GetYaxis()->SetTitleOffset(1.3);
   denominator->SetMarkerStyle(20);

   //----- for isolated trigger rates: relIso WP = 0.5, 0.3, 0.2 ----
   TH1D* rateHist05 = denominator->Clone("rateHist05");
   TH1D* rateHist10 = denominator->Clone("rateHist10");
   rateHist10->SetLineColor(2);
   rateHist10->SetMarkerColor(2);
   TH1D* rateHist20 = denominator->Clone("rateHist20");
   rateHist20->SetLineColor(6);
   rateHist20->SetMarkerColor(6);

   //----- also save histogram of isolation variable itself ------- 
   TH1D* isoDeposit =  new TH1D("isoDeposit", "", 50, 0, 50.);;
   isoDeposit->SetLineWidth(2);
   isoDeposit->Sumw2();
   isoDeposit->GetYaxis()->SetTitle("Events / GeV");
   isoDeposit->GetXaxis()->SetTitle("L1 isolation (GeV)");
   isoDeposit->GetXaxis()->SetLabelSize(0.05);
   isoDeposit->GetXaxis()->SetTitleSize(0.05);
   isoDeposit->GetYaxis()->SetLabelSize(0.05);
   isoDeposit->GetYaxis()->SetTitleSize(0.05);
   isoDeposit->GetYaxis()->SetTitleOffset(1.4);
   isoDeposit->SetMarkerStyle(20);

   //----- save histogram of relative isolation ------- 
   TH1D* isoDepRel =  new TH1D("isoDepRel", "", 25, 0, 5.);;
   isoDepRel->SetLineWidth(2);
   isoDepRel->Sumw2();
   isoDepRel->GetYaxis()->SetTitle("Events");
   isoDepRel->GetXaxis()->SetTitle("L1 iso / L1 mu pt");
   isoDepRel->GetXaxis()->SetLabelSize(0.05);
   isoDepRel->GetXaxis()->SetTitleSize(0.05);
   isoDepRel->GetYaxis()->SetLabelSize(0.05);
   isoDepRel->GetYaxis()->SetTitleSize(0.05);
   isoDepRel->GetYaxis()->SetTitleOffset(1.4);
   isoDepRel->SetMarkerStyle(20);

   //-----save histogram of pileup in isolation ------- 
   TH1D* puInIso =  new TH1D("puInIso", "", 25, 0, 25.);;
   puInIso->SetLineWidth(2);
   puInIso->Sumw2();
   puInIso->GetYaxis()->SetTitle("Events");
   puInIso->GetXaxis()->SetTitle("PU in isolation (GeV)");
   puInIso->GetXaxis()->SetLabelSize(0.05);
   puInIso->GetXaxis()->SetTitleSize(0.05);
   puInIso->GetYaxis()->SetLabelSize(0.05);
   puInIso->GetYaxis()->SetTitleSize(0.05);
   puInIso->GetYaxis()->SetTitleOffset(1.4);
   puInIso->SetMarkerStyle(20);


   TString varName = TString(var);

   ///// ----- Initialize tree branches ----------
   vector<float>   *pt;
   vector<float>   *eta;
   vector<float>   *phi;
   UInt_t          run;
   UInt_t          lumi;
   ULong64_t       evt;

   vector<float>   *fpt;
   vector<float>   *feta;
   vector<float>   *fphi;
   vector<float>   *fregionPt;
   vector<int>     *fpu;
   UInt_t          frun;
   UInt_t          flumi;
   ULong64_t       fevt;

   tree1->SetBranchAddress("run",&run);
   tree1->SetBranchAddress("lumi",&lumi);
   tree1->SetBranchAddress("evt",&evt);
   tree1->SetBranchAddress("pt",&pt);
   tree1->SetBranchAddress("eta",&eta);
   tree1->SetBranchAddress("phi",&phi);

   tree2->SetBranchAddress("run",&frun);
   tree2->SetBranchAddress("lumi",&flumi);
   tree2->SetBranchAddress("evt",&fevt);
   tree2->SetBranchAddress("pt",&fpt);
   tree2->SetBranchAddress("regionPt",&fregionPt);
   tree2->SetBranchAddress("eta",&feta);
   tree2->SetBranchAddress("phi",&fphi);
   tree2->SetBranchAddress("pu",&fpu);
   tree2->BuildIndex("run","evt");
   tree1->AddFriend(tree2, "tree2");

   ///// ----- Loop over the trees to compute isolation & fill histos --------
   for (Int_t i = 0; i < (Int_t) tree1->GetEntries(); i++) {
      tree1->GetEntry(i);
      if(frun==run && flumi==lumi && fevt==evt) {
         for(int j=0; j< pt->size(); ++j) {
            if(pt->at(j)<0.0) continue; // check that at least one muon 
            if(fabs(eta->at(j))>2.1) continue; // within acceptance 

            ///---find the nearest calo deposit in deltaR = 0.2 ---
            float mindR = 0.2;
            float isoVal = 0.0;
            float pu = 0.0;
   
            ///---- loop over all calo deposits ----------
            for(int k=0; k< fpt->size(); ++k) {
               if(fpt->at(k)<0.0) continue; 
               float deta = eta->at(j)-feta->at(k);
               float dphi = 
                  fabs(fabs(phi->at(j)-fphi->at(k))-TMath::Pi())-TMath::Pi();
               float dRiso = sqrt(deta**2 + dphi**2);
               if (dRiso < mindR) 
               { mindR = dRiso; isoVal = fregionPt->at(k); pu = fpu->at(k);}
            }

            ///----- which independent variable to plot: pt or eta ----
            float myvar = pt->at(j);
            if(varName=="eta") myvar = eta->at(j);

            ///----- compute rates only for non-isolated muons ----
            if(isoVal>0.0) {
               if(applyPUsub)  isoVal -= pu;
               float iso_relative = isoVal / pt->at(j);
               isoDeposit->Fill(isoVal);
               puInIso->Fill(pu);
               isoDepRel->Fill(iso_relative);
               denominator->Fill(myvar);
               if(iso_relative<0.5) rateHist05->Fill(myvar);
               if(iso_relative<1.0) rateHist10->Fill(myvar);
               if(iso_relative<2.0) rateHist20->Fill(myvar);
            } // if isoVal>0 condition
         }// index j loop 
      }// if frun==run condition
   }// close tree loop


   //// ---- Make cumulative histograms for rate vs pt----
   if(var=="pt") {
      makeCumulativeHistogram(*denominator);
      makeCumulativeHistogram(*rateHist05);
      makeCumulativeHistogram(*rateHist10);
      makeCumulativeHistogram(*rateHist20);
   }
   //// ---- Now divide by denominator histogram to obtain efficiency ----
   rateHist05->Divide(denominator);
   rateHist10->Divide(denominator);
   rateHist20->Divide(denominator);



   // --- Set up TDR style for plots ------ 
   gROOT->ProcessLine(".L ~/tdrstyle.C");
   setTDRStyle();
   tdrStyle->SetErrorX(0.5);
   tdrStyle->SetPadLeftMargin(0.18);
   tdrStyle->SetPadRightMargin(0.08);
   tdrStyle->SetLegendBorderSize(0);
   tdrStyle->SetTitleYOffset(1.3);


   // --- plot the rate histograms ------ 
   TCanvas* canRate = new TCanvas("canRate","",500,500);
   rateHist05->Draw("");
   rateHist10->Draw("esame");
   rateHist20->Draw("esame");
   TLegend* legend = new TLegend(0.6,0.28,0.9,0.48);
   legend->SetFillColor(0);
   legend->AddEntry(rateHist05, "relIso < 0.5", "PLE");  
   legend->AddEntry(rateHist10, "relIso < 1.0", "PLE");  
   legend->AddEntry(rateHist20, "relIso < 2.0", "PLE"); 
   if(var=="pt") legend->Draw();
   cmsPrelim();
   // canRate->SetLogy(1);
   TString plotname = TString("muonRate_")+TString(var);
   char tempst[100];
   canRate->SaveAs(plotname+TString(".png"));
   canRate->SaveAs(plotname+TString(".pdf"));


   // --- plot isolation variable ------ 
   TCanvas* canIso = new TCanvas("canIso","",500,500);
   isoDeposit->Draw();
   canIso->SaveAs("muonL1Isolation.png");
   canIso->SaveAs("muonL1Isolation.pdf");

   TCanvas* canIsoRel = new TCanvas("canIsoRel","",500,500);
   isoDepRel->Draw();
   canIsoRel->SaveAs("muonL1IsoRel.png");
   canIsoRel->SaveAs("muonL1IsoRel.pdf");

   TCanvas* canPU = new TCanvas("canPU","",500,500);
   puInIso->Draw();
   canIso->SaveAs("muonL1IsoPU.png");
   canIso->SaveAs("muonL1IsoPU.pdf");


   //--------- Finally clean up the memory -------
//    delete denominator;
//    delete isoDeposit;
//    delete isoDepRel;
//    delete puInIso;
//    delete rateHist05;
//    delete rateHist10;
//    delete rateHist20;
//    delete canRate;
//    delete canIso;
//    delete legend;
}






////--------Utility: CMS Preliminary label------------
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



/////----Utility: Make cumulative histogram ---------------
void makeCumulativeHistogram(TH1& hist) {
   int numBins = hist.GetNbinsX();
   for (int i=0; i < numBins; ++i) {
      hist.SetBinContent(i, hist.Integral(i, numBins));
   } 
}
