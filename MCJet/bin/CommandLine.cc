////////////////////////////////////////////////////////////////////////////////
//
// CommandLine
// -----------
//
//            06/15/2008 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "CommandLine.h"


#include <fstream>
#include <iomanip>


using namespace std;


////////////////////////////////////////////////////////////////////////////////
// construction / destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
CommandLine::CommandLine()
{
  
}


//______________________________________________________________________________
CommandLine::~CommandLine()
{
  
}


////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
bool CommandLine::parse(int argc,char**argv)
{
  _exe = argv[0];
  _options.clear();
  _ordered_options.clear();
  _unknowns.clear();
  
  for (int i=1;i<argc;i++) {
    string opt=argv[i];
    if(0!=opt.find("-")) {
      if (i==1) {
	bool success = parse_file(opt);
	if (!success) return false;
	continue;
      }
      else {
	cout<<"CommandLine ERROR: options must start with '-'!"<<endl;
	return false;
      }
    }
    opt.erase(0,1);
    string next=argv[i+1];
    if (/*0==next.find("-")||*/i+1>=argc) {
      cout<<"ERROR: option '"<<opt<<"' requires value!"<<endl;
      return false;
    }
    _options[opt] = make_pair(next,false);
    i++;
    if (i<argc-1) {
      next=argv[i+1];
      while (next.find("-")!=0) {
	_options[opt].first += ","+next;
	i++;
	next = (i<argc-1) ? argv[i+1] : "-";
      }
    }
  }
  
  return true;
}
 

//______________________________________________________________________________
bool CommandLine::check()
{
  bool result = true;
  OptionMap_t::const_iterator it;
  for (it = _options.begin();it!=_options.end();++it) {
    if (!it->second.second) {
      cout<<"CommandLine WARNING: unused option '"<<it->first<<"'!"<<endl;
      result = false;
    }
  }
  
  if (_unknowns.size()>0) {
    result = false;
    cout<<"\nCommandLine WARNING: "<<_unknowns.size()
	<<" the followingparameters *must* be provided:"<<endl;
    for (StrVec_t::const_iterator it=_unknowns.begin();it!=_unknowns.end();++it)
      cout<<(*it)<<endl;
    cout<<endl;
  }
  return result;
}


//______________________________________________________________________________
void CommandLine::print()
{
  cout<<"------------------------------------------------------------"<<endl;
  cout<<_exe<<" options:"<<endl;
  cout<<"------------------------------------------------------------"<<endl;
  for (StrVec_t::const_iterator itvec=_ordered_options.begin();
       itvec!=_ordered_options.end();++itvec) {
    OptionMap_t::const_iterator it=_options.find(*itvec);
    assert(it!=_options.end());
    if (it->second.first.find(",")<string::npos) {
      string tmp=it->second.first;
      string::size_type length = tmp.length();
      string::size_type pos;
      do {
	pos = tmp.find(",");
	if (tmp.length()==length) {
	  cout<<setiosflags(ios::left)<<setw(22)
	      <<it->first
	      <<resetiosflags(ios::left)
	      <<setw(3)<<"="
	      <<setiosflags(ios::right)<<setw(35)
	      <<tmp.substr(0,pos)
	      <<resetiosflags(ios::right)
	      <<endl;
	}
	else {
	  cout<<setiosflags(ios::right)<<setw(60)
	      <<tmp.substr(0,pos)
	      <<resetiosflags(ios::right)
	      <<endl;
	}
	tmp.erase(0,pos+1);
      }
      while (pos!=string::npos);
    }
    else {
      cout<<setiosflags(ios::left)<<setw(22)
	  <<it->first
	  <<resetiosflags(ios::left)
	  <<setw(3)<<"="
	  <<setiosflags(ios::right)<<setw(35)
	  <<it->second.first
	  <<resetiosflags(ios::right)
	  <<endl;
    }
  }
  cout<<"------------------------------------------------------------"<<endl;
}


//______________________________________________________________________________
bool CommandLine::parse_file(const string& file_name)
{
  ifstream fin(file_name.c_str());
  if (!fin.is_open()) {
    cout<<"Can't open configuration file "<<file_name<<endl;
    return false;
  }

  stringstream ss;
  bool filter(false);
  while(!fin.eof()){
    char next;
    fin.get(next);
    if (!filter&&next=='$') filter=true;
    if(!filter) {
      if (next=='=') ss<<" "<<next<<" ";
      else ss<<next;
    }
    if (filter&&next=='\n') filter=false;
  }
  
  string token,last_token,key,value;
  ss>>token;
  while (!ss.eof()) {
    if (token=="=") {
      if (key!=""&&value!="") _options[key] = make_pair(value,false);
      key=last_token;
      last_token="";
      value="";
    }
    else if (last_token!="") {
      if (last_token.find("\"")==0) {
	if (last_token.rfind("\"")==last_token.length()-1) {
	  last_token=last_token.substr(1,last_token.length()-2);
	  value+=(value!="")?","+last_token:last_token;
	  last_token=token;
	}
	else last_token+=" "+token;
      }
      else {
	value+=(value!="")?","+last_token:last_token;
	last_token=(token=="=")?"":token;
      }
    }
    else last_token=(token=="=")?"":token;
    ss>>token;
  }
  if (last_token!="") {
    if (last_token.find("\"")==0&&last_token.rfind("\"")==last_token.length()-1)
      last_token=last_token.substr(1,last_token.length()-2);
    value+=(value!="")?","+last_token:last_token;
  }
  if (key!=""&&value!="") _options[key] = make_pair(value,false);

  return true;
}
