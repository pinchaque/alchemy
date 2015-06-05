
#ifndef INCLUDED_nnet_NeuralNetWriter_h
#define INCLUDED_nnet_NeuralNetWriter_h

#include <string>
#include <cassert>

#include "nnet/NeuralNet.h"
#include "autil/Context.h"

namespace alch {

/*!
  \brief Writer for a neural network

  File format is (newlines don't matter):
  #inputs #outputs #hiddenlayers
  #units1 .. #unitsN
  weightsLayer1
  .. 
  weightsLayerN
*/
class NeuralNetWriter
{
 public:

  /*!
    \brief Constructor
    \param ctx Operation context for write operation
  */
  NeuralNetWriter(Context& ctx)
    : m_ctx(ctx)
  {
    ;
  }

  ~NeuralNetWriter()
  {
    ;
  }

  /*!
    \brief Writes to specified filename from NeuralNet object
    \param fname Filename to write to
    \param network NeuralNet to read from
    \retval true Success
    \retval false Error
  */
  bool write(const char* fname, const NeuralNetPtr& network);

 private:
  //! Operation context for reading
  Context& m_ctx;
};

} // namespace alch

#endif
