static map<string, TBox *>     glmap_id2box;

//======================================================================

bool                              // returns true if success
processBoxSection(FILE *fp,
		   string& theline, // the text line, that is
		   bool& new_section)
{
  vector<string> v_tokens;

  string *lid = NULL;
  TBox *box = NULL;

  cout << "Processing box section" << endl;

  new_section=false;

  while (getLine(fp,theline,"box")) {
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
	cerr << "no more than one id per box section allowed " << value << endl;
	break;
      }

      lid = new string(value);

    //------------------------------
    } else if (key == "x1x2y1y2") {
    //------------------------------
      if (!lid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }

      if (box) {
	cerr << "box already defined" << endl; continue;
      }

      Tokenize(value,v_tokens,",");

      if (v_tokens.size() != 4) {
	cerr << "expecting four coordinates x1,x2,y1,y2: " << theline << endl; continue;
      }

      float x1=str2flt(v_tokens[0]);
      float x2=str2flt(v_tokens[1]);
      float y1=str2flt(v_tokens[2]);
      float y2=str2flt(v_tokens[3]);

      box = new TBox(x1,y1,x2,y2);

    } else if (!box) {
      cerr << "key x1x2y1y2 must appear before this key: " << key << endl; continue;
    } else {
      if      (key == "linecolor")   box->SetLineColor(str2int(value));
      else if (key == "linestyle")   box->SetLineStyle(str2int(value));
      else if (key == "linewidth")   box->SetLineWidth(str2int(value));
      if      (key == "fillcolor")   box->SetFillColor(str2int(value));
      if      (key == "fillstyle")   box->SetFillStyle(str2int(value));
      else {
	cerr << "unknown key " << key << endl;
      }
    }
  }

  if (box)
    glmap_id2box.insert(pair<string,TBox *>(*lid,box));

  return (box != NULL);
}                                                   // processBoxSection

//======================================================================
