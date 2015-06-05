// -*- C++ -*-

#ifndef INCLUDED_stockdata_TestRangeDataAlg_h
#define INCLUDED_stockdata_TestRangeDataAlg_h

#include "stockdata/RangeDataAlg.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestRangeDataAlg : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestRangeDataAlg);

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

