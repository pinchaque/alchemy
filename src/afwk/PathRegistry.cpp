
#include "afwk/PathRegistry.h"

#include <sstream>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>


namespace alch {

  const char* const PathRegistry::s_preferenceFile = "pref.dat";
  const char* const PathRegistry::s_symbolFile = "symbols.dat";
  
  std::string PathRegistry::getDataDir()
  {
    std::stringstream ss;
    ss << getUserDir()
       << "/data";
    return ss.str();    
  }


  std::string PathRegistry::getUserDir()
  {
    uid_t uid = getuid();
    struct passwd* pwd = getpwuid(uid);

    std::stringstream ss;

    if (pwd && pwd->pw_dir)
    {
      ss << pwd->pw_dir
         << "/.alchemy";
    }
    else
    {
      ss << "/tmp/" << uid;
    }

    return ss.str();
  }


  std::string PathRegistry::getPreferenceFile()
  {
    std::stringstream ss;
    ss << getUserDir()
       << "/"
       << s_preferenceFile;
    return ss.str();
  }


  std::string PathRegistry::getSymbolFile()
  {
    std::stringstream ss;
    ss << getUserDir()
       << "/"
       << s_symbolFile;
    return ss.str();
  }


} // namespace alch
