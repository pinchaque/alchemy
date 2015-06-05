#include "TestPortfolio.h"

namespace alch
{

void TestPortfolio::setUp() 
{
  ;
}

void TestPortfolio::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestPortfolio::test1()
{
  Portfolio p;

  StockID id1("vivax");
  StockID id2("ibm");
  StockID id3("ebay");

  double delta = 0.0001;

  CPPUNIT_ASSERT_EQUAL(false, p.has(id1));
  CPPUNIT_ASSERT_EQUAL(false, p.has(id2));
  CPPUNIT_ASSERT_EQUAL(false, p.has(id3));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, p.get(id1), delta);
  CPPUNIT_ASSERT_EQUAL(0, (int)p.getSymbols().size());

  p.add(id1, 10.11);
  CPPUNIT_ASSERT_EQUAL(true, p.has(id1));
  CPPUNIT_ASSERT_EQUAL(false, p.has(id2));
  CPPUNIT_ASSERT_EQUAL(false, p.has(id3));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10.11, p.get(id1), delta);
  CPPUNIT_ASSERT_EQUAL(1, (int)p.getSymbols().size());

  p.add(id1, 5.55);
  CPPUNIT_ASSERT_EQUAL(true, p.has(id1));
  CPPUNIT_ASSERT_EQUAL(false, p.has(id2));
  CPPUNIT_ASSERT_EQUAL(false, p.has(id3));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.55, p.get(id1), delta);
  CPPUNIT_ASSERT_EQUAL(1, (int)p.getSymbols().size());

  p.add(id2, 7.77);
  CPPUNIT_ASSERT_EQUAL(true, p.has(id1));
  CPPUNIT_ASSERT_EQUAL(true, p.has(id2));
  CPPUNIT_ASSERT_EQUAL(false, p.has(id3));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.55, p.get(id1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.77, p.get(id2), delta);
  CPPUNIT_ASSERT_EQUAL(2, (int)p.getSymbols().size());

  p.add(id3, 9.99);
  CPPUNIT_ASSERT_EQUAL(true, p.has(id1));
  CPPUNIT_ASSERT_EQUAL(true, p.has(id2));
  CPPUNIT_ASSERT_EQUAL(true, p.has(id3));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.55, p.get(id1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.77, p.get(id2), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(9.99, p.get(id3), delta);
  CPPUNIT_ASSERT_EQUAL(3, (int)p.getSymbols().size());

  Portfolio::Symbols s = p.getSymbols();
  CPPUNIT_ASSERT_EQUAL(std::string("vivax"), s[0].getSymbol());
  CPPUNIT_ASSERT_EQUAL(std::string("ibm"), s[1].getSymbol());
  CPPUNIT_ASSERT_EQUAL(std::string("ebay"), s[2].getSymbol());


  PortfolioPtr p2 = p.clone();
  CPPUNIT_ASSERT_EQUAL(true, p2->has(id1));
  CPPUNIT_ASSERT_EQUAL(true, p2->has(id2));
  CPPUNIT_ASSERT_EQUAL(true, p2->has(id3));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.55, p2->get(id1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.77, p2->get(id2), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(9.99, p2->get(id3), delta);
  CPPUNIT_ASSERT_EQUAL(3, (int)p2->getSymbols().size());

  p2->setZero();
  CPPUNIT_ASSERT_EQUAL(true, p2->has(id1));
  CPPUNIT_ASSERT_EQUAL(true, p2->has(id2));
  CPPUNIT_ASSERT_EQUAL(true, p2->has(id3));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, p2->get(id1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, p2->get(id2), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, p2->get(id3), delta);
  CPPUNIT_ASSERT_EQUAL(3, (int)p2->getSymbols().size());
  
  p.clear();
  CPPUNIT_ASSERT_EQUAL(0, (int)p.getSymbols().size());
  CPPUNIT_ASSERT_EQUAL(false, p.has(id1));
  CPPUNIT_ASSERT_EQUAL(false, p.has(id2));
  CPPUNIT_ASSERT_EQUAL(false, p.has(id3));

}

void TestPortfolio::test2()
{
  Portfolio p;

  StockID id1("vivax");
  StockID id2("ibm");
  StockID id3("ebay");

  double delta = 0.0001;

  p.add(id1, 1.00);
  p.add(id2, 5.00);
  p.add(id3, 4.00);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, p.get(id1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, p.get(id2), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, p.get(id3), delta);

  p.normalize(100.0);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, p.get(id1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(50.0, p.get(id2), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(40.0, p.get(id3), delta);
  
  p.add(id1, 0.0);
  p.normalize(9.0);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, p.get(id1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, p.get(id2), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, p.get(id3), delta);
  p.add(id2, 0.0);
  p.add(id3, 0.0);
  p.normalize(9.0);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, p.get(id1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, p.get(id2), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, p.get(id3), delta);
}

void TestPortfolio::test3()
{
  Portfolio p;
  StockID id1("vivax");
  StockID id2("ibm");
  StockID id3("ebay");

  double delta = 0.0001;

  p.add(id1, 1.00);
  p.add(id2, 5.00);
  p.add(id3, 4.00);
  
  Portfolio p2;
  p2.add(id1, 23.00);
  p2.add(id2, 29.00);
  p2.add(id3, 48.00);

  p2.rebalance(p);

  CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, p2.get(id1), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(50.0, p2.get(id2), delta);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(40.0, p2.get(id3), delta);
}

} // namespace alch
