#include "alchemyprofile/AlchemyProfile.h"

#include "afwk/PathRegistry.h"
#include "stocknnet/DatasetGeneratorBasic.h"
#include "stocknnet/PredictionProfile.h"
#include "stocknnet/ProfileIO.h"
#include "stockdata/StockDataRetriever.h"
#include "stockdata/YahooStockDataSource.h"
#include "stockdata/StockTimeUtil.h"
#include "nnet/NeuralNetAlg.h"
#include "nnet/Statistics.h"

#include "boost/tokenizer.hpp"
#include "boost/filesystem/operations.hpp"

#include <vector>
#include <string>
#include <fstream>

namespace alch {

  const char* const AlchemyProfile::s_optionSymbol = "symbol";
  const char* const AlchemyProfile::s_optionOutput = "output";
  const char* const AlchemyProfile::s_optionProfile = "profile";

  AlchemyProfile::AlchemyProfile()
    : Framework()
    , m_outputFile("")
    , m_profiles()
  {
    ;
  }


  AlchemyProfile::~AlchemyProfile()
  {
    ;
  }


  std::string AlchemyProfile::getApplicationDescription() const
  {
    std::stringstream ss;
    ss <<
      "This application is responsible for profiling all or \n"
      "specified symbols. If the file\n" << PathRegistry::getSymbolFile() <<
      "\nexists then only those symbols will be profiled. Otherwise, all\n"
      "symbols will be profiled. Profiling output is saved to the specified\n"
      "output file in CSV format.\n"
      ;

    return ss.str();
  }

  bool AlchemyProfile::initialize()
  {
    return Framework::initialize();
  }

  bool AlchemyProfile::finalize()
  {
    return Framework::finalize();
  }

  Framework::OptionsReturnCode AlchemyProfile::processOptions(
    int argc, char** argv)
  {
    std::stringstream ssSymbol;
    ssSymbol << s_optionSymbol << "," << s_optionSymbol[0];

    getOptions().getGenericOptions().add_options()
      (ssSymbol.str().c_str(), 
       boost::program_options::value<std::string>(),
       "Specifies the symbol to read")
      (s_optionProfile,
       boost::program_options::value<std::string>(),
       "Specifies profiles to use (comma-separated)")
      (s_optionOutput,
       boost::program_options::value<std::string>(),
       "Specifies output file name")
      ;

    return Framework::processOptions(argc, argv);
  }


  bool AlchemyProfile::processApplication()
  {
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();

    if (!vm.count(s_optionProfile))
    {
      getContext() << Context::PRIORITY_error
                   << "Option not specified: -" << s_optionProfile
                   << Context::endl;
      return false;
    }
    else if (!initializeProfiles(vm[s_optionProfile].as<std::string>()))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to process profile list"
                   << Context::endl;
      return false;
    }


    if (!vm.count(s_optionOutput))
    {
      getContext() << Context::PRIORITY_error
                   << "Option not specified: -" << s_optionOutput
                   << Context::endl;
      return false;
    }
    m_outputFile = vm[s_optionOutput].as<std::string>();


    VecStockID symbolList;
    if (vm.count(s_optionSymbol))
    {
      std::string symbol(vm[s_optionSymbol].as<std::string>());
      
      getContext() << Context::PRIORITY_debug1
                   << "User --symbol=" << symbol
                   << Context::endl;

      symbolList.push_back(StockID(symbol));
    }
    else if (!retrieveSymbolList(symbolList))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to retrieve symbol list"
                   << Context::endl;
      return false;
    }
    else if (!symbolList.size())
    {
      getContext() << Context::PRIORITY_error
                   << "Empty symbol list"
                   << Context::endl;
      return false;
    }

    return processSymbolList(symbolList);
  }


  bool AlchemyProfile::getList(VecStockID& readList,
                               const std::string& file,
                               const std::string& item)
  {
    std::ifstream ifs(file.c_str());
    if (!ifs)
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open file '" << file << "'"
                   << Context::endl;
      return false;
    }

    getContext() << Context::PRIORITY_debug2
                 << "Reading " << item << " file '" << file << "'"
                 << Context::endl;

    std::string line;

    while (std::getline(ifs, line))
    {
      // skip comment lines
      if (line.size() && (line[0] == '#'))
      {
        continue;
      }

      // tokenize line and add it to readList
      typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
      boost::char_separator<char> sep(" \t\r\n,");
      tokenizer tok(line, sep);
      tokenizer::iterator end = tok.end();
      tokenizer::iterator iter;

      for (iter = tok.begin(); iter != end; ++iter)
      {
        readList.push_back(*iter);

        getContext() << Context::PRIORITY_debug2
                     << "Read " << item << ": '" << *iter << "'"
                     << Context::endl;
      }
    }

    ifs.close();

    getContext() << Context::PRIORITY_debug1
                 << "Read " << readList.size()
                 << " " << item << "(s) from file '"
                 << file << "'" << Context::endl;

    return true;
  }


  bool AlchemyProfile::getSymbolList(VecStockID& symbolList,
                                     const std::string& symbolFile)
  {
    return getList(symbolList, symbolFile, "symbol");
  }


  bool AlchemyProfile::retrieveSymbolList(VecStockID& symbolList)
  {
    symbolList.clear();
    
    std::string symbolFile = PathRegistry::getSymbolFile();

    boost::filesystem::path symbolFilePath(symbolFile,
                                           boost::filesystem::native);

    if (boost::filesystem::exists(symbolFilePath))
    {
      if (!getSymbolList(symbolList, symbolFile))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to get symbol list from '" << symbolFile << "'"
                     << Context::endl;
        return false;
      }

      getContext() << Context::PRIORITY_info
                   << "Using list of symbols from '" << symbolFile << "'"
                   << Context::endl;
    }
    else
    {
      getContext() << Context::PRIORITY_debug1
                   << "Attempting to get symbols from Yahoo..."
                   << Context::endl;

      YahooStockDataSource yahoo(getContext());

      if (!yahoo.getStockList(symbolList))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to get symbol list from Yahoo"
                     << Context::endl;
        return false;
      }

      getContext() << Context::PRIORITY_info
                   << "Using list of symbols from Yahoo"
                   << Context::endl;
    }

    // empty symbol lists are suspicious, but not necessarily an error
    if (symbolList.empty())
    {
      getContext() << Context::PRIORITY_warning
                   << "Symbol list was empty"
                   << Context::endl;
    }

    return true;
  }


  bool AlchemyProfile::processSymbolList(const VecStockID& symbolList)
  {
    getContext() << Context::PRIORITY_info
                 << "Processing " << symbolList.size() << " symbols"
                 << Context::endl;

    // profile all symbols

    std::ofstream ofs(m_outputFile.c_str());
    if (!ofs)
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open file '" << m_outputFile
                   << "' for writing.."
                   << Context::endl;
      return false;
    }

    if (!printHeader(ofs))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to print header to '" << m_outputFile << "'"
                   << Context::endl;
      return false;
    }

    VecStockID::const_iterator end = symbolList.end();
    VecStockID::const_iterator iter;
    for (iter = symbolList.begin(); iter != end; ++iter)
    {
      if (!processSymbol(*iter, ofs))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to process symbol '" << *iter
                     << "'. Continuing execution."
                     << Context::endl;
        continue;
      }
    }

    return true;
  }


  bool AlchemyProfile::printHeader(std::ostream& os)
  {
    os << "Symbol,Price,Profile,NumberDays,Ratio,FuturePrice,OverallError,"
       << "Correlation\n";
    return os;
  }


  bool AlchemyProfile::processSymbol(const StockID& symbol,
                                     std::ostream& os)
  {
    getContext() << Context::PRIORITY_info
                 << "Processing " << symbol << "..."
                 << Context::endl;

    RangeDataPtr stockData;

    if (!retrieveData(symbol, stockData))
    {
      getContext() << Context::PRIORITY_error
                   << "Data retrieval failed for symbol '" << symbol << "'"
                   << Context::endl;
      return false;
    }

    assert(stockData.get());
    assert(stockData->size());

    // switch to using adjusted data
    stockData->useAdjusted();

    // calculate neural net dataset
    DatasetGeneratorBasic generator(getContext());
    NNetDataset dataset;
    if (!generator.generateInputs(stockData, dataset))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to generate neural network dataset for '"
                   << symbol << "'"
                   << Context::endl;
      return false;
    }
    else if (!dataset.size())
    {
      getContext() << Context::PRIORITY_warning
                   << "Empty neural network dataset for '"
                   << symbol << "'"
                   << Context::endl;
      return true;
    }

    // for each profile we must add to this CSV line
    std::vector<PredictionProfile>::const_iterator end = m_profiles.end();
    std::vector<PredictionProfile>::const_iterator iter;
    for (iter = m_profiles.begin(); iter != end; ++iter)
    {
      if (!processSymbolProfile(symbol, stockData, dataset, *iter, os))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to process profile '" << iter->getName()
                     << "' for symbol '" << symbol << "'"
                     << Context::endl;
        return false;
      }
    }

    return true;
  }


  bool AlchemyProfile::initializeProfiles(const std::string& profileList)
  {
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
    tokenizer tok(profileList);

    tokenizer::iterator end = tok.end();
    tokenizer::iterator iter;
    for (iter = tok.begin(); iter != end; ++iter)
    {
      getContext() << Context::PRIORITY_debug1
                   << "Loading prediction profile '" << *iter << "'"
                   << Context::endl;

      PredictionProfile profile;

      if (!ProfileIO::exists(iter->c_str()))
      {
        getContext() << Context::PRIORITY_error
                     << "Prediction profile '"
                     << *iter << "' does not exist"
                     << Context::endl;
        return false;
      }
      else if (!ProfileIO::read(iter->c_str(), profile, getContext()))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to read prediction profile '"
                     << *iter << "'"
                     << Context::endl;
        return false;
      }

      getContext() << Context::PRIORITY_debug1
                   << "-- Name: " << profile.getName()
                   << Context::endl;
      getContext() << Context::PRIORITY_debug1
                   << "-- Number of Days: " << profile.getNumberDays()
                   << Context::endl;

      m_profiles.push_back(profile);
    }

    return true;
  }


  bool AlchemyProfile::retrieveData(const StockID& symbol,
                                    RangeDataPtr& stockData)
  {
    StockDataRetriever retriever(PathRegistry::getDataDir(), getContext());
    stockData = RangeDataPtr(new RangeData);

    // we want to retrieve *all* data for this stock so that we can compute
    // things like moving averages correctly.
    StockTime dataStartTime(boost::posix_time::min_date_time);
    StockTime dataEndTime(boost::posix_time::second_clock::local_time());
    dataEndTime = StockTimeUtil::getPreviousClose(dataEndTime);

    if (!retriever.retrieve(symbol, dataStartTime, dataEndTime, *stockData))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to retrieve data for " << symbol
                   << Context::endl;
      return false;
    }
    else if (!stockData->size())
    {
      getContext() << Context::PRIORITY_error
                   << "No data returned for " << symbol
                   << Context::endl;
      return false;
    }

    getContext() << Context::PRIORITY_debug1
                 << "Retrieved " << stockData->size() << " data records"
                 << Context::endl;

    return true;
  }


  bool AlchemyProfile::processSymbolProfile(const StockID& symbol,
                                            RangeDataPtr rangeData,
                                            const NNetDataset& nnetDataset,
                                            const PredictionProfile& profile,
                                            std::ostream& os)
  {
    NNetDataset dataset = nnetDataset;
    int datasetSize = int(dataset.size());
    assert(datasetSize >= 1);
    NeuralNet neuralNet = profile.getNeuralNet();
    int numRangeDataPoints = int(rangeData->size());
    int startIdx = (numRangeDataPoints
                    - datasetSize
                    + profile.getNumberDays());

    // propagate all the inputs for the dataset we have.. this gives us
    // the predictions for rangeData.
    NeuralNetAlg::calculateOutputs(neuralNet, dataset);

    std::vector<double> predictions;
    std::vector<double> actuals;

    predictions.reserve(dataset.size());
    actuals.reserve(dataset.size());

    // fill in predictions and actuals arrays
    for (int idx = 0; idx + startIdx < numRangeDataPoints; ++idx)
    {
      assert(idx < datasetSize);
      assert(dataset[idx].output.size());

      double ratio = dataset[idx].output[0];

      int rangeIdx = startIdx + idx;

      assert(rangeIdx >= profile.getNumberDays());
      assert(rangeIdx < numRangeDataPoints);
      const RangeData::Point& referencePoint(
        rangeData->get(rangeIdx - profile.getNumberDays()));
      const RangeData::Point& futurePoint(rangeData->get(rangeIdx));

      double referenceClose = referencePoint.close;
      double predictedClose = (1.0 + ratio) * referenceClose;
      double actualClose = futurePoint.close;
        
      
      getContext() << Context::PRIORITY_debug2
                   << "Current: "
                   << boost::posix_time::to_simple_string(
                     referencePoint.tradeTime)
                   << "  Close: " << referenceClose
                   << Context::endl;
      
      getContext() << Context::PRIORITY_debug2
                   << "Future: "
                   << boost::posix_time::to_simple_string(
                     futurePoint.tradeTime)
                   << "  Ratio: " << ratio
                   << "  Predict: " << predictedClose
                   << "  Actual: " << actualClose
                   << Context::endl;


      predictions.push_back(predictedClose);
      actuals.push_back(actualClose);
    }

    assert(predictions.size() == actuals.size());
    assert(int(predictions.size()) == (datasetSize - profile.getNumberDays()));
    double error = NeuralNetAlg::squaredError(&predictions[0], &actuals[0],
                                              int(predictions.size()));
    double correl = Statistics::correlation(&predictions[0], &actuals[0],
                                            int(predictions.size()));

    double currValue = rangeData->get(numRangeDataPoints - 1).close;
    double predictRatio = dataset[datasetSize - 1].output[0];
    double predictValue = currValue * (1.0 + predictRatio);


    // print out values for this profile to CSV line
    os << symbol << ',' << currValue
       << ",\"" << profile.getName() << "\"," << profile.getNumberDays()
       << ',' << predictRatio << ',' << predictValue << ',' << error
       << ',' << correl << '\n';

    return os;
  }

} // namespace alch
