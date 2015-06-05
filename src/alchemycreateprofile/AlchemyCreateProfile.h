// -*- C++ -*-

#ifndef INCLUDED_alchemycreateprofile_AlchemyCreateProfile_h
#define INCLUDED_alchemycreateprofile_AlchemyCreateProfile_h

#include "afwk/Framework.h"
#include "nnet/NNetDataset.h"

namespace alch {

/*!
  \brief Creates an initial prediction profile
  \ingroup alchemycreateprofile
*/
class AlchemyCreateProfile : public Framework
{
 public:

  /*!
    \brief Constructor
  */
  AlchemyCreateProfile();

  virtual ~AlchemyCreateProfile();

protected:

  std::string getApplicationName() const
  {
    return "alchemycreateprofile";
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
  static const char* const s_optionProfile;
  static const char* const s_optionLayers;
  static const char* const s_optionUnits;
  static const char* const s_optionDays;
  static const char* const s_optionName;

  std::string m_trainFile;
  std::string m_profileFile;
  int m_numLayers;
  int m_numUnits;
  int m_numDays;
  std::string m_name;
  NNetDataset m_trainData;

  bool loadParams();
  void printParams();
  bool readData(const std::string& filename,
                NNetDataset& dataset,
                const char* name);
  bool createProfile();
};

} // namespace alch

#endif
