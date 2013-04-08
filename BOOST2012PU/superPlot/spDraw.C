#include "TGaxis.h"
#include "THStack.h"
#include "TArrayD.h"
#include "drawStandardTexts.C"

//======================================================================

void drawInPad(wPad_t *wp, wTH1& myHisto,bool firstInPad,
	       const string& altdrawopt="")
{
  wp->vp->SetFillColor(wp->fillcolor);

  gStyle->cd();

  wp->vp->SetLogx(wp->logx);
  wp->vp->SetLogy(wp->logy);
  wp->vp->SetLogz(wp->logz);
  wp->vp->SetGridx(wp->gridx);
  wp->vp->SetGridy(wp->gridy);

  if (!gl_verbose) myHisto.ShutUpAlready();

  if (firstInPad) {
    if (wp->topmargin)    wp->vp->SetTopMargin   (wp->topmargin);
    if (wp->bottommargin) wp->vp->SetBottomMargin(wp->bottommargin);
    if (wp->rightmargin)  wp->vp->SetRightMargin (wp->rightmargin);
    if (wp->leftmargin)   wp->vp->SetLeftMargin  (wp->leftmargin);

    altdrawopt.size() ? myHisto.Draw(altdrawopt) : myHisto.Draw();

    // Now we can set the axis attributes and range:
    //myHisto.SetAxes();
  }
  else {
    if (!myHisto.statsAreOn()) altdrawopt.size() ?
				 myHisto.Draw(altdrawopt+ " same") :
				 myHisto.DrawSame();
    else                       altdrawopt.size() ?
				 myHisto.Draw(altdrawopt+ " sames") :
				 myHisto.DrawSames();
  }

#if 0
  myHisto.histo()->Sumw2();
  TArrayD *sumw2 = myHisto.histo()->GetSumw2();
  cout << sumw2->GetSize() << endl;
  for (int i=0; i<sumw2->GetSize(); i++)
    printf ("%g ", (*sumw2)[i]);
  cout << endl;
#endif

  wp->vp->RedrawAxis();

  wp->vp->Update();
}                                                           // drawInPad

//======================================================================

template<class T>
void drawInPad(wPad_t *wp, T *obj,const string& indrawopt, bool firstInPad=false)
{
  wp->vp->SetFillColor(wp->fillcolor);

  gStyle->cd();

  wp->vp->SetLogx(wp->logx);
  wp->vp->SetLogy(wp->logy);
  wp->vp->SetLogz(wp->logz);
  wp->vp->SetGridx(wp->gridx);
  wp->vp->SetGridy(wp->gridy);

  if (firstInPad) {
    if (wp->topmargin)    wp->vp->SetTopMargin   (wp->topmargin);
    if (wp->bottommargin) wp->vp->SetBottomMargin(wp->bottommargin);
    if (wp->rightmargin)  wp->vp->SetRightMargin (wp->rightmargin);
    if (wp->leftmargin)   wp->vp->SetLeftMargin  (wp->leftmargin);
  }

  TString drawopt = indrawopt;
  if (!firstInPad) 
    drawopt += " SAME";

  if (gl_verbose)
    cout<<"Drawing "<<obj->GetName()<<" with option(s) "<<drawopt<<endl;

  obj->Draw(drawopt);



  wp->vp->Update();
}                                                           // drawInPad

//======================================================================

void drawInPad(wPad_t *wp, wStack_t *ws, bool firstInPad,
	       const string& drawopt="")
{
  wp->vp->SetFillColor(wp->fillcolor);

  gStyle->cd();

  wp->vp->SetLogx(wp->logx);
  wp->vp->SetLogy(wp->logy);
  wp->vp->SetLogz(wp->logz);
  wp->vp->SetGridx(wp->gridx);
  wp->vp->SetGridy(wp->gridy);

  if (firstInPad) {
    if (wp->topmargin)    wp->vp->SetTopMargin   (wp->topmargin);
    if (wp->bottommargin) wp->vp->SetBottomMargin(wp->bottommargin);
    if (wp->rightmargin)  wp->vp->SetRightMargin (wp->rightmargin);
    if (wp->leftmargin)   wp->vp->SetLeftMargin  (wp->leftmargin);

    // The sum histogram is used to set up the plot itself, far easier
    // this way than messing with the histogram internal to the stack,
    // and allows for fits on the sum.
    //
    ws->sum->Draw(drawopt);
  } else {
    if (!strstr(drawopt.c_str(),"nostack")) {
      if (!ws->sum->statsAreOn()) drawopt.size() ?
				  ws->sum->Draw(drawopt+ " same") :
				  ws->sum->DrawSame();
      else {                      drawopt.size() ?
				  ws->sum->Draw(drawopt+ " sames") :
				  ws->sum->DrawSames();
      }
    }
  }

  if (gl_verbose)
    cout << "Drawing stack with option AH" << endl;
  ws->stack->Draw("AH SAME");

  ws->sum->DrawFits("same");   wp->vp->Update();
  ws->sum->DrawStats();        wp->vp->Update();

  //ws->sum->Draw("AXIG SAME");
  //wp->vp->Update();
}                                                           // drawInPad

//======================================================================

void saveCanvas2File(wCanvas_t *wc, const string& namefmt)
{
  string picfilename;
  size_t len = namefmt.length();
  size_t pos,pos0 = 0;

  do {
    // Format of output filename specified in "savenamefmt"
    // %F = first file read in
    // %C = configuration file name
    //
    pos = namefmt.find('%',pos0);
    if (pos == string::npos) {
      picfilename += namefmt.substr(pos0); // no more format codes, finish up
      break;
    } else if (pos>pos0) {
      picfilename+=namefmt.substr(pos0,pos-pos0);
    }
    // expand format codes
    if (pos != len-1) {  // make sure '%' wasn't the last character
      pos0=pos+1;
      switch (namefmt[pos0]) { 
      case 'C': picfilename += wc->title; break;
      case 'P': { // full path contained in glmap
	string datafile;
	map<string,TFile*>::const_iterator it = glmap_id2rootfile.begin();
	if (it != glmap_id2rootfile.end())
	  datafile = it->first.substr(0,it->first.find_last_of('.'));
	picfilename += datafile;
      }	break;
      case 'F': { // filename stripped of path info
	string datafile;
	map<string,TFile*>::const_iterator it = glmap_id2rootfile.begin();
	if (it != glmap_id2rootfile.end()) {
	  size_t pos1 = it->first.find_last_of('/');
	  datafile =  (pos1 != string::npos) ?
	    it->first.substr(pos1+1,it->first.find_last_of('.')-pos1-1) :
	    it->first.substr(0,it->first.find_last_of('.'));
	}
	picfilename += datafile;
      }	break;
      default:
	cerr<<"Unrecognized format code %"<<namefmt[pos0]<<endl;
	break;
      }
      pos0++;
    }
  } while (pos0<len);

  cout << "saving to..." << picfilename << endl;
  wc->c1->SaveAs(picfilename.c_str());
}                                                     // saveCanvas2File

//======================================================================

unsigned assignHistos2Multipad(canvasSet_t& cs) // returns total number of occupied pads
{
  wCanvas_t *wc0       = cs.canvases[0];
  unsigned npadspercan = wc0->npadsx*wc0->npadsy;
  unsigned npadsall    = cs.ncanvases*npadspercan;

  // Note: wci can't be wCanvas_t& because apparently filling the
  //       vector messes up the reference to the first element!
  //
  wCanvas_t wci(*wc0); // doesn't copy member "pads"
  wCanvas_t *wc = wc0;  // Divvy up the pads among multiple canvases if so specified

  unsigned ipad=0,ipad2start=0; // ipad=global pad index, not the Apple product!
  unsigned m=0;                 // m=multipad index

  // A multipad potentially references multiple sets of graphical
  // objects (histograms, graphs) to overlay. The objects within a set
  // are plotted in sequential pads, but another set of objects
  // assigned to the same multipad are overlaid sequentially on the
  // previous set. Ideally each set assigned to the same multipad
  // contains the same number of objects, but not necessarily.
  // Multiple multipads can exist; their collection of object sets are
  // assigned sequentially to ranges of available pads defined in the
  // layout section.
  //
  for (m=0; m<wc0->multipads.size(); m++) {
    wPad_t  *mp = wc0->multipads[m];

    // figure out how many pads this multipad spans, taking max of the referenced histo sets
    unsigned npads4mp=0;
    for (unsigned k=0; k<mp->histo_ids.size(); k++) {
      std::map<string,unsigned>::const_iterator it=glmap_mhid2size.find(mp->histo_ids[k]);
      if (it==glmap_mhid2size.end()) {
	if (findHisto(mp->histo_ids[k]),"")
	  npads4mp = std::max<unsigned>(npads4mp, 1);
      } else {
	npads4mp = std::max(npads4mp,it->second);
      }
    }

    npads4mp = std::min(npads4mp,npadsall-ipad2start);

    if (!npads4mp) continue;

    // Plot all objects for this multipad that can be plotted within
    // the span of assigned pads

    // Create the pads first
    for (ipad=ipad2start; ipad<ipad2start+npads4mp; ipad++) {

      unsigned   i  =  ipad % npadspercan;      // index to current pad in canvas
      unsigned cnum = (ipad / npadspercan) + 1; // current canvas number

      if (!i) { // first pad in new canvas
	if (cnum > cs.canvases.size()) {
	  if (gl_verbose)
	    cout << "making new canvas" << endl;
	  wc = new wCanvas_t(wci);
	  cs.canvases.push_back(wc);
	  wc->title = cs.title + "_" + int2str(cnum);
	  wc->pads.clear();
	  wc->latex_ids = wc0->latex_ids;
	}
      }

      wPad_t *wp = new wPad_t(*(mp));
      wp->histo_ids.clear(); 
      wp->altyh_ids.clear(); 
      wp->graph_ids.clear(); 
      if (i) wp->legid.clear(); // don't automatically propagate legend to all pads

      wc->pads.push_back(wp);
    }

    unsigned g,h,j,k,l,y;     /* g=graph index in current graph set
				 h=histo index in current histo set
				 j=graph id (set) index
				 k=histo_id (set) index
				 l=altyhisto index
				 y=histoindex in current altyhisto set */
    g=h=j=k=l=y=0;

    // Now assign
    for (ipad=ipad2start; ipad<ipad2start+npads4mp; ipad++) {

      unsigned   i  =  ipad % npadspercan;      // index to current pad in canvas
      unsigned cnum = (ipad / npadspercan) + 1; // current canvas number

      wc = cs.canvases[cnum-1];
      wPad_t *wp = wc->pads[i];

      bool foundhisto=false;
      if (k < mp->histo_ids.size()) {
	string hid=mp->histo_ids[k];

	if (!h && gl_verbose) {
	  cout<<"Assigning histo/histo set "<<hid<<" to pads ";
	  cout<<ipad2start<<"-"<<ipad2start+npads4mp-1<<endl;
	}

	if (!h && findHisto(hid, "switching to histo set"))
	  foundhisto=true;
	else {
	  hid = hid +"_"+int2str(h++);
	  if (findHisto(hid, "hit the end of histo set"))
	    foundhisto=true;
	}
	if (foundhisto) {
	  // now we associate histogram sets with the pad set
	  wp->histo_ids.push_back(hid);
	}
      }

      // altyhistos:
      if (l < mp->altyh_ids.size()) {
	string ahid=mp->altyh_ids[l];
	bool foundaltyh=false;
	if (!y && findHisto(ahid, "switching to set"))
	  foundaltyh=true;
	else {
	  ahid = ahid +"_"+int2str(y++);
	  if (findHisto(ahid, "hit the end of histo set"))
	    foundaltyh=true;
	}
	if (foundaltyh) {
	  // now we associate histogram sets with the pad set
	  wp->altyh_ids.push_back(ahid);
	}
      }

      // graphs:
      bool foundgraph=false;
      if (j < mp->graph_ids.size()) {
	string gid=mp->graph_ids[j];
	if (!g && findGraph(gid, "switching to set"))
	  foundhisto=true;
	else {
	  gid = gid +"_"+int2str(g++);
	  if (findGraph(gid, "hit the end of graph set"))
	    foundgraph=true;
	}
	if (foundgraph) {
	  // now we associate histogram sets with the pad set
	  wp->graph_ids.push_back(gid);
	}
      }

      if (!foundgraph && !foundhisto) { // reset to next graph/histo ids
	ipad=ipad2start;
	g=h=y=0;
	++j;  ++k; ++l;
	if (j == mp->graph_ids.size() &&
	    k == mp->histo_ids.size()   ) break;
      }
    } // pad loop

    ipad2start += npads4mp;

  } // multipads loop

  return std::min(npadsall,ipad);
}                                               // assignHistos2Multipad

//======================================================================

unsigned assignPads2Canvases(canvasSet_t& cs)
{
  wCanvas_t *wc0    = cs.canvases[0];
  unsigned npads    = wc0->npadsx*wc0->npadsy;
  unsigned npadsall = cs.ncanvases*npads;

  // Note: wci can't be wCanvas_t& because apparently filling the
  //       vector messes up the reference to the first element!
  //
  wCanvas_t wci(*wc0); // doesn't copy member "pads"
  wCanvas_t *wc = wc0;

  // Divvy up the pads among multiple canvases if so specified

  unsigned ipad=0;                      // ipad=global pad index 
  for (;ipad<wc0->pads.size(); ipad++) {
    unsigned   i  =  ipad % npads;      // index to current pad in canvas
    unsigned cnum = (ipad / npads) + 1; // current canvas number

    if (cnum > cs.ncanvases) break;

    if ((i==0) && (cnum > cs.canvases.size())) {
      if (gl_verbose)
	cout << "making new canvas" << endl;
      wc = new wCanvas_t(wci);
      cs.canvases.push_back(wc);
      wc->title = cs.title + "_" + int2str(cnum);
      wc->pads.clear();
      wc->latex_ids = wc0->latex_ids;
    }

    wc->pads.push_back(wc0->pads[ipad]);
  }

  return std::min(npadsall,ipad+1);
}                                                 // assignPads2Canvases

//======================================================================

void  drawPlots(canvasSet_t& cs,bool savePlots2file)
{
  wCanvas_t *wc0 = cs.canvases[0];
  unsigned npads = wc0->npadsx*wc0->npadsy;
  unsigned npadsall = cs.ncanvases*npads;

  if (!npads) {
    if (gl_verbose) cout << "Nothing to draw, guess I'm done." << endl;
    return; // no pads to draw on.

  } else if (!wc0->pads.size()) {

    /********************************************************
     * CHECK MULTIPAD OPTION, ASSIGN HISTOS TO PADS/CANVASES
     ********************************************************/

    if (wc0->multipads.size()) {
      npadsall = assignHistos2Multipad(cs);
    } else {
      cerr << "npads>0, but no pad specs supplied, exiting." << endl;
      return; // no pads to draw on.
    }
  } else if (cs.ncanvases>1) {
    npadsall = assignPads2Canvases(cs);
  } else {
    npadsall = std::min(npadsall,(unsigned)wc0->pads.size());
  }

  wc0->c1->cd();

  if (gl_verbose)
    cout << "Drawing on " << npadsall << " pad(s)" << endl;

  wLegend_t *wleg = NULL;

  /***************************************************
   * LOOP OVER PADS...
   ***************************************************/

  //vector<vector<string> >::const_iterator it;
  for (unsigned ipad = 0; ipad< npadsall; ipad++) {

    if (gl_verbose) cout << "Drawing pad# " << ipad+1 << endl;

    unsigned ipadc =  ipad % npads;
    unsigned cnum  = (ipad / npads) + 1;

    wCanvas_t *wc = cs.canvases[cnum-1];

    if (!ipadc) {
      if (cnum-1) { // first canvas already created
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
	wc->motherpad->Divide(wc->npadsx,wc->npadsy); // , wc->padxmargin,wc->padymargin);
      }

      /***************************************************
       * CHECK FOR LATEX OBJECTS ON THE CANVAS
       ***************************************************/
      
      wc->c1->cd();
      for (unsigned j=0; j<wc->latex_ids.size(); j++) {
	string& lid = wc->latex_ids[j];
	map<string,TLatex *>::const_iterator it = glmap_id2latex.find(lid);
	if (it == glmap_id2latex.end()) {
	  cerr << "ERROR: latex id " << lid << " never defined in layout" << endl;
	  exit (-1);
	}
	TLatex *ltx = it->second;
	ltx->Draw();
	wc->c1->Update();
      }
    }

    wPad_t *& wp = wc->pads[ipadc];
    wp->vp = wc->motherpad->cd(ipadc+1);

    if (!wp->histo_ids.size() &&
	!wp->stack_ids.size() &&
	!wp->graph_ids.size() &&
	!wp->macro_ids.size()) {
      cerr << "ERROR: pad #" << ipadc+1 << " has no ids defined for it";
      cerr << ", continuing to the next" << endl;
      continue;
    }


#if 0
    /***************************************************
     * Draw the frame first:
     * (Fix up frame since it can't be auto-scaled:)
     ***************************************************/
    string& hid0 = wp->histo_ids[0];
    map<string,wTH1 *>::const_iterator it = glmap_id2histo.find(hid0);
    if (it == glmap_id2histo.end()) {
      cerr << "ERROR: id0 " << hid0 << " never defined in layout" << endl;
      return;
    }
    wTH1 *myHisto = it->second;
    TH1  *h = myHisto->histo();

    if (wp->hframe->histo()->GetXaxis()->GetXmin() <=
	wp->hframe->histo()->GetXaxis()->GetXmax())
      wp->hframe->histo()->GetXaxis()->SetRangeUser(h->GetXaxis()->GetXmin(),
						    h->GetXaxis()->GetXmax());
    if (wp->hframe->histo()->GetYaxis()->GetXmin() <= 
	wp->hframe->histo()->GetYaxis()->GetXmax())
      wp->hframe->histo()->GetYaxis()->SetRangeUser(h->GetYaxis()->GetXmin(),
						    h->GetYaxis()->GetXmax());

    wp->hframe->SetStats(0);
    //wp->hframe->Draw("AXIS");
#endif

    /***************************************************
     * Check for external macros to run on the pad
     ***************************************************/
    for (size_t i=0; i<wp->macro_ids.size(); i++) {
      map<string,string>::const_iterator it = glmap_objpath2id.find(wp->macro_ids[i]);
      if (it != glmap_objpath2id.end()) {
	string path = it->second;
	int error;
	gROOT->Macro(path.c_str(), &error, kTRUE); // update current pad
	if (error) {
	  static const char *errorstr[] = {
	    "kNoError","kRecoverable","kDangerous","kFatal","kProcessing" };
	  cerr << "ERROR: error returned from macro: " << errorstr[error] << endl;
	}
      } else {
	cerr << "ERROR: macro id " << wp->macro_ids[i];
	cerr << " never defined in layout" << endl;
      }
    }
    /***************************************************
     * Check for existence of a legend, create it
     ***************************************************/
    bool drawlegend = false;

    if (wp->legid.size()) {
      map<string,wLegend_t *>::const_iterator it=glmap_id2legend.find(wp->legid);
      if (it != glmap_id2legend.end()) {
	drawlegend = true;
	wleg = it->second;
      } else {
	cerr << "ERROR: legend id " << wp->legid;
	cerr << " never defined in layout" << endl;
      }
    } else {
      // Maybe gPad already *has* a legend from macros...
      TPave *testing = (TPave *)gPad->GetPrimitive("TPave");
      if (testing &&
	  !strcmp(testing->IsA()->GetName(),"TLegend")) {
	TLegend *pullTheOtherOne = (TLegend *)testing;
	if (gl_verbose) cout << "Found legend from macro" << endl;
	wleg = new wLegend_t();
	wleg->leg = pullTheOtherOne;
	drawlegend = true;
      }
    }

    /***************************************************
     * LOOP OVER STACKS DEFINED FOR PAD...
     ***************************************************/

    if (wp->stack_ids.size()) {
      wStack_t *ws=NULL;
      for (unsigned j = 0; j < wp->stack_ids.size(); j++) {
	string& sid = wp->stack_ids[j];
	map<string,wStack_t *>::const_iterator it = glmap_id2stack.find(sid);
	if (it == glmap_id2stack.end()) {
	  cerr << "ERROR: stack id " << sid << " never defined in layout" << endl;
	  exit (-1);
	}

	bool firstInPad = !j;

	ws = it->second;
	if (!ws) { cerr<< "find returned NULL stack pointer for " << sid << endl; continue; }

	// Add the histos in the stack to any legend that exists

	//
	if (drawlegend) {
	  for (size_t i=0; i<ws->v_histos.size(); i++) {
	    wTH1 *wh = ws->v_histos[i];
	    wh->ApplySavedStyle();
	    if(wh->GetLegendEntry().size())
	      wh->Add2Legend(wleg->leg);
	  }
	}

	string drawopt("");
	if (ws->sum->GetDrawOption().size()) {
	  drawopt = ws->sum->GetDrawOption();
	  cout << "drawopt stored with histo = " << drawopt << endl;
	}

	drawInPad(wp, ws, firstInPad, drawopt);

	wp->vp->Update();
      }
    } // stack loop

    /***************************************************
     * LOOP OVER HISTOS DEFINED FOR PAD...
     ***************************************************/

    for (unsigned j = 0; j < wp->histo_ids.size(); j++) {
      string& hid = wp->histo_ids[j];
      map<string,wTH1 *>::const_iterator it = glmap_id2histo.find(hid);
      if (it == glmap_id2histo.end()) {
	cerr << "ERROR: histo id " << hid << " never defined in layout" << endl;
	exit (-1);
      }

      wTH1 *myHisto = it->second;
      
      if (myHisto) {
	bool firstInPad = !j && !wp->stack_ids.size();
	if (gl_verbose) {
	  cout << "Drawing " << hid << " => ";
	  cout << myHisto->histo()->GetName() << endl;
	  cout << "firstInPad = " << firstInPad << endl;
	}
	drawInPad(wp,*myHisto,firstInPad);

	myHisto->DrawFits("same");
	if (drawlegend && myHisto->GetLegendEntry().size()) {
	  if (wleg->drawoption.size()) myHisto->SetDrawOption(wleg->drawoption);
	  myHisto->Add2Legend(wleg->leg);
	}
	if (myHisto->statsAreOn()) {
	  myHisto->DrawStats();
	  wp->vp->Update();
	}

	myHisto->ApplySavedStyle();
	wp->vp->Update();
      }
    } // histos loop

    /***************************************************
     * LOOP OVER HISTOS DEFINED FOR ALTERNATE Y-AXIS
     ***************************************************/

    Float_t rightmax=0.0,rightmin=0.0;
    Float_t scale=0.0;
    for (unsigned j = 0; j < wp->altyh_ids.size(); j++) {
      string& hid = wp->altyh_ids[j];
      map<string,wTH1 *>::const_iterator it = glmap_id2histo.find(hid);
      if (it == glmap_id2histo.end()) {
	cerr << "ERROR: histo id " << hid << " never defined in layout" << endl;
	exit (-1);
      }

      wTH1 *myHisto = it->second;
      TH1 *h = myHisto->histo();

      if (!j) {
	//scale second set of histos to the pad coordinates
	rightmin = h->GetMinimum();
	rightmax = 1.1*h->GetMaximum();
	scale    = gPad->GetUymax()/rightmax;
      }
      TH1 *scaled=(TH1 *)h->Clone(Form("%s_%d",h->GetName(),ipad));

      scaled->Scale(scale);
      scaled->Draw("same");
   
      //draw an axis on the right side
      TGaxis *axis = new TGaxis(gPad->GetUxmax(), gPad->GetUymin(),
				gPad->GetUxmax(), gPad->GetUymax(),
				rightmin,rightmax,505,"+L");
      axis->Draw();
      gPad->Update();
      if (drawlegend && myHisto->GetLegendEntry().size()) {
	if (wleg->drawoption.size()) myHisto->SetDrawOption(wleg->drawoption);
	myHisto->Add2Legend(wleg->leg);
      }
    }

    /***************************************************
     * LOOP OVER GRAPHS DEFINED FOR PAD...
     ***************************************************/
#if 0
    TMultiGraph *mg;
    if (graph_ids.size())
      mg = new TMultiGraph();
#endif
    for( unsigned j = 0; j < wp->graph_ids.size(); j++ ) {
      string& gid = wp->graph_ids[j];
      
      wGraph_t *wg   = findGraph(gid);

      bool firstInPad = !j && !wp->histo_ids.size() && !wp->macro_ids.size();
      if( firstInPad && wg->gr && wg->gr->IsA()==TGraph::Class() )
	wg->drawopt += string("A"); // no histos drawn, need to draw the frame ourselves.

      if( wg && wg->gr ) {
	// "pre-draw" in order to define the plot elements
	wg->gr->Draw(wg->drawopt.c_str());

	if (firstInPad) {
	  // Now we can set the axis attributes and range:
	  wg->gr->GetXaxis()->ImportAttributes(wg->xax);
	  wg->gr->GetYaxis()->ImportAttributes(wg->yax);

	  cout << wg->xax->GetXmin() << " " << wg->xax->GetXmax() << endl;
	  if( wg->xax->GetXmax()>wg->xax->GetXmin() )
	    wg->gr->GetXaxis()->SetLimits(wg->xax->GetXmin(),wg->xax->GetXmax());
	  if( wg->yax->GetXmax()>wg->yax->GetXmin() )
	    wg->gr->GetYaxis()->SetRangeUser(wg->yax->GetXmin(),wg->yax->GetXmax());
	}
	// draw for good
	drawInPad<TGraph>(wp,wg->gr,wg->drawopt.c_str(),firstInPad);

	wp->vp->Update();
	if( wg->fitfn ) 
	  wg->gr->Fit(wg->fitfn);
	if( drawlegend && wg->leglabel.size() )
	  wleg->leg->AddEntry(wg->gr,wg->leglabel.c_str(),wg->legdrawopt.c_str());
      }
      if( wg && wg->gr2d ) {
	drawInPad<TGraph2D>(wp,wg->gr2d,wg->drawopt.c_str(),firstInPad);

	if (firstInPad) {
	  // Now we can set the axis attributes and range:
	  wg->gr2d->GetXaxis()->ImportAttributes(wg->xax);
	  wg->gr2d->GetYaxis()->ImportAttributes(wg->yax);

	  cout << wg->xax->GetXmin() << " " << wg->xax->GetXmax() << endl;
	  if( wg->xax->GetXmax()>wg->xax->GetXmin() )
	    wg->gr2d->GetXaxis()->SetLimits(wg->xax->GetXmin(),wg->xax->GetXmax());
	  if( wg->yax->GetXmax()>wg->yax->GetXmin() )
	    wg->gr2d->GetYaxis()->SetRangeUser(wg->yax->GetXmin(),wg->yax->GetXmax());
	}

	if (wg->contours) {
	  //cout << "setting contours "; wg->contours->Print();
	  wg->gr2d->GetHistogram()->SetContour(wg->contours->GetNoElements(),
					       wg->contours->GetMatrixArray());
	  wg->gr2d->SetLineStyle   (wg->lstyle);
	  wg->gr2d->SetLineColor   (wg->lcolor);
	  wg->gr2d->SetLineWidth   (wg->lwidth);
	}
	wp->vp->Modified();
	wp->vp->Update();
	if( drawlegend && wg->leglabel.size() )
	  wleg->leg->AddEntry(wg->gr2d,wg->leglabel.c_str(),wg->legdrawopt.c_str());
      }
    } // graph loop

    /***************************************************
     * LOOP OVER LINES DEFINED FOR PAD...
     ***************************************************/

    for( unsigned j = 0; j < wp->line_ids.size(); j++ ) {
      string drawopt("L");
      string& lid = wp->line_ids[j];
      map<string,TLine *>::const_iterator it2 = glmap_id2line.find(lid);
      if (it2 == glmap_id2line.end()) {
	cerr << "ERROR: line id " << lid << " never defined in layout" << endl;
	exit (-1);
      }

      TLine *line = it2->second;

      if (!j && !wp->histo_ids.size() && !wp->macro_ids.size())
	drawopt += string("A"); // no histos drawn, need to draw the frame ourselves.

      if (line) {
	drawInPad<TLine>(wp,line,drawopt.c_str());
	//if (drawlegend)
	//wleg->leg->AddEntry(line,lid.c_str(),"L");
      }
    }

    /***************************************************
     * LOOP OVER BOXES DEFINED FOR PAD...
     ***************************************************/

    for (unsigned j = 0; j < wp->box_ids.size(); j++) {
      string drawopt("L");
      string& bid = wp->box_ids[j];
      map<string,TBox *>::const_iterator it2 = glmap_id2box.find(bid);
      if (it2 == glmap_id2box.end()) {
	cerr << "ERROR: box id " << bid << " never defined in layout" << endl;
	exit (-1);
      }

      TBox *box = it2->second;

      if (box) {
	drawInPad<TBox>(wp,box,drawopt.c_str());
      }
    }

    /***************************************************
     * Draw the legend
     ***************************************************/

    if (drawlegend) {
      wleg->leg->Draw("same");
      wp->vp->Update();
    }

    /***************************************************
     * Draw each latex/label object
     ***************************************************/
    
    for (unsigned j=0; j<wp->latex_ids.size(); j++) {
      string& lid = wp->latex_ids[j];
      map<string,TLatex *>::const_iterator it2 = glmap_id2latex.find(lid);
      if (it2 == glmap_id2latex.end()) {
	cerr << "ERROR: latex id " << lid << " never defined in layout" << endl;
	exit (-1);
      }
      if (gl_verbose) cout << "Drawing latex object " << lid << endl;
      TLatex *ltx = it2->second;
      ltx->Draw();
      wp->vp->Update();
    }

    for (unsigned j = 0; j < wp->label_ids.size(); j++) {
      string& lid = wp->label_ids[j];
      map<string,wLabel_t *>::const_iterator it2 = glmap_id2label.find(lid);
      if (it2 == glmap_id2label.end()) {
	cerr << "ERROR: label id " << lid << " never defined in layout" << endl;
	exit (-1);
      }
      if (gl_verbose) cout << "Drawing label object " << lid << endl;
      wLabel_t *wlab = it2->second;
      drawStandardText(wlab->text, wlab->x1ndc, wlab->y1ndc,-1,-1,wlab->textsize);

      wp->vp->Update();
    }
    wc->c1->Update();

  } // pad loop

  //prdFixOverlay();

  if (savePlots2file) {
    wc0 = cs.canvases[0];
    if (!wc0->savenamefmts.size())  // define a default
      wc0->savenamefmts.push_back("%F_%C.png");
    for (size_t i=0; i<cs.canvases.size(); i++) {
      wCanvas_t *wc = cs.canvases[i];
      wc->c1->cd();
      for (size_t j=0; j<wc0->savenamefmts.size(); j++)
	saveCanvas2File(wc,wc0->savenamefmts[j]);
    }
  }
}                                                           // drawPlots

//======================================================================
