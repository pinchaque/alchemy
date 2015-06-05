// -*- C++ -*-

#ifndef INCLUDED_stockalg_TestMomentum_h
#define INCLUDED_stockalg_TestMomentum_h

#include "stockalg/Momentum.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestMomentum : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestMomentum);

  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(test2);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();
  void test2();

};

} // namespace alch

#endif

