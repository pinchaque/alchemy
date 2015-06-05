
#ifndef INCLUDED_stockalg_Momentum_h
#define INCLUDED_stockalg_Momentum_h

#include "stockdata/RangeData.h"

#include <vector>


namespace alch {

namespace Momentum
{

  //! Convenience typedef
  typedef std::vector<double> DoubleVec;


  /*!
    \brief Calculates the rate-of-change (ROC)
    \param data Data for which ROC is computed
    \param span The time span over which the ROC is calculated
    \param output [out] The calculated ROC

    If there are fewer than "points" elements in data, then output
    will be returned empty (cleared). Otherwise, output will contain
    (data.size() - span) entries.
  */
  void rateOfChange(const DoubleVec& data,
                    int span,
                    DoubleVec& output);


  /*!
    \brief Calculates the relative strength indicator (RSI)
    \param data Data for which RSI is computed (typically closing data)
    \param span The time span over which the RSI is calculated
    \param output [out] The calculated RSI

    If there are fewer than "points" elements in data, then output
    will be returned empty (cleared). Otherwise, output will contain
    (data.size() - span) entries. 

    The calculation of RSI requires knowing whether a given closing
    price is up or down for that day. For this reason, this calculation
    needs one extra day at the beginning of data to figure out if the
    first data point is "up" or "down". 
  */
  void relativeStrength(const DoubleVec& data,
                        int span,
                        DoubleVec& output);

  /*!
    \brief Calculates the Parabolic Stop-And-Reversal data
    \param data Data for which Parabolic SAR is computed (typically closing
    data)
    \param accel The acceleration for the SAR calculation
    \param maxAccel The maximum acceleration for the SAR calculation
    \param output [out] The calculated Parabolic SAR
   */
  void parabolicSAR(const RangeData& data,
                    double accel,
                    double maxAccel,
                    DoubleVec& output);


} // namespace Momentum

} // namespace alch

#endif
