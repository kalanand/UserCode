
{
#include "Riostream.h"

  ifstream in;
  in.open("Wenu-kine-Abundance.txt");

  Float_t a,b,c,d,e;
  Int_t nlines = 0;
  Float_t x[200];
  Float_t etMin[200];
  Float_t etMax[200];
  Float_t etaMin[200];
  Float_t etaMax[200];

  while (1) {
    in >> a >> b >> c >> d >> e;
    if (!in.good()) break;
    if (nlines < 20) 
      printf("a=%8f, b=%8f, c=%8f, d=%8f, e=%8f\n",a,b,c,d,e);

    etMin[nlines] = a;
    etMax[nlines] = b;
    etaMin[nlines] = c;
    etaMax[nlines] = d;
    x[nlines] = e;
    nlines++;
  }
  printf(" found %d points\n",nlines);
  in.close();




  TFile f1("root_electroneff_WenuOff_eta_et.root");
  TH2F* h1 = (TH2F*) f1.Get("sbs_eff_EtDet_EtaDet");
  TFile f2("root_electroneff_WenuOn_eta_et.root");
  TH2F* h2 = (TH2F*) f2.Get("sbs_eff_EtDet_EtaDet");
  int nX = h1->GetNbinsX();
  int nY = h1->GetNbinsY();



  Float_t Eff = 0.0, ErrSq = 0.0, den = 0.0;




  for(int i=1; i<=nX; ++i) {

    Float_t pT0 = h1->GetXaxis()->GetBinLowEdge(i);
    Float_t pT1 = h1->GetXaxis()->GetBinLowEdge(i+1);

    for(int j=1; j<=nY; ++j) {
      Float_t eta0 = h1->GetYaxis()->GetBinLowEdge(j);
      Float_t eta1 = h1->GetYaxis()->GetBinLowEdge(j+1);

      Float_t ee = h1->GetBinContent(i, j) * 
	h2->GetBinContent(i, j);
      Float_t dx = sqrt( h1->GetBinError(i,j)**2 + 
				h2->GetBinError(i,j)**2 );
      if( dx > 1.0 ) dx = 1.0;


      for(int k=0; k<nlines; ++k) {
	if(etMin[k]==pT0 && etMax[k]==pT1 && 
	   etaMin[k]==eta0 && etaMax[k]==eta1) 
	  {
	    Eff += x[k] * ee;
	    ErrSq += x[k]* x[k]* 8.0*8.0 * dx * dx;
	    den += x[k];
	  }

      } // end k loop

    } // end j loop
  } // end i loop



  Eff /= den;
  ErrSq /= den; 
  Float_t  Err = sqrt(ErrSq); 

  cout << "############# Efficiency = " << Eff << "  +/-  " << Err << endl;

}



