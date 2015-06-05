// -*- C++ -*-

#ifndef INCLUDED_stockalg_BenchmarkPerformance_h
#define INCLUDED_stockalg_BenchmarkPerformance_h

#include "boost/shared_ptr.hpp"

#include "stockdata/RangeData.h"
#include "stockdata/StockID.h"

namespace alch {

/*!
  \brief Class description
  \ingroup stockalg
*/
class BenchmarkPerformance
{
 public:

  /*!
    \brief Constructor
  */
  BenchmarkPerformance(const StockID& symbol, RangeDataPtr symbolData, 
      const StockID& bench, RangeDataPtr benchData);

  virtual ~BenchmarkPerformance()
  {
    ;
  }

  const StockID& getSymbol() const { return m_symbol; }
  const StockID& getBench() const { return m_bench; }

  RangeDataPtr getCommonSymbolData() const { return m_commonSymbolData; }
  RangeDataPtr getCommonBenchData() const { return m_commonBenchData; }

  void setRiskAversion(double val) { m_riskAversion = val; }
  double getRiskAversion() const { return m_riskAversion; }

  void setMAR(double mar) { m_mar = mar; }
  double getMAR() const { return m_mar; }

  bool valid() const;
  double correlation();
  boost::posix_time::time_duration getDuration() const;
  double getBenchTotalReturn() const;
  double getSymbolTotalReturn() const;
  double getBenchAnnualizedReturn() const;
  double getSymbolAnnualizedReturn() const;
  double getStyleBeta();
  double getBenchDV();
  double getSymbolOmega();
  double getBenchmarkOmega();

 private:
  StockID m_symbol;
  StockID m_bench;
  RangeDataPtr m_symbolData;
  RangeDataPtr m_benchData;
  RangeDataPtr m_commonSymbolData;
  RangeDataPtr m_commonBenchData;
  bool m_valid;
  double m_riskAversion;
  double m_mar;
  double m_benchDV;
};

/*!
  \brief Shared pointer to BenchmarkPerformance
  \ingroup stockalg
*/
typedef boost::shared_ptr<BenchmarkPerformance> BenchmarkPerformancePtr;

} // namespace alch

#endif
