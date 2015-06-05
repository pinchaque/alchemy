#include "TestNeuralNetAlg.h"

namespace alch
{

void TestNeuralNetAlg::setUp() 
{
  ;
}

void TestNeuralNetAlg::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestNeuralNetAlg::test1()
{
  const double value[] = { 1.5, 2.0, 3.0, 4.0, 2.5, 2.8 };
  const int valueN = (sizeof(value) / sizeof(double));
  const double target[] = { 1.6, 1.8, 8.0, 4.2, 3.0, 3.1 };
  const int targetN = (sizeof(target) / sizeof(double));
  const double delta = 0.01;

  CPPUNIT_ASSERT_EQUAL(valueN, targetN);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    1.060,
    NeuralNetAlg::squaredError(value, target, valueN),
    delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    12.715,
    NeuralNetAlg::sumOfSquares(value, target, valueN),
    delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    0.922,
    NeuralNetAlg::rmsError(value, target, valueN),
    delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    6.300,
    NeuralNetAlg::minkowskiR1(value, target, valueN),
    delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    1.050,
    NeuralNetAlg::absError(value, target, valueN),
    delta);

}

} // namespace alch
