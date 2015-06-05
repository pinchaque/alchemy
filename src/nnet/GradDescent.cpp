
#include "nnet/GradDescent.h"
#include "nnet/NeuralNet.h"

#include <cmath>

#ifndef NDEBUG
#include <iomanip>
#endif

namespace alch {

//#define DEBUG_GRADDESCENT

namespace {

  // derivative of activation function
  double derivActivation(double d)
  {
    d = ::tanh(d);
    return (1.00 - (d * d));
  }
} // anonymous namespace


void GradDescent::run(const NNetDataset& data)
{
  assert(m_network.get());

  clearWeightDelta();

#ifdef DEBUG_GRADDESCENT
  std::cerr << "*** GradDescent::run() ***\n";
#endif

  NNetDataset::const_iterator end = data.end();
  NNetDataset::const_iterator iter;
  for (iter = data.begin(); iter != end; ++iter)
  {
#ifdef DEBUG_GRADDESCENT
    std::cerr << "Datapoint: ";
    iter->dump(std::cerr);
#endif

    clearDelta();
    
    m_network->propagateInput(iter->input);

#ifdef DEBUG_GRADDESCENT
    m_network->dump(std::cerr);
#endif

    computeOutputDelta(iter->output);

    computeDelta();

    addDeltaToTotal();
  }

  updateWeights(getEta());
}


void GradDescent::run(const NNetDataset& data, double eta)
{
  setEta(eta);
  run(data);
}


void GradDescent::clearDelta()
{
  int numLayers = m_network->getNumLayers();

  assert(static_cast<int>(m_delta.size()) == numLayers);

  for (int layer = 0; layer < numLayers; ++layer)
  {
    int numUnits = m_network->getNumUnits(layer);

    assert(static_cast<int>(m_delta[layer].size()) == numUnits);

    for (int unit = 0; unit < numUnits; ++unit)
    {
      m_delta[layer][unit] = 0.00;
    }
  }
}


void GradDescent::clearWeightDelta()
{
  int numLayers = m_network->getNumLayers();

  assert(static_cast<int>(m_weightDelta.size()) == numLayers);

  for (int layer = 1; layer < numLayers; ++layer)
  {
    // number of units in prev layer
    int numUnitsPrevLayer = m_network->getNumUnits(layer - 1);
    assert(static_cast<int>(m_weightDelta[layer].size()) == numUnitsPrevLayer);

    for (int prevUnit = 0; prevUnit < numUnitsPrevLayer; ++prevUnit)
    {
      // number of units in this layer
      int numUnitsThisLayer = m_network->getNumUnits(layer);
      assert(static_cast<int>(m_weightDelta[layer][prevUnit].size())
             == numUnitsThisLayer);

      for (int unit = 0; unit < numUnitsThisLayer; ++unit)
      {
        m_weightDelta[layer][prevUnit][unit] = 0.00;
      }
    }
  }
}


void GradDescent::resize()
{
  // size the deltas
  m_delta.clear();

  int numLayers = m_network->getNumLayers();
  m_delta.resize(numLayers);

  for (int layer = 0; layer < numLayers; ++layer)
  {
    int numUnits = m_network->getNumUnits(layer);
    m_delta[layer].resize(numUnits, 0.00);
  }

  // size the weight deltas
  m_weightDelta.clear();
  m_weightDelta.resize(numLayers);
  for (int layer = 1; layer < numLayers; ++layer)
  {
    // number of units in prev layer
    int numUnits = m_network->getNumUnits(layer - 1);

    m_weightDelta[layer].resize(numUnits);

    for (int prevUnit = 0; prevUnit < numUnits; ++prevUnit)
    {
      // number of units in this layer
      m_weightDelta[layer][prevUnit].resize(m_network->getNumUnits(layer),
                                            0.00);
    }
  }
}


void GradDescent::updateWeights(double eta)
{
  int numLayers = m_network->getNumLayers();
  for (int layer = 1; layer < numLayers; ++layer)
  {
    int prevLayer = layer - 1;
    int numUnitsPrevLayer = m_network->getNumUnits(prevLayer);
    int numUnitsThisLayer = m_network->getNumUnits(layer);

    for (int prevUnit = 0; prevUnit < numUnitsPrevLayer; ++prevUnit)
    {
      // don't need to update weight for constant unit 0
      for (int unit = 1; unit < numUnitsThisLayer; ++unit)
      {
        // get a reference to existing weight
        double& weight = m_network->getWeight(layer, prevUnit, unit);

#ifdef DEBUG_GRADDESCENT
        std::cerr << "Begin: weight[" << layer << "][" << prevUnit
                  << "][" << unit << "] = " << weight << "\n";
        std::cerr << "eta(" << eta
                  << ") * weightDelta(" << m_weightDelta[layer][prevUnit][unit]
                  << ") = " << (eta * m_weightDelta[layer][prevUnit][unit])
                  << "\n";
#endif

        // update the weight
        weight -= (eta * m_weightDelta[layer][prevUnit][unit]);


#ifdef DEBUG_GRADDESCENT
        std::cerr << "Updated weight: "
                  << m_network->getWeight(layer, prevUnit, unit)
                  << " or "
                  << weight
                  << std::endl;
#endif
      }
    }
  }
}


void GradDescent::computeOutputDelta(const std::vector<double>& target)
{
  int numLayers = m_network->getNumLayers();
  assert(static_cast<int>(m_delta.size()) == numLayers);

  int outputLayer = numLayers - 1;
  const std::vector<double>& output = m_network->getOutput();
  
  int numUnits = m_network->getNumUnits(outputLayer);

  assert(static_cast<int>(m_delta[outputLayer].size()) == numUnits);
  assert(static_cast<int>(output.size()) == numUnits);
  assert(output.size() == target.size() + 1);

  for (int unit = 1; unit < numUnits; ++unit)
  {
#ifdef DEBUG_GRADDESCENT
    std::cerr << "m_delta[" << outputLayer << "][" << unit << "] = "
              << "output[unit](" << output[unit] << ") - target[unit]("
              << target[unit - 1] << ")\n";
#endif
    m_delta[outputLayer][unit] = (output[unit] - target[unit - 1]);
  }
}


void GradDescent::computeDelta()
{
  int numLayers = m_network->getNumLayers();
  assert(static_cast<int>(m_delta.size()) == numLayers);

  // start at second to last layer
  for (int layer = numLayers - 2; layer >= 1; --layer)
  {
    int nextLayer = layer + 1;
    int numUnitsNextLayer = m_network->getNumUnits(nextLayer);
    int numUnitsThisLayer = m_network->getNumUnits(layer);

    assert(static_cast<int>(m_delta[layer].size()) == numUnitsThisLayer);

    // calculate all deltas except for constant unit
    for (int unit = 1; unit < numUnitsThisLayer; ++unit)
    {
      // sum all delta*weights for above layer
      double sumDeltas = 0.00;
      for (int nextUnit = 1; nextUnit < numUnitsNextLayer; ++nextUnit)
      {
        sumDeltas += (m_network->getWeight(nextLayer, unit, nextUnit)
                      * m_delta[nextLayer][nextUnit]);
      }
      m_delta[layer][unit]
        = (derivActivation(m_network->getUnitInput(layer, unit))
           * sumDeltas);
    }
  }

}

void GradDescent::addDeltaToTotal()
{
  int numLayers = m_network->getNumLayers();
  assert(static_cast<int>(m_delta.size()) == numLayers);
  assert(static_cast<int>(m_weightDelta.size()) == numLayers);

  for (int layer = 1; layer < numLayers; ++layer)
  {
    int prevLayer = layer - 1;
    int numUnitsPrevLayer = m_network->getNumUnits(prevLayer);
    int numUnitsThisLayer = m_network->getNumUnits(layer);

    assert(static_cast<int>(m_delta[layer].size()) == numUnitsThisLayer);
    assert(static_cast<int>(m_weightDelta[layer].size()) == numUnitsPrevLayer);

    for (int prevUnit = 0; prevUnit < numUnitsPrevLayer; ++prevUnit)
    {
      assert(static_cast<int>(m_weightDelta[layer][prevUnit].size())
             == numUnitsThisLayer);

      // don't need to update weight for constant unit 0
      for (int unit = 1; unit < numUnitsThisLayer; ++unit)
      {
#ifdef DEBUG_GRADDESCENT
        std::cerr << "Begin: m_weightDelta[" << layer << "][" << prevUnit
                  << "][" << unit << "] = "
                  << m_weightDelta[layer][prevUnit][unit] << "\n";
        std::cerr << "m_delta[layer][unit](" << m_delta[layer][unit]
                  << ") * m_network->getUnitOutput(prevLayer, prevUnit)("
                  << m_network->getUnitOutput(prevLayer, prevUnit)
                  << ") = "
                  << (m_delta[layer][unit]
                      * m_network->getUnitOutput(prevLayer, prevUnit))
                  << "\n";
#endif
        // update the weight
        m_weightDelta[layer][prevUnit][unit] += 
          m_delta[layer][unit] * m_network->getUnitOutput(prevLayer, prevUnit);

#ifdef DEBUG_GRADDESCENT
        std::cerr << "Updated weightDelta: " 
                  << m_weightDelta[layer][prevUnit][unit] << "\n";
#endif
      }
    }
  }
}

} // namespace alch

