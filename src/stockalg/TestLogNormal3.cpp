#include "TestLogNormal3.h"

#include <iostream>

namespace alch
{

void TestLogNormal3::setUp() 
{
  ;
}

void TestLogNormal3::tearDown()
{
  ;
}

void TestLogNormal3::test1()
{
  double delta = 0.000001;
  double mean = 0.0;
  for (double stddev = 0.05; stddev <= 3.0; stddev += 0.1)
  {
    for (double min = mean - 10.0; min <= mean - 1.0; min += 1)
    {
      for (double max = mean + 1.0; max <= mean + 10.0; max += 1)
      {
        LogNormal3 ln3(min, max, mean, stddev);

        /*
        std::cerr << "\n\n=================================================\n";
        ln3.dump(std::cerr);
        */

        double negInf = ln3.getNegInfinity();
        double posInf = ln3.getPosInfinity();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ln3.eval(negInf), delta);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ln3.eval(posInf), delta);
      }
    }
  }
}

} // namespace alch
