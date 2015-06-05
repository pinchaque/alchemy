
#include "stockdata/Portfolio.h"

namespace alch {

  const double Portfolio::MINFLOAT = 0.00001;

  Portfolio::Portfolio()
    : m_symbols()
    , m_amount()
  {
  }

  void Portfolio::add(const StockID& id, double amount)
  {
    AmountMap::const_iterator iter = m_amount.find(id);
    if (iter == m_amount.end())
    {
      m_symbols.push_back(id);
    }

    m_amount[id] = amount;
  }


  void Portfolio::clear()
  {
    m_symbols.clear();
    m_amount.clear();
  }

  void Portfolio::setZero()
  {
    Symbols::const_iterator end = m_symbols.end();
    Symbols::const_iterator iter;
    for (iter = m_symbols.begin(); iter != end; ++iter)
    {
      m_amount[*iter] = 0.0;
    }
  }

  PortfolioPtr Portfolio::clone() const
  {
    PortfolioPtr ptf(new Portfolio());
    Symbols::const_iterator end = m_symbols.end();
    Symbols::const_iterator iter;
    for (iter = m_symbols.begin(); iter != end; ++iter)
    {
      ptf->add(*iter, get(*iter));
    }
    return ptf;
  }

  const Portfolio::Symbols& Portfolio::getSymbols() const
  {
    return m_symbols;
  }


  double Portfolio::get(const StockID& id) const
  {
    AmountMap::const_iterator iter = m_amount.find(id);
    if (iter == m_amount.end())
    {
      return 0.0;
    }
    else
    {
      return iter->second;
    }
  }

  bool Portfolio::has(const StockID& id) const
  {
    return (m_amount.find(id) != m_amount.end());
  }

  double Portfolio::getTotal() const
  {
    double total = 0.0;
    Symbols::const_iterator end = m_symbols.end();
    Symbols::const_iterator iter;
    for (iter = m_symbols.begin(); iter != end; ++iter)
    {
      double value = get(*iter);
      if (value > MINFLOAT)
      {
        total += get(*iter);
      }
    }
    return total;
  }

  void Portfolio::normalize(double total)
  {
    double oldTotal = getTotal();

    Symbols::const_iterator end = m_symbols.end();
    Symbols::const_iterator iter;
    for (iter = m_symbols.begin(); iter != end; ++iter)
    {
      double oldValue = get(*iter);
      double newValue = 0.0;

      if ((oldValue > MINFLOAT) && (oldTotal > 0.0))
      {
        newValue = total * get(*iter) / oldTotal;
      }
      add(*iter, newValue);
    }
  }

  void Portfolio::rebalance(const Portfolio& other)
  {
    double otherTotal = other.getTotal();
    double thisTotal = getTotal();

    Symbols::const_iterator end = m_symbols.end();
    Symbols::const_iterator iter;
    for (iter = m_symbols.begin(); iter != end; ++iter)
    {
      double otherValue = other.get(*iter);
      double otherPerc = otherValue / otherTotal;
      double newValue = thisTotal * otherPerc;
      add(*iter, newValue);
    }

    assert(::fabs(getTotal() - thisTotal) < MINFLOAT);
  }
  
  void Portfolio::dump(std::ostream& os)
  {
    Symbols::const_iterator end = m_symbols.end();
    Symbols::const_iterator iter;
    for (iter = m_symbols.begin(); iter != end; ++iter)
    {
      os << "  " << *iter << ":" << get(*iter);
    }
    os << std::endl;
  }

} // namespace alch
