#define kanaelec_cxx
#include "kanaelec.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "Resolution.h"
#include "PhysicsTools/KinFitter/interface/TFitConstraintM.h"
#include "PhysicsTools/KinFitter/interface/TFitConstraintEp.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleEtEtaPhi.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleCart.h"
#include "PhysicsTools/KinFitter/interface/TKinFitter.h"

#include "ElectroWeakAnalysis/VPlusJets/interface/AngularVars.h"


void kanaelec::myana(double myflag)
{
  TChain * myChain;
  // 2011 data
  myflag = -100;
  if (myflag == 20111000 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_DataAll_GoldenJSON_917invpb.root"); 
    Init(myChain);Loop( 2011, 1000,          "./RD_WenuJets_DataAll_GoldenJSON_917invpb.root");
  }

  // General Background Samples
  if (myflag == 20111001 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW423-Summer11MC_WJets.root"); 
    Init(myChain);Loop( 2011, 1001,          "./RD_WenuJets_CMSSW423-Summer11MC_WJets.root");
  }
  if (myflag == 20111002 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW423-Summer11MC_TTToLNu2Q2B.root"); 
    Init(myChain);Loop( 2011, 1002,          "./RD_WenuJets_CMSSW423-Summer11MC_TTToLNu2Q2B.root");
  }
  if (myflag == 20111003 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WZtoAnything.root"); 
    Init(myChain);Loop( 2011, 1003,          "./RD_WenuJets_CMSSW415-Spring11MC_WZtoAnything.root");
  }
  if (myflag == 20111004 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWtoAnything.root"); 
    Init(myChain);Loop( 2011, 1004,          "./RD_WenuJets_CMSSW415-Spring11MC_WWtoAnything.root");
  }
  
  // Higgs Signal Samples
  if (myflag == 20111120 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-120.root"); 
    Init(myChain);Loop( 2011, 1120,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-120.root");
  }
  if (myflag == 20111130 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-130.root"); 
    Init(myChain);Loop( 2011, 1130,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-130.root");
  }
  if (myflag == 20111140 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-140.root"); 
    Init(myChain);Loop( 2011, 1140,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-140.root");
  }
  if (myflag == 20111150 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-150.root"); 
    Init(myChain);Loop( 2011, 1150,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-150.root");
  }
  if (myflag == 20111160 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-160.root"); 
    Init(myChain);Loop( 2011, 1160,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-160.root");
  }
  if (myflag == 20111170 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-170.root"); 
    Init(myChain);Loop( 2011, 1170,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-170.root");
  }
  if (myflag == 20111180 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-180.root"); 
    Init(myChain);Loop( 2011, 1180,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-180.root");
  }
  if (myflag == 20111190 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-190.root"); 
    Init(myChain);Loop( 2011, 1190,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-190.root");
  }
  if (myflag == 20111200 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-200.root"); 
    Init(myChain);Loop( 2011, 1200,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-200.root");
  }
  if (myflag == 20111250 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-250.root"); 
    Init(myChain);Loop( 2011, 1250,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-250.root");
  }
  if (myflag == 20111300 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-300.root"); 
    Init(myChain);Loop( 2011, 1300,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-300.root");
  }
  if (myflag == 20111350 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-350.root"); 
    Init(myChain);Loop( 2011, 1350,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-350.root");
  }
  if (myflag == 20111400 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-400.root"); 
    Init(myChain);Loop( 2011, 1400,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-400.root");
  }
  if (myflag == 20111450 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-450.root"); 
    Init(myChain);Loop( 2011, 1450,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-450.root");
  }
  if (myflag == 20111500 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-500.root"); 
    Init(myChain);Loop( 2011, 1500,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-500.root");
  }
  if (myflag == 20111550 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-550.root"); 
    Init(myChain);Loop( 2011, 1550,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-550.root");
  }
  if (myflag == 20111600 || myflag == -100){
    myChain = new TChain("WJet");  
    myChain->Add("/uscms_data/d2/kalanand/WjjTrees/WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-600.root"); 
    Init(myChain);Loop( 2011, 1600,          "./RD_WenuJets_CMSSW415-Spring11MC_WWToLNuQQ_M-600.root");
  }
}

void kanaelec::Loop(int wda, int wsp, const char *outfilename)
{
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  // Out Put File Here
  TFile fresults = TFile(outfilename,"RECREATE");
  TTree *newtree = fChain->CloneTree();
  
  Float_t fit_mu_px=0,   fit_mu_py =0,   fit_mu_pz=0,   fit_mu_e=0;
  Float_t fit_nv_px=0,   fit_nv_py =0,   fit_nv_pz=0,   fit_nv_e=0;
  Float_t fit_aj_px=0,   fit_aj_py =0,   fit_aj_pz=0,   fit_aj_e=0;
  Float_t fit_bj_px=0,   fit_bj_py =0,   fit_bj_pz=0,   fit_bj_e=0;
  Float_t fit_mlvjj=0,   fit_chi2  =999;  
  Int_t   fit_NDF  =999, fit_status=999, gdevtt   =0;

  TBranch *branch_mu_px = newtree->Branch("fit_el_px", &fit_mu_px,  "fit_el_px/F");
  TBranch *branch_mu_py = newtree->Branch("fit_el_py", &fit_mu_py,  "fit_el_py/F");
  TBranch *branch_mu_pz = newtree->Branch("fit_el_pz", &fit_mu_pz,  "fit_el_pz/F");
  TBranch *branch_mu_e  = newtree->Branch("fit_el_e",  &fit_mu_e,   "fit_el_e/F");
  	   
  TBranch *branch_nv_px = newtree->Branch("fit_nv_px", &fit_nv_px,  "fit_nv_px/F");
  TBranch *branch_nv_py = newtree->Branch("fit_nv_py", &fit_nv_py,  "fit_nv_py/F");
  TBranch *branch_nv_pz = newtree->Branch("fit_nv_pz", &fit_nv_pz,  "fit_nv_pz/F");
  TBranch *branch_nv_e  = newtree->Branch("fit_nv_e",  &fit_nv_e,   "fit_nv_e/F");
  	   
  TBranch *branch_aj_px = newtree->Branch("fit_aj_px", &fit_aj_px,  "fit_aj_px/F");
  TBranch *branch_aj_py = newtree->Branch("fit_aj_py", &fit_aj_py,  "fit_aj_py/F");
  TBranch *branch_aj_pz = newtree->Branch("fit_aj_pz", &fit_aj_pz,  "fit_aj_pz/F");
  TBranch *branch_aj_e  = newtree->Branch("fit_aj_e",  &fit_aj_e,   "fit_aj_e/F");
  	   
  TBranch *branch_bj_px = newtree->Branch("fit_bj_px", &fit_bj_px,  "fit_bj_px/F");
  TBranch *branch_bj_py = newtree->Branch("fit_bj_py", &fit_bj_py,  "fit_bj_py/F");
  TBranch *branch_bj_pz = newtree->Branch("fit_bj_pz", &fit_bj_pz,  "fit_bj_pz/F");
  TBranch *branch_bj_e  = newtree->Branch("fit_bj_e",  &fit_bj_e,   "fit_bj_e/F");
  
  TBranch *branch_mlvjj = newtree->Branch("fit_mlvjj", &fit_mlvjj,  "fit_mlvjj/F");
  TBranch *branch_chi2  = newtree->Branch("fit_chi2",  &fit_chi2,   "fit_chi2/F");
  TBranch *branch_NDF   = newtree->Branch("fit_NDF",   &fit_NDF,    "fit_NDF/I");
  TBranch *branch_status= newtree->Branch("fit_status",&fit_status, "fit_status/I");
  TBranch *branch_gdevtt= newtree->Branch("gdevtt",    &gdevtt,     "gdevtt/I");

  Float_t fi2_mu_px=0,   fi2_mu_py =0,   fi2_mu_pz=0,   fi2_mu_e=0;
  Float_t fi2_nv_px=0,   fi2_nv_py =0,   fi2_nv_pz=0,   fi2_nv_e=0;
  Float_t fi2_aj_px=0,   fi2_aj_py =0,   fi2_aj_pz=0,   fi2_aj_e=0;
  Float_t fi2_bj_px=0,   fi2_bj_py =0,   fi2_bj_pz=0,   fi2_bj_e=0;
  Float_t fi2_mlvjj=0,   fi2_chi2  =999;  
  Int_t   fi2_NDF  =999, fi2_status=999;

  TBranch *branc2_mu_px = newtree->Branch("fi2_el_px", &fi2_mu_px,  "fi2_el_px/F");
  TBranch *branc2_mu_py = newtree->Branch("fi2_el_py", &fi2_mu_py,  "fi2_el_py/F");
  TBranch *branc2_mu_pz = newtree->Branch("fi2_el_pz", &fi2_mu_pz,  "fi2_el_pz/F");
  TBranch *branc2_mu_e  = newtree->Branch("fi2_el_e",  &fi2_mu_e,   "fi2_el_e/F");
  	   
  TBranch *branc2_nv_px = newtree->Branch("fi2_nv_px", &fi2_nv_px,  "fi2_nv_px/F");
  TBranch *branc2_nv_py = newtree->Branch("fi2_nv_py", &fi2_nv_py,  "fi2_nv_py/F");
  TBranch *branc2_nv_pz = newtree->Branch("fi2_nv_pz", &fi2_nv_pz,  "fi2_nv_pz/F");
  TBranch *branc2_nv_e  = newtree->Branch("fi2_nv_e",  &fi2_nv_e,   "fi2_nv_e/F");
  	   
  TBranch *branc2_aj_px = newtree->Branch("fi2_aj_px", &fi2_aj_px,  "fi2_aj_px/F");
  TBranch *branc2_aj_py = newtree->Branch("fi2_aj_py", &fi2_aj_py,  "fi2_aj_py/F");
  TBranch *branc2_aj_pz = newtree->Branch("fi2_aj_pz", &fi2_aj_pz,  "fi2_aj_pz/F");
  TBranch *branc2_aj_e  = newtree->Branch("fi2_aj_e",  &fi2_aj_e,   "fi2_aj_e/F");
  	   
  TBranch *branc2_bj_px = newtree->Branch("fi2_bj_px", &fi2_bj_px,  "fi2_bj_px/F");
  TBranch *branc2_bj_py = newtree->Branch("fi2_bj_py", &fi2_bj_py,  "fi2_bj_py/F");
  TBranch *branc2_bj_pz = newtree->Branch("fi2_bj_pz", &fi2_bj_pz,  "fi2_bj_pz/F");
  TBranch *branc2_bj_e  = newtree->Branch("fi2_bj_e",  &fi2_bj_e,   "fi2_bj_e/F");
  
  TBranch *branc2_mlvjj = newtree->Branch("fi2_mlvjj", &fi2_mlvjj,  "fi2_mlvjj/F");
  TBranch *branc2_chi2  = newtree->Branch("fi2_chi2",  &fi2_chi2,   "fi2_chi2/F");
  TBranch *branc2_NDF   = newtree->Branch("fi2_NDF",   &fi2_NDF,    "fi2_NDF/I");
  TBranch *branc2_status= newtree->Branch("fi2_status",&fi2_status, "fi2_status/I");

  // Loop over all events
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //Long64_t ientry = LoadTree(jentry);
    //if (ientry < 0) break;
    nb = newtree->GetEntry(jentry);   nbytes += nb;
    // Cut variable definitions
    double jess    = 1.00; // control the jet energy scale
    //double muoniso = (W_muon_trackiso+W_muon_hcaliso+W_muon_ecaliso-event_RhoForLeptonIsolation*3.141592653589*0.09)/W_muon_pt;
    double dijetpt = sqrt(JetPFCor_Pt[0]*JetPFCor_Pt[0]+
			  JetPFCor_Pt[1]*JetPFCor_Pt[1]+
			  2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]));

    // Save variable initialization
    fit_mu_px = 0; fit_mu_py = 0; fit_mu_pz = 0;  fit_mu_e  = 0; 
    fit_nv_px = 0; fit_nv_py = 0; fit_nv_pz = 0;  fit_nv_e  = 0; 
    fit_aj_px = 0; fit_aj_py = 0; fit_aj_pz = 0;  fit_aj_e  = 0; 
    fit_bj_px = 0; fit_bj_py = 0; fit_bj_pz = 0;  fit_bj_e  = 0; 
    fit_mlvjj = 0; fit_chi2  =999;fit_NDF   =999; fit_status=999;

    gdevtt    = 0;

    fi2_mu_px = 0; fi2_mu_py = 0; fi2_mu_pz = 0;  fi2_mu_e  = 0; 
    fi2_nv_px = 0; fi2_nv_py = 0; fi2_nv_pz = 0;  fi2_nv_e  = 0; 
    fi2_aj_px = 0; fi2_aj_py = 0; fi2_aj_pz = 0;  fi2_aj_e  = 0; 
    fi2_bj_px = 0; fi2_bj_py = 0; fi2_bj_pz = 0;  fi2_bj_e  = 0; 
    fi2_mlvjj = 0; fi2_chi2  =999;fi2_NDF   =999; fi2_status=999;

    // Good Event Selection Requirement
    if (JetPFCor_Pt[0]>30. 
	&& JetPFCor_Pt[1]>30. 
	&& JetPFCor_Pt[2]<0.5 
	&& abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.5 
	&& W_electron_et>30. 
	&& W_electron_isWP80==1 
	&& (W_electron_trackiso+W_electron_hcaliso+W_electron_ecaliso-event_RhoForLeptonIsolation*3.141592653589*0.09)/W_electron_pt<0.05 
	&& ((abs(W_electron_eta)<1.5 && abs(W_electron_deltaphi_in)<0.03 && abs(W_electron_deltaeta_in)<0.004) || (abs(W_electron_eta)>1.5 && abs(W_electron_deltaphi_in)<0.02 && abs(W_electron_deltaeta_in)<0.005)) 
	&& sqrt((W_electron_vx-event_BeamSpot_x)*(W_electron_vx-event_BeamSpot_x)+(W_electron_vy-event_BeamSpot_y)*(W_electron_vy-event_BeamSpot_y))<0.02 
	&& JetPFCor_bDiscriminator[0]<1.19 
	&& JetPFCor_bDiscriminator[1]<1.19 
	&& cosJacksonAngle2j_PFCor<0.8 
	&& cosJacksonAngle2j_PFCor>-0.6 
	&& event_met_pfmet>30. 
	&& W_mt>40.
	&& dijetpt > 40.	)      {
      gdevtt = 1;
      // Do the kinematic fit for all event!!!
      int Aj = 0, Bj = 1;    TLorentzVector  mup, nvp, ajp, bjp; 
      mup.SetPtEtaPhiE(W_electron_pt,      W_electron_eta,   W_electron_phi,   W_electron_e               );
      nvp.SetPxPyPzE(event_met_pfmet * cos(event_met_pfmetPhi), event_met_pfmet * sin(event_met_pfmetPhi), 
		     W_pzNu1, sqrt(event_met_pfmet*event_met_pfmet + W_pzNu1*W_pzNu1)                     );
      ajp.SetPtEtaPhiE(jess * JetPFCor_Pt[Aj], JetPFCor_Eta[Aj], JetPFCor_Phi[Aj], jess * JetPFCor_E[Aj]  );
      bjp.SetPtEtaPhiE(jess * JetPFCor_Pt[Bj], JetPFCor_Eta[Bj], JetPFCor_Phi[Bj], jess * JetPFCor_E[Bj]  );
      
      TLorentzVector fit_mup(0,0,0,0), fit_nvp(0,0,0,0), fit_ajp(0,0,0,0), fit_bjp(0,0,0,0) ; 
      doKinematicFit( 1, mup, nvp, ajp, bjp,  fit_mup, fit_nvp, fit_ajp, fit_bjp, fit_chi2, fit_NDF, fit_status);
      fit_mu_px = fit_mup.Px(); fit_mu_py = fit_mup.Py(); fit_mu_pz = fit_mup.Pz(); fit_mu_e = fit_mup.E(); 
      fit_nv_px = fit_nvp.Px(); fit_nv_py = fit_nvp.Py(); fit_nv_pz = fit_nvp.Pz(); fit_nv_e = fit_nvp.E(); 
      fit_aj_px = fit_ajp.Px(); fit_aj_py = fit_ajp.Py(); fit_aj_pz = fit_ajp.Pz(); fit_aj_e = fit_ajp.E(); 
      fit_bj_px = fit_bjp.Px(); fit_bj_py = fit_bjp.Py(); fit_bj_pz = fit_bjp.Pz(); fit_bj_e = fit_bjp.E(); 
      fit_mlvjj = (fit_mup+fit_nvp+fit_ajp+fit_bjp).M();

      TLorentzVector fi2_mup(0,0,0,0), fi2_nvp(0,0,0,0), fi2_ajp(0,0,0,0), fi2_bjp(0,0,0,0) ; 
      doKinematicFit( 2, mup, nvp, ajp, bjp,  fi2_mup, fi2_nvp, fi2_ajp, fi2_bjp, fi2_chi2, fi2_NDF, fi2_status);
      fi2_mu_px = fi2_mup.Px(); fi2_mu_py = fi2_mup.Py(); fi2_mu_pz = fi2_mup.Pz(); fi2_mu_e = fi2_mup.E(); 
      fi2_nv_px = fi2_nvp.Px(); fi2_nv_py = fi2_nvp.Py(); fi2_nv_pz = fi2_nvp.Pz(); fi2_nv_e = fi2_nvp.E(); 
      fi2_aj_px = fi2_ajp.Px(); fi2_aj_py = fi2_ajp.Py(); fi2_aj_pz = fi2_ajp.Pz(); fi2_aj_e = fi2_ajp.E(); 
      fi2_bj_px = fi2_bjp.Px(); fi2_bj_py = fi2_bjp.Py(); fi2_bj_pz = fi2_bjp.Pz(); fi2_bj_e = fi2_bjp.E(); 
      fi2_mlvjj = (fi2_mup+fi2_nvp+fi2_ajp+fi2_bjp).M();
    }	 // Only Good event will be performed kinematic fit
    branch_mu_px->Fill();
    branch_mu_py->Fill();
    branch_mu_pz->Fill();
    branch_mu_e ->Fill();
    
    branch_nv_px->Fill();
    branch_nv_py->Fill();
    branch_nv_pz->Fill();
    branch_nv_e ->Fill();
    
    branch_aj_px->Fill();
    branch_aj_py->Fill();
    branch_aj_pz->Fill();
    branch_aj_e ->Fill();
    
    branch_bj_px->Fill();
    branch_bj_py->Fill();
    branch_bj_pz->Fill();
    branch_bj_e ->Fill();
   
    branch_mlvjj->Fill();
    branch_chi2 ->Fill();
    branch_NDF  ->Fill();
    branch_status->Fill();

    branch_gdevtt->Fill();

    branc2_mu_px->Fill();
    branc2_mu_py->Fill();
    branc2_mu_pz->Fill();
    branc2_mu_e ->Fill();
    
    branc2_nv_px->Fill();
    branc2_nv_py->Fill();
    branc2_nv_pz->Fill();
    branc2_nv_e ->Fill();
    
    branc2_aj_px->Fill();
    branc2_aj_py->Fill();
    branc2_aj_pz->Fill();
    branc2_aj_e ->Fill();
    
    branc2_bj_px->Fill();
    branc2_bj_py->Fill();
    branc2_bj_pz->Fill();
    branc2_bj_e ->Fill();
   
    branc2_mlvjj->Fill();
    branc2_chi2 ->Fill();
    branc2_NDF  ->Fill();
    branc2_status->Fill();
  } // end event loop
  fresults.cd();
  newtree->Write("WJet");
  fresults.Close();
  std::cout <<  wda << "-" << wsp << "   ::   " << outfilename << "    "<< nentries  << std::endl;
  
}


bool kanaelec::doKinematicFit(Int_t                 fflage,
			      const TLorentzVector     mup, 
			      const TLorentzVector     nvp, 
			      const TLorentzVector     ajp, 
			      const TLorentzVector     bjp, 
			      TLorentzVector     & fit_mup, 
			      TLorentzVector     & fit_nvp,
			      TLorentzVector     & fit_ajp, 
			      TLorentzVector     & fit_bjp, 
			      Float_t            & fit_chi2,
			      Int_t              & fit_NDF, 
			      Int_t              & fit_status)
{

  bool OK                     = false;
  Resolution* resolution      = new Resolution();

  TMatrixD m1(3,3);
  TMatrixD m2(3,3);
  TMatrixD m3(3,3);
  TMatrixD m4(3,3);
  m1.Zero();
  m2.Zero();
  m3.Zero();
  m4.Zero();

  double etRes, etaRes, phiRes;
  // lepton resolution
  const std::string& leptonName = "electron";  const TLorentzVector lepton   = mup;
  if(leptonName == "electron") {
    OK = resolution->electronResolution(lepton.Et(), lepton.Eta(), etRes, etaRes, phiRes);
    if(!OK) return OK;
  } else {
    OK = resolution->muonResolution(    lepton.Et(), lepton.Eta(), etRes, etaRes, phiRes);
    if(!OK) return OK;
  }
  m1(0,0) = resolution->square(etRes);
  m1(1,1) = resolution->square(etaRes);
  m1(2,2) = resolution->square(phiRes);
  // MET resolution
  OK = resolution->PFMETResolution(     nvp.Et(),            etRes, etaRes, phiRes);
  if(!OK) return OK;
  m2(0,0) = resolution->square(etRes);
  m2(1,1) = 999.9; // resolution->square(etaRes)
  m2(2,2) = resolution->square(phiRes);
  // Leading Jet resolution
  OK = resolution->udscPFJetResolution( ajp.Et(), ajp.Eta(), etRes, etaRes, phiRes);
  if(!OK) return OK;
  m3(0,0) = resolution->square(etRes);
  m3(1,1) = resolution->square(etaRes);
  m3(2,2) = resolution->square(phiRes);
  // Leading Jet resolution
  OK = resolution->udscPFJetResolution( bjp.Et(), bjp.Eta(), etRes, etaRes, phiRes);
  if(!OK) return OK;
  m4(0,0) = resolution->square(etRes);
  m4(1,1) = resolution->square(etaRes);
  m4(2,2) = resolution->square(phiRes);

  TLorentzVector tmp_mup = mup;
  TLorentzVector tmp_nvp = nvp;
  TLorentzVector tmp_ajp = ajp;
  TLorentzVector tmp_bjp = bjp;

  // Fit Particle
  TFitParticleEtEtaPhi* particle1 = new TFitParticleEtEtaPhi( "Lepton",   "Lepton",   &tmp_mup,    &m1 );
  TFitParticleEtEtaPhi* particle2 = new TFitParticleEtEtaPhi( "Neutrino", "Neutrino", &tmp_nvp,    &m2 );
  TFitParticleEtEtaPhi* particle3 = new TFitParticleEtEtaPhi( "Jeta",     "Jeta",     &tmp_ajp,    &m3 );
  TFitParticleEtEtaPhi* particle4 = new TFitParticleEtEtaPhi( "Jetb",     "Jetb",     &tmp_bjp,    &m4 );

  // Constraint
  TFitConstraintM *mCons1 = new TFitConstraintM( "WMassConstrainta", "WMass-Constrainta", 0, 0 , 80.4);
  mCons1->addParticles1( particle1, particle2 );

  TFitConstraintM *mCons2 = new TFitConstraintM( "WMassConstraintb", "WMass-Constraintb", 0, 0 , 80.4);
  mCons2->addParticles1( particle3, particle4 );

  TFitConstraintEp *pxCons = new TFitConstraintEp( "PxConstraint", "Px-Constraint", 0, TFitConstraintEp::pX , (mup+nvp+ajp+bjp).Px() );
  pxCons->addParticles( particle1, particle2, particle3, particle4 );

  TFitConstraintEp *pyCons = new TFitConstraintEp( "PyConstraint", "Py-Constraint", 0, TFitConstraintEp::pY , (mup+nvp+ajp+bjp).Py() );
  pyCons->addParticles( particle1, particle2, particle3, particle4 );

  //Definition of the fitter
  TKinFitter* fitter = new TKinFitter("fitter", "fitter");
  if        (fflage == 1 ){
    fitter->addMeasParticle( particle1 );
    fitter->addMeasParticle( particle2 );
    fitter->addMeasParticle( particle3 );
    fitter->addMeasParticle( particle4 );
    fitter->addConstraint( pxCons );
    fitter->addConstraint( pyCons );
    fitter->addConstraint( mCons1 );
    fitter->addConstraint( mCons2 );
  }else   if(fflage == 2 ){
    fitter->addMeasParticle( particle1 );
    fitter->addMeasParticle( particle2 );
    fitter->addConstraint( mCons1 );
  }else {return false;}

  //Set convergence criteria
  fitter->setMaxNbIter( 30 );
  fitter->setMaxDeltaS( 1e-2 );
  fitter->setMaxF( 1e-1 );
  fitter->setVerbosity(1);
  fitter->fit();

  //Return the kinematic fit results
  fit_status   = fitter->getStatus();
  fit_chi2     = fitter->getS();
  fit_NDF      = fitter->getNDF();
  fit_mup      = *(particle1->getCurr4Vec()); 
  fit_nvp      = *(particle2->getCurr4Vec()); 
  fit_ajp      = *(particle3->getCurr4Vec()); 
  fit_bjp      = *(particle4->getCurr4Vec()); 
  
  if(fitter->getStatus() == 0) { OK = true;  } else { OK = false;  }
  delete resolution;
  delete particle1;
  delete particle2;
  delete particle3;
  delete particle4;
  delete mCons1;
  delete mCons2;
  delete pxCons;
  delete pyCons;
  delete fitter;

  return OK;
}
