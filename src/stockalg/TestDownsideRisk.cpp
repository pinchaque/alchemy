#include "TestDownsideRisk.h"
#include "stockalg/LogNormal3.h"
#include "nnet/Statistics.h"

namespace alch
{

void TestDownsideRisk::setUp() 
{
  ;
}

void TestDownsideRisk::tearDown()
{
  m_ctx.dump(std::cerr);
}

const double returns[] = { 0.0, 1.0, 1.0, 1.5, 1.2, 1.3, 0.6, 0.7 };
const int n = 8;
                                                                                  
void TestDownsideRisk::test1()
{
  CPPUNIT_ASSERT_EQUAL(0, 
      DownsideRisk::countPointsInRange(-1000.0, -0.001, returns, n));
  CPPUNIT_ASSERT_EQUAL(8, 
      DownsideRisk::countPointsInRange(-1.0, 1.6, returns, n));
  CPPUNIT_ASSERT_EQUAL(2, 
      DownsideRisk::countPointsInRange(0.01, 0.8, returns, n));
  CPPUNIT_ASSERT_EQUAL(2, 
      DownsideRisk::countPointsInRange(0.9, 1.1, returns, n));
  CPPUNIT_ASSERT_EQUAL(1, 
      DownsideRisk::countPointsInRange(1.19, 1.22, returns, n));
}


void TestDownsideRisk::test2()
{
  double delta = 0.001;
  double min = Statistics::min(returns, n);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, min, delta);
  
  double max = Statistics::max(returns, n);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, max, delta);

  double mean = Statistics::mean(returns, n);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9125, mean, delta);

  double stddev = Statistics::stddev(returns, n);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4734, stddev, delta);

  LogNormal3 ln3(min, max, mean, stddev);
  double negInf = ln3.getNegInfinity();
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.8215, negInf, delta);

  double posInf = ln3.getPosInfinity();
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.3936, posInf, delta);

  double sqErr = DownsideRisk::squaredError(ln3, returns, n, 0.11);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.284676, sqErr, delta);

  {
    double mar = -5.0;
    double downVar = DownsideRisk::downsideVariation(ln3, mar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, downVar, delta);

    double downDev = DownsideRisk::downsideDeviation(ln3, mar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(::sqrt(downVar), downDev, delta);

    double upsidePot = DownsideRisk::upsidePotential(ln3, mar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.9125, upsidePot, delta);
  }

  {
    double mar = -1.0;
    double downVar = DownsideRisk::downsideVariation(ln3, mar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, downVar, delta);

    double downDev = DownsideRisk::downsideDeviation(ln3, mar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(::sqrt(downVar), downDev, delta);

    double upsidePot = DownsideRisk::upsidePotential(ln3, mar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.91272, upsidePot, delta);
  }

  {
    double mar = 2.0;
    double downVar = DownsideRisk::downsideVariation(ln3, mar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.40675, downVar, delta);

    double downDev = DownsideRisk::downsideDeviation(ln3, mar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(::sqrt(downVar), downDev, delta);

    double upsidePot = DownsideRisk::upsidePotential(ln3, mar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, upsidePot, delta);
  }

  {
    double mar = 4.0;
    double downVar = DownsideRisk::downsideVariation(ln3, mar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.75676, downVar, delta);

    double downDev = DownsideRisk::downsideDeviation(ln3, mar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(::sqrt(downVar), downDev, delta);

    double upsidePot = DownsideRisk::upsidePotential(ln3, mar);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, upsidePot, delta);
  }
}


void TestDownsideRisk::test3()
{
  double delta = 0.001;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    1.666667, 
    DownsideRisk::volatilitySkewness(returns, n), 
    delta);


  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    0.0, 
    DownsideRisk::downsideFrequency(-5.0, returns, n), 
    delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    3.0/8.0, 
    DownsideRisk::downsideFrequency(0.8, returns, n), 
    delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    7.0/8.0, 
    DownsideRisk::downsideFrequency(1.45, returns, n), 
    delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    1.0,
    DownsideRisk::downsideFrequency(1.55, returns, n), 
    delta);


  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    0.0, 
    DownsideRisk::avgDownsideDeviation(-5.0, returns, n), 
    delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    0.36667,
    DownsideRisk::avgDownsideDeviation(0.8, returns, n), 
    delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    0.621429,
    DownsideRisk::avgDownsideDeviation(1.45, returns, n), 
    delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(
    0.6375,
    DownsideRisk::avgDownsideDeviation(1.55, returns, n), 
    delta);


}


} // namespace alch
