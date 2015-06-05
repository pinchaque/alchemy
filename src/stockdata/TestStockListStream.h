// -*- C++ -*-

#ifndef INCLUDED_stockdata_TestStockListStream_h
#define INCLUDED_stockdata_TestStockListStream_h

#include "stockdata/StockListStream.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestStockListStream : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestStockListStream);

  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(test2);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();
  void test2();


private:
  Context m_ctx;

};

} // namespace alch

#endif

