// -*- C++ -*-

#ifndef INCLUDED_alchemyprofile_AlchemyProfile_h
#define INCLUDED_alchemyprofile_AlchemyProfile_h

#include "afwk/Framework.h"
#include "stockdata/StockID.h"
#include "stockdata/RangeData.h"
#include "stocknnet/PredictionProfile.h"
#include "nnet/NNetDataset.h"

#include <vector>
#include <string>
#include <ostream>

namespace alch {

/*!
  \brief Runs the AlchemyProfile application
  \ingroup alchemyprofile
*/
  class AlchemyProfile : public Framework
  {
  public:

    //! Convenience typedef
    typedef std::vector<StockID> VecStockID;

    /*!
      \brief Constructor
    */
    AlchemyProfile();

    virtual ~AlchemyProfile();

  protected:

    std::string getApplicationName() const
    {
      return "alchemyprofile";
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
    static const char* const s_optionOutput;
    static const char* const s_optionProfile;

    std::string m_outputFile;
    std::vector<PredictionProfile> m_profiles;


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
    bool AlchemyProfile::getList(VecStockID& readList,
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
    \brief Reads symbol list file to populate symbolList
    \param symbolList [out] Symbol list to populate
    \retval true Success
    \retval false Error
  */
  bool retrieveSymbolList(VecStockID& symbolList);

  /*!
    \brief Processes all symbols in specified symbolList vector
    \retval true Success
    \retval false Error
  */
  bool processSymbolList(const VecStockID& symbolList);


  /*!
    \brief Prints CSV header to output stream
    \retval true Success
    \retval false Error
  */
  bool printHeader(std::ostream& os);

  /*!
    \brief Downloads data for the specified symbol
    \param symbol The symbol for which data will be downloaded
    \param os Output stream to write symbol data to
    \retval true Success
    \retval false Error
  */
  bool processSymbol(const StockID& symbol, std::ostream& os);


  /*!
    \brief Initializes the m_profiles data member based on parameters
    \param profileList List of profile names
    \retval true Success
    \retval false Error
   */
  bool initializeProfiles(const std::string& profileList);
  

  /*!
    \brief Retrieves stock data for specified symbol
    \param symbol Symbol to retrieve data for
    \param stockData [out] Stock data to populate
    \retval true Success
    \retval false Error
   */
  bool retrieveData(const StockID& symbol, RangeDataPtr& stockData);



  /*!
    \brief Processes profile for specified symbol
    \param symbol Symbol we're analyzing
    \param rangeData Data associated with that symbol
    \param nnetdataset The neural network dataset that came from the specified
    rangeData
    \param profile Prediction profile to use
    \param os [out] Output stream to write data to
    \retval true Success
    \retval false Error
   */
  bool processSymbolProfile(const StockID& symbol,
                            RangeDataPtr rangeData,
                            const NNetDataset& nnetDataset,
                            const PredictionProfile& profile,
                            std::ostream& os);
};

} // namespace alch

#endif
