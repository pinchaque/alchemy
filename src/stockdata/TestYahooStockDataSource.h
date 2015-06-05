// -*- C++ -*-

#ifndef INCLUDED_stockdata_TestYahooStockDataSource_h
#define INCLUDED_stockdata_TestYahooStockDataSource_h

#include "stockdata/YahooStockDataSource.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestYahooStockDataSource : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestYahooStockDataSource);

  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(test2);
  CPPUNIT_TEST(test3);
  CPPUNIT_TEST(test4);
  CPPUNIT_TEST(test5);
  CPPUNIT_TEST(test6);
  CPPUNIT_TEST(test7);
  CPPUNIT_TEST(test8);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();
  void test2();
  void test3();
  void test4();
  void test5();
  void test6();
  void test7();
  void test8();

private:
  Context m_ctx;
};

} // namespace alch

#endif

