
//======================================================================

int
processLoopSection(FILE *fp,
		    string& theline,
		   bool& new_section,
		   int& start,
		   int& stop)
{
  vector<string> v_tokens;
  int success = 0;

  cout << "Processing loop section" << endl;

  new_section=false;

  while (getLine(fp,theline,"loop")) {
    if (!theline.size()) continue;
    if (theline[0] == '#') continue; // comments are welcome
    if (theline[0] == '[') {
      new_section=true;
      break;
    }

    string key, value;
    if (!getKeyValue(theline,key,value)) continue;

    //--------------------
    if (key == "index") {
    //--------------------
      Tokenize(value,v_tokens,"-");
      if (v_tokens.size()!=2) {
	cerr << "Invalid format for loop index range " << value << endl;
	exit(-1);
      }
      start = str2int(v_tokens[0]);
      stop  = str2int(v_tokens[1]);

      return 1;

    } else {
      cerr << "unknown key " << key << endl;
    }
  }
  return success;
}                                                 // processGraphSection

//======================================================================
