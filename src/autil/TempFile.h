// -*- C++ -*-

#ifndef INCLUDED_autil_TempFile_h
#define INCLUDED_autil_TempFile_h

#include "boost/shared_ptr.hpp"

#include <string>

namespace alch
{

/*!
  \brief Creates and and later deletes a temporary file.
  \ingroup autil

  This class also encapsulates the logic of deciding where the temporary
  file should live. By default, if the user has not specified a path in the
  constructor, the class will attempt to put the temporary file in the system
  temporary directory.
*/
class TempFile
{
public:
  //! When to unlink the file
  enum DelMode
  {
    DELMODE_immed,     //!< Unlink the file immediately after creation
    DELMODE_destruct,  //!< Unlink the when this object is destoyed
    DELMODE_never      //!< Don't unlink this file
  };

  /*!
    \brief Construct the temporary file
    \param prefix The file prefix to use for this temporary file
    \param mode The deletion mode to use

    If prefix has a forward slash in it (thus indiciating a directory path)
    then it is used unchanged as a prefix. Otherwise, the constructor will
    prepend the system temporary directory path to it.
  */
  TempFile(const char* prefix = "alchemy",
           DelMode mode = DELMODE_destruct);

  /*! 
    \brief Destructor
  */
  virtual ~TempFile();

  //! Returns the file descriptor
  int getFD() const                  { return m_fd; }

  //! Returns the file name
  const std::string& getName() const { return m_name; }

  /*!
    \brief Explicitly unlink the file
    \returns true if file was unlinked, false if already unlinked
    \throws Exception on error
  */
  bool unlink();

  /*!
    \brief Explicitly close the file
    \returns true if file was closed, false if already closed
    \throws Exception on error
  */
  bool close();

private:
  int m_fd;
  std::string m_name;
  DelMode m_mode;
};

//! Shared pointer to TempFile
typedef boost::shared_ptr<TempFile> TempFilePtr;

} // namespace alch

#endif
