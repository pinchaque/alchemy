// -*- C++ -*-

#ifndef INCLUDED_stockalg_Normal_h
#define INCLUDED_stockalg_Normal_h

#include "boost/shared_ptr.hpp"

#ifndef NDEBUG
#include <iosfwd>
#endif

namespace alch {

/*!
  \brief Functor class for computing normal distribution
  \ingroup stockalg
*/
class Normal
{
 public:

  /*!
    \brief Constructor
  */
  Normal(double mean, double stddev)
    : m_mean(mean)
    , m_stddev(stddev)
  {
    ;
  }

  virtual ~Normal()
  {
    ;
  }

  double operator()(double x) const
  {
    return eval(x);
  }

  double eval(double x) const;

 private:
  double m_mean;
  double m_stddev;

};

/*!
  \brief Shared pointer to Normal
  \ingroup stockalg
*/
typedef boost::shared_ptr<Normal> NormalPtr;

} // namespace alch

#endif
