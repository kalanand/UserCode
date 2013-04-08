#include "TStyle.h"
#include "mystyle.C"
//#include "tdrstyle4timing.C"
#include "tdrstyle.C"

static map<string, TStyle *>    glmap_id2style;

//======================================================================

bool                                          // returns true if success
processStyleSection(FILE *fp,string& theline, bool& new_section)
{
  vector<string> v_tokens;

  string *sid      = NULL;
  TStyle *thestyle = NULL;

  if (gl_verbose)
    cout << "Processing style section" << endl;

  new_section=false;

  while (getLine(fp,theline,"style")) {
    if (!theline.size()) continue;
    if (theline[0] == '#') continue; // comments are welcome

    if (theline[0] == '[') {
      new_section=true;
      return true;
    }

    string key, value;
    if (!getKeyValue(theline,key,value)) continue;

    if (key == "id") {
      if (sid != NULL) {
	cerr << "no more than one id per style section allowed " << value << endl;
	break;
      }

      sid = new string(value);
      thestyle = new TStyle(*gStyle); // Assume current attributes, let user override specifics
      thestyle->SetNameTitle(sid->c_str(),sid->c_str());
      glmap_id2style.insert(pair<string,TStyle*>(*sid,thestyle));
      continue;
    }
    else if (!sid) {
      // assume the style is the global style
      thestyle = gStyle;
      sid = new string("using global"); // so as to pass this check the next time
      cerr << "No style ID defined, assuming global style" << endl;
      continue;
    }

    if (key == "style") {
      if (value == "TDR")
	setTDRStyle();
      else if (value == "Plain")
	gROOT->SetStyle("Plain");
      else 
	cerr << "unknown style name " << value << endl;
    }
    else if (key == "optstat")  {
      if (gl_verbose) cout << "OptStat = " << thestyle->GetOptStat() << endl;
      thestyle->SetOptStat(value.c_str());
      if (gl_verbose) cout << "OptStat = " << thestyle->GetOptStat() << endl;
    }
    else if (key == "opttitle")  thestyle->SetOptTitle(str2int(value));

    else if (key == "padrightmargin")  thestyle->SetPadRightMargin (str2flt(value));
    else if (key == "padleftmargin")   thestyle->SetPadLeftMargin  (str2flt(value));
    else if (key == "padtopmargin")    thestyle->SetPadTopMargin   (str2flt(value));
    else if (key == "padbottommargin") thestyle->SetPadBottomMargin(str2flt(value));
    else if (key == "padgridx")        thestyle->SetPadGridX       (str2int(value));
    else if (key == "padgridy")        thestyle->SetPadGridY       (str2int(value));

    // Set the position/size of the title box

    else if (key == "titlexndc")    thestyle->SetTitleX(str2flt(value));
    else if (key == "titleyndc")    thestyle->SetTitleY(str2flt(value));
    else if (key == "titlewndc")    thestyle->SetTitleW(str2flt(value));
    else if (key == "titlehndc")    thestyle->SetTitleH(str2flt(value));
    else if (key == "titlefont")    thestyle->SetTitleFont(str2int(value));
    else if (key == "titlebordersize") thestyle->SetTitleBorderSize(str2int(value));

    else if (key == "markercolor")  thestyle->SetMarkerColor(str2int(value));
    else if (key == "markerstyle")  thestyle->SetMarkerStyle(str2int(value));
    else if (key == "markersize")   thestyle->SetMarkerSize(str2int(value));
    else if (key == "linecolor")    thestyle->SetLineColor(str2int(value));
    else if (key == "linestyle")    thestyle->SetLineStyle(str2int(value));
    else if (key == "linewidth")    thestyle->SetLineWidth(str2int(value));
    else if (key == "fillcolor")    thestyle->SetFillColor(str2int(value));
    else if (key == "fillstyle")    thestyle->SetFillStyle(str2int(value));

    // axes
    else if (key == "xtitlesize")   thestyle->SetTitleSize(str2flt(value),"X");
    else if (key == "ytitlesize")   thestyle->SetTitleSize(str2flt(value),"Y");
    else if (key == "ztitlesize")   thestyle->SetTitleSize(str2flt(value),"Z");
    else if (key == "xtitleoffset") thestyle->SetTitleOffset(str2flt(value),"X");
    else if (key == "ytitleoffset") thestyle->SetTitleOffset(str2flt(value),"Y");
    else if (key == "ztitleoffset") thestyle->SetTitleOffset(str2flt(value),"Z");
    else if (key == "xlabeloffset") thestyle->SetLabelOffset(str2flt(value),"X");
    else if (key == "ylabeloffset") thestyle->SetLabelOffset(str2flt(value),"Y");
    else if (key == "zlabeloffset") thestyle->SetLabelOffset(str2flt(value),"Z");
    else if (key == "xtitlefont")   thestyle->SetTitleFont(str2int(value),"X");
    else if (key == "ytitlefont")   thestyle->SetTitleFont(str2int(value),"Y");
    else if (key == "ztitlefont")   thestyle->SetTitleFont(str2int(value),"Z");
    else if (key == "xlabelsize")   thestyle->SetLabelSize(str2flt(value),"X");
    else if (key == "ylabelsize")   thestyle->SetLabelSize(str2flt(value),"Y");
    else if (key == "zlabelsize")   thestyle->SetLabelSize(str2flt(value),"Z");
    else if (key == "xlabelfont")   thestyle->SetLabelFont(str2int(value),"X");
    else if (key == "ylabelfont")   thestyle->SetLabelFont(str2int(value),"Y");
    else if (key == "zlabelfont")   thestyle->SetLabelFont(str2int(value),"Z");
    else if (key == "xndiv")        thestyle->SetNdivisions(str2int(value),"X");
    else if (key == "yndiv")        thestyle->SetNdivisions(str2int(value),"Y");
    else if (key == "zndiv")        thestyle->SetNdivisions(str2int(value),"Z");

    // Set the position of the statbox
    else if (key == "statx2ndc")    thestyle->SetStatX(str2flt(value));
    else if (key == "staty2ndc")    thestyle->SetStatY(str2flt(value));
    else if (key == "statwndc")     thestyle->SetStatW(str2flt(value));
    else if (key == "stathndc")     thestyle->SetStatH(str2flt(value));

    else if (key == "statfont")     thestyle->SetStatFont    (str2int(value));
    else if (key == "statfontsize") thestyle->SetStatFontSize(str2flt(value));
    else if (key == "statformat")   thestyle->SetStatFormat  (value.c_str());
    else if (key == "statstyle")    thestyle->SetStatStyle   (str2int(value.c_str()));

    else if (key == "fitformat")    thestyle->SetFitFormat   (value.c_str());

    else if (key == "painttextfmt") thestyle->SetPaintTextFormat(value.c_str());
    else if (key == "markersize")   thestyle->SetMarkerSize(str2flt(value));
    else {
      cerr << "Unknown key " << key << endl;
    }

  } // while loop

  return true;
}                                                 // processStyleSection

//======================================================================
