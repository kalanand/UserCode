//STARTHEADER
// $Id$
//
// Copyright (c) 2005-2011, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
//
//----------------------------------------------------------------------
// This file is part of FastJet.
//
//  FastJet is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  The algorithms that underlie FastJet have required considerable
//  development and are described in hep-ph/0512210. If you use
//  FastJet as part of work towards a scientific publication, please
//  include a citation to the FastJet paper.
//
//  FastJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------
//ENDHEADER

#include "SubtractorWithMass.hh"
#include <cassert>
#include <sstream>
#include <limits>
using namespace std;

FASTJET_BEGIN_NAMESPACE     // defined in fastjet/internal/base.hh

PseudoJet SubtractorWithMass::result(const PseudoJet & jet) const {
  if (!jet.has_area()){
    throw Error("Trying to subtract a jet without area support");
  }

  if (_bge==0){
    throw Error("default Subtractor does not have any information about the background, which is needed to perform the subtraction");
  }

  PseudoJet subtracted_jet = jet;
  PseudoJet area4vect = jet.area_4vector();

  if (_mass_only){
    BackgroundJetPtMDensity m_density;
    const FunctionOfPseudoJet<double> * p_density = _bge->jet_density_class();
    _bge->set_jet_density_class(&m_density);
    double rho_mt = _bge->rho(jet);
    _bge->set_jet_density_class(p_density);
    PseudoJet jet_sub = jet;
    subtracted_jet -= rho_mt*PseudoJet(0.0,0.0,area4vect.pz(),area4vect.E());
    return subtracted_jet;
  } 

  double rho_pt = _bge->rho(jet);
  if (rho_pt*rho_pt*area4vect.pt2()>jet.pt2()){
    subtracted_jet *= 0.0;
  } else {
    BackgroundJetPtMDensity m_density;
    const FunctionOfPseudoJet<double> * p_density = _bge->jet_density_class();
    _bge->set_jet_density_class(&m_density);
    double rho_mt = _bge->rho(jet);
    _bge->set_jet_density_class(p_density);
    PseudoJet jet_sub = jet;

    // make sure that the extra piece does not bring the mass negative
    // The new 4-vector will have
    //    sx = px - Ax r        = kx
    //    sy = py - Ay r        = ky
    //    sz = pz - Az (r+x rm) = kz - x rm Az
    //    sE = pE - AE (r+x rm) = kE - x rm AE
    // with k the result of the "rho" (pt) subtraction.
    // The new mass (squared) is
    //    sE^2 - sz^2 - sy^2 - sx^2
    //       = mk^2 - 2 x rm (AE kE - Az kz) + x^2 rm^2 (AE^2 - Az^2)
    // 
    // If the original vector has a -ve mass, we leave it untouched
    // Otherwise, if the mass is +ve, the x at which it shall be 0 is
    //    x rm = [(AE kE - Az kz) - sqrt((AE kE - Az kz)^2 - mk^2 (AE^2 - Az^2))]/(AE^2 - Az^2)
    // If (x rm < rm) we subtract x rm otherwise, we subtract rm 
    // A few simplifications can be made:
    //  - AE^2 - Az^2 = Amt^2 (transverse mass of the area 4-vector)
    //  - AE ke - Az kz = kmt Amt cosh(yA - yk) (more time-consuming but more precise)
    // This gives
    //    x rm Amt = kmt ch(yA-yk) - sqrt(kmt^2 ch(yA-yk)^2 - mk^2)  (*)
    // If we further use 
    //    mk^2 = kmt^2 - kt^2
    // we have
    //    x rm Amt = kmt ch(yA-yk) - sqrt(kmt^2 sh(yA-yk)^2 - kt^2)
    // In practice, we shall use (*)
    subtracted_jet -= rho_pt*area4vect;

    if (_allow_negative_mass){
      subtracted_jet -= rho_mt*PseudoJet(0.0,0.0,area4vect.pz(),area4vect.E());
    } else {
      if (subtracted_jet.m2()<0.0) return subtracted_jet;
    
      double ch   = cosh(area4vect.rap() - subtracted_jet.rap());
      double kmt2 = subtracted_jet.mt2();
      double kmt  = sqrt(kmt2);
      double mk2  = subtracted_jet.m2();
      double Amt  = area4vect.mt();
      double rhs = kmt*ch - sqrt(kmt2*ch*ch - mk2);
      if (rho_mt*Amt > rhs){
        // subtraction would have given a -ve mass
        double rho_mt_max = rhs/Amt;
        subtracted_jet -= rho_mt_max*PseudoJet(0.0,0.0,area4vect.pz(),area4vect.E());
      } else {
        subtracted_jet -= rho_mt*PseudoJet(0.0,0.0,area4vect.pz(),area4vect.E());
      }
    }
  }

  return subtracted_jet;
}

//----------------------------------------------------------------------
std::string SubtractorWithMass::description() const{
  if (_bge != 0) {
    return "Subtractor that uses the following background estimator to determine rho: "+_bge->description()+" and corrects for the particle mass";
  } else {
    return "Uninitialised subtractor";
  }
}

FASTJET_END_NAMESPACE
