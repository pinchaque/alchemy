// -*- C++ -*-

#ifndef INCLUDED_stockdata_StockTimeUtil_h
#define INCLUDED_stockdata_StockTimeUtil_h

#include "stockdata/StockTime.h"


namespace alch {

/*!
  \brief Utility functions for dealing with StockTime objects
  \ingroup stockdata
*/
namespace StockTimeUtil
{


  /*!
    \brief Returns next closing time after given time
    \param stockTime Given time
    \return Next closing time after stockTime
  */
  StockTime getNextClose(const StockTime& stockTime);
  

  /*!
    \brief Returns previous closing time before given time
    \param stockTime Given time
    \return Previous closing time after stockTime
  */
  StockTime getPreviousClose(const StockTime& stockTime);

}

} // namespace alch

#endif
