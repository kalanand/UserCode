#include "TF1.h"
static map<string, TF1 *>       glmap_id2tf1;

//======================================================================

TF1 *findTF1(const string& fid)
{
  cout << "looking for " << fid << endl;
  map<string,TF1 *>::const_iterator it = glmap_id2tf1.find(fid);
  if (it == glmap_id2tf1.end()) {
    cerr << "TF1 ID " << fid << " not found" << endl;
    return NULL;
  }
  return it->second;
}                                                             // findTF1

//======================================================================

bool                              // returns true if success
processTF1Section(FILE *fp,
		  string& theline,
		  bool& new_section)
{
  string *fid  = NULL;
  string *form = NULL;
  TF1    *f1   = NULL;
  double xmin=0.0, xmax=0.0;
  int lcol=-1,lwid=-1,lsty=-1;
  vector<string> parstrs;

  cout << "Processing TF1 section" << endl;

  new_section=false;

  while (getLine(fp,theline,"TF1")) {
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
      if (fid != NULL) {
	cerr << "no more than one id per F1 section allowed " << value << endl;
	break;
      }
      fid = new string(value);
      
      map<string, TF1 *>::const_iterator fit = glmap_id2tf1.find(*fid);
      if (fit != glmap_id2tf1.end()) {
	cerr << "Function id " << *fid << " already defined" << endl;
	break;
      }

    //------------------------------
    } else if (key == "formula") {
    //------------------------------
      if (!fid) {
	cerr << "id key must be defined before formula key" << endl; continue;
      }
      if (form) {
	cerr << "Formula for " << *fid << " already defined" << endl; continue;
      }
      form = new string (value);

    //------------------------------
    }
    else if (key == "initpars") {
      if (!fid) {
	cerr << "id key must be defined before formula key" << endl; continue;
      }
      Tokenize(value,parstrs,",");
    }

    else if (key == "xmin")      xmin = str2flt(value);
    else if (key == "xmax")      xmax = str2flt(value);
    else if (key == "linecolor") lcol = str2int(value);
    else if (key == "linestyle") lsty = str2int(value);
    else if (key == "linewidth") lwid = str2int(value);

  }

  if (fid && form && (xmax > xmin)) {
    f1 = new TF1(fid->c_str(),form->c_str(),xmin,xmax);
    for (size_t i=0; i<parstrs.size(); i++) {
      f1->SetParameter(i,str2flt(parstrs[i]));
    }
    if (lsty>=0) f1->SetLineStyle(lsty);
    if (lcol>=0) f1->SetLineColor(lcol);
    if (lwid>=0) f1->SetLineWidth(lwid);

    glmap_id2tf1.insert(pair<string,TF1 *>(*fid,f1));
    //delete fid;
    //delete form;
  }

  if (fid) delete fid;
  return (f1 != NULL);
}                                                   // processTF1section

//======================================================================
