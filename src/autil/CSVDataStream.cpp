
#include "autil/CSVDataStream.h"

#include <string>
#include <sstream>
#include <ctype.h>

namespace alch {

  namespace CSVDataStream
  {

    namespace { // anonymous

      // adds invalid line message
      void invalidLine(const std::string& str, Context& ctx)
      {
        ctx << Context::PRIORITY_error
            << "Invalid line encountered while parsing csv: '"
            << str << "'" << Context::endl;
      }

      void writeRow(std::ostream& os, const CSVData::Row& row)
      {
        CSVData::Row::const_iterator end = row.end(); 
        CSVData::Row::const_iterator iter;
        int i = 0;
        for (iter = row.begin(); iter != end; ++iter)
        {
          if (i++)
          {
            os << ',';
          }

          os << quote(*iter);
        }
        os << '\n';
      }

      // reads single field from stream. returns false if there wasn't anything
      // to read, or true if something was read into field.
      bool readField(std::istream& is, std::string& field, Context& ctx)
      {
        field = "";
        std::stringstream ss;
        char lastCh = '\0';
        char ch = '\0';
        bool inQuote = false;
        for (;;)
        {
          // save the last char we got
          lastCh = ch;

          // ----- NEWLINE OR EOL -----
          // get the next char
          if (!is.get(ch) || (ch == '\n'))
          {
            // inside of quote ==> error
            if (inQuote && (lastCh != '"'))
            {
              ctx << Context::PRIORITY_error
                << "Line ended while inside quote (near '" << field
                << "')" << Context::endl;
              return false;
            }
            else
            {
              break;
            }
          }
          // ----- CARRIAGE RETURN -----
          else if (ch == '\r') // ignore carriage returns
          {
            continue;
          }
          // ----- COMMA -----
          else if (ch == ',')
          {
            // if we're in a quote, then this is a literal comma
            if (inQuote)
            {
              ss << ',';
            }
            // otherwise this signals the end of a field. we don't add it
            else
            {
              break;
            }
          }
          // ----- QUOTE -----
          else if (ch == '"')
          {
            if (inQuote)
            {
              // two quotes in a row ==> add it
              if (lastCh == '"')
              {
                ss << '"';

                // erase this char so we don't double-add quotes
                ch = '\0';
              } 
            }
            else
            {
              if (!field.length())
              {
                // start of field ==> start quoting
                inQuote = true;
              }
              else
              {
                ctx << Context::PRIORITY_error
                  << "Quote started in middle of field (near '" << field
                  << "')" << Context::endl;
                return false;
              }
            }  
          }
          // ----- ANYTHING ELSE -----
          else
          {
            ss << ch;
          }
        }

        field = ss.str();
        return true;
      }
    } // anonymous namespace

    std::string quote(const std::string& str)
    {
      // look for anything that causes us to quote
      if (str.find_first_of("\",\n\r") == std::string::npos)
      {
        return str;
      }

      // quote and double-up existing quotes
      std::stringstream ss;
      ss << '"';
      std::string::size_type len = str.length();
      for (std::string::size_type i = 0; i < len; ++i)
      {
        if (str[i] == '"')
        {
          ss << "\"\"";
        }
        else if (::isspace(str[i])) // translate all whitespace to single
        {
          ss << ' ';
        }
        else
        {
          ss << str[i];
        }
      }

      ss << '"';
      return ss.str();
    }

    std::string unquote(const std::string& str)
    {
      // no need to unquote...
      if (!str.length() || (str[0] != '"'))
      {
        return str;
      }

      std::stringstream ss;
      std::string::size_type len = str.length();

      // iterate over everything except first and last chars
      char lastch = '\0';
      for (std::string::size_type i = 1; i < (len-1); ++i)
      {
        char ch = str[i];

        if (str[i] == '"')
        {
          if (lastch == '"')
          {
            ss << str[i];
          }
          else
          {
            lastch = ch;
          }
        }
        else
        {
          ss << str[i];
          lastch = ch;
        }
      }

      return ss.str();
    }

    bool read(std::istream& is,
              CSVData& data,
              Context& ctx)
    {
      data.clear();

      int lineNum = 1;
      while (is)
      {
        // read one line from the file
        std::string line;
        std::getline(is, line);

        // ignore blank lines
        if (!line.length())
        {
          continue;
        }

        // turn line into stream and parse it one field at a time
        CSVData::Row row;
        std::istringstream iss(line);
        std::string field;
        for (;;)
        {
          if (!iss)
          {
           break;
          }
         
          if (readField(iss, field, ctx))
          {
            row.add(field);
          }
        }

        if (lineNum == 1)
        {
          data.setHeadings(row);
        }
        else
        {
          data.addRow(row);
        }

        ++lineNum;
      }

      return data.validate(ctx);
    }

    bool write(std::ostream& os,
               const CSVData& data,
               Context& ctx)
    {
      if (!data.validate(ctx))
      {
        ctx << Context::PRIORITY_error
            << "CSV data is invalid while trying to write"
            << Context::endl;
        return false;
      }

      writeRow(os, data.getHeadings());
      
      CSVData::const_iterator end = data.end();
      CSVData::const_iterator iter;
      for (iter = data.begin(); iter != end; ++iter)
      {
        writeRow(os, *iter);
      }

      return os;
    }


  } // namespace CSVDataStream

} // namespace alch
