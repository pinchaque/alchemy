
#include "stockplot/PlotData.h"

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif

namespace alch {

#ifndef NDEBUG
  void PlotData::dump(std::ostream& os) const
  {
    os << "[PlotData] Label: '" << getLabel() << "'  Style: '" << getStyle()
       << "'  #Segments: " << getDataSegments().size() << "\n";
  }
#endif

} // namespace alch
