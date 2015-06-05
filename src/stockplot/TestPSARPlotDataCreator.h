// -*- C++ -*-

#ifndef INCLUDED_stockplot_TestPSARPlotDataCreator_h
#define INCLUDED_stockplot_TestPSARPlotDataCreator_h

#include "stockplot/PSARPlotDataCreator.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestPSARPlotDataCreator : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestPSARPlotDataCreator);

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

