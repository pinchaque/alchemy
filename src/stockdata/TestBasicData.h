// -*- C++ -*-

#ifndef INCLUDED_stockdata_TestBasicData_h
#define INCLUDED_stockdata_TestBasicData_h

#include "stockdata/BasicData.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestBasicData : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestBasicData);

  CPPUNIT_TEST(test1);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();

};

} // namespace alch

#endif

