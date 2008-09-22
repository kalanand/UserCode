#include "../src/JetUtilMC.h"
const double M_PI = TMath::Pi();
const int NUM_JET_MAX = 4;
const bool makeplot = false;
const bool useLocalFile = false;
TString basename = "/uscms_data/d1/kalanand/trash/CSA07-ZeeJets_Pt_";
// basename="/uscms_data/d1/cmsjtmet/kalanand/CSA07-ZeeJets-Ntuple/CSA07-ZeeJets_Pt_";


void plot_ZJetBalance() {

  TString ptBin[21]; 
  ptBin[0]  = "0_15";
  ptBin[1]  = "15_20";
  ptBin[2]  = "20_30";
  ptBin[3]  = "30_50";
  ptBin[4]  = "50_80";
  ptBin[5]  = "80_120";
  ptBin[6]  = "120_170";
  ptBin[7]  = "170_230";
  ptBin[8]  = "230_300";
  ptBin[9]  = "300_380";
  ptBin[10] = "380_470";
  ptBin[11] = "470_600";
  ptBin[12] = "600_800";
  ptBin[13] = "800_1000";
  ptBin[14] = "1000_1400";
  ptBin[15] = "1400_1800";
  ptBin[16] = "1800_2200";
  ptBin[17] = "2200_2600";
  ptBin[18] = "2600_3000";
  ptBin[19] = "3000_3500";
  ptBin[20] = "3500_-1";


  Float_t pt[21]; 
  pt[0]  = 7.5;
  pt[1]  = 17.5;
  pt[2]  = 25.0;
  pt[3]  = 40.0;
  pt[4]  = 65.0;
  pt[5]  = 100.0;
  pt[6]  = 145.0;
  pt[7]  = 200.0;
  pt[8]  = 265.0;
  pt[9]  = 340.0;
  pt[10] = 425.0;
  pt[11] = 535.0;
  pt[12] = 700.0;
  pt[13] = 900.0;
  pt[14] = 1200.0;
  pt[15] = 1600.0;
  pt[16] = 2000.0;
  pt[17] = 2400.0;
  pt[18] = 2800.0;
  pt[19] = 3250.0;
  pt[20] = 3750.0;

  Float_t errpt[21];
  Float_t genMean[21];
  Float_t genSigma[21]; 
  Float_t recoMean[21]; 
  Float_t recoSigma[21];
 

  Float_t ptPlot[20]; 
  Float_t errptPlot[20];
  Float_t genMeanPlot[20];
  Float_t genSigmaPlot[20]; 
  Float_t recoMeanPlot[20]; 
  Float_t recoSigmaPlot[20];



  FILE *file = fopen("ZjetPtBalance.txt","w+");
  fprintf( file ,"%s    %s   %s   %s   %s \n", "pT bin", 
	   "genMean", "genSigma", "recoMean", "recoSigma");


  for(int i=0; i<21; i++) {

    errpt[i]     = 0.0;
    genMean[i]   = 0.0;
    genSigma[i]  = 0.0; 
    recoMean[i]  = 0.0; 
    recoSigma[i] = 0.0;
 
    plot_ZJetBalance( ptBin[i], genMean[i], genSigma[i], 
		      recoMean[i], recoSigma[i] );

    fprintf( file ,"%5.1f   %5.4f   %5.4f   %5.4f   %5.4f \n", pt[i],
	     genMean[i], genSigma[i], recoMean[i], recoSigma[i] );
  }

  fclose(file);

  for(int i=0; i<20; i++) {
    ptPlot[i]         = pt[i+1]; 
    errptPlot[i]     = errpt[i+1];
    genMeanPlot[i]   = genMean[i+1];
    genSigmaPlot[i]  = genSigma[i+1]; 
    recoMeanPlot[i]  = recoMean[i+1]; 
    recoSigmaPlot[i] = recoSigma[i+1];
  }






  // plot full spectrum
  TGraphErrors *ptbalanceGen  = new TGraphErrors(20, ptPlot, genMeanPlot, 
						 errptPlot, genSigmaPlot);
  TGraphErrors *ptbalanceReco = new TGraphErrors(20, ptPlot, recoMeanPlot, 
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




void plot_ZJetBalance( TString pTbin, Float_t &genMean, Float_t &genSigma, 
		       Float_t &recoMean, Float_t &recoSigma ) {


/******** Read input ROOT files ************/
/******************************************/

TChain* t = new TChain("ZJet");
if( useLocalFile == true ) t->Add("analysis_zjet.root");
 else t->Add(basename + pTbin + TString(".root"));

/*****************************************/




  Float_t JetRecoPt[NUM_JET_MAX];
  Float_t JetGenPt[NUM_JET_MAX];
  Float_t JetRecoEta[NUM_JET_MAX];
  Float_t JetGenEta[NUM_JET_MAX];
  Float_t JetRecoPhi[NUM_JET_MAX];
  Float_t JetGenPhi[NUM_JET_MAX];
  Float_t Z_Pt;
  Float_t Z_Phi;
  bool isGoodZ;
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
  int ePlusClassificationGen;
  int eMinusClassificationGen;
  int ePlusClassification;
  int eMinusClassification;

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
  t->SetBranchAddress("Z_PtGen",    &Z_PtGen);
  t->SetBranchAddress("Z_PhiGen",   &Z_PhiGen);
  t->SetBranchAddress("isGoodZ",    &isGoodZ);
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
  t->SetBranchAddress("ePlusClassificationGen", &ePlusClassificationGen);
  t->SetBranchAddress("eMinusClassificationGen",&eMinusClassificationGen);
  t->SetBranchAddress("ePlusClassification", &ePlusClassification);
  t->SetBranchAddress("eMinusClassification",&eMinusClassification);
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

//   t->SetBranchStatus("isGoodZ",     1);
//   t->SetBranchStatus("ePlusClassificationGen",  1);
//   t->SetBranchStatus("eMinusClassificationGen", 1);
//   t->SetBranchStatus("ePlusClassification",     1);
//   t->SetBranchStatus("eMinusClassification",    1);




  // No Cuts
  TH1F NoCutGen("NoCutGen","", 50, 0.0, 2.5);
  NoCutGen.Sumw2();
  TAxis* NoCutGenx = NoCutGen.GetXaxis();
  TAxis* NoCutGeny = NoCutGen.GetYaxis();
  NoCutGenx->SetTitle("p_{T,Jet} / p_{T,Z}   ");
  NoCutGeny->SetTitle("Events / 0.05         ");
  NoCutGeny->SetTitleOffset(1.2);

  TH1F NoCutReco("NoCutReco","", 50, 0.0, 2.5);
  NoCutReco.Sumw2();
  NoCutReco.SetLineColor(2);
  NoCutReco.SetMarkerColor(2);


  // Leading-jet eta cut
  TH1F LeadEtaCutGen("LeadEtaCutGen","", 50, 0.0, 2.5);
  LeadEtaCutGen.Sumw2();
  TAxis* LeadEtaCutGenx = LeadEtaCutGen.GetXaxis();
  TAxis* LeadEtaCutGeny = LeadEtaCutGen.GetYaxis();
  LeadEtaCutGenx->SetTitle("p_{T,Jet} / p_{T,Z}   ");
  LeadEtaCutGeny->SetTitle("Events / 0.05         ");
  LeadEtaCutGeny->SetTitleOffset(1.2);

  TH1F LeadEtaCutReco("LeadEtaCutReco","", 50, 0.0, 2.5);
  LeadEtaCutReco.Sumw2();
  LeadEtaCutReco.SetLineColor(2);
  LeadEtaCutReco.SetMarkerColor(2);


  // dPhi(Leading-jet, Z)  cut
  TH1F dPhiCutGen("dPhiCutGen","", 50, 0.0, 2.5);
  dPhiCutGen.Sumw2();
  TAxis* dPhiCutGenx = dPhiCutGen.GetXaxis();
  TAxis* dPhiCutGeny = dPhiCutGen.GetYaxis();
  dPhiCutGenx->SetTitle("p_{T,Jet} / p_{T,Z}   ");
  dPhiCutGeny->SetTitle("Events / 0.05         ");
  dPhiCutGeny->SetTitleOffset(1.2);

  TH1F dPhiCutReco("dPhiCutReco","", 50, 0.0, 2.5);
  dPhiCutReco.Sumw2();
  dPhiCutReco.SetLineColor(2);
  dPhiCutReco.SetMarkerColor(2);



  // Second-jet pT cut
  TH1F SecondJetCutGen("SecondJetCutGen","", 50, 0.0, 2.5);
  SecondJetCutGen.Sumw2();
  TAxis* SecondJetCutGenx = SecondJetCutGen.GetXaxis();
  TAxis* SecondJetCutGeny = SecondJetCutGen.GetYaxis();
  SecondJetCutGenx->SetTitle("p_{T,Jet} / p_{T,Z}   ");
  SecondJetCutGeny->SetTitle("Events / 0.05         ");
  SecondJetCutGeny->SetTitleOffset(1.2);

  TH1F SecondJetCutReco("SecondJetCutReco","", 50, 0.0, 2.5);
  SecondJetCutReco.Sumw2();
  SecondJetCutReco.SetLineColor(2);
  SecondJetCutReco.SetMarkerColor(2);



  // electron pT cut
  TH1F electronPtCutGen("electronPtCutGen","", 50, 0.0, 2.5);
  electronPtCutGen.Sumw2();
  TAxis* electronPtCutGenx = electronPtCutGen.GetXaxis();
  TAxis* electronPtCutGeny = electronPtCutGen.GetYaxis();
  electronPtCutGenx->SetTitle("p_{T,Jet} / p_{T,Z}   ");
  electronPtCutGeny->SetTitle("Events / 0.05         ");
  electronPtCutGeny->SetTitleOffset(1.2);

  TH1F electronPtCutReco("electronPtCutReco","", 50, 0.0, 2.5);
  electronPtCutReco.Sumw2();
  electronPtCutReco.SetLineColor(2);
  electronPtCutReco.SetMarkerColor(2);





  // Zmass cut
  TH1F ZmassCutGen("ZmassCutGen","", 50, 0.0, 2.5);
  ZmassCutGen.Sumw2();
  TAxis* ZmassCutGenx = ZmassCutGen.GetXaxis();
  TAxis* ZmassCutGeny = ZmassCutGen.GetYaxis();
  ZmassCutGenx->SetTitle("p_{T,Jet} / p_{T,Z}   ");
  ZmassCutGeny->SetTitle("Events / 0.05         ");
  ZmassCutGeny->SetTitleOffset(1.2);

  TH1F ZmassCutReco("ZmassCutReco","", 50, 0.0, 2.5);
  ZmassCutReco.Sumw2();
  ZmassCutReco.SetLineColor(2);
  ZmassCutReco.SetMarkerColor(2);




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



    //     int leadGenIndex = 0, secondGenIndex = 1, 
    //       leadRecoIndex = 0, secondRecoIndex = 1;


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


    NoCutGen.Fill( ptRatioGen );
    NoCutReco.Fill( ptRatioReco );

    if( fabs(leadGenJetEta)<1.3 ) {
      LeadEtaCutGen.Fill( ptRatioGen );
      LeadEtaCutReco.Fill( ptRatioReco );

      if( fabs(dPhiGen)>2.94 ) {
	dPhiCutGen.Fill( ptRatioGen );
	dPhiCutReco.Fill( ptRatioReco );

	if(secondGenJetPt/leadGenJetPt<0.2) {
	  SecondJetCutGen.Fill( ptRatioGen );
	  SecondJetCutReco.Fill( ptRatioReco );


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


	  if(  isPtCut && isEtaCutP && isEtaCutM && isECALisoP && 
	       isECALisoM && isTrackisoP && isTrackisoM) {

	    electronPtCutGen.Fill( ptRatioGen );
	    electronPtCutReco.Fill( ptRatioReco );
	    
	    if(fabs(mZee-91.2)<2.5) { 
	      ZmassCutGen.Fill( ptRatioGen );
	      ZmassCutReco.Fill( ptRatioReco );
	    } // end Z mass cut
	  } // end lepton cut
	} // end second jet cut
      } // end dPhi cut
    } // end lead eta cut

  } // end TTree loop



  genMean   = (Float_t) ZmassCutGen.GetMean(1);
  genSigma  = (Float_t) ZmassCutGen.GetMean(11);
  recoMean  = (Float_t) ZmassCutReco.GetMean(1); 
  recoSigma = (Float_t) ZmassCutReco.GetMean(11);


  if(makeplot==true) {
    PlotOnCanvas( NoCutGen, NoCutReco, "ptBalance-NoCut");
    PlotOnCanvas( LeadEtaCutGen, LeadEtaCutReco, "ptBalance-LeadEtaCut");
    PlotOnCanvas( dPhiCutGen, dPhiCutReco, "ptBalance-LeadEtaPhiCut");
    PlotOnCanvas( SecondJetCutGen, SecondJetCutReco, "ptBalance-LeadEtaPhiSecondJetCut");
    PlotOnCanvas( electronPtCutGen, electronPtCutReco, 
		  "ptBalance-LeadEtaPhiSecondJetCut-eleCut");
    PlotOnCanvas( ZmassCutGen, ZmassCutReco, 
		  "ptBalance-LeadEtaPhiSecondJetCut-eleCut-ZmassCut");
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


