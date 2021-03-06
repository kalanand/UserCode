// @(#)root/tmva $Id: Results.cxx,v 1.9 2009-03-22 10:15:32 stelzer Exp $
// Author: Andreas Hoecker, Peter Speckmayer, Joerg Stelzer, Helge Voss

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : Results                                                             *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Implementation (see header for description)                               *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Andreas Hoecker <Andreas.Hocker@cern.ch> - CERN, Switzerland              *
 *      Peter Speckmayer <Peter.Speckmayer@cern.ch>  - CERN, Switzerland          *
 *      Joerg Stelzer   <Joerg.Stelzer@cern.ch>  - CERN, Switzerland              *
 *      Helge Voss      <Helge.Voss@cern.ch>     - MPI-K Heidelberg, Germany      *
 *                                                                                *
 * Copyright (c) 2006:                                                            *
 *      CERN, Switzerland                                                         *
 *      MPI-K Heidelberg, Germany                                                 *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#include <vector>

#include "TH1.h"

#include "TMVA/Results.h"
#include "TMVA/MsgLogger.h"

//_______________________________________________________________________
TMVA::Results::Results( const DataSetInfo* dsi ) 
   : fTreeType(Types::kTraining),
     fDsi(dsi),
     fStorage( new TList() ),
     fHistAlias( new std::map<TString, TObject*> ),
     fLogger( new MsgLogger("Results", kINFO) )
{
   // constructor
   fStorage->SetOwner();
}

//_______________________________________________________________________
TMVA::Results::~Results() 
{
   // destructor

   // delete result-histograms
   delete fStorage;
   delete fHistAlias;
   delete fLogger;
}

//_______________________________________________________________________
void TMVA::Results::Store( TObject* obj, const char* alias )
{
   TListIter l(fStorage);
   // check if object is already in list
   while (void* p = (void*)l()) {
      if(p==obj)
         *fLogger << kFATAL << "Histogram pointer " << p << " already exists in results storage" << Endl;
   }

   TString as(obj->GetName());
   if (alias!=0) as=TString(alias);
   if (fHistAlias->find(as) != fHistAlias->end()) {
      // alias exists
      *fLogger << kFATAL << "Alias " << as << " already exists in results storage" << Endl;
   }
   if( obj->InheritsFrom("TH1") ) {
      ((TH1*)obj)->SetDirectory(0);
   }
   fStorage->Add( obj );
   fHistAlias->insert(std::pair<TString, TObject*>(as,obj));
}

//_______________________________________________________________________
TObject* TMVA::Results::GetObject(const TString & alias) const 
{
   std::map<TString, TObject*>::iterator it = fHistAlias->find(alias);

   if (it != fHistAlias->end()) return it->second;

   // alias does not exist
   return 0;
}


//_______________________________________________________________________
TH1* TMVA::Results::GetHist(const TString & alias) const 
{
   return (TH1*)GetObject(alias);
}
