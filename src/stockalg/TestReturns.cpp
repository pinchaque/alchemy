#include "TestReturns.h"

namespace alch
{

void TestReturns::setUp() 
{
  ;
}

void TestReturns::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestReturns::test1()
{
  RangeDataPtr rd(new RangeData());
  {
    RangeData::Point p;
    p.open = 0.50;
    p.close = 1.00;
    p.tradeTime = boost::posix_time::from_iso_string("19990101T000000");
    rd->add(p);
  }
  {
    RangeData::Point p;
    p.open = 0.20;
    p.close = 0.30;
    p.tradeTime = boost::posix_time::from_iso_string("19990701T000000");
    rd->add(p);
  }
  {
    RangeData::Point p;
    p.open = 0.75;
    p.close = 1.50;
    p.tradeTime = boost::posix_time::from_iso_string("20000101T000000");
    rd->add(p);
  }

  double delta = 0.01;
  boost::posix_time::time_duration dur(0,0,86400*365,0);
  CPPUNIT_ASSERT_EQUAL(dur.total_seconds(), 
      Returns::duration(rd).total_seconds());
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, Returns::total(rd), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, Returns::annualized(rd), delta);
}

void TestReturns::test2()
{
  RangeDataPtr rd(new RangeData());
  {
    RangeData::Point p;
    p.open = 0.50;
    p.close = 1.00;
    p.tradeTime = boost::posix_time::from_iso_string("19990101T000000");
    rd->add(p);
  }
  {
    RangeData::Point p;
    p.open = 0.20;
    p.close = 0.30;
    p.tradeTime = boost::posix_time::from_iso_string("19990701T000000");
    rd->add(p);
  }
  {
    RangeData::Point p;
    p.open = 0.75;
    p.close = 2.25;
    p.tradeTime = boost::posix_time::from_iso_string("20010101T000000");
    rd->add(p);
  }

  double delta = 0.01;
  boost::posix_time::time_duration dur(0,0,86400*(365+366),0);
  CPPUNIT_ASSERT_EQUAL(dur.total_seconds(), 
      Returns::duration(rd).total_seconds());
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.25, Returns::total(rd), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, Returns::annualized(rd), delta);
}

} // namespace alch
