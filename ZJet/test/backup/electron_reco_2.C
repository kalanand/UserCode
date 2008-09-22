{
///////////////////////////////////
// configure input ROOT filenames
///////////////////////////////////

bool useLocalFile = false;

///////////////////////////////////
// configure which plots to make 
///////////////////////////////////
const bool default = false;

bool plotEmECALIsolation     = default;
bool plotEmHCALIsolation     = default;
bool plotEmTrackPTIsolation  = default;
bool plotEmTrackNumIsolation = default;
bool plotEPECALIsolation     = default;
bool plotEPHCALIsolation     = default;
bool plotEPTrackPTIsolation  = default;
bool plotEPTrackNumIsolation = default;
bool plotZmass_Isolation     = default;

bool plotZmass_idPTDR        = default;
bool plotZmass_BarrelEndcap  = default;
bool plotEoverP              = default;
bool plotBremFraction        = default;
bool plotClassification      = default;
bool plotZmass_category      = default;
bool plotHoverE              = default;
bool plotZpt                 = default;
bool plotZpt_masscut         = true;
bool plotEPpt                = default;
bool plotEMpt                = default;
bool plotEMptAfterMassCut    = default;
bool plotEMptCategory        = default;
bool plotZmass               = default;



/******** Read input ROOT files ************/
/*******************************************/

TChain* t = new TChain("ZJet");
if( useLocalFile == true ) {
  t->Add("analysis_zjet.root");
}
else {
 t->Add("/uscms_data/d1/kalanand/trash/CSA07-ZeeJets_Pt_*.root");
//  t->Add("/uscms_data/d1/cmsjtmet/kalanand/CSA07-ZeeJets-Ntuple/*.root");
}
/***********************************/






/***********************************/
/********** Start making plots *******/
/***********************************/


// plot ECAL Isolation
if( plotEmECALIsolation == true ) {
  TH1F EmECALIsolation10("EmECALIsolation10","", 200, 0.0, 0.4);
  EmECALIsolation10.Sumw2();
  EmECALIsolation10.SetLineWidth(2);
  TAxis* EmECALIsolationx = EmECALIsolation10.GetXaxis();
  TAxis* EmECALIsolationy = EmECALIsolation10.GetYaxis();
  EmECALIsolationx->SetTitle("ECAL Isolation     ");
  EmECALIsolationy->SetTitle("Events / 0.002         ");
  EmECALIsolationy->SetTitleOffset(1.2);

  TH1F EmECALIsolation20("EmECALIsolation20","", 200, 0.0, 0.4);
  EmECALIsolation20.Sumw2();
  EmECALIsolation20.SetLineColor(2);
  EmECALIsolation20.SetLineWidth(2);
  EmECALIsolation20.SetMarkerColor(2);

  TH1F EmECALIsolation30("EmECALIsolation30","", 200, 0.0, 0.4);
  EmECALIsolation30.Sumw2();
  EmECALIsolation30.SetLineColor(3);
  EmECALIsolation30.SetLineWidth(2);
  EmECALIsolation30.SetMarkerColor(3);

  TH1F EmECALIsolation40("EmECALIsolation40","", 200, 0.0, 0.4);
  EmECALIsolation40.Sumw2();
  EmECALIsolation40.SetLineColor(4);
  EmECALIsolation40.SetLineWidth(2);
  EmECALIsolation40.SetMarkerColor(4);

  TH1F EmECALIsolation50("EmECALIsolation50","", 200, 0.0, 0.4);
  EmECALIsolation50.Sumw2();
  EmECALIsolation50.SetLineColor(6);
  EmECALIsolation50.SetLineWidth(2);
  EmECALIsolation50.SetMarkerColor(6);

  t->Draw("(eMinus_ecaliso_10-eMinus_sc_e)/eMinus_sc_e>>EmECALIsolation10",
	  "eMinus_ecaliso_10>-1.0 && eMinus_sc_et>0.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("(eMinus_ecaliso_20-eMinus_sc_e)/eMinus_sc_e>>EmECALIsolation20",
	  "eMinus_ecaliso_20>-1.0 && eMinus_sc_et>0.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("(eMinus_ecaliso_30-eMinus_sc_e)/eMinus_sc_e>>EmECALIsolation30",
	  "eMinus_ecaliso_30>-1.0 && eMinus_sc_et>0.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("(eMinus_ecaliso_40-eMinus_sc_e)/eMinus_sc_e>>EmECALIsolation40",
	  "eMinus_ecaliso_40>-1.0 && eMinus_sc_et>0.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("(eMinus_ecaliso_50-eMinus_sc_e)/eMinus_sc_e>>EmECALIsolation50",
	  "eMinus_ecaliso_50>-1.0 && eMinus_sc_et>0.0 && mZee>60.0 && mZee<120.0","goff");

  TCanvas can2("can2","",880,600);
  gStyle->SetOptStat(0);
  //can2.SetLogy(1);
  can2.SetGridx(1);
  can2.SetGridy(1);
  EmECALIsolation10.Draw();
  EmECALIsolation10.Draw("HIST same");
  EmECALIsolation20.Draw("HIST same");
  EmECALIsolation30.Draw("HIST same");
  EmECALIsolation40.Draw("HIST same");
  EmECALIsolation50.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.6,0.85,0.85);
  leg_hist->AddEntry(&EmECALIsolation10,"Cone size 0.10","l");
  leg_hist->AddEntry(&EmECALIsolation20,"Cone size 0.20","l");
  leg_hist->AddEntry(&EmECALIsolation30,"Cone size 0.30","l");
  leg_hist->AddEntry(&EmECALIsolation40,"Cone size 0.40","l");
  leg_hist->AddEntry(&EmECALIsolation50,"Cone size 0.50","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  TString plotname = "eMinus-ECALIsolation";
  can2.SaveAs(plotname+TString(".eps"));
  can2.SaveAs(plotname+TString(".gif"));
  can2.SaveAs(plotname+TString(".root"));
}


// plot HCAL Isolation
if(plotEmHCALIsolation == true ) {
  TH1F EmHCALIsolation10("EmHCALIsolation10","", 400, -0.1, 0.1);
  EmHCALIsolation10.Sumw2();
  EmHCALIsolation10.SetLineWidth(2);
  TAxis* EmHCALIsolationx = EmHCALIsolation10.GetXaxis();
  TAxis* EmHCALIsolationy = EmHCALIsolation10.GetYaxis();
  EmHCALIsolationx->SetTitle("HCAL Isolation     ");
  EmHCALIsolationy->SetTitle("Events / 0.0005         ");
  EmHCALIsolationy->SetTitleOffset(1.25);

  TH1F EmHCALIsolation20("EmHCALIsolation20","", 400, -0.1, 0.1);
  EmHCALIsolation20.Sumw2();
  EmHCALIsolation20.SetLineColor(2);
  EmHCALIsolation20.SetLineWidth(2);
  EmHCALIsolation20.SetMarkerColor(2);

  TH1F EmHCALIsolation30("EmHCALIsolation30","", 400, -0.1, 0.1);
  EmHCALIsolation30.Sumw2();
  EmHCALIsolation30.SetLineColor(3);
  EmHCALIsolation30.SetLineWidth(2);
  EmHCALIsolation30.SetMarkerColor(3);

  TH1F EmHCALIsolation40("EmHCALIsolation40","", 400, -0.1, 0.1);
  EmHCALIsolation40.Sumw2();
  EmHCALIsolation40.SetLineColor(4);
  EmHCALIsolation40.SetLineWidth(2);
  EmHCALIsolation40.SetMarkerColor(4);

  TH1F EmHCALIsolation50("EmHCALIsolation50","", 400, -0.1, 0.1);
  EmHCALIsolation50.Sumw2();
  EmHCALIsolation50.SetLineColor(6);
  EmHCALIsolation50.SetLineWidth(2);
  EmHCALIsolation50.SetMarkerColor(6);

  t->Draw("eMinus_hcaliso_10>>EmHCALIsolation10",
	  "eMinus_hcaliso_10>-1.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("eMinus_hcaliso_20>>EmHCALIsolation20",
	  "eMinus_hcaliso_20>-1.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("eMinus_hcaliso_30>>EmHCALIsolation30",
	  "eMinus_hcaliso_30>-1.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("eMinus_hcaliso_40>>EmHCALIsolation40",
	  "eMinus_hcaliso_40>-1.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("eMinus_hcaliso_50>>EmHCALIsolation50",
	  "eMinus_hcaliso_50>-1.0 && mZee>60.0 && mZee<120.0","goff");

  TCanvas can21("can21","",880,600);
  gStyle->SetOptStat(0);
  EmHCALIsolation10.Draw();
  EmHCALIsolation10.Draw("HIST same");
  EmHCALIsolation20.Draw("HIST same");
  EmHCALIsolation30.Draw("HIST same");
  EmHCALIsolation40.Draw("HIST same");
  EmHCALIsolation50.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.6,0.85,0.85);
  leg_hist->AddEntry(&EmHCALIsolation10,"Cone size 0.10","l");
  leg_hist->AddEntry(&EmHCALIsolation20,"Cone size 0.20","l");
  leg_hist->AddEntry(&EmHCALIsolation30,"Cone size 0.30","l");
  leg_hist->AddEntry(&EmHCALIsolation40,"Cone size 0.40","l");
  leg_hist->AddEntry(&EmHCALIsolation50,"Cone size 0.50","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  //can21.SetLogy(1);
  can21.SetGridx(1);
  can21.SetGridy(1);
  TString plotname = "eMinus-HCALIsolation";
  can21.SaveAs(plotname+TString(".eps"));
  can21.SaveAs(plotname+TString(".gif"));
  can21.SaveAs(plotname+TString(".root"));
}



// plot Track pT Isolation
if( plotEmTrackPTIsolation == true ) {
  TH1F EmTrackPTIsolation10("EmTrackPTIsolation10","", 60, 0.0, 3.0);
  EmTrackPTIsolation10.Sumw2();
  EmTrackPTIsolation10.SetLineWidth(2);
  TAxis* EmTrackPTIsolationx = EmTrackPTIsolation10.GetXaxis();
  TAxis* EmTrackPTIsolationy = EmTrackPTIsolation10.GetYaxis();
  EmTrackPTIsolationx->SetTitle("Track p_{T} Isolation     ");
  EmTrackPTIsolationy->SetTitle("Events / 0.05         ");
  EmTrackPTIsolationy->SetTitleOffset(1.2);

  TH1F EmTrackPTIsolation20("EmTrackPTIsolation20","", 60, 0.0, 3.0);
  EmTrackPTIsolation20.Sumw2();
  EmTrackPTIsolation20.SetLineColor(2);
  EmTrackPTIsolation20.SetLineWidth(2);
  EmTrackPTIsolation20.SetMarkerColor(2);

  TH1F EmTrackPTIsolation30("EmTrackPTIsolation30","", 60, 0.0, 3.0);
  EmTrackPTIsolation30.Sumw2();
  EmTrackPTIsolation30.SetLineColor(3);
  EmTrackPTIsolation30.SetLineWidth(2);
  EmTrackPTIsolation30.SetMarkerColor(3);

  TH1F EmTrackPTIsolation40("EmTrackPTIsolation40","", 60, 0.0, 3.0);
  EmTrackPTIsolation40.Sumw2();
  EmTrackPTIsolation40.SetLineColor(4);
  EmTrackPTIsolation40.SetLineWidth(2);
  EmTrackPTIsolation40.SetMarkerColor(4);

  TH1F EmTrackPTIsolation50("EmTrackPTIsolation50","", 60, 0.0, 3.0);
  EmTrackPTIsolation50.Sumw2();
  EmTrackPTIsolation50.SetLineColor(6);
  EmTrackPTIsolation50.SetLineWidth(2);
  EmTrackPTIsolation50.SetMarkerColor(6);

  TString e1Cut15 = "eMinus_ptisoatecal_15>0.0 && eMinusPt>0.0 && mZee>60.0 && mZee<120.0";
  TString e1Cut25 = "eMinus_ptisoatecal_25>0.0 && eMinusPt>0.0 && mZee>60.0 && mZee<120.0";
  TString e1Cut30 = "eMinus_ptisoatecal_30>0.0 && eMinusPt>0.0 && mZee>60.0 && mZee<120.0";
  TString e1Cut40 = "eMinus_ptisoatecal_40>0.0 && eMinusPt>0.0 && mZee>60.0 && mZee<120.0";
  TString e1Cut50 = "eMinus_ptisoatecal_50>0.0 && eMinusPt>0.0 && mZee>60.0 && mZee<120.0";

  t->Draw("eMinus_ptisoatecal_15/eMinusPt>>EmTrackPTIsolation10",
	  e1Cut15,"goff");
  t->Draw("eMinus_ptisoatecal_25/eMinusPt>>EmTrackPTIsolation20",
	  e1Cut25,"goff");
  t->Draw("eMinus_ptisoatecal_30/eMinusPt>>EmTrackPTIsolation30",
	  e1Cut30,"goff");
  t->Draw("eMinus_ptisoatecal_40/eMinusPt>>EmTrackPTIsolation40",
	  e1Cut40,"goff");
  t->Draw("eMinus_ptisoatecal_50/eMinusPt>>EmTrackPTIsolation50",
	  e1Cut50,"goff");

  TCanvas can22("can22","",880,600);
  gStyle->SetOptStat(0);
  EmTrackPTIsolation10.SetMaximum(13000);
  EmTrackPTIsolation10.Draw();
  EmTrackPTIsolation20.Draw("HIST same");
  EmTrackPTIsolation30.Draw("HIST same");
  EmTrackPTIsolation40.Draw("HIST same");
  EmTrackPTIsolation50.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.6,0.85,0.85);
  leg_hist->AddEntry(&EmTrackPTIsolation10,"Cone size 0.15","l");
  leg_hist->AddEntry(&EmTrackPTIsolation20,"Cone size 0.25","l");
  leg_hist->AddEntry(&EmTrackPTIsolation30,"Cone size 0.30","l");
  leg_hist->AddEntry(&EmTrackPTIsolation40,"Cone size 0.40","l");
  leg_hist->AddEntry(&EmTrackPTIsolation50,"Cone size 0.50","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  can22.SetGridx(1);
  can22.SetGridy(1);
  TString plotname = "eMinus-TrackPTIsolation";
  can22.SaveAs(plotname+TString(".eps"));
  can22.SaveAs(plotname+TString(".gif"));
  can22.SaveAs(plotname+TString(".root"));
}



// plot Track nTrack Isolation
if( plotEmTrackNumIsolation == true ) {
  TH1F EmTrackNumIsolation10("EmTrackNumIsolation10","", 8, 0.0, 8.0);
  EmTrackNumIsolation10.Sumw2();
  EmTrackNumIsolation10.SetLineWidth(2);
  TAxis* EmTrackNumIsolationx = EmTrackNumIsolation10.GetXaxis();
  TAxis* EmTrackNumIsolationy = EmTrackNumIsolation10.GetYaxis();
  EmTrackNumIsolationx->SetTitle("Track n_{TRACK} Isolation     ");
  EmTrackNumIsolationy->SetTitle("Events         ");
  EmTrackNumIsolationy->SetTitleOffset(1.27);

  TH1F EmTrackNumIsolation20("EmTrackNumIsolation20","", 8, 0.0, 8.0);
  EmTrackNumIsolation20.Sumw2();
  EmTrackNumIsolation20.SetLineColor(2);
  EmTrackNumIsolation20.SetLineWidth(2);
  EmTrackNumIsolation20.SetMarkerColor(2);

  TH1F EmTrackNumIsolation30("EmTrackNumIsolation30","", 8, 0.0, 8.0);
  EmTrackNumIsolation30.Sumw2();
  EmTrackNumIsolation30.SetLineColor(3);
  EmTrackNumIsolation30.SetLineWidth(2);
  EmTrackNumIsolation30.SetMarkerColor(3);

  TH1F EmTrackNumIsolation40("EmTrackNumIsolation40","", 8, 0.0, 8.0);
  EmTrackNumIsolation40.Sumw2();
  EmTrackNumIsolation40.SetLineColor(4);
  EmTrackNumIsolation40.SetLineWidth(2);
  EmTrackNumIsolation40.SetMarkerColor(4);

  TH1F EmTrackNumIsolation50("EmTrackNumIsolation50","", 8, 0.0, 8.0);
  EmTrackNumIsolation50.Sumw2();
  EmTrackNumIsolation50.SetLineColor(6);
  EmTrackNumIsolation50.SetLineWidth(2);
  EmTrackNumIsolation50.SetMarkerColor(6);

  t->Draw("eMinus_ntrkisoatecal_15>>EmTrackNumIsolation10","mZee>60.0 && mZee<120.0","goff");
  t->Draw("eMinus_ntrkisoatecal_25>>EmTrackNumIsolation20","mZee>60.0 && mZee<120.0","goff");
  t->Draw("eMinus_ntrkisoatecal_30>>EmTrackNumIsolation30","mZee>60.0 && mZee<120.0","goff");
  t->Draw("eMinus_ntrkisoatecal_40>>EmTrackNumIsolation40","mZee>60.0 && mZee<120.0","goff");
  t->Draw("eMinus_ntrkisoatecal_50>>EmTrackNumIsolation50","mZee>60.0 && mZee<120.0","goff");

  TCanvas can23("can23","",880,600);
  gStyle->SetOptStat(0);
  EmTrackNumIsolation10.SetMaximum(EmTrackNumIsolation50.GetMaximum()*1.1);
  EmTrackNumIsolation10.Draw("HIST");
  EmTrackNumIsolation20.Draw("HIST same");
  EmTrackNumIsolation30.Draw("HIST same");
  EmTrackNumIsolation40.Draw("HIST same");
  EmTrackNumIsolation50.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.6,0.85,0.85);
  leg_hist->AddEntry(&EmTrackNumIsolation10,"Cone size 0.15","l");
  leg_hist->AddEntry(&EmTrackNumIsolation20,"Cone size 0.25","l");
  leg_hist->AddEntry(&EmTrackNumIsolation30,"Cone size 0.30","l");
  leg_hist->AddEntry(&EmTrackNumIsolation40,"Cone size 0.40","l");
  leg_hist->AddEntry(&EmTrackNumIsolation50,"Cone size 0.50","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  can23.SetGridx(1);
  can23.SetGridy(1);
  TString plotname = "eMinus-TrackNumIsolation";
  can23.SaveAs(plotname+TString(".eps"));
  can23.SaveAs(plotname+TString(".gif"));
  can23.SaveAs(plotname+TString(".root"));
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


// plot ECAL Isolation
if(plotEPECALIsolation == true ) {
  TH1F EPECALIsolation10("EPECALIsolation10","", 200, 0.0, 0.4);
  EPECALIsolation10.Sumw2();
  EPECALIsolation10.SetLineWidth(2);
  TAxis* EPECALIsolationx = EPECALIsolation10.GetXaxis();
  TAxis* EPECALIsolationy = EPECALIsolation10.GetYaxis();
  EPECALIsolationx->SetTitle("ECAL Isolation     ");
  EPECALIsolationy->SetTitle("Events / 0.002         ");
  EPECALIsolationy->SetTitleOffset(1.2);

  TH1F EPECALIsolation20("EPECALIsolation20","", 200, 0.0, 0.4);
  EPECALIsolation20.Sumw2();
  EPECALIsolation20.SetLineColor(2);
  EPECALIsolation20.SetLineWidth(2);
  EPECALIsolation20.SetMarkerColor(2);

  TH1F EPECALIsolation30("EPECALIsolation30","", 200, 0.0, 0.4);
  EPECALIsolation30.Sumw2();
  EPECALIsolation30.SetLineColor(3);
  EPECALIsolation30.SetLineWidth(2);
  EPECALIsolation30.SetMarkerColor(3);

  TH1F EPECALIsolation40("EPECALIsolation40","", 200, 0.0, 0.4);
  EPECALIsolation40.Sumw2();
  EPECALIsolation40.SetLineColor(4);
  EPECALIsolation40.SetLineWidth(2);
  EPECALIsolation40.SetMarkerColor(4);

  TH1F EPECALIsolation50("EPECALIsolation50","", 200, 0.0, 0.4);
  EPECALIsolation50.Sumw2();
  EPECALIsolation50.SetLineColor(6);
  EPECALIsolation50.SetLineWidth(2);
  EPECALIsolation50.SetMarkerColor(6);


  t->Draw("(ePlus_ecaliso_10-ePlus_sc_e)/ePlus_sc_e>>EPECALIsolation10",
	  "ePlus_ecaliso_10>-1.0 && ePlus_sc_et>0.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("(ePlus_ecaliso_20-ePlus_sc_e)/ePlus_sc_e>>EPECALIsolation20",
	  "ePlus_ecaliso_20>-1.0 && ePlus_sc_et>0.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("(ePlus_ecaliso_30-ePlus_sc_e)/ePlus_sc_e>>EPECALIsolation30",
	  "ePlus_ecaliso_30>-1.0 && ePlus_sc_et>0.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("(ePlus_ecaliso_40-ePlus_sc_e)/ePlus_sc_e>>EPECALIsolation40",
	  "ePlus_ecaliso_40>-1.0 && ePlus_sc_et>0.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("(ePlus_ecaliso_50-ePlus_sc_e)/ePlus_sc_e>>EPECALIsolation50",
	  "ePlus_ecaliso_50>-1.0 && ePlus_sc_et>0.0 && mZee>60.0 && mZee<120.0","goff");


  TCanvas can24("can24","",880,600);
  gStyle->SetOptStat(0);
  EPECALIsolation10.Draw();
  EPECALIsolation10.Draw("HIST same");
  EPECALIsolation20.Draw("HIST same");
  EPECALIsolation30.Draw("HIST same");
  EPECALIsolation40.Draw("HIST same");
  EPECALIsolation50.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.6,0.85,0.85);
  leg_hist->AddEntry(&EPECALIsolation10,"Cone size 0.10","l");
  leg_hist->AddEntry(&EPECALIsolation20,"Cone size 0.20","l");
  leg_hist->AddEntry(&EPECALIsolation30,"Cone size 0.30","l");
  leg_hist->AddEntry(&EPECALIsolation40,"Cone size 0.40","l");
  leg_hist->AddEntry(&EPECALIsolation50,"Cone size 0.50","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  // can24.SetLogy(1);
  can24.SetGridx(1);
  can24.SetGridy(1);
  TString plotname = "ePlus-ECALIsolation";
  can24.SaveAs(plotname+TString(".eps"));
  can24.SaveAs(plotname+TString(".gif"));
  can24.SaveAs(plotname+TString(".root"));
}


// plot HCAL Isolation
if( plotEPHCALIsolation == true ) {
  TH1F EPHCALIsolation10("EPHCALIsolation10","", 400, -0.1, 0.1);
  EPHCALIsolation10.Sumw2();
  EPHCALIsolation10.SetLineWidth(2);
  TAxis* EPHCALIsolationx = EPHCALIsolation10.GetXaxis();
  TAxis* EPHCALIsolationy = EPHCALIsolation10.GetYaxis();
  EPHCALIsolationx->SetTitle("HCAL Isolation     ");
  EPHCALIsolationy->SetTitle("Events / 0.0005         ");
  EPHCALIsolationy->SetTitleOffset(1.25);

  TH1F EPHCALIsolation20("EPHCALIsolation20","", 400, -0.1, 0.1);
  EPHCALIsolation20.Sumw2();
  EPHCALIsolation20.SetLineColor(2);
  EPHCALIsolation20.SetLineWidth(2);
  EPHCALIsolation20.SetMarkerColor(2);

  TH1F EPHCALIsolation30("EPHCALIsolation30","", 400, -0.1, 0.1);
  EPHCALIsolation30.Sumw2();
  EPHCALIsolation30.SetLineColor(3);
  EPHCALIsolation30.SetLineWidth(2);
  EPHCALIsolation30.SetMarkerColor(3);

  TH1F EPHCALIsolation40("EPHCALIsolation40","", 400, -0.1, 0.1);
  EPHCALIsolation40.Sumw2();
  EPHCALIsolation40.SetLineColor(4);
  EPHCALIsolation40.SetLineWidth(2);
  EPHCALIsolation40.SetMarkerColor(4);


  TH1F EPHCALIsolation50("EPHCALIsolation50","", 400, -0.1, 0.1);
  EPHCALIsolation50.Sumw2();
  EPHCALIsolation50.SetLineColor(6);
  EPHCALIsolation50.SetLineWidth(2);
  EPHCALIsolation50.SetMarkerColor(6);

  t->Draw("ePlus_hcaliso_10>>EPHCALIsolation10",
	  "ePlus_hcaliso_10>-1.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("ePlus_hcaliso_20>>EPHCALIsolation20",
	  "ePlus_hcaliso_20>-1.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("ePlus_hcaliso_30>>EPHCALIsolation30",
	  "ePlus_hcaliso_30>-1.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("ePlus_hcaliso_40>>EPHCALIsolation40",
	  "ePlus_hcaliso_40>-1.0 && mZee>60.0 && mZee<120.0","goff");
  t->Draw("ePlus_hcaliso_50>>EPHCALIsolation50",
	  "ePlus_hcaliso_50>-1.0 && mZee>60.0 && mZee<120.0","goff");

  TCanvas can25("can25","",880,600);
  gStyle->SetOptStat(0);
  EPHCALIsolation10.Draw();
  EPHCALIsolation10.Draw("HIST same");
  EPHCALIsolation20.Draw("HIST same");
  EPHCALIsolation30.Draw("HIST same");
  EPHCALIsolation40.Draw("HIST same");
  EPHCALIsolation50.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.6,0.85,0.85);
  leg_hist->AddEntry(&EPHCALIsolation10,"Cone size 0.10","l");
  leg_hist->AddEntry(&EPHCALIsolation20,"Cone size 0.20","l");
  leg_hist->AddEntry(&EPHCALIsolation30,"Cone size 0.30","l");
  leg_hist->AddEntry(&EPHCALIsolation40,"Cone size 0.40","l");
  leg_hist->AddEntry(&EPHCALIsolation50,"Cone size 0.50","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  //can25.SetLogy(1);
  can25.SetGridx(1);
  can25.SetGridy(1);
  TString plotname = "ePlus-HCALIsolation";
  can25.SaveAs(plotname+TString(".eps"));
  can25.SaveAs(plotname+TString(".gif"));
  can25.SaveAs(plotname+TString(".root"));
}



// plot Track pT Isolation
if( plotEPTrackPTIsolation == true) {
  TH1F EPTrackPTIsolation10("EPTrackPTIsolation10","", 60, 0.0, 3.0);
  EPTrackPTIsolation10.Sumw2();
  EPTrackPTIsolation10.SetLineWidth(2);
  TAxis* EPTrackPTIsolationx = EPTrackPTIsolation10.GetXaxis();
  TAxis* EPTrackPTIsolationy = EPTrackPTIsolation10.GetYaxis();
  EPTrackPTIsolationx->SetTitle("Track p_{T} Isolation     ");
  EPTrackPTIsolationy->SetTitle("Events / 0.05         ");
  EPTrackPTIsolationy->SetTitleOffset(1.2);

  TH1F EPTrackPTIsolation20("EPTrackPTIsolation20","", 60, 0.0, 3.0);
  EPTrackPTIsolation20.Sumw2();
  EPTrackPTIsolation20.SetLineColor(2);
  EPTrackPTIsolation20.SetLineWidth(2);
  EPTrackPTIsolation20.SetMarkerColor(2);

  TH1F EPTrackPTIsolation30("EPTrackPTIsolation30","", 60, 0.0, 3.0);
  EPTrackPTIsolation30.Sumw2();
  EPTrackPTIsolation30.SetLineColor(3);
  EPTrackPTIsolation30.SetLineWidth(2);
  EPTrackPTIsolation30.SetMarkerColor(3);

  TH1F EPTrackPTIsolation40("EPTrackPTIsolation40","", 60, 0.0, 3.0);
  EPTrackPTIsolation40.Sumw2();
  EPTrackPTIsolation40.SetLineColor(4);
  EPTrackPTIsolation40.SetLineWidth(2);
  EPTrackPTIsolation40.SetMarkerColor(4);

  TH1F EPTrackPTIsolation50("EPTrackPTIsolation50","", 60, 0.0, 3.0);
  EPTrackPTIsolation50.Sumw2();
  EPTrackPTIsolation50.SetLineColor(6);
  EPTrackPTIsolation50.SetLineWidth(2);
  EPTrackPTIsolation50.SetMarkerColor(6);


  TString e2Cut15 = "ePlus_ptisoatecal_15>0.0 && ePlusPt>0.0 && mZee>60.0 && mZee<120.0";
  TString e2Cut25 = "ePlus_ptisoatecal_25>0.0 && ePlusPt>0.0 && mZee>60.0 && mZee<120.0";
  TString e2Cut30 = "ePlus_ptisoatecal_30>0.0 && ePlusPt>0.0 && mZee>60.0 && mZee<120.0";
  TString e2Cut40 = "ePlus_ptisoatecal_40>0.0 && ePlusPt>0.0 && mZee>60.0 && mZee<120.0";
  TString e2Cut50 = "ePlus_ptisoatecal_50>0.0 && ePlusPt>0.0 && mZee>60.0 && mZee<120.0";

  t->Draw("ePlus_ptisoatecal_15/ePlusPt>>EPTrackPTIsolation10",
	  e2Cut15,"goff");
  t->Draw("ePlus_ptisoatecal_25/ePlusPt>>EPTrackPTIsolation20",
	  e2Cut25,"goff");
  t->Draw("ePlus_ptisoatecal_30/ePlusPt>>EPTrackPTIsolation30",
	  e2Cut30,"goff");
  t->Draw("ePlus_ptisoatecal_40/ePlusPt>>EPTrackPTIsolation40",
	  e2Cut40,"goff");
  t->Draw("ePlus_ptisoatecal_50/ePlusPt>>EPTrackPTIsolation50",
	  e2Cut50,"goff");

  TCanvas can26("can26","",880,600);
  gStyle->SetOptStat(0);
  EPTrackPTIsolation10.Draw("HIST same");
  EPTrackPTIsolation20.Draw("HIST same");
  EPTrackPTIsolation30.Draw("HIST same");
  EPTrackPTIsolation40.Draw("HIST same");
  EPTrackPTIsolation50.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.6,0.85,0.85);
  leg_hist->AddEntry(&EPTrackPTIsolation10,"Cone size 0.15","l");
  leg_hist->AddEntry(&EPTrackPTIsolation20,"Cone size 0.25","l");
  leg_hist->AddEntry(&EPTrackPTIsolation30,"Cone size 0.30","l");
  leg_hist->AddEntry(&EPTrackPTIsolation40,"Cone size 0.40","l");
  leg_hist->AddEntry(&EPTrackPTIsolation50,"Cone size 0.50","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  can26.SetGridx(1);
  can26.SetGridy(1);
  TString plotname = "ePlus-TrackPTIsolation";
  can26.SaveAs(plotname+TString(".eps"));
  can26.SaveAs(plotname+TString(".gif"));
  can26.SaveAs(plotname+TString(".root"));
}



// plot Track nTrack Isolation
if( plotEPTrackNumIsolation == true) {
  TH1F EPTrackNumIsolation10("EPTrackNumIsolation10","", 8, 0.0, 8.0);
  EPTrackNumIsolation10.Sumw2();
  EPTrackNumIsolation10.SetLineWidth(2);
  TAxis* EPTrackNumIsolationx = EPTrackNumIsolation10.GetXaxis();
  TAxis* EPTrackNumIsolationy = EPTrackNumIsolation10.GetYaxis();
  EPTrackNumIsolationx->SetTitle("Track n_{TRACK} Isolation     ");
  EPTrackNumIsolationy->SetTitle("Events         ");
  EPTrackNumIsolationy->SetTitleOffset(1.27);

  TH1F EPTrackNumIsolation20("EPTrackNumIsolation20","", 8, 0.0, 8.0);
  EPTrackNumIsolation20.Sumw2();
  EPTrackNumIsolation20.SetLineColor(2);
  EPTrackNumIsolation20.SetLineWidth(2);
  EPTrackNumIsolation20.SetMarkerColor(2);


  TH1F EPTrackNumIsolation30("EPTrackNumIsolation30","", 8, 0.0, 8.0);
  EPTrackNumIsolation30.Sumw2();
  EPTrackNumIsolation30.SetLineColor(3);
  EPTrackNumIsolation30.SetLineWidth(2);
  EPTrackNumIsolation30.SetMarkerColor(3);

  TH1F EPTrackNumIsolation40("EPTrackNumIsolation40","", 8, 0.0, 8.0);
  EPTrackNumIsolation40.Sumw2();
  EPTrackNumIsolation40.SetLineColor(4);
  EPTrackNumIsolation40.SetLineWidth(2);
  EPTrackNumIsolation40.SetMarkerColor(4);


  TH1F EPTrackNumIsolation50("EPTrackNumIsolation50","", 8, 0.0, 8.0);
  EPTrackNumIsolation50.Sumw2();
  EPTrackNumIsolation50.SetLineColor(6);
  EPTrackNumIsolation50.SetLineWidth(2);
  EPTrackNumIsolation50.SetMarkerColor(6);


  t->Draw("ePlus_ntrkisoatecal_15>>EPTrackNumIsolation10","mZee>60.0 && mZee<120.0","goff");
  t->Draw("ePlus_ntrkisoatecal_25>>EPTrackNumIsolation20","mZee>60.0 && mZee<120.0","goff");
  t->Draw("ePlus_ntrkisoatecal_30>>EPTrackNumIsolation30","mZee>60.0 && mZee<120.0","goff");
  t->Draw("ePlus_ntrkisoatecal_40>>EPTrackNumIsolation40","mZee>60.0 && mZee<120.0","goff");
  t->Draw("ePlus_ntrkisoatecal_50>>EPTrackNumIsolation50","mZee>60.0 && mZee<120.0","goff");


  TCanvas can27("can27","",880,600);
  gStyle->SetOptStat(0);
  EPTrackNumIsolation10.SetMaximum(EPTrackNumIsolation50.GetMaximum()*1.1);
  EPTrackNumIsolation10.Draw("HIST");
  EPTrackNumIsolation20.Draw("HIST same");
  EPTrackNumIsolation30.Draw("HIST same");
  EPTrackNumIsolation40.Draw("HIST same");
  EPTrackNumIsolation50.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.6,0.85,0.85);
  leg_hist->AddEntry(&EPTrackNumIsolation10,"Cone size 0.15","l");
  leg_hist->AddEntry(&EPTrackNumIsolation20,"Cone size 0.25","l");
  leg_hist->AddEntry(&EPTrackNumIsolation30,"Cone size 0.30","l");
  leg_hist->AddEntry(&EPTrackNumIsolation40,"Cone size 0.40","l");
  leg_hist->AddEntry(&EPTrackNumIsolation50,"Cone size 0.50","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  can27.SetGridx(1);
  can27.SetGridy(1);
  TString plotname = "ePlus-TrackNumIsolation";
  can27.SaveAs(plotname+TString(".eps"));
  can27.SaveAs(plotname+TString(".gif"));
  can27.SaveAs(plotname+TString(".root"));
}




/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// plot Zmass_Isolation
if( plotZmass_Isolation == true ) {
  TH1F Zmass_noIso("Zmass_noIso","", 120, 60.0, 120.0);
  Zmass_noIso.Sumw2();
  TAxis* Zmass_noIsox = Zmass_noIso.GetXaxis();
  TAxis* Zmass_noIsoy = Zmass_noIso.GetYaxis();
  Zmass_noIsox->SetTitle("e^{+} e^{-} invariant mass (GeV/c^{2})          ");
  Zmass_noIsoy->SetTitle("Events / (0.5 GeV/c^{2})                  ");
  Zmass_noIsoy->SetTitleOffset(1.25);
  Zmass_noIso.SetLineWidth(2);

  TH1F Zmass_TkIso("Zmass_TkIso","", 120, 60.0, 120.0);
  Zmass_TkIso.Sumw2();
  Zmass_TkIso.SetLineColor(2);
  Zmass_TkIso.SetMarkerColor(2);
  Zmass_TkIso.SetLineWidth(2);
  Zmass_TkIso.SetFillColor(2);

  TH1F Zmass_TkECALIso("Zmass_TkECALIso","", 120, 60.0, 120.0);
  Zmass_TkECALIso.Sumw2();
  Zmass_TkECALIso.SetLineColor(4);
  Zmass_TkECALIso.SetMarkerColor(4);
  Zmass_TkECALIso.SetLineWidth(2);
  Zmass_TkECALIso.SetFillColor(4);

  TH1F Zmass_TkECALHCALIso("Zmass_TkECALHCALIso","", 120, 60.0, 120.0);
  Zmass_TkECALHCALIso.Sumw2();
  Zmass_TkECALHCALIso.SetLineColor(3);
  Zmass_TkECALHCALIso.SetMarkerColor(3);
  Zmass_TkECALHCALIso.SetLineWidth(2);
  Zmass_TkECALHCALIso.SetFillColor(3);

  TString TkIsoCut 
    =TString("eMinus_ptisoatecal_15/eMinusPt < 0.2 && ePlus_ptisoatecal_15/ePlusPt < 0.2");
  TString ECALIsoCut = TkIsoCut + 
    TString(" && (eMinus_ecaliso_20-eMinus_sc_e)/eMinus_sc_e<0.2 && (ePlus_ecaliso_20-ePlus_sc_e)/ePlus_sc_e<0.2");
  // TString HCALIsoCut 
  // = ECALIsoCut + TString(" && fabs(eMinus_hcaliso_30)<0.1 && fabs(ePlus_hcaliso_30)<0.1");

  t->Draw("mZee>>Zmass_noIso", "", "goff");
  t->Draw("mZee>>Zmass_TkIso", TkIsoCut, "goff");
  t->Draw("mZee>>Zmass_TkECALIso", ECALIsoCut,"goff");
  //t->Draw("mZee>>Zmass_TkECALHCALIso", HCALIsoCut, "goff");

  TCanvas can28("can28","",880,600);
  gStyle->SetOptStat(0);
  Zmass_TkECALIso.Draw();
  Zmass_noIso.Draw();
  Zmass_noIso.Draw("HIST same");
  Zmass_TkIso.Draw("HIST same");
  Zmass_TkECALIso.Draw("HIST same");
  //Zmass_TkECALHCALIso.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.7,0.88,0.88);
  leg_hist->AddEntry(&Zmass_noIso,"No isolation","l");
  leg_hist->AddEntry(&Zmass_TkIso,"Track Isolation in 0.2 cone < 0.2","l");
  leg_hist->AddEntry(&Zmass_TkECALIso,"+ ECAL Isolation in 0.2 cone < 0.2","l");
  //leg_hist->AddEntry(&Zmass_TkECALHCALIso,"+ HCAL Isolation in 0.3 cone < 0.2","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  TString plotname = "Zmass_Isolation";
  can28.SaveAs(plotname+TString(".eps"));
  can28.SaveAs(plotname+TString(".gif"));
  can28.SaveAs(plotname+TString(".root"));
}


// plot Z mass - electrons by PTDR id
if( plotZmass_idPTDR == true ) {
  TH1F Zmass_ptdrtight("Zmass_ptdrtight","", 120, 60.0, 120.0);
  Zmass_ptdrtight.Sumw2();
  TAxis* Zmass_ptdrtightx = Zmass_ptdrtight.GetXaxis();
  TAxis* Zmass_ptdrtighty = Zmass_ptdrtight.GetYaxis();
  Zmass_ptdrtightx->SetTitle("e^{+} e^{-} invariant mass (GeV/c^{2})        ");
  Zmass_ptdrtighty->SetTitle("Events / (0.5 GeV/c^{2})     ");
  Zmass_ptdrtighty->SetTitleOffset(1.2);
  Zmass_ptdrtight.SetLineWidth(3);

  TH1F Zmass_ptdrmedium("Zmass_ptdrmedium","", 120, 60.0, 120.0);
  Zmass_ptdrmedium.Sumw2();
  Zmass_ptdrmedium.SetLineColor(2);
  Zmass_ptdrmedium.SetMarkerColor(2);
  Zmass_ptdrmedium.SetLineWidth(3);

  TH1F Zmass_ptdrloose("Zmass_ptdrloose","", 120, 60.0, 120.0);
  Zmass_ptdrloose.Sumw2();
  Zmass_ptdrloose.SetLineColor(4);
  Zmass_ptdrloose.SetMarkerColor(4);
  Zmass_ptdrloose.SetLineWidth(4);

  t->Draw("mZee>>Zmass_ptdrtight",  "isePlusTightPTDR & iseMinusTightPTDR", 
	  "goff");
  t->Draw("mZee>>Zmass_ptdrmedium", "isePlusMediumPTDR & iseMinusMediumPTDR", 
	  "goff");
  t->Draw("mZee>>Zmass_ptdrloose",  "isePlusLoosePTDR & iseMinusLoosePTDR", 
	  "goff");

  double scaleB = (double) Zmass_ptdrtight.Integral() / 
    (double)Zmass_ptdrmedium.Integral();  

  double scaleN = (double) Zmass_ptdrtight.Integral() / 
    (double)Zmass_ptdrloose.Integral();  


  Zmass_ptdrmedium.Scale(scaleB);
  Zmass_ptdrloose.Scale(scaleN);

  TCanvas can5("can5","",880,600);
  gStyle->SetOptStat(0);
  Zmass_ptdrtight.Draw("HIST");
  Zmass_ptdrmedium.Draw("HIST same");
  Zmass_ptdrloose.Draw("HIST same");
  Zmass_ptdrtight.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.65,0.85,0.85);
  leg_hist->AddEntry(&Zmass_ptdrtight,"Tight e^{#pm}","l");
  leg_hist->AddEntry(&Zmass_ptdrmedium,"Medium e^{#pm}","l");
  leg_hist->AddEntry(&Zmass_ptdrloose,"Loose e^{#pm}","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();

  TString plotname = "Zmass_idPTDR";
  can5.SaveAs(plotname+TString(".eps"));
  can5.SaveAs(plotname+TString(".gif"));
  can5.SaveAs(plotname+TString(".root"));
}


// ----------------------------------------
// plot Z mass - electrons by Barrel-Endcap
if( plotZmass_BarrelEndcap == true ) {
  TH1F Zmass_BB("Zmass_BB","", 120, 60.0, 120.0);
  Zmass_BB.Sumw2();
  TAxis* Zmass_BBx = Zmass_BB.GetXaxis();
  TAxis* Zmass_BBy = Zmass_BB.GetYaxis();
  Zmass_BBx->SetTitle("e^{+} e^{-} invariant mass (GeV/c^{2})        ");
  Zmass_BBy->SetTitle("Events / (0.5 GeV/c^{2})     ");
  Zmass_BBy->SetTitleOffset(1.2);
  Zmass_BB.SetLineWidth(3);

  TH1F Zmass_BE("Zmass_BE","", 120, 60.0, 120.0);
  Zmass_BE.Sumw2();
  Zmass_BE.SetLineColor(2);
  Zmass_BE.SetMarkerColor(2);
  Zmass_BE.SetLineWidth(3);

  TH1F Zmass_EE("Zmass_EE","", 120, 60.0, 120.0);
  Zmass_EE.Sumw2();
  Zmass_EE.SetLineColor(4);
  Zmass_EE.SetMarkerColor(4);
  Zmass_EE.SetLineWidth(4);

  t->Draw("mZee>>Zmass_BB", 
	  "ePlusClassification > -1 && ePlusClassification<99 && eMinusClassification > -1 && eMinusClassification<99", "goff");
  t->Draw("mZee>>Zmass_BE", 
	  "(ePlusClassification > -1 && ePlusClassification<99 && eMinusClassification > 99) || (eMinusClassification > -1 && eMinusClassification<99 && ePlusClassification > 99)", "goff");
  t->Draw("mZee>>Zmass_EE",  
	  "ePlusClassification > 99 && eMinusClassification > 99", "goff");


  double scaleB = (double) Zmass_BB.Integral() / 
    (double)Zmass_BE.Integral();  

  double scaleN = (double) Zmass_BB.Integral() / 
    (double)Zmass_EE.Integral();  


  Zmass_BE.Scale(scaleB);
  Zmass_EE.Scale(scaleN);

  TCanvas can6("can6","",880,600);
  gStyle->SetOptStat(0);
  Zmass_BB.Draw("HIST");
  Zmass_BE.Draw("HIST same");
  Zmass_EE.Draw("HIST same");
  Zmass_BB.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.65,0.85,0.85);
  leg_hist->AddEntry(&Zmass_BB,"Barrel-Barrel e^{#pm}","l");
  leg_hist->AddEntry(&Zmass_BE,"Barrel-Endcap e^{#pm}","l");
  leg_hist->AddEntry(&Zmass_EE,"Endcap-Endcap e^{#pm}","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();

  TString plotname = "Zmass_BarrelEndcap";
  can6.SaveAs(plotname+TString(".eps"));
  can6.SaveAs(plotname+TString(".gif"));
  can6.SaveAs(plotname+TString(".root"));
}


//----------------------------------------
// plot E/p
if( plotEoverP == true ) {
  TH1F EmEoverP("EmEoverP","", 350, 0.0, 3.5);
  EmEoverP.Sumw2();
  TAxis* EmEoverPx = EmEoverP.GetXaxis();
  TAxis* EmEoverPy = EmEoverP.GetYaxis();
  EmEoverPx->SetTitle("E/p     ");
  EmEoverPy->SetTitle("Events / 0.01         ");
  EmEoverPy->SetTitleOffset(1.2);
  EmEoverP.SetLineColor(4);
  EmEoverP.SetMarkerColor(4);

  TH1F EpEoverP("EpEoverP","", 350, 0.0, 3.5);
  EpEoverP.Sumw2();
  EpEoverP.SetLineColor(2);
  EpEoverP.SetMarkerColor(2);
  EpEoverP.SetLineWidth(3);

  t->Draw("eMinus_EoverPin>>EmEoverP","","goff");
  t->Draw("ePlus_EoverPin>>EpEoverP","","goff");

  TCanvas can7("can7","",880,600);
  gStyle->SetOptStat(0);
  EmEoverP.Draw();
  EpEoverP.Draw("HIST same");
  EmEoverP.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.65,0.85,0.85);
  leg_hist->AddEntry(&EmEoverP,"e^{-} E_{SC} / p_{Track}","l");
  leg_hist->AddEntry(&EpEoverP,"e^{+} E_{SC} / p_{Track}","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();

  TString plotname = "EoverP";
  can7.SaveAs(plotname+TString(".eps"));
  can7.SaveAs(plotname+TString(".gif"));
  can7.SaveAs(plotname+TString(".root"));
}


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////


// plot Brem Fraction
if( plotBremFraction == true ) {
  TH1F EpBremFraction("EpBremFraction","", 200, 0.0, 200.0);
  EpBremFraction.Sumw2();
  TAxis* EpBremFractionx = EpBremFraction.GetXaxis();
  TAxis* EpBremFractiony = EpBremFraction.GetYaxis();
  EpBremFractionx->SetTitle("Brem Fraction (%)     ");
  EpBremFractiony->SetTitle("Events / %         ");
  EpBremFractiony->SetTitleOffset(1.2);

  TH1F EmBremFraction("EmBremFraction","", 200, 0.0, 200.0);
  EmBremFraction.Sumw2();
  EmBremFraction.SetLineColor(2);
  EmBremFraction.SetMarkerColor(2);

  t->Draw("ePlus_BremFraction>>EpBremFraction",
	  "mZee>60.0 && mZee<120.0","goff");
  t->Draw("eMinus_BremFraction>>EmBremFraction",
	  "mZee>60.0 && mZee<120.0","goff");

  TCanvas can3("can3","",880,600);
  gStyle->SetOptStat(0);
  EpBremFraction.Draw();
  EmBremFraction.Draw("HIST same");
  leg_hist = new TLegend(0.7,0.75,0.85,0.85);
  leg_hist->AddEntry(&EpBremFraction,"e^{+}","l");
  leg_hist->AddEntry(&EmBremFraction,"e^{-}","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();

  TString plotname = "BremFraction";
  can3.SaveAs(plotname+TString(".eps"));
  can3.SaveAs(plotname+TString(".gif"));
  can3.SaveAs(plotname+TString(".root"));
}





// plot Classification
if( plotClassification == true ) {
  TH1F EpClassification("EpClassification","", 141, -1.0, 140.0);
  EpClassification.Sumw2();
  TAxis* EpClassificationx = EpClassification.GetXaxis();
  TAxis* EpClassificationy = EpClassification.GetYaxis();
  EpClassificationx->SetTitle("Classification     ");
  EpClassificationy->SetTitle("Fractional population (%)          ");
  EpClassificationy->SetTitleOffset(1);
  EpClassification.SetLineColor(2);
  EpClassification.SetFillColor(2);
  EpClassification.SetMarkerColor(2);

  EpClassification.SetLineWidth(3);

  t->Draw("ePlusClassification>>EpClassification","ePlusClassification != -1 && mZee>60.0 && mZee<120.0","goff");


  int norm = (int) t->Draw("ePlusClassification","ePlusClassification != -1 && mZee>60.0 && mZee<120.0","goff");

  EpClassification.Scale(100.0/norm);

  TCanvas can8("can8","",880,600);
  gStyle->SetOptStat(0);
  EpClassification.Draw("hbar");
  leg_hist = new TLegend(0.4,0.55,0.88,0.88);
  leg_hist->AddEntry(&EpClassification,"#color[4]{Electron Classification}","");
  leg_hist->AddEntry(&EpClassification,"#0 = barrel : golden","l");
  leg_hist->AddEntry(&EpClassification,"10 = barrel : big-brem","l");
  leg_hist->AddEntry(&EpClassification,"20 = barrel : narrow","l");
  leg_hist->AddEntry(&EpClassification,"30-34 = barrel : showering","l");
  leg_hist->AddEntry(&EpClassification,"40 = in the cracks","l");
  leg_hist->AddEntry(&EpClassification,"100 = endcap : golden","l");
  leg_hist->AddEntry(&EpClassification,"110 = endcap : big-brem","l");
  leg_hist->AddEntry(&EpClassification,"120 = endcap : narrow","l");
  leg_hist->AddEntry(&EpClassification,"130-134 = endcap : showering","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw("CE1");
  can8.SetGridx(1);
  can8.SetGridy(1);

  TString plotname = "Classification";
  can8.SaveAs(plotname+TString(".eps"));
  can8.SaveAs(plotname+TString(".gif"));
  can8.SaveAs(plotname+TString(".root"));
}






// plot Z mass - electrons by catagory
if( plotZmass_category == true ) {
  TH1F Zmass_golden("Zmass_golden","", 120, 60.0, 120.0);
  Zmass_golden.Sumw2();
  Zmass_golden.SetLineWidth(3);

  TH1F Zmass_bigbrem("Zmass_bigbrem","", 120, 60.0, 120.0);
  Zmass_bigbrem.Sumw2();
  TAxis* Zmass_bigbremx = Zmass_bigbrem.GetXaxis();
  TAxis* Zmass_bigbremy = Zmass_bigbrem.GetYaxis();
  Zmass_bigbremx->SetTitle("e^{+} e^{-} invariant mass (GeV/c^{2})        ");
  Zmass_bigbremy->SetTitle("Events / (0.5 GeV/c^{2})     ");
  Zmass_bigbremy->SetTitleOffset(1.2);
  Zmass_bigbrem.SetLineColor(2);
  Zmass_bigbrem.SetMarkerColor(2);
  Zmass_bigbrem.SetLineWidth(3);

  TH1F Zmass_narrow("Zmass_narrow","", 120, 60.0, 120.0);
  Zmass_narrow.Sumw2();
  Zmass_narrow.SetLineColor(3);
  Zmass_narrow.SetMarkerColor(3);
  Zmass_narrow.SetLineWidth(3);

  TH1F Zmass_shower("Zmass_shower","", 120, 60.0, 120.0);
  Zmass_shower.Sumw2();
  Zmass_shower.SetLineColor(4);
  Zmass_shower.SetMarkerColor(4);
  Zmass_shower.SetLineWidth(3);

  t->Draw("mZee>>Zmass_golden",
	  "ePlusClassification % 100 == 0 && eMinusClassification % 100 == 0",
	  "goff");

  t->Draw("mZee>>Zmass_bigbrem",
	  "ePlusClassification % 100 == 10 && eMinusClassification % 100 == 10",
	  "goff");

  t->Draw("mZee>>Zmass_narrow",
	  "ePlusClassification % 100 == 20 && eMinusClassification % 100 == 20",
	  "goff");

  t->Draw("mZee>>Zmass_shower",
	  "ePlusClassification %100 > 29 && ePlusClassification %100 < 35 && eMinusClassification % 100 > 29 && eMinusClassification % 100 < 35","goff");


  double scaleB = (double) Zmass_golden.Integral() / 
    (double)Zmass_bigbrem.Integral();  

  double scaleN = (double) Zmass_golden.Integral() / 
    (double)Zmass_narrow.Integral();  

  double scaleS = (double) Zmass_golden.Integral() / 
    (double)Zmass_shower.Integral();  

  Zmass_bigbrem.Scale(scaleB);
  Zmass_narrow.Scale(scaleN);
  Zmass_shower.Scale(scaleS);

  TCanvas can4("can4","",880,600);
  gStyle->SetOptStat(0);
  Zmass_bigbrem.Draw("HIST");
  Zmass_golden.Draw("HIST same");
  Zmass_narrow.Draw("HIST same");
  Zmass_shower.Draw("HIST same");
  Zmass_golden.Draw("HIST same");
  leg_hist = new TLegend(0.6,0.65,0.85,0.85);
  leg_hist->AddEntry(&Zmass_golden,"Golden e^{#pm}","l");
  leg_hist->AddEntry(&Zmass_bigbrem,"Big-brem e^{#pm}","l");
  leg_hist->AddEntry(&Zmass_narrow,"Narrow e^{#pm}","l");
  leg_hist->AddEntry(&Zmass_shower,"Showering e^{#pm}","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();

  TString plotname = "Zmass_category";
  can4.SaveAs(plotname+TString(".eps"));
  can4.SaveAs(plotname+TString(".gif"));
  can4.SaveAs(plotname+TString(".root"));
}





// plot H/E
if( plotHoverE == true ) {
  TH1F EpHoverE("EpHoverE","", 400, -0.1, 0.1);
  EpHoverE.Sumw2();
  TAxis* h1x = EpHoverE.GetXaxis();
  TAxis* h1y = EpHoverE.GetYaxis();
  h1x->SetTitle("H/E     ");
  h1y->SetTitle("Events / 0.0005         ");
  h1y->SetTitleOffset(1.25);

  TH1F EmHoverE("EmHoverE","", 400, -0.1, 0.1);
  EmHoverE.Sumw2();
  EmHoverE.SetLineColor(2);
  EmHoverE.SetMarkerColor(2);

  t->Draw("ePlus_HoverE>>EpHoverE","mZee>60.0 && mZee<120.0","goff");
  t->Draw("eMinus_HoverE>>EmHoverE","mZee>60.0 && mZee<120.0","goff");

  TCanvas can1("can1","",880,600);
  gStyle->SetOptStat(0);
  EpHoverE.Draw();
  EmHoverE.Draw("HIST same");
  leg_hist = new TLegend(0.7,0.75,0.85,0.85);
  leg_hist->AddEntry(&EpHoverE,"e^{+}","l");
  leg_hist->AddEntry(&EmHoverE,"e^{-}","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();

  TString plotname = "HoverE";
  can1.SaveAs(plotname+TString(".eps"));
  can1.SaveAs(plotname+TString(".gif"));
  can1.SaveAs(plotname+TString(".root"));
}




// plot Z candidate pT
if( plotZpt == true ) {
  TH1F genZpt("genZpt","", 200, 0.0, 1000.0);
  genZpt.Sumw2();
  TAxis* gZx = genZpt.GetXaxis();
  TAxis* gZy = genZpt.GetYaxis();
  gZx->SetTitle("Z candidate p_{T}     ");
  gZy->SetTitle("Events / ( 5 GeV/c )  ");
  gZy->SetTitleOffset(1.2);

  TH1F recoZpt("recoZpt","", 200, 0.0, 1000.0);
  recoZpt.Sumw2();
  recoZpt.SetLineColor(2);
  recoZpt.SetMarkerColor(2);

  t->Draw("Z_PtGen>>genZpt","mZee>60.0 && mZee<120.0","goff");
  t->Draw("sqrt((ePlus_sc_e+eMinus_sc_e)**2-mZee**2)/cosh(Z_Eta)>>recoZpt",
	  "mZee>60.0 && mZee<120.0","goff");

  double aScale = (double) genZpt.Integral() / 
    (double)recoZpt.Integral();  
  recoZpt.Scale(aScale);
  genZpt.SetMaximum(recoZpt.GetMaximum()*1.1);

  TCanvas Zpt("Zpt","",880,600);
  gStyle->SetOptStat(0);
  genZpt.Draw("hist");
  recoZpt.Draw("same");
  leg_hist = new TLegend(0.6,0.7,0.85,0.86);
  leg_hist->AddEntry("","Z candidate p_{T}","");
  leg_hist->AddEntry(&genZpt,"Generator level","l");
  leg_hist->AddEntry(&recoZpt,"Calorimeter level","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  TString plotname = "Zpt";
  Zpt.SaveAs(plotname+TString(".eps"));
  Zpt.SaveAs(plotname+TString(".gif"));
  Zpt.SaveAs(plotname+TString(".root"));
}





// plot Z candidate pT after mass cut
if( plotZpt_masscut == true ) {
  TH1F genZptm("genZptm","", 200, 0.0, 1000.0);
  genZptm.Sumw2();
  TAxis* gZx = genZptm.GetXaxis();
  TAxis* gZy = genZptm.GetYaxis();
  gZx->SetTitle("Z candidate p_{T}     ");
  gZy->SetTitle("Events / ( 5 GeV/c )  ");
  gZy->SetTitleOffset(1.2);

  TH1F recoZptm("recoZptm","", 200, 0.0, 1000.0);
  recoZptm.Sumw2();
  recoZptm.SetLineColor(2);
  recoZptm.SetMarkerColor(2);

  t->Draw("Z_PtGen>>genZptm","fabs(mZee-91.2)<2.5 && ePlusPtGen>20.0 && eMinusPtGen>20.0","goff");
  t->Draw("sqrt((ePlus_sc_e+eMinus_sc_e)**2-mZee**2)/cosh(Z_Eta)>>recoZptm",
	  "fabs(mZee-91.2)<2.5 && ePlusPtGen>20.0 && eMinusPtGen>20.0","goff");

  double aScale = (double) genZptm.Integral() / 
    (double)recoZptm.Integral();  
  recoZptm.Scale(aScale);
  genZptm.SetMaximum(recoZptm.GetMaximum()*1.1);

  TCanvas Zptm("Zptm","",880,600);
  gStyle->SetOptStat(0);
  genZptm.Draw("hist");
  recoZptm.Draw("same");
  leg_hist = new TLegend(0.6,0.7,0.85,0.86);
  leg_hist->AddEntry("","Z candidate p_{T} after mass cut","");
  leg_hist->AddEntry(&genZptm,"Generator level","l");
  leg_hist->AddEntry(&recoZptm,"Calorimeter level","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  TString plotname = "Zpt-masscut";
  Zptm.SaveAs(plotname+TString(".eps"));
  Zptm.SaveAs(plotname+TString(".gif"));
  Zptm.SaveAs(plotname+TString(".root"));
}




// plot e+ candidate pT
if( plotEPpt == true ) {
  TH1F genEPpt("genEPpt","", 200, 0.0, 1000.0);
  genEPpt.Sumw2();
  TAxis* gZx = genEPpt.GetXaxis();
  TAxis* gZy = genEPpt.GetYaxis();
  gZx->SetTitle("e^{+} candidate p_{T}     ");
  gZy->SetTitle("Events / ( 5 GeV/c )  ");
  gZy->SetTitleOffset(1.2);

  TH1F recoEPpt("recoEPpt","", 200, 0.0, 1000.0);
  recoEPpt.Sumw2();
  recoEPpt.SetLineColor(2);
  recoEPpt.SetMarkerColor(2);

  t->Draw("ePlusPtGen>>genEPpt","mZee>60.0 && mZee<120.0","goff");
  t->Draw("ePlus_sc_et>>recoEPpt","mZee>60.0 && mZee<120.0","goff");

  double aScale = (double) genEPpt.Integral() / 
    (double)recoEPpt.Integral();  
  recoEPpt.Scale(aScale);
  genEPpt.SetMaximum(recoEPpt.GetMaximum()*1.1);

  TCanvas EPpt("EPpt","",880,600);
  gStyle->SetOptStat(0);
  genEPpt.Draw("hist");
  recoEPpt.Draw("same");
  leg_hist = new TLegend(0.6,0.58,0.85,0.86);
  leg_hist->AddEntry("","e^{+} candidate p_{T}","");
  leg_hist->AddEntry(&genEPpt,"Generator level","l");
  leg_hist->AddEntry(&recoEPpt,"Calorimeter level","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  TString plotname = "EPpt";
  EPpt.SaveAs(plotname+TString(".eps"));
  EPpt.SaveAs(plotname+TString(".gif"));
  EPpt.SaveAs(plotname+TString(".root"));
}




// plot e- candidate pT
if( plotEMpt == true ) {
  TH1F genEMpt("genEMpt","", 200, 0.0, 1000.0);
  genEMpt.Sumw2();
  TAxis* gZx = genEMpt.GetXaxis();
  TAxis* gZy = genEMpt.GetYaxis();
  gZx->SetTitle("e^{-} candidate p_{T}     ");
  gZy->SetTitle("Events / ( 5 GeV/c )  ");
  gZy->SetTitleOffset(1.2);

  TH1F recoEMpt("recoEMpt","", 200, 0.0, 1000.0);
  recoEMpt.Sumw2();
  recoEMpt.SetLineColor(2);
  recoEMpt.SetMarkerColor(2);

  t->Draw("eMinusPtGen>>genEMpt","mZee>60.0 && mZee<120.0","goff");
  t->Draw("eMinus_sc_et>>recoEMpt","mZee>60.0 && mZee<120.0","goff");

  double aScale = (double) genEMpt.Integral() / 
    (double)recoEMpt.Integral();  
  recoEMpt.Scale(aScale);
  genEMpt.SetMaximum(recoEMpt.GetMaximum()*1.1);

  TCanvas EMpt("EMpt","",880,600);
  gStyle->SetOptStat(0);
  genEMpt.Draw("hist");
  recoEMpt.Draw("same");
  leg_hist = new TLegend(0.6,0.58,0.85,0.86);
  leg_hist->AddEntry("","e^{-} candidate p_{T}","");
  leg_hist->AddEntry(&genEMpt,"Generator level","l");
  leg_hist->AddEntry(&recoEMpt,"Calorimeter level","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  TString plotname = "EMpt";
  EMpt.SaveAs(plotname+TString(".eps"));
  EMpt.SaveAs(plotname+TString(".gif"));
  EMpt.SaveAs(plotname+TString(".root")); 
}



// plot e- candidate pT after mass cut
if( plotEMptAfterMassCut == true ) {
  TH1F genEMpt("genEMpt","", 45, 0.0, 225.0);
  genEMpt.Sumw2();
  TAxis* gZx = genEMpt.GetXaxis();
  TAxis* gZy = genEMpt.GetYaxis();
  gZx->SetTitle("e^{-} candidate p_{T}     ");
  gZy->SetTitle("Events / ( 5 GeV/c )  ");
  gZy->SetTitleOffset(1.2);

  TH1F recoEMpt("recoEMpt","", 45, 0.0, 225.0);
  recoEMpt.Sumw2();
  recoEMpt.SetLineColor(2);
  recoEMpt.SetMarkerColor(2);

  t->Draw("eMinusPtGen>>genEMpt","fabs(mZee-91.2)<2.5","goff");
  t->Draw("eMinus_sc_et>>recoEMpt","fabs(mZee-91.2)<2.5","goff");

  double aScale = (double) genEMpt.Integral() / 
    (double)recoEMpt.Integral();  
  recoEMpt.Scale(aScale);
  genEMpt.SetMaximum(recoEMpt.GetMaximum()*1.1);

  TCanvas EMpt("EMpt","",880,600);
  gStyle->SetOptStat(0);
  genEMpt.Draw("hist");
  recoEMpt.Draw("same");
  leg_hist = new TLegend(0.6,0.58,0.85,0.86);
  leg_hist->AddEntry("","e^{-} candidate p_{T}","");
  leg_hist->AddEntry(&genEMpt,"Generator level","l");
  leg_hist->AddEntry(&recoEMpt,"Calorimeter level","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  TString plotname = "EMpt_ZmassCut";
  EMpt.SaveAs(plotname+TString(".eps"));
  EMpt.SaveAs(plotname+TString(".gif"));
  EMpt.SaveAs(plotname+TString(".root")); 
}







// plot e- candidate pT by category
if( plotEMptCategory == true ) {
  TH1F genEMptGolden("genEMptGolden","", 20, 0.0, 100.0);
  genEMptGolden.Sumw2();
  genEMptGolden.SetLineWidth(3);
  genEMptGolden.SetLineStyle(2);

  TH1F genEMptBigbrem("genEMptBigbrem","", 20, 0.0, 100.0);
  genEMptBigbrem.Sumw2();
  TAxis* gZx = genEMptBigbrem.GetXaxis();
  TAxis* gZy = genEMptBigbrem.GetYaxis();
  gZx->SetTitle("e^{-} p_{T} (GeV/c)    ");
  gZy->SetTitle("Events / ( 5 GeV/c )  ");
  gZy->SetTitleOffset(1.2);
  genEMptBigbrem.SetLineWidth(3);
  genEMptBigbrem.SetLineColor(2);
  genEMptBigbrem.SetMarkerColor(2);
  genEMptBigbrem.SetLineStyle(2);

  TH1F genEMptNarrow("genEMptNarrow","", 20, 0.0, 100.0);
  genEMptNarrow.Sumw2();
  genEMptNarrow.SetLineWidth(3);
  genEMptNarrow.SetLineColor(3);
  genEMptNarrow.SetMarkerColor(3);
  genEMptNarrow.SetLineStyle(2);

  TH1F genEMptShowering("genEMptShowering","", 20, 0.0, 100.0);
  genEMptShowering.Sumw2();
  genEMptShowering.SetLineWidth(3);
  genEMptShowering.SetLineColor(4);
  genEMptShowering.SetMarkerColor(4);
  genEMptShowering.SetLineStyle(2);

  // ---------------

  TH1F recoEMptGolden("recoEMptGolden","", 20, 0.0, 100.0);
  recoEMptGolden.Sumw2();
  recoEMptGolden.SetLineWidth(3);

  TH1F recoEMptBigbrem("recoEMptBigbrem","", 20, 0.0, 100.0);
  recoEMptBigbrem.Sumw2();
  recoEMptBigbrem.SetLineWidth(3);
  recoEMptBigbrem.SetLineColor(2);
  recoEMptBigbrem.SetMarkerColor(2);

  TH1F recoEMptNarrow("recoEMptNarrow","", 20, 0.0, 100.0);
  recoEMptNarrow.Sumw2();
  recoEMptNarrow.SetLineWidth(3);
  recoEMptNarrow.SetLineColor(3);
  recoEMptNarrow.SetMarkerColor(3);

  TH1F recoEMptShowering("recoEMptShowering","", 20, 0.0, 100.0);
  recoEMptShowering.Sumw2();
  recoEMptShowering.SetLineWidth(3);
  recoEMptShowering.SetLineColor(4);
  recoEMptShowering.SetMarkerColor(4);


  t->Draw("eMinusPtGen>>genEMptGolden","fabs(mZee-91.2)<2.5 && eMinusClassification % 100 == 0","goff");
  t->Draw("eMinusPtGen>>genEMptBigbrem","fabs(mZee-91.2)<2.5 && eMinusClassification % 100 == 10","goff");
  t->Draw("eMinusPtGen>>genEMptNarrow","fabs(mZee-91.2)<2.5 && eMinusClassification % 100 == 20","goff");
  t->Draw("eMinusPtGen>>genEMptShowering","fabs(mZee-91.2)<2.5 && eMinusClassification % 100 > 29 &&  eMinusClassification % 100 <35","goff");


  t->Draw("eMinus_sc_et>>recoEMptGolden","fabs(mZee-91.2)<2.5 && eMinusClassification % 100 == 0","goff");
  t->Draw("eMinus_sc_et>>recoEMptBigbrem","fabs(mZee-91.2)<2.5 && eMinusClassification % 100 == 10","goff");
  t->Draw("eMinus_sc_et>>recoEMptNarrow","fabs(mZee-91.2)<2.5 && eMinusClassification % 100 == 20","goff");
  t->Draw("eMinus_sc_et>>recoEMptShowering","fabs(mZee-91.2)<2.5 && eMinusClassification % 100 > 29 &&  eMinusClassification % 100 <35","goff");


  genEMptBigbrem.Scale((double) genEMptGolden.Integral()/(double) genEMptBigbrem.Integral());
  genEMptNarrow.Scale((double) genEMptGolden.Integral()/(double) genEMptNarrow.Integral());
  genEMptShowering.Scale((double) genEMptGolden.Integral()/(double) genEMptShowering.Integral());
  recoEMptGolden.Scale((double) genEMptGolden.Integral()/(double) recoEMptGolden.Integral());
  recoEMptBigbrem.Scale((double) genEMptGolden.Integral()/(double) recoEMptBigbrem.Integral());
  recoEMptNarrow.Scale((double) genEMptGolden.Integral()/(double) recoEMptNarrow.Integral());
  recoEMptShowering.Scale((double) genEMptGolden.Integral()/(double) recoEMptShowering.Integral());

  genEMptGolden.SetMaximum(recoEMptNarrow.GetMaximum()*1.1);


  TCanvas EMptCategory("EMptCategory","",880,600);
  gStyle->SetOptStat(0);
  genEMptBigbrem.Draw("hist");
  genEMptGolden.Draw("hist same");
  genEMptNarrow.Draw("hist same");
  genEMptShowering.Draw("hist same");
  recoEMptGolden.Draw("hist same");
  recoEMptBigbrem.Draw("hist same");
  recoEMptNarrow.Draw("hist same");
  recoEMptShowering.Draw("hist same");
  leg_hist = new TLegend(0.63,0.58,0.88,0.86);
  leg_hist->AddEntry(&recoEMptGolden,"Golden","l");
  leg_hist->AddEntry(&recoEMptBigbrem,"Big-brem","l");
  leg_hist->AddEntry(&recoEMptNarrow,"Narrow","l");
  leg_hist->AddEntry(&recoEMptShowering,"Showering","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  TString plotname = "EMpt_category";
  EMptCategory.SaveAs(plotname+TString(".eps"));
  EMptCategory.SaveAs(plotname+TString(".gif"));
  EMptCategory.SaveAs(plotname+TString(".root")); 
}




// plot Z candidate mass
if( plotZmass == true ) {
  TH1F genZmass("genZmass","", 120, 60.0, 120.0);
  genZmass.Sumw2();
  TAxis* gZx = genZmass.GetXaxis();
  TAxis* gZy = genZmass.GetYaxis();
  gZx->SetTitle("Z candidate mass     ");
  gZy->SetTitle("Events / GeV         ");
  gZy->SetTitleOffset(1.2);

  TH1F recoZmass("recoZmass","", 120, 60.0, 120.0);
  recoZmass.Sumw2();
  recoZmass.SetLineColor(2);
  recoZmass.SetMarkerColor(2);

  t->Draw("mZee>>recoZmass","mZee>60.0 && mZee<120.0","goff");
  t->Draw("mZeeGen>>genZmass","mZee>60.0 && mZee<120.0","goff");

  TCanvas Zmass("Zmass","",880,600);
  gStyle->SetOptStat(0);
  genZmass.Draw("hist");
  recoZmass.Draw("same");
  leg_hist = new TLegend(0.6,0.58,0.85,0.86);
  leg_hist->AddEntry("","Z -> e^{+}e^{-} invariant mass","");
  leg_hist->AddEntry(&genZmass,"Generator level","l");
  leg_hist->AddEntry(&recoZmass,"Calorimeter level","l");
  leg_hist->SetFillColor(0);
  leg_hist->Draw();
  TString plotname = "Zmass";
  Zmass.SaveAs(plotname+TString(".eps"));
  Zmass.SaveAs(plotname+TString(".gif"));
  Zmass.SaveAs(plotname+TString(".root"));
}

}





