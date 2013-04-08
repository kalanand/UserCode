
//======================================================================

int
processMacroSection(FILE *fp,
		    string& theline,
		    bool& new_section)
{
  vector<string> v_tokens;
  string *mid = NULL;
  int success = 0;

  cout << "Processing macro section" << endl;

  new_section=false;

  while (getLine(fp,theline,"macro")) {
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
      if (mid != NULL) {
	cerr << "no more than one id per graph section allowed " << value << endl;
	break;
      }
      mid = new string(value);

    //------------------------------
    } else if (key == "path") {
    //------------------------------

      if (!mid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }

      // We're actually using this map in reverse here!
      glmap_objpath2id.insert(pair<string,string>(*mid,value));

      success = 1;

    } else {
      cerr << "unknown key " << key << endl;
    }
  }
  return success;
}                                                 // processGraphSection

//======================================================================
