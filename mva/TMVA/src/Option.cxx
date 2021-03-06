// @(#)root/tmva $Id: Option.cxx,v 1.20 2009-01-26 22:47:51 andreas.hoecker Exp $   
// Author: Andreas Hoecker, Joerg Stelzer, Helge Voss

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : Option                                                                *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Implementation                                                            *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Andreas Hoecker <Andreas.Hocker@cern.ch> - CERN, Switzerland              *
 *      Joerg Stelzer   <Joerg.Stelzer@cern.ch>  - CERN, Switzerland              *
 *      Helge Voss      <Helge.Voss@cern.ch>     - MPI-K Heidelberg, Germany      *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland                                                         * 
 *      U. of Victoria, Canada                                                    * 
 *      MPI-K Heidelberg, Germany                                                 * 
 *      LAPP, Annecy, France                                                      *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://mva.sourceforge.net/license.txt)                                       *
 **********************************************************************************/

#include "TMVA/Option.h"

//______________________________________________________________________
TMVA::OptionBase::OptionBase( const TString& name, const TString& desc ) 
   : TObject(), 
     fName        ( name ), 
     fNameAllLower( name ), 
     fDescription ( desc ), 
     fIsSet       ( kFALSE ), 
     fLogger      ("OptionBase") 
{
   // constructor
   fNameAllLower.ToLower();
}

//______________________________________________________________________
Bool_t TMVA::OptionBase::SetValue( const TString& vs, Int_t ) 
{
   // set value for option
   fIsSet = kTRUE;
   SetValueLocal(vs);
   return kTRUE;
}

