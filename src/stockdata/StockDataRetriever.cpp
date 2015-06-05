
#include "stockdata/StockDataRetriever.h"
#include "stockdata/StockMetaData.h"
#include "stockdata/StockMetaDataStream.h"
#include "stockdata/MetaFileStockDataSource.h"
#include "stockdata/YahooStockDataSource.h"

#include "boost/filesystem/operations.hpp"

#include <sstream>
#include <fstream>
#include <cassert>
#include <algorithm>

namespace alch {

  bool StockDataRetriever::retrieve(const StockID& id,
                                    const StockTime& startTime,
                                    const StockTime& endTime,
                                    StockInfo& info,
                                    RangeData& data)
  
  {    
    bool retval = false;

    getContext() << Context::PRIORITY_debug1
                 << "Retrieving data for "
                 << id << " (" << startTime
                 << " - " << endTime << ")"
                 << Context::endl;


    // try to read data from local filesystem
    if (retrieveFileData(id, startTime, endTime, info, data, retval))
    {
      getContext() << Context::PRIORITY_debug1
                   << "File retrieval "
                   << (retval ? "SUCCESS" : "FATAL ERROR")
                   << " for " << id
                   << Context::endl;
      return retval;
    }
    else
    {
      getContext() << Context::PRIORITY_debug1
                   << "File retrieval FAILED -- try YAHOO"
                   << Context::endl;
    }

    // otherwise we need to get this data from Yahoo
    if (retrieveYahooData(id, startTime, endTime, info, data, retval))
    {
      getContext() << Context::PRIORITY_debug1
                   << "Yahoo retrieval "
                   << (retval ? "SUCCESS" : "FATAL ERROR")
                   << " for " << id
                   << Context::endl;
      return retval;
    }
    else
    {
      getContext() << Context::PRIORITY_debug1
                   << "Yahoo retrieval did not succeed"
                   << Context::endl;
    }

    getContext() << Context::PRIORITY_debug1
                 << "Failed to retrieve data for "
                 << id << " by any method"
                 << Context::endl;

    return false;
  }



  bool StockDataRetriever::retrieveFileData(const StockID& id,
                                            const StockTime& startTime,
                                            const StockTime& endTime,
                                            StockInfo& info,
                                            RangeData& data,
                                            bool& retval)
  {
    MetaFileStockDataSource dataSource(m_cacheDirectory, getContext());

    if (!dataSource.hasSymbolDate(id, startTime, endTime))
    {
      // data source didn't have the data -- continue processing
      return false;
    }

    if (!dataSource.retrieveDate(id, startTime, endTime, info, data))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to read data for '"
                   << id
                   << "' over date range ("
                   << startTime
                   << " - "
                   << endTime
                   << ")"
                   << Context::endl;

      // failed to get data when we should have -- error
      retval = false;
      return true;
    }

    // got data -- return success
    retval = true;
    return true;
  }


  bool StockDataRetriever::retrieveYahooData(const StockID& id,
                                             const StockTime& startTime,
                                             const StockTime& endTime,
                                             StockInfo& info,
                                             RangeData& data,
                                             bool& retval)
  {

    YahooStockDataSource yahoo(getContext());

    // get the requested date range from Yahoo
    RangeData yahooData;
    if (!yahoo.retrieve(id, info, yahooData))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while retrieving data from Yahoo for '"
                   << id
                   << "'"
                   << Context::endl;
      retval = false;
      return true;
    }
    else
    {
      getContext() << Context::PRIORITY_debug1
                   << "Read " << yahooData.size()
                   << " data records from Yahoo for " << id
                   << Context::endl;
    }

    // get the data that's in our specified range [startTime, endTime]
    // also figure out actual start and end dates for this data
    StockTime yahooStartTime(boost::posix_time::max_date_time);
    StockTime yahooEndTime(boost::posix_time::min_date_time);

    RangeData::const_iterator endIter = yahooData.end();
    RangeData::const_iterator iter;

    int numAdded = 0;
    for (iter = yahooData.begin(); iter != endIter; ++iter)
    {
      if (iter->tradeTime > yahooEndTime)
      {
        yahooEndTime = iter->tradeTime;
      }

      if (iter->tradeTime < yahooStartTime)
      {
        yahooStartTime = iter->tradeTime;
      }

      if ((iter->tradeTime >= startTime)
          && (iter->tradeTime <= endTime))
      {
        data.add(*iter);
        ++numAdded;
      }
    }

    // We now update our end time so that it is set to the last time in the
    // day. This is because Yahoo only gives us one data point per day and
    // it represents the entire day.
    yahooEndTime = StockTime(yahooEndTime.date(),
                             boost::posix_time::hours(23)
                             + boost::posix_time::minutes(59)
                             + boost::posix_time::seconds(59));

    // save this data to the stock data file and update the meta file
    if (yahooEndTime >= yahooStartTime)
    {
      MetaFileStockDataSource dataSource(m_cacheDirectory, getContext());

      // when we save, we want to use the minimum start time between what
      // was asked for and what yahoo gave us. This is because yahoo might
      // not have data back to our asking time. we want to avoid repeatedly
      // asking for it.
      if (!dataSource.save(
            id, 
            std::min(startTime, yahooStartTime),
            yahooEndTime,
            info,
            yahooData))
      {
        getContext() << Context::PRIORITY_warning
                     << "Error while trying to persist data from Yahoo for '"
                     << id
                     << "'; ignoring"
                     << Context::endl;
      }
      else
      {
        getContext() << Context::PRIORITY_debug1
                     << "Persisted " << yahooData.size()
                     << " data records for " << id << " to disk"
                     << Context::endl;
      }
    }

    getContext() << Context::PRIORITY_debug1
                 << "Returning " << numAdded
                 << " data points for specified date range out of "
                 << yahooData.size() << " total for " << id
                 << Context::endl;

    retval = true;
    return true;
  }

} // namespace alch
