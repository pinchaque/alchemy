// -*- C++ -*-

#ifndef INCLUDED_stockdata_FileStockDataSource_h
#define INCLUDED_stockdata_FileStockDataSource_h

#include "stockdata/StockDataSource.h"

#include <string>

namespace alch {


/*!
  \brief Provides access to stock data files located under a root location
  \ingroup stockdata
*/
class FileStockDataSource : public StockDataSource
{
 public:

  //! Constructor
  FileStockDataSource(const std::string& rootDir, Context& ctx);

  //! Destructor
  virtual ~FileStockDataSource();

  virtual bool getStockList(std::vector<StockID>& stockList);

  virtual bool hasSymbol(const StockID& id);

  virtual bool retrieve(const StockID& id, StockInfo& info, RangeData& data);

  virtual bool hasSymbolDate(const StockID& id, 
                             const StockTime& start,
                             const StockTime& end);

  virtual bool retrieveDate(const StockID& id, 
                            const StockTime& start,
                            const StockTime& end,
                            StockInfo& info,
                            RangeData& data);

  virtual bool save(const StockID& id, 
                    const StockTime& start,
                    const StockTime& end,
                    const StockInfo& info,
                    const RangeData& data);

  virtual bool remove(const StockID& id);


  /*!
    \brief Returns directory in which the stock symbol is stored
    \param root Root directory
    \param symbol Symbol name
    \returns Directory name
  */
  static std::string getSymbolDirectory(
    const std::string& root,
    const std::string& symbol);


  /*!
    \brief Returns the path to the data file for a given stock symbol
    \param root The root directory in which the FileStockDataSource is located
    \param symbol The name of the stock symbol
   */
  static std::string getSymbolFile(const std::string& root,
                                   const std::string& symbol);


  const std::string& getRootDir() const
  {
    return m_rootDir;
  }


  //! Returns append mode when saving files
  bool getAppend() const
  {
    return m_append;
  }


  //! Sets append mode when saving files
  void setAppend(bool val)
  {
    m_append = val;
  }


 private:

  /*!
    \brief Determines if the specified filePath exists.
    \param fileName The file to look for
    \param retval [out] This is only set when this method returns false. if
    set to true, then the file did not exist. If set to false, then the file
    was a directory.
    \retval true File existed
    \retval false File did not exist
  */
  bool isValidFile(const std::string& fileName, bool& retval);


  /*!
    \brief Populates stock list for particular subdirectory
    \param dirString Directory path (error if doesn't exist)
    \param stockList [out] List of stocks to populate
    \retval true Success
    \retval false Error
  */
  bool getStockListDirectory(const std::string& dirString,
                             std::vector<StockID>& stockList);


  //! Root directory for all stock data
  std::string m_rootDir;

  //! Whether we are appending
  bool m_append;

  //! Extension used for files
  static const char* const s_fileExtension;

};

} // namespace alch

#endif
