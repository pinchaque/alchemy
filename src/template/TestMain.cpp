#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <cassert>

#include "TestCLASSNAME.h"

int main(int argc, char** argv)
{
  using namespace alch;
    
  CppUnit::TextUi::TestRunner runner;

  runner.addTest(TestCLASSNAME::suite());

  return !runner.run();
}
