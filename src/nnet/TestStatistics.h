// -*- C++ -*-

#ifndef INCLUDED_nnet_TestStatistics_h
#define INCLUDED_nnet_TestStatistics_h

#include "nnet/Statistics.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestStatistics : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestStatistics);

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

