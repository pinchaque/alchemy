// -*- C++ -*-

#ifndef INCLUDED_stockplot_ROCPlotDataCreator_h
#define INCLUDED_stockplot_ROCPlotDataCreator_h

#include "stockplot/PlotDataCreator.h"

namespace alch {

/*!
  \brief Creates PlotData based on rate of change (ROC)
  \ingroup stockplot
*/
class ROCPlotDataCreator : public PlotDataCreator
{
 public:

  /*!
    \brief Constructor
  */
  ROCPlotDataCreator(Context& ctx);

  virtual ~ROCPlotDataCreator();

  /*!
    \brief Using the data in this class, populates the specified Plot with
    PlotData objects.
    \param plot The Plot object to add to
    \retval true Success
    \retval false error
  */
  bool create(const PlotPtr& plot);


  //! Sets the span (# trading periods) to be used when calculating
  void setSpan(int val)
  {
    m_span = val;
  }


  //! Returns the span used for calculating
  int getSpan() const
  {
    return m_span;
  }

 private:

  //! Generates a label for the plot
  std::string generateLabel() const;

  //! Adds data to the plot
  bool addData(const PlotDataPtr& plotData) const;

  //! Span to use when calculating
  int m_span;

};

} // namespace alch

#endif
