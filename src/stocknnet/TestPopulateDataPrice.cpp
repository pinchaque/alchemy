#include "TestPopulateDataPrice.h"

namespace alch
{

void TestPopulateDataPrice::setUp() 
{
  ;
}

void TestPopulateDataPrice::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestPopulateDataPrice::test1()
{
  PopulateDataPrice populate(m_ctx);

  CPPUNIT_ASSERT_EQUAL(1, populate.getNumberDays());

  populate.setNumberDays(2);

  CPPUNIT_ASSERT_EQUAL(2, populate.getNumberDays());

  RangeDataPtr rangeDataPtr(new RangeData);
  
  for (int i = 1; i <= 10; ++i)
  {
    RangeData::Point point;
    point.close = 0.1 * i;
    rangeDataPtr->add(point);
  }


  NNetDataset dataset;
  int numModified = 0;

  dataset.resize(15);

  CPPUNIT_ASSERT(populate.populate(rangeDataPtr, dataset, numModified));

  CPPUNIT_ASSERT_EQUAL(8, numModified);

  CPPUNIT_ASSERT_EQUAL(15, int(dataset.size()));

  for (int i = 0; i < 7; ++i)
  {
    CPPUNIT_ASSERT_EQUAL(0, int(dataset[i].input.size()));
    CPPUNIT_ASSERT_EQUAL(0, int(dataset[i].output.size()));
  }

  for (int i = 7; i < 15; ++i)
  {
    CPPUNIT_ASSERT_EQUAL(2, int(dataset[i].input.size()));
    CPPUNIT_ASSERT_EQUAL(0, int(dataset[i].output.size()));
  }

  double delta = 0.01;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.3333, dataset[7].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.6667, dataset[7].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.25, dataset[8].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5, dataset[8].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.2, dataset[9].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.4, dataset[9].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1667, dataset[10].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.3333, dataset[10].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.142857, dataset[11].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.285714, dataset[11].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.125, dataset[12].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.25, dataset[12].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1111, dataset[13].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.2222, dataset[13].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1, dataset[14].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.2, dataset[14].input[1], delta);
}

} // namespace alch
