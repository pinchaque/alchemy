
#include "stockplot/PlotListRangeAdjuster.h"

#include <limits>
#include <cmath>

namespace alch {

//  using namespace boost::date_time;
  using namespace boost::posix_time;
  using namespace boost::gregorian;

  double PlotListRangeAdjuster::roundDouble(double d, bool up) const
  {
    double div = 0.00;

    switch (getValueGranularity())
    {
      case VALUEGRANULARITY_exact:
      default:
        return d;

      case VALUEGRANULARITY_tenth:
        div = 0.1;
        break;

      case VALUEGRANULARITY_one:
        div = 1.0;
        break;

      case VALUEGRANULARITY_ten:
        div = 10.0;
        break;
    }

    if (up)
    {
      return ceil(d / div) * div;
    }
    else
    {
      return floor(d / div) * div;
    }
  }

  /*
    XXX contains some horrible ternary hackery
   */
  StockTime PlotListRangeAdjuster::roundTime(StockTime t, bool up) const
  {
    date tradeDate(t.date());
    time_duration tradeTime(t.time_of_day());

    date finalDate(tradeDate);
    time_duration finalTime(time_duration(0, 0, 0));

    switch (getTimeGranularity())
    {
      case VALUEGRANULARITY_exact:
      default:
        return t;

      case TIMEGRANULARITY_decade:
        finalDate = date((tradeDate.year() / 10) * 10 + (up ? 10 : 0), 1, 1);
        break;

      case TIMEGRANULARITY_year:
        finalDate = date(tradeDate.year() + (up ? 1 : 0), 1, 1);
        break;

      case TIMEGRANULARITY_halfYear:
        if (tradeDate.month() <= 6)
        {
          finalDate = date(tradeDate.year(), (up ? 7 : 1), 1);
        }
        else
        {
          finalDate = date(tradeDate.year() + (up ? 1 : 0), (up ? 1 : 7), 1);
        }
        break;

      case TIMEGRANULARITY_quarter:
        if ((tradeDate.month() <= 9) || !up)
        {
          finalDate = date(tradeDate.year(),
                           (((tradeDate.month() - 1) / 3) * 3) + (up ? 4 : 1),
                           1);
        }
        else
        {
          finalDate = date(tradeDate.year() + 1, 1, 1);
        }
        break;

      case TIMEGRANULARITY_month:
        if ((tradeDate.month() <= 11) || !up)
        {
          finalDate = date(tradeDate.year(),
                           tradeDate.month() + (up ? 1 : 0),
                           1);
        }
        else
        {
          finalDate = date(tradeDate.year() + 1, 1, 1);
        }
        break;

      case TIMEGRANULARITY_day:
        if (up)
        {
          finalDate = tradeDate + days(1);
        }
        break;
    }

    return StockTime(finalDate, finalTime);
  }


  void PlotListRangeAdjuster::adjust(const PlotListPtr& plotList) const
  {
    assert(plotList.get());

    const StockTime maxTime(from_iso_string("99991231T125959"));
    const StockTime minTime(from_iso_string("15000101T000000"));

    StockTime xMin(maxTime);
    StockTime xMax(minTime);

    const PlotList::PlotPtrVec& plotVec = plotList->getPlotList();

    PlotList::PlotPtrVec::const_iterator end = plotVec.end();
    PlotList::PlotPtrVec::const_iterator iter;
    for (iter = plotVec.begin(); iter != end; ++iter)
    {
      adjustPlot(*iter, xMin, xMax);
    }

    if (xMin != maxTime)
    {
      plotList->setXMin(roundTime(xMin, false));
    }

    if (xMax != minTime)
    {
      plotList->setXMax(roundTime(xMax, true));
    }
  }

  void PlotListRangeAdjuster::adjustPlot(const PlotPtr& plot,
                                         StockTime& xMin,
                                         StockTime& xMax) const
  {
    double yMin = std::numeric_limits<double>::max();
    double yMax = std::numeric_limits<double>::min();

    Plot::PlotDataPtrVec plotData = plot->getPlotData();
    
    // go through all PlotData objects in the price Plot
    Plot::PlotDataPtrVec::const_iterator plotDataEnd = plotData.end();
    Plot::PlotDataPtrVec::const_iterator plotDataIter;
    for (plotDataIter = plotData.begin();
         plotDataIter != plotDataEnd;
         ++plotDataIter)
    {
      assert(plotDataIter->get());

      const PlotDataSegmentPtrVec& segments
        = (*plotDataIter)->getDataSegments();
      PlotDataSegmentPtrVec::const_iterator endDS = segments.end();
      PlotDataSegmentPtrVec::const_iterator iterDS;
      for (iterDS = segments.begin(); iterDS != endDS; ++iterDS)
      {
        assert(iterDS->get());
        PlotDataSegment::const_iterator endS = (*iterDS)->end();
        PlotDataSegment::const_iterator iterS;
        for (iterS = (*iterDS)->begin(); iterS != endS; ++iterS)
        {
          if (iterS->timestamp > xMax)
          {
            xMax = iterS->timestamp;
          }

          if (iterS->timestamp < xMin)
          {
            xMin = iterS->timestamp;
          }

          if (iterS->value > yMax)
          {
            yMax = iterS->value;
          }

          if (iterS->value < yMin)
          {
            yMin = iterS->value;
          }
        }
      }
    }

    if (yMin != std::numeric_limits<double>::max())
    {
      plot->setYMin(roundDouble(yMin, false));
    }

    if (yMax != std::numeric_limits<double>::min())
    {
      plot->setYMax(roundDouble(yMax, true));
    }
  }

} // namespace alch
