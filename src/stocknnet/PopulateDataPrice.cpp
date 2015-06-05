
#include "stocknnet/PopulateDataPrice.h"

namespace alch {


  bool PopulateDataPrice::populate(const RangeDataPtr& rangeDataPtr,
                                   NNetDataset& dataset,
                                   int& numModified)
  {
    assert(m_numberDays > 0);
    assert(rangeDataPtr.get());
    const RangeData& rangeData(*rangeDataPtr);

    int datasetIdx = int(dataset.size()) - 1;
    int rangeDataIdx = int(rangeData.size()) - 1;

    numModified = 0;

    while ((datasetIdx >= 0) && (rangeDataIdx - m_numberDays >= 0))
    {
      assert(datasetIdx >= 0);
      assert(datasetIdx < int(dataset.size()));

      // this is the data point we're adding to
      NNetDatapoint& nnetPoint = dataset[datasetIdx];

      // reference value to use
      assert(rangeDataIdx - m_numberDays >= 0);
      assert(rangeDataIdx < int(rangeData.size()));
      double referenceValue = rangeData.get(rangeDataIdx).close;

      // add points for up to m_numberDays in the past
      for (int i = 1; i <= m_numberDays; ++i)
      {
        // value from this point
        double currValue = rangeData.get(rangeDataIdx - i).close;

        // relative value of current point (percentage drop from reference)
        double relativeValue = (currValue - referenceValue) / referenceValue;

        nnetPoint.input.push_back(relativeValue);
      }

      // keep track of how many points we've modified
      ++numModified;

      // decrement the pointers
      --datasetIdx;
      --rangeDataIdx;
    }

    return true;
  }

} // namespace alch
