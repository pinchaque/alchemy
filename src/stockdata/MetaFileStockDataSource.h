

#ifndef INCLUDED_stockdata_MetaFileStockDataSource_h
#define INCLUDED_stockdata_MetaFileStockDataSource_h

#include "stockdata/FileStockDataSource.h"

#include "stockdata/StockMetaData.h"

#include <string>

namespace alch {


/*!
  \brief Provides access to stock data files located under a root location
  \ingroup stockdata

  Provides the same functionality as FileStockDataSource, except this class
  also has a metafile that keeps track of which data is in the data files.
  Also, data files are sorted for easy access.
*/
class MetaFileStockDataSource : public FileStockDataSource
{
 public:

  //! Constructor
  MetaFileStockDataSource(const std::string& rootDir, Context& ctx);

  //! Destructor
  virtual ~MetaFileStockDataSource();

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
    \brief Returns meta file name for specified stock
    \param id The ID of the stock
    \return File name
  */
  std::string getMetaFileName(const StockID& id) const;


  /*!
    \brief Reads stock meta data in for specified id
    \param id The ID of the stock
    \param metaData [out] Metadata structure to populate
    \retval true Success
    \retval false Error
  */
  bool readMetaFile(const StockID& id, StockMetaData& metaData);

 private:

  /*!
    \brief Saves data to the data and meta files, overwriting existing contents
    \param id The ID of the stock to which this data belongs
    \param start The start date represented by this new data
    \param end The end date represented by this new data
    \param data The data to write
    \retval true Success
    \retval false Error
  */
  bool saveOverwrite(const StockInfo& info, 
                     const StockTime& start,
                     const StockTime& end,
                     const RangeData& data);

  /*!
    \brief Saves data to the data and meta files, appending to existing 
    contents
    \param id The ID of the stock to which this data belongs
    \param start The start date represented by this new data
    \param end The end date represented by this new data
    \param data The data to write
    \retval true Success
    \retval false Error
  */
  bool saveAppend(const StockInfo& info, 
                  const StockTime& start,
                  const StockTime& end,
                  const RangeData& data);

  /*!
    \brief Saves data to the data and meta files, merging with existing
    contents
    \param id The ID of the stock to which this data belongs
    \param start The start date represented by this new data
    \param end The end date represented by this new data
    \param data The data to write
    \retval true Success
    \retval false Error
  */
  bool saveMerge(const StockInfo& info, 
                 const StockTime& start,
                 const StockTime& end,
                 const RangeData& data);
 
  /*!
    \brief Writes out meta data to the appropriate file
    \param metaData Metadata structure to write out (including ID)
    \retval true Success
    \retval false Error

    Will overwrite any existing metadata file for this stock
  */
  bool writeMetaFile(const StockMetaData& metaData);

};

} // namespace alch

#endif
