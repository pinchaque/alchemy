// -*- C++ -*-

#ifndef INCLUDED_alchemyplot_AlchemyPlot_h
#define INCLUDED_alchemyplot_AlchemyPlot_h

#include "afwk/Framework.h"

#include "stockdata/RangeData.h"

#include "stockdata/StockTime.h"

#include "stockplot/PlotList.h"

namespace alch {

/*!
  \brief Runs the AlchemyPlot application
  \ingroup alchemyplot
*/
class AlchemyPlot : public Framework
{
 public:

  /*!
    \brief Constructor
  */
  AlchemyPlot();

  virtual ~AlchemyPlot();

protected:

  std::string getApplicationName() const
  {
    return "alchemyplot";
  }

  std::string getApplicationDescription() const;

  std::string getApplicationVersion() const
  {
    return VERSIONSTRING;
  }

  virtual bool initialize();

  virtual bool finalize();

  virtual Framework::OptionsReturnCode processOptions(int argc, char** argv);

  virtual bool processApplication();

 private:

  static const char* const s_optionSymbol;
  static const char* const s_optionStart;
  static const char* const s_optionEnd;
  static const char* const s_optionDuration;
  static const char* const s_optionLinear;
  static const char* const s_optionCSV;
  static const char* const s_optionAdjusted;
  static const char* const s_optionMA20;
  static const char* const s_optionMA50;
  static const char* const s_optionPSAR;
  static const char* const s_optionROC;
  static const char* const s_optionRSI;
  static const char* const s_optionProfile;

  std::string m_symbol;

  StockTime m_startTime;

  StockTime m_endTime;

  RangeDataPtr m_data;

  PlotListPtr m_plotListPtr;

  bool getSymbol();

  bool setDateRange();

  bool retrieveData();

  /*!
    \brief Sets up plot object with moving-average plots
    \param plot the PlotPtr objec to populate
    \param span Number of days in moving average
   */
  bool initializeMAPlot(PlotPtr plot, int span);

  bool initializePlot();

  bool initializePricePlot();
  bool initializeROCPlot();
  bool initializeRSIPlot();
  bool initializeProfilePlot(PlotPtr plot, const std::string& profileList);

  bool showPlot();

};

} // namespace alch

#endif
