// -*- C++ -*-

#ifndef INCLUDED_autil_TestCSVDataStream_h
#define INCLUDED_autil_TestCSVDataStream_h

#include "autil/CSVDataStream.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestCSVDataStream : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestCSVDataStream);

  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(test2);
  CPPUNIT_TEST(test3);
  CPPUNIT_TEST(test4);
  CPPUNIT_TEST(test5);

  CPPUNIT_TEST_SUITE_END();

  public:

  void setUp();

  void tearDown();

  void test1();
  void test2();
  void test3();
  void test4();
  void test5();


private:
  Context m_ctx;

};

} // namespace alch

#endif

