
#include "nnet/MomentumGradDescent.h"
#include "nnet/NeuralNetAlg.h"
#include "nnet/NeuralNet.h"

//#define DEBUG_MOMENDUMGRADDESCENT
#ifdef DEBUG_MOMENDUMGRADDESCENT
#include <iostream>
#endif

namespace alch {

void MomentumGradDescent::run(const NNetDataset& data)
{
  // save the net so we can revert
  assert(getNetwork().get());
  NeuralNetPtr savedNeuralNet(new NeuralNet());
  *savedNeuralNet = *getNetwork();

  // run gradient descent on the network
  GradDescent::run(data);

  // calculate our training error
  double trainError = NeuralNetAlg::calculateError(*getNetwork(), data);

#ifdef DEBUG_MOMENDUMGRADDESCENT
  std::cerr << "-- trainError(" << trainError << ") <=> lastTrainError("
            << m_lastTrainError << ")\n";
#endif

  if (trainError < m_lastTrainError)
  {

#ifdef DEBUG_MOMENDUMGRADDESCENT
    std::cerr << "-- getEta()(" << getEta() << ") *= m_alpha(" << m_alpha
              << ") ==> " << (getEta() * m_alpha) << "\n";
#endif


    // we're improving.. increase momentum!
    setEta(getEta() * m_alpha);

    // update last train error
    m_lastTrainError = trainError;
  }
  else
  {

#ifdef DEBUG_MOMENDUMGRADDESCENT
    std::cerr << "-- getEta()(" << getEta() << ") *= m_beta(" << m_beta
              << ") ==> " << (getEta() * m_beta) << "\n";
    std::cerr << "-- reverting neural net...\n";
#endif

    // error got worse.. revert weights and slow down learning
    setEta(getEta() * m_beta);
    *getNetwork() = *savedNeuralNet;
  }
}

} // namespace alch

