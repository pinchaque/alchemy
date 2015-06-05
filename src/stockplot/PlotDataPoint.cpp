
#include "stockplot/PlotDataPoint.h"

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif

namespace alch {

#ifndef NDEBUG
  void PlotDataPoint::dump(std::ostream& os) const
  {
    os << "[PlotDataPoint] ("
       << boost::posix_time::to_simple_string(timestamp)
       << ", " << value << ")\n";
  }
#endif

} // namespace alch
