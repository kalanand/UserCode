#include "TKey.h"
#include "TRegexp.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TClass.h"

//======================================================================
// Regex match a histo name in a directory
//
void regexMatchHisto( TObject    *obj,
		      TDirectory *dir,
		      TObjArray  *Args,   // list of regexes to match
		      TObjArray  *Matches)
{
  for (int i=0; i<Args->GetEntriesFast(); i++) {
    TObjString *sre = (TObjString *)(*Args)[i];
    TRegexp re(sre->GetString(),kFALSE);
    if (re.Status() != TRegexp::kOK) {
      cerr << "The regexp " << sre->GetString() << " is invalid, Status() = ";
      cerr << re.Status() << endl;
      exit(-1);
    }

    TString path( (char*)strstr( dir->GetPath(), ":" ) );
    path.Remove( 0, 2 ); // gets rid of ":/"

    TString fullspec = TString(dir->GetPath()) + "/" + obj->GetName();

    if ((fullspec.Index(re) != kNPOS) &&
	(obj->InheritsFrom("TH1"))) {
      // we have a match
      // Check to see if it's already in memory
      map<string,string>::const_iterator it = glmap_objpath2id.find(dir->GetPath());
      if (it != glmap_objpath2id.end()) {
	if (gl_verbose)
	  cout << "Object " << fullspec << " already read in, here it is" << endl;
	map<string,wTH1 *>::const_iterator hit = glmap_id2histo.find(it->second);

	// Is this okay? It's going to get wrapped again...
	TObjString *rpath = new TObjString(fullspec);
	Matches->AddLast(rpath);
	Matches->AddLast(hit->second->histo());
      } else {
	// success, record that you read it in.
	TObjString *rpath = new TObjString(fullspec);
	Matches->AddLast(rpath);
	Matches->AddLast(obj);
      }
      break; // don't let the object match more than one regex
    } // if we have a match
  } // Arg loop
}                                                     // regexMatchHisto

//======================================================================

void recurseDirs( TDirectory *thisdir,
		  void (*doFunc)(TObject *, TDirectory *,TObjArray *, TObjArray *),
		  TObjArray *Args,
		  TObjArray *Output)
{
  assert(doFunc);

  //thisdir->cd();

  // loop over all keys in this directory

  TIter nextkey( thisdir->GetListOfKeys() );
  TKey *key;
  while ( (key = (TKey*)nextkey())) {

    TObject *obj = key->ReadObj();

    if ( obj->IsA()->InheritsFrom( "TDirectory" ) ) {
      // it's a subdirectory, recurse
      //cout << "Checking path: " << ((TDirectory *)obj)->GetPath() << endl;
      recurseDirs( (TDirectory *)obj, doFunc, Args, Output );
    } else {
      doFunc(obj, thisdir, Args, Output);
    }
  } // key loop
}                                                         // recurseDirs

//======================================================================

void getHistosFromRE(const string&   mhid,
		     const string&   filepath,
		     const string&   sre,
		     vector<std::pair<string,wTH1*> >&  v_wth1)
{
  if (gl_verbose)
    cout<<"Searching for regexp "<<sre<<" in "<<filepath;

  // allow for multiple regexes in OR combination
  //
  vector<string> v_regexes;
  Tokenize(sre,v_regexes,"|");
  if (!v_regexes.size())
    v_regexes.push_back(sre);

  // Build validated TRegexp arguments in preparation for directory recursion
  //
  TObjArray *Args = new TObjArray();
  for (size_t i=0; i<v_regexes.size(); i++) {
    TRegexp re(v_regexes[i].c_str(),kTRUE);
    if (re.Status() != TRegexp::kOK) {
      cerr << "The regexp " << v_regexes[i] << " is invalid, Status() = ";
      cerr << re.Status() << endl;
      exit(-1);
    }
    else {
      Args->AddLast(new TObjString(v_regexes[i].c_str()));
    }
  }

  // Get the root file
  //
  TFile *rootfile = openRootFile(filepath);

  if (!rootfile) {
    cerr << "File failed to open, " << filepath << endl;
    Args->Delete();
    delete Args;
    return;
  }

  // Do the recursion, collect matches
  //
  TObjArray *Matches = new TObjArray();
  recurseDirs(rootfile, &regexMatchHisto, Args, Matches);
  Args->Delete();
  delete Args;

  // Returns two objects per match: 
  // 1. the (string) path that was matched and
  // 2. the object whose path matched
  //
  int nx2matches = Matches->GetEntriesFast();
  if (gl_verbose) cout << "... " << nx2matches/2 << " match(es) found.";

  // Add the matches to the global map of histos
  //
  int istart = v_wth1.size();

  for (int i=0; i<nx2matches; i+=2) {
    TString fullspec = ((TObjString *)(*Matches)[i])->GetString();
    wTH1 *wth1 = new wTH1((TH1 *)((*Matches)[i+1]));
    wth1->histo()->UseCurrentStyle();
    string hidi= mhid+"_"+int2str(istart+(i/2));
    v_wth1.push_back(std::pair<string,wTH1 *>(hidi,wth1));

    //glmap_objpath2id.insert(pair<string,string>(fullspec,hidi));
    glmap_id2histo.insert(pair<string,wTH1 *>(hidi,wth1));
    glmap_id2objpath.insert(pair<string,string>(hidi,string(fullspec.Data())));
  }

  //Matches->Delete(); // need the histos!
  delete Matches;

  if (gl_verbose) cout << endl;
}                                                     // getHistosFromRE

//======================================================================

bool                              // returns true if success
processMultiHistSection(FILE *fp,
			string& theline,
			bool& new_section)
{
  vector<string> v_tokens;
  vector<std::pair<string, wTH1 *> > v_histos;
  string mhid;

  if (gl_verbose)
    cout << "Processing multihist section" << endl;

  new_section=false;

  while (getLine(fp,theline,"multihist")) {
    if (!theline.size()) continue;
    if (theline[0] == '#') continue; // comments are welcome

    if (theline[0] == '[') {
      new_section=true;
      return true;
    }

    string key, value;
    if (!getKeyValue(theline,key,value)) continue;

    //--------------------
    if (key == "id") {
    //--------------------
      if (mhid.size()) {
	cerr << "no more than one id per histo section allowed " << value << endl;
	break;
      }

      mhid = value;

    //------------------------------
    } else if (key == "pathglob") {
    //------------------------------
      glob_t globbuf;
      
      if (!mhid.size()) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }

      Tokenize(value,v_tokens,":");
      if ((v_tokens.size() != 2) ||
	  (!v_tokens[0].size())  ||
	  (!v_tokens[1].size())    ) {
	cerr << "malformed pathglob 'fileglob:regex' " << value << endl;
	exit(-1);
      }

      // File globbing pattern can select multiple files
      // regular expression pattern can select multiple histos within each file.
      //
      string fileglob = v_tokens[0];
      string stregex  = v_tokens[1];

      int stat = glob (fileglob.c_str(), GLOB_MARK, NULL, &globbuf);
      if (stat) {
	switch (stat) {
	case GLOB_NOMATCH: cerr << "No file matching glob pattern "; break;
	case GLOB_NOSPACE: cerr << "glob ran out of memory "; break;
	case GLOB_ABORTED: cerr << "glob read error "; break;
	default: cerr << "unknown glob error stat=" << stat << " "; break;
	}
	cerr << fileglob << endl;
	exit(-1);
      }
      if (gl_verbose) cout<<globbuf.gl_pathc<<" files match the glob pattern"<<endl;
      for (size_t i=0; i<globbuf.gl_pathc; i++) {
	char *path = globbuf.gl_pathv[i];
	if (!strncmp(&path[strlen(path)-6],".root",5)) {
	  cerr << "non-root file found in glob, skipping: " << path << endl;
	} else {
	  getHistosFromRE(mhid,string(path),stregex, v_histos);
	}
      }
      if (gl_verbose) cout << v_histos.size() << " total matches found." << endl;
      globfree(&globbuf);

      glmap_mhid2size.insert(pair<string,unsigned>(mhid,v_histos.size()));

    //------------------------------
    } else if( key == "fillfromtree" ) { // converts tree array variables into a group of histos
    //------------------------------
      if( !mhid.size() ) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }

      // histo need not be pre-booked, since TTree::Draw can do that

      Tokenize(value,v_tokens,";");
      string treedrawspec=v_tokens[0];

      int ifirst=-1,ilast=-1;
      if (v_tokens.size() == 2) {
	string range=v_tokens[1];
	Tokenize(range,v_tokens,"-");
	if (v_tokens.size()==2) {
	  ifirst=str2int(v_tokens[0]);
	  ilast =str2int(v_tokens[1]);
	}
      }
      //cout << v_tokens.size() << " " << ifirst << " " << ilast << endl;
      if (ifirst<0 || 
	  ilast <0 ||
	  ilast<ifirst ) {
	cerr << "malformed filltree expression drawspec;X-Y, where X-Y is the array index range to plot"<<endl;
	exit(-1);
      }

      for (int i=ifirst; i<=ilast; i++) {
	wTH1   *wth1 = NULL;
	// defined in spTree.C
	void fillHistoFromTreeVar(std::string& drawspec,int index,wTH1 *&wth1);
	fillHistoFromTreeVar(treedrawspec,i,wth1);
	assert(wth1);
	string hidi= mhid+"_"+int2str(i-ifirst);
	v_histos.push_back(std::pair<string,wTH1 *>(hidi,wth1));
	glmap_id2histo.insert(pair<string,wTH1 *>(hidi,wth1));
      }

      glmap_mhid2size.insert(pair<string,unsigned>(mhid,v_histos.size()));

    //-----------------------
    } else if( key == "printf" ) {
    //-----------------------

      Tokenize(value,v_tokens,"\",");
      switch( v_tokens.size() ) {
      case 1: printf (v_tokens[0].c_str()); break;
      case 2: printf (v_tokens[0].c_str(),v_tokens[1].c_str()); break;
      case 3: printf (v_tokens[0].c_str(),
		      v_tokens[1].c_str(),
		      v_tokens[2].c_str()); break;
      case 4: printf (v_tokens[0].c_str(),
		      v_tokens[1].c_str(),
		      v_tokens[2].c_str(),
		      v_tokens[3].c_str()); break;
      default:
	cerr << "Unsupported number of arguments, " << value << endl;
	exit(-1);
      }

    } else if( !v_histos.size() ) {  // all other keys must have "path" defined
      cerr << "histo vector is empty" << endl;
      cerr << "key 'pathglob' or 'vartable' must define nonempty histo set before key " << key << endl;
      break;
    }

    else {
      processCommonHistoParams(key,value,v_histos);
      if( key == "printfstats" )   cout << endl;
    }
  }

  return (v_histos.size());
}                                             // processMultiHistSection

//======================================================================
