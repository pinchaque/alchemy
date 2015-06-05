// -*- C++ -*-

#ifndef INCLUDED_stockdata_StockMetaData_h
#define INCLUDED_stockdata_StockMetaData_h

#include "stockdata/StockInfo.h"
#include "stockdata/StockTime.h"

namespace alch {

/*!
  \brief Metadata for stocks data files that have been saved
  \ingroup stockdata
*/
struct StockMetaData
{
 public:


  //! The Info of the stock
  StockInfo stockInfo;

  //! Start of saved date range
  StockTime start;

  //! End of saved date range
  StockTime end;

  /*!
    \brief Constructor
  */
  StockMetaData()
    : stockInfo()
    , start(boost::posix_time::second_clock::local_time())
    , end(boost::posix_time::second_clock::local_time())
  {
    ;
  }
};

} // namespace alch

#endif
