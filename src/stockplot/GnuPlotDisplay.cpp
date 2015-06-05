
#include "stockplot/GnuPlotDisplay.h"
#include "autil/TempFile.h"

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif

#include <cassert>
#include <fstream>
#include <sstream>
#include <ctime>

namespace alch {


  namespace {

    // returns total of weights in plotVec.
    double calculateTotalWeight(const PlotList::PlotPtrVec& plotVec)
    {
      double total = 0.00;

      PlotList::PlotPtrVec::const_iterator end = plotVec.end();
      PlotList::PlotPtrVec::const_iterator iter;
      for (iter = plotVec.begin(); iter != end; ++iter)
      {
        total += (*iter)->getWeight();
      }

      return total;
    }
  }


  const char* const GnuPlotDisplay::s_timeFormat = "%Y-%m-%d";

  const char* const GnuPlotDisplay::s_filePrefix = "gnuplot";

  const TempFile::DelMode GnuPlotDisplay::s_delMode
#ifdef NDEBUG
    = TempFile::DELMODE_destruct; // optimize mode
#else
    = TempFile::DELMODE_destruct; // debug mode
#endif


  GnuPlotDisplay::GnuPlotDisplay(Context& ctx)
    : PlotDisplay(ctx)
    , m_scriptTempFile()
    , m_plotDataFiles()
  {
    ;
  }


  GnuPlotDisplay::~GnuPlotDisplay()
  {
    ;
  }

  
  bool GnuPlotDisplay::show(const PlotListPtr& plotListPtr)
  {
    // clean up all old stuff
    cleanUp();

    m_scriptTempFile = TempFilePtr(
      new TempFile(s_filePrefix, s_delMode));

    // write the script
    if (!writeScript(m_scriptTempFile->getName(), plotListPtr))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to write script to temporary file '"
                   << m_scriptTempFile->getName() << "'" << Context::endl;
      return false;      
    }
    else
    {
      getContext() << Context::PRIORITY_debug2
                   << "GnuPlot script file: '"
                   << m_scriptTempFile->getName() << "'"
                   << Context::endl;
    }

    // run the script
    if (!runScript(m_scriptTempFile->getName()))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to run the GnuPlot script '"
                   << m_scriptTempFile->getName() << "'" << Context::endl;
      return false;      
    }

    return true;
  }


#ifndef NDEBUG
  void GnuPlotDisplay::dump(std::ostream& os) const
  {
    os << "[GnuPlotDisplay]  TimeFormat: '" << s_timeFormat
       << "'  scriptTempFile: '"
       << (m_scriptTempFile.get() ? m_scriptTempFile->getName() : "<NULL>")
       << "'\n";

    std::vector<TempFilePtr>::const_iterator end = m_plotDataFiles.end();
    std::vector<TempFilePtr>::const_iterator iter;
    for (iter = m_plotDataFiles.begin(); iter != end; ++iter)
    {
      os << "  gnuplot file: '" << (*iter)->getName() << "'\n";
    }
  }
#endif


  void GnuPlotDisplay::cleanUp()
  {
    m_scriptTempFile.reset();
    m_plotDataFiles.clear();
  }


  bool GnuPlotDisplay::writeScript(const std::string& fileName,
                                   const PlotListPtr& plotListPtr)
  {
    assert(plotListPtr.get());

    const PlotList::PlotPtrVec& plotVec = plotListPtr->getPlotList();

    if (plotVec.size() < 1)
    {
      getContext() << Context::PRIORITY_error
                   << "No plots specified to GnuPlotDisplay"
                   << Context::endl;
      return false;
    }

    std::ofstream os(fileName.c_str());
    if (!os)
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open file '" << fileName
                   << "' for output." << Context::endl;
      return false;
    }

    std::string xMinString(stockTimeToString(plotListPtr->getXMin()));
    std::string xMaxString(stockTimeToString(plotListPtr->getXMax()));

    os << "set title '" << plotListPtr->getTitle() << "'\n"
       << "set style data fsteps\n"
       << "set timefmt '" << s_timeFormat << "'\n"
       << "set grid\n"
       << "set xdata time\n"
       << "set format x '" << s_timeFormat << "'\n"
       << "set xrange [ '" << xMinString << "':'" << xMaxString << "' ]\n"
       << "set multiplot\n";

    assert(int(plotVec.size()) > 0); 

    // get the total weight for all plots
    double totalWeight = calculateTotalWeight(plotVec);

    double plotXSize = 1.0;
    double plotXOrigin = 0.0;
    double plotYOrigin = 1.0;

    PlotList::PlotPtrVec::const_iterator end = plotVec.end();
    PlotList::PlotPtrVec::const_iterator iter;
    for (iter = plotVec.begin(); iter != end; ++iter)
    {
      // size is ratio of total weight
      double plotYSize = ((*iter)->getWeight() / totalWeight);

      // update origin to account for this size
      plotYOrigin -= plotYSize;

      if (!writeScriptPlot(os, plotXOrigin, plotYOrigin,
                           plotXSize, plotYSize, *iter))
      {
        os.close();
        return false;
      }

      // remove subsequent x-axis tic mark labels
      os << "set format x ''\n";
    }

    // set up for mouse interaction
    os << "unset multiplot\n"
       << "set mouse\n"
       << "pause -1\n";

    // successful if that filehandle is still good
    bool success = os;

    os.close();

    return success;
  }


  bool GnuPlotDisplay::writeScriptPlot(std::ostream& os,
                                       double xOrigin,
                                       double yOrigin,
                                       double xSize,
                                       double ySize,
                                       const PlotPtr& plotPtr)
  {
    os << "set ylabel '" << plotPtr->getYAxisLabel() << "'\n";

    // set up y-axis scaling
    if (plotPtr->getYAutoscale())
    {
      os << "set yrange [ * : * ]\n"
         << "set ytics mirror autofreq\n"
         << "set autoscale y\n";
    }
    else
    {
      os << "set yrange [ " << plotPtr->getYMin() << " : "
         << plotPtr->getYMax() << " ]\n"
         << "set ytics mirror autofreq\n"
         << "unset autoscale y\n";
    }

    // set up y-axis scale
    if (plotPtr->getLogarithmic())
    {
      os << "set logscale y\n"
         << "set ytics mirror .5 \n";
    }
    else
    {
      os << "unset logscale y\n";
    }

    // set up key (legend) display
    if (plotPtr->getShowKey())
    {
      os << "set key on left top Left reverse spacing 0.5 samplen 2\n";
    }
    else
    {
      os << "set key off\n";
    }

    os << "set size " << xSize << "," << ySize << "\n"
       << "set origin " << xOrigin << "," << yOrigin << "\n"
       << "plot ";

    // the plot command will be split over several lines. we use the variable
    // 'first' to keep track if we're on the first line (in which case we
    // don't need to preceed the command with a comma)
    bool first = true;

    Plot::PlotDataPtrVec plotData = plotPtr->getPlotData();

    // go through all PlotData objects in the price Plot
    Plot::PlotDataPtrVec::const_iterator end = plotData.end();
    Plot::PlotDataPtrVec::const_iterator iter;
    for (iter = plotData.begin(); iter != end; ++iter)
    {
      std::string fileName("");
      if (!writePlotDataToFile(*iter, fileName))
      {
        return false;
      }

      std::string style = determinePlotDataStyle(*iter);
      std::string title = (*iter)->getLabel();

      if (!first)
      {
        os << ", \\\n     ";
      }
      else
      {
        first = false;
      }

      os << "'" << fileName << "' using 1:2 title '" << title
         << "' with " << style;
    }

    // terminate the plot command
    os << "\n";

    return os;
  }


  bool GnuPlotDisplay::runScript(const std::string& fileName)
  {
    std::stringstream cmd;
    cmd << "gnuplot " << fileName << " 2>&1";
    FILE* pipe = ::popen(cmd.str().c_str(), "r");
    if (!pipe)
    {
      return false;
    }

    const int bufSize = 8192;
    char buf[bufSize + 1] = "";
    
    while (::fgets(buf, bufSize, pipe))
    {
      // strip off newline
      char* p = strchr(buf, '\n');
      if (p)
      {
        *p = '\0';
      }

      getContext() << Context::PRIORITY_error << buf << Context::endl;
    }

    return (::pclose(pipe) != -1);
  }


  /*
    In order to get StockTime into a string formatted with s_timeFormat we
    need to first put it into a struct tm. This is a bit of a hack, but at 
    least it allows us to maintain only one format string.
  */
  std::string GnuPlotDisplay::stockTimeToString(const StockTime& stockTime)
  {
    // the time structure
    struct tm timeStruct;
    memset(&timeStruct, 0, sizeof(struct tm));

    // convert stockTime into timeStruct
    boost::gregorian::date date(stockTime.date());
    boost::posix_time::time_duration duration(stockTime.time_of_day());
    timeStruct.tm_sec = duration.seconds();
    timeStruct.tm_min = duration.minutes();
    timeStruct.tm_hour = duration.hours();
    timeStruct.tm_mday = date.day();
    timeStruct.tm_mon = date.month() - 1;
    timeStruct.tm_year = date.year() - 1900;
    
    // write the time struct to a buffer and return it
    const int bufSize = 512;
    char buf[bufSize + 1] = "";
    ::strftime(buf, bufSize, s_timeFormat, &timeStruct);
    buf[bufSize] = 0;
    return buf;
  }


  bool GnuPlotDisplay::writePlotDataToFile(const PlotDataPtr& dataPtr,
                                           std::string& fileName)
  {
    TempFilePtr tempFile(new TempFile(s_filePrefix, s_delMode));

    // open up our output file
    std::ofstream os(tempFile->getName().c_str());
    if (!os)
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open temporary file for writing "
                   << "plot data '"
                   << tempFile->getName() << "'" << Context::endl;
      return false;
    }
    else
    {
      getContext() << Context::PRIORITY_debug2
                   << "GnuPlot data file: '"
                   << tempFile->getName() << "'"
                   << Context::endl;
    }

    const PlotDataSegmentPtrVec& segments = dataPtr->getDataSegments();
    PlotDataSegmentPtrVec::const_iterator end = segments.end();
    PlotDataSegmentPtrVec::const_iterator iter;
    for (iter = segments.begin(); iter != end; ++iter)
    {
      assert(iter->get());
      PlotDataSegment::const_iterator endS = (*iter)->end();
      PlotDataSegment::const_iterator iterS;
      for (iterS = (*iter)->begin(); iterS != endS; ++iterS)
      {
        os << stockTimeToString(iterS->timestamp) << ' ' << iterS->value
           << '\n';
      }
      os << '\n';
    }

    if (!os)
    {
      return false;
    }
    os.close();

    // add to vector so we can delete it later
    m_plotDataFiles.push_back(tempFile);

    // return the file name
    fileName = tempFile->getName();

    return true;
  }


  std::string GnuPlotDisplay::determinePlotDataStyle(
    const PlotDataPtr& dataPtr)
  {
    switch (dataPtr->getStyle())
    {
      case PlotData::STYLE_lines:
        return "lines";
        break;

      case PlotData::STYLE_points:
        return "points";
        break;

      case PlotData::STYLE_linespoints:
        return "linespoints";
        break;

      case PlotData::STYLE_impulses:
        return "impulses";
        break;

      case PlotData::STYLE_dots:
        return "dots";
        break;

      case PlotData::STYLE_steps:
        return "steps";
        break;

      case PlotData::STYLE_fsteps:
        return "fsteps";
        break;

      case PlotData::STYLE_histeps:
        return "histeps";
        break;

      case PlotData::STYLE_boxes:
        return "boxes";
        break;

      default:
        return "lines";
        break;
    }
  }


} // namespace alch
