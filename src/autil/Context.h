// -*- C++ -*-

#ifndef INCLUDED_autil_Context_h
#define INCLUDED_autil_Context_h

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

#include <sstream>
#include <string>
#include <vector>
#include <iosfwd>

namespace alch {

/*!
  \brief Operation context class

  Provides interface for application classes to pass messages back to the
  user.
*/
class Context
{
 public:
  //! Priority of the message
  enum Priority
  {
    PRIORITY_none = 0x0000,
    PRIORITY_error = 0x0001,
    PRIORITY_warning = 0x0002,
    PRIORITY_info = 0x0004,
    PRIORITY_debug1 = 0x0008,
    PRIORITY_debug2 = 0x0010,
    PRIORITY_debug3 = 0x0020,
    PRIORITY_all = 0xFFFF
  };
  
  //! Structure for storing message data
  struct Message
  {
    //! Priority of the message
    Context::Priority priority;

    //! Text of the message
    std::string text;

    //! Time at which message was created
    boost::posix_time::ptime creationTime;

    Message()
      : priority(Context::PRIORITY_info)
      , text("")
      , creationTime(boost::posix_time::second_clock::local_time())
    {
      ;
    }
  };

  //! Signifies end of current message
  struct End
  {
    ;
  };

  //! Signifies end of current message
  static End endl;

  //! Vector of messages
  typedef std::vector<Message> MessageVec;

  //! Constructor
  Context();

  //! Destructor
  ~Context();

  //! Returns messages stored in context
  const MessageVec& getMessages() const
  {
    return m_messages;
  }

  //! Gets current message priority
  Priority getPriority() const
  {
    return m_priority;
  }

  //! Sets the message priority
  void setPriority(Priority val)
  {
    m_priority = val;
  }

  //! Returns the current priority filter
  int getPriorityFilter() const
  {
    return m_priorityFilter;
  }

  //! Sets the current priority filter
  void setPriorityFilter(int val)
  {
    m_priorityFilter = val;
  }

  //! Returns the current flush frequency
  int getFlushFrequency() const
  {
    return m_flushFrequency;
  }

  /*!
    \brief Sets the current flush frequency
    \param val the new flush frequency

    If val is < 0, then flushing is never done automtically
    If val is = 0, then flushing is done for every message
    If val is > 0, then flushing is done when there are that many messages
    stored in m_messages.
  */
  void setFlushFrequency(int val)
  {
    m_flushFrequency = val;
  }

  //! Clears messages stored in context
  void clear();

  /*!
    \brief Performs flush operation on Context
    
    Flushes messages from the message buffer, reseting the flush counter.
    Messages are printed should they match the priority filter.
  */
  void flush();

  /*!
    \brief Prints messages to output stream
    \param os The output stream to print to
    \param filter Priority filter to use

    Prints all messages to os that match filter. Does not remove messages.
  */
  void dump(std::ostream& os, int filter = PRIORITY_all);

  //! Adds a message to the context
  void add(const Message& message);


  //! Operator that changes the priority of the forthcoming messages
  Context& operator << (Context::Priority priority);

  //! Adds a message to the context using values stored in the message
  Context& operator << (const Message& message);

  //! Completes the current message in the Context using the stored priority
  Context& operator << (const End& end);

  //! Operator that appends to the current message in the Context 
  template <class T>
  Context& operator << (const T& val)
  {
    m_messageBuffer << val;
    return *this;
  }


 private:

  /*!
    \brief Returns a string representing the specified priority
    \param priority The priority to use
    \return String representation of the priority
  */
  static std::string priorityToString(Context::Priority priority);

  /*!
    \brief Formats the message into a string
    \param str [out] The destination of the formatted message
    \param message The message to format

    Formats a message based on standard formatting conventions.
  */
  static void formatMessage(std::string& str, const Message& message);

  //! Buffered messages
  MessageVec m_messages;

  // Current priority for any added messages
  Priority m_priority;

  //! Filter to use when printing messages
  int m_priorityFilter;

  //! Frequency of buffer flushing
  int m_flushFrequency;

  //! The buffer used to hold the current message as it is constructed
  std::ostringstream m_messageBuffer;
};



} // namespace alch

#endif
