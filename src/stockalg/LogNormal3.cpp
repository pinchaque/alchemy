
#include "stockalg/LogNormal3.h"

#include <cmath>

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif

namespace alch {

  const double c_pi = 3.141592653589793238;

  // number of standard deviations to move the min and max
  const double c_mult = 4.0;


  LogNormal3::LogNormal3(double min, double max, double mean, double stddev)
    : m_mean(mean)
    , m_stddev(stddev)
  {
    assert(min <= m_mean);
    assert(max >= m_mean);
    
    if ((m_mean - min) < (max - m_mean))
    {
      m_ext = min - (c_mult * m_stddev);
    }
    else
    {
      m_ext = max + (c_mult * m_stddev);
    }

    calcStats();
  }

  LogNormal3::LogNormal3(double ext, double mean, double stddev)
    : m_ext(ext)
    , m_mean(mean)
    , m_stddev(stddev)
  {
    calcStats();
  }

  void LogNormal3::calcStats()
  {
    // these are the formulas from the provided source code
    if (m_ext < m_mean)
    {
      m_extremeAtMin = true;
      m_dif = m_mean - m_ext;
    }
    else
    {
      m_extremeAtMin = false;
      m_dif = m_ext - m_mean;
    }
    m_sigma = ::log(1.0 + (::pow(m_stddev, 2.0) / ::pow(m_dif, 2.0)));
    m_mu = ::log(m_dif) - (m_sigma / 2.0);
    m_alpha = 1.0 / ::sqrt(2.0 * c_pi * m_sigma);
    m_beta = -0.5 / m_sigma;
    
    // original book had these formulas, which are apparently wrong:
    //m_dif = ::fabs(m_mean - m_ext);
    //m_mu = ::log(m_dif) - ::pow(m_sigma, 2.0);
    //m_alpha = 1.0 / (m_sigma * ::sqrt(2.0 * c_pi));
    //m_beta = -0.5 / ::pow(m_sigma, 2.0);
  }

  double LogNormal3::getNegInfinity() const
  {
    if (m_extremeAtMin)
    {
      return m_ext;
    }
    else
    {
      return m_mean - 10.0 * m_stddev;
    }
  }

  double LogNormal3::getPosInfinity() const
  {
    if (m_extremeAtMin)
    {
      return m_mean + 10.0 * m_stddev;
    }
    else
    {
      return m_ext;
    }
  }


#ifndef NDEBUG
  void LogNormal3::dump(std::ostream& os) const
  {
    os << "[LogNormal3]\n";
    os << "m_ext = " << m_ext << "\n";
    os << "m_mean = " << m_mean << "\n";
    os << "m_stddev = " << m_stddev << "\n";
    os << "m_extremeAtMin = " << m_extremeAtMin << "\n";
    os << "m_dif = " << m_dif << "\n";
    os << "m_sigma = " << m_sigma << "\n";
    os << "m_mu = " << m_mu << "\n";
    os << "m_alpha = " << m_alpha << "\n";
    os << "m_beta = " << m_beta << "\n";
    os << "negInf = " << getNegInfinity() << "\n";
    os << "posInf = " << getPosInfinity() << "\n";
  }
#endif

  double LogNormal3::eval(double x) const
  {
    double y = 0.0;
    if (m_extremeAtMin)
    {
      y = x - m_ext;
    }
    else
    {
      y = m_ext - x;
    }

    if (y > 0.0)
    {
      return (m_alpha / y) * ::exp(m_beta * ::pow(::log(y) - m_mu, 2.0));
      // original book had this forumula
      //return (m_alpha / y) * ::exp(m_beta * (::log(y) - m_mu));
    }
    else
    {
      return 0.0;
    }
  }

} // namespace alch
