
#include "stocknnet/PopulateDataRSI.h"

#include "stockalg/Momentum.h"

namespace alch {


  bool PopulateDataRSI::populate(const RangeDataPtr& rangeDataPtr,
                                   NNetDataset& dataset,
                                   int& numModified)
  {
    assert(m_numberDays >= 0);
    assert(rangeDataPtr.get());
    const RangeData& rangeData(*rangeDataPtr);


    // Initialize the input to the RSI calculator
    Momentum::DoubleVec input;
    {
      RangeData::const_iterator end = rangeData.end();
      RangeData::const_iterator iter;
      for (iter = rangeData.begin(); iter != end; ++iter)
      {
        input.push_back(iter->close);
      }
    }

    // calculate the RSI
    Momentum::DoubleVec output;
    Momentum::relativeStrength(input, getSpan(), output);

    int datasetIdx = int(dataset.size()) - 1;
    int outputIdx = int(output.size()) - 1;

    numModified = 0;

    while ((datasetIdx >= 0) && ((outputIdx - m_numberDays) >= 0))
    {
      assert(datasetIdx >= 0);
      assert(datasetIdx < int(dataset.size()));

      assert(outputIdx - m_numberDays >= 0);
      assert(outputIdx < int(output.size()));

      // this is the data point we're adding to
      NNetDatapoint& nnetPoint = dataset[datasetIdx];

      // add points for up to m_numberDays in the past
      for (int i = 0; i <= m_numberDays; ++i)
      {
        // value from this point
        double currValue = output[outputIdx - i];
        assert(currValue >= 0.0);
        assert(currValue <= 100.0);

        // remap to [-1.0, 1.0]
        currValue = -1.0 + (0.02 * currValue);

        nnetPoint.input.push_back(currValue);
      }

      // keep track of how many points we've modified
      ++numModified;

      // decrement the pointers
      --datasetIdx;
      --outputIdx;
    }

    return true;
  }

} // namespace alch
