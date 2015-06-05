
#ifndef INCLUDED_nnet_NeuralNetReader_h
#define INCLUDED_nnet_NeuralNetReader_h

#include <string>
#include <cassert>

#include "nnet/NeuralNet.h"
#include "autil/Context.h"

namespace alch {

/*!
  \brief Reader for a neural network

  File format is (newlines don't matter):
  #inputs #outputs #hiddenlayers
  #units1 .. #unitsN
  weightsLayer1
  .. 
  weightsLayerN
*/
class NeuralNetReader
{
 public:

  /*!
    \brief Constructor
    \param ctx Operation context for read operation
  */
  NeuralNetReader(Context& ctx)
    : m_ctx(ctx)
  {
    ;
  }

  ~NeuralNetReader()
  {
    ;
  }

  /*!
    \brief Reads specified filename into NeuralNet object
    \param fname Filename to read from
    \param network NeuralNet to populate
    \retval true Success
    \retval false Error
  */
  bool read(const char* fname, NeuralNetPtr& network);

 private:
  //! Operation context for reading
  Context& m_ctx;
};

} // namespace alch

#endif
