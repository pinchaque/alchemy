// -*- C++ -*-

#ifndef INCLUDED_stocknnet_TestPopulateDataRSI_h
#define INCLUDED_stocknnet_TestPopulateDataRSI_h

#include "stocknnet/PopulateDataRSI.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestPopulateDataRSI : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestPopulateDataRSI);

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

