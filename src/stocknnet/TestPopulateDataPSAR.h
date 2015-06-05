// -*- C++ -*-

#ifndef INCLUDED_stocknnet_TestPopulateDataPSAR_h
#define INCLUDED_stocknnet_TestPopulateDataPSAR_h

#include "stocknnet/PopulateDataPSAR.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestPopulateDataPSAR : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestPopulateDataPSAR);

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

