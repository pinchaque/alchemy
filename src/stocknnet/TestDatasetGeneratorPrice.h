// -*- C++ -*-

#ifndef INCLUDED_stocknnet_TestDatasetGeneratorPrice_h
#define INCLUDED_stocknnet_TestDatasetGeneratorPrice_h

#include "stocknnet/DatasetGeneratorPrice.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestDatasetGeneratorPrice : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestDatasetGeneratorPrice);

  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(test2);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();
  void test2();

private:
  Context m_ctx;

};

} // namespace alch

#endif

