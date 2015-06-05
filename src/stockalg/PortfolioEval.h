// -*- C++ -*-

#ifndef INCLUDED_stockalg_PortfolioEval_h
#define INCLUDED_stockalg_PortfolioEval_h

#include "stockalg/PortfolioAlg.h"
#include "stockalg/Risk.h"
#include "stockdata/RangeData.h"
#include "boost/shared_ptr.hpp"

namespace alch {

/*!
  \brief Class description
  \ingroup stockalg
*/
class PortfolioEval
{
 public:

  /*!
    \brief Constructor
  */
  PortfolioEval(PortfolioAlgPtr ptfAlg);
  virtual ~PortfolioEval();

  // sets whether to bootstrap when calculating the risk characteristics
  void setBootstrap(bool bootstrap) { m_bootstrap = bootstrap; }

  // sets the minimum acceptable return
  void setMAR(double mar) { m_mar = mar; }

  // sets the risk weight
  void setWeight(double weight) { m_weight = weight; }
  
  void setRebalanceFreq(int rebalanceFreq) { m_rebalanceFreq = rebalanceFreq; }

  /*!
   \brief processes the portfolio. After this is called then you can get
   the risk, data, and score.
   */
  bool process(PortfolioPtr ptf);

  RiskPtr getRisk() const { return m_risk; }
  RangeDataPtr getData() const { return m_rangeData; }
  double getScore() const { return m_score; }
  double getRealScore() const { return m_realScore; }

 private:
    PortfolioAlgPtr m_ptfAlg;
    bool m_bootstrap;
    bool m_weight;
    double m_mar;
    RiskPtr m_risk;
    RangeDataPtr m_rangeData;
    double m_score;
    double m_realScore;
    int m_rebalanceFreq;
};

/*!
  \brief Shared pointer to PortfolioEval
  \ingroup stockalg
*/
typedef boost::shared_ptr<PortfolioEval> PortfolioEvalPtr;

} // namespace alch

#endif
