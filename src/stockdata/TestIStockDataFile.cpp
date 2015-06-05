#include "TestIStockDataFile.h"
#include <sstream>
#include <iostream>

namespace alch
{

void TestIStockDataFile::setUp() 
{
  ;
}

void TestIStockDataFile::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestIStockDataFile::test1()
{
  const char* fname = "testdata/stockdata1.dat";
  const double delta = 0.01;

  IStockDataFile f(m_ctx);

  CPPUNIT_ASSERT_EQUAL(std::string(""), f.getFileName());

  CPPUNIT_ASSERT(f.open(fname));

  CPPUNIT_ASSERT_EQUAL(std::string(fname), f.getFileName());

  {
    RangeData::Point pt1;
    RangeData::Point pt2;
    RangeData::Point pt3;

    CPPUNIT_ASSERT(f.read(pt1));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00, pt1.open, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.00, pt1.close, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.00, pt1.min, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.00, pt1.max, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.00, pt1.volume, delta);
    CPPUNIT_ASSERT_EQUAL(std::string("20020131T235959"),
                         boost::posix_time::to_iso_string(pt1.tradeTime));

    CPPUNIT_ASSERT(f.read(pt2));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.00, pt2.open, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(11.05, pt2.close, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(12.10, pt2.min, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13.15, pt2.max, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(14.20, pt2.volume, delta);
    CPPUNIT_ASSERT_EQUAL(std::string("20031231T010203"),
                         boost::posix_time::to_iso_string(pt2.tradeTime));

    CPPUNIT_ASSERT(!f.read(pt3));
  }


  CPPUNIT_ASSERT(f.rewind());

  {
    RangeData data;
    CPPUNIT_ASSERT(f.read(data));
    CPPUNIT_ASSERT_EQUAL(2, int(data.size()));    

    const RangeData::Point& pt1 = data.get(0);
    const RangeData::Point& pt2 = data.get(1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00, pt1.open, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.00, pt1.close, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.00, pt1.min, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.00, pt1.max, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.00, pt1.volume, delta);
    CPPUNIT_ASSERT_EQUAL(std::string("20020131T235959"),
                         boost::posix_time::to_iso_string(pt1.tradeTime));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.00, pt2.open, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(11.05, pt2.close, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(12.10, pt2.min, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13.15, pt2.max, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(14.20, pt2.volume, delta);
    CPPUNIT_ASSERT_EQUAL(std::string("20031231T010203"),
                         boost::posix_time::to_iso_string(pt2.tradeTime));

    data.clear();

    CPPUNIT_ASSERT(f.read(data));
    CPPUNIT_ASSERT_EQUAL(0, int(data.size()));
  }


  f.close();
  CPPUNIT_ASSERT_EQUAL(std::string(""), f.getFileName());
}

void TestIStockDataFile::test2()
{
  const char* fname = "testdata/stockdata2.dat";

  IStockDataFile f(m_ctx);

  CPPUNIT_ASSERT_EQUAL(std::string(""), f.getFileName());

  CPPUNIT_ASSERT(f.open(fname));

  CPPUNIT_ASSERT_EQUAL(std::string(fname), f.getFileName());

  RangeData data;
  CPPUNIT_ASSERT(!f.read(data));
  CPPUNIT_ASSERT_EQUAL(0, int(data.size()));    
}

void TestIStockDataFile::test3()
{
  const char* fname = "testdata/stockdata3.dat";
  const double delta = 0.01;

  IStockDataFile f(m_ctx);

  CPPUNIT_ASSERT_EQUAL(std::string(""), f.getFileName());

  CPPUNIT_ASSERT(f.open(fname));

  CPPUNIT_ASSERT_EQUAL(std::string(fname), f.getFileName());

  RangeData data;
  CPPUNIT_ASSERT(!f.read(data));
  CPPUNIT_ASSERT_EQUAL(1, int(data.size()));    

  const RangeData::Point& pt1 = data.get(0);
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.00, pt1.open, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.00, pt1.close, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.00, pt1.min, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.00, pt1.max, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.00, pt1.volume, delta);
  CPPUNIT_ASSERT_EQUAL(std::string("20031231T010203"),
                       boost::posix_time::to_iso_string(pt1.tradeTime));
}

} // namespace alch
