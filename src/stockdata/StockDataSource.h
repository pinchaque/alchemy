// -*- C++ -*-

#ifndef INCLUDED_stockdata_StockDataSource_h
#define INCLUDED_stockdata_StockDataSource_h

#include "autil/Context.h"
#include "stockdata/RangeData.h"
#include "stockdata/StockTime.h"
#include "stockdata/StockID.h"
#include "stockdata/StockInfo.h"

#include <vector>
#include <string>

namespace alch {

/*!
  \brief Interface class for classes that provide access to stock data.
  \ingroup stockdata

  This class provides interfaces for listing all the symbols available.

  Once the caller has the symbol information, they can
  use the retrieve() method to get back data for that symbol.

  Methods are also provided if the caller just wants to check if a given 
  symbol exists in this data source, or if it contains data
  for a particular date range.
*/
class StockDataSource
{
 public:

  //! Constructor
  StockDataSource(Context& ctx)
    : m_ctx(ctx)
  {
    ;
  }

  //! Destructor
  virtual ~StockDataSource()
  {
    ;
  }


  //! Returns the operation context for this object
  Context& getContext()
  {
    return m_ctx;
  }


  /*!
    \brief Returns list of stock symbols for this data source
    \param stockList [out] List of symbols (populated during execution)
    \retval true Success
    \retval false Error

    stockList is only appended to (not cleared).
  */
  virtual bool getStockList(std::vector<StockID>& stockList) = 0;


  /*!
    \brief Checks to see if the specified StockID exists in this data source.
    \param id The stock to look for
    \retval true id has at least one data point in this data source
    \retval false id does not have any data points in this data source

    The behavior of this method should be equivalent to calling retrieve()
    and checking if data is empty. The implementation may be quicker, however.
  */
  virtual bool hasSymbol(const StockID& id) = 0;


  /*!
    \brief Retrieves all data for the specified StockID
    \param id The stock to retrieve date for
    \param data [out] The array into which we're retrieving the data
    \retval true Success
    \retval false Error

    Note that it is not an error condition if this id doesn't exist.
    In this case, data will not be modified.

    data is only appended to (not cleared).
  */
  virtual bool retrieve(const StockID& id, StockInfo& info, RangeData& data) = 0;


  /*!
    \brief Checks to see if the specified StockID has any data over the
    specified date range.
    \param id The stock to look for
    \param start The starting date in the date range
    \param end The ending date in the date range
    \retval true id has at least one data point in this date range
    \retval false id does not have any data points in this date range

    The behavior of this method should be equivalent to calling retrieveDate()
    and checking if data is empty. The implementation may be quicker, however.
  */
  virtual bool hasSymbolDate(const StockID& id, 
                             const StockTime& start,
                             const StockTime& end) = 0;


  /*!
    \brief Retrieves all data for the specified StockID over the
    specified date range.
    \param id The stock to retrieve date for
    \param start The starting date in the date range
    \param end The ending date in the date range
    \param data [out] The array into which we're retrieving the data
    \retval true Success
    \retval false Error

    Note that it is not an error condition if this id doesn't exist.
    In this case, data will not be modified.

    data is only appended to (not cleared).
  */
  virtual bool retrieveDate(const StockID& id, 
                            const StockTime& start,
                            const StockTime& end,
                            StockInfo& info,
                            RangeData& data) = 0;


  /*!
    \brief Saves data into the file data source
    \param id The stock id that we're saving data for
    \param start The starting date covered by this saved data
    \param end  The ending date covered by this saved data
    \param data The data we're saving
    \retval true Success
    \retval false Error
  
    This method will save the specified data into this data source. Not
    all data sources will be able to implement this method, so the supplied
    definition will always return false (error). Note that the caller must
    supply the start-end range for which this data applies. This is so that
    the save() implementation can effectively determine if it has any gaps
    in data. Using the actual data to determine start and end is sub-optimal
    because there may be unavailable data at the beginning or end!
  */
  virtual bool save(const StockID& id, 
                    const StockTime& start,
                    const StockTime& end,
                    const StockInfo& info,
                    const RangeData& data)
  {
    return false;
  }


  /*!
    \brief Removes data for the specified stock id
    \param id The stock id that we're removing the data for
    \retval true Success
    \retval false Error
  
    This method will remove all data associated with the specified stock id.
    Not all data sources will be able to do this, and the ones that can't
    will return false. Note it is not an error if the specified ID does
    not exist.
  */
  virtual bool remove(const StockID& id)
  {
    return false;
  }


 private:
  
  //! Context for operational messages
  Context& m_ctx;
};

} // namespace alch

#endif
