// -*- C++ -*-

#ifndef INCLUDED_nnet_NeuralNet_h
#define INCLUDED_nnet_NeuralNet_h

#include "nnet/NeuralNetFunctors.h"
#include "nnet/NeuralNetTemplate.h"

namespace alch {


//! Neural network using tanh hidden and output activation function
typedef 
  NeuralNetTemplate<NeuralNetFunctors::Tanh, NeuralNetFunctors::Tanh>
  TanhNeuralNet;

//! Shared pointer to neural network
typedef boost::shared_ptr<TanhNeuralNet> TanhNeuralNetPtr;

//! Neural network using tanh hidden and linear output activation function
typedef
  NeuralNetTemplate<NeuralNetFunctors::Tanh, NeuralNetFunctors::Linear>
  LinearNeuralNet;

//! Shared pointer to neural network
typedef boost::shared_ptr<LinearNeuralNet> LinearNeuralNetPtr;

//! Default neural network type
typedef TanhNeuralNet NeuralNet;

//! Shared pointer to neural network
typedef boost::shared_ptr<NeuralNet> NeuralNetPtr;

} // namespace alch

#endif
