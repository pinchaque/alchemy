// -*- C++ -*-

#ifndef INCLUDED_stockdata_StockDataStream_h
#define INCLUDED_stockdata_StockDataStream_h

#include "autil/Context.h"
#include "stockdata/RangeData.h"
#include <iosfwd>

namespace alch {

/*!
  \brief Namespace with methods for parsing data from a stock data file
  \ingroup stockdata
*/
namespace StockDataStream
{

  /*!
    \brief Reads a single data point from a stream
    \param istream The input stream
    \param dataPoint The data point to populate
    \param ctx Context for this operation
    \retval true Success
    \retval false Error
  */
  bool read(std::istream& is,
            RangeData::Point& dataPoint,
            Context& ctx);

  /*!
    \brief Writes a single data point to a stream
    \param ostream The output stream
    \param dataPoint The data point to output
    \param ctx Context for this operation
    \retval true Success
    \retval false Error
  */
  bool write(std::ostream& os,
             const RangeData::Point& dataPoint,
             Context& ctx);
  
} // namespace StockDataStream

} // namespace alch

#endif
