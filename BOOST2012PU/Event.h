// -*- c++ -*-
#ifndef EVENT_H
#define EVENT_H

#include "fastjet/PseudoJet.hh"

#include "IFinalStateSelector.h"

#include "RandomEngine.h"

#include <vector>

#define _compare( _x_ )                                                       \
  OrderContext oc = vtx.m_orderContext;                                       \
  vtx.setOrderContext(m_orderContext);                                        \
  bool flag(false);                                                           \
  switch ( m_orderContext )                                                   \
    {                                                                         \
    case PTSUM2:                                                              \
      flag = m_ptSum2 _x_ vtx.m_ptSum2;                                       \
      break;                                                                  \
    case PTSUM:                                                               \
      flag = m_ptSum _x_ vtx.m_ptSum;                                         \
      break;                                                                  \
    case PTSUM2CHARGED:                                                       \
      flag = m_ptSum2Charged _x_ vtx.m_ptSum2Charged;                         \
      break;                                                                  \
    case PTSUMCHARGED:                                                        \
      flag = m_ptSumCharged _x_ vtx.m_ptSumCharged;                           \
      break;                                                                  \
    case PTSUM2NEUTRAL:                                                       \
      flag = m_ptSum2-m_ptSum2Charged _x_ vtx.m_ptSum2-vtx.m_ptSum2Charged;   \
      break;                                                                  \
    case PTSUMNEUTRAL:                                                        \
      flag = m_ptSum-m_ptSumCharged _x_ vtx.m_ptSum-vtx.m_ptSumCharged;       \
      break;                                                                  \
    default:                                                                  \
      break;                                                                  \
    }                                                                         \
  vtx.setOrderContext(oc);						      \
  return flag

namespace Constants
{
  static const double pi(2*asin(1.));
  static const double twoPi(2*pi); 
}

class Vertex
{
public:

  enum Type { NEUTRAL = 0,
	      CHARGED = 1, 
	      TOTAL   = 0 };

  enum OrderContext { PTSUM = 0,
		      PTSUM2 = 1,
		      PTSUMCHARGED = 2,
		      PTSUM2CHARGED = 3,
		      PTSUMNEUTRAL = 4,
		      PTSUM2NEUTRAL = 5 };

  Vertex(OrderContext oc=PTSUM2);
  Vertex(int vtx,OrderContext oc=PTSUM2);
  Vertex(const Vertex& vertex);
  virtual ~Vertex();

  Vertex& operator=(const Vertex& vertex);

  void addParticle(const fastjet::PseudoJet& pJet,double charge=0.);

  int id() const;

  double ptSum(Type vtype=TOTAL) const;
  double ptSum2(Type vtype=TOTAL ) const;

  int multiplicity(Type vtype=TOTAL) const;

  void setOrderContext(OrderContext oc=PTSUM);
  void setOrderContext(OrderContext oc=PTSUM) const;

  OrderContext orderContext() const;

  bool operator==(const Vertex& vtx) const;
  bool operator!=(const Vertex& vtx) const;
  bool operator>=(const Vertex& vtx) const;
  bool operator>(const Vertex& vtx) const;
  bool operator<=(const Vertex& vtx) const;
  bool operator<(const Vertex& vtx) const;

  
private:
  
  int m_id;
  
  double m_ptSum;
  double m_ptSum2;
  double m_ptSumCharged;
  double m_ptSum2Charged;

  unsigned int m_nPart;
  unsigned int m_nPartCharged;

  mutable OrderContext m_orderContext;
};

inline void Vertex::setOrderContext(OrderContext oc)
{ m_orderContext = oc; }
inline void Vertex::setOrderContext(OrderContext oc) const
{ m_orderContext = oc; }
inline Vertex::OrderContext Vertex::orderContext() const
{ return m_orderContext; }

inline bool Vertex::operator==(const Vertex& vtx) const
{ return m_id == vtx.id(); }
inline bool Vertex::operator!=(const Vertex& vtx) const
{ return m_id != vtx.id(); }

///

class Event;

class ParticleInfo : public fastjet::PseudoJet::UserInfoBase
{
public:

  ParticleInfo();
  ParticleInfo(int id,double charge,int vtx);
  ParticleInfo(const ParticleInfo& pInfo);
  virtual ~ParticleInfo();

//   virtual void addParticleInfo(const fastjet::PseudoJet& pJet,
// 			       int id,double charge,int vtx);
  
  int id()           const;
  double charge()    const;
  int vertexId()     const;
  //  int multiplicity() const;

  const bool vertex(const Event& thisEvt,Vertex& vertex) const; 

  //  const std::vector<Vertex>& vertices() const;
  
private:

  int    m_id;
  double m_charge;
  int    m_vertexId;
  //  int    m_multiplicity;

  //  std::vector<Vertex>    m_vertices;
};


// class EventGridView
// {
// public:

//   typedef std::vector<std::vector<double> >   grid_t;
//   typedef grid_t::iterator                    eta_iterator;
//   typedef grid_t::const_iterator              const_eta_iterator;
//   typedef std::vector<double>::iterator       phi_iterator;
//   typedef std::vector<double>::const_iterator const_phi_iterator;

//   EventGridView();
//   EventGridView(int nEtaBins,double etaMin,double etaMax,int nPhiBins);
//   virtual ~EventGridView();

//   virtual void insert(const fastjet::PseudoJet& pJet);

//   int etaIndex(eta_iterator etaIter);
//   int etaIndex(const_eta_iterator etaIter) const;
//   int etaIndex(double eta) const;

//   int phiIndex(phi_iterator phiIter);
//   int phiIndex(const_phi_iterator phiIter) const;

//   int phiIndex(double phi) const;

//   int gridIndex(int etaIdx,int phiIdx) const; 
//   int gridIndex(eta_iterator etaIter,phi_iterator phiIter) const;
//   int gridIndex(const_eta_iterator etaIter,const_phi_iterator phiIter) const;
//   int gridIndex(double eta,double phi) const;

//   bool isInRange(double eta,double phi) const;
//   bool isInRange(int etaIdx,int phiIdx) const;
//   bool isInEtaRange(double eta) const;
//   bool isInEtaRange(int etaIdx) const;
//   bool isInPhiRange(double phi) const;
//   bool isInPhiRange(int phiIdx) const;

//   double eta(int etaIdx) const;
//   double phi(int phiIdx) const;

//   int nEtaBins() const;
//   int nPhiBins() const;

//   double etaMin() const;
//   double etaMax() const;

//   double deltaEta() const;
//   double deltaPhi() const;

//   const fastjet::PseudoJet& pseudoJet(int etaIdx,int phiIdx) const;
//   const fastjet::PseudoJet& pseudoJet(int gridIdx) const;

//   void reset();

// private:

//   int m_nEtaBins;
//   int m_nPhiBins;

//   double m_etaMin;
//   double m_etaMax;
//   double m_deltaEta;

//   double m_deltaPhi;

//   grid_t m_particles;
// };

struct Converters
{
  template<class T>
  static int convertToPJ(const T& dataSource,int index,int vtx,
			 fastjet::PseudoJet& pJet)
  {
    double p(dataSource.P0[index]);
    double m(dataSource.Pm[index]);
    double e(sqrt(p*p+m*m));
    pJet.reset(dataSource.Px[index],
	       dataSource.Py[index],
	       dataSource.Pz[index],
	       e);
    pJet.set_user_info(new ParticleInfo(dataSource.ID[index],
					dataSource.Charge[index],
					vtx));
    return 1;
  }

  template<class T>
  static int convertToPJ(const T& dataSource,int findex,int lindex,int vtx,
			 std::vector<fastjet::PseudoJet>& vJet)
  {
    vJet.clear();
    for ( int i=findex;i<lindex;++i )
      {
	vJet.push_back(fastjet::PseudoJet(0.,0.,0.,0.));
	convertToPJ(dataSource,i,vtx,vJet.back());
      }
    return (int)vJet.size();
  }
};

struct Features
{
  static bool isCharged(const fastjet::PseudoJet& pJet);
  static bool nCharged(const std::vector<fastjet::PseudoJet>& vJet,int& nJets,
		       int vtx=-1);
  static bool nNeutral(const std::vector<fastjet::PseudoJet>& vJet,int& nJets,
		       int vtx=-1);
};

struct Utils
{
  static void copyPJ(const fastjet::PseudoJet& inJet,
		     fastjet::PseudoJet& outJet)
  {
    outJet = inJet;
    if ( inJet.has_user_info<ParticleInfo>() )
      { 
	const ParticleInfo& pInfo = inJet.user_info<ParticleInfo>();
	outJet.set_user_info(new ParticleInfo(pInfo));
      }
  }

  static void copyPJ(const std::vector<fastjet::PseudoJet>& inJets,
		     std::vector<fastjet::PseudoJet>&       outJets)
  {
    std::vector<fastjet::PseudoJet>::const_iterator fJet(inJets.begin());
    std::vector<fastjet::PseudoJet>::const_iterator lJet(inJets.end());
    for ( ; fJet != lJet; ++fJet )
      { outJets.push_back(*fJet);
	if ( fJet->has_user_info<ParticleInfo>() )
	  { outJets.back().set_user_info(new ParticleInfo(fJet->user_info<ParticleInfo>())); }
	
      }
  }

  static void setOrderContext(std::vector<Vertex>& vertices,
			      Vertex::OrderContext oc=Vertex::PTSUM2)
  {
    std::vector<Vertex>::iterator fVtx(vertices.begin());
    std::vector<Vertex>::iterator lVtx(vertices.end());
    for ( ; fVtx != lVtx; ++fVtx ) { fVtx->setOrderContext(oc); }
  }

  static void sortVtx(std::vector<Vertex>& vertices,
		      Vertex::OrderContext oc=Vertex::PTSUM2)
  { setOrderContext(vertices,oc);
    std::sort(vertices.begin(),vertices.end()); }
};

template<class T>
struct DataHandler
{
  bool fillEvent(T& dataSource,Event& event,long long& nPtr,int nVtx);
  int  getActVtx(int nVtx); 
};

class Event
{
public:
  Event(IFinalStateSelector* pSel=0);
  //  Event(const EventGridView& gv,IFinalStateSelector* pSel=0);
  virtual ~Event();

  virtual bool add(int ID, double charge, 
		   double px,double py,double pz,double m,
		   int vtx=0);
  virtual bool add(const fastjet::PseudoJet& pJet);
  virtual bool add(const std::vector<fastjet::PseudoJet>& vJet);

  virtual bool haveParticleEvent();
  virtual bool havePartonicEvent();

  virtual std::vector<fastjet::PseudoJet> pseudoJets(int vtx=-1);
  virtual std::vector<fastjet::PseudoJet> pseudoJets(bool charged,int vtx=-1);

  virtual bool empty(int vtx=-1) const; 
  virtual int  size(int vtx=-1)  const;

  virtual int  nParticles(int vtx=-1) const;
  virtual int  nPartons(int vtx=-1)   const;
  virtual int  nVertices()            const;

  virtual const Vertex& vertex(int vtx=0) const;
  virtual const std::vector<Vertex>& vertices() const;

//   virtual const EventGridView* eventGridView(bool charged=false) const;

  virtual void reset();

  static bool isCharged(const fastjet::PseudoJet& pJet)
  { return pJet.has_user_info<ParticleInfo>() 
      ? pJet.user_info<ParticleInfo>().charge() != 0. : false; }

protected:

  void updateVtx(int vtx);
  bool addToVtx(const fastjet::PseudoJet& pJet);

  static const Vertex& dummyVtx();

private:

  IFinalStateSelector* m_fsSel;

  mutable std::vector<std::vector<fastjet::PseudoJet> > m_particles;

  mutable std::vector<Vertex> m_vertices;
  mutable std::vector<std::vector<fastjet::PseudoJet> > m_partons;

  static Vertex _dummyVertex;
//   EventGridView* m_eventGridView;
//   EventGridView* m_chargedEventGridView;
};

inline 
const std::vector<Vertex>& Event::vertices() const
{ return m_vertices; }

// inline bool EventGridView::isInEtaRange(int etaIdx) const
// { return etaIdx >= 0 && etaIdx < m_nEtaBins; }

// inline bool EventGridView::isInEtaRange(double eta) const
// { return eta > m_etaMin && eta < m_etaMax; }

// inline bool EventGridView::isInPhiRange(int phiIdx) const
// { return phiIdx >= 0 && phiIdx < m_nPhiBins; }

// inline bool EventGridView::isInPhiRange(double phi) const
// { if ( phi < 0. ) phi+=Constants::twoPi; 
//   return phi >= 0 && phi < Constants::twoPi; }

// inline int EventGridView::gridIndex(int etaIdx,int phiIdx) const
// { return isInRange(etaIdx,phiIdx) ? etaIdx*m_nPhiBins+phiIdx : -1; }
// inline int EventGridView::gridIndex(double eta,double phi) const
// { return isInEtaRange(eta) && isInPhiRange(phi)
//     ? etaIndex(eta)*m_nPhiBins + phiIndex(phi) : -1; }

// inline int EventGridView::etaIndex(double eta) const
// { return 
//     eta > m_etaMin && eta < m_etaMax ? (int)(eta-m_etaMin)/m_deltaEta : -1; }
// inline int EventGridView::phiIndex(double phi) const
// { if ( phi < 0. ) phi += Constants::twoPi; return (int)phi/m_deltaPhi; }

inline
bool Features::isCharged(const fastjet::PseudoJet& pJet)
{ return pJet.has_user_info<ParticleInfo>() ?
    pJet.user_info<ParticleInfo>().charge() != 0. : false; }

// ROOT CINT for static functions? FIXME!
inline
bool Features::nCharged(const std::vector<fastjet::PseudoJet>& vJet,int& nJets,
			int vtx)
{
  nJets = 0;
  std::vector<fastjet::PseudoJet>::const_iterator fJet(vJet.begin());
  std::vector<fastjet::PseudoJet>::const_iterator lJet(vJet.end());
  if ( vtx < 0 )
    {
      for ( ; fJet != lJet; ++fJet )
	{
	  if ( fJet->has_user_info<ParticleInfo>() &&
	       fJet->user_info<ParticleInfo>().charge() != 0 ) ++nJets;
	}
    }
  else
    {
      for ( ; fJet != lJet; ++fJet )
	{
	  if ( fJet->has_user_info<ParticleInfo>()               &&
	       fJet->user_info<ParticleInfo>().vertexId() == vtx &&
	       fJet->user_info<ParticleInfo>().charge() != 0. ) ++nJets;
	       
	}
    }
  return nJets > 0;
}

#include "Event.icc"

#endif
