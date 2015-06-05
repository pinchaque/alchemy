#include "alchemytrain/AlchemyTrain.h"

#include "boost/filesystem/operations.hpp"

#include "stocknnet/ProfileIO.h"
#include "nnet/NNetDataStream.h"
#include "nnet/NeuralNetAlg.h"
#include "nnet/GradDescent.h"
#include "nnet/MomentumGradDescent.h"
#include "autil/TempFile.h"

#include <fstream>
#include <sstream>
#include <stdio.h>

namespace alch {

  const char* const AlchemyTrain::s_optionTrain = "train";
  const char* const AlchemyTrain::s_optionTest = "test";
  const char* const AlchemyTrain::s_optionProfile = "profile";
  const char* const AlchemyTrain::s_optionPlot = "plot";
  const char* const AlchemyTrain::s_optionEta = "eta";
  const char* const AlchemyTrain::s_optionAlpha = "alpha";
  const char* const AlchemyTrain::s_optionBeta = "beta";
  const char* const AlchemyTrain::s_optionSteps = "steps";
  const char* const AlchemyTrain::s_optionMomentum = "momentum";
  const char* const AlchemyTrain::s_optionAutoStop = "autostop";

  AlchemyTrain::AlchemyTrain()
    : Framework()
    , m_trainFile("")
    , m_testFile("")
    , m_profileName("")
    , m_useMomentum(false)
    , m_eta(0.001)
    , m_alpha(1.10)
    , m_beta(0.50)
    , m_numSteps(100)
    , m_autoStopSteps(25)
    , m_profile()
    , m_trainData()
    , m_testData()
  {
    ;
  }


  AlchemyTrain::~AlchemyTrain()
  {
    ;
  }

  std::string AlchemyTrain::getApplicationDescription() const
  {
    return
      "Trains a neural network profile based on the given set of training\n"
      "and test data. The profile must already exist; to create a new \n"
      "profile use alchemycreateprofile.\n"
      ;
  }

  bool AlchemyTrain::initialize()
  {
    return Framework::initialize();
  }

  bool AlchemyTrain::finalize()
  {
    return Framework::finalize();
  }

  Framework::OptionsReturnCode AlchemyTrain::processOptions(
    int argc, char** argv)
  {
    getOptions().getGenericOptions().add_options()
      (s_optionTrain,
       boost::program_options::value<std::string>(),
       "Name of input file for training data")
      (s_optionTest,
       boost::program_options::value<std::string>(),
       "Name of input file for testing data")
      (s_optionProfile,
       boost::program_options::value<std::string>(),
       "Name of file for neural network profile")
      (s_optionPlot, "Flag to produce a plot of train/test error")
      (s_optionMomentum, "Specifies to use momentum for training")
      (s_optionSteps,
       boost::program_options::value<int>(),
       "Number of training steps to perform")
      (s_optionAutoStop,
       boost::program_options::value<int>(),
       "Automatically stop if testing error does not decrease after this "
       "number of steps.")
      (s_optionEta,
       boost::program_options::value<double>(),
       "Value of eta for neural network training")
      (s_optionAlpha,
       boost::program_options::value<double>(),
       "Value of alpha (momentum increase multiplier) for neural network "
       "training")
      (s_optionBeta,
       boost::program_options::value<double>(),
       "Value of beta (momentum decrease multiplier) for neural network "
       "training")
      ;

    return Framework::processOptions(argc, argv);
  }


  bool AlchemyTrain::processApplication()
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

    if (!readData(m_testFile, m_testData, "testing data"))
    {
      getContext() << Context::PRIORITY_error
                   << "Application failed while reading testing data"
                   << Context::endl;
      return false;
    }
    else if (!m_testData.size())
    {
      getContext() << Context::PRIORITY_error
                   << "No testing data was read; aborting execution"
                   << Context::endl;
      return false;
    }

    if (!loadProfile())
    {
      getContext() << Context::PRIORITY_error
                   << "Application failed while loading prediction profile"
                   << Context::endl;
      return false;
    }

    if (!trainNeuralNet())
    {
      getContext() << Context::PRIORITY_error
                   << "Application failed while training neural network"
                   << Context::endl;
      return false;
    }

    if (!writeProfile())
    {
      getContext() << Context::PRIORITY_error
                   << "Application failed while writing prediction profile"
                   << Context::endl;
      return false;
    }

    return true;
  }

  bool AlchemyTrain::loadParams()
  {
    FrameworkOptions& options(getOptions());
    FrameworkOptions::VariablesMap& vm = options.getVariablesMap();

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

    // get testing file name
    if (vm.count(s_optionTest))
    {
      m_testFile = vm[s_optionTest].as<std::string>();
    }
    else
    {
      getContext() << Context::PRIORITY_error
                   << "Testing file not specified"
                   << Context::endl;
      return false;
    }

    if (!boost::filesystem::exists(
          boost::filesystem::path(m_testFile, boost::filesystem::native)))
    {
      getContext() << Context::PRIORITY_error
                   << "Testing file '" << m_testFile << "' does not exist"
                   << Context::endl;
      return false;
    }

    // get profile name
    if (vm.count(s_optionProfile))
    {
      m_profileName = vm[s_optionProfile].as<std::string>();
    }
    else
    {
      getContext() << Context::PRIORITY_error
                   << "Profile name not specified"
                   << Context::endl;
      return false;
    }

    // get number of steps
    if (vm.count(s_optionSteps))
    {
      m_numSteps = vm[s_optionSteps].as<int>();
    }

    // get number of steps for automatic stopping
    if (vm.count(s_optionAutoStop))
    {
      m_autoStopSteps = vm[s_optionAutoStop].as<int>();
    }

    // get eta
    if (vm.count(s_optionEta))
    {
      m_eta = vm[s_optionEta].as<double>();
    }

    // get alpha and beta
    if (vm.count(s_optionAlpha))
    {
      m_alpha = vm[s_optionAlpha].as<double>();
    }

    // get beta
    if (vm.count(s_optionBeta))
    {
      m_beta = vm[s_optionBeta].as<double>();
    }

    if (vm.count(s_optionMomentum))
    {
      m_useMomentum = true;
    }

    return true;
  }


  void AlchemyTrain::printParams()
  {
    getContext() << Context::PRIORITY_info
                 << "Training input file: " << m_trainFile
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Testing input file: " << m_testFile
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Profile name: " << m_profileName
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Number of training steps: " << m_numSteps
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "Auto-stop after these steps: " << m_autoStopSteps
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "eta: " << m_eta
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "alpha: " << m_alpha
                 << Context::endl;

    getContext() << Context::PRIORITY_info
                 << "beta: " << m_beta
                 << Context::endl;
  }


  bool AlchemyTrain::readData(const std::string& filename,
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


  bool AlchemyTrain::loadProfile()
  {
    if (!ProfileIO::exists(m_profileName.c_str()))
    {
      getContext() << Context::PRIORITY_error
                   << "Profile name '" << m_profileName
                   << "' does not exist"
                   << Context::endl;
      return false;
    }

    if (!ProfileIO::read(m_profileName.c_str(), m_profile, getContext()))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to read profile from '" << m_profileName
                   << "'"
                   << Context::endl;
      return false;
    }

    getContext() << Context::PRIORITY_info
                 << "Loaded profile from '" << m_profileName << "'"
                 << Context::endl;

    NeuralNetPtr neuralNet(m_profile.getNeuralNetPtr());

    getContext() << Context::PRIORITY_debug1
                 << "Number layers: " << neuralNet->getNumLayers()
                 << Context::endl;
    
    for (int layer = 0; layer <  neuralNet->getNumLayers(); ++layer)
    {
      getContext() << Context::PRIORITY_debug1
                   << "Units on layer " << layer << ": "
                   << neuralNet->getNumUnits(layer)
                   << Context::endl;
    }
    
    getContext() << Context::PRIORITY_debug1
                 << "Number input units: "
                 << neuralNet->getNumInputUnits()
                 << Context::endl;
    
    getContext() << Context::PRIORITY_debug1
                 << "Number output units: "
                 << neuralNet->getNumOutputUnits()
                 << Context::endl;

    return true;
  }


  bool AlchemyTrain::trainNeuralNet()
  {
    NeuralNetPtr neuralNet(m_profile.getNeuralNetPtr());
    assert(neuralNet.get());

    NeuralNetPtr trainNeuralNet(new NeuralNet());
    *trainNeuralNet = *neuralNet;

    GradDescentPtr grad;

    if (m_useMomentum)
    {
      getContext() << Context::PRIORITY_debug1
                   << "Using momentum trainer (alpha = " << m_alpha
                   << " / beta = " << m_beta << ")"
                   << Context::endl;

     grad = GradDescentPtr(
        new MomentumGradDescent(trainNeuralNet, m_eta, m_alpha, m_beta));
    }
    else
    {
      getContext() << Context::PRIORITY_debug1
                   << "Using gradient descent (eta = " << m_eta << ")"
                   << Context::endl;

      grad = GradDescentPtr(
        new GradDescent(trainNeuralNet, m_eta));
    }

    const int reportFreq = 1;
    const int colWidth1 = 8;
    const int colWidth2 = 15;
    const int totalWidth = colWidth1 + colWidth2 * 2 + 5;
    const int prec = 8;
    
    {

      getContext() << Context::PRIORITY_info
                   << std::setfill('-')
                   << std::setw(totalWidth) << ""
                   << Context::endl;

      std::stringstream ss;
      ss << std::left << std::setw(colWidth1) << "Step"
         << std::left << std::setw(colWidth2) << "TrainErr"
         << std::left << std::setw(colWidth2) << "TestErr";

      getContext() << Context::PRIORITY_info << ss.str() << Context::endl;

      getContext() << Context::PRIORITY_info
                   << std::setfill('-') << std::setw(totalWidth) << ""
                   << Context::endl;
    }

    int minTestErrIdx = 0;
    double minTestErr = NeuralNetAlg::calculateError(*trainNeuralNet,
                                                     m_testData);

    std::vector<double> trainErrorVec;
    trainErrorVec.reserve(m_numSteps);

    std::vector<double> testErrorVec;
    testErrorVec.reserve(m_numSteps);

    int stepsSinceImprovement = 0;

    for (int stepIdx = 1; stepIdx <= m_numSteps; ++stepIdx)
    {
      // run this step
      assert(grad.get());
      grad->run(m_trainData);

      ++stepsSinceImprovement;

      // calculate what the new error is for the neural net
      double testError = NeuralNetAlg::calculateError(*trainNeuralNet,
                                                      m_testData);
      double trainError = NeuralNetAlg::calculateError(*trainNeuralNet,
                                                       m_trainData);

      // save these error rates
      trainErrorVec.push_back(trainError);
      testErrorVec.push_back(testError);

      bool doReport = false;

      // if this error is better than our known best...
      if (testError < minTestErr)
      {
        // we got a better error -- report this
        doReport = true;

        // update known best error
        minTestErr = testError;
        minTestErrIdx = stepIdx;
        
        // save the network
        *neuralNet = *trainNeuralNet;

        // note that we improved
        stepsSinceImprovement = 0;
      }
      else if ((m_autoStopSteps >= 0)
               && (stepsSinceImprovement >= m_autoStopSteps))
      {
        getContext() << Context::PRIORITY_info
                     << "Stopping early at step " << stepIdx << " after "
                     << stepsSinceImprovement << " steps without improvement"
                     << Context::endl;
        break;
      }

      // report at least every reportFreq steps
      if (!(stepIdx % reportFreq))
      {
        doReport = true;
      }

      // print progress messages
      if (doReport)
      {
        std::stringstream ss;
        ss << std::left << std::setw(colWidth1) << stepIdx
           << std::left << std::setw(colWidth2)
           << std::setprecision(prec) << trainError
           << std::left << std::setw(colWidth2)
           << std::setprecision(prec) << testError;

        // if this was a new low then we print a pretty little flag
        if (testError == minTestErr)
        {
          ss << " <";
        }

        getContext() << Context::PRIORITY_info << ss.str() << Context::endl;
      }
    }

    {
      std::stringstream ss;
      ss  << "Minimum testing error was at step "
          << minTestErrIdx << ": "
          << std::setprecision(prec) << minTestErr;

      getContext() << Context::PRIORITY_info << ss.str() << Context::endl;
    }

    // plot the train and test error if requested
    if (getOptions().getVariablesMap().count(s_optionPlot)
        && !plotError(trainErrorVec, testErrorVec))
    {
      getContext() << Context::PRIORITY_warning
                   << "Failed to plot train and test error; continuing"
                   << Context::endl;
    }

    return true;
  }


  bool AlchemyTrain::writeProfile()
  {
    if (!ProfileIO::write(m_profileName.c_str(), m_profile, getContext()))
    {
      getContext() << Context::PRIORITY_error
                   << "Failed to write prediction profile to '"
                   << m_profileName << "'"
                   << Context::endl;
      return false;
    }

    return true;
  }


  bool AlchemyTrain::plotError(const std::vector<double>& trainError,
                               const std::vector<double>& testError)
  {
    const char* const filePrefix = "/tmp/alchemytrain";


    // write plot data
    TempFilePtr dataFile(new TempFile(filePrefix, TempFile::DELMODE_never));
    {
      std::ofstream ofs(dataFile->getName().c_str());
      if (!ofs)
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to open temporary file '"
                     << dataFile->getName() << "' for writing "
                     << "error plot data" << Context::endl;
        return false;
      }

      assert(trainError.size() == testError.size());

      for (int i = 0; i < int(trainError.size()); ++i)
      {
        ofs << i << ' '
            << std::setprecision(12) << trainError[i] << ' '
            << std::setprecision(12) << testError[i] << '\n';
      }
      ofs.close();
    }


    // write plot commands
    TempFilePtr cmdFile(new TempFile(filePrefix, TempFile::DELMODE_never));
    {
      std::ofstream ofs(cmdFile->getName().c_str());
      if (!ofs)
      {
        getContext() << Context::PRIORITY_error
                     << "Failed to open temporary file '"
                     << cmdFile->getName() << "' for writing "
                     << "error plot commands" << Context::endl;
        return false;
      }

      ofs << "set title 'Error During Training'\n"
          << "set style data fsteps\n"
          << "set grid\n"
          << "set autoscale x\n"
          << "set autoscale y\n"
          << "plot '" << dataFile->getName()
          << "' using 1:2 title 'Train Error' with lines, \\\n"
          << "'" << dataFile->getName()
          << "' using 1:3 title 'Test Error' with lines\n"
          << "set mouse\n"
          << "pause -1\n";

      ofs.close();
    }


    getContext() << Context::PRIORITY_debug1
                 << "Data file: " << dataFile->getName()
                 << Context::endl;

    getContext() << Context::PRIORITY_debug1
                 << "Command file: " << cmdFile->getName()
                 << Context::endl;

    std::stringstream cmd;
    cmd << "gnuplot " << cmdFile->getName() << " 2>&1";
    FILE* pipe = ::popen(cmd.str().c_str(), "r");
    if (!pipe)
    {
      return false;
    }

    const int bufSize = 8192;
    char buf[bufSize + 1] = "";
    
    while (::fgets(buf, bufSize, pipe))
    {
      // strip off newline
      char* p = strchr(buf, '\n');
      if (p)
      {
        *p = '\0';
      }

      getContext() << Context::PRIORITY_error << buf << Context::endl;
    }

    return (::pclose(pipe) != -1);
  }

} // namespace alch
