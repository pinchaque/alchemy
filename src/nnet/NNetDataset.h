// -*- C++ -*-

#ifndef INCLUDED_nnet_NNetDataset_h
#define INCLUDED_nnet_NNetDataset_h

#include <vector>
#include <cassert>

#ifndef NDEBUG
#include <iostream>
#include <iomanip>
#endif

namespace alch {

/*!
  \brief Data point for neural network
*/
struct NNetDatapoint
{
 public:
  NNetDatapoint()
    : input()
    , output()
  {
    ;
  }

  //! Inputs
  std::vector<double> input;

  //! Outputs
  std::vector<double> output;

  //! Removes all inputs and outputs
  void clear()
  {
    input.clear();
    output.clear();
  }

#ifndef NDEBUG
  void dump(std::ostream& os) const
  {
    os << "I( ";

    {
      std::vector<double>::const_iterator end = input.end();
      std::vector<double>::const_iterator iter;
      for (iter = input.begin(); iter != end; ++iter)
      {
        os << *iter << " ";
      }
    }

    os << ")  O( ";

    {
      std::vector<double>::const_iterator end = output.end();
      std::vector<double>::const_iterator iter;
      for (iter = output.begin(); iter != end; ++iter)
      {
        os << *iter << " ";
      }
    }

    os << ")\n";
  }
#endif
};


//! Set of datapoints for neural network
struct NNetDataset : public std::vector<NNetDatapoint>
{
#ifndef NDEBUG
  void dump(std::ostream& os) const
  {
    os << "[NNetDataset of " << size() << " points]\n";
    const_iterator final = end();
    const_iterator iter;
    int i = 0;
    for (iter = begin(); iter != final; ++iter, ++i)
    {
      os << "(" << i << ")  ";
      iter->dump(os);
    }
    os << "[End NNetDataset]\n";
  }
#endif
};

} // namespace alch

#endif
