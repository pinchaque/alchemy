#include "TestProfileMetaDataStream.h"
#include <sstream>
#include <iostream>

namespace alch
{

void TestProfileMetaDataStream::setUp() 
{
  ;
}

void TestProfileMetaDataStream::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestProfileMetaDataStream::test1()
{

  const char* result = 
    "Name this is a name\n"
    "Days 4\n"
    ;

  std::ostringstream os;

  PredictionProfile data;
  data.setName("this is a name");
  data.setNumberDays(4);

  CPPUNIT_ASSERT(ProfileMetaDataStream::write(os, data, m_ctx));

  CPPUNIT_ASSERT(os.str() == result);

  std::istringstream is(result);
  PredictionProfile newData;
  CPPUNIT_ASSERT(ProfileMetaDataStream::read(is, newData, m_ctx));
  CPPUNIT_ASSERT(data.getName() == newData.getName());
  CPPUNIT_ASSERT(data.getNumberDays() == newData.getNumberDays());
}

void TestProfileMetaDataStream::test2()
{
  // this one parses OK
  {
    const char* origData = 
      "Name this is a name\n"
      "Days 4\n"
      ;

    std::istringstream is(origData);
    PredictionProfile newData;
    CPPUNIT_ASSERT(ProfileMetaDataStream::read(is, newData, m_ctx));
  }


  // various things that should fail to parse

  {
    const char* origData = 
      "Name this is a name\n"
      "Days -4\n"
      ;

    std::istringstream is(origData);
    PredictionProfile newData;
    CPPUNIT_ASSERT(!ProfileMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "Name this is a name\n"
      "Days xyz\n"
      ;

    std::istringstream is(origData);
    PredictionProfile newData;
    CPPUNIT_ASSERT(!ProfileMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "Name\n"
      "Days 4\n"
      ;

    std::istringstream is(origData);
    PredictionProfile newData;
    CPPUNIT_ASSERT(!ProfileMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = "";
    std::istringstream is(origData);
    PredictionProfile newData;
    CPPUNIT_ASSERT(!ProfileMetaDataStream::read(is, newData, m_ctx));
  }


  {
    const char* origData = 
      "Name1 this is a name\n"
      "Days 4\n"
      ;

    std::istringstream is(origData);
    PredictionProfile newData;
    CPPUNIT_ASSERT(!ProfileMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "Name this is a name\n"
      "Days asdf4\n"
      ;

    std::istringstream is(origData);
    PredictionProfile newData;
    CPPUNIT_ASSERT(!ProfileMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "Name this is a name\n"
      ;

    std::istringstream is(origData);
    PredictionProfile newData;
    CPPUNIT_ASSERT(!ProfileMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "Name this is a name\n"
      "Days3 4\n"
      ;

    std::istringstream is(origData);
    PredictionProfile newData;
    CPPUNIT_ASSERT(!ProfileMetaDataStream::read(is, newData, m_ctx));
  }

}

} // namespace alch
