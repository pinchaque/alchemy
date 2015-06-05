// -*- C++ -*-

#ifndef INCLUDED_DIRNAME_CLASSNAME_h
#define INCLUDED_DIRNAME_CLASSNAME_h

#include "afwk/Framework.h"

namespace alch {

/*!
  \brief Runs the CLASSNAME application
  \ingroup DIRNAME
*/
class CLASSNAME : public Framework
{
 public:

  /*!
    \brief Constructor
  */
  CLASSNAME();

  virtual ~CLASSNAME();

protected:

  std::string getApplicationName() const
  {
    return "DIRNAME";
  }

  std::string getApplicationDescription() const;

  std::string getApplicationVersion() const
  {
    return VERSIONSTRING;
  }

  virtual bool initialize();

  virtual bool finalize();

  virtual Framework::OptionsReturnCode processOptions(int argc, char** argv);

  virtual bool processApplication();

 private:

  static const char* const s_optionFoo;

};

} // namespace alch

#endif
