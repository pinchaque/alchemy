// -*- C++ -*-

#ifndef INCLUDED_nnet_GradDescent_h
#define INCLUDED_nnet_GradDescent_h

#include "nnet/NNetDataset.h"
#include "nnet/NeuralNet.h"

#include "boost/shared_ptr.hpp"

#include <vector>
#include <cassert>

namespace alch {

/*!
  \brief Class that performs gradient descent to train a neural network.
*/
class GradDescent
{
 public:
  /*!
    \brief Constructor
    \param network The neural network on which we're performing gradient
    descent.
  */
  GradDescent(NeuralNetPtr network, double eta = 0.001)
    : m_network(network)
    , m_delta()
    , m_weightDelta()
    , m_eta(eta)
  {
    assert(m_network.get());
    resize();
  }


  /*!
    \brief Destructor
  */
  virtual ~GradDescent()
  {
  }


  //! Returns value for Eta (training step size)
  double getEta() const
  {
    return m_eta;
  }


  //! Sets value for Eta (training step size)
  void setEta(double val)
  {
    m_eta = val;
  }


  /*!
    \brief Runs a single iteration for given dataset
    \param data The dataset to run against

    This method will train the network over one iteration for the given
    dataset.
  */
  virtual void run(const NNetDataset& data);


  /*!
    \brief Runs a single iteration for given dataset
    \param data The dataset to run against
    \param eta The multiplier to use for training the weights

    This method will train the network over one iteration for the given
    dataset.
  */
  void run(const NNetDataset& data, double eta);


protected:

  //! Returns neural network we're operating on
  NeuralNetPtr getNetwork()
  {
    return m_network;
  }

  //! Sets neural network we're operating on
  void setNetwork(NeuralNetPtr val)
  {
    m_network = val;
  }


private:
  //! Resets values in m_delta to 0.00
  void clearDelta();

  //! Resets values in m_weightDelta to 0.00
  void clearWeightDelta();

  //! Resizes internal data for m_network
  void resize();

  /*!
    \brief Updates all weights with values from m_weightDelta
    \param eta multiplier to use when updating the weights
  */
  void updateWeights(double eta);

  /*!
    \brief Computes delta for the output layer only
    \param target The target outputs

    Uses current outputs of network.
  */
  void computeOutputDelta(const std::vector<double>& target);

  //! Computes full m_delta array based on m_network
  void computeDelta();

  //! Adds full m_delta array to m_weightDelta
  void addDeltaToTotal();

  //! The neural network on which we're performing gradient
  NeuralNetPtr m_network;

  //! Value of delta for each of unit. m_delta[layer][unit]
  std::vector<std::vector<double> > m_delta;

  //! Total adjustment to weight summed over all data points for each unit.
  std::vector<NeuralNet::LayerWeight> m_weightDelta;

  //! "eta" value that controls how fast we converge
  double m_eta;
};

//! Shared pointer to GradDescent
typedef boost::shared_ptr<GradDescent> GradDescentPtr;

} // namespace alch

#endif
