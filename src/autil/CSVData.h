// -*- C++ -*-

#ifndef INCLUDED_autil_CSVData_h
#define INCLUDED_autil_CSVData_h

#include "autil/Context.h"

#include <string>
#include <vector>
#include <iosfwd>

namespace alch {


/*!
  \brief Metadata for stocks data files that have been saved
  \ingroup autil
*/
class CSVData
{
  public:

  class Row
  {
    public:
    typedef std::vector<std::string> StrVec;
    typedef StrVec::size_type size_type;
    typedef StrVec::const_iterator const_iterator;

    Row()
      : m_data()
    {
      ;
    }

    // adds item to this row
    template <class T>
    void add(T val)
    {
      std::stringstream ss;
      ss << val;
      m_data.push_back(ss.str());
    }

    size_type size() const;
    const std::string& get(size_type i) const;
    void clear();
    bool empty() const;
    const_iterator begin() const;
    const_iterator end() const;
    bool operator==(const Row& other) const;
    bool operator!=(const Row& other) const;
    void dump(std::ostream& os) const;

    private:
      StrVec m_data;
  };


  typedef std::vector<Row>::const_iterator const_iterator;

  const_iterator begin() const;
  const_iterator end() const;

  /*!
    \brief Constructor
  */
  CSVData();
  void setHeadings(const Row& row);
  const Row& getHeadings() const;
  int numColumns() const;
  void addRow(const Row& row);
  void clearRows();
  void clearHeadings();
  void clear();
  bool operator==(const CSVData& other) const;
  bool operator!=(const CSVData& other) const;

  // validates the data, returns false on error, writes messages to ctx
  bool validate(Context& ctx) const;

  void dump(std::ostream& os) const;

  private:
  Row m_headings;
  std::vector<Row> m_values;
};

} // namespace alch

#endif
