// -*- C++ -*-

#ifndef INCLUDED_stockdata_IStockDataFile_h
#define INCLUDED_stockdata_IStockDataFile_h

#include "autil/Context.h"
#include "stockdata/RangeData.h"
#include <fstream>
#include <string>

namespace alch {

/*!
  \brief Reads in stock data from a file
  \ingroup stockdata
*/
class IStockDataFile
{
 public:

  /*!
    \brief Constructor
  */
  IStockDataFile(Context& ctx);


  /*!
    \brief Destructor
  */
  ~IStockDataFile();


  //! Returns the file name associated with this class; empty if none
  const std::string& getFileName() const
  {
    return m_fileName;
  }


  /*!
    \brief Opens file for input
    \param fileName The file name to open
    \retval true Success
    \retval false Error
  */
  bool open(const std::string& fileName);


  /*!
    \brief Reads stock data into the specified RangeData
    \param data Where the read data is stored
    \retval true Success
    \retval false Error

    This method will append to the data already contained in data, if any.
  */
  bool read(RangeData& data);


  /*!
    \brief Reads a data point
    \param dataPoint The datapoint to read into
    \retval true Success
    \retval false Error
  */
  bool read(RangeData::Point& dataPoint);


  /*!
    \brief Rewinds file pointer to beginning of file
    \retval true File stream still valid
    \retval false File stream invalid after rewind
  */
  bool rewind();


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
