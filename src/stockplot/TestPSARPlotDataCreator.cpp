#include "TestPSARPlotDataCreator.h"

namespace alch
{

void TestPSARPlotDataCreator::setUp() 
{
  ;
}

void TestPSARPlotDataCreator::tearDown()
{
  m_ctx.dump(std::cerr);
}

namespace {
  RangeDataPtr getData()
  {
    RangeDataPtr rd(new RangeData);
    {
      RangeData::Point p;
      p.open = 0.50;
      p.close = 1.00;
      p.min = 0.30;
      p.max = 1.10;
      p.volume = 1000;
      p.tradeTime = boost::posix_time::from_iso_string("19990131T235959");
      rd->add(p);
    }

    {
      RangeData::Point p;
      p.open = 0.60;
      p.close = 0.70;
      p.min = 0.20;
      p.max = 0.80;
      p.volume = 2000;
      p.tradeTime = boost::posix_time::from_iso_string("20000131T235959");
      rd->add(p);
    }

    {
      RangeData::Point p;
      p.open = 0.80;
      p.close = 1.10;
      p.min = 0.80;
      p.max = 1.20;
      p.volume = 3000;
      p.tradeTime = boost::posix_time::from_iso_string("20010131T235959");
      rd->add(p);
    }

    {
      RangeData::Point p;
      p.open = 0.40;
      p.close = 0.60;
      p.min = 0.40;
      p.max = 0.90;
      p.volume = 3000;
      p.tradeTime = boost::posix_time::from_iso_string("20020131T235959");
      rd->add(p);
    }
    return rd;
  }
}


void TestPSARPlotDataCreator::test1()
{
  // set up the data
  RangeDataPtr data = getData();

  // initialize the creator
  PSARPlotDataCreator creator(m_ctx);
  creator.setData(data);
  creator.setID("Generic");
  creator.setAccel(0.02);
  creator.setAccel(0.20);

  // initialize the plot
  PlotPtr plot(new Plot);

  // populate the plot
  CPPUNIT_ASSERT(creator.create(plot));

  // make sure the plot has what we wanted in there

  const Plot::PlotDataPtrVec& plotDataVec = plot->getPlotData();

  CPPUNIT_ASSERT_EQUAL(1, int(plotDataVec.size()));

  PlotDataPtr plotData = plotDataVec[0];

  CPPUNIT_ASSERT(PlotData::STYLE_points == plotData->getStyle());
  CPPUNIT_ASSERT(plotData->getLabel() == "Generic Parabolic SAR");

  PlotDataSegmentPtrVec dataSegments = plotData->getDataSegments();

  CPPUNIT_ASSERT_EQUAL(1, int(dataSegments.size()));

  PlotDataSegmentPtr dataSegment = dataSegments[0];

  CPPUNIT_ASSERT_EQUAL(4, int(dataSegment->size()));

#if 0
  const double delta = 0.001;

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.85, (*dataSegment)[0].value, delta);
  CPPUNIT_ASSERT_EQUAL(boost::posix_time::from_iso_string("20000131T235959"),
                       (*dataSegment)[0].timestamp);


  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.90, (*dataSegment)[1].value, delta);
  CPPUNIT_ASSERT_EQUAL(boost::posix_time::from_iso_string("20010131T235959"),
                       (*dataSegment)[1].timestamp);


  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.85, (*dataSegment)[2].value, delta);
  CPPUNIT_ASSERT_EQUAL(boost::posix_time::from_iso_string("20020131T235959"),
                       (*dataSegment)[2].timestamp);
#endif  
}

} // namespace alch
