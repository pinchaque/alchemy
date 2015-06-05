// -*- C++ -*-

#ifndef INCLUDED_stockdata_YahooStockDataSource_h
#define INCLUDED_stockdata_YahooStockDataSource_h

#include "stockdata/StockDataSource.h"

#include <string>

namespace alch {


/*!
  \brief Provides access to stock data read from finance.yahoo.com
  \ingroup stockdata
*/
class YahooStockDataSource : public StockDataSource
{
 public:

  //! Constructor
  YahooStockDataSource(Context& ctx);

  //! Destructor
  virtual ~YahooStockDataSource();

  virtual bool getStockList(std::vector<StockID>& stockList);

  virtual bool hasSymbol(const StockID& id);

  virtual bool retrieve(const StockID& id, StockInfo& info, RangeData& data);

  virtual bool hasSymbolDate(const StockID& id, 
                             const StockTime& start,
                             const StockTime& end);

  virtual bool retrieveDate(const StockID& id, 
                            const StockTime& start,
                            const StockTime& end,
                            StockInfo& info,
                            RangeData& data);

 private:

  //! Name of application used to fetch the data
  static const char* const s_fetchProgram;

  //! Name of application used to list stock symbols
  static const char* const s_listProgram;

  //! Name of application used to get info about a symbol
  static const char* const s_infoProgram;



  /*!
    \brief Runs the fetch program and returns the results
    \param progName The name of the program to run
    \param args The command-line arguments to pass
    \param results [out] The place where the result lines are stored
    \retval true Success
    \retval false Error

    This method will run the s_fetchProgram application and pass it args.
    Each line of output will be stripped of its newline character. If the
    line is not empty, it will be appended to results.
  */
  bool runFetchProgram(const std::string& progName,
                       const std::string& args,
                       std::vector<std::string>& results);

  bool processInfoLine(const std::string& str, StockInfo& info) const;

  /*!
    \brief Parses results returned from Yahoo and adds them to range data
    \param results The data we got from Yahoo
    \param data [out] Where we stored the parse stock data
    \retval true Success
    \retval false Error

    If results contains unparseable lines, warnings are added to the context
    but parsing continues. Method will return false only if a fatal error
    occurs.
  */
  bool processData(const std::vector<std::string>& results, RangeData& data);

  /*!
   \brief Fills out the info object with data for this stock
   */
  bool processInfo(const StockID& id, StockInfo& info);

  /*!
    \brief Returns list of exchanges
    \param exchangeList [out] List of exchanges
    \retval true Success
    \retval false Error
  */
  bool getExchangeList(std::vector<std::string>& exchangeList);

};

} // namespace alch

#endif
