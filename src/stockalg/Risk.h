// -*- C++ -*-

#ifndef INCLUDED_stockalg_Risk_h
#define INCLUDED_stockalg_Risk_h

#include "stockdata/RangeData.h"
#include "stockalg/LogNormal3.h"
#include "boost/shared_ptr.hpp"
#include <vector>


namespace alch {

/*!
  \brief Class description
  \ingroup stockalg
*/
class Risk
{
 public:

  /*!
    \brief Constructor
    \param data MONTHLY data for a symbol
    \param bootstrap Whether to bootstrap the data
    \param bootMult Multiplier to use when bootstrapping the data that controls
    how many points to generate
  */
  Risk(RangeDataPtr data, bool bootstrap, int bootMult = 50);

  virtual ~Risk()
  {
    ;
  }

  void setMAR(double mar) { m_mar = mar; }
  double getMAR() const { return m_mar; }

  double getSquaredError() const;
  double getDownsideVariation() const;
  double getDownsideDeviation() const;
  double getUpsidePotential() const;
  double getUpsidePotentialRatio() const;
  double getVolatilitySkewness() const;
  double getDownsideFrequency() const;
  double getAvgDownsideDeviation() const;
  double getSortinoRatio() const;

 private:
  double m_mar;
  RangeDataPtr m_data;
  std::vector<double> m_returns;
  LogNormal3Ptr m_ln3;

};

/*!
  \brief Shared pointer to Risk
  \ingroup stockalg
*/
typedef boost::shared_ptr<Risk> RiskPtr;

} // namespace alch

#endif
