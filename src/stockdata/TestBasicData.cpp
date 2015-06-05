#include "TestBasicData.h"

namespace alch
{

void TestBasicData::setUp() 
{
  ;
}

void TestBasicData::tearDown()
{
  ;
}

void TestBasicData::test1()
{
  BasicData data1;

  data1.push_back(1.23);
  data1.push_back(2.45);
  data1.push_back(3.56);

  CPPUNIT_ASSERT_EQUAL(3, int(data1.size()));
}

} // namespace alch
