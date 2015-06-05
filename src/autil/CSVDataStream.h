// -*- C++ -*-

#ifndef INCLUDED_autil_CSVDataStream_h
#define INCLUDED_autil_CSVDataStream_h

#include "autil/Context.h"
#include "autil/CSVData.h"
#include <iosfwd>

namespace alch {

/*!
  \brief Namespace with methods for parsing and writing csv data
  \ingroup autil
*/
namespace CSVDataStream
{

  /*!
    \brief Reads csv data from a stream
    \param istream The input stream
    \param data The data to populate
    \param ctx Context for this operation
    \retval true Success
    \retval false Error
  */
  bool read(std::istream& is,
            CSVData& data,
            Context& ctx);

  /*!
    \brief Writes csv data to a stream
    \param ostream The output stream
    \param data The data to output
    \param ctx Context for this operation
    \retval true Success
    \retval false Error
  */
  bool write(std::ostream& os,
             const CSVData& data,
             Context& ctx);
  std::string quote(const std::string& str);
  std::string unquote(const std::string& str);

} // namespace CSVDataStream

} // namespace alch

#endif
