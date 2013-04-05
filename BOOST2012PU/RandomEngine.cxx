
#include "RandomEngine.h"

#include <cmath>
#include <iostream>

using namespace Random;
RandomEngine* RandomEngine::m_instance = 0;

double RandomEngine::landau(double mean,double width)
{
  static double l_range = 20.;
  Landau& lf = *(Landau::instance());
  double mo(0.);
  double mx(lf.max(mean,width,mo));
  double range(mx+l_range*width);
  double x(this->random(0.,range));
  double y(this->random(0.,mx));
  while ( lf(x,mean,width) < y ) 
    {
      x = this->random(0.,range);
      y = this->random(0,mx);
    }
  return x;
}

double RandomEngine::moyal(double mop,double r,double a,double b)
{
  static size_t maxIter = 100000;
  Moyal& mo = *(Moyal::instance()); 
  double x(this->random(a,b));
  double y(this->random(0.,mo.max()));
  size_t iter(0);
  while ( mo(x,mop,r) < y && iter < maxIter ) 
    {
      x = this->random(a,b);
      y = this->random(0.,mo.max());
      ++iter;
    }
  if ( iter == maxIter )
    {
      std::cout << "[RandomEngine::moyal(" 
		<< mop << ","
		<< r << ","
		<< a << ","
		<< b << ")] - hit max # of iterations ("
		<< iter
		<< "/"
		<< maxIter
		<< "), value = " 
		<< x << std::endl;
      return mop;
    }
  return x;
}

Moyal* Moyal::m_instance = 0;

Moyal* Moyal::instance()
{
  if ( m_instance == 0 )
    {
      m_instance = new Moyal();
    }
  return m_instance;
}

Moyal::Moyal()
  : m_twoPi(4.*std::asin(1.))
  , m_max(std::sqrt(std::exp(-1.)/m_twoPi))
{
  std::cout << "[Moyal::Moyal()] - Moyal function object approximates "
	    << "Landau distribution"
	    << std::endl;
}

Moyal::~Moyal()
{ }

double Moyal::operator()(double x,double mop,double r)
{
  double lambda(r*(x-mop));
  return 
    std::sqrt(std::exp(-(lambda+std::exp(-lambda)))/m_twoPi) / m_max;
}

Landau* Landau::m_instance = 0;

Landau* Landau::instance()
{
  if ( m_instance == 0 )
    {
      m_instance = new Landau();
    }
  return m_instance;
}

Landau::Landau()
{
  std::cout << "[Landau::Landau()] - implementation from CLHEP v.2.1.0.1"
	    << " (Nov. 2010)" << std::endl;
}

Landau::~Landau()
{ }

double Landau::function(double x)
{
  ///////////////
  // Constants //
  ///////////////

  static double p1[5] = { (double).4259894875,
			  (double)-.124976255,
			  (double) .039842437,
			  (double)-.006298287635,
			  (double).001511162253 };
  static double q5[5] = { (double)1.,
			  (double)156.9424537,
			  (double)3745.310488,
			  (double)9834.698876,
			  (double)66924.28357 };
  static double p6[5] = { (double)1.000827619,
			  (double)664.9143136,
			  (double)62972.92665,
			  (double)475554.6998,
			  (double)-5743609.109 };
  static double q6[5] = { (double)1.,
			  (double)651.4101098,
			  (double)56974.73333,
			  (double)165917.4725,
			  (double)-2815759.939 };
  static double a1[3] = { (double).04166666667,
			  (double)-.01996527778,
			  (double).02709538966 };
  static double a2[2] = { (double)-1.84556867,
			  (double)-4.284640743 };
  static double q1[5] = { (double)1.,
			  (double)-.3388260629,
			  (double).09594393323,
			  (double)-.01608042283,
			  (double).003778942063 };
  static double p2[5] = { (double).1788541609,
			  (double).1173957403,
			  (double).01488850518,
			  (double)-.001394989411,
			  (double)1.283617211e-4 };
  static double q2[5] = { (double)1.,
			  (double).7428795082,
			  (double).3153932961,
			  (double).06694219548,
			  (double).008790609714 };
  static double p3[5] = { (double).1788544503,
			  (double).09359161662,
			  (double).006325387654,
			  (double)6.611667319e-5,
			  (double)-2.031049101e-6 };
  static double q3[5] = { (double)1.,
			  (double).6097809921,
			  (double).2560616665,
			  (double).04746722384,
			  (double).006957301675 };
  static double p4[5] = { (double).9874054407,
			  (double)118.6723273,
			  (double)849.279436,
			  (double)-743.7792444,
			  (double)427.0262186 };
  static double q4[5] = { (double)1.,
			  (double)106.8615961,
			  (double)337.6496214,
			  (double)2016.712389,
			  (double)1597.063511 };
  static double p5[5] = { (double)1.003675074,
			  (double)167.5702434,
			  (double)4789.711289,
			  (double)21217.86767,
			  (double)-22324.9491 };
  static double th[7] = { (double)-5.5,
			  (double)-1.,
			  (double)1.,
			  (double)5.,
			  (double)12.,
			  (double)50.,
			  (double)300. };

  // Local variables (why - memory churning for 16 bytes?)
  //  static double u, v;

  // 
  //  v = x;
  if ( x < th[0] )
    {
      double u(std::exp(x+(double)1.));
      return 
	std::exp(-1 / u) / std::sqrt(u) * 
	(double).3989422803 * ((a1[0] + (a1[1] + a1[2] * u) * u) * u + 1);
    } 
  else if ( x < th[1] )
    {
      double u(std::exp(-(x+1)));
      return
	std::exp(-u) * std::sqrt(u) * 
	(p1[0] + (p1[1] + (p1[2] + (p1[3] + p1[4] * x) * x) * x) * x) / 
	(q1[0] + (q1[1] + (q1[2] + (q1[3] + q1[4] * x) * x) * x) * x);
    }
  else if ( x < th[2] ) 
    {
      return 
	(p2[0] + (p2[1] + (p2[2] + (p2[3] + p2[4] * x) * x) * x) * x) /
	(q2[0] + (q2[1] + (q2[2] + (q2[3] + q2[4] * x) * x) * x) * x);
    } 
  else if ( x < th[3] )
    {
      return
	(p3[0] + (p3[1] + (p3[2] + (p3[3] + p3[4] * x) * x) * x) * x) / 
	(q3[0] + (q3[1] + (q3[2] + (q3[3] + q3[4] * x) * x) * x) * x);
    } 
  else if ( x < th[4] )
    {
      double u((double)1./x);
      // Computing 2nd power 
      return 
	u * u * 
	(p4[0] + (p4[1] + (p4[2] + (p4[3] + p4[4] * u) * u) * u) * u) / 
	(q4[0] + (q4[1] + (q4[2] + (q4[3] + q4[4] * u) * u) * u) * u);
    } 
  else if ( x < th[5] )
    {
      double u((double)1./x);
      // Computing 2nd power
      return
	u * u * 
	(p5[0] + (p5[1] + (p5[2] + (p5[3] + p5[4] * u) * u) * u) * u) / 
	(q5[0] + (q5[1] + (q5[2] + (q5[3] + q5[4] * u) * u) * u) * u);
    } 
  else if ( x < th[6] )
    {
      double u ((double)1./x);
      // Computing 2nd power
      return
	u * u * 
	(p6[0] + (p6[1] + (p6[2] + (p6[3] + p6[4] * u) * u) * u) * u) / 
	(q6[0] + (q6[1] + (q6[2] + (q6[3] + q6[4] * u) * u) * u) * u);
    } 
  else 
    {
      double u((double)1./(x-x*std::log(x)/(x+(double)1.)));
      // Computing 2nd power
      return u * u * ((a2[0] + a2[1] * u) * u + 1);
    }
}

double Landau::mop(double mean,double width)
{
  // numerical integration from left of mean assumes monotonic rising shape
  static const double l_step = 0.001;
  static const double l_dfac = 5.0;
  //
  double ds(l_step*(mean-width));
  double s(mean-l_dfac*width);
  double mop(0.);
  Landau& lf = *(Landau::instance());
  while ( s < mean && lf(s,mean,width) >= mop ) 
    { mop = lf(s,mean,width); s += ds; }
  //
  return mop;
}
