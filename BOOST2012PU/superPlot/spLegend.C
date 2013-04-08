#include "TLegend.h"

struct wLegend_t {
  wLegend_t(float inx1ndc=0., float iny1ndc=0.,
	    float inx2ndc=1., float iny2ndc=1.,
	    unsigned infillclr=0) :
    header(""),
    x1ndc(inx1ndc),y1ndc(iny1ndc), x2ndc(inx2ndc),y2ndc(iny2ndc),
    fillcolor(infillclr),bordersize(1),ncolumns(1),linewidth(1),
    textfont(42)
  {}
  string   header;
  float    x1ndc, y1ndc;
  float    x2ndc, y2ndc;
  unsigned fillcolor;
  int      bordersize;
  int      ncolumns;
  unsigned linewidth;
  unsigned textfont;
  float    textsize;
  string   drawoption;
  TLegend *leg;
};
static map<string, wLegend_t *> glmap_id2legend;    // the map...of legends

//======================================================================

bool                              // returns true if success
processLegendSection(FILE *fp,
		     string& theline,
		     bool& new_section)
{
  vector<string> v_tokens;

  string  *lid  = NULL;
  wLegend_t *wleg = new wLegend_t();

  if (gl_verbose)
    cout << "Processing legend section" << endl;

  new_section=false;

  while (getLine(fp,theline,"legend")) {
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
	cerr << "no more than one id per legend section allowed " << *lid << endl;
	break;
      }
      lid = new string(value);
      if (glmap_id2legend.find(*lid) != glmap_id2legend.end()) { // legend id's cannot be redefined
	cerr << "Legend ID " << *lid << " already defined, cannot be redefined." << endl;
	break;
      }
    }
    else if (key == "header")     wleg->header = value;
    else if (key == "x1ndc")      wleg->x1ndc  = str2flt(value);
    else if (key == "y1ndc")      wleg->y1ndc  = str2flt(value);
    else if (key == "x2ndc")      wleg->x2ndc  = str2flt(value);
    else if (key == "y2ndc")      wleg->y2ndc  = str2flt(value);
    else if (key == "ncol")       wleg->ncolumns   = str2int(value);
    else if (key == "textsize")   wleg->textsize   = str2flt(value);
    else if (key == "textfont")   wleg->textfont   = str2int(value);
    else if (key == "linewidth")  wleg->linewidth  = str2int(value);
    else if (key == "fillcolor")  wleg->fillcolor  = str2int(value);
    else if (key == "bordersize") wleg->bordersize = str2int(value);
    else if (key == "draw")       wleg->drawoption = value;
    else {
      cerr << "unknown key " << key << endl;
    }
  }

  wleg->leg = new TLegend(wleg->x1ndc,wleg->y1ndc,
			  wleg->x2ndc,wleg->y2ndc);

  if (wleg->header.size()) wleg->leg->SetHeader(wleg->header.c_str());
  wleg->leg->SetTextSize(wleg->textsize);
  wleg->leg->SetTextFont(wleg->textfont);
  wleg->leg->SetBorderSize(wleg->bordersize);
  wleg->leg->SetFillColor(wleg->fillcolor);
  wleg->leg->SetLineWidth(wleg->linewidth);
  wleg->leg->SetNColumns(wleg->ncolumns);
  
  glmap_id2legend.insert(pair<string,wLegend_t *>(*lid,wleg));
  return true;
}                                                // processLegendSection

//======================================================================
