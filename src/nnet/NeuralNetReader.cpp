

#include "nnet/NeuralNetReader.h"
#include <iostream>
#include <fstream>

namespace alch {

bool NeuralNetReader::read(const char* fname, NeuralNetPtr& network)
{
  std::fstream fs;
  fs.open(fname, std::ios::in);

  if (!fs)
  {
    m_ctx << Context::PRIORITY_error << "Failed to open file '" << fname << "'"
          << Context::endl;
    return false;
  }

  // Read number of inputs
  int numInputs = -1;
  fs >> numInputs;
  if (!fs)
  {
    m_ctx << Context::PRIORITY_error
          << "Read error while reading numInputs from '" << fname << "'"
          << Context::endl;
    return false;
  }
  else if (numInputs <= 0)
  {
    m_ctx << Context::PRIORITY_error << "Invalid value for numInputs ("
          << numInputs << " in '" << fname
          << "'" << Context::endl;
    return false;
  }

  // Read number of outputs
  int numOutputs = -1;
  fs >> numOutputs;
  if (!fs)
  {
    m_ctx << Context::PRIORITY_error
          << "Read error while reading numOutputs from '" << fname << "'"
          << Context::endl;
    return false;
  }
  else if (numOutputs <= 0)
  {
    m_ctx << Context::PRIORITY_error << "Invalid value for numOutputs ("
          << numOutputs << " in '" << fname
          << "'" << Context::endl;
    return false;
  }


  // create the neural network
  network = NeuralNetPtr(new NeuralNet(numInputs, numOutputs));

  // Read number of hidden layers
  int hiddenLayers = -1;
  fs >> hiddenLayers;
  if (!fs)
  {
    m_ctx << Context::PRIORITY_error
          << "Read error while reading hiddenLayers from '" << fname << "'"
          << Context::endl;
    return false;
  }
  else if (hiddenLayers < 0)
  {
    m_ctx << Context::PRIORITY_error << "Invalid value for hiddenLayers ("
          << hiddenLayers << " in '"
          << fname << "'" << Context::endl;
    return false;
  }

  // add the hidden layers
  for (int layer = 0; layer < hiddenLayers; ++layer)
  {
    int numUnits = 0;

    fs >> numUnits;
    if (!fs)
    {
      m_ctx << Context::PRIORITY_error << "Read error while reading numUnits["
            << layer << "] from '" << fname << "'" << Context::endl;
      return false;
    }
    else if (numUnits <= 0)
    {
      m_ctx << Context::PRIORITY_error << "Invalid value for numUnits["
            << layer << "] ("
            << numUnits << " in '" << fname << "'" << Context::endl;
      return false;
    }

    network->addLayer(numUnits - 1);
  }

  // set up the network weight array sizes
  network->reset();

  // now read all the weights
  int numLayers = network->getNumLayers();
  for (int layer = 1; layer < numLayers; ++layer)
  {
    int numUnitsPrevLayer = network->getNumUnits(layer - 1);
    int numUnitsThisLayer = network->getNumUnits(layer);
    
    for (int prevUnit = 0; prevUnit < numUnitsPrevLayer; ++prevUnit)
    {
      // don't need to randomize weight for constant unit 0
      for (int unit = 1; unit < numUnitsThisLayer; ++unit)
      {
        double weight = 0.00;
        fs >> weight;
        if (!fs)
        {
          m_ctx << Context::PRIORITY_error
                << "Read error while reading weight["
                << layer << "][" << prevUnit << "][" << unit << "] from '"
                << fname << "'" << Context::endl;
          return false;
        }
        network->setWeight(layer, prevUnit, unit, weight);
      }
    }
  }

  fs.close();
  return true;
}

} // namespace alch
