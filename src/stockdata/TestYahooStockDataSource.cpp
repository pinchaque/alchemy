#include "TestYahooStockDataSource.h"

namespace alch
{

void TestYahooStockDataSource::setUp() 
{
  ;
}

void TestYahooStockDataSource::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestYahooStockDataSource::test1()
{
  // test removed
}

void TestYahooStockDataSource::test2()
{
  return; // this test takes too long

  YahooStockDataSource ds(m_ctx);

  std::vector<StockID> stockList;
  CPPUNIT_ASSERT(ds.getStockList(stockList));

  CPPUNIT_ASSERT(int(stockList.size()) > 1000);

  CPPUNIT_ASSERT(stockList.end() != find(stockList.begin(),
                                         stockList.end(),
                                         StockID("INTL")));

  CPPUNIT_ASSERT(stockList.end() != find(stockList.begin(),
                                         stockList.end(),
                                         StockID("AMD")));
  
  CPPUNIT_ASSERT(stockList.end() != find(stockList.begin(),
                                         stockList.end(),
                                         StockID("EBAY")));  
  
  CPPUNIT_ASSERT(stockList.end() != find(stockList.begin(),
                                         stockList.end(),
                                         StockID("MSFT")));  
}

void TestYahooStockDataSource::test3()
{
  YahooStockDataSource ds(m_ctx);

  {
    StockID id("DOW");
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbol(id));
  }

  {
    StockID id("IBM");
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbol(id));
  }

  {
    StockID id("ibm");
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbol(id));
  }

  {
    StockID id("XIBM");
    CPPUNIT_ASSERT_EQUAL(false, ds.hasSymbol(id));
  }

  {
    StockID id("MSFT");
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbol(id));
  }

  {
    StockID id("");
    CPPUNIT_ASSERT_EQUAL(false, ds.hasSymbol(id));
  }
}


void TestYahooStockDataSource::test4()
{
  YahooStockDataSource ds(m_ctx);

  /*
    IBM has these dates:

    6-17-2004,90.50,90.56,90.07,90.44,4180200
    6-18-2004,90.20,90.80,89.90,90.06,6470800
    6-21-2004,90.40,90.43,89.31,89.49,3843400
    6-22-2004,89.28,90.24,89.13,90.02,4149300
    6-23-2004,90.10,90.84,89.84,90.79,4354700
  */

  {
    StockID id("IBM");
    StockTime start(boost::posix_time::from_iso_string("20040617T000000"));
    StockTime end(boost::posix_time::from_iso_string("20040618T000000"));
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbolDate(id, start, end));
  }

  {
    StockID id("IBM");
    StockTime start(boost::posix_time::from_iso_string("20040617T000000"));
    StockTime end(boost::posix_time::from_iso_string("20040622T000000"));
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbolDate(id, start, end));
  }

  {
    StockID id("IBM");
    StockTime start(boost::posix_time::from_iso_string("20040617T235959"));
    StockTime end(boost::posix_time::from_iso_string("20040618T000000"));
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbolDate(id, start, end));
  }

  {
    StockID id("IBM");
    StockTime start(boost::posix_time::from_iso_string("20040617T000001"));
    StockTime end(boost::posix_time::from_iso_string("20040617T235959"));
    CPPUNIT_ASSERT_EQUAL(true, ds.hasSymbolDate(id, start, end));
  }

  {
    StockID id("IBM");
    StockTime start(boost::posix_time::from_iso_string("20040619T000001"));
    StockTime end(boost::posix_time::from_iso_string("20040620T235959"));
    CPPUNIT_ASSERT_EQUAL(false, ds.hasSymbolDate(id, start, end));
  }
}

void TestYahooStockDataSource::test5()
{
  YahooStockDataSource ds(m_ctx);
  double delta = 0.01;

  StockID id("IBM");

  RangeData data;
  StockInfo info;

  CPPUNIT_ASSERT(ds.retrieve(id, info, data));

  // IBM had 10721 data points when I wrote this on 2004-08-04
  int dataSize = int(data.size());
  CPPUNIT_ASSERT(10720 < dataSize);

  const RangeData::Point& pt1 = data.get(0);
  const RangeData::Point& pt2 = data.get(dataSize - 1);

  // first data point was way back in 1962
  // note that these values may be invalidated if the ta_yahoo program 
  // adjusts them for dividends or splits. You can regenerate these values
  // by running: 'ta_yahoo -dd US.NYSE.STOCK IBM | head -n 1'
  //

  CPPUNIT_ASSERT_DOUBLES_EQUAL(578.50, pt1.open, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(578.50, pt1.max, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(572.0, pt1.min, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(572.0, pt1.close, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(387200, pt1.volume, delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.79, pt1.adjustedClose, delta);
  CPPUNIT_ASSERT_EQUAL(std::string("19620102T000000"),
                       boost::posix_time::to_iso_string(pt1.tradeTime));

  // who knows what the last data point was? it should have been recent, 
  // however!
  CPPUNIT_ASSERT(pt2.open > 0.00);
  CPPUNIT_ASSERT(pt2.close > 0.00);
  CPPUNIT_ASSERT(pt2.min > 0.00);
  CPPUNIT_ASSERT(pt2.max > 0.00);
  CPPUNIT_ASSERT(pt2.volume > 0.00);
  
  CPPUNIT_ASSERT_EQUAL(std::string("International Business Machines Corp."),
      info.getName());

  boost::posix_time::time_duration duration = 
    (boost::posix_time::second_clock::local_time() - pt2.tradeTime);


  // the trade time must have been somewhere in the past
  CPPUNIT_ASSERT(duration.total_seconds() >= 0);

  // but not more than one week!
  CPPUNIT_ASSERT(duration.hours() < 168);
}


void TestYahooStockDataSource::test6()
{
  YahooStockDataSource ds(m_ctx);
  double delta = 0.01;

  /*
    IBM has these dates:

    6-17-2004,90.50,90.56,90.07,90.44,4180200
    6-18-2004,90.20,90.80,89.90,90.06,6470800
    6-21-2004,90.40,90.43,89.31,89.49,3843400
    6-22-2004,89.28,90.24,89.13,90.02,4149300
    6-23-2004,90.10,90.84,89.84,90.79,4354700

    This method only compares volumes to help avoid any problems with 
    data renormalization that ta_yahoo may do.
  */

  {
    StockID id("IBM");
    RangeData data;
    StockInfo info;
    StockTime start(boost::posix_time::from_iso_string("20040617T000000"));
    StockTime end(boost::posix_time::from_iso_string("20040618T000000"));

    CPPUNIT_ASSERT(ds.retrieveDate(id, start, end, info, data));

    CPPUNIT_ASSERT_EQUAL(2, int(data.size()));
    
    const RangeData::Point& pt1 = data.get(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4180200, pt1.volume, delta);
    
    const RangeData::Point& pt2 = data.get(1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6470800, pt2.volume, delta);
  }

  {
    StockID id("IBM");
    RangeData data;
    StockInfo info;
    StockTime start(boost::posix_time::from_iso_string("20040617T000000"));
    StockTime end(boost::posix_time::from_iso_string("20040622T000000"));

    CPPUNIT_ASSERT(ds.retrieveDate(id, start, end, info, data));

    CPPUNIT_ASSERT_EQUAL(4, int(data.size()));
    
    const RangeData::Point& pt1 = data.get(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4180200, pt1.volume, delta);
    
    const RangeData::Point& pt2 = data.get(3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4149300, pt2.volume, delta);
  }

  {
    StockID id("IBM");
    RangeData data;
    StockInfo info;
    StockTime start(boost::posix_time::from_iso_string("20040618T000000"));
    StockTime end(boost::posix_time::from_iso_string("20040618T000000"));

    CPPUNIT_ASSERT(ds.retrieveDate(id, start, end, info, data));

    CPPUNIT_ASSERT_EQUAL(1, int(data.size()));
    
    const RangeData::Point& pt1 = data.get(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6470800, pt1.volume, delta);
  }

  {
    StockID id("IBM");
    RangeData data;
    StockInfo info;
    StockTime start(boost::posix_time::from_iso_string("20040619T000001"));
    StockTime end(boost::posix_time::from_iso_string("20040621T000000"));

    CPPUNIT_ASSERT(ds.retrieveDate(id, start, end, info, data));

    CPPUNIT_ASSERT_EQUAL(1, int(data.size()));
    
    const RangeData::Point& pt1 = data.get(0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3843400, pt1.volume, delta);
  }
}

void TestYahooStockDataSource::test7()
{
  YahooStockDataSource ds(m_ctx);
  StockID id("IBM");
  RangeData data;
  StockInfo info;
  StockTime start(boost::posix_time::from_iso_string("20040617T000000"));
  StockTime end(boost::posix_time::from_iso_string("20040618T000000"));

  // these aren't implemented for yahoo, so we expect them to fail
  CPPUNIT_ASSERT_EQUAL(false, ds.save(id, start, end, info, data));
  CPPUNIT_ASSERT_EQUAL(false, ds.remove(id));
}

void TestYahooStockDataSource::test8()
{
  YahooStockDataSource ds(m_ctx);
  StockID id("VEXMX");
  RangeData data;
  StockInfo info;
  StockTime start(boost::posix_time::from_iso_string("20040617T000000"));
  StockTime end(boost::posix_time::from_iso_string("20040618T000000"));

  CPPUNIT_ASSERT(ds.retrieveDate(id, start, end, info, data));

  CPPUNIT_ASSERT_EQUAL(std::string("Vanguard"), info.getFamily());
  CPPUNIT_ASSERT_EQUAL(3, info.getRating());
  CPPUNIT_ASSERT_EQUAL(std::string("19871221T000000"),
                       boost::posix_time::to_iso_string(info.getInception()));
  CPPUNIT_ASSERT_EQUAL(3000, info.getInitInvestment());
  CPPUNIT_ASSERT_EQUAL(6410000, info.getAssets());
  CPPUNIT_ASSERT_EQUAL(100, info.getSubsInvestment());
  CPPUNIT_ASSERT_EQUAL(std::string("Vanguard Extended Market Idx"), 
    info.getName());
  CPPUNIT_ASSERT_EQUAL(std::string("Mid-Cap Blend"), info.getCategory());

  double delta = 0.01;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25, info.getExpenseRatio(), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00, info.get12b1Fee(), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(16.00, info.getTurnover(), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.03, info.getYield(), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00, info.getDeferredLoad(), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00, info.getFrontLoad(), delta);

}
} // namespace alch
