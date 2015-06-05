
#include "stockplot/MAPlotDataCreator.h"

#include "stockalg/MovingAverage.h"

#include <sstream>

namespace alch {

  MAPlotDataCreator::MAPlotDataCreator(Context& ctx)
    : PlotDataCreator(ctx)
    , m_span(20)
  {
    ;
  }

  MAPlotDataCreator::~MAPlotDataCreator()
  {
    ;
  }

  bool MAPlotDataCreator::create(const PlotPtr& plot)
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

  std::string MAPlotDataCreator::generateLabel() const
  {
    std::stringstream ss;
    ss << getID() << " " << getSpan() << "-day MA";
    return ss.str();
  }

  bool MAPlotDataCreator::addData(const PlotDataPtr& plotData) const
  {
    assert(plotData.get());

    RangeDataPtr rangeData = getData();

    assert(rangeData.get());

    PlotDataSegmentPtr plotDataSegment(new PlotDataSegment);

    // Initialize the input to the MA calculator
    MovingAverage::DoubleVec input;
    {
      RangeData::const_iterator end = rangeData->end();
      RangeData::const_iterator iter;
      for (iter = rangeData->begin(); iter != end; ++iter)
      {
        input.push_back(iter->close);
      }
    }

    // calculate the MA
    MovingAverage::DoubleVec output;
    MovingAverage::simpleMA(input, getSpan(), output);

    // some sanity-checking to make sure we got the right # of data points
    assert(int(input.size()) == int(rangeData->size()));
    assert(int(output.size()) == (int(input.size()) - getSpan() + 1));

    // copy the output into the plot data
    // note that we skip the first "span-1" elements so that the moving
    // average is associated with the last date in the span
    {
      MovingAverage::DoubleVec::const_iterator outputEnd = output.end();
      MovingAverage::DoubleVec::const_iterator outputIter = output.begin();

      RangeData::const_iterator rangeEnd = rangeData->end();
      RangeData::const_iterator rangeIter = rangeData->begin() + getSpan() - 1;

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
