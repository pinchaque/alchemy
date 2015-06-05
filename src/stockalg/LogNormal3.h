// -*- C++ -*-

#ifndef INCLUDED_stockalg_LogNormal3_h
#define INCLUDED_stockalg_LogNormal3_h

#include "boost/shared_ptr.hpp"

#ifndef NDEBUG
#include <iosfwd>
#endif

namespace alch {

/*!
  \brief Functor for 3-parameter lognormal distribution
  \ingroup stockalg
*/
class LogNormal3
{
 public:

  /*!
    \brief Construct based on the 3 parameters
    \param ext The extreme value
    \param mean The mean value of the data series
    \param stddev Standard deviation of the data series

    This constructs the log normal 3 distribuution functor based on the 
    actual 3 parameters
  */
  LogNormal3(double ext, double mean, double stddev);

  /*!
    \brief Construct based on dataset qualities
    \param min The minimum value in the dataset
    \param max The maximum value in the dataset
    \param mean The mean value of the data series
    \param stddev Standard deviation of the data series
  */
  LogNormal3(double min, double max, double mean, double stddev);

  virtual ~LogNormal3()
  {
    ;
  }

  double operator()(double x) const
  {
    return eval(x);
  }

  /*!
   \brief Evalutes the function at the specified point
   */
  double eval(double x) const;

  double getExtremeValue() const { return m_ext; }

  /*!
   * Gets a proxy value for negative infinity that we can use when integrating
   */
  double getNegInfinity() const;

  /*!
   * Gets a proxy value for positive infinity that we can use when integrating
   */
  double getPosInfinity() const;

#ifndef NDEBUG
    //! Dumps object contents to stream
    void dump(std::ostream& os) const;
#endif


 private:
  double m_ext;
  double m_mean;
  double m_stddev;
  bool m_extremeAtMin;
  double m_dif;
  double m_sigma;
  double m_mu;
  double m_alpha;
  double m_beta;

  void calcStats();
};

/*!
  \brief Shared pointer to LogNormal3
  \ingroup stockalg
*/
typedef boost::shared_ptr<LogNormal3> LogNormal3Ptr;

} // namespace alch

#endif
