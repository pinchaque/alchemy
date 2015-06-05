
#include "stockalg/BenchmarkPerformance.h"
#include "nnet/Statistics.h"
#include "stockalg/Returns.h"
#include "stockalg/Risk.h"
#include "stockdata/RangeDataAlg.h"

namespace alch {
  
  BenchmarkPerformance::BenchmarkPerformance(
      const StockID& symbol, RangeDataPtr symbolData, 
      const StockID& bench, RangeDataPtr benchData)
    : m_symbol(symbol)
    , m_bench(bench)
    , m_symbolData(symbolData)
    , m_benchData(benchData)
    , m_commonSymbolData(new RangeData())
    , m_commonBenchData(new RangeData())
    , m_valid(false)
    , m_riskAversion(3.0)
    , m_mar(0.0)
    , m_benchDV(0.0)
  {
    m_valid = RangeDataAlg::findCommon(
        *m_symbolData, *m_benchData,
        *m_commonSymbolData, *m_commonBenchData);
  }

  bool BenchmarkPerformance::valid() const
  {
    return m_valid;
  }

  double BenchmarkPerformance::correlation()
  {
    // get the returns for those ranges
    std::vector<double> symbolReturns;
    m_commonSymbolData->getReturns(symbolReturns);
    std::vector<double> benchReturns;
    m_commonBenchData->getReturns(benchReturns);

    // calculate the correlation
    assert(symbolReturns.size() == benchReturns.size());
    return Statistics::correlation(
        &symbolReturns[0],
        &benchReturns[0],
        (int) symbolReturns.size());
  }

  boost::posix_time::time_duration 
    BenchmarkPerformance::getDuration() const
  {
    return Returns::duration(m_commonBenchData);
  }

  double BenchmarkPerformance::getBenchTotalReturn() const
  {
    return Returns::total(m_commonBenchData);
  }

  double BenchmarkPerformance::getSymbolTotalReturn() const
  {
    return Returns::total(m_commonSymbolData);
  }
 
  double BenchmarkPerformance::getBenchAnnualizedReturn() const
  {
    return Returns::annualized(m_commonBenchData);
  }

  double BenchmarkPerformance::getSymbolAnnualizedReturn() const
  {
    return Returns::annualized(m_commonSymbolData);
  }

  // Beta: Style beta = DRsymbol / DRbenchmark
  double BenchmarkPerformance::getStyleBeta()
  {
    Risk rBench(m_commonBenchData, true);
    rBench.setMAR(m_mar);
    double drBench = rBench.getDownsideDeviation();
    if (drBench == 0.0) { return 0.0; }

    Risk rSymbol(m_commonSymbolData, true);
    rSymbol.setMAR(m_mar);
    double drSymbol = rSymbol.getDownsideDeviation();

    return (drSymbol / drBench);
  }

  // gets benchmark downside variance using ALL data (not just common)
  double BenchmarkPerformance::getBenchDV()
  {
    // if we already computed it once, don't do it again
    if (m_benchDV == 0.0)
    {
      Risk r(m_benchData, true);
      r.setMAR(m_mar);
      m_benchDV = r.getDownsideVariation();
    }

    return m_benchDV;
  }

  // omega = R - A * Beta * DV
  // R: Annualized return
  // A: Risk aversion (typically 3)
  // Beta: Style beta = DRsymbol / DRbenchmark
  double BenchmarkPerformance::getSymbolOmega()
  {
    return getSymbolAnnualizedReturn()
      - (m_riskAversion * getStyleBeta() * getBenchDV());
  }

  // similar to getSymbolOmega, except that the style beta is 1.0 since this
  // *IS* the style
  double BenchmarkPerformance::getBenchmarkOmega()
  {
    return (getBenchAnnualizedReturn() - (m_riskAversion * getBenchDV()));
  }
  
} // namespace alch
