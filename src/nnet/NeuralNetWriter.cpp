

#include "nnet/NeuralNetWriter.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace alch {

bool NeuralNetWriter::write(const char* fname, const NeuralNetPtr& network)
{
  std::fstream fs;
  fs.open(fname, std::ios::out);

  if (!fs)
  {
    m_ctx << Context::PRIORITY_error << "Failed to open file '" << fname << "'"
          << Context::endl;
    return false;
  }

  // Write number of inputs
  fs << network->getNumInputUnits() << " ";
  if (!fs)
  {
    m_ctx << Context::PRIORITY_error
          << "Write error while writing numInputs to '"
          << fname << "'" << Context::endl;
    return false;
  }

  // Write number of outputs
  fs << (network->getNumOutputUnits() - 1) << " ";
  if (!fs)
  {
    m_ctx << Context::PRIORITY_error
          << "Write error while writing numOutputs to '"
          << fname << "'" << Context::endl;
    return false;
  }

  // Write number of hidden layers
  int hiddenLayers = network->getNumLayers() - 2;
  fs << hiddenLayers << "\n";
  if (!fs)
  {
    m_ctx << Context::PRIORITY_error
          << "Write error while writing hiddenLayers to '"
          << fname << "'" << Context::endl;
    return false;
  }

  // add the hidden layers
  for (int layer = 0; layer < hiddenLayers; ++layer)
  {
    fs << network->getNumUnits(layer + 1) << " ";
    if (!fs)
    {
      m_ctx << Context::PRIORITY_error << "Write error while writing numUnits["
            << layer << "] to '" << fname << "'" << Context::endl;
      return false;
    }
  }
  fs << "\n";

  // now write all the weights
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
        fs << std::setprecision(16)
           << network->getWeight(layer, prevUnit, unit) << " ";
        if (!fs)
        {
          m_ctx << Context::PRIORITY_error
                << "Write error while writing weight["
                << layer << "][" << prevUnit << "][" << unit << "] to '"
                << fname << "'" << Context::endl;
          return false;
        }
      }
      fs << "\n";
    }
  }

  fs.close();
  return true;
}

} // namespace alch

