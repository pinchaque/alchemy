
#include "stockalg/Risk.h"
#include "stockalg/DownsideRisk.h"
#include "stockalg/Returns.h"
#include "nnet/Statistics.h"

namespace alch {

  Risk::Risk(RangeDataPtr data, bool bootstrap, int bootMult)
    : m_mar(0.0)
    , m_data(data)
    , m_returns()
    , m_ln3()
  {
    m_data->getReturns(m_returns);

    // if bootstrapping...
    if (bootstrap)
    {
      std::vector<double> bootstrapReturns;
      DownsideRisk::bootstrap(12, m_returns.size() * bootMult,
                    m_returns, bootstrapReturns);
      m_returns = bootstrapReturns;
    }
    // otherwise we need to annualize the returns we got
    else
    {
      std::vector<double> bootstrapReturns;
      DownsideRisk::selectConsecutive(12, m_returns, bootstrapReturns);
      m_returns = bootstrapReturns;
    }

    int n = (int)m_returns.size();
    const double* returns = &m_returns[0];
    double min = Statistics::min(returns, n);
    double max = Statistics::max(returns, n);
    double mean = Statistics::mean(returns, n);
    double stddev = Statistics::stddev(returns, n);

    m_ln3 = LogNormal3Ptr(new LogNormal3(min, max, mean, stddev));
  }

  double Risk::getSquaredError() const
  {
    return DownsideRisk::squaredError(*m_ln3, &m_returns[0], m_returns.size(), 
        0.01);
  }

  double Risk::getDownsideVariation() const
  {
    return DownsideRisk::downsideVariation(*m_ln3, m_mar);
  }

  double Risk::getDownsideDeviation() const
  {
    return DownsideRisk::downsideDeviation(*m_ln3, m_mar);
  }

  double Risk::getUpsidePotential() const
  {
    return DownsideRisk::upsidePotential(*m_ln3, m_mar);
  }

  double Risk::getUpsidePotentialRatio() const
  {
    return DownsideRisk::upsidePotentialRatio(
        getUpsidePotential(), getDownsideDeviation());
  }

  double Risk::getVolatilitySkewness() const
  {
    return DownsideRisk::volatilitySkewness(&m_returns[0], m_returns.size());
  }

  double Risk::getDownsideFrequency() const
  {
    return DownsideRisk::downsideFrequency(
        m_mar, &m_returns[0], m_returns.size());
  }

  double Risk::getAvgDownsideDeviation() const
  {
    return DownsideRisk::avgDownsideDeviation(
        m_mar, &m_returns[0], m_returns.size());
  }

  double Risk::getSortinoRatio() const
  {
    double annReturn = Returns::annualized(m_data);
    return DownsideRisk::sortinoRatio(annReturn, m_mar, getDownsideDeviation());
  }



} // namespace alch
