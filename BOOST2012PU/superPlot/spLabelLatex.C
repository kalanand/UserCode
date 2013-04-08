#include "TLatex.h"

struct wLabel_t {
  wLabel_t(const string& intxt,
	   float inx1ndc=0., float iny1ndc=0.,
	   float inx2ndc=1., float iny2ndc=1.,
	   float intxtsz=0.035) :
    text(intxt),
    x1ndc(inx1ndc),y1ndc(iny1ndc), x2ndc(inx2ndc),y2ndc(iny2ndc), textsize(intxtsz) {}
  string   text;
  float    x1ndc, y1ndc;
  float    x2ndc, y2ndc;
  float    textsize;
  unsigned textfont;
};
static map<string, wLabel_t *>  glmap_id2label;
static map<string, TLatex *>    glmap_id2latex;

//======================================================================

bool                                          // returns true if success
processLabelSection(FILE   *fp,
		    string& theline,
		    bool&   new_section)
{
  vector<string> v_tokens;

  if (gl_verbose)
    cout << "Processing label section" << endl;

  string  *lid  = NULL;
  wLabel_t *wlab = new wLabel_t("FillMe");

  new_section=false;

  while (getLine(fp,theline,"label")) {
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
	cerr << "no more than one id per label section allowed " << *lid << endl;
	break;
      }
      lid = new string(value);
      if (glmap_id2label.find(*lid) != glmap_id2label.end()) { // label id's cannot be redefined
	cerr << "Label ID " << *lid << " already defined, cannot be redefined." << endl;
	break;
      }
    }
    else if (key == "text")       wlab->text   = value;
    else if (key == "x1ndc")      wlab->x1ndc  = str2flt(value);
    else if (key == "y1ndc")      wlab->y1ndc  = str2flt(value);

    else if (key == "textsize")   wlab->textsize   = str2flt(value);
    else if (key == "textfont")   wlab->textfont   = str2int(value);
    else {
      cerr << "unknown key " << key << endl;
    }
  } // while loop

  glmap_id2label.insert(pair<string,wLabel_t *>(*lid,wlab));
  return true;
}                                                 // processLabelSection

//======================================================================

bool                                          // returns true if success
processLatexSection(FILE   *fp,
		    string& theline,
		    bool&   new_section)
{
  vector<string> v_tokens;

  if (gl_verbose)
    cout << "Processing latex section" << endl;
  string  *lid  = NULL;
  double posx=.5,posy=.5;

  new_section=false;
  TLatex  *ltx=NULL;

  while (getLine(fp,theline,"latex")) {
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
	cerr << "no more than one id per latex section allowed " << *lid << endl;
	break;
      }
      lid = new string(value);
      if (glmap_id2latex.find(*lid) != glmap_id2latex.end()) { // latex id's cannot be redefined
	cerr << "Latex ID " << *lid << " already defined, cannot be redefined." << endl;
	break;
      }
      ltx = new TLatex();
      ltx->SetNDC();
    }
    else if (key == "text") {
      if (value[0]=='@') {
	assert(0);
	string temp=value;
	value = extractAlias(temp.substr(1));
      }
      if (value.size())
	ltx->SetText(posx,posy,value.c_str());
    }
    else if( key == "kstest" ) {

      Tokenize(value,v_tokens,","); 
      if (v_tokens.size()!=3 &&
	  v_tokens.size()!=4   ) {
	cerr << "Invalid format for key kstest: kstest=histoid1,histoid2[,option],printf format" << endl;
	exit(-1);
      }
      TH1 *cmph1 = findHisto(v_tokens[0]);  assert(cmph1);
      TH1 *cmph2 = findHisto(v_tokens[1]);  assert(cmph2);

      if (gl_verbose)
	cout<<"Comparing histos "<<cmph1->GetName()<<", "<<cmph2->GetName()<<endl;

      double ksprob;
      if (v_tokens.size()==4) {
	ksprob = cmph1->KolmogorovTest(cmph2,v_tokens[2].c_str());
	ltx->SetText(posx,posy,Form(v_tokens[3].c_str(),ksprob));
      }
      else {
	ksprob = cmph1->KolmogorovTest(cmph2);
	ltx->SetText(posx,posy,Form(v_tokens[2].c_str(),ksprob));
      }
    }

    else if (key == "xpos")     { posx = str2flt(value); ltx->SetX(posx); }
    else if (key == "ypos")     { posy = str2flt(value); ltx->SetY(posy); }
    else if (key == "angle")    ltx->SetTextAngle(str2flt(value));
    else if (key == "textsize") ltx->SetTextSize(str2flt(value));
    else if (key == "textfont") ltx->SetTextFont(str2int(value));
    else {
      cerr << "unknown key " << key << endl;
    }
  } // while loop

  if (ltx) {
    glmap_id2latex.insert(pair<string,TLatex *>(*lid,ltx));
    return true;
  }
  return false;
}                                                 // processLatexSection
