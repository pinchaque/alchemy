
#include "stockalg/MovingAverage.h"

namespace alch {


namespace MovingAverage
{

  void simpleMA(const DoubleVec& data,
                int span,
                DoubleVec& output)
  {
    output.clear();

    int dataSize = data.size();

    // make sure we have enough data to do this!
    if (span > dataSize)
    {
      return;
    }

    output.reserve(dataSize - span + 1);

    // compute running total for first window
    int i = 0;
    double runningTotal = 0.00;
    for (i = 0; i < span; ++i)
    {
      runningTotal += data[i];
    }
    output.push_back(runningTotal / span);

    // now iterate through rest of data, adjusting the running total for
    // each new encountered data point and adding the average to the output
    // array
    while (i < dataSize)
    {
      runningTotal -= data[i - span];
      runningTotal += data[i];
      output.push_back(runningTotal / span);
      ++i;
    }
    
  }


  void weightedMA(const DoubleVec& data,
                  int span,
                  DoubleVec& output)
  {
    output.clear();

    int dataSize = data.size();

    // make sure we have enough data to do this!
    if (span > dataSize)
    {
      return;
    }

    output.reserve(dataSize - span + 1);

    // 1 + 2 + 3 + ... + N  = ((N * (N + 1)) / 2)
    int divisor = ((span * (span + 1)) / 2);

    // brute-force N^2 algorithm
    int start = 0;
    int end = start + span;
    while (end <= dataSize)
    {
      double runningTotal = 0.00;
      int weight = 0;
      for (int i = start; i < end; ++i)
      {
        runningTotal += (++weight * data[i]);
      }
      output.push_back(runningTotal / divisor);

      ++start;
      ++end;
    }
  }


  void exponentialMA(const DoubleVec& data,
                     int span,
                     double exponent,
                     DoubleVec& output)
  {
    output.clear();

    int dataSize = data.size();

    // make sure we have enough data to do this!
    if (span > dataSize)
    {
      return;
    }

    output.reserve(dataSize - span + 1);

    // compute running total for first window
    int i = 0;
    double runningTotal = 0.00;
    for (i = 0; i < span; ++i)
    {
      runningTotal += data[i];
    }
    double ema = (runningTotal / span);
    output.push_back(ema);

    // now iterate through rest of data. each data point is compared to the
    // existing ema, multiplied by the exponent, and added to the ema.
    while (i < dataSize)
    {
      double diff = (data[i] - ema);
      ema += (diff * exponent);
      output.push_back(ema);
      ++i;
    }
  }

} // namespace MovingAverage

} // namespace alch
