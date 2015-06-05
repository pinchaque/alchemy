// -*- C++ -*-

#ifndef INCLUDED_stockdata_TestStockTime_h
#define INCLUDED_stockdata_TestStockTime_h

#include "stockdata/StockTime.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestStockTime : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestStockTime);

  CPPUNIT_TEST(test1);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();

};

} // namespace alch

#endif

