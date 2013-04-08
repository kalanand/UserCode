// Script takes output of HBHEHORecPulseAnal1.cc and reconstructs the HBHEHO
// pulse from successive TDC-indexed histograms.
//
//#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TString.h"

#ifdef MAIN
typedef unsigned long uint32_t;
#endif

struct FileInfo_t {
  FileInfo_t(TFile *infp,TString infpath,TString inrpath, TString indescr, int inctn,int incln):
    fp(infp),filepath(infpath),rootpath(inrpath),descr(indescr),cutnum2get(inctn),clsnum2get(incln) {}
  TFile *fp;
  TString filepath;
  TString rootpath;
  TString descr;
  int     cutnum2get;
  int     clsnum2get;
};

typedef struct {
  TH1    *p;
  int     signum;
  int     cutnum;
  int     clsnum;
  bool    signal;
  string  descr;
  string  filedescr;
}
HistInfo_t;

#define NUMSIGSTRS 5
static const char *sigstrs[NUMSIGSTRS] = {
  "unk", "e+X", "e+l+X", "other", "e+X vs All Bkgnd"
};

#define NUMCLS 5
static const char *clsstrs[NUMCLS] = {
  "susy", "ttbar", "wjets", "zjets", "other"
};

#define NUMCUTS 5
static const char *cutstrs[NUMCUTS] = {
  "no cuts", "1e1j", "LJetMinET>150GeV", "numjets>=3","MinMET>250GeV"
};

//======================================================================
// Got this from
// http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html

void Tokenize(const string& str,
	      vector<string>& tokens,
	      const string& delimiters = " ")
{
  // Skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type pos     = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos) {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}                                                            // Tokenize

//======================================================================

int getFileInfo(const char *filewithpaths,
		vector<FileInfo_t>& v_rootfiles)
{
  char line[256];

  FILE *pathfp = fopen(filewithpaths, "r");

  if (!pathfp) {
    cerr << "File not found, " << filewithpaths << endl;
    return 0;
  }

  while (!feof(pathfp) && fgets(line, 256, pathfp)) {
    char path[256];
    char shortdescr[80];
    int cutnum,clsnum;

    if (line[0] == '#') continue;
    int nscanned = sscanf(line, "%s %s %d %d", path, shortdescr, &cutnum,&clsnum);

    TString filepath( (char *)strtok(path,":") );
    TString rootpath( (char*)strtok( NULL, "" ) );
    rootpath.Remove( 0, 1 );
   
    TFile *tfile =  new TFile(filepath);

    if (tfile->IsZombie()) {
      cerr << "File failed to open, " << filepath << endl;
      return 0;
    }
    if (nscanned != 4) {
      cerr << "pathfile requires <pathstring> <description string> <cutnumber> <class number>\n";
      cerr << line << endl;
      return 0;
    }
    
    FileInfo_t fileinfo(tfile,filepath,rootpath,shortdescr,cutnum,clsnum);
    v_rootfiles.push_back(fileinfo);
  }
  fclose(pathfp);
  return 1;
}                                                         // getFileInfo

//======================================================================

bool getOneHisto(FileInfo_t &file,
		 const char *hnamefmt,
		 HistInfo_t &hi,
		 HistInfo_t &sumhi,
		 int         signum)
{
  char hname[80];

  file.fp->cd(file.rootpath);
  TDirectory *dir = gDirectory;

  sprintf(hname, hnamefmt,hi.cutnum,hi.clsnum,signum);

  TH1 *h1p = (TH1 *)dir->FindObjectAny(hname);

  if (!h1p) {
    cout << "\tcouldn't get " << hname;
    cout << " from " << file.filepath << ":" << file.rootpath << endl;
    return false;
  }

  hi.p      = h1p;
  hi.signum = signum;
  hi.filedescr = file.descr;

  // process the sum
  if (!hi.signal) {
    if (sumhi.p) sumhi.p->Add(hi.p);
    else {
      sumhi.p = (TH1 *)hi.p->Clone();
      char newname[80];
      sprintf (newname,"%s_sum",hi.p->GetName());
      sumhi.p->SetName(newname);
    }
  }

  return true;
}

//======================================================================
// signum=1 => e+X
// signum=2 => e+l+X
// signum=(0,3,4) => others
//
int getHistosOrgBySigType(FileInfo_t& file,
			  const char *hnamefmt,
			  vector<HistInfo_t>& v_histinfo1,
			  vector<HistInfo_t>& v_histinfo2,
			  vector<HistInfo_t>& v_histinfo3,
			  vector<HistInfo_t>& v_sumhistinfo,
			  int cutnum = -1)
{
  char path[256];

  HistInfo_t hi;
  hi.p = 0;

  char *ptr = strrchr(file.filepath.Data(),'/');
  if (ptr) ptr++;
  else ptr = (char *)file.filepath.Data();

  strncpy(path,ptr,256);

  //hi.descr = string(strtok(path,"_")) + " (" + string(clsstrs[file.clsnum2get]) + ")";
  //if (!hi.descr.compare(0,2,"lm")) hi.signal = true;
  //else hi.signal = false;
  
  hi.descr = string(clsstrs[file.clsnum2get]);
  hi.filedescr = file.descr;

  if (!strncmp(ptr,"lm",2)) {
    hi.signal = true;
    hi.descr += " ("  + string(ptr).substr(0,3) +  ")"; 
  } else {
    hi.signal = false;
    if (file.clsnum2get == 4) // other
      hi.descr += " ("  + file.descr +  ")"; 
  }

  if (cutnum == -1) hi.cutnum = file.cutnum2get;
  else              hi.cutnum = cutnum;

  hi.clsnum = file.clsnum2get;

  HistInfo_t sumhi = hi;

  /**************************************************************************/
  cout << "Getting histos " << hnamefmt << " from " << file.filepath << endl;
  /**************************************************************************/

  /********************
   * signum=1 => e+X
   ********************/

  if (getOneHisto(file,hnamefmt,hi,sumhi,1)) {
    v_histinfo1.push_back(hi);
    if (hi.signal) sumhi = hi;
  }

  /********************
   * signum=2 => e+L+X
   ********************/

  if (getOneHisto(file,hnamefmt,hi,sumhi,2))
    v_histinfo2.push_back(hi);

  /********************************************
   * signum=(0,3,4) => others (added together)
   ********************************************/

  hi.p = 0;
  HistInfo_t hi2 = hi;

  if (getOneHisto(file,hnamefmt,hi2,sumhi,0))
    hi = hi2;

  if (getOneHisto(file,hnamefmt,hi2,sumhi,3)) {
    if (hi.p) hi.p->Add(hi2.p);
    else      hi.p = hi2.p;
  }

  if (getOneHisto(file,hnamefmt,hi2,sumhi,4)) {
    if (hi.p) hi.p->Add(hi2.p);
    else      hi.p = hi2.p;
  }    

  if (hi.p)    v_histinfo3.push_back(hi);
  if (sumhi.p) v_sumhistinfo.push_back(sumhi);

  return 1;
}                                               // getHistosOrgBySigType

//======================================================================

int fixupClassHist(TH1 *phist)
{
  phist->GetXaxis()->SetLabelSize(.05);
  phist->GetYaxis()->SetLabelSize(.05);
  phist->GetYaxis()->SetRangeUser(0,4);
  //phist->GetYaxis()->SetBinLabel(1,"unclsd");
  return 1;
}

//======================================================================

int fixupJetProjectionHist(TH1 *phist)
{
  phist->GetXaxis()->SetRangeUser(0,1000.0);
  return 1;
}

//======================================================================

int fixupJetMultiplicityHist(TH1 *phist)
{
  phist->GetXaxis()->SetRangeUser(-0.5,10.5);
  return 1;
}

//======================================================================

int getHistoAllCuts(FileInfo_t& file,
		    const char *fmtstr,
		    vector<HistInfo_t>& v_histinfo,
		    int (*fixuphisto)(TH1 *phist))
{
  char hname[80];

  cout << "getHistoAllCuts " << fmtstr << " from " << file.filepath << endl;

  file.fp->cd(file.rootpath);
  TDirectory *currentdir = gDirectory;

  TH2F *h2p;
  for (int i=0; i<NUMCUTS; i++) {
    sprintf(hname, fmtstr, i);
    h2p = (TH2F *)currentdir->FindObjectAny(hname);

    if (!h2p) {
      cout << "\tCouldn't 'FindObjectAny' " << hname;
      cout << " in " << file.rootpath << endl;
      continue;
    }

    HistInfo_t hi;

    // Some fix-up...
    if (fixuphisto)
      (*fixuphisto)((TH1 *)h2p);
    
    hi.p = (TH1 *)h2p;
    char *cp = strrchr(file.filepath.Data(),'/');
    if (!cp) cp = (char *)file.filepath.Data();
    else cp++;

    hi.descr = string(clsstrs[file.clsnum2get]);
    hi.filedescr = file.descr;

    if (!strncmp(cp,"lm",2)) {
      hi.signal = true;
      hi.descr += " ("  + string(cp).substr(0,3) +  ")"; 
    } else {
      hi.signal = false;
      if (file.clsnum2get == 4) // other
	hi.descr += " ("  + file.descr +  ")"; 
    }

    hi.cutnum = i;
    hi.clsnum = file.clsnum2get;
    
    v_histinfo.push_back(hi);
  }

  return 1;
}                                                     // getHistoAllCuts

//======================================================================

void plotVecOnOneCanvas(std::vector<HistInfo_t>& v_histinfo,
			const char *titlestr,
			const char *drawstring,
			int optstats)
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(optstats);
  gStyle->SetStatFontSize(.1);
  gStyle->SetPaintTextFormat(".1f");
  gStyle->SetPalette(1,0);

  TCanvas *c1 = new TCanvas(titlestr,titlestr,
			    //v_histinfo.size()*400,400);
			    800,800);
  c1->SetFillColor(10);
  c1->SetTopMargin(0.04);
  c1->SetRightMargin(0.01);
  c1->SetLeftMargin(0.5);
  c1->SetBottomMargin(.1);
  //c1->Divide(v_histinfo.size());
  c1->Divide(2,2);

  
  for (unsigned int i=0; i<v_histinfo.size(); i++) {
    TH2F *h2p = (TH2F *)v_histinfo[i].p;
    c1->cd(i+1);
    h2p->Draw(drawstring);
  }

  c1->cd();
}

//======================================================================

void CollectCutStats(vector<HistInfo_t>& v_hi,  // v(class histos) per file
		     map<string,vector<double> >& m_stats)
{
  // each cut has a classhist.
  // integrate the columns along one row for all background statistics
  // for signal, separate to e+x,e+l+x,other
  //
  pair<map<string,vector<double> >::iterator,bool> ret;

  for (size_t i =0;i < v_hi.size(); i++) {
    HistInfo_t hi = v_hi[i];

    map<string,vector<double> >::iterator it;

    if (hi.signal) {
      double oneEsum        = hi.p->GetBinContent(2,1);
      double oneEplusLepsum = hi.p->GetBinContent(3,1);
      double othersum       = hi.p->GetBinContent(1,1) +
                              hi.p->GetBinContent(4,1) +
                              hi.p->GetBinContent(5,1);

      string statstr = hi.descr + " (1e)";
      it = m_stats.find(statstr);
      if (it == m_stats.end()) {
	vector<double> v_stats(NUMCUTS,0.0);
	ret = m_stats.insert(std::pair<string,vector<double> >(statstr,v_stats));
	it = ret.first;
      }
      it->second[hi.cutnum] += oneEsum;

      statstr = hi.descr + " (1e1L)";
      it = m_stats.find(statstr);
      if (it == m_stats.end()) {
	vector<double> v_stats(NUMCUTS,0.0);
	ret = m_stats.insert(std::pair<string,vector<double> >(statstr,v_stats));
	it = ret.first;
      }
      it->second[hi.cutnum] += oneEplusLepsum;

      statstr = hi.descr + " (oth)";
      it = m_stats.find(statstr);
      if (it == m_stats.end()) {
	vector<double> v_stats(NUMCUTS,0.0);
	ret = m_stats.insert(std::pair<string,vector<double> >(statstr,v_stats));
	it = ret.first;
      }
      it->second[hi.cutnum] += othersum;

    } else { // background

      int nbinsx = hi.p->GetNbinsX();

      for (int j=1; j<NUMCLS; j++) { 
	double sum = hi.p->Integral(1,nbinsx,j+1,j+1);
	string statstr = string(clsstrs[j]);
	if (j==4) statstr += " (" + hi.filedescr + ")";

	it = m_stats.find(statstr);
	if (it == m_stats.end()) {
	  vector<double> v_stats(NUMCUTS,0.0);
	  ret = m_stats.insert(std::pair<string,vector<double> >(statstr,v_stats));
	  it = ret.first;
	}
	it->second[hi.cutnum] += sum;
      }
    }
  } // loop over vector of histinfo
}                                                     // CollectCutStats

//======================================================================

void DumpCutStats(map<string,vector<double> >& m_stats)
{
  map<string,vector<double> >::iterator it;

  cout << setw(10) << " ";
  for (int i=0; i<NUMCUTS; i++) cout << setw(20) << cutstrs[i];
  cout << endl;

  cout.precision(1);

  for (it=m_stats.begin(); it != m_stats.end(); it++) {
    cout << setw(10) << it->first;
    double lastnev = 0;
    for (int i=0; i<NUMCUTS; i++) {
      double nevents = it->second[i];
      cout << setw(11) << fixed << nevents;
      if (!i)           cout << " (100.0%)";
      else if (lastnev) cout << " (" << fixed << setw(5) << 100.0*nevents/lastnev << "%)";
      else              cout << "         ";
      lastnev = nevents;
    }
    cout << endl;
  }
}                                                        // DumpCutStats

//======================================================================

void plotHistsOverlay(vector<vector<HistInfo_t> >& vv_hi,
		      //string& titlestr,
		      string& titleappender,
		      bool logscale=false,
		      bool saveplots=false,
		      bool projecty=false,
		      int  nbin=0,
		      int (*fixuphisto)(TH1 *phist)=0,
		      float minx=-1.0, float maxx=-2.0)
{
#if 0
  gStyle->SetStatX(.97);
  gStyle->SetStatY(.97);
#endif
  gStyle->SetOptStat(0);

  gStyle->SetTitleX(0.1);
  gStyle->SetTitleY(1.0);

  gROOT->SetStyle("Plain");

  int numpl = vv_hi.size();
  if (!numpl) return;

  // adjust axes tick labels/title/positions/sizes
  //
  double maxval = -1e99;
  int signum = 0;
  TH1  *first = 0;
  TCanvas *c1 = 0;
  string titlestr;

  vector<vector<HistInfo_t> >::const_iterator it;
  for (it = vv_hi.begin();it != vv_hi.end(); it++) {
    signum++;

    if (!it->size()) continue;

    if (!c1) {
      titlestr  = string(strtok((char *)(*it)[0].p->GetTitle(),","));
      titlestr += titleappender;

      /************************************************************************************/
      cout << "plotHistsOverlay: title= " << titlestr << ", numplots = " << numpl << endl;
      /************************************************************************************/
      int plotdim = (numpl>1) ? 300: 400;
  
      c1 = new TCanvas(titlestr.c_str(),titlestr.c_str(),numpl*plotdim,plotdim);
      c1->SetFillColor(10);
      if (logscale) c1->SetLogy();
      if (numpl > 1)
	c1->Divide(numpl);
    }

    string sigstr(sigstrs[signum]);
    //string sigstr(sigstrs[4]);

    TVirtualPad *tp = c1->cd(signum);
    if (logscale) tp->SetLogy();
    tp->SetTopMargin(0.02);
    tp->SetRightMargin(0.03);
    tp->SetLeftMargin(0.08);
    tp->SetBottomMargin(.1);

    vector<int> numSamplesPerClass(NUMCLS,0);

    /*********************************************************/
    cout << "\tProcessing signature " << sigstr;
    cout << ", " << it->size() << " sample(s)" << endl;
    /*********************************************************/

    float legminy = 1.0 - 0.055*it->size();
    float legminx = 0;
    //TLegend *leg = new TLegend(0.75,legminy,0.98,1.0);
    TLegend *leg = new TLegend();
    leg->SetX2NDC(0.98);
    leg->SetY2NDC(1.0);
    leg->SetY1NDC(legminy);
    //leg->SetTextSize(0.05);
    leg->SetFillColor(10);
    leg->SetBorderSize(1);

    // Overlay histos on the current pad

    first = 0;
    for (unsigned int j=0; j<it->size(); j++) {
      HistInfo_t hi = (*it)[j];
      TH1  *h1p = hi.p;
      if (!h1p) continue;

      if (projecty) {
	TH2 *h2p = (TH2 *)h1p;
	string prjname=string(h2p->GetName()) + "_" + hi.filedescr;
	h1p = (TH1 *)new TH1D(*(h2p->ProjectionY(prjname.c_str(),nbin,nbin)));
      }

      if (fixuphisto) (*fixuphisto)(h1p);

      if (maxval < h1p->GetMaximum())
	maxval = h1p->GetMaximum();

      //cout << '\t' << h1p->GetName() << " " << h1p->GetEntries() << endl;

      int colornum = (hi.clsnum==4) ? 6 : hi.clsnum+1;

      numSamplesPerClass[hi.clsnum]++;
      int linestyle = numSamplesPerClass[hi.clsnum];

      if (!j) {
	first = h1p;
	h1p->SetXTitle(titlestr.c_str());
	h1p->GetXaxis()->CenterTitle(true);
	h1p->GetXaxis()->SetLabelSize(.04);
	h1p->GetXaxis()->SetTitleSize(.04);
	h1p->GetXaxis()->SetTitleOffset(1.1);
	//h1p->GetYaxis()->SetLabelSize(.04);
	h1p->SetTitle(sigstr.c_str());
	h1p->SetMinimum(0.1);
	h1p->SetLineColor(colornum);
	h1p->SetLineWidth(2);
	h1p->SetLineStyle(linestyle);
	
	h1p->Draw();
      }
      else {
	h1p->SetLineColor(colornum);
	h1p->SetLineWidth(2);
	h1p->SetLineStyle(linestyle);

	h1p->Draw("same");
      }
      leg->AddEntry(h1p,hi.descr.c_str(),"l");
      if (legminx < (float)hi.descr.size())
	legminx = (float)hi.descr.size();

    } // current pad histo loop

    int legplot = std::min(numpl,4);
    if (signum == legplot) {
      leg->SetX1NDC(max(0.0,1.0-(legminx*0.035)));
      leg->Draw("");
    }

    // poor-man's autoscale:
    if ((maxval > 0) && first) {
      int iexp = (int)(log10(maxval) + 1.0);
      double plotmax = pow(10.0,(double)iexp);
      double plotmin = logscale ? 0.1 : 0.0;
      first->GetYaxis()->SetRangeUser(plotmin,plotmax);
    }

    if (maxx > minx)
      first->GetXaxis()->SetRangeUser(minx,maxx);

  } // pad loop

  string filename(first->GetName());
  cout << first->GetName() << endl;
  filename = filename.substr(0,filename.find("cls"));
  filename += ".png";
  if (saveplots) c1->SaveAs(filename.c_str());
}                                                    // plotHistsOverlay

//======================================================================

void ProcessClassHistos(vector<FileInfo_t>& v_rootfiles)
{
  map<string,vector<double> > m_stats;
  map<TString,bool> files;
  files.clear();

  // Sometimes multiple classes are contained in one file, so keep track
  // of whether we've already processed a file previously...
  //
  for (unsigned int i=0; i<v_rootfiles.size(); i++) {
    FileInfo_t file = v_rootfiles[i];
    map<TString,bool>::const_iterator it = files.find(file.filepath);
    if (it == files.end()) {
      vector<HistInfo_t> v_histinfo;
      if (!getHistoAllCuts(file,"classhistcut%d",v_histinfo,fixupClassHist))
	continue;
      //plotVecOnOneCanvas(v_histinfo, file.filepath.Data(),"TEXT COLZ",0);
      CollectCutStats(v_histinfo,m_stats);
      files.insert(pair<TString,bool>(file.filepath,true));
    }
  }

  DumpCutStats(m_stats);
}                                                  // ProcessClassHistos

//======================================================================

void PlotVar(vector<FileInfo_t>& v_rootfiles,
	     const char *line,
	     bool logscale,
	     bool saveplots,
	     bool projecty=false,
	     int  nbin=0,
	     int (*fixuphisto)(TH1 *phist)=0)
{
  char *varname;
  float minx = -1.0;
  float maxx = -2.0;

  if (strchr(line,',')) {
    varname = strtok((char *)line,",");
    sscanf(strtok(NULL,","),"%f",&minx);
    sscanf(strtok(NULL,","),"%f",&maxx);
  }
  else varname = (char *)line;
    

  // Loop through all the cuts
  //
  for (int cutnum = 0; cutnum < NUMCUTS; cutnum++) {
    vector<vector<HistInfo_t> > vv_hipersig(4);
    vector<vector<HistInfo_t> > vv_hiallbkg(1);

    // Get the histos for the particular applied cut, 
    //   and organize by signature type
    //
    for (unsigned int i=0; i<v_rootfiles.size(); i++) {
      if (!getHistosOrgBySigType(v_rootfiles[i],
				 varname,
				 vv_hipersig[0], // e+X
				 vv_hipersig[1], // e+l+X
				 vv_hipersig[2], // other
				 vv_hipersig[3], // signal e+X only, bkgnd all added together
				 cutnum))
	continue;
    }

    // Plot the breakdown by the grouped signature types
    //
    string title = ", " + string(cutstrs[cutnum]);
    plotHistsOverlay(vv_hipersig,title,logscale,saveplots,projecty,nbin,fixuphisto,minx,maxx);

    //    ... and a summary plot
    //
    vv_hiallbkg[0] = vv_hipersig[3];
    title = " all, " + string(cutstrs[cutnum]);
    //plotHistsOverlay(vv_hiallbkg,title,logscale,saveplots,projecty,nbin,fixuphisto,minx,maxx);

  } // loop over cuts
}                                                             // PlotVar

//======================================================================

void PlotJetETs(vector<FileInfo_t>& v_rootfiles, bool logscale)
{

  // Get jetethist for profiling
  //
  for (int cutnum = 0; cutnum < NUMCUTS; cutnum++) {
    vector<vector<HistInfo_t> > vv_hipersig(4);
    //vector<vector<HistInfo_t> > vv_hiallbkg(1);
    string title;
    
    for (unsigned int i=0; i<v_rootfiles.size(); i++) {
      if (!getHistosOrgBySigType(v_rootfiles[i],
				 "jetetbynumbercut%dcls%dsig%d",
				 vv_hipersig[0], // e+X
				 vv_hipersig[1], // e+l+X
				 vv_hipersig[2], // other
				 vv_hipersig[3], // signal e+X only, bkgnd all added together
				 cutnum))
	continue;
    }

    title = string("Leading Jet ET (GeV)") + ", " + string(cutstrs[cutnum]);
    plotHistsOverlay(vv_hipersig,title,logscale,false,true,1,fixupJetProjectionHist);
#if 0
    title = string("Leading Jet ET (GeV) (all)") + ", " + string(cutstrs[cutnum]);
    plotHistsOverlay(vv_hiallbkg,title,logscale,false,true,1,fixupJetProjectionHist);
#endif
    title = string("Sub-leading Jet ET (GeV)") + ", " + string(cutstrs[cutnum]);
    plotHistsOverlay(vv_hipersig,title,logscale,false,true,2,fixupJetProjectionHist);
#if 0
    title = string("Sub-leading Jet ET (GeV) (all)") + ", " + string(cutstrs[cutnum]);
    plotHistsOverlay(vv_hiallbkg,title,logscale,false,true,2,fixupJetProjectionHist);
#endif
  }
}                                                          // PlotJetETs

//======================================================================

void multiplot(const char* filewithpaths,
	       const char* filewithhistnames,
	       bool logscale,
	       bool saveplots=false)
{
  vector<FileInfo_t> v_rootfiles;

  FILE *hnamefp = fopen(filewithhistnames, "r");

  if (!hnamefp) {
    cerr << "File not found, " << filewithhistnames << endl;
    return;
  }

  if (!getFileInfo(filewithpaths, v_rootfiles))
    return;

  cout << "Processing " << v_rootfiles.size() << " filespecs" << endl;

  // EXTRACT HISTOGRAMS AND PLOT

  // Always get classhist out and plot, all cuts for one file on one canvas
  //
  ProcessClassHistos(v_rootfiles);

  // plot jet multiplicities
  //PlotVar(v_rootfiles,"numjhcut%dcls%dsig%d",logscale,saveplots,true,1,fixupJetMultiplicityHist);

  //PlotJetETs(v_rootfiles, logscale);

  char line[128];
  while (!feof(hnamefp) && fgets(line, 128, hnamefp)) {
    if (char *newline = strchr(line,'\n')) *newline=0;
    if (line[0] == '#') continue;
    cout << "Processing " << line << endl;
    PlotVar(v_rootfiles, line, logscale, saveplots);
  }
}
