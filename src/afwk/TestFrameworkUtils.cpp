#include "TestFrameworkUtils.h"

namespace alch
{

void TestFrameworkUtils::setUp() 
{
  ;
}

void TestFrameworkUtils::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestFrameworkUtils::test1()
{

  CPPUNIT_ASSERT_EQUAL(std::string("start"),
                       std::string(FrameworkUtils::getOptionStart()));
  CPPUNIT_ASSERT_EQUAL(std::string("end"),
                       std::string(FrameworkUtils::getOptionEnd()));
  CPPUNIT_ASSERT_EQUAL(std::string("duration"),
                       std::string(FrameworkUtils::getOptionDuration()));

  CPPUNIT_ASSERT_EQUAL(
    std::string("20050101T130000"),
    to_iso_string(FrameworkUtils::stringToTime("2005-01-01 13:00:00")));

  CPPUNIT_ASSERT_EQUAL(
    std::string("20050101T140000"),
    to_iso_string(FrameworkUtils::stringToTime("2005-01-01 14:00:00")));

  CPPUNIT_ASSERT_EQUAL(
    std::string("20050101T130000"),
    to_iso_string(FrameworkUtils::stringToTime("2005-01-01")));

  CPPUNIT_ASSERT_EQUAL(
    std::string("20011111T130001"),
    to_iso_string(FrameworkUtils::stringToTime("2001-11-11 13:00:01")));
}

} // namespace alch
