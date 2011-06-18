// @(#)root/tmva $Id: TMVAClassification.C 31458 2009-11-30 13:58:20Z stelzer $
/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Root Macro: TMVAClassification                                                 *
 *                                                                                *
 * This macro provides examples for the training and testing of the               *
 * TMVA classifiers.                                                              *
 *                                                                                *
 * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
 * and linearly correlated input variables.                                       *
 *                                                                                *
 * The methods to be used can be switched on and off by means of booleans, or     *
 * via the prompt command, for example:                                           *
 *                                                                                *
 *    root -l TMVAClassification.C\(\"Fisher,Likelihood\"\)                       *
 *                                                                                *
 * (note that the backslashes are mandatory)                                      *
 * If no method given, a default set is used.                                     *
 *                                                                                *
 * The output file "TMVA.root" can be analysed with the use of dedicated          *
 * macros (simply say: root -l <macro.C>), which can be conveniently              *
 * invoked through a GUI that will appear at the end of the run of this macro.    *
 **********************************************************************************/

#include <cstdlib>
#include <iostream> 
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"

#include "tmvaglob.C"
#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

// read input data file with ascii format (otherwise ROOT) ?
Bool_t ReadDataFromAsciiIFormat = kFALSE;
   
void TMVAClassification( TString myMethodList = "" ) 
{

//    TString curDynamicPath( gSystem->GetDynamicPath() );
//    gSystem->SetDynamicPath( "/usr/local/bin/root/bin:" + curDynamicPath );

//    TString curIncludePath(gSystem->GetIncludePath());
//    gSystem->SetIncludePath( " -I /usr/local/bin/root/include " + curIncludePath );

//    // load TMVA shared library created in local release: for MAC OSX
//    if (TString(gSystem->GetBuildArch()).Contains("macosx") ) gSystem->Load( "libTMVA.so" );


   // gSystem->Load( "libTMVA" );
//   TMVA::Tools::Instance();

//    // welcome the user
//    TMVA::gTools().TMVAWelcomeMessage();
   
//    TMVAGlob::SetTMVAStyle();

//    // this loads the library
//    TMVA::Tools::Instance();

   //---------------------------------------------------------------
   // default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   Use["Cuts"]            = 1;
   // Use["Likelihood"]      = 1;
 
   // ---------------------------------------------------------------

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification" << std::endl;

   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // Create a new root output file.
   TString outfileName( "TMVA.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   // Create the factory object. Later you can choose the methods
   // whose performance you'd like to investigate. The factory will
   // then run the performance analysis for you.
   //
   // The first argument is the base of the name of all the
   // weightfiles in the directory weight/ 
   //
   // The second argument is the output file for the training results
   // All TMVA output can be suppressed by removing the "!" (not) in 
   // front of the "Silent" argument in the option string
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile, 
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D" );

   // If you wish to modify default settings 
   // (please check "src/Config.h" to see all available global options)
   //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
   //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";

   // Define the input variables that shall be used for the MVA training
   // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
   // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
   // factory->AddVariable( "myvar1 := var1+var2", 'F' );
   // factory->AddVariable( "myvar2 := var1-var2", "Expression 2", "", 'F' );
   // factory->AddVariable( "var3",                "Variable 3", "units", 'F' );
   // factory->AddVariable( "var4",                "Variable 4", "units", 'F' );

   factory->AddVariable("deltaEta := deta", 'F');
   factory->AddVariable("deltaPhi := dphi", 'F');
   factory->AddVariable("sigmaIetaIeta := sieie", 'F');
   factory->AddVariable("HoverE := hoe", 'F');
   factory->AddVariable("trackIso := trackiso", 'F');
   factory->AddVariable("ecalIso := ecaliso", 'F');
   factory->AddVariable("hcalIso := hcaliso", 'F');
   //factory->AddVariable("nMissingHits := misshits", 'I');


   // You can add so-called "Spectator variables", which are not used in the MVA training, 
   // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the 
   // input variables, the response values of all trained MVAs, and the spectator variables
   factory->AddSpectator( "et",  'F' );
   factory->AddSpectator( "eta",  'F' );
   factory->AddSpectator( "phi",  'F' );


   // read training and test data
    TFile *input = TFile::Open( "SigElectrons.root" );
    TFile *inputB = TFile::Open( "BkgElectrons.root" );

   std::cout << "--- TMVAClassification : Using input file: " << input->GetName() << std::endl;
   
   TTree *signal     = (TTree*)input->Get("ntuple");
   TTree *background = (TTree*)inputB->Get("ntuple");
   
   factory->AddSignalTree    ( signal,     1.0 );
   factory->AddBackgroundTree( background, 1.0 );

   
   // This would set individual event weights (the variables defined in the 
   // expression need to exist in the original TTree)
   //    for signal    : factory->SetSignalWeightExpression("weight1*weight2");
   //    for background: factory->SetBackgroundWeightExpression("weight1*weight2");
   //factory->SetBackgroundWeightExpression("weight");

   // Apply additional cuts on the signal and background samples (can be different)

   TCut mycuts = ""; 
   TCut mycutb = ""; 



   // tell the factory to use all remaining events in the trees after training for testing:
   factory->PrepareTrainingAndTestTree( mycuts, mycutb,
                                        "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

   // If no numbers of events are given, half of the events in the tree are used for training, and 
   // the other half for testing:
   //    factory->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );  
   // To also specify the number of testing events, use:
   //    factory->PrepareTrainingAndTestTree( mycut, 
   //                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );  

   // ---- Book MVA methods
   //
   // please lookup the various method configuration options in the corresponding cxx files, eg:
   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

   // Cut optimisation
   if (Use["Cuts"])
      factory->BookMethod( TMVA::Types::kCuts, "Cuts", 
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );
   
   // Likelihood
   if (Use["Likelihood"])
      factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", 
                           "H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" ); 



   // --------------------------------------------------------------------------------------------------

   // ---- Now you can tell the factory to train, test, and evaluate the MVAs

   // Train MVAs using the set of training events
    factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
    factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();    

   // --------------------------------------------------------------
   
   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;      

   delete factory;

//   gROOT->ProcessLine(".x /usr/local/bin/root/tmva/test/correlations.C");
   gROOT->ProcessLine(".x /usr/local/bin/root/tmva/test/variables.C");
}
