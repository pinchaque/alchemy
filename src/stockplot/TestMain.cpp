#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <cassert>

#include "TestCSVPlotDisplay.h"
#include "TestGnuPlotDisplay.h"
#include "TestPricePlotDataCreator.h"
#include "TestMAPlotDataCreator.h"
#include "TestPSARPlotDataCreator.h"
#include "TestPlotListRangeAdjuster.h"

int main(int argc, char** argv)
{
  using namespace alch;
    
  CppUnit::TextUi::TestRunner runner;

  runner.addTest(TestCSVPlotDisplay::suite());
  runner.addTest(TestGnuPlotDisplay::suite());
  runner.addTest(TestPricePlotDataCreator::suite());
  runner.addTest(TestMAPlotDataCreator::suite());
  runner.addTest(TestPSARPlotDataCreator::suite());
  runner.addTest(TestPlotListRangeAdjuster::suite());

  return !runner.run();
}
