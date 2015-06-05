// -*- C++ -*-

#ifndef INCLUDED_afwk_FrameworkOptions_h
#define INCLUDED_afwk_FrameworkOptions_h

#include "boost/program_options.hpp"

namespace alch {

/*!
  \brief Class encapsulating program options for an application
  \ingroup afwk
*/
class FrameworkOptions
{
 public:

  //! Convenience typedef for description of options
  typedef boost::program_options::options_description Description;

  //! Convenience typedef for description of positional options
  typedef boost::program_options::positional_options_description Positional;


  //! Convenience typedef for variables map
  typedef boost::program_options::variables_map VariablesMap;


  /*!
    \brief Constructor
  */
  FrameworkOptions();


  virtual ~FrameworkOptions();

  /*!
    \brief Processes given commandline to populate the variables map
    \param argc Number of command-line arguments
    \param argv Command-line arguments (argv[0] is executable name)
   */
  void process(int argc, char** argv);


  VariablesMap& getVariablesMap()
  {
    return m_variablesMap;
  }


  //! Visible options only allowed on command line
  Description& getGenericOptions()
  {
    return m_genericOptions;
  }


  //! Application options allowed on CL or in config file
  Description& getConfigOptions()
  {
    return m_configOptions;
  }


  //! Hidden options allowed on CL or in config file
  Description& getHiddenOptions()
  {
    return m_hiddenOptions;
  }


  //! Positional option descriptions
  Positional& getPositionalOptions()
  {
    return m_positionalOptions;
  }

 private:

  VariablesMap m_variablesMap;

  //! Visible options only allowed on command line
  Description m_genericOptions;

  //! Application options allowed on CL or in config file
  Description m_configOptions;

  //! Hidden options allowed on CL or in config file
  Description m_hiddenOptions;

  //! Positional option descriptions
  Positional m_positionalOptions;

};

} // namespace alch

#endif
