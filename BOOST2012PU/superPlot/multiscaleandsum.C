
#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "TH1.h"
#include "TF1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TGraph.h"

#ifdef MAIN
typedef unsigned long uint32_t;
#endif

struct HistInfo_t {
  HistInfo_t(TFile *infp,TString infpath,TString inrpath,
	     float inxs, int innev, float inxc,float inwt):
    fp(infp),filepath(infpath),rootpath(inrpath),xsec(inxs),
    nev(innev),xcutoff(inxc),weight(inwt) {}
  HistInfo_t() :
    fp(NULL),filepath(""),rootpath(""),xsec(-9e99),nev(0),xcutoff(0),weight(0) {}
  TFile  *fp;
  TH1    *hp;
  TString filepath;
  TString rootpath;
  float   xsec;
  int     nev;
  float   xcutoff;
  float   weight;
  
};

//======================================================================

int getHistInfo(const char *filewithpaths,
		vector<HistInfo_t>& v_histos,
		float integluminvpb)
{
  char line[256];

  FILE *pathfp = fopen(filewithpaths, "r");

  if (!pathfp) {
    cerr << "File not found, " << filewithpaths << endl;
    return 0;
  }

  while (!feof(pathfp) && fgets(line, 256, pathfp)) {
    char path[256];
    int nev;
    float xsec,xcutoff=0,weight;

    if (line[0] == '#') continue;

    int nscanned = sscanf(line, "%s %f %d %f", path,&xsec,&nev,&xcutoff);

    TString filepath( (char *)strtok(path,":") );
    TString rootpath( (char*)strtok( NULL, "" ) );
    rootpath.Remove( 0, 1 );

    TFile *tfile =  new TFile(filepath);
    
    if (tfile->IsZombie()) {
      cerr << "File failed to open, " << path << endl;
      return 0;
    }
    if ((nscanned != 3) &&
	(nscanned != 4)   ) {
      cerr << "pathfile requires <pathstring> <xsec> <nevents> [xcutoff]\n";
      return 0;
    }
    weight = (xsec*integluminvpb)/((float)nev);
    HistInfo_t histinfo(tfile,filepath,rootpath,xsec,nev,xcutoff,weight);

    // Now get the Histo
    histinfo.fp->GetObject(histinfo.rootpath,histinfo.hp);
    if (!histinfo.hp) {
      cout << "WARNING: couldn't get " << histinfo.rootpath;
      cout << " from " << histinfo.filepath << endl;
      exit(-1);
    }
    cout << "Got " << histinfo.rootpath << " from " << histinfo.filepath;
    cout << ", weight = " << weight << endl;
    v_histos.push_back(histinfo);
  }
  return 1;
}                                                         // getHistInfo

//======================================================================

void processHisto(int i,
		  HistInfo_t& addend,
		  HistInfo_t& sum,
		  int nrebin,
		  bool writeErrors)
{
  TH1 *ahp   = (TH1*)addend.hp;
  TH1 *htemp = NULL;

  cout << "Processing " << addend.filepath << ":" << addend.rootpath << endl;

  if (nrebin > 1)
    ahp->Rebin(nrebin);

  if (writeErrors) {
    ahp->Sumw2();
    htemp = (TH1 *)ahp->Clone();
  }

  ahp->Scale(addend.weight);

  int nbins = ahp->GetNbinsX()*ahp->GetNbinsY()*ahp->GetNbinsZ();
  for (int ibin=1; ibin<=nbins; ibin++) {
    float error;
    if (ahp->GetXaxis()->GetBinLowEdge(ibin) >= addend.xcutoff) {
      ahp->SetBinContent(ibin,0.);
      error = 0.;
    }
    if (writeErrors) {
      error = addend.weight*htemp->GetBinError(ibin);
      ahp->SetBinError(ibin,error);
    }
  }

  if (writeErrors) {
    // to force weighted summing when we get to it
    // NOTE: I learned the hard way, this bit has to be set AFTER
    //      the scaling.
    // ahp->SetBit(TH1::kIsAverage);
    delete htemp;
  }

  if (!i) {
    sum    = addend;
    sum.hp = (TH1 *)addend.hp->Clone();
  }
  else
    sum.hp->Add(addend.hp);
}                                                            // addHisto

//======================================================================

void multiscaleandsum(const char* filewithinfo,
		      float integluminvpb,
		      int   rebin=1,
		      bool writeErrors=false)
{
  vector<HistInfo_t> v_histos;

  if (!getHistInfo(filewithinfo, v_histos,integluminvpb))
    return;

  // EXTRACT HISTOGRAMS, SCALE AND SUM
  char sumname[80];
  sprintf (sumname, "%s_sum_%dinvpb.root", filewithinfo,(int)integluminvpb);
  TFile *sumfp = new TFile(sumname,"RECREATE");

  HistInfo_t sum;

  for (unsigned int i=0; i<v_histos.size(); i++) {
    processHisto(i,v_histos[i],sum,rebin,writeErrors);
  }
  sumfp->Write();
}
