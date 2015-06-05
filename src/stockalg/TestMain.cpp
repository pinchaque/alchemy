#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <cassert>

#include "TestMovingAverage.h"
#include "TestMomentum.h"
#include "TestIntegrate.h"
#include "TestDownsideRisk.h"
#include "TestLogNormal3.h"
#include "TestReturns.h"
#include "TestPortfolioAlg.h"
#include "TestPortfolioPermute.h"

int main(int argc, char** argv)
{
  using namespace alch;
    
  CppUnit::TextUi::TestRunner runner;

  runner.addTest(TestMovingAverage::suite());
  runner.addTest(TestMomentum::suite());
  runner.addTest(TestIntegrate::suite());
  runner.addTest(TestDownsideRisk::suite());
  runner.addTest(TestLogNormal3::suite());
  runner.addTest(TestReturns::suite());
  runner.addTest(TestPortfolioAlg::suite());
  runner.addTest(TestPortfolioPermute::suite());

  return !runner.run();
}
