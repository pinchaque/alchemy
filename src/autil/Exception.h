// -*- C++ -*-
#ifndef INCLUDED_autil_Exception
#define INCLUDED_autil_Exception

#include <stdexcept>
#include <string>

namespace alch {

//! Exception class
class Exception : public std::exception
{
public:
  //! Constructor
  Exception()
    : m_file("")
    , m_line(0)
    , m_errno(0)
    , m_message()
  {
    ;
  }

   /*!
    \brief Constructor
      \param file [in] File exception was thrown from, __FILE__
      \param line [in] Line exception was thrown from, __LINE__
      \param msg [in] Context dependent error message
      \param error [in] Value of error number, errno
  */
  Exception(const char* file, int line, const std::string& msg, int err = 0)
    : m_file(file ? file : "")
    , m_line(line)
    , m_errno(err)
    , m_message(msg)
  {
    ;
  }


  virtual ~Exception() throw()
  {
    ;
  }


  //! Return the exception message or an empty string "" if not set
  const char* Exception::what() const throw()
  {
    return getMessage();
  }


  //! Return the name of the file the exception was thrown from
  const char* getFile() const throw()
  {
    return m_file;
  }


  //! Return the line in the file the exception was thrown from
  int getLine() const throw()
  {
    return m_line;
  }


  //! Return the value of the system error number, or 0 if not set
  int getErrno() const throw()
  {
    return m_errno;
  }


  //! Return the context dependent error message for the exception
  const char* getMessage() const throw()
  {
    try
    {
      return m_message.c_str();
    }
    catch (...) {}
    return "";
  }


private:
  const char* m_file;      //!< Value of __FILE__ 
  int         m_line;      //!< Value of __LINE__
  int         m_errno;     //!< System errno
  std::string m_message;   //!< Error message
};


/*!
  \brief Throws exception e, sets exception errno to 0
  \param e [in] exception class to throw
  \param m [in] error message text

  This needs be a macro to capture the file and line correctly
*/
#define ATHROW(e, m)  throw e(__FILE__, __LINE__, (m))


/*!
  \brief Throws exception e, sets exception errno to n 
  \param e [in] exception class to throw
  \param m [in] error message text
  \param n [in] system error number
  
  This needs be a macro to capture the file and line correctly
*/
#define ATHROWE(e, m, n)  throw e(__FILE__, __LINE__, (m), (n))

} // namespace alch


#endif // INCLUDED_autil_Exception
