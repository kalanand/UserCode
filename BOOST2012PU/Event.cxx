
#include "Event.h"

#include <cstdio>

#include <algorithm> 

Vertex Event::_dummyVertex(-1);

const Vertex& Event::dummyVtx() { return _dummyVertex; }

////////////
// Vertex //
////////////

Vertex::Vertex(OrderContext oc) 
  : m_id(-1)
  , m_ptSum(0.)
  , m_ptSum2(0.)
  , m_ptSumCharged(0.)
  , m_ptSum2Charged(0.)
  , m_nPart(0)
  , m_nPartCharged(0)
  , m_orderContext(oc)
{ }

Vertex::Vertex(int vtx,OrderContext oc)
  : m_id(vtx)
  , m_ptSum(0.)
  , m_ptSum2(0.)
  , m_ptSumCharged(0.)
  , m_ptSum2Charged(0.)
  , m_nPart(0)
  , m_nPartCharged(0)
  , m_orderContext(oc)
{ }

Vertex::Vertex(const Vertex& vertex)
  : m_id(vertex.m_id)
  , m_ptSum(vertex.m_ptSum)
  , m_ptSum2(vertex.m_ptSum2)
  , m_ptSumCharged(vertex.m_ptSumCharged)
  , m_ptSum2Charged(vertex.m_ptSum2Charged)
  , m_nPart(vertex.m_nPart)
  , m_nPartCharged(vertex.m_nPartCharged)
  , m_orderContext(vertex.m_orderContext)
{ }

Vertex::~Vertex()
{ }

Vertex& Vertex::operator=(const Vertex& vertex)
{
  m_id            = vertex.m_id;
  m_ptSum         = vertex.m_ptSum;
  m_ptSum2        = vertex.m_ptSum2;
  m_ptSumCharged  = vertex.m_ptSumCharged;
  m_ptSum2Charged = vertex.m_ptSum2Charged;
  m_nPart         = vertex.m_nPart;
  m_nPartCharged  = vertex.m_nPartCharged;
  m_orderContext  = vertex.m_orderContext;
  return *this;
}

void Vertex::addParticle(const fastjet::PseudoJet& pJet,double charge)
{
  double pT(pJet.perp());
  double pT2(pT*pT);
  m_ptSum  += pT;
  m_ptSum2 += pT2;
  ++m_nPart;
  if ( charge != 0. )
    {
      m_ptSumCharged  += pT;
      m_ptSum2Charged += pT2;
      ++m_nPartCharged;
    }
}

double Vertex::ptSum(Type vtype) const
{ return vtype == TOTAL 
    ? m_ptSum :        vtype == CHARGED 
    ? m_ptSumCharged : vtype == NEUTRAL
    ? m_ptSum-m_ptSumCharged : 0.;
}

double Vertex::ptSum2(Type vtype) const
{ return vtype == TOTAL 
    ? m_ptSum2 :        vtype == CHARGED 
    ? m_ptSum2Charged : vtype == NEUTRAL
    ? m_ptSum2-m_ptSum2Charged : 0.;
}

int Vertex::id() const
{ return m_id; }

int Vertex::multiplicity(Type vtype) const
{ return vtype == TOTAL
    ? m_nPart :        vtype == CHARGED
    ? m_nPartCharged : vtype == NEUTRAL
    ? m_nPart-m_nPartCharged : 0; }
  
bool Vertex::operator>=(const Vertex& vtx) const
{ _compare( >= ); }
bool Vertex::operator>(const Vertex& vtx) const
{ _compare( > ); }
bool Vertex::operator<=(const Vertex& vtx) const
{ _compare( <= ); }
bool Vertex::operator<(const Vertex& vtx) const
{ _compare( < ); }

//////////////////////////
// Particle Information //
//////////////////////////

ParticleInfo::ParticleInfo()
  : fastjet::PseudoJet::UserInfoBase()
  , m_id(-9999)
  , m_charge(0.)
  , m_vertexId(-1)
  , m_interactionType(Vertex::UNKNOWN)
{ }

ParticleInfo::ParticleInfo(int id,double charge,int vtx,Vertex::InteractionType type)
  : fastjet::PseudoJet::UserInfoBase()
  , m_id(id)
  , m_charge(charge)
  , m_vertexId(vtx)
  , m_interactionType(type)
{ }

ParticleInfo::ParticleInfo(const ParticleInfo& pInfo)
  : fastjet::PseudoJet::UserInfoBase()
  , m_id(pInfo.m_id)
  , m_charge(pInfo.m_charge)
  , m_vertexId(pInfo.m_vertexId)
  , m_interactionType(pInfo.m_interactionType)
{ }

ParticleInfo::~ParticleInfo()
{ }

// void ParticleInfo::addParticleInfo(const fastjet::PseudoJet& pJet,
// 				   int id,double charge,int vtx)
// { 
//   // look for vertices
//   std::vector<Vertex>::iterator fVtx(m_vertices.begin());
//   std::vector<Vertex>::iterator lVtx(m_vertices.end());
//   while ( fVtx != lVtx && vtx != (*fVtx).id() )
//     { ++fVtx; }
//   if ( fVtx != lVtx )
//     { fVtx->addParticle(pJet,charge); }
//   else
//     { m_vertices.push_back(Vertex(vtx));
//       m_vertices.back().addParticle(pJet,charge); }
//   // refer to first vertex
//   m_id       = id;
//   m_charge   = charge; 
//   m_vertexId = vtx;
// }

int ParticleInfo::id() const { return m_id; }
double ParticleInfo::charge() const { return m_charge; }
int ParticleInfo::vertexId() const { return m_vertexId; }
Vertex::InteractionType ParticleInfo::interactionType() const 
{ return m_interactionType; }
bool ParticleInfo::isSignal() const 
{ return ( m_interactionType & Vertex::SIGNAL ) == Vertex::SIGNAL; }
bool ParticleInfo::isPileup() const 
{ return ( m_interactionType & Vertex::PILEUP ) == Vertex::PILEUP; }

const bool ParticleInfo::vertex(const Event& thisEvt,Vertex& vertex) const
{
  int vtx(this->vertexId());
  if ( vtx >= 0 && vtx < thisEvt.vertices().size() )
    { vertex = thisEvt.vertices().at(vtx); return true; }
  else
    { return false; }
}

///////////
// Event //
///////////

Event::Event(IFinalStateSelector* pSel) 
  : m_fsSel(pSel)
	    //  , m_eventGridView(0)
	    //  , m_chargedEventGridView(0)
{ }

// Event::Event(const EventGridView& eventGridView,IFinalStateSelector* pSel)
//   : m_fsSel(pSel)
// 	    //  , m_eventGridView(new EventGridView(eventGridView))
// 	    //  , m_chargedEventGridView(new EventGridView(eventGridView))
// { }

Event::~Event()
{ }

bool Event::add(int id,double charge,
		double px,double py,double pz,double m,
		int vtx,Vertex::InteractionType type)
{
  double e(sqrt(px*px+py*py+pz*pz+m*m));
  // create PseudoJet
  fastjet::PseudoJet pJet(px,py,pz,e); 
  pJet.set_user_info(new ParticleInfo(id,charge,vtx,type));
  return this->add(pJet);
}

bool Event::add(const fastjet::PseudoJet& pJet)
{
  if ( !pJet.has_user_info<ParticleInfo>() ) return false;
  if ( m_fsSel == 0 || (m_fsSel != 0 && m_fsSel->accept(pJet)) )
    {
      int vtx(pJet.user_info<ParticleInfo>().vertexId());
      if ( vtx < 0 )
	{ 
	  printf("[Event::add(const fastjet::PseudoJet&)] - ERROR - invalid vertex index %i\n",vtx); 
	  return false; 
	}
      this->updateVtx(vtx);
      m_particles[vtx].push_back(pJet);
      this->addToVtx(pJet);
      return true;
    } 
  return false;
} 

bool Event::add(const std::vector<fastjet::PseudoJet>& vJet)
{
  std::vector<fastjet::PseudoJet>::const_iterator fJet(vJet.begin());
  std::vector<fastjet::PseudoJet>::const_iterator lJet(vJet.end());
  bool isOk(true);
  for ( ; fJet != lJet; ++fJet ) 
    { if ( isOk ) { isOk = this->add(*fJet); } else { this->add(*fJet); } }
  return isOk;
}


void Event::updateVtx(int vtx)
{
  //
  if ( vtx >= (int)m_vertices.size() ) 
    { 
      m_particles.resize((size_t)(vtx+1),std::vector<fastjet::PseudoJet>());
      m_vertices.resize((size_t)(vtx+1),Vertex(-1));
      m_vertices[vtx] = Vertex(vtx);
    }
}

bool Event::addToVtx(const fastjet::PseudoJet& pJet)
{
  if ( pJet.has_user_info<ParticleInfo>() )
    {
      // check on vertex id
      int vtx(pJet.user_info<ParticleInfo>().vertexId());
      if ( vtx < 0 )
	{
	  printf("Event::addToVtx(...) - ERROR - unknown vertex, index = %i\n",
		 vtx);
	  return false;
	}
      m_vertices[vtx].addParticle(pJet,
				  pJet.user_info<ParticleInfo>().charge());
      return true;
    }
  printf("[Event::addToVtx(...)] - WARNING - invalid user information type\n");
  return false;
}

bool Event::haveParticleEvent() { return this->nParticles() > 0; }
bool Event::havePartonicEvent() { return this->nPartons()   > 0; }

bool Event::empty(int vtx) const
{
  if ( vtx < 0 )
    {
      size_t iv(0);
      bool   isEmpty(true);
      while ( iv < m_particles.size() && isEmpty )
	{ isEmpty = m_particles.at(vtx).empty(); if ( isEmpty ) ++iv; }
      return iv == m_particles.size();
    }
  else if ( vtx < m_particles.size() )
    { return m_particles.at(vtx).empty(); }
  else
    { return true; }
}

int Event::size(int vtx) const
{
  if ( vtx < 0 )
    {
      size_t np(0);
      for ( size_t iv(0); iv<m_particles.size(); ++iv )
	{ np += m_particles.at(vtx).size(); }
      return (int)np;
    }
  else if ( vtx < m_particles.size() )
    { return (int)m_particles.at(vtx).size(); }
  else
    { return -1; }
}

int Event::nParticles(int vtx) const
{ return this->size(vtx); }

int Event::nPartons(int /*vtx*/) const
{ return -1; }

int Event::nVertices() const
{ return (int)m_vertices.size(); }

void Event::reset() 
{
  m_particles.clear();
  m_partons.clear();
  m_vertices.clear();
  if ( m_fsSel != 0 ) m_fsSel->reset();
//   if ( m_eventGridView != 0 ) m_eventGridView->reset();
//   if ( m_chargedEventGridView != 0 ) m_chargedEventGridView->reset();
}

std::vector<fastjet::PseudoJet> Event::pseudoJets(int vtx)
{
  std::vector<fastjet::PseudoJet> pVec;
  if ( vtx < 0 )
    {
      std::vector<std::vector<fastjet::PseudoJet> >::const_iterator 
	fVtx(m_particles.begin());
      std::vector<std::vector<fastjet::PseudoJet> >::const_iterator 
	lVtx(m_particles.end());
      for ( ; fVtx != lVtx; ++fVtx ) { Utils::copyPJ(*fVtx,pVec); }
    } // all vertices
  else if ( vtx < m_particles.size() )
    { Utils::copyPJ(m_particles.at(vtx),pVec); }
  return pVec;
}

std::vector<fastjet::PseudoJet> Event::pseudoJets(Vertex::InteractionType type)
{
  std::vector<fastjet::PseudoJet> pVec;
  std::vector<std::vector<fastjet::PseudoJet> >::const_iterator fVtx(m_particles.begin());
  std::vector<std::vector<fastjet::PseudoJet> >::const_iterator lVtx(m_particles.end());
  for ( ; fVtx != lVtx; ++fVtx ) { Utils::copyPJ(*fVtx,pVec,type); }
  return pVec;
}

std::vector<fastjet::PseudoJet> Event::pseudoJets(bool charged,int vtx)
{
  std::vector<fastjet::PseudoJet> pVec;
  // check all vertices
  if ( vtx < 0 )
    {
      std::vector<std::vector<fastjet::PseudoJet> >::const_iterator fVtx(m_particles.begin());
      std::vector<std::vector<fastjet::PseudoJet> >::const_iterator lVtx(m_particles.end());
      for ( ; fVtx != lVtx; ++fVtx )
	{ Utils::copyPJ(*fVtx,pVec,charged); }  // loop on all vertices
    } // all vertices request
  // check specific vertex
  else if ( vtx < m_particles.size() )
    { Utils::copyPJ(m_particles.at(vtx),pVec,charged); }

  return pVec;
}

std::vector<fastjet::PseudoJet> Event::pseudoJets(bool charged,Vertex::InteractionType type)
{
  std::vector<fastjet::PseudoJet> pVec;
  std::vector<std::vector<fastjet::PseudoJet> >::const_iterator fVtx(m_particles.begin());
  std::vector<std::vector<fastjet::PseudoJet> >::const_iterator lVtx(m_particles.end());
  for ( ; fVtx != lVtx; ++fVtx ) { Utils::copyPJ(*fVtx,pVec,type,charged); }
  return pVec;
}

const Vertex& Event::vertex(int id) const
{ 
  if ( id > 0 )
    {
      std::vector<Vertex>::const_iterator fVtx(m_vertices.begin());
      std::vector<Vertex>::const_iterator lVtx(m_vertices.end());
      while ( fVtx != lVtx && fVtx->id() != id ) { ++fVtx; }
      return fVtx != lVtx ? *fVtx : dummyVtx();
    }
  else
    {
      std::vector<Vertex>::const_iterator fVtx(m_vertices.begin());
      std::vector<Vertex>::const_iterator lVtx(m_vertices.end());
      double ptm(0.);
      size_t idx(0);
      for ( ; fVtx != lVtx; ++fVtx )
	{ if ( fVtx->ptSum2(Vertex::CHARGED) > ptm ) 
	    { ptm = fVtx->ptSum2(Vertex::CHARGED); 
	      idx = fVtx - m_vertices.begin(); }
	}
      return m_vertices.at(idx);
    }
}
//       std::vector<Vertex> orderedVtx;
//       std::copy(m_vertices.begin(),m_vertices.end(),std::back_insert_iterator<std::vector<Vertex>>(orderedVtx));
// //       Utils::sortVtx(orderedVtx,Vertex::PTSUM2);
// //       return orderedVtx.front();
//     }
// }

 
// const EventGridView* Event::eventGridView(bool charged)
// { return charged ? m_eventGridView : m_chargedEventGridView; }

//  void Event::EventGridView::reset()
//  { std::vector<double> 
   
//  }

