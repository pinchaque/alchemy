// -*- C++ -*-

#ifndef INCLUDED_stockalg_TestLogNormal3_h
#define INCLUDED_stockalg_TestLogNormal3_h

#include "stockalg/LogNormal3.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestLogNormal3 : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestLogNormal3);

  CPPUNIT_TEST(test1);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();

};

} // namespace alch

#endif

