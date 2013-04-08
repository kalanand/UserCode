#include "TVectorD.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"

#ifndef LINELEN
#define LINELEN 512
#endif

static map<string, TVectorD *>    glmap_id2vec;

//======================================================================

void loadVectorsFromFile(const char *filename, 
			 const char *scanfmt,
			 TVectorD&   vx,
			 TVectorD&   vy,
			 char        xheader[],
			 char        yheader[])
{
  char linein[LINELEN];
  vector<double> v;

  FILE *fp = fopen(filename, "r");

  if (!fp) {
    cerr << "File failed to open, " << filename << endl;
    return;
  }

  if (gl_verbose)
    cout << "Loading vectors from file " << filename;

  while (!feof(fp) && fgets(linein,LINELEN,fp)) {
    double x, y;
    if( linein[0]=='#' ) {
      if (!v.size()) {           // first line, try to read headers
	TString hscanfmt(scanfmt);
	hscanfmt.ReplaceAll("lf","s");
	if( sscanf(&linein[1],hscanfmt.Data(),xheader,yheader) != 2 ) {
	  cerr << "failed to read in column headers" << endl;
	}
      }
      continue;                // comments are welcome
    }

    if( sscanf(linein, scanfmt, &x, &y) != 2 ) {
      cerr << "scan failed, file " << filename << ", line = " << linein << endl;
      return;
    }
    else {
      v.push_back(x); v.push_back(y);
    }
  }

  int vecsize = v.size()/2;
  vx.ResizeTo(vecsize);
  vy.ResizeTo(vecsize);

  if (gl_verbose) cout << "; read " << vecsize << " lines" << endl;

  for (int i=0; i<vecsize; i++) {
    vx[i] = v[2*i];
    vy[i] = v[2*i+1];
  }
}                                                 // loadVectorsFromFile

//======================================================================

void loadVectorsFromFile(const char *filename, 
			 TVectorD&   vx,
			 TVectorD&   vy,
			 TVectorD&   vz)
{
  char linein[LINELEN];
  vector<double> v;

  FILE *fp = fopen(filename, "r");

  if (!fp) {
    cerr << "File failed to open, " << filename << endl;
    return;
  }

  if (gl_verbose) cout << "Loading vectors from file " << filename;

  while (!feof(fp) && fgets(linein,LINELEN,fp)) {
    double x, y, z;
    if( linein[0]=='#' ) continue;                // comments are welcome
    if (sscanf(linein, "%lf %lf %lf", &x, &y, &z) != 3) {
      cerr << "scan failed, file " << filename << ", line = " << linein << endl;
      return;
    }
    else {
      v.push_back(x); v.push_back(y); v.push_back(z);
    }
  }

  int vecsize = v.size()/3;
  vx.ResizeTo(vecsize);
  vy.ResizeTo(vecsize);
  vz.ResizeTo(vecsize);

  if (gl_verbose) cout << "; read " << vecsize << " lines" << endl;

  for (int i=0; i<vecsize; i++) {
    vx[i] = v[2*i];
    vy[i] = v[2*i+1];
    vz[i] = v[2*i+2];
  }
}                                                 // loadVectorsFromFile

//======================================================================

void loadVectorsFromFile(const char *filename, 
			 const char *scanfmt,
			 TVectorD&   vx, TVectorD&   vy,
			 TVectorD&   exl,TVectorD&   exh,
			 TVectorD&   eyl,TVectorD&   eyh)
{
  char linein[LINELEN];
  vector<double> v;

  FILE *fp = fopen(filename, "r");

  if (!fp) {
    cerr << "File failed to open, " << filename << endl;
    return;
  }

  if (gl_verbose) cout << "Loading vectors from file " << filename;

  while (!feof(fp) && fgets(linein,LINELEN,fp)) {
    double x, y, ymin,ymax;
    if( linein[0]=='#' ) continue;                // comments are welcome
    if (sscanf(linein, scanfmt, &x, &ymin, &ymax, &y) != 4) {
      cerr << "scan failed, file " << filename << ", line = " << linein << endl;
      return;
    }
    else {
      v.push_back(x); v.push_back(y);
      v.push_back(0); v.push_back(0);
      v.push_back(y-ymin); v.push_back(ymax-y);
    }
  }

  int vecsize = v.size()/6;
  vx.ResizeTo(vecsize);
  vy.ResizeTo(vecsize);
  exl.ResizeTo(vecsize);
  exh.ResizeTo(vecsize);
  eyl.ResizeTo(vecsize);
  eyh.ResizeTo(vecsize);

  if (gl_verbose) cout << "; read " << vecsize << " lines" << endl;

  for (int i=0; i<vecsize; i++) {
    vx[i]  = v[6*i];
    vy[i]  = v[6*i+1];
    exl[i] = v[6*i+2];
    exh[i] = v[6*i+3];
    eyl[i] = v[6*i+4];
    eyh[i] = v[6*i+5];
  }
}                                                 // loadVectorsFromFile

//======================================================================

void
printVectorsToFile(wGraph_t *wg) // , const string& filename)
{
  if (wg->gr->InheritsFrom("TGraphAsymmErrors")) {
    TGraphAsymmErrors *agr = (TGraphAsymmErrors *)wg->gr;
    cout<<"lox\tctrx\thix\tloy\tctry\thiy\tex\tey"<<endl;
    for (int i=0; i<agr->GetN(); i++) {
      double x,y, ex, ey, lox,hix,loy,hiy;
      agr->GetPoint(i,x,y);
      ex = agr->GetErrorX(i);
      ey = agr->GetErrorY(i);
      lox = x-ex; hix = x+ex;
      loy = y-ey; hiy = y+ey;
      cout <<lox<<"\t"<<x<<"\t"<<hix<<"\t";
      cout <<loy<<"\t"<<y<<"\t"<<hiy<<"\t"<<ex<<"\t"<<ey<<endl;
    }
  }
}                                                  // printVectorsToFile

//======================================================================

TGraph *getGraphFromSpec(const string& gid,
			 const string& fullspec) // alias expansion assumed
{
  TGraph  *gr     = NULL;
  TFile *rootfile = NULL;
  vector<string> v_tokens;

  string gspec;
  string rootfn;

  if (gl_verbose) cout << "processing " << fullspec << endl;

  // process the (expanded) specification
  Tokenize(fullspec,v_tokens,":");
  if ((v_tokens.size() != 2) ||
      (!v_tokens[0].size())  ||
      (!v_tokens[1].size())    ) {
    cerr << "malformed root graph path file:folder/subfolder/.../graph " << fullspec << endl;
    return NULL;
  } else {
    rootfn = v_tokens[0];
    gspec  = v_tokens[1];
  }

  map<string,string>::const_iterator it = glmap_objpath2id.find(fullspec);
  if (it != glmap_objpath2id.end()) {
    // Allow the possibility to run the script a second time in root
    if (gl_verbose) cout << "Object " << fullspec << " already read in, here it is" << endl;
    map<string,wGraph_t *>::const_iterator git = glmap_id2graph.find(it->second);
    if (git == glmap_id2graph.end()) {
      if (gl_verbose) cout << "oops, sorry, I lied." << endl;
      return NULL;
    }
    gr = git->second->gr;
  } else {

    rootfile = openRootFile(rootfn);

    if (rootfile) {
      gr = (TGraph *)rootfile->Get(gspec.c_str());
      if (!gr) {
	cerr << "couldn't find " << gspec << " in " << rootfn << endl;
      } else {
	// success, record that you read it in.
	glmap_objpath2id.insert(pair<string,string>(fullspec,gid));
      }
    }
  }
  return gr;
}                                                    // getGraphFromSpec

//======================================================================
// takes the ID of a graph to fill into a pre-booked histo
//
void fill1DHistoFromGraph(std::string& gid,
			  wTH1 *&wth1)
{
  map<string, wGraph_t *>::const_iterator it=glmap_id2graph.find(gid);
  if (it==glmap_id2graph.end()) {
    cerr<<"Couldn't find graph with id "<<gid<<", define first"<<endl;
    exit(-1);
  }

  if (gl_verbose)
    cout << "Loading histo from graph " << gid << endl;

  TH1 *h = wth1->histo();

  for (int ibin=1; ibin <= h->GetNbinsX(); ibin++)
    h->SetBinContent(ibin,it->second->gr->Eval(h->GetBinCenter(ibin)));

}                                               //  fill1DHistoFromGraph

//======================================================================

bool                              // returns true if success
processGraphSection(FILE *fp,
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

  if (gl_verbose) cout << "Processing graph section" << endl;

  new_section=false;

  while (getLine(fp,theline,"graph")) {
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
	cerr << "no more than one id per graph section allowed " << value << endl;
	break;
      }

      gid = new string(value);

    //------------------------------
    } else if (key == "vectorfile") {
    //------------------------------
      if (!gid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      string path     = value;
      string scanspec = asymerrors ?  "%lf %lf %lf %lf" : "%lf %lf";

      Tokenize(value,v_tokens,",");
      if (v_tokens.size() > 1) {
	path     = v_tokens[0];
	scanspec = v_tokens[1];
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
      if (wg) {
	cerr << "graph already defined" << endl; continue;
      }

      wg = new wGraph_t();

      Tokenize(value,v_tokens,",");

      string path=v_tokens[0];
      if (inSet<string>(glset_graphFilesReadIn,path)) {
	cerr << "vector file " << path << " already read in" << endl; break;
      }

      switch(v_tokens.size()) {
      case 1:  wg->gr2d = new TGraph2D(path.c_str()); break;
      case 2:  wg->gr2d = new TGraph2D(path.c_str(),v_tokens[1].c_str()); break;
      case 3:  wg->gr2d = new TGraph2D(path.c_str(),v_tokens[1].c_str(),v_tokens[2].c_str()); break;
      default:
	cerr << "malformed vectorfile2d spec path[,format[,option]] " << value << endl;
	break;
      }

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

      if ( gl_verbose ) { 
	std::cout << "Dump of bin contents, errors" << std::endl ; 
	if ( tmph1->GetNbinsX() == tmph2->GetNbinsX() ) { 
	  for (int ib=1; ib<=tmph1->GetNbinsX(); ib++) {
	    std::cout << ib << ": " << tmph1->GetBinContent(ib) << "+/-" << tmph1->GetBinError(ib) 
		      << ", " << tmph2->GetBinContent(ib) << "+/-" << tmph2->GetBinError(ib) << std::endl ; 
	  }
	} else { 
	  cerr << "Histograms being divided do not have same number of bins!!!" << endl ; 
	}
      }

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

      // Fix in case something broke

      for (int i=0; i<wg->gr->GetN(); i++) {
          if ( wg->gr->GetErrorYhigh(i) == 0. || wg->gr->GetErrorYlow(i) == 0 ) { // Something bad happened
              if ( gl_verbose ) std::cout << "Problem with Bayes divide, checking..." << std::endl ;
              double pass  = tmph1->GetBinContent(i+1) ; 
              double total = tmph2->GetBinContent(i+1) ;
              if ( gl_verbose ) std::cout << pass << "/" << total << std::endl ;
              if ( pass == total ) {
                  if ( gl_verbose ) std::cout << "Everything OK" << std::endl ;
              } else { 
                  if ( gl_verbose ) std::cout << "Yep, something is broken" << std::endl ;
                  double xval, yval ;
                  wg->gr->GetPoint(i,xval,yval) ;
                  yval = pass / total ;
                  // Use simplified efficiency assumption
                  // double u1 = tmph1->GetBinError(i+1) / tmph1->GetBinContent(i+1) ; 
                  // double u2 = tmph2->GetBinError(i+1) / tmph2->GetBinContent(i+1) ; 
                  // double unc = yval * sqrt( u1*u1 + u2*u2 ) ; 
                  double unc = sqrt( yval * (1.-yval)/tmph2->GetBinContent(i+1) ) ; 
                  double uhi = ( (yval + unc > 1.)?(1.-yval):(unc) ) ; 
                  double ulo = ( (yval - unc < 0.)?(yval):(unc) ) ;
                  wg->gr->SetPoint(i,xval,yval) ;
                  ((TGraphAsymmErrors*)wg->gr)->SetPointError(i,wg->gr->GetErrorXlow(i),wg->gr->GetErrorXhigh(i),ulo,uhi) ; 
//                   wg->gr->SetPointEYhigh(i,uhi) ; 
//                   wg->gr->SetPointEYlow(i,ulo) ; 
              }
          }   
          if (gl_verbose) std::cout << i << ": " << wg->gr->GetErrorYhigh(i) << "/" << wg->gr->GetErrorYlow(i) << std::endl ; 
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
