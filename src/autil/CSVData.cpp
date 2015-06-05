
#include "autil/CSVData.h"

#include <cassert>


namespace alch {
  

  const std::string& CSVData::Row::get(CSVData::Row::size_type i) const
  {
    assert(i >= 0);
    assert(i < m_data.size());
    return m_data[i];
  }
  // number of values in this row
  CSVData::Row::size_type CSVData::Row::size() const
  {
    return m_data.size();
  }

  void CSVData::Row::clear()
  {
    m_data.clear();
  }

  bool CSVData::Row::empty() const
  {
    return m_data.empty();
  }

  CSVData::Row::const_iterator CSVData::Row::begin() const
  {
    return m_data.begin();
  }

  CSVData::Row::const_iterator CSVData::Row::end() const
  {
    return m_data.end();
  }

  bool CSVData::Row::operator==(const CSVData::Row& other) const
  {
    if (size() != other.size())
    {
      return false;
    }

    CSVData::Row::size_type max = size();
    for (CSVData::Row::size_type i = 0; i < max; ++i)
    {
      if (get(i) != other.get(i))
      {
        return false;
      }
    }

    return true;
  }

  bool CSVData::Row::operator!=(const CSVData::Row& other) const
  {
    return !(*this == other);
  }

  CSVData::CSVData()
    : m_headings()
    , m_values()
    {
      ;
    }
  
  CSVData::const_iterator CSVData::begin() const 
  { 
    return m_values.begin(); 
  }
 
  CSVData::const_iterator CSVData::end() const 
  { 
    return m_values.end(); 
  }

  void CSVData::setHeadings(const CSVData::Row& row)
  {
    m_headings = row;
  }

  const CSVData::Row& CSVData::getHeadings() const
  {
    return m_headings; 
  }

  int CSVData::numColumns() const
  {
    return static_cast<int>(m_headings.size());
  }

  void CSVData::addRow(const CSVData::Row& row)
  {
    m_values.push_back(row);
  }

  void CSVData::clearRows()
  {
    m_values.clear();
  }

  void CSVData::clearHeadings()
  {
    m_headings.clear();
  }

  void CSVData::clear()
  {
    clearRows();
    clearHeadings();
  }

  // validates the data, returns false on error, writes messages to ctx
  bool CSVData::validate(Context& ctx) const
  {
    // check if no headings
    if (m_headings.empty())
    {
      if (m_values.empty())
      {
        return true;
      }
      else
      {
        ctx << Context::PRIORITY_error
          << "CSV file missing headings" << Context::endl;
        return false;
      }
    }

    // check all values
    CSVData::Row::size_type numHeadings = m_headings.size();
    CSVData::const_iterator end = m_values.end();
    CSVData::const_iterator iter;
    int i = 2;
    for (iter = m_values.begin(); iter != end; ++iter, ++i)
    {
      if (iter->size() != numHeadings)
      {
        ctx << Context::PRIORITY_error
          << "CSV file row " << i << " had " << iter->size() 
          << " values for " << numHeadings << " headings" << Context::endl;
        return false;
      }
    }

    return true;
  }

  bool CSVData::operator==(const CSVData& other) const
  {
    if (m_headings != other.m_headings)
    {
      return false;
    }

    if (m_values.size() != other.m_values.size())
    {
      return false;
    }

    std::vector<CSVData::Row>::size_type max = m_values.size();
    std::vector<CSVData::Row>::size_type i;
    for (i = 0; i < max; ++i)
    {
      if (m_values[i] != other.m_values[i])
      {
        return false;
      }
    }

    return true;
  }

  bool CSVData::operator!=(const CSVData& other) const
  {
    return !(*this == other);
  }


  void CSVData::Row::dump(std::ostream& os) const
  {
    CSVData::Row::const_iterator end = m_data.end();
    CSVData::Row::const_iterator iter;
    for (iter = m_data.begin(); iter != end; ++iter)
    {
      os << "  [" << *iter << "]";
    }
    os << "\n";
  }

  void CSVData::dump(std::ostream& os) const
  {
    os << "HEADINGS: ";
    m_headings.dump(os);

    CSVData::const_iterator end = m_values.end();
    CSVData::const_iterator iter;
    int i = 1;
    for (iter = m_values.begin(); iter != end; ++iter, ++i)
    {
      os << "ROW[" << i << "]: ";
      iter->dump(os);
    }
  }
} // namespace alch
