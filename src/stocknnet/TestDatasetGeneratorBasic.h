// -*- C++ -*-

#ifndef INCLUDED_stocknnet_TestDatasetGeneratorBasic_h
#define INCLUDED_stocknnet_TestDatasetGeneratorBasic_h

#include "stocknnet/DatasetGeneratorBasic.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestDatasetGeneratorBasic : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestDatasetGeneratorBasic);

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

