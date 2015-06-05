#include "TestProfileIO.h"

#include "boost/filesystem/operations.hpp"

namespace alch
{

  const char* baseName = "profileiotest";


void TestProfileIO::setUp() 
{
  ;
}

void TestProfileIO::tearDown()
{
  
  boost::filesystem::path neuralNetPath(
    ProfileIO::getNeuralNetFileName(baseName),
    boost::filesystem::native);
  
  boost::filesystem::path metaDataPath(
    ProfileIO::getMetaDataFileName(baseName),
    boost::filesystem::native);
  
  boost::filesystem::remove(neuralNetPath);
  boost::filesystem::remove(metaDataPath);
}

void TestProfileIO::test1()
{

  CPPUNIT_ASSERT_EQUAL(std::string("/tmp/foo.meta"),
                       ProfileIO::getMetaDataFileName("/tmp/foo"));

  CPPUNIT_ASSERT_EQUAL(std::string("/tmp/foo.nnet"),
                       ProfileIO::getNeuralNetFileName("/tmp/foo"));

  CPPUNIT_ASSERT_EQUAL(std::string(baseName) + ".meta",
                       ProfileIO::getMetaDataFileName(baseName));
  
  CPPUNIT_ASSERT_EQUAL(std::string(baseName) + ".nnet",
                       ProfileIO::getNeuralNetFileName(baseName));
  
  boost::filesystem::path neuralNetPath(
    ProfileIO::getNeuralNetFileName(baseName),
    boost::filesystem::native);

  boost::filesystem::path metaDataPath(
    ProfileIO::getMetaDataFileName(baseName),
    boost::filesystem::native);

  boost::filesystem::remove(neuralNetPath);
  boost::filesystem::remove(metaDataPath);

  CPPUNIT_ASSERT(!boost::filesystem::exists(neuralNetPath));
  CPPUNIT_ASSERT(!boost::filesystem::exists(metaDataPath));

  CPPUNIT_ASSERT(!ProfileIO::exists(baseName));

  NeuralNetPtr nnet(new NeuralNet(6, 7));
  PredictionProfile profile;
  profile.setNumberDays(4);
  profile.setName("my name");
  profile.setNeuralNet(nnet);

  CPPUNIT_ASSERT(ProfileIO::write(baseName, profile, m_ctx));
  CPPUNIT_ASSERT(ProfileIO::exists(baseName));

  PredictionProfile profile2;
  CPPUNIT_ASSERT(ProfileIO::read(baseName, profile2, m_ctx));
  CPPUNIT_ASSERT_EQUAL(4, profile2.getNumberDays());
  CPPUNIT_ASSERT_EQUAL(std::string("my name"), profile2.getName());
  CPPUNIT_ASSERT_EQUAL(6, profile2.getNeuralNet().getNumInputUnits());
  CPPUNIT_ASSERT_EQUAL(8, profile2.getNeuralNet().getNumOutputUnits());

  CPPUNIT_ASSERT(ProfileIO::remove(baseName));

  CPPUNIT_ASSERT(!boost::filesystem::exists(neuralNetPath));
  CPPUNIT_ASSERT(!boost::filesystem::exists(metaDataPath));
  CPPUNIT_ASSERT(!ProfileIO::exists(baseName));

}

} // namespace alch
