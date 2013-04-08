static map<string, string>          glmap_aliii;        // better than aliases

//======================================================================

const string& extractAlias(const string& input)
{
  map<string,string>::const_iterator it;
  it = glmap_aliii.find(input);
  if (it == glmap_aliii.end()) {
    cerr<<"alias "<<input<<" not found, define reference in ALIAS section first."<<endl;
    return nullstr;
  }
  return it->second;
}                                                        // extractAlias

//======================================================================

void expandAliii(const string& input, 
		 string& output)
{
  const bool include_delimiters = true;
  const string ispunc("\",./<>?;:'[]{}\\|`~!#$%^&*()_-+=@ ");

  output.clear();
  vector<string> v_tokens;
  Tokenize(input,v_tokens, ispunc, include_delimiters);
  for (size_t i=0; i<v_tokens.size(); i++) {
    if (*v_tokens[i].rbegin() == '@' &&
	i<v_tokens.size()-1) {
      int len=v_tokens[i].length();
      if (len>1) // put the punctuation back! But not the '@'
	output += v_tokens[i].substr(0,--len);
      output += extractAlias(v_tokens[++i]);
    } else
      output += v_tokens[i];
  }
  //cout << "expanded "<<input<<"==>"<<output<<endl;
}                                                         // expandAliii

//======================================================================

bool                                        // returns true if success
processAliasSection(FILE *fp,string& theline, bool& new_section)
{
  vector<string> v_tokens;

  if (gl_verbose)
    cout << "Processing alias section" << endl;

  new_section=false;

  while (getLine(fp,theline,"alias")) {

    if (!theline.size())   continue;
    if (theline[0] == '#') continue; // comments are welcome

    if (theline[0] == '[') {
      new_section=true;
      return true;
    }

    string key, value;
    if (!getKeyValue(theline,key,value,false)) continue;

    //--------------------
    if (key=="includefile") { // reserved for putting aliii in a separate file
    //--------------------
      FILE *fp2 = fopen(value.c_str(),"r");
      if (!fp2) {
	cerr<<"Error, couldn't open alias include file '"<<value<<"'"<<endl;
	exit(-1);
      }
      if (gl_verbose)
	cout << "Loading include file '" << value << "'" << endl;
      processAliasSection(fp2,theline,new_section);
      fclose(fp2);
    } else {
      map<string,string>::const_iterator it;
      it = glmap_aliii.find(key);
      if (it != glmap_aliii.end()) {
	cerr << "Error in ALIAS section: key '" << key;
	cerr << "' already defined." << endl;
      } else {
	// Alias can include other aliii, but certain characters if
	// encountered are considered to bound the alias key, rather
	// than be a part of it, such as '/' and ':'
	string aspec = value;
#if 0
	if (aspec.find('@') != string::npos) {
	  string temp=aspec;
	  expandAliii(temp,aspec);
	  if (!aspec.size()) continue;
	}
#endif
	glmap_aliii.insert(pair<string,string>(key,aspec));
	if (gl_verbose)
	  cout << "alias '" << key << "' added" << endl;
      }
    }
  } // while loop

  return true;
}                                                 // processAliasSection

//======================================================================
