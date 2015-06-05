// -*- C++ -*-

#ifndef INCLUDED_stocknnet_PredictionProfile_h
#define INCLUDED_stocknnet_PredictionProfile_h

#include "nnet/NeuralNet.h"

#include "boost/shared_ptr.hpp"

#include <string>

namespace alch {

/*!
  \brief Prediction profile, including the neural network and metadata
  \ingroup stocknnet
*/
class PredictionProfile
{
 public:

  /*!
    \brief Constructor
  */
  PredictionProfile()
    : m_neuralNet()
    , m_name("Generic prediction profile")
    , m_numberDays(1)
  {
    ;
  }

  virtual ~PredictionProfile()
  {
    ;
  }

  //! returns the neural net
  const NeuralNetPtr getNeuralNetPtr() const
  {
    return m_neuralNet;
  }

  //! returns the neural net
  const NeuralNet& getNeuralNet() const
  {
    return *m_neuralNet;
  }

  //! returns the neural net
  void setNeuralNet(NeuralNetPtr val)
  {
    m_neuralNet = val;
  }

  //! returns number of days in future the prediction is for
  int getNumberDays() const
  {
    return m_numberDays;
  }

  //! sets number of days in future the prediction is for
  void setNumberDays(int val)
  {
    m_numberDays = val;
  }

  //! returns name of this prediction profile
  const std::string& getName() const
  {
    return m_name;
  }

  //! sets name for this prediction profile
  void setName(const std::string& val)
  {
    m_name = val;
  }

 private:

  NeuralNetPtr m_neuralNet;

  std::string m_name;

  int m_numberDays;
  
};

/*!
  \brief Shared pointer to PredictionProfile
  \ingroup stocknnet
*/
typedef boost::shared_ptr<PredictionProfile> PredictionProfilePtr;

} // namespace alch

#endif
