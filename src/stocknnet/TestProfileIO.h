// -*- C++ -*-

#ifndef INCLUDED_stocknnet_TestProfileIO_h
#define INCLUDED_stocknnet_TestProfileIO_h

#include "stocknnet/ProfileIO.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestProfileIO : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestProfileIO);

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

