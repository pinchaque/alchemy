// -*- C++ -*-

#ifndef INCLUDED_stockdata_BasicData_h
#define INCLUDED_stockdata_BasicData_h

#ifndef INCLUDED_vector
#include <vector>
#define INCLUDED_vector
#endif

namespace alch {

/*!
  \brief Type that encapsulates simple price points
  \ingroup stockdata

  Basic data is a vector of price points without any accompanying information,
  such as volume or times.
*/
typedef std::vector<double> BasicData;

} // namespace alch

#endif
