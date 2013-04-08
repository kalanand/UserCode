
//======================================================================

bool                                        // returns true if success
processPadSection(FILE *fp,string& theline, wPad_t * wpad, bool& new_section)
{
  vector<string> v_tokens;
  float xmin=1e99, xmax=-1e99;
  float ymin=1e99, ymax=-1e99;

  if (gl_verbose)
    cout << "Processing pad section" << endl;

  new_section=false;

  while (getLine(fp,theline,"pad")) {

    if (!theline.size()) continue;
    if (theline[0] == '#') continue; // comments are welcome

    if (theline[0] == '[') {
      new_section=true;
      return true;
    }

    string key, value;
    if (!getKeyValue(theline,key,value)) continue;

    if      (key == "histos") {
      Tokenize(value,wpad->histo_ids," ,"); 
      if (!wpad->histo_ids.size()) wpad->histo_ids.push_back(value);
    }
    else if (key == "stackem") {
      Tokenize(value,wpad->stack_ids," ,");
      if (!wpad->stack_ids.size()) wpad->stack_ids.push_back(value);
    }
    else if (key == "altyhistos") { // histos on an alternate y-axis
      if (!wpad->histo_ids.size()) {
	cerr << "No 'histos' defined yet! Define alternates after them." << endl;
	continue;
      }
      Tokenize(value,wpad->altyh_ids," ,"); 
      if (!wpad->altyh_ids.size()) wpad->altyh_ids.push_back(value);
    }
    else if (key == "graphs") {
      Tokenize(value,wpad->graph_ids,","); 
      if (!wpad->graph_ids.size()) wpad->graph_ids.push_back(value);
    }
    else if (key == "rootmacros") {
      Tokenize(value,wpad->macro_ids,","); 
      if (!wpad->macro_ids.size()) wpad->macro_ids.push_back(value);
    }
    else if (key == "lines") {
      Tokenize(value,wpad->line_ids,","); 
      if (!wpad->line_ids.size()) wpad->line_ids.push_back(value);
    }
    else if (key == "boxes") {
      Tokenize(value,wpad->box_ids,","); 
      if (!wpad->box_ids.size()) wpad->box_ids.push_back(value);
    }
    else if (key == "labels") {
      Tokenize(value,wpad->label_ids," ,"); 
      if (!wpad->label_ids.size())  wpad->label_ids.push_back(value);
    }
    else if (key == "latex") {
      Tokenize(value,wpad->latex_ids," ,"); 
      if (!wpad->latex_ids.size())  wpad->latex_ids.push_back(value);
    }
    else if (key == "title")        wpad->hframe->histo()->SetTitle(value.c_str());
    else if (key == "xtitle")       wpad->hframe->histo()->SetXTitle(value.c_str());
    else if (key == "ytitle")       wpad->hframe->histo()->SetYTitle(value.c_str());
    else if (key == "ztitle")       wpad->hframe->histo()->SetZTitle(value.c_str());
    else if (key == "rightmargin")  wpad->rightmargin  = str2flt(value);
    else if (key == "leftmargin")   wpad->leftmargin   = str2flt(value);
    else if (key == "topmargin")    wpad->topmargin    = str2flt(value);
    else if (key == "bottommargin") wpad->bottommargin = str2flt(value);
    else if (key == "fillcolor")    wpad->fillcolor    = str2int(value);
    else if (key == "legend")       wpad->legid        = value;
    else if (key == "logx")         wpad->logx         = str2int(value);
    else if (key == "logy")         wpad->logy         = str2int(value);
    else if (key == "logz")         wpad->logz         = str2int(value);
    else if (key == "gridx")        wpad->gridx        = str2int(value);
    else if (key == "gridy")        wpad->gridy        = str2int(value);
    else if (key == "xmin") {
      xmin = str2flt(value);
      wpad->hframe->SetXaxis("",false,0,0,0,0,0,xmin,xmax);
    }
    else if (key == "xmax") {
      xmax = str2flt(value);
      wpad->hframe->SetXaxis("",false,0,0,0,0,0,xmin,xmax);
    }
    else if (key == "ymin") {
      ymin = str2flt(value);
      wpad->hframe->SetYaxis("",false,0,0,0,0,0,ymin,ymax);
    }
    else if (key == "ymax") {
      ymax = str2flt(value);
      wpad->hframe->SetYaxis("",false,0,0,0,0,0,ymin,ymax);
    }
    else {
      cerr << "Unknown key " << key << endl;
    }


  } // while loop

  return true;
}                                                   // processPadSection

//======================================================================
