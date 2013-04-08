#include <sstream>

#ifndef LINELEN
#define LINELEN 512
#endif

#include "utils.C"

static unsigned gl_linect=0;

//======================================================================

bool getLine(FILE *fp, string& theline, const string& callerid="")
{ 
  char linein[LINELEN];

  if (!feof(fp) && fgets(linein,LINELEN,fp)) {
    if (strlen(linein) && (linein[strlen(linein)-1] == '\n'))
      linein[strlen(linein)-1] = '\0';

    if (gl_inloop) {
      TString tstrline(linein); 
      int nfmt = tstrline.CountChar('%');
      if (nfmt) {
	switch(nfmt){
	case 1: Form(tstrline,gl_loopindex); break;
	case 2: Form(tstrline,gl_loopindex,gl_loopindex); break;
	case 3: Form(tstrline,gl_loopindex,gl_loopindex,gl_loopindex); break;
	case 4: Form(tstrline,gl_loopindex,gl_loopindex,gl_loopindex,gl_loopindex); break;
	case 5: Form(tstrline,
		     gl_loopindex,gl_loopindex,
		     gl_loopindex,gl_loopindex,gl_loopindex); break;
	case 6: Form(tstrline,
		     gl_loopindex,gl_loopindex,gl_loopindex,
		     gl_loopindex,gl_loopindex,gl_loopindex); break;
	default:
	  cerr << "Too many % fmt specifiers, fix me!" << endl;
	  exit(-1);
	}
      }
      theline=string(tstrline.Data());
    } else
      theline = string(linein);
  } else return false;

  //cout << theline <<  ", callerid = " << callerid << endl;

  gl_linect++;

  return true;
}                                                             // getLine

//======================================================================

bool getKeyValue(const string& theline,
		 string& key,
		 string& value,
		 bool expandAliii=true)
{ 
  vector<string> v_tokens;
  Tokenize(theline,v_tokens,"=",true);

  if ((v_tokens.size() < 3) ||
      (!v_tokens[0].size()) ||
      (!v_tokens[2].size())    ) {
    cerr << "malformed key=value line " << theline << endl;
    return false;
  }

  key = v_tokens[0];

  // allow for '=' in the value, but not in the key!
  //
  for (unsigned i=2; i<v_tokens.size(); i++) {
    value+=v_tokens[i];
  }

  for( int i=0; 
       expandAliii && (value.find('@') != string::npos);
       i++) {
    string temp=value;
    extern void expandAliii(const string& input, 
			    string& output);
      
    expandAliii(temp,value);
    //cout << value << endl;
    if( !value.size()) return false;
    if( i>=10 ) {
      cerr << "Potential alias mutual self-reference cycle detected, please fix!" << endl;
      cerr << "line = " << theline << endl;
      exit(-1);
    }
  }

  return true;
}                                                         // getKeyValue
