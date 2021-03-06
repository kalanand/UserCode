// @(#)root/tmva $Id: ConvergenceTest.cxx,v 1.4 2009-04-10 08:53:25 speckmayer Exp $
// Author: Andreas Hoecker, Peter Speckmayer, Joerg Stelzer, Helge Voss

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : ConvergenceTest                                                             *
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

#include "TMVA/ConvergenceTest.h"
#include "TMath.h"


//_______________________________________________________________________
TMVA::ConvergenceTest::ConvergenceTest() 
   : fCurrentValue( 0 ),
     fImprovement( 0 ),
     fSteps( 0 ),
     fCounter( -1 ),
     fConvValue( FLT_MAX ),
     fMaxCounter( 0 ),
     fBestResult( FLT_MAX ),
     fLastResult( FLT_MAX )
{
   // constructor
}

//_______________________________________________________________________
TMVA::ConvergenceTest::~ConvergenceTest() 
{
   // destructor
}



//_______________________________________________________________________
Bool_t TMVA::ConvergenceTest::HasConverged( Bool_t withinConvergenceBand )
{
   // gives back true if the last "steps" steps have lead to an improvement of the
   // "fitness" of the "individuals" of at least "improvement"
   // 
   // this gives a simple measure of if the estimator of the MLP is
   // converging and no major improvement is to be expected. 
   //
   if( fSteps < 0 || fImprovement < 0 ) return kFALSE;

   if (fCounter < 0) {
      fConvValue = fCurrentValue;
   }
   Float_t improvement = 0; 
   if( withinConvergenceBand )
      improvement = TMath::Abs(fCurrentValue - fConvValue);
   else
      improvement = fConvValue - fCurrentValue;
   if ( improvement <= fImprovement || fSteps<0) {
      fCounter ++;
   } else {
      fCounter = 0;
      fConvValue = fCurrentValue;
   }
   if (fCounter < fSteps) return kFALSE;
   return kTRUE;
}

//_______________________________________________________________________
Float_t TMVA::ConvergenceTest::Progress()
{
   // returns a float from 0 (just started) to 1 (finished)
   if( fCounter > fMaxCounter ) 
      fMaxCounter = fCounter; 
   return Float_t(fMaxCounter)/Float_t(fSteps);  
}


//_______________________________________________________________________
Float_t TMVA::ConvergenceTest::SpeedControl( UInt_t ofSteps )
{
   // this function provides the ability to change the learning rate according to
   // the success of the last generations. 
   // 
   // Parameters:
   //      int ofSteps :  = if OF the number of STEPS given in this variable (ofSteps) the 
   //                       rate of improvement has to be calculated
   //
   // using this function one can increase the stepSize of the mutation when we have 
   // good success (to pass fast through the easy phase-space) and reduce the learning rate
   // if we are in a difficult "territory" of the phase-space. 
   //

   // < is valid for "less" comparison (for minimiztions)
   if ( fBestResult > fLastResult || fSuccessList.size() <=0 ) { 
      fLastResult = fBestResult;
      fSuccessList.push_front( 1 ); // it got better
   } else {
      fSuccessList.push_front( 0 ); // it stayed the same
   }
   while( ofSteps <= fSuccessList.size() ) // remove the older entries in the success-list
      fSuccessList.erase( fSuccessList.begin() );
   Int_t n = 0;
   Int_t sum = 0;
   std::deque<Short_t>::iterator vec = fSuccessList.begin();
   for (; vec<fSuccessList.end() ; vec++) {
      sum += *vec;
      n++;
   }

   return sum/Float_t(n);
}
