
void textTable() {

  // TString histname = "sbs_eff_EtDet_EtaDet";
  TString histname = "truth_eff_EtDet_EtaDet";
  TString infile[4] = {"root_electroneff_WenuOff_eta_et.root", 
		       "root_electroneff_WenuOn_eta_et.root",
		       "root_electroneff_ZeeOff_eta_et.root",
		       "root_electroneff_ZeeOn_eta_et.root"};


//   TString infile[40] = {"root_electroneff_1_eta_et.root",
// 			"root_electroneff_2_eta_et.root", 
// 			"root_electroneff_3_eta_et.root",
// 			"root_electroneff_4_eta_et.root",
// 			"root_electroneff_5_eta_et.root",
// 			"root_electroneff_6_eta_et.root",
// 			"root_electroneff_7_eta_et.root",
// 			"root_electroneff_8_eta_et.root",
// 			"root_electroneff_101_eta_et.root",
// 			"root_electroneff_102_eta_et.root", 
// 			"root_electroneff_103_eta_et.root",
// 			"root_electroneff_104_eta_et.root",
// 			"root_electroneff_105_eta_et.root",
// 			"root_electroneff_106_eta_et.root",
// 			"root_electroneff_107_eta_et.root",
// 			"root_electroneff_108_eta_et.root",
// 			"root_electroneff_201_eta_et.root",
// 			"root_electroneff_202_eta_et.root", 
// 			"root_electroneff_203_eta_et.root",
// 			"root_electroneff_204_eta_et.root",
// 			"root_electroneff_205_eta_et.root",
// 			"root_electroneff_206_eta_et.root",
// 			"root_electroneff_207_eta_et.root",
// 			"root_electroneff_208_eta_et.root",
// 			"root_electroneff_301_eta_et.root",
// 			"root_electroneff_302_eta_et.root", 
// 			"root_electroneff_303_eta_et.root",
// 			"root_electroneff_304_eta_et.root",
// 			"root_electroneff_305_eta_et.root",
// 			"root_electroneff_306_eta_et.root",
// 			"root_electroneff_307_eta_et.root",
// 			"root_electroneff_308_eta_et.root",
// 			"root_electroneff_401_eta_et.root",
// 			"root_electroneff_402_eta_et.root", 
// 			"root_electroneff_403_eta_et.root",
// 			"root_electroneff_404_eta_et.root",
// 			"root_electroneff_405_eta_et.root",
// 			"root_electroneff_406_eta_et.root",
// 			"root_electroneff_407_eta_et.root",
// 			"root_electroneff_408_eta_et.root"};
  
  // auxiliary variables
  TString tablefile = "";
  TString rootfile  = "";

  TString pname[4] = { "table-WenuOff.txt", 
		       "table-WenuOn.txt", 
		       "table-ZeeOff.txt", 
		       "table-ZeeOn.txt"};

//   TString pname[40] = { "elec-eff-table-WenuSelection-A1.txt", 
// 		       "elec-eff-table-WenuSelection-A2.txt",
// 		       "elec-eff-table-WenuSelection-A3.txt",
// 		       "elec-eff-table-WenuSelection-B1.txt",
// 		       "elec-eff-table-WenuSelection-B2.txt",
// 		       "elec-eff-table-WenuSelection-B3.txt",
// 		       "elec-eff-table-ZeeSelection-A.txt",
// 		       "elec-eff-table-ZeeSelection-B.txt",
// 		       "elec-eff-table-WenuSelection-A1-noId.txt", 
// 		       "elec-eff-table-WenuSelection-A2-noId.txt",
// 		       "elec-eff-table-WenuSelection-A3-noId.txt",
// 		       "elec-eff-table-WenuSelection-B1-noId.txt",
// 		       "elec-eff-table-WenuSelection-B2-noId.txt",
// 		       "elec-eff-table-WenuSelection-B3-noId.txt",
// 		       "elec-eff-table-ZeeSelection-A-noId.txt",
// 		       "elec-eff-table-ZeeSelection-B-noId.txt",
// 		       "elec-eff-table-WenuSelection-A1-noTrackIso.txt", 
// 		       "elec-eff-table-WenuSelection-A2-noTrackIso.txt",
// 		       "elec-eff-table-WenuSelection-A3-noTrackIso.txt",
// 		       "elec-eff-table-WenuSelection-B1-noTrackIso.txt",
// 		       "elec-eff-table-WenuSelection-B2-noTrackIso.txt",
// 		       "elec-eff-table-WenuSelection-B3-noTrackIso.txt",
// 		       "elec-eff-table-ZeeSelection-A-noTrackIso.txt",
// 		       "elec-eff-table-ZeeSelection-B-noTrackIso.txt",
// 		       "elec-eff-table-WenuSelection-A1-noIso.txt", 
// 		       "elec-eff-table-WenuSelection-A2-noIso.txt",
// 		       "elec-eff-table-WenuSelection-A3-noIso.txt",
// 		       "elec-eff-table-WenuSelection-B1-noIso.txt",
// 		       "elec-eff-table-WenuSelection-B2-noIso.txt",
// 		       "elec-eff-table-WenuSelection-B3-noIso.txt",
// 		       "elec-eff-table-ZeeSelection-A-noIso.txt",
// 			"elec-eff-table-ZeeSelection-B-noIso.txt",
// 			"elec-eff-table-WenuSelection-A1-noSigmaEtaEta.txt", 
// 		       "elec-eff-table-WenuSelection-A2-noSigmaEtaEta.txt",
// 		       "elec-eff-table-WenuSelection-A3-noSigmaEtaEta.txt",
// 		       "elec-eff-table-WenuSelection-B1-noSigmaEtaEta.txt",
// 		       "elec-eff-table-WenuSelection-B2-noSigmaEtaEta.txt",
// 		       "elec-eff-table-WenuSelection-B3-noSigmaEtaEta.txt",
// 		       "elec-eff-table-ZeeSelection-A-noSigmaEtaEta.txt",
// 		       "elec-eff-table-ZeeSelection-B-noSigmaEtaEta.txt"};


  for(int i=0; i<4; i++) {

    tablefile =  pname[i];
    rootfile  = infile[i];
    makeTable (rootfile, histname, tablefile);
  }
  
}







// generic method to make a efficiency table

void makeTable(TString rootfilename, 
	       TString histname, char* tablefilename)
{

  TFile myhistos(rootfilename); 
  TH2F* h = (TH2F*) myhistos.Get(histname);
  if(histname.Contains("WenuOff")) h->Scale(0.91);
  int nX = h->GetNbinsX();
  int nY = h->GetNbinsY();


  FILE *file = fopen(tablefilename,"w+");


  for(int i=1; i<=nX; ++i) {
  
    Double_t pT0 = h->GetXaxis()->GetBinLowEdge(i);
    Double_t pT1 = h->GetXaxis()->GetBinLowEdge(i+1);

    for(int j=1; j<=nY; ++j) {
      Double_t x = h->GetBinContent(i,j);
      Double_t dx = 7.0 * h->GetBinError(i,j);
      if( dx > 1.0 ) dx = 1.0;
      Double_t eta0 = h->GetYaxis()->GetBinLowEdge(j);
      Double_t eta1 = h->GetYaxis()->GetBinLowEdge(j+1);

      fprintf( file ,"%4.1f  %4.1f   %+6.4f   %+6.4f  %6.4f   %6.4f \n", 
	       pT0, pT1, eta0, eta1, x, dx);
    }
  }

  fclose(file);
}
