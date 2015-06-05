#include "TestOStockDataFile.h"

#include "stockdata/IStockDataFile.h"

namespace alch
{

void TestOStockDataFile::setUp() 
{
  m_testFName = "/tmp/teststockdata1.dat";
}

void TestOStockDataFile::tearDown()
{
  unlink(m_testFName.c_str());
}

void TestOStockDataFile::test1()
{
  const char* fname = "testdata/stockdata1.dat";

  RangeData data1;

  {

    IStockDataFile inF(m_ctx);
    CPPUNIT_ASSERT(inF.open(fname));
    CPPUNIT_ASSERT(inF.read(data1));
    CPPUNIT_ASSERT_EQUAL(2, int(data1.size()));    
    inF.close();
  }

  {
    OStockDataFile outF(m_ctx);
    CPPUNIT_ASSERT_EQUAL(std::string(""), outF.getFileName());

    CPPUNIT_ASSERT(outF.open(m_testFName, OStockDataFile::MODE_overwrite));

    CPPUNIT_ASSERT_EQUAL(m_testFName, outF.getFileName());

    CPPUNIT_ASSERT(outF.write(data1));

    outF.close();

    CPPUNIT_ASSERT_EQUAL(std::string(""), outF.getFileName());
  }

  {
    OStockDataFile outF(m_ctx);
    CPPUNIT_ASSERT_EQUAL(std::string(""), outF.getFileName());

    CPPUNIT_ASSERT(outF.open(m_testFName, OStockDataFile::MODE_append));

    CPPUNIT_ASSERT_EQUAL(m_testFName, outF.getFileName());

    CPPUNIT_ASSERT(outF.write(data1.get(0)));

    outF.close();
  }


  {
    RangeData data2;

    IStockDataFile inF(m_ctx);
    CPPUNIT_ASSERT(inF.open(m_testFName));
    CPPUNIT_ASSERT(inF.read(data2));
    CPPUNIT_ASSERT_EQUAL(3, int(data2.size()));

    CPPUNIT_ASSERT(data1.get(0) == data2.get(0));
    CPPUNIT_ASSERT(data1.get(1) == data2.get(1));
    CPPUNIT_ASSERT(data1.get(0) == data2.get(2));

    inF.close();
  }

}

} // namespace alch
