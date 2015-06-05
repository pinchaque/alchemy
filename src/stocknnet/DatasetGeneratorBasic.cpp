
#include "stocknnet/DatasetGeneratorBasic.h"

#include "stocknnet/PopulateDataMA.h"
#include "stocknnet/PopulateDataPSAR.h"
#include "stocknnet/PopulateDataPrice.h"
#include "stocknnet/PopulateDataRSI.h"


namespace alch {

  bool DatasetGeneratorBasic::generate(const RangeDataPtr& rangeDataPtr,
                                       NNetDataset& dataset)
  {
    NNetDataset tmpDataset;

    // generate the inputs in dataset
    if (!generateInputs(rangeDataPtr, tmpDataset))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to generate inputs for neural network"
                   << Context::endl;
      return false;
    }
    else if (!tmpDataset.size())
    {
      getContext() << Context::PRIORITY_warning
                   << "Generated empty dataset for neural network"
                   << Context::endl;
      return true;
    }

    // generate the outputs in dataset
    if (!generateOutputs(rangeDataPtr, tmpDataset))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to generate outputs for neural network"
                   << Context::endl;
      return false;
    }


    // now append the dataset we created to the one that was passed in
    dataset.insert(dataset.end(), tmpDataset.begin(), tmpDataset.end());

    return true;
  }

  namespace {
    inline int getInputsIndex(const NNetDataset& dataset)
    {
      if (dataset.size() > 0)
      {
        return int(dataset[dataset.size() - 1].input.size());
      }
      else
      {
        return -1;
      }
    }
  }


  /*
    Each populator tells us how many data records it added. All data records
    are added starting at the end of dataset. minimumFill is thus the smallest
    number of data records modified by any algorithm, and is then the
    number of records at the end that we keep.

    The startPointIdx / endPointIdx / getInputsIndex() stuff is only used
    for debugging.
  */

  bool DatasetGeneratorBasic::generateInputs(const RangeDataPtr& rangeDataPtr,
                                             NNetDataset& dataset)
  {
    RangeDataPtr sumRangeData(new RangeData);
    rangeDataPtr->summarize(*sumRangeData, getNumberDays());

    const int historySize = 3;

    NNetDataset tmpDataset;

    // resize dataset to hold at least one point for each input data point
    tmpDataset.resize(rangeDataPtr->size());

    // keep track of the minimum number of things that were put into dataset
    int minimumFill = int(tmpDataset.size());

    ////////////////////////////////////////////////////////////
    // price
    {
      int startPointIdx = getInputsIndex(tmpDataset);

      PopulateDataPrice pop(getContext());
      pop.setNumberDays(historySize);

      int numModified = 0;

      if (!pop.populate(sumRangeData, tmpDataset, numModified))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to populate dataset with price data"
                     << Context::endl;
        return false;
      }

      if (numModified < minimumFill)
      {
        minimumFill = numModified;
      }

      int endPointIdx = getInputsIndex(tmpDataset) - 1;
      getContext() << Context::PRIORITY_debug1
                   << "Data points " << startPointIdx << " to "
                   << endPointIdx << " are price history"
                   << Context::endl;
    }


    ////////////////////////////////////////////////////////////
    // 2N-day Moving average
    {
      int movingAvgSpan = 2 * getNumberDays();

      int startPointIdx = getInputsIndex(tmpDataset);

      PopulateDataMA pop(getContext());
      pop.setNumberDays(historySize);
      pop.setSpan(movingAvgSpan);

      int numModified = 0;

      if (!pop.populate(rangeDataPtr, tmpDataset, numModified))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to populate dataset with moving average data"
                     << Context::endl;
        return false;
      }

      if (numModified < minimumFill)
      {
        minimumFill = numModified;
      }
      
      int endPointIdx = getInputsIndex(tmpDataset) - 1;
      getContext() << Context::PRIORITY_debug1
                   << "Data points " << startPointIdx << " to "
                   << endPointIdx << " are "
                   << movingAvgSpan << "-day MA"
                   << Context::endl;
    }

    ////////////////////////////////////////////////////////////
    // N-Day Moving average
    {
      int movingAvgSpan = getNumberDays();

      int startPointIdx = getInputsIndex(tmpDataset);

      PopulateDataMA pop(getContext());
      pop.setNumberDays(historySize);
      pop.setSpan(movingAvgSpan);

      int numModified = 0;

      if (!pop.populate(rangeDataPtr, tmpDataset, numModified))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to populate dataset with moving average data"
                     << Context::endl;
        return false;
      }

      if (numModified < minimumFill)
      {
        minimumFill = numModified;
      }
      
      int endPointIdx = getInputsIndex(tmpDataset) - 1;
      getContext() << Context::PRIORITY_debug1
                   << "Data points " << startPointIdx << " to "
                   << endPointIdx << " are "
                   << movingAvgSpan << "-day MA"
                   << Context::endl;
    }

    ////////////////////////////////////////////////////////////
    // RSI
    {
      int startPointIdx = getInputsIndex(tmpDataset);

      PopulateDataRSI pop(getContext());
      pop.setNumberDays(historySize);
      pop.setSpan(14);

      int numModified = 0;

      if (!pop.populate(sumRangeData, tmpDataset, numModified))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to populate dataset with RSI data"
                     << Context::endl;
        return false;
      }

      if (numModified < minimumFill)
      {
        minimumFill = numModified;
      }
      
      int endPointIdx = getInputsIndex(tmpDataset) - 1;
      getContext() << Context::PRIORITY_debug1
                   << "Data points " << startPointIdx << " to "
                   << endPointIdx << " are RSI"
                   << Context::endl;
    }

    ////////////////////////////////////////////////////////////
    // PSAR
    {
      int startPointIdx = getInputsIndex(tmpDataset);

      PopulateDataPSAR pop(getContext());
      pop.setNumberDays(historySize);
      pop.setAccel(0.02);
      pop.setMaxAccel(0.20);

      int numModified = 0;

      if (!pop.populate(sumRangeData, tmpDataset, numModified))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to populate dataset with PSAR data"
                     << Context::endl;
        return false;
      }

      if (numModified < minimumFill)
      {
        minimumFill = numModified;
      }
      
      int endPointIdx = getInputsIndex(tmpDataset) - 1;
      getContext() << Context::PRIORITY_debug1
                   << "Data points " << startPointIdx << " to "
                   << endPointIdx << " are PSAR"
                   << Context::endl;
    }

    // print out debugging stuff
    getContext() << Context::PRIORITY_debug2
                 << "generateInputs: tmpDataset.size() = "
                 << tmpDataset.size()
                 << Context::endl;

    getContext() << Context::PRIORITY_debug2
                 << "generateInputs: minimumFill = " << minimumFill
                 << Context::endl;


    int numSkip = 0;
    if (minimumFill < int(tmpDataset.size()))
    {
      numSkip = tmpDataset.size() - minimumFill;
      // now we need to use minimumFill to hack off incomplete inputs
      tmpDataset.erase(tmpDataset.begin(),
                       tmpDataset.begin() + numSkip);
    }

    getContext() << Context::PRIORITY_debug2
                 << "generateInputs: hacked off " << numSkip << " points"
                 << Context::endl;

    getContext() << Context::PRIORITY_debug2
                 << "generateInputs: new tmpDataset.size() =  "
                 << tmpDataset.size()
                 << Context::endl;

    getContext() << Context::PRIORITY_debug2
                 << "generateInputs: #inputs = "
                 << (tmpDataset.size()
                     ? tmpDataset[tmpDataset.size() - 1].input.size()
                     : 0)
                 << Context::endl;

#if 0
    for (int i = 0; i < int(tmpDataset.size()); ++i)
    {
      getContext() << Context::PRIORITY_debug2
                   << "dataset[" << i << "].input = { "
                   << tmpDataset[i].input[0] << ", "
                   << tmpDataset[i].input[1] << ", "
                   << tmpDataset[i].input[2] << ", "
                   << tmpDataset[i].input[3] << "... }"
                   << Context::endl;
    }
#endif

    // now append the dataset we created to the one that was passed in
    dataset.insert(dataset.end(), tmpDataset.begin(), tmpDataset.end());

    return true;
  }


  bool DatasetGeneratorBasic::generateOutputs(const RangeDataPtr& rangeDataPtr,
                                              NNetDataset& dataset)
  {
    // for each input in dataset, we now add the output that is 
    // getNumberDays() in the future. This means that the last getNumberDays()
    // datapoints will not have an output associated with them.
    
    if (getNumberDays() >= int(dataset.size()))
    {
      getContext() << Context::PRIORITY_warning
                   << "Not enough data points (" << dataset.size() 
                   << ") generated for requested number of days ("
                   << getNumberDays() << ")"
                   << Context::endl;
      return true;
    }

    getContext() << Context::PRIORITY_debug2
                 << "generateOutputs: Original dataset size: "
                 << dataset.size() 
                 << Context::endl;

    // start off by chopping off last numDays datapoints since they will 
    // not have an output
    dataset.erase(dataset.begin() + dataset.size() - getNumberDays(),
                  dataset.end());

    getContext() << Context::PRIORITY_debug2
                 << "generateOutputs: Target dataset size: "
                 << dataset.size() 
                 << Context::endl;

    getContext() << Context::PRIORITY_debug2
                 << "generateOutputs: rangeData size: "
                 << rangeDataPtr->size() 
                 << Context::endl;

    // now work backward through rangeData and dataset, assigning
    // outputs
    int i = dataset.size() - 1;
    int j = rangeDataPtr->size() - 1;

    // we need at least getNumberDays() more points in j so that we can
    // compare future closing price against current one
    assert(j - i > getNumberDays());

    while ((i >= 0) && (j >= 0))
    {
      double currClose = rangeDataPtr->get(j - getNumberDays()).close;
      double futureClose = rangeDataPtr->get(j).close;
      double relativeClose = (futureClose - currClose) / currClose;
      dataset[i].output.push_back(relativeClose);

      --i;
      --j;
    }

    // we should have definitely populated all of dataset since there
    // should have been enough points in rangeDataPtr. i has been decremented
    // below zero to exit the loop.
    assert(i == -1);

    return true;
  }

} // namespace alch
