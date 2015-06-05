// -*- C++ -*-

#ifndef INCLUDED_stockdata_OStockDataFile_h
#define INCLUDED_stockdata_OStockDataFile_h

#include "autil/Context.h"
#include "stockdata/RangeData.h"
#include <fstream>
#include <string>

namespace alch {

/*!
  \brief Outputs a stock data file
  \ingroup stockdata
*/
class OStockDataFile
{
 public:

  //! Mode in which files are opened
  enum Mode
  {
    MODE_append = 1, //!< Append mode (file writing starts at end)
    MODE_overwrite = 2 //!< Overwrite mode (file is cleared before writing)
  };


  /*!
    \brief Constructor
  */
  OStockDataFile(Context& ctx);


  /*!
    \brief Destructor
  */
  ~OStockDataFile();


  //! Returns the file name associated with this class; empty if none
  const std::string& getFileName() const
  {
    return m_fileName;
  }


  /*!
    \brief Opens file for output in specified mode
    \param fileName The file name to open
    \param mode The mode in which the file should be opened
    \retval true Success
    \retval false Error
  */
  bool open(const std::string& fileName, Mode mode);


  /*!
    \brief Writes stock data contained in RangeData to the file
    \param data The data to write
    \retval true Success
    \retval false Error
  */
  bool write(const RangeData& data);


  /*!
    \brief Writes a stock data point to the file
    \param dataPoint The datapoint to write
    \retval true Success
    \retval false Error
  */
  bool write(const RangeData::Point& dataPoint);


  /*!
    \brief Closes the file
  */
  void close();


 private:
  
  //! Context for operational messages
  Context& m_ctx;

  //! The file stream
  std::fstream m_fstream;

  //! The file name associated with this class
  std::string m_fileName;
};

} // namespace alch

#endif
