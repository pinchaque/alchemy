// -*- C++ -*-

#ifndef INCLUDED_stockdata_TestStockDataRetriever_h
#define INCLUDED_stockdata_TestStockDataRetriever_h

#include "stockdata/StockDataRetriever.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestStockDataRetriever : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestStockDataRetriever);

  CPPUNIT_TEST(test1);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();

  private:
  Context m_ctx;
};

} // namespace alch

#endif

