// -*- C++ -*-

#ifndef INCLUDED_stockalg_TestMovingAverage_h
#define INCLUDED_stockalg_TestMovingAverage_h

#include "stockalg/MovingAverage.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestMovingAverage : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestMovingAverage);

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

};

} // namespace alch

#endif

