#include "TVectorD.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"

#ifndef LINELEN
#define LINELEN 512
#endif

void dumpAxis(const TAxis *axis);

struct wGraph_t {
  wGraph_t() :
    lcolor(1),lstyle(1),lwidth(1),
    mcolor(1),mstyle(3),msize(1),
    fcolor(0),fstyle(1001),
    leglabel(""),drawopt(""),legdrawopt(""),
    xax(new TAxis(1,0,0)),yax(new TAxis(1,0,0)),zax(new TAxis(1,0,0)),
    gr(NULL),gr2d(NULL),fitfn(NULL),contours(NULL) {
    xax->ResetAttAxis("x"); // dumpAxis(xax);
    yax->ResetAttAxis("y");
    zax->ResetAttAxis("z");
  }
  wGraph_t(const wGraph_t& wg,const string& newname) :
    lcolor(wg.lcolor),lstyle(wg.lstyle),lwidth(wg.lwidth),
    mcolor(wg.mcolor),mstyle(wg.mstyle),msize(wg.msize),
    leglabel(wg.leglabel),drawopt(wg.drawopt),legdrawopt(wg.legdrawopt),
    xax(new TAxis(1,0,0)),yax(new TAxis(1,0,0)),zax(new TAxis(1,0,0)),
    gr(NULL), gr2d(NULL),fitfn(NULL),contours(NULL) {
    xax->ResetAttAxis("x");
    yax->ResetAttAxis("y");
    zax->ResetAttAxis("z");
    if (wg.gr)
      gr = (TGraph *)wg.gr->Clone(newname.c_str());
    else if (wg.gr2d)
      gr2d = (TGraph2D *)wg.gr2d->Clone(newname.c_str());
    }
  int  lcolor,lstyle,lwidth;
  int  mcolor,mstyle,msize;
  int  fcolor,fstyle;
  string leglabel;
  string drawopt;
  string legdrawopt;
  TAxis *xax,*yax,*zax; /* have to save separately since the internal graph
			   axes are not created until after being drawn. */
  TGraph *gr;
  TGraph2D *gr2d;
  TF1 *fitfn;
  TVectorD *contours;
};

static set<string> glset_graphFilesReadIn;  // keep track of graphs read in

static map<string, wGraph_t *>    glmap_id2graph;

//======================================================================

bool                              // returns true if success
processMultiGraphSection(FILE *fp,
			 string& theline,
			 bool& new_section)
{
  vector<string> v_tokens;
  TString  title;
  string   *gid  = NULL;
  TVectorD vx,vy,vz,exl,exh,eyl,eyh;
  float xoffset=0.0,yoffset=0.0, yscale=1.0;
  float xmin=0.,xmax=0.,ymin=0.,ymax=0.,zmin=0.,zmax=0.;
  bool asymerrors = false;
  wGraph_t *wg = NULL;

  char xheader[80],yheader[80];
  xheader[0]=0;
  yheader[0]=0;

  if (gl_verbose) cout << "Processing multigraph section" << endl;

  new_section=false;

  while (getLine(fp,theline,"multigraph")) {
    if (!theline.size()) continue;
    if (theline[0] == '#') continue; // comments are welcome

    if (theline[0] == '[') {
      new_section=true;
      break;
    }

    string key, value;
    if (!getKeyValue(theline,key,value)) continue;

    //--------------------
    if (key == "id") {
    //--------------------
      if (gid != NULL) {
	cerr << "no more than one id per section allowed " << value << endl;
	break;
      }

      gid = new string(value);

    //------------------------------
    } else if (key == "graphs") {
    //------------------------------
      if (!gid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }

      Tokenize(value,v_tokens,","); 

      for (size_t i=0; i<v_tokens.size(); i++) {
	map<string,wGraph_t *>::const_iterator it = glmap_id2graph.find(v_tokens[i]);
	if( it == glmap_id2graph.end() ) {
	  cerr << "Graph ID " << v_tokens[i] << " not found,";
	  cerr << "graphs must be defined before multigraph section" << endl;
	  break;
	}
	wg  = new wGraph_t(*(it->second),*gid);
	
      }
      if (wg) {
	cerr << "graph already defined" << endl; continue;
      }
      if (inSet<string>(glset_graphFilesReadIn,path)) {
	cerr << "vector file " << path << " already read in" << endl; break;
      }

      wg = new wGraph_t();

      if (asymerrors)
	loadVectorsFromFile(path.c_str(),scanspec.c_str(),vx,vy,exl,exh,eyl,eyh);
      else
	loadVectorsFromFile(path.c_str(),scanspec.c_str(),vx,vy,xheader,yheader);

      if (strlen(xheader)) wg->xax->SetTitle(xheader);
      if (strlen(yheader)) wg->yax->SetTitle(yheader);

    //------------------------------
    } else if (key == "vectorfile2d") {
    //------------------------------
      if (!gid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      string path = value;
      if (wg) {
	cerr << "graph already defined" << endl; continue;
      }
      if (inSet<string>(glset_graphFilesReadIn,path)) {
	cerr << "vector file " << path << " already read in" << endl; break;
      }

      wg = new wGraph_t();

      wg->gr2d = new TGraph2D(path.c_str());

      if (wg->gr2d->IsZombie()) {
	cerr << "Unable to make Graph2D from file " << path << endl;
	exit(-1);
      }
      wg->gr2d->SetName(gid->c_str());

    //------------------------------
    } else if (key == "path") {
    //------------------------------

      if (!gid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      if (wg) {
	cerr << "graph already defined" << endl; continue;
      }
      wg = new wGraph_t();
      wg->gr  = getGraphFromSpec(*gid,value);
      if (!wg->gr) continue;

    //------------------------------
    } else if (key == "clone") {
    //------------------------------

      if (!gid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      if (wg) {
	cerr << "graph already defined" << endl; continue;
      }
      map<string,wGraph_t *>::const_iterator it = glmap_id2graph.find(value);
      if( it == glmap_id2graph.end() ) {
	cerr << "Graph ID " << value << " not found,";
	cerr << "clone must be defined after the clonee" << endl;
	break;
      }
      wg  = new wGraph_t(*(it->second),*gid);

    //------------------------------
    } else if (key == "bayesdiv") {
    //------------------------------

      Tokenize(value,v_tokens,",/"); // either comma-separated or using '/'
      if (v_tokens.size() != 2) {
	cerr << "expect comma-separated list of exactly two histo specs to divide! ";
	cerr << theline << endl;
	continue;
      }

      TH1 *tmph1 = (TH1 *)findHisto(v_tokens[0]); if (!tmph1) exit(-1);
      TH1 *tmph2 = (TH1 *)findHisto(v_tokens[1]); if (!tmph2) exit(-1);

      cout << tmph1->GetNbinsX() << " " << tmph2->GetNbinsX() << endl;

      wg = new wGraph_t();

      // equivalent to BayesDivide
      //
      if (gl_verbose) wg->gr = new TGraphAsymmErrors(tmph1,tmph2,"debug");
      else            wg->gr = new TGraphAsymmErrors(tmph1,tmph2,"");
      //if (gl_verbose) wg->gr = new TGraphAsymmErrors(tmph1,tmph2,"cl=0.683 b(1,1) mode v");
      //else            wg->gr = new TGraphAsymmErrors(tmph1,tmph2,"cl=0.683 b(1,1) mode");
      if (!wg->gr) {
	cerr << "BayesDivide didn't work! wonder why..." << endl;
	continue;
      } else if (gl_verbose) {
	cout << wg->gr->GetN() << " points in the graph" << endl;
      }

    } else if (!wg) {
      cerr<<"One of keys path,clone,vectorfile,vectorfile2d or bayesdiv must be defined before key..."<<key<<endl;
    } else {
      if     ( key == "xoffset" )      xoffset   = str2flt(value);
      else if( key == "yoffset" )      yoffset   = str2flt(value);
      else if( key == "yscale" )       yscale    = str2flt(value);
      else if( key == "title"  )       title     = TString(value);
      else if( key == "xtitle" )       wg->xax->SetTitle(value.c_str());
      else if( key == "ytitle" )       wg->yax->SetTitle(value.c_str());
      else if( key == "ztitle" )       wg->zax->SetTitle(value.c_str());
      else if( key == "xtitleoffset" ) wg->xax->SetTitleOffset(str2flt(value));
      else if( key == "ytitleoffset" ) wg->yax->SetTitleOffset(str2flt(value));
      else if( key == "ztitleoffset" ) wg->zax->SetTitleOffset(str2flt(value));
      else if( key == "xmin" )         xmin      = str2flt(value);
      else if( key == "xmax" )         xmax      = str2flt(value);
      else if( key == "ymin" )         ymin      = str2flt(value);
      else if( key == "ymax" )         ymax      = str2flt(value);
      else if( key == "zmin" )         zmin      = str2flt(value);
      else if( key == "zmax" )         zmax      = str2flt(value);
      else if( key == "linecolor" )    wg->lcolor  = str2int(value);
      else if( key == "linestyle" )    wg->lstyle  = str2int(value);
      else if( key == "linewidth" )    wg->lwidth  = str2int(value);
      else if( key == "markercolor" )  wg->mcolor  = str2int(value);
      else if( key == "markerstyle" )  wg->mstyle  = str2int(value);
      else if( key == "markersize"  )  wg->msize   = str2int(value);
      else if( key == "fillcolor" )    wg->fcolor  = str2int(value);
      else if( key == "fillstyle" )    wg->fstyle  = str2int(value);
      else if( key == "xndiv" )        wg->xax->SetNdivisions(str2int(value));
      else if( key == "yndiv" )        wg->yax->SetNdivisions(str2int(value));
      else if( key == "asymerrors" )   asymerrors  = (bool)str2int(value);
      else if( key == "leglabel" )     wg->leglabel   = value;
      else if( key == "draw" )         wg->drawopt    = value;
      else if( key == "legdraw" )      wg->legdrawopt = value;
      else if( key == "setprecision" ) cout << setprecision(str2int(value));
      else if( key == "printvecs2file") printVectorsToFile(wg); // ,value);
      else if( key == "fittf1" ) {
	TF1 *tf1 = findTF1(value);
	if( !tf1 ) {
	  cerr << "TF1 " << value << " must be defined first" << endl;
	  continue;
	}
	string funcnewname = value+(*gid);
	wg->fitfn = new TF1(*tf1);
	wg->fitfn->SetName(funcnewname.c_str());
      }
      else if ( key == "contours" ) {
	Tokenize(value,v_tokens,",");
	wg->contours = new TVectorD(v_tokens.size());
	for (size_t i=0; i<v_tokens.size(); i++)
	  wg->contours[i] = str2flt(v_tokens[i]);
      }
      else {
	cerr << "unknown key " << key << endl;
      }
#if 0
      processCommonHistoParams(key,value,*wh);
#endif
    }
  }

  //cout << title << endl;
  if (wg->gr2d) {
    wg->gr2d->SetTitle(title);
    wg->gr2d->SetLineStyle   (wg->lstyle);
    wg->gr2d->SetLineColor   (wg->lcolor);
    wg->gr2d->SetLineWidth   (wg->lwidth);
    wg->gr2d->SetMarkerColor (wg->mcolor);
    wg->gr2d->SetMarkerStyle (wg->mstyle);
    wg->gr2d->SetMarkerSize  (wg->msize);
    wg->gr2d->SetFillStyle   (wg->fstyle);
    wg->gr2d->SetFillColor   (wg->fcolor);
    if (zmax>zmin)  
      wg->zax->SetLimits(zmin,zmax);
  } else {
    if (vx.GetNoElements()) { // load utility guarantees the same size for both
      if (yscale  != 1.0) vy *= yscale;
      if (xoffset != 0.0) vx += xoffset;
      if (yoffset != 0.0) vy += yoffset;
      if (asymerrors) 
	wg->gr = new TGraphAsymmErrors(vx,vy,exl,exh,eyl,eyh);
      else
	wg->gr = new TGraph(vx,vy);
    }
    wg->gr->UseCurrentStyle();
    wg->gr->SetTitle(title);
    wg->gr->SetLineStyle   (wg->lstyle);
    wg->gr->SetLineColor   (wg->lcolor);
    wg->gr->SetLineWidth   (wg->lwidth);
    wg->gr->SetMarkerColor (wg->mcolor);
    wg->gr->SetMarkerStyle (wg->mstyle);
    wg->gr->SetMarkerSize  (wg->msize);
    wg->gr->SetFillStyle   (wg->fstyle);
    wg->gr->SetFillColor   (wg->fcolor);

    if (xmax>xmin) wg->xax->SetLimits(xmin,xmax);
    if (ymax>ymin) wg->yax->SetLimits(ymin,ymax);
  }

  glmap_id2graph.insert(pair<string,wGraph_t *>(*gid,wg));

  return (wg != NULL);
}                                                 // processGraphSection

//======================================================================
