//#include "MB_Py8.h"

#include "TChain.h"

#include "MB_Py8.h"

#include <cstdio>
#include <cstdlib>

#include <string>
#include <fstream>

#include <vector>

#include <algorithm>

struct configData
{
  std::string   mainName;
  int           mu;
  std::string   fileList;
  std::string   outFile;
  long long     nevents;
  bool          mainNameTag;
  bool          muTag;
  bool          fileListTag;
  bool          outFileTag;
  bool          neventsTag;
  bool          PUsub;

  configData() 
    : mainName("main"),mu(1),fileList("NONE"),outFile("test.root"),nevents(-1),
      mainNameTag(false),muTag(false),fileListTag(false),outFileTag(false),
      neventsTag(false),PUsub(false)
  {}
};

void interpretConfig(int argc,char** argv,configData& config)
{
  std::string mn(argv[0]);
  config.mainName = mn.substr(mn.find_last_of("/")+1);
  config.mainNameTag = true;

  std::vector<std::string> argList;
  for ( int i=1;i<argc;++i ) { argList.push_back(std::string(argv[i])); }
  // check argument
  std::vector<std::string>::iterator fStr(argList.begin());
  std::vector<std::string>::iterator lStr(argList.end());
  for ( ; fStr != lStr; ++fStr )
    {
      if ( fStr->find("--mu=") != std::string::npos )
	{ config.mu = atoi((fStr->substr(fStr->find("=")+1)).c_str()); 
	  config.muTag = true; }
      else if ( fStr->find("--flist=") != std::string::npos )
	{ config.fileList = fStr->substr(fStr->find("=")+1); 
	  config.fileListTag = true; }
      else if ( fStr->find("--output=") != std::string::npos )
	{ config.outFile = fStr->substr(fStr->find("=")+1); 
	  config.outFileTag = true; }
      else if ( fStr->find("--nevts=") != std::string::npos )
	{ config.nevents = atoi((fStr->substr(fStr->find("=")+1)).c_str()); 
	  config.neventsTag = true; }
      else if ( fStr->find("--PUsub=") != std::string::npos )
	{ config.PUsub = atoi((fStr->substr(fStr->find("=")+1)).c_str()); }
      else if ( fStr->find("--help") != std::string::npos )
	{ printf("[%s] - INFO - usage: %s --mu=<# interactions> --flist=<list of input files> --netvs=<# of events to be analyzed>\n",config.mainName.c_str(),
		 argv[0]); }
      else 
	{ printf("[%s::interpretConfig(%i,...)] - WARNING - unknown configuration \042%s\042\n",config.mainName.c_str(),argc,fStr->c_str()); }
    }
};

void printConfig(configData& config)
{
  printf("[%s] - INFO - this configuration:\n",config.mainName.c_str());
  printf("[%s] - INFO - average number of interactions <mu> ... %i",
	 config.mainName.c_str(),config.mu);
  if ( config.muTag ) { printf("\n"); } else { printf(" [default]\n"); }
  printf("[%s] - INFO - file list ............................. \042%s\042",
	 config.mainName.c_str(),config.fileList.c_str());
  if ( config.fileListTag ) { printf("\n"); } else { printf(" [default]\n"); }
  printf("[%s] - INFO - output ROOT file name ............................. \042%s\042",
	 config.mainName.c_str(),config.outFile.c_str());
  if ( config.outFileTag ) { printf("\n"); } else { printf(" [default]\n"); }
  printf("[%s] - INFO - Pileup subtraction ............................. \042%s\042\n",
	 config.mainName.c_str(),config.PUsub ? "true" : "false");
  if ( config.nevents > 0 )
    { printf("[%s] - INFO - number of events to be analyzed ....... %i",
	     config.mainName.c_str(),(int)config.nevents); }
  else
    { printf("[%s] - INFO - number of events to be analyzed ....... <all>",
	     config.mainName.c_str()); }
  if ( config.neventsTag ) { printf("\n"); } else { printf(" [default]\n"); }

};

int main(int argc,char** argv)
{
  // check on arguments
  configData config;
  interpretConfig(argc,argv,config);
  printConfig(config);

  if ( config.fileList == "NONE" )
    { printf("[%s] - WARNING - no input file list given, exit!\n",
	     config.mainName.c_str()); 
      return 0; }
  //
  TChain* chain = new TChain("MB_Py8");
  std::string fname;
  std::ifstream inStream;
  std::vector<std::string> lofFiles;
  inStream.open(config.fileList.c_str());
  do
    {
      inStream >> fname;
      if ( std::find(lofFiles.begin(),lofFiles.end(),fname) ==
           lofFiles.end() )
        {
	  // printf("[%s] - INFO - found filename %s\n",argv[0],fname.c_str());
	  if ( !fname.empty() ) chain->Add(fname.c_str(),-1);
          lofFiles.push_back(fname);
        }
    }
  while ( !inStream.eof() );
  printf("[%s] - INFO - found %i files with %i events total\n",
	 config.mainName.c_str(),(int)lofFiles.size(),chain->GetEntries());


  // analysis module
  MB_Py8* pData = new MB_Py8(config.mu,chain);

  pData->Loop(config.mu,config.nevents,config.outFile,config.PUsub);

  return 0;
};
