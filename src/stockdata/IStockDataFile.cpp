
#include "stockdata/IStockDataFile.h"
#include "stockdata/StockDataStream.h"

namespace alch {

IStockDataFile::IStockDataFile(Context& ctx)
  : m_ctx(ctx)
  , m_fstream()
  , m_fileName()
{
}


IStockDataFile::~IStockDataFile()
{
  close();
}


bool IStockDataFile::open(const std::string& fileName)
{
  close();
  m_fileName = fileName;
  m_fstream.clear();
  m_fstream.open(m_fileName.c_str(), std::ios::in); 
  return m_fstream;
}


bool IStockDataFile::read(RangeData& data)
{
  // keep reading until eof or error
  while (m_fstream)
  {
    RangeData::Point dataPoint;

    // read the data point
    if (!read(dataPoint))
    {
      break;
    }

    // add the data point to the dataset
    data.add(dataPoint);
  }

  // if we hit eof, then we read OK, otherwise we must have hit an error
  return m_fstream.eof();
}


bool IStockDataFile::read(RangeData::Point& dataPoint)
{
  if (m_fstream)
  {
    return StockDataStream::read(m_fstream, dataPoint, m_ctx);
  }
  else
  {
    return false;
  }
}


bool IStockDataFile::rewind()
{
  if (m_fstream || m_fstream.eof())
  {
    m_fstream.clear();
    m_fstream.seekg(0, std::ios::beg);
    m_fstream.clear();
    return m_fstream;
  }
  else
  {
    return false;
  }
}


void IStockDataFile::close()
{
  if (m_fstream)
  {
    m_fstream.close();
  }
  m_fileName = "";
}


} // namespace alch
