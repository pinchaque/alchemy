// -*- C++ -*-

#ifndef INCLUDED_nnet_TestNNetDataset_h
#define INCLUDED_nnet_TestNNetDataset_h

#include "nnet/NNetDataset.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestNNetDataset : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestNNetDataset);

  CPPUNIT_TEST(test1);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();


}; // class TestNNetDataset

} // namespace alch

#endif

