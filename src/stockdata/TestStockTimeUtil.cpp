#include "TestStockTimeUtil.h"

namespace alch
{

  using namespace boost::posix_time;
  using namespace boost::gregorian;
  using namespace StockTimeUtil;

void TestStockTimeUtil::setUp() 
{
  ;
}

void TestStockTimeUtil::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestStockTimeUtil::test1()
{
  {
    StockTime orig(from_iso_string("20050201T000000")); // Tuesday
    StockTime nextClose(getNextClose(orig));
    StockTime prevClose(getPreviousClose(orig));

    CPPUNIT_ASSERT(nextClose == from_iso_string("20050201T130000"));
    CPPUNIT_ASSERT(prevClose == from_iso_string("20050131T130000"));
  }

  {
    StockTime orig(from_iso_string("20050201T140000")); // Tuesday
    StockTime nextClose(getNextClose(orig));
    StockTime prevClose(getPreviousClose(orig));

    CPPUNIT_ASSERT(nextClose == from_iso_string("20050202T130000"));
    CPPUNIT_ASSERT(prevClose == from_iso_string("20050201T130000"));
  }

  {
    StockTime orig(from_iso_string("20050204T140000")); // Friday
    StockTime nextClose(getNextClose(orig));
    StockTime prevClose(getPreviousClose(orig));
    CPPUNIT_ASSERT(nextClose == from_iso_string("20050207T130000"));
    CPPUNIT_ASSERT(prevClose == from_iso_string("20050204T130000"));
  }

  {
    StockTime orig(from_iso_string("20050205T140000")); // Saturday
    StockTime nextClose(getNextClose(orig));
    StockTime prevClose(getPreviousClose(orig));

    CPPUNIT_ASSERT(nextClose == from_iso_string("20050207T130000"));
    CPPUNIT_ASSERT(prevClose == from_iso_string("20050204T130000"));
  }

  {
    StockTime orig(from_iso_string("20050206T140000")); // Sunday
    StockTime nextClose(getNextClose(orig));
    StockTime prevClose(getPreviousClose(orig));

    CPPUNIT_ASSERT(nextClose == from_iso_string("20050207T130000"));
    CPPUNIT_ASSERT(prevClose == from_iso_string("20050204T130000"));
  }

  {
    StockTime orig(from_iso_string("20050207T125959")); // Monday
    StockTime nextClose(getNextClose(orig));
    StockTime prevClose(getPreviousClose(orig));

    CPPUNIT_ASSERT(nextClose == from_iso_string("20050207T130000"));
    CPPUNIT_ASSERT(prevClose == from_iso_string("20050204T130000"));
  }

  {
    StockTime orig(from_iso_string("20050207T130000")); // Monday
    StockTime nextClose(getNextClose(orig));
    StockTime prevClose(getPreviousClose(orig));

    CPPUNIT_ASSERT(nextClose == from_iso_string("20050208T130000"));
    CPPUNIT_ASSERT(prevClose == from_iso_string("20050207T130000"));
  }
}

} // namespace alch
