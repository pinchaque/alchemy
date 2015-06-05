// -*- C++ -*-

#ifndef INCLUDED_stockdata_TestIStockDataFile_h
#define INCLUDED_stockdata_TestIStockDataFile_h

#include "stockdata/IStockDataFile.h"
#include "autil/Context.h"
#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestIStockDataFile : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestIStockDataFile);

  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(test2);
  CPPUNIT_TEST(test3);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();
  void test2();
  void test3();

private:
  Context m_ctx;
};

} // namespace alch

#endif

