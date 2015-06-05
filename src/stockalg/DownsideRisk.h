// -*- C++ -*-

#ifndef INCLUDED_stockalg_DownsideRisk_h
#define INCLUDED_stockalg_DownsideRisk_h

#include <cmath>
#include <limits>
#include <vector>

#include "nnet/Statistics.h"
#include "stockalg/Integrate.h"

namespace alch {

namespace DownsideRisk
{
  /*!
   * Functor class for what we need to integrate when calculating downside
   * risk.
   */
  template <class T>
  class DownsideRiskFn
  {
    private:
    T m_distribFn;
    double m_mar;

    public:
    DownsideRiskFn(T fn, double mar)
      : m_distribFn(fn)
      , m_mar(mar)
    { }

    double operator()(double x)
    {
      return (::pow(m_mar - x, 2.0) * m_distribFn(x));
    }
  };  

  /*!
   * Functor class for what we need to integrate when calculating upside
   * potential.
   */
  template <class T>
  class UpsidePotentialFn
  {
    private:
    T m_distribFn;
    double m_mar;

    public:
    UpsidePotentialFn(T fn, double mar)
      : m_distribFn(fn)
      , m_mar(mar)
    { }

    double operator()(double x)
    {
      if (x > m_mar)
      {
        return (x - m_mar) * m_distribFn(x);
      }
      else
      {
        return 0.0;
      }
    }
  };  


  /*!
   * Calculates the downside variation based on the specified distribution
   * function, minimum acceptable risk, and appropriate setting of 
   * negative infinity.
   */
  template <class T>
  double downsideVariation(T distribFn, double mar)
  {
    DownsideRiskFn<T> riskFn(distribFn, mar);
    double negInf = distribFn.getNegInfinity();
    if (negInf >= mar)
    {
      return 0.0;
    }
    
    double val = integrate(riskFn, negInf, mar);
    if (val < 0)
    {
      return std::numeric_limits<double>::max();
    }
    else
    {
      return val;
    }
  }

  /*!
   * Calcualtes the downside deviation, which is the sqrt of the downside
   * variation
   */
  template <class T>
  double downsideDeviation(T distribFn, double mar)
  {
    double var = downsideVariation(distribFn, mar);
    assert(var >= 0.0);
    return ::sqrt(var);
  }

  /*!
   * Calculate sthe upside potential of the specified distribution function
   * using the specified minimum return and positive infinity.
   */
  template <class T>
  double upsidePotential(T distribFn, double mar)
  {
    double posInf = distribFn.getPosInfinity();
    UpsidePotentialFn<T> potentialFn(distribFn, mar);
    if (mar >= posInf)
    {
      return 0.0;
    }
    else
    {
      return integrate(potentialFn, mar, posInf);
    }
  }

  /*!
   * Calculates the squared error between a distribution function and the
   * actual returns, given a bin size.
   */
  template <class T>
  double squaredError(T distribFn, const double* returns, int n, double binSize)
  {
    assert(binSize > 0.0);
    double intBinSize = binSize / 10.0;
    double negInf = distribFn.getNegInfinity();
    double posInf = distribFn.getPosInfinity();
    assert(negInf <= posInf);
    double total = 0.0;

    for (double start = negInf; start < posInf; start += binSize)
    {
      double end = start + binSize;
      double predicted = integrate(distribFn, start, end, intBinSize);
      double actual =
        (static_cast<double>(countPointsInRange(start, end, returns, n))
         / static_cast<double>(n));

      /*
      std::cerr << "range = " << start << ", " << end << std::endl;
      std::cerr << "predicted = " << predicted << std::endl;
      std::cerr << "actual = " << actual << std::endl;
      */

      total += ::pow(predicted - actual, 2.0);
    }

    assert(total >= 0.0);
    return ::sqrt(total);
  }

  /*!
   * Counts the number of datapoints in the specified range.
   */
  int countPointsInRange(
      double start, double end, const double* returns, int n);


  /*!
   * Calculates the upside potential ratio
   */
  double upsidePotentialRatio(double upsidePotential, double downsideDeviation);

  /*!
   * Returns the sortino ratio for the specified returns and risk
   */
  double sortinoRatio(
      double actualReturn, 
      double targetReturn, 
      double downsideDeviation);
  
  /*!
   * Returns volatility skewness for the specified set of returns.
   */
  double volatilitySkewness(const double* returns, int n);

  /*!
   * Returns the ratio (0..1) of times that the returns were below the
   * specified MAR.
   */
  double downsideFrequency(double mar, const double* returns, int n);

  /*!
   * Returns the average amount below mar that the returns were.
   */
  double avgDownsideDeviation(double mar, const double* returns, int n);
  
  /*!
   * Bootstraps a new dataset by sampling from origData. newData will be
   * populated with the results of sampling numChains number of return
   * chains, each chain of chainSize returns.  The returns are cumulative,
   * much like they would be when adding up monthly returns in a year.
   */
  void bootstrap(int chainSize, int numChains,
      const std::vector<double>& origData,
      std::vector<double>& newData);

  /*!
   * Selects consecutive chains of returns from origData
   */
  void selectConsecutive(int chainSize,
      const std::vector<double>& origData,
      std::vector<double>& newData);
  
} // namespace DownsideRisk
} // namespace alch

#endif
