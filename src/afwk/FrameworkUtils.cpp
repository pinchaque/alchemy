
#include "afwk/FrameworkUtils.h"

#include "stockdata/StockTimeUtil.h"
#include "stockdata/StockListStream.h"

namespace alch {

namespace FrameworkUtils {

  static const char* const s_optionStart = "start";
  static const char* const s_optionEnd = "end";
  static const char* const s_optionDuration = "duration";

  const char* const getOptionStart()
  {
    return s_optionStart;
  }

  const char* const getOptionEnd()
  {
    return s_optionEnd;
  }

  const char* const getOptionDuration()
  {
    return s_optionDuration;
  }


  StockTime stringToTime(const std::string& str)
  {
    if (str.find(' ') == std::string::npos)
    {
      return boost::posix_time::time_from_string(str + " 13:00:00");
    }
    else
    {
      return boost::posix_time::time_from_string(str);
    }
  }


  bool getDateRange(FrameworkOptions::VariablesMap& vm,
                    StockTime& startTime,
                    StockTime& endTime,
                    Context& ctx,
                    int duration)
  {
    // duration of plot
    if (vm.count(getOptionDuration()))
    {
      duration = vm[getOptionDuration()].as<int>();
      if (duration <= 0)
      {
        ctx << Context::PRIORITY_error
            << "Invalid duration specified: " << duration
            << Context::endl;
        return false;
      }
    }

    // initial values
    endTime = boost::posix_time::second_clock::local_time();
    startTime = endTime - boost::gregorian::date_duration(duration);

    if (vm.count(getOptionStart()) && vm.count(getOptionEnd()))
    {
      startTime = stringToTime(vm[getOptionStart()].as<std::string>());
      endTime = stringToTime(vm[getOptionEnd()].as<std::string>());
    }
    else if (vm.count(getOptionStart()))
    {
      startTime = stringToTime(vm[getOptionStart()].as<std::string>());
      endTime = startTime + boost::gregorian::date_duration(duration);
    }
    else if (vm.count(getOptionEnd()))
    {
      endTime = stringToTime(vm[getOptionEnd()].as<std::string>());
      startTime = endTime - boost::gregorian::date_duration(duration);
    }

    endTime = StockTimeUtil::getPreviousClose(endTime);

    ctx << Context::PRIORITY_debug1
        << "Start date: " << startTime
        << Context::endl;

    ctx << Context::PRIORITY_debug1
        << "End date:   " << endTime
        << Context::endl;

    return true;
  }

  bool FrameworkUtils::getList(StockList& readList,
      const std::string& file,
      Context& ctx)
  {
    std::ifstream ifs(file.c_str());
    if (!ifs)
    {
      ctx << Context::PRIORITY_error
        << "Failed to open file '" << file << "'"
        << Context::endl;
      return false;
    }

    ctx << Context::PRIORITY_debug2
      << "Reading symbol file '" << file << "'"
      << Context::endl;

    if (!StockListStream::read(ifs, readList, ctx))
    {
      ctx << Context::PRIORITY_error
        << "Failed while reading list from file '" << file << "'"
        << Context::endl;
      return false;
    }

    ifs.close();

    ctx << Context::PRIORITY_debug1
      << "Read " << readList.size()
      << " symbols from file '"
      << file << "'" << Context::endl;

    return true;
  }


} // namespace FrameworkUtils

} // namespace alch
