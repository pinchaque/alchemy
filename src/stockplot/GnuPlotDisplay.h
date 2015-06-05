// -*- C++ -*-

#ifndef INCLUDED_stockplot_GnuPlotDisplay_h
#define INCLUDED_stockplot_GnuPlotDisplay_h

#include "stockplot/PlotDisplay.h"
#include "autil/TempFile.h"
#include "stockplot/Plot.h"

#include <iostream>
#include <string>


namespace alch {

/*!
  \brief Displays a PlotList object using GnuPlot
  \ingroup stockplot
*/
class GnuPlotDisplay : public PlotDisplay
{
 public:

  /*!
    \brief Constructor
  */
  GnuPlotDisplay(Context& ctx);

  virtual ~GnuPlotDisplay();
  
  /*!
    \brief Displays the PlotList using the settings in this object and
    within the PlotList itself.
    \retval true Success
    \retval false Error
  */
  virtual bool show(const PlotListPtr& plotListPtr);


#ifndef NDEBUG
    //! Dumps object contents to stream
    void dump(std::ostream& os) const;
#endif

 private:

  /*!
    \brief Clean up all temporary files
  */
  void cleanUp();


  /*!
    \brief Writes a gnuplot script to fileName to execute the plots described
    by plotlistPtr
    \param fileName The output filename
    \param plotListPtr The plots to create a script for
    \retval true Success
    \retval false Error
  */
  bool writeScript(const std::string& fileName,
                   const PlotListPtr& plotListPtr);

  /*!
    \brief Adds to gnuplot script file for a single plot
    \param os File pointer
    \param xOrigin Value for X Origin in gnuplot multiplot mode
    \param yOrigin Value for Y Origin in gnuplot multiplot mode
    \param xSize Value for X Size in gnuplot multiplot mode
    \param ySize Value for Y Size in gnuplot multiplot mode
    \param plotPtr The plot we're printing
    \retval true Success
    \retval false Error
  */
  bool writeScriptPlot(std::ostream& os,
                       double xOrigin,
                       double yOrigin,
                       double xSize,
                       double ySize,
                       const PlotPtr& plotPtr);

  /*!
    \brief Runs a gnuplot script specified by fileName.
    \param fileName The script to execute
    \retval true Success
    \retval false Error
  */
  bool runScript(const std::string& fileName);


  /*!
    \brief Converts a StockTime into a string formatted by s_timeFormat
    \param stockTime The time to convert
    \return Time formatted by s_timeFormat
  */
  std::string stockTimeToString(const StockTime& stockTime);

  /*!
    \brief Writes the plot data contained in dataPtr to a temporary file, 
    returning the file name in fileName
    \param dataPtr The data to write
    \param fileName [out] The file name that was used
    \retval true Success
    \retval false Error

    On success, the file specified by fileName will have been created by
    creating a TempFilePtr and adding it to m_plotDataFiles.
  */
  bool writePlotDataToFile(const PlotDataPtr& dataPtr, std::string& fileName);


  /*!
    \brief Determines the gnuplot style string to use based on the value
    stored in dataPtr.
    \param dataPtr The data containg the plot style to use
    \return gnuplot compliant style string, or 'lines' if not recognized
  */
  std::string determinePlotDataStyle(const PlotDataPtr& dataPtr);


  //! Format string for all time output
  static const char* const s_timeFormat;

  //! Prefix to use for temporary files
  static const char* const s_filePrefix;

  //! Temporary file delete mode
  static const TempFile::DelMode s_delMode;

  //! Temporary file used to hold gnuplot script
  TempFilePtr m_scriptTempFile;

  //! Temporary files used to hold gnuplot data files
  std::vector<TempFilePtr> m_plotDataFiles;
};

} // namespace alch

#endif
