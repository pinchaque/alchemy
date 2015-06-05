
#include "nnet/Statistics.h"

#include <cassert>
#include <cmath>
#include <limits>

namespace alch {

namespace Statistics
{
  double mean(const double* data, int n)
  {
    if (n <= 0)
    {
      return 0.0;
    }

    assert(n > 0);
    assert(data);

    double total = 0.0;

    for (int i = 0; i < n; ++i)
    {
      total += data[i];
    }

    return (total / n);
  }


  double variance(const double* data, int n)
  {
    if (n <= 1)
    {
      return 0.0;
    }

    assert(n > 1);
    assert(data);

    double avg = mean(data, n);

    double total = 0.0;

    for (int i = 0; i < n; ++i)
    {
      double tmp = (data[i] - avg);
      total += (tmp * tmp);
    }

    return (total / (n - 1.0));
  }


  double stddev(const double* data, int n)
  {
    return ::sqrt(variance(data, n));
  }


  double correlation(const double* data1, const double* data2, int n)
  {
    if (n <= 0)
    {
      return 0.0;
    }

    assert(n > 0);
    assert(data1);
    assert(data2);

    double totalProduct = 0.0;
    double totalX = 0.0;
    double totalY = 0.0;

    double meanX = mean(data1, n);
    double meanY = mean(data2, n);

    for (int i = 0; i < n; ++i)
    {
      double diffX = (data1[i] - meanX);
      double diffY = (data2[i] - meanY);

      totalProduct += diffX * diffY ;
      totalX += diffX * diffX;
      totalY += diffY * diffY;
    }

    double correlation = (totalProduct / (::sqrt(totalX) * ::sqrt(totalY)));

    assert(correlation >= -1.01);
    assert(correlation <= 1.01);

    return correlation;
  }

  double min(const double* data, int n)
  {
    double ret = std::numeric_limits<double>::max();

    for (int i = 0; i < n; ++i)
    {
      if (data[i] < ret)
      {
        ret = data[i];
      }
    }

    return ret;
  }

  double max(const double* data, int n)
  {
    double ret = std::numeric_limits<double>::min();

    for (int i = 0; i < n; ++i)
    {
      if (data[i] > ret)
      {
        ret = data[i];
      }
    }

    return ret;
  }

} // namespace Statistics

} // namespace alch
