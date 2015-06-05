// -*- C++ -*-

#ifndef INCLUDED_afwk_TestPathRegistry_h
#define INCLUDED_afwk_TestPathRegistry_h

#include "afwk/PathRegistry.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestPathRegistry : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestPathRegistry);

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

