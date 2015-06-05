// -*- C++ -*-

#ifndef INCLUDED_stocknnet_PopulateDataPSAR_h
#define INCLUDED_stocknnet_PopulateDataPSAR_h

#include "stocknnet/PopulateData.h"

namespace alch {

/*!
  \brief Populates neural network based on moving average data
  \ingroup stocknnet

  All prices are normalized by percentage higher/lower than the current price.
  Using 0 days means that the input that is added is the percentage difference
  between the current PSAR and the current price.
*/
class PopulateDataPSAR : public PopulateData
{
 public:

  /*!
    \brief Constructor
    \param ctx Operation context to use for all operations
  */
  PopulateDataPSAR(Context& ctx)
    : PopulateData(ctx)
    , m_numberDays(0)
    , m_accel(0.02)
    , m_maxAccel(0.20)
  {
    ;
  }


  virtual ~PopulateDataPSAR()
  {
    ;
  }

  virtual bool populate(const RangeDataPtr& rangeDataPtr,
                        NNetDataset& dataset,
                        int& numModified);


  //! Sets number of days of history to add
  void setNumberDays(int val)
  {
    assert(val >= 0);
    m_numberDays = val;
  }


  //! Returns number of days of history to add
  int getNumberDays() const
  {
    return m_numberDays;
  }


  //! Sets the acceleration to use for PSAR plot
  void setAccel(double val)
  {
    m_accel = val;
  }


  //! Returns the accel used for calculating PSAR
  double getAccel() const
  {
    return m_accel;
  }


  //! Sets the maximum acceleration to use for PSAR plot
  void setMaxAccel(double val)
  {
    m_maxAccel = val;
  }


  //! Returns the maximum acceleration used for calculating PSAR
  double getMaxAccel() const
  {
    return m_maxAccel;
  }


 private:

  //! Number of days in the past to add
  int m_numberDays;

  //! Acceleration to use when calculating PSAR
  double m_accel;

  //! Maximum acceleration to use when calculating PSAR
  double m_maxAccel;

};

/*!
  \brief Shared pointer to PopulateDataPSAR
  \ingroup stocknnet
*/
typedef boost::shared_ptr<PopulateDataPSAR> PopulateDataPSARPtr;

} // namespace alch

#endif
