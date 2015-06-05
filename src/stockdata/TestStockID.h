// -*- C++ -*-

#ifndef INCLUDED_stockdata_TestStockID_h
#define INCLUDED_stockdata_TestStockID_h

#include "stockdata/StockID.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestStockID : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestStockID);

  CPPUNIT_TEST(test1);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();

};

} // namespace alch

#endif

