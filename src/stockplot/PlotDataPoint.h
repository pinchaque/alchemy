// -*- C++ -*-

#ifndef INCLUDED_stockplot_PlotDataPoint_h
#define INCLUDED_stockplot_PlotDataPoint_h

#include "stockdata/StockTime.h"

#include "boost/shared_ptr.hpp"

#include <vector>
#include <string>

#ifndef NDEBUG
#include <iosfwd>
#endif

namespace alch {

/*!
  \brief A single data point in a stock plot
  \ingroup stockplot
*/
struct PlotDataPoint
{
  //! The timestamp for this data point (x value)
  StockTime timestamp;
  
  //! The value at this data point (y value)
  double value;


  /*!
    \brief Constructor
  */
  PlotDataPoint()
    : timestamp(boost::posix_time::second_clock::local_time())
    , value(0.00)
  {
    ;
  }

#ifndef NDEBUG
  //! Dumps object contents to stream
  void dump(std::ostream& os) const;
#endif
};

//! Shared pointer to PlotDataPoint
typedef boost::shared_ptr<PlotDataPoint> PlotDataPointPtr;

} // namespace alch

#endif
