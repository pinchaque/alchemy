// -*- C++ -*-

#ifndef INCLUDED_stockdata_StockID_h
#define INCLUDED_stockdata_StockID_h

#include <string>
#include <iostream>
#include <algorithm>

namespace alch {

/*!
  \brief Unique identifier for a security
  \ingroup stockdata

  Encapsulates an identifier for a security.
*/
class StockID
{
 public:

  /*!
    \brief Constructor: Creates an empty ID
  */
  StockID()
    : m_symbol("")
  {
    ;
  }

  /*!
    \brief Constructor: Creates based on symbol name
  */
  StockID(const std::string& symbol)
    : m_symbol(symbol)
  {
    std::transform(m_symbol.begin(), m_symbol.end(), m_symbol.begin(),
                   ::tolower);
  }

  ~StockID()
  {
    ;
  }

  //! Returns the symbol name
  const std::string& getSymbol() const
  {
    return m_symbol;
  }

  //! Sets the symbol name
  void setSymbol(const std::string& val)
  {
    m_symbol = val;
    std::transform(m_symbol.begin(), m_symbol.end(), m_symbol.begin(),
                   ::tolower);
  }

  bool operator == (const StockID& other) const
  {
    return (m_symbol == other.m_symbol);
  }

  bool operator != (const StockID& other) const
  {
    return !(*this == other);
  }

  bool operator < (const StockID& other) const
  {
    return 0 > strcmp(this->m_symbol.c_str(), other.m_symbol.c_str());
  }


 private:
  std::string m_symbol;
};


/*!
  \brief Allows StockID object to be written to std::ostream
  \param os The std::ostream
  \param id The StockID
  \ingroup stockdata
*/
inline std::ostream& operator << (std::ostream& os, const StockID& id)
{
  os << id.getSymbol();
  return os;
}

} // namespace alch

#endif
