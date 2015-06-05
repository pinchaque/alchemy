#include "autil/Context.h"

#include <iostream>
#include <iomanip>

namespace alch {

  Context::End Context::endl;


  Context::Context()
    : m_messages()
    , m_priority(PRIORITY_info)
    , m_priorityFilter(PRIORITY_error | PRIORITY_warning | PRIORITY_info)
    , m_flushFrequency(0)
    , m_messageBuffer()
  {
    ;
  }


  Context::~Context()
  {
    flush();
  }


  void Context::clear()
  {
    m_messages.clear();
    m_messageBuffer.clear();
  }


  void Context::flush()
  {
    dump(std::cerr, m_priorityFilter);
    clear();
  }


  void Context::dump(std::ostream& os, int filter)
  {
    MessageVec::const_iterator end = m_messages.end();
    MessageVec::const_iterator iter;
    for (iter = m_messages.begin(); iter != end; ++iter)
    {
      if (filter & iter->priority)
      {
        std::string str;
        formatMessage(str, *iter);
        os << str << std::endl;
      }
    }
  }


  void Context::add(const Message& message)
  {
    m_messages.push_back(message);
    int numMessages(m_messages.size());

    if (m_flushFrequency >= 0)
    {
      if (!m_flushFrequency || (numMessages >= m_flushFrequency))
      {
        flush();
      }
    }
  }


  Context& Context::operator << (Context::Priority priority)
  {
    m_priority = priority;
    return *this;
  }


  Context& Context::operator << (const Context::Message& message)
  {
    add(message);
    return *this;
  }


  Context& Context::operator << (const End& end)
  {
    Message message;
    message.priority = m_priority;
    message.text = m_messageBuffer.str();
    add(message);
    m_messageBuffer.str("");
    return *this;
  }


  std::string Context::priorityToString(Context::Priority priority)
  {
    switch (priority)
    {
      case PRIORITY_error:
        return "Error";

      case PRIORITY_warning:
        return "Warning";

      case PRIORITY_info:
        return "Info";

      case PRIORITY_debug1:
      case PRIORITY_debug2:
      case PRIORITY_debug3:
        return "Debug";

      default:
        return "Unknown";
    }
  }

  void Context::formatMessage(std::string& str, const Message& message)
  {
    boost::posix_time::time_duration duration
      = message.creationTime.time_of_day();

    boost::gregorian::date date = message.creationTime.date();

    std::stringstream ss;
    ss << "["
       << std::setw(2) << std::setfill('0') << date.month()
       << "/"
       << std::setw(2) << std::setfill('0') << date.day()
       << " "
       << std::setw(2) << std::setfill('0') << duration.hours()
       << ":"
       << std::setw(2) << std::setfill('0') << duration.minutes()
       << ":"
       << std::setw(2) << std::setfill('0') << duration.seconds()
       << "] "
       << priorityToString(message.priority)
       << ": "
       << message.text;

    str = ss.str();
  }


} // namespace alch
