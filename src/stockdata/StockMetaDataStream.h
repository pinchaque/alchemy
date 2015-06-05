// -*- C++ -*-

#ifndef INCLUDED_stockdata_StockMetaDataStream_h
#define INCLUDED_stockdata_StockMetaDataStream_h

#include "autil/Context.h"
#include "stockdata/StockMetaData.h"
#include <iosfwd>

namespace alch {

/*!
  \brief Namespace with methods for parsing and writing stock meta data
  \ingroup stockdata
*/
namespace StockMetaDataStream
{

  /*!
    \brief Reads stock meta data from a stream
    \param istream The input stream
    \param data The data to populate
    \param ctx Context for this operation
    \retval true Success
    \retval false Error
  */
  bool read(std::istream& is,
            StockMetaData& data,
            Context& ctx);

  /*!
    \brief Writes stock meta data to a stream
    \param ostream The output stream
    \param data The data to output
    \param ctx Context for this operation
    \retval true Success
    \retval false Error
  */
  bool write(std::ostream& os,
             const StockMetaData& data,
             Context& ctx);
} // namespace StockMetaDataStream

} // namespace alch

#endif
