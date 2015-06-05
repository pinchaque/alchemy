#include "TestStockDataRetriever.h"

#include "stockdata/FileStockDataSource.h"

#include "boost/filesystem/operations.hpp"

#include <iostream>
#include <unistd.h>

namespace alch
{

  using namespace boost::posix_time;
  using namespace boost::gregorian;
  using namespace boost::filesystem;

void TestStockDataRetriever::setUp() 
{
  ;
}

void TestStockDataRetriever::tearDown()
{
  m_ctx.dump(std::cerr);
}

namespace {
  bool cacheExists(const char* dirName, const StockID& id)
  {
    return exists(FileStockDataSource::getSymbolFile(dirName, id.getSymbol()));
  }
}

void TestStockDataRetriever::test1()
{
  const char* dirName = "testdatacache";
  m_ctx.setPriorityFilter(Context::PRIORITY_all);

  // clean up cache
  boost::filesystem::remove_all(dirName);
  CPPUNIT_ASSERT(!boost::filesystem::exists(dirName));

  // initiate cache directory
  boost::filesystem::create_directory(dirName);
  CPPUNIT_ASSERT(boost::filesystem::is_directory(dirName));

  StockDataRetriever retriever(dirName, m_ctx);
  StockID id("MSFT");

  /*
    Command-line equivalent call:
    ta_yahoo -dd US.NASDAQ.STOCK MSFT | grep -c '^1-[0-9]*-2002,'
    21
   */

  // test for cache file existance
  CPPUNIT_ASSERT(!cacheExists(dirName, id));

  // this first attempt to get the data should go to yahoo
  RangeData yahooData;
  StockInfo yahooInfo;
  {
    // get data for January 2002
    StockTime startTime(from_iso_string("20020101T000000"));
    StockTime endTime(from_iso_string("20020131T235959"));
    CPPUNIT_ASSERT(retriever.retrieve(id, startTime, endTime, 
          yahooInfo, yahooData));

    // make sure we got correct number of data points
    CPPUNIT_ASSERT_EQUAL(int(21), int(yahooData.size()));
  }

  // test for cache file existance
  CPPUNIT_ASSERT(cacheExists(dirName, id));

  // this second attempt should be reading from the file on disk
  RangeData fileData;
  StockInfo fileInfo;
  {
    // get data for January 2002
    StockTime startTime(from_iso_string("20020101T000000"));
    StockTime endTime(from_iso_string("20020131T235959"));
    CPPUNIT_ASSERT(retriever.retrieve(id, startTime, endTime, 
          fileInfo, fileData));

    // make sure we got correct number of data points
    CPPUNIT_ASSERT_EQUAL(int(21), int(fileData.size()));
  }

  CPPUNIT_ASSERT(yahooData == fileData);
  CPPUNIT_ASSERT(yahooInfo == fileInfo);
}

} // namespace alch
