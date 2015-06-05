
#include "stockalg/PortfolioEval.h"
#include "stockalg/Returns.h"

#include <cmath>
#include <limits>

namespace alch {

  PortfolioEval::PortfolioEval(PortfolioAlgPtr ptfAlg)
    : m_ptfAlg(ptfAlg)
    , m_bootstrap(true)
    , m_weight(3.0)
    , m_mar(0.0)
    , m_risk()
    , m_rangeData()
    , m_score(std::numeric_limits<double>::min())
    , m_realScore(std::numeric_limits<double>::min())
    , m_rebalanceFreq(0)
  {
    ;
  }

  PortfolioEval::~PortfolioEval()
  {
    ;
  }


  bool PortfolioEval::process(PortfolioPtr ptf)
  {
    m_risk = RiskPtr();
    m_score = std::numeric_limits<double>::min();
    m_realScore = std::numeric_limits<double>::min();
    m_rangeData = m_ptfAlg->generateData(ptf, m_rebalanceFreq);

    if (!m_rangeData)
    {
      return false;
    }

    // calculate the risk characteristics for this portfolio
    m_risk = RiskPtr(new Risk(m_rangeData, m_bootstrap));
    m_risk->setMAR(m_mar);

    m_realScore = m_risk->getUpsidePotentialRatio();
    m_score = ::pow(m_realScore, 2.0);

    /*
    double dd = m_risk->getDownsideDeviation();
    double returns = Returns::annualized(m_rangeData);
    m_realScore = returns - m_weight * dd;

    // the score we use for selection is based off of how much better we
    // are than the MAR, amplified up
    m_score = ::pow(10.0, (m_realScore - m_mar) * 10.0);
    */

    return true;
  }
} // namespace alch
