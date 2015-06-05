// -*- C++ -*-

#ifndef INCLUDED_stockalg_PortfolioPermute_h
#define INCLUDED_stockalg_PortfolioPermute_h

#include "boost/shared_ptr.hpp"

#include "stockdata/Portfolio.h"

#include <vector>

namespace alch {

/*!
  \brief Generates random permutations of a portfolio
  \ingroup stockalg

  We do random permutations because there are too many combinations otherwise
*/
class PortfolioPermute
{
 public:

  /*!
    \brief Constructor
  */
  PortfolioPermute(PortfolioPtr ptf, double totalAmount);

  virtual ~PortfolioPermute()
  {
    ;
  }

  PortfolioPtr next();

 private:
  double m_totalAmount;
  PortfolioPtr m_ptf;
};

/*!
  \brief Shared pointer to PortfolioPermute
  \ingroup stockalg
*/
typedef boost::shared_ptr<PortfolioPermute> PortfolioPermutePtr;

} // namespace alch

#endif
