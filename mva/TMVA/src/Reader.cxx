// @(#)root/tmva $Id: Reader.cxx,v 1.102 2009-04-03 21:16:51 andreas.hoecker Exp $
// Author: Andreas Hoecker, Joerg Stelzer, Helge Voss

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : Reader                                                                *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Reader class to be used in the user application to interpret the trained  *
 *      MVAs in an analysis context                                               *
 *                                                                                *
 * Authors (alphabetical order):                                                  *
 *      Andreas Hoecker <Andreas.Hocker@cern.ch> - CERN, Switzerland              *
 *      Peter Speckmayer <peter.speckmayer@cern.ch> - CERN, Switzerland           *
 *      Joerg Stelzer <Joerg.Stelzer@cern.ch>    - CERN, Switzerland              *
 *      Helge Voss      <Helge.Voss@cern.ch>     - MPI-K Heidelberg, Germany      *
 *      Kai Voss        <Kai.Voss@cern.ch>       - U. of Victoria, Canada         *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland                                                         *
 *      U. of Victoria, Canada                                                    *
 *      MPI-K Heidelberg, Germany                                                 *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://ttmva.sourceforge.net/LICENSE)                                         *
 **********************************************************************************/

//_______________________________________________________________________
//
//  The Reader class serves to use the MVAs in a specific analysis context.
//  Within an event loop, a vector is filled that corresponds to the variables
//  that were used to train the MVA(s) during the training stage. This vector
//  is transfered to the Reader, who takes care of interpreting the weight
//  file of the MVA of choice, and to return the MVA's output. This is then
//  used by the user for further analysis.
//
//  ---------------------------------------------------------------------
//  Usage:
//
//    // ------ before starting the event loop (eg, in the initialisation step)
//
//    //
//    // create TMVA::Reader object
//    //
//    TMVA::Reader *reader = new TMVA::Reader();
//
//    // create a set of variables and declare them to the reader
//    // - the variable names must corresponds in name and type to
//    // those given in the weight file(s) that you use
//    Float_t var1, var2, var3, var4;
//    reader->AddVariable( "var1", &var1 );
//    reader->AddVariable( "var2", &var2 );
//    reader->AddVariable( "var3", &var3 );
//    reader->AddVariable( "var4", &var4 );
//
//    // book the MVA of your choice (prior training of these methods, ie,
//    // existence of the weight files is required)
//    reader->BookMVA( "Fisher method",  "weights/Fisher.weights.txt"   );
//    reader->BookMVA( "MLP method",     "weights/MLP.weights.txt" );
//    // ... etc
//
//    // ------- start your event loop
//
//    for (Long64_t ievt=0; ievt<myTree->GetEntries();ievt++) {
//
//      // fill vector with values of variables computed from those in the tree
//      var1 = myvar1;
//      var2 = myvar2;
//      var3 = myvar3;
//      var4 = myvar4;
//
//      // retrieve the corresponding MVA output
//      double mvaFi = reader->EvaluateMVA( "Fisher method" );
//      double mvaNN = reader->EvaluateMVA( "MLP method"    );
//
//      // do something with these ...., e.g., fill them into your ntuple
//
//    } // end of event loop
//
//    delete reader;
//  ---------------------------------------------------------------------
//
//  An example application of the Reader can be found in TMVA/macros/TMVApplication.C.
//_______________________________________________________________________

#include "TMVA/Reader.h"

#include "TTree.h"
#include "TLeaf.h"
#include "TString.h"
#include "TClass.h"
#include "TH1D.h"
#include "TKey.h"
#include "TVector.h"
#include "TXMLEngine.h"
#include <cstdlib>

#include <string>
#include <vector>
#include <fstream>

#include <iostream>
#ifndef ROOT_TMVA_Tools
#include "TMVA/Tools.h"
#endif
#include "TMVA/Config.h"
#include "TMVA/ClassifierFactory.h"
#include "TMVA/IMethod.h"
#include "TMVA/MethodCuts.h"

ClassImp(TMVA::Reader)

//_______________________________________________________________________
TMVA::Reader::Reader( const TString& theOption, Bool_t verbose )
   : Configurable( theOption ),
     fDataSetInfo(),
     fVerbose( verbose ),
     fSilent ( kFALSE ),
     fColor  ( kFALSE ),
     fMvaEventError( -1 ),
     fLogger ( new MsgLogger(this) )
{
   // constructor

   DataSetManager::CreateInstance(fDataInputHandler);
   DataSetManager::Instance().AddDataSetInfo(fDataSetInfo);

   SetConfigName( GetName() );
   DeclareOptions();
   ParseOptions();

   Init();
}

//_______________________________________________________________________
TMVA::Reader::Reader( std::vector<TString>& inputVars, const TString& theOption, Bool_t verbose )
   : Configurable( theOption ),
     fDataSetInfo(),
     fVerbose( verbose ),
     fSilent ( kFALSE ),
     fColor  ( kFALSE ),
     fMvaEventError( -1 ),
     fLogger ( new MsgLogger(this) )
{
   // constructor
   SetConfigName( GetName() );
   DeclareOptions();
   ParseOptions();

   // arguments: names of input variables (vector)
   //            verbose flag
   for (std::vector<TString>::iterator ivar = inputVars.begin(); ivar != inputVars.end(); ivar++) {
      DataInfo().AddVariable( *ivar );
   }

   Init();
}

//_______________________________________________________________________
TMVA::Reader::Reader( std::vector<std::string>& inputVars, const TString& theOption, Bool_t verbose )
   : Configurable( theOption ),
     fDataSetInfo(),
     fVerbose( verbose ),
     fSilent ( kFALSE ),
     fColor  ( kFALSE ),
     fMvaEventError( -1 ),
     fLogger ( new MsgLogger(this) )
{
   // constructor
   SetConfigName( GetName() );   
   DeclareOptions();
   ParseOptions();

   // arguments: names of input variables (vector)
   //            verbose flag
   for (std::vector<std::string>::iterator ivar = inputVars.begin(); ivar != inputVars.end(); ivar++) {
      DataInfo().AddVariable( ivar->c_str() );
   }

   Init();
}

//_______________________________________________________________________
TMVA::Reader::Reader( const std::string& varNames, const TString& theOption, Bool_t verbose )
   : Configurable( theOption ),
     fDataSetInfo(),
     fVerbose( verbose ),
     fSilent ( kFALSE ),
     fColor  ( kFALSE ),
     fMvaEventError( -1 ),
     fLogger ( new MsgLogger(this) )
{
   // constructor
   SetConfigName( GetName() );
   DeclareOptions();
   ParseOptions();

   // arguments: names of input variables given in form: "name1:name2:name3"
   //            verbose flag
   DecodeVarNames(varNames);
   Init();
}

//_______________________________________________________________________
TMVA::Reader::Reader( const TString& varNames, const TString& theOption, Bool_t verbose )
   : Configurable( theOption ),
     fDataSetInfo(),
     fVerbose( verbose ),
     fSilent ( kFALSE ),
     fColor  ( kFALSE ),
     fMvaEventError( -1 ),
     fLogger ( new MsgLogger(this) )
{
   // constructor
   SetConfigName( GetName() );
   DeclareOptions();
   ParseOptions();

   // arguments: names of input variables given in form: "name1:name2:name3"
   //            verbose flag
   DecodeVarNames(varNames);
   Init();
}

//_______________________________________________________________________
void TMVA::Reader::DeclareOptions()
{
   // declaration of configuration options
   if (gTools().CheckForSilentOption( GetOptions() )) Log().InhibitOutput(); // make sure is silent if wanted to

   DeclareOptionRef( fVerbose, "V",      "Verbose flag" );
   DeclareOptionRef( fColor,   "Color",  "Color flag (default True)" );
   DeclareOptionRef( fSilent,  "Silent", "Boolean silent flag (default False)" );   
}

//_______________________________________________________________________
TMVA::Reader::~Reader( void )
{
   // destructor
   delete fLogger;
}

//_______________________________________________________________________
void TMVA::Reader::Init( void )
{
   // default initialisation (no member variables)
   // default initialisation (no member variables)
   if (Verbose()) fLogger->SetMinType( kVERBOSE );
   
   gConfig().SetUseColor( fColor );
   gConfig().SetSilent  ( fSilent );
}

//_______________________________________________________________________
void TMVA::Reader::AddVariable( const TString& expression, Float_t* datalink )
{
   // Add a float variable or expression to the reader
   DataInfo().AddVariable( expression, "", "", 0, 0, 'F', kFALSE ,(void*)datalink ); // <= should this be F or rather T?
}

//_______________________________________________________________________
void TMVA::Reader::AddVariable( const TString& expression, Int_t* datalink )
{
   // Add an integer variable or expression to the reader
   DataInfo().AddVariable(expression, "", "", 0, 0, 'I', kFALSE, (void*)datalink ); // <= should this be F or rather T?
}

//_______________________________________________________________________
void TMVA::Reader::AddSpectator( const TString& expression, Float_t* datalink )
{
   // Add a float spectator or expression to the reader
   DataInfo().AddSpectator( expression, "", "", 0, 0, 'F', kFALSE ,(void*)datalink ); 
}

//_______________________________________________________________________
void TMVA::Reader::AddSpectator( const TString& expression, Int_t* datalink )
{
   // Add an integer spectator or expression to the reader
   DataInfo().AddSpectator(expression, "", "", 0, 0, 'I', kFALSE, (void*)datalink );
}

//_______________________________________________________________________
TString
TMVA::Reader::GetMethodTypeFromFile( const TString& filename ) {
   // read the method type from the file

   ifstream fin( filename );
   if (!fin.good()) { // file not found --> Error
      Log() << kFATAL << "<BookMVA> fatal error: "
            << "unable to open input weight file: " << filename << Endl;
   }

   TString fullMethodName("");
   if (filename.EndsWith(".xml")) {
      fin.close();
      void* doc      = gTools().xmlengine().ParseFile(filename); 
      void* rootnode = gTools().xmlengine().DocGetRootElement(doc); // node "MethodSetup"
      gTools().ReadAttr(rootnode, "Method", fullMethodName);
   } else {
      char buf[512];
      fin.getline(buf,512);
      while (!TString(buf).BeginsWith("Method")) fin.getline(buf,512);
      fullMethodName = TString(buf);
      fin.close();
   }
   TString methodType = fullMethodName(0,fullMethodName.Index("::"));
   if (methodType.Contains(" ")) methodType = methodType(methodType.Last(' ')+1,methodType.Length());
   return methodType;
}



//_______________________________________________________________________
TMVA::IMethod* TMVA::Reader::BookMVA( const TString& methodTag, const TString& weightfile )
{
   // read method name from weight file

   // assert non-existence
   if (fMethodMap.find( methodTag ) != fMethodMap.end())
      Log() << kFATAL << "<BookMVA> method tag \"" << methodTag << "\" already exists!" << Endl;

   TString methodType(GetMethodTypeFromFile(weightfile));

   Log() << kINFO << "Booking \"" << methodTag << "\" of type \"" << methodType << "\" from " << weightfile << "." << Endl;

   MethodBase* method = dynamic_cast<MethodBase*>(this->BookMVA( Types::Instance().GetMethodType(methodType),
                                                                 weightfile ) );

   return fMethodMap[methodTag] = method;
}

//_______________________________________________________________________
TMVA::IMethod* TMVA::Reader::BookMVA( TMVA::Types::EMVA methodType, const TString& weightfile )
{
   // books MVA method from weightfile
   IMethod* im = ClassifierFactory::Instance().Create(std::string(Types::Instance().GetMethodName( methodType )),
                                                      DataInfo(), weightfile );
   
   MethodBase *method = (dynamic_cast<MethodBase*>(im));

   method->SetupMethod();

   // when reading older weight files, they could include options
   // that are not supported any longer
   method->DeclareCompatibilityOptions();

   // read weight file
   method->ReadStateFromFile();

   // check for unused options
   method->CheckSetup();
   
   Log() << kINFO << "Booked classifier \"" << method->GetMethodName()
         << "\" of type: \"" << method->GetMethodTypeName() << "\"" << Endl;
   
   return method;
}

//_______________________________________________________________________
Double_t TMVA::Reader::EvaluateMVA( const std::vector<Float_t>& /*inputVec*/, const TString& methodTag, Double_t aux )
{
   // Evaluate a vector<float> of input data for a given method
   // The parameter aux is obligatory for the cuts method where it represents the efficiency cutoff

   return EvaluateMVA( methodTag, aux );
}

//_______________________________________________________________________
Double_t TMVA::Reader::EvaluateMVA( const std::vector<Double_t>& /*inputVec*/, const TString& methodTag, Double_t aux )
{
   // Evaluate a vector<double> of input data for a given method
   // The parameter aux is obligatory for the cuts method where it represents the efficiency cutoff

   return EvaluateMVA( methodTag, aux );
}

//_______________________________________________________________________
Double_t TMVA::Reader::EvaluateMVA( const TString& methodTag, Double_t aux )
{
   // evaluates MVA for given set of input variables
   IMethod* method = 0;

   std::map<TString, IMethod*>::iterator it = fMethodMap.find( methodTag );
   if (it == fMethodMap.end()) {
      Log() << kINFO << "<EvaluateMVA> unknown classifier in map; "
              << "you looked for \"" << methodTag << "\" within available methods: " << Endl;
      for (it = fMethodMap.begin(); it!=fMethodMap.end(); it++) Log() << " --> " << it->first << Endl;
      Log() << "Check calling string" << kFATAL << Endl;
   }

   else method = it->second;

   return this->EvaluateMVA( dynamic_cast<TMVA::MethodBase*>(method), aux );
}

//_______________________________________________________________________
Double_t TMVA::Reader::EvaluateMVA( MethodBase* method, Double_t aux )
{
   // evaluates the MVA

   // the aux value is only needed for MethodCuts: it sets the required signal efficiency
   if (method->GetMethodType() == TMVA::Types::kCuts)
      dynamic_cast<TMVA::MethodCuts*>(method)->SetTestSignalEfficiency( aux );

   return method->GetMvaValue( &fMvaEventError ); // attributed MVA response and error
}

//_______________________________________________________________________
const std::vector< Float_t >& TMVA::Reader::EvaluateRegression( const TString& methodTag, Double_t aux )
{
   // evaluates MVA for given set of input variables
   IMethod* method = 0;

   std::map<TString, IMethod*>::iterator it = fMethodMap.find( methodTag );
   if (it == fMethodMap.end()) {
      Log() << kINFO << "<EvaluateMVA> unknown method in map; "
              << "you looked for \"" << methodTag << "\" within available methods: " << Endl;
      for (it = fMethodMap.begin(); it!=fMethodMap.end(); it++) Log() << " --> " << it->first << Endl;
      Log() << "Check calling string" << kFATAL << Endl;
   }

   else method = it->second;
   return this->EvaluateRegression( dynamic_cast<TMVA::MethodBase*>(method), aux );
}

//_______________________________________________________________________
const std::vector< Float_t >& TMVA::Reader::EvaluateRegression( MethodBase* method, Double_t /*aux*/ )
{
   // evaluates the regression MVA
   return method->GetRegressionValues();
}


//_______________________________________________________________________
Float_t TMVA::Reader::EvaluateRegression( UInt_t tgtNumber, const TString& methodTag, Double_t aux )
{ 
   // evaluates the regression MVA
   try {
      return EvaluateRegression(methodTag, aux).at(tgtNumber); 
   }
   catch (std::out_of_range e) {
      Log() << kWARNING << "Regression could not be evaluated for target-number " << tgtNumber << Endl;
      return 0;
   }
}

//_______________________________________________________________________
TMVA::IMethod* TMVA::Reader::FindMVA( const TString& methodTag )
{
   // return pointer to method with tag "methodTag"
   std::map<TString, IMethod*>::iterator it = fMethodMap.find( methodTag );
   if (it != fMethodMap.end()) return it->second;
   Log() << kERROR << "Method " << methodTag << " not found!" << Endl;
   return 0;
}

//_______________________________________________________________________
TMVA::MethodCuts* TMVA::Reader::FindCutsMVA( const TString& methodTag )
{
   // special function for Cuts to avoid dynamic_casts in ROOT macros, 
   // which are not properly handled by CINT
   return dynamic_cast<MethodCuts*>(FindMVA(methodTag));
}

//_______________________________________________________________________
Double_t TMVA::Reader::GetProba( const TString& methodTag,  Double_t ap_sig, Double_t mvaVal )
{
   // evaluates probability of MVA for given set of input variables
   IMethod* method = 0;
   std::map<TString, IMethod*>::iterator it = fMethodMap.find( methodTag );
   if (it == fMethodMap.end()) {
      for (it = fMethodMap.begin(); it!=fMethodMap.end(); it++) Log() << "M" << it->first << Endl;
      Log() << kFATAL << "<EvaluateMVA> unknown classifier in map: " << method << "; "
              << "you looked for " << methodTag<< " while the available methods are : " << Endl;
   }
   else method = it->second;

   MethodBase* kl = dynamic_cast<MethodBase*>(method);
   if (mvaVal == -9999999) mvaVal = kl->GetMvaValue();

   return kl->GetProba( mvaVal, ap_sig );
}

//_______________________________________________________________________
Double_t TMVA::Reader::GetRarity( const TString& methodTag, Double_t mvaVal )
{
   // evaluates the MVA's rarity
   IMethod* method = 0;
   std::map<TString, IMethod*>::iterator it = fMethodMap.find( methodTag );
   if (it == fMethodMap.end()) {
      for (it = fMethodMap.begin(); it!=fMethodMap.end(); it++) Log() << "M" << it->first << Endl;
      Log() << kFATAL << "<EvaluateMVA> unknown classifier in map: \"" << method << "\"; "
              << "you looked for \"" << methodTag<< "\" while the available methods are : " << Endl;
   }
   else method = it->second;

   MethodBase* kl = dynamic_cast<MethodBase*>(method);
   if (mvaVal == -9999999) mvaVal = kl->GetMvaValue();

   return kl->GetRarity( mvaVal );
}

// ---------------------------------------------------------------------------------------
// ----- methods related to the decoding of the input variable names ---------------------
// ---------------------------------------------------------------------------------------

//_______________________________________________________________________
void TMVA::Reader::DecodeVarNames( const std::string& varNames )
{
   // decodes "name1:name2:..." form
   size_t ipos = 0, f = 0;
   while (f != varNames.length()) {
      f = varNames.find( ':', ipos );
      if (f > varNames.length()) f = varNames.length();
      std::string subs = varNames.substr( ipos, f-ipos ); ipos = f+1;
      DataInfo().AddVariable( subs.c_str() );
   }
}

//_______________________________________________________________________
void TMVA::Reader::DecodeVarNames( const TString& varNames )
{
   // decodes "name1:name2:..." form

   TString format;
   Int_t   n = varNames.Length();
   TString format_obj;

   for (int i=0; i< n+1 ; i++) {
      format.Append(varNames(i));
      if (varNames(i) == ':' || i == n) {
         format.Chop();
         format_obj = format;
         format_obj.ReplaceAll("@","");
         DataInfo().AddVariable( format_obj );
         format.Resize(0);
      }
   }
}
