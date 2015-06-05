// -*- C++ -*-

#ifndef INCLUDED_afwk_PathRegistry_h
#define INCLUDED_afwk_PathRegistry_h

#include <string>

namespace alch {

/*!
  \brief Static class storing values for paths used by framework applications
  \ingroup afwk
*/
class PathRegistry
{
 public:

  /*!
    \brief Constructor
  */
  PathRegistry()
  {
    ;
  }

  virtual ~PathRegistry()
  {
    ;
  }

  
  //! Returns top-level directory to use for stored data
  static std::string getDataDir();

  //! Returns directory used to store user preferences and data
  static std::string getUserDir();

  //! Returns file name used to store user preferences
  static std::string getPreferenceFile();

  //! Returns file name used to store which symbols to retrieve for an exchange
  static std::string getSymbolFile();

 private:

  static const char* const s_preferenceFile;

  static const char* const s_symbolFile;

};

} // namespace alch

#endif
