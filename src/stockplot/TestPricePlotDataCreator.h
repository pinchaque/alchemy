// -*- C++ -*-

#ifndef INCLUDED_stockplot_TestPricePlotDataCreator_h
#define INCLUDED_stockplot_TestPricePlotDataCreator_h

#include "stockplot/PricePlotDataCreator.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestPricePlotDataCreator : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestPricePlotDataCreator);

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

