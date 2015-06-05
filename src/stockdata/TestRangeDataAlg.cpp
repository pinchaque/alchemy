#include "TestRangeDataAlg.h"

namespace alch
{

void TestRangeDataAlg::setUp() 
{
  ;
}

void TestRangeDataAlg::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestRangeDataAlg::test1()
{
  RangeData d1;
  RangeData d2;
  RangeData f;

  {
    RangeData::Point p;
    p.tradeTime = boost::posix_time::from_iso_string("19990131T000000");
    d1.add(p);
  }

  {
    RangeData::Point p;
    p.tradeTime = boost::posix_time::from_iso_string("19990228T000000");
    d1.add(p);
    d2.add(p);
    f.add(p);
  }

  {
    RangeData::Point p;
    p.tradeTime = boost::posix_time::from_iso_string("19980131T000000");
    d2.add(p);
  }

  {
    RangeData::Point p;
    p.tradeTime = boost::posix_time::from_iso_string("19990331T000000");
    d1.add(p);
    d2.add(p);
    f.add(p);
  }

  {
    RangeData::Point p;
    p.tradeTime = boost::posix_time::from_iso_string("19990430T000000");
    d1.add(p);
    d2.add(p);
    f.add(p);
  }

  {
    RangeData::Point p;
    p.tradeTime = boost::posix_time::from_iso_string("19990531T000000");
    d2.add(p);
  }

  {
    RangeData::Point p;
    p.tradeTime = boost::posix_time::from_iso_string("19990630T000000");
    d1.add(p);
    d2.add(p);
    f.add(p);
  }

  f.sortDate();
  RangeData f1;
  RangeData f2;
  CPPUNIT_ASSERT(RangeDataAlg::findCommon(d1, d2, f1, f2));

  CPPUNIT_ASSERT_EQUAL((int)f.size(), (int)f1.size());
  CPPUNIT_ASSERT_EQUAL((int)f.size(), (int)f2.size());

  for (int i = 0; i < (int)f1.size(); ++i)
  {
    CPPUNIT_ASSERT(f.get(i).tradeTime == f1.get(i).tradeTime);
    CPPUNIT_ASSERT(f1.get(i).tradeTime == f2.get(i).tradeTime);
  }
}

} // namespace alch
