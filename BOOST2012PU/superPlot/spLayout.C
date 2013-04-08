
//======================================================================

bool                                          // returns true if success
processLayoutSection(FILE         *fp,
		     string&       theline,
		     canvasSet_t&  cs,
		     bool&         new_section)
{
  if (gl_verbose)
    cout << "Processing layout section" << endl;

  new_section=false;

  wCanvas_t *wc0 = cs.canvases[0];

  while (getLine(fp,theline,"layout")) {
    if (!theline.size()) continue;
    if (theline[0] == '#') continue; // comments are welcome

    if (theline[0] == '[') {
      new_section=true;
      return true;
    }

    string key, value;
    if (!getKeyValue(theline,key,value)) continue;

    if (key == "ncanvases") {
      unsigned long ncanvases = str2int(value);
      if (ncanvases > 0) cs.ncanvases = ncanvases;
      else if (gl_verbose)
	cout << "ncanvases="<<ncanvases<<"?? You can't be serious." << endl;
    }
    else if (key == "npadsx") {
      unsigned long npadsx = str2int(value);
      if (npadsx > 0) wc0->npadsx = npadsx;
      else if (gl_verbose)
	cout << "npadsx="<<npadsx<<"?? You can't be serious." << endl;
    }
    else if (key == "npadsy") {
      unsigned long npadsy = str2int(value);
      if (npadsy > 0) wc0->npadsy = npadsy;
      else if (gl_verbose)
	cout << "npadsy="<<npadsy<<"?? You can't be serious." << endl;
    }
    else if (key == "padxdim") {
      unsigned long padxdim = str2int(value);
      if (padxdim > 0) wc0->padxdim = padxdim;
    }
    else if (key == "padydim") {
      unsigned long padydim = str2int(value);
      if (padydim > 0) wc0->padydim = padydim;
    }
    else if (key == "padxmargin") {
      float padxmargin = str2flt(value);
      if (padxmargin > 0.) wc0->padxmargin = padxmargin;
    }
    else if (key == "padymargin") {
      float padymargin = str2flt(value);
      if (padymargin > 0.) wc0->padymargin = padymargin;
    }
    else if (key == "leftmargin") {
      float leftmargin = str2flt(value);
      if (leftmargin > 0.) wc0->leftmargin = leftmargin;
    }
    else if (key == "rightmargin") {
      float rightmargin = str2flt(value);
      if (rightmargin > 0.) wc0->rightmargin = rightmargin;
    }
    else if (key == "topmargin") {
      float topmargin = str2flt(value);
      if (topmargin > 0.) wc0->topmargin = topmargin;
    }
    else if (key == "bottommargin") {
      float bottommargin = str2flt(value);
      if (bottommargin > 0.) wc0->bottommargin = bottommargin;
    }
    else if (key == "latex") {
      Tokenize(value,wc0->latex_ids," ,"); 
      if (!wc0->latex_ids.size())  wc0->latex_ids.push_back(value);
    }
    else if (key == "fillcolor")
      wc0->fillcolor = str2int(value);
    else if (key == "savenamefmts") {
      vector<string> v_tokens;
      Tokenize(value,v_tokens,",");
      for (size_t i=0; i<v_tokens.size(); i++) {
	value = v_tokens[i];
	if (value.find('@') != string::npos) {
	  string temp=value;
	  expandAliii(temp,value);
	  if (!value.size()) continue;
	}
	wc0->savenamefmts.push_back(value);
      }
    }
    else {
      cerr << "Unknown key " << key << endl;
    }

  } // while loop

  return true;
}                                                // processLayoutSection

//======================================================================
