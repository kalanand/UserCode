void AddCorrectedZmassToTree(char* inFile = "ZeeCands_WP95WP95.root")
{

   TFile fin(inFile, "read");
   TTree* in_tree = (TTree*) fin.Get("tree");

  Long64_t nentries = in_tree->GetEntries();
  Float_t mass_corrected, escale;
  Float_t mass, tag_sc_eta, probe_sc_eta;

   TString prefix = "CorrectEscale_";
   TFile fout( prefix + TString(inFile), "recreate");
   TTree *newtree = in_tree->CloneTree();
   nentries = newtree->GetEntries();


  TBranch *mass_branch = newtree->Branch("mass_corrected", &mass_corrected,
                                       "mass_corrected/F");
  TBranch *escale_branch = newtree->Branch("escale", &escale,"escale/F");

  newtree->SetBranchAddress("mass", &mass);
  newtree->SetBranchAddress("tag_sc_eta", &tag_sc_eta);
  newtree->SetBranchAddress("probe_sc_eta", &probe_sc_eta);


  for (Int_t i = 0; i < nentries; i++) {
    newtree->GetEntry(i);
    escale = 1.0;
    if( fabs(tag_sc_eta)<1.5 && fabs(probe_sc_eta)<1.5 ) 
       escale = 1.0115;
    else if( fabs(tag_sc_eta)>1.5 && fabs(probe_sc_eta)>1.5 ) 
       escale = 1.0292;
    else escale = sqrt(1.0115*1.0292);
    escale_branch->Fill();

    mass_corrected = escale*mass;
    mass_branch->Fill();
  }

  fout.cd();
  newtree->Write("tree");
  fout.Close();
}
