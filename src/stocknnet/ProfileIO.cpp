
#include "stocknnet/ProfileIO.h"

#include "autil/Context.h"
#include "nnet/NeuralNetReader.h"
#include "nnet/NeuralNetWriter.h"
#include "stocknnet/ProfileMetaDataStream.h"

#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"

#include <fstream>

namespace alch {

namespace ProfileIO
{

  bool read(const char* baseName,
            PredictionProfile& profile,
            Context& ctx)
  {
    // read the meta data
    std::string metaDataFileName(getMetaDataFileName(baseName));
    std::ifstream ifs(metaDataFileName.c_str());
    if (!ifs || !ProfileMetaDataStream::read(ifs, profile, ctx))
    {
      ctx << Context::PRIORITY_error
          << "Failed to read profile meta data from '"
          << metaDataFileName << "'"
          << Context::endl;
      return false;
    }

    // read the neural network
    std::string neuralNetFileName(getNeuralNetFileName(baseName));
    NeuralNetReader nNetReader(ctx);
    NeuralNetPtr neuralNet;
    if (!nNetReader.read(neuralNetFileName.c_str(), neuralNet))
    {
      ctx << Context::PRIORITY_error
          << "Failed to read neural network from '"
          << neuralNetFileName << "'"
          << Context::endl;
      return false;
    }
    else
    {
      profile.setNeuralNet(neuralNet);
    }

    return true;
  }


  bool write(const char* baseName,
             const PredictionProfile& profile,
             Context& ctx)
  {
    // write the meta data
    std::string metaDataFileName(getMetaDataFileName(baseName));
    std::ofstream ofs(metaDataFileName.c_str());
    if (!ofs || !ProfileMetaDataStream::write(ofs, profile, ctx))
    {
      ctx << Context::PRIORITY_error
          << "Failed to write profile meta data to '"
          << metaDataFileName << "'"
          << Context::endl;
      return false;
    }

    // write the neural network
    std::string neuralNetFileName(getNeuralNetFileName(baseName));
    NeuralNetWriter nNetWriter(ctx);
    if (!nNetWriter.write(neuralNetFileName.c_str(),
                          profile.getNeuralNetPtr()))
    {
      ctx << Context::PRIORITY_error
          << "Failed to write neural network to '"
          << neuralNetFileName << "'"
          << Context::endl;
      return false;
    }

    return true;
  }


  bool exists(const char* baseName)
  {
    boost::filesystem::path neuralNetPath(
      getNeuralNetFileName(baseName),
      boost::filesystem::native);

    boost::filesystem::path metaDataPath(
      getMetaDataFileName(baseName),
      boost::filesystem::native);

    return (boost::filesystem::exists(neuralNetPath)
            && boost::filesystem::exists(metaDataPath));
  }


  bool remove(const char* baseName)
  {
    boost::filesystem::path neuralNetPath(
      getNeuralNetFileName(baseName),
      boost::filesystem::native);
    boost::filesystem::remove(neuralNetPath);

    boost::filesystem::path metaDataPath(
      getMetaDataFileName(baseName),
      boost::filesystem::native);
    boost::filesystem::remove(metaDataPath);

    return true;
  }


  std::string getNeuralNetFileName(const char* baseName)
  {
    std::string ret(baseName);
    ret += ".nnet";
    return ret;
  }


  std::string getMetaDataFileName(const char* baseName)
  {
    std::string ret(baseName);
    ret += ".meta";
    return ret;
  }


} // namespace ProfileIO


} // namespace alch
