void makeTreeFromText(char* inputFile, char* outputFile) {
   TFile fout( outputFile, "recreate");
   TTree tree("tree","tree");
   tree.ReadFile( inputFile, "run/I:lumi/I:event/I:mass/F:tag_sc_et:tag_sc_eta:tag_sc_phi:probe_sc_et:probe_sc_eta:probe_sc_phi");

   tree.Write();
   fout.Close();
}
