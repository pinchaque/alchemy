#include "TestCSVDataStream.h"
#include <sstream>
#include <iostream>

namespace alch
{

void TestCSVDataStream::setUp() 
{
  ;
}

void TestCSVDataStream::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestCSVDataStream::test1()
{

  const char* result = 
    "h1,h2,h3,h4\n"
    "1,2,3,4\n"
    "5,6,7,8\n"
    ;

  std::ostringstream os;

  CSVData data;

  {
    CSVData::Row row;
    row.add("h1");
    row.add("h2");
    row.add("h3");
    row.add("h4");
    data.setHeadings(row);
  }

  {
    CSVData::Row row;
    row.add(1);
    row.add(2);
    row.add(3);
    row.add(4);
    data.addRow(row);
  }

  {
    CSVData::Row row;
    row.add(5);
    row.add(6);
    row.add(7);
    row.add(8);
    data.addRow(row);
  }

  CPPUNIT_ASSERT(CSVDataStream::write(os, data, m_ctx));

  CPPUNIT_ASSERT_EQUAL(std::string(result), os.str());

  std::istringstream is(result);
  CSVData newData;
  CPPUNIT_ASSERT(CSVDataStream::read(is, newData, m_ctx));
  CPPUNIT_ASSERT_EQUAL(data.getHeadings().size(), newData.getHeadings().size());
  CPPUNIT_ASSERT_EQUAL(data.numColumns(), newData.numColumns());
  CPPUNIT_ASSERT(data == newData);
}

// various tests that don't parse
void TestCSVDataStream::test2()
{
  {
    const char* origData = 
      "h1,h2,h3\n"
      "End\n"
      ;

    std::istringstream is(origData);
    CSVData newData;
    CPPUNIT_ASSERT(!CSVDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "h1,h2,h3\n"
      "1,2,3,4\n"
      ;

    std::istringstream is(origData);
    CSVData newData;
    CPPUNIT_ASSERT(!CSVDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "h1,h2,h3\n"
      "1,2,3\n"
      "1,2,3\n"
      "1,2,3,4\n"
      "1,2,3\n"
      ;

    std::istringstream is(origData);
    CSVData newData;
    CPPUNIT_ASSERT(!CSVDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "h1,h2,h3\n"
      "1,2,3\n"
      "1,2,\"3\n"
      ;

    std::istringstream is(origData);
    CSVData newData;
    CPPUNIT_ASSERT(!CSVDataStream::read(is, newData, m_ctx));
  }
}

// more advanced writing and parsing
void TestCSVDataStream::test3()
{
  const char* result = 
    "h1,h2\n"
    "quoted value 1,2\n"
    "8,\"quoted,value\"\n"
    "9,\"quoted\"\"value\"\n"
    "10,\"a\"\"\"\n"
    "11,\"\"\"b\"\n"
    "12,\"a\"\"\"\"b\"\n"
    ;

  std::ostringstream os;

  CSVData data;

  {
    CSVData::Row row;
    row.add("h1");
    row.add("h2");
    data.setHeadings(row);
  }

  {
    CSVData::Row row;
    row.add("quoted value 1");
    row.add(2);
    data.addRow(row);
  }

  {
    CSVData::Row row;
    row.add(8);
    row.add("quoted,value");
    data.addRow(row);
  }

  {
    CSVData::Row row;
    row.add(9);
    row.add("quoted\"value");
    data.addRow(row);
  }

  {
    CSVData::Row row;
    row.add(10);
    row.add("a\"");
    data.addRow(row);
  }

  {
    CSVData::Row row;
    row.add(11);
    row.add("\"b");
    data.addRow(row);
  }

  {
    CSVData::Row row;
    row.add(12);
    row.add("a\"\"b");
    data.addRow(row);
  }

  CPPUNIT_ASSERT(CSVDataStream::write(os, data, m_ctx));

  CPPUNIT_ASSERT_EQUAL(std::string(result), os.str());

  std::istringstream is(result);
  CSVData newData;
  CPPUNIT_ASSERT(CSVDataStream::read(is, newData, m_ctx));
  CPPUNIT_ASSERT_EQUAL(data.getHeadings().size(), newData.getHeadings().size());
  CPPUNIT_ASSERT_EQUAL(data.numColumns(), newData.numColumns());
  
  data.dump(std::cout);
  newData.dump(std::cout);

  CPPUNIT_ASSERT(data == newData);

}

void TestCSVDataStream::test4()
{
  CPPUNIT_ASSERT_EQUAL(std::string("1"), CSVDataStream::quote("1"));
  CPPUNIT_ASSERT_EQUAL(std::string("a"), CSVDataStream::quote("a"));
  CPPUNIT_ASSERT_EQUAL(std::string("\"a b\""), CSVDataStream::quote("a\nb"));
  CPPUNIT_ASSERT_EQUAL(std::string("\"a,b\""), CSVDataStream::quote("a,b"));
  CPPUNIT_ASSERT_EQUAL(std::string("\"a\"\"b\""), CSVDataStream::quote("a\"b"));
  CPPUNIT_ASSERT_EQUAL(std::string("\"a\"\"\""), CSVDataStream::quote("a\""));
  CPPUNIT_ASSERT_EQUAL(std::string("\"\"\"a\""), CSVDataStream::quote("\"a"));
  CPPUNIT_ASSERT_EQUAL(
      std::string("\"a\"\"\"\"b\""), 
      CSVDataStream::quote("a\"\"b"));
}

void TestCSVDataStream::test5()
{
  CPPUNIT_ASSERT_EQUAL(std::string("1"), CSVDataStream::unquote("1"));
  CPPUNIT_ASSERT_EQUAL(std::string("a"), CSVDataStream::unquote("a"));
  CPPUNIT_ASSERT_EQUAL(
      std::string("a b"), 
      CSVDataStream::unquote("\"a b\""));
  CPPUNIT_ASSERT_EQUAL(
      std::string("a,b"), 
      CSVDataStream::unquote("\"a,b\""));
  CPPUNIT_ASSERT_EQUAL(
      std::string("a\"b"), 
      CSVDataStream::unquote("\"a\"\"b\""));
  CPPUNIT_ASSERT_EQUAL(
      std::string("a\""), 
      CSVDataStream::unquote("\"a\"\"\""));
  CPPUNIT_ASSERT_EQUAL(
      std::string("\"a"), 
      CSVDataStream::unquote("\"\"\"a\""));
  CPPUNIT_ASSERT_EQUAL(
      CSVDataStream::quote("a\"\"b"),
      std::string("\"a\"\"\"\"b\"")); 
}

} // namespace alch
