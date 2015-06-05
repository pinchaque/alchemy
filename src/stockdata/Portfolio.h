// -*- C++ -*-

#ifndef INCLUDED_stockdata_Portfolio_h
#define INCLUDED_stockdata_Portfolio_h

#include "boost/shared_ptr.hpp"

#include "stockdata/StockID.h"

#include <vector>
#include <map>

namespace alch {

  class Portfolio;

  /*!
  \brief Shared pointer to Portfolio
  \ingroup stockdata
*/
typedef boost::shared_ptr<Portfolio> PortfolioPtr;

/*!
  \brief Class description
  \ingroup stockdata
*/
class Portfolio
{
 public:
  typedef std::vector<StockID> Symbols;
  typedef std::map<StockID, double> AmountMap;

  /*!
    \brief Constructor
  */
  Portfolio();

  // adds/sets an amount
  void add(const StockID& id, double amount = 0.0);

  // clears out all symbols and amount
  void clear();

  // sets all amounts to 0
  void setZero();

  // clones this portfolio
  PortfolioPtr clone() const;

  // gets the list of symbols
  const Symbols& getSymbols() const;

  // gets #amount for particular symbol
  double get(const StockID& id) const;

  // checks whether symbol exists
  bool has(const StockID& id) const;

  // normalizes this portfolio to the specified total value
  void normalize(double total);

  // returns total of amount of this portfolio
  double getTotal() const;
  
  // rebalances this portfolio so that the ratios of securities match the other
  // one
  void rebalance(const Portfolio& other);

  virtual ~Portfolio()
  {
    ;
  }

  void dump(std::ostream& os);

  static const double MINFLOAT;

 private:
  Symbols m_symbols;
  AmountMap m_amount;
};


} // namespace alch

#endif
