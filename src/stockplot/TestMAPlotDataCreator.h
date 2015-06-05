// -*- C++ -*-

#ifndef INCLUDED_stockplot_TestMAPlotDataCreator_h
#define INCLUDED_stockplot_TestMAPlotDataCreator_h

#include "stockplot/MAPlotDataCreator.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestMAPlotDataCreator : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestMAPlotDataCreator);

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

