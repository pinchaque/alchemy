// -*- C++ -*-

#ifndef INCLUDED_stockplot_PlotDisplay_h
#define INCLUDED_stockplot_PlotDisplay_h

#include "stockplot/PlotList.h"
#include "autil/Context.h"

namespace alch {

/*!
  \brief Interface for displaying a plot
  \ingroup stockplot
*/
class PlotDisplay
{
public:

  /*!
    \brief Constructor
  */
  PlotDisplay(Context& ctx)
    : m_ctx(ctx)
  {
    ;
  }


  virtual ~PlotDisplay()
  {
    ;
  }

  
  virtual bool show(const PlotListPtr& plotListPtr) = 0;


protected:

  //! Returns context for the display operations
  Context& getContext()
  {
    return m_ctx;
  }


private:

  Context& m_ctx;
};

} // namespace alch

#endif
