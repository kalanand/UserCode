/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: TMVAClassificationCategoryApplication                              *
 *                                                                                *
 * This macro provides a simple example on how to use the trained classifiers     *
 * within an analysis module                                                      *
 **********************************************************************************/

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"
#include "TStopwatch.h"

#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#include "TMVA/MethodCuts.h"

// two types of category methods are implemented
Bool_t UseOffsetMethod = kTRUE;

int main( int argc, char** argv ) 
{
   //---------------------------------------------------------------
   // default MVA methods to be trained + tested
   std::map<std::string,int> Use;
   // ---
   Use["FisherCat"]     = 1;
   Use["LikelihoodCat"] = 1;
   // ---------------------------------------------------------------

   std::cout << std::endl
             << "==> Start TMVAClassificationCategoryApplication" << std::endl;

   if (argc>1)
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

   for (int i=1; i<argc; i++) {
      std::string regMethod(argv[i]);
      if (Use.find(regMethod) == Use.end()) {
         std::cout << "Method " << regMethod << " not known in TMVA under this name. Please try one of:" << std::endl;
         for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
         std::cout << std::endl;
         return 1;
      }
      Use[regMethod] = kTRUE;
   }

   std::cout << "Running the following methods" << std::endl;
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) 
      if(it->second) std::cout << it->first << " ";
   std::cout << std::endl;

   //
   // create the Reader object
   //
   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    

   // create a set of variables and spectators and declare them to the reader
   // - the variable names must corresponds in name and type to 
   // those given in the weight file(s) that you use
   Float_t var1, var2, var3, var4, eta;
   reader->AddVariable( "var1", &var1 );
   reader->AddVariable( "var2", &var2 );
   reader->AddVariable( "var3", &var3 );
   reader->AddVariable( "var4", &var4 );

   reader->AddSpectator( "eta", &eta );

   //
   // book the MVA methods
   //
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = it->first + " method";
         TString weightfile = "weights/TMVAClassificationCategory_" + TString(it->first) + ".weights.xml";
         reader->BookMVA( methodName, weightfile ); 
      }
   }

   //
   // book output histograms
   //
   UInt_t nbin = 100;
   std::map<std::string,TH1F*> hist;
   if (Use["LikelihoodCat"]) hist["LikelihoodCat"] = new TH1F( "MVA_LikelihoodCat",   "MVA_LikelihoodCat", nbin, -1, 0.9999 );
   if (Use["FisherCat"])     hist["FisherCat"]     = new TH1F( "MVA_FisherCat",       "MVA_FisherCat",     nbin, -4, 4 );

   // Prepare input tree (this must be replaced by your data source)
   // in this example, there is a toy tree with signal and one with background events
   // we'll later on use only the "signal" events for the test in this example.
   //
   TString fname( "" );
   if (UseOffsetMethod) fname = "data/toy_sigbkg_categ_offset.root";
   else                 fname = "data/toy_sigbkg_categ_varoff.root";
   std::cout << "--- TMVAClassificationApp    : Accessing " << fname << "!" << std::endl;
   TFile *input = TFile::Open(fname);
   if (!input) {
      std::cout << "ERROR: could not open data file: " << fname << std::endl;
      exit(1);
   }

   // prepare the tree
   // - here the variable names have to corresponds to your tree
   // - you can use the same variables as above which is slightly faster,
   //   but of course you can use different ones and copy the values inside the event loop
   //
   TTree* theTree = (TTree*)input->Get("TreeS");
   std::cout << "--- Use signal sample for evalution" << std::endl;
   theTree->SetBranchAddress( "var1", &var1 );
   theTree->SetBranchAddress( "var2", &var2 );
   theTree->SetBranchAddress( "var3", &var3 );
   theTree->SetBranchAddress( "var4", &var4 );

   theTree->SetBranchAddress( "eta",  &eta );


   std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
   TStopwatch sw;
   sw.Start();
   for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {

      if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;

      theTree->GetEntry(ievt);

      // 
      // evaluate the methods and fill to histograms
      // 
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
         if (!it->second) continue;
         TString methodName = it->first + " method";
         hist[it->first]->Fill( reader->EvaluateMVA( methodName ) );
      }

   }
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();

   //
   // write histograms
   //
   TFile *target  = new TFile( "TMVApp.root","RECREATE" );
   for (std::map<std::string,TH1F*>::iterator it = hist.begin(); it != hist.end(); it++)
      if(it->second) it->second->Write();
   target->Close();
   std::cout << "--- Created root file: \"TMVApp.root\" containing the MVA output histograms" << std::endl;

   delete reader;
   std::cout << "==> TMVAClassificationApplication is done!" << std::endl << std::endl;
}
