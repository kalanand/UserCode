#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm> // min,max
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctype.h>     // isdigit
#include <stdlib.h>
//#define __size_t unsigned // needed for glob.h!!
#include "/usr/lib/gcc/x86_64-redhat-linux/4.1.2/include/stddef.h"
#include <glob.h>
#include <assert.h>

using namespace std;

#include "MyHistoWrapper.cc"
#include "inSet.hh"
#include "TFile.h"
#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"

struct wPad_t {
  wPad_t(string name) : topmargin(0.),bottommargin(0.),
			rightmargin(0.),leftmargin(0.),
			fillcolor(10),logx(0),logy(0),logz(0),
			legid("")
  { hframe = new wTH1(new TH1F(name.c_str(),name.c_str(),100,0.0,1.0)); }
  wPad_t(const wPad_t& wp) {
    topmargin   = wp.topmargin;
    bottommargin= wp.bottommargin;
    rightmargin = wp.bottommargin;
    leftmargin  = wp.leftmargin;
    fillcolor   = wp.fillcolor;
    logx        = wp.logx;
    logy        = wp.logy;
    logz        = wp.logz;
    gridx       = wp.gridx;
    gridy       = wp.gridy;
    legid       = wp.legid;
    titlexndc   = wp.titlexndc;
    titleyndc   = wp.titleyndc;
    //hframe      = ;           // the frame histo, holds lots of pad info
  }
  float topmargin, bottommargin, rightmargin, leftmargin;
  unsigned fillcolor;
  unsigned logx, logy,logz;
  unsigned gridx, gridy;
  string legid;
  float titlexndc, titleyndc;
  wTH1 *hframe;           // the frame histo, holds lots of pad info
  vector<string> histo_ids;
  vector<string> stack_ids;
  vector<string> altyh_ids;
  vector<string> graph_ids;
  vector<string> macro_ids;
  vector<string> label_ids;
  vector<string> latex_ids;
  vector<string> line_ids;
  vector<string> box_ids;
  TVirtualPad *vp;
};

struct wCanvas_t {
  wCanvas_t() {wCanvas_t("");}
  wCanvas_t(const string& intitle,
	    unsigned innpadsx=0, unsigned inpadxdim=600,
	    unsigned innpadsy=0, unsigned inpadydim=600,
	    float inpadxmarg=0.01, float inpadymarg=0.01,
	    float inleftmarg=0., float inrightmarg=0.,
	    float intopmarg=0.,  float inbottommarg=0.) :
    title(intitle),
    npadsx(innpadsx),npadsy(innpadsy),
    padxdim(inpadxdim),padydim(inpadydim),
    padxmargin(inpadxmarg),padymargin(inpadymarg),
    leftmargin(inleftmarg),rightmargin(inrightmarg),
    topmargin(intopmarg),bottommargin(inbottommarg),
    optstat("nemr"), fillcolor(10) {}
  wCanvas_t(const wCanvas_t& wc) :
    title(wc.title), npadsx(wc.npadsx),npadsy(wc.npadsy),
    padxdim(wc.padxdim),padydim(wc.padydim),
    padxmargin(wc.padxmargin),padymargin(wc.padymargin),
    leftmargin(wc.leftmargin),rightmargin(wc.rightmargin),
    topmargin(wc.topmargin),bottommargin(wc.bottommargin),
    optstat(wc.optstat), fillcolor(wc.fillcolor) {}
  string   style;
  string   title;
  unsigned npadsx;
  unsigned npadsy;
  unsigned padxdim;
  unsigned padydim;
  float    padxmargin;
  float    padymargin;
  float    leftmargin;    // These margins determine whether the motherpad
  float    rightmargin;   // overlays the canvas completely, or only partially
  float    topmargin;
  float    bottommargin;
  string   optstat;
  unsigned fillcolor;
  TPad    *motherpad;
  vector<wPad_t *> multipads;
  vector<wPad_t *> pads;
  vector<string> latex_ids;
  vector<string> savenamefmts;
  TCanvas *c1;
};

struct canvasSet_t {
  canvasSet_t(const string& intitle, unsigned inncanvas=1):
    title(intitle), ncanvases(inncanvas) { canvases.clear(); }
  string   title;
  unsigned ncanvases;
  vector<wCanvas_t *> canvases;
};

static map<string, string>      glmap_objpath2id;  // keep track of objects read in
static map<string, string>      glmap_id2objpath;  // keep track of objects read in
static map<string, wTH1 *>      glmap_id2histo;
static map<string, unsigned>    glmap_mhid2size;

static int gl_loopindex;
static bool gl_inloop;

static string nullstr;

static bool gl_verbose;

#include "spUtils.C"
#include "spLoop.C"
#include "spRootFile.C"
#include "spAlias.C"
#include "spSample.C"
#include "spMacro.C"
#include "spStyle.C"
#include "spTF1.C"
#include "spHisto.C"
#include "spHmath.C"
#include "spGraph.C"
#include "spTree.C"
#include "spLabelLatex.C"
#include "spLayout.C"
#include "spLegend.C"
#include "spLine.C"
#include "spBox.C"
#include "spMultiHist.C"
#include "spPad.C"
#include "spStack.C"
#include "spDraw.C"

//======================================================================

void clearEverything()
{
  glmap_objpath2id.clear();
  glmap_id2objpath.clear();
  glmap_aliii.clear();
  glmap_id2graph.clear();
  glmap_id2histo.clear();
  glmap_id2stack.clear();
  glmap_id2label.clear();
  glmap_id2latex.clear();
  glmap_id2legend.clear();
  glmap_id2line.clear();
  glmap_id2box.clear();
  glmap_id2rootfile.clear();
  glmap_id2style.clear();
  glmap_id2tf1.clear();
  glmap_id2tree.clear();
  glmap_id2sample.clear();
}

//======================================================================

void parseCanvasLayout(const string& layoutFile,
		       canvasSet_t&  cs)
{
  if (gl_verbose)
    cout << "Processing " << layoutFile << endl;

  FILE *oldfp=NULL, *fp = fopen(layoutFile.c_str(),"r");
  if (!fp) {
    cerr << "Error, couldn't open " << layoutFile << endl;
    exit(-1);
  }

  // Usually only need one canvas. Any other canvases will be
  // initialized with the parameters picked up for the first one here.
  //
  wCanvas_t *wc = new wCanvas_t(cs.title+"_1");
  cs.canvases.push_back(wc);

  int start=-1,stop=-1;
  bool   new_section = false;
  string section("");
  string theline;
  vector<string> v_tokens;
  bool keepgoing;
  do {
    keepgoing = getLine(fp,theline,"layoutintro");
  } while (keepgoing && theline[0] != '['); // skip lines until you find a section

  while (keepgoing) {
    if (!theline.size()) continue;
    if (theline[0] == '[') {
      Tokenize(theline,v_tokens," []");
      section = v_tokens[0];
    }

    int success = 0;

    if      (!section.size()) continue;

    if      (section == "LOOP") {
      success = processLoopSection (fp,theline,new_section,start,stop);
      if (success) {
	// Copy loop section to a temp file for repeated processing
	gl_inloop=true;
	oldfp = fp;
	fp = fopen("/tmp/splotsection.txt","w+");
	while (keepgoing) {
	  keepgoing = getLine(oldfp,theline,"layoutintro");
	  if (theline[0]=='[' && (theline.find("ENDLOOP") !=  string::npos)) break;
	  fprintf(fp,"%s\n",theline.c_str());
	}
	gl_loopindex=start;
	rewind(fp);
	do {
	  keepgoing = getLine(fp,theline,"loop setup");
	} while (keepgoing && theline[0] != '['); // skip lines until you find a section
	new_section = true;
      }
    }
    else if (section == "STYLE")  success=processStyleSection (fp,theline,new_section);
    else if (section == "LAYOUT") success=processLayoutSection(fp,theline,cs,new_section);

    else if (section == "PAD") {
      string padname = string("pad")+int2str((int)(wc->pads.size()+1))+string("frame");
      wPad_t *wpad = new wPad_t(padname);
      success=processPadSection(fp,theline,wpad,new_section);
      //assert(wpad->histo_ids.size());
      wc->pads.push_back(wpad);
    }
    else if (section == "MULTIPAD") {
      wPad_t *wpad = new wPad_t(Form("multipad%d",(int)wc->multipads.size()));
      success=processPadSection(fp,theline,wpad,new_section);
      //assert(wc->multipad->histo_ids.size());
      wc->multipads.push_back(wpad);
      // Have to read in multihist before assigning histos to pads, so pended to drawPlots
    }
    else if (section == "HISTO")     success=processHistoSection    (fp,theline,new_section);
    else if (section == "MULTIHIST") success=processMultiHistSection(fp,theline,new_section);
    else if (section == "HMATH")     success=processHmathSection    (fp,theline,new_section);
    else if (section == "STACK")     success=processStackSection    (fp,theline,new_section);
    else if (section == "GRAPH")     success=processGraphSection    (fp,theline,new_section);
    else if (section == "LEGEND")    success=processLegendSection   (fp,theline,new_section);
    else if (section == "LABEL")     success=processLabelSection    (fp,theline,new_section);
    else if (section == "LATEX")     success=processLatexSection    (fp,theline,new_section);
    else if (section == "LINE")      success=processLineSection     (fp,theline,new_section);
    else if (section == "BOX")       success=processBoxSection      (fp,theline,new_section);
    else if (section == "ALIAS")     success=processAliasSection    (fp,theline,new_section);
    else if (section == "SAMPLE")    success=processSampleSection   (fp,theline,new_section);
    else if (section == "TF1")       success=processTF1Section      (fp,theline,new_section);
    else if (section == "MACRO")     success=processMacroSection    (fp,theline,new_section);
    else if (section == "TREE")      success=processTreeSection     (fp,theline,new_section);
    else {
      cerr << "Unknown section " << section << " in " << layoutFile << endl;
    }

    if (!success) {
      cerr << "Last section processed ("<<section<<") reports failure, line="<< endl;
      cerr << gl_linect <<":\t" << theline << endl;
      exit(-1);
    }

    if (!keepgoing && gl_inloop) { // reached end of temp file
      if (++gl_loopindex <= stop) { // increment loop index, check loop exit
	rewind(fp);
	do {
	  keepgoing = getLine(fp,theline,"loop iteration");
	} while (keepgoing && theline[0] != '['); // skip lines until you find a section
	new_section = true;
      } else {
	gl_inloop=false; // loop is done
	fclose(fp);      // close temp file
	fp = oldfp;      // revert to original config file
      }
    }

    if (new_section) {
      new_section = false;
      keepgoing = true;
    } else {
      keepgoing = getLine(fp,theline,"layoutelse");
    }
  } // while (getline...
}                                                   // parseCanvasLayout

//======================================================================

canvasSet_t *initCanvasSet(const string& cLayoutFile)
{
  canvasSet_t *cs = new canvasSet_t(stripDirsAndSuffix(cLayoutFile));

  parseCanvasLayout(cLayoutFile, *cs);

  // Set Styles:
  gStyle->SetPalette(1,0); // always!

  wCanvas_t *wc = cs->canvases[0];
  unsigned npadsmin =  wc->npadsx*wc->npadsy;

  if (npadsmin) {
    wc->c1 = new TCanvas(wc->title.c_str(),wc->title.c_str(),
			 wc->padxdim*wc->npadsx,
			 wc->padydim*wc->npadsy);

    float left = wc->leftmargin;
    float bot  = wc->bottommargin;
    float rhgt = 1-wc->rightmargin;
    float top  = 1-wc->topmargin;
    wc->motherpad = new TPad("mother","",left,bot,rhgt,top);
    wc->c1->SetFillColor(wc->fillcolor);
    wc->motherpad->SetFillColor(wc->fillcolor);
    wc->motherpad->Draw();
    wc->motherpad->cd();

    wc->motherpad->Divide(wc->npadsx,wc->npadsy);
  			     // , wc->padxmargin,wc->padymargin);

    if (gl_verbose) {
      cout << "Canvas " << wc->c1->GetName() << " dimensions "
	   << wc->npadsx << "x" << wc->npadsy << endl;
      cout << "Canvas " << wc->c1->GetName() << " margins "
	   << wc->padxmargin << "x" << wc->padymargin << endl;
    }

  }
  return cs;
}                                                       // initCanvasSet

//======================================================================

void superPlot(const string& canvasLayout="canvas.txt",
	       bool savePlot2file=false,
	       bool verbose=true,
	       bool dotdrStyle=false)
{
  gl_verbose = verbose;

  clearEverything();

  if (dotdrStyle) {
    setTDRStyle();
    gROOT->ForceStyle();
    tdrStyle->SetOptTitle(1);
    tdrStyle->SetTitleFont(42);
    //tdrStyle->SetTitleFontSize(0.05);
    //tdrStyle->SetTitleBorderSize(2);
  } else
    setPRDStyle();


  drawPlots(*initCanvasSet(canvasLayout),savePlot2file);
}
