// -*- C++ -*-

#ifndef INCLUDED_stocknnet_TestProfileMetaDataStream_h
#define INCLUDED_stocknnet_TestProfileMetaDataStream_h

#include "stocknnet/ProfileMetaDataStream.h"
#include "autil/Context.h"

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>

namespace alch
{


class TestProfileMetaDataStream : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE(TestProfileMetaDataStream);

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

