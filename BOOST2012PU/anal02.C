//#include "MB_Py8.h"

#include "TChain.h"

#include "MB_Py8.h"
#include "Zprime_Py8.h"

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
  std::string   fileList_pu;
  std::string   fileList_signal;
  std::string   outFile;
  long long     nevents;
  bool          mainNameTag;
  bool          muTag;
  bool          fileList_puTag;
  bool          fileList_signalTag;
  bool          outFileTag;
  bool          neventsTag;
  bool          PUsub;

  configData() 
    : mainName("main"),mu(1),fileList_pu("NONE"),fileList_signal("NONE"),
      outFile("test.root"),nevents(-1),
      mainNameTag(false),muTag(false),fileList_puTag(false),
      fileList_signalTag(false),outFileTag(false),
      neventsTag(false),PUsub(false)
  {}
};

void interpretConfig(int argc,char** argv,configData& config)
{
  std::string mn(argv[0]);
  config.mainName    = mn.substr(mn.find_last_of("/")+1);
  config.mainNameTag = true;

  std::vector<std::string> argList;
  for ( int i=1;i<argc;++i ) { argList.push_back(std::string(argv[i])); }

  // check arguments
  std::vector<std::string>::iterator fStr(argList.begin());
  std::vector<std::string>::iterator lStr(argList.end());
  for ( ; fStr != lStr; ++fStr )
    {
      // number of interactions
      if ( fStr->find("--mu") != std::string::npos )
	{ 
	  config.mu = atoi((fStr->substr(fStr->find("=")+1)).c_str()); 
	  config.muTag = true; 
	}
      else if ( fStr->find("--puflist=") != std::string::npos )
	{ config.fileList_pu = fStr->substr(fStr->find("=")+1); 
	  config.fileList_puTag = true; }
      else if ( fStr->find("--output=") != std::string::npos )
	{ config.outFile = fStr->substr(fStr->find("=")+1); 
	  config.outFileTag = true; }
      else if ( fStr->find("--nevts=") != std::string::npos )
	{ config.nevents = atoi((fStr->substr(fStr->find("=")+1)).c_str()); 
	  config.neventsTag = true; }
      else if ( fStr->find("--sigflist=") != std::string::npos )
	{ config.fileList_signal = fStr->substr(fStr->find("=")+1);
	  config.fileList_signalTag = true; }
      else if ( fStr->find("--PUsub=") != std::string::npos )
	{ config.PUsub = atoi((fStr->substr(fStr->find("=")+1)).c_str()); }
      else if ( fStr->find("--help") != std::string::npos )
	{ printf("[%s] - INFO - usage:\n      %s --mu=<# interactions> --puflist=<list of pileup files> -- sigflist=<list of signal files> --netvs=<# of (signal) events to be analyzed>\n",config.mainName.c_str(),
		 argv[0]); }
      else 
	{ printf("[%s::interpretConfig(%i,...)] - WARNING - unknown configuration \042%s\042\n",config.mainName.c_str(),argc,fStr->c_str()); }
    }
}

void printConfig(configData& config)
{
  printf("[%s] - INFO - this configuration:\n",config.mainName.c_str());
  printf("[%s] - INFO - average number of interactions <mu> ... %i",
	 config.mainName.c_str(),config.mu);
  if ( config.muTag ) { printf("\n"); } else { printf(" [default]\n"); }
  printf("[%s] - INFO - PU file list .......................... \042%s\042",
	 config.mainName.c_str(),config.fileList_pu.c_str());
  if ( config.fileList_puTag ) { printf("\n"); } else { printf(" [default]\n"); }
  printf("[%s] - INFO - signal file list ...................... \042%s\042",
	 config.mainName.c_str(),config.fileList_signal.c_str());
  if ( config.fileList_signalTag ) { printf("\n"); } else { printf(" [default]\n"); }
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

TChain* fillChain(const std::string& chainName,const std::string& fList)
{
  TChain* chain = new TChain(chainName.c_str());
  std::string fname;
  std::ifstream inStream;
  std::vector<std::string> lofFiles;
  inStream.open(fList.c_str());
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
  printf("[fillChain(%s)] - INFO - found %i files with %i events total\n",
	 chainName.c_str(),(int)lofFiles.size(),chain->GetEntries());
  return chain; 
}

int main(int argc,char** argv)
{
  // check on arguments
  configData config;
  interpretConfig(argc,argv,config);
  printConfig(config);

  bool haveSignal(config.fileList_signal != "NONE");
  bool havePU(config.fileList_pu != "NONE");

  if ( !haveSignal ) { printf("[%s] - WARNING - no signal file list given.\n",config.mainName.c_str()); }

  if ( !havePU ) { printf("[%s] - WARNING - no input pile-up file list given\n",config.mainName.c_str()); }
  //
  if ( !haveSignal && !havePU ) { printf("[%s] - WARNING - no input file lists given at all, exit!\n",config.mainName.c_str()); return 0; }
 
  TChain* pileupChain = 0;
   MB_Py8* pPileUp     = 0;
  if ( havePU ) { pileupChain = fillChain("MB_Py8",config.fileList_pu); pPileUp = new MB_Py8(config.mu,pileupChain); }

  TChain* signalChain = 0;
  Zprime_Py8* pSignal = 0;
  if ( haveSignal ) { signalChain = fillChain("Zprime_Py8",config.fileList_signal); pSignal = new Zprime_Py8(signalChain); }
  printf("[%s] signal handler at %p\n",config.mainName.c_str(),(void*)pSignal);

  // analyse only signal
  if ( haveSignal && !havePU ) { pSignal->Loop(config.nevents, config.outFile,config.PUsub); }

  // analyse only pile up
  if ( havePU && !haveSignal ) { pPileUp->Loop(config.mu,config.nevents,config.outFile,config.PUsub); }

  // analysis mixed events
  if ( havePU && haveSignal ) 
    { pSignal->Loop(*pPileUp,config.mu,config.nevents,config.outFile,
		    "",config.PUsub); }

  return 0;
};
