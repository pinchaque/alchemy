
#include "stockplot/CSVPlotDisplay.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <ctime>
#include <map>

namespace alch {


  const char* const CSVPlotDisplay::s_timeFormat = "%Y-%m-%d";

  CSVPlotDisplay::CSVPlotDisplay(Context& ctx, const std::string& fileName)
    : PlotDisplay(ctx)
    , m_fileName(fileName)
    , m_numValues(0)
  {
    ;
  }


  CSVPlotDisplay::~CSVPlotDisplay()
  {
    ;
  }

  
  bool CSVPlotDisplay::show(const PlotListPtr& plotListPtr)
  {

    std::ofstream os(m_fileName.c_str());
    if (!os)
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open file '" << m_fileName
                   << "' for output." << Context::endl;
      return false;
    }

    // write the header
    if (!writeHeader(os, plotListPtr))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to write header to '" << m_fileName
                   << "'" << Context::endl;
      return false;
    }

    // write the data
    if (!writeData(os, plotListPtr))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to write data to '" << m_fileName
                   << "'" << Context::endl;
      return false;
    }


    return true;
  }



  bool CSVPlotDisplay::writeHeader(std::ostream& os,
                                   const PlotListPtr& plotListPtr)
  {
    assert(plotListPtr.get());

    os << "Date";

    m_numValues = 0;

    const PlotList::PlotPtrVec& plotVec = plotListPtr->getPlotList();

    PlotList::PlotPtrVec::const_iterator plotEnd = plotVec.end();
    PlotList::PlotPtrVec::const_iterator plotIter;
    for (plotIter = plotVec.begin(); plotIter != plotEnd; ++plotIter)
    {
      Plot::PlotDataPtrVec plotData = (*plotIter)->getPlotData();

      // go through all PlotData objects in the price Plot
      Plot::PlotDataPtrVec::const_iterator plotDataEnd = plotData.end();
      Plot::PlotDataPtrVec::const_iterator plotDataIter;
      for (plotDataIter = plotData.begin();
           plotDataIter != plotDataEnd;
           ++plotDataIter)
      {

        os << "," << (*plotDataIter)->getLabel();
        ++m_numValues;
      }
    }

    os << '\n';

    return true;
  }


  bool CSVPlotDisplay::writeData(std::ostream& os,
                                 const PlotListPtr& plotListPtr)
  {
    assert(plotListPtr.get());

    // in order to write out the data, we need to set up a transformation
    // on the data so that it's organized by date. however some bits of
    // data will be missing for specific dates and the data points can
    // be coming in any order, so we use some arrays of pointers to 
    // determine what data is indeed set.
    typedef boost::shared_ptr<double> ValuePtr;
    typedef std::vector<ValuePtr> ValuePtrVec;
    typedef std::map<StockTime, ValuePtrVec> DataMap;
    DataMap dataMap;

    int dataIdx = 0;

    const PlotList::PlotPtrVec& plotVec = plotListPtr->getPlotList();
    PlotList::PlotPtrVec::const_iterator plotEnd = plotVec.end();
    PlotList::PlotPtrVec::const_iterator plotIter;
    for (plotIter = plotVec.begin(); plotIter != plotEnd; ++plotIter)
    {
      Plot::PlotDataPtrVec plotData = (*plotIter)->getPlotData();
      Plot::PlotDataPtrVec::const_iterator plotDataEnd = plotData.end();
      Plot::PlotDataPtrVec::const_iterator plotDataIter;
      for (plotDataIter = plotData.begin();
           plotDataIter != plotDataEnd;
           ++plotDataIter)
      {
        const PlotDataSegmentPtrVec& segments
          = (*plotDataIter)->getDataSegments();
        PlotDataSegmentPtrVec::const_iterator segmentEnd = segments.end();
        PlotDataSegmentPtrVec::const_iterator segmentIter;
        for (segmentIter = segments.begin();
             segmentIter != segmentEnd;
             ++segmentIter)
        {

          PlotDataSegment::const_iterator pointEnd = (*segmentIter)->end();
          PlotDataSegment::const_iterator pointIter;
          for (pointIter = (*segmentIter)->begin();
               pointIter != pointEnd;
               ++pointIter)
          {

            // is this date already in the map?

            if (dataMap.find(pointIter->timestamp) == dataMap.end())
            {
              // nope.. create it with the proper size!
              dataMap[pointIter->timestamp].resize(m_numValues);
            }

            // now add this value
            dataMap[pointIter->timestamp][dataIdx]
              = ValuePtr(new double(pointIter->value));
          }
        }

        ++dataIdx;
      }
    }


    // now that we have the map populated, we can write it out by iterating
    // through it by date. Uninitialized pointers are written out as
    // empty cells.
    DataMap::const_iterator end = dataMap.end();
    DataMap::const_iterator iter;
    for (iter = dataMap.begin(); iter != end; ++iter)
    {
      const StockTime& stockTime(iter->first);
      const ValuePtrVec& valueVec(iter->second);

      // write the date
      os << stockTimeToString(stockTime);

      // write out all elements in valueVec
      ValuePtrVec::const_iterator valueEnd = valueVec.end();
      ValuePtrVec::const_iterator valueIter;
      for (valueIter = valueVec.begin(); valueIter != valueEnd; ++valueIter)
      {
        os << ',';

        if (valueIter->get())
        {
          os << **valueIter;
        }
      }

      os << '\n';
    }

    return os;
  }


  /*
    In order to get StockTime into a string formatted with s_timeFormat we
    need to first put it into a struct tm. This is a bit of a hack, but at 
    least it allows us to maintain only one format string.
  */
  std::string CSVPlotDisplay::stockTimeToString(const StockTime& stockTime)
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

} // namespace alch
