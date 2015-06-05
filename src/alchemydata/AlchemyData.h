// -*- C++ -*-

#ifndef INCLUDED_alchemydata_AlchemyData_h
#define INCLUDED_alchemydata_AlchemyData_h

#include "afwk/Framework.h"

#include "stockdata/StockID.h"
#include "stockdata/RangeData.h"
#include "stockdata/MetaFileStockDataSource.h"

#include <vector>
#include <string>

namespace alch {

/*!
  \brief Runs the AlchemyData application
  \ingroup alchemydata
*/
class AlchemyData : public Framework
{
 public:

  //! Convenience typedef
  typedef std::vector<StockID> VecStockID;

  /*!
    \brief Constructor
  */
  AlchemyData();

  virtual ~AlchemyData();

protected:

  std::string getApplicationName() const
  {
    return "alchemydata";
  }

  std::string getApplicationVersion() const
  {
    return VERSIONSTRING;
  }

  std::string getApplicationDescription() const;

  virtual bool initialize();

  virtual bool finalize();

  virtual Framework::OptionsReturnCode processOptions(int argc, char** argv);

  virtual bool processApplication();

 private:

  static const char* const s_optionSymbol;
  static const char* const s_optionFile;


  /*!
    \brief Reads in specified list file
    \param readList [out] The list that was read
    \param file The name of the file to read from
    \param item The name of the things we're reading (for messages)
    \retval true Success
    \retval false Error

    Lines beginning with # are ignored as comments
    List can be on separate lines or all on same line.
  */
  bool AlchemyData::getList(VecStockID& readList,
                            const std::string& file,
                            const std::string& item);

  /*!
    \brief Reads in specified symbol list file
    \param symbolList [out] The symbol list that was read
    \param symbolFile The name of the file to read from
    \retval true Success
    \retval false Error

    Lines beginning with # are ignored as comments
    List of symbols can be on separate lines or all on same line.
  */
  bool getSymbolList(VecStockID& symbolList,
                     const std::string& symbolFile);

  /*!
    \brief Processes symbol list file
    \retval true Success
    \retval false Error
   */
  bool processSymbolList(const std::string& symbolFile);

  /*!
    \brief Downloads data for the specified symbol
    \param symbol The symbol for which data will be downloaded
    \retval true Success
    \retval false Error
  */
  bool processSymbol(const StockID& symbol);

  /*!
    \brief Persists specified data to disk
    \param id The ID of the symbol from whence the data came
    \param data The data records to persist
    \param startTime The starting time for this data
    \param endTime The ending time for this data
    \param dataSource The data source we're writing to
    \retval true Success
    \retval false Error
  */
  bool persistData(const StockID& id,
                   const RangeData& data,
                   const StockTime& startTime,
                   const StockTime& endTime,
                   const StockInfo& info,
                   MetaFileStockDataSource& dataSource);

};

} // namespace alch

#endif
