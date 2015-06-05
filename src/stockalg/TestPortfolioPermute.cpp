#include "TestPortfolioPermute.h"

namespace alch
{

void TestPortfolioPermute::setUp() 
{
  ;
}

void TestPortfolioPermute::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestPortfolioPermute::test1()
{
  PortfolioPtr pOrig(new Portfolio());

  StockID id1("vivax");
  StockID id2("ibm");
  StockID id3("ebay");
  pOrig->add(id1);
  pOrig->add(id2);
  pOrig->add(id3);

  int total = 20;
  PortfolioPermute pp(pOrig, total);
  double delta = 0.001;

  PortfolioPtr p;
  for (int i = 0; i < 5000; ++i)
  {
    // check the new portfolio
    p = pp.next();
    CPPUNIT_ASSERT(p != PortfolioPtr());
    Portfolio::Symbols s = p->getSymbols();
    CPPUNIT_ASSERT_EQUAL(3, (int)s.size());

    // sum up the shares we distributed
    Portfolio::Symbols::const_iterator end = s.end();
    Portfolio::Symbols::const_iterator iter;
    double checkTotal = 0.0;
    for (iter = s.begin(); iter != end; ++iter)
    {
      double amount = p->get(*iter);
      checkTotal += amount;
    }
    
    // check the sums
    CPPUNIT_ASSERT_DOUBLES_EQUAL(total, checkTotal, delta);
  }
}

} // namespace alch
