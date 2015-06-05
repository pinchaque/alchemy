
#include "stockdata/StockMetaDataStream.h"

#include <map>
#include <string>
#include <sstream>

namespace alch {

  namespace StockMetaDataStream
  {

    namespace { // anonymous
      const char* c_symbolTag = "Symbol";
      const char* c_startTag = "Start";
      const char* c_endTag = "End";
      const char* c_descriptionTag = "Description";
      const char* c_expenseRatioTag = "ExpenseRatio";
      const char* c_12b1FeeTag = "12b1Fee";
      const char* c_familyTag = "Family";
      const char* c_ratingTag = "Rating";
      const char* c_inceptionTag = "Inception";
      const char* c_initInvestmentTag = "InitInvestment";
      const char* c_subsInvestmentTag = "SubsInvestment";
      const char* c_assetsTag = "Assets";
      const char* c_turnoverTag = "Turnover";
      const char* c_yieldTag = "Yield";
      const char* c_deferredLoadTag = "DeferredLoad";
      const char* c_frontLoadTag = "FrontLoad";
      const char* c_categoryTag = "Category";

      // parses a date in iso format from str, returning false on error
      bool parseDate(const std::string& str, StockTime& stockTime)
      {
        try 
        {
          stockTime = boost::posix_time::from_iso_string(str);
        }
        catch (std::exception ex)
        {
          return false;
        }
        return !stockTime.date().is_not_a_date();
      }

      // adds invalid line message
      void invalidLine(const std::string& str, Context& ctx)
      {
        ctx << Context::PRIORITY_error
            << "Invalid line encountered while parsing stock metadata: '"
            << str << "'" << Context::endl;
      }

      double string2double(const std::string& str)
      {
        double d = 0.0;
        std::istringstream iss(str);
        iss >> d;
        return d;
      }

      int string2int(const std::string& str)
      {
        int i = 0;
        std::istringstream iss(str);
        iss >> i;
        return i;
      }

    template<class T>
    void writeLine(
        std::ostream& os, 
        const std::string& key, 
        const T& value, Context& ctx)
    {
      std::stringstream ss;
      ss << value;

      // remove newlines from the string
      std::string value2(ss.str());
      std::string::size_type pos;
      while ((pos = value2.find_first_of('\n')) != std::string::npos)
      {
        value2[pos] = ' ';
      }
      os << key << " " << value2 << "\n";
    }

    } // anonymous namespace


    bool read(std::istream& is,
              StockMetaData& data,
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

        // trim off newlines
        std::string::size_type nlPos = str.find('\n');
        if (nlPos != std::string::npos)
        {
          str.erase(nlPos);
        }

        std::string tag = str.substr(0, spacePos);
        std::string value = str.substr(spacePos + 1);

        // save the pair for later
        tagMap[tag] = value;
      }

      // now look to make sure we got all the tags we needed
      mapType::const_iterator tagMapEnd = tagMap.end();
      if ((tagMap.find(c_symbolTag) == tagMapEnd)
          || (tagMap.find(c_startTag) == tagMapEnd)
          || (tagMap.find(c_endTag) == tagMapEnd))
      {
        ctx << Context::PRIORITY_error
            << "Invalid or incomplete stock metadata" << Context::endl;
        return false;
      }

      // ok, we got all the tags, so we write the data
      StockInfo& sid = data.stockInfo;

      StockID id = sid.getID();
      id.setSymbol(tagMap[c_symbolTag]);
      sid.setID(id);

      if (tagMap.find(c_descriptionTag) != tagMapEnd)
      { sid.setName(tagMap[c_descriptionTag]); }

      if (tagMap.find(c_familyTag) != tagMapEnd)
      { sid.setFamily(tagMap[c_familyTag]); }
      
      if (tagMap.find(c_categoryTag) != tagMapEnd)
      { sid.setCategory(tagMap[c_categoryTag]); }
    
      if (tagMap.find(c_expenseRatioTag) != tagMapEnd)
      { sid.setExpenseRatio(string2double(tagMap[c_expenseRatioTag])); }
    
      if (tagMap.find(c_12b1FeeTag) != tagMapEnd)
      { sid.set12b1Fee(string2double(tagMap[c_12b1FeeTag])); }
      
      if (tagMap.find(c_turnoverTag) != tagMapEnd)
      { sid.setTurnover(string2double(tagMap[c_turnoverTag])); }
      
      if (tagMap.find(c_yieldTag) != tagMapEnd)
      { sid.setYield(string2double(tagMap[c_yieldTag])); }
      
      if (tagMap.find(c_deferredLoadTag) != tagMapEnd)
      { sid.setDeferredLoad(string2double(tagMap[c_deferredLoadTag])); }
      
      if (tagMap.find(c_frontLoadTag) != tagMapEnd)
      { sid.setFrontLoad(string2double(tagMap[c_frontLoadTag])); }
      
      if (tagMap.find(c_ratingTag) != tagMapEnd)
      { sid.setRating(string2int(tagMap[c_ratingTag])); }
      
      if (tagMap.find(c_initInvestmentTag) != tagMapEnd)
      { sid.setInitInvestment(string2int(tagMap[c_initInvestmentTag])); }
      
      if (tagMap.find(c_subsInvestmentTag) != tagMapEnd)
      { sid.setSubsInvestment(string2int(tagMap[c_subsInvestmentTag])); }
      
      if (tagMap.find(c_assetsTag) != tagMapEnd)
      { sid.setAssets(string2int(tagMap[c_assetsTag])); }

      StockTime incep;
      parseDate(tagMap[c_inceptionTag], incep);
      sid.setInception(incep);

      // make sure we have valid values
      if (!parseDate(tagMap[c_startTag], data.start)
          || !parseDate(tagMap[c_endTag], data.end)
          || !data.stockInfo.getSymbol().size())
      {
        ctx << Context::PRIORITY_error
            << "Invalid or incomplete stock metadata for "
            << data.stockInfo.getSymbol()
            << Context::endl;
        return false;
      }

      return true;
    }

    bool write(std::ostream& os,
               const StockMetaData& data,
               Context& ctx)
    {
      const StockInfo& sid = data.stockInfo;
      
      writeLine(os, c_symbolTag, sid.getSymbol(), ctx);
      writeLine(os, c_startTag, boost::posix_time::to_iso_string(data.start), ctx);
      writeLine(os, c_endTag, boost::posix_time::to_iso_string(data.end), ctx);
      writeLine(os, c_descriptionTag, sid.getName(), ctx);
      writeLine(os, c_expenseRatioTag, sid.getExpenseRatio(), ctx);
      writeLine(os, c_12b1FeeTag, sid.get12b1Fee(), ctx);
      writeLine(os, c_familyTag, sid.getFamily(), ctx);
      writeLine(os, c_ratingTag, sid.getRating(), ctx);
      writeLine(os, c_inceptionTag, 
          boost::posix_time::to_iso_string(sid.getInception()), ctx);
      writeLine(os, c_initInvestmentTag, sid.getInitInvestment(), ctx);
      writeLine(os, c_subsInvestmentTag, sid.getSubsInvestment(), ctx);
      writeLine(os, c_assetsTag, sid.getAssets(), ctx);
      writeLine(os, c_turnoverTag, sid.getTurnover(), ctx);
      writeLine(os, c_yieldTag, sid.getYield(), ctx);
      writeLine(os, c_deferredLoadTag, sid.getDeferredLoad(), ctx);
      writeLine(os, c_frontLoadTag, sid.getFrontLoad(), ctx);
      writeLine(os, c_categoryTag, sid.getCategory(), ctx);

      return os;
    }


  } // namespace StockMetaDataStream

} // namespace alch
