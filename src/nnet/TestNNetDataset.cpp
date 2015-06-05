#include "TestNNetDataset.h"

namespace alch
{


void TestNNetDataset::setUp() 
{
  ;
}

void TestNNetDataset::tearDown()
{
  ;
}

void TestNNetDataset::test1()
{
  NNetDatapoint dp;
  CPPUNIT_ASSERT_EQUAL(0, int(dp.input.size()));
  CPPUNIT_ASSERT_EQUAL(0, int(dp.output.size()));

  dp.input.push_back(1.23);
  dp.input.push_back(2.34);
  dp.output.push_back(3.45);

  CPPUNIT_ASSERT_EQUAL(2, int(dp.input.size()));
  CPPUNIT_ASSERT_EQUAL(1.23, dp.input[0]);
  CPPUNIT_ASSERT_EQUAL(2.34, dp.input[1]);
  CPPUNIT_ASSERT_EQUAL(1, int(dp.output.size()));
  CPPUNIT_ASSERT_EQUAL(3.45, dp.output[0]);

  NNetDataset ds;
  CPPUNIT_ASSERT_EQUAL(0, int(ds.size()));
  ds.push_back(dp);
  ds.push_back(dp);
  CPPUNIT_ASSERT_EQUAL(2, int(ds.size()));
  CPPUNIT_ASSERT_EQUAL(dp.input.size(), ds[0].input.size());
  CPPUNIT_ASSERT_EQUAL(dp.input.size(), ds[1].input.size());
}

} // namespace alch

