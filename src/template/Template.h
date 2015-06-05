// -*- C++ -*-

#ifndef INCLUDED_DIRNAME_CLASSNAME_h
#define INCLUDED_DIRNAME_CLASSNAME_h

#include "boost/shared_ptr.hpp"

#ifndef NDEBUG
#include <iosfwd>
#endif

namespace alch {

/*!
  \brief Class description
  \ingroup DIRNAME
*/
class CLASSNAME
{
 public:

  /*!
    \brief Constructor
  */
  CLASSNAME()
  {
    ;
  }

  virtual ~CLASSNAME()
  {
    ;
  }

#ifndef NDEBUG
    //! Dumps object contents to stream
    void dump(std::ostream& os) const;
#endif

 private:
};

/*!
  \brief Shared pointer to CLASSNAME
  \ingroup DIRNAME
*/
typedef boost::shared_ptr<CLASSNAME> CLASSNAMEPtr;

} // namespace alch

#endif
