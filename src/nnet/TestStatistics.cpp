#include "TestStatistics.h"

namespace alch
{

void TestStatistics::setUp() 
{
  ;
}

void TestStatistics::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestStatistics::test1()
{
  const double data1[] = { 1.0, 2.0, 3.0, 4.0, 2.5, 2.8 };
  const int n1 = (sizeof(data1) / sizeof(double));
  const double data2[] = { 4.0, 3.0, 6.0, 4.2, 3.8, 4.1 };
  const int n2 = (sizeof(data2) / sizeof(double));
  const double delta = 0.01;

  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, Statistics::min(data1, n1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, Statistics::min(data2, n2), delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, Statistics::max(data1, n1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, Statistics::max(data2, n2), delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.55, Statistics::mean(data1, n1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.18, Statistics::mean(data2, n2), delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0150, Statistics::variance(data1, n1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9777, Statistics::variance(data2, n2), delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0075, Statistics::stddev(data1, n1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9888, Statistics::stddev(data2, n2), delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3564,
                               Statistics::correlation(data1, data2, n1),
                               delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3564,
                               Statistics::correlation(data2, data1, n1),
                               delta);

}

} // namespace alch
