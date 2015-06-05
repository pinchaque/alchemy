
#include "stockalg/PortfolioEvolve.h"
#include "stockalg/PortfolioPermute.h"

#undef DEBUGEVOLVE

namespace alch {

  PortfolioEvolve::PortfolioEvolve(
      int popSize, 
      PortfolioPtr ptf, 
      PortfolioEvalPtr eval,
      Context& ctx)
    : m_ctx(ctx)
    , m_eval(eval)
    , m_ptfTotal(0.0)
    , m_pop()
    , m_popSize(popSize)
    , m_probCrossover(0.25)
    , m_probMutation(0.01)
    , m_fitness()
    , m_minFitness(0.0)
    , m_maxFitness(0.0)
    , m_totalFitness(0.0)
    , m_selProb()
    , m_cumulProb()
    , m_amountMax(20.0)
  {
    generateInitialPop(ptf);
    calcFitness();
  }

  PortfolioEvolve::~PortfolioEvolve()
  {
    ;
  }

  void PortfolioEvolve::generateInitialPop(PortfolioPtr ptf)
  {
    // get the total amount to use based on the give portfolio
    m_ptfTotal = 0.0;
    const Portfolio::Symbols& symbols = ptf->getSymbols();
    Portfolio::Symbols::const_iterator end = symbols.end();
    Portfolio::Symbols::const_iterator iter;
    for (iter = symbols.begin(); iter != end; ++iter)
    {
      assert(ptf->has(*iter));
      m_ptfTotal += ptf->get(*iter);
    }

    // use portfolio permute to get random portfolios
    PortfolioPermute pp(ptf, m_ptfTotal);
    m_pop.clear();
    m_pop.reserve(m_popSize);
    for (int i = 0; i < m_popSize; ++i)
    {
      m_pop.push_back(pp.next());
    }

    assert((int) m_pop.size() == m_popSize);
  }

  void PortfolioEvolve::resetFitness()
  {
    m_totalFitness = 0.0;
    m_minFitness = 0.0;
    m_maxFitness = 0.0;
    m_fitness.clear();
    m_fitness.reserve(m_popSize);
    m_selProb.clear();
    m_selProb.reserve(m_popSize);
    m_cumulProb.clear();
    m_cumulProb.reserve(m_popSize);
  }

  void PortfolioEvolve::calcFitness()
  {
    // clear out our fitness values
    resetFitness();

    assert((int) m_pop.size() == m_popSize);

    // calculate the fitness using m_eval
    m_minFitness = std::numeric_limits<double>::max();
    m_maxFitness = std::numeric_limits<double>::min();
    double minReal = 0.0;
    double maxReal = 0.0;
    for (int i = 0; i < m_popSize; ++i)
    {
      bool ret = m_eval->process(m_pop[i]);
      assert(ret);

      double score = m_eval->getScore();
      double realScore = m_eval->getRealScore();
      if (score < m_minFitness) 
      { 
        m_minFitness = score; 
        minReal = realScore;
      }
      if (score > m_maxFitness) 
      { 
        m_maxFitness = score; 
        maxReal = realScore;
      }
      m_fitness.push_back(score);
    }
    assert((int)m_fitness.size() == m_popSize);

    m_ctx << Context::PRIORITY_debug1
          << "Fitness range [" << m_minFitness << ", " << m_maxFitness << "]" 
          << " real[" << minReal << ", " << maxReal << "]" 
          << Context::endl;

    // now we raise the bar by bringing m_minFitness up closer to m_maxFitness
    if (m_minFitness == m_maxFitness)
    {
      m_minFitness *= 0.9;
    }
    else
    {
      assert(m_maxFitness >= m_minFitness);
      double newMin = 0.9 * m_maxFitness;
      if (newMin > m_minFitness)
      {
        m_minFitness = newMin;
      }
    }

    // calculate total fitness. we may need to scale up so that we have
    // positive fitness so that we get positive probabilities
    m_totalFitness = 0.0;
    int numZero = 0;
    for (int i = 0; i < m_popSize; ++i)
    {
      double newScore = m_fitness[i] - m_minFitness;
      if (newScore < Portfolio::MINFLOAT) 
      {
        newScore = 0.0; 
        ++numZero;
      }
      assert(newScore >= 0.0);

      m_fitness[i] = newScore;
      m_totalFitness += newScore;
    }


    m_ctx << Context::PRIORITY_debug1
          << "Threshold " << m_minFitness << ": " << numZero 
          << " out of " << m_popSize << " " << (100.0 * numZero / m_popSize) 
          << "% were zero" << Context::endl;

    assert(m_totalFitness > Portfolio::MINFLOAT);

    // calculate the selection probabilities
    double cumulProb = 0.0;
    for (int i = 0; i < m_popSize; ++i)
    {
      double selProb = m_fitness[i] / m_totalFitness;
      m_selProb.push_back(selProb);
      cumulProb += selProb;

#ifdef DEBUGEVOLVE
      std::cerr << "[" << i << "] fitness = " << m_fitness[i] << " / prob = " << selProb << " / cumul = " << cumulProb << std::endl;
#endif
      m_cumulProb.push_back(cumulProb);
    }
  }

  void PortfolioEvolve::getBest(
      PortfolioPtr& ptf, double& score, int& index) const
  {
    score = std::numeric_limits<double>::min();
    index = -1;
    ptf = PortfolioPtr();
    
    // now iterate through all our portfolios
    for (int i = 0; i < m_popSize; ++i)
    {
      if (m_fitness[i] > score)
      {
        index = i;
        score = m_fitness[i];
        ptf = m_pop[i];
      }
    }

    // we need to adjust the fitness to our original scale
    score += m_minFitness;
  }

  void PortfolioEvolve::runGeneration()
  {
    std::vector<PortfolioPtr> newPop;
    selection(newPop);
    crossover(newPop);
    mutation(newPop);
    correct(newPop);

    // make sure the best-scoring element from last round is also in this one
    PortfolioPtr bestPtf;
    double score;
    int idx;
    getBest(bestPtf, score, idx);
    newPop[0] = PortfolioPtr(new Portfolio(*bestPtf));
    
    m_pop = newPop;
    calcFitness();
  }

  PortfolioPtr PortfolioEvolve::doSelectOne() const
  {
    double rnd = drand48();
    for (int i = 0; i < m_popSize; ++i)
    {
      if (rnd < m_cumulProb[i])
      {
        return PortfolioPtr(new Portfolio(*m_pop[i]));
      }
    }

    // shouldn't get here...
    return PortfolioPtr(new Portfolio(*m_pop[m_popSize - 1]));
  }

  void PortfolioEvolve::selection(std::vector<PortfolioPtr>& newPop)
  {
    newPop.clear();
    newPop.reserve(m_popSize);

    for (int i = 0; i < m_popSize; ++i)
    {
      PortfolioPtr ptf = doSelectOne();
      newPop.push_back(ptf);
    }
  }
  
  void PortfolioEvolve::crossover(std::vector<PortfolioPtr>& newPop)
  {
    std::vector<PortfolioPtr>::const_iterator iter;
    std::vector<PortfolioPtr>::const_iterator end = newPop.end();
    PortfolioPtr lastChosen;
    for (iter = newPop.begin(); iter != end; ++iter)
    {
      // skip population members that aren't selected for crossover
      if (drand48() > m_probCrossover)
      {
        continue;
      }
      
      if (lastChosen == PortfolioPtr())
      {
        lastChosen = *iter;
      }
      else
      {
        doCrossover(*iter, lastChosen);
        lastChosen = PortfolioPtr();
      }
    }
  }

  void PortfolioEvolve::doCrossover(PortfolioPtr ptf, PortfolioPtr other)
  {
#ifdef DEBUGEVOLVE
    std::cerr << "Doing crossover of : ";
    ptf->dump(std::cerr);
    std::cerr << "With:";
    other->dump(std::cerr);
#endif

    const Portfolio::Symbols& symbols = ptf->getSymbols();
    int numSites = (int)(drand48() * symbols.size()) + 1;

    // randomly cross over numSites sites (may be some duplicates)
    for (int i = 0; i < numSites; ++i)
    {
      int idx = (int)(drand48() * symbols.size());
      double val = ptf->get(symbols[idx]);
      ptf->add(symbols[idx], other->get(symbols[idx])); 
      other->add(symbols[idx], val);
    
#ifdef DEBUGEVOLVE
      std::cerr << "Crossover at site " << idx << " symbol " << symbols[idx] 
        << "\n";
#endif
    }

#ifdef DEBUGEVOLVE
    std::cerr << "THIS Pre-normalized: ";
    ptf->dump(std::cerr);
#endif
    ptf->normalize(m_ptfTotal);
#ifdef DEBUGEVOLVE
    std::cerr << "THIS After normalized: ";
    ptf->dump(std::cerr);
    std::cerr << "\n\n";
#endif

#ifdef DEBUGEVOLVE
    std::cerr << "OTHER Pre-normalized: ";
    other->dump(std::cerr);
#endif
    other->normalize(m_ptfTotal);
#ifdef DEBUGEVOLVE
    std::cerr << "OTHER After normalized: ";
    other->dump(std::cerr);
    std::cerr << "\n\n";
#endif
  }

  /*
   We do mutation as follows:
   - each holding gets a m_probMutation chance of being mutated
   - if a holding gets mutated, its #shares/percentage gets set to a new
     value
   - all non-mutated holdings in that portfolio are then re-scaled around the
     mutated ones
   - then the portfolio as a whole is rescaled (this is necessary in case two
     mutations are done, the combination of which push the portfolio over
     the acceptable total)
   */
  void PortfolioEvolve::mutation(std::vector<PortfolioPtr>& newPop)
  {
    std::vector<PortfolioPtr>::const_iterator iter;
    std::vector<PortfolioPtr>::const_iterator end = newPop.end();
    for (iter = newPop.begin(); iter != end; ++iter)
    {
      // keep track of which symbols were mutated, and their total value
      PortfolioPtr ptf = *iter;

#ifdef DEBUGEVOLVE
      std::cerr << "Original portfolio: ";
      ptf->dump(std::cerr);
#endif
      bool mutated = false;

      const Portfolio::Symbols& symbols = ptf->getSymbols();
      Portfolio::Symbols::const_iterator symEnd = symbols.end();
      Portfolio::Symbols::const_iterator symIter;
      for (symIter = symbols.begin(); symIter != symEnd; ++symIter)
      {
        assert(ptf->has(*symIter));

        // are we mutating this symbol?
        if (drand48() <= m_probMutation)
        {
          // yes -- do the mutation
          // mutate by multiplying value from 0.5 to 1.5
          double oldValue = ptf->get(*symIter);
          double newValue = (0.5 + drand48()) * oldValue;
          ptf->add(*symIter, newValue);
          mutated = true;

#ifdef DEBUGEVOLVE
      std::cerr << "Mutated " << *symIter << " from " << oldValue << " to " 
        << newValue << "\n";
#endif
        }
      }

      if (mutated)
      {
#ifdef DEBUGEVOLVE
        std::cerr << "Mutated portfolio before normalize:";
        ptf->dump(std::cerr);
#endif
        ptf->normalize(m_ptfTotal);
#ifdef DEBUGEVOLVE
        std::cerr << "Final mutated portfolio: ";
        ptf->dump(std::cerr);
        std::cerr << "\n\n";
#endif
      }
    }
  }


  // corrects portfolios in the population to obey our constraints
  void PortfolioEvolve::correct(std::vector<PortfolioPtr>& newPop)
  {
    std::vector<PortfolioPtr>::const_iterator iter;
    std::vector<PortfolioPtr>::const_iterator end = newPop.end();
    for (iter = newPop.begin(); iter != end; ++iter)
    {
      // keep track of which symbols were mutated, and their total value
      PortfolioPtr ptf = *iter;

#ifdef DEBUGEVOLVE
      std::cerr << "Original portfolio: ";
      ptf->dump(std::cerr);
#endif
      bool corrected = false;

      const Portfolio::Symbols& symbols = ptf->getSymbols();
      Portfolio::Symbols::const_iterator symEnd = symbols.end();
      Portfolio::Symbols::const_iterator symIter;
      for (symIter = symbols.begin(); symIter != symEnd; ++symIter)
      {
        assert(ptf->has(*symIter));
        double oldValue = ptf->get(*symIter);

        if (oldValue <= m_amountMax)
        {
          continue;
        }

        double newValue = 0.0;
        ptf->add(*symIter, newValue);
        corrected = true;

#ifdef DEBUGEVOLVE
        std::cerr << "Corrected " << *symIter << " from " << oldValue << " to " 
          << newValue << "\n";
#endif
      }

      if (corrected)
      {
#ifdef DEBUGEVOLVE
        std::cerr << "Corrected portfolio before normalize:";
        ptf->dump(std::cerr);
#endif
        ptf->normalize(m_ptfTotal);
#ifdef DEBUGEVOLVE
        std::cerr << "Final corrected portfolio: ";
        ptf->dump(std::cerr);
        std::cerr << "\n\n";
#endif
      }
    }
  }
} // namespace alch
