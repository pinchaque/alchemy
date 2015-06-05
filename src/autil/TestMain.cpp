#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <cassert>

#include "TestCSVDataStream.h"

int main(int argc, char** argv)
{
  using namespace alch;
    
  CppUnit::TextUi::TestRunner runner;

  runner.addTest(TestCSVDataStream::suite());

  return !runner.run();
}
