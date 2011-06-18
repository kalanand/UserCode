{
   // ********* Input and output file names ******************* //
   //char* inputFileName = "../allTPtrees_35invpb.root";
   char* inputFileName = "../allTPtrees_35invpb_Dec22ReReco.root";
   // char* inputTreeName = "PhotonToGsfElectron/fitter_tree";
   char* inputTreeName = "GsfElectronPlusGsfElectron/fitter_tree";
   char* outputFileName = "test.root";


   TFile fin(inputFileName, "read");
   TTree* in_tree = (TTree*) fin.Get( inputTreeName );

  Long64_t nentries = in_tree->GetEntries();
  Float_t tag_gsfEle_eta,
     tag_gsfEle_charge,tag_gsfEle_EoverP,tag_gsfEle_bremFraction,
     tag_gsfEle_deltaPhi, tag_gsfEle_deltaEta, tag_gsfEle_HoverE,
     tag_gsfEle_trackiso, tag_gsfEle_et, tag_gsfEle_ecaliso, 
     tag_gsfEle_hcaliso, tag_sc_et, tag_gsfEle_deltaEtaOut, 
     tag_gsfEle_deltaPhiOut, tag_gsfEle_missingHits,  
     tag_gsfEle_sigmaIetaIeta, tag_gsfEle_e1x5, tag_dcot, tag_dist,
     tag_gsfEle_e5x5, tag_gsfEle_e2x5Max, tag_gsfEle_eSeedClusterOverP,
     tag_gsfEle_classification, tag_track_d0,
     tag_track_vx, tag_track_vy, tag_track_px, 
     tag_track_py, tag_track_pt, event_PrimaryVertex_x, 
     event_PrimaryVertex_y;




  Int_t event_nPV;

  TFile newfile( outputFileName, "RECREATE");
  TTree *newtree = in_tree->CloneTree(0);
  float  tag_myBDT;

  newtree->Branch("tag_myBDT", &tag_myBDT,"tag_myBDT/F");
 
  in_tree->SetBranchAddress("tag_gsfEle_eta", &tag_gsfEle_eta);
  in_tree->SetBranchAddress("tag_gsfEle_missingHits", &tag_gsfEle_missingHits);
  in_tree->SetBranchAddress("tag_gsfEle_deltaPhi", &tag_gsfEle_deltaPhi);
  in_tree->SetBranchAddress("tag_gsfEle_deltaEta", &tag_gsfEle_deltaEta);
  in_tree->SetBranchAddress("tag_gsfEle_HoverE", &tag_gsfEle_HoverE);
  in_tree->SetBranchAddress("tag_gsfEle_trackiso", &tag_gsfEle_trackiso);
  in_tree->SetBranchAddress("tag_gsfEle_et", &tag_gsfEle_et);
  in_tree->SetBranchAddress("tag_gsfEle_ecaliso", &tag_gsfEle_ecaliso);
  in_tree->SetBranchAddress("tag_gsfEle_hcaliso", &tag_gsfEle_hcaliso);
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
  in_tree->SetBranchAddress("tag_dcot", &tag_dcot);
  in_tree->SetBranchAddress("tag_dist", &tag_dist);
  in_tree->SetBranchAddress("event_nPV", &event_nPV);
  in_tree->SetBranchAddress("tag_track_vx", &tag_track_vx);
  in_tree->SetBranchAddress("tag_track_vy", &tag_track_vy); 
  in_tree->SetBranchAddress("tag_track_px", &tag_track_px);
  in_tree->SetBranchAddress("tag_track_py", &tag_track_py); 
  in_tree->SetBranchAddress("tag_track_pt", &tag_track_pt); 
  in_tree->SetBranchAddress("event_PrimaryVertex_x", &event_PrimaryVertex_x);
  in_tree->SetBranchAddress("event_PrimaryVertex_y", &event_PrimaryVertex_y);




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


  gSystem->Load( "weights/TMVAClassification_BDT.class_C.so");
  ReadBDT bdtReader(inputVarsBDT);


  int count = 0;

  for (Int_t i = 0; i < nentries; i++) {
    in_tree->GetEntry(i);
    if(i%10000==0) cout << "Now processing event " << i << endl;

// now compute the BDT output

   std::vector<double> tmvaInputValsBDT;
   tmvaInputValsBDT.push_back(tag_gsfEle_eta);
   tmvaInputValsBDT.push_back(event_nPV);
   tmvaInputValsBDT.push_back(tag_gsfEle_deltaEta); 
   tmvaInputValsBDT.push_back(tag_gsfEle_EoverP);
   tmvaInputValsBDT.push_back(tag_gsfEle_bremFraction);
   tmvaInputValsBDT.push_back(tag_gsfEle_deltaPhi);
   tmvaInputValsBDT.push_back(tag_gsfEle_sigmaIetaIeta);
   tmvaInputValsBDT.push_back(tag_gsfEle_HoverE);
   tmvaInputValsBDT.push_back(tag_gsfEle_trackiso/tag_gsfEle_et);
   tmvaInputValsBDT.push_back(tag_gsfEle_ecaliso/tag_gsfEle_et);
   tmvaInputValsBDT.push_back(tag_gsfEle_hcaliso/tag_gsfEle_et);
   tmvaInputValsBDT.push_back(tag_gsfEle_missingHits);
   tmvaInputValsBDT.push_back(tag_dcot);
   tmvaInputValsBDT.push_back(tag_dist);
   tmvaInputValsBDT.push_back((-(tag_track_vx-event_PrimaryVertex_x)*tag_track_py+(tag_track_vy-event_PrimaryVertex_y)*tag_track_px)/tag_track_pt);
   tmvaInputValsBDT.push_back(tag_gsfEle_charge);
   tmvaInputValsBDT.push_back(tag_gsfEle_deltaEtaOut);
   tmvaInputValsBDT.push_back(tag_gsfEle_deltaPhiOut);
   tmvaInputValsBDT.push_back(tag_gsfEle_e1x5/tag_gsfEle_e5x5);
   tmvaInputValsBDT.push_back(tag_gsfEle_e2x5Max/tag_gsfEle_e5x5);
   tmvaInputValsBDT.push_back(tag_gsfEle_eSeedClusterOverP);
   tmvaInputValsBDT.push_back(tag_gsfEle_classification);
   tmvaInputValsBDT.push_back(tag_track_d0);


    tag_myBDT = (float) bdtReader.GetMvaValue(tmvaInputValsBDT );
    count ++;
    newtree->Fill();
  }

  cout << "Tag+SC signal yield = " << count << endl;

  newfile.cd();
  newtree->Write("tree");
  newfile .Close();

}
