#include "TVectorD.h"

#ifndef LINELEN
#define LINELEN 512
#endif

static map<string, pair<double,double> >  glmap_id2sample;

//======================================================================

double
getSampleScaleFactor(const string& sid,double luminvpb)
{
  map<string,pair<double,double> >::const_iterator it = glmap_id2sample.find(sid);
  if (it != glmap_id2sample.end()) {
    double nev  = it->second.first;
    double xsec = it->second.second;
    if (gl_verbose) cout << nev << "\t" << xsec << "\t" << luminvpb << endl;
    return(xsec*luminvpb/nev);
  } else {
    cerr << "Sample " << sid << " not found, ";
    cerr << "must be defined in [SAMPLE] section first" << endl;
  }
  return 0;
}                                                // getSampleScaleFactor

//======================================================================

bool                              // returns true if success
processSampleSection(FILE *fp,
		     string& theline,
		     bool& new_section)
{
  string *sid = NULL;
  double xsecpb=0.0, filteff=1.0,br=1.0;
  double nevents=0;

  if (gl_verbose)
    cout << "Processing sample section" << endl;

  new_section=false;

  while (getLine(fp,theline,"sample")) {
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
      if (sid != NULL) {
	cerr << "no more than one id per sample section allowed " << value << endl;
	break;
      }
      sid = new string(value);

    //------------------------------
    } else if (key == "xsecpb") {
    //------------------------------
      if (!sid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      xsecpb=(double)str2flt(value);

    //------------------------------
    } else if (key == "br") {
    //------------------------------
      if (!sid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      br=(double)str2flt(value);

    //------------------------------
    } else if (key == "nevents") {
    //------------------------------
      if (!sid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      nevents=str2flt(value);

    //------------------------------
    } else if (key == "filteff") {
    //------------------------------
      if (!sid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      filteff=str2flt(value);

    } else {
      cerr << "unknown key " << key << endl;
    }
  }

  xsecpb *= br;
  if (filteff>0.0) xsecpb /= filteff;

  if ((nevents>0)&&
      (xsecpb>0.)) {
    pair<double,double> sample(nevents,xsecpb);
    glmap_id2sample.insert(pair<string, pair<double,double> >(*sid,sample));
    return true;
  }
  return false;
}                                                // processSampleSection

//======================================================================
