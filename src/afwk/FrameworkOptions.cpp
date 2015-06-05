
#include "afwk/FrameworkOptions.h"
#include "afwk/PathRegistry.h"
#include <fstream>

namespace alch {

  FrameworkOptions::FrameworkOptions()
    : m_variablesMap()
    , m_genericOptions("Command-line Options")
    , m_configOptions("Configuration File / Command-line Options")
    , m_hiddenOptions("Hidden Options")
  {
    ;
  }

  FrameworkOptions::~FrameworkOptions()
  {
    ;
  }


  void FrameworkOptions::process(int argc, char** argv)
  {
    // set up some options groups for convenience
    Description commandLineOptions;
    commandLineOptions
      .add(getGenericOptions())
      .add(getConfigOptions())
      .add(getHiddenOptions());
    
    Description configFileOptions;
    configFileOptions
      .add(getConfigOptions())
      .add(getHiddenOptions());
   
    // parse the command line
    VariablesMap& vm = getVariablesMap();
    store(boost::program_options::command_line_parser(argc, argv)
          .options(commandLineOptions)
          .positional(getPositionalOptions())
          .run(),
          vm);
    
    // parse user preference file
    std::string preferenceFile(PathRegistry::getPreferenceFile());
    std::ifstream ifs(preferenceFile.c_str());
    if (ifs)
    {
      store(parse_config_file(ifs, configFileOptions), vm);
      notify(vm);
    }
  }

} // namespace alch
