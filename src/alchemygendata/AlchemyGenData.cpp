#include "alchemygendata/AlchemyGenData.h"
#include "afwk/PathRegistry.h"
#include "afwk/FrameworkUtils.h"
#include "stockdata/StockDataRetriever.h"
#include "stockdata/StockTimeUtil.h"

#include "nnet/NNetDataset.h"
#include "nnet/NNetDataStream.h"
#include "stocknnet/DatasetGeneratorBasic.h"

#include <sstream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#include <algorithm>

namespace alch {

  const char* const AlchemyGenData::s_optionTrain = "train";
  const char* const AlchemyGenData::s_optionTest = "test";
  const char* const AlchemyGenData::s_optionDays = "days";
  const char* const AlchemyGenData::s_optionPercent = "percent";
  const char* const AlchemyGenData::s_optionSymbol = "symbol";
  const char* const AlchemyGenData::s_optionRandomize = "randomize";
  const char* const AlchemyGenData::s_optionSample = "sample";

  AlchemyGenData::AlchemyGenData()
    : Framework()
    , m_symbol("")
    , m_trainFile("")
    , m_testFile("")
    , m_daysAdvance(1)
    , m_trainRatio(0.80)
    , m_sampleRatio(1.00)
    , m_data()
  {
    ;
  }


  AlchemyGenData::~AlchemyGenData()
  {
    ;
  }

  std::string AlchemyGenData::getApplicationDescription() const
  {
    return
      "Generates training datasets for the neural network based on stock\n"
      "market data.\n";
  }

  bool AlchemyGenData::initialize()
  {
    return Framework::initialize();
  }

  bool AlchemyGenData::finalize()
  {
    return Framework::finalize();
  }

  Framework::OptionsReturnCode AlchemyGenData::processOptions(
    int argc, char** argv)
  {
    std::stringstream ssSymbol;
    ssSymbol << s_optionSymbol << "," << s_optionSymbol[0];

    getOptions().getGenericOptions().add_options()
      (ssSymbol.str().c_str(), 
       boost::program_options::value<std::string>(),
       "Specifies the symbol to generate data for")
      (FrameworkUtils::getOptionStart(), 
       boost::program_options::value<std::string>(),
       "Start of date range to generate data for")
      (FrameworkUtils::getOptionEnd(),
       boost::program_options::value<std::string>(),
       "End of date range to generate data for")
      (s_optionTrain,
       boost::program_options::value<std::string>(),
       "Name of output file for training data")
      (s_optionTest,
       boost::program_options::value<std::string>(),
       "Name of output file for testing data")
      (s_optionDays,
       boost::program_options::value<int>(),
       "Number of days in advance for the predicted price")
      (s_optionPercent,
       boost::program_options::value<double>(),
       "Percentage of data used for training (default 80%)")
      (s_optionSample,
       boost::program_options::value<double>(),
       "Percentage of data to sample (default 100%)")
      (s_optionRandomize, "Whether to randomize order of output data")
      ;

    return Framework::processOptions(argc, argv);
  }


  bool AlchemyGenData::processApplication()
  {
    // process parameters passed in by user
    if (!loadParams())
    {
      getContext() << Context::PRIORITY_error
                   << "Application failed while loading parameters"
                   << Context::endl;
      return false;
    }
    printParams();

    // load the stock data into memory
    if (!loadData())
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to retrieve data for specified symbol"
                   << Context::endl;
      return false;
    }

    // generate the neural network dataset based on loaded data
    NNetDataset dataset;
    if (!generateDataset(dataset))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to generate neural network dataset"
                   << Context::endl;
      return false;
    }

    // randomize the dataset
    if (!randomizeDataset(dataset))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to randomize neural network dataset"
                   << Context::endl;
      return false;
    }

    // handle the sampling and outputting of the dataset
    if (!outputDataset(dataset))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to output neural network dataset"
                   << Context::endl;
      return false;
    }

    return true;
  }


  bool AlchemyGenData::loadParams()
  {

    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();

    // get symbol
    if (vm.count(s_optionSymbol))
    {
      m_symbol = vm[s_optionSymbol].as<std::string>();
    }
    else
    {
      getContext() << Context::PRIORITY_error
                   << "Symbol not specified"
                   << Context::endl;
      return false;
    }

    // get training file name
    if (vm.count(s_optionTrain))
    {
      m_trainFile = vm[s_optionTrain].as<std::string>();
    }
    else
    {
      getContext() << Context::PRIORITY_error
                   << "Training file not specified"
                   << Context::endl;
      return false;
    }

    // get testing file name
    if (vm.count(s_optionTest))
    {
      m_testFile = vm[s_optionTest].as<std::string>();
    }
    else
    {
      getContext() << Context::PRIORITY_error
                   << "Testing file not specified"
                   << Context::endl;
      return false;
    }

    // get days in advance
    if (vm.count(s_optionDays))
    {
      m_daysAdvance = vm[s_optionDays].as<int>();
    }
    else
    {
      getContext() << Context::PRIORITY_error
                   << "Days in advance not specified"
                   << Context::endl;
      return false;
    }

    if (m_daysAdvance <= 0)
    {
      getContext() << Context::PRIORITY_error
                   << "Days in advance must be greater than 0"
                   << Context::endl;
      return false;
    }

    // get training data percentage
    if (vm.count(s_optionPercent))
    {
      m_trainRatio = vm[s_optionPercent].as<double>() / 100.0;
    }

    if ((m_trainRatio < 0.0) || (m_trainRatio > 1.0))
    {
      getContext() << Context::PRIORITY_error
                   << "Training data percentage must be between 0 and 100%"
                   << Context::endl;
      return false;
    }

    // get sample percentage
    if (vm.count(s_optionSample))
    {
      m_sampleRatio = vm[s_optionSample].as<double>() / 100.0;
    }

    if ((m_sampleRatio < 0.0) || (m_sampleRatio > 1.0))
    {
      getContext() << Context::PRIORITY_error
                   << "Sampling percentage must be between 0 and 100%"
                   << Context::endl;
      return false;
    }

    return true;
  }



  void AlchemyGenData::printParams()
  {
    getContext() << Context::PRIORITY_info
                 << "Symbol: " << m_symbol
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Training output file: " << m_trainFile
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Testing output file: " << m_testFile
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Days in advance: " << m_daysAdvance
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Training data ratio: " << m_trainRatio
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Data sampling ratio: " << m_sampleRatio
                 << Context::endl;
  }



  bool AlchemyGenData::loadData()
  {
    // load the data
    StockDataRetriever retriever(PathRegistry::getDataDir(), getContext());
    StockID id(m_symbol);
    m_data = RangeDataPtr(new RangeData);

    // we want to retrieve *all* data for this stock so that we can compute
    // things like moving averages correctly.
    StockTime dataStartTime(boost::posix_time::min_date_time);
    StockTime dataEndTime(boost::posix_time::second_clock::local_time());
    dataEndTime = StockTimeUtil::getPreviousClose(dataEndTime);

    FrameworkOptions::VariablesMap& vm = getOptions().getVariablesMap();
    if (vm.count(FrameworkUtils::getOptionStart())
        && vm.count(FrameworkUtils::getOptionEnd()))
    {
      dataStartTime = FrameworkUtils::stringToTime(
        vm[FrameworkUtils::getOptionStart()].as<std::string>());
      dataEndTime = FrameworkUtils::stringToTime(
        vm[FrameworkUtils::getOptionEnd()].as<std::string>());
    }

    getContext() << Context::PRIORITY_info
                 << "Retrieving data for range: ("
                 << dataStartTime << " - " << dataEndTime << ")"
                 << Context::endl;

    if (!retriever.retrieve(id, dataStartTime, dataEndTime, *m_data))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to retrieve data for " << id
                   << Context::endl;
      return false;
    }
    else if (!m_data->size())
    {
      getContext() << Context::PRIORITY_error
                   << "No data returned for " << id
                   << Context::endl;
      return false;
    }

    getContext() << Context::PRIORITY_debug1
                 << "Retrieved " << m_data->size() << " data records"
                 << Context::endl;

    // use adjusted data for all calculations
    m_data->useAdjusted();

    return true;
  }



  bool AlchemyGenData::generateDataset(NNetDataset& dataset)
  {
    DatasetGeneratorBasic generator(getContext());

    generator.setNumberDays(m_daysAdvance);

    // generate the dataset
    if (!generator.generate(m_data, dataset))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to generate neural network data for dataset"
                   << Context::endl;
      return false;
    }
    else if (!dataset.size())
    {
      getContext() << Context::PRIORITY_error
                   << "Generated empty neural network dataset"
                   << Context::endl;
      return false;
    }

    // sanity-check the dataset
    assert(dataset.size() > 0);
    int numInputs = dataset[0].input.size();
    int numOutputs = dataset[0].output.size();
    int numDataPoints = int(dataset.size());
 
    getContext() << Context::PRIORITY_debug1
                 << "Neural network dataset has " << numDataPoints
                 << " data points"
                 << Context::endl;
   
    for (int i = 1; i < numDataPoints; ++i)
    {
      int numInputsI = int(dataset[i].input.size());
      if (numInputsI != numInputs)
      {
        getContext() << Context::PRIORITY_error
                     << "Dataset point " << i << " has " << numInputsI 
                     << " inputs; expected " << numInputs
                     << Context::endl;
        return false;
      }

      int numOutputsI = int(dataset[i].output.size());
      if (numOutputsI != numOutputs)
      {
        getContext() << Context::PRIORITY_error
                     << "Dataset point " << i << " has " << numOutputsI 
                     << " outputs; expected " << numOutputs
                     << Context::endl;
        return false;
      }
    }

    getContext() << Context::PRIORITY_debug1
                 << "Each datapoint has " << numInputs << " input(s) and "
                 << numOutputs << " output(s)"
                 << Context::endl;

    return true;
  }



  bool AlchemyGenData::randomizeDataset(NNetDataset& dataset)
  {
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();

    if (vm.count(s_optionRandomize) && dataset.size())
    {
      int datasetSize = int(dataset.size());
      int steps =  datasetSize * 10;
      getContext() << Context::PRIORITY_info
                   << "Randomizing dataset for " << steps << " steps"
                   << Context::endl;
    
      for (int i = 0; i < steps; ++i)
      {
        int idx1 = static_cast<int>(drand48() * datasetSize);
        int idx2 = static_cast<int>(drand48() * datasetSize);

        if (idx1 != idx2)
        {
          std::swap(dataset[idx1], dataset[idx2]);
        }
      }
    }

    return true;
  }


  bool AlchemyGenData::outputDataset(const NNetDataset& dataset)
  {
    std::ofstream testFile(m_testFile.c_str(), std::ios::trunc);
    if (!testFile)
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open '" << m_testFile << "' for writing"
                   << Context::endl;
      return false;
    }

    std::ofstream trainFile(m_trainFile.c_str(), std::ios::trunc);
    if (!trainFile)
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open '" << m_trainFile << "' for writing"
                   << Context::endl;
      return false;
    }

    // variables to keep track of what went where
    int numTrain = 0;
    int numTest = 0;
    int total = 0;

    NNetDataset::const_iterator end = dataset.end();
    NNetDataset::const_iterator iter;
    for (iter = dataset.begin(); iter != end; ++iter)
    {
      ++total;

      // skip this datapoint if it doesn't pass the sampling frequency
      if (m_sampleRatio < drand48())
      {
        continue;
      }

      // testing datapoint
      if (m_trainRatio < drand48())
      {
        ++numTest;

        if (!NNetDataStream::writeDatapoint(testFile, *iter, getContext()))
        {
          getContext() << Context::PRIORITY_error
                       << "Failed to write datapoint to test file "
                       << m_testFile << Context::endl;
          return false;
        }
      }
      // training datapoint
      else
      {
        ++numTrain;

        if (!NNetDataStream::writeDatapoint(trainFile, *iter, getContext()))
        {
          getContext() << Context::PRIORITY_error
                       << "Failed to write datapoint to train file "
                       << m_trainFile << Context::endl;
          return false;
        }
      }
    }

    // print out some simple statistics on number of points
    {
      getContext() << Context::PRIORITY_info
                   << "Total number of points: " << total
                   << Context::endl;

      int numSaved = numTrain + numTest;
      double percentSaved = total ? (100.0 * numSaved / total) : 0.00;
      getContext() << Context::PRIORITY_info
                   << "Number of points saved: " << numSaved
                   << " ("
                   << std::setprecision(2)
                   << percentSaved << "%)"
                   << Context::endl;

      double percentTrain = total ? (100.0 * numTrain / numSaved) : 0.00;
      getContext() << Context::PRIORITY_info
                   << "Number of train points: " << numTrain
                   << " ("
                   << std::setprecision(2)
                   << percentTrain << "%)"
                   << Context::endl;

      double percentTest = total ? (100.0 * numTest / numSaved) : 0.00;
      getContext() << Context::PRIORITY_info
                   << "Number of test points:  " << numTest
                   << " ("
                   << std::setprecision(2)
                   << percentTest << "%)"
                   << Context::endl;
    }

    return true;
  }


} // namespace alch
