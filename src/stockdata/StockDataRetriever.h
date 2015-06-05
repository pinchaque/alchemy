// -*- C++ -*-

#ifndef INCLUDED_stockdata_StockDataRetriever_h
#define INCLUDED_stockdata_StockDataRetriever_h

#include "autil/Context.h"
#include "stockdata/StockTime.h"
#include "stockdata/StockID.h"
#include "stockdata/StockInfo.h"
#include "stockdata/RangeData.h"

#include <string>

namespace alch {

/*!
  \brief Class that provides a high-level interface to stock data retrieval
  \ingroup stockdata
*/
class StockDataRetriever
{
 public:

  /*!
    \brief Constructor
    \param cacheDirectory The directory in which stock data will be cached
    \param ctx Context for operational messages
  */
  StockDataRetriever(const std::string& cacheDirectory,
                     Context& ctx)
    : m_cacheDirectory(cacheDirectory)
    , m_ctx(ctx)
  {
    ;
  }

  ~StockDataRetriever()
  {
    ;
  }

  /*!
    \brief Retrieves stock data for specified period.
    \param id The stock identifier for which we're retrieving data.
    \param startTime The start of the time period
    \param endTime The end of the time period
    \param data [out] The array into which we're retrieving the data
    \retval true Success
    \retval false Failure
  */
  bool retrieve(const StockID& id,
                const StockTime& startTime,
                const StockTime& endTime,
                StockInfo& info,
                RangeData& data);


  //! Returns the operation context for this object
  Context& getContext()
  {
    return m_ctx;
  }

 private:

  //! Cache directory for stock data
  std::string m_cacheDirectory;
  
  //! Context for operational messages
  Context& m_ctx;


  /*!
    \brief Retrieves data from the local filesystem
    \param id The stock identifier for which we're retrieving data.
    \param startTime The start of the time period
    \param endTime The end of the time period
    \param data [out] The array into which we're retrieving the data
    \param retval [out] true if data was returned, false otherwise
    \retval true Operation "succeeded" and no further processing should be done
    \retval false Operation did not return data but we should try other options
  */    
  bool retrieveFileData(const StockID& id,
                        const StockTime& startTime,
                        const StockTime& endTime,
                        StockInfo& info,
                        RangeData& data,
                        bool& retval);


  /*!
    \brief Retrieves data from yahoo, persisting it on retrieval
    \param id The stock identifier for which we're retrieving data.
    \param startTime The start of the time period
    \param endTime The end of the time period
    \param data [out] The array into which we're retrieving the data
    \param retval [out] true if data was returned, false otherwise
    \retval true Operation "succeeded" and no further processing should be done
    \retval false Operation did not return data but we should try other options
  */    
  bool retrieveYahooData(const StockID& id,
                         const StockTime& startTime,
                         const StockTime& endTime,
                         StockInfo& info,
                         RangeData& data,
                         bool& retval);
};

} // namespace alch

#endif
