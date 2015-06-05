// -*- C++ -*-

#ifndef INCLUDED_nnet_Statistics_h
#define INCLUDED_nnet_Statistics_h

namespace alch {

/*!
  \brief Functions for calculating statistical properties
  \ingroup nnet
*/
namespace Statistics
{

  /*!
    \brief Calculates the mean of specified data
    \param data The data
    \param n Number of elements in data
    \return Calculated mean
  */
  double mean(const double* data, int n);


  /*!
    \brief Calculates the variance of specified data
    \param data The data
    \param n Number of elements in data
    \return Calculated mean
  */
  double variance(const double* data, int n);


  /*!
    \brief Calculates the standard deviation of specified data
    \param data The data
    \param n Number of elements in data
    \return Calculated mean
  */
  double stddev(const double* data, int n);


  /*!
    \brief Calculates the correlation of specified data
    \param data1 Dataset 1
    \param data2 Dataset 2
    \param n Number of elements in data
    \return Calculated mean
  */
  double correlation(const double* data1, const double* data2, int n);

  /*!
   \brief Returns the minimum value in the specified dataset
  */
  double min(const double* data, int n);

  /*!
   \brief Returns the maximum value in the specified dataset
  */
  double max(const double* data, int n);

} // namespace Statistics
} // namespace alch

#endif
