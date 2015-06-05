#include "alchemycreateprofile/AlchemyCreateProfile.h"

#include "nnet/NeuralNetAlg.h"
#include "stocknnet/ProfileIO.h"
#include "nnet/NNetDataStream.h"

#include "boost/filesystem/operations.hpp"

#include <fstream>

namespace alch {

  const char* const AlchemyCreateProfile::s_optionTrain = "train";
  const char* const AlchemyCreateProfile::s_optionProfile = "profile";
  const char* const AlchemyCreateProfile::s_optionLayers = "layers";
  const char* const AlchemyCreateProfile::s_optionUnits = "units";
  const char* const AlchemyCreateProfile::s_optionDays = "days";
  const char* const AlchemyCreateProfile::s_optionName = "name";

  AlchemyCreateProfile::AlchemyCreateProfile()
    : Framework()
    , m_trainFile("")
    , m_profileFile("")
    , m_numLayers(2)
    , m_numUnits(60)
    , m_numDays(1)
    , m_name("Generic profile")
    , m_trainData()
  {
    ;
  }


  AlchemyCreateProfile::~AlchemyCreateProfile()
  {
    ;
  }

  std::string AlchemyCreateProfile::getApplicationDescription() const
  {
    return
      "Creates an initial prediction profile and writes it to disk.\n";
      ;
  }

  bool AlchemyCreateProfile::initialize()
  {
    return Framework::initialize();
  }

  bool AlchemyCreateProfile::finalize()
  {
    return Framework::finalize();
  }

  Framework::OptionsReturnCode AlchemyCreateProfile::processOptions(
    int argc, char** argv)
  {
    getOptions().getGenericOptions().add_options()
      (s_optionName,
       boost::program_options::value<std::string>(),
       "Name to use to identify this prediction profile.")
      (s_optionProfile,
       boost::program_options::value<std::string>(),
       "Base name of prediction profile. Files will be created with this "
       "base name plus extensions.")
      (s_optionTrain,
       boost::program_options::value<std::string>(),
       "Name of input file for training data")
      (s_optionLayers,
       boost::program_options::value<int>(),
       "Number of hidden layers in neural network")
      (s_optionUnits,
       boost::program_options::value<int>(),
       "Number of units per layer in neural network")
      (s_optionDays,
       boost::program_options::value<int>(),
       "Number of days in advance reprensented by training data")
      ;

    return Framework::processOptions(argc, argv);
  }


  bool AlchemyCreateProfile::processApplication()
  {
    // process parameters passed in by user
    if (!loadParams())
    {
      getContext() << Context::PRIORITY_error
                   << "Application failed while loading parameters"
                   << Context::endl;
      return false;
    }
    printParams();

    if (!readData(m_trainFile, m_trainData, "training data"))
    {
      getContext() << Context::PRIORITY_error
                   << "Application failed while reading training data"
                   << Context::endl;
      return false;
    }
    else if (!m_trainData.size())
    {
      getContext() << Context::PRIORITY_error
                   << "No training data was read; aborting execution"
                   << Context::endl;
      return false;
    }

    if (!createProfile())
    {
      getContext() << Context::PRIORITY_error
                   << "Application failed while creating prediction profile"
                   << Context::endl;
      return false;
    }

    return true;
  }

  bool AlchemyCreateProfile::loadParams()
  {
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();

    if (vm.count(s_optionName))
    {
      m_name = vm[s_optionName].as<std::string>();
    }
    else
    {
      getContext() << Context::PRIORITY_error
                   << "Profile name not specified"
                   << Context::endl;
      return false;
    }

    // get training file name
    if (vm.count(s_optionTrain))
    {
      m_trainFile = vm[s_optionTrain].as<std::string>();
    }
    else
    {
      getContext() << Context::PRIORITY_error
                   << "Training file not specified"
                   << Context::endl;
      return false;
    }

    if (!boost::filesystem::exists(
          boost::filesystem::path(m_trainFile, boost::filesystem::native)))
    {
      getContext() << Context::PRIORITY_error
                   << "Training file '" << m_trainFile << "' does not exist"
                   << Context::endl;
      return false;
    }

    // get profile file name
    if (vm.count(s_optionProfile))
    {
      m_profileFile = vm[s_optionProfile].as<std::string>();
    }
    else
    {
      getContext() << Context::PRIORITY_error
                   << "Profile file not specified"
                   << Context::endl;
      return false;
    }

    // get number of layers
    if (vm.count(s_optionLayers))
    {
      m_numLayers = vm[s_optionLayers].as<int>();
    }

    // get number of units
    if (vm.count(s_optionUnits))
    {
      m_numUnits = vm[s_optionUnits].as<int>();
    }

    // get number of days
    if (vm.count(s_optionDays))
    {
      m_numDays = vm[s_optionDays].as<int>();
    }
    else
    {
      getContext() << Context::PRIORITY_error
                   << "Number of days wasn't specified"
                   << Context::endl;
      return false;
    }

    if (m_numDays <= 0)
    {
      getContext() << Context::PRIORITY_error
                   << "Invalid number of days specified (must be > 0)"
                   << Context::endl;
      return false;
    }

    return true;
  }


  void AlchemyCreateProfile::printParams()
  {
    getContext() << Context::PRIORITY_info
                 << "Training input file: " << m_trainFile
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Profile file: " << m_profileFile
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Hidden layers: " << m_numLayers
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Number of units: " << m_numUnits
                 << Context::endl;
  }


  bool AlchemyCreateProfile::readData(const std::string& filename,
                                      NNetDataset& dataset,
                                      const char* name)
  {
    std::ifstream ifs(filename.c_str());
    if (!ifs)
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to open file '" << filename << "' to read "
                   << name
                   << Context::endl;
      return false;
    }

    if (!NNetDataStream::readDataset(ifs, dataset, getContext()))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed while reading " << name << " from file '"
                   << filename << "'"
                   << Context::endl;
      return false;
    }

    getContext() << Context::PRIORITY_info
                 << "Read " << dataset.size() << " data points from file '"
                 << filename << "'"
                 << Context::endl;

    return true;
  }


  bool AlchemyCreateProfile::createProfile()
  {
    if (ProfileIO::exists(m_profileFile.c_str()))
    {
      getContext() << Context::PRIORITY_info
                   << "Profile '" << m_profileFile
                   << "' already exists; overwriting"
                   << Context::endl;

      ProfileIO::remove(m_profileFile.c_str());
    }
    else
    {
      getContext() << Context::PRIORITY_info
                   << "Profile '" << m_profileFile
                   << "' does not exist; creating"
                   << Context::endl;
    }

    // we use the training dataset to determine the number of input
    // and output units
    assert(m_trainData.size());
    int inputUnits = int(m_trainData[0].input.size());
    int outputUnits = int(m_trainData[0].output.size());
    
    NeuralNetPtr neuralNet(
      new NeuralNet(inputUnits, outputUnits, m_numLayers, m_numUnits));
    
    NeuralNetAlg::randomizeWeights(*neuralNet, -1.00, 1.00);

    PredictionProfile profile;
    profile.setNumberDays(m_numDays);
    profile.setName(m_name);
    profile.setNeuralNet(neuralNet);

    if (!ProfileIO::write(m_profileFile.c_str(), profile, getContext()))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to write prediction profile to base name '"
                   << m_profileFile << "'"
                   << Context::endl;
      return false;
    }

    return true;
  }

} // namespace alch
