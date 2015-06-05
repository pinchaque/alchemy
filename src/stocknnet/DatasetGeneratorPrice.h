// -*- C++ -*-

#ifndef INCLUDED_stocknnet_DatasetGeneratorPrice_h
#define INCLUDED_stocknnet_DatasetGeneratorPrice_h

#include "stocknnet/DatasetGenerator.h"

namespace alch {

/*!
  \brief Price dataset generator with everything hard-coded
  \ingroup stocknnet
*/
class DatasetGeneratorPrice : public DatasetGenerator
{
 public:

  /*!
    \brief Constructor
  */
  DatasetGeneratorPrice(Context& ctx)
    : DatasetGenerator(ctx)
    , m_numberDays(1)
  {
    ;
  }

  virtual ~DatasetGeneratorPrice()
  {
    ;
  }

  virtual bool generate(const RangeDataPtr& rangeDataPtr,
                        NNetDataset& dataset);

  virtual bool generateInputs(const RangeDataPtr& rangeDataPtr,
                              NNetDataset& dataset);

  //! Sets number of days in advance for outputs
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

 private:

  //! number of days in future for output datapoint
  int m_numberDays;


};

} // namespace alch

#endif
