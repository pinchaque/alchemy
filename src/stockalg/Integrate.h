// -*- C++ -*-

#ifndef INCLUDED_stockalg_Integrate_h
#define INCLUDED_stockalg_Integrate_h

#include "boost/shared_ptr.hpp"

#ifndef NDEBUG
#include <iosfwd>
#endif

namespace alch {

  /*!
   * Integrates specified function between start and end. Uses columns of
   * width binSize. Function is called with the parentheses operator.
   */
  template <class T>
  double integrate(
    T fn,
    double start, 
    double end, 
    double binSize = 0.001)
  {
    assert(start <= end);
    if (start == end)
    {
      return 0.0;
    }

    double total = 0.00;
    for (double d = start; d < end; d += binSize)
    {
      double binStart = d;
      double binEnd = std::min(d + binSize, end);
      double binWidth = (binEnd - binStart);
      double binMid = binStart + (binWidth / 2.0);

      double valStart = fn(binStart);
      double valMid = fn(binMid);
      double valEnd = fn(binEnd);
      //std::cerr << "fn[" << binMid << "] = " << valMid << "\n";

      total += (binWidth * (valStart + (4.0 * valMid) + valEnd)) / 6.0;
    }

    return total;

  }
} // namespace alch

#endif
