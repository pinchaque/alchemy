// -*- C++ -*-

#ifndef INCLUDED_stockplot_Plot_h
#define INCLUDED_stockplot_Plot_h

#include "stockplot/PlotData.h"

#include "boost/shared_ptr.hpp"

#ifndef NDEBUG
#include <iosfwd>
#endif

#include <vector>
#include <string>

namespace alch {

/*!
  \brief Encapsulates all data series that will be plotted together on a
  single plot. This also includes attributes of such a plot, such as the
  title and data ranges.
  \ingroup stockplot
*/
class Plot
{
 public:
  //! Vector of PlotDataPtr
  typedef std::vector<PlotDataPtr> PlotDataPtrVec;

  /*!
    \brief Constructor
  */
  Plot()
    : m_plotData()
    , m_yAxisLabel("y-axis")
    , m_yAutoscale(true)
    , m_yMin(0.00)
    , m_yMax(100.00)
    , m_weight(1.00)
    , m_logarithmic(false)
    , m_showKey(true)
  {
    ;
  }


  virtual ~Plot()
  {
    ;
  }


  const std::string&  getYAxisLabel() const
  {
    return m_yAxisLabel;
  }


  void setYAxisLabel(const std::string& val)
  {
    m_yAxisLabel = val;
  }

  bool getYAutoscale() const
  {
    return m_yAutoscale;
  }


  void setYAutoscale(bool val = true)
  {
    m_yAutoscale = val;
  }


  double getYMin() const
  {
    return m_yMin;
  }


  void setYMin(double val)
  {
    m_yMin = val;
  }


  double getYMax() const
  {
    return m_yMax;
  }


  void setYMax(double val)
  {
    m_yMax = val;
  }


  double getWeight() const
  {
    return m_weight;
  }


  void setWeight(double val)
  {
    m_weight = val;
  }


  bool getLogarithmic() const
  {
    return m_logarithmic;
  }


  void setLogarithmic(bool val = true)
  {
    m_logarithmic = val;
  }

  bool getShowKey() const
  {
    return m_showKey;
  }


  void setShowKey(bool val = true)
  {
    m_showKey = val;
  }


  const PlotDataPtrVec& getPlotData() const
  {
    return m_plotData;
  }


  /*!
    \brief Adds a PlotData object to this plot
    \param val The new PlotData

    The new PlotData is automatically appended to existing PlotData.
  */
  void addPlotData(const PlotDataPtr& val)
  {
    m_plotData.push_back(val);
  }


  /*!
    \brief Removes all data plots from this object
  */
  void clear()
  {
    m_plotData.clear();
  }


#ifndef NDEBUG
    //! Dumps object contents to stream
    void dump(std::ostream& os) const;
#endif


 private:

  //! The various data series to be put onto this plot
  PlotDataPtrVec m_plotData;

  //! Label to use for y-axis
  std::string m_yAxisLabel;

  //! Whether to use autoscaling for y axis
  bool m_yAutoscale;

  //! The minimum y value to plot
  double m_yMin;

  //! The maximum y value to plot
  double m_yMax;

  //! The weight of vertical space this plot should occupy
  double m_weight;

  //! Use logarithmic plot for data
  bool m_logarithmic;

  //! Show the key (legend) for this plot
  bool m_showKey;
};

/*!
  \brief Shared pointer to Plot
  \ingroup stockplot
*/
typedef boost::shared_ptr<Plot> PlotPtr;

} // namespace alch

#endif
