// -*- C++ -*-

#ifndef INCLUDED_stockplot_TestPlotListRangeAdjuster_h
#define INCLUDED_stockplot_TestPlotListRangeAdjuster_h

#include "stockplot/PlotListRangeAdjuster.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestPlotListRangeAdjuster : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestPlotListRangeAdjuster);

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

