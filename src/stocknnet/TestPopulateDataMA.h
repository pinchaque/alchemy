// -*- C++ -*-

#ifndef INCLUDED_stocknnet_TestPopulateDataMA_h
#define INCLUDED_stocknnet_TestPopulateDataMA_h

#include "stocknnet/PopulateDataMA.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestPopulateDataMA : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestPopulateDataMA);

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

