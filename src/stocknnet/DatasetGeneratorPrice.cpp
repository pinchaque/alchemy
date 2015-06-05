
#include "stocknnet/DatasetGeneratorPrice.h"


namespace alch {

  bool DatasetGeneratorPrice::generate(const RangeDataPtr& rangeDataPtr,
                                       NNetDataset& dataset)
  {
    int numberDays = getNumberDays();
    int rangeDataSize = rangeDataPtr->size() - numberDays;
    dataset.clear();
    dataset.resize(rangeDataSize);

    for (int i = 0; i < rangeDataSize; ++i)
    {
      double mappedCloseCurr =
        ((rangeDataPtr->get(i).close - 50.0 ) / 50.0);
      double mappedCloseFuture =
        ((rangeDataPtr->get(i + numberDays).close - 50.0) / 50.0);
      dataset[i].input.push_back(mappedCloseCurr);
      dataset[i].output.push_back(mappedCloseFuture);
    }

    return true;
  }

  bool DatasetGeneratorPrice::generateInputs(const RangeDataPtr& rangeDataPtr,
                                             NNetDataset& dataset)
  {
    int rangeDataSize = rangeDataPtr->size();
    dataset.clear();
    dataset.resize(rangeDataSize);

    for (int i = 0; i < rangeDataSize; ++i)
    {
      double mappedClose = ((rangeDataPtr->get(i).close - 50.0 ) / 50.0);
      dataset[i].input.push_back(mappedClose);
    }

    return true;
  }

} // namespace alch
