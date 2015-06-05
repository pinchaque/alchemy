
#include "stockdata/MetaFileStockDataSource.h"
#include "stockdata/StockMetaDataStream.h"

#include "boost/filesystem/operations.hpp"

#include <sstream>
#include <fstream>
#include <algorithm>

namespace alch {

  MetaFileStockDataSource::MetaFileStockDataSource(const std::string& rootDir, 
                                                   Context& ctx)
    : FileStockDataSource(rootDir, ctx)
  {
    ;
  }


  //! Destructor
  MetaFileStockDataSource::~MetaFileStockDataSource()
  {
    ;
  }



  bool MetaFileStockDataSource::getStockList(
    std::vector<StockID>& stockList)
  {
    return FileStockDataSource::getStockList(stockList);
  }


  bool MetaFileStockDataSource::hasSymbol(const StockID& id)
  {
    boost::filesystem::path metaFilePath(getMetaFileName(id),
                                         boost::filesystem::native);
    return (boost::filesystem::exists(metaFilePath)
            && FileStockDataSource::hasSymbol(id));
  }


  bool MetaFileStockDataSource::retrieve(const StockID& id, StockInfo& info, 
      RangeData& data)
  {
    StockMetaData metaData;
    if (readMetaFile(id, metaData))
    {
      info = metaData.stockInfo;
    }

    return FileStockDataSource::retrieve(id, info, data);
  }

  bool MetaFileStockDataSource::hasSymbolDate(const StockID& id, 
                                              const StockTime& start,
                                              const StockTime& end)
  {
    StockMetaData metaData;
    if (!readMetaFile(id, metaData))
    {
      return false;
    }

    // check to see if the metaFile date range contains the one we're
    // asking for
    return ((metaData.start <= start) && (metaData.end >= end));
  }


  bool MetaFileStockDataSource::retrieveDate(const StockID& id, 
                                             const StockTime& start,
                                             const StockTime& end,
                                             StockInfo& info,
                                             RangeData& data)
  { 
    if (!hasSymbolDate(id, start, end))
    {
      return false;
    }

    StockMetaData metaData;
    if (readMetaFile(id, metaData))
    {
      info = metaData.stockInfo;
    }

    return FileStockDataSource::retrieveDate(id, start, end, info, data);
  }


  bool MetaFileStockDataSource::save(const StockID& id,
                                     const StockTime& start,
                                     const StockTime& end,
                                     const StockInfo& info,
                                     const RangeData& data)
  {
    getContext() << Context::PRIORITY_debug1
                 << "Saving data for '"
                 << id
                 << "' over " << start << " - " << end
                 << Context::endl;


    // nothing to persist
    if (!data.size())
    {
      getContext() << Context::PRIORITY_debug1
                   << "Empty data set for '"
                   << id
                   << "' -- nothing to do!"
                   << Context::endl;
      return true;
    }

    // verify that new data points are sorted
    RangeData sortedData(data);
    sortedData.sortDate();

    // existing meta data 
    StockMetaData origMetaData;

    // Don't have metadata--just overwrite since there's nothing there
    if (!readMetaFile(id, origMetaData))
    {
      getContext() << Context::PRIORITY_debug1
                   << "No existing meta data file for '"
                   << id
                   << "' -- overwriting with new data"
                   << Context::endl;
      return saveOverwrite(info, start, end, sortedData);
    }
    // if new data points are entirely after existing data points, 
    // then we can simply append
    // note this assumes that there are no gaps between existing data
    // and new data!!!
    else if (origMetaData.end < start)
    {
      getContext() << Context::PRIORITY_debug1
                   << "Existing meta data end "
                   << origMetaData.end
                   << " is before new start " << start
                   << " -- appending new data"
                   << Context::endl;
      return saveAppend(info, start, end, sortedData);
    }
    // otherwise we must merge new data points in
    else
    {
      getContext() << Context::PRIORITY_debug1
                   << "Performing merge operation on old and new data"
                   << Context::endl;
      return saveMerge(info, start, end, sortedData);
    }
  }


  bool MetaFileStockDataSource::remove(const StockID& id)
  {
    // remove the meta file
    boost::filesystem::path metaFilePath(getMetaFileName(id),
                                         boost::filesystem::native);
    bool ret = boost::filesystem::remove(metaFilePath);

    // remove the data file -- don't shortcircuit
    return (FileStockDataSource::remove(id) && ret);
  }


  std::string MetaFileStockDataSource::getMetaFileName(const StockID& id) const
  {
    std::stringstream ss;

    ss << FileStockDataSource::getSymbolDirectory(getRootDir(), id.getSymbol())
       << "/" << id.getSymbol() << ".meta";

    return ss.str();
  }


  bool MetaFileStockDataSource::readMetaFile(const StockID& id,
                                             StockMetaData& metaData)
  {
    std::string metaFileName(getMetaFileName(id));
    boost::filesystem::path metaFilePath(metaFileName,
                                         boost::filesystem::native);

    if (!boost::filesystem::exists(metaFilePath))
    {
      getContext() << Context::PRIORITY_debug1
                   << "Metadata file '"
                   << metaFileName
                   << "' did not exist"
                   << Context::endl;
      return false;
    }


    std::ifstream ifs(metaFileName.c_str());

    if (!ifs)
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open metadata file '"
                   << metaFileName
                   << "'"
                   << Context::endl;
      return false;
    }

    if (!StockMetaDataStream::read(ifs, metaData, getContext())
        || (id != metaData.stockInfo.getID()))
    {
      getContext() << Context::PRIORITY_error
                   << "Possibly corrupt metadata file '"
                   << metaFileName
                   << "': contained StockID '"
                   << metaData.stockInfo.getID()
                   << "' does not match requested StockID '"
                   << id
                   << "'"
                   << Context::endl;
      ifs.close();
      return false;
    }

    ifs.close();

    return true;
  }



  bool MetaFileStockDataSource::saveOverwrite(const StockInfo& info, 
                                              const StockTime& start,
                                              const StockTime& end,
                                              const RangeData& data)
  {
    // set up the new metadata
    StockMetaData metaData;
    metaData.stockInfo = info;
    metaData.start = start;
    metaData.end = end;

    // set up overwrite mode
    setAppend(false);

    // put the data into the file
    if (!FileStockDataSource::save(info.getID(), start, end, info, data))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while trying to write data to '"
                   << getSymbolFile(getRootDir(), info.getSymbol())
                   << "'" << Context::endl;

      return false;
    }
    else
    {
      getContext() << Context::PRIORITY_debug1
                   << "Wrote " << data.size() << " data records to '"
                   << getSymbolFile(getRootDir(), info.getSymbol())
                   << "'" << Context::endl;
    }

    if (!writeMetaFile(metaData))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while trying to save metadata"
                   << Context::endl;
      return false;
    }

    return true;
  }


  bool MetaFileStockDataSource::saveAppend(const StockInfo& info, 
                                           const StockTime& start,
                                           const StockTime& end,
                                           const RangeData& data)
  {
    // set up the new metadata
    StockMetaData metaData;
    metaData.stockInfo = info;
    metaData.start = start;
    readMetaFile(info.getID(), metaData); // ignoring errors
    metaData.end = end; // new end value

    // set up append mode
    setAppend(true);

    // put the data into the file
    if (!FileStockDataSource::save(info.getID(), start, end, info, data))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while trying to append data to '"
                   << getSymbolFile(getRootDir(), info.getSymbol())
                   << "'" << Context::endl;

      return false;
    }
    else
    {
      getContext() << Context::PRIORITY_debug1
                   << "Appended " << data.size() << " data records to '"
                   << getSymbolFile(getRootDir(), info.getSymbol())
                   << "'" << Context::endl;
    }

    if (!writeMetaFile(metaData))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while trying to save metadata"
                   << Context::endl;
      return false;
    }

    return true;
  }


  bool MetaFileStockDataSource::saveMerge(const StockInfo& info,
                                          const StockTime& start,
                                          const StockTime& end,
                                          const RangeData& data)
  {
    // set up the new metadata
    StockMetaData metaData;
    metaData.stockInfo = info;
    metaData.start = start;
    metaData.end = end;


    // merge with original metadata
    {
      // read the meta data in
      StockMetaData origMetaData;
      if (!readMetaFile(info.getID(), origMetaData))
      {
        getContext() << Context::PRIORITY_error
                     << "Error reading existing meta data file"
                     << Context::endl;

        return false;
      }

      getContext() << Context::PRIORITY_debug1
                   << "Existing meta data: "
                   << origMetaData.start 
                   << " - "
                   << origMetaData.end
                   << Context::endl;

      getContext() << Context::PRIORITY_debug1
                   << "New meta data: "
                   << metaData.start 
                   << " - "
                   << metaData.end
                   << Context::endl;

      if (origMetaData.start < metaData.start)
      {
        metaData.start = origMetaData.start;
      }

      if (origMetaData.end > metaData.end)
      {
        metaData.end = origMetaData.end;
      }

      getContext() << Context::PRIORITY_debug1
                   << "Combined meta data: "
                   << metaData.start 
                   << " - "
                   << metaData.end
                   << Context::endl;
    }

    // read the existing data
    RangeData origData;
    StockInfo origInfo;
    if (!retrieve(info.getID(), origInfo, origData))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while reading existing data from '"
                   << getSymbolFile(getRootDir(), info.getSymbol())
                   << "'" << Context::endl;

      return false;
    }

    // merge the data
    origData.merge(data);

    // set up overwrite mode
    setAppend(false);

    // put the merged data into the file
    if (!FileStockDataSource::save(info.getID(), start, end, info, origData))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while trying to append data to '"
                   << getSymbolFile(getRootDir(), info.getSymbol())
                   << "'" << Context::endl;

      return false;
    }
    else
    {
      getContext() << Context::PRIORITY_debug1
                   << "Wrote " << data.size() << " merged data records to '"
                   << getSymbolFile(getRootDir(), info.getSymbol())
                   << "'" << Context::endl;
    }

    if (!writeMetaFile(metaData))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while trying to save metadata"
                   << Context::endl;
      return false;
    }

    return true;
  }


  bool MetaFileStockDataSource::writeMetaFile(const StockMetaData& metaData)
  {
    std::string metaFileName = getMetaFileName(metaData.stockInfo.getID());
    std::ofstream ofs(metaFileName.c_str(), std::ios::out | std::ios::trunc);

    if (!ofs)
    {
      getContext() << Context::PRIORITY_error
                   << "Error while opening metafile '"
                   << metaFileName
                   << "' for output"
                   << Context::endl;
      return false;
    }

    if (!StockMetaDataStream::write(ofs, metaData, getContext()))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while trying to save metadata to '"
                   << metaFileName
                   << "'" << Context::endl;

      ofs.close();

      return false;
    }

    ofs.close();

    return true;
  }

} // namespace alch
