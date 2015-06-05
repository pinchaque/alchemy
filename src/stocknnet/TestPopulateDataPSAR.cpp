#include "TestPopulateDataPSAR.h"

#include <cmath>

namespace alch
{

void TestPopulateDataPSAR::setUp() 
{
  ;
}

void TestPopulateDataPSAR::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestPopulateDataPSAR::test1()
{
  PopulateDataPSAR populate(m_ctx);
  double delta = 0.001;

  CPPUNIT_ASSERT_EQUAL(0, populate.getNumberDays());
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.02, populate.getAccel(), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.20, populate.getMaxAccel(), delta);

  populate.setNumberDays(2);
  populate.setAccel(0.03);
  populate.setMaxAccel(0.30);

  CPPUNIT_ASSERT_EQUAL(2, populate.getNumberDays());
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.03, populate.getAccel(), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.30, populate.getMaxAccel(), delta);

  RangeDataPtr rangeDataPtr(new RangeData);
  
  for (int i = 1; i <= 30; ++i)
  {
    RangeData::Point point;
    point.close = 5.0 * ::sin(3.1416 * 0.07 * i) + 10.0;
    point.open = point.close - 0.5;
    point.min = point.close - 0.6;
    point.max = point.close + 0.1;
    rangeDataPtr->add(point);
  }


  NNetDataset dataset;
  int numModified = 0;

  dataset.resize(35);

  CPPUNIT_ASSERT(populate.populate(rangeDataPtr, dataset, numModified));

  CPPUNIT_ASSERT_EQUAL(28, numModified);

  CPPUNIT_ASSERT_EQUAL(35, int(dataset.size()));

  for (int i = 0; i < 7; ++i)
  {
    CPPUNIT_ASSERT_EQUAL(0, int(dataset[i].input.size()));
    CPPUNIT_ASSERT_EQUAL(0, int(dataset[i].output.size()));
  }

  for (int i = 7; i < 35; ++i)
  {
    CPPUNIT_ASSERT_EQUAL(3, int(dataset[i].input.size()));
    CPPUNIT_ASSERT_EQUAL(0, int(dataset[i].output.size()));

#if 0
    std::cout << "\n";
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

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.193017, dataset[7].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.135064, dataset[7].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0540993, dataset[7].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.227568, dataset[8].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.193017, dataset[8].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.135064, dataset[8].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.239512, dataset[9].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.227568, dataset[9].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.193017, dataset[9].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.230587, dataset[10].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.239512, dataset[10].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.227568, dataset[10].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.203287, dataset[11].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.230587, dataset[11].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.239512, dataset[11].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.160677, dataset[12].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.203287, dataset[12].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.230587, dataset[12].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.111318, dataset[13].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.160677, dataset[13].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.203287, dataset[13].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0548217, dataset[14].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.111318, dataset[14].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.160677, dataset[14].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.134595, dataset[15].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0548217, dataset[15].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.111318, dataset[15].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.210905, dataset[16].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.134595, dataset[16].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0548217, dataset[16].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.31017, dataset[17].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.210905, dataset[17].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.134595, dataset[17].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.423439, dataset[18].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.31017, dataset[18].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.210905, dataset[18].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.544207, dataset[19].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.423439, dataset[19].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.31017, dataset[19].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.661925, dataset[20].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.544207, dataset[20].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.423439, dataset[20].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.7609, dataset[21].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.661925, dataset[21].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.544207, dataset[21].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.820166, dataset[22].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.7609, dataset[22].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.661925, dataset[22].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.81642, dataset[23].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.820166, dataset[23].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.7609, dataset[23].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.732544, dataset[24].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.81642, dataset[24].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.820166, dataset[24].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5705, dataset[25].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.732544, dataset[25].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.81642, dataset[25].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.358842, dataset[26].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5705, dataset[26].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.732544, dataset[26].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.156671, dataset[27].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.358842, dataset[27].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5705, dataset[27].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.234703, dataset[28].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.156671, dataset[28].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.358842, dataset[28].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.309171, dataset[29].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.234703, dataset[29].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.156671, dataset[29].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.381385, dataset[30].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.309171, dataset[30].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.234703, dataset[30].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.433886, dataset[31].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.381385, dataset[31].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.309171, dataset[31].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.462765, dataset[32].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.433886, dataset[32].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.381385, dataset[32].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.468203, dataset[33].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.462765, dataset[33].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.433886, dataset[33].input[2], delta);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.452706, dataset[34].input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.468203, dataset[34].input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.462765, dataset[34].input[2], delta);
}

} // namespace alch
