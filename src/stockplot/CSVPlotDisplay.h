// -*- C++ -*-

#ifndef INCLUDED_stockplot_CSVPlotDisplay_h
#define INCLUDED_stockplot_CSVPlotDisplay_h

#include "stockplot/PlotDisplay.h"
#include "stockplot/PlotList.h"
#include "stockdata/StockTime.h"

#include <string>


namespace alch {

/*!
  \brief Writes a PlotList object to a CSV file
  \ingroup stockplot
*/
class CSVPlotDisplay : public PlotDisplay
{
 public:

  CSVPlotDisplay(Context& ctx, const std::string& fileName);

  virtual ~CSVPlotDisplay();

  virtual bool show(const PlotListPtr& plotListPtr);


 private:

  bool writeHeader(std::ostream& os, const PlotListPtr& plotListPtr);

  bool writeData(std::ostream& os, const PlotListPtr& plotListPtr);

  /*!
    \brief Converts a StockTime into a string formatted by s_timeFormat
    \param stockTime The time to convert
    \return Time formatted by s_timeFormat
  */
  std::string stockTimeToString(const StockTime& stockTime);


  //! Format string for all time output
  static const char* const s_timeFormat;

  //! CSV file name
  std::string m_fileName;

  //! Number of values in each row in the CSV file
  int m_numValues;
};

} // namespace alch

#endif
