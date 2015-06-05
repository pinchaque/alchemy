// -*- C++ -*-

#ifndef INCLUDED_stocknnet_PopulateDataMA_h
#define INCLUDED_stocknnet_PopulateDataMA_h

#include "stocknnet/PopulateData.h"

namespace alch {

/*!
  \brief Populates neural network based on moving average data
  \ingroup stocknnet

  All prices are normalized by percentage higher/lower than the current price.
  Using 0 days means that the input that is added is the percentage difference
  between the current MA and the current price.
*/
class PopulateDataMA : public PopulateData
{
 public:

  /*!
    \brief Constructor
    \param ctx Operation context to use for all operations
  */
  PopulateDataMA(Context& ctx)
    : PopulateData(ctx)
    , m_numberDays(0)
    , m_span(20)
  {
    ;
  }


  virtual ~PopulateDataMA()
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

  //! Sets the span (# trading periods) to be used when calculating the MA
  void setSpan(int val)
  {
    assert(val >= 1);
    m_span = val;
  }


  //! Returns the span used for calculating the MA
  int getSpan() const
  {
    return m_span;
  }


 private:

  //! Number of days in the past to add
  int m_numberDays;

  //! Span to use when calculating moving average
  int m_span;
};

/*!
  \brief Shared pointer to PopulateDataMA
  \ingroup stocknnet
*/
typedef boost::shared_ptr<PopulateDataMA> PopulateDataMAPtr;

} // namespace alch

#endif
