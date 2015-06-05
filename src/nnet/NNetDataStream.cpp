
#include "nnet/NNetDataStream.h"

#include <sstream>
#include <string>

namespace alch {

  namespace NNetDataStream
  {

    namespace {

      void parseValues(const std::string& str, std::vector<double>& vec)
      {
        std::istringstream iss(str);
        
        while (iss)
        {
          double d;
          iss >> d;
          if (iss)
          {
            vec.push_back(d);
          }
        }
      }

    } // anonymous namespace


    /*
      Each line in the file consists of input and output fields, separated
      by a pipe symbol.
    */

    bool readDatapoint(std::istream& is,
                       NNetDatapoint& dataPoint,
                       Context& ctx)
    {
      std::string line;
      std::getline(is, line, '\n');
      std::string::size_type pos = line.find('|');

      // skip blank lines
      if (!line.length() || ((line.length() == 1) && (line[0] == '\n')))
      {
        return false;
      }

      if (pos == std::string::npos)
      {
        ctx << Context::PRIORITY_error
            << "Couldn't find pipe symbol in line." << Context::endl;
        return false;
      }

      std::string inputs = line.substr(0, pos);
      std::string outputs = line.substr(pos + 1);

      // parse the inputs
      parseValues(inputs, dataPoint.input);

      // parse the outputs
      parseValues(outputs, dataPoint.output);

      return true;
    }
    
    
    bool writeDatapoint(std::ostream& os,
                        const NNetDatapoint& dataPoint,
                        Context& ctx)
    {
      std::vector<double>::const_iterator end = dataPoint.input.end();
      std::vector<double>::const_iterator iter;
      for (iter = dataPoint.input.begin(); iter != end; ++iter)
      {
        os << *iter << " ";
      }

      os << "|";

      end = dataPoint.output.end();
      for (iter = dataPoint.output.begin(); iter != end; ++iter)
      {
        os << " " << *iter;
      }
      os << "\n";

      return os;
    }



    bool readDataset(std::istream& is,
                     NNetDataset& dataset,
                     Context& ctx)
    {
      // keep track of how many inputs and outputs we expect for each line. -1
      // means we haven't found any yet
      int numInputs = -1;
      int numOutputs = -1;

      int lineNum = 0;
      while (is)
      {
        ++lineNum;

        NNetDatapoint point;
        if (!NNetDataStream::readDatapoint(is, point, ctx))
        {
          if (is)
          {
            ctx << Context::PRIORITY_error
                << "Failed to read datapoint from line " << lineNum
                << ". Ignoring." << Context::endl;
          }
          continue;
        }

        if (numInputs < 0)
        {
          numInputs = point.input.size();
        }
        else if (numInputs != static_cast<int>(point.input.size()))
        {
          ctx << Context::PRIORITY_error
              << "Invalid number of inputs (" << point.input.size()
              << ") in line " << lineNum << "; expected " << numInputs
              << Context::endl;
          return false;
        }

        if (numOutputs < 0)
        {
          numOutputs = point.output.size();
        }
        else if (numOutputs != static_cast<int>(point.output.size()))
        {
          ctx << Context::PRIORITY_error
              << "Invalid number of outputs (" << point.output.size()
              << ") in line " << lineNum << "; expected " << numOutputs
              << Context::endl;
          return false;
        }

        // add this datapoint!
        dataset.push_back(point);
      }

      return true;
    }


    bool writeDataset(std::ostream& os,
                      const NNetDataset& dataset,
                      Context& ctx)
    {
      NNetDataset::const_iterator end = dataset.end();
      NNetDataset::const_iterator iter;
      for (iter = dataset.begin(); iter != end; ++iter)
      {
        if (!writeDatapoint(os, *iter, ctx))
        {
          return false;
        }
      }

      return os;
    }


  } // namespace NNetDataStream

} // namespace alch
