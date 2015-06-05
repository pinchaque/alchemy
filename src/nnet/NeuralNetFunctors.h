// -*- C++ -*-

#ifndef INCLUDED_nnet_NeuralNetFunctors_h
#define INCLUDED_nnet_NeuralNetFunctors_h

#include <cmath>

namespace alch {

namespace NeuralNetFunctors {

  //! Linear activation function
  struct Linear
  {
    double operator()(double val)
    {
      return val;
    }
  };

  //! Hyperbolic tangent activation function
  struct Tanh
  {
    double operator()(double val)
    {
      return ::tanh(val);
    }
  };

} // namespace NeuralNetFunctors

} // namespace alch

#endif
