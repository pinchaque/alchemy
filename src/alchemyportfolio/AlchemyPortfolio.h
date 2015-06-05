// -*- C++ -*-

#ifndef INCLUDED_alchemyportfolio_AlchemyPortfolio_h
#define INCLUDED_alchemyportfolio_AlchemyPortfolio_h

#include "afwk/Framework.h"

#include "autil/CSVData.h"
#include "stockdata/StockID.h"
#include "stockdata/StockInfo.h"
#include "stockdata/StockList.h"
#include "stockdata/RangeData.h"
#include "stockdata/MetaFileStockDataSource.h"
#include "stockalg/PortfolioAlg.h"
#include "stockalg/PortfolioEval.h"

#include <vector>
#include <string>
#include <map>
#include <iosfwd>

namespace alch {

/*!
  \brief Runs the AlchemyPortfolio application
  \ingroup alchemyportfolio
*/
class AlchemyPortfolio : public Framework
{
 public:

  /*!
    \brief Constructor
  */
  AlchemyPortfolio();

  virtual ~AlchemyPortfolio();

protected:

  std::string getApplicationName() const
  {
    return "alchemyportfolio";
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
  static const char* const s_optionMAR;
  static const char* const s_optionStart;
  static const char* const s_optionEnd;
  static const char* const s_optionOut;
  static const char* const s_optionCorrel;
  static const char* const s_optionPlot;
  static const char* const s_optionBootstrap;
  static const char* const s_optionNumPortfolios;
  static const char* const s_optionTotal;
  static const char* const s_optionInc;
  static const char* const s_optionRiskWeight;
  static const char* const s_optionPopSize;
  static const char* const s_optionProbCrossover;
  static const char* const s_optionProbMutation;
  static const char* const s_optionAmountMax;
  static const char* const s_optionRebalance;

  typedef std::map<std::string, RangeDataPtr> DataCache;
  typedef std::map<std::string, StockInfoPtr> InfoCache;

  StockTime m_startTime;
  StockTime m_endTime;
  StockList m_symbolList;
  StockList m_benchmarkList;
  double m_mar;
  std::string m_outFile;
  std::string m_correlFile;
  std::string m_plotFile;
  DataCache m_dataCache;
  InfoCache m_infoCache;
  bool m_bootstrap;
  int m_numPortfolios;
  double m_total;
  double m_amountMax;
  PortfolioAlgPtr m_portfolioAlg;
  PortfolioEvalPtr m_portfolioEval;
  double m_riskWeight;
  int m_popSize;
  double m_probCrossover;
  double m_probMutation;
  int m_rebalanceFreq;

  bool setDateRange();
  RangeDataPtr retrieveData(const StockID& id);
  bool getSymbolList(const std::string& symbolFile, StockList& symbolList);
  bool processSymbols();
  bool finalizeCSV(const CSVData& csv, const std::string& file);
  PortfolioPtr getInitialPortfolio();
  void writeCSVHeader(CSVData& csv);
  StockInfoPtr getStockInfo(const StockID& id);
  bool initPortfolioAlg(PortfolioPtr ptf);
  bool initPortfolioEval();
  bool writeCorrel();
  double processPortfolio(int index, PortfolioPtr ptf, CSVData& csv);
  bool plotPortfolio(PortfolioPtr ptf);
};

} // namespace alch

#endif
