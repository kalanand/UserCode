#include "../src/JetUtilMC.h"
#include "UserDefinedData.h"
//#include "myUtilities.h"
const double M_PI = TMath::Pi();
const int NUM_JET_MAX = 4;
const bool makeplot = false;
const bool useLocalFile = false;
TString basename = "/uscms_data/d1/kalanand/trash/CSA07-ZeeJets_Pt_";
// basename="/uscms_data/d1/cmsjtmet/kalanand/CSA07-ZeeJets-Ntuple/CSA07-ZeeJets_Pt_";




void plot_ZJetBalance() {

  Float_t pt[NGenPtBins];
  TString ptBin[NGenPtBins];

  
  for(int i=0; i<NGenPtBins; i++) {
    pt[i] = 0.5*( GenPt[i] + GenPt[i+1] );
    ptBin[i] = Form("%d_%d", (int) GenPt[i], (int) GenPt[i+1] );
  }


  Float_t errpt[NGenPtBins];
  Float_t genMean[NGenPtBins];
  Float_t genSigma[NGenPtBins]; 
  Float_t recoMean[NGenPtBins]; 
  Float_t recoSigma[NGenPtBins];
 
  Float_t ptPlot[NGenPtBins]; 
  Float_t errptPlot[NGenPtBins];
  Float_t genMeanPlot[NGenPtBins];
  Float_t genSigmaPlot[NGenPtBins]; 
  Float_t recoMeanPlot[NGenPtBins]; 
  Float_t recoSigmaPlot[NGenPtBins];


  FILE *file = fopen("ZjetPtBalance.txt","w+");
  fprintf( file ,"%s    %s   %s   %s   %s \n", "pT bin", 
	   "genMean", "genSigma", "recoMean", "recoSigma");

  TH1F* responseHistGen;
  TH1F* responseHistReco;
  TH1F* genJetpt;
  TH1F* caloJetpt;
  TH1F* Zpt;

  TFile respHistFile(JetResponseFilename,"RECREATE");

  for(int i=0; i<NGenPtBins; i++) {

    responseHistGen = new TH1F("responseHistGen_"+ptBin[i],"", 40, 0.0, 2.0);
    responseHistGen->Sumw2();
    TAxis* responseHistGenx = responseHistGen->GetXaxis();
    TAxis* responseHistGeny = responseHistGen->GetYaxis();
    responseHistGenx->SetTitle("p_{T,Jet} / p_{T,Z}   ");
    responseHistGeny->SetTitle("Events / 0.05         ");
    responseHistGeny->SetTitleOffset(1.2);
    
    responseHistReco = new TH1F("responseHistReco_"+ptBin[i],"", 40, 0.0, 2.0);
    responseHistReco->Sumw2();
    responseHistReco->SetLineColor(2);
    responseHistReco->SetMarkerColor(2);
    

    genJetpt = new TH1F("genJetpt_"+ptBin[i],"", NGenPtBins-1, GenPt);
    genJetpt->Sumw2();

    caloJetpt = new TH1F("caloJetpt_"+ptBin[i],"", NGenPtBins-1, GenPt);
    caloJetpt->Sumw2();

    recoZpt = new TH1F("recoZpt_"+ptBin[i],"", NGenPtBins-1, GenPt);
    recoZpt->Sumw2();

    errpt[i]     = 0.0;
    genMean[i]   = 0.0;
    genSigma[i]  = 0.0; 
    recoMean[i]  = 0.0; 
    recoSigma[i] = 0.0;
 
    plot_ZJetBalance( i, *responseHistGen, *responseHistReco, 
		      *genJetpt, *caloJetpt, *recoZpt);


    genMean[i]   = (Float_t) responseHistGen->GetMean(1);
    genSigma[i]  = (Float_t) responseHistGen->GetMean(11);
    recoMean[i]  = (Float_t) responseHistReco->GetMean(1); 
    recoSigma[i] = (Float_t) responseHistReco->GetMean(11);

    fprintf( file ,"%5.1f   %5.4f   %5.4f   %5.4f   %5.4f \n", pt[i],
	     genMean[i], genSigma[i], recoMean[i], recoSigma[i] );

    respHistFile.cd();
    responseHistGen->Write();
    responseHistReco->Write();
    genJetpt->Write();
    caloJetpt->Write();
    recoZpt->Write();

    delete responseHistGen;
    delete responseHistReco;
    delete genJetpt;
    delete caloJetpt;
    delete recoZpt;
  }

  fclose(file);
  respHistFile.Close();





  for(int i=0; i<NGenPtBins; i++) {
    ptPlot[i]         = pt[i+1]; 
    errptPlot[i]     = errpt[i+1];
    genMeanPlot[i]   = genMean[i+1];
    genSigmaPlot[i]  = genSigma[i+1]; 
    recoMeanPlot[i]  = recoMean[i+1]; 
    recoSigmaPlot[i] = recoSigma[i+1];
  }



  // plot full spectrum
  TGraphErrors *ptbalanceGen  = new TGraphErrors(NGenPtBins, ptPlot, genMeanPlot, 
						 errptPlot, genSigmaPlot);
  TGraphErrors *ptbalanceReco = new TGraphErrors(NGenPtBins, ptPlot, recoMeanPlot, 
						 errptPlot, recoSigmaPlot);



  // plot Zmumu values

  Float_t ptmm[9] = { 40.0, 60.0, 100.0, 140.0, 200.0, 250.0, 
		      330.0, 400.0, 520.0 };
  Float_t balancemm[9] = { 0.496, 0.568, 0.66, 0.71, 0.75, 0.765, 
			   0.775, 0.79, 0.81 }; 
  TGraph *ptbalancemm = new TGraph( 9, ptmm, balancemm);


  ptbalanceGen->GetXaxis()->SetTitle("p_{T}^{Z} [GeV/c]     ");
  ptbalanceGen->GetYaxis()->SetTitle("p_{T}^{Jet} / p_{T}^{Z}   ");
  ptbalanceGen->SetMarkerStyle(22);
  ptbalanceGen->SetMarkerSize(1.2);
  ptbalanceGen->SetTitle("");
  ptbalanceGen->SetMinimum(0.2);
  ptbalanceReco->SetMarkerColor(2);
  ptbalanceReco->SetLineColor(2);
  ptbalanceReco->SetMarkerStyle(22);
  ptbalanceReco->SetMarkerSize(1.2);
  ptbalanceReco->SetMinimum(0.2);
  ptbalancemm->SetMarkerStyle(24);
  ptbalancemm->SetMarkerSize(1.2);
  ptbalancemm->SetMinimum(0.2);
  ptbalancemm->SetMarkerColor(4);
  ptbalancemm->SetLineColor(4);

  //   TCanvas c1("c1","",800,600);
  //   c1.SetGrid();
  //   ptbalanceGen->Draw("APL");
  //   ptbalanceReco->Draw("PL");
  //   ptbalancemm->Draw("PL");
  //   leg_hist = new TLegend(0.6,0.35,0.85,0.55);
  //   leg_hist->AddEntry( ptbalanceGen, "Generator level", "l");
  //   leg_hist->AddEntry( ptbalanceReco,"Calorimeter level","l");
  //   leg_hist->AddEntry( ptbalancemm,"Z#rightarrow#mu#mu","l");
  //   leg_hist->SetFillColor(0);
  //   leg_hist->Draw();
  //   c1.SaveAs("PtBalanceVsPt.eps");
  //   c1.SaveAs("PtBalanceVsPt.gif");
  //   c1.SaveAs("PtBalanceVsPt.root");
  //   c1.Close();
  //   delete leg_hist;
  delete ptbalanceGen;
  delete ptbalanceReco;


  // plot full spectrum
  TGraphErrors *ptbalanceGen2  = new TGraphErrors(14, ptPlot, genMeanPlot, 
						  errptPlot, genSigmaPlot);
  TGraphErrors *ptbalanceReco2 = new TGraphErrors(14, ptPlot, recoMeanPlot, 
						  errptPlot, recoSigmaPlot);
  
  ptbalanceGen2->GetXaxis()->SetTitle("p_{T}^{Z} [GeV/c]     ");
  ptbalanceGen2->GetYaxis()->SetTitle("p_{T}^{Jet} / p_{T}^{Z}   ");
  ptbalanceGen2->SetMarkerStyle(22);
  ptbalanceGen2->SetMarkerSize(1.2);
  ptbalanceGen2->SetTitle("");
  ptbalanceGen2->SetMinimum(0.2);
  ptbalanceReco2->SetMarkerColor(2);
  ptbalanceReco2->SetLineColor(2);
  ptbalanceReco2->SetMarkerStyle(22);
  ptbalanceReco2->SetMarkerSize(1.2);
  ptbalanceReco2->SetMinimum(0.2);

  TCanvas c2("c2","",800,600);
  c2.SetGrid();
  ptbalanceGen2->Draw("APL");
  ptbalanceReco2->Draw("PL");
  ptbalancemm->Draw("PL");
  leg_hist = new TLegend(0.6,0.35,0.85,0.55);
  leg_hist->AddEntry( ptbalanceGen2, "Generator level", "l");
  leg_hist->AddEntry( ptbalanceReco2,"Calorimeter level","l");
  leg_hist->AddEntry( ptbalancemm,"Z#rightarrow#mu#mu","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  c2.SaveAs("PtBalanceVsPt_1000.eps");
  c2.SaveAs("PtBalanceVsPt_1000.gif");
  c2.SaveAs("PtBalanceVsPt_1000.root");
  c2.Close();

  delete leg_hist;
  delete ptbalanceGen2;
  delete ptbalanceReco2;
}






void plot_ZJetBalance( int bin, TH1& responseHistGen, 
		       TH1& responseHistReco, TH1& genJetpt, 
		       TH1& caloJetpt, TH1& recoZpt ) {

  TString ptBin[NGenPtBins];
  for(int i=0; i<NGenPtBins-1; i++) { 
    ptBin[i] = Form("%d_%d", (int) GenPt[i], (int) GenPt[i+1] );
  }
  ptBin[NGenPtBins-1] = "3500_-1";

  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////

  TChain* t = new TChain("ZJet");
  if( useLocalFile == true ) t->Add("analysis_zjet.root");
  else t->Add(basename + ptBin[bin] + TString(".root"));

  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////

  Float_t JetRecoPt[NUM_JET_MAX];
  Float_t JetGenPt[NUM_JET_MAX];
  Float_t JetRecoEta[NUM_JET_MAX];
  Float_t JetGenEta[NUM_JET_MAX];
  Float_t JetRecoPhi[NUM_JET_MAX];
  Float_t JetGenPhi[NUM_JET_MAX];
  Float_t Z_Pt;
  Float_t Z_Phi;
  Float_t Z_Eta;
  Float_t ePlusPt;
  Float_t eMinusPt;
  Float_t Z_PtGen;
  Float_t Z_PhiGen;
  Float_t ePlusPtGen;
  Float_t eMinusPtGen;
  Float_t ePlusEtaGen;
  Float_t eMinusEtaGen;
  Float_t ePlusPhiGen;
  Float_t eMinusPhiGen;
  Float_t mZeeGen;
  Float_t mZee;

  Float_t ePlus_ecaliso_20;
  Float_t ePlus_sc_e;
  Float_t eMinus_ecaliso_20;
  Float_t eMinus_sc_e;
  Float_t ePlus_ptisoatecal_15;	   
  Float_t eMinus_ptisoatecal_15;
  

  t->SetBranchAddress("JetGenPt",   JetGenPt);
  t->SetBranchAddress("JetRecoPt",  JetRecoPt);
  t->SetBranchAddress("JetGenEta",  JetGenEta);
  t->SetBranchAddress("JetRecoEta", JetRecoEta);
  t->SetBranchAddress("JetGenPhi",  JetGenPhi);
  t->SetBranchAddress("JetRecoPhi", JetRecoPhi);
  t->SetBranchAddress("Z_Pt",       &Z_Pt);
  t->SetBranchAddress("Z_Phi",      &Z_Phi);
  t->SetBranchAddress("Z_Eta",      &Z_Eta);
  t->SetBranchAddress("Z_PtGen",    &Z_PtGen);
  t->SetBranchAddress("Z_PhiGen",   &Z_PhiGen);
  t->SetBranchAddress("ePlusPt",    &ePlusPt);
  t->SetBranchAddress("eMinusPt",   &eMinusPt);
  t->SetBranchAddress("ePlusPtGen", &ePlusPtGen);
  t->SetBranchAddress("eMinusPtGen",&eMinusPtGen);
  t->SetBranchAddress("ePlusEtaGen", &ePlusEtaGen);
  t->SetBranchAddress("eMinusEtaGen",&eMinusEtaGen);
  t->SetBranchAddress("ePlusPhiGen", &ePlusPhiGen);
  t->SetBranchAddress("eMinusPhiGen",&eMinusPhiGen);
  t->SetBranchAddress("mZeeGen",     &mZeeGen);
  t->SetBranchAddress("mZee",        &mZee);
  t->SetBranchAddress("ePlus_ecaliso_20",      &ePlus_ecaliso_20);
  t->SetBranchAddress("ePlus_sc_e",            &ePlus_sc_e);
  t->SetBranchAddress("eMinus_ecaliso_20",     &eMinus_ecaliso_20);
  t->SetBranchAddress("eMinus_sc_e",           &eMinus_sc_e);
  t->SetBranchAddress("ePlus_ptisoatecal_15",  &ePlus_ptisoatecal_15);	   
  t->SetBranchAddress("eMinus_ptisoatecal_15", &eMinus_ptisoatecal_15);

  t->SetBranchStatus("*",    0);
  t->SetBranchStatus("JetGenPt",    1);
  t->SetBranchStatus("JetRecoPt",   1);
  t->SetBranchStatus("JetGenEta",   1);
  t->SetBranchStatus("JetRecoEta",  1);
  t->SetBranchStatus("JetGenPhi",   1);
  t->SetBranchStatus("JetRecoPhi",  1);
  t->SetBranchStatus("Z_Pt",        1);
  t->SetBranchStatus("Z_Phi",       1);
  t->SetBranchStatus("Z_Eta",       1);
  t->SetBranchStatus("Z_PtGen",     1);
  t->SetBranchStatus("Z_PhiGen",    1);
  t->SetBranchStatus("ePlusPt",     1);
  t->SetBranchStatus("eMinusPt",    1);
  t->SetBranchStatus("ePlusPtGen",  1);
  t->SetBranchStatus("eMinusPtGen", 1);
  t->SetBranchStatus("ePlusEtaGen", 1);
  t->SetBranchStatus("eMinusEtaGen",1);
  t->SetBranchStatus("ePlusPhiGen", 1);
  t->SetBranchStatus("eMinusPhiGen",1);
  t->SetBranchStatus("mZeeGen",     1);
  t->SetBranchStatus("mZee",        1);
  t->SetBranchStatus("ePlus_ecaliso_20",      1);
  t->SetBranchStatus("ePlus_sc_e",            1);
  t->SetBranchStatus("eMinus_ecaliso_20",     1);
  t->SetBranchStatus("eMinus_sc_e",           1);
  t->SetBranchStatus("ePlus_ptisoatecal_15",  1);	   
  t->SetBranchStatus("eMinus_ptisoatecal_15", 1);



  for (Long64_t entry =0; entry < t->GetEntries(); entry++) {
    t->GetEntry(entry);
    if(entry%10000==0) std::cout<<"************ Event # "<< entry <<std::endl;

    if(!(mZee>60.0 && mZee<120.0)) continue;


    int leadGenIndex=-1, secondGenIndex=-1, 
      leadRecoIndex=-1, secondRecoIndex=-1;


    FindLeadIndex(JetGenPt, JetGenEta, JetGenPhi, ePlusEtaGen, 
		  ePlusPhiGen, eMinusEtaGen, eMinusPhiGen, 
		  leadGenIndex, secondGenIndex);

    FindLeadIndex(JetRecoPt, JetRecoEta, JetRecoPhi, ePlusEtaGen, 
		  ePlusPhiGen, eMinusEtaGen, eMinusPhiGen, 
		  leadRecoIndex, secondRecoIndex);

    if(leadGenIndex==-1 || leadRecoIndex==-1) continue;

    float leadGenJetPt=JetGenPt[leadGenIndex]; 
    float secondGenJetPt=JetGenPt[secondGenIndex]; 
    float leadRecoJetPt=JetRecoPt[leadRecoIndex];
    float secondRecoJetPt=JetRecoPt[secondRecoIndex];

    float leadGenJetPhi = JetGenPhi[leadGenIndex]; 
    float leadRecoJetPhi = JetRecoPhi[leadRecoIndex]; 

    float leadGenJetEta = JetGenEta[leadGenIndex]; 
    float leadRecoJetEta = JetRecoEta[leadRecoIndex]; 


    double dPhiGen = dPhi(leadGenJetPhi, Z_PhiGen);
    double dPhiReco = dPhi(leadRecoJetPhi, Z_Phi);
    double ptRatioGen  = leadGenJetPt/Z_PtGen;
    double ptRatioReco = leadRecoJetPt/Z_PtGen;

    bool isPtCut  = (ePlusPtGen>20.0) && (eMinusPtGen>20.0);
    bool isEtaCutP = (fabs(ePlusEtaGen)<1.4442) || 
      (fabs(ePlusEtaGen)>1.560 && fabs(ePlusEtaGen)<2.5);
    bool isEtaCutM = (fabs(eMinusEtaGen)<1.4442) || 
      (fabs(eMinusEtaGen)>1.560 && fabs(eMinusEtaGen)<2.5);
    
    bool isECALisoP = (ePlus_ecaliso_20 > -1.0) && 
      ((ePlus_ecaliso_20-ePlus_sc_e)/ePlus_sc_e < 0.2);
    bool isECALisoM = (eMinus_ecaliso_20 > -1.0) && 
      ((eMinus_ecaliso_20-eMinus_sc_e)/eMinus_sc_e < 0.2);
    
    bool isTrackisoP = (ePlus_ptisoatecal_15/ePlusPtGen < 0.2);
    bool isTrackisoM = (eMinus_ptisoatecal_15/eMinusPtGen < 0.2);

    float zptreco = sqrt((ePlus_sc_e+eMinus_sc_e)**2-mZee**2)/cosh(Z_Eta);
    
    if( fabs(leadGenJetEta)<1.3 && fabs(dPhiGen)>2.94 && 
	secondGenJetPt/leadGenJetPt<0.2 && isPtCut && 
	isEtaCutP && isEtaCutM && isECALisoP && isECALisoM && 
	isTrackisoP && isTrackisoM && fabs(mZee-91.2)<2.5 && 
	zptreco > GenPt[bin] && zptreco < GenPt[bin+1]) { 

      responseHistGen.Fill( ptRatioGen );
      responseHistReco.Fill( ptRatioReco );

      genJetpt.Fill(leadGenJetPt); 
      caloJetpt.Fill(leadRecoJetPt);
      recoZpt.Fill( zptreco );
    } // end lead eta cut
    
  } // end TTree loop


  if(makeplot==true) {
    PlotOnCanvas( responseHistGen, responseHistReco, 
		  "ptBalance-allCuts_"+ptBin[bin]);
  }

}









// Draw pT balance plots
void PlotOnCanvas(TH1& genHist, TH1& recoHist, TString plotname) {


  TLine* line1 = new TLine(1.0, genHist.GetMinimum(), 
			   1.0, genHist.GetMaximum());
  line1->SetLineColor(4);
  line1->SetLineWidth(2);

  TCanvas canvas("canvas", "", 880, 600);
  gStyle->SetOptStat(0);
  genHist.Draw("hist");
  line1->Draw();
  recoHist.Draw("same");
  recoHist.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.65,0.85,0.85);
  leg_hist->AddEntry(&genHist,"Generator level","l");
  leg_hist->AddEntry(&recoHist,"Calorimeter level","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  canvas.SaveAs(plotname+TString(".eps"));
  canvas.SaveAs(plotname+TString(".gif"));
  canvas.SaveAs(plotname+TString(".root"));

  delete line1;
  delete leg_hist;
}






// find the leading and second jet indices after taking out the electrons

void FindLeadIndex(float pT[], float eta[], float phi[], 
		   float e1eta, float e1phi, float e2eta, float e2phi, 
		   int &lead, int &sec) {

  float max = 0.0;
  lead = -1;
  for (int i=0; i<NUM_JET_MAX; i++) {
    if(radius(eta[i], phi[i], e1eta, e1phi) < 0.3) continue;
    if(radius(eta[i], phi[i], e2eta, e2phi) < 0.3) continue;
    if(pT[i]>max) { max = pT[i]; lead = i; }
  }

  max = 0.0;
  sec = -1;
  for (int i=0; i<NUM_JET_MAX; i++) {
    if(i==lead) continue;
    if(radius(eta[i], phi[i], e1eta, e1phi) < 0.3) continue;
    if(radius(eta[i], phi[i], e2eta, e2phi) < 0.3) continue;
    if(pT[i]>max) { max = pT[i]; sec = i; }
  }
}
