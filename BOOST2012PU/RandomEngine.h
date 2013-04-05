// -*- c++ -*-
#ifndef RANDOMENGINE_H
#define RANDOMENGINE_H

#include "TRandom1.h"
#include <ctime>

#ifndef _HAVE_NO_BOOST_
#include <boost/math/special_functions/round.hpp>
#define iround boost::math::iround
#else
#include <cmath>

namespace Random
{
  static int iround(double x)
  {
    int ix((int)x);
    if ( std::abs((double)ix-x) > 0.5 ) 
      {
	if ( x < 0. ) { --ix; } else { ++ix; }
      }
    return ix;
  }
}
#endif

namespace Random
{
  static unsigned int seed()
  {
    time_t seconds = time(0);
    return (unsigned int)seconds;
  }

  class RandomEngine
  {
  public:
    ~RandomEngine() {}
    static RandomEngine* instance();
    double gauss(double mean=0.,double sigma=1.);
    double poisson(double mean);
    int    ipoisson(double mean);
    int    ipoisson(int mean);
    double random(double a=0.,double b=1.);
    int    irandom(int a=0,int b=10);
    double landau(double m,double w);
    double moyal(double m,double w,double a,double b);
  private:
    static RandomEngine* m_instance;
    TRandom1* m_engine;
    RandomEngine(); 
  };

  class Landau
  {
  public:
 
    ~Landau();
    static Landau* instance();

    double operator()(double x,double mean,double width);
    static double landau(double x,double mean,double width);
    static double mop(double mean,double w);
    static double max(double mean,double w,double& mop);

  protected:

    Landau();

  private:

    double function(double x);
    static Landau* m_instance;

  };

  class Moyal
  {
  public:
    ~Moyal();
    static Moyal* instance();
    
    double operator()(double x,double mop,double r);
    static double moyal(double x,double mop,double r);
    static double max();

  protected:

    Moyal();

  private:
    
    static Moyal* m_instance;
    double m_twoPi;
    double m_max;
  };
}

inline Random::RandomEngine::RandomEngine()
  : m_engine(new TRandom1(seed())) {}

inline Random::RandomEngine* Random::RandomEngine::instance()
{
  if ( m_instance == 0 ) m_instance = new RandomEngine();
  return m_instance;
}

inline double Random::RandomEngine::gauss(double mean,double sigma)
{ return m_engine->Gaus(mean,sigma); }

inline double Random::RandomEngine::poisson(double mean)
{ return m_engine->Poisson(mean); }

inline double Random::RandomEngine::random(double a,double b)
{ return a + m_engine->Rndm()*(b-a); }

inline int Random::RandomEngine::ipoisson(int mean)
{ return iround(this->poisson((double)mean)); }

inline int Random::RandomEngine::ipoisson(double mean)
{ return iround(this->poisson(mean)); }

inline int Random::RandomEngine::irandom(int a,int b)
{ return iround(a + m_engine->Rndm()*(b-a)); }

inline double Random::Landau::operator()(double x,double mean,double width) 
{
  double u((x-(mean+(double)0.222782*width))/width);
  return this->function(u);
}

inline double Random::Landau::landau(double x,double mean,double width)
{
  Landau& lf = *(Landau::instance());
  return lf(x,mean,width);
} 

inline double Random::Landau::max(double mean,double width,double& mop)
{
  Landau& lf = *(Landau::instance());
  mop = lf.mop(mean,width);
  return lf(mop,mean,width);
}

inline double Random::Moyal::moyal(double x,double mop,double r)
{
  Moyal& mo = *(Moyal::instance());
  return mo(x,mop,r);
}

inline double Random::Moyal::max()
{ 
  Moyal& mo = *(Moyal::instance());
  return mo.m_max; }
#endif
