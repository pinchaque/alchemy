// -*- C++ -*-

#ifndef INCLUDED_stockalg_TestPortfolioAlg_h
#define INCLUDED_stockalg_TestPortfolioAlg_h

#include "stockalg/PortfolioAlg.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestPortfolioAlg : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestPortfolioAlg);

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

