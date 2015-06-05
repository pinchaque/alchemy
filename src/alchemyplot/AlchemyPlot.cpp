#include "alchemyplot/AlchemyPlot.h"

#include "afwk/PathRegistry.h"
#include "afwk/FrameworkUtils.h"
#include "stockplot/PricePlotDataCreator.h"
#include "stockplot/MAPlotDataCreator.h"
#include "stockplot/PSARPlotDataCreator.h"
#include "stockplot/ROCPlotDataCreator.h"
#include "stockplot/RSIPlotDataCreator.h"
#include "stockplot/ProfilePlotDataCreator.h"
#include "stockplot/CSVPlotDisplay.h"
#include "stockplot/GnuPlotDisplay.h"
#include "stockdata/StockDataRetriever.h"
#include "stockdata/StockTimeUtil.h"
#include "stocknnet/DatasetGeneratorBasic.h"
#include "stocknnet/PredictionProfile.h"
#include "stocknnet/ProfileIO.h"

#include "boost/tokenizer.hpp"

#include <sstream>

namespace alch {

  const char* const AlchemyPlot::s_optionSymbol = "symbol";
  const char* const AlchemyPlot::s_optionLinear = "linear";
  const char* const AlchemyPlot::s_optionAdjusted = "adjusted";
  const char* const AlchemyPlot::s_optionCSV = "csv";
  const char* const AlchemyPlot::s_optionMA20 = "ma20";
  const char* const AlchemyPlot::s_optionMA50 = "ma50";
  const char* const AlchemyPlot::s_optionPSAR = "psar";
  const char* const AlchemyPlot::s_optionROC = "roc";
  const char* const AlchemyPlot::s_optionRSI = "rsi";
  const char* const AlchemyPlot::s_optionProfile = "profile";
  
  AlchemyPlot::AlchemyPlot()
    : Framework()
    , m_symbol("")
    , m_startTime(boost::posix_time::second_clock::local_time())
    , m_endTime(boost::posix_time::second_clock::local_time())
    , m_data()
    , m_plotListPtr()
  {
    ;
  }


  AlchemyPlot::~AlchemyPlot()
  {
    ;
  }

  std::string AlchemyPlot::getApplicationDescription() const
  {
    std::stringstream ss;
    ss <<
      "This application displays plots of stock data along with technical \n"
      "analysis information for the stock.\n";

    return ss.str();
  }

  bool AlchemyPlot::initialize()
  {
    return Framework::initialize();
  }

  bool AlchemyPlot::finalize()
  {
    return Framework::finalize();
  }

  Framework::OptionsReturnCode AlchemyPlot::processOptions(
    int argc, char** argv)
  {
    std::stringstream ssSymbol;
    ssSymbol << s_optionSymbol << "," << s_optionSymbol[0];
    
    std::stringstream ssLinear;
    ssLinear << s_optionLinear << "," << s_optionLinear[0];

    std::stringstream ssAdjusted;
    ssAdjusted << s_optionAdjusted << ",j";

    getOptions().getGenericOptions().add_options()
      (ssSymbol.str().c_str(), 
       boost::program_options::value<std::string>(),
       "Specifies the symbol for which the plot will be created")
      (FrameworkUtils::getOptionStart(), 
       boost::program_options::value<std::string>(),
       "Start of date range for plot")
      (FrameworkUtils::getOptionEnd(),
       boost::program_options::value<std::string>(),
       "End of date range for plot")
      (FrameworkUtils::getOptionDuration(), 
       boost::program_options::value<int>(),
       "Number of days to plot (can be overridden by start/end)")
      (ssLinear.str().c_str(), 
       "Specifies that plot should be linear (default is logarithmic)")
      (ssAdjusted.str().c_str(), 
       "Specifies that plot should use dividend- and split-adjusted data")
      (s_optionCSV, 
       boost::program_options::value<std::string>(),
       "Outputs CSV-format plot data to specified file")
      (s_optionMA20, "Plots 20-day moving average")
      (s_optionMA50, "Plots 50-day moving average")
      (s_optionPSAR, "Plots Parabolic Stop-and-Reversal")
      (s_optionROC, "Plots Rate Of Change")
      (s_optionRSI, "Plots Relative Strength Indicator")
      (s_optionProfile,
       boost::program_options::value<std::string>(),
       "Plots specified profiles (comma-separated)")
      ;

    return Framework::processOptions(argc, argv);
  }


  bool AlchemyPlot::processApplication()
  {
    if (!getSymbol())
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to get symbol for the plot"
                   << Context::endl;
      return false;
    }

    if (!setDateRange())
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to set date range for plot"
                   << Context::endl;
      return false;
    }

    if (!retrieveData())
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to retrieve data for plot"
                   << Context::endl;
      return false;
    }

    if (!initializePlot())
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to initialize plot"
                   << Context::endl;
      return false;
    }

    if (!showPlot())
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to show plot"
                   << Context::endl;
      return false;
    }

    return true;
  }



  bool AlchemyPlot::getSymbol()
  {
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();

    if (vm.count(s_optionSymbol))
    {
      m_symbol = vm[s_optionSymbol].as<std::string>();
    }

    if (m_symbol.empty())
    {
      getContext() << Context::PRIORITY_error
                   << "Symbol not specified"
                   << Context::endl;
      return false;
    }

    return true;
  }


  bool AlchemyPlot::setDateRange()
  {
    FrameworkOptions::VariablesMap& vm = getOptions().getVariablesMap();
    if (!FrameworkUtils::getDateRange(vm, m_startTime, m_endTime,
                                      getContext()))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to get date range"
                   << Context::endl;
      return false;
    }

    return true;
  }


  bool AlchemyPlot::retrieveData()
  {
    StockDataRetriever retriever(PathRegistry::getDataDir(), getContext());
    StockID id(m_symbol);
    m_data = RangeDataPtr(new RangeData);

    // we want to retrieve *all* data for this stock so that we can compute
    // things like moving averages correctly.
    StockTime dataStartTime(boost::posix_time::min_date_time);
    StockTime dataEndTime(boost::posix_time::second_clock::local_time());
    dataEndTime = StockTimeUtil::getPreviousClose(dataEndTime);

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


    // account for adjusted data
    FrameworkOptions::VariablesMap& vm = getOptions().getVariablesMap();
    if (vm.count(s_optionAdjusted))
    {
      getContext() << Context::PRIORITY_debug1
                   << "Using adjusted data for plot"
                   << Context::endl;
      m_data->useAdjusted();
    }


    return true;
  }

 
  bool AlchemyPlot::initializeMAPlot(PlotPtr plot, int span)
  {
    MAPlotDataCreator creator(getContext());
    creator.setSpan(span);
    creator.setData(m_data);
    creator.setID(m_symbol);

    // populate the plot
    return creator.create(plot);
  }


  bool AlchemyPlot::initializePlot()
  {
    m_plotListPtr = PlotListPtr(new PlotList);
    m_plotListPtr->setXMin(m_startTime);
    m_plotListPtr->setXMax(m_endTime);

    return (initializePricePlot()
            && initializeROCPlot()
            && initializeRSIPlot());
  }


  bool AlchemyPlot::initializePricePlot()
  {
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();

    bool logarithmic = !vm.count(s_optionLinear);

    // initialize the plot
    PlotPtr plot(new Plot);
    plot->setLogarithmic(logarithmic);
    plot->setWeight(30.0);
    plot->setYAxisLabel("Price");
    plot->setShowKey(true);

    // plot the price
    {
      PricePlotDataCreator creator(getContext());
      creator.setData(m_data);
      creator.setID(m_symbol);

      // populate the plot
      if (!creator.create(plot))
      {    
        getContext() << Context::PRIORITY_error
                     << "Failed to create price plot"
                     << Context::endl;
        return false;
      }
    }

    // plot the 20-day moving average
    if (vm.count(s_optionMA20) && !initializeMAPlot(plot, 20))
    {
        getContext() << Context::PRIORITY_error
                     << "Failed to create 20-day MA plot"
                     << Context::endl;
        return false;
    }

    // plot the 50-day moving average
    if (vm.count(s_optionMA50) && !initializeMAPlot(plot, 50))
    {
        getContext() << Context::PRIORITY_error
                     << "Failed to create 50-day MA plot"
                     << Context::endl;
        return false;
    }

    // plot the profile
    if (vm.count(s_optionProfile)
        && !initializeProfilePlot(plot, vm[s_optionProfile].as<std::string>()))
    {
        getContext() << Context::PRIORITY_error
                     << "Failed to create profile plot"
                     << Context::endl;
        return false;
    }


    if (vm.count(s_optionPSAR))
    {
      PSARPlotDataCreator creator(getContext());
      creator.setAccel(0.02);
      creator.setMaxAccel(0.20);
      creator.setData(m_data);
      creator.setID(m_symbol);

      // populate the plot
      if (!creator.create(plot))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to create parabolic SAR plot"
                     << Context::endl;
        return false;
      }
    }


    m_plotListPtr = PlotListPtr(new PlotList);
    m_plotListPtr->setXMin(m_startTime);
    m_plotListPtr->setXMax(m_endTime);
    m_plotListPtr->addPlot(plot);

    return true;
  }

  bool AlchemyPlot::initializeROCPlot()
  {
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();

    // plot the price
    if (vm.count(s_optionROC))
    {
      // initialize the plot
      PlotPtr plot(new Plot);
      plot->setWeight(10.0);

      ROCPlotDataCreator creator(getContext());
      creator.setData(m_data);
      creator.setID(m_symbol);

      // populate the plot
      if (!creator.create(plot))
      {    
        getContext() << Context::PRIORITY_error
                     << "Failed to create ROC plot"
                     << Context::endl;
        return false;
      }

      m_plotListPtr->addPlot(plot);
    }

    return true;
  }

  bool AlchemyPlot::initializeRSIPlot()
  {
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();

    // plot the price
    if (vm.count(s_optionRSI))
    {
      // initialize the plot
      PlotPtr plot(new Plot);
      plot->setWeight(10.0);

      RSIPlotDataCreator creator(getContext());
      creator.setData(m_data);
      creator.setID(m_symbol);

      // populate the plot
      if (!creator.create(plot))
      {    
        getContext() << Context::PRIORITY_error
                     << "Failed to create RSI plot"
                     << Context::endl;
        return false;
      }

      m_plotListPtr->addPlot(plot);
    }

    return true;
  }


  bool AlchemyPlot::initializeProfilePlot(PlotPtr plot,
                                          const std::string& profileList)
  {
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
    tokenizer tok(profileList);

    tokenizer::iterator end = tok.end();
    tokenizer::iterator iter;
    for (iter = tok.begin(); iter != end; ++iter)
    {
      getContext() << Context::PRIORITY_debug1
                   << "Creating plot for profile '" << *iter << "'"
                   << Context::endl;

      PredictionProfile profile;

      if (!ProfileIO::exists(iter->c_str()))
      {
        getContext() << Context::PRIORITY_warning
                     << "Prediction profile '"
                     << *iter << "' does not exist; skipping"
                     << Context::endl;
        continue;
      }
      else if (!ProfileIO::read(iter->c_str(), profile, getContext()))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to read prediction profile '"
                     << *iter << "'"
                     << Context::endl;
        return false;
      }
      else
      {
        getContext() << Context::PRIORITY_debug1
                     << "-- Name: " << profile.getName()
                     << Context::endl;
        getContext() << Context::PRIORITY_debug1
                     << "-- Number of Days: " << profile.getNumberDays()
                     << Context::endl;
      }

      DatasetGeneratorBasic generator(getContext());
      generator.setNumberDays(profile.getNumberDays());

      ProfilePlotDataCreator creator(getContext(), profile, generator);
      creator.setData(m_data);
      creator.setID(m_symbol);

      // populate the plot
      if (!creator.create(plot))
      {    
        getContext() << Context::PRIORITY_error
                     << "Failed to create profile plot"
                     << Context::endl;
        return false;
      }
    }

    return true;
  }


  bool AlchemyPlot::showPlot()
  {
    // show the plot
    GnuPlotDisplay display(getContext());
    if (!display.show(m_plotListPtr))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to create plot"
                   << Context::endl;
      return false;
    }


    // CSV output if requested
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();
    if (vm.count(s_optionCSV))
    {
      std::string fileName = vm[s_optionCSV].as<std::string>();
      CSVPlotDisplay csvDisplay(getContext(), fileName);
      if (!csvDisplay.show(m_plotListPtr))
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to create CSV file '" << fileName << "'"
                     << Context::endl;
        return false;
      }
    }


    return true;
  }

} // namespace alch
