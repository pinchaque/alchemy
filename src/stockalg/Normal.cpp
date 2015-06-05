
#include "stockalg/Normal.h"

#include <cmath>

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif

namespace alch {

  const double c_pi = 3.141592653589793238;
  double Normal::eval(double x) const
  {
    double ret = 1.0 / (m_stddev * ::sqrt(2.0 * c_pi));
    ret *= exp(-0.5 * ::pow((x - m_mean) / m_stddev, 2.0));
    return ret;
  }

} // namespace alch
