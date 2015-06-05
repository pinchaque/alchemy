#include "TestDatasetGeneratorPrice.h"

namespace alch
{

void TestDatasetGeneratorPrice::setUp() 
{
  ;
}

void TestDatasetGeneratorPrice::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestDatasetGeneratorPrice::test1()
{
  double delta = 0.01;
  m_ctx.setPriorityFilter(Context::PRIORITY_all);
  DatasetGeneratorPrice generator(m_ctx);
  
  generator.setNumberDays(1);

  RangeDataPtr rangeDataPtr(new RangeData);
  
  for (int i = 1; i <= 20; ++i)
  {
    RangeData::Point point;
    point.close = i * 5.0;
    point.open = point.close - 0.5;
    point.min = point.close - 0.6;
    point.max = point.close + 0.1;
    rangeDataPtr->add(point);
  }

  // generate the dataset
  NNetDataset dataset;
  CPPUNIT_ASSERT(generator.generateInputs(rangeDataPtr, dataset));
  
  int numDataPoints = int(dataset.size());
  CPPUNIT_ASSERT_EQUAL(20, numDataPoints);
  CPPUNIT_ASSERT_EQUAL(1, int(dataset[0].input.size()));
  CPPUNIT_ASSERT_EQUAL(0, int(dataset[0].output.size()));

  for (int i = 0; i < numDataPoints; ++i)
  {
    CPPUNIT_ASSERT_EQUAL(dataset[0].input.size(), dataset[i].input.size());
    CPPUNIT_ASSERT_EQUAL(dataset[0].output.size(), dataset[i].output.size());

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

    for (int j = 0; j < int(dataset[i].output.size()); ++j)
    {
      std::cout << "  CPPUNIT_ASSERT_DOUBLES_EQUAL("
                << dataset[i].output[j]
                << ", dataset[" 
                << i
                << "].output["
                << j
                << "], delta);\n";
    }
#endif
  }

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.9, dataset[0].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.8, dataset[1].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.7, dataset[2].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.6, dataset[3].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5, dataset[4].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.4, dataset[5].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.3, dataset[6].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.2, dataset[7].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1, dataset[8].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0, dataset[9].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, dataset[10].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, dataset[11].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3, dataset[12].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4, dataset[13].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, dataset[14].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6, dataset[15].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7, dataset[16].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, dataset[17].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9, dataset[18].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1, dataset[19].input[0], delta);
}

void TestDatasetGeneratorPrice::test2()
{
  double delta = 0.01;
  m_ctx.setPriorityFilter(Context::PRIORITY_all);
  DatasetGeneratorPrice generator(m_ctx);
  
  generator.setNumberDays(1);

  RangeDataPtr rangeDataPtr(new RangeData);
  
  for (int i = 1; i <= 20; ++i)
  {
    RangeData::Point point;
    point.close = i * 5.0;
    point.open = point.close - 0.5;
    point.min = point.close - 0.6;
    point.max = point.close + 0.1;
    rangeDataPtr->add(point);
  }

  // generate the dataset
  NNetDataset dataset;
  CPPUNIT_ASSERT(generator.generate(rangeDataPtr, dataset));
  
  int numDataPoints = int(dataset.size());
  CPPUNIT_ASSERT_EQUAL(19, numDataPoints);
  CPPUNIT_ASSERT_EQUAL(1, int(dataset[0].input.size()));
  CPPUNIT_ASSERT_EQUAL(1, int(dataset[0].output.size()));

  for (int i = 0; i < numDataPoints; ++i)
  {
    CPPUNIT_ASSERT_EQUAL(dataset[0].input.size(), dataset[i].input.size());
    CPPUNIT_ASSERT_EQUAL(dataset[0].output.size(), dataset[i].output.size());

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

    for (int j = 0; j < int(dataset[i].output.size()); ++j)
    {
      std::cout << "  CPPUNIT_ASSERT_DOUBLES_EQUAL("
                << dataset[i].output[j]
                << ", dataset[" 
                << i
                << "].output["
                << j
                << "], delta);\n";
    }
#endif
  }

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.9, dataset[0].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.8, dataset[0].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.8, dataset[1].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.7, dataset[1].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.7, dataset[2].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.6, dataset[2].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.6, dataset[3].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5, dataset[3].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5, dataset[4].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.4, dataset[4].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.4, dataset[5].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.3, dataset[5].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.3, dataset[6].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.2, dataset[6].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.2, dataset[7].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1, dataset[7].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1, dataset[8].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0, dataset[8].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0, dataset[9].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, dataset[9].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, dataset[10].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, dataset[10].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, dataset[11].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3, dataset[11].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3, dataset[12].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4, dataset[12].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4, dataset[13].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, dataset[13].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, dataset[14].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6, dataset[14].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6, dataset[15].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7, dataset[15].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7, dataset[16].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, dataset[16].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, dataset[17].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9, dataset[17].output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9, dataset[18].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1, dataset[18].output[0], delta);
}

} // namespace alch
