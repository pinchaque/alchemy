#include "alchemyportfolio/AlchemyPortfolio.h"

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

#include "stockalg/Risk.h"
#include "stockalg/Returns.h"
#include "stockalg/PortfolioPermute.h"
#include "stockalg/PortfolioEval.h"
#include "stockalg/PortfolioEvolve.h"

#include "boost/tokenizer.hpp"
#include "boost/filesystem/operations.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace alch {

  const char* const AlchemyPortfolio::s_optionFile = "file";
  const char* const AlchemyPortfolio::s_optionMAR = "mar";
  const char* const AlchemyPortfolio::s_optionOut = "out";
  const char* const AlchemyPortfolio::s_optionCorrel = "correl";
  const char* const AlchemyPortfolio::s_optionPlot = "plot";
  const char* const AlchemyPortfolio::s_optionBootstrap = "bootstrap";
  const char* const AlchemyPortfolio::s_optionNumPortfolios = "numptf";
  const char* const AlchemyPortfolio::s_optionTotal = "total";
  const char* const AlchemyPortfolio::s_optionRiskWeight = "weight";
  const char* const AlchemyPortfolio::s_optionAmountMax = "max";
  const char* const AlchemyPortfolio::s_optionPopSize = "popsize";
  const char* const AlchemyPortfolio::s_optionProbCrossover = "probcrossover";
  const char* const AlchemyPortfolio::s_optionProbMutation = "probmutation";
  const char* const AlchemyPortfolio::s_optionRebalance = "rebalance";

  AlchemyPortfolio::AlchemyPortfolio()
    : Framework()
    , m_startTime(boost::posix_time::second_clock::local_time())
    , m_endTime(boost::posix_time::second_clock::local_time())
    , m_symbolList()
    , m_mar(0.00)
    , m_outFile("portfolio.csv")
    , m_correlFile("correl.csv")
    , m_plotFile("plot.csv")
    , m_dataCache()
    , m_infoCache()
    , m_bootstrap(true)
    , m_numPortfolios(500)
    , m_total(100)
    , m_amountMax(0.0)
    , m_portfolioAlg()
    , m_portfolioEval()
    , m_riskWeight(4.0)
    , m_popSize(100)
    , m_probCrossover(0.25)
    , m_probMutation(0.01)
    , m_rebalanceFreq(0)
  {
    ;
  }


  AlchemyPortfolio::~AlchemyPortfolio()
  {
    ;
  }


  std::string AlchemyPortfolio::getApplicationDescription() const
  {
    std::stringstream ss;
    ss << "todo"
        ;

    return ss.str();
  }

  bool AlchemyPortfolio::initialize()
  {
    return Framework::initialize();
  }

  bool AlchemyPortfolio::finalize()
  {
    return Framework::finalize();
  }

  Framework::OptionsReturnCode AlchemyPortfolio::processOptions(
    int argc, char** argv)
  {

    std::stringstream ssFile;
    ssFile << s_optionFile << "," << s_optionFile[0];
    
    std::stringstream ssOut;
    ssOut << s_optionOut << "," << s_optionOut[0];
    
    std::stringstream ssBootstrap;
    ssBootstrap << s_optionBootstrap << "," << s_optionBootstrap[0];
   
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
      (s_optionMAR,
       boost::program_options::value<double>(),
       "Minimum Acceptable Return (MAR) in percentage points")
      (ssOut.str().c_str(), 
       boost::program_options::value<std::string>(),
       "Output CSV file name")
      (s_optionCorrel,
       boost::program_options::value<std::string>(),
       "CSV file name for portfolio correlations")
      (s_optionPlot,
       boost::program_options::value<std::string>(),
       "CSV file name for portfolio returns plot")
      (s_optionNumPortfolios,
       boost::program_options::value<int>(),
       "Number of portfolios to generate")
      (s_optionTotal,
       boost::program_options::value<double>(),
       "Total amount to use for each portfolio trial")
      (s_optionAmountMax,
       boost::program_options::value<double>(),
       "Max percentage of any security")
      (s_optionRiskWeight,
       boost::program_options::value<double>(),
       "Risk weight when ranking portfolios")
      (s_optionRebalance,
       boost::program_options::value<int>(),
       "Rebalance portfolio after this many periods")
      (s_optionPopSize,
       boost::program_options::value<int>(),
       "Population size for evolutionary algorithm")
      (s_optionProbMutation,
       boost::program_options::value<double>(),
       "Probability of mutating individual holding during evolutionary alg")
      (s_optionProbCrossover,
       boost::program_options::value<double>(),
       "Probability a portfolio will be crossed over during evolutionary alg")
      (ssBootstrap.str().c_str(),
       boost::program_options::value<bool>(),
       "Whether to bootstrap portfolio data before calculating statistics")
      ;

    return Framework::processOptions(argc, argv);
  }


  bool AlchemyPortfolio::processApplication()
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

    if (vm.count(s_optionCorrel))
    {
      m_correlFile = vm[s_optionCorrel].as<std::string>();
    }

    if (vm.count(s_optionPlot))
    {
      m_plotFile = vm[s_optionPlot].as<std::string>();
    }

    if (vm.count(s_optionRiskWeight))
    {
      m_riskWeight = vm[s_optionRiskWeight].as<double>();
    }

    if (vm.count(s_optionAmountMax))
    {
      m_amountMax = vm[s_optionAmountMax].as<double>();
    }

    if (vm.count(s_optionPopSize))
    {
      m_popSize = vm[s_optionPopSize].as<int>();
    }

    if (vm.count(s_optionRebalance))
    {
      m_rebalanceFreq = vm[s_optionRebalance].as<int>();
    }

    if (vm.count(s_optionProbCrossover))
    {
      m_probCrossover = vm[s_optionProbCrossover].as<double>();
    }

    if (vm.count(s_optionProbMutation))
    {
      m_probMutation = vm[s_optionProbMutation].as<double>();
    }

    if (vm.count(s_optionMAR))
    {
      m_mar = vm[s_optionMAR].as<double>() / 100.0;
    }

    if (vm.count(s_optionBootstrap))
    {
      m_bootstrap = vm[s_optionBootstrap].as<bool>();
    }

    if (vm.count(s_optionNumPortfolios))
    {
      m_numPortfolios = vm[s_optionNumPortfolios].as<int>();
    }

    if (vm.count(s_optionTotal))
    {
      m_total = vm[s_optionTotal].as<double>();
    }

    if (m_total <= 0)
    {
      getContext() << Context::PRIORITY_error
                   << "Total must be greater than 0"
                   << Context::endl;
      return false;
    }

    std::string symbolFile = vm[s_optionFile].as<std::string>();
    if (!getSymbolList(symbolFile, m_symbolList))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to get symbol list"
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

  bool AlchemyPortfolio::setDateRange()
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

  RangeDataPtr AlchemyPortfolio::retrieveData(const StockID& id)
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


  bool AlchemyPortfolio::getSymbolList(
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


  bool AlchemyPortfolio::processSymbols()
  { 

    // Initialize our portfolio algorithm classes
    PortfolioPtr ptf = getInitialPortfolio();
    if (!initPortfolioAlg(ptf))
    {
      return false;
    }
    if (!initPortfolioEval())
    {
      return false;
    }
    PortfolioEvolve pe(m_popSize, ptf, m_portfolioEval, getContext());
    pe.setProbCrossover(m_probCrossover);
    pe.setProbMutation(m_probMutation);
    pe.setAmountMax(m_amountMax);

    // write CSV header for output statistics
    // (must happen after initPortfolio so that we have the stock info loaded)
    CSVData csv;
    writeCSVHeader(csv);

    // write out the correlation data
    if (!writeCorrel())
    {
      return false;
    }

    double bestRank = 0.0;
    int bestIdx = -1;
    PortfolioPtr bestPtf(new Portfolio());
    for (int ptfIdx = 0; ptfIdx < m_numPortfolios; ++ptfIdx)
    {
      pe.runGeneration();

      PortfolioPtr pCurr;
      double score;
      int idx;
      pe.getBest(pCurr, score, idx);

      double rank = processPortfolio(ptfIdx, pCurr, csv);

      if (!(ptfIdx % 1))
      {
        getContext() << Context::PRIORITY_info
          << "[" << ptfIdx << "] index = "
          << idx << " / score = " << m_portfolioEval->getScore() 
          << " / realscore = " << m_portfolioEval->getRealScore() 
          << Context::endl;
      }

      if (rank > bestRank)
      {
        bestRank = rank;
        bestPtf = pCurr;
        bestIdx = ptfIdx;
      }
    }


    getContext() << Context::PRIORITY_info
      << "Best portfolio was index " << bestIdx << " with rank " << bestRank
      << Context::endl;

    if (!plotPortfolio(bestPtf))
    {
      return false;
    }

    if (!finalizeCSV(csv, m_outFile))
    {
      return false;
    }

    return true;
  }

  bool AlchemyPortfolio::finalizeCSV(
      const CSVData& csv, 
      const std::string& file)
  {
    std::ofstream ofs;
    ofs.open(file.c_str());
    if (!ofs)
    {
      getContext() << Context::PRIORITY_error
        << "Failed to open output file " << file
        << Context::endl;
      return false;
    }
    else if (!CSVDataStream::write(ofs, csv, getContext()))
    {
      getContext() << Context::PRIORITY_error
        << "Failed to write CSV file" << file
        << Context::endl;
      ofs.close();
      return false;
    }

    ofs.close();

    return true;
  }

  PortfolioPtr AlchemyPortfolio::getInitialPortfolio()
  {
    PortfolioPtr p(new Portfolio());

    StockList::const_iterator end = m_symbolList.end();
    StockList::const_iterator iter;
    for (iter = m_symbolList.begin(); iter != end; ++iter)
    {
      p->add(*iter, 1.0);
    }
    p->normalize(m_total);

    // if we aren't allowed to have enough of each symbol...
    if (m_amountMax < (m_total / (double)m_symbolList.size()))
    {
      m_amountMax = m_total * 3.0 / (double)m_symbolList.size();
      getContext() << Context::PRIORITY_info
        << "Using computed max amount: " << m_amountMax
        << Context::endl;
    }

    return p;
  }

  void AlchemyPortfolio::writeCSVHeader(CSVData& csv)
  {
    CSVData::Row header;
    header.add("Portfolio Index");

    StockList::const_iterator end = m_symbolList.end();
    StockList::const_iterator iter;
    for (iter = m_symbolList.begin(); iter != end; ++iter)
    {
      StockInfoPtr info = getStockInfo(iter->getSymbol());
      header.add(std::string(info->getFullName()
        + " - " + info->getCategory()));
    }

    std::stringstream ss;
    ss << "Rank (risk weight " << m_riskWeight << ")";

    header.add("Downside Deviation");
    header.add("Upside Potential");
    header.add("Upside Potential Ratio");
    header.add("Volatility Skewness");
    header.add("Downside Frequency");
    header.add("Avg Downside Deviatation");
    header.add("Sortino Ratio");
    header.add("Full Annualized Return");
    header.add("#Months");
    header.add(ss.str());

    csv.setHeadings(header);
  }

  bool AlchemyPortfolio::writeCorrel()
  {
    CSVData csv;
    CSVData::Row header;
    header.add("Symbol");

    StockList::const_iterator end = m_symbolList.end();
    StockList::const_iterator iter;
    for (iter = m_symbolList.begin(); iter != end; ++iter)
    {
      StockInfoPtr info = getStockInfo(iter->getSymbol());
      header.add(std::string(info->getFullName()
            + " - " + info->getCategory()));
    }

    csv.setHeadings(header);

    for (iter = m_symbolList.begin(); iter != end; ++iter)
    {
      CSVData::Row row;
      StockInfoPtr info = getStockInfo(iter->getSymbol());
      row.add(std::string(info->getFullName()
        + " - " + info->getCategory()));
      
      RangeDataPtr rowData = retrieveData(*iter);

      StockList::const_iterator iter2;
      for (iter2 = m_symbolList.begin(); iter2 != end; ++iter2)
      {
        RangeDataPtr colData = retrieveData(*iter2);

        RangeDataPtr cmn1(new RangeData());
        RangeDataPtr cmn2(new RangeData());
        if (!RangeDataAlg::findCommon(*rowData, *colData, *cmn1, *cmn2))
        {
          getContext() << Context::PRIORITY_error
            << "Failed to compute correlation for " << *iter
            << " and " << *iter2 << Context::endl;
          row.add("N/A");
          continue;
        }

        std::vector<double> r1;
        cmn1->getReturns(r1);
        std::vector<double> r2;
        cmn2->getReturns(r2);
        assert(r1.size() == r2.size());
        double correl 
          = Statistics::correlation(&r1[0], &r2[0], (int) r1.size());
        row.add(correl);
      }

      csv.addRow(row);
    }
    
    return finalizeCSV(csv, m_correlFile);
  }

  StockInfoPtr AlchemyPortfolio::getStockInfo(const StockID& id)
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

  bool AlchemyPortfolio::initPortfolioAlg(PortfolioPtr ptf)
  {
    m_portfolioAlg = PortfolioAlgPtr(new PortfolioAlg(ptf, getContext()));
    StockList::const_iterator end = m_symbolList.end();
    StockList::const_iterator iter;
    for (iter = m_symbolList.begin(); iter != end; ++iter)
    {
      RangeDataPtr data = retrieveData(*iter);
      m_portfolioAlg->addData(*iter, data);
    }

    if (!m_portfolioAlg->trimReturns())
    {
      getContext() << Context::PRIORITY_error
        << "Trim returns failed for portfolio"
        << Context::endl;
      return false;
    }

    return true;
  }
  
  bool AlchemyPortfolio::initPortfolioEval()
  {
    m_portfolioEval = PortfolioEvalPtr(new PortfolioEval(m_portfolioAlg));
    m_portfolioEval->setBootstrap(m_bootstrap);
    m_portfolioEval->setWeight(m_riskWeight);
    m_portfolioEval->setMAR(m_mar);
    m_portfolioEval->setRebalanceFreq(m_rebalanceFreq);
    return true;
  }

  double AlchemyPortfolio::processPortfolio(
      int index,
      PortfolioPtr ptf, 
      CSVData& csv)
  {
    CSVData::Row row;
    row.add(index);

    // add portfolio ratios to the output row


    Portfolio::Symbols syms = ptf->getSymbols();
    Portfolio::Symbols::const_iterator end = syms.end();
    Portfolio::Symbols::const_iterator iter;
    for (iter = syms.begin(); iter != end; ++iter)
    {
      row.add(ptf->get(*iter));
        
      /*
      getContext() << Context::PRIORITY_debug1
        << "Portfolio[" << index << "][" << *iter << "] = " << ptf->get(*iter)
        << Context::endl;
        */
    }

    // generate the results for this portfolio
    if (!m_portfolioEval->process(ptf))
    {
      getContext() << Context::PRIORITY_error
        << "Failed to generate data for portfolio" << index
        << Context::endl;
      return std::numeric_limits<double>::min();
    }

    // calculate the risk characteristics for this portfolio
    RiskPtr risk = m_portfolioEval->getRisk();
    row.add(risk->getDownsideDeviation());
    row.add(risk->getUpsidePotential());  
    row.add(risk->getUpsidePotentialRatio());  
    row.add(risk->getVolatilitySkewness());  
    row.add(risk->getDownsideFrequency());  
    row.add(risk->getAvgDownsideDeviation());  
    row.add(risk->getSortinoRatio());  
    
    RangeDataPtr data = m_portfolioEval->getData();
    row.add(Returns::annualized(data));
    row.add(data->size());

    double score = m_portfolioEval->getScore();
    row.add(score);

    csv.addRow(row);

    return score;
  }


  bool AlchemyPortfolio::plotPortfolio(PortfolioPtr ptf)
  {
    CSVData csv;

    // portfolio iterators
    Portfolio::Symbols syms = ptf->getSymbols();
    Portfolio::Symbols::const_iterator end = syms.end();
    Portfolio::Symbols::const_iterator iter;

    // set up the header 
    CSVData::Row header;
    header.add("Date");
    for (iter = syms.begin(); iter != end; ++iter)
    {
      StockInfoPtr info = getStockInfo(iter->getSymbol());
      std::stringstream ss;
      ss << info->getFullName() << " - " << info->getCategory()
        << " - weight " << ptf->get(*iter) << "%";
      header.add(ss.str());
    }
    header.add("Combined Portfolio");
    csv.setHeadings(header);

    // the full matrix of returns
    std::vector<RangeDataPtr> trimData = m_portfolioAlg->getTrimReturns();
    assert(trimData.size() == syms.size());
    trimData.push_back(m_portfolioAlg->generateData(ptf, m_rebalanceFreq));
    int numSymbols = (int) trimData.size();
    assert(numSymbols > 0);

    // need to weight all the returns by what we started the portfolio
    // with
    std::vector<double> weights;
    for (int i = 0; i < numSymbols; ++i)
    {
      assert((int) trimData[i]->size() > 0);
      weights.push_back(m_total / trimData[i]->get(0).close);
    }
    assert((int)weights.size() == numSymbols);


    int numPoints = (int) trimData[0]->size();
    for (int i = 0; i < numPoints; ++i)
    {
      StockTime t = trimData[0]->get(i).tradeTime;

      CSVData::Row row;
      row.add(boost::gregorian::to_iso_extended_string(t.date()));

      for (int j = 0; j < numSymbols; ++j)
      {
        assert((int) trimData[j]->size() > i);
        row.add(trimData[j]->get(i).close * weights[j]);
      }

      csv.addRow(row);
    }
     
    return finalizeCSV(csv, m_plotFile);
  }

} // namespace alch
