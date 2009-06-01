{
// gSystem.Load("libFWCoreFWLite.so");
// AutoLibraryLoader::enable();

TFile* f1 = new TFile("pat-hist-Zee_noCut.root");
TH1F* deta1 = f1->Get("deltaEta");
TH1F* dphi1 = f1->Get("deltaPhi");
TH1F* tk1 = f1->Get("trackIso");
TH1F* ecal1 = f1->Get("ecalIso");
TH1F* hcal1 = f1->Get("hcalIso");
TH1F* sihih1 = f1->Get("sigmaIetaIeta");
TH1F* eta1 = f1->Get("elecEta");
TH1F* phi1 = f1->Get("elecPhi");


//doubleedmValueMap_eleIsoFromDepsTk__RECO.obj.ids_.first.id_

//TFile* f2 = new TFile("~berryhil/nobackup/redigi/wenu_redigi.root");
TFile* f2 = new TFile("~berryhil/nobackup/redigi/zee_m20_v1.root");
TTree* t2 = (TTree*) f2->Get("probe_tree");


TH1F* eta2 = new TH1F("eta2","",1000, -3, 3);
eta2->Sumw2();
TH1F* phi2 = new TH1F("phi2","",1000, -3.5, 3.5);
phi2->Sumw2();
TH1F* deta2 = new TH1F("deta2","",1000, -0.1, 0.1);
deta2->Sumw2();
TH1F* dphi2 = new TH1F("dphi2","",1000, -0.1, 0.1);
dphi2->Sumw2();
TH1F* tk2 = new TH1F("tk2","",10000, 0, 100);
tk2->Sumw2();
TH1F* ecal2 = new TH1F("ecal2","",10000, 0, 100);
ecal2->Sumw2();
TH1F* hcal2 = new TH1F("hcal2","",10000, 0, 100);
hcal2->Sumw2();
TH1F* sihih2 = new TH1F("sihih2","",1000, 0.0, 0.2);
sihih2->Sumw2();


TString etacut = "abs(probe_ele_eta)<1.4442 || (abs(probe_ele_eta)>1.56 && abs(probe_ele_eta)<2.5)";

t2->Draw("probe_ele_eta>>eta2", etacut, "goff");
t2->Draw("probe_ele_phi>>phi2", etacut, "goff");
t2->Draw("probe_ele_dhi>>deta2", etacut,"goff");
t2->Draw("probe_ele_dfi>>dphi2", etacut ,"goff");
t2->Draw("probe_isolation_value>>tk2", etacut, "goff");
t2->Draw("probe_ecal_isolation_value>>ecal2", etacut, "goff");
t2->Draw("probe_hcal_isolation_value>>hcal2", etacut, "goff");
t2->Draw("probe_ele_sihih>>sihih2", etacut, "goff");


eta2->SetLineColor(2);
phi2->SetLineColor(2);
deta2->SetLineColor(2);
dphi2->SetLineColor(2);
tk2->SetLineColor(2);
ecal2->SetLineColor(2);
hcal2->SetLineColor(2);
sihih2->SetLineColor(2);

eta2->SetMarkerColor(2);
phi2->SetMarkerColor(2);
deta2->SetMarkerColor(2);
dphi2->SetMarkerColor(2);
tk2->SetMarkerColor(2);
ecal2->SetMarkerColor(2);
hcal2->SetMarkerColor(2);
sihih2->SetMarkerColor(2);




gStyle->SetOptStat(0);

//eta1->SetMinimum(2);
eta1->GetYaxis()->SetNoExponent();

//phi1->SetMinimum(2);
phi1->GetYaxis()->SetNoExponent();

//deta1->SetMinimum(2);
//deta1->GetXaxis()->SetRangeUser(-0.02, 0.02);
deta1->GetYaxis()->SetNoExponent();

//dphi1->SetMinimum(2);
//dphi1->GetXaxis()->SetRangeUser(-0.1, 0.1);
dphi1->GetYaxis()->SetNoExponent();

//sihih1->SetMinimum(2);
//sihih1->GetXaxis()->SetRangeUser(0,0.05);
sihih1->GetYaxis()->SetNoExponent();

//tk1->SetMinimum(2);
//tk1->GetXaxis()->SetRangeUser(0,10);
tk1->GetYaxis()->SetNoExponent();

//ecal1->SetMinimum(2);
//ecal1->GetXaxis()->SetRangeUser(0,10);
ecal1->GetYaxis()->SetNoExponent();

//hcal1->SetMinimum(2);
//hcal1->GetXaxis()->SetRangeUser(0,10);
hcal1->GetYaxis()->SetNoExponent();

eta2->Scale( eta1->Integral() / eta2->Integral() );
phi2->Scale( phi1->Integral() / phi2->Integral() );
deta2->Scale( deta1->Integral() / deta2->Integral() );
dphi2->Scale( dphi1->Integral() / dphi2->Integral() );
sihih2->Scale( sihih1->Integral() / sihih2->Integral() );
tk2->Scale( tk1->Integral() / tk2->Integral() );
ecal2->Scale( ecal1->Integral() / ecal2->Integral() );
hcal2->Scale( hcal1->Integral() / hcal2->Integral() );



TCanvas* etaCan = new TCanvas("etaCan","", 500, 500);
eta1->Draw();
eta2->Draw("same");
etaCan->SetLogy(1);
etaCan->SaveAs("eta.gif");


TCanvas* phiCan = new TCanvas("phiCan","", 500, 500);
phi1->Draw();
phi2->Draw("same");
phiCan->SetLogy(1);
phiCan->SaveAs("phi.gif");



TCanvas* detaCan = new TCanvas("detaCan","", 500, 500);
deta1->Draw();
deta2->Draw("same");
detaCan->SetLogy(1);
detaCan->SaveAs("deta.gif");


TCanvas* phiCan = new TCanvas("dphiCan","", 500, 500);
dphi1->Draw();
dphi2->Draw("same");
dphiCan->SetLogy(1);
dphiCan->SaveAs("dphi.gif");

TCanvas* shhCan = new TCanvas("shhCan","", 500, 500);
sihih1->Draw();
sihih2->Draw("same");
shhCan->SetLogy(1);
shhCan->SaveAs("sihih.gif");


TCanvas* tkCan = new TCanvas("tkCan","", 500, 500);
tk1->Draw();
tk2->Draw("same");
tkCan->SetLogy(1);
tkCan->SaveAs("tk.gif");

TCanvas* ecalCan = new TCanvas("ecalCan","", 500, 500);
ecal1->Draw();
ecal2->Draw("same");
ecalCan->SetLogy(1);
ecalCan->SaveAs("ecal.gif");

TCanvas* hcalCan = new TCanvas("hcalCan","", 500, 500);
hcal1->Draw();
hcal2->Draw("same");
hcalCan->SetLogy(1);
hcalCan->SaveAs("hcal.gif");

}
