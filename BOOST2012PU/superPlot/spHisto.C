#include <algorithm>

inline bool isnumeric(const std::string& foo) {
  return (count_if(foo.begin(), foo.end(), ::isdigit) == (int)foo.size());
  //std::count_if(c.begin(), c.end(),
  //std::bind2nd(static_cast< bool (*)( char, std::locale const& ) >(&std::isalpha ),std::locale() ) ) ;
}
//======================================================================

TH1 *findHisto(const string& hid, const string& errmsg="")
{
  map<string,wTH1 *>::const_iterator it = glmap_id2histo.find(hid);
  if( it == glmap_id2histo.end() ) {
    // Try finding the first
    cerr << "Histo ID " << hid << " not found. " << errmsg << endl;
    if( gl_verbose) {
      cout << "Available histo IDs are: " << endl;
      for (it = glmap_id2histo.begin(); it != glmap_id2histo.end(); it++)
	cout << it->first << " ";
      cout << endl;
    }
    return NULL;
  }
  return it->second->histo();
}                                                           // findHisto

//======================================================================

void printUsage(const string additionalinfo="") {
  cerr << "\nERROR in printfstat format: " << additionalinfo<< endl;
  cerr << "printfstat format: \"printf fmt str\",arg1,arg2,...";
  cerr << "where argX = {k|s|i|I|o|u|r|m|M|e|n|p|f|h|d}"    << endl;
  cerr << "	where p=filepath:histo path"                << endl;
  cerr << "	      h=histo path"                         << endl;
  cerr << "	      f=filename"                           << endl;
  cerr << "	      d=histo containing directory/folder"  << endl;
  cerr << "	      I=integral including over/underflows" << endl;
  cerr << "	      ksiourmen=> see ROOT documentation"   << endl;
}

double getStat(TH1 *h, const string& key)
{
  char c = key[0];
  int nbinsx = (int)h->GetNbinsX();
  switch( c ) {
  case 'k': return h->GetKurtosis();
  case 's': return h->GetSkewness();
  case 'i': return h->Integral();
  case 'I': return h->Integral( 0,nbinsx+1 ); // include u/oflows
  case 'o': return h->GetBinContent( nbinsx+1 );
  case 'u': return h->GetBinContent( 0 );
  case 'r': return h->GetRMS();
  case 'm': return h->GetMean();
  case 'M': return h->GetMeanError();
  case 'e': return h->GetEntries();
  default:break;
  }
  return -9e99;
}

float getStatArg(TH1 *h, const string& argstr)
{
  float arg;
  if( isnumeric(argstr) ) {
    arg = str2flt(argstr);
  } else if (argstr.find_first_of("ksiIourmMep") != string::npos) {
    arg = getStat(h,argstr);
  } else {
    printUsage("argument not understood: "+argstr);
    return -9e99;
  }
  return arg;
}

void printHistoStats(TH1 *h, const string& histo_id, const string& printfspec)
{
  if (gl_verbose)
    cout << "print statistics for histo "<<histo_id<<" => "<<h->GetName()<<endl;

  vector<string> v_args, v_fmts;
  Tokenize(printfspec,v_args,"\",");
#if 0
  if( v_args.size()<2 ) {
    printUsage("insufficient number of arguments");
    return;
  }
#endif
  // first arg must be a quoted printf format string
  bool includepcts=true;
  Tokenize( v_args[0],v_fmts,"%\\",includepcts ); // might include "%%" to print % sign

  if( v_fmts.size() < 2 ) {
    printUsage("insufficient number of format specifiers");
    return;
  }
#if 0
  cout<<endl;
  cout<<"args: "; for( size_t i=0; i<v_args.size(); i++ ) cout<<i<<":\""<<v_args[i]<<"\" | "; cout<<endl;
  cout<<"fmts: "; for( size_t i=0; i<v_fmts.size(); i++ ) cout<<i<<":\""<<v_fmts[i]<<"\" | "; cout<<endl;
#endif

  //cout << v_args.size() << "\t" << v_fmts.size() << "\t";
  for( size_t iarg=1,jfmt=0; jfmt<v_fmts.size(); ) {
    string arg = (iarg < v_args.size()) ? v_args[iarg] : "";
    string fmt = v_fmts[jfmt++];

    //cout << "(\""<<fmt<<"\","<<arg<<")\t"<<iarg<<"\t"<<jfmt<<"\t";

    // handle escape characters \n and \t
    if( (fmt == "\\") &&
	(jfmt<v_fmts.size()) ) {
      switch (v_fmts[jfmt][0]) {
      case 'n': cout << "\n"; break;
      case 't': cout << "\t"; break;
      default : cout << "\\" << v_fmts[jfmt][0]; break;
      }
      if (v_fmts[jfmt].size() > 1)
	v_fmts[jfmt] = v_fmts[jfmt].substr(1);
      else
	jfmt++;
      continue;
    }
    if( fmt.find("%") == string::npos ) {
      //printf( fmt.c_str() );
      cout << fmt;
    } else if( (fmt.size() > 1) &&
	       (fmt.find_first_not_of("%") == string::npos) ) {
      if (fmt.size()==2)
	printf ("%%");
      else {
	cerr << "quit f*!)!#%ing around" << endl;
	exit(-1);
      }
    } else {
      if( jfmt > v_fmts.size()-1 ) {
	printUsage("not enough format specifiers for number of arguments");
	return;
      }
      fmt = "%" + v_fmts[jfmt++];

      if( arg.size() > 2 ) { // check for normalization
	size_t pos = arg.find("/");
	if( pos != string::npos ) {
	  string numerstr = arg.substr(0,pos);
	  string denomstr = arg.substr(pos+1);
	  float  numer    = getStatArg(h,numerstr);
	  float  denom    = getStatArg(h,denomstr);
	  printf( fmt.c_str(), numer/denom );
	} else {
	  printUsage("unrecognized argument: "+arg);
	  return;
	}
      } else if (arg.find_first_of("ksiIourmMep") != string::npos) {
	double stat = getStat(h,arg);
	printf( fmt.c_str(), stat );
      } else {
	char c = arg[0];
	switch( c ) {
	case 'n': printf( fmt.c_str(), h->GetName() ); break;
	case 'p': // full histo path
	case 'f': // root filename
	case 'h': // histo path in file
	case 'd': // name of containing directory/folder
	  {
	    map<string,string>::const_iterator it = glmap_id2objpath.find(histo_id);
	    if( it == glmap_id2objpath.end() ) {
	      cerr << "Couldn't find path for histo with id " << histo_id << endl;
	      return;
	    }
	    if( c=='p')  printf( fmt.c_str(), (it->second).c_str() );
	    else {
	      vector<string> v_tokens;
	      Tokenize(it->second,v_tokens,":");
	      switch (c) {
	      case 'h': printf( fmt.c_str(), v_tokens[1].c_str() ); break;
	      case 'd':
		{
		  size_t dirnumber=0;
		  if (arg.size()>1) dirnumber=(size_t)str2int(arg.substr(1));
		  Tokenize(v_tokens[1],v_tokens,"/");
		  if( v_tokens.size() < 2) cout << "/";
		  else if (dirnumber>=v_tokens.size()) continue;
		  else printf( fmt.c_str(), v_tokens[dirnumber].c_str() );
		}
		break;
	      case 'f':
		{
		  Tokenize(v_tokens[0],v_tokens,"/");
		  printf( fmt.c_str(), (v_tokens.rbegin())->c_str() );
		}
		break;
	      }
	    }
	  }
	  break;
	default:
	  printUsage("Unrecognized stat specifier: "+arg);
	  return;
	} // switch (c)
      } // non stat key
      iarg++;

    } // fmt = "%X"
  } // arg loop
}                                                     // printHistoStats

//======================================================================

void printHisto2File(TH1 *histo, string filename)
{
  FILE *fp = fopen(filename.c_str(),"w");
  if( histo->InheritsFrom("TH3") ) {
    TH3 *h3 = (TH3 *)histo;
    int totaln = (int)h3->GetEntries();
    fprintf(fp,"#%s\t%s\t%s\tw\t%%\n",
	    h3->GetXaxis()->GetTitle(),
	    h3->GetYaxis()->GetTitle(),
	    h3->GetZaxis()->GetTitle());
    for (int ix=1; ix<=h3->GetNbinsX(); ix++)
      for (int iy=1; iy<=h3->GetNbinsY(); iy++)
	for (int iz=1; iz<=h3->GetNbinsZ(); iz++)
	  fprintf(fp,"%d\t%d\t%d\t%d\t%f\n",
		  (int)h3->GetXaxis()->GetBinCenter(ix),
		  (int)h3->GetYaxis()->GetBinCenter(iy),
		  (int)h3->GetZaxis()->GetBinCenter(iz),
		  (int)h3->GetBinContent(ix,iy,iz),
		  100.*h3->GetBinContent(ix,iy,iz)/(float)totaln);
    cout << "File " << filename << " written with contents of ";
    cout << h3->GetName() << endl;
  } else if( histo->InheritsFrom("TH2") ) {
    TH2 *h2 = (TH2 *)histo;
    fprintf(fp,"#x\ty\tz\n");
    for (int ix=1; ix<=h2->GetNbinsX(); ix++)
      for (int iy=1; iy<=h2->GetNbinsY(); iy++)
	fprintf(fp,"%d\t%d\t%d\n",
		(int)h2->GetXaxis()->GetBinCenter(ix),
		(int)h2->GetYaxis()->GetBinCenter(iy),
		(int)h2->GetBinContent(ix,iy));
    cout << "File " << filename << " written with contents of ";
    cout << h2->GetName() << endl;
  } else {
    TH1 *h1 = (TH1 *)histo;
    fprintf(fp,"#xlo\tx\txhi\tylo\ty\tyhi\n");
    for (int ix=1; ix<=h1->GetNbinsX(); ix++)
      fprintf(fp,"%f\t%f\t%f\t%f\t%f\t%f\n",
	      h1->GetBinLowEdge(ix),
	      h1->GetBinCenter(ix),
	      (h1->GetBinLowEdge(ix)+h1->GetBinWidth(ix)),
	      (h1->GetBinContent(ix)-h1->GetBinError(ix)),
	      h1->GetBinContent(ix),
	      (h1->GetBinContent(ix)+h1->GetBinError(ix)));
    cout << "File " << filename << " written with contents of ";
    cout << h1->GetName() << endl;
  }
  fclose(fp);
}                                                     // printHisto2File

//======================================================================

void saveHisto2File(TH1 *histo, string outspec)
{
  TFile *rootfile;
  string rootfn,newname;
  TH1 *target;

#if 0
  //This filename input can be an alias
  //
  if( rootfn[0] == '@') {  // reference to an alias defined in ALIAS section
    rootfn = extractAlias(rootfn.substr(1));
    if( !rootfn.size()) return;
  }
#endif

  if( outspec.find(":") != string::npos) {
    vector<string> v_tokens;
    Tokenize(outspec,v_tokens,":");

    rootfn = v_tokens[0];
    newname = v_tokens[1];
  } else
    rootfn = outspec;


  rootfile = openRootFile(rootfn,"UPDATE");

  if (rootfile) {
    target = histo;
    if( newname.size() ) {
      cout<<"Writing histo "<<newname<<" to file "<<rootfn<<endl;
      target = (TH1 *)histo->Clone(newname.c_str());
      //target->SetTitle(histo->GetTitle());
    } else
      cout<<"Writing histo "<<histo->GetName()<<" to file "<<rootfn<<endl;

    target->SetDirectory(rootfile);
    target->Write();
    rootfile->Close();
  }
}                                                      // saveHisto2File

//======================================================================
// takes table of "xloedge bincontent binerror"
//
void load1DHistoContentsFromTextFile(const char *filename, 
				     wTH1 *&wth1)
{
  char linein[LINELEN];
  double xloedge[100],binc[100],bine[100];

  FILE *fp = fopen(filename, "r");

  if (!fp) {
    cerr << "File failed to open, " << filename << endl;
    return;
  }

  if (gl_verbose)
    cout << "Loading vectors from file " << filename;

  int nbins=0;
  while (!feof(fp) && fgets(linein,LINELEN,fp)) {

    if (nbins >= 100) {
      cerr << "Change me! I can only take 100 bins max." << endl;
      exit(-1);
    }
    int nscan= sscanf(linein, "%lf %lf %lf", &xloedge[nbins], &binc[nbins], &bine[nbins]);

    if (nscan==1) { // take this as the end of the histogram, the last "lowedge"
      break;
    } else  if (nscan != 3) {
      cerr << "scan failed, file " << filename << ", line = " << linein << endl;
      return;
    }
    nbins++;
  }
  
  wth1 = new wTH1(new TH1D(filename,filename,nbins,xloedge));
  
  if (gl_verbose) cout << "; read " << nbins << " lines" << endl;
  
  for (int ibin=1; ibin<=nbins; ibin++) {
    wth1->histo()->Fill(wth1->histo()->GetXaxis()->GetBinCenter(ibin),binc[ibin-1]);
    wth1->histo()->SetBinError(ibin,bine[ibin-1]);
  }
}                                    //  load1DHistoContentsFromTextFile

//======================================================================
// takes table of "xbincenter ybincenter bincontent"
//
void load2DHistoContentsFromTextFile(const char *filename, 
				     wTH1& wth1, // must be prebooked
				     const char *fmtstr = "%lf %lf %lf")
{
  char linein[LINELEN];

  FILE *fp = fopen(filename, "r");

  if (!fp) {
    cerr << "File failed to open, " << filename << endl;
    return;
  }

  if (gl_verbose)
    cout << "Loading bin contents from file " << filename << endl;

  int nrec=0;
  cout<<"nrec\txbc\tybc\tibin\tbinc"<<endl;
  while (!feof(fp) && fgets(linein,LINELEN,fp)) {
    double xbc,ybc,binc;
    if (linein[0]=='#') continue; // comments are welcome
    int nscan= sscanf(linein, fmtstr, &xbc, &ybc, &binc);

    if (nscan != 3) {
      cerr << "scan failed:";
      cerr << "  nscan  = " << nscan    << endl;
      cerr << "  file   = " << filename << endl;
      cerr << "  fmtstr = " << fmtstr   << endl;
      cerr << "  line   = " << linein;
      break;
    }
    nrec++;
    int ibin = wth1.histo()->FindFixBin(xbc,ybc);
    cout<<nrec<<"\t"<<xbc<<"\t"<<ybc<<"\t"<<ibin<<"\t"<<binc<<endl;
    wth1.histo()->SetBinContent(ibin,binc);
  }
  cout << nrec << " records read in" << endl;
}                                    //  load2DHistoContentsFromTextFile

//======================================================================
// takes a single column of numbers to fill into a pre-booked histo
//
void fill1DHistoFromTextFile(const string& scanspec,
			     wTH1 *&wth1)
{
  char linein[LINELEN];
  vector<string> v_tokens;

  Tokenize(scanspec,v_tokens,"\",");

  string filename = v_tokens[0];

  TString scanfmt("%lf");
  if (v_tokens.size() > 1) {
    scanfmt = TString(v_tokens[1].c_str());
  }
  FILE *fp = fopen(filename.c_str(), "r");

  if (!fp) {
    cerr << "File failed to open, " << filename << endl;
    return;
  }

  if (gl_verbose)
    cout << "Loading numbers from file " << filename;

  double num;
  while (!feof(fp) && fgets(linein,LINELEN,fp)) {

    int nscan= sscanf(linein, scanfmt.Data(), &num);
    if (!nscan) {
      cerr << "Error reading line " << linein;
      continue;
    }

    wth1->histo()->Fill(num);
  }

  fclose(fp);
}                                            //  fill1DHistoFromTextFile

//======================================================================

wTH1 *getHistoFromSpec(const string& hid,
		       const string& spec)
{
  wTH1  *wth1     = NULL;
  TFile *rootfile = NULL;
  vector<string> v_tokens;
  string fullspec;     // potentially expanded from aliases.

  if( gl_verbose)
    cout << "processing " << spec << endl;

  string hspec;
  string rootfn;

  fullspec = spec;

  Tokenize(fullspec,v_tokens,":");
  if( (v_tokens.size() != 2) ||
      (!v_tokens[0].size())  ||
      (!v_tokens[1].size())    ) {
    cerr << "malformed root histo path file:folder/subfolder/.../histo " << fullspec << endl;
    return NULL;
  } else {
    rootfn = v_tokens[0];
    hspec  = v_tokens[1];
  }

#if 0
  map<string,string>::const_iterator it = glmap_objpath2id.find(fullspec);
  if( it != glmap_objpath2id.end() ) {
    // Allow the possibility to run the script a second time in root
    if( gl_verbose)
      cout << "Object " << fullspec << " already read in, here it is" << endl;
    map<string,wTH1 *>::const_iterator hit = glmap_id2histo.find(it->second);
    if( hit == glmap_id2histo.end() ) {
      if( gl_verbose)
	cout << "oops, sorry, I lied." << endl;
      return NULL;
    }
    wth1 = hit->second;
  } else {
#endif
    rootfile = openRootFile(rootfn);
    if (rootfile) {
      TH1 *h1 = (TH1 *)rootfile->Get(hspec.c_str());
      if( !h1) {
	cerr << "couldn't find " << hspec << " in " << rootfn << endl;
      } else {
	static int linearCounter=0;
	char fakename[100];
	sprintf(fakename,"hist%d",linearCounter++);
	h1=(TH1*)(h1->Clone(fakename));

	// success, record that you read it in.
	if( gl_verbose) cout << "Found " << fullspec << endl;
	glmap_objpath2id.insert(pair<string,string>(fullspec,hid));
	glmap_id2objpath.insert(pair<string,string>(hid,fullspec));
	h1->UseCurrentStyle();
	wth1 = new wTH1(h1);
	glmap_id2histo.insert(pair<string,wTH1 *>(hid,wth1));
      }
    }
    //}
  return wth1;
}                                                    // getHistoFromSpec

//======================================================================

void processCommonHistoParams(const string& key, 
			      const string& value,
			      const string& histo_id,
			      wTH1& wh)
{
  static float xmin=1e99, xmax=-1e99;
  static float ymin=1e99, ymax=-1e99;
  static float zmin=1e99, zmax=-1e99;

  vector<string> v_tokens;

  int os = wh.GetOptStat();

  // Allow user to define a set of common parameters to be used multiple times
  //
  if( key == "applystyle" ) {
    map<string,TStyle *>::const_iterator it = glmap_id2style.find(value);
    if( it == glmap_id2style.end() ) {
      cerr << "Style " << value << " not found, ";
      cerr << "must be defined first" << endl;
      return;
    } else {
      wh.SaveStyle(it->second);
    }
  }
  else if( key == "draw" )        wh.SetDrawOption(value);
  else if( key == "fitopt" )      wh.SetFitOption(value);
  else if( key == "fitrange" ) {
    Tokenize(value,v_tokens,"," );
    if( v_tokens.size() != 2 ) {
      cerr << "fitrange key expects xmin,xmax; value= ";
      cerr << value << endl;
    } else {
      wh.SetFitRange(str2flt(v_tokens[0]),str2flt(v_tokens[1]));
    }
  }
  else if( key == "title" )       wh.histo()->SetTitle(value.c_str());
  else if( key == "notitle" )     wh.histo()->SetBit(TH1::kNoTitle,str2int(value));

  else if( key == "markercolor" ) wh.SetMarker(str2int(value) );
  else if( key == "markerstyle" ) wh.SetMarker(0,str2int(value));
  else if( key == "markersize" )  wh.SetMarker(0,0,str2int(value));
  else if( key == "linecolor" )   wh.SetLine(str2int(value));
  else if( key == "linestyle" )   wh.SetLine(0,str2int(value));
  else if( key == "linewidth" )   wh.SetLine(0,0,str2int(value));
  else if( key == "fillcolor" )   wh.histo()->SetFillColor(str2int(value));
  else if( key == "fillstyle" )   wh.histo()->SetFillStyle(str2int(value));
  else if( key == "leglabel" )    wh.SetLegendEntry(value);

  else if( key == "reduceerror" ) { 
    for (int ibin=1;ibin<=wh.histo()->GetNbinsX(); ibin++) { 
      double binError = (wh.histo()->GetBinError(ibin)) * str2int(value) ; 
      wh.histo()->SetBinError(ibin,binError) ; 
    }
  }

  else if( key == "rebin" ) {
    if (str2int(value))
      ((TH1 *)wh.histo())->Rebin(str2int(value));
  }
  else if( key == "rebinx" ) {
    if( wh.histo()->InheritsFrom("TH2") ) ((TH2 *)wh.histo())->RebinX(str2int(value));
    else                                 ((TH1 *)wh.histo())->Rebin(str2int(value));
  }
  else if( key == "rebiny" ) {
    if( wh.histo()->InheritsFrom("TH2") ) ((TH2 *)wh.histo())->RebinY(str2int(value));
    else {
      cerr << "\trebiny: not defined for 1-d histograms" << endl;
    }
  } 

  // axes
  else if( key == "xtitle" )       wh.SetXaxis(value);
  else if( key == "ytitle" )       wh.SetYaxis(value);
  else if( key == "ztitle" )       wh.SetZaxis(value);
  else if( key == "xtitlesize" )   wh.SetXaxis("",false,str2flt(value));
  else if( key == "ytitlesize" )   wh.SetYaxis("",false,str2flt(value));
  else if( key == "ztitlesize" )   wh.SetZaxis("",false,str2flt(value));
  else if( key == "xtitleoffset" ) wh.SetXaxis("",false,0,str2flt(value));
  else if( key == "ytitleoffset" ) wh.SetYaxis("",false,0,str2flt(value));
  else if( key == "ztitleoffset" ) wh.SetZaxis("",false,0,str2flt(value));
  else if( key == "xtitlefont" )   wh.SetXaxis("",false,0,0,str2int(value));
  else if( key == "ytitlefont" )   wh.SetYaxis("",false,0,0,str2int(value));
  else if( key == "ztitlefont" )   wh.SetZaxis("",false,0,0,str2int(value));
  else if( key == "xlabelsize" )   wh.SetXaxis("",false,0,0,0,str2flt(value));
  else if( key == "ylabelsize" )   wh.SetYaxis("",false,0,0,0,str2flt(value));
  else if( key == "zlabelsize" )   wh.SetZaxis("",false,0,0,0,str2flt(value));
  else if( key == "xlabelfont" )   wh.SetXaxis("",false,0,0,0,0,str2int(value));
  else if( key == "ylabelfont" )   wh.SetYaxis("",false,0,0,0,0,str2int(value));
  else if( key == "zlabelfont" )   wh.SetZaxis("",false,0,0,0,0,str2int(value));
  else if( key == "xndiv" )  wh.SetXaxis("",false,0,0,0,0,0,1e99,-1e99,str2int(value));
  else if( key == "yndiv" )  wh.SetYaxis("",false,0,0,0,0,0,1e99,-1e99,str2int(value));
  else if( key == "zndiv" )  wh.SetZaxis("",false,0,0,0,0,0,1e99,-1e99,str2int(value));

  else if( key == "xmin" ) { xmin=str2flt(value); wh.SetXaxis("",false,0,0,0,0,0,xmin,xmax); }
  else if( key == "xmax" ) { xmax=str2flt(value); wh.SetXaxis("",false,0,0,0,0,0,xmin,xmax); }
  else if( key == "ymin" ) { ymin=str2flt(value); wh.SetYaxis("",false,0,0,0,0,0,ymin,ymax); }
  else if( key == "ymax" ) { ymax=str2flt(value); wh.SetYaxis("",false,0,0,0,0,0,ymin,ymax); }
  else if( key == "zmin" ) { zmin=str2flt(value); wh.SetZaxis("",false,0,0,0,0,0,zmin,zmax); }
  else if( key == "zmax" ) { zmax=str2flt(value); wh.SetZaxis("",false,0,0,0,0,0,zmin,zmax); }

  else if( key == "xbinlabels" ) {
    Tokenize(value,v_tokens,"," );
    if( !v_tokens.size() ) {
      cerr << "expect comma-separated list of bin labels ";
      cerr << value << endl;
    }
    if( gl_verbose )
      cout << "Loaded " << v_tokens.size() << " x-axis bin labels" << endl;
    for (int ibin=1; ibin<=min((int)v_tokens.size(),wh.histo()->GetNbinsX()); ibin++)
      wh.histo()->GetXaxis()->SetBinLabel( ibin,v_tokens[ibin-1].c_str() );
  }

  else if( key == "ybinlabels" ) {
    Tokenize(value,v_tokens,"," );
    if( !v_tokens.size() ) {
      cerr << "expect comma-separated list of bin labels ";
      cerr << value << endl;
    }
    for( int ibin=1; ibin<=min((int)v_tokens.size(),wh.histo()->GetNbinsY() ); ibin++)
      wh.histo()->GetYaxis()->SetBinLabel( ibin,v_tokens[ibin-1].c_str() );
  }

  // stats box
  else if( key == "statson" )    wh.SetStats(str2int(value) != 0);
  else if( key == "statsx1ndc" ) wh.SetStats(wh.statsAreOn(),os,str2flt(value));
  else if( key == "statsy1ndc" ) wh.SetStats(wh.statsAreOn(),os,0.0,str2flt(value));
  else if( key == "statsx2ndc" ) wh.SetStats(wh.statsAreOn(),os,0.0,0.0,str2flt(value));
  else if( key == "statsy2ndc" ) wh.SetStats(wh.statsAreOn(),os,0.0,0.0,0.0,str2flt(value));
  else if( key == "optstat" ) {
    if( value.find_first_not_of("012") == string::npos )  // it's an integer already
      wh.SetStats( wh.statsAreOn(),str2int(value) );
    else {
      os = 0;
      if     ( value.find('K') != string::npos ) os += 200000000; //  kurtosis and kurtosis error printed
      else if( value.find('k') != string::npos ) os += 100000000; //  kurtosis printed
      if     ( value.find('S') != string::npos ) os += 20000000; //  skewness and skewness error printed
      else if( value.find('s') != string::npos ) os += 10000000; //  skewness printed
      if     ( value.find('i') != string::npos ) os += 1000000; //  integral of bins printed
      if     ( value.find('o') != string::npos ) os += 100000; //  number of overflows printed
      if     ( value.find('u') != string::npos ) os += 10000; //  number of underflows printed
      if     ( value.find('R') != string::npos ) os += 2000; //  rms and rms error printed
      else if( value.find('r') != string::npos ) os += 1000; //  rms printed
      if     ( value.find('M') != string::npos ) os += 200; //  mean value mean error values printed
      else if( value.find('m') != string::npos ) os += 100; //  mean value printed
      if     ( value.find('e') != string::npos ) os += 10; //  number of entries printed
      if     ( value.find('n') != string::npos ) os += 1; //  name of histogram is printed
      wh.SetStats( wh.statsAreOn(),os );
    }
  }
  else if( (key == "errorson" ) &&
	   str2int(value)     ) wh.histo()->Sumw2();

  //==============================
  else if( key == "erroroption" ) {
  //==============================
    // only for TProfiles.
    // allowed options are
    // ' ' (default, e=spread/sqrt(n)
    // 's' (e=spread)
    // others -> see ROOT documentation!
    //
    if( wh.histo()->InheritsFrom("TProfile") ) {
      ((TProfile *)wh.histo())->SetErrorOption(value.c_str());
    }
  }

  // iostream parameters
  else if( key == "setprecision" ) cout << setprecision( str2int(value) );
  else if( key == "setwidth" )     cout << setw        ( str2int(value) );

  else if( key == "printfstats" )  printHistoStats( wh.histo(),histo_id,value );
  else if( key == "print2file" )   printHisto2File( wh.histo(),value );
  else if( key == "save2file" )     saveHisto2File( wh.histo(),value );
  else if( key == "normalize" ) {
    TH1 *h1 = (TH1 *)wh.histo();
    float norm = str2flt(value);
    if( h1->Integral() > 0.0 )
      h1->Scale( norm/h1->Integral() );
    else
      cerr << h1->GetName() << " integral is ZERO, cannot normalize." << endl;
  }
  //==============================
  else if( key == "scalebyfactor" ) {
  //==============================

    double sf = (double)str2flt(value);
    TH1 *h1 = (TH1 *)wh.histo();
    if( gl_verbose ) cout << "scaling histo "<<h1->GetName()<<" by "<<sf<<endl;
    h1->Scale(sf);
  }
  //==============================
  else if( key == "scalebysample" ) {
  //==============================

    // Expect value=@samplename(integlumi_invpb)
    Tokenize(value,v_tokens,"()");
    if( v_tokens.size() != 2 ) {
      cerr << "invalid scalebysample specifier, " << value << endl;
      return;
    }
    
    double sf = getSampleScaleFactor(v_tokens[0],
				     (double)str2flt(v_tokens[1]));
    TH1 *h1 = (TH1 *)wh.histo();
    if( gl_verbose ) cout << "scaling histo "<<h1->GetName()<<" by "<<sf<<endl;
    h1->Scale(sf);
  }
  //==============================
  else if( key == "fits" ) {
  //==============================

    Tokenize(value,v_tokens,","); 
    for (size_t i=0; i<v_tokens.size(); i++) {
      TF1 *tf1 = findTF1(v_tokens[i]);
      if( !tf1 ) {
	cerr << "TF1 " << v_tokens[i] << " must be defined first" << endl;
	continue;
      }
      string funcnewname = v_tokens[i]+histo_id;
      TF1 *mytf1 = new TF1(*tf1);
      mytf1->SetName(funcnewname.c_str());
      wh.loadFitFunction(mytf1);
    }
  }
  else {
    cerr << "unknown key " << key << endl;
  }
}                                            // processCommonHistoParams

//======================================================================

void processCommonHistoParams(const string& key, 
			      const string& value,
			      const std::vector<std::pair<string,wTH1 *> >& v_wh)
{
  for (size_t i=0; i<v_wh.size(); i++) {
    processCommonHistoParams(key,value,v_wh[i].first,*(v_wh[i].second));
  }
}

//======================================================================

bool                              // returns true if success
processHistoSection(FILE *fp,
		    string& theline,
		    bool& new_section)
{
  vector<string> v_tokens;

  wTH1   *wth1 = NULL;
  string *hid  = NULL;

  if( gl_verbose )
    cout << "Processing histo section" << endl;

  new_section=false;

  while (getLine(fp,theline,"histo")) {
    if( !theline.size() ) continue;
    if( theline[0] == '#' ) continue; // comments are welcome

    if( theline[0] == '[' ) {
      new_section=true;
      return true;
    }

    string key, value;
    if( !getKeyValue(theline,key,value) ) continue;

    //--------------------
    if( key == "id" ) {
    //--------------------
      if( hid != NULL ) {
	cerr << "no more than one id per histo section allowed " << value << endl;
	break;
      }

      hid = new string(value);

    //------------------------------
    } else if( key == "clone" ) {
    //------------------------------
      if( !hid ) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      map<string,wTH1 *>::const_iterator it = glmap_id2histo.find(value);
      if( it == glmap_id2histo.end() ) {
	cerr << "Histo ID " << value << " not found,";
	cerr << "clone must be defined after the clonee" << endl;
	break;
      }
      wth1 = it->second->Clone(string(it->second->histo()->GetName())+"_"+(*hid),
			       string(it->second->histo()->GetTitle()));
      glmap_id2histo.insert(pair<string,wTH1 *>(*hid,wth1));

    //------------------------------
    } else if( key == "path" ) {
    //------------------------------
      if( !hid ) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      if( wth1 ) {
	cerr << "histo already defined" << endl; continue;
      }
      wth1  = getHistoFromSpec(*hid,value);
      if( !wth1 ) continue;

    //------------------------------
    } else if( key == "book1dpars" ) { // book a blank 1D histo with provided parameters
    //------------------------------
      if( !hid ) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      if( wth1 ) {
	cerr << "histo already defined" << endl; continue;
      }

      Tokenize(value,v_tokens,","); // title can't have commas in it!

      if( (v_tokens.size() != 5) ||
	  (!v_tokens[0].size())  ||
	  (!v_tokens[1].size())  ||
	  (!isnumeric(v_tokens[2]))  ||
	  (!v_tokens[3].size())  ||
	  (!v_tokens[4].size()) ) {
	for (unsigned i=0; i<v_tokens.size(); i++)
	  cout << v_tokens[i] << endl;
	cerr << "malformed 1d parameter specification " << value << endl; continue;
      }

      wth1 = new wTH1(new TH1D(v_tokens[0].c_str(),
			       v_tokens[1].c_str(),
			       str2int(v_tokens[2]),
			       str2flt(v_tokens[3]),
			       str2flt(v_tokens[4])));

      glmap_id2histo.insert(pair<string,wTH1 *>(*hid,wth1));

    //------------------------------
    } else if( key == "book2dpars" ) { // book a blank 2D histo with provided parameters
    //------------------------------
      if( !hid ) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      if( wth1 ) {
	cerr << "histo already defined" << endl; continue;
      }

      Tokenize(value,v_tokens,","); // title can't have commas in it!

      if( (v_tokens.size() != 8) ||
	  (!v_tokens[0].size())  ||
	  (!v_tokens[1].size())  ||
	  (!isnumeric(v_tokens[2]))  ||
	  (!v_tokens[3].size())  ||
	  (!v_tokens[4].size())  ||
	  (!isnumeric(v_tokens[5]))  ||
	  (!v_tokens[6].size())  ||
	  (!v_tokens[7].size())     ) {
	for (unsigned i=0; i<v_tokens.size(); i++)
	  cout << v_tokens[i] << endl;
	cerr << "malformed 1d parameter specification " << value << endl; continue;
      }

      wth1 = new wTH1(new TH2D(v_tokens[0].c_str(),
			       v_tokens[1].c_str(),
			       str2int(v_tokens[2]),
			       str2flt(v_tokens[3]),
			       str2flt(v_tokens[4]),
			       str2int(v_tokens[5]),
			       str2flt(v_tokens[6]),
			       str2flt(v_tokens[7])
			       )
		      );

      glmap_id2histo.insert(pair<string,wTH1 *>(*hid,wth1));

    //------------------------------
    } else if( key == "loadtxtfile" ) {
    //------------------------------
      if( !hid ) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      load1DHistoContentsFromTextFile(value.c_str(),wth1);

      glmap_id2histo.insert(pair<string,wTH1 *>(*hid,wth1));

    //------------------------------
    } else if( key == "load2dtxtfile" ) {
    //------------------------------
      if( !hid ) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      if( !wth1 ) {
	cerr << "Must book 2d histo with 'book2dpars' first" << endl; continue;
      }

      Tokenize(value,v_tokens,",");

      switch(v_tokens.size()) {
      case 1:  load2DHistoContentsFromTextFile(v_tokens[0].c_str(),*wth1); break;
      case 2:  load2DHistoContentsFromTextFile(v_tokens[0].c_str(),*wth1,v_tokens[1].c_str()); break;
      default:
	cerr << "malformed load2dtxtfile spec path[,format]] " << value << endl;
	break;
      }

    //------------------------------
    } else if( key == "filltxtfile" ) { // fill pre-booked 1D histo with list of numbers
    //------------------------------
      if( !hid ) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      if( !wth1 ) {
	cerr << "histo not defined yet!" << endl; continue;
      }

      fill1DHistoFromTextFile(value,wth1);

    //------------------------------
    } else if( key == "fillfromgraph" ) { // converts graph into 1D histo
    //------------------------------
      if( !hid ) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      if( !wth1 ) {
	cerr << "histo not defined yet!" << endl; continue;
      }

      // defined in spGraph.C
      void fill1DHistoFromGraph(std::string& gid,wTH1 *&wth1);
      
      fill1DHistoFromGraph(value,wth1);

    //------------------------------
    } else if( key == "fillfromtree" ) { // converts tree into 1D histo
    //------------------------------
      if( !hid ) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }

      // defined in spTree.C
      void fillHistoFromTreeVar(std::string& gid,int index,wTH1 *&wth1);

      fillHistoFromTreeVar(value,0,wth1);

      assert(wth1);

      glmap_id2histo.insert(pair<string,wTH1 *>(*hid,wth1));

    //------------------------------
    } else if( key == "hprop" ) {    // fill a histogram with a per-bin property of another
    //------------------------------
      if( !hid ) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }
      if( wth1 ) {
	cerr << "histo already defined" << endl; continue;
      }

      // Tokenize again to get the histo ID and the desired property
      string hprop = value;
      Tokenize(hprop,v_tokens,":");
      if( (v_tokens.size() != 2) ||
	  (!v_tokens[0].size())  ||
	  (!v_tokens[1].size())    ) {
	cerr << "malformed hid:property specification " << hprop << endl; continue;
      }

      string tgthid = v_tokens[0];
      string prop   = v_tokens[1];

      map<string,wTH1 *>::const_iterator it = glmap_id2histo.find(tgthid);
      if( it == glmap_id2histo.end() ) {
	cerr << "Histo ID " << tgthid << " not found, histo must be defined first" << endl;
	break;
      }
      TH1 *tgth1 = it->second->histo();
      wth1       = it->second->Clone(string(tgth1->GetName())+"_"+prop,
				     string(tgth1->GetTitle())+" ("+prop+")");
      TH1 *h1=wth1->histo();
      h1->Clear();
      glmap_id2histo.insert(pair<string,wTH1 *>(*hid,wth1));
      
      if( !prop.compare("errors") ) {
	int nbins = h1->GetNbinsX()*h1->GetNbinsY()*h1->GetNbinsZ();
	for (int ibin=1; ibin<=nbins; ibin++)
	  h1->SetBinContent(ibin,tgth1->GetBinError(ibin));
      }
      else {
	cerr << "Unrecognized property: " << prop << endl;
	break;
      }

    } else if( !wth1 ) {  // all other keys must have "path" defined
      cerr << "key 'path' or 'clone' must be defined before key " << key << endl;
      break;
    }

    else {
      processCommonHistoParams(key,value,*hid,*wth1);
    }
  }
  return (wth1 != NULL);
}                                                 // processHistoSection

//======================================================================
