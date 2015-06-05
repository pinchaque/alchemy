#include "TestStockDataStream.h"
#include <sstream>
#include <iostream>

namespace alch
{

void TestStockDataStream::setUp() 
{
  ;
}

void TestStockDataStream::tearDown()
{
  ;
}

const char* testStr1 =
  "20020131T235959,0.00,1.00,2.00,3.00,4.00,1.23\n"
  "20031231T010203,10.00,11.00,12.00,13.00,14.00,1.23\n";

void TestStockDataStream::test1()
{
  std::string str(testStr1);
  double delta = 0.01;

  Context ctx;
  RangeData::Point pt;
  std::istringstream iss(str);

  bool val = StockDataStream::read(iss, pt, ctx);
  ctx.dump(std::cerr);
  CPPUNIT_ASSERT(val);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00, pt.open, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.00, pt.close, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.00, pt.min, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.00, pt.max, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.00, pt.volume, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.23, pt.adjustedClose, delta);
  CPPUNIT_ASSERT_EQUAL(std::string("20020131T235959"),
                       boost::posix_time::to_iso_string(pt.tradeTime));

  val = StockDataStream::read(iss, pt, ctx);
  ctx.dump(std::cerr);
  CPPUNIT_ASSERT(val);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10.00, pt.open, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(11.00, pt.close, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(12.00, pt.min, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(13.00, pt.max, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(14.00, pt.volume, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.23, pt.adjustedClose, delta);
  CPPUNIT_ASSERT_EQUAL(std::string("20031231T010203"),
                       boost::posix_time::to_iso_string(pt.tradeTime));
}

void TestStockDataStream::test2()
{
  std::string str(testStr1);
  double delta = 0.01;

  Context ctx;
  RangeData::Point pt1;
  RangeData::Point pt2;

  std::istringstream iss(str);

  {
    bool val = StockDataStream::read(iss, pt1, ctx);
    ctx.dump(std::cerr);
    CPPUNIT_ASSERT(val);

    std::ostringstream oss;
    val = StockDataStream::write(oss, pt1, ctx);
    ctx.dump(std::cerr);
    CPPUNIT_ASSERT(val);

    std::istringstream iss2(oss.str());
    val = StockDataStream::read(iss2, pt2, ctx);
    ctx.dump(std::cerr);
    CPPUNIT_ASSERT(val);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pt1.open, pt2.open, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pt1.close, pt2.close, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pt1.min, pt2.min, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pt1.max, pt2.max, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pt1.volume, pt2.volume, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pt1.adjustedClose, pt2.adjustedClose, delta);
    CPPUNIT_ASSERT(pt1.tradeTime == pt2.tradeTime);
  }

  {
    bool val = StockDataStream::read(iss, pt1, ctx);
    ctx.dump(std::cerr);
    CPPUNIT_ASSERT(val);

    std::ostringstream oss;
    val = StockDataStream::write(oss, pt1, ctx);
    ctx.dump(std::cerr);
    CPPUNIT_ASSERT(val);

    std::istringstream iss2(oss.str());
    val = StockDataStream::read(iss2, pt2, ctx);
    ctx.dump(std::cerr);
    CPPUNIT_ASSERT(val);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pt1.open, pt2.open, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pt1.close, pt2.close, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pt1.min, pt2.min, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pt1.max, pt2.max, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pt1.volume, pt2.volume, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pt1.adjustedClose, pt2.adjustedClose, delta);
    CPPUNIT_ASSERT(pt1.tradeTime == pt2.tradeTime);
  }
}
} // namespace alch
