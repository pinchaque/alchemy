#include "TestMovingAverage.h"

namespace alch
{

void TestMovingAverage::setUp() 
{
  ;
}

void TestMovingAverage::tearDown()
{
  ;
}

void TestMovingAverage::test1()
{
  MovingAverage::DoubleVec data;
  data.push_back(1.00);
  data.push_back(10.00);
  data.push_back(4.00);
  data.push_back(8.00);
  data.push_back(12.00);
  data.push_back(4.00);

  MovingAverage::DoubleVec output;
  output.push_back(0.00);

  MovingAverage::simpleMA(data, 100, output);

  CPPUNIT_ASSERT_EQUAL(0, int(output.size()));


  MovingAverage::simpleMA(data, 2, output);

  CPPUNIT_ASSERT_EQUAL(5, int(output.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.50, output[0], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.00, output[1], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.00, output[2], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10.00, output[3], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.00, output[4], 0.01);


  MovingAverage::simpleMA(data, 3, output);

  CPPUNIT_ASSERT_EQUAL(4, int(output.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.00, output[0], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.33, output[1], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.00, output[2], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.00, output[3], 0.01);
}

void TestMovingAverage::test2()
{
  MovingAverage::DoubleVec data;
  data.push_back(1.00);
  data.push_back(10.00);
  data.push_back(4.00);
  data.push_back(8.00);
  data.push_back(12.00);
  data.push_back(4.00);

  MovingAverage::DoubleVec output;
  output.push_back(0.00);

  MovingAverage::weightedMA(data, 100, output);

  CPPUNIT_ASSERT_EQUAL(0, int(output.size()));


  MovingAverage::weightedMA(data, 2, output);

  CPPUNIT_ASSERT_EQUAL(5, int(output.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.00, output[0], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.00, output[1], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.66, output[2], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10.66, output[3], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.66, output[4], 0.01);


  MovingAverage::weightedMA(data, 3, output);

  CPPUNIT_ASSERT_EQUAL(4, int(output.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.5, output[0], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.00, output[1], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(9.33, output[2], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.33, output[3], 0.01);
}

void TestMovingAverage::test3()
{
  MovingAverage::DoubleVec data;
  data.push_back(1.00);
  data.push_back(10.00);
  data.push_back(4.00);
  data.push_back(8.00);
  data.push_back(12.00);
  data.push_back(4.00);

  MovingAverage::DoubleVec output;
  output.push_back(0.00);

  const double exponent = 0.1;
  MovingAverage::exponentialMA(data, 100, exponent, output);

  CPPUNIT_ASSERT_EQUAL(0, int(output.size()));


  MovingAverage::exponentialMA(data, 2, exponent, output);

  CPPUNIT_ASSERT_EQUAL(5, int(output.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.50, output[0], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.35, output[1], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.62, output[2], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.25, output[3], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.03, output[4], 0.01);

  MovingAverage::exponentialMA(data, 3, exponent, output);

  CPPUNIT_ASSERT_EQUAL(4, int(output.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, output[0], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.3, output[1], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.97, output[2], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.77, output[3], 0.01);
}

} // namespace alch
