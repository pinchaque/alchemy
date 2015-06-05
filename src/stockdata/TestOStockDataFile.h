// -*- C++ -*-

#ifndef INCLUDED_stockdata_TestOStockDataFile_h
#define INCLUDED_stockdata_TestOStockDataFile_h

#include "stockdata/OStockDataFile.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>
#include <string>

namespace alch
{


class TestOStockDataFile : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestOStockDataFile);

  CPPUNIT_TEST(test1);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();

private:
  std::string m_testFName;
  Context m_ctx;
};

} // namespace alch

#endif

