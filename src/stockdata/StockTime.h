// -*- C++ -*-

#ifndef INCLUDED_stockdata_StockTime_h
#define INCLUDED_stockdata_StockTime_h

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

namespace alch {

/*!
  \brief Time class used for all stock trades
  \ingroup stockdata

  Based on the posix_time class from boost.
*/
typedef boost::posix_time::ptime StockTime;


/*!
  \brief Allows StockTime object to be written to std::ostream
  \param os The std::ostream
  \param id The StockTime
  \ingroup stockdata
*/
inline std::ostream& operator << (std::ostream& os, const StockTime& t)
{
  os << boost::posix_time::to_simple_string(t);
  return os;
}

} // namespace alch

#endif
