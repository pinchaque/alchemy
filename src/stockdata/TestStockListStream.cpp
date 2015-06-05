#include "TestStockListStream.h"
#include <sstream>
#include <iostream>

namespace alch
{

void TestStockListStream::setUp() 
{
  ;
}

void TestStockListStream::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestStockListStream::test1()
{

  const char* result = 
    "vasgx\n"
    "vivax\n"
    "ebay\n"
    ;

  std::ostringstream os;

  StockList data;
  data.add(StockID("vasgx"));
  data.add(StockID("VIVAX"));
  data.add(StockID("ebay"));

  CPPUNIT_ASSERT(StockListStream::write(os, data, m_ctx));

  CPPUNIT_ASSERT_EQUAL(std::string(result), os.str());

  std::istringstream is(result);
  StockList newData;
  CPPUNIT_ASSERT(StockListStream::read(is, newData, m_ctx));

  CPPUNIT_ASSERT_EQUAL(3, (int)(newData.size()));
  CPPUNIT_ASSERT_EQUAL(std::string("vasgx"), newData.get(0).getSymbol());
  CPPUNIT_ASSERT_EQUAL(std::string("vivax"), newData.get(1).getSymbol());
  CPPUNIT_ASSERT_EQUAL(std::string("ebay"), newData.get(2).getSymbol());
}

void TestStockListStream::test2()
{
  const char* result = 
    "# initial comment line\n"
    "vasgx\n"
    "   viVAX\n"
    "EBAY # extra comments\n"
    " # extra comments\n"
    ;
  std::istringstream is(result);
  StockList data;
  CPPUNIT_ASSERT(StockListStream::read(is, data, m_ctx));
  CPPUNIT_ASSERT_EQUAL(3, (int)(data.size()));
  CPPUNIT_ASSERT_EQUAL(std::string("vasgx"), data.get(0).getSymbol());
  CPPUNIT_ASSERT_EQUAL(std::string("vivax"), data.get(1).getSymbol());
  CPPUNIT_ASSERT_EQUAL(std::string("ebay"), data.get(2).getSymbol());
}

} // namespace alch
