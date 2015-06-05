#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <cassert>

#include "TestFrameworkUtils.h"
#include "TestPathRegistry.h"

int main(int argc, char** argv)
{
  using namespace alch;
    
  CppUnit::TextUi::TestRunner runner;

  runner.addTest(TestFrameworkUtils::suite());
  runner.addTest(TestPathRegistry::suite());

  return !runner.run();
}
