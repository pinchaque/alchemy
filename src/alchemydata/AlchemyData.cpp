#include "alchemydata/AlchemyData.h"

#include "afwk/PathRegistry.h"

#include "stockdata/MetaFileStockDataSource.h"
#include "stockdata/YahooStockDataSource.h"
#include "stockdata/StockTime.h"
#include "stockdata/StockTimeUtil.h"
#include "stockdata/StockInfo.h"

#include "boost/tokenizer.hpp"
#include "boost/filesystem/operations.hpp"

#include <vector>
#include <string>
#include <fstream>

namespace alch {

  const char* const AlchemyData::s_optionSymbol = "symbol";
  const char* const AlchemyData::s_optionFile = "file";

  AlchemyData::AlchemyData()
    : Framework()
  {
    ;
  }


  AlchemyData::~AlchemyData()
  {
    ;
  }


  std::string AlchemyData::getApplicationDescription() const
  {
    std::stringstream ss;
    ss <<
      "This application is responsible for fetching data for all or \n"
      "specified symbols. If no symbols are specified via the \n"
      << s_optionSymbol << " or " << s_optionFile << " arguments, the file\n" 
      << PathRegistry::getSymbolFile() <<
      "\nwill be used. If that doesn't exist, then all symbols will be \n"
      "downloaded. Data is stored in the directory\n"
       << PathRegistry::getDataDir() << "/.\n"
      ;

    return ss.str();
  }

  bool AlchemyData::initialize()
  {
    return Framework::initialize();
  }

  bool AlchemyData::finalize()
  {
    return Framework::finalize();
  }

  Framework::OptionsReturnCode AlchemyData::processOptions(
    int argc, char** argv)
  {
    std::stringstream ssSymbol;
    ssSymbol << s_optionSymbol << "," << s_optionSymbol[0];

    getOptions().getGenericOptions().add_options()
      (ssSymbol.str().c_str(), 
       boost::program_options::value<std::string>(),
       "Specifies the symbol to read")
      ;

    std::stringstream ssFile;
    ssFile << s_optionFile << "," << s_optionFile[0];

    getOptions().getGenericOptions().add_options()
      (ssFile.str().c_str(), 
       boost::program_options::value<std::string>(),
       "File of symbols to read")
      ;

    return Framework::processOptions(argc, argv);
  }


  bool AlchemyData::processApplication()
  {
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();

    if (vm.count(s_optionSymbol))
    {
      std::string symbol(vm[s_optionSymbol].as<std::string>());
      
      getContext() << Context::PRIORITY_debug1
                   << "User --symbol=" << symbol
                   << Context::endl;
      
      return processSymbol(symbol);
    }
    else
    {
      std::string symbolFile = PathRegistry::getSymbolFile();
      if (vm.count(s_optionFile))
      {
        symbolFile = vm[s_optionFile].as<std::string>();
      }

      return processSymbolList(symbolFile);
    }
  }


  bool AlchemyData::getList(VecStockID& readList,
                            const std::string& file,
                            const std::string& item)
  {
    std::ifstream ifs(file.c_str());
    if (!ifs)
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open file '" << file << "'"
                   << Context::endl;
      return false;
    }

    getContext() << Context::PRIORITY_debug2
                 << "Reading " << item << " file '" << file << "'"
                 << Context::endl;

    std::string line;

    while (std::getline(ifs, line))
    {
      // skip comment lines
      if (line.size() && (line[0] == '#'))
      {
        continue;
      }

      // tokenize line and add it to readList
      typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
      boost::char_separator<char> sep(" \t\r\n,");
      tokenizer tok(line, sep);
      tokenizer::iterator end = tok.end();
      tokenizer::iterator iter;

      for (iter = tok.begin(); iter != end; ++iter)
      {
        readList.push_back(*iter);

        getContext() << Context::PRIORITY_debug2
                     << "Read " << item << ": '" << *iter << "'"
                     << Context::endl;
      }
    }

    ifs.close();

    getContext() << Context::PRIORITY_debug1
                 << "Read " << readList.size()
                 << " " << item << "(s) from file '"
                 << file << "'" << Context::endl;

    return true;
  }


  bool AlchemyData::getSymbolList(VecStockID& symbolList,
                                  const std::string& symbolFile)
  {
    return getList(symbolList, symbolFile, "symbol");
  }


  bool AlchemyData::processSymbolList(const std::string& symbolFile)
  {
    VecStockID symbolList;

    boost::filesystem::path symbolFilePath(symbolFile,
                                           boost::filesystem::native);

    if (boost::filesystem::exists(symbolFilePath))
    {
      if (!getSymbolList(symbolList, symbolFile))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to get symbol list from '" << symbolFile << "'"
                     << Context::endl;
        return false;
      }

      getContext() << Context::PRIORITY_info
                   << "Using list of symbols from '" << symbolFile << "'"
                   << Context::endl;
    }
    else
    {
      getContext() << Context::PRIORITY_debug1
                   << "Attempting to get symbols from Yahoo..."
                   << Context::endl;

      YahooStockDataSource yahoo(getContext());

      if (!yahoo.getStockList(symbolList))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to get symbol list from Yahoo"
                     << Context::endl;
        return false;
      }

      getContext() << Context::PRIORITY_info
                   << "Using list of symbols from Yahoo"
                   << Context::endl;
    }

    // empty symbol lists are suspicious, but not necessarily an error
    if (symbolList.empty())
    {
      getContext() << Context::PRIORITY_warning
                   << "Symbol list was empty"
                   << Context::endl;
      return true;
    }

    getContext() << Context::PRIORITY_info
                 << "Processing " << symbolList.size() << " symbols"
                 << Context::endl;

    

    // download data for all symbols

    VecStockID::const_iterator end = symbolList.end();
    VecStockID::const_iterator iter;
    for (iter = symbolList.begin(); iter != end; ++iter)
    {
      if (!processSymbol(*iter))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to process symbol '" << *iter
                     << "'. Continuing execution."
                     << Context::endl;
        continue;
      }
    }

    return true;
  }

    

  bool AlchemyData::processSymbol(const StockID& symbol)
  {
    getContext() << Context::PRIORITY_info
                 << "Processing " << symbol << "..."
                 << Context::endl;

    // default to getting all data
    StockTime startTime(boost::posix_time::min_date_time);
    StockTime endTime(boost::posix_time::second_clock::local_time());

    // back up to previous close time
    endTime = StockTimeUtil::getPreviousClose(endTime);

    // if we have a meta file for this symbol already, then we only want
    // to retrieve data starting the day after
    MetaFileStockDataSource fileDataSource(PathRegistry::getDataDir(),
                                           getContext());
    StockMetaData metaData;
    if (fileDataSource.readMetaFile(symbol, metaData))
    {
      // get data starting at appropriate time on day after than what we have
      startTime = StockTimeUtil::getNextClose(metaData.end);

      getContext() << Context::PRIORITY_debug1
                   << "Meta data file exists and has date range "
                   << metaData.start << " - " << metaData.end
                   << Context::endl;
    }
    else
    {
      getContext() << Context::PRIORITY_debug1
                   << "Meta data file does not exist "
                   << Context::endl;
    }

    // startTime is the next time at which we should download after the meta
    // file end date. endTime is our current time.
    if (startTime > endTime)
    {
      getContext() << Context::PRIORITY_info
                   << "Data for '" << symbol << "' is current (end date "
                   << metaData.end << ")"
                   << Context::endl;

      getContext() << Context::PRIORITY_debug1
                   << "next: " << startTime << " > current: " << endTime
                   << Context::endl;

      return true;
    }

    getContext() << Context::PRIORITY_debug1
                 << "Retrieving date range "
                 << startTime << " - " << endTime
                 << Context::endl;


    // get the requested date range from Yahoo
    YahooStockDataSource yahoo(getContext());
    RangeData yahooData;
    StockInfo info;
    if (!yahoo.retrieveDate(symbol, startTime, endTime, info, yahooData))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while retrieving data from Yahoo for '"
                   << symbol << "'" << Context::endl;
      return false;
    }

    // save this data to the stock data file
    if (!persistData(symbol, yahooData, startTime, endTime, info, fileDataSource))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while retrieving data from Yahoo for '"
                   << symbol << "'" << Context::endl;
      return false;
    }

    return true;
  }

  bool AlchemyData::persistData(const StockID& id,
                                const RangeData& data,
                                const StockTime& startTime,
                                const StockTime& endTime,
                                const StockInfo& info,
                                MetaFileStockDataSource& dataSource)
  {
    // nothing to persist
    if (!data.size())
    {
      return true;
    }

    // put the data into the file
    std::string dataDir = PathRegistry::getDataDir();

    getContext() << Context::PRIORITY_debug2
                 << "Writing data for '" << id << "'" << Context::endl;
    getContext() << Context::PRIORITY_debug2
                 << "--- Dates "
                 << boost::posix_time::to_simple_string(startTime)
                 << " to "
                 << boost::posix_time::to_simple_string(endTime)
                 << Context::endl;

    getContext() << Context::PRIORITY_debug2
                 << "Saving " << data.size() << " data records to file '"
                 << FileStockDataSource::getSymbolFile(dataDir,
                                                       id.getSymbol())
                 << "'" << Context::endl;

    if (!dataSource.save(id, startTime, endTime, info, data))
    {
      getContext() << Context::PRIORITY_error
                   << "Error while trying to save data to '"
                   << FileStockDataSource::getSymbolFile(dataDir,
                                                         id.getSymbol())
                   << "'" << Context::endl;

      return false;
    }

    return true;
  }

} // namespace alch
