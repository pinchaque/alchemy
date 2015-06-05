#include "alchemyrisk/AlchemyRisk.h"

#include "afwk/PathRegistry.h"
#include "afwk/FrameworkUtils.h"

#include "autil/CSVData.h"
#include "autil/CSVDataStream.h"

#include "nnet/Statistics.h"

#include "stockdata/MetaFileStockDataSource.h"
#include "stockdata/YahooStockDataSource.h"
#include "stockdata/StockTime.h"
#include "stockdata/StockTimeUtil.h"
#include "stockdata/StockDataRetriever.h"
#include "stockdata/RangeDataAlg.h"

#include "stockalg/DownsideRisk.h"
#include "stockalg/LogNormal3.h"
#include "stockalg/Risk.h"
#include "stockalg/Returns.h"

#include "stockplot/CSVPlotDisplay.h"
#include "stockplot/GnuPlotDisplay.h"

#include "boost/tokenizer.hpp"
#include "boost/filesystem/operations.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace alch {

  const char* const AlchemyRisk::s_optionFile = "file";
  const char* const AlchemyRisk::s_optionBenchmarks = "benchmarks";
  const char* const AlchemyRisk::s_optionMAR = "mar";
  const char* const AlchemyRisk::s_optionOut = "out";
  const char* const AlchemyRisk::s_optionBootstrap = "bootstrap";
  const char* const AlchemyRisk::s_optionPlot = "plot";

  AlchemyRisk::AlchemyRisk()
    : Framework()
    , m_startTime(boost::posix_time::second_clock::local_time())
    , m_endTime(boost::posix_time::second_clock::local_time())
    , m_symbolList()
    , m_benchmarkList()
    , m_mar(0.00)
    , m_outFile("output.csv")
    , m_bootstrap(true)
    , m_plot(false)
    , m_benchmarks()
    , m_dataCache()
    , m_infoCache()
  {
    ;
  }


  AlchemyRisk::~AlchemyRisk()
  {
    ;
  }


  std::string AlchemyRisk::getApplicationDescription() const
  {
    std::stringstream ss;
    ss << "todo"
        ;

    return ss.str();
  }

  bool AlchemyRisk::initialize()
  {
    return Framework::initialize();
  }

  bool AlchemyRisk::finalize()
  {
    return Framework::finalize();
  }

  Framework::OptionsReturnCode AlchemyRisk::processOptions(
    int argc, char** argv)
  {

    std::stringstream ssFile;
    ssFile << s_optionFile << "," << s_optionFile[0];
    
    std::stringstream ssOut;
    ssOut << s_optionOut << "," << s_optionOut[0];
    
    std::stringstream ssBootstrap;
    ssBootstrap << s_optionBootstrap << "," << s_optionBootstrap[0];
    
    std::stringstream ssPlot;
    ssPlot << s_optionPlot << "," << s_optionPlot[0];

    getOptions().getGenericOptions().add_options()
      (FrameworkUtils::getOptionStart(),
       boost::program_options::value<std::string>(),
       "Start of date range")
      (FrameworkUtils::getOptionEnd(),
       boost::program_options::value<std::string>(),
       "End of date range")
      (ssFile.str().c_str(), 
       boost::program_options::value<std::string>(),
       "File of symbols to read")
      (s_optionBenchmarks,
       boost::program_options::value<std::string>(),
       "File of benchmark symbols")
      (s_optionMAR,
       boost::program_options::value<double>(),
       "Minimum Acceptable Return (MAR) in percentage points.")
      (ssOut.str().c_str(), 
       boost::program_options::value<std::string>(),
       "Output CSV file name")
      (ssBootstrap.str().c_str(), 
       boost::program_options::value<bool>(),
       "Whether to bootstrap the data")
      (ssPlot.str().c_str(), 
       boost::program_options::value<bool>(),
       "Whether to plot the data")
      ;

    return Framework::processOptions(argc, argv);
  }


  bool AlchemyRisk::processApplication()
  {
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();

    if (!vm.count(s_optionFile))
    {
      getContext() << Context::PRIORITY_error
                   << "Need to specify symbol file"
                   << Context::endl;
      return false;
    }

    if (!vm.count(s_optionBenchmarks))
    {
      getContext() << Context::PRIORITY_error
                   << "Need to specify benchmark file"
                   << Context::endl;
      return false;
    }

    if (!setDateRange())
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to set date range"
                   << Context::endl;
      return false;
    }

    if (vm.count(s_optionOut))
    {
      m_outFile = vm[s_optionOut].as<std::string>();
    }

    if (vm.count(s_optionBootstrap))
    {
      m_bootstrap = vm[s_optionBootstrap].as<bool>();
    }

    if (vm.count(s_optionPlot))
    {
      m_plot = vm[s_optionPlot].as<bool>();
    }

    if (vm.count(s_optionMAR))
    {
      m_mar = vm[s_optionMAR].as<double>() / 100.0;
    }

    std::string symbolFile = vm[s_optionFile].as<std::string>();
    if (!getSymbolList(symbolFile, m_symbolList))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to get symbol list"
                   << Context::endl;
      return false;
    }

    std::string benchmarkFile = vm[s_optionBenchmarks].as<std::string>();
    if (!getSymbolList(benchmarkFile, m_benchmarkList))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to get benchmark list"
                   << Context::endl;
      return false;
    }

    if (!discoverBenchmarks())
    {
      getContext() << Context::PRIORITY_error
                   << "Failed benchmark discovery"
                   << Context::endl;
      return false;
    }

    if (!processSymbols())
    {
      getContext() << Context::PRIORITY_error
                   << "Failed risk calculations"
                   << Context::endl;
      return false;
    }

    return true;
  }

  bool AlchemyRisk::setDateRange()
  {
    // get the date range, default to 5 years
    FrameworkOptions::VariablesMap& vm = getOptions().getVariablesMap();
    if (!FrameworkUtils::getDateRange(vm, m_startTime, m_endTime,
                                      getContext(), 365*5))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to get date range"
                   << Context::endl;
      return false;
    }

    // adjust so we are only looking at whole months
    // start time should be the first day of a month. Back it up to the first 
    // day of the month that it's in.
    StockTime newStart(
        boost::gregorian::date(
          m_startTime.date().year(),
          m_startTime.date().month(),
          1), 
        boost::posix_time::hours(0));
    m_startTime = newStart;

    // End time should be the last day of the last month. Move it back to the
    // first day of this month, then back it up one day.
    StockTime newEnd(
        boost::gregorian::date(
          m_endTime.date().year(),
          m_endTime.date().month(),
          1), 
        boost::posix_time::hours(23)
        + boost::posix_time::minutes(59)
        + boost::posix_time::seconds(59)
        );
    newEnd -= boost::gregorian::date_duration(1);
    m_endTime = newEnd;


    getContext() << Context::PRIORITY_debug1
      << "New start date: " 
      << boost::posix_time::to_simple_string(m_startTime)
      << Context::endl;

    getContext() << Context::PRIORITY_debug1
      << "New end date: " 
      << boost::posix_time::to_simple_string(m_endTime)
      << Context::endl;

    return true;
  }

  RangeDataPtr AlchemyRisk::retrieveData(const StockID& id)
  {
    DataCache::iterator iter = m_dataCache.find(id.getSymbol());
    if (iter != m_dataCache.end())
    {
      getContext() << Context::PRIORITY_debug2
                   << "Using cached data for " << id
                   << Context::endl;
      return iter->second;
    }

    StockDataRetriever retriever(PathRegistry::getDataDir(), getContext());
    RangeDataPtr data(new RangeData);
    StockInfoPtr stockInfo(new StockInfo);

    if (!retriever.retrieve(id, m_startTime, m_endTime, *stockInfo, *data))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to retrieve data for " << id
                   << Context::endl;
      return RangeDataPtr();
    }
    else if (!data->size())
    {
      getContext() << Context::PRIORITY_error
                   << "No data returned for " << id
                   << Context::endl;
      return RangeDataPtr();
    }

    getContext() << Context::PRIORITY_debug1
                 << "Retrieved " << data->size() << " data records"
                 << Context::endl;

    // account for adjusted data
    getContext() << Context::PRIORITY_debug1
      << "Using adjusted data" << Context::endl;
    data->useAdjusted();

    // now create our monthly returns
    RangeDataPtr monthlyData(new RangeData);
    data->summarizeMonthly(*monthlyData);

    getContext() << Context::PRIORITY_debug1
                 << "Retrieved " << monthlyData->size() 
                 << " monthly data records"
                 << Context::endl;

    // save data to cache
    m_dataCache[id.getSymbol()] = monthlyData;
    m_infoCache[id.getSymbol()] = stockInfo;

    return monthlyData;
  }


  bool AlchemyRisk::getSymbolList(
      const std::string& symbolFile, 
      StockList& symbolList)
  {
    symbolList.clear();

    if (!FrameworkUtils::getList(symbolList, symbolFile, getContext()))
    {
      return false;
    }
    else if (symbolList.empty())
    {
      getContext() << Context::PRIORITY_warning
                   << "Symbol list was empty"
                   << Context::endl;
      return false;
    }

    return true;
  }


  bool AlchemyRisk::processSymbols()
  { 
    // write CSV header
    CSVData csvData;

    CSVData::Row header;
    header.add("Symbol");
    header.add("Name");
    header.add("Category");
    header.add("Family");
    header.add("Expense Ratio");
    header.add("12b1 Fee");
    header.add("Morningstar Rating");
    header.add("Inception Date");
    header.add("Initial Investment");
    header.add("Subsequent Investment");
    header.add("Assets");
    header.add("Turnover");
    header.add("Yield");
    header.add("Deferred Load");
    header.add("Front Load");
    header.add("Squared Error");
    header.add("Downside Variance");
    header.add("Downside Deviation");
    header.add("Upside Potential");
    header.add("Upside Potential Ratio");
    header.add("Volatility Skewness");
    header.add("Downside Frequency");
    header.add("Avg Downside Deviatation");
    header.add("Sortino Ratio");
    header.add("Full Annualized Return");
    header.add("#Months");
    header.add("Benchmark");
    header.add("Benchmark Months");
    header.add("Correlation");
    header.add("Common Annualized Return");
    header.add("Bench Annualized Return");
    header.add("Omega");
    header.add("Benchmark Omega");
    header.add("Excess Omega");
    header.add("Style Beta");
    header.add("Benchmark Downside Variance");

    
    csvData.setHeadings(header);

    StockList::const_iterator end = m_symbolList.end();
    StockList::const_iterator iter;
    for (iter = m_symbolList.begin(); iter != end; ++iter)
    {
      CSVData::Row row;
      if (!processSymbol(*iter, row))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to process symbol '" << *iter
                     << "'. Continuing execution."
                     << Context::endl;
        continue;
      }

      csvData.addRow(row);
    }

    std::ofstream ofs;
    ofs.open(m_outFile.c_str());
    if (!ofs)
    {
      getContext() << Context::PRIORITY_error
        << "Failed to open output file " << m_outFile
        << Context::endl;
      return false;
    }
    else if (!CSVDataStream::write(ofs, csvData, getContext()))
    {
      getContext() << Context::PRIORITY_error
        << "Failed to write CSV file" << m_outFile
        << Context::endl;
      ofs.close();
      return false;
    }

    ofs.close();

    return true;
  }


  bool AlchemyRisk::processSymbol(const StockID& symbol, CSVData::Row& row)
  {
    getContext() << Context::PRIORITY_info
                 << "Processing " << symbol << "..."
                 << Context::endl;

    RangeDataPtr data = retrieveData(symbol);
    if (!data)
    {
      return false;
    }

    Risk risk(data, m_bootstrap);
    risk.setMAR(m_mar);

    BenchmarkPerformancePtr bp = m_benchmarks[symbol.getSymbol()];
    if (!bp)
    {
      getContext() << Context::PRIORITY_error
        << "Missing benchmark for " << symbol
        << Context::endl;
      return false;
    }

    StockInfoPtr info = getStockInfo(symbol.getSymbol());
    StockInfoPtr benchInfo = getStockInfo(bp->getBench().getSymbol());
    double symOmega = bp->getSymbolOmega();
    double benchOmega = bp->getBenchmarkOmega();

    row.add(symbol.getSymbol());  
    row.add(info->getName());
    row.add(info->getCategory());
    row.add(info->getFamily());
    row.add(info->getExpenseRatio());
    row.add(info->get12b1Fee());
    row.add(info->getRating());
    row.add(boost::posix_time::to_simple_string(info->getInception()));
    row.add(info->getInitInvestment());
    row.add(info->getSubsInvestment());
    row.add(info->getAssets());
    row.add(info->getTurnover());
    row.add(info->getYield());
    row.add(info->getDeferredLoad());
    row.add(info->getFrontLoad());
    row.add(risk.getSquaredError());  
    row.add(risk.getDownsideVariation());  
    row.add(risk.getDownsideDeviation());  
    row.add(risk.getUpsidePotential());  
    row.add(risk.getUpsidePotentialRatio());  
    row.add(risk.getVolatilitySkewness());  
    row.add(risk.getDownsideFrequency());  
    row.add(risk.getAvgDownsideDeviation());  
    row.add(risk.getSortinoRatio());  
    row.add(Returns::annualized(data));
    row.add(data->size());
    row.add(benchInfo->getFullName());
    row.add(bp->getCommonBenchData()->size());
    row.add(bp->correlation());
    row.add(Returns::annualized(bp->getCommonSymbolData()));
    row.add(Returns::annualized(bp->getCommonBenchData()));
    row.add(symOmega);
    row.add(benchOmega);
    row.add(symOmega - benchOmega);
    row.add(bp->getStyleBeta());
    row.add(bp->getBenchDV());

    /*
    if (m_plot)
    {
      plotSymbol(symbol, vecReturns, bootReturns, ln3);
    }
    */

    return true;
  }

  void AlchemyRisk::plotSymbol(
    const StockID& symbol, 
    const std::vector<double>& vecReturns, 
    const std::vector<double>& bootReturns, 
    const LogNormal3& ln3)
  {
    double xmin = std::min(
        Statistics::min(&vecReturns[0], vecReturns.size()),
        Statistics::min(&bootReturns[0], bootReturns.size()));
    xmin = std::min(xmin, ln3.getNegInfinity());

    double xmax = std::max(
        Statistics::max(&vecReturns[0], vecReturns.size()),
        Statistics::max(&bootReturns[0], bootReturns.size()));
    xmax = std::max(xmax, ln3.getPosInfinity());

    double binSize = (xmax - xmin) / 500.0;

    {
      double lnSize = binSize / 5.0;
      std::string outFile(symbol.getSymbol() + "_ln3.csv");
      std::ofstream ofs;
      ofs.open(outFile.c_str());
      if (!ofs)
      {
        getContext() << Context::PRIORITY_error
          << "Failed to open output file " << outFile
          << Context::endl;
        return;
      }
      ofs << "Return,LN3 Probability\n";
      for (double d = xmin; d < xmax; d += lnSize)
      {
        ofs << d << "," << ln3(d) << "\n";
      }
      ofs.close();
    }

    {
      std::string outFile(symbol.getSymbol() + "_returns.csv");
      std::ofstream ofs;
      ofs.open(outFile.c_str());
      if (!ofs)
      {
        getContext() << Context::PRIORITY_error
          << "Failed to open output file " << outFile
          << Context::endl;
        return;
      }
      ofs << "Return,Actual,Bootstrap\n";
      double nActual = (double)vecReturns.size();
      double nBootstrap = (double)bootReturns.size();
      for (double d = xmin; d < xmax; d += binSize)
      {
        double end = d + binSize;

        double iActual = DownsideRisk::countPointsInRange(
            d, end, &vecReturns[0], vecReturns.size());
        
        double iBootstrap = DownsideRisk::countPointsInRange(
            d, end, &bootReturns[0], bootReturns.size());

        double pActual = iActual / nActual / binSize;
        double pBootstrap = iBootstrap / nBootstrap / binSize;

        ofs << d << "," << pActual << "," << pBootstrap << "\n";
      }
      ofs.close();
    }
  }

  StockInfoPtr AlchemyRisk::getStockInfo(const StockID& id)
  {
    if (m_infoCache.find(id.getSymbol()) != m_infoCache.end())
    {
      return m_infoCache[id.getSymbol()];
    }

    StockInfoPtr info(new StockInfo());
    info->setID(id);
    m_infoCache[id.getSymbol()] = info;
    return info;
  }

  bool AlchemyRisk::discoverBenchmarks()
  { 
    const std::string benchFile("benchmark.csv");
    bool ret = true;
    
    // write CSV header
    CSVData csvData;

    // set up header
    CSVData::Row header;
    header.add("Symbol");
    StockList::const_iterator bEnd = m_benchmarkList.end();
    StockList::const_iterator bIter;
    for (bIter = m_benchmarkList.begin(); bIter != bEnd; ++bIter)
    {
      header.add(getStockInfo(*bIter)->getFullName());
    }
    header.add("Name");
    header.add("Category");
    header.add("Best Benchmark");
    csvData.setHeadings(header);


    // for each symbol...
    StockList::const_iterator end = m_symbolList.end();
    StockList::const_iterator iter;
    for (iter = m_symbolList.begin(); iter != end; ++iter)
    {
      getContext() << Context::PRIORITY_info
        << "Computing correlation for " << *iter
        << Context::endl;

      CSVData::Row row;
      row.add(iter->getSymbol());

      bEnd = m_benchmarkList.end();

      // keep track of strongest correlation
      StockList::const_iterator bestIter = bEnd;
      double bestCorrel = 0.0;
      BenchmarkPerformancePtr bestBP;
      for (bIter = m_benchmarkList.begin(); bIter != bEnd; ++bIter)
      {
        // get symbol and benchmark data
        RangeDataPtr symbolData = retrieveData(*iter);
        if (!symbolData) { return false; }

        RangeDataPtr benchData = retrieveData(*bIter);
        if (!benchData) { return false; }

        BenchmarkPerformancePtr bp(
            new BenchmarkPerformance(*iter, symbolData, *bIter, benchData));
        if (!bp->valid())
        {
          getContext() << Context::PRIORITY_error
            << "Failed to compute benchmark performance"
            << Context::endl;
          row.add("N/A");
          ret = false;
          continue;
        }

        double correl = bp->correlation();
        row.add(correl);

        if (::fabs(correl) > ::fabs(bestCorrel))
        {
          bestCorrel = correl;
          bestIter = bIter;
          bestBP = bp;
        }
      }

      StockInfoPtr info = getStockInfo(*iter);
      row.add(info->getName());
      row.add(info->getCategory());

      // log best benchmark, if any
      if (bestIter == bEnd)
      {
        row.add("N/A");
      }
      else
      {
        // finish setting up benchmark performance
        bestBP->setMAR(m_mar);
        m_benchmarks[iter->getSymbol()] = bestBP;
        row.add(getStockInfo(*bestIter)->getFullName());
      }

      csvData.addRow(row);
    }
 

    std::ofstream ofs;
    ofs.open(benchFile.c_str());
    if (!ofs)
    {
      getContext() << Context::PRIORITY_error
        << "Failed to open benchmark output file " << benchFile
        << Context::endl;
      return false;
    }
    else if (!CSVDataStream::write(ofs, csvData, getContext()))
    {
      getContext() << Context::PRIORITY_error
        << "Failed to write benchmark output file" << benchFile
        << Context::endl;
      ofs.close();
      return false;
    }

    ofs.close();

    return ret;
  }

} // namespace alch
