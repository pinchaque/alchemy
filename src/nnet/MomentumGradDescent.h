// -*- C++ -*-

#ifndef INCLUDED_nnet_MomentumGradDescent_h
#define INCLUDED_nnet_MomentumGradDescent_h

#include <vector>
#include <cassert>
#include "nnet/NNetDataset.h"
#include "nnet/NeuralNet.h"
#include "nnet/GradDescent.h"

namespace alch {

/*!
  \brief Class that performs gradient descent with momentum to train a neural 
  network.
*/
class MomentumGradDescent : public GradDescent
{
 public:
  /*!
    \brief Constructor
    \param network The neural network on which we're performing gradient
    descent.
    \param eta The initial learning rate parameter
    \param alpha The multiplier when error rate is decreasing
    \param beta the multiplier when error rate is increasing
  */
  MomentumGradDescent(NeuralNetPtr network, 
                      double eta = 0.001,
                      double alpha = 1.10, 
                      double beta = 0.50)
    : GradDescent(network, eta)
    , m_alpha(alpha)
    , m_beta(beta)
    , m_lastTrainError(99999.9)
  {
    ;
  }


  /*!
    \brief Destructor
  */
  ~MomentumGradDescent()
  {
  }


  /*!
    \brief Runs a single iteration for given dataset
    \param data The dataset to run against

    This method will train the network over one iteration for the given
    dataset.
  */
  virtual void run(const NNetDataset& data);


 private:

  //! "alpha" value we use to adjust m_eta when error rate is decreasing
  double m_alpha;

  //! "beta" value we use to adjust m_eta when error rate is increasing
  double m_beta;

  //! The last training error we encountered
  double m_lastTrainError;
};

} // namespace alch

#endif
