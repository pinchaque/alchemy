#include "DIRNAME/CLASSNAME.h"

#include <sstream>

namespace alch {

  const char* const CLASSNAME::s_optionFoo = "foo";

  CLASSNAME::CLASSNAME()
    : Framework()
  {
    ;
  }


  CLASSNAME::~CLASSNAME()
  {
    ;
  }

  std::string CLASSNAME::getApplicationDescription() const
  {
    return "Description of application, with linefeeds\n";
  }

  bool CLASSNAME::initialize()
  {
    return Framework::initialize();
  }

  bool CLASSNAME::finalize()
  {
    return Framework::finalize();
  }

  Framework::OptionsReturnCode CLASSNAME::processOptions(
    int argc, char** argv)
  {

    std::stringstream ssFoo;
    ssFoo << s_optionFoo << "," << s_optionFoo[0];

    getOptions().getGenericOptions().add_options()
      (ssFoo.str().c_str(), 
       boost::program_options::value<std::string>(),
       "Description")
      ;

    return Framework::processOptions(argc, argv);
  }


  bool CLASSNAME::processApplication()
  {
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();
    return true;
  }

} // namespace alch
