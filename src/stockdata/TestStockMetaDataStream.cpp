#include "TestStockMetaDataStream.h"
#include <sstream>
#include <iostream>

namespace alch
{

void TestStockMetaDataStream::setUp() 
{
  ;
}

void TestStockMetaDataStream::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestStockMetaDataStream::test1()
{

  const char* result = 
    "Symbol ibm\n"
    "Start 20040105T010203\n"
    "End 20040305T040506\n"
    "Description \n"
    "ExpenseRatio 0\n"
    "12b1Fee 0\n"
    "Family \n"
    "Rating 0\n"
    "Inception 20040105T010203\n"
    "InitInvestment 0\n"
    "SubsInvestment 0\n"
    "Assets 0\n"
    "Turnover 0\n"
    "Yield 0\n"
    "DeferredLoad 0\n"
    "FrontLoad 0\n"
    "Category \n"
    ;

  std::ostringstream os;

  StockMetaData data;
  StockID id("ibm");
  data.stockInfo.setID(id);
  data.stockInfo.setInception(boost::posix_time::from_iso_string("20040105T010203"));
  data.start = boost::posix_time::from_iso_string("20040105T010203");
  data.end = boost::posix_time::from_iso_string("20040305T040506");

  CPPUNIT_ASSERT(StockMetaDataStream::write(os, data, m_ctx));

  CPPUNIT_ASSERT_EQUAL(std::string(result), os.str());

  std::istringstream is(result);
  StockMetaData newData;
  CPPUNIT_ASSERT(StockMetaDataStream::read(is, newData, m_ctx));
  CPPUNIT_ASSERT(data.stockInfo.getSymbol() == newData.stockInfo.getSymbol());
  CPPUNIT_ASSERT(data.start == newData.start);
  CPPUNIT_ASSERT(data.end == newData.end);
}

void TestStockMetaDataStream::test2()
{
  // this one parses OK
  {
    const char* origData = 
      "Symbol ibm\n"
      "Start 20040105T010203\n"
      "End 20040305T040506\n"
      ;

    std::istringstream is(origData);
    StockMetaData newData;
    CPPUNIT_ASSERT(StockMetaDataStream::read(is, newData, m_ctx));
  }


  // various things that should fail to parse

  {
    const char* origData = 
      "Symbol ibm\n"
      "Start1 20040105T010203\n"
      "End 20040305T040506\n"
      ;

    std::istringstream is(origData);
    StockMetaData newData;
    CPPUNIT_ASSERT(!StockMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "Symbol ibm "
      "Start 20040105T010203\n"
      "End 20040305T040506\n"
      ;

    std::istringstream is(origData);
    StockMetaData newData;
    CPPUNIT_ASSERT(!StockMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "Start 20040105T010203\n"
      "End 20040305T040506\n"
      ;

    std::istringstream is(origData);
    StockMetaData newData;
    CPPUNIT_ASSERT(!StockMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = "";
    std::istringstream is(origData);
    StockMetaData newData;
    CPPUNIT_ASSERT(!StockMetaDataStream::read(is, newData, m_ctx));
  }


  {
    const char* origData = 
      "Symbol ibm "
      "Start 20040105T010203 "
      "End 20040305T040506 "
      ;

    std::istringstream is(origData);
    StockMetaData newData;
    CPPUNIT_ASSERT(!StockMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "symbol ibm\n"
      "start 20040105T010203\n"
      "end 20040305T040506\n"
      ;

    std::istringstream is(origData);
    StockMetaData newData;
    CPPUNIT_ASSERT(!StockMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "Symbol ibm\n"
      "Start 20040105X010203\n"
      "End 20040305T040506\n"
      ;

    std::istringstream is(origData);
    StockMetaData newData;
    CPPUNIT_ASSERT(!StockMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "Symbol ibm\n"
      "Start X20040105T010203\n"
      "End 20040305T040506\n"
      ;

    std::istringstream is(origData);
    StockMetaData newData;
    CPPUNIT_ASSERT(!StockMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "Symbol ibm\n"
      "Start \n"
      "End 20040305T040506\n"
      ;

    std::istringstream is(origData);
    StockMetaData newData;
    CPPUNIT_ASSERT(!StockMetaDataStream::read(is, newData, m_ctx));
  }

  {
    const char* origData = 
      "Symbol ibm\n"
      "Start 20040105T010203\n"
      "End 9 20040305T040506\n"
      ;

    std::istringstream is(origData);
    StockMetaData newData;
    CPPUNIT_ASSERT(!StockMetaDataStream::read(is, newData, m_ctx));
  }
}

void TestStockMetaDataStream::test3()
{

  const char* result = 
    "Symbol ibm\n"
    "Start 20040105T010203\n"
    "End 20040305T040506\n"
    "Description this is a description\n"
    "ExpenseRatio 0.33\n"
    "12b1Fee 12.33\n"
    "Family Vanguard\n"
    "Rating 4\n"
    "Inception 19900101T020202\n"
    "InitInvestment 2300\n"
    "SubsInvestment 120\n"
    "Assets 123990\n"
    "Turnover 12.3\n"
    "Yield 1.02\n"
    "DeferredLoad 1.03\n"
    "FrontLoad 4.55\n"
    "Category Mid-Cap funds\n"
    ;

  std::ostringstream os;

  StockMetaData data;
  data.start = boost::posix_time::from_iso_string("20040105T010203");
  data.end = boost::posix_time::from_iso_string("20040305T040506");

  StockID id("ibm");
  data.stockInfo.setID(id);

  StockInfo& sid = data.stockInfo;
  sid.setName("this is a description");
  sid.setExpenseRatio(0.33);
  sid.set12b1Fee(12.33);
  sid.setFamily("Vanguard");
  sid.setRating(4);
  sid.setInception(boost::posix_time::from_iso_string("19900101T020202"));
  sid.setInitInvestment(2300);
  sid.setSubsInvestment(120);
  sid.setAssets(123990);
  sid.setTurnover(12.30);
  sid.setYield(1.02);
  sid.setDeferredLoad(1.03);
  sid.setFrontLoad(4.55);
  sid.setCategory("Mid-Cap\nfunds");

  CPPUNIT_ASSERT(StockMetaDataStream::write(os, data, m_ctx));

  CPPUNIT_ASSERT_EQUAL(std::string(result), os.str());

  std::istringstream is(result);
  StockMetaData newData;
  const StockInfo& newSid = newData.stockInfo;
  CPPUNIT_ASSERT(StockMetaDataStream::read(is, newData, m_ctx));
  CPPUNIT_ASSERT(data.start == newData.start);
  CPPUNIT_ASSERT(data.end == newData.end);
  CPPUNIT_ASSERT_EQUAL(sid.getSymbol(), newSid.getSymbol());
  CPPUNIT_ASSERT_EQUAL(sid.getName(), newSid.getName());
  CPPUNIT_ASSERT_EQUAL(sid.getFamily(), newSid.getFamily());
  CPPUNIT_ASSERT_EQUAL(sid.getRating(), newSid.getRating());
  CPPUNIT_ASSERT(sid.getInception() == newSid.getInception());
  CPPUNIT_ASSERT_EQUAL(sid.getInitInvestment(), newSid.getInitInvestment());
  CPPUNIT_ASSERT_EQUAL(sid.getSubsInvestment(), newSid.getSubsInvestment());
  CPPUNIT_ASSERT_EQUAL(sid.getAssets(), newSid.getAssets());
  CPPUNIT_ASSERT_EQUAL(std::string("Mid-Cap funds"), newSid.getCategory());

  double delta = 0.001;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sid.getExpenseRatio(), newSid.getExpenseRatio(), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sid.get12b1Fee(), newSid.get12b1Fee(), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sid.getTurnover(), newSid.getTurnover(), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sid.getYield(), newSid.getYield(), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sid.getDeferredLoad(), newSid.getDeferredLoad(), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sid.getFrontLoad(), newSid.getFrontLoad(), delta);
}

} // namespace alch
