{


   // ****** Define your selection: "WP95" or "WP80" *********** //
   const double MINMASS = 60.0;
   const double MAXMASS = 120.0;

   // ********* Input and output file names ******************* //
   char* inputFileName = "../allTPtrees_35invpb.root";
   char* inputTreeName = "PhotonToGsfElectron/fitter_tree";
   char* outputFileName = "test.root";


   TFile fin(inputFileName, "read");
   TTree* in_tree = (TTree*) fin.Get( inputTreeName );

  Long64_t nentries = in_tree->GetEntries();
  Float_t mass, tag_sc_eta, probe_sc_eta, tag_sc_phi,probe_sc_et,probe_sc_phi, 
     tag_gsfEle_charge,tag_gsfEle_EoverP,tag_gsfEle_bremFraction,
     tag_gsfEle_deltaPhi, tag_gsfEle_deltaEta, tag_gsfEle_HoverE,
     tag_gsfEle_trackiso, tag_gsfEle_et, tag_gsfEle_ecaliso, 
     tag_gsfEle_hcaliso, tag_sc_et, tag_gsfEle_deltaEtaOut, 
     tag_gsfEle_deltaPhiOut, tag_gsfEle_missingHits, tag_gsfEle_isEB, 
     tag_gsfEle_isEE, tag_gsfEle_sigmaIetaIeta, tag_gsfEle_e1x5,
     tag_gsfEle_e5x5, tag_gsfEle_e2x5Max, tag_gsfEle_eSeedClusterOverP,
     tag_gsfEle_classification, tag_track_d0, tag_track_normalizedChi2;


  Int_t probe_isWP80, tag_isWP80, event_nPV;
  UInt_t run,lumi,event;

  TFile newfile( outputFileName, "RECREATE");
  TTree *newtree = in_tree->CloneTree(0);
  float  tag_myLH, tag_myBDT, tag_myNN;

  newtree->Branch("tag_myLH", &tag_myLH,"tag_myLH/F");
  newtree->Branch("tag_myBDT", &tag_myBDT,"tag_myBDT/F");
  newtree->Branch("tag_myNN", &tag_myNN,"tag_myNN/F");


  in_tree->SetBranchAddress("mass", &mass);
  in_tree->SetBranchAddress("tag_sc_eta", &tag_sc_eta);
  in_tree->SetBranchAddress("tag_gsfEle_missingHits", &tag_gsfEle_missingHits);
  in_tree->SetBranchAddress("tag_gsfEle_deltaPhi", &tag_gsfEle_deltaPhi);
  in_tree->SetBranchAddress("tag_gsfEle_deltaEta", &tag_gsfEle_deltaEta);
  in_tree->SetBranchAddress("tag_gsfEle_HoverE", &tag_gsfEle_HoverE);
  in_tree->SetBranchAddress("tag_gsfEle_trackiso", &tag_gsfEle_trackiso);
  in_tree->SetBranchAddress("tag_gsfEle_et", &tag_gsfEle_et);
  in_tree->SetBranchAddress("tag_gsfEle_ecaliso", &tag_gsfEle_ecaliso);
  in_tree->SetBranchAddress("tag_gsfEle_hcaliso", &tag_gsfEle_hcaliso);
  in_tree->SetBranchAddress("tag_sc_et", &tag_sc_et);
  in_tree->SetBranchAddress("tag_gsfEle_isEB", &tag_gsfEle_isEB);
  in_tree->SetBranchAddress("tag_gsfEle_isEE", &tag_gsfEle_isEE);
  in_tree->SetBranchAddress("tag_gsfEle_sigmaIetaIeta", &tag_gsfEle_sigmaIetaIeta);
  in_tree->SetBranchAddress("tag_gsfEle_charge", tag_gsfEle_charge);
  in_tree->SetBranchAddress("tag_gsfEle_EoverP", &tag_gsfEle_EoverP);
  in_tree->SetBranchAddress("tag_gsfEle_bremFraction", &tag_gsfEle_bremFraction);
  in_tree->SetBranchAddress("tag_gsfEle_deltaEtaOut", &tag_gsfEle_deltaEtaOut);
  in_tree->SetBranchAddress("tag_gsfEle_deltaPhiOut", &tag_gsfEle_deltaPhiOut);
  in_tree->SetBranchAddress("tag_gsfEle_e1x5", &tag_gsfEle_e1x5);
  in_tree->SetBranchAddress("tag_gsfEle_e5x5", &tag_gsfEle_e5x5);
  in_tree->SetBranchAddress("tag_gsfEle_e2x5Max", &tag_gsfEle_e2x5Max);
  in_tree->SetBranchAddress("tag_gsfEle_eSeedClusterOverP", &tag_gsfEle_eSeedClusterOverP);
  in_tree->SetBranchAddress("tag_gsfEle_classification", &tag_gsfEle_classification);
  in_tree->SetBranchAddress("tag_track_d0", &tag_track_d0);
  in_tree->SetBranchAddress("tag_track_normalizedChi2", &tag_track_normalizedChi2);
  in_tree->SetBranchAddress("tag_isWP80", &tag_isWP80);
  in_tree->SetBranchAddress("probe_isWP80", &probe_isWP80);


  in_tree->SetBranchAddress("run", &run);
  in_tree->SetBranchAddress("lumi", &lumi);
  in_tree->SetBranchAddress("event", &event);
  in_tree->SetBranchAddress("event_nPV", &event_nPV);
  in_tree->SetBranchAddress("tag_sc_phi", &tag_sc_phi);
  in_tree->SetBranchAddress("probe_et", &probe_sc_et);
  in_tree->SetBranchAddress("probe_eta", &probe_sc_eta);
  in_tree->SetBranchAddress("probe_phi", &probe_sc_phi);


  std::vector<std::string> inputVarsBDT; //23
  inputVarsBDT.push_back("probe_gsfEle_eta");
  inputVarsBDT.push_back("event_nPV");
  inputVarsBDT.push_back("probe_gsfEle_deltaEta");
  inputVarsBDT.push_back("probe_gsfEle_EoverP");
  inputVarsBDT.push_back("probe_gsfEle_bremFraction");
  inputVarsBDT.push_back("probe_gsfEle_deltaPhi");
  inputVarsBDT.push_back("probe_gsfEle_sigmaIetaIeta");
  inputVarsBDT.push_back("probe_gsfEle_HoverE");
  inputVarsBDT.push_back("probe_gsfEle_trackiso/probe_gsfEle_et");
  inputVarsBDT.push_back("probe_gsfEle_ecaliso/probe_gsfEle_et");
  inputVarsBDT.push_back("probe_gsfEle_hcaliso/probe_gsfEle_et");
  inputVarsBDT.push_back("probe_gsfEle_missingHits");
  inputVarsBDT.push_back("probe_dcot");
  inputVarsBDT.push_back("probe_dist");
  inputVarsBDT.push_back("(-(probe_track_vx-event_PrimaryVertex_x)*probe_track_py+(probe_track_vy-event_PrimaryVertex_y)*probe_track_px)/probe_track_pt");
  inputVarsBDT.push_back("probe_gsfEle_charge");
  inputVarsBDT.push_back("probe_gsfEle_deltaEtaOut");
  inputVarsBDT.push_back("probe_gsfEle_deltaPhiOut");
  inputVarsBDT.push_back("probe_gsfEle_e1x5/probe_gsfEle_e5x5");
  inputVarsBDT.push_back("probe_gsfEle_e2x5Max/probe_gsfEle_e5x5");
  inputVarsBDT.push_back("probe_gsfEle_eSeedClusterOverP");
  inputVarsBDT.push_back("probe_gsfEle_classification");
  inputVarsBDT.push_back("probe_track_d0");


//   std::vector<std::string> inputVarsLH; // 11
//   inputVarsLH.push_back("probe_gsfEle_eta");
//   inputVarsLH.push_back("event_nPV");
//   inputVarsLH.push_back("probe_gsfEle_deltaEta");
//   inputVarsLH.push_back("probe_gsfEle_EoverP");
//   inputVarsLH.push_back("probe_gsfEle_bremFraction");
//   inputVarsLH.push_back("probe_gsfEle_deltaPhi");
//   inputVarsLH.push_back("probe_gsfEle_sigmaIetaIeta");
//   inputVarsLH.push_back("probe_gsfEle_HoverE");
//   inputVarsLH.push_back("probe_gsfEle_trackiso/probe_gsfEle_et");
//   inputVarsLH.push_back("probe_gsfEle_ecaliso/probe_gsfEle_et");
//   inputVarsLH.push_back("probe_gsfEle_hcaliso/probe_gsfEle_et");


  gSystem->Load( "weights/TMVAClassification_BDT.class_C.so");
  ReadBDT bdtReader(inputVarsBDT);


  int count = 0;
  int lastEvent = 0;
  int lastRun = 0;

  for (Int_t i = 0; i < nentries; i++) {
    in_tree->GetEntry(i);
    if(nentries%10000==0) cout << "Now processing event " << nentries << endl;

// mass cut   
    if( mass<MINMASS || mass>MAXMASS ) continue; 
    if( tag_sc_et<20.0 ) continue; 
   if( probe_sc_et<20.0 ) continue; 



// now same for the tag 
   float eta =tag_sc_eta;
   int nPV  =event_nPV;
   float deltaEta =tag_gsfEle_deltaEta; 
   float charge  =tag_gsfEle_charge;
   float EoverP =tag_gsfEle_EoverP;
   float bremFraction =tag_gsfEle_bremFraction;
   float deltaPhi =tag_gsfEle_deltaPhi;
   float sigmaIetaIeta =tag_gsfEle_sigmaIetaIeta;
   float HoverE =tag_gsfEle_HoverE;
   float trackIso =tag_gsfEle_trackiso/tag_gsfEle_et;
   float ecalIso  =tag_gsfEle_ecaliso/tag_gsfEle_et;
   float hcalIso  =tag_gsfEle_hcaliso/tag_gsfEle_et;
   float deltaEtaOut =tag_gsfEle_deltaEtaOut;
   float deltaPhiOut =tag_gsfEle_deltaPhiOut;
   float R15 =tag_gsfEle_e1x5/tag_gsfEle_e5x5;
   float R25 =tag_gsfEle_e2x5Max/tag_gsfEle_e5x5;
   float eSeedClusterOverP =tag_gsfEle_eSeedClusterOverP;
   float classification =tag_gsfEle_classification;
   float track_d0 =tag_track_d0;
   float trackChi2 =tag_track_normalizedChi2;
   float misHits  =tag_gsfEle_missingHits;

   std::vector<double> tmvaInputValsBDTTag;
   tmvaInputValsBDTTag.push_back(eta);
   tmvaInputValsBDTTag.push_back(nPV);
   tmvaInputValsBDTTag.push_back(deltaEta); 
   tmvaInputValsBDTTag.push_back(charge);
   tmvaInputValsBDTTag.push_back(EoverP);
   tmvaInputValsBDTTag.push_back(bremFraction);
   tmvaInputValsBDTTag.push_back(deltaPhi);
   tmvaInputValsBDTTag.push_back(sigmaIetaIeta);
   tmvaInputValsBDTTag.push_back(HoverE);
   tmvaInputValsBDTTag.push_back(trackIso);
   tmvaInputValsBDTTag.push_back(ecalIso);
   tmvaInputValsBDTTag.push_back(hcalIso);
   tmvaInputValsBDTTag.push_back(deltaEtaOut);
   tmvaInputValsBDTTag.push_back(deltaPhiOut);
   tmvaInputValsBDTTag.push_back(R15);
   tmvaInputValsBDTTag.push_back(R25);
   tmvaInputValsBDTTag.push_back(eSeedClusterOverP);
   tmvaInputValsBDTTag.push_back(classification);
   tmvaInputValsBDTTag.push_back(track_d0);
   tmvaInputValsBDTTag.push_back(trackChi2);
   tmvaInputValsBDTTag.push_back(misHits);


   std::vector<double> tmvaInputValsLHTag;
   tmvaInputValsLHTag.push_back(eta);
   tmvaInputValsLHTag.push_back(nPV);
   tmvaInputValsLHTag.push_back(deltaEta); 
   tmvaInputValsLHTag.push_back(EoverP);
   tmvaInputValsLHTag.push_back(bremFraction);
   tmvaInputValsLHTag.push_back(deltaPhi);
   tmvaInputValsLHTag.push_back(sigmaIetaIeta);
   tmvaInputValsLHTag.push_back(HoverE);
   tmvaInputValsLHTag.push_back(trackIso);
   tmvaInputValsLHTag.push_back(ecalIso);
   tmvaInputValsLHTag.push_back(hcalIso);


    tag_myBDT = (float) bdtReader.GetMvaValue(tmvaInputValsBDTTag );
 

    bool select = probe_isWP80==0;

// now we have selected events, just do not double count the TT events
    if(select) {
//        if( !(lastEvent==event && lastRun==run) )
//        printf("%d %d %d %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n", 
//        run,lumi,event,mass,tag_sc_et,tag_sc_eta,tag_sc_phi,
//        probe_sc_et,probe_sc_eta,probe_sc_phi);
        
       if( !(lastEvent==event && lastRun==run) ) count ++;
       if( !(lastEvent==event && lastRun==run) ) newtree->Fill();

       lastEvent=event;
       lastRun = run;
    }
  }

  cout << "Z signal yield = " << count << endl;

  newfile.cd();
  newtree->Write("tree");
  newfile .Close();

}
