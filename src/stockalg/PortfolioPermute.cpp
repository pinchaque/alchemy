
#include "stockalg/PortfolioPermute.h"

#include <iostream>

namespace alch {

  PortfolioPermute::PortfolioPermute(PortfolioPtr ptf, double totalAmount)
    : m_totalAmount(totalAmount)
    , m_ptf(ptf)
  {
    assert(!m_ptf->getSymbols().empty());
    assert(m_totalAmount > 0.0);
  }

  PortfolioPtr PortfolioPermute::next()
  {
    // init and zero-out our new portfolio
    PortfolioPtr ptf = m_ptf->clone();
    ptf->setZero();

    // assign random amounts to each symbol
    double sum = 0.0;
    const Portfolio::Symbols& symbols = m_ptf->getSymbols();
    Portfolio::Symbols::const_iterator end = symbols.end();
    Portfolio::Symbols::const_iterator iter;
    for (iter = symbols.begin(); iter != end; ++iter)
    {
      double amount = drand48();
      ptf->add(*iter, amount);
      sum += amount;
    }

    // now we normalize so the sum is m_totalAmount
    double factor = m_totalAmount / sum;

    for (iter = symbols.begin(); iter != end; ++iter)
    {
      assert(ptf->has(*iter));
      double amount = ptf->get(*iter);
      amount *= factor;
      ptf->add(*iter, amount);
    }

    return ptf;
  }
  
} // namespace alch
