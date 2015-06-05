
#include "stockplot/Plot.h"

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif

namespace alch {

#ifndef NDEBUG
  void Plot::dump(std::ostream& os) const
  {
    os << "[Plot]  Y-Range: (" 
       << getYMin() << ", " << getYMax() << "  #Plots: "
       << m_plotData.size() << "\n";
  }
#endif

} // namespace alch
