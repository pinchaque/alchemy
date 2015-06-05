
#include "stockdata/OStockDataFile.h"
#include "stockdata/StockDataStream.h"
#include <cassert>

namespace alch {

OStockDataFile::OStockDataFile(Context& ctx)
  : m_ctx(ctx)
  , m_fstream()
  , m_fileName()
{
  ;
}

OStockDataFile::~OStockDataFile()
{
  close();
}

bool OStockDataFile::open(const std::string& fileName, 
                          OStockDataFile::Mode mode)
{
  close();
  m_fstream.clear();
  m_fileName = fileName;
  std::ios::openmode iosMode = std::ios::out;

  if (mode == MODE_append)
  {
    iosMode |= std::ios::app;
  }
  else if (mode == MODE_overwrite)
  {
    iosMode |= std::ios::trunc;
  }
  else
  {
    assert(0 && "Mode not supported");
  }

  m_fstream.open(m_fileName.c_str(), iosMode); 
  return m_fstream;
}


bool OStockDataFile::write(const RangeData& data)
{
  RangeData::const_iterator end = data.end();
  RangeData::const_iterator iter;
  for (iter = data.begin(); iter != end; ++iter)
  {
    if (!write(*iter))
    {
      return false;
    }
  }
  return true;
}


bool OStockDataFile::write(const RangeData::Point& dataPoint)
{
  if (m_fstream)
  {
    return StockDataStream::write(m_fstream, dataPoint, m_ctx);
  }
  else
  {
    return false;
  }
}

void OStockDataFile::close()
{
  if (m_fstream)
  {
    m_fstream.close();
  }
  m_fileName = "";
}

} // namespace alch
