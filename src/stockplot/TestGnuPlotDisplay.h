// -*- C++ -*-

#ifndef INCLUDED_stockplot_TestGnuPlotDisplay_h
#define INCLUDED_stockplot_TestGnuPlotDisplay_h

#include "stockplot/GnuPlotDisplay.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestGnuPlotDisplay : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestGnuPlotDisplay);

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

