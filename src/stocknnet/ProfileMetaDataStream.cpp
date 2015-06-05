
#include "stocknnet/ProfileMetaDataStream.h"

#include <map>
#include <string>

namespace alch {

  namespace ProfileMetaDataStream
  {

    namespace { // anonymous

      const char* c_nameTag = "Name";

      const char* c_daysTag = "Days";

      // adds invalid line message
      void invalidLine(const std::string& str, Context& ctx)
      {
        ctx << Context::PRIORITY_error
            << "Invalid line encountered while parsing prediction profile "
            << "metadata: '" << str << "'" << Context::endl;
      }

    } // anonymous namespace


    bool read(std::istream& is,
              PredictionProfile& data,
              Context& ctx)
    {
      typedef std::map<std::string, std::string> mapType;
      mapType tagMap;
      while (is)
      {
        std::string str;
        std::getline(is, str);

        // ignore blank lines
        if (!str.length())
        {
          continue;
        }

        // parse line into tag value pairs based on space
        std::string::size_type spacePos = str.find(' ');
        if (spacePos == std::string::npos)
        {
          invalidLine(str, ctx);
          return false;
        }

        std::string tag = str.substr(0, spacePos);
        std::string value = str.substr(spacePos + 1);

        // save the pair for later
        tagMap[tag] = value;
      }

      // now look to make sure we got all the tags we needed
      mapType::const_iterator tagMapEnd = tagMap.end();
      if ((tagMap.find(c_nameTag) == tagMapEnd)
          || (tagMap.find(c_daysTag) == tagMapEnd))
      {
        ctx << Context::PRIORITY_error
            << "Invalid or incomplete prediction profile metadata"
            << Context::endl;
        return false;
      }

      // ok, we got all the tags, so we write the data
      data.setName(tagMap[c_nameTag]);


      int days = ::atoi(tagMap[c_daysTag].c_str());

      if (days > 0)
      {
        data.setNumberDays(days);
      }
      else
      {
        ctx << Context::PRIORITY_error
            << "Invalid setting for parameter '" << c_daysTag << "' ("
            << tagMap[c_daysTag] << ") in profile metadata"
            << Context::endl;
        return false;
      }

      return true;
    }

    bool write(std::ostream& os,
               const PredictionProfile& data,
               Context& ctx)
    {
      os << c_nameTag << " " << data.getName() << "\n";

      os << c_daysTag << " " << data.getNumberDays() << "\n";

      return os;
    }


  } // namespace ProfileMetaDataStream

} // namespace alch
