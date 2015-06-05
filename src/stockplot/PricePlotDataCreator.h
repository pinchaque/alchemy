// -*- C++ -*-

#ifndef INCLUDED_stockplot_PricePlotDataCreator_h
#define INCLUDED_stockplot_PricePlotDataCreator_h

#include "stockplot/PlotDataCreator.h"

namespace alch {

/*!
  \brief Creates PlotData based on price
  \ingroup stockplot
*/
class PricePlotDataCreator : public PlotDataCreator
{
 public:

  /*!
    \brief Constructor
  */
  PricePlotDataCreator(Context& ctx);

  virtual ~PricePlotDataCreator();

  /*!
    \brief Using the data in this class, populates the specified Plot with
    PlotData objects.
    \param plot The Plot object to add to
    \retval true Success
    \retval false error
  */
  bool create(const PlotPtr& plot);

 private:

  //! Generates a label for the plot
  std::string generateLabel() const;

  //! Adds data to the plot
  bool addData(const PlotDataPtr& plotData) const;

};

} // namespace alch

#endif
