#include "TestPathRegistry.h"
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

namespace alch
{

void TestPathRegistry::setUp() 
{
  ;
}

void TestPathRegistry::tearDown()
{
  m_ctx.dump(std::cerr);
}

void TestPathRegistry::test1()
{
  uid_t uid = getuid();
  struct passwd* pwd = getpwuid(uid);
  CPPUNIT_ASSERT(pwd);
  CPPUNIT_ASSERT(pwd->pw_dir);
  std::string userDir(pwd->pw_dir);
  userDir += "/.alchemy";

  {
    CPPUNIT_ASSERT_EQUAL(userDir, PathRegistry::getUserDir());
  }

  {
    std::stringstream ss;
    ss << userDir << "/data";
    CPPUNIT_ASSERT_EQUAL(ss.str(), PathRegistry::getDataDir());
  }

  {
    std::stringstream ss;
    ss << userDir << "/pref.dat";
    CPPUNIT_ASSERT_EQUAL(ss.str(), PathRegistry::getPreferenceFile());
  }

  {
    std::stringstream ss;
    ss << userDir << "/symbols.dat";
    CPPUNIT_ASSERT_EQUAL(ss.str(), PathRegistry::getSymbolFile());
  }

}

} // namespace alch
