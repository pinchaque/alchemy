// -*- C++ -*-

#ifndef INCLUDED_stocknnet_PopulateData_h
#define INCLUDED_stocknnet_PopulateData_h

#include "nnet/NNetDataset.h"
#include "autil/Context.h"
#include "stockdata/RangeData.h"

#include "boost/shared_ptr.hpp"

namespace alch {

/*!
  \brief Interface class for populating dataset for one analysis algorithm
  \ingroup stocknnet
*/
class PopulateData
{
 public:

  /*!
    \brief Constructor
    \param ctx Operation context to use for all operations
  */
  PopulateData(Context& ctx)
    : m_ctx(ctx)
  {
    ;
  }


  virtual ~PopulateData()
  {
    ;
  }


  /*!
    \brief Populates dataset with some neural network inputs based on 
    specified range data.
    \param rangeDataPtr The data to use for populating the dataset
    \param dataset [out] The dataset to populate
    \param numModified [out] The number of NNetDatapoint objects affected
    \retval true Success
    \retval false error

    Populating the dataset means adding zero or more neural network inputs
    to each element in the specified dataset. Some algorithms may not be
    able to add inputs to each element, in which case they should start
    adding from the end of dataset and work backwards, returning the number
    of NNetDatapoint objects affected in numModified. The rationale for this
    is so the calling DatasetGenerator can then trim off the NNetDatapoint
    objects that do not have enough inputs. Note that the same number of
    inputs should be added to each element starting at the numModified index.
  */
  virtual bool populate(const RangeDataPtr& rangeDataPtr,
                        NNetDataset& dataset,
                        int& numModified) = 0;


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
  \brief Shared pointer to PopulateData
  \ingroup stocknnet
*/
typedef boost::shared_ptr<PopulateData> PopulateDataPtr;

} // namespace alch

#endif
