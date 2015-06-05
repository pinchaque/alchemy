// -*- C++ -*-

#ifndef INCLUDED_stockalg_PortfolioAlg_h
#define INCLUDED_stockalg_PortfolioAlg_h

#include "boost/shared_ptr.hpp"

#include "autil/Context.h"
#include "stockdata/Portfolio.h"
#include "stockdata/RangeData.h"

namespace alch {

/*!
  \brief Provides algorithms for applying return data to a portfolio to
  track what its overall return would be. 
  \ingroup stockalg
*/
class PortfolioAlg
{
 public:

   typedef std::map<StockID, RangeDataPtr> ReturnsMap;

  /*!
    \brief Constructor
  */
  PortfolioAlg(PortfolioPtr ptf, Context& ctx)
    : m_ctx(ctx)
    , m_returns()
    , m_trimReturns()
    , m_ptf(ptf)
  {
    ;
  }

  void clear()
  {
    m_returns.clear();
    m_trimReturns.clear();
  }

  void addData(const StockID& id, RangeDataPtr data)
  {
    m_returns[id] = data;
  }

  // populates m_trimReturns for the specified portfolio's symbols given
  // the data that's already been loaded. returns true on success.
  bool trimReturns();

  // checks m_trimReturns, add comments, returns true if OK
  bool checkTrimReturns();

  const std::vector<RangeDataPtr>& getTrimReturns() const
  {
    return m_trimReturns;
  }

  // generates a new set of valuation data based on the data that we have
  // loaded for the individual components of the portfolio. 
  // rebalances after every rebalnceFreq periods, or never if 0
  RangeDataPtr generateData(PortfolioPtr ptf, int rebalanceFreq) const;

  virtual ~PortfolioAlg()
  {
    ;
  }

 private:
  Context& m_ctx;
  ReturnsMap m_returns;
  std::vector<RangeDataPtr> m_trimReturns;
  PortfolioPtr m_ptf;
};

/*!
  \brief Shared pointer to PortfolioAlg
  \ingroup stockalg
*/
typedef boost::shared_ptr<PortfolioAlg> PortfolioAlgPtr;

} // namespace alch

#endif
