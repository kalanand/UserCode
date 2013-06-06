// -*- c++ -*-
#ifndef SELECTORS_H
#define SELECTORS_H

#include "IFinalStateSelector.h"

#include <vector>
#include <limits>

namespace Selectors
{
  class EtaRange : virtual public IFinalStateSelector
  {
  public:
    EtaRange(double etaMin=-6.,double etaMax=6.);
    virtual ~EtaRange();

    virtual bool accept(const fastjet::PseudoJet& pJet);
    virtual void reset();
  
  private:
    
    double _etaMin;
    double _etaMax;
  };

  class ChargedEvent : virtual public IFinalStateSelector
  {
  public:
    ChargedEvent();
    virtual ~ChargedEvent();

    virtual bool accept(const fastjet::PseudoJet& pJet);
    virtual void reset();
  };

  class NeutralEvent : virtual public IFinalStateSelector
  {
  public:
    NeutralEvent();
    virtual ~NeutralEvent();

    virtual bool accept(const fastjet::PseudoJet& pJet);
    virtual void reset();
  };

  class PtThreshold : virtual public IFinalStateSelector
  {
  public:
    PtThreshold(double ptMin=0.);
    virtual ~PtThreshold();

    virtual bool accept(const fastjet::PseudoJet& pJet);
    virtual void reset();

  private:

    double _ptMin;
  };

  class PtRange : virtual public IFinalStateSelector
  {
  public:
    PtRange(double ptMin=0.,double ptMax=std::numeric_limits<double>::max());
    virtual ~PtRange();

    virtual bool accept(const fastjet::PseudoJet& pJet);
    virtual void reset();

  private:

    double _ptMin;
    double _ptMax;
  };

  class Handler : virtual public IFinalStateSelector
  {
  public:
    Handler();
    virtual ~Handler();

    virtual bool accept(const fastjet::PseudoJet& pJet);
    virtual void reset();

    virtual void registerSelector(IFinalStateSelector* pSel); 
    virtual void registerSelector(IFinalStateSelector& rSel);

  protected:

    //    virtual void _register(IFinalStateSelector* pSel);

  private:

    std::vector<IFinalStateSelector*> _selectors;
  };
}
#endif
