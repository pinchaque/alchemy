// -*- C++ -*-

#ifndef INCLUDED_alchemygendata_AlchemyGenData_h
#define INCLUDED_alchemygendata_AlchemyGenData_h

#include "afwk/Framework.h"
#include "stockdata/RangeData.h"
#include "nnet/NNetDataset.h"

namespace alch {

/*!
  \brief Generates neural network input data files based on stock data
  \ingroup alchemygendata
*/
class AlchemyGenData : public Framework
{
 public:

  /*!
    \brief Constructor
  */
  AlchemyGenData();

  virtual ~AlchemyGenData();

protected:

  std::string getApplicationName() const
  {
    return "alchemygendata";
  }

  std::string getApplicationDescription() const;

  std::string getApplicationVersion() const
  {
    return VERSIONSTRING;
  }

  virtual bool initialize();

  virtual bool finalize();

  virtual Framework::OptionsReturnCode processOptions(int argc, char** argv);

  virtual bool processApplication();

 private:

  static const char* const s_optionTrain;
  static const char* const s_optionTest;
  static const char* const s_optionDays;
  static const char* const s_optionPercent;
  static const char* const s_optionSymbol;
  static const char* const s_optionRandomize;
  static const char* const s_optionSample;


  std::string m_symbol;
  std::string m_trainFile;
  std::string m_testFile;
  int m_daysAdvance;
  double m_trainRatio;
  double m_sampleRatio;
  RangeDataPtr m_data;

  bool loadParams();

  void printParams();

  bool loadData();

  bool generateDataset(NNetDataset& dataset);

  bool randomizeDataset(NNetDataset& dataset);

  bool outputDataset(const NNetDataset& dataset);

};

} // namespace alch

#endif
