// -*- C++ -*-

#ifndef INCLUDED_stockplot_PlotDataSegment_h
#define INCLUDED_stockplot_PlotDataSegment_h

#include "stockplot/PlotDataPoint.h"

#include "boost/shared_ptr.hpp"

#include <vector>

namespace alch {

/*!
  \brief A contiguous series of DataPoint objects
  \ingroup stockplot
*/
typedef std::vector<PlotDataPoint> PlotDataSegment;


/*!
  \brief Shared pointer to PlotDataSegment
  \ingroup stockplot
*/
typedef boost::shared_ptr<PlotDataSegment> PlotDataSegmentPtr;


/*!
  \brief A vector of DataSegment
  \ingroup stockplot
*/
typedef std::vector<PlotDataSegmentPtr> PlotDataSegmentPtrVec;


} // namespace alch

#endif
