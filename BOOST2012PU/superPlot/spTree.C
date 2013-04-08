#include "TTree.h"
#include "TPRegexp.h"
#include "TGraph.h"

static map<string, TTree *>       glmap_id2tree;

//======================================================================

TTree *findTree(const string& tid)
{
  map<string,TTree *>::const_iterator it = glmap_id2tree.find(tid);
  if (it == glmap_id2tree.end()) {
    cerr << "Tree ID " << tid << " not found" << endl;
    return NULL;
  }
  if( gl_verbose)
    cout << "Found tree " << tid << endl;
  return it->second;
}                                                            // findTree

//======================================================================

void fillHistoFromTreeVar(std::string& treedrawspec,
			  int  index,
			  wTH1 *&wth1)
{
  // Sample treedrawspec:
  // mytree:"TDCwinstart[%d]:runnum>>winstrt%d(70,202000,209000)","evtnum==1","prof P"
  //
  vector<string> v_tokens;
  string tid;
  TString drawspec;
  Tokenize(treedrawspec,v_tokens,":",true);
  if( (v_tokens.size() < 2) ||
      (!v_tokens[0].size())  ||
      (!v_tokens[2].size())    ) {
    cerr << "malformed root tree draw spec treeid:\"varexp\",\"selection\",option: " << treedrawspec << endl;
    return;
  }

  tid = v_tokens[0];
  for (size_t i=2; i<v_tokens.size(); i++) {
    drawspec += v_tokens[i];
  }
  int fmtcnt = drawspec.CountChar('%');

  if (fmtcnt) { // use index for tree array var
    switch(fmtcnt) {
    case 1: drawspec = Form(drawspec,index); break;
    case 2: drawspec = Form(drawspec,index,index); break;
    case 3: drawspec = Form(drawspec,index,index,index); break;
    case 4: drawspec = Form(drawspec,index,index,index,index); break;
    case 5: drawspec = Form(drawspec,index,index,index,index,index); break;
    case 6: drawspec = Form(drawspec,index,index,index,index,index,index); break;
    default:
      cerr << "More than six fmt specifiers in drawspec found, fix me! " << drawspec <<endl;
      exit(-1);
    }
  }
  if( gl_verbose)
    cout<<"drawspec="<<drawspec<<endl;

  TTree *tree = findTree(tid);
  assert (tree);

  // can't use comma as delimiter since histo with binning spec may be supplied
  TObjArray *tokens = drawspec.Tokenize("\"");
  TString hname;

  TString varexp = ((TObjString *)(*tokens)[0])->GetString();

  if (varexp.Contains(">>")) {
    TObjArray *rematches = TPRegexp(">>(\\w+)").MatchS(varexp); // get histo name
    assert(rematches->GetEntriesFast() ==2);
    hname = ((TObjString *)(*rematches)[1])->GetString();

    if (wth1 && !hname.EqualTo(wth1->histo()->GetName())) {
      cerr << "Error: histo name in treedraw spec "<<hname;
      cerr <<" doesn't match named histo "<<wth1->histo()->GetName()<<endl;
      exit(-1);
    }
  } else { // add histo name
    assert (wth1);
    hname = TString(wth1->histo()->GetName());
    varexp = varexp + ">>+" + hname; // append to pre-existing histo
  }

  if( gl_verbose)
    cout<<"varexp="<<varexp<<", hname="<<hname<<endl;
  switch(tokens->GetEntriesFast()) {
  case 1:
    tree->Draw(varexp,"","goff");
    break;
  case 3:
    {
      TString cut = ((TObjString *)(*tokens)[2])->GetString();
      tree->Draw(varexp,cut,"goff"); 
    }
    break;
  case 4: // assume the cut string is blank
    {
      TString gopt = ((TObjString *)(*tokens)[3])->GetString();
      gopt = gopt + " goff";
      tree->Draw(varexp,"",gopt);
    }
    break;
  case 5:
    {
      TString cut  = ((TObjString *)(*tokens)[2])->GetString();
      TString gopt = ((TObjString *)(*tokens)[4])->GetString();
      gopt = gopt + " goff";
      tree->Draw(varexp,cut,gopt);
    }
    break;
  default:
    cerr << "malformed root tree draw spec treeid:varexp,selection,option";
    for (int i=0; i<tokens->GetEntriesFast(); i++)
      cerr << i<<": "<< ((TObjString *)(*tokens)[i])->GetString() << " ";
    cerr << endl;
    break;
  }
  if (!wth1) {
    wth1 = new wTH1((TH1*)gDirectory->Get(hname));
    assert(wth1);
    wth1->histo()->UseCurrentStyle();
  }
}                                                // fillHistoFromTreeVar

//======================================================================

void fillGraphFromTreeVar(std::string& treedrawspec,int index,wGraph_t *&pwg)
{
  // Sample treedrawspec:
  // mytree:"TDCwinstart[%d]:runnum","evtnum==1","P"
  //
  vector<string> v_tokens;
  string tid;
  TString drawspec;
  Tokenize(treedrawspec,v_tokens,":",true);
  if( (v_tokens.size() < 2) ||
      (!v_tokens[0].size())  ||
      (!v_tokens[2].size())    ) {
    cerr << "malformed root tree draw spec treeid:\"varexp\",\"selection\",option: " << treedrawspec << endl;
    return;
  }

  tid = v_tokens[0];
  for (size_t i=2; i<v_tokens.size(); i++) {
    drawspec += v_tokens[i];
  }
  int fmtcnt = drawspec.CountChar('%');

  if (fmtcnt) { // use index for tree array var
    switch(fmtcnt) {
    case 1: drawspec = Form(drawspec,index); break;
    case 2: drawspec = Form(drawspec,index,index); break;
    case 3: drawspec = Form(drawspec,index,index,index); break;
    case 4: drawspec = Form(drawspec,index,index,index,index); break;
    case 5: drawspec = Form(drawspec,index,index,index,index,index); break;
    case 6: drawspec = Form(drawspec,index,index,index,index,index,index); break;
    default:
      cerr << "More than six fmt specifiers in drawspec found, fix me! " << drawspec <<endl;
      exit(-1);
    }
  }
  if( gl_verbose)
    cout<<"drawspec="<<drawspec<<endl;

  TTree *tree = findTree(tid);
  assert (tree);

  // can't use comma as delimiter since histo with binning spec may be supplied
  TObjArray *tokens = drawspec.Tokenize("\"");
  TString hname;

  TString varexp = ((TObjString *)(*tokens)[0])->GetString();

  if( gl_verbose)
    cout<<"varexp="<<varexp<<endl;
  switch(tokens->GetEntriesFast()) {
  case 1:
    tree->Draw(varexp,"","goff");
    break;
  case 3:
    {
      TString cut = ((TObjString *)(*tokens)[2])->GetString();
      tree->Draw(varexp,cut,"goff"); 
    }
    break;
  case 4: // assume the cut string is blank
    {
      TString gopt = ((TObjString *)(*tokens)[3])->GetString();
      gopt = gopt + " goff";
      tree->Draw(varexp,"",gopt);
    }
    break;
  case 5:
    {
      TString cut  = ((TObjString *)(*tokens)[2])->GetString();
      TString gopt = ((TObjString *)(*tokens)[4])->GetString();
      gopt = gopt + " goff";
      tree->Draw(varexp,cut,gopt);
    }
    break;
  default:
    cerr << "malformed root tree draw spec treeid:varexp,selection,option";
    for (int i=0; i<tokens->GetEntriesFast(); i++)
      cerr << i<<": "<< ((TObjString *)(*tokens)[i])->GetString() << " ";
    cerr << endl;
    break;
  }

  assert(tree->GetSelectedRows());

  if (!pwg)
    pwg = new wGraph_t();

  assert(pwg);
  pwg->gr = new TGraph(tree->GetSelectedRows(),
		       tree->GetV2(), tree->GetV1());

}                                                // fillGraphFromTreeVar

//======================================================================

TTree *getTreeFromSpec(const string& tid,
		       const string& spec)
{
  TTree  *tree    = NULL;
  TFile *rootfile = NULL;
  vector<string> v_tokens;
  string fullspec;

  if( gl_verbose)
    cout << "processing " << spec << endl;

  string tspec;
  string rootfn;

  fullspec = spec;

  Tokenize(fullspec,v_tokens,":");
  int ntok = (int)v_tokens.size();
  if( ((ntok != 2) &&
       (ntok != 3)   ) ||       //ntok==3 means file specifier is a URL
      (!v_tokens[0].size())  ||
      (!v_tokens[1].size())    ) {
    cerr << "malformed root tree path file:folder/subfolder/.../histo " << fullspec << endl;
    return NULL;
  } else {
    rootfn = (ntok==2) ? v_tokens[0] : v_tokens[0]+":"+v_tokens[1];
    tspec  = (ntok==2) ? v_tokens[1] : v_tokens[2];
  }

#if 0
  map<string,string>::const_iterator it = glmap_objpath2id.find(fullspec);
  if( it != glmap_objpath2id.end() ) {
    // Allow the possibility to run the script a second time in root
    if( gl_verbose)
      cout << "Object " << fullspec << " already read in, here it is" << endl;
    map<string,tree *>::const_iterator hit = glmap_id2histo.find(it->second);
    if( hit == glmap_id2histo.end() ) {
      if( gl_verbose)
	cout << "oops, sorry, I lied." << endl;
      return NULL;
    }
    tree = hit->second;
  } else {
#endif
    rootfile = openRootFile(rootfn);
    if (rootfile) {
      tree = (TTree *)rootfile->Get(tspec.c_str());
      if( !tree) {
	cerr << "couldn't find " << tspec << " in " << rootfn << endl;
      } else {
	// success, record that you read it in.
	if( gl_verbose) cout << "Found " << fullspec << endl;
	glmap_objpath2id.insert(pair<string,string>(fullspec,tid));
	glmap_id2objpath.insert(pair<string,string>(tid,fullspec));
	glmap_id2tree.insert(pair<string,TTree *>(tid,tree));
      }
    }
    //}
  return tree;
}                                                     // getTreeFromSpec

//======================================================================

bool                              // returns true if success
processTreeSection(FILE *fp,
		   string& theline,
		   bool& new_section)
{
  string *tid  = NULL;
  TTree  *t1   = NULL;

  if (gl_verbose)
    cout << "Processing Tree section" << endl;

  new_section=false;

  while (getLine(fp,theline,"Tree")) {
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
      if (tid != NULL) {
	cerr << "no more than one id per F1 section allowed " << value << endl;
	break;
      }
      tid = new string(value);
      
      map<string, TTree *>::const_iterator tit = glmap_id2tree.find(*tid);
      if (tit != glmap_id2tree.end()) {
	cerr << "Tree id " << *tid << " already defined" << endl;
	break;
      }
    //------------------------------
    } else if( key == "path" ) {
    //------------------------------
      if( !tid ) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      if( t1 ) {
	cerr << "histo already defined" << endl; continue;
      }
      t1 = getTreeFromSpec(*tid,value);
      if( !t1 ) continue;
    }
    else {
      cerr << "unknown key " << key << endl;
    }
  }

  if (tid) delete tid;
  return (t1 != NULL);
}                                                  // processTreesection

//======================================================================
