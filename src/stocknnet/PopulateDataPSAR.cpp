
#include "stocknnet/PopulateDataPSAR.h"

#include "stockalg/Momentum.h"

namespace alch {


  bool PopulateDataPSAR::populate(const RangeDataPtr& rangeDataPtr,
                                  NNetDataset& dataset,
                                  int& numModified)
  {
    assert(m_numberDays >= 0);
    assert(rangeDataPtr.get());
    const RangeData& rangeData(*rangeDataPtr);

    // calculate the PSAR
    Momentum::DoubleVec output;
    Momentum::parabolicSAR(rangeData, getAccel(), getMaxAccel(), output);

    int datasetIdx = int(dataset.size()) - 1;
    int rangeDataIdx = int(rangeData.size()) - 1;
    int outputIdx = int(output.size()) - 1;

    numModified = 0;

    while ((datasetIdx >= 0)
           && ((outputIdx - m_numberDays) >= 0)
           && ((rangeDataIdx - m_numberDays) >= 0))
    {
      assert(datasetIdx >= 0);
      assert(datasetIdx < int(dataset.size()));

      assert(outputIdx - m_numberDays >= 0);
      assert(outputIdx < int(output.size()));

      assert(rangeDataIdx - m_numberDays >= 0);
      assert(rangeDataIdx < int(rangeData.size()));

      // this is the data point we're adding to
      NNetDatapoint& nnetPoint = dataset[datasetIdx];

      // add points for up to m_numberDays in the past
      for (int i = 0; i <= m_numberDays; ++i)
      {
        // reference value to use
        double referenceValue = rangeData.get(rangeDataIdx - i).close;

        // value from this point
        double currValue = output[outputIdx - i];

        // relative value of current point (percentage drop from reference)
        double relativeValue = (referenceValue - currValue) / referenceValue;

        nnetPoint.input.push_back(relativeValue);
      }

      // keep track of how many points we've modified
      ++numModified;

      // decrement the pointers
      --datasetIdx;
      --rangeDataIdx;
      --outputIdx;
    }

    return true;
  }

} // namespace alch
