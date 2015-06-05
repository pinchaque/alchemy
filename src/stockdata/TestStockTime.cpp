#include "TestStockTime.h"
#include <iostream>

using namespace boost::posix_time;
using namespace boost::gregorian;

namespace alch
{

void TestStockTime::setUp() 
{
  ;
}

void TestStockTime::tearDown()
{
  ;
}

void TestStockTime::test1()
{

  date d(2002,Feb,1); //an arbitrary date

  //construct a time by adding up some durations durations
  StockTime t1(d, hours(5)+minutes(4)+seconds(2)+millisec(1));

  //construct a new time by subtracting some times
  StockTime t2 = t1 - hours(5)- minutes(4)- seconds(2)- millisec(1);

  //construct a duration by taking the difference between times
  time_duration td = t2 - t1;
    
  std::cout << to_simple_string(t2) << " - " 
         << to_simple_string(t1) << " = "
         << to_simple_string(td) << std::endl;

}

} // namespace alch
