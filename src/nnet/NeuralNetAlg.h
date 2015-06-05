//

#ifndef INCLUDED_nnet_NeuralNetAlg_h
#define INCLUDED_nnet_NeuralNetAlg_h

#include "nnet/NeuralNet.h"
#include "nnet/NNetDataset.h"
#include <vector>
#include <cassert>
#include <stdlib.h>

#ifndef NDEBUG
#include <iostream>
#endif

namespace alch {

/*!
  \brief Neural Network algorithms
*/
namespace NeuralNetAlg
{

  /*!
    \brief Initializes the neural network algorithms
  */
  void init();

  /*!
    \brief Randomizes the weights of the specified neural network
    \param net The network for which we are randomizing the weights
    \param min The minimum weight value
    \param max The maximum weight value

    Assumes net is fully initialized. Each existing weight in net will
    be set to a value between min and max.
  */
  void randomizeWeights(NeuralNet& net, double min, double max);


  /*!
    \brief Calculates the squared error for the network on a dataset
    \param net The neural network
    \param dataset The dataset to calculate

    The network is used to calculate the outputs for each input in the
    dataset. The network output is compared to the datapoint output
    and the difference is used to calculate the squared error.
  */
  double calculateError(NeuralNet& net, const NNetDataset& dataset);
 

  /*!
    \brief Calculates sum-of-squares error for two arrays
    \param value The actual values that have been produced
    \param target The target values
    \param n The number of elements in value and target to compare
    \return Error

    This error metric grows with the size of the dataset.
  */
  double sumOfSquares(const double* value, const double* target, int n);


  /*!
    \brief Calculates squared error for two arrays
    \param value The actual values that have been produced
    \param target The target values
    \param n The number of elements in value and target to compare
    \return Error

    This error metric is divided by the number of data points so it should
    be relatively independent of the data size.
  */
  double squaredError(const double* value, const double* target, int n);


  /*!
    \brief Calculates RMS error for two arrays
    \param value The actual values that have been produced
    \param target The target values
    \param n The number of elements in value and target to compare
    \return Error

    This error metric is divided by the sum-of-squares difference in the
    target values from their mean, and as such should be better at comparing
    between different datasets.
  */
  double rmsError(const double* value, const double* target, int n);


  /*!
    \brief Calculates Minkowski error with R=1 for two arrays
    \param value The actual values that have been produced
    \param target The target values
    \param n The number of elements in value and target to compare
    \return Error

    This error metric does not square the errors, which makes it less 
    susceptible to outlying data points.
  */
  double minkowskiR1(const double* value, const double* target, int n);


  /*!
    \brief Calculates absolute value error for two arrays
    \param value The actual values that have been produced
    \param target The target values
    \param n The number of elements in value and target to compare
    \return Error

    This error metric does not square the errors, which makes it less 
    susceptible to outlying data points. The error is divided by the number
    of data points to make error value independent of data size.
  */
  double absError(const double* value, const double* target, int n);

 
  /*!
    \brief Calculates output value for each point in dataset
    \param net The neural network to use for calculation
    \param dataset [in/out] The dataset to use for input propagation and
    output storage

    Clears outputs before writing to it.
  */
  void calculateOutputs(NeuralNet& net,
                        NNetDataset& dataset);
}

} // namespace alch

#endif
