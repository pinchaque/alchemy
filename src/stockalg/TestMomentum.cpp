#include "TestMomentum.h"

namespace alch
{

void TestMomentum::setUp() 
{
  ;
}

void TestMomentum::tearDown()
{
  ;
}

void TestMomentum::test1()
{
  Momentum::DoubleVec data;
  data.push_back(985);
  data.push_back(980);
  data.push_back(972);
  data.push_back(975);
  data.push_back(965);
  data.push_back(967);
  data.push_back(972);
  data.push_back(965);
  data.push_back(974);
  data.push_back(980);
  data.push_back(964);
  data.push_back(960);
  data.push_back(950);
  data.push_back(960);
  data.push_back(965);
  data.push_back(970);
  data.push_back(974);
  data.push_back(980);
  data.push_back(985);

  Momentum::DoubleVec output;
  output.push_back(0.00);

  Momentum::rateOfChange(data, 100, output);

  CPPUNIT_ASSERT_EQUAL(0, int(output.size()));


  Momentum::rateOfChange(data, 10, output);

  CPPUNIT_ASSERT_EQUAL(9, int(output.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.980, output[0], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.980, output[1], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.977, output[2], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.985, output[3], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.000, output[4], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.003, output[5], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.002, output[6], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.016, output[7], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.011, output[8], 0.01);
}

void TestMomentum::test2()
{
  Momentum::DoubleVec data;
  data.push_back(981);
  data.push_back(985);
  data.push_back(980);
  data.push_back(972);
  data.push_back(975);
  data.push_back(965);
  data.push_back(985);
  data.push_back(990);
  data.push_back(980);
  data.push_back(995);

  Momentum::DoubleVec output;
  output.push_back(0.00);

  Momentum::relativeStrength(data, 100, output);

  CPPUNIT_ASSERT_EQUAL(0, int(output.size()));


  Momentum::relativeStrength(data, 3, output);

  CPPUNIT_ASSERT_EQUAL(7, int(output.size()));

  /*
    for (int i = 0; i < 7; ++i)
    {
    std::cout << "  CPPUNIT_ASSERT_DOUBLES_EQUAL(" << output[i]
    << ", output[" << i << "], 0.01);\n";
    }
  */

  CPPUNIT_ASSERT_DOUBLES_EQUAL(38.0952, output[0], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(31.5789, output[1], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(25, output[2], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(53.4884, output[3], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(55.5556, output[4], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(55.5556, output[5], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(50, output[6], 0.01);
}

} // namespace alch
