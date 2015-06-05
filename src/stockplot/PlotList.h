// -*- C++ -*-

#ifndef INCLUDED_stockplot_PlotList_h
#define INCLUDED_stockplot_PlotList_h

#include "stockdata/StockTime.h"
#include "stockplot/Plot.h"

#include "boost/shared_ptr.hpp"

#ifndef NDEBUG
#include <iosfwd>
#endif

#include <vector>

namespace alch {

/*!
  \brief Class encapsulating a series of separate charts that will be plotted
  with one another utilizing the same x axis range.
  \ingroup stockplot
*/
class PlotList
{
 public:
  //! Vector of PlotPtr
  typedef std::vector<PlotPtr> PlotPtrVec;

  /*!
    \brief Constructor
  */
  PlotList()
    : m_title("")
    , m_xMin(boost::posix_time::second_clock::local_time())
    , m_xMax(boost::posix_time::second_clock::local_time())
  {
    ;
  }

  virtual ~PlotList()
  {
    ;
  }


  const std::string& getTitle() const
  {
    return m_title;
  }


  void setTitle(const std::string& val)
  {
    m_title = val;
  }


  const StockTime& getXMin() const
  {
    return m_xMin;
  }


  void setXMin(const StockTime& val)
  {
    m_xMin = val;
  }


  const StockTime& getXMax() const
  {
    return m_xMax;
  }


  void setXMax(const StockTime& val)
  {
    m_xMax = val;
  }



  const PlotPtrVec&  getPlotList() const
  {
    return m_plotList;
  }


  /*!
    \brief Adds a PlotList object to this plot
    \param val The new PlotList

    The new PlotList is automatically appended to existing PlotList.
  */
  void addPlot(const PlotPtr& val)
  {
    m_plotList.push_back(val);
  }


  /*!
    \brief Removes all plots from this object
  */
  void clear()
  {
    m_plotList.clear();
  }


#ifndef NDEBUG
    //! Dumps object contents to stream
    void dump(std::ostream& os) const;
#endif

 private:

  //! Title for this data series
  std::string m_title;

  //! List of plots
  PlotPtrVec m_plotList;

  //! The start time of the plot
  StockTime m_xMin;

  //! The end time of the plot
  StockTime m_xMax;
};


/*!
  \brief Shared pointer to PlotList
  \ingroup stockplot
*/
typedef boost::shared_ptr<PlotList> PlotListPtr;

} // namespace alch

#endif
