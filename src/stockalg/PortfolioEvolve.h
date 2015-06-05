// -*- C++ -*-

#ifndef INCLUDED_stockalg_PortfolioEvolve_h
#define INCLUDED_stockalg_PortfolioEvolve_h

#include "boost/shared_ptr.hpp"

#include "stockdata/Portfolio.h"
#include "stockalg/PortfolioEval.h"
#include "autil/Context.h"

namespace alch {

/*!
  \brief Class description
  \ingroup stockalg
*/
class PortfolioEvolve
{
 public:

  PortfolioEvolve(int popSize, PortfolioPtr ptf, PortfolioEvalPtr eval, 
      Context& ctx);
  virtual ~PortfolioEvolve();

  void setProbCrossover(double val) { m_probCrossover = val; }
  void setProbMutation(double val) { m_probMutation = val; }
  void setAmountMax(double val) { m_amountMax = val; }

  // runs a single generation
  void runGeneration();

  // returns best portfolio with its eval score and index
  void getBest(PortfolioPtr& ptf, double& score, int& index) const;


 private:
  Context& m_ctx;
  PortfolioEvalPtr m_eval;
  double m_ptfTotal;
  std::vector<PortfolioPtr> m_pop;
  int m_popSize;
  double m_probCrossover;
  double m_probMutation;
  std::vector<double> m_fitness;
  double m_minFitness;
  double m_maxFitness;
  double m_totalFitness;
  std::vector<double> m_selProb;
  std::vector<double> m_cumulProb;
  double m_amountMax;


  void generateInitialPop(PortfolioPtr ptf);
  void resetFitness();
  void calcFitness();

  PortfolioPtr doSelectOne() const;
  void doCrossover(PortfolioPtr ptf, PortfolioPtr other);
  void selection(std::vector<PortfolioPtr>& newPop);
  void crossover(std::vector<PortfolioPtr>& newPop);
  void mutation(std::vector<PortfolioPtr>& newPop);
  void correct(std::vector<PortfolioPtr>& newPop);

};

/*!
  \brief Shared pointer to PortfolioEvolve
  \ingroup stockalg
*/
typedef boost::shared_ptr<PortfolioEvolve> PortfolioEvolvePtr;

} // namespace alch

#endif
