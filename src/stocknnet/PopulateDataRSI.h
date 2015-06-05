// -*- C++ -*-

#ifndef INCLUDED_stocknnet_PopulateDataRSI_h
#define INCLUDED_stocknnet_PopulateDataRSI_h

#include "stocknnet/PopulateData.h"

namespace alch {

/*!
  \brief Populates neural network based on Relative Strength Index
  \ingroup stocknnet

  RSI is naturally valued between 0 and 100. For the neural net this is
  remapped to -1..1.
*/
class PopulateDataRSI : public PopulateData
{
 public:

  /*!
    \brief Constructor
    \param ctx Operation context to use for all operations
  */
  PopulateDataRSI(Context& ctx)
    : PopulateData(ctx)
    , m_numberDays(0)
    , m_span(14)
  {
    ;
  }


  virtual ~PopulateDataRSI()
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

  //! Sets the span (# trading periods) to be used when calculating the RSI
  void setSpan(int val)
  {
    assert(val >= 1);
    m_span = val;
  }


  //! Returns the span used for calculating the RSI
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
  \brief Shared pointer to PopulateDataRSI
  \ingroup stocknnet
*/
typedef boost::shared_ptr<PopulateDataRSI> PopulateDataRSIPtr;

} // namespace alch

#endif
