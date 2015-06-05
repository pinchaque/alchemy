// -*- C++ -*-

#ifndef INCLUDED_alchemytrain_AlchemyTrain_h
#define INCLUDED_alchemytrain_AlchemyTrain_h

#include "afwk/Framework.h"
#include "stocknnet/PredictionProfile.h"
#include "nnet/NNetDataset.h"

namespace alch {

/*!
  \brief Trains a neural network
  \ingroup alchemytrain
*/
class AlchemyTrain : public Framework
{
 public:

  /*!
    \brief Constructor
  */
  AlchemyTrain();

  virtual ~AlchemyTrain();

protected:

  std::string getApplicationName() const
  {
    return "alchemytrain";
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
  static const char* const s_optionProfile;
  static const char* const s_optionPlot;
  static const char* const s_optionEta;
  static const char* const s_optionAlpha;
  static const char* const s_optionBeta;
  static const char* const s_optionSteps;
  static const char* const s_optionMomentum;
  static const char* const s_optionAutoStop;
 

  std::string m_trainFile;
  std::string m_testFile;
  std::string m_profileName;
  bool m_useMomentum;
  double m_eta;
  double m_alpha;
  double m_beta;
  int m_numSteps;
  int m_autoStopSteps;
  PredictionProfile m_profile;
  NNetDataset m_trainData;
  NNetDataset m_testData;

  bool loadParams();
  void printParams();
  bool readData(const std::string& filename,
                NNetDataset& dataset,
                const char* name);
  bool loadProfile();
  bool trainNeuralNet();
  bool writeProfile();
  bool plotError(const std::vector<double>& trainError,
                 const std::vector<double>& testError);
};

} // namespace alch

#endif
