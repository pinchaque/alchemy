// -*- C++ -*-

#ifndef INCLUDED_stockdata_TestStockDataStream_h
#define INCLUDED_stockdata_TestStockDataStream_h

#include "stockdata/StockDataStream.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestStockDataStream : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestStockDataStream);

  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(test2);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();
  void test2();

};

} // namespace alch

#endif

