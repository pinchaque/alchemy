#include "TestPortfolioAlg.h"

namespace alch
{

void TestPortfolioAlg::setUp() 
{
  ;
}

void TestPortfolioAlg::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestPortfolioAlg::test1()
{
  CPPUNIT_ASSERT(1);
}

} // namespace alch
