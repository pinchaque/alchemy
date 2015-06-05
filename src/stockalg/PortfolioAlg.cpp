
#include "stockalg/PortfolioAlg.h"
#include "stockdata/StockList.h"

namespace alch {

  bool PortfolioAlg::trimReturns()
  {
    // our trimmed returns -- one for each symbol in the portfolio
    m_trimReturns.clear();

    // iterators to take us through all the rangedatas
    std::vector<RangeData::const_iterator> iters;
    std::vector<RangeData::const_iterator> ends;

    const Portfolio::Symbols& pSyms = m_ptf->getSymbols();
    Portfolio::Symbols::const_iterator end = pSyms.end();
    Portfolio::Symbols::const_iterator iter;
    for (iter = pSyms.begin(); iter != end; ++iter)
    {
      m_trimReturns.push_back(RangeDataPtr(new RangeData()));

      ReturnsMap::const_iterator rIter = m_returns.find(*iter);
      if (rIter == m_returns.end())
      {
        m_ctx << Context::PRIORITY_error
          << "PortfolioAlg missing returns for symbol " << *iter
          << Context::endl;
        return false;
      }
      
      iters.push_back(rIter->second->begin());
      ends.push_back(rIter->second->end());
    }

    // make sure we got at least one
    if (m_trimReturns.empty() || iters.empty())
    {
      m_ctx << Context::PRIORITY_error
        << "PortfolioAlg trimreturns/iters was empty" << Context::endl;
      return false;
    }

    std::vector<RangeData::const_iterator>::size_type i; 
    std::vector<RangeData::const_iterator>::size_type num = iters.size(); 

    for (;;)
    {
      StockTime minTime(boost::posix_time::max_date_time);
      StockTime maxTime(boost::posix_time::min_date_time);

      // do first loop through our iterators to see if any are at the
      // end. also keep track of max and min
      for (i = 0; i < num; ++i)
      {
        RangeData::const_iterator iter = iters[i];
        if (iter == ends[i])
        {
          // once we hit one end we're all done!
          return checkTrimReturns();
        }

        // update the min and max times for position i
        if (iter->tradeTime < minTime)
        {
          minTime = iter->tradeTime;
        }
        if (iter->tradeTime > maxTime)
        {
          maxTime = iter->tradeTime;
        }
      }

      // if min != max, then we know that position i cannot be added as-is
      // we now do another pass through it to bring the lagging iterators
      // forward
      if (minTime != maxTime)
      {
        for (i = 0; i < num; ++i)
        {
          while ((iters[i] != ends[i]) && (iters[i]->tradeTime < maxTime))
          {
            ++(iters[i]);
          }
        }

        continue;
      }

      // if we get here, we know all the elements at position i have the
      // same time, so we can add them to m_trimReturns
      for (i = 0; i < num; ++i)
      {
        // add this point
        m_trimReturns[i]->add(*iters[i]);

        // advance the iterator
        ++(iters[i]);
      }
    }
  }

  bool PortfolioAlg::checkTrimReturns()
  {
    m_ctx << Context::PRIORITY_debug1
      << "Number of returns in m_trimReturns: " << m_trimReturns.size()
      << Context::endl;

    if (m_trimReturns.empty())
    {
      return false;
    }

    RangeData::size_type baseNumPoints = m_trimReturns[0]->size();
    bool ret = true;

    const Portfolio::Symbols& syms = m_ptf->getSymbols();
    assert(syms.size() == m_trimReturns.size());
    for (int i = 0; i < (int) m_trimReturns.size(); ++i)
    {
      RangeDataPtr data = m_trimReturns[i];
      RangeData::size_type numPoints = data->size();

      m_ctx << Context::PRIORITY_debug1
        << "Symbol " << syms[i] << " has " 
        << data->size() << " data points";

      if (numPoints > 0)
      {
        m_ctx << " in range [" 
          << boost::posix_time::to_simple_string(data->get(0).tradeTime)
          << " - "
          << boost::posix_time::to_simple_string(
              data->get(numPoints - 1).tradeTime);
      }

      if (numPoints != baseNumPoints)
      {
        ret = false;
      }

      m_ctx << Context::endl;
    }
    return ret;
  }

  namespace {
    void addDataPoint(RangeDataPtr ret, double total, StockTime time)
    {
      RangeData::Point p;
      p.tradeTime = time;
      p.open = p.close = p.min = p.max = p.adjustedClose = total;
      p.volume = 0.0;

      ret->add(p);
    }
  }
  
  RangeDataPtr PortfolioAlg::generateData(
      PortfolioPtr ptf, int rebalanceFreq) const
  {
    // if we got no results, then we return empty range data
    if (m_trimReturns.empty() || m_trimReturns[0]->empty())
    {
      return RangeDataPtr();
    }

    // values for each symbol in the portfolio
    std::vector<double> values;
    std::vector<double> origPerc;
    double origTotal = ptf->getTotal();

    const Portfolio::Symbols& syms = m_ptf->getSymbols();
    Portfolio::Symbols::const_iterator end = syms.end();
    Portfolio::Symbols::const_iterator iter;
    for (iter = syms.begin(); iter != end; ++iter)
    {
      assert(ptf->has(*iter));

      double value = ptf->get(*iter);
      values.push_back(value);
      origPerc.push_back(value / origTotal);
    }

    RangeDataPtr rangeData(new RangeData());
    addDataPoint(rangeData, origTotal, m_trimReturns[0]->get(0).tradeTime);

    int numReturns = m_trimReturns[0]->size();
    int numSymbols = m_trimReturns.size();

    assert(numSymbols == (int) syms.size());
    assert(numSymbols == (int) values.size());

    // iterate through all our return dates
    for (int i = 1; i < numReturns; ++i)
    {
      StockTime time = m_trimReturns[0]->get(i).tradeTime;

      // iterate through all our symbols
      double newTotal = 0.0;
      for (int j = 0; j < numSymbols; ++j)
      {
        // some sanity-checking
        RangeDataPtr dataJ = m_trimReturns[j];
        assert(numReturns == (int) dataJ->size());
        RangeData::Point p = dataJ->get(i);
        assert(p.tradeTime == time);

        // figure out our % increase to get to time period i
        double lastValue = dataJ->get(i - 1).close;
        double currValue = p.close;
        double ret = 0.0;
        if (lastValue > Portfolio::MINFLOAT)
        {
          ret = (currValue - lastValue) / lastValue;
        }

        // adjust value for this symbol for this time period
        values[j] += ret * values[j];
        newTotal += values[j];
      }

      // rebalancing back to original percentags of new total
      if (rebalanceFreq && !(i % rebalanceFreq))
      {
        for (int j = 0; j < numSymbols; ++j)
        {
          values[j] = newTotal * origPerc[j];
        }
      }
    
      addDataPoint(rangeData, newTotal, time);
    }

    assert((int) rangeData->size() == numReturns);
    return rangeData;
  }

} // namespace alch
