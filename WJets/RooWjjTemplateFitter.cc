#include "TROOT.h"
#include "TTree.h"
#include "TCut.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TAxis.h"

#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooArgSet.h"
#include "RooPlot.h"
#include "RooArgList.h"
#include "RooAddPdf.h"
#include "RooExponential.h"
#include "RooBifurGauss.h"
#include "RooCBShape.h"
#include "RooHist.h"
#include "RooFitResult.h"
#include "RooChebyshevPDF.h"


char const * jetCut = "abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<2.5 && "
  "sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))>40. && "
"abs(abs(abs(JetPFCor_Phi[0]-event_met_pfmetPhi)-TMath::Pi())-TMath::Pi())>0.5 && "
  "event_met_pfmet>25. && W_mt>40. && JetPFCor_Pt[0]>30."
  " && JetPFCor_Pt[1]>30. && JetPFCor_Pt[2]<30."
  " && JetPFCor_bDiscriminator[0]<1.19"
  " && JetPFCor_bDiscriminator[1]<1.19";

//   " && abs(abs(abs(JetPFCor_Phi[0]-event_met_pfmetPhi)-TMath::Pi())-TMath::Pi()) >0.5"
//    " && abs(abs(abs(W_phi-atan2((JetPFCor_Py[0]+JetPFCor_Py[1]),(JetPFCor_Px[0]+JetPFCor_Px[1])))-TMath::Pi())-TMath::Pi())>2.74";
      
// "sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*cos(abs(abs(abs(JetPFCor_Phi[0]-JetPFCor_Phi[1])-TMath::Pi())-TMath::Pi())) ) >50 && "

//   "sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(abs(abs(abs(JetPFCor_Phi[0]-JetPFCor_Phi[1])-TMath::Pi())-TMath::Pi()))) >30. && "
//"abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.5 &&"

char const * muCut = "W_muon_pt>25."
   " && (W_muon_trackiso+W_muon_hcaliso+W_muon_ecaliso-event_RhoForLeptonIsolation*3.141592653589*0.09)/W_muon_pt<0.1"
   " && abs(W_muon_d0bsp)<0.02";

char const * eleCut = "W_electron_et>30."
   " && (W_electron_trackiso+W_electron_hcaliso+W_electron_ecaliso-event_RhoForLeptonIsolation*3.141592653589*0.09)/W_electron_pt<0.05"
   " && W_electron_isWP80"
   " && ((abs(W_electron_eta)<1.5 &&"
   " abs(W_electron_deltaphi_in)<0.03"
   " && abs(W_electron_deltaeta_in)<0.004)"
   " || (abs(W_electron_eta)>1.5"
   " && abs(W_electron_deltaphi_in)<0.02"
   " && abs(W_electron_deltaeta_in)<0.005))"
   " && sqrt((W_electron_vx-event_BeamSpot_x)**2+(W_electron_vy-event_BeamSpot_y)**2)<0.02";

/*
muCut = "W_muon_pt>25."
   " && (W_muon_trackiso+W_muon_hcaliso+W_muon_ecaliso-event_RhoForLeptonIsolation*3.141592653589*0.09)/W_muon_pt<0.1"
   " && abs(abs(abs(JetPFCor_Phi[0]-(W_muon_phi)-TMath::Pi())-TMath::Pi()))>2.1"
   " && abs(abs(abs(JetPFCor_Phi[1]-(W_muon_phi)-TMath::Pi())-TMath::Pi()))>2.3"
   " && abs(W_muon_d0bsp)<0.02";

eleCut = "W_electron_et>30."
   " && (W_electron_trackiso+W_electron_hcaliso+W_electron_ecaliso-event_RhoForLeptonIsolation*3.141592653589*0.09)/W_electron_pt<0.05"
   " && W_electron_isWP80 && ((abs(W_electron_eta)<1.5"
   " && abs(W_electron_deltaphi_in)<0.03"
   " && abs(W_electron_deltaeta_in)<0.004)"
   " || (abs(W_electron_eta)>1.5 && abs(W_electron_deltaphi_in)<0.02"
   " && abs(W_electron_deltaeta_in)<0.005))"
   " && abs(abs(abs(JetPFCor_Phi[0]-(W_electron_phi)-TMath::Pi())-TMath::Pi()))>1.8"
   " && abs(abs(abs(JetPFCor_Phi[1]-(W_electron_phi)-TMath::Pi())-TMath::Pi()))>2."
   " && sqrt((W_electron_vx-event_BeamSpot_x)**2+(W_electron_vy-event_BeamSpot_y)**2)<0.02";
*/

void RooWjjTemplateFitter(TString PLOTVAR = "MassV2j_PFCor", 
			  TString mCut = "") {

  TCut* jetsSel;
  TCut* mu_selection;
  TCut* ele_selection;

  jetsSel = new TCut (jetCut);
  mu_selection = new TCut ( (*jetsSel) && TCut(muCut) );
  ele_selection = new TCut ( (*jetsSel) && TCut(eleCut) );
  
  TCut massCut = mCut.Data();
  
  *mu_selection = TCut( *mu_selection && massCut);
  *ele_selection = TCut( *ele_selection && massCut);

  TTree * treeTemp;
  TFile wjetsShape_mu_file("/uscms_data/d2/kalanand/WjjTrees/WmunuJets_CMSSW415-Spring11MC_WJets.root", "READ");
  treeTemp = (TTree*) wjetsShape_mu_file.Get("WJet");
  //ActivateTreeBranches(*treeTemp);
  gROOT->cd();
  TTree* tree1 = treeTemp->CopyTree(*mu_selection);


  TFile wjetsShape_ele_file("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WJets.root", "READ");
  treeTemp = (TTree*) wjetsShape_ele_file.Get("WJet");
  //ActivateTreeBranches(*treeTemp, true);
  gROOT->cd();
  TTree* tree2 = treeTemp->CopyTree(*ele_selection);
  RooRealVar mVjj(PLOTVAR, "m_{jj}", 20.0, 400.0, "GeV/c^{2}");
  RooCategory channel("channel", "channel");
  channel.defineType("muon");
  channel.defineType("electron");

  RooDataSet * edata = new RooDataSet("edata", "edata", RooArgSet(mVjj),
				      RooFit::Import(*tree2));
  RooDataSet * mdata = new RooDataSet("mdata", "mdata", RooArgSet(mVjj),
				      RooFit::Import(*tree1));
  wjetsShape_mu_file.Close();
  wjetsShape_ele_file.Close();
  delete tree1;
  delete tree2;

  edata->Print();
  mdata->Print();
  RooDataSet wjetsData("wjetsData", "wjetsData", RooArgSet(mVjj),
		       RooFit::Index(channel),
		       RooFit::Import("muon", *mdata), 
		       RooFit::Import("electron", *edata));
  delete edata;
  delete mdata;
  wjetsData.Print("v");

  //Build pdf
  RooRealVar sigmaL1("sigmaL1", "#sigma_{L,1}", 22., 0., 1000., "GeV/c^{2}");
  sigmaL1.setPlotLabel(sigmaL1.GetTitle());
  RooRealVar sigmaR1("sigmaR1", "#sigma_{R,1}", 90., 0., 1000., "GeV/c^{2}");
  sigmaR1.setPlotLabel(sigmaR1.GetTitle());
  RooRealVar mean1("mean1", "<m>_{1}", 350., 0., 1000., "GeV/c^{2}");
  mean1.setPlotLabel(mean1.GetTitle());

  RooRealVar sigmaL2("sigmaL2", "#sigma_{L,2}", 22., 0., 2000., "GeV/c^{2}");
  sigmaL2.setPlotLabel(sigmaL2.GetTitle());
  RooRealVar sigmaR2("sigmaR2", "#sigma_{R,2}", 150., 0., 2000., "GeV/c^{2}");
  sigmaR2.setPlotLabel(sigmaR2.GetTitle());
  RooRealVar mean2("mean2", "<m>_{2}", 375, 0., 2000., "GeV/c^{2}");
  mean2.setPlotLabel(mean2.GetTitle());

  RooRealVar sigmaL3("sigmaL3", "#sigma_{L,3}", 20., 0., 3000., "GeV/c^{2}");
  sigmaL3.setPlotLabel(sigmaL3.GetTitle());
  RooRealVar sigmaR3("sigmaR3", "#sigma_{R,3}", 300., 0., 3000., "GeV/c^{2}");
  sigmaR3.setPlotLabel(sigmaR3.GetTitle());
  RooRealVar mean3("mean3", "<m>_{3}", 250., 0., 3000., "GeV/c^{2}");
  mean3.setPlotLabel(mean3.GetTitle());

  // RooArgList coefs("coefs");
  // for(int c = 1; c <= 14; ++c) {
  //   RooRealVar cn(TString::Format("c%i", c), TString::Format("c_{%i}", c),
  // 		  0., -10., 10.);
  //   cn.setPlotLabel(cn.GetTitle());
  //   cn.setError(0.01);
  //   coefs.addClone(cn);
  // }

  // coefs.Print("v");

  RooBifurGauss g1("g1", "g1", mVjj, mean1, sigmaL1, sigmaR1);
  RooBifurGauss g2("g2", "g2", mVjj, mean2, sigmaL2, sigmaR2);
  RooBifurGauss g3("g3", "g3", mVjj, mean2, sigmaL2, sigmaR3);

  RooRealVar fg1("fg1", "f_{g1}", 0.4, 0., 1.);
  fg1.setPlotLabel(fg1.GetTitle());
  RooRealVar fg2("fg2", "f_{g2}", 0.65, 0., 1.);
  fg2.setPlotLabel(fg2.GetTitle());
  
  RooAddPdf sum("sum", "sum", RooArgList(g1, g2,g3), 
  		RooArgList(fg1,fg2), true);
  // RooChebyshevPDF sum("sum", "sum", mVjj, coefs);

  RooArgSet * params = sum.getParameters(RooArgSet(mVjj,channel));
  params->readFromFile("initialParams.txt");
  
  RooFitResult * fr = sum.fitTo(wjetsData, RooFit::Minos(false),
				RooFit::Save(true));
  
  RooPlot * mf = mVjj.frame(RooFit::Name(PLOTVAR+"_plot"),RooFit::Bins(38));
  wjetsData.plotOn(mf, RooFit::Name("data_pts"));
  sum.plotOn(mf, RooFit::Name("wjets_shape"));
  //sum.paramOn(mf);
  //g2.plotOn(mf);

  RooHist * resHist = mf->residHist("data_pts", "wjets_shape");
  

  TCanvas * c1 = new TCanvas("shape", "shape");
  c1->cd();
  mf->Draw();
  TCanvas * c2 = new TCanvas("residuals", "residuals");
  c2->cd();
  resHist->Draw("ap");
  c2->SetGridy();
  resHist->GetXaxis()->SetTitle(mVjj.GetTitle());
  resHist->GetYaxis()->SetTitle("Fit residuals");
  c2->Modified();

  c1->Print(PLOTVAR + "-MC-template-fit.png");
  c1->Print(PLOTVAR + "-MC-template-fit.pdf");
  c1->Print(PLOTVAR + "-MC-template-fit.eps");
  c2->Print(PLOTVAR + "-MC-template-fit-residuals.png");
  c2->Print(PLOTVAR + "-MC-template-fit-residuals.pdf");
  c2->Print(PLOTVAR + "-MC-template-fit-residuals.eps");


  params->writeToFile("lastWjjTemplateFit.txt");
  fr->Print();
  delete params;
}
