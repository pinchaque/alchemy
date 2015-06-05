
#include "stockalg/DownsideRisk.h"

#include "nnet/Statistics.h"

#include <vector>

namespace alch {
namespace DownsideRisk {


  int countPointsInRange(
    double start, double end, const double* returns, int n) 
  {
    int count = 0;
    for (int i = 0; i < n; ++i)
    {
      if ((returns[i] >= start) && (returns[i] <= end))
      {
        ++count;
      }
    }
    return count;
  }


  double upsidePotentialRatio(double upsidePotential, double downsideDeviation)
  {
    if (downsideDeviation != 0.0)
    {
      return (upsidePotential / downsideDeviation);
    }
    else
    {
      return 0.0;
    }
  }


  /*!
   * Returns the sortino ratio for the specified returns and risk
   */
  double sortinoRatio(
      double actualReturn, 
      double targetReturn, 
      double downsideRisk)
  {
    return ((actualReturn - targetReturn) / downsideRisk);
  }
  
  /*!
   * Returns volatility skewness for the specified set of returns.
   */
  double volatilitySkewness(const double* returns, int n)
  {
    // no returns -- all at the mean -- return 1
    if (n == 0)
    {
      return 1.0;
    }

    double avg = Statistics::mean(returns, n);

    int numAbove = 0;
    for (int i = 0; i < n; ++i)
    {
      if (returns[i] >= avg)
      {
        ++numAbove;
      }
    }
    int numBelow = n - numAbove;

    // if we didn't get any below the mean, that means they must have all
    // been right at the mean so we return 1.
    if (numBelow == 0)
    {
      return 1.0;
    }
    else
    {
      return ((double)numAbove / (double)numBelow);
    }
  }
 
  double downsideFrequency(double mar, const double* returns, int n)
  {
    if (n == 0) { return 0.0; }

    int count = 0;
    for (int i = 0; i < n; ++i)
    {
      if (returns[i] < mar)
      {
        ++count;
      }
    }

    return ((double)count / (double)n);
  }

  double avgDownsideDeviation(double mar, const double* returns, int n)
  {
    if (n == 0) { return 0.0; }

    double total = 0.0;
    int count = 0;
    for (int i = 0; i < n; ++i)
    {
      if (returns[i] < mar)
      {
        ++count;
        total += mar - returns[i];
      }
    }

    if (count == 0) { return 0.0; }

    return (total / (double)count);
  }

  void bootstrap(int chainSize, int numChains,
      const std::vector<double>& origData,
      std::vector<double>& newData)
  {
    newData.clear();
    newData.reserve(numChains);
    int n = (int)origData.size();

    if (n == 0)
    {
      return;
    }
    
    for (int i = 0; i < numChains; ++i)
    {
      double ret = 1.0;
      for (int j = 0; j < chainSize; ++j)
      {
        int idx = (int)(drand48() * n);
        assert(idx >= 0);
        assert(idx < n);

        ret += ret * origData[idx];
      }

      newData.push_back(ret - 1.0);
    }
  }

  void selectConsecutive(int chainSize, 
      const std::vector<double>& origData,
      std::vector<double>& newData)
  {
    int newSize = origData.size() - chainSize + 1;
    newData.clear();
    newData.reserve(newSize);
    int n = (int)origData.size();

    if (n == 0)
    {
      return;
    }

    int maxI = n - chainSize + 1;
    for (int i = 0; i < maxI; ++i)
    {
      double ret = 1.0;
      for (int j = 0; j < chainSize; ++j)
      {
        int idx = j + i;
        assert(idx >= 0);
        assert(idx < n);

        ret += ret * origData[idx];
      }

      newData.push_back(ret - 1.0);
    }

    assert((int)newData.size() == newSize);
  }


} // namespace DownsideRisk
} // namespace alch
