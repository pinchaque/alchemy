#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <cassert>

#include "TestNNetDataset.h"
#include "TestNNetDataStream.h"
#include "TestStatistics.h"
#include "TestNeuralNetAlg.h"

int main(int argc, char** argv)
{
  using namespace alch;
    
  CppUnit::TextUi::TestRunner runner;

  runner.addTest(TestNNetDataset::suite());
  runner.addTest(TestNNetDataStream::suite());
  runner.addTest(TestStatistics::suite());
  runner.addTest(TestNeuralNetAlg::suite());

  return !runner.run();
}
