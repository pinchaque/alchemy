
#include "stockplot/ROCPlotDataCreator.h"

#include "stockalg/Momentum.h"

#include <sstream>

namespace alch {

  ROCPlotDataCreator::ROCPlotDataCreator(Context& ctx)
    : PlotDataCreator(ctx)
    , m_span(10)
  {
    ;
  }

  ROCPlotDataCreator::~ROCPlotDataCreator()
  {
    ;
  }

  bool ROCPlotDataCreator::create(const PlotPtr& plot)
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

    // set other plot parameters
    {
      std::stringstream ss;
      ss << "ROC (" << getSpan() << "-day)";
      plot->setYAxisLabel(ss.str());
    }
    plot->setLogarithmic(false);
    plot->setShowKey(false);
    plot->setYAutoscale(true);
    plot->setYMin(0.00);
    plot->setYMax(100.00);

    return true;
  }

  std::string ROCPlotDataCreator::generateLabel() const
  {
    std::stringstream ss;
    ss << getID() << " " << getSpan() << "-day ROC";
    return ss.str();
  }

  bool ROCPlotDataCreator::addData(const PlotDataPtr& plotData) const
  {
    assert(plotData.get());

    RangeDataPtr rangeData = getData();

    assert(rangeData.get());

    PlotDataSegmentPtr plotDataSegment(new PlotDataSegment);

    // Initialize the input to the ROC calculator
    Momentum::DoubleVec input;
    {
      RangeData::const_iterator end = rangeData->end();
      RangeData::const_iterator iter;
      for (iter = rangeData->begin(); iter != end; ++iter)
      {
        input.push_back(iter->close);
      }
    }

    // calculate the ROC
    Momentum::DoubleVec output;
    Momentum::rateOfChange(input, getSpan(), output);

    // some sanity-checking to make sure we got the right # of data points
    assert(int(input.size()) == int(rangeData->size()));
    assert(int(output.size()) == (int(input.size()) - getSpan()));

    // copy the output into the plot data
    // note that we skip the first "span" elements so that the ROC is
    // associated with the last date in the span
    {
      Momentum::DoubleVec::const_iterator outputEnd = output.end();
      Momentum::DoubleVec::const_iterator outputIter = output.begin();

      RangeData::const_iterator rangeEnd = rangeData->end();
      RangeData::const_iterator rangeIter = rangeData->begin() + getSpan();

      for (;
           (rangeIter != rangeEnd) && (outputIter != outputEnd);
           ++rangeIter, ++outputIter)
      {
        PlotDataPoint dataPoint;
        dataPoint.timestamp = rangeIter->tradeTime;
        dataPoint.value = *outputIter;
        plotDataSegment->push_back(dataPoint);
      }
    }

    plotData->addPlotDataSegment(plotDataSegment);

    return true;
  }

} // namespace alch
