
#include "stockalg/Momentum.h"

#include <cmath>

namespace alch {


namespace Momentum
{

  void rateOfChange(const DoubleVec& data,
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

    output.reserve(dataSize - span);

    for (int i = span; i < dataSize; ++i)
    {
      output.push_back(data[i] / data[i - span]);
    }
  }


  void relativeStrength(const DoubleVec& data,
                        int span,
                        DoubleVec& output)
  {
    output.clear();

    int dataSize = data.size();

    // make sure we have enough data to do this!
    // we need the first datapoint to be able to tell "up" from "down"
    if ((span + 1) > dataSize)
    {
      return;
    }

    // totals of all up and down days
    int numUpValues = 0;
    double totalUpValues = 0.00;
    int numDownValues = 0;
    double totalDownValues = 0.00;

    // iterate through all data points. If we are in the first "span" points
    // we simply accumulate the ups and downs. Otherwise we add to the 
    // output vector.
    for (int i = 1; i < dataSize; ++i)
    {
      // change is positive if it was an "up" day
      double change = data[i] - data[i - 1];

      if (change < 0.00)
      {
        ++numDownValues;
        totalDownValues += -change;
      }
      else
      {
        ++numUpValues;
        totalUpValues += change;
      }

      // if we don't yet have the first span points, we continue
      if (i < span)
      {
        continue;
      }

      // we now add the data point to output
      double RSI = 0.00;

      if (!numUpValues)
      {
        RSI = 0.00;
      }
      else if (!numDownValues || (totalDownValues == 0.00))
      {
        RSI = 100.00;
      }
      else
      {
        assert(numUpValues);
        assert(numDownValues);

        double RS = ((totalUpValues / numUpValues)
                     / (totalDownValues / numDownValues));
        RSI = 100.0 - (100.0 / (1.0 + RS));
      }

      output.push_back(RSI);

      // now we have to remove the value that happened "span" points ago
      // we don't know whether it was up or down, so we recalc that
      double oldChange = data[i - span + 1] - data[i - span];
      if (oldChange < 0.00)
      {
        --numDownValues;
        totalDownValues -= -oldChange;
      }
      else
      {
        --numUpValues;
        totalUpValues -= oldChange;
      }
    }
  }


  namespace {
    inline bool approxEqual(double a, double b)
    {
      return (::fabs(a - b) < 0.01);
    }
  }


  void parabolicSAR(const RangeData& data,
                    double accel,
                    double maxAccel,
                    DoubleVec& output)
  {
    output.clear();

    int dataSize = data.size();

    if (dataSize <= 0)
    {
      return;
    }

    output.reserve(dataSize);

    // starting values
    bool longTrade = true;
    double currAccel = accel;
    double extremePoint = longTrade ? data.get(0).min : data.get(0).max;

    int idx = 1;

    while (idx < dataSize)
    {
      // reset parameters for this trend
      output.push_back(extremePoint);
      currAccel = accel;
      extremePoint = longTrade ? data.get(idx).max : data.get(idx).min;

      // handle all points in a long trade
      if (longTrade)
      {
        bool done = false;

        while ((idx < dataSize) && !done)
        {
          const RangeData::Point& prevPoint = data.get(idx - 1);
          const RangeData::Point& point = data.get(idx);

          // previous SAR
          double SAR = output[idx - 1];
          
          // calculate current SAR value
          SAR += currAccel * (prevPoint.max - SAR);

          // SAR is not moved into trading range over this and previous day
          {
            double localMin = std::min(prevPoint.min, point.min);
            if (SAR > localMin)
            {
              SAR = localMin;
            }
          }

          // update extreme point and acceleration
          if (point.max > extremePoint)
          {
            extremePoint = point.max;
            currAccel += accel;
          }

          // we are done when the data crosses over the SAR
          done = approxEqual(point.min, SAR) || (SAR > point.min);

          if (!done)
          {
            // add the SAR to the output array
            output.push_back(SAR);
          }

          // look at next data point
          idx++;
        }
      }
      // handle all points in a short trade
      else // if !longTrade
      {
        bool done = false;

        while ((idx < dataSize) && !done)
        {
          const RangeData::Point& prevPoint = data.get(idx - 1);
          const RangeData::Point& point = data.get(idx);

          // previous SAR
          double SAR = output[idx - 1];
          
          // calculate current SAR value
          SAR += currAccel * (prevPoint.min - SAR);

          // SAR is not moved into trading range over this and previous day
          {
            double localMax = std::max(prevPoint.max, point.max);
            if (SAR < localMax)
            {
              SAR = localMax;
            }
          }

          // update extreme point and acceleration
          if (point.min < extremePoint)
          {
            extremePoint = point.min;
            currAccel += accel;
          }

          // we are done when the data crosses over the SAR
          done = approxEqual(point.max, SAR) || (SAR < point.max);

          if (!done)
          {
            // add the SAR to the output array
            output.push_back(SAR);
          }

          // look at next data point
          idx++;
        }
      }

      // switch from short to long and vice versa
      longTrade = !longTrade;
    }


    // pad out the rest of the output with extremePoint. There should be only
    // one needed at most. This is needed in the case where we had a PSAR
    // crossover on the last data point.
    if (int(output.size()) < dataSize)
    {
      output.push_back(extremePoint);
    }
    assert(int(output.size()) == dataSize);
  }


} // namespace Momentum

} // namespace alch
