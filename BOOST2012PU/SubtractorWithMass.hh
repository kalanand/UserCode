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

#ifndef __FASTJET_TOOLS_SUBTRACTOR_WITH_MASS_HH__
#define __FASTJET_TOOLS_SUBTRACTOR_WITH_MASS_HH__

#include "fastjet/tools/Transformer.hh" // to derive Subtractor from Transformer
#include "fastjet/tools/JetMedianBackgroundEstimator.hh" // used as a ctor argument

FASTJET_BEGIN_NAMESPACE     // defined in fastjet/internal/base.hh


//----------------------------------------------------------------------
/// @ingroup tools_background
/// \class SubtractorWithMass
/// Class that helps perform jet background subtraction. Compared to
/// the simpler Subtractor, this also corrects for the particle masses.
///
/// This class derives from Transformer and makes use of a pointer to
/// a JetMedianBackgroundEstimator object in order to determine the
/// background in the vicinity of a given jet and then subtract
/// area*background from the jet. The background estimation must
/// support density classes which is why we require it to be of the
/// type JetMedianBackgroundEstimator.
///
/// \section input Input conditions
/// 
/// The original jet must have area support (4-vector)
///
/// \section output Output/interface
/// 
/// The underlying structure of the returned, subtracted jet
/// (i.e. constituents, pieces, etc.) is identical to that of the
/// original jet.
///
class SubtractorWithMass : public Transformer{
public:
  /// define a subtractor based on a BackgroundEstimator
  ///  \param bge        the rho estimator 
  ///  \param mass_only  when true, only apply the extra correction due
  ///                    to the mass
  SubtractorWithMass(JetMedianBackgroundEstimator * bge, 
                     bool mass_only=false, bool allow_negative_mass=false)
    : _bge(bge), _mass_only(mass_only), _allow_negative_mass(allow_negative_mass){}

  /// default constructor
  SubtractorWithMass() : _bge(0), _mass_only(false){}

  /// default dtor
  virtual ~SubtractorWithMass(){};

  /// returns a jet that's subtracted
  ///
  /// \param jet    the jet that is to be subtracted
  /// \return       the subtracted jet
  virtual PseudoJet result(const PseudoJet & jet) const;

  /// class description
  virtual std::string description() const;

protected:

  /// the tool used to estimate the background
  /// if has to be mutable in case its underlying selector takes a reference jet
  mutable JetMedianBackgroundEstimator * _bge;

  /// if true, only correct for the mass pf the particles. This could
  /// be useful if the 4-vector is already corrected for the
  /// "standard" 4-vector area term
  bool _mass_only;

  /// if true, the rho_m A_m term will be subtraqcted blindly without
  /// any check that the resulting 4-vector has a positive mass
  /// squared. Allowing -ve m2 coulb be helpful for comparisons with
  /// "shape subtraction"
  bool _allow_negative_mass;
};

FASTJET_END_NAMESPACE

#endif  // __FASTJET_TOOLS_SUBTRACTOR_WITH_MASS_HH__

