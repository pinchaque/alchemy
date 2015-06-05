
#include "stockdata/StockListStream.h"

#include <map>
#include <string>
#include <sstream>

namespace alch {

  namespace StockListStream
  {

    bool read(std::istream& is,
              StockList& data,
              Context& ctx)
    {
      data.clear();

      std::string line;
      while (std::getline(is, line))
      {
        // skip leading blanks
        std::string::size_type pos = line.find_first_not_of(" ");
        if (pos != std::string::npos)
        {
          line = line.substr(pos);
        }

        // skip comment lines
        if (line.size() && (line[0] == '#'))
        {
          continue;
        }
        // first word is the symbol, everything else is description
        std::string symbol = line;
        pos = line.find(" ");
        if (pos != std::string::npos)
        {
          symbol = line.substr(0, pos);
        }

        if (symbol.length())
        {
          StockID id;
          id.setSymbol(symbol);
          data.add(id);
        }
      }

      return true;
    }

    bool write(std::ostream& os,
               const StockList& data,
               Context& ctx)
    {
      StockList::const_iterator end = data.end();
      StockList::const_iterator iter;
      for (iter = data.begin(); iter != end; ++iter)
      {
        os << iter->getSymbol() << "\n";
      }

      return os;
    }


  } // namespace StockListStream

} // namespace alch
