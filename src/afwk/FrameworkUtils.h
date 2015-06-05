// -*- C++ -*-

#ifndef INCLUDED_afwk_FrameworkUtils_h
#define INCLUDED_afwk_FrameworkUtils_h

#include "afwk/FrameworkOptions.h"
#include "autil/Context.h"
#include "stockdata/StockTime.h"
#include "stockdata/StockID.h"
#include "stockdata/StockList.h"

#include <vector>
#include <string>
#include <fstream>


namespace alch {

/*!
  \brief Utility methods for framework applications
  \ingroup afwk
*/
namespace FrameworkUtils
{


  

  //! Returns name of option to use for start dates
  const char* const getOptionStart();

  //! Returns name of option to use for enddates
  const char* const getOptionEnd();

  //! Returns name of option to use for durations
  const char* const getOptionDuration();


  /*!
    \brief converts a string entered on command line into a StockTime
    \param str String entered on command line in YYYY-MM-DD format
    
    Needed because boost::posix_time::time_from_string does not handle
    the case where the date does not have HH:MM:SS.
  */
  StockTime stringToTime(const std::string& str);


  /*!
    \brief Returns date range specified by user
    \param vm Variables map for command-line variables
    \param startTime [out] Start time
    \param endTime [out] End time
    \param ctx [out] Operation context
    \param duration Default duration to use
    \retval true Success
    \retval false Error

    Uses the command-line arguments getOptionStart(), getOptionEnd(), and
    getOptionDuration() from specified VariablesMap to get the values input
    by the user. Populates startTime and endTime accordingly.
  */
  bool getDateRange(FrameworkOptions::VariablesMap& vm,
                    StockTime& startTime,
                    StockTime& endTime,
                    Context& ctx,
                    int duration = 90);

  /*!
    \brief Gets list of stock IDs from specified file
    \param readList [in] List of stock IDs
    \param file File to read
    \param item Name of item (for messages
    \retval true Success
    \retval false Error
  */
  bool getList(StockList& readList,
      const std::string& file,
      Context& ctx);


} // namespace FrameworkUtils

} // namespace alch

#endif
