#include "TestFileStockDataSource.h"
#include <iostream>

namespace alch
{

void TestFileStockDataSource::setUp() 
{
  ;
}

void TestFileStockDataSource::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestFileStockDataSource::test1()
{
  std::string exch;
  exch = FileStockDataSource::getSymbolDirectory("/data/root", "nyse");
  CPPUNIT_ASSERT_EQUAL(std::string("/data/root/n"), exch);

  exch = FileStockDataSource::getSymbolDirectory("/data/root", "AMD");
  CPPUNIT_ASSERT_EQUAL(std::string("/data/root/a"), exch);

  exch = FileStockDataSource::getSymbolDirectory("/data/root", "a");
  CPPUNIT_ASSERT_EQUAL(std::string("/data/root/a"), exch);

  std::string sym;
  sym = FileStockDataSource::getSymbolFile("/data/root", "intl");
  CPPUNIT_ASSERT_EQUAL(std::string("/data/root/i/intl.dat"), sym);
}

void TestFileStockDataSource::test2()
{
  FileStockDataSource ds("testdata", m_ctx);

  {
    std::vector<StockID> stockList;
    CPPUNIT_ASSERT(ds.getStockList(stockList));

    CPPUNIT_ASSERT_EQUAL(4, int(stockList.size()));

    CPPUNIT_ASSERT(stockList.end() != find(stockList.begin(),
                                           stockList.end(),
                                           StockID("abc")));

    CPPUNIT_ASSERT(stockList.end() != find(stockList.begin(),
                                           stockList.end(),
                                           StockID("ghi")));

    CPPUNIT_ASSERT(stockList.end() != find(stockList.begin(),
                                           stockList.end(),
                                           StockID("jkl")));

    CPPUNIT_ASSERT(stockList.end() != find(stockList.begin(),
                                           stockList.end(),
                                           StockID("def")));
  }
}

void TestFileStockDataSource::test3()
{
  FileStockDataSource ds("testdata", m_ctx);

  {
    StockID id("abc");
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbol(id));
  }

  {
    StockID id("ghi");
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbol(id));
  }

  {
    StockID id("def");
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbol(id));
  }

  {
    StockID id("jkl");
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbol(id));
  }

  {
    StockID id("ghh");
    CPPUNIT_ASSERT_EQUAL(false, ds.hasSymbol(id));
  }

  {
    StockID id("xxx");
    CPPUNIT_ASSERT_EQUAL(false, ds.hasSymbol(id));
  }

  {
    StockID id("a");
    CPPUNIT_ASSERT_EQUAL(false, ds.hasSymbol(id));
  }

  {
    StockID id("123");
    CPPUNIT_ASSERT_EQUAL(false, ds.hasSymbol(id));
  }
}

void TestFileStockDataSource::test4()
{
  FileStockDataSource ds("testdata", m_ctx);

  {
    StockID id("abc");
    StockTime start(boost::posix_time::from_iso_string("20020131T235959"));
    StockTime end(boost::posix_time::from_iso_string("20020202T235959"));
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbolDate(id, start, end));
  }

  {
    StockID id("abc");
    StockTime start(boost::posix_time::from_iso_string("20020202T235959"));
    StockTime end(boost::posix_time::from_iso_string("20040202T235959"));
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbolDate(id, start, end));
  }

  {
    StockID id("abc");
    StockTime start(boost::posix_time::from_iso_string("19990131T235959"));
    StockTime end(boost::posix_time::from_iso_string("20040202T235959"));
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbolDate(id, start, end));
  }

  {
    StockID id("abc");
    StockTime start(boost::posix_time::from_iso_string("19980131T235959"));
    StockTime end(boost::posix_time::from_iso_string("19990202T235959"));
    CPPUNIT_ASSERT_EQUAL(false, ds.hasSymbolDate(id, start, end));
  }

  {
    StockID id("abc");
    StockTime start(boost::posix_time::from_iso_string("20040131T235959"));
    StockTime end(boost::posix_time::from_iso_string("20040202T235959"));
    CPPUNIT_ASSERT_EQUAL(false, ds.hasSymbolDate(id, start, end));
  }

  {
    StockID id("abc");
    StockTime start(boost::posix_time::from_iso_string("20020202T235959"));
    StockTime end(boost::posix_time::from_iso_string("20020302T235959"));
    CPPUNIT_ASSERT_EQUAL(false, ds.hasSymbolDate(id, start, end));
  }

}


void TestFileStockDataSource::test5()
{
  FileStockDataSource ds("testdata", m_ctx);
  double delta = 0.01;

  StockID id("abc");

  RangeData data;
  StockInfo info;

  CPPUNIT_ASSERT(ds.retrieve(id, info, data));
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
}


void TestFileStockDataSource::test6()
{
  FileStockDataSource ds("testdata", m_ctx);
  double delta = 0.01;

  {
    StockID id("abc");
    StockTime start(boost::posix_time::from_iso_string("20020131T235959"));
    StockTime end(boost::posix_time::from_iso_string("20020202T235959"));

    RangeData data;
    StockInfo info;
    CPPUNIT_ASSERT(ds.retrieveDate(id, start, end, info, data));

    CPPUNIT_ASSERT_EQUAL(1, int(data.size()));

    const RangeData::Point& pt1 = data.get(0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00, pt1.open, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.00, pt1.close, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.00, pt1.min, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.00, pt1.max, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.00, pt1.volume, delta);
    CPPUNIT_ASSERT_EQUAL(std::string("20020131T235959"),
                         boost::posix_time::to_iso_string(pt1.tradeTime));
  }

  {
    StockID id("abc");
    StockTime start(boost::posix_time::from_iso_string("20020202T235959"));
    StockTime end(boost::posix_time::from_iso_string("20040202T235959"));

    RangeData data;
    StockInfo info;
    CPPUNIT_ASSERT(ds.retrieveDate(id, start, end, info, data));

    CPPUNIT_ASSERT_EQUAL(1, int(data.size()));

    const RangeData::Point& pt2 = data.get(0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.00, pt2.open, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(11.05, pt2.close, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(12.10, pt2.min, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(13.15, pt2.max, delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(14.20, pt2.volume, delta);
    CPPUNIT_ASSERT_EQUAL(std::string("20031231T010203"),
                         boost::posix_time::to_iso_string(pt2.tradeTime));
  }

  {
    StockID id("abc");
    StockTime start(boost::posix_time::from_iso_string("19990131T235959"));
    StockTime end(boost::posix_time::from_iso_string("20040202T235959"));

    RangeData data;
    StockInfo info;
    CPPUNIT_ASSERT(ds.retrieveDate(id, start, end, info, data));

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
  }

  {
    StockID id("abc");
    StockTime start(boost::posix_time::from_iso_string("19980131T235959"));
    StockTime end(boost::posix_time::from_iso_string("19990202T235959"));

    RangeData data;
    StockInfo info;
    CPPUNIT_ASSERT(ds.retrieveDate(id, start, end, info, data));

    CPPUNIT_ASSERT_EQUAL(0, int(data.size()));
  }
}


void TestFileStockDataSource::test7()
{
  FileStockDataSource ds("testdata", m_ctx);

  RangeData origData;
  {
    StockID id("abc");
    StockInfo info;
    CPPUNIT_ASSERT(ds.retrieve(id, info, origData));
    CPPUNIT_ASSERT_EQUAL(2, int(origData.size()));    
  }

  {
    StockID id("test_abc");
    StockInfo info;
    StockTime start(origData.get(0).tradeTime);
    StockTime end(origData.get(1).tradeTime);
    CPPUNIT_ASSERT(ds.save(id, start, end, info, origData));
  }

  RangeData newData;
  {
    StockID id("test_abc");
    StockInfo info;
    CPPUNIT_ASSERT(ds.retrieve(id, info, newData));
    CPPUNIT_ASSERT_EQUAL(2, int(newData.size()));
    ds.remove(id);
  }

  CPPUNIT_ASSERT(origData == newData);
}

} // namespace alch
