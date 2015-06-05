#include "TestCLASSNAME.h"

namespace alch
{

void TestCLASSNAME::setUp() 
{
  ;
}

void TestCLASSNAME::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestCLASSNAME::test1()
{
  CPPUNIT_ASSERT(1);
}

} // namespace alch
