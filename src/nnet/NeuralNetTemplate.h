// -*- C++ -*-

#ifndef INCLUDED_nnet_NeuralNetTemplate_h
#define INCLUDED_nnet_NeuralNetTemplate_h

#include "boost/shared_ptr.hpp"

#include <vector>
#include <cassert>

#ifndef NDEBUG
#include <iostream>
#endif

namespace alch {


/*!
  \brief Neural Network data representation class
*/
template <typename TActivation, typename TOutputActivation>
class NeuralNetTemplate
{
 public:

  //! The weights that belong to a single layer. First index is the unit
  //! in the previous layer. Second index is the unit in the current layer.
  typedef std::vector<std::vector<double> > LayerWeight;

  /*!
    \brief Constructor: Creates neural network of specified size
    \param inputUnits Number of input units
    \param outputUnits Number of output units
    \param hiddenLayers Number of layers, not including the output layer
    \param unitsPerLayer Number of units in each of the hidden layers

    This constructor effectively creates a "rectangular" network by which
    each layer has the same number of units. If you want a network
    with differently-sized layers then you can use the addLayer() method.

    Note that all neural networks have an input and output layer. The number
    of input and output units cannot be changed after construction.
  */
  NeuralNetTemplate(int inputUnits = 1,
            int outputUnits = 1,
            int hiddenLayers = 0,
            int unitsPerLayer = 0)
    : m_weight()
    , m_input()
    , m_output()
    , m_numUnits()
    , m_outputActivation()
    , m_activation()
  {
    assert(inputUnits >= 1);
    assert(outputUnits >= 1);

    // create input layer; add 1 for constant unit
    m_numUnits.push_back(inputUnits + 1);

    // create the hidden layers; add 1 for constant unit
    for (int i = 0; i < hiddenLayers; ++i)
    {
      assert(unitsPerLayer > 0);
      m_numUnits.push_back(unitsPerLayer + 1);
    }

    // create output layer; add 1 for constant unit
    m_numUnits.push_back(outputUnits + 1);

    // sets up weight, input, output
    reset();
  }


  /*!
    \brief Destructor
  */
  ~NeuralNetTemplate()
  {
  }


  /*!
    \brief Returns number of layers in the network

    The returned number includes both the input and output layers.
  */
  int getNumLayers() const
  {
    return m_numUnits.size();
  }


  /*!
    \brief Returns number of input units not including constant unit.
  */
  int getNumInputUnits() const
  {
    assert(m_numUnits.size() >= 2);
    return (m_numUnits[0] - 1);
  }


  /*!
    \brief Returns number of output units
  */
  int getNumOutputUnits() const
  {
    assert(m_numUnits.size() >= 2);
    return m_numUnits[m_numUnits.size() - 1];
  }


  /*!
    \brief Returns number of units in specified layer
    \param layer The layer to look for units in. can be in range
    [0..getNumLayers() - 1]. 

    getNumUnits(0) will return number of input units including the constant
    unit.

    getNumUnits(getNumLayers() - 1) will return the number of output units.
  */
  int getNumUnits(int layer) const
  {
    assert(layer >= 0);
    assert(layer < static_cast<int>(m_numUnits.size()));
    return m_numUnits[layer];
  }

  /*!
    \brief Appends a layer to the network
    \param numUnits Number of units in the layer we're adding

    Once this method is called, the network is invalid until reset() is called.
    You may make successive calls to addLayer without calling reset()
    inbetween.
  */
  void addLayer(int numUnits)
  {
    int numLayers = getNumLayers();

    assert(numUnits > 0);
    assert(numLayers >= 2);

    // dup the last element
    m_numUnits.push_back(m_numUnits[numLayers - 1]);

    // insert the layer
    m_numUnits[numLayers - 1] = numUnits + 1;
  }

  /*!
    \brief Returns the weight of the connection between fromUnit in
    toLayer-1 and toUnit in toLayer.
    \param toLayer The layer in which toUnit resides
    \param fromUnit The unit index number of the unit in the previous layer
    (toLayer - 1) from which this connection originated.
    \param toUnit The unit index number of the unit in the current layer
    (toLayer) at which this connection ends.
  */
  double& getWeight(int toLayer, int fromUnit, int toUnit)
  {
    assert(toLayer >= 1);
    assert(toLayer < getNumLayers());
    assert(toLayer < static_cast<int>(m_weight.size()));

    assert(fromUnit >= 0);
    assert(fromUnit < static_cast<int>(m_weight[toLayer].size()));
    assert(fromUnit < m_numUnits[toLayer - 1]);

    assert(toUnit >= 0);
    assert(toUnit < static_cast<int>(m_weight[toLayer][fromUnit].size()));
    assert(toUnit < m_numUnits[toLayer]);

    return m_weight[toLayer][fromUnit][toUnit];
  }

  /*!
    \brief Sets the weight of the connection between fromUnit in
    toLayer-1 and toUnit in toLayer.
    \param toLayer The layer in which toUnit resides
    \param fromUnit The unit index number of the unit in the previous layer
    (toLayer - 1) from which this connection originated.
    \param toUnit The unit index number of the unit in the current layer
    (toLayer) at which this connection ends.
  */
  void setWeight(int toLayer, int fromUnit, int toUnit, double value)
  {
    assert(toLayer >= 1);
    assert(toLayer < getNumLayers());
    assert(toLayer < static_cast<int>(m_weight.size()));

    assert(fromUnit >= 0);
    assert(fromUnit < static_cast<int>(m_weight[toLayer].size()));
    assert(fromUnit < m_numUnits[toLayer - 1]);

    assert(toUnit >= 0);
    assert(toUnit < static_cast<int>(m_weight[toLayer][fromUnit].size()));
    assert(toUnit < m_numUnits[toLayer]);

    m_weight[toLayer][fromUnit][toUnit] = value;
  }

  /*!
    \brief Sets ths neural network inputs to be the specified values
    \param val The network inputs

    Note that the size of the specified array must be the same as the
    number of input units in the network set during construction.
  */
  void setInput(const std::vector<double>& val)
  {
    assert(m_output.size() >= 2);
    assert(m_output[0].size() == (val.size() + 1));

    m_output[0][0] = 1.00;
    int valSize = val.size();
    for (int i = 0; i < valSize; ++i)
    {
      m_output[0][i + 1] = val[i];
    }
  }


  /*!
    \brief Returns a reference to the output units
  */
  const std::vector<double>& getOutput() const
  {
    int outputLayer = m_output.size() - 1;
    assert(m_output.size() >= 2);
    return m_output[outputLayer];
  }


  /*!
    \brief Returns unit output for specified layer/unit
    \param layer The layer in which the unit resides [0, numLayer-1]
    \param unit The unit index within that layer
  */
  double getUnitOutput(int layer, int unit) const
  {
    assert(layer >= 0);
    assert(layer < getNumLayers());
    assert(layer < static_cast<int>(m_output.size()));

    assert(unit >= 0);
    assert(unit < static_cast<int>(m_output[layer].size()));
    assert(unit < m_numUnits[layer]);

    return m_output[layer][unit];
  }

  /*!
    \brief Returns unit input for specified layer/unit
    \param layer The layer in which the unit resides [1, numLayer-1]
    \param unit The unit index within that layer
  */
  double getUnitInput(int layer, int unit) const
  {
    assert(layer >= 1);
    assert(layer < getNumLayers());
    assert(layer < static_cast<int>(m_input.size()));

    assert(unit >= 0);
    assert(unit < static_cast<int>(m_input[layer].size()));
    assert(unit < m_numUnits[layer]);

    return m_input[layer][unit];
  }


  /*!
    \brief Sets network inputs and propagates them to the output
    \param input The network inputs

    This will use the specified input and existing network weights to 
    calculate the new network outputs. The tree will be modified with
    new input/output values for the individual units. The outputs can
    be retrieved with getOutput();
  */
  void propagateInput(const std::vector<double>& input)
  {
    setInput(input);
    
    for (int layer = 1; layer < getNumLayers(); ++layer)
    {
      computeInputs(layer);
      computeOutputs(layer);
    }
  }


  /*!
    \brief Resizes and reinitializes the input and output data structures.
    
    It is necessary to call this method after the network has been fully
    sized via the addLayer() method.
  */
  void reset()
  {
    int numLayers = getNumLayers();

    //////////////////////////////////////////////////////////////////////
    // resize input
    m_input.clear();
    m_input.resize(numLayers);

    // inputs to the hidden layers
    for (int i = 0; i < numLayers; ++i)
    {
      m_input[i].resize(m_numUnits[i], 0.00);
    }

    //////////////////////////////////////////////////////////////////////
    // resize output
    m_output.clear();
    m_output.resize(numLayers);

    // outputs from the hidden layers
    for (int i = 0; i < numLayers; ++i)
    {
      m_output[i].resize(m_numUnits[i], 0.00);
    }

    // set up single-valued outputs
    for (int i = 0; i < numLayers; ++i)
    {
      m_output[i][0] = 1.00;
    }

    //////////////////////////////////////////////////////////////////////
    // resize weights
    m_weight.clear();
    m_weight.resize(numLayers);
    for (int i = 1; i < numLayers; ++i)
    {
      // number of units in prev layer
      int maxJ = m_numUnits[i - 1];

      m_weight[i].resize(maxJ);

      for (int j = 0; j < maxJ; ++j)
      {
        // number of units in this layer
        int maxK = 0;
        maxK = m_numUnits[i];

        m_weight[i][j].resize(maxK, 0.00);
      }
    }
  }



#ifndef NDEBUG
  /*!
    \brief Prints neural network to stream
    \param os Output stream
  */
  void dump(std::ostream& os)
  {
    const int colWidth = 8;

    int numLayers = getNumLayers();
    for (int i = 0; i < numLayers; ++i)
    {
      int numUnits = m_numUnits[i];

      os << "--- Layer " << i << " ---\n";

      os << "Input:  ";
      for (int j = 0; j < numUnits; ++j)
      {
        os << std::setw(colWidth) << m_input[i][j] << " ";
      }
      os << "\n";

      os << "Output: ";
      for (int j = 0; j < numUnits; ++j)
      {
        os << std::setw(colWidth) << m_output[i][j] << " ";
      }
      os << "\n\n";
    }
  }

#endif

private:
  //! The weights of all layers
  std::vector<LayerWeight> m_weight;

  //! The inputs to all units. m_input[layer][unit]
  std::vector<std::vector<double> > m_input;

  //! The outputs from all units. m_output[layer][unit]
  std::vector<std::vector<double> > m_output;

  //! Number of units in each layer. m_numUnits[layer]. Includes constant
  //! unit.
  std::vector<int> m_numUnits;

  //! Output activation functor
  TOutputActivation m_outputActivation;

  //! Activation functor
  TActivation m_activation;


  //! Computes the outputs for the specified layer [0..N-1]
  void computeOutputs(int layer)
  {
    assert(layer >= 0);
    assert(layer < getNumLayers());
    assert(static_cast<int>(m_input.size()) == getNumLayers());
    assert(static_cast<int>(m_input[layer].size()) == m_numUnits[layer]);
    assert(static_cast<int>(m_output.size()) == getNumLayers());
    assert(static_cast<int>(m_output[layer].size()) == m_numUnits[layer]);

    // set constant output by def
    m_output[layer][0] = 1.00;

    // is this the output layer?
    int outputLayer = getNumLayers() - 1;
    if (layer == outputLayer)
    {
      for (int unit = 1; unit < m_numUnits[layer]; ++unit)
      {
        // output activation function
        m_output[layer][unit] = m_outputActivation(m_input[layer][unit]);
      }
    }
    else
    {
      for (int unit = 1; unit < m_numUnits[layer]; ++unit)
      {
        // hidden unit activation function
        m_output[layer][unit] = m_activation(m_input[layer][unit]);
      }
    }
  }



  //! Computes the inputs for the specified layer [1..N-1]
  void computeInputs(int layer)
  {
    assert(layer >= 1);
    assert(layer < getNumLayers());
    assert(static_cast<int>(m_input.size()) == getNumLayers());
    assert(static_cast<int>(m_input[layer].size()) == m_numUnits[layer]);
    assert(static_cast<int>(m_output.size()) == getNumLayers());
    assert(static_cast<int>(m_output[layer].size()) == m_numUnits[layer]);

    int prevLayer = layer - 1;

    for (int unit = 1; unit < m_numUnits[layer]; ++unit)
    {
      double weightSum = 0.00;

      for (int prevUnit = 0; prevUnit < m_numUnits[prevLayer]; ++prevUnit)
      {
        weightSum += (m_weight[layer][prevUnit][unit]
                      * m_output[prevLayer][prevUnit]);
      }

      m_input[layer][unit] = weightSum;
    }
  }

}; // class NeuralNetTemplate


} // namespace alch

#endif
