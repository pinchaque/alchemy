#include "TestPopulateDataRSI.h"

#include <cmath>

namespace alch
{

void TestPopulateDataRSI::setUp() 
{
  ;
}

void TestPopulateDataRSI::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestPopulateDataRSI::test1()
{
  PopulateDataRSI populate(m_ctx);

  CPPUNIT_ASSERT_EQUAL(0, populate.getNumberDays());
  CPPUNIT_ASSERT_EQUAL(14, populate.getSpan());

  populate.setNumberDays(0);
  populate.setSpan(14);

  CPPUNIT_ASSERT_EQUAL(0, populate.getNumberDays());
  CPPUNIT_ASSERT_EQUAL(14, populate.getSpan());

  RangeDataPtr rangeDataPtr(new RangeData);
  
  for (int i = 1; i <= 30; ++i)
  {
    RangeData::Point point;
    point.close = 5.0 * ::sin(3.1416 * 0.07 * i) + 10.0;
    rangeDataPtr->add(point);
  }


  NNetDataset dataset;
  int numModified = 0;

  dataset.resize(25);

  CPPUNIT_ASSERT(populate.populate(rangeDataPtr, dataset, numModified));

  CPPUNIT_ASSERT_EQUAL(16, numModified);

  CPPUNIT_ASSERT_EQUAL(25, int(dataset.size()));

  for (int i = 0; i < 9; ++i)
  {
    CPPUNIT_ASSERT_EQUAL(0, int(dataset[i].input.size()));
    CPPUNIT_ASSERT_EQUAL(0, int(dataset[i].output.size()));
  }

  for (int i = 9; i < 25; ++i)
  {
    CPPUNIT_ASSERT_EQUAL(1, int(dataset[i].input.size()));
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
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.0519307, dataset[9].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.139535, dataset[10].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.235395, dataset[11].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.346107, dataset[12].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.481537, dataset[13].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.658249, dataset[14].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1, dataset[15].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.955657, dataset[16].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.707225, dataset[17].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.529867, dataset[18].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.393479, dataset[19].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.281912, dataset[20].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.185443, dataset[21].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.0975336, dataset[22].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.0131952, dataset[23].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0719891, dataset[24].input[0], delta);
}

} // namespace alch
