#include "alchemylist/AlchemyList.h"

#include "stockdata/YahooStockDataSource.h"
#include "stockdata/StockID.h"
#include "afwk/PathRegistry.h"

#include <iostream>
#include <sstream>


namespace alch {

  AlchemyList::AlchemyList()
    : Framework()
  {
    ;
  }


  AlchemyList::~AlchemyList()
  {
    ;
  }

  std::string AlchemyList::getApplicationDescription() const
  {
    std::stringstream ss;
    ss <<
      "This application is responsible for listing the symbols that are\n"
      "available.\n"
      ;

    return ss.str();
  }

  bool AlchemyList::initialize()
  {
    return Framework::initialize();
  }

  bool AlchemyList::finalize()
  {
    return Framework::finalize();
  }

  Framework::OptionsReturnCode AlchemyList::processOptions(
    int argc, char** argv)
  {
    return Framework::processOptions(argc, argv);
  }


  bool AlchemyList::processApplication()
  {
    return printSymbolList();
  }


  bool AlchemyList::printSymbolList()
  {
    YahooStockDataSource yahoo(getContext());
    std::vector<StockID> stockList;

    if (yahoo.getStockList(stockList))
    {
      std::vector<StockID>::const_iterator end = stockList.end();
      std::vector<StockID>::const_iterator iter;
      for (iter = stockList.begin(); iter != end; ++iter)
      {
        std::cout << *iter << "\n";
      }

      getContext() << Context::PRIORITY_info
                   << "Wrote " << stockList.size() << " symbols to STDOUT"
                   << Context::endl;

      getContext() << Context::PRIORITY_info
                   << "Store the list of symbols you wish to retrieve "
                   << "in the file '"
                   << PathRegistry::getSymbolFile()
                   << "'" << Context::endl;
      return true;
    }
    else
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to get stock list"
                   << Context::endl;
      return false;
    }
  }

} // namespace alch
