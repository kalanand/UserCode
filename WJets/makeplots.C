const double intLUMI = 935.;

void makeplots ()
{
   const double WJets_scale = 31500.* intLUMI/51000000;
   // const double WJets_scale = 31500.* intLUMI/15161497;
  // const double WW_scale = (27.79 * 36./2050240) / 0.1075 / 0.676;
  const double WW_scale = (43.* intLUMI/2050240);
  // correcting for BR
  // const double WZ_scale = (10.4 * 36./2194752) / 0.1075 / 0.6991;
  const double WZ_scale = (18. * intLUMI/2194752);
  // correcting for BR
  const double TT_scale = 2.*12. * intLUMI/4816224;


  const double normFactor = 1.6;
  char* dijetPt = "sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))";

  TFile *file0 = TFile::Open("data/WenuJets_DataAll_GoldenJSON_917invpb.root");
  TTree* tree = (TTree*) file0->Get("WJet");
  tree->SetAlias("dijetPt", dijetPt);

  TFile *file1 = TFile::Open("data/WenuJets_CMSSW423-Summer11MC_WJets.root");
  TTree* treeMC_WJets = (TTree*) file1->Get("WJet");
  treeMC_WJets->SetAlias("dijetPt", dijetPt);

  TFile *file2 = TFile::Open("data/WenuJets_CMSSW415-Spring11MC_WWtoAnything.root");
  TTree* treeMC_WW_enujj = (TTree*) file2->Get("WJet");
  treeMC_WW_enujj->SetAlias("dijetPt", dijetPt);

  TFile *file3 = TFile::Open("data/WenuJets_CMSSW415-Spring11MC_WZtoAnything.root");
  TTree* treeMC_WZ_enujj = (TTree*) file3->Get("WJet");
  treeMC_WZ_enujj->SetAlias("dijetPt", dijetPt);

  TFile *file4 = TFile::Open("data/WenuJets_CMSSW415-Spring11MC_TTToLNu2Q2B.root");
  TTree* treeMC_TT = (TTree*) file4->Get("WJet");
  treeMC_TT->SetAlias("dijetPt", dijetPt);


//////////// binning
  const int jjmass_NBINS = 11;
  const double jjmass_MIN = 45.;
  const double jjmass_MAX = 155.;


//   const int jjmass_NBINS = 15;
//   const double jjmass_MIN = 70.;
//   const double jjmass_MAX = 100.;


  const int jjdeta_NBINS = 40;
  const double jjdeta_MIN = -5.;
  const double jjdeta_MAX = 5.;

  const int jjdphi_NBINS = 16;
  const double jjdphi_MIN = 0;
  const double jjdphi_MAX = 3.14159265358979312;


///////////// cuts 
  TCut WleptPre("JetPFCor_Pt[0]>30. && JetPFCor_Pt[1]>30. && JetPFCor_Pt[2]<0.5 && abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.5 && W_electron_et>30. && W_electron_isWP80==1 && (W_electron_trackiso+W_electron_hcaliso+W_electron_ecaliso-event_RhoForLeptonIsolation*3.141592653589*0.09)/W_electron_pt<0.05 && ((abs(W_electron_eta)<1.5 && abs(W_electron_deltaphi_in)<0.03 && abs(W_electron_deltaeta_in)<0.004) || (abs(W_electron_eta)>1.5 && abs(W_electron_deltaphi_in)<0.02 && abs(W_electron_deltaeta_in)<0.005)) && sqrt((W_electron_vx-event_BeamSpot_x)**2+(W_electron_vy-event_BeamSpot_y)**2)<0.02 && JetPFCor_bDiscriminator[0]<1.19 && JetPFCor_bDiscriminator[1]<1.19 && cosJacksonAngle2j_PFCor<0.8 && cosJacksonAngle2j_PFCor>-0.6");
  TCut goodMET( WleptPre && "event_met_pfmet>30.");
  TCut goodW(goodMET && "W_mt>40.");
  TCut twojets(TString(dijetPt) + TString(">40."));

  TCut twoBJets("numPFCorJetBTags==2");
  TCut noBJets("");

//   TCut centrality("abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.5");
//   TCut dphicut("abs(abs(abs(JetPFCor_Phi[0]-JetPFCor_Phi[1])-TMath::Pi())-TMath::Pi())>1.5 && abs(JetPFCor_dphiMET[0])>1.0 && abs(JetPFCor_dphiMET[1])>1.0");

  // TCut centrality("abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.3");

  TCut centrality("abs(tanh(0.5*(JetPFCor_Y[0]-JetPFCor_Y[1])))<0.4");

  TCut dphicut121M("abs(abs(abs(JetPFCor_Phi[0]-JetPFCor_Phi[1])-TMath::Pi())-TMath::Pi())>1.5 && abs(JetPFCor_dphiMET[0])>0.6");
  TCut dphicut122M("abs(abs(abs(JetPFCor_Phi[0]-JetPFCor_Phi[1])-TMath::Pi())-TMath::Pi())>1.5");
  TCut dphicut1M2M("abs(JetPFCor_dphiMET[0])>0.6");
  TCut dphicut("abs(abs(abs(JetPFCor_Phi[0]-JetPFCor_Phi[1])-TMath::Pi())-TMath::Pi())>1.5 && abs(JetPFCor_dphiMET[0])>0.6");

  TCut dphicutWjj("abs(abs(abs(W_phi-atan2((JetPFCor_Py[0]+JetPFCor_Py[1]),(JetPFCor_Px[0]+JetPFCor_Px[1])))-TMath::Pi())-TMath::Pi())>2.94");

  TCut allCuts( goodW && twojets && noBJets);


///////////// variables
  char* mjj = "Mass2j_PFCor";
  char* jjdeta = "JetPFCor_Eta[0]-JetPFCor_Eta[1]";
  char* jjcostheta = "tanh(0.5*(JetPFCor_Y[0]-JetPFCor_Y[1]))";
  char* jjdphi = "abs(abs(abs(JetPFCor_Phi[0]-JetPFCor_Phi[1])-TMath::Pi())-TMath::Pi())";
  char* eMETdphi = "abs(abs(abs(W_electron_phi-event_met_pfmetPhi)-TMath::Pi())-TMath::Pi())";
  char* ej1dphi = "abs(abs(abs(W_electron_phi-JetPFCor_Phi[0])-TMath::Pi())-TMath::Pi())";
  char* ej2dphi = "abs(abs(abs(W_electron_phi-JetPFCor_Phi[1])-TMath::Pi())-TMath::Pi())";

  char* dphiWjj = "abs(abs(abs(W_phi-atan2((JetPFCor_Py[0]+JetPFCor_Py[1]),(JetPFCor_Px[0]+JetPFCor_Px[1])))-TMath::Pi())-TMath::Pi())";
// ################# How many events we started with ?
  int nEvents = tree->Draw( "W_mt", goodW, "goff");;
  cout << "# events in the W+jj skim = " << nEvents << endl;


// ################# How many good W+jj are there ?
  nEvents = tree->Draw( "W_mt", goodW, "goff");
  cout << "Number of W+jj events = " << nEvents << endl;



  gROOT->ProcessLine(".L ~/tdrstyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);
  gStyle->SetOptFit(1111);
  //tdrStyle->SetOptStat("mer"); 
  char temp[1000];



  TCanvas* c_wmt = new TCanvas("c_wmt","W mT", 500, 500);
  TH1D* h_wmt = new TH1D("h_wmt","", 60, 40, 160);
  tree->Draw("W_mt>>h_wmt", goodW, "goff");
  h_wmt->GetXaxis()->SetTitle("W m_{T} [GeV]");
  h_wmt->GetYaxis()->SetTitle("Events / 2 GeV");
  h_wmt->GetYaxis()->SetTitleOffset(1.4);
  TH1D* h_wmt_WJets = new TH1D("h_wmt_WJets","", 60, 40, 160);
  h_wmt_WJets->SetLineColor(2);
  h_wmt_WJets->SetFillColor(2);
  treeMC_WJets->Draw( "W_mt>>h_wmt_WJets", goodW, "goff");
  h_wmt_WJets->Scale( WJets_scale );
  TH1D* h_wmt_WW_enujj = new TH1D("h_wmt_WW_enujj","", 60, 40, 160);
  h_wmt_WW_enujj->SetFillColor(kOrange);
  TH1D* h_wmt_WZ_enujj = h_wmt_WW_enujj->Clone("h_wmt_WZ_enujj");
  treeMC_WW_enujj->Draw( "W_mt>>h_wmt_WW_enujj", goodW, "goff");
  h_wmt_WW_enujj->Scale( WW_scale );
  treeMC_WZ_enujj->Draw( "W_mt>>h_wmt_WZ_enujj", goodW, "goff");
  h_wmt_WZ_enujj->Scale( WZ_scale );
  h_wmt_WW_enujj->Add(h_wmt_WZ_enujj);
  TH1D* h_wmt_TT = new TH1D("h_wmt_TT","", 60, 40, 160);
  h_wmt_TT->SetLineColor(3);
  h_wmt_TT->SetLineWidth(2);
  h_wmt_TT->SetFillColor(3);
  treeMC_TT->Draw( "W_mt>>h_wmt_TT", goodW, "goff");
  h_wmt_TT->Scale( TT_scale );
  h_wmt->SetMinimum(0.0);
  h_wmt_WW_enujj->Add(h_wmt_TT);
  h_wmt_WJets->Add(h_wmt_WW_enujj);
  h_wmt->SetMaximum( 1.3 * h_wmt->GetMaximum() );
  h_wmt->Draw("e");
  h_wmt_WJets->Draw("same");
  h_wmt_TT->Draw("same");
  h_wmt_WW_enujj->Draw("same");
  h_wmt->Draw("esame");
  c_wmt->RedrawAxis();
  TLegend* legend = new TLegend(0.55,0.7,0.89,0.89);
  legend->SetFillColor(0);
  legend->AddEntry(h_wmt, "Data", "PLE");  
  legend->AddEntry(h_wmt_WW_enujj, "WW/WZ#rightarrow l#nujj", "F");
  legend->AddEntry(h_wmt_WJets, "W(#rightarrow l#nu)+jj", "F");
  legend->AddEntry(h_wmt_TT, "t#bar{t},single top", "F");
  legend->Draw();
  cmsPrelim();
  c_wmt->SaveAs("Wenu-mt.gif");
  c_wmt->SaveAs("Wenu-mt.pdf");



//////////////////////////////////////////////////////////
  TCanvas* c_wmet = new TCanvas("c_wmet","W MET", 500, 500);
  TH1D* h_wmet = new TH1D("h_wmet","", 50, 0, 100);
  tree->Draw("event_met_pfmet>>h_wmet", WleptPre && "W_mt>40.", "goff");
  h_wmet->GetXaxis()->SetTitle("MET [GeV]");
  h_wmet->GetYaxis()->SetTitle("Events / 2 GeV");
  h_wmet->GetYaxis()->SetTitleOffset(1.4);
  TH1D* h_wmet_WJets = new TH1D("h_wmet_WJets","", 50, 0, 100);
  h_wmet_WJets->SetLineColor(2);
  h_wmet_WJets->SetFillColor(2);
  treeMC_WJets->Draw( "event_met_pfmet>>h_wmet_WJets", WleptPre && "W_mt>40.", "goff");
  h_wmet_WJets->Scale( WJets_scale );
  TH1D* h_wmet_WW_enujj = new TH1D("h_wmet_WW_enujj","", 50, 0, 100);
  h_wmet_WW_enujj->SetFillColor(kOrange);
  TH1D* h_wmet_WZ_enujj = h_wmet_WW_enujj->Clone("h_wmet_WZ_enujj");
  treeMC_WW_enujj->Draw( "event_met_pfmet>>h_wmet_WW_enujj", WleptPre && "W_mt>40.", "goff");
  h_wmet_WW_enujj->Scale( WW_scale );
  treeMC_WZ_enujj->Draw( "event_met_pfmet>>h_wmet_WZ_enujj", WleptPre && "W_mt>40.", "goff");
  h_wmet_WZ_enujj->Scale( WZ_scale );
  h_wmet_WW_enujj->Add(h_wmet_WZ_enujj);
  TH1D* h_wmet_TT = new TH1D("h_wmet_TT","", 50, 0, 100);
  h_wmet_TT->SetLineColor(3);
  h_wmet_TT->SetLineWidth(2);
  h_wmet_TT->SetFillColor(3);
  treeMC_TT->Draw( "event_met_pfmet>>h_wmet_TT", WleptPre && "W_mt>40.", "goff");
  h_wmet_TT->Scale( TT_scale );
  h_wmet->SetMinimum(0.0);
  h_wmet_WW_enujj->Add(h_wmet_TT);
  h_wmet_WJets->Add(h_wmet_WW_enujj);
  h_wmet->SetMaximum( 1.3 * h_wmet->GetMaximum() );
  h_wmet->Draw("e");
  h_wmet_WJets->Draw("same");
  h_wmet_TT->Draw("same");
  h_wmet_WW_enujj->Draw("same");
  h_wmet->Draw("esame");
  c_wmet->RedrawAxis();
  TLegend* legend = new TLegend(0.55,0.7,0.89,0.89);
  legend->SetFillColor(0);
  legend->AddEntry(h_wmet, "Data", "PLE");  
  legend->AddEntry(h_wmet_WW_enujj, "WW/WZ#rightarrow l#nujj", "F");
  legend->AddEntry(h_wmet_WJets, "W(#rightarrow l#nu)+jj", "F");
  legend->AddEntry(h_wmet_TT, "t#bar{t},single top", "F");
  legend->Draw();
  cmsPrelim();
  TArrow* ar2 = new TArrow(30,50,30,1800,0.03,"<|");
  ar2->SetLineColor(1);
  ar2->SetFillColor(1);
  ar2->Draw();
  c_wmet->SaveAs("Wenu-met.gif");
  c_wmet->SaveAs("Wenu-met.pdf");



///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

  TCanvas* c_dijetPt = new TCanvas("c_dijetPt","dijet pT", 500, 500);
  TH1D* h_dijetPt = new TH1D("h_dijetPt","", 50, 0, 500);
  tree->Draw("dijetPt>>h_dijetPt", goodW, "goff");
  h_dijetPt->GetXaxis()->SetTitle("Dijet p_{T} [GeV]");
  h_dijetPt->GetYaxis()->SetTitle("Events / 10 GeV");
  h_dijetPt->GetYaxis()->SetTitleOffset(1.3);
  h_dijetPt->GetXaxis()->SetNdivisions(505);
  TH1D* h_dijetPt_WJets = new TH1D("h_dijetPt_WJets","", 50, 0, 500);
  h_dijetPt_WJets->SetLineColor(2);
  h_dijetPt_WJets->SetFillColor(2);
  treeMC_WJets->Draw( "dijetPt>>h_dijetPt_WJets", goodW, "goff");
  h_dijetPt_WJets->Scale( WJets_scale );
  TH1D* h_dijetPt_WW_enujj = new TH1D("h_dijetPt_WW_enujj","", 50, 0, 500);
  h_dijetPt_WW_enujj->SetFillColor(kOrange);
  TH1D* h_dijetPt_WZ_enujj = h_dijetPt_WW_enujj->Clone("h_dijetPt_WZ_enujj");
  treeMC_WW_enujj->Draw( "dijetPt>>h_dijetPt_WW_enujj", goodW, "goff");
  h_dijetPt_WW_enujj->Scale( WW_scale );
  treeMC_WZ_enujj->Draw( "dijetPt>>h_dijetPt_WZ_enujj", goodW, "goff");
  h_dijetPt_WZ_enujj->Scale( WZ_scale );
  h_dijetPt_WW_enujj->Add(h_dijetPt_WZ_enujj);
  TH1D* h_dijetPt_TT = new TH1D("h_dijetPt_TT","", 50, 0, 500);
  h_dijetPt_TT->SetLineColor(3);
  h_dijetPt_TT->SetLineWidth(2);
  h_dijetPt_TT->SetFillColor(3);
  treeMC_TT->Draw( "dijetPt>>h_dijetPt_TT", goodW, "goff");
  h_dijetPt_TT->Scale( TT_scale );
  h_dijetPt->SetMinimum(0.02);
  h_dijetPt->SetMaximum(100000);
  h_dijetPt_WW_enujj->Add(h_dijetPt_TT);
  h_dijetPt_WJets->Add(h_dijetPt_WW_enujj);
  h_dijetPt->SetMaximum( 1.3 * h_dijetPt->GetMaximum() );
  h_dijetPt->Draw("e");
  h_dijetPt_WJets->Draw("same");
  h_dijetPt_WW_enujj->Draw("same");
  h_dijetPt_TT->Draw("same");
  h_dijetPt->Draw("esame");
  c_dijetPt->RedrawAxis();
  TLegend* legend = new TLegend(0.55,0.7,0.89,0.89);
  legend->SetFillColor(0);
  legend->AddEntry(h_dijetPt, "Data", "PLE");  
  legend->AddEntry(h_dijetPt_WW_enujj, "WW/WZ#rightarrow l#nujj", "F");
  legend->AddEntry(h_dijetPt_WJets, "W(#rightarrow l#nu)+jj", "F");
  legend->AddEntry(h_dijetPt_TT, "t#bar{t},single top", "F");
  legend->Draw();
  cmsPrelim();
  c_dijetPt->SetLogy();
  TArrow* ar2 = new TArrow(40,0.1,40,4000,0.03,"<|");
  ar2->SetLineColor(1);
  ar2->SetFillColor(1);
  ar2->Draw();
  c_dijetPt->SaveAs("Wenu-dijetPt.gif");
  c_dijetPt->SaveAs("Wenu-dijetPt.pdf");
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////



/*


//   TCanvas* c_wy = new TCanvas("c_wy","y of neutrino", 500, 500);
//   TH1D* h_wy = new TH1D("h_wy","", 40, -4, 4);
//   tree->Draw( "W_y>>h_wy", goodWn, "e");
//   h_wy->GetXaxis()->SetTitle("y of neutrino");
//   h_wy->GetYaxis()->SetTitle("Events / 0.2");
//   TArrow* ar3 = new TArrow(-2.6,80,-2.6,140,0.03,"<|");
//   ar3->SetLineColor(2);
//   ar3->SetFillColor(2);
//   ar3->Draw();
//   TArrow* ar32 = new TArrow(2.6,80,2.6,140,0.03,"<|");
//   ar32->SetLineColor(2);
//   ar32->SetFillColor(2);
//   ar32->Draw();
//   c_wy->SaveAs("W-rapidity.gif");




  TCanvas* c_jjmass = new TCanvas("c_jjmass","jj mass", 500, 500);
  TH1D* h_jjmass = new TH1D("h_jjmass","", jjmass_NBINS, jjmass_MIN, jjmass_MAX);
  TH1D* h_jjmass_WJets = new TH1D("h_jjmass_WJets","", jjmass_NBINS, jjmass_MIN, jjmass_MAX);
  h_jjmass_WJets->SetLineColor(4);
  h_jjmass_WJets->SetLineWidth(2);
  TH1D* h_jjmass_WW_enujj = new TH1D("h_jjmass_WW_enujj","", jjmass_NBINS, jjmass_MIN, jjmass_MAX);
  h_jjmass_WW_enujj->SetFillColor(kOrange);
  TH1D* h_jjmass_WZ_enujj = h_jjmass_WW_enujj->Clone("h_jjmass_WZ_enujj");

  tree->Draw("Mass2j_PFCor>>h_jjmass", allCuts, "e");
//   gPad->Update();
//   TPaveStats *st = (TPaveStats*)h_jjmass->FindObject("stats");
//   st->SetName("stats1");
//   st->SetX1NDC(0.66); //new x start position
//   st->SetX2NDC(0.81); //new x end position
//   st->SetOptStat(1001100);
//   h_jjmass->GetListOfFunctions()->Add(st);

  treeMC_WJets->Draw( "Mass2j_PFCor>>h_jjmass_WJets", allCuts, "goff");
  h_jjmass_WJets->Scale( WJets_scale );
  //h_jjmass_WJets->Smooth(1000);
  h_jjmass_WJets->Draw("sames");
//   gPad->Update();
//   st = (TPaveStats*)h_jjmass_WJets->FindObject("stats");
//   st->SetName("stats2");
//   st->SetX1NDC(0.66); //new x start position
//   st->SetX2NDC(0.81); //new x end position
//   st->SetY1NDC(0.68); //new y start position
//   st->SetY2NDC(0.83); //new y end position
//   st->SetFillColor(kBlue-9);
//   st->SetOptStat(1100);
//   h_jjmass_WJets->GetListOfFunctions()->Add(st);

  treeMC_WW_enujj->Draw("Mass2j_PFCor>>h_jjmass_WW_enujj", allCuts, "goff");
  h_jjmass_WW_enujj->Scale( WW_scale );
  treeMC_WZ_enujj->Draw("Mass2j_PFCor>>h_jjmass_WZ_enujj", allCuts, "goff");
  h_jjmass_WZ_enujj->Scale( WZ_scale );
  h_jjmass_WW_enujj->Add(h_jjmass_WZ_enujj);
//  h_jjmass_WW_enujj->Smooth(1000);
  h_jjmass_WW_enujj->Draw("sames");
//   gPad->Update();
//   st = (TPaveStats*)h_jjmass_WW_enujj->FindObject("stats");
//   st->SetFillColor(kOrange);
//   st->SetName("stats3");
//   st->SetY1NDC(0.68); //new y start position
//   st->SetY2NDC(0.83); //new y end position
//   st->SetOptStat(1100);
//   h_jjmass_WW_enujj->GetListOfFunctions()->Add(st);

  TH1D* h_jjmass_TT = new TH1D("h_jjmass_TT","", jjmass_NBINS, jjmass_MIN, jjmass_MAX);
  h_jjmass_TT->SetLineColor(4);
  h_jjmass_TT->SetLineWidth(2);
  h_jjmass_TT->SetLineStyle(2);
  treeMC_TT->Draw( "Mass2j_PFCor>>h_jjmass_TT", goodMET, "goff");
//  h_jjmass_TT->Smooth(1000);
//  h_jjmass_TT->Scale( 4*TT_scale );
  //h_jjmass_TT->Draw("same");

  h_jjmass->SetMaximum( 1.5 * h_jjmass->GetMaximum() );
  h_jjmass->GetXaxis()->SetTitle("m_{jj} of leading two jets [GeV]");
  sprintf(temp, "Events / %.1f GeV", (jjmass_MAX-jjmass_MIN)/jjmass_NBINS);
  h_jjmass->GetYaxis()->SetTitle(temp);
  h_jjmass->GetXaxis()->SetNdivisions(505);
  c_jjmass->RedrawAxis();
  c_jjmass->SaveAs("jj-mass.gif");


// ################# How many good W+jj are there ?
  cout << "Number of observed events in data = " <<  h_jjmass->Integral() << endl;
  cout << "Number of W+jj events predicted from MC = " <<  h_jjmass_WJets->Integral() << endl;
  cout << "Number of WW events predicted from MC = " <<  h_jjmass_WW_enujj->Integral()/10. << endl;





// // ############## Print out how many events survive W mass cut:70--90
//   h_jjmass->GetXaxis()->SetRangeUser(60., 100.);
//   cout << "Data events in 60--100 GeV mass window = " << h_jjmass->Integral() << endl;
//   h_jjmass_WJets->GetXaxis()->SetRangeUser(60., 100.);
//   cout << "W+jets events in 60--100 GeV mass window = " << h_jjmass_WJets->Integral() << endl;
//   h_jjmass_WW_enujj->GetXaxis()->SetRangeUser(60., 100.);
//   cout << "Signal events in 60--100 GeV mass window = " << h_jjmass_WW_enujj->Integral() << endl;  



//   TCanvas* c_bbmass = new TCanvas("c_bbmass","bb mass", 500, 500);
//   TH1D* h_bbmass = new TH1D("h_bbmass","", jjmass_NBINS, jjmass_MIN, jjmass_MAX);
//   tree->Draw(TString(mjj)+TString(">>h_bbmass"), goodW && twojets && twoBJets && centrality && dphicut, "e");
//   sprintf(temp, "Events / %.1f GeV", (jjmass_MAX-jjmass_MIN)/jjmass_NBINS);
//   h_bbmass->GetYaxis()->SetTitle(temp);
//   c_bbmass->SaveAs("bb-mass.gif");


//   TCanvas* c_jjdeta = new TCanvas("c_jjdeta","jj #Delta#eta", 500, 500);
//   TH1D* h_jjdeta = new TH1D("h_jjdeta","", jjdeta_NBINS, jjdeta_MIN, jjdeta_MAX);
//   tree->Draw(TString(jjdeta)+TString(">>h_jjdeta"), goodW, "goff");
//   h_jjdeta->GetXaxis()->SetTitle("#Delta#eta of leading two jets");
//   sprintf(temp, "Events / %.1f", (jjdeta_MAX-jjdeta_MIN)/jjdeta_NBINS);
//   h_jjdeta->GetYaxis()->SetTitle(temp);
//   TH1D* h_jjdeta_WJets = new TH1D("h_jjdeta_WJets","", 4*jjdeta_NBINS, jjdeta_MIN, jjdeta_MAX);
//   h_jjdeta_WJets->SetLineColor(4);
//   h_jjdeta_WJets->SetLineWidth(2);
//   treeMC_WJets->Draw( TString(jjdeta)+TString(">>h_jjdeta_WJets"), goodW, "goff");
//   h_jjdeta_WJets->Smooth(1000);
//   h_jjdeta_WJets->Scale( 4*WJets_scale );
//   TH1D* h_jjdeta_WW_enujj = new TH1D("h_jjdeta_WW_enujj","", 10*jjdeta_NBINS, jjdeta_MIN, jjdeta_MAX);
//   h_jjdeta_WW_enujj->SetFillColor(kOrange);
//   TH1D* h_jjdeta_WZ_enujj = h_jjdeta_WW_enujj->Clone("h_jjdeta_WZ_enujj");
//   treeMC_WW_enujj->Draw( TString(jjdeta)+TString(">>h_jjdeta_WW_enujj"), goodW, "goff");
//   h_jjdeta_WW_enujj->Scale( 10*WW_scale );
//   treeMC_WZ_enujj->Draw( TString(jjdeta)+TString(">>h_jjdeta_WZ_enujj"), goodW, "goff");
//   h_jjdeta_WZ_enujj->Scale( 10*WZ_scale );
//   h_jjdeta_WW_enujj->Add(h_jjdeta_WZ_enujj);
//   h_jjdeta_WW_enujj->Smooth(1000);
//   // h_jjdeta->SetMaximum( 1.15 * h_jjdeta_WJets->GetMaximum() );
//   h_jjdeta->Draw("e");
//   h_jjdeta_WJets->Draw("same");
//   h_jjdeta_WW_enujj->Draw("same");
//   TArrow* ar4 = new TArrow(-1.3, 5, -1.3, 60,0.03,"<|");
//   ar4->SetLineColor(2);
//   ar4->SetFillColor(2);
//   ar4->Draw();
//   TArrow* ar42 = new TArrow(1.3, 5, 1.3, 60, 0.03,"<|");
//   ar42->SetLineColor(2);
//   ar42->SetFillColor(2);
//   ar42->Draw();
//   c_jjdeta->SaveAs("jj-deta.gif");

//   int integral = h_jjdeta->Integral();
//   cout << "#### Number of W+jj events = " << integral << endl;





  TCanvas* c_jjcostheta = new TCanvas("c_jjcostheta","jj costhetastar", 500, 500);
  TH1D* h_jjcostheta = new TH1D("h_jjcostheta","", 20, -1, 1);
  h_jjcostheta->SetMinimum(0);
  tree->Draw(TString(jjcostheta)+TString(">>h_jjcostheta"), goodW && twojets && dphicut, "goff");
  h_jjcostheta->GetXaxis()->SetTitle("cos#theta* of leading two jets");
  h_jjcostheta->GetYaxis()->SetTitle("Events / 0.1");
  TH1D* h2_jjcostheta = new TH1D("h2_jjcostheta","", 20, -1, 1);
  h2_jjcostheta->SetLineColor(2);
  h2_jjcostheta->SetMarkerColor(2);
  tree->Draw(TString(jjcostheta)+TString(">>h2_jjcostheta"), goodW && twojets && centrality && dphicut, "goff");

  TH1D* h_jjcostheta_WJets = new TH1D("h_jjcostheta_WJets","", 4*20, -1, 1);
  h_jjcostheta_WJets->SetLineColor(4);
  h_jjcostheta_WJets->SetLineWidth(2);
  treeMC_WJets->Draw( TString(jjcostheta)+TString(">>h_jjcostheta_WJets"), goodW && twojets && dphicut, "goff");
  h_jjcostheta_WJets->Smooth(1000);
  h_jjcostheta_WJets->Scale( 4*WJets_scale );
  TH1D* h_jjcostheta_WW_enujj = new TH1D("h_jjcostheta_WW_enujj","", 4*20, -1, 1);
  h_jjcostheta_WW_enujj->SetFillColor(kOrange);
  TH1D* h_jjcostheta_WZ_enujj = h_jjcostheta_WW_enujj->Clone("h_jjcostheta_WZ_enujj");
  treeMC_WW_enujj->Draw( TString(jjcostheta)+TString(">>h_jjcostheta_WW_enujj"), goodW && twojets && dphicut, "goff");
  h_jjcostheta_WW_enujj->Scale( 4*WW_scale );
  treeMC_WZ_enujj->Draw( TString(jjcostheta)+TString(">>h_jjcostheta_WZ_enujj"), goodW && twojets && dphicut, "goff");
  h_jjcostheta_WZ_enujj->Scale( 4*WZ_scale );
  h_jjcostheta_WW_enujj->Add(h_jjcostheta_WZ_enujj);
  h_jjcostheta_WW_enujj->Smooth(1000);
  h_jjcostheta->SetMaximum( 2. * h_jjcostheta->GetMaximum() );
  h_jjcostheta->Draw("e");
  h2_jjcostheta->Draw("esame");
  h_jjcostheta_WJets->Draw("same");
  h_jjcostheta_WW_enujj->Draw("same");
  c_jjcostheta->SaveAs("jj-costhetastar.gif");





  TCanvas* c_jjdphi = new TCanvas("c_jjdphi","jj #Delta#phi", 500, 500);
  TH1D* h_jjdphi = new TH1D("h_jjdphi","", jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
  tree->Draw(TString(jjdphi)+TString(">>h_jjdphi"), goodW && twojets && centrality && dphicut1M2M, "goff");
  h_jjdphi->GetXaxis()->SetTitle("#Delta#phi of leading two jets");
  sprintf(temp, "Events / %.1f", (jjdphi_MAX-jjdphi_MIN)/jjdphi_NBINS);
  h_jjdphi->GetYaxis()->SetTitle(temp);
  h_jjdphi->SetMinimum(3);
  TH1D* h_jjdphi_WJets = new TH1D("h_jjdphi_WJets","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
  h_jjdphi_WJets->SetLineColor(4);
  h_jjdphi_WJets->SetLineWidth(2);
  treeMC_WJets->Draw( TString(jjdphi)+TString(">>h_jjdphi_WJets"), goodW && twojets && centrality && dphicut1M2M, "goff");
  h_jjdphi_WJets->Smooth(1000);
  h_jjdphi_WJets->Scale( 4*WJets_scale );
  TH1D* h_jjdphi_WW_enujj = new TH1D("h_jjdphi_WW_enujj","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
  h_jjdphi_WW_enujj->SetFillColor(kOrange);
  TH1D* h_jjdphi_WZ_enujj = h_jjdphi_WW_enujj->Clone("h_jjdphi_WZ_enujj");
  treeMC_WW_enujj->Draw( TString(jjdphi)+TString(">>h_jjdphi_WW_enujj"), goodW && twojets && centrality && dphicut1M2M, "goff");
  h_jjdphi_WW_enujj->Scale( 4*WW_scale );
  treeMC_WZ_enujj->Draw( TString(jjdphi)+TString(">>h_jjdphi_WZ_enujj"), goodW && twojets && centrality && dphicut1M2M, "goff");
  h_jjdphi_WZ_enujj->Scale( 4*WZ_scale );
  h_jjdphi_WW_enujj->Add(h_jjdphi_WZ_enujj);
  h_jjdphi_WW_enujj->Smooth(1000);
  // h_jjdphi->SetMaximum( 10 * h_jjdphi_WJets->GetMaximum() );
  h_jjdphi->SetMinimum(0);
  h_jjdphi->Draw("e");
  h_jjdphi_WJets->Draw("same");
  h_jjdphi_WW_enujj->Draw("same");
  // c_jjdphi->SetLogy();
  c_jjdphi->RedrawAxis();
  TArrow* ar5 = new TArrow(1.5, 5, 1.5, 80,0.03,"<|");
  ar5->SetLineColor(2);
  ar5->SetFillColor(2);
  ar5->Draw();
  c_jjdphi->SaveAs("jj-dphi.gif");







  TCanvas* c_j1dphiMET = new TCanvas("c_j1dphiMET","dPhi between leading jet and MET", 500, 500);
  TH1D* h_j1dphiMET = new TH1D("h_j1dphiMET","", jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
  tree->Draw("abs(JetPFCor_dphiMET[0])>>h_j1dphiMET", goodW && twojets && centrality && dphicut122M, "goff");
  h_j1dphiMET->GetXaxis()->SetTitle("#Delta#phi between leading jet and MET");
  sprintf(temp, "Events / %.1f", (jjdphi_MAX-jjdphi_MIN)/jjdphi_NBINS);
  h_j1dphiMET->GetYaxis()->SetTitle(temp);
  h_j1dphiMET->SetMinimum(3);
  TH1D* h_j1dphiMET_WJets = new TH1D("h_j1dphiMET_WJets","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
  h_j1dphiMET_WJets->SetLineColor(4);
  h_j1dphiMET_WJets->SetLineWidth(2);
  treeMC_WJets->Draw( "abs(JetPFCor_dphiMET[0])>>h_j1dphiMET_WJets", goodW && twojets && centrality && dphicut122M, "goff");
  h_j1dphiMET_WJets->Smooth(1000);
  h_j1dphiMET_WJets->Scale( 4*WJets_scale );
  TH1D* h_j1dphiMET_WW_enujj = new TH1D("h_j1dphiMET_WW_enujj","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
  h_j1dphiMET_WW_enujj->SetFillColor(kOrange);
  TH1D* h_j1dphiMET_WZ_enujj = h_j1dphiMET_WW_enujj->Clone("h_j1dphiMET_WZ_enujj");
  treeMC_WW_enujj->Draw( "abs(JetPFCor_dphiMET[0])>>h_j1dphiMET_WW_enujj", goodW && twojets && centrality && dphicut122M, "goff");
  h_j1dphiMET_WW_enujj->Scale( 4*WW_scale );

  treeMC_WZ_enujj->Draw( "abs(JetPFCor_dphiMET[0])>>h_j1dphiMET_WZ_enujj", goodW && twojets && centrality && dphicut122M, "goff");
  h_j1dphiMET_WZ_enujj->Scale( 4*WZ_scale );
  h_j1dphiMET_WW_enujj->Add(h_j1dphiMET_WZ_enujj);
  h_j1dphiMET_WW_enujj->Smooth(1000);
  // h_j1dphiMET->SetMaximum( 10 * h_j1dphiMET_WJets->GetMaximum() );
  h_j1dphiMET->SetMinimum(0);
  h_j1dphiMET->Draw("e");
  h_j1dphiMET_WJets->Draw("same");
  h_j1dphiMET_WW_enujj->Draw("same");
  // c_j1dphiMET->SetLogy();
  c_j1dphiMET->RedrawAxis();
  TArrow* ar6 = new TArrow(0.6, 5, 0.6, 50,0.03,"<|");
  ar6->SetLineColor(2);
  ar6->SetFillColor(2);
  ar6->Draw();
  c_j1dphiMET->SaveAs("j1dphiMET.gif");






//   TCanvas* c_j2dphiMET = new TCanvas("c_j2dphiMET","dPhi between 2nd jet and MET", 500, 500);
//   TH1D* h_j2dphiMET = new TH1D("h_j2dphiMET","", jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
//   tree->Draw("abs(JetPFCor_dphiMET[1])>>h_j2dphiMET", goodW && twojets && centrality && dphicut121M, "goff");
//   h_j2dphiMET->GetXaxis()->SetTitle("#Delta#phi between 2nd jet and MET");
//   sprintf(temp, "Events / %.1f", (jjdphi_MAX-jjdphi_MIN)/jjdphi_NBINS);
//   h_j2dphiMET->GetYaxis()->SetTitle(temp);
//   h_j2dphiMET->SetMinimum(1);
//   TH1D* h_j2dphiMET_WJets = new TH1D("h_j2dphiMET_WJets","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
//   h_j2dphiMET_WJets->SetLineColor(4);
//   h_j2dphiMET_WJets->SetLineWidth(2);
//   treeMC_WJets->Draw( "abs(JetPFCor_dphiMET[1])>>h_j2dphiMET_WJets", goodW && twojets && centrality && dphicut121M, "goff");
//   h_j2dphiMET_WJets->Smooth(1000);
//   h_j2dphiMET_WJets->Scale( 4*WJets_scale );
//   TH1D* h_j2dphiMET_WW_enujj = new TH1D("h_j2dphiMET_WW_enujj","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
//   h_j2dphiMET_WW_enujj->SetFillColor(kOrange);
//   treeMC_WW_enujj->Draw( "abs(JetPFCor_dphiMET[1])>>h_j2dphiMET_WW_enujj", goodW && twojets && centrality && dphicut121M, "goff");
//   h_j2dphiMET_WW_enujj->Smooth(1000);
//   h_j2dphiMET_WW_enujj->Scale( 4*WW_scale );
//   h_j2dphiMET->SetMaximum( 10 * h_j2dphiMET_WJets->GetMaximum() );
//   h_j2dphiMET->Draw("e");
//   h_j2dphiMET_WJets->Draw("same");
//   h_j2dphiMET_WW_enujj->Draw("same");
//   c_j2dphiMET->SetLogy();
//   c_j2dphiMET->RedrawAxis();
// //   TArrow* ar7 = new TArrow(2.5, 5, 2.5, 80,0.03,"<|");
// //   ar7->SetLineColor(2);
// //   ar7->SetFillColor(2);
// //   ar7->Draw();
//   c_j2dphiMET->SaveAs("j2dphiMET.gif");









//   TCanvas* c_edphiMET = new TCanvas("c_edphiMET","dPhi between electron and MET", 500, 500);
//   TH1D* h_edphiMET = new TH1D("h_edphiMET","", jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
//   tree->Draw( TString(eMETdphi)+TString(">>h_edphiMET"), goodW && twojets && centrality && dphicut, "goff");
//   h_edphiMET->GetXaxis()->SetTitle("#Delta#phi between electron and MET");
//   sprintf(temp, "Events / %.1f", (jjdphi_MAX-jjdphi_MIN)/jjdphi_NBINS);
//   h_edphiMET->GetYaxis()->SetTitle(temp);
//   h_edphiMET->SetMinimum(0.5);
//   TH1D* h_edphiMET_WJets = new TH1D("h_edphiMET_WJets","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
//   h_edphiMET_WJets->SetLineColor(4);
//   h_edphiMET_WJets->SetLineWidth(2);
//   treeMC_WJets->Draw( TString(eMETdphi)+TString(">>h_edphiMET_WJets"), goodW && twojets && centrality && dphicut, "goff");
//   h_edphiMET_WJets->Smooth(1000);
//   h_edphiMET_WJets->Scale( 4*WJets_scale );
//   TH1D* h_edphiMET_WW_enujj = new TH1D("h_edphiMET_WW_enujj","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
//   h_edphiMET_WW_enujj->SetFillColor(kOrange);
//   treeMC_WW_enujj->Draw( TString(eMETdphi)+TString(">>h_edphiMET_WW_enujj"), goodW && twojets && centrality && dphicut, "goff");
//   h_edphiMET_WW_enujj->Smooth(1000);
//   h_edphiMET_WW_enujj->Scale( 4*WW_scale );
//   h_edphiMET->SetMaximum( 10 * h_edphiMET_WJets->GetMaximum() );
//   h_edphiMET->Draw("e");
//   h_edphiMET_WJets->Draw("same");
//   h_edphiMET_WW_enujj->Draw("same");
//   c_edphiMET->SetLogy();
//   c_edphiMET->RedrawAxis();
// //   TArrow* ar5 = new TArrow(1.5, 5, 1.5, 80,0.03,"<|");
// //   ar5->SetLineColor(2);
// //   ar5->SetFillColor(2);
// //   ar5->Draw();
//   c_edphiMET->SaveAs("edphiMET.gif");





  TCanvas* c_dphiWjj = new TCanvas("c_dphiWjj","dPhi between two W's", 500, 500);
  TH1D* h_dphiWjj = new TH1D("h_dphiWjj","", jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
  tree->Draw( TString(dphiWjj)+TString(">>h_dphiWjj"), goodW && twojets && centrality && dphicut, "goff");
  h_dphiWjj->GetXaxis()->SetTitle("#Delta#phi between two W's");
  sprintf(temp, "Events / %.1f", (jjdphi_MAX-jjdphi_MIN)/jjdphi_NBINS);
  h_dphiWjj->GetYaxis()->SetTitle(temp);
  h_dphiWjj->SetMinimum(0.5);
  TH1D* h_dphiWjj_WJets = new TH1D("h_dphiWjj_WJets","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
  h_dphiWjj_WJets->SetLineColor(4);
  h_dphiWjj_WJets->SetLineWidth(2);
  treeMC_WJets->Draw( TString(dphiWjj)+TString(">>h_dphiWjj_WJets"), goodW && twojets && centrality && dphicut, "goff");
  h_dphiWjj_WJets->Smooth(1000);
  h_dphiWjj_WJets->Scale( 4*WJets_scale );
  TH1D* h_dphiWjj_WW_enujj = new TH1D("h_dphiWjj_WW_enujj","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
  h_dphiWjj_WW_enujj->SetFillColor(kOrange);
  TH1D* h_dphiWjj_WZ_enujj = h_dphiWjj_WW_enujj->Clone("h_dphiWjj_WZ_enujj");
  treeMC_WW_enujj->Draw( TString(dphiWjj)+TString(">>h_dphiWjj_WW_enujj"), goodW && twojets && centrality && dphicut, "goff");
  h_dphiWjj_WW_enujj->Scale( 4*WW_scale );
  treeMC_WZ_enujj->Draw( TString(dphiWjj)+TString(">>h_dphiWjj_WZ_enujj"), goodW && twojets && centrality && dphicut, "goff");
  h_dphiWjj_WZ_enujj->Scale( 4*WZ_scale );
  h_dphiWjj_WW_enujj->Add(h_dphiWjj_WZ_enujj);
  h_dphiWjj_WW_enujj->Smooth(1000);
  h_dphiWjj->SetMaximum( 1.3 * h_dphiWjj->GetMaximum() );
  h_dphiWjj->GetXaxis()->SetRangeUser(1.5,3.2);
  h_dphiWjj->SetMinimum(0);
  h_dphiWjj->Draw("e");
  h_dphiWjj_WJets->Draw("same");
  h_dphiWjj_WW_enujj->Draw("same");
  // c_dphiWjj->SetLogy();
  c_dphiWjj->RedrawAxis();
  TArrow* ar5 = new TArrow(2.84, 5, 2.84, 80,0.03,"<|");
  ar5->SetLineColor(2);
  ar5->SetFillColor(2);
  ar5->Draw();
  c_dphiWjj->SaveAs("dphiWjj.gif");





//   TCanvas* c_ej1dphi = new TCanvas("c_ej1dphi","dPhi between electron and MET", 500, 500);
//   TH1D* h_ej1dphi = new TH1D("h_ej1dphi","", jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
//   tree->Draw( TString(ej1dphi)+TString(">>h_ej1dphi"), goodW && twojets && centrality && dphicut, "goff");
//   h_ej1dphi->GetXaxis()->SetTitle("#Delta#phi between electron and MET");
//   sprintf(temp, "Events / %.1f", (jjdphi_MAX-jjdphi_MIN)/jjdphi_NBINS);
//   h_ej1dphi->GetYaxis()->SetTitle(temp);
//   h_ej1dphi->SetMinimum(0.5);
//   TH1D* h_ej1dphi_WJets = new TH1D("h_ej1dphi_WJets","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
//   h_ej1dphi_WJets->SetLineColor(4);
//   h_ej1dphi_WJets->SetLineWidth(2);
//   treeMC_WJets->Draw( TString(ej1dphi)+TString(">>h_ej1dphi_WJets"), goodW && twojets && centrality && dphicut, "goff");
//   h_ej1dphi_WJets->Smooth(1000);
//   h_ej1dphi_WJets->Scale( 4*WJets_scale );
//   TH1D* h_ej1dphi_WW_enujj = new TH1D("h_ej1dphi_WW_enujj","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
//   h_ej1dphi_WW_enujj->SetFillColor(kOrange);
//   treeMC_WW_enujj->Draw( TString(ej1dphi)+TString(">>h_ej1dphi_WW_enujj"), goodW && twojets && centrality && dphicut, "goff");
//   h_ej1dphi_WW_enujj->Smooth(1000);
//   h_ej1dphi_WW_enujj->Scale( 4*WW_scale );
//   h_ej1dphi->SetMaximum( 10 * h_ej1dphi_WJets->GetMaximum() );
//   h_ej1dphi->Draw("e");
//   h_ej1dphi_WJets->Draw("same");
//   h_ej1dphi_WW_enujj->Draw("same");
//   c_ej1dphi->SetLogy();
//   c_ej1dphi->RedrawAxis();
// //   TArrow* ar5 = new TArrow(1.5, 5, 1.5, 80,0.03,"<|");
// //   ar5->SetLineColor(2);
// //   ar5->SetFillColor(2);
// //   ar5->Draw();
//   c_ej1dphi->SaveAs("ej1dphi.gif");






//   TCanvas* c_ej2dphi = new TCanvas("c_ej2dphi","dPhi between electron and MET", 500, 500);
//   TH1D* h_ej2dphi = new TH1D("h_ej2dphi","", jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
//   tree->Draw( TString(ej2dphi)+TString(">>h_ej2dphi"), goodW && twojets && centrality && dphicut, "goff");
//   h_ej2dphi->GetXaxis()->SetTitle("#Delta#phi between electron and MET");
//   sprintf(temp, "Events / %.1f", (jjdphi_MAX-jjdphi_MIN)/jjdphi_NBINS);
//   h_ej2dphi->GetYaxis()->SetTitle(temp);
//   h_ej2dphi->SetMinimum(0.5);
//   TH1D* h_ej2dphi_WJets = new TH1D("h_ej2dphi_WJets","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
//   h_ej2dphi_WJets->SetLineColor(4);
//   h_ej2dphi_WJets->SetLineWidth(2);
//   treeMC_WJets->Draw( TString(ej2dphi)+TString(">>h_ej2dphi_WJets"), goodW && twojets && centrality && dphicut, "goff");
//   h_ej2dphi_WJets->Smooth(1000);
//   h_ej2dphi_WJets->Scale( 4*WJets_scale );
//   TH1D* h_ej2dphi_WW_enujj = new TH1D("h_ej2dphi_WW_enujj","", 4*jjdphi_NBINS, jjdphi_MIN, jjdphi_MAX);
//   h_ej2dphi_WW_enujj->SetFillColor(kOrange);
//   treeMC_WW_enujj->Draw( TString(ej2dphi)+TString(">>h_ej2dphi_WW_enujj"), goodW && twojets && centrality && dphicut, "goff");
//   h_ej2dphi_WW_enujj->Smooth(1000);
//   h_ej2dphi_WW_enujj->Scale( 4*WW_scale );
//   h_ej2dphi->SetMaximum( 10 * h_ej2dphi_WJets->GetMaximum() );
//   h_ej2dphi->Draw("e");
//   h_ej2dphi_WJets->Draw("same");
//   h_ej2dphi_WW_enujj->Draw("same");
//   c_ej2dphi->SetLogy();
//   c_ej2dphi->RedrawAxis();
// //   TArrow* ar5 = new TArrow(1.5, 5, 1.5, 80,0.03,"<|");
// //   ar5->SetLineColor(2);
// //   ar5->SetFillColor(2);
// //   ar5->Draw();
//   c_ej2dphi->SaveAs("ej2dphi.gif");







//   TCanvas* c_numTags = new TCanvas("c_numTags","number of b-tags", 500, 500);
//   TH1D* h_numTags = new TH1D("h_numTags","", 3, -0.5, 2.5);
//   tree->Draw("numPFCorJetBTags>>h_numTags", goodW && twojets && centrality && dphicut, "goff");
//   h_numTags->GetXaxis()->SetTitle("Number of B-tags");
//   h_numTags->SetMinimum(0.6);
//   TH1D* h_numTags_WJets = new TH1D("h_numTags_WJets","", 3, -0.5, 2.5);
//   h_numTags_WJets->SetLineColor(4);
//   h_numTags_WJets->SetLineWidth(2);
//   treeMC_WJets->Draw("numPFCorJetBTags>>h_numTags_WJets", goodW && twojets && centrality && dphicut, "goff");
//   h_numTags_WJets->Scale( WJets_scale );
//   TH1D* h_numTags_WW_enujj = new TH1D("h_numTags_WW_enujj","", 3, -0.5, 2.5);
//   h_numTags_WW_enujj->SetFillColor(kOrange);
//   treeMC_WW_enujj->Draw("numPFCorJetBTags>>h_numTags_WW_enujj", goodW && twojets && centrality && dphicut, "goff");
//   h_numTags_WW_enujj->Scale( WW_scale );
//   h_numTags->SetMaximum( 10 * h_numTags_WJets->GetMaximum() );
//   h_numTags->Draw("e");
//   h_numTags_WJets->Draw("same");
//   h_numTags_WW_enujj->Draw("same");
//   c_numTags->SetLogy();
//   c_numTags->SaveAs("nBTags.gif");

// // ################# Print the three bin contents
//   h_numTags->Print("all");


//   tdrStyle->SetPadRightMargin(0.13);
//   TCanvas* c_mTVsMjj = new TCanvas("c_mTVsMjj","W mT vs mjj", 500, 500);
//   TH2D* h_mTVsMjj = new TH2D("h_mTVsMjj","", 30, 0., 300.,  20, 40., 200.);
//   h_mTVsMjj->GetXaxis()->SetTitle("m_{jj}    ");
//   h_mTVsMjj->GetXaxis()->SetNdivisions(505);
//   h_mTVsMjj->GetYaxis()->SetTitle("    m_{T}");
//   h_mTVsMjj->GetYaxis()->SetNdivisions(505);
//   sprintf(temp, "W_mt:%s>>h_mTVsMjj", mjj);
//   gStyle->SetPalette(1);
//   tree->Draw(temp, goodW && twojets && noBJets && centrality && dphicut, "lego2z");
//   // c_mTVsMjj->RedrawAxis();
//   c_mTVsMjj->SaveAs("mTVsMjj-lego.gif");





//  tdrStyle->SetPadRightMargin(0.13);
//   TCanvas* c_detaVsdphi = new TCanvas("c_detaVsdphi","W mT vs mjj", 500, 500);
//   TH2D* h_detaVsdphi = new TH2D("h_detaVsdphi","", 10, 0., 3.14159265358979312,  20, -5., 5.);
//   h_detaVsdphi->SetMarkerStyle(6);
//   h_detaVsdphi->GetXaxis()->SetTitle("Dijet #Delta#phi    ");
//   h_detaVsdphi->GetXaxis()->SetNdivisions(505);
//   h_detaVsdphi->GetYaxis()->SetTitle("Dijet #Delta#eta");
//   h_detaVsdphi->GetYaxis()->SetNdivisions(505);
//   sprintf(temp, "%s:%s>>h_detaVsdphi", jjdeta, jjdphi);
//   tree->Draw(temp, goodW && noBJets);
//   c_detaVsdphi->SaveAs("detaVsdphi-lego.gif");




*/




/*
  TCanvas* c_jjmass_dphicut = new TCanvas("c_jjmass_dphicut","jj mass", 500, 500);
  TH1D* h_jjmass_dphicut = new TH1D("h_jjmass_dphicut","", jjmass_NBINS, jjmass_MIN, jjmass_MAX);
  TH1D* h_jjmass_dphicut_detacut = new TH1D("h_jjmass_dphicut_detacut","", jjmass_NBINS, jjmass_MIN, jjmass_MAX);
  h_jjmass_dphicut_detacut->SetFillColor(5);
  TH1D* h_jjmass_dphicut_detacut2 = new TH1D("h_jjmass_dphicut_detacut2","", jjmass_NBINS, jjmass_MIN, jjmass_MAX);
  h_jjmass_dphicut_detacut2->SetFillColor(7);
  TH1D* h_jjmass_dphicut_detacut3 = new TH1D("h_jjmass_dphicut_detacut3","", jjmass_NBINS, jjmass_MIN, jjmass_MAX);
  h_jjmass_dphicut_detacut3->SetFillColor(4);
  TH1D* h_jjmass_dphicut_detacut4 = new TH1D("h_jjmass_dphicut_detacut4","", jjmass_NBINS, jjmass_MIN, jjmass_MAX);
  h_jjmass_dphicut_detacut4->SetFillColor(2);
  sprintf(temp, "%s>>h_jjmass_dphicut", mjj);
  tree->Draw( temp, goodW, "e");
  gPad->Update();

  TPaveStats *st = (TPaveStats*)h_jjmass_dphicut->FindObject("stats");
  st->SetName("stats1");
  st->SetX1NDC(0.66); //new x start position
  st->SetX2NDC(0.81); //new x end position
  h_jjmass_dphicut->GetListOfFunctions()->Add(st);
  sprintf(temp, "%s>>h_jjmass_dphicut_detacut", mjj);
  tree->Draw( temp, goodW && centrality && dphicut, "sames");
  gPad->Update();
  st = (TPaveStats*)h_jjmass_dphicut_detacut->FindObject("stats");
  st->SetFillColor(5);
  h_jjmass_dphicut_detacut->GetListOfFunctions()->Add(st);
  sprintf(temp, "%s>>h_jjmass_dphicut_detacut2", mjj);
  tree->Draw( temp, goodW && centrality2 && dphicut, "sames");
  gPad->Update();
  st = (TPaveStats*)h_jjmass_dphicut_detacut2->FindObject("stats");
  st->SetName("stats2");
  st->SetX1NDC(0.66); //new x start position
  st->SetX2NDC(0.81); //new x end position
  st->SetY1NDC(0.68); //new y start position
  st->SetY2NDC(0.83); //new y end position
  st->SetFillColor(7);
  h_jjmass_dphicut_detacut2->GetListOfFunctions()->Add(st);
  sprintf(temp, "%s>>h_jjmass_dphicut_detacut3", mjj);
  tree->Draw( temp, goodW && centrality3 && dphicut, "sames");
  gPad->Update();
  st = (TPaveStats*)h_jjmass_dphicut_detacut3->FindObject("stats");
  st->SetFillColor(4);
  st->SetName("stats3");
  st->SetY1NDC(0.68); //new y start position
  st->SetY2NDC(0.83); //new y end position
  h_jjmass_dphicut_detacut3->GetListOfFunctions()->Add(st);
  sprintf(temp, "%s>>h_jjmass_dphicut_detacut4", mjj);
  tree->Draw( temp, goodW && noBJets && centrality3 && dphicut, "sames");
  gPad->Update();
  st = (TPaveStats*)h_jjmass_dphicut_detacut4->FindObject("stats");
  st->SetFillColor(2);
  st->SetName("stats3");
  st->SetY1NDC(0.52); //new y start position
  st->SetY2NDC(0.67); //new y end position
  h_jjmass_dphicut_detacut4->GetListOfFunctions()->Add(st);

  h_jjmass_dphicut->GetXaxis()->SetTitle("m_{jj} of leading two jets [GeV]");
  sprintf(temp, "Events / %.1f GeV", (jjmass_MAX-jjmass_MIN)/jjmass_NBINS);
  h_jjmass_dphicut->GetYaxis()->SetTitle(temp);
  h_jjmass_dphicut->GetXaxis()->SetNdivisions(505);
  c_jjmass_dphicut->SaveAs("jj-mass-dphicut.gif");
// ############## Print out how many events survive W mass cut:70--90
  h_jjmass_dphicut_detacut4->GetXaxis()->SetRangeUser(70., 90.);
  cout << "events in 70--90 GeV mass window = " << h_jjmass_dphicut_detacut4->Integral() << endl;
  
*/
}


////CMS Preliminary label and lumu
void cmsPrelim()
{
   const float LUMINOSITY = intLUMI;
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);

  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.96,"#sqrt{s} = 7 TeV");
  if (LUMINOSITY > 0.) {
    latex.SetTextAlign(31); // align right
    latex.DrawLatex(0.5,0.84,Form("#int #font[12]{L} dt = %.0f pb^{-1}",
    LUMINOSITY));
  }
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.96,"CMS preliminary 2011");
}
