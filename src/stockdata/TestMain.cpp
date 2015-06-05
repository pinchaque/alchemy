#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <cassert>

#include "TestBasicData.h"
#include "TestIStockDataFile.h"
#include "TestOStockDataFile.h"
#include "TestRangeData.h"
#include "TestStockID.h"
#include "TestStockTime.h"
#include "TestStockDataRetriever.h"
#include "TestStockDataStream.h"
#include "TestStockMetaDataStream.h"
#include "TestStockListStream.h"
#include "TestFileStockDataSource.h"
#include "TestYahooStockDataSource.h"
#include "TestStockTimeUtil.h"
#include "TestRangeDataAlg.h"
#include "TestPortfolio.h"

int main(int argc, char** argv)
{
  using namespace alch;
    
  CppUnit::TextUi::TestRunner runner;

//#define RUN_YAHOO_TESTS
#ifdef RUN_YAHOO_TESTS
  runner.addTest(TestYahooStockDataSource::suite());
#endif

  runner.addTest(TestBasicData::suite());
  runner.addTest(TestRangeData::suite());
  runner.addTest(TestRangeDataAlg::suite());
  runner.addTest(TestStockID::suite());
  runner.addTest(TestStockTime::suite());
  runner.addTest(TestIStockDataFile::suite());
  runner.addTest(TestOStockDataFile::suite());
  runner.addTest(TestStockDataRetriever::suite());
  runner.addTest(TestStockDataStream::suite());
  runner.addTest(TestStockMetaDataStream::suite());
  runner.addTest(TestStockListStream::suite());
  runner.addTest(TestFileStockDataSource::suite());
  runner.addTest(TestStockTimeUtil::suite());
  runner.addTest(TestPortfolio::suite());

  return !runner.run();
}
