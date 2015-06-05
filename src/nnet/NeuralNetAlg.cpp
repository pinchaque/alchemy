
#include "nnet/NeuralNetAlg.h"

#include "nnet/Statistics.h"

#include <cmath>

namespace alch {

namespace NeuralNetAlg
{
  void init()
  {
    // XXX  this is called by application initialization
    // srand48(time(0));
  }

  void randomizeWeights(NeuralNet& net, double min, double max)
  {
    assert(max >= min);

    double range = (max - min);
    int numLayers = net.getNumLayers();
    for (int layer = 1; layer < numLayers; ++layer)
    {
      int numUnitsPrevLayer = net.getNumUnits(layer - 1);
      int numUnitsThisLayer = net.getNumUnits(layer);

      for (int prevUnit = 0; prevUnit < numUnitsPrevLayer; ++prevUnit)
      {
        // don't need to randomize weight for constant unit 0
        for (int unit = 1; unit < numUnitsThisLayer; ++unit)
        {
          double weight = ((drand48() * range) + min);
          net.setWeight(layer, prevUnit, unit, weight);
        }
      }
    }
  }

  double calculateError(NeuralNet& net, const NNetDataset& dataset)
  {
    double error = 0.00;
    int totalPoints = 0;

    // calculate squared error for each point in dataset
    NNetDataset::const_iterator end = dataset.end();
    NNetDataset::const_iterator iter;
    for (iter = dataset.begin(); iter != end; ++iter)
    {
      net.propagateInput(iter->input);
      const std::vector<double>& output = net.getOutput();
      const std::vector<double>& target = iter->output;

      assert(output.size() == target.size() + 1);
      int numOutput = output.size();

      for (int i = 1; i < numOutput; ++i)
      {
        double diff = output[i] - target[i - 1];

        // sum the squared error
        error += (diff * diff);

        // keep track of how many datapoints we're summing over
        ++totalPoints;
      }
    }

    // return .25 times sum of all squared errors
    return 0.25 * error / (double) totalPoints;
  }


  double sumOfSquares(const double* value, const double* target, int n)
  {
    assert(value);
    assert(target);
    assert(n >= 0);

    if (!n)
    {
      return 0.00;
    }

    double error = 0.00;

    for (int i = 0; i < n; ++i)
    {
      double diff = value[i] - target[i];

      // sum the squared error
      error += (diff * diff);
    }

    return 0.5 * error;
  }


  double squaredError(const double* value, const double* target, int n)
  {
    return 0.5 * sumOfSquares(value, target, n) / (double) n;
  }


  double rmsError(const double* value, const double* target, int n)
  {
    assert(value);
    assert(target);
    assert(n >= 0);

    if (!n)
    {
      return 0.00;
    }

    double numerator = 0.00;
    double denominator = 0.00;
    double targetMean = Statistics::mean(target, n);

    for (int i = 0; i < n; ++i)
    {
      double diff1 = value[i] - target[i];
      numerator += (diff1 * diff1);

      double diff2 = target[i] - targetMean;
      denominator += (diff2 * diff2);
    }

    if (denominator == 0.00)
    {
      return 0.00;
    }
    else
    {
      return (numerator / denominator);
    }
  }


  double minkowskiR1(const double* value, const double* target, int n)
  {
    assert(value);
    assert(target);
    assert(n >= 0);

    if (!n)
    {
      return 0.00;
    }

    double error = 0.00;

    for (int i = 0; i < n; ++i)
    {
      error += ::fabs(value[i] - target[i]);
    }

    return error;
  }


  double absError(const double* value, const double* target, int n)
  {
    return minkowskiR1(value, target, n) / (double) n;
  }


  void calculateOutputs(NeuralNet& net, NNetDataset& dataset)
  {
    int datasetSize = int(dataset.size());
    for (int idx = 0; idx < datasetSize; ++idx)
    {
       net.propagateInput(dataset[idx].input);
       const std::vector<double>& outputs(net.getOutput());
       int outputsSize = int(outputs.size());
       assert(outputsSize > 1);

       dataset[idx].output.clear();
       dataset[idx].output.reserve(outputsSize);

       std::vector<double>::const_iterator end = outputs.end();
       std::vector<double>::const_iterator iter;
       for (iter = outputs.begin() + 1; iter != end; ++iter)
       {
         dataset[idx].output.push_back(*iter);
       }
    }
  }

} // namespace NeuralNetAlg

} // namespace alch


