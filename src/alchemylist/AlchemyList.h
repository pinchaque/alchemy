// -*- C++ -*-

#ifndef INCLUDED_alchemylist_AlchemyList_h
#define INCLUDED_alchemylist_AlchemyList_h

#include "afwk/Framework.h"

namespace alch {

/*!
  \brief Runs the AlchemyList application, which returns a list of stocks
  \ingroup alchemylist
*/
class AlchemyList : public Framework
{
 public:

  /*!
    \brief Constructor
  */
  AlchemyList();

  virtual ~AlchemyList();

protected:

  std::string getApplicationName() const
  {
    return "alchemylist";
  }

  std::string getApplicationDescription() const;

  std::string getApplicationVersion() const
  {
    return VERSIONSTRING;
  }

  virtual bool initialize();

  virtual bool finalize();

  virtual Framework::OptionsReturnCode  processOptions(int argc, char** argv);

  virtual bool processApplication();

 private:

  /*!
    \brief Prints list of symbols
    \retval true Success
    \retval false Error
  */
  bool printSymbolList();

};

} // namespace alch

#endif
