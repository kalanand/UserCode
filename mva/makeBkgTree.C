#include "Riostream.h"
void makeBkgTree() {
//  Read data from an ascii file and create a root file with an ntuple.
 
   ifstream in;
   in.open("BkgElectrons.txt");

    Float_t et, eta, phi, deta, dphi, 
       sieie, hoe, trackiso, ecaliso, hcaliso;

   Int_t nlines = 0;
   TFile *f = new TFile("BkgElectrons.root","RECREATE");
   TNtuple *ntuple = new TNtuple("ntuple","data from ascii file",
   "et:eta:phi:deta:dphi:sieie:hoe:trackiso:ecaliso:hcaliso");

   while (1) {
      in >> et >> eta >> phi >> sieie >> deta >> dphi >> 
         hoe >> trackiso >> ecaliso >> hcaliso;

      if (!in.good()) break;
      ntuple->Fill(et, eta, phi, deta, dphi, 
       sieie, hoe, trackiso/et, ecaliso/et, hcaliso/et);
      nlines++;
   }
   printf(" found %d points\n",nlines);

   in.close();

   f->Write();
}
