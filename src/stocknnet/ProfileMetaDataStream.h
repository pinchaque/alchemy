// -*- C++ -*-

#ifndef INCLUDED_stocknnet_ProfileMetaDataStream_h
#define INCLUDED_stocknnet_ProfileMetaDataStream_h

#include "autil/Context.h"
#include "stocknnet/PredictionProfile.h"
#include <iosfwd>

namespace alch {

/*!
  \brief Namespace with methods for parsing and writing prediction profile
  meta data
  \ingroup stocknnet
*/
namespace ProfileMetaDataStream
{

  /*!
    \brief Reads prediction profile meta data from a stream
    \param istream The input stream
    \param data The data to populate
    \param ctx Context for this operation
    \retval true Success
    \retval false Error
  */
  bool read(std::istream& is,
            PredictionProfile& data,
            Context& ctx);

  /*!
    \brief Writes prediction profile meta data to a stream
    \param ostream The output stream
    \param data The data to output
    \param ctx Context for this operation
    \retval true Success
    \retval false Error
  */
  bool write(std::ostream& os,
             const PredictionProfile& data,
             Context& ctx);
  
} // namespace ProfileMetaDataStream

} // namespace alch

#endif
