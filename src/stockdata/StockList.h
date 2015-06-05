// -*- C++ -*-

#ifndef INCLUDED_stockdata_StockList_h
#define INCLUDED_stockdata_StockList_h

#include "stockdata/StockID.h"
#include <vector>

namespace alch {

/*!
  \brief Metadata for stocks data files that have been saved
  \ingroup stockdata
*/
struct StockList
{
  public:

  typedef std::vector<StockID>::const_iterator const_iterator;
  typedef std::vector<StockID>::size_type size_type;

  const_iterator begin() const { return m_ids.begin(); }
  const_iterator end() const { return m_ids.end(); }

  void add(const StockID& id)
  {
    m_ids.push_back(id);
  }

  void clear() { m_ids.clear(); }
  size_type size() const { return m_ids.size(); }
  bool empty() const { return m_ids.empty(); }
  const StockID& get(size_type i) const { return m_ids[i]; }

 
  /*!
    \brief Constructor
  */
  StockList()
    : m_ids()
  {
    ;
  }

  private:
  std::vector<StockID> m_ids;
};

} // namespace alch

#endif
