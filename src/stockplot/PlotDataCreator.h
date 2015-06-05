// -*- C++ -*-

#ifndef INCLUDED_stockplot_PlotDataCreator_h
#define INCLUDED_stockplot_PlotDataCreator_h

#include "stockdata/RangeData.h"
#include "stockplot/Plot.h"
#include "autil/Context.h"

#include "boost/shared_ptr.hpp"

#include <string>

namespace alch {

/*!
  \brief Virtual parent class of plot data creators
  \ingroup stockplot

  Classes derived from this one are capable of adding PlotData objects to
  a Plot object for graphical display. 
*/
class PlotDataCreator
{
 public:

  /*!
    \brief Constructor
  */
  PlotDataCreator(Context& ctx)
    : m_ctx(ctx)
    , m_data()
    , m_id("")
  {
    ;
  }

  virtual ~PlotDataCreator()
  {
    ;
  }

  //! Sets the data to use for creating the PlotData object
  void setData(const RangeDataPtr& val)
  {
    m_data = val;
  }

  //! Returns the data to use for creating the PlotData object
  const RangeDataPtr& getData() const
  {
    return m_data;
  }


  //! Sets the ID for this plot
  void setID(const std::string& val)
  {
    m_id = val;
  }


  //! Returns the ID for this plot
  const std::string& getID() const
  {
    return m_id;
  }

  
  /*!
    \brief Using the data in this class, populates the specified Plot with
    PlotData objects.
    \param plot The Plot object to add to
    \retval true Success
    \retval false error

    This method will add as many PlotData objects as needed to represent
    the data.
  */
  virtual bool create(const PlotPtr& plot) = 0;

protected:

  //! Returns context for the display operations
  Context& getContext()
  {
    return m_ctx;
  }

 private:

  //! Operation context to use for plot creation
  Context& m_ctx;

  //! The data we are using for this plot
  RangeDataPtr m_data;

  //! Identifier for the plot
  std::string m_id;
};

/*!
  \brief Shared pointer to PlotDataCreator
  \ingroup stockplot
*/
typedef boost::shared_ptr<PlotDataCreator> PlotDataCreatorPtr;

} // namespace alch

#endif
