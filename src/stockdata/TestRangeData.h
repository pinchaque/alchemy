// -*- C++ -*-

#ifndef INCLUDED_stockdata_TestRangeData_h
#define INCLUDED_stockdata_TestRangeData_h

#include "stockdata/RangeData.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestRangeData : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestRangeData);

  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(test2);
  CPPUNIT_TEST(test3);
  CPPUNIT_TEST(test4);
  CPPUNIT_TEST(test5);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();
  void test2();
  void test3();
  void test4();
  void test5();

};

} // namespace alch

#endif

