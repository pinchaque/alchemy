
#include "stockplot/PricePlotDataCreator.h"

#include <sstream>

namespace alch {

  PricePlotDataCreator::PricePlotDataCreator(Context& ctx)
    : PlotDataCreator(ctx)
  {
    ;
  }

  PricePlotDataCreator::~PricePlotDataCreator()
  {
    ;
  }

  bool PricePlotDataCreator::create(const PlotPtr& plot)
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

  std::string PricePlotDataCreator::generateLabel() const
  {
    std::stringstream ss;
    ss << getID() << " Price";
    return ss.str();
  }

  bool PricePlotDataCreator::addData(const PlotDataPtr& plotData) const
  {
    assert(plotData.get());

    RangeDataPtr rangeData = getData();

    assert(rangeData.get());

    PlotDataSegmentPtr plotDataSegment(new PlotDataSegment);

    RangeData::const_iterator end = rangeData->end();
    RangeData::const_iterator iter;
    for (iter = rangeData->begin(); iter != end; ++iter)
    {
      PlotDataPoint dataPoint;
      dataPoint.timestamp = iter->tradeTime;
      dataPoint.value = iter->close;
      plotDataSegment->push_back(dataPoint);
    }

    plotData->addPlotDataSegment(plotDataSegment);

    return true;
  }

} // namespace alch
