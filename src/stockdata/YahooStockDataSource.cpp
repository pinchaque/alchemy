
#include "stockdata/YahooStockDataSource.h"
#include <boost/tokenizer.hpp>
#include <sstream>
#include <stdio.h>

namespace alch {

  namespace { // anonymous

    void stockTimeToDate(const StockTime& stockTime, std::string& str)
    {

      boost::gregorian::date date = stockTime.date();

      std::stringstream ss;
      ss << std::setw(4) << std::setfill('0') << date.year()
         << "-"
         << std::setw(2) << std::setfill('0') << date.month()
         << "-"
         << std::setw(2) << std::setfill('0') << date.day();

      str = ss.str();
    }

    // parses a date in ISO format from str, returning false on error
    bool parseDate(const std::string& str, StockTime& stockTime)
    {
      try
      {
        StockTime tmpStockTime =
          boost::posix_time::from_iso_string(str);
    
        if (tmpStockTime == boost::posix_time::not_a_date_time)
        {
          return false;
        }
        else
        {
          stockTime = tmpStockTime;
          return true;
        }
      }
      catch (...)
      {
        return false;
      }
    }

    // parses a double from str, returning false on error
    bool parseDouble(const std::string& str, double& d)
    {
      std::istringstream ss(str);
      ss >> d;
      return ss;
    }

    /*!
      \brief Processes a line of stock data from Yahoo
      \param line The line to process
      \param [out] dataPoint Where we store the results
      \retval true Line successfully parsed
      \retval false Line parsing failed

      Lines of the following form are parsed:
      Stock output is "Date,Open,High,Low,Close,Volume"
      Funds output is "Date,Close". Date are "mm-dd-yyyy"
    */
    bool processLine(const std::string& line, RangeData::Point& dataPoint)
    {
      // parse line based on commas
      boost::tokenizer<boost::escaped_list_separator<char> > tok(line);

      boost::tokenizer<boost::escaped_list_separator<char> >::iterator end
        = tok.end();
      boost::tokenizer<boost::escaped_list_separator<char> >::iterator iter
        = tok.begin();


      // we have either:
      // 2 tokens (for funds) 
      // 6 tokens (for stocks without adjusted close)
      // 7 tokens (for stocks with adjusted close)
      int numTok = int(std::distance(iter, end));
      if ((numTok != 2) && (numTok != 6) && (numTok != 7))
      {
        return false;
      }

      assert(iter != end);
      if (!parseDate(*iter++, dataPoint.tradeTime))
      {
        return false;
      }

      assert(iter != end);
      if (!parseDouble(*iter++, dataPoint.open))
      {
        return false;
      }

      // if iter has hit end, then we only had two tokens. That second
      // token should have been the fund close value. What we do for 
      // consistency is to make open/close/min/max all the same anyway so
      // it doesn't really matter where we stored it!
      if (iter == end)
      {
        dataPoint.max = dataPoint.open;
        dataPoint.min = dataPoint.open;
        dataPoint.close = dataPoint.open;
        dataPoint.adjustedClose = dataPoint.open;
        dataPoint.volume = 0.00;
        return true;
      }

      assert(iter != end);
      if (!parseDouble(*iter++, dataPoint.max))
      {
        return false;
      }

      assert(iter != end);
      if (!parseDouble(*iter++, dataPoint.min))
      {
        return false;
      }

      assert(iter != end);
      if (!parseDouble(*iter++, dataPoint.close))
      {
        return false;
      }

      assert(iter != end);
      if (!parseDouble(*iter++, dataPoint.volume))
      {
        return false;
      }

      if (iter != end)
      {
        if (!parseDouble(*iter++, dataPoint.adjustedClose))
        {
          return false;
        }
      }
      else
      {
        dataPoint.adjustedClose = dataPoint.close;
      }

      assert(iter == end);

      return true;
    }

    double string2double(const std::string& str)
    {
      double d = 0.0;
      std::istringstream iss(str);
      iss >> d;
      return d;
    }

    int string2int(const std::string& str)
    {
      int i = 0;
      std::istringstream iss(str);
      iss >> i;
      return i;
    }


  } // anonymous namespace



  const char* const YahooStockDataSource::s_fetchProgram = "yahooquote.pl";
  const char* const YahooStockDataSource::s_infoProgram = "yahooinfo.pl";
  const char* const YahooStockDataSource::s_listProgram = "yahoolist.pl";


  YahooStockDataSource::YahooStockDataSource(Context& ctx)
    : StockDataSource(ctx)
  {
  }


  YahooStockDataSource::~YahooStockDataSource()
  {
  }



  bool YahooStockDataSource::getStockList(std::vector<StockID>& stockList)
  {
    std::vector<std::string> results;
    if (!runFetchProgram(s_listProgram, "", results))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while running list program " << s_listProgram
                   << Context::endl;
      return false;
    }

    // each line in results is a stock name
    stockList.insert(stockList.end(), results.begin(), results.end());

    return true;
  }


  bool YahooStockDataSource::hasSymbol(const StockID& id)
  {
    if (!id.getSymbol().length())
    {
      return false;
    }

    RangeData data;
    StockInfo info;

    return (retrieve(id, info, data) && data.size());
  }


  bool YahooStockDataSource::retrieve(const StockID& id, StockInfo& info, 
      RangeData& data)
  {
    StockTime start(boost::posix_time::from_iso_string("19000101T000000"));
    StockTime end(boost::posix_time::second_clock::local_time()
                  + boost::posix_time::hours(24));

    return retrieveDate(id, start, end, info, data);
  }


  bool YahooStockDataSource::hasSymbolDate(const StockID& id, 
                                           const StockTime& start,
                                           const StockTime& end)
  {
    RangeData data;
    StockInfo info;

    return (retrieveDate(id, start, end, info, data) && data.size());
  }


  bool YahooStockDataSource::processInfoLine(const std::string& str,
      StockInfo& info) const
  {
    // ignore blank lines
    if (!str.length())
    {
      return true;
    }

    // ignore lines with no value
    std::string::size_type spacePos = str.find(' ');
    if (spacePos == std::string::npos)
    {
      return true;
    }

    std::string tag = str.substr(0, spacePos);
    std::string value = str.substr(spacePos + 1);

    if (!value.length())
    {
      return true;
    }

    if (tag == "ExpenseRatio")
    {
      info.setExpenseRatio(string2double(value));
    }
    else if (tag == "Max12b1Fee")
    {
      info.set12b1Fee(string2double(value));
    }
    else if (tag == "Family")
    {
      info.setFamily(value);
    }
    else if (tag == "Rating")
    {
      info.setRating(string2int(value));
    }
    else if (tag == "Inception")
    {
      StockTime t;
      if (parseDate(value, t))
      {
        info.setInception(t);
      }
    }
    else if (tag == "InitInvestment")
    {
      info.setInitInvestment(string2int(value));
    }
    else if (tag == "Assets")
    {
      info.setAssets(string2int(value));
    }
    else if (tag == "Turnover")
    {
      info.setTurnover(string2double(value));
    }
    else if (tag == "SubsInvestment")
    {
      info.setSubsInvestment(string2int(value));
    }
    else if (tag == "Yield")
    {
      info.setYield(string2double(value));
    }
    else if (tag == "DeferredLoad")
    {
      info.setDeferredLoad(string2double(value));
    }
    else if (tag == "FrontLoad")
    {
      info.setFrontLoad(string2double(value));
    }
    else if (tag == "Category")
    {
      info.setCategory(value);
    }
    else if (tag == "Name")
    {
      info.setName(value);
    }

    return true;
  }

  bool YahooStockDataSource::processInfo(const StockID& id, 
                                         StockInfo& info)
  {
    std::vector<std::string> results;
    if (!runFetchProgram(s_infoProgram, id.getSymbol(), results))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while running info program " << s_infoProgram
                   << Context::endl;
      return false;
    }

    
    std::vector<std::string>::const_iterator iter;
    std::vector<std::string>::const_iterator end = results.end();
    for (iter = results.begin(); iter != end; ++iter)
    {
      if (!processInfoLine(*iter, info))
      {
        getContext() << Context::PRIORITY_error
                   << "Invalid line from yahoo info: '"
                   << info << "'" << Context::endl;
        return false;
      }
      else
      {
        getContext() << Context::PRIORITY_debug2
                   << "Processed info line '" << info << "'" << Context::endl;
      }
    }

    return true;
  }

  bool YahooStockDataSource::retrieveDate(const StockID& id, 
                                          const StockTime& start,
                                          const StockTime& end,
                                          StockInfo& info,
                                          RangeData& data)
  {
    // at least set up the ID for the info
    info.setID(id);

    if (!processInfo(id, info))
    {
      getContext() << Context::PRIORITY_warning
                   << "Failed to fill StockInfo for Yahoo id " << id
                   << Context::endl;
    }

    std::string startString("");
    stockTimeToDate(start, startString);

    std::string endString("");
    stockTimeToDate(end, endString);

    // fetch daily adjusted data
    std::stringstream ss;
    ss << startString << " " << endString << " " << id.getSymbol();

    std::vector<std::string> results;
    if (!runFetchProgram(s_fetchProgram, ss.str(), results))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while running fetch program " << s_fetchProgram
                   << Context::endl;
      return false;
    }

    // process the data we got back, adding it to rangedata
    return processData(results, data);
  }


  bool YahooStockDataSource::runFetchProgram(const std::string& progName,
                                             const std::string& args,
                                             std::vector<std::string>& results)
  {
    std::stringstream ss;
    ss << progName << " " << args << " 2>&1";
    FILE *pipe = popen(ss.str().c_str(), "r");

    if (!pipe)
    {
      getContext() << Context::PRIORITY_error
                   << "Error while starting command '" << ss.str() << "'"
                   << Context::endl;
      return false;
    }

    const int bufSize = 2048;
    char buf[bufSize] = "\0";

    while (fgets(buf, bufSize, pipe))
    {
      // strip off newline
      char* p = strchr(buf, '\n');
      if (p)
      {
        *p = '\0';
      }

      if (!strncmp(buf, "Error", 5))
      {
        getContext() << Context::PRIORITY_error << "Error reported by "
                     << progName << ": '" << buf << "'"
                     << Context::endl;
      }
      // if there's anything left on the line, then we return it
      else if (buf[0])
      {
        results.push_back(buf);
      }
    }

    if (pclose(pipe) == -1)
    {
      getContext() << Context::PRIORITY_error
                   << "Error during pclose() with errno = " << errno
                   << Context::endl;
      return false;
    }
    else
    {
      return true;
    }
  }


  bool YahooStockDataSource::processData(
    const std::vector<std::string>& results,
    RangeData& data)
  {

    std::vector<std::string>::const_iterator end = results.end();
    std::vector<std::string>::const_iterator iter;
    for (iter = results.begin(); iter != end; ++iter)
    {
      RangeData::Point dataPoint;
      if (processLine(*iter, dataPoint))
      {
        data.add(dataPoint);
      }
      else
      {
        getContext() << Context::PRIORITY_error
                     << "Warning: Failed to parse line from Yahoo: '"
                     << *iter << "'" << Context::endl;
      }
    }

    return true;
  }

#ifdef YAHOO_DYNAMIC_LIST

  bool YahooStockDataSource::getExchangeList(
    std::vector<std::string>& exchangeList)
  {
    std::vector<std::string> results;
    if (!runFetchProgram(s_listProgram, "-c", results))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while running fetch program"
                   << Context::endl;
      return false;
    }

    // each line in results is an exchange name
    exchangeList.insert(exchangeList.end(), results.begin(), results.end());

    return true;
  }
#else

  bool YahooStockDataSource::getExchangeList(
    std::vector<std::string>& exchangeList)
  {
    exchangeList.push_back("US.NASDAQ.FUND");
    exchangeList.push_back("US.NASDAQ.INDEX"); // IXIC (nasdaq)
    exchangeList.push_back("US.NASDAQ.STOCK");
    exchangeList.push_back("US.NYSE.INDEX");
    exchangeList.push_back("US.NYSE.STOCK");
    exchangeList.push_back("US.OTHER.INDEX"); // Russell 2000
    exchangeList.push_back("US.CBOT.INDEX"); // DJI

    return true;
  }
#endif


} // namespace alch
