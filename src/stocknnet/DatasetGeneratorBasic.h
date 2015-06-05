// -*- C++ -*-

#ifndef INCLUDED_stocknnet_DatasetGeneratorBasic_h
#define INCLUDED_stocknnet_DatasetGeneratorBasic_h

#include "stocknnet/DatasetGenerator.h"

namespace alch {

/*!
  \brief Basic dataset generator with everything hard-coded
  \ingroup stocknnet
*/
class DatasetGeneratorBasic : public DatasetGenerator
{
 public:

  /*!
    \brief Constructor
  */
  DatasetGeneratorBasic(Context& ctx)
    : DatasetGenerator(ctx)
    , m_numberDays(1)
  {
    ;
  }

  virtual ~DatasetGeneratorBasic()
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

  /*!
    \brief Adds outputs to given dataset
    \param rangeDataPtr Data that produced this dataset
    \param dataset [in/out] Dataset to add outputs to
    \retval true Success
    \retval false Error

    dataset may be resized/modified and should only contain data relevant
    to the specified rangeDataPtr.
  */
  bool generateOutputs(const RangeDataPtr& rangeDataPtr,
                       NNetDataset& dataset);


};

} // namespace alch

#endif
