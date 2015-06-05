
#include "stockplot/ProfilePlotDataCreator.h"

#include "nnet/NeuralNetAlg.h"
#include "nnet/NNetDataset.h"

#include <sstream>

namespace alch {

  ProfilePlotDataCreator::ProfilePlotDataCreator(
    Context& ctx,
    const PredictionProfile& profile,
    DatasetGenerator& generator)
    : PlotDataCreator(ctx)
    , m_profile(profile)
    , m_generator(generator)
  {
    ;
  }

  ProfilePlotDataCreator::~ProfilePlotDataCreator()
  {
    ;
  }

  bool ProfilePlotDataCreator::create(const PlotPtr& plot)
  {
    assert(plot.get());

    // create our new PlotData
    PlotDataPtr plotData(new PlotData);

    // set up PlotData basics
    plotData->setStyle(PlotData::STYLE_lines);
    plotData->setLabel(generateLabel());

    // Add our data
    if (!addData(plotData))
    {
      return false;
    }

    // add the PlotData to the Plot
    plot->addPlotData(plotData);

    return true;
  }

  std::string ProfilePlotDataCreator::generateLabel() const
  {
    std::stringstream ss;
    ss << getID() << " " << m_profile.getNumberDays() << "-day Prediction";
    return ss.str();
  }

  bool ProfilePlotDataCreator::addData(const PlotDataPtr& plotData)
  {
    assert(plotData.get());

    RangeDataPtr rangeData = getData();
    assert(rangeData.get());

    if (!rangeData->size())
    {
      getContext() << Context::PRIORITY_warning
                   << "No range data passed to plot for " << getID()
                   << Context::endl;
      return true;
    }

    // generate the neural network inputs based on the dataset we're using
    NNetDataset dataset;
    if (!m_generator.generateInputs(rangeData, dataset))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to generate neural network dataset from range "
                   << "data for " << getID()
                   << Context::endl;
      return false;
    }


    PlotDataSegmentPtr plotDataSegment(new PlotDataSegment);
    
    // the values we plot must extend m_profile.getNumberDays() into the
    // future beyond what's in rangeData--we have to estimate the dates
    // for these additional points.
    
    NeuralNet neuralNet = m_profile.getNeuralNet();
    int datasetSize = int(dataset.size());
    int numRangeDataPoints = int(rangeData->size());
    int startIdx = (numRangeDataPoints
                    - datasetSize
                    + m_profile.getNumberDays());

    // initialize trade time to last time in range data
    StockTime tradeTime(rangeData->get(rangeData->size() - 1).tradeTime);

    // propagate all the inputs for the dataset we have.. this gives us
    // the predictions for rangeData.
    NeuralNetAlg::calculateOutputs(neuralNet, dataset);

    // assign values in dataset based on rangeData and output ratio
    for (int idx = 0; idx < datasetSize; ++idx)
    {
      assert(dataset[idx].output.size());

      double ratio = dataset[idx].output[0];

      int rangeIdx = startIdx + idx;

      assert(rangeIdx >= m_profile.getNumberDays());
      double referenceClose
        = rangeData->get(rangeIdx - m_profile.getNumberDays()).close;
      double value = (1.0 + ratio) * referenceClose;

      if (rangeIdx < numRangeDataPoints)
      {
        tradeTime = rangeData->get(rangeIdx).tradeTime;
      }
      else
      {
        // increment by 7/5 of a day to keep x scale relatively even
        tradeTime = (tradeTime
                     + boost::gregorian::date_duration(1)
                     + boost::posix_time::hours(9)
                     + boost::posix_time::minutes(36));
      }

#if 0      
      getContext() << Context::PRIORITY_debug2
                   << boost::posix_time::to_simple_string(tradeTime)
                   << ": refClose:" << referenceClose
                   << " Ratio:" << ratio
                   << " Predict:" << value
                   << " Actual:"
                   << ((rangeIdx < numRangeDataPoints)
                       ? rangeData->get(rangeIdx).close
                       : 0.00)
                   << Context::endl;
#endif

      PlotDataPoint dataPoint;
      dataPoint.timestamp = tradeTime;
      dataPoint.value = value;
      plotDataSegment->push_back(dataPoint);
    }

    plotData->addPlotDataSegment(plotDataSegment);

    return true;
  }


} // namespace alch
