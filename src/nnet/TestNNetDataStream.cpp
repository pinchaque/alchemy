#include "TestNNetDataStream.h"
#include <sstream>
#include <iostream>

namespace alch
{

void TestNNetDataStream::setUp() 
{
  ;
}

void TestNNetDataStream::tearDown()
{
  ;
}

const char* testStr1 =
  "0.01 0.02 0.03 | 0.04 0.05 0.06\n"
  "1.01 1.02 1.03 | 1.04 1.05 1.06\n";

void TestNNetDataStream::test1()
{
  std::string str(testStr1);
  double delta = 0.01;

  Context ctx;
  ctx.setPriorityFilter(Context::PRIORITY_all);
  NNetDatapoint pt;
  std::istringstream iss(str);

  CPPUNIT_ASSERT(NNetDataStream::readDatapoint(iss, pt, ctx));

  CPPUNIT_ASSERT_EQUAL(3, int(pt.input.size()));
  CPPUNIT_ASSERT_EQUAL(3, int(pt.output.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, pt.input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.02, pt.input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.03, pt.input[2], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.04, pt.output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.05, pt.output[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.06, pt.output[2], delta);


  pt.clear();

  CPPUNIT_ASSERT(NNetDataStream::readDatapoint(iss, pt, ctx));
  CPPUNIT_ASSERT_EQUAL(3, int(pt.input.size()));
  CPPUNIT_ASSERT_EQUAL(3, int(pt.output.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.01, pt.input[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.02, pt.input[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.03, pt.input[2], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.04, pt.output[0], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.05, pt.output[1], delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.06, pt.output[2], delta);

  std::stringstream oss;
  CPPUNIT_ASSERT(NNetDataStream::writeDatapoint(oss, pt, ctx));

  CPPUNIT_ASSERT_EQUAL(std::string("1.01 1.02 1.03 | 1.04 1.05 1.06\n"),
                       oss.str());

  CPPUNIT_ASSERT(!NNetDataStream::readDatapoint(iss, pt, ctx));
}



void TestNNetDataStream::test2()
{
  double delta = 0.01;

  Context ctx;
  NNetDataset dataset;
  std::istringstream iss(testStr1);

  CPPUNIT_ASSERT(NNetDataStream::readDataset(iss, dataset, ctx));
  CPPUNIT_ASSERT_EQUAL(2, int(dataset.size()));

  {
    const NNetDatapoint& pt(dataset[0]);
    CPPUNIT_ASSERT_EQUAL(3, int(pt.input.size()));
    CPPUNIT_ASSERT_EQUAL(3, int(pt.output.size()));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.01, pt.input[0], delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.02, pt.input[1], delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.03, pt.input[2], delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.04, pt.output[0], delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.05, pt.output[1], delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.06, pt.output[2], delta);
  }


  {
    const NNetDatapoint& pt(dataset[1]);
    CPPUNIT_ASSERT_EQUAL(3, int(pt.input.size()));
    CPPUNIT_ASSERT_EQUAL(3, int(pt.output.size()));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.01, pt.input[0], delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.02, pt.input[1], delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.03, pt.input[2], delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.04, pt.output[0], delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.05, pt.output[1], delta);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.06, pt.output[2], delta);
  }


  std::ostringstream oss;

  CPPUNIT_ASSERT(NNetDataStream::writeDataset(oss, dataset, ctx));
  CPPUNIT_ASSERT_EQUAL(std::string(testStr1), oss.str());
}


} // namespace alch
