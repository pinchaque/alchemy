// -*- C++ -*-

#ifndef INCLUDED_stockplot_PlotData_h
#define INCLUDED_stockplot_PlotData_h

#include "stockplot/PlotDataSegment.h"

#include "boost/shared_ptr.hpp"

#include <string>

#ifndef NDEBUG
#include <iosfwd>
#endif

namespace alch {

/*!
  \brief A single data series that is plotted. It consists of a series of
  possibly discontinuous data points (x, y values) along with other 
  characteristics of the data series, such as line style and label.
  \ingroup stockplot
*/
class PlotData
{
 public:


  //! Enumeration of line plot types
  enum Style
  {
    STYLE_lines,
    STYLE_points,
    STYLE_linespoints,
    STYLE_impulses,
    STYLE_dots,
    STYLE_steps,
    STYLE_fsteps,
    STYLE_histeps,
    STYLE_boxes
  };

  /*!
    \brief Constructor
  */
  PlotData()
    : m_label("")
    , m_dataSegments()
    , m_style(STYLE_lines)
  {
    ;
  }

  virtual ~PlotData()
  {
    ;
  }

  const std::string& getLabel() const
  {
    return m_label;
  }

  void setLabel(const std::string& val)
  {
    m_label = val;
  }

  const Style& getStyle() const
  {
    return m_style;
  }

  void setStyle(const Style& val)
  {
    m_style = val;
  }

  const PlotDataSegmentPtrVec& getDataSegments() const
  {
    return m_dataSegments;
  }

  /*!
    \brief Adds a data segment to the plot data
    \param val The new data segment

    The new data segment is automatically appended to existing data segments.
  */
  void addPlotDataSegment(const PlotDataSegmentPtr& val)
  {
    m_dataSegments.push_back(val);
  }

  /*!
    \brief Removes all data series from this object
  */
  void clear()
  {
    m_dataSegments.clear();
  }

#ifndef NDEBUG
    //! Dumps object contents to stream
    void dump(std::ostream& os) const;
#endif

 private:

  //! Label for this data series
  std::string m_label;

  //! The data series
  PlotDataSegmentPtrVec m_dataSegments;

  //! The plot style
  Style m_style;
};

/*!
  \brief Shared pointer to PlotData
  \ingroup stockplot
*/
typedef boost::shared_ptr<PlotData> PlotDataPtr;

} // namespace alch

#endif
