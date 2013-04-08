static map<string, TLine *>     glmap_id2line;

//======================================================================

bool                              // returns true if success
processLineSection(FILE *fp,
		   string& theline, // the text line, that is
		   bool& new_section)
{
  vector<string> v_tokens;

  string *lid = NULL;
  TLine *line = NULL;

  cout << "Processing line section" << endl;

  new_section=false;

  while (getLine(fp,theline,"line")) {
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
      if (lid != NULL) {
	cerr << "no more than one id per line section allowed " << value << endl;
	break;
      }

      lid = new string(value);

    //------------------------------
    } else if (key == "x1x2y1y2") {
    //------------------------------
      if (!lid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }

      if (line) {
	cerr << "line already defined" << endl; continue;
      }

      Tokenize(value,v_tokens,",");

      if (v_tokens.size() != 4) {
	cerr << "expecting four coordinates x1,x2,y1,y2: " << theline << endl; continue;
      }

      float x1=str2flt(v_tokens[0]);
      float x2=str2flt(v_tokens[1]);
      float y1=str2flt(v_tokens[2]);
      float y2=str2flt(v_tokens[3]);

      line = new TLine(x1,y1,x2,y2);

    } else if (!line) {
      cerr << "key x1x2y1y2 must appear before this key: " << key << endl; continue;
    } else {
      if      (key == "linecolor")   line->SetLineColor(str2int(value));
      else if (key == "linestyle")   line->SetLineStyle(str2int(value));
      else if (key == "linewidth")   line->SetLineWidth(str2int(value));
      else {
	cerr << "unknown key " << key << endl;
      }
    }
  }

  if (line)
    glmap_id2line.insert(pair<string,TLine *>(*lid,line));

  return (line != NULL);
}                                                  // processLineSection

//======================================================================
