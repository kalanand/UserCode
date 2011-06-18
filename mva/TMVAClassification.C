// @(#)root/tmva $Id: TMVAClassification.C,v 1.36 2009-04-14 13:08:13 andreas.hoecker Exp $
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
   // The explicit loading of the shared libTMVA is done in TMVAlogon.C, defined in .rootrc
   // if you use your private .rootrc, or run from a different directory, please copy the 
   // corresponding lines from .rootrc

   // methods to be processed can be given as an argument; use format:
   //
   // mylinux~> root -l TMVAClassification.C\(\"myMethod1,myMethod2,myMethod3\"\)
   //
   // if you like to use a method via the plugin mechanism, we recommend using
   // 
   // mylinux~> root -l TMVAClassification.C\(\"P_myMethod\"\)
   // (an example is given for using the BDT as plugin (see below),
   // but of course the real application is when you write your own
   // method based)

   // this loads the library
   TMVA::Tools::Instance();

   //---------------------------------------------------------------
   // default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   Use["Cuts"]            = 1;
   Use["CutsD"]           = 1;
   Use["CutsPCA"]         = 1;
   Use["CutsGA"]          = 1;
   Use["CutsSA"]          = 1;
   // ---
   Use["Likelihood"]      = 1;
   Use["LikelihoodD"]     = 1; // the "D" extension indicates decorrelated input variables (see option strings)
   Use["LikelihoodPCA"]   = 1; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
   Use["LikelihoodKDE"]   = 1;
   Use["LikelihoodMIX"]   = 1;
   // ---
   Use["PDERS"]           = 1;
   Use["PDERSD"]          = 1;
   Use["PDERSPCA"]        = 1;
   Use["PDERSkNN"]        = 1; // depreciated until further notice
   Use["PDEFoam"]         = 1;
   // --
   Use["KNN"]             = 1;
   // ---
   Use["HMatrix"]         = 1;
   Use["Fisher"]          = 1;
   Use["FisherG"]         = 1;
   Use["BoostedFisher"]   = 1;
   Use["LD"]              = 1;
   // ---
   Use["FDA_GA"]          = 1;
   Use["FDA_SA"]          = 1;
   Use["FDA_MC"]          = 1;
   Use["FDA_MT"]          = 1;
   Use["FDA_GAMT"]        = 1;
   Use["FDA_MCMT"]        = 1;
   // ---
   Use["MLP"]             = 1; // this is the recommended ANN
   Use["MLPBFGS"]         = 1; // recommended ANN with optional training method
   Use["CFMlpANN"]        = 1; // *** missing
   Use["TMlpANN"]         = 1; 
   // ---
   Use["SVM"]             = 1;
   // ---
   Use["BDT"]             = 1;
   Use["BDTD"]            = 0;
   Use["BDTG"]            = 1;
   Use["BDTB"]            = 0;
   // ---
   Use["RuleFit"]         = 1;
   // ---
   Use["Plugin"]          = 0;
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
//    factory->AddVariable( "myvar1 := var1+var2", 'F' );
//    factory->AddVariable( "myvar2 := var1-var2", "Expression 2", "", 'F' );
//    factory->AddVariable( "var3",                "Variable 3", "units", 'F' );
//    factory->AddVariable( "var4",                "Variable 4", "units", 'F' );


   factory->AddVariable("eta := probe_gsfEle_eta", 'F');
   factory->AddVariable("nPV := event_nPV", 'I');
   factory->AddVariable("deltaEta := probe_gsfEle_deltaEta", 'F');
   factory->AddVariable("EoverP := probe_gsfEle_EoverP", 'F');
   factory->AddVariable("bremFraction := probe_gsfEle_bremFraction", 'F');
   factory->AddVariable("deltaPhi := probe_gsfEle_deltaPhi", 'F');
   factory->AddVariable("sigmaIetaIeta := probe_gsfEle_sigmaIetaIeta", 'F');
   factory->AddVariable("HoverE := probe_gsfEle_HoverE", 'F');

   factory->AddVariable("trackIso := probe_gsfEle_trackiso/probe_gsfEle_et", 'F');
   factory->AddVariable("ecalIso := probe_gsfEle_ecaliso/probe_gsfEle_et", 'F');
   factory->AddVariable("hcalIso := probe_gsfEle_hcaliso/probe_gsfEle_et", 'F');
   factory->AddVariable("misHits := probe_gsfEle_missingHits", 'F');
   factory->AddVariable("dcot := probe_dcot", 'F');
   factory->AddVariable("dist := probe_dist", 'F');
   factory->AddVariable("tip := (-(probe_track_vx-event_PrimaryVertex_x)*probe_track_py + (probe_track_vy-event_PrimaryVertex_y)*probe_track_px)/probe_track_pt", 'F');



   if (!Use["Likelihood"] ) {
      factory->AddVariable("charge := probe_gsfEle_charge", 'F');
      factory->AddVariable("deltaEtaOut := probe_gsfEle_deltaEtaOut", 'F');
      factory->AddVariable("deltaPhiOut := probe_gsfEle_deltaPhiOut", 'F');
      factory->AddVariable("R15 := probe_gsfEle_e1x5/probe_gsfEle_e5x5", 'F');
      factory->AddVariable("R25 := probe_gsfEle_e2x5Max/probe_gsfEle_e5x5", 'F');
      factory->AddVariable("eSeedClusterOverP := probe_gsfEle_eSeedClusterOverP", 'F');
      factory->AddVariable("classification := probe_gsfEle_classification", 'F');
      factory->AddVariable("track_d0 := probe_track_d0", 'F');
      // factory->AddVariable("trackChi2 := probe_track_normalizedChi2", 'F');
   }


   // You can add so-called "Spectator variables", which are not used in the MVA training, 
   // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the 
   // input variables, the response values of all trained MVAs, and the spectator variables
//    factory->AddSpectator( "spec1:=var1*2",  "Spectator 1", "units", 'F' );
//    factory->AddSpectator( "spec2:=var1*3",  "Spectator 2", "units", 'F' );
   
   factory->AddSpectator("isWP60 := probe_isWP60", "O");
   factory->AddSpectator("isWP70 := probe_isWP70", "O");
   factory->AddSpectator("isWP80 := probe_isWP80", "O");
   factory->AddSpectator("isWP85 := probe_isWP85", "O");
   factory->AddSpectator("isWP90 := probe_isWP90", "O");
   factory->AddSpectator("isWP95 := probe_isWP95", "O");
   factory->AddSpectator("pfmet := event_met_pfmet", "F");
   factory->AddSpectator("eidHyperTight4 := probe_eidCicHyperTight4", "F");
   factory->AddSpectator("eidHyperTight3 := probe_eidCicHyperTight3", "F");
   factory->AddSpectator("eidHyperTight2 := probe_eidCicHyperTight2", "F");
   factory->AddSpectator("eidHyperTight1 := probe_eidCicHyperTight1", "F");
   factory->AddSpectator("eidSuperTight := probe_eidCicSuperTight", "F");
   factory->AddSpectator("eidTight := probe_eidCicTight", "F");
   factory->AddSpectator("eidMedium := probe_eidCicMedium", "F");
   factory->AddSpectator("eidLoose := probe_eidCicLoose", "F");
   factory->AddSpectator("eidVeryLoose := probe_eidCicVeryLoose", "F");
   factory->AddSpectator("eidLikelihood := probe_eidLikelihood", "F");
   factory->AddSpectator("dRjet := probe_dRjet", "F");
   factory->AddSpectator("nJets := probe_nJets", "F");
   factory->AddSpectator("et := probe_gsfEle_et", "F");
   factory->AddSpectator("phi := probe_gsfEle_phi", "F");
   factory->AddSpectator("trackerDrivenSeed := probe_gsfEle_trackerDrivenSeed", "F");
   factory->AddSpectator("d0 := probe_track_d0", "F");
   factory->AddSpectator("passingHLT := probe_passingHLT", "F");
   factory->AddSpectator("run := run", "I");
   factory->AddSpectator("lumi := lumi", "I");
   factory->AddSpectator("event := event", "I");
   factory->AddSpectator("mass", "F");
   factory->AddSpectator("isSignal := tag_isWP60 && mass>80. && mass<100. && event_met_pfmet<20.", "O");
   factory->AddSpectator("isBackground := !tag_isWP95 && mass<80.", "O");




   factory->AddSpectator("isWP60New := (abs(probe_dcot)>0.02 || abs(probe_dist)>0.02) && probe_gsfEle_missingHits==0 && ((probe_gsfEle_isEB && abs(probe_gsfEle_deltaPhi)<0.0203 && abs(probe_gsfEle_deltaEta)<0.00504 && probe_gsfEle_HoverE<0.0236 && probe_gsfEle_trackiso/probe_gsfEle_et<0.03 && probe_gsfEle_ecaliso/probe_gsfEle_et<0.0432 && probe_gsfEle_hcaliso/probe_gsfEle_et<0.0324 && probe_gsfEle_sigmaIetaIeta<0.00994) || (probe_gsfEle_isEE && abs(probe_gsfEle_deltaPhi)<0.0201 && abs(probe_gsfEle_deltaEta)<0.00376 && probe_gsfEle_HoverE<0.00973 && probe_gsfEle_trackiso/probe_gsfEle_et<0.0307 && probe_gsfEle_ecaliso/probe_gsfEle_et<0.0263 && probe_gsfEle_hcaliso/probe_gsfEle_et<0.00885 && probe_gsfEle_sigmaIetaIeta<0.0264 && abs( (-(probe_track_vx-event_PrimaryVertex_x)*probe_track_py + (probe_track_vy-event_PrimaryVertex_y)*probe_track_px)/probe_track_pt)<0.00983))", "O");
   factory->AddSpectator("isWP70New := (abs(probe_dcot)>0.02 || abs(probe_dist)>0.02) && probe_gsfEle_missingHits==0 && ((probe_gsfEle_isEB && abs(probe_gsfEle_deltaPhi)<0.0203 && abs(probe_gsfEle_deltaEta)<0.00504 && probe_gsfEle_HoverE<0.0306 && probe_gsfEle_trackiso/probe_gsfEle_et<0.0432 && probe_gsfEle_ecaliso/probe_gsfEle_et<0.0544 && probe_gsfEle_hcaliso/probe_gsfEle_et<0.0324 && probe_gsfEle_sigmaIetaIeta<0.0103) || (probe_gsfEle_isEE && abs(probe_gsfEle_deltaPhi)<0.0201 && abs(probe_gsfEle_deltaEta)<0.00587 && probe_gsfEle_HoverE<0.00973 && probe_gsfEle_trackiso/probe_gsfEle_et<0.0307 && probe_gsfEle_ecaliso/probe_gsfEle_et<0.0349 && probe_gsfEle_hcaliso/probe_gsfEle_et<0.0205 && probe_gsfEle_sigmaIetaIeta<0.0284 && abs( (-(probe_track_vx-event_PrimaryVertex_x)*probe_track_py + (probe_track_vy-event_PrimaryVertex_y)*probe_track_px)/probe_track_pt)<0.0118))", "O");
   factory->AddSpectator("isWP80New := (abs(probe_dcot)>0.02 || abs(probe_dist)>0.02) && probe_gsfEle_missingHits==0 && ((probe_gsfEle_isEB && abs(probe_gsfEle_deltaPhi)<0.0385 && abs(probe_gsfEle_deltaEta)<0.00504 && probe_gsfEle_HoverE<0.035 && probe_gsfEle_trackiso/probe_gsfEle_et<0.0535 && probe_gsfEle_ecaliso/probe_gsfEle_et<0.0793 && probe_gsfEle_hcaliso/probe_gsfEle_et<0.0521 && probe_gsfEle_sigmaIetaIeta<0.0106) || (probe_gsfEle_isEE && abs(probe_gsfEle_deltaPhi)<0.0201 && abs(probe_gsfEle_deltaEta)<0.0068 && probe_gsfEle_HoverE<0.0256 && probe_gsfEle_trackiso/probe_gsfEle_et<0.0471 && probe_gsfEle_ecaliso/probe_gsfEle_et<0.0394 && probe_gsfEle_hcaliso/probe_gsfEle_et<0.0337 && probe_gsfEle_sigmaIetaIeta<0.0284 && abs( (-(probe_track_vx-event_PrimaryVertex_x)*probe_track_py + (probe_track_vy-event_PrimaryVertex_y)*probe_track_px)/probe_track_pt)<0.0259))", "O");
   factory->AddSpectator("isWP85New := (abs(probe_dcot)>0.02 || abs(probe_dist)>0.02) && probe_gsfEle_missingHits==0 && ((probe_gsfEle_isEB && abs(probe_gsfEle_deltaPhi)<0.0593 && abs(probe_gsfEle_deltaEta)<0.00504 && probe_gsfEle_HoverE<0.0463 && probe_gsfEle_trackiso/probe_gsfEle_et<0.0686 && probe_gsfEle_ecaliso/probe_gsfEle_et<0.0825 && probe_gsfEle_hcaliso/probe_gsfEle_et<0.0521 && probe_gsfEle_sigmaIetaIeta<0.0128) || (probe_gsfEle_isEE && abs(probe_gsfEle_deltaPhi)<0.0342 && abs(probe_gsfEle_deltaEta)<0.0068 && probe_gsfEle_HoverE<0.0348 && probe_gsfEle_trackiso/probe_gsfEle_et<0.0471 && probe_gsfEle_ecaliso/probe_gsfEle_et<0.0497 && probe_gsfEle_hcaliso/probe_gsfEle_et<0.0408 && probe_gsfEle_sigmaIetaIeta<0.0296 && abs( (-(probe_track_vx-event_PrimaryVertex_x)*probe_track_py + (probe_track_vy-event_PrimaryVertex_y)*probe_track_px)/probe_track_pt)<0.045))", "O");
   factory->AddSpectator("isWP90New := (abs(probe_dcot)>0.02 || abs(probe_dist)>0.02) && probe_gsfEle_missingHits==0 && ((probe_gsfEle_isEB && abs(probe_gsfEle_deltaPhi)<0.15 && abs(probe_gsfEle_deltaEta)<0.00543 && probe_gsfEle_HoverE<0.0698 && probe_gsfEle_trackiso/probe_gsfEle_et<0.0811 && probe_gsfEle_ecaliso/probe_gsfEle_et<0.0944 && probe_gsfEle_hcaliso/probe_gsfEle_et<0.187 && probe_gsfEle_sigmaIetaIeta<0.03) || (probe_gsfEle_isEE && abs(probe_gsfEle_deltaPhi)<0.15 && abs(probe_gsfEle_deltaEta)<0.00878 && probe_gsfEle_HoverE<0.0348 && probe_gsfEle_trackiso/probe_gsfEle_et<0.0762 && probe_gsfEle_ecaliso/probe_gsfEle_et<0.0497 && probe_gsfEle_hcaliso/probe_gsfEle_et<0.0665 && probe_gsfEle_sigmaIetaIeta<0.04 && abs( (-(probe_track_vx-event_PrimaryVertex_x)*probe_track_py + (probe_track_vy-event_PrimaryVertex_y)*probe_track_px)/probe_track_pt)<0.0724))", "O");
   factory->AddSpectator("isWP95New := probe_gsfEle_missingHits<=1 && ((probe_gsfEle_isEB && abs(probe_gsfEle_deltaPhi)<0.15 && abs(probe_gsfEle_deltaEta)<0.00741 && probe_gsfEle_HoverE<0.0809 && probe_gsfEle_trackiso/probe_gsfEle_et<0.0996 && probe_gsfEle_ecaliso/probe_gsfEle_et<0.14 && probe_gsfEle_hcaliso/probe_gsfEle_et<0.183 && probe_gsfEle_sigmaIetaIeta<0.03) || (probe_gsfEle_isEE && abs(probe_gsfEle_deltaPhi)<0.15 && abs(probe_gsfEle_deltaEta)<0.0101 && probe_gsfEle_HoverE<0.0691 && probe_gsfEle_trackiso/probe_gsfEle_et<0.0751 && probe_gsfEle_ecaliso/probe_gsfEle_et<0.064 && probe_gsfEle_hcaliso/probe_gsfEle_et<0.0764 && probe_gsfEle_sigmaIetaIeta<0.04 && abs( (-(probe_track_vx-event_PrimaryVertex_x)*probe_track_py + (probe_track_vy-event_PrimaryVertex_y)*probe_track_px)/probe_track_pt)<0.0955))", "O");



   // read training and test data
    TFile *input = TFile::Open( "../allTPtrees_35invpb.root" );
 
   std::cout << "--- TMVAClassification : Using input file: " << input->GetName() << std::endl;
   
   TTree *signal     = (TTree*)input->Get("GsfElectronPlusGsfElectron/fitter_tree");
   TTree *background = (TTree*)input->Get("GsfElectronPlusGsfElectron/fitter_tree");
   

      // global event weights per tree (see below for setting event-wise weights)
      Double_t signalWeight     = 1.0;
      Double_t backgroundWeight = 1.0;

      // ====== register trees ====================================================
      //
      // the following method is the prefered one:
      // you can add an arbitrary number of signal or background trees
      factory->AddSignalTree    ( signal,     signalWeight     );
      factory->AddBackgroundTree( background, backgroundWeight );

      // To give different trees for training and testing, do as follows:
      //    factory->AddSignalTree( signalTrainingTree, signalTrainWeight, "Training" );
      //    factory->AddSignalTree( signalTestTree,     signalTestWeight,  "Test" );

      // Use the following code instead of the above two or four lines to add signal and background 
      // training and test events "by hand"
      // NOTE that in this case one should not give expressions (such as "var1+var2") in the input 
      //      variable definition, but simply compute the expression before adding the event
      // 
      //    // --- begin ----------------------------------------------------------
      //    std::vector<Double_t> vars( 4 ); // vector has size of number of input variables
      //    Float_t  treevars[4];
      //    for (Int_t ivar=0; ivar<4; ivar++) signal->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
      //    for (Int_t i=0; i<signal->GetEntries(); i++) {
      //       signal->GetEntry(i);
      //       for (Int_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
      //       // add training and test events; here: first half is training, second is testing
      //       // note that the weight can also be event-wise	
      //       if (i < signal->GetEntries()/2) factory->AddSignalTrainingEvent( vars, signalWeight ); 
      //       else                            factory->AddSignalTestEvent    ( vars, signalWeight ); 
      //    }
      //
      //    for (Int_t ivar=0; ivar<4; ivar++) background->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
      //    for (Int_t i=0; i<background->GetEntries(); i++) {
      //       background->GetEntry(i); 
      //       for (Int_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
      //       // add training and test events; here: first half is training, second is testing
      //       // note that the weight can also be event-wise	
      //       if (i < background->GetEntries()/2) factory->AddBackgroundTrainingEvent( vars, backgroundWeight ); 
      //       else                                factory->AddBackgroundTestEvent    ( vars, backgroundWeight ); 
      //    }
      //    // --- end ------------------------------------------------------------
      //
      // ====== end of register trees ==============================================

   
   // This would set individual event weights (the variables defined in the 
   // expression need to exist in the original TTree)
   //    for signal    : factory->SetSignalWeightExpression("weight1*weight2");
   //    for background: factory->SetBackgroundWeightExpression("weight1*weight2");
      // factory->SetBackgroundWeightExpression("weight");

   // Apply additional cuts on the signal and background samples (can be different)
 //   TCut mycuts = "abs(eta)>1.5"; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
//    TCut mycutb = "abs(eta)>1.5"; // for example: TCut mycutb = "abs(var1)<0.5";

  TCut mycuts = "tag_isWP60 && event_met_pfmet<20.0 && mass>80. && mass<100. && probe_gsfEle_et>20. && tag_gsfEle_et>20."; 
  TCut mycutb = "!tag_isWP95 && mass<80. && probe_gsfEle_et>20. && tag_gsfEle_et>20."; 



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

   if (Use["CutsD"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsD", 
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );

   if (Use["CutsPCA"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsPCA", 
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );

   if (Use["CutsGA"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsGA",
                           "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );
   
   if (Use["CutsSA"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsSA",
                           "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );
   
   // Likelihood
   if (Use["Likelihood"])
      factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", 
                           "H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" ); 

   // test the decorrelated likelihood
   if (Use["LikelihoodD"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodD", 
                           "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=Decorrelate" ); 

   if (Use["LikelihoodPCA"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodPCA", 
                           "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA" ); 
 
   // test the new kernel density estimator
   if (Use["LikelihoodKDE"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodKDE", 
                           "!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" ); 

   // test the mixed splines and kernel density estimator (depending on which variable)
   if (Use["LikelihoodMIX"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodMIX", 
                           "!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" ); 

   // test the multi-dimensional probability density estimator
   // here are the options strings for the MinMax and RMS methods, respectively:
   //      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );   
   //      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );   
   if (Use["PDERS"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERS", 
                           "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );

   if (Use["PDERSkNN"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERSkNN", 
                           "!H:!V:VolumeRangeMode=kNN:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );

   if (Use["PDERSD"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERSD", 
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate" );

   if (Use["PDERSPCA"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERSPCA", 
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA" );

   // Multi-dimensional likelihood estimator using self-adapting phase-space binning
   if (Use["PDEFoam"])
      factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoam", 
                           "H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0333:nActiveCells=500:nSampl=2000:nBin=5:CutNmin=T:Nmin=100:Kernel=None:Compress=T" );

   // K-Nearest Neighbour classifier (KNN)
   if (Use["KNN"])
      factory->BookMethod( TMVA::Types::kKNN, "KNN", 
                           "H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );
   // H-Matrix (chi2-squared) method
   if (Use["HMatrix"])
      factory->BookMethod( TMVA::Types::kHMatrix, "HMatrix", "!H:!V" ); 

   // Fisher discriminant   
   if (Use["Fisher"])
      factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=60:NsmoothMVAPdf=10" );

   // Fisher with Gauss-transformed input variables
   if (Use["FisherG"])
      factory->BookMethod( TMVA::Types::kFisher, "FisherG", "H:!V:VarTransform=Gauss" );

   // Composite classifier: ensemble (tree) of boosted Fisher classifiers
   if (Use["BoostedFisher"])
      factory->BookMethod( TMVA::Types::kFisher, "BoostedFisher", "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2");

   // Linear discriminant (same as Fisher)
   if (Use["LD"])
      factory->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=None" );

	// Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
   if (Use["FDA_MC"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MC",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1" );
   
   if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_GA",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );

   if (Use["FDA_SA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_SA",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

   if (Use["FDA_MT"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );

   if (Use["FDA_GAMT"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_GAMT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );

   if (Use["FDA_MCMT"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MCMT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20" );

   // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
   if (Use["MLP"])
      factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=500:HiddenLayers=N+5:TestRate=10:EpochMonitoring" );

   if (Use["MLPBFGS"])
      factory->BookMethod( TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=500:HiddenLayers=N+5:TestRate=10:TrainingMethod=BFGS:!EpochMonitoring" );


   // CF(Clermont-Ferrand)ANN
   if (Use["CFMlpANN"])
      factory->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=2000:HiddenLayers=N+1,N"  ); // n_cycles:#nodes:#nodes:...  
  
   // Tmlp(Root)ANN
   if (Use["TMlpANN"])
      factory->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); // n_cycles:#nodes:#nodes:...
  
   // Support Vector Machine
   if (Use["SVM"])
      factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );
   
   // Boosted Decision Trees
   if (Use["BDTG"]) // Gradient Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTG", 
                           "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:NNodesMax=5" );

   if (Use["BDT"])  // Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDT", 
                           "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
   
   if (Use["BDTB"]) // Bagging
      factory->BookMethod( TMVA::Types::kBDT, "BDTB", 
                           "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );

   if (Use["BDTD"]) // Decorrelation + Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTD", 
                           "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning:VarTransform=Decorrelate" );
   
   // RuleFit -- TMVA implementation of Friedman's method
   if (Use["RuleFit"])
      factory->BookMethod( TMVA::Types::kRuleFit, "RuleFit",
                           "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );
   
   // For an example of the category classifier, see: TMVAClassificationCategory

   // --------------------------------------------------------------------------------------------------

   // As an example how to use the ROOT plugin mechanism, book BDT via
   // plugin mechanism
   if (Use["Plugin"]) {
         //
         // first the plugin has to be defined, which can happen either through the following line in the local or global .rootrc:
         //
         // # plugin handler          plugin name(regexp) class to be instanciated library        constructor format
         // Plugin.TMVA@@MethodBase:  ^BDT                TMVA::MethodBDT          TMVA.1         "MethodBDT(TString,TString,DataSet&,TString)"
         // 
         // or by telling the global plugin manager directly
      gPluginMgr->AddHandler("TMVA@@MethodBase", "BDT", "TMVA::MethodBDT", "TMVA.1", "MethodBDT(TString,TString,DataSet&,TString)");
      factory->BookMethod( TMVA::Types::kPlugins, "BDT",
                           "!H:!V:NTrees=400:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=CostComplexity:PruneStrength=50" );
   }

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

   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) TMVAGui( outfileName );
}
