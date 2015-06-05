
#include "DIRNAME/CLASSNAME.h"

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif

namespace alch {

#ifndef NDEBUG
  void CLASSNAME::dump(std::ostream& os) const
  {
    os << "[CLASSNAME]\n";
  }
#endif

} // namespace alch
