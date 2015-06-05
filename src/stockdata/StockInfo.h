// -*- C++ -*-

#ifndef INCLUDED_stockdata_StockInfo_h
#define INCLUDED_stockdata_StockInfo_h

#include "stockdata/StockID.h"
#include "stockdata/StockTime.h"

#include <string>
#include <iostream>
#include <algorithm>

namespace alch {

/*!
  \brief Extended info for a security
  \ingroup stockdata
*/
class StockInfo
{
 public:

  /*!
    \brief Constructor: Creates an empty Info
  */
  StockInfo()
    : m_id("")
    , m_name("")
    , m_expenseRatio(0.0)
    , m_12b1Fee(0.0)
    , m_family("")
    , m_rating(0)
    , m_inception(boost::posix_time::second_clock::local_time())
    , m_initInvestment(0)
    , m_subsInvestment(0)
    , m_assets(0)
    , m_turnover(0.0)
    , m_yield(0.0)
    , m_deferredLoad(0.0)
    , m_frontLoad(0.0)
    , m_category("")
  {
    ;
  }

  ~StockInfo()
  {
    ;
  }

  const std::string& getSymbol() const { return m_id.getSymbol(); }
  const StockID& getID() const { return m_id; }
  const std::string& getName() const { return m_name; }
  void setName(const std::string& val) { m_name = val; }
  double getExpenseRatio() const { return m_expenseRatio; }
  double get12b1Fee() const { return m_12b1Fee; }
  const std::string& getFamily() const { return m_family; }
  int getRating() const { return m_rating; }
  StockTime getInception() const { return m_inception; }
  int getInitInvestment() const { return m_initInvestment; }
  int getSubsInvestment() const { return m_subsInvestment; }
  int getAssets() const { return m_assets; }
  double getTurnover() const { return m_turnover; }
  double getYield() const { return m_yield; }
  double getDeferredLoad() const { return m_deferredLoad; }
  double getFrontLoad() const { return m_frontLoad; }
  const std::string& getCategory() const { return m_category; }

  void setID(const StockID& val) { m_id = val; }
  void setExpenseRatio(double val) { m_expenseRatio = val; }
  void set12b1Fee(double val) { m_12b1Fee = val; }
  void setFamily(const std::string& val) { m_family = val; }
  void setRating(int val) { m_rating = val; }
  void setInception(StockTime val) { m_inception = val; }
  void setInitInvestment(int val) { m_initInvestment = val; }
  void setSubsInvestment(int val) { m_subsInvestment = val; }
  void setAssets(int val) { m_assets = val; }
  void setTurnover(double val) { m_turnover = val; }
  void setYield(double val) { m_yield = val; }
  void setDeferredLoad(double val) { m_deferredLoad = val; }
  void setFrontLoad(double val) { m_frontLoad = val; }
  void setCategory(const std::string& val) { m_category = val; }

  std::string getFullName() const
  {
    if (m_name.length())
    {
      return m_name + " (" + m_id.getSymbol() + ")";
    }
    else
    {
      return m_id.getSymbol();
    }
  }

  bool operator == (const StockInfo& other) const
  {
    return (m_id == other.m_id);
  }

  bool operator != (const StockInfo& other) const
  {
    return !(*this == other);
  }

 private:
  StockID m_id;
  std::string m_name;
  double m_expenseRatio;
  double m_12b1Fee;
  std::string m_family;
  int m_rating;
  StockTime m_inception;
  int m_initInvestment;
  int m_subsInvestment;
  int m_assets;
  double m_turnover;
  double m_yield;
  double m_deferredLoad;
  double m_frontLoad;
  std::string m_category;
};

typedef boost::shared_ptr<StockInfo> StockInfoPtr;


/*!
  \brief Allows StockInfo object to be written to std::ostream
  \param os The std::ostream
  \param id The StockInfo
  \ingroup stockdata
*/
inline std::ostream& operator << (std::ostream& os, const StockInfo& info)
{
  os << info.getFullName();
  return os;
}

} // namespace alch

#endif
