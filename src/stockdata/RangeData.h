// -*- C++ -*-

#ifndef INCLUDED_stockdata_RangeData_h
#define INCLUDED_stockdata_RangeData_h

#include "stockdata/StockTime.h"

#include "boost/shared_ptr.hpp"

#include <vector>
#include <cassert>
#include <iosfwd>

namespace alch {

/*!
  \brief Class that encapsulates stock data trading over a range of prices
  \ingroup stockdata

  Range data represents the situation where multiple stock transactions
  have taken place over a period of time. This gives rise to multiple
  price points, which are summarized in this class as an open, close, 
  minimum, and maximum. Because multiple transactions have taken place,
  the range also includes the volume during that trading period. This
  class represents multiple trading periods as a vector.

  Basic data can be generated from range data, and this class provides
  methods to do that.

  Range data can be summarized into larger periods of time, thus producing
  fewer data points. For example, daily range data can be transformed
  into weekly. This class provides methods for this transformation.
*/
class RangeData
{
 public:

  /*!
    \brief A single data point (trading period)
  */
  struct Point
  {
    //! Opening price
    double open;

    //! Closing price
    double close;

    //! Minimum trading price over the period
    double min;

    //! Maximum trading price over the period
    double max;

    //! Number of shares transacted over the period
    double volume;

    //! Adjusted closing price
    double adjustedClose;

    //! Date and time of transacation
    StockTime tradeTime;

    //! Constructor: Initialize everything to 0
    Point()
      : open(0.00)
      , close(0.00)
      , min(0.00)
      , max(0.00)
      , volume(0.00)
      , adjustedClose(0.00)
      , tradeTime(boost::posix_time::second_clock::local_time())
    {
      ;
    }


    //! Determines if this RangeData is equal to another
    bool operator == (const Point& other) const;


    //! Determines if this RangeData is not equal to another
    bool operator != (const Point& other) const
    {
      return !(*this == other);
    }


#ifndef NDEBUG
    //! Dumps object contents to stream
    void dump(std::ostream& os) const;
#endif

  };


  //! Data type of vector of range data points
  typedef std::vector<Point> PointVector;


  //! Size type of vector
  typedef PointVector::size_type size_type;

  //! Constant iterator for vector
  typedef PointVector::const_iterator const_iterator;

  //! Iterator for vector
  typedef PointVector::iterator iterator;


  /*!
    \brief Constructor: Create empty range data
  */
  RangeData()
    : m_data()
  {
    ;
  }


  //! Destructor
  ~RangeData()
  {
    ;
  }


  //! Determines if this RangeData is equal to another
  bool operator == (const RangeData& other) const;


  //! Determines if this RangeData is equal to another
  bool operator != (const RangeData& other) const
  {
    return !(*this == other);
  }


  /*!
    \brief Adds a data point to the vector
  */
  void add(const Point& val)
  {
    m_data.push_back(val);
  }


  /*!
    \brief Clears all data points from the vector
  */
  void clear()
  {
    m_data.clear();
  }


  /*!
    \brief Returns number of elements in the vector
  */
  size_type size() const
  {
    return m_data.size();
  }

  /*!
    \brief returns true if there are no elements
  */
  bool empty() const
  {
    return m_data.empty();
  }

  /*!
    \brief Returns specific data point from vector
  */
  const Point& get(size_type i) const
  {
    assert(i >= 0);
    assert(i < m_data.size());
    return m_data[i];
  }


  /*!
    \brief Reserves space in data point vector
    \param i Number of spaces to reserve
  */
  void reserve(size_type i)
  {
    m_data.reserve(i);
  }


  /*!
    \brief Resizes data point vector
    \param i New size
  */
  void resize(size_type i)
  {
    m_data.resize(i);
  }


  /*!
    \brief Returns iterator to beginning of vector
  */
  const_iterator begin() const
  {
    return m_data.begin();
  }


  /*!
    \brief Returns iterator to endning of vector
  */
  const_iterator end() const
  {
    return m_data.end();
  }


  /*!
    \brief Returns iterator to endning of vector
  */
  iterator end()
  {
    return m_data.end();
  }


  /*!
    \brief Returns iterator to beginning of vector
  */
  iterator begin()
  {
    return m_data.begin();
  }


  /*!
    \brief Modifies the data so that the close value is set to the adjusted
    close value

    This method is useful when the user has requested operations with the
    adjusted close values. When this method is called, the adjustedClose
    values for all data points is copied into the close value. Note that
    this permanently overwrites the close value.
  */
  void useAdjusted();


  /*!
    \brief Summarizes the data in this class into a larger periods of time.
    \param newData [out] The summarized data output (will be cleared)
    \param points The number of data points to summarize into a single point

    Every "points" number of data points in this class are turned into
    a single data point in newData. The data members of the new point are
    assigned as follows:
      tradeTime: the tradeTime of the last point
      open: the value of open for the first point
      close: the value of close for the last point
      min: the minimum value of min for all points
      max: the maximum value of max for all points
      volume: the average volume over all points
      adjustedClose: the adjusted close value for the last point

    If there is a remainder data points in this class (i.e. the number of
    data points does not divide evenly by "points") then these are combined
    into a final data point in newData.
  */
  void summarize(RangeData& newData, int points) const;

  /*!
    Works similarly to summarize(), but summarizes to monthly data. The
    date is the last trading day of that month.
  */
  void summarizeMonthly(RangeData& newData) const;


  /*!
    \brief Sorts data by increasing date
  */
  void sortDate();


  /*!
    \brief Merges another dataset in with this one
    \param other The other dataset
    \param removeDups Remove duplicates during merge

    Assumes both this and the other data set are already sorted. Removes
    any duplicate entries in the merge process if removeDups is true.
  */
  void merge(const RangeData& other, bool removeDups = true);


  /*!
   \brief Calculates the returns for each data point and places them in returns.
   \param returns [out] The returns for each period.

   This uses close-close values because we may be using adjusted close values.
   Opening values are ignored.
  */
  void getReturns(std::vector<double>& returns) const;


#ifndef NDEBUG
  void dump(std::ostream& os) const;
#endif

 private:
  Point getSummaryPoint(size_type startIdx, size_type endIdx) const;

  PointVector m_data;
};

/*!
  \brief Shared pointer to RangeData
  \ingroup stockdata
*/
typedef boost::shared_ptr<RangeData> RangeDataPtr;

} // namespace alch

#endif
