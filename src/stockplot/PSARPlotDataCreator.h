// -*- C++ -*-

#ifndef INCLUDED_stockplot_PSARPlotDataCreator_h
#define INCLUDED_stockplot_PSARPlotDataCreator_h

#include "stockplot/PlotDataCreator.h"

namespace alch {

/*!
  \brief Creates PlotData based on parabolic stop-and-reversal system
  \ingroup stockplot
*/
class PSARPlotDataCreator : public PlotDataCreator
{
 public:

  /*!
    \brief Constructor
  */
  PSARPlotDataCreator(Context& ctx);

  virtual ~PSARPlotDataCreator();

  /*!
    \brief Using the data in this class, populates the specified Plot with
    PlotData objects.
    \param plot The Plot object to add to
    \retval true Success
    \retval false error
  */
  bool create(const PlotPtr& plot);


  //! Sets the acceleration to use for PSAR plot
  void setAccel(double val)
  {
    m_accel = val;
  }


  //! Returns the accel used for calculating PSAR
  double getAccel() const
  {
    return m_accel;
  }

  //! Sets the maximum acceleration to use for PSAR plot
  void setMaxAccel(double val)
  {
    m_maxAccel = val;
  }


  //! Returns the maximum acceleration used for calculating PSAR
  double getMaxAccel() const
  {
    return m_maxAccel;
  }

 private:

  //! Generates a label for the plot
  std::string generateLabel() const;

  //! Adds data to the plot
  bool addData(const PlotDataPtr& plotData) const;

  //! Acceleration to use when calculating PSAR
  double m_accel;

  //! Maximum acceleration to use when calculating PSAR
  double m_maxAccel;

};

} // namespace alch

#endif
