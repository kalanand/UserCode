#include "../src/JetUtilMC.h"

const double MINPTCUT = 0.0;

const int NGenPtBins=17;
const double GenPt[NGenPtBins+1] = {0,30,60,80,120,170,230,300,380,470,600,800,1000,1400,1800,2200,3000,5000};



const bool usingCorrectedCaloJetPt = false;
const double M_PI = TMath::Pi();
const int NUM_JET_MAX = 4;
const bool storeResponsHistogramsInRootFile = true;
const bool makeplot_ZptBalance = false;
const bool makeplot_Response = false;
const bool storeResponseInTextFile = true;
const bool useLocalFile = false;
TString basename = "/uscms_data/d1/kalanand/trash/CSA07-ZeeJets_Pt_";

// TString basename="/uscms_data/d1/cmsjtmet/kalanand/CSA07-ZeeJets-Ntuple/CSA07-ZeeJets_Pt_";




void plot_ZJetBalance() {

  for(int i=0; i<5; i++) {plot_ZJetBalance(i);}
}





void plot_ZJetBalance(int JetAlgo) {

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
 

  TString algoSt;
  if(JetAlgo==0) algoSt = "_ic5";
  if(JetAlgo==1) algoSt = "_ic7";
  if(JetAlgo==2) algoSt = "_mp5";
  if(JetAlgo==3) algoSt = "_mp7";
  if(JetAlgo==4) algoSt = "_kt6";

  TString textFilename = "ZjetPtBalance" + algoSt + TString(".txt");
  TString JetResponseFilename 
    = "Histograms_ZjetResponse" + algoSt + TString(".root");

  FILE *file = fopen(textFilename,"w+");
  fprintf( file ,"%s    %s   %s   %s   %s \n", "pT bin", 
	   "genMean", "genSigma", "recoMean", "recoSigma");

  TH1F* responseHistGen;
  TH1F* responseHistReco;
  TH1F* genJetpt;
  TH1F* caloJetpt;
  TH1F* Zpt;

  if(storeResponsHistogramsInRootFile == true) {
    TFile respHistFile(JetResponseFilename,"RECREATE");
  }


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


    plot_ZJetBalance(  JetAlgo, i, *responseHistGen, *responseHistReco, 
		      *genJetpt, *caloJetpt, *recoZpt);

    genMean[i]   = (Float_t) responseHistGen->GetMean(1);
    genSigma[i]  = (Float_t) responseHistGen->GetMean(11);
    recoMean[i]  = (Float_t) responseHistReco->GetMean(1); 
    recoSigma[i] = (Float_t) responseHistReco->GetMean(11);


    if(storeResponseInTextFile == true) {
      fprintf( file ,"%5.1f   %5.4f   %5.4f   %5.4f   %5.4f \n", pt[i],
	       genMean[i], genSigma[i], recoMean[i], recoSigma[i] );
    }

    if(storeResponsHistogramsInRootFile == true) {
      respHistFile.cd();
      responseHistGen->Write();
      responseHistReco->Write();
      genJetpt->Write();
      caloJetpt->Write();
      recoZpt->Write();
    }
    
    delete responseHistGen;
    delete responseHistReco;
    delete genJetpt;
    delete caloJetpt;
    delete recoZpt;
  }

  fclose(file);
  respHistFile.Close();





  if(makeplot_Response == true) {
    // plot full spectrum
    TGraphErrors *ptbalanceGen  = new TGraphErrors(NGenPtBins, pt, genMean, 
						   errpt, genSigma);
    TGraphErrors *ptbalanceReco = new TGraphErrors(NGenPtBins, pt, recoMean, 
						   errpt, recoSigma);
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

    TCanvas c1("c1","",800,600);
    c1.SetGrid();
    ptbalanceGen->Draw("APL");
    ptbalanceReco->Draw("PL");
    ptbalancemm->Draw("PL");
    leg_hist = new TLegend(0.6,0.35,0.85,0.55);
    leg_hist->AddEntry( ptbalanceGen, "Generator level", "l");
    leg_hist->AddEntry( ptbalanceReco,"Calorimeter level","l");
    leg_hist->AddEntry( ptbalancemm,"Z#rightarrow#mu#mu","l");
    leg_hist->SetFillColor(0);
    leg_hist->Draw();
    c1.SaveAs("PtBalanceVsPt.eps");
    c1.SaveAs("PtBalanceVsPt.gif");
    c1.SaveAs("PtBalanceVsPt.root");
    c1.Close();
    delete leg_hist;
    delete ptbalanceGen;
    delete ptbalanceReco;
    delete ptbalancemm;
  }

}






void plot_ZJetBalance( int index, int bin, TH1& responseHistGen, 
		       TH1& responseHistReco, TH1& genJetpt, 
		       TH1& caloJetpt, TH1& recoZpt ) {

  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////

  TChain* t = new TChain("ZJet");
  if( useLocalFile == true ) t->Add("analysis_zjet.root");
  else t->Add(basename + TString("*.root"));

  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////

  Float_t JetRecoPt[20][4];
  Float_t JetCorPt[20][4];
  Float_t JetGenPt[20][4];
  Float_t JetRecoEta[20][4];
  Float_t JetGenEta[20][4];
  Float_t JetRecoPhi[20][4];
  Float_t JetGenPhi[20][4];
  Float_t JetRecoType[20][4];
  Float_t Z_Pt;
  Float_t Z_Phi;
  Float_t Z_Eta;
  Float_t Z_PtGen;
  Float_t Z_PhiGen;
  Float_t ePlusPt;
  Float_t eMinusPt;
  Float_t ePlusPtGen;
  Float_t eMinusPtGen;
  Float_t ePlusEta;
  Float_t eMinusEta;
  Float_t ePlusPhi;
  Float_t eMinusPhi;
  Float_t mZeeGen;
  Float_t mZee;

  Float_t ePlus_ecaliso_20;
  Float_t ePlus_sc_e;
  Float_t eMinus_ecaliso_20;
  Float_t eMinus_sc_e;
  Float_t ePlus_ptisoatecal_15;	   
  Float_t eMinus_ptisoatecal_15;
  

  t->SetBranchAddress("JetGenPt",   JetGenPt);
  t->SetBranchAddress("JetGenEta",  JetGenEta);
  t->SetBranchAddress("JetGenPhi",  JetGenPhi);
  t->SetBranchAddress("JetCorPt",   JetCorPt);
  t->SetBranchAddress("JetRecoPt",  JetRecoPt);
  t->SetBranchAddress("JetRecoEta", JetRecoEta);
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
  t->SetBranchAddress("ePlusEta",   &ePlusEta);
  t->SetBranchAddress("eMinusEta",  &eMinusEta);
  t->SetBranchAddress("ePlusPhi",   &ePlusPhi);
  t->SetBranchAddress("eMinusPhi",  &eMinusPhi);
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
  t->SetBranchStatus("JetGenEta",   1);
  t->SetBranchStatus("JetGenPhi",   1);
  t->SetBranchStatus("JetRecoPt",   1);
  t->SetBranchStatus("JetRecoEta",  1);
  t->SetBranchStatus("JetRecoPhi",  1);
  t->SetBranchStatus("JetCorPt",    1);
  t->SetBranchStatus("JetCorEta",   1);
  t->SetBranchStatus("JetCorPhi",   1);
  t->SetBranchStatus("Z_Pt",        1);
  t->SetBranchStatus("Z_Phi",       1);
  t->SetBranchStatus("Z_Eta",       1);
  t->SetBranchStatus("Z_PtGen",     1);
  t->SetBranchStatus("Z_PhiGen",    1);
  t->SetBranchStatus("ePlusPt",     1);
  t->SetBranchStatus("eMinusPt",    1);
  t->SetBranchStatus("ePlusPtGen",  1);
  t->SetBranchStatus("eMinusPtGen", 1);
  t->SetBranchStatus("ePlusEta",    1);
  t->SetBranchStatus("eMinusEta",   1);
  t->SetBranchStatus("ePlusPhi",    1);
  t->SetBranchStatus("eMinusPhi",   1);
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
    if(entry%20000==0) std::cout<<"************ Event # "<< entry <<std::endl;


    bool isPtCut  = (ePlusPt>20.0) && (eMinusPt>20.0) && 
      (Z_Pt > GenPt[bin]) && (Z_Pt < GenPt[bin+1]);


    bool isEtaCutP = (fabs(ePlusEta)<1.4442) || 
      (fabs(ePlusEta)>1.560 && fabs(ePlusEta)<2.5);
    bool isEtaCutM = (fabs(eMinusEta)<1.4442) || 
      (fabs(eMinusEta)>1.560 && fabs(eMinusEta)<2.5);
    bool isECALiso = (ePlus_ecaliso_20 > 0.0) && 
      (ePlus_ecaliso_20 < 0.2) && (eMinus_ecaliso_20 > 0.0) && 
      (eMinus_ecaliso_20 < 0.2);    
    bool isTrackiso = (ePlus_ptisoatecal_15 < 0.2) && 
      (eMinus_ptisoatecal_15 < 0.2);


    if( !(isPtCut && isEtaCutP && isEtaCutM && isECALiso && 
	  isTrackiso && fabs(mZee-91.2)<2.5) )  continue;

    float leadGenJetPt=JetGenPt[index][0]; 
    float secondGenJetPt=JetGenPt[index][0]; 


    int leadRecoIndex=-1, secondRecoIndex=-1;
    float leadRecoJetPt   = 0.0;
    float secondRecoJetPt = 0.0;

    leadRecoIndex = 0; 
    secondRecoIndex = 1;
    leadRecoJetPt   =  JetRecoPt[index][0];
    secondRecoJetPt =  JetRecoPt[index][1];


//   FindLeadIndex(JetRecoCorrectedPt[index], leadRecoIndex[index], secondRecoIndex[index]);
//   leadRecoJetPt   = JetRecoCorrectedPt[index][leadRecoIndex];
//   secondRecoJetPt = JetRecoCorrectedPt[index][secondRecoIndex];

//   if(leadRecoIndex==-1) continue;


    // ************ test: apply jet pT cut *****************
    if( JetRecoPt[index][0] < MINPTCUT || 
	JetRecoPt[index][1] < MINPTCUT ) continue;


    float leadRecoJetEta = JetRecoEta[index][leadRecoIndex];
    double dPhiReco = dPhi(JetRecoPhi[index][leadRecoIndex], Z_Phi);


    if( fabs(dPhiReco)<2.94 || fabs(leadRecoJetEta)>1.3 || 
	secondRecoJetPt/leadRecoJetPt>0.2 ) continue;


    double ptRatioGen  = leadGenJetPt/Z_PtGen;
    double ptRatioReco = 0.0;

    if(usingCorrectedCaloJetPt) ptRatioReco = JetCorPt[index][0]/Z_Pt;
    else ptRatioReco = JetRecoPt[index][0]/Z_Pt;


    responseHistGen.Fill( ptRatioGen );
    responseHistReco.Fill( ptRatioReco );

    genJetpt.Fill(leadGenJetPt); 
    caloJetpt.Fill(leadRecoJetPt);
    recoZpt.Fill( Z_Pt );

    
  } // end TTree loop


  if(makeplot_ZptBalance==true) {

    TString plotname = 
      Form("ptBalance-allCuts_%d_%d", (int) GenPt[bin], 
	   (int) GenPt[bin+1] );
    
    PlotOnCanvas( responseHistGen, responseHistReco, 
		  plotname);
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









// find the leading and second jet indices

void FindLeadIndex(float pT[], int &lead, int &sec) {

  float max = 0.0;
  lead = -1;
  for (int i=0; i<NUM_JET_MAX; i++) {
    if(pT[i]>max) { max = pT[i]; lead = i; }
  }

  max = 0.0;
  sec = -1;
  for (int i=0; i<NUM_JET_MAX; i++) {
    if(i==lead) continue;
    if(pT[i]>max) { max = pT[i]; sec = i; }
  }
}
