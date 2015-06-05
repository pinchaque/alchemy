// -*- C++ -*-

#ifndef INCLUDED_stockalg_TestIntegrate_h
#define INCLUDED_stockalg_TestIntegrate_h

#include "stockalg/Integrate.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestIntegrate : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestIntegrate);

  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(test2);
  CPPUNIT_TEST(test3);
  CPPUNIT_TEST(test4);
  CPPUNIT_TEST(test5);
  CPPUNIT_TEST(test6);
  CPPUNIT_TEST(test7);
  CPPUNIT_TEST(test8);
  CPPUNIT_TEST(test9);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();
  void test2();
  void test3();
  void test4();
  void test5();
  void test6();
  void test7();
  void test8();
  void test9();

private:
  Context m_ctx;

};

} // namespace alch

#endif

