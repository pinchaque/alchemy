
#include "stockplot/RSIPlotDataCreator.h"

#include "stockalg/Momentum.h"

#include <sstream>

namespace alch {

  RSIPlotDataCreator::RSIPlotDataCreator(Context& ctx)
    : PlotDataCreator(ctx)
    , m_span(14)
  {
    ;
  }

  RSIPlotDataCreator::~RSIPlotDataCreator()
  {
    ;
  }

  bool RSIPlotDataCreator::create(const PlotPtr& plot)
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
      ss << "RSI (" << getSpan() << "-day)";
      plot->setYAxisLabel(ss.str());
    }
    plot->setLogarithmic(false);
    plot->setShowKey(false);
    plot->setYAutoscale(true);

    return true;
  }

  std::string RSIPlotDataCreator::generateLabel() const
  {
    std::stringstream ss;
    ss << getID() << " " << getSpan() << "-day RSI";
    return ss.str();
  }

  bool RSIPlotDataCreator::addData(const PlotDataPtr& plotData) const
  {
    assert(plotData.get());

    RangeDataPtr rangeData = getData();

    assert(rangeData.get());

    PlotDataSegmentPtr plotDataSegment(new PlotDataSegment);

    // Initialize the input to the calculator
    Momentum::DoubleVec input;
    {
      RangeData::const_iterator end = rangeData->end();
      RangeData::const_iterator iter;
      for (iter = rangeData->begin(); iter != end; ++iter)
      {
        input.push_back(iter->close);
      }
    }

    // calculate the RSI
    Momentum::DoubleVec output;
    Momentum::relativeStrength(input, getSpan(), output);

    // some sanity-checking to make sure we got the right # of data points
    assert(int(input.size()) == int(rangeData->size()));
    assert(int(output.size()) == (int(input.size()) - getSpan()));

    // copy the output into the plot data
    // note that we skip the first "span+1" elements so that the RSI is
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
