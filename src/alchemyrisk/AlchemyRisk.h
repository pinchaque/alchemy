// -*- C++ -*-

#ifndef INCLUDED_alchemyrisk_AlchemyRisk_h
#define INCLUDED_alchemyrisk_AlchemyRisk_h

#include "afwk/Framework.h"

#include "autil/CSVData.h"
#include "stockdata/StockID.h"
#include "stockdata/StockInfo.h"
#include "stockdata/StockList.h"
#include "stockdata/RangeData.h"
#include "stockdata/MetaFileStockDataSource.h"
#include "stockalg/LogNormal3.h"
#include "stockalg/BenchmarkPerformance.h"

#include <vector>
#include <string>
#include <map>
#include <iosfwd>

namespace alch {

/*!
  \brief Runs the AlchemyRisk application
  \ingroup alchemyrisk
*/
class AlchemyRisk : public Framework
{
 public:

  /*!
    \brief Constructor
  */
  AlchemyRisk();

  virtual ~AlchemyRisk();

protected:

  std::string getApplicationName() const
  {
    return "alchemyrisk";
  }

  std::string getApplicationVersion() const
  {
    return VERSIONSTRING;
  }

  std::string getApplicationDescription() const;

  virtual bool initialize();

  virtual bool finalize();

  virtual Framework::OptionsReturnCode processOptions(int argc, char** argv);

  virtual bool processApplication();

 private:

  static const char* const s_optionFile;
  static const char* const s_optionBenchmarks;
  static const char* const s_optionMAR;
  static const char* const s_optionStart;
  static const char* const s_optionEnd;
  static const char* const s_optionOut;
  static const char* const s_optionBootstrap;
  static const char* const s_optionPlot;

  StockTime m_startTime;
  StockTime m_endTime;
  StockList m_symbolList;
  StockList m_benchmarkList;
  double m_mar;
  std::string m_outFile;
  bool m_bootstrap;
  bool m_plot;
  std::map<std::string, BenchmarkPerformancePtr> m_benchmarks;
  typedef std::map<std::string, RangeDataPtr> DataCache;
  typedef std::map<std::string, StockInfoPtr> InfoCache;
  DataCache m_dataCache;
  InfoCache m_infoCache;

  bool setDateRange();
  
  /*!
    Retrieves data for specified stock, returning RangeDataPtr() on error.
   */
  RangeDataPtr retrieveData(const StockID& id);

  /*!
    \brief Reads in specified symbol list file
    \param symbolList [out] The symbol list that was read
    \param symbolFile The name of the file to read from
    \retval true Success
    \retval false Error

    Lines beginning with # are ignored as comments
    List of symbols can be on separate lines or all on same line.
  */
  bool getSymbolList(const std::string& symbolFile, StockList& symbolList);

  /*!
    \brief Processes symbols for the application
    \retval true Success
    \retval false Error
   */
  bool processSymbols();

  /*!
    \brief Performs computation for a single symbol
    \param symbol The symbol for which data will be downloaded
    \retval true Success
    \retval false Error
  */
  bool processSymbol(const StockID& symbol, CSVData::Row& row);

  void plotSymbol(
      const StockID& symbol,
      const std::vector<double>& vecReturns,
      const std::vector<double>& bootstrapReturns,
      const LogNormal3& ln3);

  /*!
   Calculates the correlation between our symbol and benchmark lists, and
   stores the best benchmark for each symbol.
   */
  bool discoverBenchmarks();

  // gets info for a particular stock from our cache, creating an empty
  // info object if none
  StockInfoPtr getStockInfo(const StockID& id);

};

} // namespace alch

#endif
