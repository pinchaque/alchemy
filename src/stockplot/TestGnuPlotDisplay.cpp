#include "TestGnuPlotDisplay.h"

#include "stockplot/PlotList.h"

#include <sstream>
#include <cmath>

namespace alch
{
  using namespace boost::posix_time;
  using namespace boost::gregorian;

  namespace
  {
    void createPlotList(PlotListPtr plotListPtr)
    {
      plotListPtr->setTitle("TestGnuPlotDisplay");
      plotListPtr->setXMin(time_from_string("2004-01-01 23:59:59.000"));
      plotListPtr->setXMax(time_from_string("2004-01-10 23:59:59.000"));

      PlotPtr plotPtr(new Plot());
      
      plotPtr->setYMin(10.00);
      plotPtr->setYMax(20.00);
      
      for (int idx = 0; idx < 2; ++idx)
      {

        double startValue = 11.00 + double(idx);

        PlotDataPtr plotDataPtr(new PlotData());

        std::stringstream ss;
        ss << "PlotData " << idx;
        plotDataPtr->setLabel(ss.str());

        plotDataPtr->setStyle(PlotData::STYLE_lines);

        PlotDataSegmentPtr plotDataSegment(new PlotDataSegment);

        for (int day = 1; day < 10; ++day)
        {
          PlotDataPoint point;
          point.timestamp
            = StockTime(date(2004, Jan, day), time_duration(0, 0, 0));
          point.value = startValue + 4.0 + (4.5 * sin(day));
          plotDataSegment->push_back(point);
        }

        plotDataPtr->addPlotDataSegment(plotDataSegment);
        plotPtr->addPlotData(plotDataPtr);
      }

      plotListPtr->addPlot(plotPtr);
    }
  }

void TestGnuPlotDisplay::setUp() 
{
  ;
}

void TestGnuPlotDisplay::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestGnuPlotDisplay::test1()
{
  PlotListPtr plotListPtr(new PlotList());

  createPlotList(plotListPtr);

#ifndef NDEBUG
  plotListPtr->dump(std::cerr);
#endif

  Context ctx;
  GnuPlotDisplay display(ctx);

#ifndef NDEBUG
//  CPPUNIT_ASSERT(display.show(plotListPtr));
  display.dump(std::cerr);
#endif
}

} // namespace alch
