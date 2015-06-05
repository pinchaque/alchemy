#include "TestStockID.h"

namespace alch
{

void TestStockID::setUp() 
{
  ;
}

void TestStockID::tearDown()
{
  ;
}

void TestStockID::test1()
{
  {
    StockID id;

    CPPUNIT_ASSERT_EQUAL(std::string(""), id.getSymbol());

    id.setSymbol("AnotherSymbol");

    CPPUNIT_ASSERT_EQUAL(std::string("anothersymbol"), id.getSymbol());
  }

  {
    StockID id("AnotherSymbol");

    CPPUNIT_ASSERT_EQUAL(std::string("anothersymbol"), id.getSymbol());
  }

  CPPUNIT_ASSERT(StockID("foo") == StockID("Foo"));
  CPPUNIT_ASSERT(StockID("foo") != StockID("Foo2"));

}

} // namespace alch
