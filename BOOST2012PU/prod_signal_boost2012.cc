// main01.cc is a part of the PYTHIA event generator.
// Copyright (C) 2008 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program. It fits on one slide in a talk. 
// It studies the charged multiplicity distribution at the LHC.

#include <stdio.h>
#include <stdlib.h>

#include <sstream>
#include <iostream>
#include <iomanip>

#include <sys/times.h>
#include <ctime>

#include <vector> 
#include <string>
#include <map>

#include "Pythia.h"

#include "TTree.h"
#include "TFile.h"
#include "TRandom1.h"

using namespace Pythia8;
using namespace std;

#ifdef _SINGLE_PRECISION_
#  define _FLOAT_T_ float
#else
#  define _FLOAT_T_ float
#endif

#ifndef _AUTHOR_
#define _AUTHOR_ "David W. Miller"
#endif

#ifndef _EMAIL_
#define _EMAIL_ "<David.W.Miller_at_uchicago.edu>"
#endif

//----------------------------------------------------------------------
/// a function that pretty prints a list of jets

int main(int argc, char* argv[]) 
{

  // timing
  tms* startTime = new tms();
  clock_t sT = times(startTime);

  // module name
  string moduleName("SignalProcessor");

  // messages
#ifdef _SINGLE_PRECISION_
  printf("\n\n[%s] - INFO - Single precision version compiled on %s at %s <%s>\n",
	 moduleName.c_str(),__DATE__,__TIME__,__FILE__);
#else
  printf("\n\n[%s] - INFO - Double precision version compiled on %s at %s <%s>\n",
	 moduleName.c_str(),__DATE__,__TIME__,__FILE__);
#endif

  printf("\n[%s] - INFO - Author %s %s\n\n",moduleName.c_str(),_AUTHOR_,_EMAIL_);

  printf("[%s] - INFO - BeginJob (user time/system time) (%10i,%10i) - return (%10i)\n", moduleName.c_str(), (int)startTime->tms_utime, (int)startTime->tms_stime, (int)sT);


  // Check that correct number of command-line arguments
  if (argc != 3) {
    cerr << " Unexpected number of command-line arguments. \n You are"
         << " expected to provide one input and one output file name. \n"
         << " Program stopped! " << endl;
    return 1;
  }

  // Check that the provided input name corresponds to an existing file.
  ifstream is(argv[1]);  
  if (!is) {
    cerr << " Command-line file " << argv[1] << " was not found. \n"
         << " Program stopped! " << endl;
    return 1;
  }

  // Confirm that external files will be used for input and output.
  cout << "\n >>> PYTHIA settings will be read from file " << argv[1] 
       << " <<< \n >>> HepMC events will be written to file " 
       << argv[2] << " <<< \n" << endl;

  // Set number of entries per branch
  printf("[%s] - INFO - Initializing Branches \n", moduleName.c_str() );
  int    mPart(100000); 

  // Initialize files
  printf("[%s] - INFO - Initializing I/O \n", moduleName.c_str() );
  
  TFile* rFile((TFile*)0);
  TTree* rTree((TTree*)0);
 
  // ROOT tuple structure
  int Nentry;
  int Npartons;
  int Nparticles;
  int ID[mPart];
  int Stat[mPart];

  _FLOAT_T_ Charge[mPart];
  _FLOAT_T_ Px[mPart];
  _FLOAT_T_ Py[mPart];
  _FLOAT_T_ Pz[mPart];
  _FLOAT_T_ P0[mPart];
  _FLOAT_T_ Pm[mPart];
  _FLOAT_T_ Pt[mPart];
  _FLOAT_T_ Rap[mPart];
  _FLOAT_T_ Phi[mPart];
  _FLOAT_T_ Eta[mPart];
 
  // processing directives
  int    nFile(1);
 
  // Generator. Process selection. LHC initialization. Histogram.
  printf("[%s] - INFO - Initializing Pythia \n", moduleName.c_str() );
  Pythia pythia;
 
  // Read in commands 
  printf("[%s] - INFO - Reading Pythia Options \n", moduleName.c_str() );
  
  // from external file.
  //pythia.readFile(argv[1]);    
  
  // manually
  pythia.readString("Beams:eCM = 8000.");    
  pythia.readString("Main:numberOfEvents = 100000");    
  pythia.readString("Main:timesAllowErrors = 10");  
  pythia.readString("Main:spareWord1 = signal_Zprime_");
  pythia.readString("Main:timesAllowErrors = 10");
  //pythia.readString("Random:setSeed = on");
  pythia.readString("Init:showAllSettings = off");
  pythia.readString("Init:showChangedParticleData = on");
  pythia.readString("Init:showAllParticleData = off");
  pythia.readString("NewGaugeBoson:ffbar2gmZZprime = on");    
  pythia.readString("Zprime:gmZmode= 3");
  pythia.readString("32:m0 = 1500");
  pythia.readString("32:onMode = off");
  pythia.readString("32:onIfAny = 6 -6");

  // Extract settings to be used in the main program.
  int    nEvent    = pythia.mode("Main:numberOfEvents");
  int    nAbort    = pythia.mode("Main:timesAllowErrors");
  double CMenergy  = pythia.parm("Beams:eCM");
  string fName     = pythia.word("Main:spareWord1"); 
 
  printf("[%s] - INFO - number of events/file updated %i\n", moduleName.c_str(), nEvent);
  printf("[%s] - INFO - center-of-mass energy updated %7.1f GeV\n", moduleName.c_str(), CMenergy); 
  printf("[%s] - INFO - Output ROOT file prefix: %s\n", moduleName.c_str(), fName.c_str()); 
 
  /// Set the random number seed
  time_t seconds      = time(0);             // time since epoch
  TRandom1* randomEng = new TRandom1((unsigned int)seconds);
  int pySeed = (int)(randomEng->Rndm() * 900000000);
  if ( pySeed == 0 )        pySeed == 1;
  if ( pySeed > 900000000 ) pySeed = 900000000;
 
  ostringstream ostr;
  ostr << "Random:seed = " << pySeed;
  pythia.readString(ostr.str());

  printf("[%s] - INFO - configured random number generator with seed %i\n", moduleName.c_str(), pySeed);

  pythia.init();

  // Begin event loop. Generate event. Skip if error. List first one.
  int totEvts(nEvent*nFile);
  int iFile(0);
  printf("[%s] - INFO - process %i events in %i files (total events %i)\n", moduleName.c_str(), nEvent, nFile, totEvts);

  int iEvt(0);
  while (iEvt < totEvts)
  {
    if ( !pythia.next() ) continue;
    _FLOAT_T_ pthat(pythia.info.pTHat());
    if ( iEvt < 1 ) 
    {
      pythia.info.list(); 
      pythia.event.list();
    }   
   
   
    /// On the first and last events, create and write the output files
    if ( iEvt%nEvent == 0 ) 
    {
      if (iEvt!=0)
      {
        printf("[%s] - INFO - write tuple after %i events\n", moduleName.c_str(),iEvt);
        rTree->Write();
        printf("[%s] - INFO - close file #%i\n", moduleName.c_str(),iFile);
        rFile->Close();
        ++iFile;
      }
     
      string fN = fName;
      ostringstream xStr;
      xStr << fName << setfill('0') << setw(5) << iFile << ".root";
      printf("[%s] - INFO - opening file #%i \042%s\042\n", moduleName.c_str(),iFile+1,xStr.str().c_str());
      rFile = new TFile(xStr.str().c_str(),"RECREATE");
      rTree = new TTree("Zprime_Py8","Zprime to ttbar ROOT tree", 1);
      printf("[%s] - INFO - setting branches in tree %s\n", moduleName.c_str(),rTree->GetName());
      rTree->Branch ("Nentry",    &Nentry,    "Nentry/I");
      rTree->Branch ("Npartons",  &Npartons,  "Npartons/I");
      rTree->Branch ("Nparticles",&Nparticles,"Nparticles/I");
      rTree->Branch ("ID",        ID,         "ID[Nentry]/I");
      rTree->Branch ("Stat",      Stat,       "Stat[Nentry]/I");

 #ifdef _SINGLE_PRECISION_
      rTree->Branch ("Charge",    Charge,     "Charge[Nentry]/F");
      rTree->Branch ("Px",        Px,         "Px[Nentry]/F");
      rTree->Branch ("Py",        Py,         "Py[Nentry]/F");
      rTree->Branch ("Pz",        Pz,         "Pz[Nentry]/F");
      rTree->Branch ("P0",        P0,         "P0[Nentry]/F");
      rTree->Branch ("Pm",        Pm,         "Pm[Nentry]/F");
      rTree->Branch ("Pt",        Pt,         "Pt[Nentry]/F");       
      rTree->Branch ("Rap",       Rap,        "Rap[Nentry]/F");       
      rTree->Branch ("Phi",       Phi,        "Phi[Nentry]/F");       
      rTree->Branch ("Eta",       Eta,        "Eta[Nentry]/F");  
 #else
      rTree->Branch ("Charge",    Charge,     "Charge[Nentry]/D");
      rTree->Branch ("Px",        Px,         "Px[Nentry]/D");
      rTree->Branch ("Py",        Py,         "Py[Nentry]/D");
      rTree->Branch ("Pz",        Pz,         "Pz[Nentry]/D");
      rTree->Branch ("P0",        P0,         "P0[Nentry]/D");
      rTree->Branch ("Pm",        Pm,         "Pm[Nentry]/D");
      rTree->Branch ("Pt",        Pt,         "Pt[Nentry]/D");       
      rTree->Branch ("Rap",       Rap,        "Rap[Nentry]/D");       
      rTree->Branch ("Phi",       Phi,        "Phi[Nentry]/D");       
      rTree->Branch ("Eta",       Eta,        "Eta[Nentry]/D");  
 #endif

    }
  
    // loop partonic event
    Npartons   = 0;
    for (int i=0;i<pythia.event.size();i++)
    {
      if ( pythia.event[i].status() == -21 || pythia.event[i].status() == -22 || pythia.event[i].status() == -23 )
      {
        // status word
        if ( pythia.event[i].status() == -21 )      { Stat[Npartons] = -2; }
        else if ( pythia.event[i].status() == -23 ) { Stat[Npartons] = -1; }
        else if ( pythia.event[i].status() == -22 ) { Stat[Npartons] = -2; }
         
        // tuple content
        ID[Npartons]     = pythia.event[i].id();
        Charge[Npartons] = pythia.event[i].charge();
        Px[Npartons]     = pythia.event[i].px();
        Py[Npartons]     = pythia.event[i].py();
        Pz[Npartons]     = pythia.event[i].pz();
        P0[Npartons]     = pythia.event[i].e();
        Pm[Npartons]     = pythia.event[i].m();
        Pt[Npartons]     = pythia.event[i].pT();
        Rap[Npartons]    = pythia.event[i].y();
        Phi[Npartons]    = pythia.event[i].phi();
        Eta[Npartons]    = pythia.event[i].eta();
        Npartons++;
      }
    }
     
    // loop final state particles
    Nparticles = 0;
    for (int i=0;i<pythia.event.size();i++)
    {	
      if (pythia.event[i].isFinal())
      {
        Stat[Npartons+Nparticles]   = 2;
        ID[Npartons+Nparticles]     = pythia.event[i].id();
        Charge[Npartons+Nparticles] = pythia.event[i].charge();
        Px[Npartons+Nparticles]     = pythia.event[i].px();
        Py[Npartons+Nparticles]     = pythia.event[i].py();
        Pz[Npartons+Nparticles]     = pythia.event[i].pz();
        P0[Npartons+Nparticles]     = pythia.event[i].e();
        Pm[Npartons+Nparticles]     = pythia.event[i].m();
        Pt[Npartons+Nparticles]     = pythia.event[i].pT();
        Rap[Npartons+Nparticles]    = pythia.event[i].y();
        Phi[Npartons+Nparticles]    = pythia.event[i].phi();
        Eta[Npartons+Nparticles]    = pythia.event[i].eta();
        Nparticles++;
      }
    }
     
    Nentry=Npartons+Nparticles;
   
    if ( iEvt%1000 == 0 ) { printf("[%s] - INFO - number of events processed %9i\n", moduleName.c_str(),iEvt+1);}
     
    rTree->Fill();
    ++iEvt;
  }

  pythia.statistics();

  rTree->Write();
  rFile->Close();

  // timing
  tms* endTime = new tms();
  clock_t eT = times(endTime);
  printf("[%s] - INFO - EndJob (user time/system time) (%10i,%10i) - return (%10i)\n", moduleName.c_str(), (int)endTime->tms_utime, (int)endTime->tms_stime, (int)eT);
  printf("[%s] - INFO - EndJob differences (user time/system time) (%10i,%10i) - return (%10i)\n", moduleName.c_str(), (int)(endTime->tms_utime - startTime->tms_utime), (int)(endTime->tms_stime - startTime->tms_stime), (int)(eT-sT));
 
  return 1;
}
