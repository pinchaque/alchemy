
#include "stockdata/FileStockDataSource.h"
#include "stockdata/IStockDataFile.h"
#include "stockdata/OStockDataFile.h"

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/exception.hpp"
#include "boost/filesystem/convenience.hpp"

#include <sstream>
#include <cassert>
#include <iostream>

namespace alch {


  const char* const FileStockDataSource::s_fileExtension = "dat";


  FileStockDataSource::FileStockDataSource(const std::string& rootDir, 
                                           Context& ctx)
    : StockDataSource(ctx)
    , m_rootDir(rootDir)
  {
    ;
  }


  //! Destructor
  FileStockDataSource::~FileStockDataSource()
  {
    ;
  }


  bool FileStockDataSource::getStockList(std::vector<StockID>& stockList)
  {
    // iterate through all subdirectories
    for (char ch = 'a'; ch <= 'z'; ++ch)
    {
      std::string dirString(m_rootDir + '/' + ch);
      boost::filesystem::path dirPath(dirString, boost::filesystem::native);

      if (boost::filesystem::exists(dirPath)
          && boost::filesystem::is_directory(dirPath)
          && !getStockListDirectory(dirString, stockList))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to get stock list for directory '"
                     << dirString << "'"
                     << Context::endl;
        return false;
      }
    }

    return true;
  }


  bool FileStockDataSource::hasSymbol(const StockID& id)
  {
    boost::filesystem::path filePath(getSymbolFile(m_rootDir, 
                                                   id.getSymbol()), 
                                     boost::filesystem::native);

    return  (boost::filesystem::exists(filePath)
             && !boost::filesystem::is_directory(filePath));
  }



  bool FileStockDataSource::retrieve(const StockID& id, StockInfo& info, 
      RangeData& data)
  {
    info.setID(id);
    std::string fileName = getSymbolFile(m_rootDir, 
                                         id.getSymbol());

    // now we know that the file exists, so let's try to parse it.
    bool retval = true;
    if (!isValidFile(fileName, retval))
    {
      return retval;
    }

    IStockDataFile inF(getContext());

    if (!inF.open(fileName))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open file '" << fileName << "'"
                   << Context::endl;
      return false;
    }

    if (!inF.read(data))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while reading stock data from file '"
                   << fileName << "'" << Context::endl;
      return false;
    }

    return true;
  }


  bool FileStockDataSource::hasSymbolDate(const StockID& id, 
                                          const StockTime& start,
                                          const StockTime& end)
  {
    assert(start <= end);
    std::string fileName = getSymbolFile(m_rootDir, 
                                         id.getSymbol());

    // use a dummy context since we don't care about errors
    Context ctx;
    IStockDataFile inF(ctx);

    if (!inF.open(fileName))
    {
      return false;
    }

    // read one data point at a time, checking to see if it's in this date
    // range
    bool retval = false;
    RangeData::Point dataPoint;
    while (inF.read(dataPoint))
    {
      if ((dataPoint.tradeTime >= start)
          && (dataPoint.tradeTime <= end))
      {
        retval = true;
        break;
      }
    }

    return retval;
  }


  bool FileStockDataSource::retrieveDate(const StockID& id, 
                                         const StockTime& start,
                                         const StockTime& end,
                                         StockInfo& info,
                                         RangeData& data)
  {
    assert(start <= end);
    info.setID(id);
    std::string fileName = getSymbolFile(m_rootDir, 
                                         id.getSymbol());

    // now we know that the file exists, so let's try to parse it.
    bool retval = true;
    if (!isValidFile(fileName, retval))
    {
      return retval;
    }

    IStockDataFile inF(getContext());

    if (!inF.open(fileName))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open file '" << fileName << "'"
                   << Context::endl;
      return false;
    }

    // read one data point at a time, checking to see if it's in this date
    // range
    RangeData::Point dataPoint;
    while (inF.read(dataPoint))
    {
      if ((dataPoint.tradeTime >= start)
          && (dataPoint.tradeTime <= end))
      {
        data.add(dataPoint);
      }
    }

    return true;
  }


  bool FileStockDataSource::save(const StockID& id, 
                                 const StockTime& start,
                                 const StockTime& end,
                                 const StockInfo& info,
                                 const RangeData& data)
  {
    // create the root directory if it doesn't already exist
    std::string dirName = getSymbolDirectory(m_rootDir, id.getSymbol());
    boost::filesystem::path dirPath(dirName, boost::filesystem::native);

    // if dirName exists but is not a directory, then this is an error
    if (boost::filesystem::exists(dirPath)
        && !boost::filesystem::is_directory(dirPath))
    {
      getContext() << Context::PRIORITY_error
                   << "Path exists but is not a directory '" << dirName << "'"
                   << Context::endl;
      return false;
    }
    // doesn't exist--create it
    else if (!boost::filesystem::exists(dirPath))
    {
      boost::filesystem::create_directory(dirPath);

      if (!boost::filesystem::is_directory(dirPath))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to create symbol directory '" << dirName
                     << "'" << Context::endl;
        return false;
      }
    }

    std::string fileName = getSymbolFile(m_rootDir, 
                                         id.getSymbol());

    OStockDataFile outF(getContext());

    OStockDataFile::Mode mode = (m_append
                                 ? OStockDataFile::MODE_append
                                 : OStockDataFile::MODE_overwrite);

    if (!outF.open(fileName, mode))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open file for append '" << fileName << "'"
                   << Context::endl;
      return false;
    }

    if (!outF.write(data))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while writing stock data to file '" << fileName
                   << "'" << Context::endl;
      return false;
    }

    return true;
  }


  bool FileStockDataSource::remove(const StockID& id)
  {
    std::string fileName = getSymbolFile(m_rootDir, 
                                         id.getSymbol());
    ::remove(fileName.c_str());
    return true;
  }


  std::string FileStockDataSource::getSymbolDirectory(
    const std::string& root,
    const std::string& symbol)
  {
    assert(!symbol.empty());

    std::stringstream ss;
    ss << root << "/" << char(::tolower(symbol[0]));
    return ss.str();
  }


  std::string FileStockDataSource::getSymbolFile(
    const std::string& root,
    const std::string& symbol)
  {
    std::stringstream ss;
    ss << getSymbolDirectory(root, symbol) << "/" << symbol << "."
       << s_fileExtension;
    return ss.str();
  }



  bool FileStockDataSource::isValidFile(
    const std::string& fileName,
    bool& retval)
  {
    boost::filesystem::path filePath(fileName, boost::filesystem::native);

    // not an error if it doesn't exist
    if (!boost::filesystem::exists(filePath))
    {
      retval = true;
      return false;
    }

    // it's an error if it's a directory
    if (boost::filesystem::is_directory(filePath))
    {
      getContext() << Context::PRIORITY_error << "Invalid directory '"
                   << fileName
                   << "' when looking for symbol file" << Context::endl;

      retval = false;
      return false;
    }

    return true;
  }


  bool FileStockDataSource::getStockListDirectory(
    const std::string& dirString,
    std::vector<StockID>& stockList)
  {
    boost::filesystem::path dirPath(dirString, boost::filesystem::native);

    if (!boost::filesystem::is_directory(dirPath))
    {
      getContext() << Context::PRIORITY_error
                   << "Invalid stock directory '" << dirPath.string() << "'"
                   << Context::endl;
      return false;
    }    

    try 
    {
      boost::filesystem::directory_iterator end;
      boost::filesystem::directory_iterator iter(dirPath);
      std::string extensionWithDot(".");
      extensionWithDot += s_fileExtension;

      for ( ; iter != end; ++iter)
      {
        if (boost::filesystem::exists(*iter)
            && !boost::filesystem::is_directory(*iter)
            && (boost::filesystem::extension(*iter) == extensionWithDot))
        {
          stockList.push_back(
            StockID(boost::filesystem::basename(iter->leaf())));
        }
      }
    }
    catch (boost::filesystem::filesystem_error err)
    {
      getContext() << Context::PRIORITY_error
                   << "Unable to retrieve stock list from '"
                   << dirPath.string()
                   << "'" << Context::endl;
      return false;
    }

    return true;
  }

} // namespace alch
