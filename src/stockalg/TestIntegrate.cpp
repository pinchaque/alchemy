#include "TestIntegrate.h"

#include "stockalg/Normal.h"
#include "stockalg/LogNormal3.h"

namespace alch
{

void TestIntegrate::setUp() 
{
  ;
}

void TestIntegrate::tearDown()
{
  m_ctx.dump(std::cerr);
}

double constant(double x)
{
  return 10.0; 
}

double line(double x)
{
  return x * 2.0;
}

const double delta = 0.01;

void TestIntegrate::test1()
{
  CPPUNIT_ASSERT_DOUBLES_EQUAL(150.0, integrate(constant, -2.5, 12.5), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, integrate(constant, 12.5, 12.5), delta);
}

void TestIntegrate::test2()
{
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, integrate(line, 1.0, 3.0), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, integrate(line, -11.0, 11.0), delta);
}

void TestIntegrate::test3()
{
  const double PI = 3.1415926535;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, integrate(::sin, -1.0 * PI, PI), delta);
}

void TestIntegrate::test4()
{
  Normal fn(1.5, 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, integrate(fn, -8.5, 11.5), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, integrate(fn, 1.5, 11.5), delta);
}

void TestIntegrate::test5()
{
  LogNormal3 fn(-1.0, 1.5, 0.02);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, integrate(fn, 1.0, 11.5), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, integrate(fn, 11.0, 11.5), delta);
}


void TestIntegrate::test6()
{
  LogNormal3 fn(0.2, 1.5, 0.05);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, integrate(fn, -4.0, 8.5), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, integrate(fn, 11.0, 11.5), delta);
}

void TestIntegrate::test7()
{
  LogNormal3 fn(2.8, 1.5, 0.05);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, integrate(fn, -4.0, 9.5), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, integrate(fn, 11.0, 11.5), delta);
}

void TestIntegrate::test8()
{
  LogNormal3 fn(-5, 2.8, 1.5, 0.05);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, integrate(fn, -8.0, 9.5), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, integrate(fn, 11.0, 11.5), delta);
}

void TestIntegrate::test9()
{
  LogNormal3 fn(1.0, 3.8, 1.5, 0.05);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, integrate(fn, -2.0, 9.5), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, integrate(fn, 11.0, 11.5), delta);
}


} // namespace alch
