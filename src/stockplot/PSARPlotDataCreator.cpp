
#include "stockplot/PSARPlotDataCreator.h"

#include "stockalg/Momentum.h"

#include <sstream>

namespace alch {

  PSARPlotDataCreator::PSARPlotDataCreator(Context& ctx)
    : PlotDataCreator(ctx)
    , m_accel(0.02)
    , m_maxAccel(0.20)
  {
    ;
  }

  PSARPlotDataCreator::~PSARPlotDataCreator()
  {
    ;
  }

  bool PSARPlotDataCreator::create(const PlotPtr& plot)
  {
    assert(plot.get());

    // create our new PlotData
    PlotDataPtr plotData(new PlotData);

    // set up PlotData basics
    plotData->setStyle(PlotData::STYLE_points);
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

  std::string PSARPlotDataCreator::generateLabel() const
  {
    std::stringstream ss;
    ss << getID() << " Parabolic SAR";
    return ss.str();
  }

  bool PSARPlotDataCreator::addData(const PlotDataPtr& plotData) const
  {
    assert(plotData.get());

    RangeDataPtr rangeData = getData();

    assert(rangeData.get());

    PlotDataSegmentPtr plotDataSegment(new PlotDataSegment);

    // calculate the MA
    Momentum::DoubleVec output;
    Momentum::parabolicSAR(*rangeData, getAccel(), getMaxAccel(), output);

    // some sanity-checking to make sure we got the right # of data points
    assert(int(output.size()) == int(rangeData->size()));

    // copy the output into the plot data
    {
      Momentum::DoubleVec::const_iterator outputEnd = output.end();
      Momentum::DoubleVec::const_iterator outputIter = output.begin();

      RangeData::const_iterator rangeEnd = rangeData->end();
      RangeData::const_iterator rangeIter = rangeData->begin();

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
