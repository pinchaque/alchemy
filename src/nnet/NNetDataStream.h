// -*- C++ -*-

#ifndef INCLUDED_nnet_NNetDataStream_h
#define INCLUDED_nnet_NNetDataStream_h

#include "autil/Context.h"
#include "nnet/NNetDataset.h"
#include <iosfwd>

namespace alch {

/*!
  \brief Namespace with methods for parsing neural network dataset
  \ingroup nnet
*/
namespace NNetDataStream
{

  /*!
    \brief Reads a single data point from a stream
    \param istream The input stream
    \param dataPoint The data point to populate
    \param ctx Context for this operation
    \retval true Success
    \retval false Error
  */
  bool readDatapoint(std::istream& is,
                     NNetDatapoint& dataPoint,
                     Context& ctx);

  /*!
    \brief Writes a single data point to a stream
    \param ostream The output stream
    \param dataPoint The data point to output
    \param ctx Context for this operation
    \retval true Success
    \retval false Error
  */
  bool writeDatapoint(std::ostream& os,
                      const NNetDatapoint& dataPoint,
                      Context& ctx);

  /*!
    \brief Reads an entire dataset from a stream
    \param istream The input stream
    \param dataset The dataset to populate
    \param ctx Context for this operation
    \retval true Success
    \retval false Error
  */
  bool readDataset(std::istream& is,
                   NNetDataset& dataset,
                   Context& ctx);

  /*!
    \brief Writes an entire to a stream
    \param ostream The output stream
    \param dataset The dataset to output
    \param ctx Context for this operation
    \retval true Success
    \retval false Error
  */
  bool writeDataset(std::ostream& os,
                      const NNetDataset& dataset,
                      Context& ctx);
  


} // namespace NNetDataStream

} // namespace alch

#endif
