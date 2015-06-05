// -*- C++ -*-

#ifndef INCLUDED_stocknnet_PopulateDataPrice_h
#define INCLUDED_stocknnet_PopulateDataPrice_h

#include "stocknnet/PopulateData.h"

namespace alch {

/*!
  \brief Populates neural network based on price data
  \ingroup stocknnet

  All prices are normalized by percentage higher/lower than the last price.
  Using 1 day means that the input that is added is the percentage different
  the penultimate price is. The current price is not added since it would 
  always be 1.0.
*/
class PopulateDataPrice : public PopulateData
{
 public:

  /*!
    \brief Constructor
    \param ctx Operation context to use for all operations
  */
  PopulateDataPrice(Context& ctx)
    : PopulateData(ctx)
    , m_numberDays(1)
  {
    ;
  }


  virtual ~PopulateDataPrice()
  {
    ;
  }

  virtual bool populate(const RangeDataPtr& rangeDataPtr,
                        NNetDataset& dataset,
                        int& numModified);


  //! Sets number of days of history to add
  void setNumberDays(int val)
  {
    assert(val >= 1);
    m_numberDays = val;
  }

  //! Returns number of days of history to add
  int getNumberDays() const
  {
    return m_numberDays;
  }

 private:

  //! Number of days in the past to add
  int m_numberDays;
};

/*!
  \brief Shared pointer to PopulateDataPrice
  \ingroup stocknnet
*/
typedef boost::shared_ptr<PopulateDataPrice> PopulateDataPricePtr;

} // namespace alch

#endif
