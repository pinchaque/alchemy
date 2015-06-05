// -*- C++ -*-

#ifndef INCLUDED_stockalg_Returns_h
#define INCLUDED_stockalg_Returns_h

#include "boost/shared_ptr.hpp"

#include "stockdata/RangeData.h"

namespace alch {

namespace Returns {

  boost::posix_time::time_duration duration(RangeDataPtr data);
  double annualized(RangeDataPtr data);
  double total(RangeDataPtr data);

} // namespace Returns

} // namespace alch

#endif
