#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <cassert>

#include "TestDatasetGeneratorBasic.h"
#include "TestDatasetGeneratorPrice.h"
#include "TestPopulateDataPrice.h"
#include "TestPopulateDataMA.h"
#include "TestPopulateDataPSAR.h"
#include "TestPopulateDataRSI.h"
#include "TestProfileIO.h"
#include "TestProfileMetaDataStream.h"

int main(int argc, char** argv)
{
  using namespace alch;
    
  CppUnit::TextUi::TestRunner runner;

  runner.addTest(TestDatasetGeneratorBasic::suite());
  runner.addTest(TestDatasetGeneratorPrice::suite());
  runner.addTest(TestPopulateDataPrice::suite());
  runner.addTest(TestPopulateDataMA::suite());
  runner.addTest(TestPopulateDataPSAR::suite());
  runner.addTest(TestPopulateDataRSI::suite());
  runner.addTest(TestProfileIO::suite());
  runner.addTest(TestProfileMetaDataStream::suite());

  return !runner.run();
}
