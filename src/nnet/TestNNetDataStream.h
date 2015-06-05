// -*- C++ -*-

#ifndef INCLUDED_nnet_TestNNetDataStream_h
#define INCLUDED_nnet_TestNNetDataStream_h

#include "nnet/NNetDataStream.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestNNetDataStream : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestNNetDataStream);

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

