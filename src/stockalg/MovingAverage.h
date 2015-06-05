
#ifndef INCLUDED_stockalg_MovingAverage_h
#define INCLUDED_stockalg_MovingAverage_h


#ifndef INCLUDED_vector
#include <vector>
#define INCLUDED_vector
#endif


namespace alch {

namespace MovingAverage
{

  //! Convenience typedef
  typedef std::vector<double> DoubleVec;


  /*!
    \brief Calculates simple moving average
    \param data Data for which moving average is computed
    \param span Number of data span to consider in each moving average
    data point
    \param output [out] The calculated moving average

    If there are fewer than "span" elements in data, then output
    will be returned empty (cleared). Otherwise, output will contain
    (data.size() - span + 1) entries.
  */
  void simpleMA(const DoubleVec& data,
                int span,
                DoubleVec& output);


  /*!
    \brief Calculates weighted moving average
    \param data Data for which moving average is computed
    \param span Number of data span to consider in each moving average
    data point
    \param output [out] The calculated moving average

    A weighted moving average is similar to a simple moving average, except
    that each data point is weighted by its index within the data array.
    The oldest data point (index 0) is multiplied by 1, the second-oldest
    (index 1) is multiplied by 2, and so on.

    If there are fewer than "span" elements in data, then output
    will be returned empty (cleared). Otherwise, output will contain
    (data.size() - span + 1) entries.
  */
  void weightedMA(const DoubleVec& data,
                  int span,
                  DoubleVec& output);

  /*!
    \brief Calculates exponential moving average
    \param data Data for which moving average is computed
    \param span Number of data span to consider in each moving average
    data point
    \param output [out] The calculated moving average

    The exponential MA is a simpler means to calculate something similar
    to the weighted MA. Recommended values for exponent:

    Time Span (weeks)       Exponent
    5                       0.4
    10                      0.2
    15                      0.13
    20                      0.1
    40                      0.05
    80                      0.025

    If there are fewer than "span" elements in data, then output
    will be returned empty (cleared). Otherwise, output will contain
    (data.size() - span + 1) entries.
  */
  void exponentialMA(const DoubleVec& data,
                     int span,
                     double exponent,
                     DoubleVec& output);


} // namespace MovingAverage

} // namespace alch

#endif
