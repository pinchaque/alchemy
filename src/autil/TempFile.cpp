#include "autil/TempFile.h"
#include "autil/Exception.h"

#include <sstream> 
#include <vector>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

namespace alch
{

TempFile::TempFile(const char* prefix, TempFile::DelMode mode)
  : m_fd(-1)
  , m_mode(mode)
{
  std::stringstream nameSS;
  const char* slashPos = strchr(prefix, '/');

  // prefix isn't a path, so we prepend the system temporary directory
  if (!slashPos)
  {
    char* tmpDir = getenv("TMPDIR");

    if (tmpDir)
    {
      nameSS << tmpDir << "/";
    }
    else
    {
      nameSS << "/tmp/";
    }
  }

  nameSS << prefix << "XXXXXX";

  std::string foo(nameSS.str());
  // nameVec is null-terminated char array
  std::vector<char> nameVec(foo.begin(), foo.end());
  nameVec.push_back(0);

  if ((m_fd = ::mkstemp(&nameVec[0])) == -1)
  {
    std::stringstream ss;
    ss << "Error creating temp file '" << &nameVec[0] << "'";
    ATHROWE(Exception, ss.str(), errno);
  }

  if (mode == DELMODE_immed)
  {
    unlink();
  }
  else
  {
    m_name = &nameVec[0];
  }
}

TempFile::~TempFile()
{
  if (m_mode == DELMODE_destruct)
  {
    try {
      unlink();
    } catch (...) { }
  }

  try {
    close();
  } catch (...) { }
}

bool TempFile::unlink()
{
  if (m_name.empty())
    return false;

  if (::unlink(m_name.c_str()) == -1)
  {
    ATHROWE(Exception, "Error unlinking temp file", errno);
  }

  m_name.clear();
  return true;
}

bool TempFile::close()
{
  if (m_fd == -1)
    return false;

  if (::close(m_fd) == -1)
  {
    ATHROWE(Exception, "Error closing temp file", errno);
  }

  m_fd = -1;
  return true;
}

} // namespace cmn
