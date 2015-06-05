
#include "stockdata/StockDataStream.h"

#include <boost/tokenizer.hpp>
#include <sstream>

namespace alch {

  namespace StockDataStream
  {


    namespace {

      // parses a double from str, returning false on error
      bool parseDouble(const std::string& str, double& d)
      {
        std::istringstream ss(str);
        ss >> d;
        return ss;
      }

    } // anonymous namespace

    /*
      Data format is:
      open,close,min,max,volume\n
    */

    bool read(std::istream& is,
              RangeData::Point& dataPoint,
              Context& ctx)
    {
      std::string str;
      std::getline(is, str);

      boost::tokenizer<boost::escaped_list_separator<char> > tok(str);

      boost::tokenizer<boost::escaped_list_separator<char> >::iterator end
        = tok.end();
      boost::tokenizer<boost::escaped_list_separator<char> >::iterator iter
        = tok.begin();

      if (std::distance(iter, end) != 7)
      {
        // if we just got to EOF, then we don't bother with the error
        // message, we just return false.
        if (!is.eof())
        {
          ctx << Context::PRIORITY_error << "Invalid line: '" << str << "'"
              << Context::endl;
        }

        return false;
      }

      dataPoint.tradeTime = boost::posix_time::from_iso_string(*iter++);
      
      if (!parseDouble(*iter++, dataPoint.open))
      {
        ctx << Context::PRIORITY_error << "Invalid line: '" << str << "'"
            << Context::endl;
        return false;
      }
      assert(iter != end);

      if (!parseDouble(*iter++, dataPoint.close))
      {
        ctx << Context::PRIORITY_error << "Invalid line: '" << str << "'"
            << Context::endl;
        return false;
      }
      assert(iter != end);

      if (!parseDouble(*iter++, dataPoint.min))
      {
        ctx << Context::PRIORITY_error << "Invalid line: '" << str << "'"
            << Context::endl;
        return false;
      }
      assert(iter != end);

      if (!parseDouble(*iter++, dataPoint.max))
      {
        ctx << Context::PRIORITY_error << "Invalid line: '" << str << "'"
            << Context::endl;
        return false;
      }
      assert(iter != end);

      if (!parseDouble(*iter++, dataPoint.volume))
      {
        ctx << Context::PRIORITY_error << "Invalid line: '" << str << "'"
            << Context::endl;
        return false;
      }
      assert(iter != end);

      if (!parseDouble(*iter++, dataPoint.adjustedClose))
      {
        ctx << Context::PRIORITY_error << "Invalid line: '" << str << "'"
            << Context::endl;
        return false;
      }
      assert(iter == end);

      return true;
    }
    

    bool write(std::ostream& os,
               const RangeData::Point& dataPoint,
               Context& ctx)
    {
      os << std::setprecision(12)
         << boost::posix_time::to_iso_string(dataPoint.tradeTime) << ","
         << dataPoint.open << ","
         << dataPoint.close << ","
         << dataPoint.min << ","
         << dataPoint.max << ","
         << dataPoint.volume << ","
         << dataPoint.adjustedClose
         << "\n";
      return os;
    }

  } // namespace StockDataStream

} // namespace alch
