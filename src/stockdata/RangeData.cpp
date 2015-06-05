
#include "stockdata/RangeData.h"

#include <limits>
#include <algorithm>
#include <cmath>

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif

namespace alch {

  namespace {
    bool earlier(const RangeData::Point& p1, const RangeData::Point& p2)
    {
      return (p1.tradeTime < p2.tradeTime);
    }

    bool approxEqual(double a, double b, double delta)
    {
      return (::fabs(a - b) <= delta);
    }
  }

  bool RangeData::Point::operator == (const RangeData::Point& other) const
  {
    const double priceDelta = 0.0001;
    const double volumeDelta = 0.01;

    return ((approxEqual(other.open, open, priceDelta))
            && (approxEqual(other.close, close, priceDelta))
            && (approxEqual(other.min, min, priceDelta))
            && (approxEqual(other.max, max, priceDelta))
            && (approxEqual(other.volume, volume, volumeDelta))
            && (approxEqual(other.adjustedClose, adjustedClose, priceDelta))
            && (other.tradeTime == tradeTime)
      );
  }


  bool RangeData::operator == (const RangeData& other) const
  {
    if (size() != other.size())
    {
      return false;
    }

    size_type n = size();
    for (size_type i = 0; i < n; ++i)
    {
      if (get(i) != other.get(i))
      {
        return false;
      }
    }

    return true;
  }


  void RangeData::useAdjusted()
  {
    PointVector::iterator end = m_data.end();
    PointVector::iterator iter;
    for (iter = m_data.begin(); iter != end; ++iter)
    {
      iter->close = iter->adjustedClose;
    }
  }


  void RangeData::summarize(RangeData& newData, int points) const
  {
    assert(points > 0);

    newData.clear();

    // nothing to do if we have no data!
    if (size() <= 0)
    {
      return;
    }

    size_type totalPoints = size();

    size_type startIdx = 0;

    while (startIdx < totalPoints)
    {
      // [startIdx, endIdx] are the data points we're summarizing
      size_type endIdx = startIdx + points - 1;
      if (endIdx >= totalPoints)
      {
        endIdx = totalPoints - 1;
      }

      // actual number of data points (may be less than points for last
      // batch)
      
      // the new data point
      Point point = getSummaryPoint(startIdx, endIdx);

      // add the data point!
      newData.add(point);

      // update startIdx to consider next window
      startIdx = endIdx + 1;
    }
  }

  RangeData::Point RangeData::getSummaryPoint(
      size_type startIdx, size_type endIdx) const
  {
    int numPoints = (endIdx - startIdx + 1);
    RangeData::Point point;

    // open is the open of the starting point
    point.open = get(startIdx).open;

    // close is the close of the ending point
    point.close = get(endIdx).close;
    point.adjustedClose = get(endIdx).adjustedClose;

    // initialize min, max, and volume to be values in the ending point
    point.min = get(endIdx).min;
    point.max = get(endIdx).max;
    point.volume = get(endIdx).volume;
    point.tradeTime = get(endIdx).tradeTime;

    // iterate through all points except end, updating min, max, volume
    for (size_type i = startIdx; i < endIdx; ++i)
    {
      const Point& testPoint = get(i);

      if (testPoint.min < point.min)
      {
        point.min = testPoint.min;
      }

      if (testPoint.max > point.max)
      {
        point.max = testPoint.max;
      }

      point.volume += testPoint.volume;
    }

    // now we average the volume
    point.volume /= numPoints;

    return point;
  }

  void RangeData::summarizeMonthly(RangeData& newData) const
  {
    newData.clear();

    // nothing to do if we have no data!
    if (size() <= 0)
    {
      return;
    }

    size_type totalPoints = size();
    size_type startIdx = 0;
    size_type endIdx = 1;
    int currMonth = get(0).tradeTime.date().month();

    while (endIdx < totalPoints)
    {
      int newMonth = get(endIdx).tradeTime.date().month();

      // if we are in a new month, then we create a point for up to the
      // previous end index
      if (newMonth != currMonth)
      {
        // the new data point
        Point point = getSummaryPoint(startIdx, endIdx - 1);

        // add the data point!
        newData.add(point);

        // update startIdx to consider next window
        startIdx = endIdx;
        currMonth = newMonth;
      }

      ++endIdx;
    }

    newData.add(getSummaryPoint(startIdx, endIdx - 1));
  }


  void RangeData::sortDate()
  {
    std::sort(begin(), end(), earlier);
  }


  void RangeData::merge(const RangeData& other, bool removeDups)
  {
    RangeData mergedData;
    mergedData.resize(size() + other.size());

    std::merge(begin(), end(),
               other.begin(), other.end(),
               mergedData.begin(), earlier);

    RangeData::iterator newEnd = mergedData.end();

    if (removeDups)
    {
      newEnd = std::unique(mergedData.begin(), mergedData.end());
    }

    m_data.resize(std::distance(mergedData.begin(), newEnd));
    std::copy(mergedData.begin(), newEnd, m_data.begin());
  }

  void RangeData::getReturns(std::vector<double>& returns) const
  {
    returns.clear();

    PointVector::const_iterator iter = m_data.begin();
    PointVector::const_iterator end = m_data.end();

    if (iter == end)
    {
      return;
    }

    double lastClose = iter->close;
    ++iter;

    while (iter != end)
    {
      if (lastClose == 0.0)
      {
        returns.push_back(0.0);
      }
      else
      {
        returns.push_back((iter->close - lastClose) / lastClose);
      }

      lastClose = iter->close;

      ++iter;
    }
  }


#ifndef NDEBUG

  void RangeData::Point::dump(std::ostream& os) const
  {
    os << std::setprecision(4) << std::showpoint
       << "[RangeData::Point]"
       << "  time:" << boost::posix_time::to_iso_string(tradeTime)
       << "  open:" << open
       << "  close:"   << close
       << "  min:" << min
       << "  max:" << max
       << "  volume:" << volume
       << "  adjustedClose:" << adjustedClose
       << "\n";
  }

  void RangeData::dump(std::ostream& os) const
  {
    os << "[RangeData]\n";
    PointVector::const_iterator end = m_data.end();
    PointVector::const_iterator iter;
    for (iter = m_data.begin(); iter != end; ++iter)
    {
      os << "  ";
      iter->dump(os);
    }
  }

#endif

} // namespace alch
