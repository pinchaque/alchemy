// -*- C++ -*-

#ifndef INCLUDED_stocknnet_DatasetGenerator_h
#define INCLUDED_stocknnet_DatasetGenerator_h

#include "nnet/NNetDataset.h"
#include "autil/Context.h"
#include "stockdata/RangeData.h"

#include "boost/shared_ptr.hpp"

namespace alch {

/*!
  \brief Interface class for populating a NNetDataset object
  \ingroup stocknnet
*/
class DatasetGenerator
{
 public:

  /*!
    \brief Constructor
    \param ctx Operation context to use for all operations
  */
  DatasetGenerator(Context& ctx)
    : m_ctx(ctx)
  {
    ;
  }

  virtual ~DatasetGenerator()
  {
    ;
  }

  /*!
    \brief Generates inputs and outputs for a dataset 
    \param rangeDataPtr The data to use for generating this dataset
    \param dataset [out] The dataset to populate
    \retval true Success
    \retval false error   

    This method appends a series of NNetDatapoint objects to dataset based
    on all datapoints contained in rangeDataPtr. Because outputs are also 
    being added, the number of datapoints added to dataset is typically 
    limited by the number of days in advance the outputs are.
    .
  */
  virtual bool generate(const RangeDataPtr& rangeDataPtr,
                        NNetDataset& dataset) = 0;

  /*!
    \brief Generates inputs only for a dataset 
    \param rangeDataPtr The data to use for generating this dataset
    \param dataset [out] The dataset to populate
    \retval true Success
    \retval false error   

    This method appends a series of NNetDatapoint objects to dataset based
    on all datapoints contained in rangeDataPtr. The appended NNetDatapoint
    objects will not have any output values in them -- only inputs.
  */
  virtual bool generateInputs(const RangeDataPtr& rangeDataPtr,
                              NNetDataset& dataset) = 0;


protected:

  //! Returns context for the operations
  Context& getContext()
  {
    return m_ctx;
  }


 private:
  
  //! Operation context to use for plot creation
  Context& m_ctx;

};

/*!
  \brief Shared pointer to DatasetGenerator
  \ingroup stocknnet
*/
typedef boost::shared_ptr<DatasetGenerator> DatasetGeneratorPtr;

} // namespace alch

#endif
