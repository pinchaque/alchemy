// -*- C++ -*-

#ifndef INCLUDED_stockplot_PlotListRangeAdjuster_h
#define INCLUDED_stockplot_PlotListRangeAdjuster_h

#include "stockplot/PlotList.h"

#include "boost/shared_ptr.hpp"

namespace alch {

/*!
  \brief Adjusts the data ranges in a PlotList and contained Plot objects
  \ingroup stockplot
*/
class PlotListRangeAdjuster
{
 public:

  enum TimeGranularity
  {
    TIMEGRANULARITY_exact = 0,
    TIMEGRANULARITY_decade,
    TIMEGRANULARITY_year,
    TIMEGRANULARITY_halfYear,
    TIMEGRANULARITY_quarter,
    TIMEGRANULARITY_month,
    TIMEGRANULARITY_day
  };

  enum ValueGranularity
  {
    VALUEGRANULARITY_exact = 0,
    VALUEGRANULARITY_tenth,
    VALUEGRANULARITY_one,
    VALUEGRANULARITY_ten
  };

  /*!
    \brief Constructor
  */
  PlotListRangeAdjuster()
    : m_timeGranularity(TIMEGRANULARITY_month)
    , m_valueGranularity(VALUEGRANULARITY_tenth)
  {
    ;
  }

  virtual ~PlotListRangeAdjuster()
  {
    ;
  }

  //! Sets the granularity to use for rounding time values
  void setTimeGranularity(TimeGranularity val)
  {
    m_timeGranularity = val;
  }

  //! Gets the granularity used for rounding time values
  TimeGranularity getTimeGranularity() const
  {
    return m_timeGranularity;
  }

  //! Sets the granularity to use for rounding values
  void setValueGranularity(ValueGranularity val)
  {
    m_valueGranularity = val;
  }

  //! Gets the granularity used for rounding values
  ValueGranularity getValueGranularity() const
  {
    return m_valueGranularity;
  }


  /*!
    \brief Adjusts the range values in specified plot list
    \param plotList The PlotList to adjust
    
    The adjustments are made based on the granularity set within this object.
    The minimum values will be rounded down to the nearest Granularity
    level, and the maximum values will be rounded up to the nearest
    Granularity level.

    This method will recurse through all Plot and subsidiary objects to
    look at all data points, setting the appropriate X and Y min/max values
    in the process. While that is done, the granularity rounding is performed.
  */
  void adjust(const PlotListPtr& plotList) const;


  /*!
    \brief Adjusts the range within a single plot
    \param plot The plot to adjust
    \param xMin [in/out] The minimum X value
    \param xMax [in/out] The maximum X value

    This method will iterate through all data points in the specified plot
    and adjust the Y range within that plot to account for them. It will
    also update xMin and xMax for these data points for usage in the PlotList.
  */
  void adjustPlot(const PlotPtr& plot, StockTime& xMin, StockTime& xMax) const;


  /*!
    \brief Rounds a double to the value as specified by granularity
    \param d The double to round
    \param up Whether to round up or down
    \return The rounded double
    
    Rounding policy is controlled by the setting of m_valueGranularity.
  */
  double roundDouble(double d, bool up) const;


  /*!
    \brief Rounds a time to the value as specified by granularity
    \param t The time to round
    \param up Whether to round up or down
    \return The rounded time
    
    Rounding policy is controlled by the setting of m_timeGranularity.
  */
  StockTime roundTime(StockTime t, bool up) const;
 
 private:

  TimeGranularity m_timeGranularity;

  ValueGranularity m_valueGranularity;
};


} // namespace alch

#endif
