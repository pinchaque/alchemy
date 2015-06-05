// -*- C++ -*-

#ifndef INCLUDED_stockplot_ProfilePlotDataCreator_h
#define INCLUDED_stockplot_ProfilePlotDataCreator_h

#include "stockplot/PlotDataCreator.h"

#include "stocknnet/DatasetGenerator.h"
#include "stocknnet/PredictionProfile.h"

namespace alch {

/*!
  \brief Creates PlotData based on a prediction profile
  \ingroup stockplot
*/
class ProfilePlotDataCreator : public PlotDataCreator
{
 public:

  /*!
    \brief Constructor
    \param ctx Operation context to use
    \param profile Prediction profile to use for plot
    \param generator Generator of dataset to use for this plot
  */
  ProfilePlotDataCreator(Context& ctx,
                         const PredictionProfile& profile,
                         DatasetGenerator& generator);

  virtual ~ProfilePlotDataCreator();

  /*!
    \brief Using the data in this class, populates the specified Plot with
    PlotData objects.
    \param plot The Plot object to add to
    \retval true Success
    \retval false error
  */
  virtual bool create(const PlotPtr& plot);

 private:

  //! Generates a label for the plot
  std::string generateLabel() const;

  //! Adds data to the plot
  bool addData(const PlotDataPtr& plotData);

  const PredictionProfile& m_profile;

  DatasetGenerator& m_generator;

};

} // namespace alch

#endif
