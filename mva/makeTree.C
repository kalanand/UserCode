#include "Riostream.h"
void makeTree() {
//  Read data from an ascii file and create a root file with an ntuple.
 
   ifstream in;
   in.open("SigElectrons.txt");

    Float_t run, event, lumi, Zmass, tag_et, tag_eta, tag_phi, probe_et, probe_eta, probe_phi;

   Int_t nlines = 0;
   TFile *f = new TFile("goldenZeeEvents.root","RECREATE");
   TNtuple *ntuple = new TNtuple("ntuple","data from ascii file",
   "run:event:lumi:Zmass:tag_et:tag_eta:tag_phi:probe_et:probe_eta:probe_phi");

   while (1) {
      in >> run >> event >> lumi >> Zmass >> tag_et >> tag_eta >> tag_phi >> probe_et >> probe_eta >> probe_phi;

      if (!in.good()) break;
      ntuple->Fill(run, event, lumi, Zmass, tag_et, tag_eta, tag_phi, probe_et, probe_eta, probe_phi);
      nlines++;
   }
   printf(" found %d points\n",nlines);

   in.close();

   f->Write();
}
