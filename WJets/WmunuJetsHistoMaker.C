#define WmunuJetsHistoMaker_cxx
#include "WmunuJetsHistoMaker.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "TLorentzVector.h"
#include "AngularVars.h"

void WmunuJetsHistoMaker::Loop(int wda, int wsp,
		const char *outfilename)
{
//   In a ROOT session, you can do:
//      Root > .L WmunuJetsHistoMaker.C
//      Root > WmunuJetsHistoMaker t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   // Out Put File Here
   TFile fresults = TFile(outfilename,"RECREATE");
   // Out Put Histogram Here!
   TH1F *hh_btag_SSVHE       = new TH1F("hh_btag_SSVHE",         "",    100,  -10,    10);

   TH1F *hh_ttb_mjj_all      = new TH1F("hh_ttb_mjj_all",        "",     45,    40,   400);
   TH1F *hh_ttb_mjj_nob      = new TH1F("hh_ttb_mjj_nob",        "",     45,    40,   400);

   TH1F *hh_lvjj_mjj         = new TH1F("hh_lvjj_mjj",           "",     45,    40,   400);
   TH1F *hh_lvjj_mlvjj       = new TH1F("hh_lvjj_mlvjj",         "",     30,    150,  600);

   TH1F *hh_lvjj_cosanwinww  = new TH1F("hh_lvjj_cosanwinww",    "",    10,   -1,     1);
   TH1F *hh_lvjj_phipl       = new TH1F("hh_lvjj_phipl",         "",    40,   -1,     1);
   TH1F *hh_lvjj_ctuv        = new TH1F("hh_lvjj_ctuv",          "",    20,   -1,     1);
   TH1F *hh_lvjj_ctjj        = new TH1F("hh_lvjj_ctjj",          "",    17,   -0.6,   1.1);
   TH1F *hh_lvjj_jac         = new TH1F("hh_lvjj_jac",           "",    12,    0,     0.6);

   TH1F *hh_lvjj_jjcentral   = new TH1F("hh_lvjj_jjcentral",     "",    20,  0.0,   1.0);
   TH1F *hh_lvjj_jjdphi      = new TH1F("hh_lvjj_jjdphi",        "",    32,  0.0,   3.141593);
   TH1F *hh_lvjj_ljmetdphi   = new TH1F("hh_lvjj_ljmetdphi",     "",    32,  0.0,   3.141593);
   TH1F *hh_lvjj_wwdphi      = new TH1F("hh_lvjj_wwdphi",        "",    25,  2.15,   3.15);

   TH1F *hh_lvjj_dgmjj       = new TH1F("hh_lvjj_dgmjj",         "",     45,    40,    400);
   TH1F *hh_lvjj_dgmlvjj     = new TH1F("hh_lvjj_dgmlvjj",       "",     30,    150,   600);
								 
   TH1F *hh_lvjj_kkmjj       = new TH1F("hh_lvjj_kkmjj",         "",     45,    40,    400);
   TH1F *hh_lvjj_kkmlvjj     = new TH1F("hh_lvjj_kkmlvjj",       "",     30,    150,   600);

   const double btssvloose = 1.19;
   const double btssvmdium = 1.93;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   
      // Setup according to different data and mc samples
      double muiso = 999; // Different relative isolation for 2011 and 2010 data
      double jess  = 1.0; // Can use different jet energy scale factor for 2011 and 2010 or even on different MC samples
      if ( wda == 2011 )  { 
         muiso = (W_muon_trackiso+W_muon_hcaliso+W_muon_ecaliso-event_RhoForLeptonIsolation*3.141592653589*0.09)/W_muon_pt;
         event_met_pfmet -= 0.5*(event_nPV-1); // MET has a slope of 0.5 GeV/nPV
      }
      if ( wda == 2010 )  muiso = (W_muon_trackiso+W_muon_hcaliso+W_muon_ecaliso)/W_muon_pt;

      if ( wda == 2011 && wsp == 0 ) jess =1.00;
      if ( wda == 2010 && wsp == 0 ) jess =1.00;


      // -----CUT! Good Muon,
      if ( !( event_met_pfmet>25. 
	      && W_muon_pt>25. 
          && fabs(W_muon_eta)<2.4 
	      && muiso<0.15
	      ) ) continue;

      // ----- Count pfJet Btaging
      const unsigned int total_PFjetss = 6;
      int * tbtagloose = new int[total_PFjetss];
      int * tbtagmdium = new int[total_PFjetss];
      for ( size_t ijet=0; ijet < total_PFjetss; ++ijet) {
	//cout << "pfJet pt : " << PFjetss[ijet].pt << endl;
	tbtagloose[ijet] = 0;
	tbtagmdium[ijet] = 0;
	if (JetPFCor_bDiscriminator[ijet]>btssvloose) {tbtagloose[ijet] = 1;}
	else{tbtagloose[ijet] = 0;}
	if (JetPFCor_bDiscriminator[ijet]>btssvmdium) {tbtagmdium[ijet] = 1;}
	else{tbtagmdium[ijet] = 0;}
	hh_btag_SSVHE->Fill(JetPFCor_bDiscriminator[ijet]);
      }

      // ----- TTbar event selection to study jet energy scale
      if (numPFCorJets>3                                                     // n jet >=4
	  && JetPFCor_Pt[3]>25                                               // jet pt > 25
	  && (tbtagmdium[0]+tbtagmdium[1]+tbtagmdium[2]+tbtagmdium[3]>0.5)   // at least one bjet
	  ) {
	for ( size_t Aj=0; Aj < 4; ++Aj) {
	  for ( size_t Bj=Aj+1; Bj < 4; ++Bj) {
	    TLorentzVector ajp;
	    TLorentzVector bjp;
	    ajp.SetPtEtaPhiE(JetPFCor_Pt[Aj], JetPFCor_Eta[Aj], JetPFCor_Phi[Aj], JetPFCor_E[Aj]);
	    bjp.SetPtEtaPhiE(JetPFCor_Pt[Bj], JetPFCor_Eta[Bj], JetPFCor_Phi[Bj], JetPFCor_E[Bj]);
	    double massjj = (ajp+bjp).M();
	    hh_ttb_mjj_all->Fill(massjj);
	    if(tbtagmdium[Aj]<0.5 && tbtagmdium[Bj]<0.5) 	  hh_ttb_mjj_nob->Fill(massjj);
	  }
	}
      }
      // ----- WuvWjj event selection, basic event seletion : two no-btag jets with pt>20 GeV
      if (   JetPFCor_Pt[0] > 20
	  && JetPFCor_Pt[1] > 20
	  && JetPFCor_Pt[2] < 20
      && JetPFCor_NeutralHadronEnergyFrac[0]<0.9
      && JetPFCor_NeutralEmEnergyFrac[0]<0.9
      && JetPFCor_ChargedHadronEnergyFrac[0]>0.1
      && JetPFCor_ChargedEmEnergyFrac[0]<0.9
      && JetPFCor_NeutralHadronEnergyFrac[1]<0.9
      && JetPFCor_NeutralEmEnergyFrac[1]<0.9
      && JetPFCor_ChargedHadronEnergyFrac[1]>0.1
      && JetPFCor_ChargedEmEnergyFrac[1]<0.9
      && JetPFCor_ElectronEnergyFraction[0]<0.1
      && JetPFCor_MuonEnergyFraction[0]<0.1
      && JetPFCor_ElectronEnergyFraction[1]<0.1
      && JetPFCor_MuonEnergyFraction[1]<0.1
	  && tbtagmdium[0]  < 0.5
	  && tbtagmdium[1]  < 0.5
	  ) {
         size_t Aj = 0; 
         size_t Bj = 1;
	TLorentzVector ajp, bjp, mup, nvp; 	// Here is the lorentz vector for muon, neutrino, lead jet, and next to lead jet.
	ajp.SetPtEtaPhiE(jess * JetPFCor_Pt[Aj], JetPFCor_Eta[Aj], JetPFCor_Phi[Aj], jess * JetPFCor_E[Aj]);
	bjp.SetPtEtaPhiE(jess * JetPFCor_Pt[Bj], JetPFCor_Eta[Bj], JetPFCor_Phi[Bj], jess * JetPFCor_E[Bj]);
	mup.SetPtEtaPhiE(W_muon_pt,              W_muon_eta,       W_muon_phi,       W_muon_e             );
	nvp.SetXYZM     (event_met_pfmet * cos(event_met_pfmetPhi), event_met_pfmet * sin(event_met_pfmetPhi), W_pzNu1, 0);

	double massjj     = (ajp+bjp).M();
	double masslvjj   = (ajp+bjp+mup+nvp).M();

	hh_lvjj_mjj->Fill(massjj);
	if (massjj>60&&massjj<100) hh_lvjj_mlvjj->Fill(masslvjj);

	// Dan's angular variables
	double cosanwinww = JacksonAngle( (mup+nvp), (ajp+bjp) );
	float  phipl      = -999, ctuv = -999, ctjj = -999;
	dg_kin_Wuv_Wjj(mup,nvp,ajp,bjp, phipl, ctuv, ctjj);
	double jac        = (ajp+bjp).Pt()/(ajp+bjp+mup+nvp).M();


	// Kalanand's cut variables
	double jjcentral = fabs(tanh(0.5*(JetPFCor_Y[0]-JetPFCor_Y[1])));
	double jjdphi    = fabs(fabs(fabs(JetPFCor_Phi[0]-JetPFCor_Phi[1])-TMath::Pi())-TMath::Pi());
	double ljmetdphi = fabs(JetPFCor_dphiMET[0]);
	double wwdphi    = fabs(fabs(fabs(W_phi-atan2((JetPFCor_Py[0]+JetPFCor_Py[1]),(JetPFCor_Px[0]+JetPFCor_Px[1])))-TMath::Pi())-TMath::Pi());


    Bool_t cut1KM = ( wwdphi    >  2.84 );
    Bool_t cut2KM = ( jjcentral <  1.5  );
    Bool_t cut3KM = ( ljmetdphi >  0.5  );
    Bool_t cut4KM = ( fabs(phipl) > 0.5);
    Bool_t cut5KM = ( fabs(cosanwinww) < 1.0 );
    Bool_t cut6KM =  (ctjj > -0.6  && ctjj < 0.8);
    Bool_t cut7KM = ( ctuv > -0.8 && ctuv < 0.8);
    Bool_t cut8KM = ( jac     >  0.2 );
    Bool_t cut9KM = (jjdphi > 0.5 &&  jjdphi < 2.5);


    Bool_t cutKM = cut1KM && cut2KM && cut3KM && cut4KM && cut5KM &&
       cut6KM && cut7KM && cut8KM && cut9KM;
    Bool_t NminusonecutKM1 = cut2KM && cut3KM && cut4KM && cut5KM &&
       cut6KM && cut7KM && cut8KM && cut9KM;
    Bool_t NminusonecutKM2 = cut1KM && cut3KM && cut4KM && cut5KM &&
       cut6KM && cut7KM && cut8KM && cut9KM;
    Bool_t NminusonecutKM3 = cut1KM && cut2KM && cut4KM && cut5KM &&
       cut6KM && cut7KM && cut8KM && cut9KM;
    Bool_t NminusonecutKM4 = cut1KM && cut2KM && cut3KM && cut5KM &&
       cut6KM && cut7KM && cut8KM && cut9KM;
    Bool_t NminusonecutKM5 = cut1KM && cut2KM && cut3KM && cut4KM &&
       cut6KM && cut7KM && cut8KM && cut9KM;
    Bool_t NminusonecutKM6 = cut1KM && cut2KM && cut3KM && cut4KM && cut5KM &&
       cut7KM && cut8KM && cut9KM;
    Bool_t NminusonecutKM7 = cut1KM && cut2KM && cut3KM && cut4KM && cut5KM &&
       cut6KM && cut8KM && cut9KM;
    Bool_t NminusonecutKM8 = cut1KM && cut2KM && cut3KM && cut4KM && cut5KM &&
       cut6KM && cut7KM && cut9KM;
    Bool_t NminusonecutKM9 = cut1KM && cut2KM && cut3KM && cut4KM && cut5KM &&
       cut6KM && cut7KM && cut8KM;

	if(NminusonecutKM1) hh_lvjj_wwdphi->Fill(wwdphi);
	if(NminusonecutKM2) hh_lvjj_jjcentral->Fill(jjcentral);
	if(NminusonecutKM3) hh_lvjj_ljmetdphi->Fill(ljmetdphi);
	if(NminusonecutKM4) hh_lvjj_phipl->Fill(phipl);
	if(NminusonecutKM5) hh_lvjj_cosanwinww->Fill(cosanwinww);
	if(NminusonecutKM6) hh_lvjj_ctjj->Fill(ctjj);
	if(NminusonecutKM7) hh_lvjj_ctuv->Fill(ctuv);
	if(NminusonecutKM8) hh_lvjj_jac->Fill(jac);
	if(NminusonecutKM9) hh_lvjj_jjdphi->Fill(jjdphi);


	//  -----SET.1 Final Mjj and Mlvjj after Dan's angular cuts
	if ( cut1KM && cosanwinww > -0.8 && cosanwinww < 0.8
	     && ctuv    > -0.8 && ctuv       < 0.8    
	     && ctjj    > -0.6 && ctjj       < 0.8    
	     && jac     >  0.2
	     ){
	  hh_lvjj_dgmjj->Fill(massjj);
	  if (massjj>60&&massjj<100) hh_lvjj_dgmlvjj->Fill(masslvjj);
	}

	//  -----SET.2 Final Mjj and Mlvjj after Kalanand's cuts 
	if ( cutKM ){	  
	  hh_lvjj_kkmjj->Fill(massjj);
	  if (massjj>60&&massjj<100) hh_lvjj_kkmlvjj->Fill(masslvjj);
	}
	
      }
      
   }


   // Save out histogram
   fresults.cd();
   hh_btag_SSVHE->Write();

   hh_ttb_mjj_all->Write();
   hh_ttb_mjj_nob->Write();

   hh_lvjj_mjj->Write();
   hh_lvjj_mlvjj->Write();

   hh_lvjj_cosanwinww->Write();
   hh_lvjj_phipl->Write();
   hh_lvjj_ctuv->Write();
   hh_lvjj_ctjj->Write();
   hh_lvjj_jac->Write();

   hh_lvjj_jjcentral->Write();
   hh_lvjj_jjdphi->Write();
   hh_lvjj_ljmetdphi->Write();
   hh_lvjj_wwdphi->Write();

   hh_lvjj_dgmjj->Write();
   hh_lvjj_dgmlvjj->Write();

   hh_lvjj_kkmjj->Write();
   hh_lvjj_kkmlvjj->Write();

   fresults.Close();


   std::cout << outfilename << "    "<< nentries  << std::endl;
}




void WmunuJetsHistoMaker::myana()
{
  TChain * myChain;
  // 2011 data
//   myChain = new TChain("WJet");  
//   myChain->Add("WmunuJets_Data2011_231invpb_May9DCS.root"); 
//   Init(myChain);Loop( 2011, 0,                              "RD_WmunuJets_Data2011_231invpb_May9DCS.root");

  myChain = new TChain("WJet");  
  myChain->Add("WmunuJets_Data2011_178invpb_May6JSON.root"); 
  Init(myChain);Loop( 2011, 0,                              "RD_WmunuJets_Data2011_178invpb_May6JSON.root");

  myChain = new TChain("WJet");  
  myChain->Add("WmunuJets_Spring11MC_WJets.root"); 
  Init(myChain);Loop( 2011, 1,                              "RD_WmunuJets_Spring11MC_WJets.root");

  myChain = new TChain("WJet");  
  myChain->Add("WmunuJets_Spring11MC_TTToLNu2Q2B.root"); 
  Init(myChain);Loop( 2011, 2,                              "RD_WmunuJets_Spring11MC_TTToLNu2Q2B.root");

  myChain = new TChain("WJet");  
  myChain->Add("WmunuJets_Spring11MC_WZtoAnything.root"); 
  Init(myChain);Loop( 2011, 3,                              "RD_WmunuJets_Spring11MC_WZtoAnything.root");

  myChain = new TChain("WJet");  
  myChain->Add("WmunuJets_Spring11MC_WWtoAnything.root"); 
  Init(myChain);Loop( 2011, 4,                              "RD_WmunuJets_Spring11MC_WWtoAnything.root");

  myChain = new TChain("WJet");  
  myChain->Add("WmunuJets_Spring11MC_WWToLNuQQ_M-180.root"); 
  Init(myChain);Loop( 2011, 5,                              "RD_WmunuJets_Spring11MC_WWToLNuQQ_M-180.root");

  myChain = new TChain("WJet");  
  myChain->Add("WmunuJets_Spring11MC_WWToLNuQQ_M-300.root"); 
  Init(myChain);Loop( 2011, 6,                              "RD_WmunuJets_Spring11MC_WWToLNuQQ_M-300.root");



//   // 2010 data
//   myChain = new TChain("WJet");  
//   myChain->Add("WenuJets_Data2010.root"); 
//   Init(myChain);Loop( 2010, 0,                              "./RD_WenuJets_Data2010.root");

//   myChain = new TChain("WJet");  
//   myChain->Add("WenuJets_Winter10MC_WJets.root"); 
//   Init(myChain);Loop( 2010, 1,                              "./RD_WenuJets_Winter10MC_WJets.root");

//   myChain = new TChain("WJet");  
//   myChain->Add("WenuJets_Winter10MC_TTPythia.root"); 
//   Init(myChain);Loop( 2010, 2,                              "./RD_WenuJets_Winter10MC_TTPythia.root");

//   myChain = new TChain("WJet");  
//   myChain->Add("WenuJets_Winter10MC_WZtoAnything.root"); 
//   Init(myChain);Loop( 2010, 3,                              "./RD_WenuJets_Winter10MC_WZtoAnything.root");

//   myChain = new TChain("WJet");  
//   myChain->Add("WenuJets_Winter10MC_WWtoAnything.root"); 
//   Init(myChain);Loop( 2010, 4,                              "./RD_WenuJets_Winter10MC_WWtoAnything.root");



}
