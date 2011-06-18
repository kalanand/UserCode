#include "Riostream.h"
void makeSigTree() {
//  Read data from an ascii file and create a root file with an ntuple.
 
   ifstream in;
   in.open("SigElectrons.txt");

    Float_t run, event, lumi, Zmass, et, eta, phi, charge, deta, dphi, 
       sieie, eop, hoe, fbrem, trackiso, ecaliso, hcaliso, misshits, npv;

   Int_t nlines = 0;
   TFile *f = new TFile("SigElectrons.root","RECREATE");
   TNtuple *ntuple = new TNtuple("ntuple","data from ascii file",
   "et:eta:phi:deta:dphi:sieie:hoe:trackiso:ecaliso:hcaliso:misshits");

   while (1) {
      in >> et >> eta >> phi >> deta >> dphi >> sieie >> 
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
