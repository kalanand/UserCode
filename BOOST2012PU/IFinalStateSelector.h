// -*- c++ -*-
#ifndef IFINALSTATESELECTOR_H
#define IFINALSTATESELECTOR_H

#include "fastjet/PseudoJet.hh"

class IFinalStateSelector
{
public:
  virtual ~IFinalStateSelector() { }

  virtual bool accept(const fastjet::PseudoJet& pJet) = 0;
  virtual void reset() = 0;
};
#endif
