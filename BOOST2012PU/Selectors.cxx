
#include "Selectors.h"

#include "Event.h"

#include <algorithm>

using namespace Selectors;

EtaRange::EtaRange(double etaMin,double etaMax) : 
  _etaMin(etaMin), _etaMax(etaMax)
{ }

EtaRange::~EtaRange()
{ }

bool EtaRange::accept(const fastjet::PseudoJet& pJet)
{ return 
    pJet.pseudorapidity() > _etaMin && pJet.pseudorapidity() < _etaMax; }

void EtaRange::reset()
{ }

ChargedEvent::ChargedEvent()
{ }

ChargedEvent::~ChargedEvent()
{ }

bool ChargedEvent::accept(const fastjet::PseudoJet& pJet)
{ return Features::isCharged(pJet); }

void ChargedEvent::reset()
{ }

NeutralEvent::NeutralEvent()
{ }

NeutralEvent::~NeutralEvent()
{ }

bool NeutralEvent::accept(const fastjet::PseudoJet& pJet)
{ return !Features::isCharged(pJet); }

void NeutralEvent::reset()
{ }

PtThreshold::PtThreshold(double ptMin) : _ptMin(ptMin)
{ }

PtThreshold::~PtThreshold()
{ }

bool PtThreshold::accept(const fastjet::PseudoJet& pJet)
{ return pJet.perp() > _ptMin; }

void PtThreshold::reset()
{ }

PtRange::PtRange(double ptMin,double ptMax) : _ptMin(ptMin), _ptMax(ptMax)
{ }

PtRange::~PtRange()
{ }

bool PtRange::accept(const fastjet::PseudoJet& pJet)
{ return pJet.perp() > _ptMin && pJet.perp() < _ptMax; }

void PtRange::reset()
{ }

Handler::Handler() : _selectors(0)
{ }

Handler::~Handler()
{ }

bool Handler::accept(const fastjet::PseudoJet& pJet)
{
  std::vector<IFinalStateSelector*>::iterator fSel(_selectors.begin());
  std::vector<IFinalStateSelector*>::iterator lSel(_selectors.end());
  while ( fSel != lSel && (*fSel)->accept(pJet) ) { ++fSel; }
  return fSel == lSel;
}

void Handler::reset()
{
  std::vector<IFinalStateSelector*>::iterator fSel(_selectors.begin());
  std::vector<IFinalStateSelector*>::iterator lSel(_selectors.end());
  for ( ; fSel != lSel; ++fSel ) { (*fSel)->reset(); }
}

void Handler::registerSelector(IFinalStateSelector& rSel)
{ this->registerSelector(&rSel); }

void Handler::registerSelector(IFinalStateSelector* pSel)
{
  // already registered
  if ( std::find(_selectors.begin(),_selectors.end(),pSel) !=
       _selectors.end() ) return;
  _selectors.push_back(pSel);
}
