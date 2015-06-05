// -*- C++ -*-

#ifndef INCLUDED_afwk_Framework_h
#define INCLUDED_afwk_Framework_h

#include "autil/Context.h"

#include "afwk/FrameworkOptions.h"

#include "boost/date_time/posix_time/posix_time.hpp"

#include <string>
#include <iosfwd>

namespace alch {

/*!
  \brief Class description
  \ingroup afwk
*/
class Framework
{
 public:

  //! Return codes for run() method
  enum AppReturnCode
  {
    //! Success
    APPRETURNCODE_success = 0,

    //! Failure during application initialization
    APPRETURNCODE_initializeFailure = 1,

    //! Failure during options parsing
    APPRETURNCODE_optionsFailure = 2,

    //! Failure during application execution
    APPRETURNCODE_application = 3,

    //! Failure during application finalization
    APPRETURNCODE_finalizeFailure = 4
  };


  //! Return codes for processOptions() method
  enum OptionsReturnCode
  {
    //! Success: continue processing
    OPTIONSRETURNCODE_continue = 0,

    //! Error: stop processing
    OPTIONSRETURNCODE_error = 1,

    //! Success: stop processing
    OPTIONSRETURNCODE_stop = 2
  };

  /*!
    \brief Constructor
  */
  Framework();

  virtual ~Framework();

  /*!
    \brief Runs the application
    \param argc Number of command-line arguments
    \param argv Command-line arguments
    \retval 0 Success
    \retval non-zero Error

    This method is designed to be called from main().
  */
  AppReturnCode run(int argc, char** argv);

protected:

  // Returns application name
  virtual std::string getApplicationName() const = 0;

  // Returns application description
  virtual std::string getApplicationDescription() const = 0;

  // Returns application version string
  virtual std::string getApplicationVersion() const = 0;

  //! Initialize the application
  virtual bool initialize();

  //! Finalize the application
  virtual bool finalize();

  /*!
    \brief Processes application options from command line and config files
    \return Inidicates whether application should continue
  */
  virtual OptionsReturnCode processOptions(int argc, char** argv);

  /*!
    \brief Runs the application logic
    \retval true Success
    \retval false Error
  */
  virtual bool  processApplication() = 0;

  Context& getContext()
  {
    return m_ctx;
  }

  FrameworkOptions& getOptions()
  {
    return m_options;
  }

  /*!
    \brief Returns debug level
    \retval 0 No debugging
    \retval 1-3 Debug levels 1 (lowest) through 3 (highest)
  */
  int getDebugLevel() const
  {
    return m_debugLevel;
  }

 private:

  static const char* const s_optionHelp;

  static const char* const s_optionVersion;

  static const char* const s_optionVerbose;

  static const char* const s_optionDebug;

  static const char* const s_optionSeed;

  //! Prints application version information
  void printVersion(std::ostream& os);

  //! Prints the application usage message
  void printUsage(std::ostream& os);

  //! Prints out configuration file locations
  void printConfigFiles();

  //! Creates directories needed by application
  bool createDirectories();

  //! Prints total elapsed time
  void printElapsed(const boost::posix_time::time_duration& elapsed);

  Context m_ctx;

  FrameworkOptions m_options;

  int m_debugLevel;

  int m_seed;

};

} // namespace alch

#endif
