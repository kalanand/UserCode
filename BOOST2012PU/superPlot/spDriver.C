#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include "TSystem.h"
#include "superPlot.C"

using namespace std;

typedef map<string, vector<string> > varTable_t;

//======================================================================

void loadMultiAliii(const string& tableFilename,
		    varTable_t& m_table,
		    size_t& nrows,
		    bool verbose=true)
{
  FILE *fpin = fopen(tableFilename.c_str(),"r");
  if (!fpin) {
    cerr<<"Error, couldn't open alias include file '"<<tableFilename<<"'"<<endl;
    exit(-1);
  }
  if (verbose) cout << "Loading include table '" << tableFilename << "'" << endl;

  string theline;

  // expect the first line to define the var names as column headers

  vector<string> parnames;
  vector<string> v_tokens;

  if (!getLine(fpin,theline,"alias")) {
    cerr << "ERROR: include table file empty" << endl;
    return;
  } else {
    Tokenize(theline, parnames,"\t");
    if (!parnames.size()) {
      //===================================================================
      cerr << "ERROR: first line of include table ";
      cerr << "must contain var names as column headers" << endl;
      //===================================================================
      return;
    }
    vector<string> empty;
    size_t ncols = parnames.size();
    if( verbose ) cout << "Loading values for variables ";
    for (size_t i=0; i<ncols; i++) {
      string parspec = parnames[i];
      Tokenize(parspec,v_tokens,".");
      if (v_tokens.size() != 2) {
	//===================================================================
	cerr << "\n\nERROR: Column header must be of the form SECTION.var, ";
	//===================================================================
	cerr << parspec << endl;
	exit(-1);
      }
      pair<map<string,vector<string> >::iterator, bool> returnval;
      returnval = m_table.insert(pair<string,vector<string> >(parnames[i],empty));
      if (!returnval.second) {
	//===================================================================
	cerr << "\n\n!! Cannot multiply define two variables of the same name in alias table!";
	//===================================================================
	cerr << endl;
	exit(-1);
      }
      if ( verbose ) cout << parnames[i] << " ";
    }
    if( verbose ) cout << endl;
  }

  while (getLine(fpin,theline,"alias")) {
    if (!theline.size())   continue;
    if (theline[0] == '#') continue; // comments are welcome
    if (theline[0] == '^') {
      cout << theline.substr(1) << endl;
      continue;
    }

    Tokenize(theline, v_tokens,"\t");
    if (v_tokens.size() != parnames.size()) {
      cerr << "Rejecting line in vartable " << theline << endl;
    } else {
      for (size_t i=0; i<parnames.size(); i++) {
	m_table[parnames[i]].push_back(v_tokens[i]);
      }
    }
  }  
  nrows = m_table.begin()->second.size();
  fclose(fpin);
}                                                      // loadMultiAliii

//======================================================================

void writeTmpConfig(const string& canvasLayout,
		    const string& tmpname,
		    const varTable_t& m_partable,
		    string& postfix,
		    int   irow)
{
  FILE *fpout = fopen(tmpname.c_str(),"w");
  if (!fpout) {
    cerr << "FATAL ERROR: could not open "<<tmpname<<endl;
    exit(-1);
  }

  string cursection("");
  varTable_t::const_iterator it;
  for (it  = m_partable.begin();
       it != m_partable.end();
       it++) {
    string parspec = it->first;
    const vector<string>& values = it->second;

    if (parspec == "DRIVER.prefix")
      cout << values[irow];
    else if (parspec == "DRIVER.postfix") {
      postfix = values[irow];
    } else {
      vector<string> subtokens;
      Tokenize(parspec,subtokens,".");
      if (subtokens.size() != 2) {
	cerr << "\n\nERROR: Column header must be of the form SECTION.var, ";
	cerr << parspec << endl;
	exit(-1);
      }
    
      string section = subtokens[0];
      string var     = subtokens[1];

      if (section != cursection) {
	// variables belonging to the same section had better be in
	// consecutive columns and ordered according to the needs of superPlot...
	//
	fprintf(fpout,"[%s]\n", section.c_str());
	cursection = section;
      }
      fprintf(fpout,"%s=%s\n",var.c_str(), values[irow].c_str());
    }
  }
  fclose(fpout);

  const string catstring = "cat "+canvasLayout+">>"+tmpname;
  gSystem->Exec(catstring.c_str());
}

//======================================================================

void spDriver(const string& canvasLayout="canvas.txt",
	      const string& tableFilename="vartable.txt",
	      bool saveplots=false,
	      bool verbose=true)
{
  varTable_t m_partable;   // multiple values per parameter

  size_t nrows;
  loadMultiAliii(tableFilename,m_partable, nrows,verbose);

  // Everything's loaded, now run through each row of the table
  // as a separate superPlot job
  //
  for (size_t irow=0; irow<nrows; irow++) {
    if (verbose)
      cout<<"================================================================="<<endl;
    string tmpname = "/tmp/"+stripDirsAndSuffix(canvasLayout)+"_"+int2str(irow)+".txt";
    string postfix;
    writeTmpConfig(canvasLayout,tmpname,m_partable,postfix,irow);
    superPlot(tmpname,saveplots,verbose);
    if (postfix.size())
      printf(postfix.c_str());
    unlink(tmpname.c_str());
  }
}
