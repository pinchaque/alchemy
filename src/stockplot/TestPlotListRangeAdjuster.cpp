#include "TestPlotListRangeAdjuster.h"

namespace alch
{
  using namespace boost::posix_time;
  using namespace boost::gregorian;

void TestPlotListRangeAdjuster::setUp() 
{
  ;
}

void TestPlotListRangeAdjuster::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestPlotListRangeAdjuster::test1()
{
  PlotListRangeAdjuster a;

  const StockTime testTime(from_iso_string("20020131T235959"));


  CPPUNIT_ASSERT(a.getTimeGranularity()
                 == PlotListRangeAdjuster::TIMEGRANULARITY_month);

  // EXACT
  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_exact);
    StockTime roundedDate = a.roundTime(testTime, true);
    CPPUNIT_ASSERT_EQUAL(std::string("20020131T235959"),
                         to_iso_string(roundedDate));
  }

  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_exact);
    StockTime roundedDate = a.roundTime(testTime, false);
    CPPUNIT_ASSERT_EQUAL(std::string("20020131T235959"),
                         to_iso_string(roundedDate));
  }

  // DECADE
  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_decade);
    StockTime roundedDate = a.roundTime(testTime, true);
    CPPUNIT_ASSERT_EQUAL(std::string("20100101T000000"),
                         to_iso_string(roundedDate));
  }

  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_decade);
    StockTime roundedDate = a.roundTime(testTime, false);
    CPPUNIT_ASSERT_EQUAL(std::string("20000101T000000"),
                         to_iso_string(roundedDate));
  }

  // YEAR
  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_year);
    StockTime roundedDate = a.roundTime(testTime, true);
    CPPUNIT_ASSERT_EQUAL(std::string("20030101T000000"),
                         to_iso_string(roundedDate));
  }

  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_year);
    StockTime roundedDate = a.roundTime(testTime, false);
    CPPUNIT_ASSERT_EQUAL(std::string("20020101T000000"),
                         to_iso_string(roundedDate));
  }

  // HALFYEAR
  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_halfYear);
    StockTime roundedDate = a.roundTime(testTime, true);
    CPPUNIT_ASSERT_EQUAL(std::string("20020701T000000"),
                         to_iso_string(roundedDate));
  }

  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_halfYear);
    StockTime roundedDate = a.roundTime(testTime, false);
    CPPUNIT_ASSERT_EQUAL(std::string("20020101T000000"),
                         to_iso_string(roundedDate));
  }

  // QUARTER
  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_quarter);
    StockTime roundedDate = a.roundTime(testTime, true);
    CPPUNIT_ASSERT_EQUAL(std::string("20020401T000000"),
                         to_iso_string(roundedDate));
  }

  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_quarter);
    StockTime roundedDate = a.roundTime(testTime, false);
    CPPUNIT_ASSERT_EQUAL(std::string("20020101T000000"),
                         to_iso_string(roundedDate));
  }
  {
    const StockTime testTime2(from_iso_string("20021130T235959"));
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_quarter);
    StockTime roundedDate = a.roundTime(testTime2, true);
    CPPUNIT_ASSERT_EQUAL(std::string("20030101T000000"),
                         to_iso_string(roundedDate));
  }
  {
    const StockTime testTime2(from_iso_string("20021130T235959"));
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_quarter);
    StockTime roundedDate = a.roundTime(testTime2, false);
    CPPUNIT_ASSERT_EQUAL(std::string("20021001T000000"),
                         to_iso_string(roundedDate));
  }

  // MONTH
  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_month);
    StockTime roundedDate = a.roundTime(testTime, true);
    CPPUNIT_ASSERT_EQUAL(std::string("20020201T000000"),
                         to_iso_string(roundedDate));
  }

  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_month);
    StockTime roundedDate = a.roundTime(testTime, false);
    CPPUNIT_ASSERT_EQUAL(std::string("20020101T000000"),
                         to_iso_string(roundedDate));
  }

  // DAY
  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_day);
    StockTime roundedDate = a.roundTime(testTime, true);
    CPPUNIT_ASSERT_EQUAL(std::string("20020201T000000"),
                         to_iso_string(roundedDate));
  }

  {
    a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_day);
    StockTime roundedDate = a.roundTime(testTime, false);
    CPPUNIT_ASSERT_EQUAL(std::string("20020131T000000"),
                         to_iso_string(roundedDate));
  }
}


void TestPlotListRangeAdjuster::test2()
{
  PlotListRangeAdjuster a;

  const double testValue = 51.23;
  const double delta = 0.0001;

  CPPUNIT_ASSERT(a.getValueGranularity()
                 == PlotListRangeAdjuster::VALUEGRANULARITY_tenth);

  // EXACT
  {
    a.setValueGranularity(PlotListRangeAdjuster::VALUEGRANULARITY_exact);
    double roundedValue = a.roundDouble(testValue, true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(51.23, roundedValue, delta);
  }
  {
    a.setValueGranularity(PlotListRangeAdjuster::VALUEGRANULARITY_exact);
    double roundedValue = a.roundDouble(testValue, false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(51.23, roundedValue, delta);
  }

  // TENTH
  {
    a.setValueGranularity(PlotListRangeAdjuster::VALUEGRANULARITY_tenth);
    double roundedValue = a.roundDouble(testValue, true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(51.30, roundedValue, delta);
  }
  {
    a.setValueGranularity(PlotListRangeAdjuster::VALUEGRANULARITY_tenth);
    double roundedValue = a.roundDouble(testValue, false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(51.20, roundedValue, delta);
  }

  // ONE
  {
    a.setValueGranularity(PlotListRangeAdjuster::VALUEGRANULARITY_one);
    double roundedValue = a.roundDouble(testValue, true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(52.00, roundedValue, delta);
  }
  {
    a.setValueGranularity(PlotListRangeAdjuster::VALUEGRANULARITY_one);
    double roundedValue = a.roundDouble(testValue, false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(51.00, roundedValue, delta);
  }

  // TEN
  {
    a.setValueGranularity(PlotListRangeAdjuster::VALUEGRANULARITY_ten);
    double roundedValue = a.roundDouble(testValue, true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(60.00, roundedValue, delta);
  }
  {
    a.setValueGranularity(PlotListRangeAdjuster::VALUEGRANULARITY_ten);
    double roundedValue = a.roundDouble(testValue, false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(50.00, roundedValue, delta);
  }

}

namespace
{
  void createPlotList(PlotListPtr plotListPtr)
  {
    plotListPtr->setTitle("TestPlotListRangeAdjuster");
    plotListPtr->setXMin(time_from_string("2001-01-01 23:59:59.000"));
    plotListPtr->setXMax(time_from_string("2002-01-10 23:59:59.000"));

    PlotPtr plotPtr(new Plot());
      
    plotPtr->setYMin(100.00);
    plotPtr->setYMax(205.00);
      
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

void TestPlotListRangeAdjuster::test3()
{
  PlotListPtr plotList(new PlotList);
  createPlotList(plotList);

  PlotListRangeAdjuster a;
  a.setTimeGranularity(PlotListRangeAdjuster::TIMEGRANULARITY_day);
  a.setValueGranularity(PlotListRangeAdjuster::VALUEGRANULARITY_ten);

  a.adjust(plotList);

  CPPUNIT_ASSERT_EQUAL(std::string("20040101T000000"),
                       to_iso_string(plotList->getXMin()));

  CPPUNIT_ASSERT_EQUAL(std::string("20040110T000000"),
                       to_iso_string(plotList->getXMax()));

  CPPUNIT_ASSERT_EQUAL(1, int(plotList->getPlotList().size()));

  PlotPtr plot = plotList->getPlotList()[0];

  // min: ~10.79  max: ~20.51
  CPPUNIT_ASSERT_EQUAL(10.00, plot->getYMin());
  CPPUNIT_ASSERT_EQUAL(30.00, plot->getYMax());
}

} // namespace alch
