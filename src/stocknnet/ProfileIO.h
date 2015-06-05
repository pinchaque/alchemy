// -*- C++ -*-

#ifndef INCLUDED_stocknnet_ProfileIO_h
#define INCLUDED_stocknnet_ProfileIO_h

#include "stocknnet/PredictionProfile.h"

#include "autil/Context.h"

namespace alch {

/*!
  \brief Namespace for input and output of prediction profiles
  \ingroup stocknnet
*/
namespace ProfileIO
{

  /*!
    \brief Reads a prediction profile from disk
    \param baseName The base name of the prediction profile
    \param profile [out] The prediction profile to populate
    \param ctx The operation context to use
    \retval true Success
    \retval false Error

    The prediction profile consists of multiple files, all of which share
    the same base filename.
  */
  bool read(const char* baseName,
            PredictionProfile& profile,
            Context& ctx);


  /*!
    \brief Writes a prediction profile to disk
    \param baseName The base name of the prediction profile
    \param profile The prediction profile to populate
    \param ctx The operation context to use
    \retval true Success
    \retval false Error

    The prediction profile consists of multiple files, all of which share
    the same base filename.
  */
  bool write(const char* baseName,
             const PredictionProfile& profile,
             Context& ctx);

  /*!
    \brief Determines whether a prediction profile exists
    \retval true Profile exists
    \retval false Profile does not exist

    Because the prediction profile consists of multiple files, this function
    will only return true if all files exist. It does not open the files 
    to determine if they are readable or properly formatted.
  */
  bool exists(const char* baseName);


  /*!
    \brief Removes a prediction profile
    \retval true Success
    \retval false Error

    Removes all files associated with a prediction profile. It is not an
    error if the profile doesn't exist.
  */
  bool remove(const char* baseName);


  /*!
    \brief Returns the neural network filename for the given base name
    \param baseName The base name for the prediction profile
    \return Neural network filename
  */
  std::string getNeuralNetFileName(const char* baseName);


  /*!
    \brief Returns the meta data filename for the given base name
    \param baseName The base name for the prediction profile
    \return Meta Data filename
  */
  std::string getMetaDataFileName(const char* baseName);

} // namespace ProfileIO

} // namespace alch

#endif
