
#include "afwk/Framework.h"
#include "afwk/PathRegistry.h"

#include "boost/filesystem/convenience.hpp"
#include "boost/filesystem/exception.hpp"

#include <sstream>
#include <iostream>
#include <exception>
#include <stdlib.h>

namespace alch {

  const char* const Framework::s_optionHelp = "help";

  const char* const Framework::s_optionVersion = "version";

  const char* const Framework::s_optionVerbose = "verbose";

  const char* const Framework::s_optionDebug = "debug";

  const char* const Framework::s_optionSeed = "seed";

  Framework::Framework()
    : m_ctx()
    , m_options()
    , m_debugLevel(0)
    , m_seed(0)
  {
  }


  Framework::~Framework()
  {
  }


  Framework::AppReturnCode Framework::run(int argc, char** argv)
  {
    boost::posix_time::ptime startTime(
      boost::posix_time::second_clock::local_time());

    if (!initialize())
    {
      getContext() << Context::PRIORITY_error
                   << "Failure during application initialization"
                   << Context::endl;
      return APPRETURNCODE_initializeFailure;
    }

    {
      std::stringstream ss;
      ss << "Starting up application ";
      printVersion(ss);
      getContext() << Context::PRIORITY_info << ss.str() << Context::endl;
    }

    getContext() << Context::PRIORITY_info
                 << "Processing options"
                 << Context::endl;

    OptionsReturnCode optRetCode = OPTIONSRETURNCODE_continue;

    try
    {
      optRetCode = processOptions(argc, argv);
    }
    catch (boost::program_options::error err)
    {
      getContext() << Context::PRIORITY_error << err.what() << Context::endl;
      optRetCode = OPTIONSRETURNCODE_error;
    }

    switch (optRetCode)
    {
      case OPTIONSRETURNCODE_continue:
        break;

      case OPTIONSRETURNCODE_stop:
        return APPRETURNCODE_success;

      case OPTIONSRETURNCODE_error:
      default:
        return APPRETURNCODE_optionsFailure;
    }

    // general initialization stuff
    srand48(m_seed);

    getContext() << Context::PRIORITY_debug1
                 << "Using random number generator seed: " << m_seed
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Running application logic"
                 << Context::endl;

    if (!processApplication())
    {
      getContext() << Context::PRIORITY_error
                   << "Failure during application execution"
                   << Context::endl;
      return APPRETURNCODE_application;
    }


    getContext() << Context::PRIORITY_info
                 << "Finalizing application"
                 << Context::endl;

    if (!finalize())
    {
      getContext() << Context::PRIORITY_error
                   << "Failure during application finalization"
                   << Context::endl;
      return APPRETURNCODE_finalizeFailure;
    }

    // print out elapsed time
    boost::posix_time::ptime endTime(
      boost::posix_time::second_clock::local_time());
    boost::posix_time::time_duration elapsed = endTime - startTime;
    printElapsed(elapsed);
    
    return APPRETURNCODE_success;
  }


  bool Framework::initialize()
  {
    // Set up context
    getContext().setPriorityFilter(Context::PRIORITY_error
                                   | Context::PRIORITY_warning);

    // initialize generic options
    {
      std::stringstream ssHelp;
      ssHelp << s_optionHelp << "," << s_optionHelp[0];

      std::stringstream ssVersion;
      ssVersion << s_optionVersion << "," << s_optionVersion[0];

      std::stringstream ssVerbose;
      ssVerbose << s_optionVerbose;

      getOptions().getGenericOptions().add_options()
        (ssVersion.str().c_str(), "Print application version")
        (ssHelp.str().c_str(), "Print usage message")    
      ;

      getOptions().getConfigOptions().add_options()
        (ssVerbose.str().c_str(),
         "Verbose output")
        (s_optionSeed,
         boost::program_options::value<int>(&m_seed)->default_value(time(0)),
         "Seed to use for random number generator")
      ;

      getOptions().getHiddenOptions().add_options()
        (s_optionDebug, 
         boost::program_options::value<int>(&m_debugLevel)->default_value(0),
         "Set debug level (0-3)")
      ;
    }

    // initialize directories
    if (!createDirectories())
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to create one or more application directories"
                   << Context::endl;
      return false;
    }

    return true;
  }


  bool Framework::finalize()
  {
    return true;
  }


  Framework::OptionsReturnCode Framework::processOptions(int argc, char** argv)
  {
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();

    options.process(argc, argv);


    // handle context setup options    
    if (vm.count(s_optionVerbose))
    {
      getContext().setPriorityFilter(getContext().getPriorityFilter()
                                     | Context::PRIORITY_info);
      getContext() << Context::PRIORITY_info << "Using verbose mode"
                   << Context::endl;
    }

    if (getDebugLevel() > 0)
    {
      int val = Context::PRIORITY_info | Context::PRIORITY_debug1;

      if (getDebugLevel() >= 2)
      {
        val |= Context::PRIORITY_debug2;

        if (getDebugLevel() >= 3)
        {
          val |= Context::PRIORITY_debug3;
        }
      }

      getContext().setPriorityFilter(getContext().getPriorityFilter() | val);

      getContext() << Context::PRIORITY_info
                   << "Using debug level "
                   << getDebugLevel()
                   << Context::endl;
    }

    printConfigFiles();

    // process generic options that may halt program
    if (vm.count(s_optionHelp))
    {
      printUsage(std::cout);
      return OPTIONSRETURNCODE_stop;
    }
    
    if (vm.count(s_optionVersion))
    {
      printVersion(std::cout);
      std::cout << "\n";
      return OPTIONSRETURNCODE_stop;
    }

    return OPTIONSRETURNCODE_continue;
  }


  void Framework::printVersion(std::ostream& os)
  {
    os << getApplicationName()
       << " "
       << getApplicationVersion();
  }


  void Framework::printUsage(std::ostream& os)
  {
      FrameworkOptions::Description visibleOptions("Allowable Options");
      visibleOptions
        .add(getOptions().getGenericOptions())
        .add(getOptions().getConfigOptions());

      std::cout << "\nUsage: " << getApplicationName() << " [options]\n\n"
                << getApplicationDescription()
                << "\n\n"
                << visibleOptions
                << "\n";
  }

  void Framework::printConfigFiles()
  {
    getContext() << Context::PRIORITY_info
                 << "User directory: " << PathRegistry::getUserDir()
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Data directory: " << PathRegistry::getDataDir()
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Preference file: " << PathRegistry::getPreferenceFile()
                 << Context::endl;  

    getContext() << Context::PRIORITY_info
                 << "Symbol file: " << PathRegistry::getSymbolFile()
                 << Context::endl;
  }


  bool Framework::createDirectories()
  {
    try
    {
      boost::filesystem::path dataDir(PathRegistry::getDataDir(),
                                      boost::filesystem::native);
      boost::filesystem::path userDir(PathRegistry::getUserDir(),
                                      boost::filesystem::native);

      boost::filesystem::create_directories(dataDir);
      boost::filesystem::create_directories(userDir);
    }
    catch (boost::filesystem::filesystem_error err)
    {
      getContext() << Context::PRIORITY_error
                   << err.what()
                   << Context::endl;
      return false;
    }

    return true;
  }


  void Framework::printElapsed(const boost::posix_time::time_duration& elapsed)
  {
    getContext() << Context::PRIORITY_info
                 << "Total elapsed time: "
                 << std::setw(2) << std::setfill('0') << elapsed.hours()
                 << ":"
                 << std::setw(2) << std::setfill('0') << elapsed.minutes()
                 << ":"
                 << std::setw(2) << std::setfill('0') << elapsed.seconds()
                 << Context::endl;    
  }

} // namespace alch
