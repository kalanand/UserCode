
void plot_ZJetBalance() {

  const bool plotdebug = false;
  const bool useLocalFile = false;
 const int NUM_JET_MAX = 4;



/******** Read input ROOT files ************/
/*******************************************/

TChain* t = new TChain("ZJet");
if( useLocalFile == true ) {
  t->Add("analysis_zjet.root");
}
else {
  t->Add("/uscms_data/d1/cmsjtmet/kalanand/CSA07-ZeeJets-Ntuple/*.root");
}
/***********************************/


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





  TH1F h1("h1","", 50, 0.0, 2.5);
  h1.Sumw2();
  TAxis* h1x = h1.GetXaxis();
  TAxis* h1y = h1.GetYaxis();
  h1x->SetTitle("p_{T,Jet} / p_{T,Z}   ");
  h1y->SetTitle("Events / 0.05         ");
  h1y->SetTitleOffset(1.2);

  TH1F h2("h2","", 50, 0.0, 2.5);
  h2.Sumw2();
  h2.SetLineColor(2);
  h2.SetMarkerColor(2);



  TH1F LeadEtaCutGen("LeadEtaCutGen","", 50, 0.0, 2.5);
  LeadEtaCutGen.Sumw2();
  TAxis* h1x = LeadEtaCutGen.GetXaxis();
  TAxis* h1y = LeadEtaCutGen.GetYaxis();
  LeadEtaCutGenx->SetTitle("p_{T,Jet} / p_{T,Z}   ");
  LeadEtaCutGeny->SetTitle("Events / 0.05         ");
  LeadEtaCutGeny->SetTitleOffset(1.2);

  TH1F LeadEtaCutReco("LeadEtaCutReco","", 50, 0.0, 2.5);
  LeadEtaCutReco.Sumw2();
  LeadEtaCutReco.SetLineColor(2);
  LeadEtaCutReco.SetMarkerColor(2);








  if( plotdebug ) {
    // plot delta_R w.r.t. e+
    TH1F dR_eplus_gen("dR_eplus_gen","", 100, 0.0, 1.0);
    dR_eplus_gen.Sumw2();
    TAxis* gZx = dR_eplus_gen.GetXaxis();
    TAxis* gZy = dR_eplus_gen.GetYaxis();
    gZx->SetTitle("#Delta R     ");
    gZy->SetTitle("Events / 0.01  ");
    gZy->SetTitleOffset(1.2);
    TH1F dR_eplus_reco("dR_eplus_reco","", 100, 0.0, 1.0);
    dR_eplus_reco.Sumw2();
    dR_eplus_reco.SetLineColor(2);
    dR_eplus_reco.SetMarkerColor(2);


    // plot delta_R w.r.t. e-
    TH1F dR_eminus_gen("dR_eminus_gen","", 100, 0.0, 1.0);
    dR_eminus_gen.Sumw2();
    TAxis* gZx = dR_eminus_gen.GetXaxis();
    TAxis* gZy = dR_eminus_gen.GetYaxis();
    gZx->SetTitle("#Delta R     ");
    gZy->SetTitle("Events / 0.01  ");
    gZy->SetTitleOffset(1.2);
    TH1F dR_eminus_reco("dR_eminus_reco","", 100, 0.0, 1.0);
    dR_eminus_reco.Sumw2();
    dR_eminus_reco.SetLineColor(2);
    dR_eminus_reco.SetMarkerColor(2);
  }




  for (Long64_t entry =0; entry < t->GetEntries(); entry++) {
    t->GetEntry(entry);
    if(entry%5000==0) std::cout<<"************ Event # "<< entry <<std::endl;

    // for golden electrons
    //  if(ePlusClassification % 100  || eMinusClassification % 100) continue;

    // for golden + isolated electrons
//     if(ePlusClassification % 100  || 
//    eMinusClassification % 100 || isGoodZ==0) continue;

//     for (int j=0; j<NUM_JET_MAX; j++) {
//       dR_eplus_gen.Fill(radius(JetGenEta[j], 
// 			       JetGenPhi[j], ePlusEtaGen, ePlusPhiGen));
//       dR_eminus_gen.Fill(radius(JetGenEta[j], 
// 				JetGenPhi[j], eMinusEtaGen, eMinusPhiGen));
//       dR_eplus_reco.Fill(radius(JetRecoEta[j], 
// 				JetRecoPhi[j], ePlusEtaGen, ePlusPhiGen));
//       dR_eminus_reco.Fill(radius(JetRecoEta[j], 
// 				 JetRecoPhi[j], eMinusEtaGen, eMinusPhiGen));
//     }


    int leadGenIndex=-1, secondGenIndex=-1, 
      leadRecoIndex=-1, secondRecoIndex=-1;


    FindLeadIndex(JetGenPt, JetGenEta, JetGenPhi, NUM_JET_MAX, 
		  ePlusEtaGen, ePlusPhiGen, eMinusEtaGen, eMinusPhiGen, 
		  leadGenIndex, secondGenIndex);


    FindLeadIndex(JetRecoPt, JetRecoEta, JetRecoPhi, NUM_JET_MAX, 
		  ePlusEtaGen, ePlusPhiGen, eMinusEtaGen, eMinusPhiGen, 
		  leadRecoIndex, secondRecoIndex);


    if(leadGenIndex==-1 || leadRecoIndex==-1) continue;

//     FindLeadIndex(JetGenPt, 10, leadGenIndex, secondGenIndex);
//     FindLeadIndex(JetRecoPt, 10, leadRecoIndex, secondRecoIndex);

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



    genZmass.Fill(mZeeGen);
    recoZmass.Fill(mZee);







    bool isCutGenJet = (fabs(leadGenJetEta)<1.3) && 
      (fabs(dPhiGen)>2.94) && (secondGenJetPt/leadGenJetPt<0.2);

    bool isCutCaloJet = (fabs(leadRecoJetEta)<1.3) && 
      (fabs(dPhiRico)>2.94) && (secondRecoJetPt/leadRecoJetPt<0.2);







    /*
    if(Z_PtGen>80.0 && Z_PtGen<120.0 && fabs(leadGenJetEta)<1.3 && 
       fabs(dPhiGen)>2.7 && secondGenJetPt/leadGenJetPt<0.2) 
      h1.Fill(leadGenJetPt/Z_PtGen);

    if(Z_Pt>80.0 && Z_Pt<120.0  && fabs(leadRecoJetEta)<1.3 && 
       fabs(dPhiReco)>2.7 && secondRecoJetPt/leadRecoJetPt<0.2) 
      h2.Fill(leadRecoJetPt/Z_Pt);

    if(fabs(dPhiGen)>2.7 && fabs(leadGenJetEta)<1.3 && ePlusPtGen>20.0 
       && eMinusPtGen>20.0 && fabs(ePlusEtaGen)<1.0 
       && fabs(eMinusEtaGen)<1.0 && mZeeGen>60.0 && mZeeGen<120.0){
    */


    if(!(Z_PtGen>80.0 && Z_PtGen<120.0)) continue;
    // if(fabs(leadGenJetEta)<1.3) {
    // if(fabs(dPhiGen)>2.7 && fabs(leadGenJetEta)<1.3) {
    if(fabs(dPhiGen)>2.7 && fabs(leadGenJetEta)<1.3 && 
       fabs(mZeeGen-91.2)<2.5){

      
      h1.Fill(leadGenJetPt/Z_PtGen);
      h2.Fill(leadRecoJetPt/Z_Pt);
      /*  h2.Fill(leadRecoJetPt/Z_PtGen);*/
    }
  }


  TLine* line1 = new TLine(1.0, h1.GetMinimum(), 1.0, h1.GetMaximum());
  line1->SetLineColor(4);
  line1->SetLineWidth(2);



  TCanvas canvas("canvas","",880,600);
  gStyle->SetOptStat(0);
  h1.Draw("hist");
  line1->Draw();
  h2.Draw("same");
  h2.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.65,0.85,0.85);
  /*  leg_hist->AddEntry("","Gluon Jets","");*/
  leg_hist->AddEntry(&h1,"Generator level","l");
  leg_hist->AddEntry(&h2,"Calorimeter level","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();

  // TString plotname = "Fig13b-nocuts";
  // TString plotname = "Fig13b-ZptCut";
  // TString plotname = "Fig13b-ZptCut-LeadEtaCut";
  // TString plotname = "Fig13b-ZptCut-LeadEtaCut-dPhiCut";
    TString plotname = "Fig13b-ZptCut-LeadEtaCut-dPhiCut-ZmassCut";


  //  TString plotname = "Fig13b";
  canvas.SaveAs(plotname+TString(".eps"));
  canvas.SaveAs(plotname+TString(".gif"));
  canvas.SaveAs(plotname+TString(".root"));






  if(plotdebug) {
    // -------------------------------
    TCanvas dR_eplus("dR_eplus","",880,600);
    gStyle->SetOptStat(0);
    dR_eplus_gen.Draw("hist");
    dR_eplus_reco.Draw("same");
    leg_hist = new TLegend(0.6,0.58,0.85,0.86);
    leg_hist->AddEntry("","#Delta R relative to e^{+}","");
    leg_hist->AddEntry(&dR_eplus_gen,"Generator level","l");
    leg_hist->AddEntry(&dR_eplus_reco,"Calorimeter level","l");
    leg_hist->SetFillColor(0);
    leg_hist->Draw();
    TString plotname = "dR_eplus";
    dR_eplus.SaveAs(plotname+TString(".eps"));
    dR_eplus.SaveAs(plotname+TString(".gif"));
    dR_eplus.SaveAs(plotname+TString(".root"));
  
    // -------------------------------
    TCanvas dR_eminus("dR_eminus","",880,600);
    gStyle->SetOptStat(0);
    dR_eminus_gen.Draw("hist");
    dR_eminus_reco.Draw("same");
    leg_hist = new TLegend(0.6,0.58,0.85,0.86);
    leg_hist->AddEntry("","#Delta R relative to e^{-}","");
    leg_hist->AddEntry(&dR_eminus_gen,"Generator level","l");
    leg_hist->AddEntry(&dR_eminus_reco,"Calorimeter level","l");
    leg_hist->SetFillColor(0);
    leg_hist->Draw();
    TString plotname = "dR_eminus";
    dR_eminus.SaveAs(plotname+TString(".eps"));
    dR_eminus.SaveAs(plotname+TString(".gif"));
    dR_eminus.SaveAs(plotname+TString(".root"));

    // -------------------------------
    // -------------------------------

  }
}



void FindLeadIndex(float pT[], float eta[], float phi[], int size, 
		   float e1eta, float e1phi, float e2eta, float e2phi, 
		   int &lead, int &sec) {

  float max = 0.0;
  lead = -1;
  for (int i=0; i<size; i++) {
    if(radius(eta[i], phi[i], e1eta, e1phi) < 0.3) continue;
    if(radius(eta[i], phi[i], e2eta, e2phi) < 0.3) continue;
    if(pT[i]>max) { max = pT[i]; lead = i; }
  }

  max = 0.0;
  sec = -1;
  for (int i=0; i<size; i++) {
    if(i==lead) continue;
    if(radius(eta[i], phi[i], e1eta, e1phi) < 0.3) continue;
    if(radius(eta[i], phi[i], e2eta, e2phi) < 0.3) continue;
    if(pT[i]>max) { max = pT[i]; sec = i; }
  }
}




// void FindLeadIndex(float pT[], int size, int &lead, int &sec) {

//   float max = 0.0;
//   lead = -1;
//   for (int i=0; i<size; i++) {
//     if(pT[i]>max) { max = pT[i]; lead = i; }
//   }

//   max = 0.0;
//   sec = -1;
//   for (int i=0; i<size; i++) {
//     if(i==lead) continue;
//     if(pT[i]>max) { max = pT[i]; sec = i; }
//   }
// }





double dPhi(double phi1,double phi2){
   phi1=Phi_0_2pi(phi1);
   phi2=Phi_0_2pi(phi2);
   return Phi_mpi_pi(phi1-phi2);
}




double Phi_mpi_pi(double x) {
  const double M_PI = TMath::Pi();
   while (x >= M_PI) x -= 2*M_PI;
   while (x < -M_PI) x += 2*M_PI;
   return x;
}





double Phi_0_2pi(double x) {
  const double M_PI = TMath::Pi();
  while (x >= 2*M_PI) x -= 2*M_PI;
  while (x <     0.)  x += 2*M_PI;
  return x;
}



double radius(double eta1, double phi1,double eta2, double phi2){
 
  const double TWOPI= 2.0*TMath::Pi();
 
  phi1=Phi_0_2pi(phi1);
  phi2=Phi_0_2pi(phi2);

  double dphi=Phi_0_2pi(phi1-phi2);
  dphi = TMath::Min(dphi,TWOPI-dphi);
  double deta = eta1-eta2;

  return sqrt(deta*deta+dphi*dphi);
}
