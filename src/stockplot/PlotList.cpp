
#include "stockplot/PlotList.h"

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif

namespace alch {

#ifndef NDEBUG
  void PlotList::dump(std::ostream& os) const
  {
    os << "[PlotList]  Title: '" << getTitle() << "'  Range: ("
       << m_xMin << ", " << m_xMax << "\n";
  }
#endif

} // namespace alch
