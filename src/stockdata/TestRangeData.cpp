#include "TestRangeData.h"

namespace alch
{

void TestRangeData::setUp() 
{
  ;
}

void TestRangeData::tearDown()
{
  ;
}



void TestRangeData::test1()
{
  RangeData rd;
  int i = 0;

  // add data points
  for (i = 0; i < 5; ++i)
  {
    RangeData::Point p;
    p.open = i + 0.01;
    p.close = i + 0.02;
    p.min = i + 0.03;
    p.max = i + 0.04;
    p.volume = i + 0.05;
    p.adjustedClose = i + 0.06;

    rd.add(p);

    CPPUNIT_ASSERT_EQUAL(i + 1, int(rd.size()));
  }
 
  // get data points directly
  for (i = 0; i < 5; ++i)
  {
    const RangeData::Point& p = rd.get(i);
    CPPUNIT_ASSERT_EQUAL(i + 0.01, p.open);
    CPPUNIT_ASSERT_EQUAL(i + 0.02, p.close);
    CPPUNIT_ASSERT_EQUAL(i + 0.03, p.min);
    CPPUNIT_ASSERT_EQUAL(i + 0.04, p.max);
    CPPUNIT_ASSERT_EQUAL(i + 0.05, p.volume);
    CPPUNIT_ASSERT_EQUAL(i + 0.06, p.adjustedClose);
  }

  // set close to be same as adjusted close
  rd.useAdjusted();

  // get data points using iterator
  RangeData::const_iterator end = rd.end();
  RangeData::const_iterator iter;
  i = 0;
  for (iter = rd.begin(); iter != end; ++iter, ++i)
  {
    const RangeData::Point& p = *iter;
    CPPUNIT_ASSERT_EQUAL(i + 0.01, p.open);
    CPPUNIT_ASSERT_EQUAL(i + 0.06, p.close);
    CPPUNIT_ASSERT_EQUAL(i + 0.06, p.adjustedClose);
    CPPUNIT_ASSERT_EQUAL(i + 0.03, p.min);
    CPPUNIT_ASSERT_EQUAL(i + 0.04, p.max);
    CPPUNIT_ASSERT_EQUAL(i + 0.05, p.volume);
  }

  rd.clear();
  CPPUNIT_ASSERT_EQUAL(0, int(rd.size()));
}


void TestRangeData::test2()
{
  RangeData rd;

  {
    RangeData::Point p;
    p.open = 0.50;
    p.close = 1.00;
    p.min = 0.30;
    p.max = 1.10;
    p.volume = 1000;
    p.tradeTime = boost::posix_time::from_iso_string("19990131T235959");
    rd.add(p);
  }

  {
    RangeData::Point p;
    p.open = 0.60;
    p.close = 0.70;
    p.min = 0.20;
    p.max = 0.80;
    p.volume = 2000;
    p.tradeTime = boost::posix_time::from_iso_string("20000131T235959");
    rd.add(p);
  }

  {
    RangeData::Point p;
    p.open = 0.80;
    p.close = 1.10;
    p.min = 0.80;
    p.max = 1.20;
    p.volume = 3000;
    p.tradeTime = boost::posix_time::from_iso_string("20010131T235959");
    rd.add(p);
  }

  {
    RangeData::Point p;
    p.open = 0.40;
    p.close = 0.60;
    p.min = 0.40;
    p.max = 0.90;
    p.volume = 3000;
    p.tradeTime = boost::posix_time::from_iso_string("20020131T235959");
    rd.add(p);
  }

  RangeData newData;
  rd.summarize(newData, 3);

  CPPUNIT_ASSERT_EQUAL(2, int(newData.size()));

  {
    const RangeData::Point& p = newData.get(0);
    CPPUNIT_ASSERT_EQUAL(0.50, p.open);
    CPPUNIT_ASSERT_EQUAL(1.10, p.close);
    CPPUNIT_ASSERT_EQUAL(0.20, p.min);
    CPPUNIT_ASSERT_EQUAL(1.20, p.max);
    CPPUNIT_ASSERT_EQUAL(2000.0, p.volume);
    CPPUNIT_ASSERT(boost::posix_time::from_iso_string("20010131T235959")
                   == p.tradeTime);
    rd.add(p);
  }

  {
    const RangeData::Point& p = newData.get(1);
    CPPUNIT_ASSERT_EQUAL(0.40, p.open);
    CPPUNIT_ASSERT_EQUAL(0.60, p.close);
    CPPUNIT_ASSERT_EQUAL(0.40, p.min);
    CPPUNIT_ASSERT_EQUAL(0.90, p.max);
    CPPUNIT_ASSERT_EQUAL(3000.0, p.volume);
    CPPUNIT_ASSERT(boost::posix_time::from_iso_string("20020131T235959")
                   == p.tradeTime);
    rd.add(p);
  }

}



void TestRangeData::test3()
{

  RangeData::Point p1;
  p1.open = 0.50;
  p1.close = 1.00;
  p1.min = 0.30;
  p1.max = 1.10;
  p1.volume = 1000;
  p1.tradeTime = boost::posix_time::from_iso_string("19990131T235959");

  RangeData::Point p2;
  p2.open = 0.60;
  p2.close = 0.70;
  p2.min = 0.20;
  p2.max = 0.80;
  p2.volume = 2000;
  p2.tradeTime = boost::posix_time::from_iso_string("20000131T235959");

  RangeData::Point p3;
  p3.open = 0.80;
  p3.close = 1.10;
  p3.min = 0.80;
  p3.max = 1.20;
  p3.volume = 3000;
  p3.tradeTime = boost::posix_time::from_iso_string("20010131T235959");

  RangeData::Point p4;
  p4.open = 0.40;
  p4.close = 0.60;
  p4.min = 0.40;
  p4.max = 0.90;
  p4.volume = 3000;
  p4.tradeTime = boost::posix_time::from_iso_string("20020131T235959");

  RangeData rd;
  rd.add(p3);
  rd.add(p2);
  rd.add(p4);
  rd.add(p1);

  // initial order
  CPPUNIT_ASSERT(rd.get(0) == p3);
  CPPUNIT_ASSERT(rd.get(1) == p2);
  CPPUNIT_ASSERT(rd.get(2) == p4);
  CPPUNIT_ASSERT(rd.get(3) == p1);

  rd.sortDate();

  // sorted order
  CPPUNIT_ASSERT(rd.get(0) == p1);
  CPPUNIT_ASSERT(rd.get(1) == p2);
  CPPUNIT_ASSERT(rd.get(2) == p3);
  CPPUNIT_ASSERT(rd.get(3) == p4);


  RangeData::Point p5;
  p5.open = 0.80;
  p5.close = 1.10;
  p5.min = 0.80;
  p5.max = 1.20;
  p5.volume = 3000;
  p5.tradeTime = boost::posix_time::from_iso_string("19970131T235959");

  RangeData::Point p6;
  p6.open = 0.40;
  p6.close = 0.60;
  p6.min = 0.40;
  p6.max = 0.90;
  p6.volume = 3000;
  p6.tradeTime = boost::posix_time::from_iso_string("20030131T235959");

  RangeData::Point p7;
  p7.open = 0.40;
  p7.close = 0.60;
  p7.min = 0.40;
  p7.max = 0.90;
  p7.volume = 3000;
  p7.tradeTime = boost::posix_time::from_iso_string("20010231T235959");

  RangeData rd2;
  rd2.add(p3);
  rd2.add(p5);
  rd2.add(p4);
  rd2.add(p6);
  rd2.add(p7);
  

  // initial order
  CPPUNIT_ASSERT(rd2.get(0) == p3);
  CPPUNIT_ASSERT(rd2.get(1) == p5);
  CPPUNIT_ASSERT(rd2.get(2) == p4);
  CPPUNIT_ASSERT(rd2.get(3) == p6);
  CPPUNIT_ASSERT(rd2.get(4) == p7);

  rd2.sortDate();

  // sorted order
  CPPUNIT_ASSERT(rd2.get(0) == p5);
  CPPUNIT_ASSERT(rd2.get(1) == p3);
  CPPUNIT_ASSERT(rd2.get(2) == p7);
  CPPUNIT_ASSERT(rd2.get(3) == p4);
  CPPUNIT_ASSERT(rd2.get(4) == p6);

  // test merging without removing dups
  {
    RangeData rdTest = rd;

    rdTest.merge(rd2, false);

    CPPUNIT_ASSERT(rdTest.size() == 9);
    CPPUNIT_ASSERT(rdTest.get(0) == p5);
    CPPUNIT_ASSERT(rdTest.get(1) == p1);
    CPPUNIT_ASSERT(rdTest.get(2) == p2);
    CPPUNIT_ASSERT(rdTest.get(3) == p3);
    CPPUNIT_ASSERT(rdTest.get(4) == p3);
    CPPUNIT_ASSERT(rdTest.get(5) == p7);
    CPPUNIT_ASSERT(rdTest.get(6) == p4);
    CPPUNIT_ASSERT(rdTest.get(7) == p4);
    CPPUNIT_ASSERT(rdTest.get(8) == p6);    
  }

  // test merging with removing dups
  {
    RangeData rdTest = rd;

    rdTest.merge(rd2, true);

    CPPUNIT_ASSERT(rdTest.size() == 7);
    CPPUNIT_ASSERT(rdTest.get(0) == p5);
    CPPUNIT_ASSERT(rdTest.get(1) == p1);
    CPPUNIT_ASSERT(rdTest.get(2) == p2);
    CPPUNIT_ASSERT(rdTest.get(3) == p3);
    CPPUNIT_ASSERT(rdTest.get(4) == p7);
    CPPUNIT_ASSERT(rdTest.get(5) == p4);
    CPPUNIT_ASSERT(rdTest.get(6) == p6);    
  }

}

/* test of monthly summary */
void TestRangeData::test4()
{
  RangeData rd;

  {
    RangeData::Point p;
    p.open = 0.50;
    p.close = 1.00;
    p.min = 0.30;
    p.max = 1.10;
    p.volume = 1000;
    p.tradeTime = boost::posix_time::from_iso_string("19990131T235959");
    rd.add(p);
  }

  {
    RangeData::Point p;
    p.open = 0.60;
    p.close = 0.70;
    p.min = 0.20;
    p.max = 0.80;
    p.volume = 2000;
    p.tradeTime = boost::posix_time::from_iso_string("19990201T235959");
    rd.add(p);
  }

  {
    RangeData::Point p;
    p.open = 0.80;
    p.close = 1.10;
    p.min = 0.80;
    p.max = 1.20;
    p.volume = 3000;
    p.tradeTime = boost::posix_time::from_iso_string("19990202T235959");
    rd.add(p);
  }

  {
    RangeData::Point p;
    p.open = 0.40;
    p.close = 0.60;
    p.min = 0.40;
    p.max = 0.90;
    p.volume = 3000;
    p.tradeTime = boost::posix_time::from_iso_string("19990220T235959");
    rd.add(p);
  }

  {
    RangeData::Point p;
    p.open = 0.50;
    p.close = 1.00;
    p.min = 0.30;
    p.max = 1.10;
    p.volume = 1000;
    p.tradeTime = boost::posix_time::from_iso_string("19990320T235959");
    rd.add(p);
  }

  RangeData newData;
  rd.summarizeMonthly(newData);

  CPPUNIT_ASSERT_EQUAL(3, int(newData.size()));

  {
    const RangeData::Point& p = newData.get(0);
    CPPUNIT_ASSERT_EQUAL(0.50, p.open);
    CPPUNIT_ASSERT_EQUAL(1.00, p.close);
    CPPUNIT_ASSERT_EQUAL(0.30, p.min);
    CPPUNIT_ASSERT_EQUAL(1.10, p.max);
    CPPUNIT_ASSERT_EQUAL(1000.0, p.volume);
    CPPUNIT_ASSERT(boost::posix_time::from_iso_string("19990131T235959")
                   == p.tradeTime);
    rd.add(p);
  }

  {
    const RangeData::Point& p = newData.get(1);
    CPPUNIT_ASSERT_EQUAL(0.60, p.open);
    CPPUNIT_ASSERT_EQUAL(0.60, p.close);
    CPPUNIT_ASSERT_EQUAL(0.20, p.min);
    CPPUNIT_ASSERT_EQUAL(1.20, p.max);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2666.6667, p.volume, 0.01);
    CPPUNIT_ASSERT(boost::posix_time::from_iso_string("19990220T235959")
                   == p.tradeTime);
    rd.add(p);
  }

  {
    const RangeData::Point& p = newData.get(2);
    CPPUNIT_ASSERT_EQUAL(0.50, p.open);
    CPPUNIT_ASSERT_EQUAL(1.00, p.close);
    CPPUNIT_ASSERT_EQUAL(0.30, p.min);
    CPPUNIT_ASSERT_EQUAL(1.10, p.max);
    CPPUNIT_ASSERT_EQUAL(1000.0, p.volume);
    CPPUNIT_ASSERT(boost::posix_time::from_iso_string("19990320T235959")
                   == p.tradeTime);
    rd.add(p);
  }
}


void TestRangeData::test5()
{
  RangeData rd;

  {
    RangeData::Point p;
    p.open = 0.50;
    p.close = 1.00;
    p.min = 0.30;
    p.max = 1.10;
    p.volume = 1000;
    p.tradeTime = boost::posix_time::from_iso_string("19990131T235959");
    rd.add(p);
  }

  {
    RangeData::Point p;
    p.open = 0.90;
    p.close = 0.60;
    p.min = 0.20;
    p.max = 0.80;
    p.volume = 2000;
    p.tradeTime = boost::posix_time::from_iso_string("20000131T235959");
    rd.add(p);
  }

  {
    RangeData::Point p;
    p.open = 0.80;
    p.close = 0.80;
    p.min = 0.80;
    p.max = 1.20;
    p.volume = 3000;
    p.tradeTime = boost::posix_time::from_iso_string("20010131T235959");
    rd.add(p);
  }

  {
    RangeData::Point p;
    p.open = 0.80;
    p.close = 0.72;
    p.min = 0.80;
    p.max = 1.20;
    p.volume = 3000;
    p.tradeTime = boost::posix_time::from_iso_string("20010131T235959");
    rd.add(p);
  }


  std::vector<double> returns;
  rd.getReturns(returns);

  double delta = 0.001;
  CPPUNIT_ASSERT_EQUAL(3, (int)returns.size());
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.4, returns[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.33333, returns[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1, returns[2], delta);
}

} // namespace alch
