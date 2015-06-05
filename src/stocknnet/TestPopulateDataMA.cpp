#include "TestPopulateDataMA.h"

namespace alch
{

void TestPopulateDataMA::setUp() 
{
  ;
}

void TestPopulateDataMA::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestPopulateDataMA::test1()
{
  PopulateDataMA populate(m_ctx);

  CPPUNIT_ASSERT_EQUAL(0, populate.getNumberDays());
  CPPUNIT_ASSERT_EQUAL(20, populate.getSpan());

  populate.setNumberDays(2);
  populate.setSpan(3);

  CPPUNIT_ASSERT_EQUAL(2, populate.getNumberDays());
  CPPUNIT_ASSERT_EQUAL(3, populate.getSpan());

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

  CPPUNIT_ASSERT_EQUAL(6, numModified);

  CPPUNIT_ASSERT_EQUAL(15, int(dataset.size()));

  for (int i = 0; i < 9; ++i)
  {
    CPPUNIT_ASSERT_EQUAL(0, int(dataset[i].input.size()));
    CPPUNIT_ASSERT_EQUAL(0, int(dataset[i].output.size()));
  }

  for (int i = 9; i < 15; ++i)
  {
    CPPUNIT_ASSERT_EQUAL(3, int(dataset[i].input.size()));
    CPPUNIT_ASSERT_EQUAL(0, int(dataset[i].output.size()));

#if 0
    for (int j = 0; j < int(dataset[i].input.size()); ++j)
    {
      std::cout << "  CPPUNIT_ASSERT_DOUBLES_EQUAL("
                << dataset[i].input[j]
                << ", dataset[" 
                << i
                << "].input["
                << j
                << "], delta);\n";
    }
#endif
  }

  double delta = 0.01;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, dataset[9].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25, dataset[9].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.333333, dataset[9].input[2], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.166667, dataset[10].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, dataset[10].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25, dataset[10].input[2], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.142857, dataset[11].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.166667, dataset[11].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, dataset[11].input[2], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.125, dataset[12].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.142857, dataset[12].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.166667, dataset[12].input[2], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.111111, dataset[13].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.125, dataset[13].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.142857, dataset[13].input[2], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, dataset[14].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.111111, dataset[14].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.125, dataset[14].input[2], delta);
}

} // namespace alch
