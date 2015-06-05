// -*- C++ -*-

#ifndef INCLUDED_stockdata_RangeDataAlg_h
#define INCLUDED_stockdata_RangeDataAlg_h

#include "stockdata/RangeData.h"

namespace alch {

namespace RangeDataAlg
{
  /*!
    Finds all points in common between d1 and d2 and writes them to f1
    and f2. Assumes d1 and d2 are in chronological order. Finds points
    with the exact same timestamp. 
    At the end, f1 and f2 will have:
    - Same number of data points
    - Datapoints are in the same ascending chronological order
    - Datapoints at the same index represent the same point in time.

    Debug messages are logged so you can tell what happened
  */
  bool findCommon(const RangeData& d1, const RangeData& d2,
    RangeData& f1, RangeData& f2);
}

} // namespace alch

#endif
