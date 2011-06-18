{
   TFile f("Zee_occupancy.root","recreate");
   double et[7] = {25, 30, 35, 40, 45, 50, 200};
   double eta[11] = {-2.5, -2.0, -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0, 2.5};

   TFile::Open("ZeeEvents_corrected_35invpb_WP80.root");

   TH2D tag("tag","", 6, et, 10, eta);
   TH2D probe("probe","", 6, et, 10, eta);
   tree->Draw("tag_sc_eta:tag_sc_et>>tag");
   tree->Draw("probe_sc_eta:probe_sc_et>>probe");

   f.cd();
   tag->Write();
   probe->Write();
   f.Close();
}
