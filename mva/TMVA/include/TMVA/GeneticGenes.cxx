// @(#)root/tmva $Id: GeneticGenes.cxx,v 1.19 2009-03-06 08:36:17 andreas.hoecker Exp $    
// Author: Peter Speckmayer

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : TMVA::GeneticGenes                                                    *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Implementation (see header for description)                               *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Peter Speckmayer <speckmay@mail.cern.ch>  - CERN, Switzerland             *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland                                                         *
 *      MPI-K Heidelberg, Germany                                                 *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#include "TMVA/GeneticGenes.h"
#include "TMVA/GeneticPopulation.h"

//_______________________________________________________________________
//                                                                      
// Cut optimisation interface class for genetic algorithm               //
//_______________________________________________________________________

ClassImp(TMVA::GeneticGenes)
   
//_______________________________________________________________________
TMVA::GeneticGenes::GeneticGenes( std::vector<Double_t> & f  ) 
{
   // Constructor:
   // set the factors of this individual
   fFactors = f;
   fFitness = 0;
}

Bool_t TMVA::operator <(const TMVA::GeneticGenes& first, const TMVA::GeneticGenes& second)
{
   return first.fFitness < second.fFitness;
}
