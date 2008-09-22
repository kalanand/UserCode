void Fig12b() {

  TH1F h1("h1","", 50, 0.0, 2.5);
  h1.Sumw2();
  TAxis* h1x = h1.GetXaxis();
  TAxis* h1y = h1.GetYaxis();
  h1x->SetTitle("p_{T,Jet} / p_{T,Z}   ");
  h1y->SetTitle("Events / 0.05         ");
  h1y->SetTitleOffset(1.2);

  TH1F h2("h2","", 50, 0.0, 2.5);
  h2.Sumw2();
  // h2.SetLineStyle(2);
  h2.SetLineColor(2);
  h2.SetMarkerColor(2);

  TFile f1("analysis_zjet.root");
  TTree* t = (TTree*)f1.Get("ZJet");

  Float_t JetRecoPt[10];
  Float_t JetGenPt[10];
  Float_t JetRecoEta[10];
  Float_t JetGenEta[10];
  Float_t JetRecoPhi[10];
  Float_t JetGenPhi[10];
  Float_t Z_Pt;
  Float_t Z_Phi;
  Float_t Z_PtGen;
  Float_t Z_PhiGen;
  bool isGoodZ;

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


  for (Long64_t entry =0; entry < t->GetEntries(); entry++) {
    t->GetEntry(entry);
    if(entry%5000==0) std::cout<<"************ Event # "<< entry <<std::endl;


    int leadGenIndex=0, secondGenIndex=1, leadRecoIndex=0, secondRecoIndex=1;
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


//     if(Z_PtGen>80.0 && Z_PtGen<120.0 && fabs(leadGenJetEta)<1.3 && 
//        fabs(dPhiGen)>2.7 && secondGenJetPt/leadGenJetPt<0.2) 
//       h1.Fill(leadGenJetPt/Z_PtGen);

//     if(Z_Pt>80.0 && Z_Pt<120.0  && fabs(leadRecoJetEta)<1.3 && 
//        fabs(dPhiReco)>2.7 && secondRecoJetPt/leadRecoJetPt<0.2) 
//       h2.Fill(leadRecoJetPt/Z_Pt);


    if(!(Z_PtGen>80.0 && Z_PtGen<120.0)) continue;

    if(fabs(dPhiGen)>2.7 && fabs(leadGenJetEta)<1.3) {
      h1.Fill(leadGenJetPt/Z_PtGen);
      h2.Fill(leadRecoJetPt/Z_Pt);
    }
  }


  TLine* line1 = new TLine(1.0, h1.GetMinimum(), 1.0, h1.GetMaximum());
  line1->SetLineColor(4);
  line1->SetLineWidth(2);


  TCanvas canvas("canvas","",880,600);
  gStyle->SetOptStat(0);
  h1.Draw("hist");
  h2.Draw("same");
  line1->Draw();
  leg_hist = new TLegend(0.6,0.65,0.85,0.85);
  leg_hist->AddEntry("","Gluon Jets","");
  leg_hist->AddEntry(&h1,"Generator level","l");
  leg_hist->AddEntry(&h2,"Calorimeter level","l");
  leg_hist->Draw();
  leg_hist->SetFillColor(0);
  canvas.SaveAs("Fig12b.eps");
  canvas.SaveAs("Fig12b.gif");
  canvas.SaveAs("Fig12b.root");
}





void FindLeadIndex(float alist[], int size, int &lead, int &sec) {

  float max = 0.0;
  lead = -1;
  for (int i=0; i<size; i++) {
    if(alist[i]>max) { max = alist[i]; lead = i; }
  }

  max = 0.0;
  sec = -1;
  for (int i=0; i<size; i++) {
    if(i==lead) continue;
    if(alist[i]>max) { max = alist[i]; sec = i; }
  }
}





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
